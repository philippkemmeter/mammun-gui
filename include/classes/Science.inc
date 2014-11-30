<?
require_once(dirname(__FILE__).'/Techtree.inc');
/**
 * Verwaltet alle Forschungen.
 */

/**
 * Hilfklasse ScienceProject repräsentiert ein Forschungsprojekt, das gerade
 * läuft oder sich im Pausezustand befindet.
 *
 * Alle Funktionen, die mit einem "_" beginnen, sind private außer für die
 * Friend-Class Science. Friends werden von PHP nicht unterstützt, daher müssen
 * die Funktionen public markiert werden.
 *
 * @author Philipp Kemmeter
 */
class ScienceProject {
	private $uid;
	private $cur_level;
	private $finish_time;
	private $last_stamp;
	private $sc_id;
	private $paused;
	private $progress;			// fortschritt als Verhältnis forschzeit/gesamtzeit
	private $sc_count;			// wie viele Wissenschaftler zugeteilt sind
	private $grow;				// wie hoch das Wachstum der Forschung ansich ist
	private $need_write_back;	// ob es nötig ist, die DB zu aktualisieren

	const PROJECT_PAUSED_FINISH_TIME = 3000000000;
	const E_UNSET_THIS = 1;	// errorcode, wenn construct fehlschlug

	private static $INSTANCES;	// singleton: eine Instanz pro uid-sc_id-Paar

	/**
	 * Konstruktor ist private. Statische Funktion _get_instance() benutzen.
	 *
	 * @param stdClass $data_obj	Die Daten der Forschung
	 */
	public function __construct(stdClass $data_obj) {
		$this->init_from_data($data_obj);
	}

	private function init_from_data(stdClass $data_obj) {
		$this->uid = $data_obj->uid;
		$this->sc_id = $data_obj->sc_id;
		$this->cur_level = $data_obj->cur_level;
		$this->grow = $data_obj->grow;
		$this->progress = $data_obj->progress;
		$this->finish_time = $data_obj->finish_time;
		$this->last_stamp = $data_obj->last_stamp;
		$this->sc_count = $data_obj->sc_count;
		$this->paused  = $data_obj->paused;
	}

	/**
	 * Holt die Daten erneut vom Server.
	 */
	public function update() {
		$data = Server::get_instance(CONF_ENGINE_SERVER_URL)->
			get_science_project_data($this->uid, $this->sc_id);
		if (!$data)
			return false;

		$this->init_from_data($data);
	}

	//// GETTER ////
	/**
	 * Gibt zurück, wie weit das Projekt gerade geforscht ist, wobei 1 fertig und
	 * 0-1 entsprechend das Verhältnis angibt (prozent).
	 * @return float aktueller Forschungsfortschritt
	 */
	public function get_progress() {
		if ($this->paused)	// Dann ist der Progress aktuell in der DB (und somit im Objekt)
			return $this->progress;
		return $this->progress + (Time::get_t_now() -  $this->last_stamp)
			/ $this->get_time_needed();
	}

	/**
	 * Gibt zurück, wie viele Scientists am Projekt derzeit arbeiten
	 * @return int Scientists, die gerade am Projekt arbeiten
	 */
	public function get_sc_count() {
		return $this->sc_count;
	}

	/**
	 * Gibt die ID der Science zurück, die gerade durch dieses Projekt erforscht
	 * wird.
	 * @return int die sc_id
	 */
	public function get_sc_id() {
		return $this->sc_id;
	}

	/**
	 * Gibt den Zeitpunkt zurück, wann die Forschung abgeschlossen sein wird.
	 * @return t_stamp Forschungsendzeitpunkt
	 */
	public function get_finish_time() {
		return $this->finish_time;
	}

	/**
	 * Gibt die Zeit zurück, die die Forschung bei aktuell zugewiesenen Forschern
	 * bis zum entgültigen Erforschen andauern würde (konjunktiv, da ja noch in
	 * der Zukunft permanent Forscher hin und her gezogen werden können UND da
	 * dies in der Vergangenheit ggf. bereits getan wurde, was NICHT berücksichtigt
	 * wird!)
	 *
	 * @return int Aktuelle Insgesamtdauer bei gleichbleibender aktueller Forscherzahl
	 */
	public function get_time_needed() {
		return Science::get_instance($this->uid)
			->science_time_needed($this->grow, $this->sc_count);
	}

	/**
	 * Gibt den Namen der Forschung zurück
	 * @return string Name der Forschung
	 */
	public function get_name() {
		return ScienceConstants::$SC_NAME[$this->sc_id];
	}

	/**
	 * Gibt das Level der Projektes, das gerade angestrebt wird. Hat ein User
	 * eine Forschung Level 1 und erforscht jetzt Level 2, dann wird 2 zurück-
	 * gegeben.
	 *
	 * @return unsigned int	aktuelles Level
	 */
	public function get_level() {
		return $this->cur_level+1;
	}
}


define ('MAGIC_SCIENCE_NUMBER', 		395042400);//131680800; //das ist 3600*10000*7.3156 zum normieren
define ('ITEM_SCIENCE_ID',				10000);


