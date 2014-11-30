<?php
/**
 * Alle Klassenkonstanten, die mit Medallien zusammenhängen
 *
 * @author Philipp Kemmeter
 */

interface IMedals {
	/**
	 * Wenn eine spezifische Forschung erforscht wird (amount ist dann das
	 * erreichte Level, subtype die SC-ID)
	 * oder wenn allgemein eine bestimmte Anzahl von Forschungsstufen erreicht
	 * wurden.
	 */
	const TYPE_SCIENCE_RESEARCHED_LEVEL = 1;
	/**
	 * Wenn die x. Forschung eines Typs oder die x. Forschung insgesamt
	 * erforscht wurde. (also z.B. die 10. Ability). Amount ist dann X, NICHT
	 * das Level, subtype ist die Kategorie. Die Numerischen Codes hält die
	 * Techtree-Klasse.
	 */
	const TYPE_SCIENCE_RESEARCHED_CATEGORY = 2;
	/**
	 * Wird ausgelöst, wenn ein Gebäude errichtet wird. subtype ist hier der
	 * Typ des Gebäudes, amount die Anzahl
	 */
	const TYPE_BUILDING_CONSTRUCTED = 3;
	/**
	 * Wird ausgelöst, wenn ein Sektor erfolgreich nach reinmarschieren
	 * übernommen wurde. Subtype dann die User-Gruppe, die man besiegt hat:
	 * 0: alle
	 * 1: NOBODY
	 * 2: normaler User
	 * 3: Host-User (ist gleich definiert)
	 * (s. MapCompute-ATTDEF-Konstanten)
	 */
	const TYPE_ATTACK_SUCCESSFUL = 4;
	const TYPE_DEFEND_SUCCESSFUL = 5;	// TODO
	/**
	 * Nach dem Training der Einheit Nr. amount vom Typ subtype (oder bei 0
	 * insgesamt) bekommt man diesen Orden
	 */
	const TYPE_UNITS_TRAINED = 6;
	/**
	 * Für das Aufladen von Klunkern
	 */
	const TYPE_KLUNKER_BOUGHT = 8;	// TODO
	/**
	 * Für Handelsaktivitäten
	 */
	const TYPE_TRADE_ACTIVITY = 9;	// TODO
}
?>