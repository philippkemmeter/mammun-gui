<?php
require_once(dirname(__FILE__).'/exceptions/IllegalArgumentException.inc');

/**
 * Diese Klasse ist rein statisch und gruppiert alle Funktionen, die zum prüfen
 * von  Werten  nötig  sind.  Immer,  wenn  der  Test  fehlschlägt,  wird  eine
 * IllegalArgumentException  geworfen  mit  einer  schönen  Fehlermeldung  à la
 * Mengenlehre.
 *
 * @author Philipp Kemmeter
 */
class ValueChecker {
	/**
	 * Instanziierung der Klasse nicht möglich.
	 */
	private function __construct() {}

	/**
	 * Prüft,  ob der  übergebene  Wert  ein String  ist oder  zu einem  String
	 * gecastet werden kann.
	 *
	 * Mithilfe der anderen Parameter kann der String näher eingegrenzt werden.
	 * Es wird der übergebene Wert zurückgegeben.
	 *
	 * @param mixed $value					Der zu prüfende Wert
	 * @param string $varname				Name der Variable  (für die Ausgabe
	 * 										der Fehlermeldung.
	 * @param bool $empty_allowed			Ob Leerstring okay ist.
	 * @param int/FALSE $min				Minimale Anzahl  Zeichen. Bei FALSE
	 * 										gibt es kein Minumum.
	 * @param int/FALSE $max				Maximale  Anzahl Zeichen. Bei FALSE
	 * 										gibt es kein Maxmimum.
	 * @param string $disallowed_chars		String  mit   Zeichen,   die  nicht
	 * 										enthalten sein dürfen.
	 * @throws IllegalArgumentException		Wenn der zu testende Wert nicht den
	 * 										übergebenen Kriterien entspricht.
	 * @return string
	 */
	public static function string($value, $varname, $empty_allowed=false,
		$min=false, $max=false, $disallowed_chars='')
	{
		if (!$empty_allowed && empty($value)) {
			throw new IllegalArgumentException(
				"\$$varname must not be empty; '$value' given"
			);
		}
		if (!empty($disallowed_chars)) {
			$c_arr = str_split($disallowed_chars);
			for ($i = 0, $n = count($c_arr); $i < $n; $i++) {
				if (strpos($value, $c_arr[$i]) !== false) {
					throw new IllegalArgumentException(
						"\$$varname must not contain '{$c_arr[$i]}'; ".
							"'$value' given"
					);
				}
			}
		}

		if (($min !== false) && (strlen($value) < $min)) {
			throw new IllegalArgumentException(
				"\$$varname must be at least $min characters long; ".
					"'$value' given"
			);
		}
		if (($max !== false) && (strlen($value) > $max)) {
			throw new IllegalArgumentException(
				"\$$varname must be at most $max characters long; ".
					"'$value' given"
			);
		}

		if (($min !== false) && ($max !== false) && ($min > $max)) {
			throw new IllegalArgumentException(
				"\$max has to be greater than \$min"
			);
		}

		return (string)$value;
	}

