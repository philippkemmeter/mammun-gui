<?php
require_once ('../include/std_include.inc');
Language::get_instance()->load_language_file("facebook_publish");
$state = User::get_current_user(true)->get_state();
if (($state == User::STATE_CHOOSING_MAPS) || ($state == User::STATE_NEW_PLAYER))
{
	include('choose_map.php');
	die();
}
else if (($state != User::STATE_CHOOSING_SCIENCES_LOST_LAST_MAP) &&
	($state != User::STATE_CHOOSING_SCIENCES_WON_LAST_MAP))
{
	echo "Wrong state...$state...{$_SERVER['PHP_SELF']}";
	kill_session();
}



require_once('../include/classes/Worlds.inc');


$r = new ClientServerResponse();
ob_start();
if (isset($_GET['ajaxid'])) {// Ein ping-und-echo-Parameter für AJAX-Request-Zuweisungen
	$r->add_value($_GET['ajaxid']);
	$this_win_id = $_GET['ajaxid'];
}
Language::get_instance()->load_language_file('island_left');


$expected_data = array (
	'choose_sciences'	=>	'ebool',
);
$GP = $cryptObj->load_GET_POST($expected_data);

if (@$GP['choose_sciences']) {
	try {
		$selected_sciences = Server::get_instance(CONF_ENGINE_SERVER_URL)
				->cmd_select_all_sciences_auto();
		$state = User::get_current_user(true)->get_state();
		include('choose_map.php');
		die();
	}
	catch (Exception $e) {
		error_log(print_r($e,true));
		die("A strange error occured. Please report this to an administrator.");
	}
}
$map_id = User::get_current_user()->get_map_id();
$world_id = User::get_current_user()->get_world_id();
$data = Server::get_instance(CONF_ENGINE_SERVER_URL)
	->get_template_data($world_id, $map_id);
$template_id = $data->template_id;

// $_SESSION['join_map'] wird in der ingame.inc gesetzt, wenn jemand
// eine Insel verlassen will und direkt weiß, wohin er wechseln will.
// siehe auch map_visit_navi.tmpl.inc
if (isset($_SESSION['join_map'])) {
	$dest_world_id = $_SESSION['join_map']['world_id'];
	$dest_map_id = $_SESSION['join_map']['map_id'];

	$users = MapPlayers::get_instance($dest_world_id, $dest_map_id)->
		get_users_on_map(true);
	
	$dest_user_nicks = array();
	for ($i = 0, $n = count($users); $i < $n; ++$i)
		$dest_user_nicks[] = '<b>'.$users[$i]->get_nick_main().'</b>';
}

if ($state == User::STATE_CHOOSING_SCIENCES_LOST_LAST_MAP) {
	if (isset($_SESSION['join_map'])) {
		$text = str_replace(
			array('[ISLAND_NAME]', '[USERS]'),
			array(
				Worlds::get_world_name($world_id) . " #" . $map_id,
				LanguagePrint::listing($dest_user_nicks)
			),
			LNG_MAP_LOST_TEXT_DEST
		);
	}
	else {
		$text = str_replace(
			'[ISLAND_NAME]',
			Worlds::get_world_name($world_id) . " #" . $map_id,
			LNG_MAP_LOST_TEXT
		);
	}
	include ('templates/island_left/map_lost.tmpl.inc');
}
else {
	if (isset($_SESSION['join_map'])) {
		$text = str_replace(
			array('[ISLAND_NAME]', '[USERS]'),
			array(
				Worlds::get_world_name($world_id) . " #" . $map_id,
				LanguagePrint::listing($dest_user_nicks)
			),
			LNG_MAP_WON_TEXT_DEST
		);
	}
	else {
		$text = str_replace(
			'[ISLAND_NAME]',
			Worlds::get_world_name($world_id) . " #" . $map_id,
			LNG_MAP_WON_TEXT
		);
	}
	include ('templates/island_left/map_won.tmpl.inc');
}
$r->add_value(ob_get_clean());
die($r);
?>
