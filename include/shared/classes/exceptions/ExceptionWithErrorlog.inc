<?php
/**
 * Diese Klasse erzeugt eine Exception, welche zusätzlich im Logbuch gesichert
 * werden. Es ist möglich, zusätzliche Informationen zu übergeben, die NUR
 * geloggt werden sollen und somit nicht in $message enthalten sind (und der
 * User somit niemals zu Gesicht bekommen kann).
 * Diese Exception ist für entsprechende kritische Fehler gedacht, die debuggt
 * werden müssen.
 *
 * @author Philipp Kemmeter
 */
class ExceptionWithErrorlog extends Exception {
	protected $uid;

	/**
	 * Einziger Konstruktor der Klasse. Mit Erzeugen dieser Exception wird
	 * automatisch ein errorlog-Eintrag erstellt.
	 *
	 * @param string $message 	Nachricht, welche dem User zurückgegeben werden
	 * 							soll. Es wird diese Nachricht dann nochmal
	 * 							inklusive weiterer Daten geloggt.
	 * @param int $uid 			Die ID des Users, bei dem der Fehler auftrat
	 * @param string $additional_info	Dieser String wird NUR geloggt geschickt
	 * 									(der User sieht diesen Text NICHT)
	 * @param int $code
	 */
	public function __construct($message, $uid, $additional_info='', $code=0)
	{
		parent::__construct($message, $code);
		$this->uid = $uid;
		error_log("In {$this->getFile()} line {$this->getLine()}: The ".
			"following Error occured to User with ID ".$uid.": ".
			$message."\n  Additional: ".$additional_info);
	}

	/**
	 * Gibt die ID des Users zurück, dem der Fehler passiert ist
	 *
	 * @return int die UID
	 */
	public function getUid() {	// an der Namingconvention von Exception angelehnt
		return $this->uid;
	}
}
?>