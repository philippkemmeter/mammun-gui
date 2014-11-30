<?php
@include_once(dirname(__FILE__).'/_private_config.inc');
define ('CONF_GAME_ROOT',		dirname(__FILE__).'/../..');
define ('CONF_UNIT_TESTS_ROOT',	dirname(__FILE__));
if (!defined('CONF_ENGINE_SERVER_MAIN_URL'))
	define ('CONF_ENGINE_SERVER_MAIN_URL', 'http://www.mammun.com:8323/game');

if (!defined('CONF_WCF_SERVER_URL'))
	define ('CONF_WCF_SERVER_URL', 'http://www.mammun.com:8324');

?>