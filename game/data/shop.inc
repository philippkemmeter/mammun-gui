<?php
include_once ('../include/std_include.inc');
$r = new ClientServerResponse();
if (isset($_GET['ajaxid'])) {// Ein ping-und-echo-Parameter für AJAX-Request-Zuweisungen
	$AJAX_OUTPUT = true;
	$r->add_value($_GET['ajaxid']);
	$this_win_id = $_GET['ajaxid'];
}
else
	$AJAX_OUTPUT = false;
ob_start();

require_once('../include/shared/classes/datastructures/UnitClothes.inc');
require_once('../include/shared/classes/ItemConstants.inc');
require_once('../include/classes/Techtree.inc');
require_once('../include/classes/Avatar.inc');

$current_user = User::get_current_user();

Language::get_instance()->load_language_file('shop');
Language::get_instance()->load_language_file('item_details');

$expected_data = array (
	'show_chest_item_list'	=>	'bool',
	'get_chest_item_list'	=>	'bool',
	'type'					=>	'int',
	'id'					=>	'int',
	'buy'					=>	'bool',
	'amount'				=>	'int'
);
$GP = $cryptObj->load_GET_POST($expected_data);


/**
 * Anzeige aller Items des Typs in Schatzkisten. Wird nur erzeugt, wenn Schatz
 * ausverkauft.
 */
