<?
require_once(dirname(__FILE__).'/../main_funcs.inc');
define("ENCRYPT_DEBUG", 0);
define("SECRET_KEY", "Wer andern eine Grube graebt hat oft einen an der Waffel... Muh!");
define("KEY_LENGTH", 10);
define("EN_KEY_LENGTH", 32); // ein 10stelliger Key ist nach verschluesselung 32 Zeichen lang

function is_intValued($var) {
	// Determines if a variable's value is an integer regardless of type
	// meant to be an analogy to PHP's is_numeric()
	if (is_int($var))
		return TRUE;
	if (is_string($var) and $var === (string) (int) $var)
		return TRUE;
	if (is_float($var) and $var === (float) (int) $var)
		return TRUE;
	else
		return FALSE;
}

/**
 * Aus der PV-Zeit. Veraltet. Nicht mehr benutzen. Bitte RequestChecker nutzen.
 * @deprecated
 */
class PV_encryption {
	var $td = '';
	var $iv = '';
	var $ks = '';
	var $key = '';

	/**
	 * Erstellt ein neues PV_encryption-Objekt.
	 * Initialisiert die mcrypt-Ressourcen $td, $iv, $key.
	 *
	 * ACHTUNG: Muss NACH session_start aufgerufen werden, aus einem noch unklaren Grund killt er sonst
	 * die Session.
	 *
	 * @param $secret_key Key zum ver- und entschluesseln.
	 * @return PV_encryption
	 */
	public function __construct($secret_key = SECRET_KEY) {
		if (function_exists('mcrypt_module_open')) {
			$this->td = mcrypt_module_open(MCRYPT_BLOWFISH, '/usr/lib/php5/20060613/mcrypt', MCRYPT_MODE_ECB, '');
			srand(time());
			$this->iv = mcrypt_create_iv(mcrypt_enc_get_iv_size($this->td), MCRYPT_RAND);/*MCRYPT_DEV_RANDOM*/
			$this->ks = mcrypt_enc_get_key_size($this->td);
		}
		$this->setKey($secret_key);
	}

	/**
	 * Setzt den Key auf den angegebenen Wert
	 *
	 * @param $secret_key Key zum ver- und entschluesseln.
	 */

