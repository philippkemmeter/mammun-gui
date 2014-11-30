<?php
require_once ('../include/shared/classes/server_com/ClientServer.inc');
require_once ('../include/std_include.inc');

if(!defined('NO_COMMUNITY')) {
	ClientServer::send_request(
		CONF_WCF_SERVER_URL.'/index.php?action=UserLogout&'.
		't='.$_SESSION['WCF_ST'].'&s='.$_SESSION['WCF_SID'], true, true
	);
}

Server::get_instance(CONF_ENGINE_SERVER_URL)->cmd_logout(0);
session_destroy();
include ('templates/logout.tmpl.inc');
?>