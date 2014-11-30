<?php

/**
 * Ein Punkt im 2-Dimensionalen Raum
 */

class Point2D {
	public $x;
	public $y;
	public function __construct($x=0, $y=0) {
		$this->x = (float)$x;
		$this->y = (float)$y;
	}
}

?>