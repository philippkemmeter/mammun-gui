<?php
require_once(dirname(__FILE__).'/ValueChecker.inc');

/**
 * Verwaltet einheitliche Timestamps
 *
 * @author Philipp Kemmeter
 */
class Time {
	const ONE_HOUR = 3600;
	const ONE_DAY = 86400;
	const TWO_DAYS = 172800;
	const THREE_DAYS = 259200;
	const FOUR_DAYS = 345600;
	const FIFE_DAYS = 432000;
	const SIX_DAYS = 518400;
	const ONE_WEEK = 604800;
	const TWO_WEEKS = 1209600;
	const THREE_WEEKS = 1814400;
	const FOUR_WEEKS = 2419200;

	static $t_now = -1;

	/**
	 * Gibt die gespeicherte aktuelle Uhrzeit zurück.
	 *
	 * Die Idee ist, dass wenn ein Skript mehrere Sekunden braucht, man
	 * innerhalb der Datei überall denselben Timestamp bekommt, wenn man
	 * ihn nicht bewusst neu setzt.
	 *
	 * @return uint
	 */
	static function get_t_now() {
		if (self::$t_now == -1) {
			self::$t_now = time();
			return self::$t_now;
		}
		return self::$t_now;

	}

	/**
	 * Setzt die gespeicherte aktuelle Uhrzeit.
	 *
	 * @param uint $t_now
	 */
	static function set_t_now($t_now) {
		ValueChecker::t_stamp($t_now, 't_now');
		self::$t_now = $t_now;
	}

	/**
	 * Statische Klasse, kein Konstruktor
	 */
	private function __construct() {}
}
?>
