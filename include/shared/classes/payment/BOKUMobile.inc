<?php
/**
 * Diese Klasse enthält Kommunikationsmethoden mit BOKU.
 * 
 * @author bolek
 */
class BOKUMobile {
	/**
	 * Diese Funktion generiert die Signatur für eine BOKU-Anfrage gemäß der
	 * Security-Spezifikation von BOKU (BOKU Security Implementation). Dazu
	 * muss ein Asso-Array von POST-Key-Value-Paren übergeben werden, welche
	 * an den BOKU-Server gesendet werden sollen. Auf der Basis der Variablen
	 * sowie des Security-Keys (self::SECURITY_KEY) wird dann ein md5-Hash
	 * erstellt, welcher als Parameter sig zusätzlich an den Server geschickt
	 * werden muss.
	 * Achtung: timestamp muss im übergebenen POST-Array enthalten sein.
	 * 
	 * @param string[] $post_array
	 * @return string
	 */
	private static function create_signatur($post_array) {
		if (!$post_array['timestamp'])
			throw new IllegalArgumentException("Kein timestamp in \$postarray: ".
													print_r($post_array,true));
		unset($post_array['password']);
		ksort($post_array);
		$result_string = '';
		foreach ($post_array as $key => $value) {
			if (empty($value))
				continue;
			$result_string .= "$key$value";
		}
		$result_string .= self::SECURITY_KEY;
		return md5($result_string);
		
	}
	
	/**
	 * Konvertiert ein Key-Value-Pair-Array in einen POST-String (&..=..&..=)
	 * 
	 * @param string[] $post_array
	 * @return string
	 */
	private static function create_post_string($post_array) {
		$result = '';
		foreach ($post_array as $key => $value)
			$result .= "$key=$value&";
		return substr($result, 0, -1);
	}
}
?>