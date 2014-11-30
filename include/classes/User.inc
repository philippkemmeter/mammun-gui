<?php
require_once (dirname(__FILE__).'/../shared/classes/IUser.inc');
require_once (dirname(__FILE__).'/../shared/classes/server_com/ClientServer.inc');
require_once (dirname(__FILE__).'/map/MapPlayers.inc');
require_once (dirname(__FILE__).
	'/../shared/classes/exceptions/IllegalArgumentException.inc');
/**
 * Spiegelt einen User mit all seinen Eigenschaften wider. [users-Tabelle]
 */

class User implements IUser {
	private $user_data = array();
	private $data_changed = false;
	private $personal_data = null;
	private $auth_keys = null;

	private static $current_user = null;

	/**
	 * Instanzen werden hier gespeichert zur Instanzüberwachung. Jede Instanz wird
	 * mehrfach gespeichert: einmal pro Nick,damit man eine User-Instanz anhand
	 * des Nicks effizient finden kann, und einmal pro Mode nach ID [also
	 * einmal pro FFA-ID und einmal pro Clan-ID].
	 */
	private static $INSTANCES;

	/**
	 * Gibt die Instanz eines Users-Objektes mit der User-ID $id zurück.
	 * Automatische Instanzüberwachung puffert und spart somit DB-Zugriffe.
	 * Wird $forced gesetzt, wird der Puffer übergangen, und in jedem Fall neu
	 * aus der DB die Daten gelesen. Diese Variante ist mit bedacht zu wählen
	 * und i.d.R. unnötig
	 * Hier kann auch die ID des Host-Users übergeben werden, um diesen virtuellen
	 * User als Objekt zu bekommen.
	 * @param int $id 		ID in der Datenbank des Users
	 * @param bool $forced	ob gezwungen vom Server geladen werden soll
	 * @return User
	 */
	public static function get_instance_by_id($id, $forced=false) {
		$id_old = $id;
		$id = (int)$id;
		if ($id < 0)
			throw new IllegalArgumentException("\$id has to be a ".
				"positive integer value", $id);
		if (!isset(self::$INSTANCES[$id]) || $forced) {
			$new_user = new User($id, $forced);
			self::$INSTANCES[$id] = $new_user;
			self::$INSTANCES[strtolower($new_user->get_nick())] = $new_user;
		}
		return self::$INSTANCES[$id];
	}

	/**
	 * Gibt die Instanz eines Users-Objektes mit dem Nick $nick zurück.
	 * Automatische Instanzüberwachung puffert und spart somit DB-Zugriffe.
	 * Wird $forced gesetzt, wird der Puffer übergangen, und in jedem Fall neu
	 * aus der DB die Daten gelesen. Diese Variante ist mit bedacht zu wählen
	 * und i.d.R. unnötig.
	 * Hierüber kann KEIN virtueller User geholt werden, da die virtuellen
	 * User KEINEN EINDEUTIGEN Nick haben!
	 * @param $nick		Nick in der Datenbank des Users
	 * @param $forced	ob gezwungen aus der DB geladen werden soll
	 * @return User
	 */
	public static function get_instance_by_nick($nick, $forced=false) {
		if (!isset(self::$INSTANCES[strtolower($nick)]) || $forced) {
			$new_user = new User($nick, $forced);
			self::$INSTANCES[$new_user->get_id()] = $new_user;
			self::$INSTANCES[strtolower($new_user->get_nick())] = $new_user;
		}
		return self::$INSTANCES[strtolower($nick)];
	}

	/**
	 * Setzt den aktuellen User, also den, der gerade alle Dateien des Spiels
	 * benutzt, durch die übergebene ID
	 * @param int 	User-ID
	 * @throws Exception	wenn User nicht exisitiert
	 * @return User		den durch die ID definierten, neu gesetzten User
	 */
	public static function set_current_user_by_id($id) {
		self::$current_user = self::get_instance_by_id($id);
		return self::$current_user;
	}

