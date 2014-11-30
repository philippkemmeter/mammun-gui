<?php
/**
 * Diese Datei zeigt Forschungsstatusse und Dialoge an. Per AJAX geladen.
 */
require_once ('../include/classes/MammunFBWrapper.inc');
require_once ('../include/std_include.inc');
require_once ('../include/classes/Worlds.inc');
require_once ('../include/classes/Avatar.inc');

if (isset($_GET['ajaxid'])) {// Ein ping-und-echo-Parameter für AJAX-Request-Zuweisungen
	$AJAX_OUTPUT = true;
	$r = new ClientServerResponse();
	$r->add_value($_GET['ajaxid']);
	$this_win_id = $_GET['ajaxid'];
}
ob_start();

Language::get_instance()->load_language_file('invitation');
Language::get_instance()->load_language_file('facebook_publish');

$current_user = User::get_current_user();

$expected_data = array (
	'send'				=> 	'bool',
	'email'				=> 	'string',
	'facebook_send'		=> 	'bool',
	'chosen_friends'	=>  'string',
	'chosen_fb_friends'	=>  'string',
	'fb_send'			=>	'bool',
	'get_facebook_friends'=>'bool',
	'mammun_friends'	=> 'bool',
	'general_invitation'=>	'bool'			// ob der Spieler NICHT auf die
											// Karte, sondern allgemein einlädt
);
$GP = $cryptObj->load_GET_POST($expected_data);

/**
 * User hat über das Facebook-Formular die Facebook-User ausgesucht, die er
 * einladen möchte und die Einladung an Facebook abgeschickt.
 */
if (isset($_REQUEST['ids'])) {
	// TODO: Spamschutz hier einbauen.
	$fb_user_ids = $_REQUEST['ids'];

	include('templates/invitation/facebook_invitation_feedback.tmpl.inc');
	die();
}

$direct_to_map = empty($GP['general_invitation'])
					? true
					: !$GP['general_invitation'];

$x = ClientServer::send_request(
	CONF_ENGINE_SERVER_URL."?get_map_data=".serialize(
		array(
			$current_user->get_id(),
			$current_user->get_world_id(),
			$current_user->get_map_id())
		)
);
$map_data = $x[0];
$mentor_map = $map_data->mentor_map;
$timeout_days = 28; // $mentor_map ? 14 : 28;

/**
 * User hat unser Formular zum Benutzereinladen ausgefüllt und abgeschickt.
 */
if (isset($GP['send']))
{
	$friends = array();

	$correct_data = explode(',', html_entity_decode($GP['email']));
	$friend_nicks = explode(',', html_entity_decode($GP['chosen_friends']));
	$friend_fb_ids = explode(',', html_entity_decode($GP['chosen_fb_friends']));

	/**
	 * Diese Schleife durchläuft die eingetragenen Mail-Adressen und
	 * interpretiert die Form "Name <Mail-Addy>".
	 * Das wird dann in ein Friend-Objekt pro E-Mail-Adresse gepackt mit den
	 * Daten "name" (Username) und "email" (Mail-Adresse), da der Server diese
	 * Form erwartet.
	 */
	$n = count($correct_data);
	for ($i = 0; $i < $n; $i++)	{
		$friend = new stdClass();
		$friend_data = explode('<', $correct_data[$i]);
		if (count($friend_data) == 1) {
			$friend->name = '';
			$friend->email = trim(str_replace('>', '', $friend_data[0]));
		}
		else {
			$friend->name = trim($friend_data[0]);
			$friend->email = trim(str_replace('>', '', $friend_data[1]));
		}
		$friends[] = $friend;
	}
	/**
	 * Und diese Schleife durchläuft die eingetragenen Freund-Mammun-Nicks und
	 * setzt sie in Friend-Object so ein, dass der Server weiß, dass er sie
	 * ingame einladen soll.
	 */
	$n = count($friend_nicks);
	for ($i = 0; $i < $n; $i++) {
		$friend = new stdClass();
		$friend->mammun_nick = trim($friend_nicks[$i]);
		$friends[] = $friend;
	}

	/**
	 * HIER FACEBOOK EINLADUNGEN VERSCHICKEN
	 *
	 * (GEHT NICHT, WEIL FB-API DAS NICHT ERLAUBT !)
	 */
	/**
	 * $friend_fb_ids enthält alle FB-UserIDs, die benachrichtigt werden sollen
	 */
	//error_log(print_r($friend_fb_ids, true));

	$r = new ClientServerResponse();
	try {
		Server::get_instance(CONF_ENGINE_SERVER_URL)
			->cmd_invite_friends($friends, '', $direct_to_map);


		$r->add_value(1);
		$r->add_value(str_replace(
			'[DAYS]', $timeout_days, LNG_INVITATION_SUCCESSFUL
		));
	}
	catch (Exception $e) {
		if ($e->getCode() == 1) {
			$r->add_value(0);
			$r->add_value(LNG_ERR_ALREADY_INVITED);
		}
		else {
			$r->add_value(0);
			$r->add_value(LNG_ERR_STRANGE_ERROR);
		}
	}
	die($r);
}

