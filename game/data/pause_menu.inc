<?php
/**
 * File created by Philipp Kemmeter on 13.02.2009 at 14:25:03
 */

include_once ('../include/std_include.inc');
include_once('../include/classes/map/MapPlayers.inc');
include_once('../include/classes/ScoreCache.inc');
include_once('../include/classes/EventLog.inc');
require_once('../include/classes/MammunFBWrapper.inc');
require_once('../include/classes/Avatar.inc');
Language::get_instance()->load_language_file("facebook_publish");

if (isset($_GET['ajaxid'])) {// Ein ping-und-echo-Parameter für AJAX-Request-Zuweisungen
	$AJAX_OUTPUT = true;
	$r = new ClientServerResponse();
	$r->add_value($_GET['ajaxid']);
	$this_win_id = $_GET['ajaxid'];
}
else
	$AJAX_OUTPUT = false;
ob_start();


Language::get_instance()->load_language_file('building_action');
Language::get_instance()->load_language_file('mini_diplomacy_tab');
Language::get_instance()->load_language_file('pause_menu.tmpl');

$current_user = User::get_current_user();

$expected_data = array (
	'get_event_log'	=> 'bool',
	'get_buddies' => 'bool'
);
$GP = $cryptObj->load_GET_POST($expected_data);



/**
 * Dieses Array wird mir allen Nicks gefüllt, die durch Avatare auf der Seite
 * vorkommen. Dann wird diese Liste gebündelt an den Community-Server geschickt,
 * um die Avatare zu erfragen, die dann den Nicks zugeordnet werden.
 */
$all_avatar_nicks = array();


$event_log = EventLog::get_my_log(
	$current_user->get_world_id(), $current_user->get_map_id()
);
if (isset($GP['get_event_log'])) {
	$r = new ClientServerResponse();
	$r->add_value(1);
	$r->add_value($event_log);
	die($r);
}

if (isset($GP['get_buddies']))
{
	$r = new ClientServerResponse();
	try
	{
		$buddies = ClientServer::send_request(
			CONF_WCF_SERVER_URL.'/gausus_wcf_hacks/friend_interface.php'
			.'?get_friends='.User::get_current_user()->get_nick());
		$buddies = $buddies[0];
		$r->add_value($buddies);
	}
	catch(Exception $e)
	{
		$r->add_value(0);
		$r->add_value($e);
	}
	die($r);
}

if ($current_user->get_state() == IUser::STATE_PLAYING) {
	$world_id = User::get_current_user()->get_world_id();
	$map_id= User::get_current_user()->get_map_id();
	$ranking_data = Server::get_instance(CONF_ENGINE_SERVER_URL)
		->multi_call(array(
				"get_map_ranking"		=>	array($world_id, $map_id),
				"get_map_score_gains"	=>	array($world_id, $map_id)
			));
	$map_ranking = $ranking_data[0];
	$map_score_gains = $ranking_data[1];
}
else {
	$world_id = 0;
	$map_id = 0;
	$map_ranking = array();
	$map_score_gains = array();
}

/**
 * Wir wissen nun die aktuelle Punktzahl aller Spieler. Diese wird deswegen
 * gleich an den ScoreCache weitergegeben, damit sie in der Session gespeichert
 * wird, um Servernachfragen zu vermeiden und den Cache auf den neusten Stand
 * zu bringen.
 */
for ($i = 0; $i < count($map_ranking); $i++) {
	ScoreCache::set_map_score($world_id, $map_id, $map_ranking[$i]->uid,
											$map_ranking[$i]->score);
}

$obj_map_players = MapPlayers::get_instance($world_id, $map_id);


$users = $obj_map_players->get_users_on_map();
foreach ($users as $user)
	$all_avatar_nicks[] = $user->get_nick();

$i_have_won =
	($obj_map_players->get_user_state_on_map($current_user->get_id())
		== MapPlayers::STATE_VICTOR);

/**
 * Community-Zeugs: 1. Freunde
 */
$buddies = ClientServer::send_request(
	CONF_WCF_SERVER_URL.'/gausus_wcf_hacks/friend_interface.php'
		.'?get_friends='.User::get_current_user()->get_nick()
);
$buddies = $buddies[0];
$xp_assoc = null;
foreach ($buddies as $type => $data_arr) {
	foreach ($data_arr as $data) {
		$all_avatar_nicks[] = $data->nick;
		$xp_assoc[$data->nick] = User::get_instance_by_nick($data->nick)
			->get_cur_level();
	}
}

$WCF_user_ID = $current_user->get_personal_data()->id_wcf;

/**
 * Community-Zeugs: 2. Welt
 */
$random_user_json = ClientServer::send_request(
	CONF_WCF_SERVER_URL.'/gausus_wcf_hacks/get_random_world_user.php'
		.'?exclude='.$current_user->get_nick(),
	true
);
$random_user = json_decode($random_user_json);
foreach ($random_user as $type => $data)
	$all_avatar_nicks[] = $data->nick;

/**
 * Jetzt die Avatare erfragen
 */
$avatar_data = Avatar::get_avatars($all_avatar_nicks);

include_once('templates/pause_menu.tmpl.inc');
if ($AJAX_OUTPUT) {
	$r->add_value(ob_get_clean());
	die($r);
}
else
	ob_flush();
?>
