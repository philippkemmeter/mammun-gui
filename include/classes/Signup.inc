<?php
/**
 * File created by Philipp Kemmeter on 05.02.2009 at 17:36:00
 */

require_once(dirname(__FILE__).'/../shared/classes/ISignup.inc');
require_once(dirname(__FILE__).'/../shared/classes/server_com/ClientServer.inc');

/**
 * Signup ist eine rein statische Klasse zur Erstellung eines neuen Users auf
 * dem Server. Dabei wird natürlich eine Serverbindung hergestellt.
 *
 * Im Endeffekt ist diese Klasse nur ein Wrapper für die Serververbindung :)
 *
 * @author Philipp Kemmeter
 */
class Signup implements ISignup {
	private function __construct() {}

	/**
	 * Fügt einen neuen User ins Spiel ein. Wrapper-Funktion für die Server-
	 * Funktionalität der Userregistrierung
	 *
	 * @param string $nick		Nickname
	 * @param string $email		Email-Adresse
	 * @param string $firstname	Vorname
	 * @param string $surname	Nachname
	 * @param string $recruit	Der Name des Users, der ihn geworben hat
	 * @param bool $news		Ob Newsletter bestellt wird oder nicht
	 * @param string $pass		Gewünschtes Passwort
	 * @param string $activation_url
	 * @param string $map_key	Der Schlüssel, der ihn direkt auf eine Karte
	 * 							setzen kann (s. MapChooserPrivate)
	 * @return string[2] 		Array. Erstes Element enhält den gesetzten Nick,
	 * 							zweites das gesetzte Passwort (kann beides
	 * 							vom gewünschten abweichen)
	 * @throws Exception	Wenn irgendwelche Angaben falsch
	 */
	public static function register($nick, $email, $firstname, $surname,
		$recruit, $news, $pass, $activation_url, $fb_id=0, $map_key='',
		$lang='de', $visit='')
	{
		list($nick, $pass) = Server::get_instance(CONF_ENGINE_SERVER_URL)
			->cmd_register_user(
				$nick, $email, $firstname, $surname, $recruit,
				$news, $pass, $fb_id, $activation_url, $map_key, $lang, $visit
			);
		ClientServer::send_request(
			CONF_WCF_SERVER_URL."/gausus_wcf_hacks/register_interface.php?".
			"name=$nick&pass=$pass&mail=$email",
			true
		);
		return array($nick, $pass);
	}

	/**
	 * Aktiviert einen User. Wrapper-Funktion fï¿½r die Serverfunktionalität
	 * der User-Aktivierung.
	 *
	 * @param string $nick
	 * @param string $validation
	 * @return stdClass 	Daten email, pass und nick
	 * @throws Exception	Wenn was nicht stimmt ;-)
	 */
	public static function activate($nick, $validation) {
		return Server::get_instance(CONF_ENGINE_SERVER_URL)->cmd_activate_user(
			$nick, $validation
		);
	}

	/**
	 * Links the Facebook account indicated by the $fb_id with an already existing
	 * Mammun account with the specified $nick and $pass.
	 *
	 * @param string $fb_id		Facebook user id
	 * @param string $nick		Old Mammun nickname
	 * @param string $pass		Old Mammun password
	 *
	 * @throws Exception		IUser-Konstanten
	 */
	public static function connect_account_with_fb($fb_id, $nick, $pass)
	{
		Server::get_instance(CONF_ENGINE_SERVER_URL)->cmd_connect_account_with_fb(
			$fb_id, $nick, $pass);
	}
}
?>