/**
 * Die eigentliche Science-Klasse. Verwaltet alle Forschungen eines Users.
 *
 * @author Philipp Kemmeter
 */
class Science {
	private $uid;
	private /*ScienceProject[]*/ $science_projects;	// Alle Forschungsprojekte des Users
													// assoziativ als 'sc_id' => $project
	private /*int*/ $max_projects_parallel;	// Wie viele Projekte der User glz. forschen darf
	private $science_pow;					// Die aktuelle Fähigkeit der Wissenschaftler des akt. Users

	private static $INSTANCES;				// singleton: eine Instanz pro User

	const ERR_TOO_MANY_PROJ_PARALLEL = 1;
	const ERR_NO_PROJ_POSSIBLE_AT_ALL = 2;
	const ERR_ALL_SCIENTISTS_AT_WORK = 3;


	/**
	 * @return Science
	 */
	public static function get_instance($uid) {
		if (!isset(self::$INSTANCES[$uid]))
			self::$INSTANCES[$uid] = new Science($uid);
		return self::$INSTANCES[$uid];
	}

	private function __construct($uid) {
		$this->uid = $uid;
		$this->science_projects = array();
		$world_id = User::get_instance_by_id($uid)->get_world_id();
		$map_id = User::get_instance_by_id($uid)->get_map_id();
		$data = Server::get_instance(CONF_ENGINE_SERVER_URL)->multi_call(
			array (
				"get_science_data" 	=> $uid,
				"get_map_score"		=> array($world_id,$map_id,$uid)
			)
		);
		ScoreCache::set_map_score(
			$world_id, $map_id, $uid, $data[1]
		);
		$this->init($data[0]);
	}

	private function init(stdClass $data) {
		$this->max_projects_parallel = $data->max_projects_parallel;
		$this->science_projects = array();
		foreach ($data->science_projects_data as $sc_id => $proj_data) {
			$this->science_projects[$sc_id] = new ScienceProject($proj_data);
		}
		$this->science_pow = $data->science_pow;
	}

	/**
	 * Startet die Forschung der gewählten $sc_id oder nimmt die Forschung wieder
	 * auf, wenn pausiert. Sind bereits Forscher dem Projekt zugeteilt, wird die
	 * aktuelle Anzahl auf die übergebene korrigiert.
	 *
	 * @param int $sc_id			ID der zu forschenden Forschung
	 * @param int $scientist_count	Anzahl der Forscher, die zugeteilt werden sollen
	 * @throws HackattackException	Wenn unerlaubte aktionen ausgeführt werden
	 * @throws Exception			Bei allen anderen Fehlern (TC-level zu klein etc)
	 */
	public function start_research($sc_id, $scientist_amount) {
		$data = Server::get_instance(CONF_ENGINE_SERVER_URL)
			->cmd_science_start_research($this->uid, $sc_id, $scientist_amount);

		$this->init($data);
	}

	/**
	 * Stoppt das Forschungsprojekt mit der übergebenen sc_id
	 *
	 * @param int $sc_id	Welche Forschung. Wenn -1 (default), dann ALLE
	 * @throws ExceptionWithErrormail	bei Designfehlern :)
	 *
	 */
	public function stop_research($sc_id=-1) {
		$data = Server::get_instance(CONF_ENGINE_SERVER_URL)
			->cmd_science_stop_research($this->uid, $sc_id);

		$this->init($data);
	}

	public function set_scientists_of_project($sc_id, $amount) {
		$this->start_research($sc_id, $amount);
	}

	/// GETTER
	public function get_science_projects() {
		return $this->science_projects;
	}
	public function get_science_pow() {
		return $this->science_pow;
	}

	public function get_max_projects_parallel() {
		return $this->max_projects_parallel;
	}

	/**
	 * Gibt zurück, wie viele Forscher insgesamt gerade arbeiten.
	 * @return int Arbeitende-Forscher-Anzahl
	 */
	public function get_scientists_at_work() {
		$this->science_projects = $this->get_science_projects(); // ggf noch nicht initialisiert
		if (!$this->science_projects)
			return 0;
		$result = 0;
		foreach ($this->science_projects as $project) {
			$result += $project->get_sc_count();
		}
		return $result;
	}

	/**
	 * @return Techtree
	 */
	public function get_techtree() {
		return Techtree::get_instance($this->uid);
	}

	/**
	 * Berechnet anhand von der Science-Power, wie lange ein Forschungsprojekt
	 * benötigt, wenn es einen gewissen Wachstumswert aufweist und bestimmt viele
	 * Forscher zugeteilt sind.
	 * @param int $grow		Wachstumswert - spiegelt die Forschungsgeschwindigkeit
	 * 						abstrakt wider
	 * @param int $count	Wieviele Forscher der Forschung zugeteilt sind
	 */
	public function science_time_needed($grow, $count) {
		if ($count == 0)
			return 0;
		return (MAGIC_SCIENCE_NUMBER * $grow /
					($count * $this->get_science_pow()
						* (-atan($count / 3000 - 5.5) + 6)
					)
				);
	}
}
?>
