<?php
require_once('include/shared/classes/server_com/ClientServer.inc');

$req_string = '';
foreach ($_REQUEST as $key => $value)
	$req_string .= "$key=$value&";
$req_string .= rand();	// anti cache

echo ClientServer::send_request2("www.mammun.com", 80,
	"/game/sync_login.php",
	$req_string, true);
?>