<?php
include_once(dirname(__FILE__).'/ExceptionWithErrorlog.inc');

/**
 * Repräsentiert die Fehlerfälle, welche derart kritisch sind, dass man die
 * Entwickler per Mail benachrichtigt werden sollen. Beispiele sind Design-Fehler
 * (ähnlich also assertions) oder andere Fehler, die theoretisch nie auftreten
 * können dürfen.
 * Zusätzlich wird alles geloggt. ExceptionWithErrormail ist somit eine 
 * Erweiterung von ExceptionWithErrorlog.
 * 
 * @author Philipp Kemmeter
 */

class ExceptionWithErrormail extends ExceptionWithErrorlog {
	/**
	 * Einziger Konstruktor der Klasse. Mit Erzeugen dieser Exception wird 
	 * automatisch eine Mail an die Leute geschickt, die übergeben wurden.
	 * Zusätzlich wird ein ähnlicher Eintrag im Log erstellt.
	 *
	 * @param string $message 	Nachricht, welche dem User zurückgegeben werden 
	 * 							soll. Es wird diese Nachricht dann nochmal Per 
	 * 							Mail verschickt inlusive zusätzlicher Daten.
	 * @param int $uid 			Die ID des Users, bei dem der Fehler auftrat
	 * @param string $report_to	Wer per Mail benachrichtigt werden soll
	 * @param string $topic		E-Mail-Topic
	 * @param string $additional_info	Dieser String wird NUR per Mail geschickt
	 * 									(der User sieht diesen Text NICHT)
	 */
	public function __construct($message, $uid, $report_to='All', 
		$topic='Error',	$additional_info='') 
	{
		parent::__construct($message, $uid, $additional_info);		
		errormail("The following Error occured to User with ID ".$uid.":\n".
			$message."\n\nAdditional:\n".$additional_info, $report_to, $topic);
	}
}


?>