	/**
	 * Prüft, ob der übergebene  Wert ein Float  ist oder  zu einem  Float ohne
	 * Wertverlust gecastet werden kann.
	 *
	 * Somit sind Integer ebenso  erlaubt wie auch Strings, die einen Floatwert
	 * repräsentieren.
	 *
	 * @param mixed $value					Der zu prüfende Wert
	 * @param string $varname				Name der Variable  (für die Ausgabe
	 * 										der Fehlermeldung.
	 * @param int/FALSE $min				Kleinst  erlaubter Wert.  Bei FALSE
	 * 										gibt es kein Minimum.
	 * @param int/FALSE $max				Größt  erlaubter  Wert.  Bei  FALSE
	 * 										gibt es kein Maximum.
	 * @throws IllegalArgumentException		Wenn der zu testende Wert nicht den
	 * 										übergebenen Kriterien entspricht.
	 * @return float
	 */
	public static function float($value, $varname, $min=false,
		$max=false)
	{
		if ($min === false) {
			if ($max === false) {
				if (!is_anfloat($value)) {
					throw new IllegalArgumentException(
						"\$$varname", 'R', $value);
				}
			}
			else if ($max == 0) {
				if (!is_anfloat($value) || ($value > $max)) {
					throw new IllegalArgumentException(
						"\$$varname", 'R0-', $value
					);
				}
			}
			else if ($max == 1) {
				if (!is_anfloat($value) || ($value > $max)) {
					throw new IllegalArgumentException(
						"\$$varname", 'R-', $value
					);
				}
			}
			else {
				if (!is_anfloat($value) || ($value > $max)) {
					throw new IllegalArgumentException(
						"\$$varname", "]-INF; $max]", $value
					);
				}
			}
		}
		else {
			if ($max === false) {
				if ($min == 0) {
					if (!is_anfloat($value) || ($value < $min)) {
						throw new IllegalArgumentException(
							"\$$varname", 'R0+', $value
						);
					}
				}
				else if ($min == 1) {
					if (!is_anfloat($value) || ($value < $min)) {
						throw new IllegalArgumentException(
							"\$$varname", 'R+', $value
						);
					}
				}
				else {
					if (!is_anfloat($value) || ($value < $min)) {
						throw new IllegalArgumentException(
							"\$$varname", "[$min; INF[", $value
						);
					}
				}
			}
			else {
				if ($min > $max) {
					throw new IllegalArgumentException(
						"\$max has to be greater than \$min"
					);
				}
				if (!is_anfloat($value) || ($value < $min) || ($value > $max)) {
					throw new IllegalArgumentException(
						"\$$varname", "[$min; $max]", $value
					);
				}
			}
		}

		return (float)$value;
	}

	/**
	 * Prüft, ob der übergebene Wert eine ID ist.
	 *
	 * IDs sind Integerwerte die >= 0 sind. Viele sind auch > 0.
	 *
	 * @param mixed $value					Der zu prüfende Wert
	 * @param string $varname				Name der Variable  (für die Ausgabe
	 * 										der Fehlermeldung.
	 * @param bool $zero_allowed			Ob 0 als Wert erlaubt ist.
	 * @throws IllegalArgumentException		Wenn der zu testende Wert nicht den
	 * 										übergebenen Kriterien entspricht.
	 * @return uint
	 */
	public static function id($value, $varname, $zero_allowed=false) {
		if ($zero_allowed)
			return self::int($value, $varname, 0);
		else
			return self::int($value, $varname, 1);
	}

	/**
	 * Prüft, ob der übergebene Wert eine gültige Sector-ID ist.
	 *
	 * Eine Sector-ID ist eine ID im Wertebereich von 0 bis 99.
	 *
	 * @param mixed $value					Der zu prüfende Wert
	 * @param string $varname				Name der Variable  (für die Ausgabe
	 * 										der Fehlermeldung.
	 * @throws IllegalArgumentException		Wenn der zu testende Wert nicht den
	 * 										übergebenen Kriterien entspricht.
	 * @return uint
	 */
	public static function sector_id($value, $varname) {
		return self::int($value, $varname, 0, 99);
	}

	/**
	 * Prüft, ob der angegebene Wert ein gültiger UNIX-Timestamp ist.
	 *
	 * @param mixed $value					Der zu prüfende Wert
	 * @param string $varname				Name der Variable  (für die Ausgabe
	 * 										der Fehlermeldung.
	 * @throws IllegalArgumentException		Wenn der zu testende Wert nicht den
	 * 										übergebenen Kriterien entspricht.
	 * @return uint
	 */
	public static function t_stamp($value, $varname) {
		return self::int($value, $varname, 0);
	}

