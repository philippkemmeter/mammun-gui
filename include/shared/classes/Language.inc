<?php
@include_once (dirname(__FILE__).'/../../_config.inc');

/**
 * Verwaltet die Language-Packs
 */

class Language {
	private static $INSTANCE;
	private $language;

	/**
	 * @return Language
	 */
	public static function get_instance() {	// singleton
		if (!self::$INSTANCE)
			self::$INSTANCE = new Language();
		return self::$INSTANCE;
	}
	private function __construct() {
		$this->detect_language();
	}

	/**
	 * @global $LNG
	 * @return unknown_type
	 */
	private function detect_language() {
		global $LNG;
		$lng = false;

		if (isset ($_REQUEST['lang'])) {
			$lng = ($_REQUEST['lang'] != 'en') ? 'de' : 'en';
		}
		if (!isset ($_SESSION))
			session_start();

		if (!$lng) {
			//TODO: secure_session_start verwenden! Dazu müssen die Funktionen zentral ausgelagert sein.
			//		Dies hier ist eine potentielle Sicherheitslücke, da es secure_session_start() umgeht!
			if (isset($_SESSION['Language']['language'])) {
				$lng = ($_SESSION['Language']['language'] != 'en') ? 'de' : 'en';
			} else {
				$lng = 'de';
			}
		}

		$_SESSION['Language']['language'] = $lng;

		$this->language = $lng;
		$LNG = $lng;
	}

	/**
	 * Lädt die Sprachkonstanten, die dem übergebenen Schlüssel zugeordnet ist.
	 * Der Schlüssel ist dabei (der Naming-Convention folgend) der Name der Datei,
	 * die die Sprachdatei anfordert, ohne die letzte Dateiendung (also z.B.
	 * 'science' oder 'science.tmpl' (jeweils das .inc weggelassen).
	 *
	 * @param string $key	Schlüssel, unter welchem die Sprachkonstanten zu finden sind
	 */
	public function load_language_file($key) {
		// Sprachdateien sind prinzipiell aus Nutzersicht ini-Dateien, die
		// Variable="Wert"-Zuordnungen enthalten. Aus diesen Ini-Dateien werden
		// dann inc-Dateien, welche die defines enthalten und nur noch inkludiert
		// werden müssen (wodurch der ini-Parsing-Schritt zukünftig entfällt).
		// Wird die Ini-Datei geändert oder existiert noch keine inc-Datei, wird
		// die inc-Datei (neu) generiert.
		$inc_file = CONF_LANGUAGE_FILE_DIR.'/'.$this->language.'/inc/'.
				$key.'.'.$this->language.'.inc';
		$ini_file = CONF_LANGUAGE_FILE_DIR.'/'.$this->language.'/ini/'.
				$key.'.'.$this->language.'.ini';
		// Fall 1: ini-Datei existiert NICHT
		//         => keine generierung möglich, versuchen inc-Datei zu laden
		if (!file_exists($ini_file)) {
			if (!file_exists($inc_file)) {
				echo "<b>Warning</b>: Language file for $key could not be loaded.<br\>\n";
				echo $ini_file.'<br/>\n';
				return;	// Wir sind fertig!
			}
		}
		// Fall 2: ini-Datei existiert und inc_datei ist entweder nicht da, oder
		//         inc-Datei ist älter als ini-Datei => neu generierung von inc-Datei
		else if (!file_exists($inc_file)
						|| filemtime($inc_file) < filemtime($ini_file))
		{
			$this->create_lng_file_from_ini($ini_file, $inc_file);
			if (!file_exists($inc_file)) {
				echo "<b>Warning</b>: Generation of language file for $key failed.<br\>\n";
				echo "<b>Warning</b>: Language file for $key could not be loaded.<br\>\n";
				return;	// Wir sind fertig!
			}
		}
		// Fall 3: alles sauber, wir können die inc-Datei mit bestem Gewissen includen
		// => wir müssen nichts tun, da wir jetzt sowieso eine schöne inc-Datei haben
		// und diese nun includen :)
		include_once ($inc_file);
	}

