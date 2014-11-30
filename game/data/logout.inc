<?php
require_once ('../include/shared/classes/server_com/ClientServer.inc');
require_once ('../include/std_include.inc');

if(!defined('NO_COMMUNITY')) {
	ClientServer::send_request(
		CONF_WCF_SERVER_URL.'/index.php?action=UserLogout&'.
		't='.(@$_SESSION['WCF_ST']).'&s='.(@$_SESSION['WCF_SID']), true, true
	);
}

Server::get_instance(CONF_ENGINE_SERVER_URL)->cmd_logout(0);
Server::get_instance(CONF_PAYMENT_SERVER_URL, CONF_MAMMUN_AUTH_SECRET)
	->unvisit_all_items(
		User::get_current_user()->get_id()
	);
Server::get_instance(CONF_PAYMENT_SERVER_URL, CONF_MAMMUN_AUTH_SECRET)
	->release_item_locks(
		User::get_current_user()->get_id()
	);
$fb_sig = @$_SESSION['fb']['fb_sig'];
$fb_sig_post = @$_SESSION['fb']['fb_sig_post'];
$fb_session = @$_SESSION['fb']['session'];
session_destroy();
session_start();
$_SESSION['fb']['fb_sig'] = $fb_sig;
$_SESSION['fb']['fb_sig_post'] = $fb_sig_post;
$_SESSION['fb']['session'] = $fb_session;
$user_in_facebook =
	(isset($_SESSION['fb']['fb_sig']) || isset($_SESSION['fb']['fb_sig_post'])
		|| isset($_SESSION['fb']['session']));
unset($_SESSION['fb']);

if ($user_in_facebook) {
	$redirect_page = CONF_FACEBOOK_APP_URL;
}
else {
	$redirect_page = CONF_LOGIN_PAGE_URI;
}
include ('templates/logout.tmpl.inc');
?>
