<?php
/**
 * Diese Datei sollte von allen Projektdateien eingebunden werden,
 * außer von denen, die von anderen Dateien eingebunden wurden,
 * die das schon eingebunden haben. Um letzteres sicher zur gewährleisten,
 * hat diese Datei eine C-like Art damit umzugehen :)
 * Das ist nur zur Sicherheit, denn natürlich hilf ein konsequentes
 * include_once genauso.
 *
 * @author Philipp Kemmeter
 */
@session_start();
if (!empty($_GET['essid'])) {
	$_SESSION['essid'] = $_GET['essid'];
}
if (!empty($_REQUEST['map_key'])) {
	$_SESSION['map_key'] = $_REQUEST['map_key'];
}
if (!empty($_REQUEST['visit'])) {
	$_SESSION['visit'] = $_REQUEST['visit'];
}
if (isset($_GET['wcf_s'])) {
	$_SESSION['WCF_SID'] = $_GET['wcf_s'];
}
if (isset($_GET['wcf_t'])) {
	$_SESSION['WCF_ST'] = $_GET['wcf_t'];
}
if (!isset($_SESSION['sid']))
	$_SESSION['sid'] = session_id();

require_once (dirname(__FILE__).'/const.inc');
require_once (dirname(__FILE__).'/_config.inc');
require_once (dirname(__FILE__).'/shared/classes/ScienceConstants.inc');
require_once (dirname(__FILE__).'/shared/classes/Time.inc');
require_once (dirname(__FILE__).'/shared/classes/ValueChecker.inc');
require_once (dirname(__FILE__).'/shared/classes/server_com/ClientServer.inc');
if (isset($_SESSION['essid']))
	ClientServer::set_session('PHPSESSID', $_SESSION['essid']);

require_once (dirname(__FILE__).'/classes/User.inc');
require_once (dirname(__FILE__).'/shared/classes/PV_encryption.inc');
require_once (dirname(__FILE__).'/shared/classes/LanguagePrint.inc');
/**
 * @var PV_encryption
 */
$cryptObj = new PV_encryption();

function kill_session () {
	error_log("Session killed by {$_SERVER['PHP_SELF']}");
	if (isset($_GET['ajaxid'])) {
		$r = new ClientServerResponse();
		$r->add_relogin_command('');
		die($r);
	}
	else {
		include ('logout.php');
		die();
	}
}

if ($_SESSION['sid'] != session_id()) {
	error_log("Sessionmerge! Aktuelle Session passt nicht zur ID!");
	kill_session();
}
?>
