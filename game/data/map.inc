<?php
/**
 * @author Philipp Kemmeter
 */
require_once ('../include/std_include.inc');
require_once ('../include/classes/MapDescriptionData.inc');
require_once ('../include/classes/Avatar.inc');
require_once ('../include/classes/buildings/Buildings.inc');
require_once ('../include/classes/Worlds.inc');
require_once ('../include/classes/map/MapPlayers.inc');
require_once ('../include/classes/EventMessages.inc');
require_once ('../include/classes/ScoreCache.inc');
Language::get_instance()->load_language_file("facebook_publish");
$expected_data = array (
	'world_id'					=>	'int',
	'map_id'					=>	'int',
	'update'					=>	'bool',
	'change_map'				=>	'bool',
	'change_color'				=>	'string',
	'get_preview_data'			=>	'bool',
	'get_friend_maps'			=>	'bool',
	'delete_message'			=>	'int',
	'clear_session_first_map'	=>	'bool',
	'get_friend_maps'			=>	'bool',
	'request_invitation'		=>	'bool'
);
$GP = $cryptObj->load_GET_POST($expected_data);

if(isset($GP['change_color'])) {
	$current_user = User::get_current_user();
	MapPlayers::get_instance(
		$current_user->get_world_id(),
		$current_user->get_map_id()
	)->change_color($current_user->get_id(), $GP['change_color']);
	die();
}

if(isset($GP['clear_session_first_map']))
{
	try
	{
		unset($_SESSION['map_just_chosen']);
	}
	catch(Exception $e)
	{
		die($e);
	}
	die(0);
}
if (!isset($map_id)) {
	if (isset($GP['map_id']))
		$map_id = $GP['map_id'];
	else if (User::get_current_user())
		$map_id = User::get_current_user()->get_map_id();
	else
		$map_id = 0;
}
if (!isset($world_id)) {
	if (isset($GP['world_id']))
		$world_id = $GP['world_id'];
	else if (User::get_current_user())
		$world_id = User::get_current_user()->get_world_id();
	else
		$world_id = 0;
}
if (isset($GP['delete_message'])) {
	EventMessages::delete_message($GP['delete_message']);
	/**
	 * Punkte-Cache für den aktuellen Spieler löschen, weil sich die Punkte
	 * verändert haben können durch die Nachricht.
	 */
	if (User::get_current_user()) {
		ScoreCache::clear_cache_league(0, User::get_current_user()->get_id());
		ScoreCache::clear_cache_map(0, User::get_current_user()->get_id());
	}
	die();
}

/**
 * Ein Update soll erfolgen. Alle Kartendaten werden angefordert. Bei der
 * Gelegenheit werden auch alle Spieldaten mitgeschickt.
 */
if (@$GP['update']) {
	$r = new ClientServerResponse();
	try{
		$uid = User::get_current_user()->get_id();
		$map_data = Server::get_instance(CONF_ENGINE_SERVER_URL)
			->get_map_data($uid, $world_id, $map_id);
		$r->add_value(1);
		$r->add_value($map_data->dominator);
		$r->add_value(Buildings::convert_server_buildings($map_data->building));
		$r->add_value($map_data->moving_units);
		$r->add_value($map_data->units);
		$r->add_value($map_data->fight);
		$r->add_value(null);
		$r->add_value(null);
		$r->add_value(EventMessages::get_my_log());
		$score_obj = new stdClass();
		$score_obj->map_score = ScoreCache::get_map_score($world_id, $map_id,
			$uid, true);
		$score_obj->league_score = ScoreCache::get_league_score(0, $uid, true);
		$r->add_value($score_obj);
	}
	catch(Exception $e) {
		$r->add_exception($e);
	}

	die($r);
}

if (isset($GP['request_invitation'])) {
	$r = new ClientServerResponse();
	$map_id = $GP['map_id'];
	$world_id = $GP['world_id'];
	$mp = MapPlayers::get_instance($world_id, $map_id);
	$uids_on_map = $mp->get_uids_on_map(true);
	Server::get_instance(CONF_ENGINE_SERVER_URL)
		->cmd_request_invitation($uids_on_map);
	$r->add_value(1);
	die($r);
}

if (isset($GP['get_preview_data'])) {
	$world_id = $GP['world_id'];
	$map_id = $GP['map_id'];
	require_once('../include/classes/MapChooserPrivate.inc');
	try {
		$auth_key_data = MapChooserPrivate::get_data_by_auth_keys(array());
		$is_invited = false;
		if (is_array($auth_key_data) || is_object($auth_key_data)) {
			foreach ($auth_key_data as $data) {
				if (($world_id == $data->world_id)
					&& ($map_id == $data->map_id))
				{
					$is_invited = true;
					break;
				}
			}
		}
	}
	catch(Exception $e) {
		$is_invited = false;
	}

	$r = new ClientServerResponse();
	try {
		$data = Server::get_instance(CONF_ENGINE_SERVER_URL)
			->get_template_data($world_id, $map_id);
		$template_id = $data->template_id;
		$max_players = $data->max_players;

		$mp = MapPlayers::get_instance($world_id, $map_id);
		$p = $mp->get_users_on_map();
		$n = $max_players;

		$players = array();
		$nicks = array();
		for ($i = 0; $i < $n; $i++) {
			$tmp = new stdClass();
			if (isset($p[$i])) {
				$tmp->nick = $p[$i]->get_nick();
				$tmp->alive = ($mp->get_user_state_on_map($p[$i]->get_id())
											== MapPlayers::STATE_STILL_ALIVE);
				$tmp->color = $mp->get_color($p[$i]->get_id());
			}
			else {
				$tmp->nick = '';
				$tmp->color = constant('MapPlayers::PLAYER_COLOR'.($i+1));
			}
			$players[] = $tmp;
			$nicks[] = $tmp->nick;
		}
		if (count($p) > 0)
			$avatars = Avatar::get_avatars($nicks);
		else
			$avatars = array();

		$r->add_value(1);
		$r->add_value($world_id);
		$r->add_value($map_id);
		$r->add_value($template_id);
		$r->add_value($max_players);
		$r->add_value($players);
		$r->add_value($avatars);
		$r->add_value($is_invited ? 1 : 0);

	}
	/*catch (IllegalArgumentException $e) {
		$r->add_value(0);
		$r->add_value($map_id);
	}*/
	catch (Exception $e) {
		//$r->add_exception($e);
		$r->add_value(0);
		$r->add_value($world_id);
		$r->add_value($map_id);
	}
	die($r);
}

