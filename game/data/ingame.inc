<?php
/**
 * @author Philipp Kemmeter
 */
require_once ('../include/std_include.inc');
require_once ('../include/classes/MammunFBWrapper.inc');
require_once ('../include/classes/MammunFB2Wrapper.inc');
require_once ('../include/classes/MapChooserPrivate.inc');

//$facebook_keys = MammunFBWrapper::$facebook_keys;
foreach	($_REQUEST as $key => $value)
{
	//This if statement doesn't work for some reason... will fix later.
//	if (in_array($facebook_keys, $key))
//	{
		$_SESSION['fb'][$key] = $value;
//	}
}

$current_user = User::get_current_user();
$expected_data = array (
	'start'		=> 'bool',
	'relogin'	=> 'ebool',
	'leave_map'	=> 'ebool',
	'join_map'	=> 'bool',
	'world_id'	=> 'int',
	'map_id'	=> 'int'
);

$GP = $cryptObj->load_GET_POST($expected_data);
/**
 * Sprache für die Sitzung bei der Game-Engine setzen
 */
Server::get_instance(CONF_ENGINE_SERVER_URL)->set_language(
	Language::get_instance()->get_language()
);
/**
 * Sprache für die Sitzung für die Community setzen
 */
ClientServer::send_request(
	CONF_WCF_SERVER_URL.'/gausus_wcf_hacks/language_interface.php?language='.
		Language::get_instance()->get_language().'&s='.$_SESSION['WCF_SID'].
		'&t='.$_SESSION['WCF_ST']
);

// Spieler will auf eine neue Insel wechseln. Das wird erst durch
// leave_map wirklich getriggert. Wir vermerken hier nur den Wunsch und wohin,
// so dass wir ihn dann automatisch durch die Menüs führen können in den
// veschiedenen Screens.
if (@$GP['join_map'] && @$GP['world_id'] && isset($GP['map_id'])) {
	$_SESSION['join_map'] = array(
		'world_id'	=> $GP['world_id'],
		'map_id'	=> $GP['map_id']
	);
}

if (@$GP['leave_map']) {
	$r = new ClientServerResponse();
	try {
		if (Server::get_instance(CONF_ENGINE_SERVER_URL)->cmd_leave_map(0)) {
			$r->add_value(1);
		}
		else {
			$r->add_value(0);
		}
	}
	catch (HackattackException $e) {
		$r->add_value("!");
		if ($e->getCode()
			== HackattackException::CODE_LEAVING_MAP_WHILE_NOT_PLAYING)
		{
			$r->add_value("Komisch...");
		}
		else {
			$r->add_value("Komisch...");
		}
	}
	if (@$GP['join_map'] && @$GP['world_id'] && isset($GP['map_id'])) {
		$r->add_value($GP['world_id']);
		$r->add_value($GP['map_id']);
		// Wir geben die Zielkoordinaten zurück. Der Ajax-Response-Dialog
		// muss das dann behandeln und sie weiterreichen.
		// Das ist zur Absicherung, falls die Session stirbt. Ansonsten
		// speichern wir das auch noch in der Session (s.o.)
	}

	$current_user = User::get_instance_by_id($current_user->get_id(),true);
	die($r);
}

if (@$GP['relogin'])
	$current_user = User::get_instance_by_id($current_user->get_id(),true);

if (isset($GP['start'])) {
	/**
	 * Man möchte einen Spieler auf der Insel per Direct-Link besuchen.
	 */
	if (!empty($_SESSION['visit'])) {
		$visit_arr = explode(".", $_SESSION['visit']);
		$visit_nick = ($visit_arr[1] == 0)
			? $visit_arr[0]
			: $visit_arr[0].'.'.$visit_arr[1];
		try {
			// FIXME: get_homesector is obsolete
			$map_id = floor(User::get_instance_by_nick($visit_nick)
				->get_homesector()/100);
			$world_id = floor($map_id/100);
			$map_id = $map_id%100;
			if (!empty($visit_arr[2]))
				$_SESSION['map_key'] = $visit_arr[2];
		}
		catch(Exception $e) {
			include_once('../include/classes/EventMessages.inc');
			EventMessages::add_entry(
				EventMessages::TYPE_VISIT_MAP_FAILED,
				0,
				$visit_nick
			);
			$world_id = 0;
			$map_id = 0;
		}
		unset($_SESSION['visit']);
	}


	/**
	 * Ist in der Session ein Map-Key, dann hat der Spieler ihn per GET oder
	 * POST an die std_include.inc irgendwie geschickt und somit sein Wissen
	 * ob dieses Map-Keys bewiesen. Wir versuchen jetzt dem User diesen Key
	 * einzutragen. Alles weitere läuft dann später von selbst über die Engine.
	 */
	if (!empty($_SESSION['map_key'])) {
		$key = substr($_SESSION['map_key'], 0, 6);
		$res_by_uid = substr($_SESSION['map_key'], 6);
		if (empty($res_by_uid) && $visit_nick) {
			$res_by_uid = User::get_instance_by_nick($visit_nick)->get_id();
		}
		unset($_SESSION['map_key']);
		try {
			$current_user->add_auth_key($key, $res_by_uid);
		}
		catch (Exception $e) {
			// @TODO:
			error_log($e->getMessage());
		}
		/**
		 * Direkt die zu besuchende Insel zeigen.
		 */
		try {
			$map_key_map_data = MapChooserPrivate::get_data_by_auth_key(
				$key
			);
			$world_id = $map_key_map_data->world_id;
			$map_id = $map_key_map_data->map_id;
		}
		catch (Exception $e) {
			// @TODO:
			error_log($e->getMessage());
		}
	}

	/**
	 * Wenn wir in Facebook sind, dann bei den ersten Logins den Facebook-
	 * Avatar kopieren.
	 */
	if (isset($_SESSION['fb']['fb_sig'])|| isset($_SESSION['fb']['fb_sig_post'])
		|| isset($_SESSION['fb']['session']))
	{
		/**
		 * Achtung: Jeder API-Call dauert ein paar Sekunden! Daher wird das
		 * nur bei den ersten Logins automatisch gemacht.
		 */
		if ($current_user->get_login_count() < 6) {
			/**
			 * Alte Authentifizierung
			 */
			if (!isset($_SESSION['fb']['session'])) {
				$facebook = MammunFBWrapper::getFacebook();
				$fb_user = MammunFBWrapper::getUser();
				$fb_user_data = $facebook->api_client->users_getInfo(
					$fb_user,
					array(/* Add any other data here, which is required*/
						'pic_big'
					)
				);
				$fb_user_data = $fb_user_data[0];
				$pic = $fb_user_data['pic_big'];
			}
			else {
				$facebook = MammunFB2Wrapper::getFacebook();
				$fb_user_data = $facebook->api('me/?fields=picture&type=large');
				$pic = $fb_user_data['picture'];
			}

			ClientServer::send_request(
				CONF_WCF_SERVER_URL.'/wbb/index.php?form=AvatarEditPlain&s='
					.$_SESSION['WCF_SID'].'&t='.$_SESSION['WCF_ST'].
						'&avatarURL='.urlencode($pic),
				true
			);
		}
	}

	include('map_ui.inc');
}
else {
	include ('templates/ingame.tmpl.inc');
}
?>
