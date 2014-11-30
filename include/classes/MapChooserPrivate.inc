<?php
require_once(dirname(__FILE__).'/MapChooser.inc');
/**
 * Die Klasse ist zuständig dafür, dass der User eine FFA-Karte wählen kann, dass
 * diese sich füllt und, wenn voll, gestartet wird.
 */
class MapChooserPrivate extends MapChooser {
	private static $INSTANCES;

	protected $auth_key;
	protected $available_templates;

	/**
	 * @param uint $uid
	 * @return MapChooserPrivate
	 */
	public static function get_instance($uid, $world_id=0) {
		if (@!self::$INSTANCES[$uid][$world_id])
			self::$INSTANCES[$uid][$world_id] = new self($uid, $world_id);

		return self::$INSTANCES[$uid][$world_id];
	}

	/**
	 * @param uint $uid
	 * @param uint $world_id
	 * @return MapChooserPrivate
	 */
	private function __construct($uid, $world_id=0) {
		$this->uid = (int)$uid;
		$this->world_id = (int)$world_id;
		$this->init();
	}

	protected function init() {
		$user = User::get_instance_by_id($this->uid);
		$cur_level = $user->get_cur_level();
		$x = Server::get_instance(CONF_ENGINE_SERVER_URL)
			->get_map_chooser_private_data($this->uid, true, $this->world_id);
		$this->available_maps = Converter::stdClassToArray($x->available_maps);
		$this->available_templates = Converter::stdClassToArray(
			$x->available_templates);
		$this->chosen_map_index = $x->chosen_map_index;
		$this->auth_key = $x->auth_key;
	}

	/**
	 * Versucht den Spieler auf die Karte zu setzen, die sein Auth-Key angibt.
	 *
	 * Je nach Auth-Key bewirbt sich der Spieler als Tag-Team-Partner eines
	 * Spielers. Er wurde also eingeladen. Dieser Tag-Team-Platz ist jedoch
	 * nicht sicher, da sich ggf. mehrere Leute auf den Platz beworben haben.
	 * In dem Fall wird die Exception "self::ERR_MAP_ALREADY_FULL" geworfen.
	 *
 	 * @param string $auth_key	Die Hälfte der Heimatsektoren sind reserviert.
	 * 							Der $auth_key entscheidet, für welchen dieser
	 * 							Sektoren er sich bewirbt (könnte schon belegt
	 * 							sein)
	 */
	public function choose_map_by_auth_key($auth_key, $home=0, $color=0,
		$no_invitation=false)
	{
		$x = Server::get_instance(CONF_ENGINE_SERVER_URL)
			->cmd_map_chooser_private_choose_map_by_auth_key($this->uid,
				$auth_key, $this->world_id, $home, $color, $no_invitation);
		$this->available_maps = Converter::stdClassToArray($x->available_maps);
		$this->available_templates = Converter::stdClassToArray(
			$x->available_templates);
		$this->chosen_map_index = $x->chosen_map_index;
	}

	/**
	 * Setzt den User auf die Map mit der angegebenen ID
	 *
	 * @param int $map_index	Der User wird auf die
	 * 							Karte available_maps[$map_index] gesetzt
	 */
	public function choose_map($map_index) {
		throw new Exception("Das gezielte Auswählen von Karten ist nur über".
			"einen Authkey möglich. Ansonsten muss zufällig gesiedelt werden.");
	}

	/**
	 * Mithilfe dieser Funktion wird der Spieler auf die nächste freie Karte
	 * gesetzt.
	 */
	public function choose_random_map() {
		$x = Server::get_instance(CONF_ENGINE_SERVER_URL)
			->cmd_map_chooser_private_choose_random_map($this->uid,
				$this->world_id);
		$this->available_maps = Converter::stdClassToArray($x->available_maps);
		$this->available_templates = Converter::stdClassToArray(
			$x->available_templates);
		$this->chosen_map_index = $x->chosen_map_index;
		$this->auth_key = $x->auth_key;
	}

	/**
	 * Eröffnet eine neue Insel auf dem niedrigsten Host mit dem angegebenen
	 * Template, damit neue Spieler von außen eingeladen werden können.
	 *
	 * @param uint $template_id
	 */
	public function open_new_mentor_map($template_id, $home=0, $color=0,
		$no_invitations=false)
	{
		$wdata = Worlds::get_world_data_by_level(0, IWorld::MAP_TYPE_FFA);

		$rules = new stdClass();
		$rules->public = false;
		$rules->mentor = true;

		$x = Server::get_instance(CONF_ENGINE_SERVER_URL)
			->cmd_map_chooser_private_open_new_map($this->uid, $wdata->id,
				$template_id, $rules, $no_invitations, $home, $color);
		$this->available_maps = Converter::stdClassToArray($x->available_maps);
		$this->available_templates = Converter::stdClassToArray(
			$x->available_templates);
		$this->chosen_map_index = $x->chosen_map_index;
		$this->auth_key = $x->auth_key;
	}