	/**
	 * Setzt den aktuellen User, also den, der gerade alle Dateien des Spiels
	 * benutzt, durch den übergebenen Nickname
	 * @param string	Nick des Users
	 * @throws Exception	wenn User nicht exisitiert
	 * @return User		den durch den Nick definierten, neu gesetzten User
	 */
	public static function set_current_user_by_nick($nick) {
		self::$current_user = self::get_instance_by_nick($nick);
		return self::$current_user;
	}

	/**
	 * Gibt den aktuellen User zurück, also den, der gerade alle Dateien des
	 * Spiels benutzt.
	 * Wenn $forced angegeben wird, dann wird er erneut vom Server erfragt und
	 * der lokale Cache wird umgangen.
	 *
	 * @param bool $forced
	 * @return User
	 */
	public static function get_current_user($forced=false) {
		if (!self::$current_user || $forced) {
			if (isset($_SESSION['User']['current_user_id']))
				self::$current_user = self::get_instance_by_id(
					$_SESSION['User']['current_user_id'], $forced
				);
			else {
				self::$current_user = self::get_instance_by_id(0,$forced);
				$_SESSION['User']['current_user_id']
					= self::$current_user->get_id();
			}
		}

		return self::$current_user;
	}

	/**
	 * Erstellt ein neues User-Objekt. Zur eindeutigen Identifizierung des Users
	 * kann als einziger Parameter entweder der Nick oder die ID des Users
	 * übergeben werden.
	 * @param int/string/array $data	ID oder Nick des Users, bzw User-Daten
	 * @throws Exception	wenn User nicht exisitiert
	 */
	private function __construct($data, $forced=false) {
		if (!is_array($data) && !is_object($data)
			&& !$forced && isset($_SESSION['User'][$data])
			&& is_array($_SESSION['User'][$data])
			&& isset($_SESSION['User'][$data]['user_data'])
			&& false)
		{
			$this->user_data = $_SESSION['User'][$data]['user_data'];
		}
		else {
			if (is_array($data) || is_object($data)) {
				/**
				 * Achtung! Das ist ne gefährliche Kiste. Es wird nicht geprüft,
				 * ob Daten korrekt oder vollständig. Bevor Konstruktor also
				 * mit nem Daten-Array aufgerufen wird, MUSS sichergestellt
				 * werden, dass die Daten auch aus einem vorherigen
				 * "SELECT * FROM users" * hervorgingen
				 * (s. self::get_users_waiting_on_map).
				 */
				$data = (array)$data;
				$this->user_data = (array)$data['user_data'];
			}
			else{
				$x = Server::get_instance(CONF_ENGINE_SERVER_URL)
					->get_user_data($data);
				if (!is_object($x))
					throw new Exception(
						$data." ".var_export($x,1),
						IUser::ERR_USER_DOES_NOT_EXIST
					);
				$this->user_data = (array)$x->user_data;
			}
		}
		$this_data = $this->to_php_assoc_array();
		$_SESSION['User'][$this->get_id()] = $this_data;
		$_SESSION['User'][$this->get_nick()] = $this_data;
	}


	function save() {
		if ($this->user_data) {
			$this_data = array(
				'user_data'		=>	$this->user_data
			);
			$_SESSION['User'][$this->get_id()] = $this_data;
			$_SESSION['User'][$this->get_nick()] = $this_data;
			$_SESSION['User']['current_user_id']
				= self::get_current_user()->get_id();
		}
	}

	/**
	 * Die meisten Getter werden hier dynamisch erstellt. Es ist ein Abbild der
	 * Datenbank
	 * Beispiel: User::get_instance($id)->get_nick();
	 * Ebenso die Setter.
	 * Die Daten werden direkt im Objekt, aber erst beim Destruieren des
	 * Objektes auch in die DB und Session geschrieben.
	 *
	 * Gewisse Attribute dürfen nicht geändert werden (z.B. id, nick o.ä.).
	 * Dafür gibt es dann auch keine Setter.
	 */

