<?

//achtung: die construct und get_free_place fkt-nen scheinen doch recht langsam
//zu laufen, bei 255 gebäuden in der building queue kam auf dem notebook ein
//30sec timeout wegen denen. das könnte durchaus an den datenbankzugriffen liegen
//wg. get_buildings_in_sector oder ähnlichen. kurzer test: auslagern der
//get_buildings_in_sector db zugriffe war es, dauert aber trotz änderung noch viel zu lang!.
//trotzdem unbedingt auf datenbankzugriffe testen!

/*
require_once (dirname(__FILE__).'/../languages.inc');
if (!$language)
	$language = get_language();

include_language_file('Buildings', $language);
*/
require_once (dirname(__FILE__).'/../../shared/classes/Language.inc');
Language::get_instance()->load_language_file('Buildings');
require_once (dirname(__FILE__).'/../Science.inc');
require_once (dirname(__FILE__).'/../User.inc');
require_once (dirname(__FILE__).'/../../shared/classes/buildings/IBuildings.inc');
require_once (dirname(__FILE__).'/../../shared/classes/datastructures/'.
													'JSMouseEventStruct.inc');
require_once (dirname(__FILE__).'/../../shared/classes/datastructures/'.
													'WorldCoordinates.inc');
require_once (dirname(__FILE__).'/../../shared/classes/datastructures/IIndividualization.inc');

require_once (dirname(__FILE__).'/Building.inc');
require_once (dirname(__FILE__).'/BuildingOnMap.inc');
require_once (dirname(__FILE__).'/../Resource.inc');

class Buildings implements IBuildings {
	private $userID;
	private $amount;
	private $area;
	private $buildings;
	private $buildable_buildings = NULL;
	private $treasure_buildings = NULL;

	private static $type_to_constkey_map;

	private static $buildings_in_sector;

	const DEBUG = 0;
	private static $INSTANCE = array();	// instanzüberwachung. Nur eine Instanz pro UID innerhalb von PV
 										// Konstruktor ist private. Objekt statisch über get_instance($uid)
 										// anfordern! (s.u.)

 	/**
	 * Diese Statische Funtion als Konstruktor benutzen !
	 * @return Buildings
	 */
 	public static function get_instance($uid) {
 		if (self::DEBUG) {
			echo "Building: User id@get_instance: $uid<br>\n";
 		}
 		if (!self::$INSTANCE || !self::$INSTANCE[$uid]) {
 			self::$INSTANCE[$uid] = new Buildings($uid);
 		}

 		return self::$INSTANCE[$uid];
 	}

	private function __construct($uid) {
		if (self::DEBUG)
 			echo "Building: constructor called: $uid<br>\n";

		$this->userID = $uid;
		/*if (isset($_SESSION['Buildings'][$uid]['buildable_buildings']))
			$this->buildable_buildings =
				$_SESSION['Buildings'][$uid]['buildable_buildings'];*/
		if (self::DEBUG)
			print_r($_SESSION['Buildings'][$uid]);
	}

	public function __destruct() {
		/*if (self::DEBUG) {
 			echo "Building: destructor called: {$this->userID}<br>\n";
 			print_r($_SESSION['Buildings'][$this->userID]);
		}
		$_SESSION['Buildings'][$this->userID]['buildable_buildings'] =
			$this->buildable_buildings;
		if (self::DEBUG)
			print_r($_SESSION['Buildings'][$this->userID]);*/
	}

	/**
	 * Gibt die User-ID zurück
	 *
	 * @return int user-ID
	 */
	public function get_user_id() {
		return $this->userID;
	}

	/**
	 * Gibt die Gebäude zurück, die ein User bauen kann.
	 *
	 * @param bool $force	Ob erzwungen der Cache umgangen werden soll und
	 * 						die Daten neu vom Server geholt werden sollen.
	 * @return Building[]	Die Liste der Gebäude, die gebaut werden dürfen
	 */
	public function get_buildable_buildings($force = false) {
		if (($this->buildable_buildings !== NULL) && !$force)
			return $this->buildable_buildings;

		$data = Server::get_instance(CONF_ENGINE_SERVER_URL)
			->get_buildable_buildings_data($this->userID);
		$result = array();
		for ($i = 0; $i < count($data); $i++) {
			$result[] = new Building($data[$i]);
		}
		$this->buildable_buildings = $result;
		return $result;
	}

