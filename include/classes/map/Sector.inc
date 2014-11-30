<?php
require_once (dirname(__FILE__).'/../Resource.inc');
require_once (dirname(__FILE__).'/../../shared/classes/Direction.inc');
require_once (dirname(__FILE__).'/../../shared/classes/datastructures/'.
														'WorldCoordinates.inc');
/**
 * Klasse repräsentiert einen Sektor auf der Karte. Enthalten sind Attribute
 * wie Einheiten, Gebäude, ob sichtbar etc etc...
 */
class Sector {
	private $world_id;
	private $map_id;
	private $sector_id;

	private $host_activity;
	private $max_resources;
	private $home_sector_of;
	private $dominator;
	private $fight;

	private $data_initialized = false;


	/**
	 * Singleton: 1 Instanz pro Sektor-ID
	 */
	static private $INSTANCE = array();

	/**
	 * Diese statische Funktion als Konstruktor benutzen
	 *
	 * @param WorldCoordinates $sector_c
	 * @return Sector	Das gewünschte Sector-Objekt :)
	 */
 	public static function get_instance(WorldCoordinates $sector_c,
 		$force=false)
 	{
 		$world_id = $sector_c->get_world_id();
 		$map_id = $sector_c->get_map_id();
 		$sector_id = $sector_c->get_sector_id();

 		if (!isset(self::$INSTANCE[$world_id][$map_id][$sector_id]) || $force) {
 			self::$INSTANCE[$world_id][$map_id][$sector_id]
 				= new Sector($sector_c, $force);
 		}
 		return self::$INSTANCE[$world_id][$map_id][$sector_id];
 	}

	/**
	 * Konstruktor. Übergebene ID identifiert den Sektor. Er muss der Sektor-
	 * nummer entsprechend, welche auf den Karten benutzt wird (0-99)
	 *
	 * @param int $id Die Sektor-ID - LANGE ID
	 */
	private function __construct(WorldCoordinates $sector_c, $force) {
		$this->world_id = $sector_c->get_world_id();
		$this->map_id = $sector_c->get_map_id();
		$this->sector_id = $sector_c->get_sector_id();
		/**
		 * Um Zugriffe zu sparen, wird der Rest normalerweise on-demand
		 * gefüllt. Das kann mit $force umgangen werden, was jedoch auch
		 * zu einem direkten Abholen der Daten vom Server führt.
		 */
		if ($force) {
			init(true);
		}
	}

	private function init($force = false) {
		if (!$this->data_initialized || $force) {
			$data = Server::get_instance(CONF_ENGINE_SERVER_URL)
				->get_sector_data($this->world_id, $this->map_id,
					$this->sector_id);
			$this->host_activity = $data->host_activity;
			$this->max_resources = $data->max_resources;
			$this->home_sector_of = $data->home_sector_of;
			$this->dominator = $data->dominator;
			$this->fight = $data->fight;
			$this->data_initialized = true;
		}
	}

	public function get_fight() {
		return $this->fight;
	}

	public function get_max_resources($which=false) {
		$this->init();
		if ($which !== false)
			return ceil($this->max_resources[$which]);
		else
			return $this->max_resources;
	}

	/**
	 * Gibt zurück, wie viel Resource der angegebene User in diesem Sektor
	 * abbaut. Ist ein Wrapper der Resource::get_resource_gain-Funktion.
	 *
	 * @param uint $uid
	 * @param uint $which
	 * @param bool $split	(aufgeteilte Resourcen in Gebäude- abh. von Sektor,
	 * 						Gebäude- unabh., und Forschungsgain)
	 * @return ufloat
	 */
	public function get_resource_gain($uid, $which=false, $split=false) {
		$uid_old = $uid;
		$uid = (int)$uid;
		if ($uid <= 0)
			throw new IllegalArgumentException("\$uid has to be a positive ".
				"integer value.", $uid_old);

		if ($which !== false)
			return Resource::get_instance($uid)->get_resource_gain($which,
				$this->sector_id, $split);
		else {
			$result = array();
			for ($i = 0; $i < Resource::NUM_RESOURCES; $i++) {
				$result[$i] =
					Resource::get_instance($uid)->get_resource_gain($i,
						$this->sector_id, $split
					);
			}
			return $result;
		}
	}

	/**
	 * Gibt die Höhe der Wirtsaktivität in ganzaligen Prozent zurück.
	 *
	 * @return [0,1,...100]
	 */
	public function get_host_activity() {
		$this->init();
		return $this->host_activity;
	}

	/**
	 * Gibt zurück, ob dieser Sektor ein Homesector ist.
	 *
	 * @return bool
	 */
	public function is_homesector() {
		$this->init();
		return (bool)($this->home_sector_of);
	}

	/**
	 * Gibt die den User zurück, der diesen Homesector besitzt, oder FALSE,
	 * wenn der Sektor kein Homesector ist.
	 *
	 * @return uint/FALSE
	 */
	public function get_homesector_uid() {
		$this->init();
		return $this->home_sector_of;
	}

	public function get_dominator_id() {
		$this->init();
		return $this->dominator;
	}

	public function get_dominator_nick() {
		$this->init();
		if ($this->get_dominator_id()) {
			$this->dominator_nick =
				User::get_instance_by_id($this->get_dominator_id())->get_nick();
		}
		else
			$this->dominator_nick = '';

		return $this->dominator_nick;
	}

	/**
	 * Ob der gewählte User durch den Sektor hindurch ziehen darf
	 *
	 * @param int $uid	die ID des Users
	 * @return bool		ob er durchziehn darf
	 */
	public function may_move_through($uid) {
		//TODO
		return true;
	}
}

?>