	/**
	 * Prüft, ob der angegebene Wert ein Integer ist  oder verlustfrei in einen
	 * Integer gecastet werden kann.
	 *
	 * Dabei  können  durch   die  anderen  Parameter  weitere  Einschränkungen
	 * definiert werden.
	 *
	 * @param mixed $value					Der zu prüfende Wert
	 * @param string $varname				Name der Variable  (für die Ausgabe
	 * 										der Fehlermeldung
	 * @param int/FALSE $min				Kleinst  erlaubter Wert.  Bei FALSE
	 * 										gibt es kein Minimum.
	 * @param int/FALSE $max				Größt  erlaubter  Wert.  Bei  FALSE
	 * 										gibt es kein Maximum.
	 * @throws IllegalArgumentException		Wenn der zu testende Wert nicht den
	 * 										übergebenen Kriterien entspricht.
	 * @return int
	 */
	public static function int($value, $varname, $min=false, $max=false) {
		if ($min === false) {
			if ($max === false) {
				if (!is_anint($value)) {
					throw new IllegalArgumentException("\$$varname",
						'Z', $value);
				}
			}
			else {
				if (!is_anint($value) || ($value > $max)) {
					throw new IllegalArgumentException(
						"\$$varname", '{-INF ..., '.($max-1).", $max}", $value
					);
				}
			}
		}
		else {
			if ($max === false) {
				if ($min == 0) {
					if (!is_anint($value) || ($value < $min)) {
						throw new IllegalArgumentException(
							"\$$varname", 'N0', $value
						);
					}
				}
				else if ($min == 1) {
					if (!is_anint($value) || ($value < $min)) {
						throw new IllegalArgumentException(
							"\$$varname", 'N1', $value
						);
					}
				}
				else {
					if (!is_anint($value) || ($value < $min)) {
						throw new IllegalArgumentException(
							"\$$varname", '{'.$min.', '.($min+1).', ... INF}',
							$value
						);
					}
				}
			}
			else {
				if ($min > $max) {
					throw new IllegalArgumentException(
						"\$max has to be greater than \$min"
					);
				}
				if (!is_anint($value) || ($value < $min) || ($value > $max)) {
					throw new IllegalArgumentException(
						"\$$varname",
						'{'.$min.', '.($min+1).', ... , '.$max.'}',
						$value
					);
				}
			}
		}

		return (int)$value;
	}

	/**
	 * Prüft, ob der übergebene Wert in dem Übergebenen Array enthalten ist.
	 *
	 * @param mixed $value					Der zu prüfende Wert
	 * @param string $varname				Name der Variable  (für die Ausgabe
	 * 										der Fehlermeldung.
	 * @param array $values					Das Array worin <code>$value</code>
	 * 										enthalten sein soll.
	 * @throws IllegalArgumentException		Wenn der zu testende Wert nicht den
	 * 										übergebenen Kriterien entspricht.
	 * @return mixed
	 */
	public static function values($value, $varname, array $values) {
		if (!in_array($value, $values)) {
			throw new IllegalArgumentException(
				"\$$varname", '{'.implode("}, {", $values) . '}'
			);
		}

		return $value;
	}

	/**
	 * Prüft, ob der übergebene Wert ein Bool  ist oder verlustfrei in ein Bool
	 * gecastet werden kann.
	 *
	 * Es werden somit neben TRUE und FALSE auch 1 und 0 akzeptiert.
	 *
	 * @param mixed $value					Der zu prüfende Wert
	 * @param string $varname				Name der Variable  (für die Ausgabe
	 * 										der Fehlermeldung.
	 * @throws IllegalArgumentException		Wenn der zu testende Wert nicht den
	 * 										übergebenen Kriterien entspricht.
	 * @return bool
	 */
	public static function bool($value, $varname) {
		if ($value === 'true')
			$value = true;
		elseif ($value === 'false') 
			$value = false;

		if (!is_bool($value) && ($value !== 1) && ($value !== 0))
			throw new IllegalArgumentException(
				"\$$varname should be boolean, $value given"
			);

		return (bool)$value;
	}

	/**
	 * Prüft,  ob es sich  bei dem  übergebenen Wert  um eine  gültige E -Mail-
	 * Adresse handelt.
	 *
	 * @param mixed $value					Der zu prüfende Wert
	 * @param string $varname				Name der Variable  (für die Ausgabe
	 * 										der Fehlermeldung.
	 * @param bool $strict					Wenn $strict FALSE ist, dann werden
	 * 										auch Werte der Form 'NAME <EMAIL>',
	 * 										z.B. 'Phil <p.kemmeter@gausus.de>',
	 * 										akzeptiert. Default ist TRUE.
	 * @throws IllegalArgumentException		Wenn der zu testende Wert nicht den
	 * 										übergebenen Kriterien entspricht.
	 * @return string
	 */
	public static function email($value, $varname, $strict=true) {
		if (!$strict && (($p1 = strpos($value, '<')) !== false)) {
			$tokens = explode('<', $value);
			if (// 1. Check: > enthalten und < vor >
				!($p2 = strpos($value, '>')) || ($p1 > $p2) ||
				// 2. Check: nur einmal < und vor < und danach ist auch Inhalt
				(count($tokens) != 2) || empty($tokens[0])
				|| empty($tokens[1]))
			{
				throw new IllegalArgumentException(
					"\$$varname should be an email address, $value given. ".
						"(No strict check)"
				);
			}

			$name = trim($tokens[0]);
			$tokens = explode('>', $tokens[1]);
			$tokens[1] = trim($tokens[1]);

			if (// 3. Check: nur einmal > und nach > kommt höchstens blank
				(count($tokens) != 2) || !empty($tokens[1])) {
				throw new IllegalArgumentException(
					"\$$varname should be an email address, $value given. ".
						"(No strict check)"
				);
			}
			$value = $tokens[0];
		}
		if (!($email = filter_var($value, FILTER_VALIDATE_EMAIL)))
			throw new IllegalArgumentException(
				"\$$varname should be an email address, $value given"
			);

		return isset($name) ? "$name <$email>" : $email;
	}