	function load_GET_POST($expected_data)
	/**
	 * Läd alle werte aus den encrypteten dataX und dataY ararys sowie get POST.
	 *
	 * @param $expected_data hat das format array('name'=>'type')
	 * erlaubte typen sind:
	 *   'eint' (encrypted integer, wird aus dataX oder dataY geladen
	 *   'ebool'(encrypted bool, wird aus dataX oder dataY geladen. gecheckt wird einfach ob der wert vorhanden ist oder nicht
	 *   'estring' (encrypted string, wird aus dataX oder dataY geladen und mittels check_String überprüft
	 *   'enospacestring' (string, darf aber kein leerzeichen enthalten. wird aus dataX bzw dataY geladen und mittels check_string gecheckt
	 *   '(e)int' (encrypted integer, wird aus dataX oder dataY geladen WENN NICHT IN dataX VORHANDE WIRD ER AUS POST/GET GELADEN
	 *   '(e)bool'(encrypted bool, wird aus dataX oder dataY geladen. gecheckt wird einfach ob der wert vorhanden ist oder nicht WENN NICHT IN dataX VORHANDE WIRD ER AUS POST/GET GELADEN
	 *   '(e)string' (encrypted string, wird aus dataX oder dataY geladen und mittels check_String überprüft WENN NICHT IN dataX VORHANDE WIRD ER AUS POST/GET GELADEN
	 *   '(e)nospacestring' (string, darf aber kein leerzeichen enthalten. wird aus $_POST bzw $_GET geladen und mittels check_string gecheckt WENN NICHT IN dataX VORHANDE WIRD ER AUS POST/GET GELADEN
	 *   'seint' (seperately encrypted integer, wird aus einem separaten variablennamen a la race1=sadkgubhligubflksjngsdk3zr89035ztifgbdl geladen (sinnvoll z.b. bei tribes mit mehreren select boxen
	 *   'int' (integer, wird aus $_POST bzw $_GET geladen und gecastet
	 *   'bool' (bool, wird einfach gecheckt ob der entsprechende Wert in $_POST oder $_GET gesetzt ist
	 *   'string' (string, wird aus $_POST bzw $_GET geladen und mittels check_string gecheckt
	 *   'nospacestring' (string, darf aber kein leerzeichen enthalten. wird aus $_POST bzw $_GET geladen und mittels check_string gecheckt
	 *   ACHTUNG: nicht mehr erlaubt ist ein leerer Type '' führt zu errormail und die.
	 */
	{
		$nothing = false;
		$de_data = null;
		if ($_SERVER['REQUEST_METHOD'] == 'POST') {
			if (isset($_POST['dataX'])) {
				$session = 1;
				$data = $_POST['dataX'];
			}
			elseif (isset($_POST['dataY'])) {
				$session = 0;
				$data = $_POST['dataY'];
			}
			else
				$nothing = true;
		}
		else {
			if (isset($_GET['dataX'])) {
				$session = 1;
				$data = $_GET['dataX'];
			}
			elseif (isset($_GET['dataY'])) {
				$session = 0;
				$data = $_GET['dataY'];
			}
			else
				$nothing = true;
		}
		if ((!$nothing) && ($data) && (($de_data = $this->do_decryption($data, $expected_data, $session)) === false)) {
			if ($_SERVER['PHP_SELF'] == '/activate.php') {
				error_log ('encrypt_error1: could not decrypt in activate.inc'); //HACKATTACK!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
			}
			else {
				error_log ('encryption error: could not decrypt (wrong key?)');
			}
			die();
		}
		if ($_SERVER['REQUEST_METHOD'] == 'POST') {
			foreach ($_POST as $n => $v) {
				if (isset($de_data[$n]))
					continue;
				if (isset($expected_data[$n]))
					$de_data[$n] = $this->load_one_post($n, $expected_data[$n]);
			}
		}
		else {
			foreach ($_GET as $n => $v) {
				if (isset($de_data[$n]))
					continue;
				if (isset($expected_data[$n]))
					$de_data[$n] = $this->load_one_get($n, $expected_data[$n]);
			}
		}
		foreach ($expected_data as $name => $type) {
			if (!isset ($de_data[$name])) {
				if (($name{0} == '%') && ($type{0} != 'e')) {
					if ($_SERVER['REQUEST_METHOD'] == 'POST') {
						foreach ($_POST as $n => $v)
							if (preg_match('/^'.substr($name, 1).'$/i', $n))
								$de_data[$n] = $this->load_one_post($n, $type);
					}
					else {
						foreach ($_GET as $n => $v)
							if (preg_match('/^'.substr($name, 1).'$/i', $n))
								$de_data[$n] = $this->load_one_get($n, $type);
					}
				}
				else
					if (isset ($_POST[$name])) {
						$de_data[$name] = $this->load_one_post($name, $type);
					}
					else
						if (isset ($_GET[$name])) {
							$de_data[$name] = $this->load_one_get($name, $type);

						}
			}
		}
		//    print_r($de_data);
		return $de_data ? $de_data : array();
	}

	private function load_one($data, $name, $type) {
		if (($type == 'ebool') || ($type == 'eint') || ($type == 'estring')) {
			error_log ('encryption error, expected '.$name.' to be in dataX but found it in POST');
			die();
		}
		$de_data = null;
		if (($type == 'string') || ($type == '(e)string') || ($type == 'nospacestring') || ($type == '(e)nospacestring')) {
			if (($de_data = check_string($data[$name], (($type == 'nospacestring') || ($type == '(e)nospacestring')))) === false) {
				error_log ('encrypt_error2'); //HACKATTACK!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
				die();
			}
		}
		else if (($type == 'serialized') || ($type == '(e)serialized')) {
			$de_data = unserialize(stripslashes($data[$name]));
		}
		else if (($type == 'int') || ($type == '(e)int')) {
			if (is_intValued($data[$name]))
				$de_data = (int) $data[$name];
		}
		else if (($type == 'bool') || ($type == '(e)bool')) {
			if ($data[$name])
				$de_data = true;
		}
		else if ($type == 'seint') {
			if (($de_data = $this->do_decryption($data[$name], array ($name => 'seint'))) === false) {
				error_log("Encrytion of $name: {$data[$name]} (seint) failed.");
				error_log ('encrypt_error3'); //HACKATTACK!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
				die();
			}
			else
				$de_data = (int) $de_data[$name];
		}
		else if ($type == 'seint_nosession') {
			if (($de_data = $this->do_decryption($data[$name], array ($name => 'seint'), false)) === false) {
				error_log ('encrypt_error4'); //HACKATTACK!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
				die();
			}
			else
				$de_data = (int) $de_data[$name];
		}
		else {
			error_log ('encrypt_error5');
			die();
		}
		return $de_data;
	}