	public function set_var_value_of_lng_file($key, $varname, $value) {
		$inc_file = CONF_LANGUAGE_FILE_DIR.'/'.$this->language.'/inc/'.
				$key.'.'.$this->language.'.inc';
		$ini_file = CONF_LANGUAGE_FILE_DIR.'/'.$this->language.'/ini/'.
				$key.'.'.$this->language.'.ini';

		$value = str_replace("\n", '\n'."\n\t", $value);

		if (file_exists($ini_file)) {
			$ini_contents = parse_ini_file($ini_file);
		}
		else
			$ini_contents = null;

		$ini_contents[$varname] = $value;

		$res = array();
	    foreach($ini_contents as $k => $val)
	    {
	        $res[] = "$k = ".(is_numeric($val) ? $val : '"'.$val.'"');
	    }

		if ($fp = fopen($ini_file, 'w'))
	    {
	        $startTime = microtime();
	        do
	        {            $canWrite = flock($fp, LOCK_EX);
	           // If lock not obtained sleep for 0 - 100 milliseconds, to avoid collision and CPU load
	           if(!$canWrite) usleep(round(rand(0, 100)*1000));
	        } while ((!$canWrite)and((microtime()-$startTime) < 1000));

	        //file was locked so now we can store information
	        if ($canWrite)
	        {            fwrite($fp, implode("\r\n", $res));
	            flock($fp, LOCK_UN);
	        }
	        fclose($fp);
	    }
	}

	/**
	 * Generiert eine include-Sprachdatei aus einer ini-Datei heraus.
	 *
	 * @param string $ini_file_name		ini-Datei, worin die Informationen stehen
	 * @param string $inc_file_name		inc-Datei, wohinein geschrieben werden soll
	 */
	private function create_lng_file_from_ini($ini_file_name, $inc_file_name) {
		$ini_contents = parse_ini_file($ini_file_name);
		$inc_file_str = "<?\n";	// Dateiinhalt der neuen .inc-Datei
		foreach ($ini_contents as $key => $value) {
			$value =
				str_replace(
					'<br/> ', '<br/>',
				preg_replace(
					'/^\s/', '',
				preg_replace(
					'/\s\s+/', ' ',
					str_replace(
						array("\t", "\n", "\r", '\n', " - ", "...", "[i]", "[/i]",
							"[b]", "[/b]", ",,", "''", "'", '"',
							"[list]", "[/list]"),
						array(' ', '', '', '<br/>', " – ", "…", "<i>", "</i>",
							"<b>", "</b>", "„", "“", "’", "&quot;",
							"<ul>", "</ul>"),
						preg_replace(
							'/([a-zA-Z]+)(\d+)/', '$1<sub>$2</sub>',
							preg_replace(
								'/\[\*\](.*)/', '<li>$1</li>',
								htmlspecialchars($value, ENT_NOQUOTES)
							)
						)
					)
				)
				)
				);
			$inc_file_str .= "define ('LNG_".strtoupper($key)."', '".$value
				."');\n";
		}
		$inc_file_str .= "?>";
		//$inc_file_name = basename($inc_file_name);
		if (!($handle = fopen($inc_file_name, 'w'))) {
			echo "<b>Error</b>: Could not open ".$inc_file_name." for writing.<br>\n";
			return;
		}
		if (!fwrite($handle, $inc_file_str)) {
			echo "<b>Error</b>: Could not write into ".basename($inc_file_name)."<br>\n";
			fclose($handle);
			return;
		}
		fclose($handle);
	}

	public function get_language() {
		return $this->language;
	}

	public function set_language($lng) {
		$_SESSION['Language']['language'] = $lng;

		$this->language = $lng;
		$LNG = $lng;
	}
}
?>
