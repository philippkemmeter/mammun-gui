<?php
require_once (dirname(__FILE__).'/../shared/classes/stdClassGS.inc');

class MapDescriptionData {
	protected static $INSTANCES;

	protected $data;
	protected $world_id;
	protected $map_id;

	/**
	 * Gibt die aktuelle Instanz zurück.
	 *
	 * @param int $world_id
	 * @param int $map_id
	 * @return MapDescriptionData
	 */
	public static function get_instance($world_id, $map_id) {
		if (self::$INSTANCES[$world_id][$map_id])
			return self::$INSTANCES[$world_id][$map_id];
		return null;
	}


	/**
	 * Erzeugt explizit eine Neue Instanz unter der ID $id, und weist ihr die
	 * übergebenen Daten zu. Gibt die neu erzeugte Instanz zurück. Diese kann
	 * dann per get_instance abgerufen werden.
	 *
	 * @param int $world_id
	 * @param int $map_id
	 * @param stdClass/array $data
	 * @return MapDescriptionData
	 */
	public static function set_instance($world_id, $map_id, $data) {
		$data = (array)$data;
		if (self::$INSTANCES[$world_id][$map_id])
			unset(self::$INSTANCES[$world_id][$map_id]);
		self::$INSTANCES[$world_id][$map_id] = new MapDescriptionData(
			$world_id, $map_id, $data);
		return self::$INSTANCES[$world_id][$map_id];
	}

	protected function __construct($world_id, $map_id, array $data) {
		$this->world_id = $world_id;
		$this->map_id = $map_id;
		$this->data = $data;
	}

	/**
	 * Getter und Setter werden dynamisch erstellt. Es gilt, dass man alles
	 * setzen kann, was man will, aber nicht gesetzte Eigenschaften, die durch
	 * get geholt werden sollen, einen Fehler erzeugen. Beispiel:
	 *
	 * $x = stdClassX::set_instance(1, array('x' => 3, 'y' => 7));
	 * $x->set_z(4);	// hiermit, wird get_z() miterzeugt
	 * $x->get_z();		// kein Problem, obwohl initial nicht dabei gewesen
	 * $x->get_u();		// FATAL ERROR, für U gibt es keinen Wert
	 * $x->muh();		// FATAL EROOR, es gibt nur getter und setter :)
	 * $x->get_id();	// kein Problem, die ID wird extra gespeichert
	 *
	 * Explizite Getter und Setter können Objekte haben, die dann
	 * hiervon erben :)
	 */
	public function __call($fn_name, array $params) {
		if (strpos($fn_name, 'get_') === 0) {	// "->get_[...]"
			$what_to_get = substr($fn_name, 4);
			if (!isset($this->data[$what_to_get])) {
				debug_print_backtrace();
				print_r($data);
				trigger_error(sprintf('Call to method function: %s::%s().', get_class($this), $fn_name), E_USER_ERROR);
			}
			return $this->data[$what_to_get];
		}
		else if (strpos($fn_name, 'set_') === 0) {
			$what_to_set = substr($fn_name, 4);
			$this->data[$what_to_set] = $params[0];
		}
		else {
			trigger_error(sprintf('Call to method function: %s::%s().', get_class($this), $fn_name), E_USER_ERROR);
		}
	}

	public function get_world_id() {
		return $this->world_id;
	}

	public function get_map_id() {
		return $this->get_map_id();
	}
}
?>