	public function __call($fn_name, array $params) {
		if (strpos($fn_name, 'get_') === 0) {	// "->get_[...]"
			$what_to_get = substr($fn_name, 4);
			if (!isset($this->user_data[$what_to_get])) {
				error_log(print_r(debug_backtrace(), true));
				error_log(print_r($this->user_data, true));
				error_log($what_to_get);
				trigger_error(sprintf('Call to undefined function: %s::%s().', get_class($this), $fn_name), E_USER_ERROR);
			}
			return $this->user_data[$what_to_get];
		}
		else {
			trigger_error(sprintf('Call to undefined function: %s::%s().', get_class($this), $fn_name), E_USER_ERROR);
		}
	}

	/**
	 * Gibt ein Array mit Objekten zurück, die die Authentifizierungscodes und
	 * zusätzliche Informationen beinhaltet.
	 *
	 * Rückgabeobjekte habe folgende Form:
	 * <code>
	 * $o->auth_key		Authentifizierungsschlüssel
	 * $o->received_by	UID des Users, der ihm den Schlüssel geschickt hat
	 * <code>
	 *
	 * @param bool $forced		Erzwungener Serverrequest
	 * @return stdClass[]
	 */
	public function get_auth_keys_with_data($forced=false) {
		/**
		 * Session-Cache?
		 */
		if (!$this->auth_keys || $forced)
			$this->auth_keys = Server::get_instance(CONF_ENGINE_SERVER_URL)
				->get_user_auth_keys($this->get_id());
		return $this->auth_keys;
	}

	/**
	 * Fügt dem Spielen den angegebenen Authentifizierungsschlüssel zu seiner
	 * Sammlung hinzu, damit die Einladung dauerhaft vermerkt bleibt.
	 *
	 * Mit <code>$fire_invitation_event</code> kann bestimmt werden, dass das
	 * als Einladung behandelt wird und das entsprechende Event gefeuert wird.
	 *
	 * @param char[6] $auth_key
	 * @param uint $received_by
	 * @param bool $fire_invitation_event=false
	 */
	public function add_auth_key($auth_key, $received_by,
		$fire_invitation_event=false)
	{
		$this->auth_keys = Server::get_instance(CONF_ENGINE_SERVER_URL)
			->add_user_auth_key($this->get_id(), $auth_key, $received_by,
				$fire_invitation_event);
	}

	/**
	 * Gibt die ID der Karte zurück, auf die der Spieler sich gerade befindet.
	 *
	 * Spielt er gerade auf keiner Karte, wird die ID der Karte zurückgegeben,
	 * auf der er zuletzt war.
	 *
	 * Explizit definierter Getter, da die map_id nicht in der Tabelle existiert,
	 * sondern in das homesector-Feld mit einkodiert ist.
	 * @return int
	 */
	public function get_map_id() {
		if ($this->get_state() == self::STATE_PLAYING)
			return floor($this->get_homesector()/100) % 100;
		else
			return $this->get_last_map_id();
	}

	/**
	 * Gibt die ID der Welt, auf der der Spieler sich gerade befindet.
	 *
	 * Spielt er gerade in keiner Welt, wird die ID der Welt zurückgegeben,
	 * auf der er zuletzt war.
	 *
	 * Explizit definierter Getter, da die world_id nicht in der Tabelle existiert,
	 * sondern in das homesector-Feld mit einkodiert ist.
	 * @return int
	 */
	public function get_world_id() {
		if ($this->get_state() == self::STATE_PLAYING)
			return floor($this->get_homesector()/10000);
		else
			return $this->get_last_world_id();
	}

	/**
	 * Gibt die ID der Karte zurück, auf die der Spieler gerade wartet.
	 * Explizit definierter Getter, da die waiting_map_id nicht in der Tabelle
	 * existiert,
	 * sondern in das waiting_homesector-Feld mit einkodiert ist.
	 * @return int
	 */
	public function get_waiting_map_id() {
		return floor($this->get_waiting_homesector()/100);
	}

