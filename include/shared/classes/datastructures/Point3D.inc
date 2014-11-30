<?php
require_once(dirname(__FILE__).'/Point2D.inc');

/**
 * Ein Punkt im 3-Dimensionalen Raum
 */

class Point3D extends Point2D {
	public $z;
	public function __construct($x=0, $y=0, $z=0) {
		parent::__construct($x, $y);
		$this->z = (float)$z;
	}
}

?>