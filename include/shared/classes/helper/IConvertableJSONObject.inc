<?php


/**
 * Objekte die diese Schnittstelle implementieren, können eine 
 * JSON-Objekt-Repräsentation von sich selbst zurückgeben. Ob nur die Werte
 * oder auch die Funkionen und in welchem Umfang ist dem Objekt überlassen.
 */

interface IConvertableJSONObject {
	public function to_json_object();
}

?>