	/**
	 * Gibt alle privaten Daten des Users zurück. Wird $forced=true gesetzt,
	 * wird der lokale Cache übergangen und erneut beim Server angefragt.
	 *
	 * @return stdClass
	 */
	public function get_personal_data($forced=false) {
		if (!$this->personal_data || $forced) {
			$this->personal_data = Server::get_instance(CONF_ENGINE_SERVER_URL)
				->get_user_personal($this->get_id());
		}
		return $this->personal_data;
	}

	/**
	 * Gibt die Farbe zurück, die der Spieler gerade auf der Karte hat, auf der
	 * er gerade spielt. Wenn er sich auf keiner Karte befindet, wird ein
	 * Leerstring zurückgegeben.
	 *
	 * @return string
	 */
	public function get_color() {
		if (($this->get_state() != self::STATE_PLAYING)
			&& ($this->get_state() != self::STATE_MAP_IS_STARTING))
		{
			return 'neutral';
		}

		return MapPlayers::get_instance(
			$this->get_world_id(),
			$this->get_map_id()
		)->get_color($this->get_id());
	}

	/**
	 * Gibt einen HTML-String zurück, der den Nick schön darstellt.
	 *
	 * @return String
	 */
	public function get_nick_beautified() {
		return	/*'<b>'.$this->get_nick_main().'</b>'.
				'<span style="font-size: 0.8em; opacity: 0.5">'
					.$this->get_nick_suffix().'</span>';*/
				self::beautify_nick($this->get_nick());
	}

	public function get_nick_main() {
		return $this->get_main_part_of_nick($this->get_nick());
	}

	public static function beautify_nick($nick_with_suffix) {
		$x = explode(".", $nick_with_suffix);
		$result = '<span class="username">'.ucfirst($x[0]);
		/*if ($x[1]) {
			$result .= '<span class="usersuffix">.'. $x[1].'</span>';
			$result .= '<span class="usersuffix star">&nbsp;</span>';
		}
		else {
			$result .= '<span class="usersuffix star">✮</span>';
		}*/
		return $result . '</span>';
	}

	public static function get_main_part_of_nick($nick_with_suffix) {
		$x = explode(".", $nick_with_suffix);
		return ucfirst($x[0]);
	}

	/**
	 * Setzt das angegebene Tutorial als gelesen.
	 *
	 * @param $flag
	 */
	public function add_tutorial($flag) {
		$tutorial = Server::get_instance(CONF_ENGINE_SERVER_URL)->
			cmd_tutorial_text_read($flag);
		$this->user_data['tutorial'] = $tutorial;
		$this->save();
	}

	/**
	 * Gibt zurück, ob der User das zu $flag gehörende Tutorial bereits
	 * bestätigt hat.
	 *
	 * @param $flag
	 * @return bool
	 */
	public function has_tutorial($flag) {
		return ($this->get_tutorial() == ($this->get_tutorial() | $flag));
	}

	public function get_cur_score($forced=false) {
		require_once (dirname(__FILE__).'/ScoreCache.inc');
		return ScoreCache::get_league_score(0, $this->get_id(), $forced);
	}

	public function get_cur_level($forced=false) {
		require_once(dirname(__FILE__).
						'/../shared/classes/score/GameScoreCalculation.inc');
		return GameScoreCalculation::calc_level_by_score($this->get_cur_score(
			$forced)
		);
	}

	/**
	 * Gibt das Objekt zu einer stdClass geschrumpft zurück.
	 *
	 * @return stdClass
	 */
	public function to_php_std_class() {
		$x = new stdClass();
		$x->user_data = $this->user_data;
		return $x;
	}

	/**
	 * Gibt das Objekt als assotiatives Array zurück.
	 *
	 * @return array
	 */
	public function to_php_assoc_array() {
		return array (
			'user_data'	=>	$this->user_data,
		);
	}
}
?>