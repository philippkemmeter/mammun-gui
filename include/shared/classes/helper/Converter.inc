<?php


/**
 * Die Converter-Klasse vereint mehrer Funktionen, um gewisse Typen und
 * Darstellungen in andere zu überführen.
 * Converter-Klasse ist als Namespace zu verstehen.
 */
class Converter {	/*Namespace*/

	/**
	 * Konvertiert ein stdObjekt in ein assotiatives Array.
	 *
	 * @param stdClass $array
	 * @return array
	 */
	public static function stdClassToArray($o) {
		if (is_array($o))
			return $o;
		foreach ($o as $k => $v) {
			$result[$k] = $v;
		}
		return $result;
	}

	/**
	 * Gibt einen String zurück, der das übergebene Array als Array-Deklaration
	 * in Javascript enthält. (also 'new Array(....);'). Da keine assotiativen
	 * Arrays in Javascript unterstützt werden, geht diese Information verloren!
	 * Bei assotiativen Arrays die Funtion arrayToJSObject benutzen.
	 *
	 * @param array $array	Das zu konvertierende Array
	 * @return string		Der resultierende Javascript-Array-String
	 */
	public static function arrayToJSArray(array $array) {
		$result = 'new Array(';
		$result .= self::arrayToJSArrayRecursive($array);
		$result .= ')';
		return $result;
	}

	private static function arrayToJSArrayRecursive(array $array) {
		if (empty($array))
			return '';
		foreach ($array as $data) {
			if (is_array($data)) {
				$result .= 'new Array(';
				$result .= self::arrayToJSArrayRecursive($data);
				$result .= '),';
			}
			else
				$result .= (is_numeric($data))? "$data,"	: "'$data',";
		}
		$result{strlen($result)-1} = ' ';	// letztes Komma löschen

		return $result;
	}

	/**
	 * Gibt einen String zurück, der das übergebene Array als JSON-Array
	 * enthält.
	 *
	 * @param array $array	Das zu konvertierende Array
	 * @return string		Der resultierende JSON-Array-String
	 */
	public static function arrayToJSON(array $array) {
		return json_encode($array);
	}

	public static function objectToJSON(stdClass $obj) {
		return json_encode($obj);
	}

	/**
	 * Portabler wrapper für die json_encode-Funktion. Nötig, da json_encode
	 * fehlerhafterweise primitive Datentypen nicht in ein Array umwandelt,
	 * und etwas wie "x" nicht in allen PHP-Versionen dekodiert werden kann.
	 * Wenn xToJson benutzt wird, dann kann der String nur per jsonToX
	 * dekodiert werden!
	 *
	 * @param mixed $mixed
	 * @return string
	 */
	public static function xToJson($mixed) {
		if ($mixed instanceof IConvertablePHPStdClass)
			return $mixed->to_php_std_class();
		else if (is_array($mixed) || is_object($mixed))
			return json_encode($mixed);
		else
			return json_encode(array($mixed));
	}

	/**
	 * Wandelt einen JSON-String in das um, was er darstellt. Wird dabei
	 * $reduce_one_elem_arrays=TRUE angegeben (default), werden einelementige
	 * Arrays auf das eine Element reduziert. Somit können auch primitive
	 * Typen mit xToJson kodiert und dann sauber mit jsonToX wieder dekodiert
	 * werden.
	 *
	 * @param string $string
	 * @param bool $reduce_one_elem_arrays
	 * @return mixed
	 */
	public static function jsonToX($string, $reduce_one_elem_arrays=TRUE) {
		$temp_result = json_decode($string);
		if ($reduce_one_elem_arrays && is_array($temp_result) &&
			(count($temp_result) == 1))
		{
			return $temp_result[0];
		}
		else
			return $temp_result;
	}

	/**
	 * Konvertiert einen HTTP-Query-String in ein Assoc-Array.
	 *
	 * Ein Query-String der Form <code>x=8&y=9</code> würde dann zu:
	 * <code>
	 * array(
	 * 	'x' => 8,
	 * 	'y' => 9
	 * );
	 * </code>
	 *
	 * @param string $string
	 * @return assoc
	 */
	public static function queryStringToAssoc($string) {
		$all_pairs = explode('&', $string);
		$n = count($all_pairs);
		for ($i = 0; $i < $n; $i++) {
			list($key, $value) = explode('=', $all_pairs[$i]);
			$result[$key] = $value;
		}
		return $result;
	}

	/**
	 * Konvertiert ein Assoc-Array in einen HTTP-Query-String.
	 *
	 * Array der Form:
	 * <code>
	 * array(
	 * 	'x' => 8,
	 * 	'y' => 9
	 * );
	 * </code>
	 *
	 * Würde dann zum String <code>x=8&y=9</code>.
	 *
	 * @param string $string
	 * @return assoc
	 */
	public static function assocToQueryString(array $assoc) {
		$result = '';
		foreach ($assoc as $key => $value) {
			$result .= "{$key}={$value}&";
		}
		return substr($result, 0, -1);
	}

	private function __construct() {}	// no Constructor => Namespace
}


?>