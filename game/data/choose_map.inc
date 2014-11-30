<?php
include_once ('../include/std_include.inc');
include_once ('../include/classes/ScoreCache.inc');
include_once ('../include/classes/Techtree.inc');
include_once ('../include/classes/Worlds.inc');
require_once('../include/classes/Avatar.inc');
require_once('../include/classes/EventMessages.inc');
require_once ('../include/classes/MammunFBWrapper.inc');

Language::get_instance()->load_language_file("facebook_publish");
Language::get_instance()->load_language_file("map_ui");

if (isset($_GET['ajaxid'])) {// Ein ping-und-echo-Parameter für AJAX-Request-Zuweisungen
	$AJAX_OUTPUT = true;
	$r = new ClientServerResponse();
	$r->add_value($_GET['ajaxid']);
	$this_win_id = $_GET['ajaxid'];
}
else
	$AJAX_OUTPUT = false;
$current_user = User::get_current_user();
ob_start();
if (($current_user->get_state() == User::STATE_CHOOSING_SCIENCES_LOST_LAST_MAP)
	|| ($current_user->get_state() == User::STATE_CHOOSING_SCIENCES_WON_LAST_MAP))
{
	try {
		$selected_sciences = Server::get_instance(CONF_ENGINE_SERVER_URL)
				->cmd_select_all_sciences_auto();
		$state = User::get_current_user(true)->get_state();
	}
	catch (Exception $e) {
		error_log("A strange error occured. Please report this to an administrator.");
		error_log(print_r($e,true));
		die("A strange error occured. Please report this to an administrator.");
	}
}