	private function load_one_post($name, $type) {
		return $this->load_one($_POST, $name, $type);
	}

	private function load_one_get($name, $type) {
		return $this->load_one($_GET, $name, $type);
	}

	function setKey($secret_key = SECRET_KEY) {
		$this->key = substr(md5($secret_key), 0, $this->ks);
	}

	/**
	 * Verschluesselt Daten und gibt das Ergebnis der Verschl. zurueck
	 *
	 * @param $value zu verschluesselder Wert
	 * @result das verschluesselte Ergebnis
	 */
	function encrypt($value, $session = true) {
		if (($session) && (is_array($value)))
			$value['sid'] = session_id();
		if (function_exists('mcrypt_generic_init')) {
			mcrypt_generic_init($this->td, $this->key, $this->iv);
			$returnValue = base64_encode(mcrypt_generic($this->td, serialize($value)));
			mcrypt_generic_deinit($this->td);
		}
		else {
			$returnValue = base64_encode(serialize($value));
		}
		$returnValue = str_replace(array('+'),array('-'),$returnValue); // MIME::Base64::URLSafe
		return $returnValue;
	}

	/**
	 * Entschluesselt Daten und vergleicht die Keys des herauskommenden Arrays
	 * mit denen, die uebergeben wurden als erwaretete Werte. Bei Abweichung
	 * gibt die Funktion false zurueck;
	 *
	 * @param $value zu entschluessender Wert
	 * @param $expected_data ein assoziatives Array, das erwartete Keys auf
	 *                       erwartete Types abbildet. Die Keys muessen nicht
	 *                       alle vorkommen, aber wenn, dann mit den richtigen
	 *                       Typen.
	 * @return gibt den entschluesselten Wert zurueck oder false, wenn etwas
	 *         unerwartetes herauskommt (z.B. falsche keys)
	 */
	function decrypt($value, $expected_data = '', $session = true) {
		$value = str_replace(array('-'),array('+'),$value); // MIME::Base64::URLSafe
		if (function_exists('mcrypt_generic_init')) {
			mcrypt_generic_init($this->td, $this->key, $this->iv);
			$de_value = unserialize(mdecrypt_generic($this->td, base64_decode($value)));
			mcrypt_generic_deinit($this->td);
		}
		else {
			$de_value = unserialize(base64_decode($value));
		}

		if (ENCRYPT_DEBUG) {
			error_log('session:'.$session."\n".
					'value:'.$value."\n".
					'de_value:'.print_r($de_value, true)
			);
		}

		if (!empty ($expected_data)) {
			$returnValue = array ();

			// Session-ID-Test
			if ($session) {
				if ($de_value['sid'] != session_id()) {

					if (ENCRYPT_DEBUG) {
						error_log("session-error: erwartet:".session_id().', gelesen:'.$de_value['sid']);
					}
					if (!session_id()) {
						//	    errormail('session died but this is no encrypt error :) PV_encryption');
						exit;
					}
					return false;
				}
			}

			for (reset($de_value); $key = key($de_value); next($de_value)) {
				if ($key == 'sid')
					continue; 	// sid wird immer mitgeschickt und muss
								// somit bei expected_data nicht angegeben
								// werden
				$found = false;
				$ok = false;
				for (reset($expected_data); $key2 = key($expected_data); next($expected_data)) {
					if (($key == $key2) || (($key2{0} == '%') && preg_match('/^'.substr($key2, 1).'$/i', $key))) {
						if (ENCRYPT_DEBUG)
							error_log("$key: {$de_value[$key]} - $key2: {$expected_data[$key2]}<br>");

						if (($expected_data[$key2] == '(e)int') || ($expected_data[$key2] == 'eint') || ($expected_data[$key2] == 'seint')) {
							if (!isset ($de_value[$key]))
								$ok = true;
							else {
								$found = is_intValued($de_value[$key]); // auf Typ Integer hin überprüfen
								if (ENCRYPT_DEBUG && !$found)
									error_log("not an int $de_value[$key]<br>\n");
							}
						}
						else
							if ((($expected_data[$key2] == '(e)bool') || ($expected_data[$key2] == 'ebool')) && ($de_value[$key])) {
								$found = 1;
								$de_value[$key] = 1;
							}
							else
								if (($expected_data[$key2] == 'estring') || ($expected_data[$key2] == '(e)string') || ($expected_data[$key2] == '(e)nospacestring') || ($expected_data[$key2] == 'enospacestring')) {
									if (!isset ($de_value[$key]))
										$ok = true;
									else {
										//		  echo 'OKOK<br><br>';
										$de_value[$key] = check_string($de_value[$key], (($expected_data[$key2] == '(e)nospacestring') || ($expected_data[$key2] == 'enospacestring'))); // gefaehrliche Zeichen filtern
										$found = true;
									}
								}
					}
				}
				if ((!$found) && (!$ok)) {
					/*	    if (ENCRYPT_DEBUG)
					  echo( "!found error:".$key.' was sent but is not classified!');
					return false;*/ //geht nich mehr wegen accesscontrol
				}
				else
					if ($found)
						$returnValue[$key] = $de_value[$key];
				if (ENCRYPT_DEBUG)
					error_log("returned:{$returnValue[$key]}<br>");
			}
			return $returnValue;
		}

		return $de_value;
	}

