<?php
/**
 * Dieses Interface beschreibt classen, welche ein POST-Paket repräsentieren zum
 * Zahlen.
 * 
 * @author Philipp Kemmeter
 */
interface IPOSTPayment {
	/**
	 * Gibt die Instanz des Users zurück (SINGLETON)
	 * @param uint $uid
	 * @return unknown_type
	 */
	public static function get_instance($uid);
	
	/**
	 * Setzt die URL, welche angesurft werden soll, wenn das externe Payment
	 * erledigt ist.
	 * @param string $url
	 * @throws IllegalArgumentException
	 */
	public function set_response_url($url);
	
	/**
	 * Setzt die Zahlungsinformationen.
	 * 
	 * @param ufloat $payment_amount		Wie viel Geld
	 * @param string $payment_usage			Was gekauft wurde
	 * @param string $identification_transactionid	TransaktionsID
	 * @param string $payment_currency		Währung (DEFAULT: EUR)
	 */
	public function set_payment_information($payment_amount, $payment_usage,
		$identification_transactionid, $payment_currency='EUR');
	
	/**
	 * Nach Konfiguration des kompletten POST-Pakets kann dieses dann jetzt
	 * abgeschickt werden.  Die Rückgabe der Funktion hängt von der 
	 * Zahlungsmethode und somit von der Klasse ab, die diese Schnittstelle
	 * implementiert.
	 * 
	 * @return string[]
	 */
	public function commit_post_payment();
}
?>