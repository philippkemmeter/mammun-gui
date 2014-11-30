<?php


/**
 * Objekte die diese Schnittstelle implementieren, können eine 
 * JSON-Array-Repräsentation von sich selbst zurückgeben. Da keine Assotiativen-
 * Arrays in JSON existieren und Arrays keine Funktionen haben, ist hier von
 * der implementierenden Klasse nur zu entscheiden, welche Attribute sie
 * freigeben will und in welcher Reihenfolge diese im Array erscheinen.
 */

interface IConvertableJSONArray {
	public function to_json_array();
}



?>