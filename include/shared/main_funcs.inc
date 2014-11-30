<?
/**
 * Diese Datei beinhaltete alle Funktionen, die allgemein nützlich sind.
 *
 * @author Philipp Kemmeter
 * @package gausus_fw
 */

function rush_hour()
{
	$hour=date('H',Time::get_t_now());
	if ((($hour>6)&&($hour<9))||(($hour>16)&&($hour<22)))
		return (mysql_result(query(
			'SELECT COUNT(*) FROM users WHERE last_login>'.
				(Time::get_t_now()-60)),0)>10
		); //mehr als 5 user on //return (rand(0,10)>2);
	else
		return 0;
}

/**
 * Prüft, ob der Wert ein Integer ist  oder verlustlos zu einem Iteger gecastet
 * werden kann.
 *
 * Das bedeutet, dass sowohl Integer, die als Integer angelegt wurden, als auch
 * ganzzahlige Werte in einem String oder Float hier TRUE zurückgeben.
 *
 * Booleans geben FALSE zurück.
 *
 * @param mixed $value
 * @return bool
 */
function is_anint($value) {
	return ((is_int($value))
		|| (is_float($value) && $value === (float)(int)$value)
		|| (is_string($value) && $value === (string)(int) $value));
}

/**
 * Prüft,  ob der Wert ein  Float ist  oder verlustlos zu  einem Float gecastet
 * werden kann.
 *
 * Das bedeutet, dass  sowohl Float,  die als Float  angelegt wurden,  als auch
 * ganzzahlige Werte hier TRUE zurückgeben.
 *
 * Booleans geben FALSE zurück.
 *
 * @param mixed $value
 * @return bool
 */
function is_anfloat($value) {
	return ((is_float($value))
		|| (is_string($value) && $value === (string)(float) $value)
		|| is_anint($value));
}

function abbr_number($value) {
	if (abs($value) > 1000000)
		return ((int)($value / 100000) / 10).'M';
	else if (abs($value) > 1000)
		return ((int) ($value / 100) / 10).'k';
	else
		return $value;
}

function string_decode($string)
{
	$to_find = array("&#32;", "&#33;", "&deg;", "&quot;", "&sect;", "&#36;", "&#37;", "&#47;", "&#40;", "&#41;", "&#61;", "&#63;", "&acute;", "&#96;", "&#92;", "&szlig;", "&#125", "&#93;", "&#91;", "&#123", "&sup3;", "&sup2;", "&#64;", "&#43;", "&#42;", "&#126;", "&#39;", "&#45;", "&lt;", "&gt;", "&#124;", "&auml;", "&ouml;", "&uuml;", "&Auml;", "&Ouml;", "&Uuml;", "&euro;", "&amp;");
	$to_replace = array(" ", "!", "°", "\"", "§", "$", "%", "/", "(", ")", "=", "?", "´", "`", "\\", "ß", "}", "]", "[", "{", "³", "²", "@", "+", "*", "~", "'", "-","<", ">", "|", "ä", "ö", "ü", "Ä", "Ö", "Ü", "?", "&");

	$string = str_replace($to_find,$to_replace,$string);  // Sonderzeichen (evtl. gefährlich) ersetzen

	return $string;
	//      $to_find = array ("&#32;");
	//      $replace_with = array (" ");
	//      return(html_entity_decode(str_replace($to_find,$replace_with,$string),ENT_QUOTES));
}

/**
 *  returns if it is chrismas time (till 26th of december)
 */
function is_chrismas_time($t=false) {
	if (!$t)
	$t = time();
	$m = (int)date('m', $t);
	$d = (int)date('d', $t);
	return (($m == 12) && ($d <= 26));
}

/**
 * Gibt zurück, ob heute Ostern ist. Die Osterzeit ist von Ostermontag bis
 * Ostersonntag.
 *
 * @return bool
 */
function is_easter($t=false) {
	if (!$t)
		$t = time();
	$easter_start = easter_date((int)date('Y', $t));
	$easter_end = $easter_start + 172800 /*24*2*3600*/;

	return (($t >= $easter_start) && ($t <= $easter_end));
}

function pic_path($pic_name) {
	if (substr($pic_name, 0, 4) == '/var') {
		$pic_name = str_replace('/var/www/www.project-virus.de/htdocs', '..', $pic_name);
		return $pic_name;
	}
	if (substr($pic_name,0,7)=='http://')
		return ($pic_name);
	else
	{
		//     if (!User::get_current_user()->get_pic_path())
		return ('../pix/'.$pic_name);
		//  else
		//      return (User::get_current_user()->get_pic_path().$pic_name);
	}

}

