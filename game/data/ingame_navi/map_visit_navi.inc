<?php
require_once('../include/classes/Avatar.inc');

/**
 * Meine Freunde
 */
$buddies = ClientServer::send_request(
	CONF_WCF_SERVER_URL.'/gausus_wcf_hacks/friend_interface.php'
		.'?get_friends='.User::get_current_user()->get_nick()
);
$buddies = $buddies[0];

$map_players = MapPlayers::get_instance(
	User::get_current_user()->get_world_id(),
	User::get_current_user()->get_map_id()
);
$uids_on_map = $map_players->get_uids_on_map(true);
$all_nicks = array();
$all_map_ids = array();
$i = 0;
foreach ($buddies as $type => $data_arr) {
	shuffle($data_arr);
	foreach ($data_arr as $data) {
		$nick = $data->nick;
		$user = User::get_instance_by_nick($nick);
		if ($user->get_map_id() <= 0)
			continue;
		if (in_array($user->get_id(), $uids_on_map))
			continue;
		$all_nicks[] = $nick;
		$all_map_ids[$nick] = array(
			$user->get_world_id(), $user->get_map_id()
		);

		$i++;
		if ($i >= 2)
			break;
	}
}
/**
 * Jetzt die Avatare erfragen
 */
if (!empty($all_nicks))
	$avatar_data = Avatar::get_avatars($all_nicks);
else
	$avatar_data = array();

include('templates/ingame_navi/map_visit_navi.tmpl.inc');
?>