<?php
require_once (dirname(__FILE__).'/../../shared/classes/buildings/AbstractBuilding.inc');
require_once (dirname(__FILE__).'/../../shared/classes/datastructures/Point2D.inc');
require_once (dirname(__FILE__).'/../../shared/classes/helper/IConvertableJSONArray.inc');
require_once (dirname(__FILE__).'/../../shared/classes/helper/IConvertableJSArray.inc');
require_once (dirname(__FILE__).'/../../shared/classes/helper/IConvertablePHPArray.inc');
require_once (dirname(__FILE__).'/../../shared/classes/helper/Converter.inc');
require_once (dirname(__FILE__).'/../../shared/classes/exceptions/ExceptionWithErrormail.inc');

/**
 * Repräsentiert ein Gebäude, das auf der Karte platziert wurde.
 */

class BuildingOnMap extends AbstractBuilding
	implements IConvertableJSONArray, IConvertableJSArray,IConvertablePHPArray
{
	protected $max_sp = 0;		// Wieviel SP es maximal haben kann
	protected $uid = 0;			// Wem das Gebäude gehört
	protected $world_id = 0;
	protected $map_id = 0;
	protected $sector_id = 0;	// In welchem Sektor es platziert wurde
	protected $ulc = NULL;		// Wo es sich im Sektor befinden (UpperLeftCorner)

	protected $t_stamp = 0;		// Wann Gebäude(aus)bau gestartet
	protected $progress = 0;	// Wie weit es bereits ist
	protected $finish_time = 0;	// Wann ein Upgrade fertig ist
	protected $upgrading = 0;
	protected $xp_level = 0;
	protected $color = 0;

	public function __construct(stdClass $data) {
		$this->init($data);
	}

	protected function init($data) {
		$this->sp = (int)$data->sp;
		$this->att = (int)$data->att;
		$this->def = (int)$data->def;
		$this->const_key = $data->const_key;
		$this->size = (int)$data->size;
		$this->height = (float)$data->height;
		$this->level = (int)$data->level;
		$this->type = (int)$data->type;
		$this->pic = $data->pic;
		$this->grow = (float)$data->grow;
		$this->cost = (int)$data->cost;
		$this->upgrade_cost = (int)$data->upgrade_cost;
		$this->req_sc_id = (int)$data->req_sc_id;
		$this->req_level = (int)$data->req_level;
		$this->extra = $data->extra;
		$this->custom = (bool)$data->custom;

		$this->name = @constant('LNG_B_'.$data->const_key);
		$this->desc = @constant('LNG_B_'.$data->const_key.'_DESC');
		$this->ulc = $data->ulc;
		$this->uid = $data->uid;
		$this->world_id = $data->world_id;
		$this->map_id = $data->map_id;
		$this->sector_id = $data->sector_id;
		$this->max_sp = $data->max_sp;
		$this->t_stamp = $data->t_stamp;
		$this->progress = $data->progress;
		$this->finish_time = $data->finish_time;
		$this->upgrading = (bool)$data->is_upgrading;
		$this->color = $data->color;
		$this->xp_level = $data->xp_level;
		$this->const_key = $data->const_key;
	}

	/**
	 * Gibt zurück, ob der User dieses Gebäude auch upgraden darf
	 *
	 * @param int $uid	Die ID des Users, der den Pilz upgraden will
	 * @return bool		ob er das darf
	 */
	public function upgradable_by_user($uid) {
		// 1. gehört das Gebäude dem User?
		if ($uid != $this->uid)
			return false;

		// 2. Ist gerade bereits ein upgrade im Gange ?
		if ($this->is_upgrading())
			return false;

		// 3. Gibt es überhaupt ein nächstes Level ?
		if ($this->upgrade_cost <= 0)
			return false;

		return true;
	}

	/**
	 * Gibt zurück, ob der User diesen Pilz abreißen darf
	 *
	 * @param int $uid	Die ID des Users, der den Pilz abreißen will
	 * @return bool		ob er das darf
	 */
	public function destroyable_by_user($uid) {
		// 1. gehört der Pilz dem User?
		if ($uid != $this->uid)
			return false;

		// 2. Ist gerade ein Upgrade im Gange ?
		if ($this->is_upgrading())
			return false;

		return true;
	}

	/////// GETTER//////////

	/**
	 * Gibt die eindeutige ID des Gebäudes zurück.
	 *
	 * @return uint
	 */
	public function get_id() {
		return ($this->id) ? $this->id : 0;
	}

	/**
	 * Gibt den Zeitpunkt zurück, zu dem der letzte Upgrade gestartet wurde.
	 *
	 * @return uint
	 */
	public function get_t_stamp() {
		return $this->t_stamp;
	}

	/**
	 * Gibt die Structurepoints zurück, die das Gebäude dann hat, wenn es
	 * komplett repariert ist (also der höchst mögliche SP-Wert).
	 *
	 * @return uint
	 */
	public function get_max_sp() {
		return $this->max_sp;
	}

	/**
	 * Gibt die ID des Besitzers des Gebäudes zurück.
	 *
	 * @return uint
	 */
	public function get_uid() {
		return ($this->uid) ? $this->uid : 0;
	}

	/**
	 * Gibt die Position der oberen linken Ecke des Gebäudes an als Point
	 *
	 * @return Point2D	obere linke Ecke
	 */
	public function get_ulc() {
		return $this->ulc;
	}

	/**
	 * Gibt die ID des Sektors zurück, in dem das Gebäude steht
	 *
	 * @return uint
	 */
	public function get_sector_id() {
		return $this->sector_id;
	}

	/**
	 * Gibt zurück, ob das Gebäude gerade ausgebaut/konstruiert wird.
	 *
	 * @return bool
	 */
	public function is_upgrading() {
		return $this->upgrading;
	}

	/**
	 * Gibt den Aktuellen Ausbausfortschritt an
	 *
	 * @return ufloat
	 */
	public function get_progress() {
		return $this->progress + ((Time::get_t_now() - $this->t_stamp) / $this->get_grow());
	}

	public function get_finish_time() {
		return $this->finish_time;
	}

	public function get_xp_level() {
		return $this->xp_level;
	}

	public function get_color() {
		return $this->color;
	}

	public function get_map_id() {
		return $this->map_id;
	}

	public function get_world_id() {
		return $this->world_id;
	}

	/////////// CONVERTER /////////////

	/**
	 * Implementierung von IConvertableJSONArray. Gibt Objekt als JSON-Array
	 * zurück. Dabei werden die Array-Elemente wie folgt belegt:
	 * [0]: NAME
	 * [1]: array(ULC->X, ULC->Y)
	 * [2]: PIC_PATH_AND_FILE_NAME
	 * [3]: array(SIZE, HEIGHT)
	 * [4]: ID
	 *
	 * @return string
	 */
	public function to_json_array() {
		return Converter::arrayToJSON($this->to_php_array());
	}

	/**
	 * Implementierung von IConvertableJSArray. Gibt Objekt als JS-Array
	 * zurück. Dabei werden die Array-Elemente wie folgt belegt:
	 * [0]: NAME
	 * [1]: array(ULC->X, ULC->Y)
	 * [2]: PIC_PATH_AND_FILE_NAME
	 * [3]: array(SIZE, HEIGHT)
	 * [4]: encrypted(ID)	// encrypted, da ID sensibel
	 *
	 * @return string
	 */
	public function to_js_array() {
		return Converter::arrayToJSArray($result);
	}
	/**
	 * Implementierung von IConvertableJSArray. Gibt Objekt als JS-Array
	 * zurück. Dabei werden die Array-Elemente wie folgt belegt:
	 * [0]: NAME
	 * [1]: array(ULC->X, ULC->Y)
	 * [2]: PIC_PATH_AND_FILE_NAME
	 * [3]: array(SIZE, HEIGHT)
	 * [4]: array(PROGRESS, GROW, T_STAMP)
	 * [5]: IS_NEUTRAL_BUILDING
	 *
	 * @return string
	 */
	public function to_php_array() {
		$result = array();
		$result[0] = $this->get_name();
		$ulc = $this->get_ulc();
		$result[1] = array((int)$ulc->x, (int)$ulc->y);
		$result[2] = $this->get_pic();
		$result[3] = array($this->get_size(), $this->get_height());
		$result[4] = array($this->get_progress(), $this->get_grow(), $this->get_t_stamp());
		$result[5] = (($this->get_type() >= IBuildings::NEUTRAL_BUILDINGS_BEGIN)
				&& ($this->get_type() <=  IBuildings::NEUTRAL_BUILDINGS_END));
		return $result;
	}

	public function to_php_std_class() {
		$result = parent::to_php_std_class();
		$result->ulc = $this->get_ulc();
		$result->uid = $this->get_uid();
		$result->is_upgrading = $this->is_upgrading();
		$result->finish_time = $this->get_finish_time();
		$result->progress = $this->get_progress();
		$result->t_stamp = $this->get_t_stamp();
		$result->max_sp = $this->get_max_sp();
		$result->world_id = $this->get_world_id();
		$result->map_id = $this->get_map_id();
		$result->sector_id = $this->get_sector_id();
		$result->const_key = $this->get_const_key();
		$result->xp_level = $this->get_xp_level();
		$result->color = $this->get_color();
		return $result;
	}

	public function is_visible_to_user($uid) {
		return true;
	}
}
?>