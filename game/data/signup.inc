<?php
/**
 * File created by Philipp Kemmeter on 05.02.2009 at 15:41:30
 */

@session_start();
if (!empty($_GET['essid'])) {
	$_SESSION['essid'] = $_GET['essid'];
}
if (isset($_GET['map_key'])) {
	$_SESSION['map_key'] = $_GET['map_key'];
}
if (isset($_REQUEST['visit'])) {
	$_SESSION['visit'] = $_REQUEST['visit'];
}
require_once ('../include/const.inc');
require_once ('../include/_config.inc');
require_once ('../include/shared/classes/Time.inc');
require_once ('../include/shared/classes/server_com/ClientServer.inc');
if (isset($_SESSION['essid']))
	ClientServer::set_session('PHPSESSID', $_SESSION['essid']);

require_once ('../include/shared/classes/PV_encryption.inc');
require_once ('../include/classes/User.inc');
/**
 * @var PV_encryption
 */
$cryptObj = new PV_encryption();

include_once('../include/shared/classes/Language.inc');
include_once ('../include/classes/Signup.inc');


Language::get_instance()->load_language_file('signup_');
$language = Language::get_instance()->get_language();

$expected_data = array (
	'submitok'			=>	'bool',
	'submit_waiting'	=>	'string',
	'nick'				=>	'(e)string',
	'validate'			=>	'(e)string',
	'pass'				=>	'string',
	'email'				=>	'string',
	'email2'			=>	'string',
	'news'				=>	'bool',
	'firstname'			=>	'string',
	'surname'			=>	'string',
	'agb_accepted'		=>	'bool',
	'recruit'			=>	'string',
	'recruit2'			=>	'string'
);
$GP = $cryptObj->load_GET_POST($expected_data);
/**
 * Er hat die Anmeldung bestätigt
 */
if (@$GP['submitok']) {
	$response = new ClientServerResponse();
	if (!$GP['agb_accepted']) {
		$response->add_value(0);
		$response->add_value(LNG_ERR_AGB_NOT_ACCEPTED);
		$response->add_value(100);
	}
	else if ($GP['email'] != $GP['email2']) {
		$response->add_value(0);
		$response->add_value(LNG_ERR_EMAIL_DO_NOT_MATCH);
		$response->add_value(101);
	}
	else {
		try {
			if (strlen($_SESSION['map_key']) > 6) {
				$tmp = $_SESSION['map_key'];
				$_SESSION['map_key'] = substr($tmp, 0, 6);
				$GP['recruit'] = substr($tmp, 6);
			}
			Signup::register(
				$GP['nick'],
				$GP['email'],
				$GP['firstname'],
				$GP['surname'],
				$GP['recruit'],
				$GP['news'],
				$GP['pass'],
				$_SERVER['HTTP_HOST'].'/game/signup.php',
				0,
				$_SESSION['map_key'],
				$language,
				$_SESSION['visit']
			);
			$response->add_value(1);
		}
		catch (Exception $e) {
			$response->add_value(0);
			switch ($e->getCode()) {
				case Signup::ERR_EMAIL_ADRESS_INVALID:
					$response->add_value(LNG_ERR_EMAIL_ADDRESS_INVALID);
					break;
				case Signup::ERR_EMAIL_ALREADY_REGISTERED:
					$response->add_value(LNG_ERR_EMAIL_ALREADY_REGISTERED);
					break;
				case Signup::ERR_NICK_ALREADY_REGISTERED:
					$response->add_value(LNG_ERR_NICK_ALREADY_REGISTERED);
					break;
				case Signup::ERR_NICK_CONTAINS_INVALID_CHARS:
					$response->add_value(LNG_ERR_NICK_CONTAINS_INVALID_CHARS);
					break;
				case Signup::ERR_NICK_TOO_LONG:
					$response->add_value(LNG_ERR_NICK_TOO_LONG);
					break;
				case Signup::ERR_NICK_TOO_SHORT:
					$response->add_value(LNG_ERR_NICK_TOO_SHORT);
					break;
				case Signup::ERR_NO_EMAIL:
					$response->add_value(LNG_ERR_NO_EMAIL);
					break;
				case Signup::ERR_NO_FIRSTNAME:
					$response->add_value(LNG_ERR_NO_FIRSTNAME);
					break;
				case Signup::ERR_NO_SURNAME:
					$response->add_value(LNG_ERR_NO_SURNAME);
					break;
				case Signup::ERR_NO_NICK:
					$response->add_value(LNG_ERR_NO_NICK);
					break;
				case Signup::ERR_RECRUIT_USER_DOES_NOT_EXIST:
					$response->add_value(
						str_replace('[NICK]', $GP['recruit'],
							LNG_ERR_RECRUIT_NOT_EXIST
						)
					);
					break;
				case Signup::ERR_SERVER_FULL:
					$response->add_value(LNG_ERR_SERVER_FULL);
					break;
				default:
					$response->add_value(LNG_ERR_UNKNOWN_ERROR.'. Code: '.$e->getCode());
					break;
			}
			$response->add_value($e->getCode());
		}
	}
	echo $response;

	die();
}
/**
 * Er möchte einen Account aktivieren
 */
else if ($GP['validate']) {
	$error_text = '';
	try {
		$data = Signup::activate($GP['nick'], $GP['validate']);
	}
	catch (Exception $e) {
		if ($e->getCode() == Signup::ERR_VALIDATION_KEY_INVALID) {
			$error_text = LNG_ERR_VALIDATION_KEY_INVALID;
		}
		else if ($e->getCode() == Signup::ERR_USER_ALREADY_ACTIVATED) {
			// ignore
		}
		else {
			$error_text = LNG_ERR_UNKNOWN_ERROR;
		}
	}
	if (empty($error_text)) {
		error_log("Kein Fehler aufgetreten!");
		/**
		 * User ist jetzt schon im Spiel eingeloggt. Session-Daten sichern...
		 */
		ClientServer::set_session($data->session_name, $data->session_id);
		$_SESSION['essid'] = $data->session_id;
		/**
		 * ... und User auch in die Community einloggen
		 */
		$result = ClientServer::send_request(
			CONF_WCF_SERVER_URL . "/wbb/index.php?form=UserLoginBridge&".
			"loginUsername=".$GP['nick']."&loginPassword=",
			true,
			CONF_MAMMUN_AUTH_SECRET
		);
		$sid_pos=strpos($result, 'wcf_sid')+8;
		$sid_end=strpos($result, ';', $sid_pos);
		$token_pos=strpos($result, 'sec_token')+10;
		$token_end=strpos($result, ';', $token_pos)-1;
		$_SESSION['WCF_SID'] = substr($result, $sid_pos, $sid_end-$sid_pos);
		$_SESSION['WCF_ST'] = substr($result, $token_pos, $token_end-$token_pos);
		$redirect_page = "/$language/?".SID.'&just_activated=true';
		include('templates/ajax_redirect.tmpl.inc');
	}
	else {
		include ('templates/signup/activate.tmpl.inc');
	}
	die();
}
include ('templates/signup/signup.tmpl.inc');

?>