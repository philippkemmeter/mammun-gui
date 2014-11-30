<?php


/**
 * Objekte die diese Schnittstelle implementieren, können eine assotiative 
 * PHP-Array-Repräsentation von sich selbst zurückgeben.
 */

interface IConvertablePHPStdClass {
	public function to_php_std_class();
}


?>