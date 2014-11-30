<?php
require_once ('../include/shared/classes/server_com/ClientServer.inc');
require_once ('../include/_config.inc');

$page = $_GET['page'];
unset($_GET['page']);

echo ClientServer::forward_request(
	CONF_ENGINE_SERVER_MAIN_URL.'/'.$page,
	$_GET

);
?>