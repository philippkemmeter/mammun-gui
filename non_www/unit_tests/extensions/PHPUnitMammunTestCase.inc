<?php
require_once (dirname(__FILE__).'/../_config.inc');
require_once (CONF_GAME_ROOT . '/include/std_include.inc');

class PHPUnitMammunTestCase extends PHPUnit_Framework_TestCase {

	public static function login_user($nick) {
		$result = ClientServer::send_request(
			CONF_WCF_SERVER_URL . "/wbb/index.php?form=UserLoginBridge&".
			"loginUsername=".$nick."&loginPassword=",
			true,
			CONF_MAMMUN_AUTH_SECRET
		);
		$sid_pos=strpos($result, 'wcf_sid')+8;
		$sid_end=strpos($result, ';', $sid_pos);
		$token_pos=strpos($result, 'sec_token')+10;
		$token_end=strpos($result, ';', $token_pos)-1;
		$_SESSION['WCF_SID']= substr($result, $sid_pos, $sid_end-$sid_pos);
		$_SESSION['WCF_ST']= substr($result, $token_pos, $token_end-$token_pos);

		// Im Unit-Test-Mode brauchen wir kein PW
		$result = ClientServer::send_request(
			CONF_ENGINE_SERVER_MAIN_URL.'/login_action.php?nick='.$nick
				.'&pass=&login=1'
		);

		/**
		 * Engine-Server-Session-ID (ESSID) in der aktuellen GUI-Session speichern,
		 * damit sie später für alle Server-Requests zur Verfügung steht.
		 */
		$_SESSION['essid'] = ClientServer::get_session_id();
	}

	public static function install_db_dump($dump_file) {
		Server::get_instance(
			CONF_ENGINE_SERVER_MAIN_URL.
				'/../non_www/unit_tests/phpunit_interface_for_gui.php'
		)->install_db_dump($dump_file);
	}

}