<?php


/**
 * Objekte die diese Schnittstelle implementieren, können eine assotiative 
 * PHP-Array-Repräsentation von sich selbst zurückgeben.
 */

interface IConvertablePHPAssocArray {
	public function to_php_assoc_array();
}



?>