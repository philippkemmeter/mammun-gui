<?
require_once(dirname(__FILE__).'/../main_funcs.inc');
require_once(dirname(__FILE__).'/ValueChecker.inc');

/**
 * RequestChecker
 *
 * Diese Klasse prüft hereinkommende GET- und POST-Parameter auf ihre 
 * Korrektheit, um XSS-Attacken und SQL-Injections zu verhindern.
 *
 * Sie entspricht dem älteren PV_encryption, dass aus nur noch da ist, um alte
 * Projekte lauffähig zu halten.
 *
 * In Zukunft sollten alle auf RequestChecker umstellen. PV_encryption wird
 * nicht mehr supportet.
 *
 * @author Philipp Kemmeter
 */
class RequestChecker {
	/**
	 * Lädt die übergebenen GET- bzw. POST-Variablen.
     *
	 * Dabei werden nur die geladen, die in $expected_data enthalten sind und
	 * auf den angegebenen Typ hin geprüft. Strings werden zusätzlich auf
	 * gefährliche Zeichen hin untersucht - diese werden silent ersetzt.
	 * Falls jedoch eine Variable einen falschen Typ aufweist, wird eine
	 * IllegalArgumentException geworfen.
	 *
	 * Es müssen nicht alle erwarteten Parameter auch übergeben werden. Alles,
	 * was aber nicht erwartet wird, wird einfach nicht geladen.
	 *
	 * Gültige Typen sind:
	 * int
	 * id
	 * float
	 * string
	 * nospacestring
	 * email
	 *
	 * @param array $expected_data Assotiatives Array (parameter-name => typ)
	 * @return array
	 */ 
	public static function load_GET_POST($expected_data) {
		if ($_SERVER['REQUEST_METHOD'] == 'POST') {
			foreach ($_POST as $n => $v) {
				if (isset($data[$n]))
					continue;
				if (isset($expected_data[$n]))
					$data[$n] = self::load_one_post($n, $expected_data[$n]);
			}
		}
		else {
			foreach ($_GET as $n => $v) {
				if (isset($data[$n]))
					continue;
				if (isset($expected_data[$n]))
					$data[$n] = self::load_one_get($n, $expected_data[$n]);
			}
		}
		foreach ($expected_data as $name => $type) {
			if (!isset($data[$name])) {
				if (isset ($_POST[$name])) {
					$data[$name] = self::load_one_post($name, $type);
				}
				else if (isset ($_GET[$name])) {
					$data[$name] = self::load_one_get($name, $type);
				}
			}
		}
		return isset($data) ? $data : array();
	}

	private static function load_one($data, $name, $type) {
		$de_data = null;
		if (isset($data[$name])) {
			if ($type == 'string' || $type == 'nospacestring' 
				|| $type == 'email')
			{
				$value = check_string($data[$name], $type == 'nospacestring');
				if ($type == 'nospacestring')
					$type = 'string';
			}
			else
				$value = $data[$name];

			$de_data = ValueChecker::$type($value, $name);
		}

		return $de_data;
	}

	private static function load_one_post($name, $type) {
		return self::load_one($_POST, $name, $type);
	}

	private static function load_one_get($name, $type) {
		return self::load_one($_GET, $name, $type);
	}

}
?>
