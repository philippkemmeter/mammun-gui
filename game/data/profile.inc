<?
/**
 * Diese Datei zeigt Forschungsstatusse und Dialoge an. Per AJAX geladen.
 */
require_once ('../include/std_include.inc');
require_once('../include/classes/MapDescriptionData.inc');
require_once('../include/shared/classes/ItemConstants.inc');
require_once('../include/shared/classes/datastructures/UnitClothes.inc');
require_once('../include/classes/Avatar.inc');
require_once('../include/classes/Medals.inc');

if (isset($_GET['ajaxid'])) {// Ein ping-und-echo-Parameter für AJAX-Request-Zuweisungen
	$AJAX_OUTPUT = true;
	$r = new ClientServerResponse();
	$r->add_value($_GET['ajaxid']);
	$this_win_id = $_GET['ajaxid'];
}
ob_start();
Language::get_instance()->load_language_file('profile');
Language::get_instance()->load_language_file('medals');
Language::get_instance()->load_language_file('countries_iso3166');
$current_user = User::get_current_user();

$expected_data = array (
	'nick'	=>		'string'
);

$GP = $cryptObj->load_GET_POST($expected_data);


/**
 * Dieses Array wird mir allen Nicks gefüllt, die durch Avatare auf der Seite
 * vorkommen. Dann wird diese Liste gebündelt an den Community-Server geschickt,
 * um die Avatare zu erfragen, die dann den Nicks zugeordnet werden.
 */
$all_avatar_nicks = array();


$profile_user = User::get_instance_by_nick($GP['nick']);
if ($profile_user->get_state() == USER::STATE_PLAYING) {
	$profile_world_id = $profile_user->get_world_id();
	$profile_map_id = $profile_user->get_map_id();
}
else {
	$profile_world_id = 0;
	$profile_map_id = 0;
}

if ($profile_world_id) {
	$x = Server::get_instance(CONF_ENGINE_SERVER_URL)->get_template_data(
		$profile_world_id,
		$profile_map_id
	);
	MapDescriptionData::set_instance($x->world_id, $x->map_id, $x);
	$template_id = MapDescriptionData::get_instance($x->world_id, $x->map_id)
		->get_template_id();
	if (!$template_id)
		$template_id = 0;
}
else
	$template_id = 0;

$won_maps = Server::get_instance(CONF_ENGINE_SERVER_URL)
	->get_maps_won_by_user($profile_user->get_id());

$profile_user_personal = $profile_user->get_personal_data();
$current_user_personal = $current_user->get_personal_data();
/**
 * Ist der User mein Freund?
 */
$friend_status = ClientServer::send_request(
	CONF_WCF_SERVER_URL.'/gausus_wcf_hacks/friend_interface.php?is_my_friend='.
		$profile_user_personal->id_wcf.'&s='.$_SESSION['WCF_SID']
);
$friend_status = $friend_status[0];
/**
 * Seine Freunde
 */
$buddies = ClientServer::send_request(
	CONF_WCF_SERVER_URL.'/gausus_wcf_hacks/friend_interface.php'
		.'?get_friends='.$profile_user->get_nick()
);
$buddies = $buddies[0];
foreach ($buddies as $type => $data_arr)
	foreach ($data_arr as $data)
		$all_avatar_nicks[] = $data->nick;

/**
 * Map-Ranking ermitteln
 */
if ($profile_world_id) {
	$map_ranking =
		MapPlayers::get_instance($profile_world_id, $profile_map_id)
			->get_users_on_map();
	foreach ($map_ranking as $user)
		$all_avatar_nicks[] = $user->get_nick();
}
else {
	$map_ranking = array();
}

//
///**
// * Seine Schätze
// */
//$uid = $profile_user->get_id();
//$all_items = Converter::stdClassToArray(
//	Server::get_instance(CONF_PAYMENT_SERVER_URL, CONF_MAMMUN_AUTH_SECRET)
//		->get_my_items($uid, 0)
//);
//
///**
// * Die erhaltenen Items für die Ausgabe konvertieren. Regeln sind:
// * 1. Keine User-IDs nur Nicks
// * 2. Timestamps sollen menschenlesbar sein
// * 3. Bestimmte Infos filtern
// * 4. Name und Beschreibung der Items hinzufügen
// */
//$item_amount = 0;
//$my_items = array();
//$cost = array();
//foreach ($all_items as $type => $items) {
//	foreach ($items as $i => $data) {
//		/**
//		 * Nur die anzeigen, die im Schaufenster sind.
//		 */
//		if ($all_items[$type][$i]->amount_mp <= 0) {
//			unset($all_items[$type][$i]);
//			continue;
//		}
//		unset($data->offers);
//		$all_items[$type][$i]->name
//			= ItemConstants::$ITEM_NAMES[$type][$data->id];
//		$all_items[$type][$i]->desc
//			= ItemConstants::$ITEM_DESCS[$type][$data->id];
//		$item_amount += $all_items[$type][$i]->amount_mp
//			+ $all_items[$type][$i]->amount;
//		$my_items[] = $all_items[$type][$i];
//		$cost[] = (int)($all_items[$type][$i]->shop_price);
//	}
//}
//
//array_multisort($cost, SORT_DESC, $my_items);
///*error_log(print_r($cost,true));
//error_log(print_r($my_items,true));*/
//
//$all_items_json = Converter::xToJson($my_items);

if ($profile_world_id) {
	try {
		$most_expensive_units = Server::get_instance(CONF_ENGINE_SERVER_URL)
			->get_most_expensive_units($profile_user->get_id(),
				$profile_world_id, $profile_map_id, 3);
	}
	catch(Exception $e) {
		$most_expensive_units = array();
	}
}

/**
 * Jetzt die Avatare erfragen
 */
$all_avatar_nicks[] = $profile_user->get_nick();
$avatar_data = Avatar::get_avatars($all_avatar_nicks);

/**
 * Medallien
 */
$my_medals_tmp = Medals::get_user_medals($profile_user->get_id());
$all_medals = Medals::get_medals(0,0);

// Nach Kategorie sortieren
$my_medals = null;
$max_amount = null;
foreach ($my_medals_tmp as $m) {
	$my_medals[$m->type][$m->subtype][$m->amount] = $m;
	$my_medals[$m->type][$m->subtype][$m->amount]->name =
		constant('LNG_MEDAL_'.$m->id.'_HEADLINE');
	$my_medals[$m->type][$m->subtype][$m->amount]->desc =
		constant('LNG_MEDAL_'.$m->id.'_TOOLTIP');

	if (!isset($max_amount[$m->type]) || ($m->amount > $max_amount[$m->type]))
		$max_amount["{$m->type}-{$m->subtype}"] = $m->amount;
}
$best_3_medals = array();
if ($max_amount) {
	arsort($max_amount);
	$i = 0;
	foreach ($max_amount as $type_subtype => $amt) {
		if ($i >= 3)
			break;

		list($type, $subtype) = explode('-', $type_subtype);
		$best_3_medals[] = $my_medals[$type][$subtype][$amt];
		$i++;
	}
}

$medal_cat_names = array();
foreach ($all_medals as $type => $foo)
	$medal_cat_names[$type] = @constant("LNG_MEDAL_CAT_{$type}_NAME");

include('templates/profile/profile.tmpl.inc');

if ($AJAX_OUTPUT) {
	$r->add_value(ob_get_clean());
	echo $r;
}
else
	ob_flush();
?>