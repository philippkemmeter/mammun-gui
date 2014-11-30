<?php
session_start();
require_once ('../include/shared/classes/server_com/ClientServer.inc');
require_once ('../include/_config.inc');
require_once ('../include/classes/User.inc');



function get_facebook_cookie($app_id, $application_secret) {
	$args = array();
	@parse_str(trim($_COOKIE['fbs_' . $app_id], '\\"'), $args);
	ksort($args);
	$payload = '';
	foreach ($args as $key => $value) {
		if ($key != 'sig') {
			$payload .= $key . '=' . $value;
		}
	}
	if (!isset($args['sig']))
		return null;
	if (md5($payload . $application_secret) != $args['sig']) {
		return null;
	}
	return $args;
}

session_destroy();
session_unset();

$r = new ClientServerResponse();
$cookie = get_facebook_cookie(CONF_FACEBOOK_APP_ID, CONF_FACEBOOK_API_SECRET);
try
{
	if(empty($_REQUEST['nick']))
	{	//hier den Nick holen
		if (isset($_REQUEST['uid'])) {
			$nick = Server::get_instance(CONF_ENGINE_SERVER_URL)
				->get_nick_by_fb_id($_REQUEST['uid']);
		}
		else if (isset($cookie['uid']))	{
			$nick = Server::get_instance(CONF_ENGINE_SERVER_URL)
				->get_nick_by_fb_id($cookie['uid']);
		}
		else
			throw new Exception("no nick given");
	}
	else {
		$nick = $_REQUEST['nick'];
	}
}
catch(Exception $e) {
	/**
	 * Im Fehlerfall (egal welcher Art) den Relogin-Command schicken.
	 */
	$r->add_relogin_command("");
	die($r);
}

session_start();

if (!defined('NO_COMMUNITY')) {
	/*.$_REQUEST['pass']*/
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

}

$fb_params = '';
foreach ($_REQUEST as $key => $value)
{
	$fb_params .= "{$key}={$value}&";
}

try {
	if ($cookie) {
			ClientServer::send_request(CONF_ENGINE_SERVER_MAIN_URL
				.'/login_action.php?nick='.$user_nick.'&login=1&'.$fb_params);
	}
	else
	{
		$_GET['ajaxid'] = 0;
		$result = ClientServer::send_request(
			CONF_ENGINE_SERVER_MAIN_URL.'/login_action.php?nick='.$nick
				.'&pass='.$_REQUEST['pass'].'&login=1'
		);
	}

	foreach ($result as $value)
		$r->add_value($value);

	$r->add_session(
//		ClientServer::get_session_name(),
//		ClientServer::get_session_id()
		session_name(),
		session_id()
	);

	/**
	 * Engine-Server-Session-ID (ESSID) in der aktuellen GUI-Session speichern,
	 * damit sie später für alle Server-Requests zur Verfügung steht.
	 */
	$_SESSION['essid'] = ClientServer::get_session_id();

	/**
	 * Die Instanz des aktuellen Users erzeugen, weil daraus ein Session-Objekt
	 * entsteht, das dann von der Startseite aus direkt Angesprochen werden kann,
	 * um auch dort (Selber Server, anderes Projekt) die Nutzerdaten verfügbar zu
	 * haben.
	 * Ja, das ist ein Hack ^^
	 */
	User::get_current_user();
}
catch(Exception $e) {
	/**
	 * Im Fehlerfall (egal welcher Art) den Relogin-Command schicken.
	 */
	$r->add_relogin_command("");
}

echo $r;
?>