if (($GP['show_chest_item_list']) || $GP['get_chest_item_list']) {
	$all_avatar_nicks = array();
	/**
	 * Freunde vom Community-Server holen
	 */
	$friends = ClientServer::send_request(
		CONF_WCF_SERVER_URL.'/gausus_wcf_hacks/friend_interface.php'
			.'?get_friends='.User::get_current_user()->get_nick()
	);
	$friends = $friends[0];
	/**
	 * ... und in die entsprechende Form bringen (einfaches Array von UIDs)
	 * Dazu dient die Hilfsfunktion conver_friends
	 */
	if (!is_object($friends))
		$friends = array();
	else {
		$friends = array_merge((array)$friends->onlineFriends,
			(array)$friends->offlineFriends);
		array_walk($friends, 'convert_friends');
	}
	// Nun Schatzkistenanfrage explizit der Freunde.
	if (empty($friends)) {
		$chest_items_friends = array();
	}
	else {
		$chest_items_friends = Server::get_instance(CONF_PAYMENT_SERVER_URL,
			CONF_MAMMUN_AUTH_SECRET)
				->get_shop_alternative_chest_items($GP['type'], $GP['id'], 5,
					$friends, array($current_user->get_id()));
	}

	/**
	 * Spieler auf der Karte
	 */
	$map_excludes = array_merge($friends, array($current_user->get_id()));
	$map_players = MapPlayers::get_instance(
		$current_user->get_world_id(),
		$current_user->get_map_id()
	)->get_uids_on_map();

	// Schatzkistenanfrage explizit Kartenmitspieler
	if (empty($map_players)) {
		$chest_items_map = array();
	}
	else {
		$chest_items_map = Server::get_instance(CONF_PAYMENT_SERVER_URL,
			CONF_MAMMUN_AUTH_SECRET)
				->get_shop_alternative_chest_items($GP['type'], $GP['id'], 5,
					$map_players, $map_excludes);
	}

	// Schatzkistenanfrage Welt
	$chest_items_world = Server::get_instance(CONF_PAYMENT_SERVER_URL,
		CONF_MAMMUN_AUTH_SECRET)
			->get_shop_alternative_chest_items($GP['type'], $GP['id'], 5,
				array(), array_merge($map_excludes, $map_players));

	/**
	 * Ziel: Es sollen im optimalen Fall 2 Freunde mit Direktpreis, einer mit
	 * Trade, sowie ein Kartenspieler und ein Weltweit angezeigt werden, wobei
	 * von den letzteren beiden genau einer Trade und einer Direkt hat (welcher
	 * ist egal). Falls kein Freund mit Trade vorhanden, soll dieser durch
	 * einen Kartenspieler oder einen Weltspieler ersetzt werden. Dasselbe gilt
	 * für den Fall, dass es zu wenig Direktpreis-Freunde gibt.
	 *
	 * Beim Fehlen werden zunächst die Mitspieler auf der Karte als
	 * Freundersatz benutzt, dann als letzter Fallback die Welt.
	 *
	 * Der Worstcase ist gegeben, wenn keine Freund-Items und keine Karten-Items
	 * existieren, und alles aus der Welt geholt werden muss, und dort nur
	 * direct- oder nur trade-Angebote liegen. Der Fall wird als schlimmer
	 * angesehen, als wenn es einfach keine Kisten gibt, in denen etwas
	 * drinliegt.
	 */
	$chest_items = array();
	/**
	 * Freundangebot hinzufügen [direct, trade, direct].
	 */
	$n_direct = 0;
	$n_trade = 0;
	$n_world_direct = 0;
	$n_map_direct = 0;
	$n_friend_direct = 0;
	$n_world_trade = 0;
	$n_map_trade = 0;
	$n_friend_trade = 0;

	/**
	 * Hilfsfunktion zum Ändern der Daten für die Ausgabe
	 * @param stdClass $item
	 */
	function jsout($item) {
		global $cryptObj;
		global $all_avatar_nicks;
		$item->dataX = $cryptObj->do_encryption(array(
			'uid'	=>	$item->uid,
		));
		$item->nick = User::get_instance_by_id($item->uid)->get_nick();
		$all_avatar_nicks[] = $item->nick;	// Nick merken für Avatare
		unset($item->uid);
		return $item;
	}

	if (isset($chest_items_friends->direct[$n_friend_direct])) {
		$chest_items[] = jsout($chest_items_friends->direct[$n_friend_direct]);
		$n_direct++;
		$n_friend_direct++;
	}
	if (isset($chest_items_friends->trade[$n_friend_trade])) {
		$chest_items[] = jsout($chest_items_friends->trade[$n_friend_trade]);
		$n_trade++;
		$n_friend_trade++;
	}
	if (isset($chest_items_friends->direct[$n_friend_direct])) {
		$chest_items[] = jsout($chest_items_friends->direct[$n_friend_direct]);
		$n_direct++;
		$n_friend_direct++;
	}

	/**
	 * Ggf. nicht existente Freund-Direkt-Angebote versuchen mit Freund-Trade
	 * zu ersetzen
	 */
	$n = 3 - ($n_direct + $n_trade);
	for ($i = 0; $i < $n; $i++) {
		if (isset($chest_items_friends->trade[$n_friend_trade])) {
			$chest_items[] = jsout($chest_items_friends->trade[$n_friend_trade]);
			$n_trade++;
			$n_friend_trade++;
		}
	}
	/**
	 * Jetzt noch fehlende Freund-Direkt-Angebote durch Map-Direkt ersetzen
	 */
	$n = 2 - $n_direct;
	for ($i = 0; $i < $n; $i++) {
		if (isset($chest_items_map->direct[$n_map_direct])) {
			$chest_items[] = jsout($chest_items_map->direct[$n_map_direct]);
			$n_direct++;
			$n_map_direct++;
		}
	}
	/**
	 * Nun das fehlende Direkt durch Welt-Direkt ersetzen.
	 */
	$n = 2 - $n_direct;
	for ($i = 0; $i < $n; $i++) {
		if (isset($chest_items_world->direct[$n_world_direct])) {
			$chest_items[] = jsout($chest_items_world->direct[$n_world_direct]);
			$n_direct++;
			$n_world_direct++;
		}
	}

	/**
	 * Zwischenstand:
	 *
	 * Jetzt haben wir höchsten zwei Directs und insgesamt höchsten 3 Items
	 * eingefügt.
	 * Restlichen Items die auf fünf fehlen durch Trade-Items ersetzen.
	 */
	/**
	 * Einer von der Welt soll auf jeden Fall dabei sein
	 */
	if (($n_world_direct == 0) && (isset($chest_items_world->trade[$n_world_trade]))) {
		$chest_items[] = jsout($chest_items_world->trade[$n_world_trade]);
		$n_trade++;
		$n_world_trade++;
	}
	/**
	 * Einer von der Map soll auf jeden Fall dabei sein
	 */
	if (($n_map_direct == 0) && (isset($chest_items_map->trade[$n_map_trade]))) {
		$chest_items[] = jsout($chest_items_map->trade[$n_map_trade]);
		$n_trade++;
		$n_map_trade++;
	}

	/**
	 * Das jetzt noch existent Defizit soll in der Reihenfolge versucht
	 * werden zu füllen:
	 * 1. Freund-Trades
	 * 2. Map-Trades
	 * 3. Welt-Trades
	 * 4. Welt-Directs
	 * 5. Map-Directs
	 * 6. Freund-Directs
	 */
	$trys = array(
		array(
			$chest_items_friends->trade,
			$chest_items_map->trade,
			$chest_items_world->trade,
			$chest_items_world->direct,
			$chest_items_map->direct,
			$chest_items_friends->direct
		),
		array(
			$n_friend_trade,
			$n_map_trade,
			$n_world_trade,
			$n_world_direct,
			$n_map_direct,
			$n_friend_direct
		)
	);
	$try_n = count($trys[0]);
	$n_total = $n_direct + $n_trade;
	for ($try_i = 0; $try_i < $try_n; $try_i++) {
		$needed = 5 - $n_total;
		for ($i = 0; $i < $needed; $i++) {
			if (isset($trys[0][$try_i][$trys[1][$try_i]])) {
				$chest_items[] = jsout($trys[0][$try_i][$trys[1][$try_i]]);
				$n_total++;
				$trys[1][$try_i]++;
			}
		}
	}

	$avatar_data = Avatar::get_avatars($all_avatar_nicks);


	if ($GP['get_chest_item_list']) {
		$r->add_value(1);
		$r->add_value(json_encode($chest_items));
	}
	else {
		include('templates/shop/chest_item_list.tmpl.inc');
		$r->add_value(ob_get_clean());
	}
	die($r);
}
else if ($GP['buy']) {
	$restrictions = Server::get_instance(CONF_ENGINE_SERVER_URL)
			->get_item_restrictions($current_user->get_id(),0);

/*if (isset($restrictions->{$GP['type']}->{$GP['id']})
		&& (!$restrictions->{$GP['type']}->{$GP['id']}->may_buy_from_shop))
	{
		$r->add_value(0);
		$r->add_value(
			str_replace(
				array('[LINK]', '[/LINK]', '[AMT]'),
				array(
					'<a href="'
						. 'javascript: UIButtons.open_friend_invitation(); '
						. 'MammunUI.destroy_warning();">',
					'</a>', $restrictions->{$GP['type']}->{$GP['id']}->brezeln
				),
				LNG_NOT_ENOUGH_BREZELN_ERR
			)
		);
		die($r);
	}*/
			
	try {

		$x = Server::get_instance(CONF_PAYMENT_SERVER_URL,
			CONF_MAMMUN_AUTH_SECRET)->cmd_buy_item(
				$current_user->get_id(), $GP['type'], $GP['id'], $GP['amount']
			);
		$klunker = $x->klunker_left;
		$cost = $x->cost;
		$r->add_value(1);
		$r->add_value(
			str_replace(
				array('[ITEM]', '[KKR]'),
				array(ItemConstants::$ITEM_NAMES[$GP['type']][$GP['id']], $cost),
				LNG_ITEM_BOUGHT_OK
			)
		);
		$r->add_value($klunker);
		$r->add_value($x->item_amount);
	}
	catch (Exception $e) {
		switch ($e->getCode()) {
			case 1:
				/**
				 * Item gibt's nicht mehr
				 */
				$r->add_value(0);
				$r->add_value(
					str_replace("[ITEM]",
						ItemConstants::$ITEM_NAMES[$GP['type']][$GP['id']],
						LNG_ITEM_DOES_NOT_EXIST_ITEM_BOUGHT
					)
				);
				break;
			case 2:
				list($foo, $tmp) = explode('|',$e->getMessage());
				list($kkr_need, $kkr_have) = explode(',',$tmp);
				$r->add_value(0);
				$r->add_value(str_replace(
					array(
						"[ITEM]",
						"[KKR_NEED]",
						"[KKR_HAVE]",
						"[LINK]",
						"[/LINK]"
					),
					array(
						ItemConstants::$ITEM_NAMES[$GP['type']][$GP['id']],
						$kkr_need,
						$kkr_have,
						'<a href="javascript:UIButtons.toggle_buy_klunker();">',
						'</a>'
					),
					LNG_NOT_ENOUGH_KLUNKER_ITEM_BOUGHT
				));
				break;
			default:
				$r->add_value(0);
				$r->add_value(LNG_ITEM_BOUGHT_ERROR);
		}
	}
	catch (IllegalArgumentException $e) {
		$r->add_value(0);
		$r->add_value(LNG_ITEM_BOUGHT_ERROR);
	}
	die($r);
}