	/**
	 * Mach die Mapwahl rückgängig. Der Spieler wird somit wieder in den Zustand
	 * zurückgesetzt, als habe er bislang noch keine Karte gewählt.
	 */
	public function unchoose_map() {
		$x = Server::get_instance(CONF_ENGINE_SERVER_URL)
			->cmd_map_chooser_private_unchoose_map($this->uid, $this->world_id);
		$this->available_maps = Converter::stdClassToArray($x->available_maps);
		$this->available_templates = Converter::stdClassToArray(
			$x->available_templates);
		$this->chosen_map_index = $x->chosen_map_index;
		$this->auth_key = $x->auth_key;
	}

	/**
	 * Der Spieler kann hiermit, statt einer laufenden Karte beizutreten, eine
	 * neue eröffnen und mit seinen Regeln konfigurieren.
	 *
	 * <code>$rules</code> bestimmt die Regeln und muss von folgendes Struktur
	 * sein:
	 * <code>
	 * $rules->public bool		Ob die Karte öffentlich zugänglich (TRUE) oder
	 * 							mit auth-key abgeschlossen sein soll (FALSE)
	 * 							[default: FALSE]
	 * $rules->time_factor uint	Geschwindigkeitsfaktor [default: 1]
	 * </code>
	 *
	 * @param uint $template_id
	 * @param stdClass $rules
	 */
	public function open_new_normal_map($template_id, $rules=null, $home=0,
		$color=0)
	{
		$x = Server::get_instance(CONF_ENGINE_SERVER_URL)
			->cmd_map_chooser_private_open_new_map($this->uid, $this->world_id,
				$template_id, $rules, true, $home, $color);
		$this->available_maps = Converter::stdClassToArray($x->available_maps);
		$this->available_templates = Converter::stdClassToArray(
			$x->available_templates);
		$this->chosen_map_index = $x->chosen_map_index;
		$this->auth_key = $x->auth_key;
	}

	/**
	 * Gibt die Daten zurück, die dem entsprechenden Auth-Key zugeordnet sind.
	 *
	 * Die Rückgabe ist ein Standartobjekt folgender Struktor:
	 *
	 * $o->world_id
	 * $o->map_id		ID der Karte, für die der Key gilt
	 * $o->sector_ids	Liste der Sektoren, die in Frage kommen. Ist die Liste
	 * 					leer, sind entweder alle möglichen Sektoren belegt, oder
	 * 					die Reservierungsdauer ist ausgelaufen.
	 * 					Dann kann höchstens versucht werden, ohne Authkey normal
	 * 					der Karte beizutreten (falls sie nicht voll ist).
	 * $o->res_by_uid	ID des Users, der den Platz reserviert hat
	 * $o->min_level	Die Stufe, die der User mindestens braucht, um hier
	 * 					mitspielen zu dürfen
	 * $o->max_level	Die Stude, die der User maximal haben darf, um hier ohne
	 * 					Handicap mitspielen zu dürfen
	 *
	 * @param string $auth_key
	 * @return stdClass[]
	 */
	public static function get_data_by_auth_key($auth_key) {
		$x = Server::get_instance(CONF_ENGINE_SERVER_URL)
					->get_map_chooser_private_data_by_auth_keys(array($auth_key));
		return $x[0];
	}
	public static function get_data_by_auth_keys(array $auth_keys) {
		return Server::get_instance(CONF_ENGINE_SERVER_URL)
					->get_map_chooser_private_data_by_auth_keys($auth_keys);
	}

	/**
	 * Gibt alle wählbaren Templates zurück
	 *
	 * Der Rückgabewert ist ein Array von assotiatives Arraya mit folgenden
	 * Keys:
	 * <code>
	 * $a[]->template_id		Template-ID
	 * $a[]->num_players		Anzahl der Spieler, die auf einer Insel eines
	 * 							solchen Templates spielen können und sollen.
	 * </code>
	 *
	 * @return assoc[]
	 */
	public function get_available_templates() {
		return $this->available_templates;
	}

	/**
	 * Gibt den Auth-Key der Karte zurück, um die es sich beim aktuellen
	 * Wechsel dreht
	 *
	 * @return char[6]
	 */
	public function get_auth_key() {
		return $this->auth_key;
	}

	/**
	 * Gibt den Auth-Key der Karte zurück, auf die der Spieler mit der ID
	 * $uid gerade spielt.
	 *
	 * Wenn der Spieler gerade spielt, ist keine Instanz von MapChoosePrivate
	 * erstellbar, weil er sonst ja Karten wählen könnte. Deswegen ist das
	 * hier ein statischer Hack.  Fraglich, ob das nicht vielleicht dann in
	 * eine andere Klasse gehört...
	 *
	 * @param uint $uid
	 * @return char[6]
	 */
	public static function get_auth_key_for_current_map($uid) {
		return Server::get_instance(CONF_ENGINE_SERVER_URL)
			->get_map_chooser_private_auth_key($uid);
	}
}
?>