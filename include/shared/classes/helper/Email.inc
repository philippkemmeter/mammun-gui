<?php
/**
 * File created by Philipp Kemmeter on 05.02.2009 at 18:24:58
 */

/**
 * Email-Funktionen.
 *
 * @author Philipp Kemmeter
 */
class Email {
	private function __construct() {}

	/**
	 * Gibt zurück, ob der übergebene String eine E-Mail-Adresse darstellt.
	 *
	 * @param string $str
	 * @return bool
	 */
	public static function validate_email_string($str) {
		$str = strtolower($str);
		$at_pos = strpos($str, '@');
		$point_pos = strpos($str, '.');
		$at_pos2 = strrpos($str, '@');
		$point_pos2 = strrpos($str, '.');
		if ($at_pos !== $at_pos2) {
			return false;
		}
		if (!$at_pos || !$point_pos) {
			return false;
		}
		if ($at_pos >= $point_pos2) {
			return false;
		}
		if (preg_match("/[^a-z0-9_.@+-]/", $str)) {
			return false;
		}
		$domain = substr($str, $at_pos+1, $point_pos2-$at_pos-1);
		$name = substr($str, 0, $at_pos);
		$tld = substr($str, $point_pos2+1);
		if (strlen($tld) < 2)
			return false;
		if (strlen($domain) < 2)
			return false;
		if (strlen($name) < 2)
			return false;

		return true;
	}
}
?>