	/**
	 * Prüft, ob es sich bei dem übergebenen Wert um einen gültigen Mysql-Date-
	 * String handelt (YYYY-MM-DD).
	 *
	 * @param mixed $value					Der zu prüfende Wert
	 * @param string $varname				Name der Variable  (für die Ausgabe
	 * 										der Fehlermeldung).
	 * @param bool $zero_date_allowed       Ob 0000-00-00 erlaubt ist.
	 * @param string $min                   Kleinstes erlaubtes Datum.
	 * @param string $max                   Größtes erlaubtes Datum.
	 * @return string
	 */
	public static function mysqldate($value, $varname, 
		$zero_date_allowed=false, $min=false, $max=false) 
	{
		$value = self::string($value, $varname);
		if ($value == '0000-00-00') {
			if (!$zero_date_allowed) {
				throw new IllegalArgumentException(
					"\$$varname must not be 0000-00-00."
				);
			}
			else {
				return $value;
			}
		}
		
		if (!preg_match('/^\d\d\d\d-\d\d-\d\d$/', $value)) {
			throw new IllegalArgumentException(
				"\$$varname", 'YYYY-MM-DD', $value
			);
		}

		list($year, $month, $day) = explode('-', $value);
		if (!checkdate($month, $day, $year)) {
			throw new IllegalArgumentException(
				"\$$varname is no valid date ($value)"
			);
		}

		if ($min !== false) {
			$min = self::mysqldate($min, 'min');
			if (strcmp($min, $value) > 0) {
				throw new IllegalArgumentException(
					"\$$varname has to be at least $min, $value given"
				);
			}
		}

		if ($max !== false) {
			$max = self::mysqldate($max, 'max');
			if (strcmp($max, $value) < 0) {
				throw new IllegalArgumentException(
					"\$$varname has to be no greater than $max, $value given"
				);
			}
		}

		return $value;
	}

	/**
	 * Prüft, ob der angegebene Wert eine URL ist (absolut).
	 *
	 * @param string $value
	 * @param string $varname
	 * @return string
	 */
	public static function url($value, $varname) {
		$value = self::string($value, $varname);

		if (!preg_match('/^(http|ftp|https):\/\/[\w\-_]+(\.[\w\-_]+)+([\w\-\.,@?^=%&:\/~\+#]*[\w\-\@?^=%&\/~\+#])?$/', $value)) 
		{
			throw new IllegalArgumentException(
				"\$$varname is not a valid absolute URL, $value given"
			);
		}

		return $value;
	}

	/**
	 * Prüft, ob der angegebene Wert ein gültiger Hex-Color-Code ist.
	 *
	 * @param string $value
	 * @param string $varname
	 * @param bool $leading_hash Ob ein # vorangestellt werden muss oder 
	 *                           nicht darf.
	 * @return string
	 */
	public static function colhex($value, $varname, $leading_hash = false) {
		$value = self::string($value, $varname, false, 0, $leading_hash ? 7 : 6);
		
		if ($leading_hash && ($value{0} != '#')) {
			throw new IllegalArgumentException(
				"\$$varname should start with a #, $value given"
			);
		}

		for ($i = $leading_hash ? 1 : 0; $i < strlen($value); $i++) {
			if ((hexdec($value{$i}) == 0) && $value{$i} != 0) {
				throw new IllegalArgumentException(
					"\$$varname shall be a hex color code, $value given"
				);
			}
		}

		return $value;
	}
}
?>