$techtree = Techtree::get_instance($current_user->get_id());

$all_researched_sciences = $techtree->get_cur_researched_scs();

$researched_races= array();		// enthält gleich alle erforschten Rassen
foreach ($all_researched_sciences['races'] as $sc_id => $data) {
	// Scientists sind nicht anpassbare Rassen
	if ($sc_id == 2)
		continue;
	// Keine Werkzeuge anzeigen
	if (in_array($sc_id, ScienceConstants::$ALL_TOOLS))
		continue;
	if ($data->level > 0) {	// sollte ohnehin so sein
		$researched_races[] = $sc_id;
		$allowed_ability_types[] = $techtree->get_allowed_abilities_of_race(
			$sc_id
		);
	}
}

/**
 * Freunde vom Community-Server holen
 */
$friends = ClientServer::send_request(
	CONF_WCF_SERVER_URL.'/gausus_wcf_hacks/friend_interface.php'
		.'?get_friends='.User::get_current_user()->get_nick()
);
$friends = $friends[0];
/**
 * ... und in die entsprechende Form bringen (einfaches Array von UIDs)
 * Dazu dient die Hilfsfunktion conver_friends
 */
function convert_friends(&$item, $key){
	$item = User::get_instance_by_nick($item->nick)->get_id();
}
if (!is_object($friends))
	$friends = array();
