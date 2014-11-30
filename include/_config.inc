<?
/**
 * Diese Datei enthält Serverabhängige Konstanten, welche angepasst werden müssen,
 * bei Umzügen oder Auswechseln der Community etc.
 */
@include_once(dirname(__FILE__).'/../_private_config.inc');
if (!defined('CONF_ENGINE_SERVER_MAIN_URL'))
	define ('CONF_ENGINE_SERVER_MAIN_URL',		'http://localhost:8123/game');
if (!defined('CONF_PAYMENT_SERVER_MAIN_URL'))
	define ('CONF_PAYMENT_SERVER_MAIN_URL',		'http://localhost:8125/game');

if (!defined('CONF_WCF_SERVER_URL'))
	define ('CONF_WCF_SERVER_URL', 'http://localhost:8124');



if (!defined('CONF_MY_SERVER_URL')) {
	/**
	 * Diese Konstante hält die URL zu dem Server, auf dem gerade diese Datei
	 * installiert ist - also zu "meinem" Server.
	 * Wenn der Port nicht 80 ist, muss er explizit angegeben werden in der URL,
	 * sonst kann man ihn bekanntlich weglassen.
	 */
	if ($_SERVER['SERVER_PORT'] != 80) {
		define ('CONF_MY_SERVER_URL', 'http://'.$_SERVER['SERVER_NAME'].':'.
											$_SERVER['SERVER_PORT']);
	}
	else {
		define ('CONF_MY_SERVER_URL', 'http://'.$_SERVER['SERVER_NAME']);
	}
}

if (!defined('CONF_LOGIN_PAGE_URI')) {
	define ('CONF_LOGIN_PAGE_URI',	'/');
}

if (!defined('CONF_FACEBOOK_API_KEY'))
        define ('CONF_FACEBOOK_API_KEY',    '625457925d3db8f7b0066b5c99333fe3');
if (!defined('CONF_FACEBOOK_API_SECRET'))
        define ('CONF_FACEBOOK_API_SECRET', 'af84b870d0641e838f615dc9e3035c13');

if (!defined('CONF_FACEBOOK_APP_URL'))
	define ('CONF_FACEBOOK_APP_URL',	'http://apps.facebook.com/MammunIslands');

if (!defined('CONF_FACEBOOK_APP_ID'))
	define('CONF_FACEBOOK_APP_ID',		'355337526974');

if (!defined('CONF_FACEBOOK_SHARE_URL'))
	define('CONF_FACEBOOK_SHARE_URL',	'http://www.facebook.com/share.php?u=http%3A%2F%2Fapps.facebook.com%2FMammunIslands%2F');


define ('CONF_ENGINE_SERVER_URL',	CONF_ENGINE_SERVER_MAIN_URL.
										'/interface_for_gui.php');
define ('CONF_PAYMENT_SERVER_URL',	CONF_PAYMENT_SERVER_MAIN_URL.
												'/interface.php');

define ('CONF_LANGUAGE_FILE_DIR',	dirname(__FILE__).'/../game/language');

define ('CONF_UPLD_TMP_DIR', dirname(__FILE__).'/../__u/');

define ('CONF_MAMMUN_AUTH_SECRET', '293489f41898d18a53a6cf49ab476b96');

//http://www.facebook.com/share.php?u=http%3A%2F%2Fapps.facebook.com%2FMammunIslands%2Fmap_id=12345&t=Geiler%20Entdecker
?>