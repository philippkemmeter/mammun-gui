<?php
require_once(dirname(__FILE__).'/IIndividualization.inc');

class IndividualItem implements IIndividualization {
	public $id;
	public $type;

	public function __construct($type, $id=0) {
		if (is_object($type)) {
			$this->id = (int)$type->id;
			$this->type = (int)$type->type;
		}
		else {
			$this->type = $type;
			$this->id = $id;
		}
	}

	/**
	 * Konvertiert das Objekt in ein Standardobjekt
	 * @see include/shared/classes/helper/IConvertablePHPStdClass#to_php_std_class()
	 */
	public function to_php_std_class() {
		$x = new stdClass();
		$x->id = $this->id;
		$x->type = $this->type;
		return $x;
	}

	/**
	 * Konvertiert das Objekt in ein php Array
	 * @see include/shared/classes/helper/IConvertablePHPArray#to_php_array()
	 */
	public function to_php_array() {
		$x = array();
		$x[$this->type] = $this->id;
		return $x;
	}
}
?>