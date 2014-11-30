<?php
require_once (dirname(__FILE__).'/IGSS.inc');

/**
 * Eine Standartklasse, die im Gegensatz zur normalen Standardklasse Getter und
 * Setter aufweist, UND Singleton unterstützt. Da allerdings die Daten explizit
 * gesetzt werden müssen, ist es auch klar, dass ein get_instance durchaus
 * fehlschlagen kann, und die Funktion set_instance Sinn macht.
 *
 * @author Philipp Kemmeter
 */
class stdClassGSS implements IGSS {	// GSS = Getter, Setter, Singleton
	protected $data;
	protected $id;

	protected static $INSTANCES;

	/**
	 * Gibt die aktuelle Instanz zurück, die der ID zugeordnet ist, oder NULL,
	 * wenn keine Instanz mit der ID $id besteht.
	 *
	 * @param int $id
	 * @return stdClassGSS
	 */
	public static function get_instance($id) {
		if (self::$INSTANCES[$id])
			return self::$INSTANCES;
		return null;
	}


	/**
	 * Erzeugt explizit eine Neue Instanz unter der ID $id, und weist ihr die
	 * übergebenen Daten zu. Gibt die neu erzeugte Instanz zurück. Diese kann
	 * dann per get_instance abgerufen werden.
	 *
	 * @param int $id
	 * @param stdClass/array $data
	 * @return stdClassGSS
	 */
	public static function set_instance($id, $data) {
		$data = (array)$data;
		if (self::$INSTANCES[$id])
			unset(self::$INSTANCES[$id]);
		self::$INSTANCES[$id] = new stdClassGSS($id, $data);
		return self::$INSTANCES[$id];
	}

	/**
	 * Erzeugt eine neue stdClassGSS-Instanz. Private: get/set_instance
	 * benutzen.
	 *
	 * @param int $id
	 * @param array $data
	 * @return stdClassGSS
	 */
	protected function __construct($id, array $data) {
		$this->data = $data;
		$this->id = $id;
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
				trigger_error(sprintf('Call to undefined function: %s::%s().', get_class($this), $fn_name), E_USER_ERROR);
			}
			return $this->data[$what_to_get];
		}
		else if (strpos($fn_name, 'set_') === 0) {
			$what_to_set = substr($fn_name, 4);
			$this->data[$what_to_set] = $params[0];
		}
		else {
			trigger_error(sprintf('Call to undefined function: %s::%s().', get_class($this), $fn_name), E_USER_ERROR);
		}
	}

	/**
	 * Gibt die ID zurück. Wird per Datenübergabe zusätzlich eine ID-Wert
	 * angegeben, so wird dieser überschattet und kann nicht mehr abgerufen
	 * werden!
	 *
	 * @return int
	 */
	public function get_id() {
		return $this->id;
	}

}
?>