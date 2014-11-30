<?php

/**
 * Konstantendefinition für den EventLog
 *
 * Die Codes sind alles Bits, damit man komprimiert Filter in die DB schreiben
 * kann. Deswegen kann eine Gruppe nicht mehr als 64 Codes enthalten, da es
 * keine größeren Blöcke gibt, als welche mit 64 Bit.
 * Zudem sollte alles in 8, 16, 24, 32 oder 64er Gruppen eingeteilt werden, da
 * das die Mysql-Blockgrößen sind.
 *
 * @author Philipp Kemmeter
 */
interface IEventLog {
	/* Game-Engine-Events: [63 Codes] */
	const TYPE_GAME_ENGINE_MIN = 0;

	/** Forschung fertig */
	const TYPE_SC_FINISHED = 1;
	/**
	 * Einladung auf Insel ist fehlgeschlagen, weil z.B. die Insel schon
	 * voll war. Der Spieler wird dann darüber informiert, dass der Spieler,
	 * mit dem er eigentlich spielen wollte, nicht da ist, aber besucht werden
	 * kann.
	 */
	const TYPE_FIRST_MAP_INVIT_FAILED = 2;
	/**
	 * Besuch mit visit-Flag ist fehlgeschlagen, weil der Nutzer z.B. nicht
	 * exisitert.
	 */
	const TYPE_VISIT_MAP_FAILED = 3;

	/** Training eines Kämpfers beendet */
	const TYPE_TRAINING_FINISHED = 10;
	/** Training eines Forschers beendet */
	const TYPE_TRAINING_FINISHED_SCIENTIST = 11;

	/** Einladung von einem Spieler auf eine Insel erhalten */
	const TYPE_MAP_INVITATION_RECEIVED = 15;
	/** Einladung auf meine Insel wurde erfragt */
	const TYPE_MAP_INVITATION_REQUEST = 16;

	/** Gebäude wurde von anderem angemalt */
	const TYPE_BUILDING_HAS_BEEN_PAINTED = 17;
	/** User hat sich für das Anmalen bedankt */
	const TYPE_THANKS_FOR_PAINTED_BUILDING = 18;
	/** Nutzer hat meine Farbe auf seinem Gebäude übermalt oder abgewaschen */
	const TYPE_REPAINTED_HIS_MY_COLORED_BUILDING = 19;

	/** Gebäude fertiggestellt */
	const TYPE_CONSTRUCTION_COMPLETED = 20;
	/** Upgrade eines Gebäudes abgeschlossen */
	const TYPE_UPGRADE_COMPLETED = 21;
	/** Geschenk für das letzte Level-Up erhalten (XP-Gebäude) */
	const TYPE_LEVELUP_PRESENT_GAINED = 22;
	/** Level-Up-Geschenk nachträglich erhalten (gab's früher noch nicht) */
	const TYPE_LEVELUP_PRESENT_GAINED_OLD = 23;

	/** Kleidung von (gestorbener) Einheit wurden in Schatzkiste gelegt */
	const TYPE_CLOTHES_OF_UNIT_GAINED = 33;

	/** Mein Sektor wurde angegriffen */
	const TYPE_SECTOR_ATTACKED = 50;	// TODO
	/** Kampf um Sektor gewonnen */
	const TYPE_SECTOR_BATTLE_WON = 51;
	/** Kampf um Sektor verloren */
	const TYPE_SECTOR_BATTLE_LOST = 52;
	/** Gegner hat Nachschub geschickt in einen meiner umkämpften Sektoren */
	const TYPE_REINFORCEMENTS_SENT = 53;// TODO
	/** Ich habe eine Einheit vom Gegner gekillt */
	const TYPE_TRIBE_KILLED = 54;
	/** Eine meiner Einheiten ist gestorben */
	const TYPE_TRIBE_DIED = 55;
	/** Ich habe einen Sektor kampflos erobert (weil er leer war) */
	const TYPE_SECTOR_WON_NO_BATTLE = 56;

	/** Osterei im Dunkel gefunden */
	const TYPE_FOUND_EASTER_EGG_IN_DARK = 60;
	/** Nutzer ist der Einladung von Spieler gefolgt */
	const TYPE_USER_RECRUITED = 61;

	/** Alle Gegner wurden ausgelöscht, ich habe gewonnen */
	const TYPE_ALL_ENEMIES_DEFEATED = 63;
	const TYPE_GAME_ENGINE_MAX = 63;

	/* GAP für weitere Cods, insgesamt noch Platz für 32 */

	/* Payment-Events: [16 Codes] */
	const TYPE_PAYMENT_MIN = 96;

	/** Klunker wurden erfolgreich aufgeladen */
	const TYPE_KLUNKER_RECEIVED = 100;	// TODO
	/** Mein Angebot wurde akzeptiert */
	const TYPE_TRADE_ACCEPTED = 101;
	/** Mein Angebot wurde abgelehnt */
	const TYPE_TRADE_DENIED = 102;
	/** Ich habe ein Geschenk erhalten */
	const TYPE_GIFT_RECEIVED = 103;
	/** Ich habe ein Item direkt verkauft */
	const TYPE_ITEM_SOLD_DIRECTLY = 104;

	const TYPE_PAYMENT_MAX = 111;
	/* GAP für weitere Codes, insgesamt noch Platz für 80 */

	/* Special Events, that are commands to the client to do something [16]*/
	const TYPE_SPECIAL_MIN = 192;
	/**
	 * Der Farbwahl-Screen soll mit Einladungsoptionen geöffnet werden. Der
	 * Text des Screens ist dann auch anders, und zwar geht es jetzt nicht
	 * mehr um die Farbwahl, sondern darum, dass man Leute einlädt. Nur
	 * optisch ist der Screen derselbe.
	 */
	const TYPE_OPEN_COL_CHOOSE_DIALOG_INVITE = 198;
	/** Der Farbwahl-Screen soll geöffnet werden. */
	const TYPE_OPEN_COL_CHOOSE_DIALOG = 199;
	/** Der Einladungsdialog soll geöffnet werden */
	const TYPE_OPEN_INVITATION_DIALOG = 200;
	const TYPE_SPECIAL_MAX = 207;

	/* GAP für weitere Codes, insgesamt noch Platz für 32 */

	/* Special Events, that are represented by normal Popups [16 Codes]*/

	const TYPE_SPECIAL_POPUPS_MIN = 240;
	/** Der Spieler wird aufgefordert FB-Share zu benutzen */
	const TYPE_USE_FACEBOOK_SHARE = 253;
	/** Es gibt eien neuen Katalog. Der Spieler bekommt ein Inform-Popup */
	const TYPE_NEW_CATALOG = 254;
	/** Eine generelle Ankündigung kann hierüber verbreitet werden */
	const TYPE_SHOW_ANNOUNCEMENT = 255;
	const TYPE_SPECIAL_POPUPS_MAX = 255;
}