/**
 * @todo rename to array_merge_recursive
 */
function my_array_merge_recursive ($arr,$ins) {
	if(is_array($arr)) {
		if(is_array($ins)) {
			foreach($ins as $k=>$v){
				if(isset($arr[$k]) && is_array($v) && is_array($arr[$k])) {
					$arr[$k] = my_array_merge_recursive($arr[$k],$v);
				}
				else {
					while (isset($arr[$k]))
					$k++;
					$arr[$k] = $v;
				}
			}
		}
	}
	elseif(!is_array($arr)&&(strlen($arr)==0||$arr==0))
	{
		$arr=$ins;
	}
	return $arr;
}

function my_array_flatten($a,$f = array()) {
	if(!$a || !is_array($a))
		return '';
	foreach($a as $k=>$v){
		if(is_array($v))
			$f = my_array_flatten($v, $f);
		else
			$f[] = $v;
	}
	return $f;
}

function random_value_key_pair(array $array) {
	$rand=rand(1,count($array));
	for ($i=0;$i<$rand;$i++)
		$random=each($array);
	return $random;
}

function random_value($array)
{
	shuffle($array);
	reset($array);
	$random=each($array);
	return($random['value']);
}

function random_key($array)
{
	$rand=rand(1,count($array));
	for ($i=0;$i<$rand;$i++)
		$random=each($array);
	return($random['key']);
}




function check_string($string,$space_not_allowed=0) {
	if ($space_not_allowed)
		$string = str_replace(" ", '', $string);
	/*$to_find =    array("&", "°", "\"", "§", "$", "%", "/", "(", ")", "=", "?", "´", "`", "ß", "}", "]", "[", "{", "³", "²", "+", "*", "~", "'", "<", ">", "|", "ä", "ö", "ü", "Ä", "Ö", "Ü", "?");
	 $to_replace = array("&amp;", "&deg;", "&quot;", "&sect;", "&#36;", "&#37;", "&#47;", "&#40;", "&#41;", "&#61;", "&#63;", "&acute;", "&#96;", "&szlig;", "&#125", "&#93;", "&#91;", "&#123", "&sup3;", "&sup2;", "&#43;", "&#42;", "&#126;", "&#39;", "&lt;", "&gt;", "&#124;", "&auml;", "&ouml;", "&uuml;", "&Auml;", "&Ouml;", "&Uuml;", "&euro;");

	 $string = str_replace($to_find,$to_replace,$string);  // Sonderzeichen (evtl. gefährlich) ersetzen*/
	$string = str_replace(
		array("&", '\'', '"', '<', '>', '\\', '/', '#'),
		array('＆', '’', '“','＜', '＞', '＼', '／', '＃'),
		$string
	);
	/*if (!$space_not_allowed)
	 {
		$to_find = array (" ");
		$replace_with = array ("&#32;");
		$string = str_replace($to_find,$replace_with,$string);  // Spaces ersetzen
		}*/


	if (preg_match_all('/[\'"&<>]/',
		$string, $matches))
	{// gefährliche Sonderzeichen rauslöschen
		$string = str_replace($matches[0], '', $string);
	}

	return $string;
}


function secInTime($t)
{

	/***********************************************************************************
	 formatiert zeitangaben in Sekunden in $hour:$min:$sec
	 ***********************************************************************************/


	$t = $t + 3600;		//workaround für...klappt nur für deutsche Winterzeit
	$day = floor ($t/86400);
	$t-=$day*86400;
	$hour = floor($t / 3600);
	$min = floor(($t - $hour*3600) / 60);
	$sec = floor($t - $hour*3600 - $min*60);

	if ($sec < 10) { $sec = "0".$sec; }
	if ($min < 10) { $min = "0".$min; }
	if ($hour < 10) { $hour = "0".$hour; }

	return ($day?$day.' d, ':'').$hour.":".$min.":".$sec;
}


function translate_date($date_str, $language='de') {
	if ($language === 'de') {
		$search = array('Monday', 'Tuesday', 'Wednesday', 'Thursday', 'Friday', 'Saturday', 'Sunday');
		$replace = array('Montag', 'Dienstag', 'Mittwoch', 'Donnerstag', 'Freitag', 'Samstag', 'Sonntag');
		return str_replace($search, $replace, $date_str);
	}
	else
		return $date_str;
}
?>