	/**
	 * Gibt alle Gebäude zurück, die im Laufe des Spiels erforschbar sind.
	 *
	 * @param bool $force	Ob erzwungen der Cache umgangen werden soll und
	 * 						die Daten neu vom Server geholt werden sollen.
	 * @return Building[]
	 */
	public static function get_all_non_custom_buildings($force = false) {
		if (!isset($_SESSION['Buildings']['all_normal_buildings_data'])
			|| $force)
		{
			$_SESSION['Buildings']['all_normal_buildings_data'] =
				Server::get_instance(CONF_ENGINE_SERVER_URL)
					->get_all_non_custom_buildings_data(0);
		}
		$n = count($_SESSION['Buildings']['all_normal_buildings_data']);
		for ($i = 0; $i < $n; $i++) {
			$result[] = new Building(
				$_SESSION['Buildings']['all_normal_buildings_data'][$i]
			);
		}
		return $result;
	}

	/**
	 * Gibt ein Array zurück, indem die Gebäudeposition der Gebäude mit den
	 * Typnummern gespeichert ist.
	 *
	 * @param [0,...99] $sector_id
	 * @return BuildingOnMap[]
	 */
	public static function get_buildings_in_sector(WorldCoordinates $sector_c) {
		$world_id = $sector_c->get_world_id();
		$map_id = $sector_c->get_map_id();
		$sector_id = $sector_c->get_sector_id();
		if (!isset(self::$buildings_in_sector[$world_id][$map_id][$sector_id])){
			$data = Server::get_instance(CONF_ENGINE_SERVER_URL)
				->get_buildings_in_sector_data($sector_c);
			self::$buildings_in_sector[$world_id][$map_id][$sector_id] =
				self::convert_buildings_data_to_buildings_on_map($data);
		}
		return self::$buildings_in_sector[$world_id][$map_id][$sector_id];
	}

	/**
	 * Liest die Schatzkistengebäude des Spielers aus und verpackt sie
	 * in json
	 *
	 * @param
	 * @return
	 */
	public function get_treasure_buildings()
	{
		$all_items = Converter::stdClassToArray(
            Server::get_instance(CONF_PAYMENT_SERVER_URL,
                    CONF_MAMMUN_AUTH_SECRET)->get_my_items(
                            $this->userID,
                            IIndividualization::TYPE_BUILDING
                    	)
                    );
                    return $all_items;
	}

	/**
	 * Konvertiert die Daten der Gebäude auf der Karte in ein Array von
	 * BuildingOnMap-Objekten
	 * @param stdClass $data
	 * @return BuildingOnMap[]
	 */
	private static function convert_buildings_data_to_buildings_on_map(array $data) {
		$result = array();
		for ($i = 0; $i < count($data); $i++) {
			$result[] = new BuildingOnMap($data[$i]);
		}
		return $result;
	}

	/**
	 * Neukonstruktion von Gebäuden.
	 *
	 * @param WorldCoordinates $sector_c
	 * @param int $x 		x-Koordinate des zu bauenden Gebäudes
	 * @param int $y 		y-Koordinate des zu bauenden Gebäudes
	 * @param int $b_type 	Typ des zu bauenden Gebäudes
	 * @param uint			$color
	 *
	 * @return uint			Gibt zurück, wie viele Punkte der Spieler bekam
	 *
	 * @throws Exception	Wenn nicht gebaut werden konnte
	 */
	public function construct(WorldCoordinates $sector_c, $x, $y, $b_type,
		$color)
	{
		$world_id = $sector_c->get_world_id();
		$map_id = $sector_c->get_map_id();
		$sector_id = $sector_c->get_sector_id();
		$data = Server::get_instance(CONF_ENGINE_SERVER_URL)
			->cmd_buildings_construct($this->userID, $sector_c, $x, $y, $b_type,
				$color);
		if ($data) {
			self::$buildings_in_sector[$world_id][$map_id][$sector_id] =
				self::convert_buildings_data_to_buildings_on_map(
					$data->buildings_data
				);
			ScoreCache::set_map_score(
				User::get_instance_by_id($this->userID)->get_world_id(),
				User::get_instance_by_id($this->userID)->get_map_id(),
				$this->userID,
				$data->cur_map_score
			);
			ScoreCache::set_league_score(
				0,
				$this->userID,
				$data->cur_league_score
			);
			if (isset($data->klunker))
				return $data->klunker;
			else
				return false;
		}
		else {
			throw new Exception("Server did not respond. Please try again.");
		}
	}

	/**
	 * Baut das Gebäude mit der übergebenen $id aus.
	 *
	 * @param [0,1..99] $sector_id
	 * @param uint $ulc_x
	 * @param uint $ulc_y
	 * @throws Exception   Wenn der User das Gebäude nicht aufrüsten darf/kann
	 */
	public function upgrade($sector_id, $ulc_x, $ulc_y) {
		// TODO
	}