if (!isset($_SESSION['facebook_friends'])) {
	$fb_friends = array();
	$fb_invite_excludes = array();
	try
	{
		$facebook = MammunFBWrapper::getFacebook();
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
			 * Keine FB-Freunde anzeigen, die Mammun nicht spielen
			 */
			if (in_array($fb_friend_id, $fb_friends_in_mammun)) {
				$fb_invite_excludes[] = $f;
			}
			else {
				if ($i >= 6)
					continue;
				$user_details = $facebook->api_client->users_getInfo($fb_friend_id,
				array('first_name', 'pic_square'));

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
			$f->avatar = 'pattern/spacer.gif';
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

/**
 * Checken, ob der Inhalt der Seite überhaupt zutreffend ist. Wenn der User-State
 * nicht stimmt, wurde entweder versucht zu bescheißen, oder im Spielcode ist
 * ein Designfehler. Kann auch vorkommen, wenn sich der State im laufenden Spiel
 * ändert. In jedem Fall löst ein kill_session() die Situation.
 */
if ($current_user->get_state() != User::STATE_CHOOSING_MAPS
	&& $current_user->get_state() != User::STATE_WAITING_ON_MAP
	&& $current_user->get_state() != User::STATE_WAITING_FOR_CLANMAP
	&& $current_user->get_state() != User::STATE_WAITING_ON_MAP_AND_READY
	&& $current_user->get_state() != User::STATE_NEW_PLAYER
	&& !$current_user->get_waiting_state())
{
	echo "Wrong state...$state...{$_SERVER['PHP_SELF']}";
	kill_session();
}

Language::get_instance()->load_language_file('choose_map');
//include_once ('../include/classes/MapChooserFFA.inc');
include_once ('../include/classes/MapChooserPrivate.inc');
//include_once ('../include/classes/MapChooserPrivate.inc');

$expected_data = array (
	'chosen_map_index'			=>	'eint',
	'unchoose_map'				=>	'ebool',
	'toggle_ready'				=>	'ebool',
	'start_mentor_map'			=>	'ebool',
	'world_id'					=>	'(e)int',
	'get_available_map_data'	=>	'ebool',
	'choose_map_randomly'		=>	'ebool',
	'choose_by_auth_key'		=>	'(e)string',
	'start_new_map'				=>	'ebool',
	'template_id'				=>	'int',
	'invitations'				=>	'string',
	'color'						=>	'string',
	'home'						=>	'int'
);
$GP = $cryptObj->load_GET_POST($expected_data);

$world_id = isset($GP['world_id']) ? $GP['world_id'] : 0;
$map_chooser = MapChooserPrivate::get_instance(
	$current_user->get_id(),
	$world_id
);
$reload_userdata = false;

// $_SESSION['join_map'] wird in der ingame.inc gesetzt, wenn jemand
// eine Insel verlassen will und direkt weiß, wohin er wechseln will.
// siehe auch map_visit_navi.tmpl.inc
if (isset($_SESSION['join_map'])) {
	$available_maps = $map_chooser->get_available_maps();

	$destination = $_SESSION['join_map'];
	$choose_map_index = -1;
	foreach ($available_maps as $index => $map_obj) {
		if (($map_obj->world_id == $destination['world_id'])
			&& ($map_obj->map_id == $destination['map_id']))
		{
			$choose_map_index = $index;
			break;
		}
	}

	// Das wird jetzt vom Dialog benutzt, um die Wahl automatisch zu treffen
	// und dann die Insel zu laden
	$_SESSION['join_map']['choose_map_index'] = $choose_map_index;
}

/**
 * API-Anfrage: Die JS-GUI will die verfügbaren Karten haben :)
 */
if (isset($GP['get_available_map_data'])) {
	$r = new ClientServerResponse();

	$available_maps = $map_chooser->get_available_maps();
	$availbale_map_ids = array();
	$choose_map_hrefs = array();
	$n = count($available_maps);
	foreach ($available_maps as $index => $map_obj) {
		$availbale_map_ids[] = array($map_obj->world_id, $map_obj->map_id);
		$choose_map_hrefs[$map_obj->world_id][$map_obj->map_id]=
			'choose_map.php?dataX='.
			$cryptObj->do_encryption(array(
					'chosen_map_index' 	=> $index,
					'world_id'			=> $world_id
				)
			);
	}
	sort($availbale_map_ids);

	$r->add_value(1);
	$r->add_value($availbale_map_ids);
	$r->add_value($choose_map_hrefs);
	die($r);
}
/**
 * Der Spieler möchte auf dem ersten Host nochmal spielen, um Freunde
 * von außen einzuladen.
 */
else if (isset($GP['start_mentor_map'])) {
	$template_id = $GP['template_id'];
	$r = new ClientServerResponse();

	/**
	 * Wenn ein ganz neuer Spieler eine Mentorenkarte startet, dann wird
	 * das Tutorial dem Spieler noch gezeigt, wo die Einladungen am Ende
	 * erklärt werden. Deswegen wollen wir nicht direkt beim Siedeln
	 * einladen. Sonst schon! :)
	 */
	$no_invitations = (User::get_current_user()->get_state()
						== IUser::STATE_NEW_PLAYER);

	try {
		$map_chooser->open_new_mentor_map($template_id, $GP['home'],
			$GP['color'], $no_invitations);
	}
	catch (Exception $e) {

	}
	/**
	 * This is so that we know, on map load, that this is the first load of
	 * this map. Used for facebook publish.
	 */
	$_SESSION['map_just_chosen'] = true;
	/**
	 * Es hat sich wahrscheinlich der State geändert. Daher müssen die
	 * Userdaten nochmal neu vom Server geholt werden.
	 */
	$current_user = User::get_instance_by_id($current_user->get_id(), true);
	$r->add_value(1);
	die($r);
}
/**
 * Spieler hat sich gerade für eine Karte entschieden
 */
else if (isset($GP['chosen_map_index'])) {
	$r = new ClientServerResponse();

	try {
		$map_chooser->choose_map($GP['chosen_map_index']);
	}
	catch (Exception $e) {

	}
	/**
	 * This is so that we know, on map load, that this is the first load of
	 * this map. Used for facebook publish.
	 */
	$_SESSION['map_just_chosen'] = true;
	/**
	 * Es hat sich wahrscheinlich der State geändert. Daher müssen die
	 * Userdaten nochmal neu vom Server geholt werden.
	 */
	$current_user = User::get_instance_by_id($current_user->get_id(), true);
	$r->add_value(1);
	die($r);
}
/**
 * Spieler möchte die Inselwahl an das System abgeben. Es wird für ihn
 * gewählt.
 */
else if (isset($GP['choose_map_randomly'])) {
	$r = new ClientServerResponse();

	try {
		$map_chooser->choose_random_map();
	}
	catch (Exception $e) {

	}
	/**
	 * This is so that we know, on map load, that this is the first load of
	 * this map. Used for facebook publish.
	 */
	$_SESSION['map_just_chosen'] = true;
	/**
	 * Es hat sich wahrscheinlich der State geändert. Daher müssen die
	 * Userdaten nochmal neu vom Server geholt werden.
	 */
	$current_user = User::get_instance_by_id($current_user->get_id(), true);
	/**
	 * Farbe konnte er bisher nicht wählen, daher jetzt.
	 */
	//TODO: EventMessages::add_entry(EventMessages::TYPE_OPEN_COL_CHOOSE_DIALOG);
	$r->add_value(1);
	die($r);
}
/**
 * Der Spieler hat einen Auth-Key und möchte der Insel beitreten, die zu
 * dem Auth-Key gehört.
 */
else if (isset($GP['choose_by_auth_key'])) {
	$r = new ClientServerResponse();
	try {
		/**
		 * Wenn ein ganz neuer Spieler eine Mentorenkarte startet, dann wird
		 * das Tutorial dem Spieler noch gezeigt, wo die Einladungen am Ende
		 * erklärt werden. Deswegen wollen wir nicht direkt beim Siedeln
		 * einladen. Sonst schon! :)
		 */
		$no_invitations = (User::get_current_user()->get_state()
							== IUser::STATE_NEW_PLAYER);

		$map_chooser->choose_map_by_auth_key($GP['choose_by_auth_key'],
			$GP['home'], $GP['color'], $no_invitations);

		/**
		 * This is so that we know, on map load, that this is the first load
		 * of this map. Used for facebook publish.
		 */
		$_SESSION['map_just_chosen'] = true;

		/**
		 * Es hat sich wahrscheinlich der State geändert. Daher müssen die
		 * Userdaten nochmal neu vom Server geholt werden.
		 */
		$current_user =
					User::get_instance_by_id($current_user->get_id(), true);
		$r->add_value(1);
	}
	catch (Exception $e)
	{
		/**
		 * The map change was rejected because (most likely) the island was
		 * already full.
		 *
		$r->add_value(0);
		$r->add_value($e->getCode());*/
	}

	die($r);
}
/**
 * Spieler möchte die Karte nicht mehr gewählt haben, sondern sich eine
 * andere aussuchen => zurück zum Zustand, wo er nichts gewählt hat
 */
else if (isset($GP['unchoose_map'])) {
	$map_chooser->unchoose_map();
	/**
	 * Es hat sich wahrscheinlich der State geändert. Daher müssen die
	 * Userdaten nochmal neu vom Server geholt werden.
	 */
	$current_user = User::get_instance_by_id($current_user->get_id(), true);
}

if ($current_user->get_state() == User::STATE_PLAYING) {
	include('templates/ajax_redirect.tmpl.inc');
}
else {
	switch ($current_user->get_waiting_state()) {
		case 0:
		case User::STATE_CHOOSING_MAPS:
			if (@$GP['start_new_map']) {
				/**
				 * Er hat alles gewählt, um eine neue Karte zu starten
				 */
				if ((@$GP['template_id']) && (@$GP['invitations'])) {
					$invitations = explode(',', $GP['invitations']);
					$n = count($invitations);
					/**
					 * Herausfinden, in welcher Welt eröffnet werden muss
					 */
					$min = INF;
					for ($i = 0; $i < $n; $i++) {
						$lvl = User::get_instance_by_nick($invitations[$i])
							->get_cur_level();
						if ($lvl < $min)
							$min = $lvl;
					}
					$lvl = ($min < 6) ? 5 : ceil($min/5)*5;
					if ($lvl > $current_user->get_cur_level())
						$lvl = $current_user->get_cur_level();

					$data = Worlds::get_world_data_by_level($lvl,
						Worlds::MAP_TYPE_FFA);

					/**
					 * Insel erzeugen und starten
					 */
					MapChooserPrivate::get_instance($current_user->get_id(),
						$data->id)->open_new_normal_map($GP['template_id'],
						null, $GP['home'], $GP['color']);

					/**
					 * Freunde einladen
					 */
					$n = count($invitations);
					$friends = array();
					for ($i = 0; $i < $n; $i++) {
						$friend = new stdClass();
						$friend->mammun_nick = trim($invitations[$i]);
						$friends[] = $friend;
					}
					try {
						Server::get_instance(CONF_ENGINE_SERVER_URL)
							->cmd_invite_friends($friends, '', true);
					}
					catch(Exception $e) {
						error_log("File ".__FILE__." line ".__LINE__.": ".
							"Fehler beim initialen Einladen der Freunde ".
							"auf die Insel direkt nach der Eröffnung.\n".
							"Mindestens einer dieser Freunde hat ".
							"fehlerhafte Daten in der DB: ".
							print_r($friends(true)));
					}

					/**
					 * This is so that we know, on map load, that this is
					 * the first load of this map. Used for facebook
					 * publish.
					 */
					$_SESSION['map_just_chosen'] = true;
					/**
					 * Der State hat sich sicher geändert. Da er gecacht
					 * wird, holen wir die neuen Userdaten neu vom Server
					 * (clear cache).
					 */
					$current_user = User::get_instance_by_id(
						$current_user->get_id(), true);

					$r = new ClientServerResponse();
					$r->add_value(1);
					die($r);
				}
				/**
				 * Er hat noch nicht alles gewählt => Template anzeigen
				 * zum Auswählen
				 */
				else {
					/**
					 * Alle Templates, die er auswählen kann
					 */
					$map_templates =
						MapChooserPrivate::get_instance($current_user->get_id())
							->get_available_templates();

					$chosen_template = $map_templates[6];

					include('templates/choose_map/'.
						'private_create_new_map.tmpl.inc');
				}
				break;
			}
			$all_nicks = array();

			$auth_keys_w_data = $current_user->get_auth_keys_with_data();
			$auth_keys = array();
			$n = count($auth_keys_w_data);
			for ($i = 0; $i < $n; $i++)
				$auth_keys[] = $auth_keys_w_data[$i]->auth_key;

			try {
				$map_invitations
					= MapChooserPrivate::get_data_by_auth_keys($auth_keys);
			}
			catch (Exception $e) {
				$map_invitations = array();
			}

			$auto_choose_map_invitation = -1;


			/**
			 * Merge $auth_keys_w_data into $map_invitations
			 */
			$m = count($map_invitations);
			for ($j = 0; $j < $m; $j++) {
				for ($i = 0; $i < $n; $i++) {
					if ($map_invitations[$j]->auth_key
						== $auth_keys_w_data[$i]->auth_key)
					{
						$map_invitations[$j]->received_by =
							User::get_instance_by_id(
							$auth_keys_w_data[$i]->received_by)->get_nick();

						$all_nicks[] = $map_invitations[$j]->received_by;
					}
				}
				$data = Server::get_instance(CONF_ENGINE_SERVER_URL)
					->get_template_data(
						$map_invitations[$j]->world_id,
						$map_invitations[$j]->map_id
					);
				$map_invitations[$j]->template_id = $data->template_id;
				$map_invitations[$j]->max_players = $data->max_players;

				$map_invitations[$j]->players_on_map = array();
				$mp = MapPlayers::get_instance(
					$map_invitations[$j]->world_id,
					$map_invitations[$j]->map_id
				);
				$p = $mp->get_users_on_map();
				$p_n = count($p);
				for ($i = 0; $i < $p_n; $i++) {
					$tmp = new stdClass();
					$map_invitations[$j]->players_on_map[]
						= $p[$i]->get_nick();
					$all_nicks[] = $p[$i]->get_nick();
				}

				$map_invitations[$j]->choose_map_href =
					'choose_map.php?dataX='.$cryptObj->do_encryption(array(
						'choose_by_auth_key'
								=> $map_invitations[$j]->auth_key,
						'world_id'
								=> floor($map_invitations[$j]->world_id)
					));


				if (isset($_SESSION['join_map']) &&
					($_SESSION['join_map']['world_id'] 
						== $map_invitations[$j]->world_id)
					&& ($_SESSION['join_map']['map_id']
						== $map_invitations[$j]->map_id))
				{
					$auto_choose_map_invitation = $j;
				}
			}
			unset($auth_keys_w_data);	// not needed no more


			$avatar_data = Avatar::get_avatars($all_nicks);

			/**
			 * Alle Templates, die er auswählen kann
			 */
			$map_templates =
				MapChooserPrivate::get_instance($current_user->get_id())
					->get_available_templates();

			foreach ($map_templates as $i => $template) {
				if ($template->template_id == 103)
					$chosen_template = $map_templates[$i];
			}

			include('templates/choose_map/private_choose_map.tmpl.inc');
			break;
		/**
		 * Die Karte wird gestartet, oder es ist irgendwas schräg.
		 * In jedem Fall reload nötig.
		 */
		default:
				include('templates/ajax_redirect.tmpl.inc');
	}
}
if ($AJAX_OUTPUT) {
	$r->add_value(ob_get_clean());
	echo $r;
}
else
	ob_flush();
?>
