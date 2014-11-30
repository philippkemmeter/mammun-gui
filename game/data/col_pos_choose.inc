<?php
include('../include/std_include.inc');
include_once('../include/classes/map/MapPlayers.inc');
include_once('../include/classes/MapChooserPrivate.inc');
include_once('../include/classes/Avatar.inc');

if (isset($_GET['ajaxid'])) {// Ein ping-und-echo-Parameter für AJAX-Request-Zuweisungen
	$AJAX_OUTPUT = true;
	$r = new ClientServerResponse();
	$r->add_value($_GET['ajaxid']);
	$this_win_id = $_GET['ajaxid'];
}
else
	$AJAX_OUTPUT = false;
ob_start();

$expected_data = array (
        'chosen_map_index'         		=>      'eint',
        'unchoose_map'              	=>      'ebool',
        'toggle_ready'               	=>      'ebool',
        'start_mentor_map'            	=>      'ebool',
        'world_id'     					=>      '(e)int',
        'get_available_map_data'       	=>      'ebool',
        'choose_map_randomly'           =>      'ebool',
        'choose_by_auth_key'            =>      '(e)string',
        'start_new_map'          		=>      'ebool',
        'template_id'        			=>      'int',
        'color'                         => 		'string',
        'home'                          => 		'int',
		'invite'						=>		'bool'
);
$GP = $cryptObj->load_GET_POST($expected_data);
$get_appendix = '';
foreach(array_merge($_GET, $_POST) as $key => $value)
	$get_appendix.='&'.$key.'='.$value;

$current_user = User::get_current_user();

// Daten noch nicht komplett
if (!isset($GP['home']) || !isset($GP['color']))
{
	if (isset($GP['start_mentor_map']) || isset($GP['start_new_map']))
	{
        $template_id = $GP['template_id'];      // ID ist mitgeschickt
        // Template-Daten vom Server holen
        $template_data = Server::get_instance(CONF_ENGINE_SERVER_URL)
                        ->get_template_data_t($template_id);

        // Mögliche Startpositionen auslesen
        $homesectors = $template_data->homesectors;
        // Keine Spieler auf der Insel bisher
        $players_on_map = array();
        $players_on_map_linear = array();
        $random_position = array_rand($homesectors, 1);
	}

	else {
		if (isset($GP['choose_by_auth_key'])) {
	        // Wir haben nur den Auth-Key aka Map-Key
	        $auth_key = $GP['choose_by_auth_key'];

	        // Wir holen alle Daten vom Server, die wir dazu bekommen
	        $map_choose_data = MapChooserPrivate::get_data_by_auth_key($auth_key);

	        // Dazu gehört auch die ID der Karte, aber nicht das template
	        $world_id = $map_choose_data->world_id;
	        $map_id = $map_choose_data->map_id;
		}
		else {
			$world_id = User::get_current_user()->get_world_id();
			$map_id = User::get_current_user()->get_map_id();
		}

        // Wir holen die Template-Daten über die Map-ID
        $data = Server::get_instance(CONF_ENGINE_SERVER_URL)
                                ->get_template_data($world_id, $map_id);
        $template_id = $data->template_id;
        $homesectors = $data->homesectors;

        $mp = MapPlayers::get_instance($world_id, $map_id);
        $users_on_map = $mp->get_users_on_map();
        $players_on_map_linear = array();
		foreach ($users_on_map as $user) {
			$hs = $mp->get_homesector($user->get_id()) % 100;
			$player = new stdClass();
			$player->color = $user->get_color();
			$player->nick = $user->get_nick();
			$players_on_map[$hs % 10][floor($hs / 10)] = $player;
			$players_on_map_linear[] = $player;
			$nicks[] = $player->nick;
		}

        // Jeder User auf der Karte ist ein User-Objekt und besitzt die
        // Funktion get_color, um die aktuell gewählte Farbe zu erhalten
	}

	$player_me = new stdClass();
	$player_me->color = User::get_current_user()->get_color();
	$player_me->nick = User::get_current_user()->get_nick();
	$nicks[] = $player_me->nick;
	$avatars = Avatar::get_avatars($nicks);

	$invite_mode = isset($GP['invite']) ? $GP['invite'] : false;

	include ('templates/col_pos_choose.tmpl.inc');
}
// Daten alle da: Weiter wie früher
else {
	if ($current_user->get_state() == IUser::STATE_PLAYING) {
		MapPlayers::get_instance(
			$current_user->get_world_id(), $current_user->get_map_id()
		)->change_color($current_user->get_id(), $GP['color']);
		$r = new ClientServerResponse();
		$r->add_value(1);
		die($r);
	}
	else {
		include ('choose_map.inc');
	}
}
if ($AJAX_OUTPUT) {
	$r->add_value(ob_get_clean());
	echo $r;
}
else
	ob_flush();

?>