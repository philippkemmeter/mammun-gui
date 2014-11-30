<?php


/**
 * Objekte die diese Schnittstelle implementieren, können eine 
 * PHP-Array-Repräsentation von sich selbst zurückgeben. Hierbei sind keine
 * Assotiativen Arrays gemeint, sondern nur die nummerierten!
 */

interface IConvertablePHPArray {
	public function to_php_array();
}



?>