/**
 * Auf der ersten Karte (und nur da) auch die Facebook-Freunde einladen.
 * (Oder mit get_facebook_friends explizit erfragen)
 */
if (isset($GP['get_facebook_friends'])
	|| (($mentor_map || !$direct_to_map) && !$GP['mammun_friends']))
{
	if (!isset($_SESSION['facebook_friends'])) {
		$fb_friends = array();
		$fb_invite_excludes = array();
		$facebook = MammunFBWrapper::getFacebook();

		try
		{
			$fb_friend_ids = $facebook->api_client->friends_get();
			$fb_friends_in_mammun = array_keys(
				(array)(Server::get_instance(CONF_ENGINE_SERVER_URL)
					->get_nick_by_fb_id($fb_friend_ids))
			);

			$i = 0;
			foreach ($fb_friend_ids as $fb_friend_id) {
				$f = new stdClass();
				$f->fb_id = $fb_friend_id;
				/**
				 * Keine FB-Freunde anzeigen, die schon Mammun spielen
				 */
				if (in_array($fb_friend_id, $fb_friends_in_mammun)) {
					$fb_invite_excludes[] = $f;
				}
				else {
					if ($i >= 6)
						continue;
					$user_details = $facebook->api_client->users_getInfo(
						$fb_friend_id,
						array('first_name', 'pic_square')
					);

					$f->first_name = $user_details[0]['first_name'];
					$f->avatar = empty($user_details[0]['pic_square'])
						? Avatar::get_default_avatar(44)
						: $user_details[0]['pic_square'];


					$fb_friends[] = $f;
					$i++;
				}
			}
		}
		catch (Exception $e)
		{
		}
		if (count($fb_friends) < 6) {
			for ($i = count($fb_friends); $i < 6; $i++) {
				$f = new stdClass();
				$f->first_name = '';
				$f->avatar = 'ui/icons/labels/avatars/44/default.png';
				$f->fb_id = 0;
				$fb_friends[] = $f;
			}
		}
		$_SESSION['facebook_friends']['fb_friends']
			= $fb_friends;
		$_SESSION['facebook_friends']['fb_invite_excludes']
			= $fb_invite_excludes;
	}
	else {
		$fb_friends = $_SESSION['facebook_friends']['fb_friends'];
		$fb_invite_excludes
			= $_SESSION['facebook_friends']['fb_invite_excludes'];
	}
	if (isset($GP['get_facebook_friends'])) {
		$r = new ClientServerResponse();
		$r->add_value($fb_friends);
		$r->add_value($fb_invite_excludes);
		die($r);
	}
}

$this_world = Worlds::get_world_data_by_id(
				$current_user->get_world_id(), IWorld::MAP_TYPE_FFA
);
/**
 * Alle Freunde, die er bereits auf diese Insel eingeladen hat
 */
$invited_friends = Server::get_instance(CONF_ENGINE_SERVER_URL)
	->get_invited_friends(
		$current_user->get_world_id(),
		$current_user->get_map_id()
	);

$users =
	MapPlayers::get_instance(
		$current_user->get_world_id(),
		$current_user->get_map_id()
	)->get_users_on_map();

$nicks_on_map = array();
foreach ($users as $p) {
	$nicks_on_map[] = $p->get_nick();
}

if (!$direct_to_map) {
	include('templates/invitation/invite_outsidefriends_general.tmpl.inc');
}
else {
	if ($mentor_map && !$GP['mammun_friends']) {
		include('templates/invitation/invite_outsidefriends_to_map.tmpl.inc');
	}
	else {
		include('templates/invitation/invite_mammunfriends_to_map.tmpl.inc');
	}
}

if ($AJAX_OUTPUT) {
	$r->add_value(ob_get_clean());
	echo $r;
}
else
	ob_flush();
?>