	/**
	 * erzeugt eine zufaellige Zeichenfolge der Laenge $length und gibt
	 * diese zurueck;
	 *
	 * @param $length Laenge des Strings
	 * @result der generierte String
	 */
	function getRandomKey($length = KEY_LENGTH) {
		$result = '';
		for ($i = 0; $i < $length; $i ++)
			$result .= chr(rand(97, 122));

		return $result;
	}

	/**
	 * komfortable encrypt-Funktion ;)
	 *
	 * @param $data 			zu Verschluesseldes
	 * @param bool $session		ob die Session mitberücksichtigt werden soll
	 */
	function do_encryption(array $data, $session = true) {
		$this->setKey();
		$secret_key = $this->getRandomKey();
		$encrypted_key = $this->encrypt($secret_key, $session);

		$this->setKey($secret_key);

		return $encrypted_key. ($this->encrypt($data, $session));
	}

	/**
	 * komfortable decrypt-Funktion ;)
	 *
	 * @param string $dataX zu Entschluesseldes
	 * @param array $expected_data ein assoziatives Array, das erwartete Keys auf
	 *                       erwartete Types abbildet. Die Keys muessen nicht
	 *                       alle vorkommen, aber wenn, dann mit den richtigen
	 *                       Typen.
	 * @return gibt den entschluesselten Wert zurueck oder false, wenn etwas
	 *         unerwartetes herauskommt (z.B. falsche keys)
	 */
	function do_decryption($dataX, array $expected_data, $session = true) {
		$this->setKey();
		if (ENCRYPT_DEBUG)
			error_log("$this->key");
		$secret_key = $this->decrypt(substr($dataX, 0, EN_KEY_LENGTH), '', $session);
		if (ENCRYPT_DEBUG)
			error_log("Got Key: $secret_key");
		$data = substr($dataX, EN_KEY_LENGTH);

		$this->setKey($secret_key);

		return $this->decrypt($data, $expected_data, $session);
	}
}
?>
