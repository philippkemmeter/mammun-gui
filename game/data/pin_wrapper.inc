<?php
require_once ('../include/shared/classes/server_com/ClientServer.inc');
require_once ('../include/_config.inc');

header('Content-Type: image/png');
echo ClientServer::send_request(CONF_ENGINE_SERVER_MAIN_URL.
	'/pin.php?dataY='.$_GET['dataY'], true
);
?>