if (@$GP['get_friend_maps']) {
	$r = new ClientServerResponse();

	/**
	 * Meine Freunde
	 */
	$buddies = ClientServer::send_request(
		CONF_WCF_SERVER_URL.'/gausus_wcf_hacks/friend_interface.php'
			.'?get_friends='.User::get_current_user()->get_nick()
	);
	$buddies = $buddies[0];

	$all_nicks = array();
	foreach ($buddies as $type => $data_arr)
		foreach ($data_arr as $data)
			$all_nicks[] = $data->nick;

	$maps = (array)(Server::get_instance(CONF_ENGINE_SERVER_URL)
		->get_current_maps_of_users($all_nicks, true));

	$maps_tmp = array_values($maps);
	$maps = array();
	$n = count($maps_tmp);
	for ($i = 0; $i < $n; $i++) {
		$double_exists = false;
		for ($j = $i+1; $j < $n; $j++) {
			if ($maps_tmp[$i] == $maps_tmp[$j]) {
				$double_exists = true;
				break;
			}
		}
		if (!$double_exists)
			$maps[] = $maps_tmp[$i];
	}

	function tmp_cmp($a, $b) {
		if ($a[0] == $b[0]) {
			if ($a[1] == $b[1])
				return 0;
			return ($a[1] < $b[1]) ? -1 : 1;
		}
		return ($a[0] < $b[0]) ? -1 : 1;
	}
	usort($maps, "tmp_cmp");

	$r->add_value(1);
	$r->add_value($maps);
	die($r);
}

/**
 * Erzeugt die Spielkarte und gibt sie aus
 */
try {
	$current_user = User::get_current_user();
	$x = ClientServer::send_request(
		CONF_ENGINE_SERVER_URL."?get_template_data=".serialize(
			array($world_id, $map_id))."&get_map_data=".serialize(
			array($current_user->get_id(), $world_id, $map_id))
	);
	$map_desc = MapDescriptionData::set_instance(
		$x[0]->world_id, $x[0]->map_id, $x[0]);
	$template_id = $map_desc->get_template_id();
	$max_players = $map_desc->get_max_players();
	$map_data = $x[1];

	/** TODO: HACK! **/
	if (($current_user->get_world_id() == $world_id)
		&& ($current_user->get_map_id() == $map_id))
	{
		$_SESSION['MapDescription']['mentor_map'] = $map_data->mentor_map;
		$_SESSION['MapDescription']['closed'] = $map_data->closed;
	}

	$mp = MapPlayers::get_instance($world_id, $map_id, true);
	$users = $mp->get_users_on_map();
	$user_colors = new stdClass();
	$nicks = array();
	$players = array();
	foreach ($users as $user) {
		$user_colors->{$user->get_nick()} =	$mp->get_color($user->get_id());
		$nicks[] = $user->get_nick();
		$tmp = new stdClass();
		$tmp->nick = $user->get_nick();
		$tmp->alive = ($mp->get_user_state_on_map($user->get_id())
									== MapPlayers::STATE_STILL_ALIVE);
		$tmp->color = $mp->get_color($user->get_id());
		$players[] = $tmp;
	}
	$avatar_data = Avatar::get_avatars($nicks);
	$world_name = Worlds::get_world_name($world_id, IWorld::MAP_TYPE_FFA);

	$event_log = EventMessages::get_my_log();
}
catch(Exception $e) {
	$ex = $e;
}

/**
 * Das Template ist bereits geladen, aber die Daten sollen alle aktuallisiert
 * werden, weil die Karte gewechselt wurde:
 */
if (@$GP['change_map']) {
	$r = new ClientServerResponse();
	if (isset($ex)) {
		$r->add_exception($ex);
	}
	else {
		$o = new stdClass();
		$o->template_id = $template_id;
		$o->avatar_data = $avatar_data;
		$o->max_players = $max_players;
		$o->world_name = $world_name;
		$r->add_value($o);
		$r->add_value($map_data->dominator);
		$r->add_value(Buildings::convert_server_buildings($map_data->building));
		$r->add_value($map_data->moving_units);
		$r->add_value($map_data->units);
		$r->add_value($map_data->fight);
		$r->add_value($map_data->bio);
		$r->add_value($user_colors);
		$r->add_value((int)$map_data->mentor_map);
		$r->add_value((int)$map_data->closed);
	}
	die($r);
}
if (isset($ex)) {
	throw $ex;
}

include('templates/map.tmpl.inc');
?>