else {
	$friends = array_merge((array)$friends->onlineFriends,
		(array)$friends->offlineFriends);
	array_walk($friends, 'convert_friends');
}

foreach ( MapPlayers::get_instance(
		$current_user->get_world_id(),
		$current_user->get_map_id())->get_uids_on_map() as $mapplayer_uid )
{
	if ($mapplayer_uid != User::get_current_user()->get_id())
		$friends[] = $mapplayer_uid;
}
if (empty($friends))
	$friends = array(0);

/**
 * Liefert alle Items zurück, die entweder wir oder ein Freund/Mitspieler
 * verkauft.
 */
$all_items = Converter::stdClassToArray(
	Server::get_instance(CONF_PAYMENT_SERVER_URL, CONF_MAMMUN_AUTH_SECRET)
	->get_all_items(
		array(
			0,
			array(
				UnitClothes::TYPE_OUTERWEAR,
				UnitClothes::TYPE_TROUSERS,
				UnitClothes::TYPE_SHOES,
				UnitClothes::TYPE_JEWELRY,
				UnitClothes::TYPE_HAIR_HAT,
				UnitClothes::TYPE_WEAPON,
				IIndividualization::TYPE_BUILDING
			),
			$friends
		)
	)
);
require_once('../include/classes/TreasureChest.inc');
$treasure_chest = TreasureChest::get_instance($current_user->get_id());
$my_items = $treasure_chest->get_items();

$restrictions = Server::get_instance(CONF_ENGINE_SERVER_URL)
	->get_item_restrictions($current_user->get_id(),0);
$statistics = Server::get_instance(CONF_ENGINE_SERVER_URL)
	->get_item_statistics(0,0);

foreach ($all_items as $type => $items) {
	foreach ($items as $i => $data) {
		$all_items[$type][$i]->my_instance = 0;
		if (is_array($my_items[$type])) {
			foreach ($my_items[$type] as $my_i => $my_data) {
				if ($my_data->id == $data->id) {
					$all_items[$type][$i]->my_instance = $my_data;
					break;
				}
			}
		}
		$all_items[$type][$i]->name = ItemConstants::$ITEM_NAMES_2[$type][$data->id];
		$all_items[$type][$i]->desc = ItemConstants::$ITEM_DESCS[$type][$data->id];
		if (isset($statistics->$type->{$data->id})) {
			$all_items[$type][$i]->rareness_factor
				= ($statistics->$type->{$data->id}->rareness_factor) / 10;
		}
		else {
			$all_items[$type][$i]->rareness_factor = 0;
		}
		if (isset($restrictions->$type->{$data->id})) {
			$all_items[$type][$i]->xp =
				(int)$restrictions->$type->{$data->id}->usr_level;
			$all_items[$type][$i]->may_buy =
				(int)$restrictions->$type->{$data->id}->may_buy_from_shop;
		}
		else {
			$all_items[$type][$i]->xp = 0;
			$all_items[$type][$i]->may_buy = 1;
		}
		$all_items[$type][$i]->user_nick = User::get_instance_by_id(
			$all_items[$type][$i]->uid
		)->get_nick();
		unset($all_items[$type][$i]->uid);
		//error_log($data->id . " ".($statistics>$type->{$data->id}->rarerareness_factor));
	}
}
$all_items_json = Converter::xToJson($all_items);

if (!isset($_SESSION['shop_catalog_show'])) {
	$_SESSION['shop_catalog_show'] = true;
	$open_catalog = true;
}
else {
	$open_catalog = false;
}

include('templates/shop/shop.tmpl.inc');

if ($AJAX_OUTPUT) {
	$r->add_value(ob_get_clean());
	echo $r;
}
else
	ob_flush();
?>