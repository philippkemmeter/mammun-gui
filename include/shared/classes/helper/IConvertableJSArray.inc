<?php


/**
 * Objekte die diese Schnittstelle implementieren, können eine 
 * JS-Array-Repräsentation von sich selbst zurückgeben. Im Unterschied zu JSON-Arrays
 * sind hier direkt in JS (ohne eval) einfügbarer String gemeint (also "new Array(...)").
 */

interface IConvertableJSArray {
	public function to_js_array();
}


?>