	/**
	 * Konvertiert die Gebäude, die der Server (MapView-Klasse) erzeugt, und
	 * ergänzt diese mit Namen und Beschreibung.
	 *
	 * @param mixed[] $buildings_array
	 * @return mixed[]
	 */
	public static function convert_server_buildings($buildings_array) {
		/**
		 * Erster Versuch: Wir glauben, dass $buildings_array alle Gebäude
		 * in mehreren Sektoren meint, also eine Verschlachtelte Schleife:
		 */
		foreach ($buildings_array as $sector => $buildings) {
			foreach ($buildings as &$building) {
				if (!is_object($building)) {
					/**
					 * Das ist nun fehlgeschlagen. Also keine verschachtelte
					 * Schleife.
					 */
					foreach ($buildings_array as &$building) {
						$building->name
							= @constant('LNG_B_'.$building->const_key);

						$building->desc
							= @constant('LNG_B_'.$building->const_key.'_DESC');
					}
					break 2;	// Verlässt beide Schleifen.
				}

				/**
				 * Ansonsten, wenn's doch geklappt hat, hier weiter.
				 */
				$building->name
					= @constant('LNG_B_'.$building->const_key);

				$building->desc
					= @constant('LNG_B_'.$building->const_key.'_DESC');
			}
		}

		return $buildings_array;
	}

	/**
	 * Gibt den Constkey eines Typs zurück.
	 *
	 * @param uint $type
	 * @param unit $level
	 */
	private static function get_const_key_by_type($type) {
		if ($type <= 0)
			return '';

		$type = (int)$type;
		if (empty(self::$type_to_constkey_map)) {
			if (!isset($_SESSION['Buildings']['type_to_constkey_map'])) {
				$_SESSION['Buildings']['type_to_constkey_map'] =
					Converter::stdClassToArray(
						Server::get_instance(CONF_ENGINE_SERVER_URL)
							->get_buildings_type_to_constkey_map()
					);
			}
			self::$type_to_constkey_map =
				$_SESSION['Buildings']['type_to_constkey_map'];

		}
		return isset(self::$type_to_constkey_map[$type])
			? self::$type_to_constkey_map[$type]
			: '';
	}

	/**
	 * Gibt den Gebäudenamen des Gebäudes mit dem angegebenen Typ zurück.
	 *
	 * @param uint $type
	 * @return string
	 */
	public static function get_b_name($type) {
		return constant('LNG_B_'.self::get_const_key_by_type($type));
	}

	/**
	 * Gibt die Gebäudebeschreibung des Gebäudes mit dem angegeben Typ zurück.
	 *
	 * @param uint $type
	 * @return string
	 */
	public static function get_b_desc($type) {
		return @constant('LNG_B_'.self::get_const_key_by_type($type).'_DESC');
	}

	/**
	 * Zerstört das Gebäude mit der übergebenen $id.
	 *
	 * @param [0,1..99] $sector_id
	 * @param uint $ulc_x
	 * @param uint $ulc_y
	 * @throws Exception Wenn der User das Gebäude nicht abreißen darf/kann
	 */
	public function destroy(WorldCoordinates $sector_c, $ulc_x, $ulc_y) {
		$data = Server::get_instance(CONF_ENGINE_SERVER_URL)
			->cmd_buildings_destroy($this->uid, $sector_c, $ulc_x, $ulc_y);

		if ($data) {
			self::$buildings_in_sector[$sector_c->get_world_id()]
				[$sector_c->get_map_id()][$sector_c->get_map_id()]
					= self::convert_buildings_data_to_buildings_on_map($data);
		}
	}

	/**
	 * Ändert die Farbe des Gebäudes.
	 *
	 * Der Umfärber muss dafür mit Klunkern bezahlen.
	 *
	 * @param [0,1,..99] $sector_id
	 * @param uint $ulc_x
	 * @param uint $ulc_y
	 * @param uint/String $color
	 * @throws Exception
	 * @return uint/FALSE
	 */
	public function change_color(WorldCoordinates $sector_c, $ulc_x, $ulc_y,
		$color)
	{
		$data = Server::get_instance(CONF_ENGINE_SERVER_URL)
			->cmd_buildings_change_color($this->uid, $sector_c, $ulc_x,
				$ulc_y, $color);

		if ($data) {
			self::$buildings_in_sector[$sector_c->get_world_id()]
				[$sector_c->get_map_id()][$sector_c->get_sector_id()]
					= self::convert_buildings_data_to_buildings_on_map(
					$data->buildings_data
				);
			if (isset($data->klunker))
				return $data->klunker;
			else
				return false;
		}
	}
}
?>