<?php
require_once(dirname(__FILE__).'/Language.inc');

/**
 * Diese Klasse hilft dabei, bestimmte Ausgaben für verschiedene Sprachräume
 * so, wie es in diesen Sprachräumen für gewöhnlich üblich ist, darzustellen.
 *
 * @author Philipp Kemmeter
 */
class LanguagePrint {
	const FULL_WORD = 1;
	const SHORT_WORD = 2;
	const NUMBER = 3;
	const NUMBER_ZEROFILL = 4;

	private function __construct() {
	}

	/**
	 * Gibt die Fließkommazahl entsprechend der Sprache des Nutzers zurück.
	 *
	 * @param float $value     Zu formatierende Fließkommazahl
	 * @param uint $dec_places Anzahl Nachkommastellen
	 * @return string
	 */
	public static function float($value, $dec_places=10) {
		$value = ValueChecker::float($value, 'value');
		$dec_places = ValueChecker::int($dec_places, 'dec_places', 0);

		if ($dec_places == 0)
			return (string)((int)$value);

		$p = pow(10, $dec_places);

		$value = (string)(floor($value * $p) / $p);

		switch (Language::get_instance()->get_language()) {
			case 'de':
				$value = str_replace('.', ',', $value);
				break;
		}

		return $value;
	}

	/**
	 * Gibt einen Zeitraum in Tagen gerundet zurück.
	 *
	 * Beispielsweise "3 days".
	 * Wird der optionale Parameter $add_ago auf TRUE gesetzt, dann wird
	 * "3 days ago" bzw "vor 3 Tagen" usw zurückgegeben
	 *
	 * @param uint $value   Zeitraum in Sekunden.
	 * @param bool $add_ago Ob "vor X Tagen" / "X days ago" ausgegeben werden
	 *                      soll.
	 * @return string
	 */
	public static function duration_as_days($value, $add_ago = false) {
		$value = ValueChecker::int($value, 'value', 0);
		$add_ago = ValueChecker::bool($add_ago, 'add_ago');

		$value = floor($value / 86400);

		switch (Language::get_instance()->get_language()) {
			case 'de':
				if ($add_ago)
					$value = "vor $value Tagen";
				else
					$value = "$value Tage";
				break;
			default:
				if ($add_ago)
					$value = "$value days ago";
				else
					$value = "$value days";
		}

		return $value;
	}

	/**
	 * Gibt ein Array als Aufzählung zurück.
	 *
	 * Dabei wird alles so formatiert, dass man es innerhalb eines Satzen 
	 * nutzen kann, also bsp:
	 * Hund, Katze und Maus
	 *
	 * Sprich: Komma trennt alle bis auf das letzte Argument, welches dann
	 * je nach Sprachraum mit 'und' oder 'and' o.ä. angebunden wird
	 */
	public static function listing(array $values) {
		if (count($values) == 0)
			return '';
		if (count($values) == 1)
			return $values[0];

		$v = array();
		for ($i = 0, $n = count($values) - 1; $i < $n; ++$i)
			$v[] = $values[$i];

		$result = implode(', ', $v);

		$v = $values[count($values)-1];

		switch (Language::get_instance()->get_language()) {
			case 'de':
				$result .= ' und ' . $v;
				break;
			case 'fr':
				$result .= ' et ' . $v;
				break;
			default:
				$result .= ' and ' . $v;;
		}

		return $result;

	}

	/**
	 * Gibt das Datum entsprechend der Sprache aus.
	 *
	 * @param uint $value      Unix-Timestamp des Datums
	 * @param uint $day_type   Typ der Tagausgabe. Gültige Werte sind ::NUMBER
	 *                         ::NUMBER_ZEROFILL.
	 * @param uint $month_type Typ der Monatsausgane. Gültige Werte sind
	 *                         ::FULL_WORD, ::SHORT_WORD, ::NUMBER, 
	 *                         ::NUMBER_ZEROFILL.
	 * @return string
	 */
	public static function date($value, $day_type = self::NUMBER_ZEROFILL,
		$month_type = self::NUMBER_ZEROFILL)
	{
		$value = ValueChecker::t_stamp($value, 'value');
		$day_type = ValueChecker::values(
			$day_type, 'day_type',
			array(self::NUMBER, self::NUMBER_ZEROFILL)
		);
		$month_type = ValueChecker::values(
			$month_type, 'month_type',
			array(
				self::FULL_WORD, self::SHORT_WORD, self::NUMBER,
				self::NUMBER_ZEROFILL
			)
		);

		if ($day_type == self::NUMBER)
			$day_type = 'j';
		else
			$day_type = 'd';


		if (($month_type == self::NUMBER) 
			|| ($month_type == self::NUMBER_ZEROFILL))
		{
			if ($month_type == self::NUMBER)
				$month_type = 'n';
			else 
				$month_type = 'm';
		
			switch(Language::get_instance()->get_language()) {
				case 'de':
					$value = date("$day_type.$month_type.Y", $value);
					break;
				default:
					$value = date("$month_type-$day_type-Y", $value);
			}
		}
		else {
			if ($month_type == self::SHORT_WORD)
				$month_type = 'M';
			else
				$month_type = 'F';

			switch(Language::get_instance()->get_language()) {
				case 'de':
					$value = date("$day_type. $month_type Y", $value);
					$value = str_replace(
						array(
							'January', 'February', 'March', 'May', 'June',
							'July', 'October', 'December'
						),
						array(
							'Januar', 'Februar', 'März', 'Mai', 'Juni',
							'Juli', 'Oktober', 'Dezember'
						),
						$value
					);
					break;
				default:
					$value = date("$month_type $date, Y", $value);
			}
		}

		return $value;
	}
}
?>
