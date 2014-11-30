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
require_once('../include/classes/TreasureChest.inc');
require_once('../include/classes/Avatar.inc');
require_once(
	'../include/shared/classes/community_hacks/CommunityMessageSender.inc');

$current_user = User::get_current_user();

Language::get_instance()->load_language_file('treasure_chest');
Language::get_instance()->load_language_file('shop');
Language::get_instance()->load_language_file('item_details');


$expected_data = array (
	'type'				=>	'int',
	'id'				=>	'int',
	'set_item_properties'=>	'bool',
	'sellable'			=>	'bool',
	'exclude_direct'	=>	'int',
	'price'				=>	'int',
	'buy_directly'		=>	'bool',
	'make_offer'		=>	'bool',
	'accept_offer'		=>	'string',
	'reject_offer'		=>	'string',
	'uid'				=>	'eint',
	'nick'				=>	'string',
	'get_offers_amount'	=>	'bool',
	'unvisit'			=>	'bool',
	'withdraw_offer'	=>  'string',
	'make_a_gift'		=>	'string'
);
$GP = $cryptObj->load_GET_POST($expected_data);


/**
 * Es wird nur angefragt, wie viele Angebote insgesamt für Items in der
 * Schatzkiste vorliegen.
 */
if (isset($GP['get_offers_amount'])) {
	$result = Server::get_instance(CONF_PAYMENT_SERVER_URL,
		CONF_MAMMUN_AUTH_SECRET)
						->get_my_offers_amount_total($current_user->get_id());
	$r->add_value($result);
	die($r);
}


if (isset($GP['buy_directly'])) {
	try {
		$x = Server::get_instance(CONF_PAYMENT_SERVER_URL,
			CONF_MAMMUN_AUTH_SECRET)
				->buy_directly_from_user(
					$current_user->get_id(), $GP['uid'], $GP['price'],
					$GP['type'], $GP['id']
				);
		$r->add_value(1);
		$r->add_value(
			str_replace	(
				"[ITEM]",
				ItemConstants::$ITEM_NAMES[$GP['type']][$GP['id']],
				LNG_DIRECT_OK
			)
		);
		$r->add_value($x->kkr);
		$r->add_value($x->item_amount);
		$r->add_value($GP['type']);
		$r->add_value($GP['id']);
	}
	catch (Exception $e) {
		$r->add_value(0);
		switch ($e->getCode()) {
			case 2:
				$r->add_value(
					str_replace(
						array("[LINK]", "[/LINK]", "[ITEM]"),
						array(
							'<a href="javascript: UIButtons.toggle_buy_klunker();">',
							'</a>',
							ItemConstants::$ITEM_NAMES[$GP['type']][$GP['id']]
						),
						LNG_NOT_ENOUGH_KKR_DIRECT
					)
				);
				break;
			default:
				$r->add_value(LNG_STRANGE_ERROR_DIRECT);
		}
	}
	catch (IllegalArgumentException $e) {
		$r->add_value(0);
		$r->add_value(LNG_STRANGE_ERROR_OFFER);
	}
	die($r);
}

/**
 * Es soll ein Angebot unterbreitet werden.
 */
if (isset($GP['make_offer'])) {
	try {
		$new_kkr = Server::get_instance(CONF_PAYMENT_SERVER_URL,
			CONF_MAMMUN_AUTH_SECRET)->make_offer(
				$current_user->get_id(), $GP['uid'], $GP['price'],
				$GP['type'], $GP['id']
			);
		$offer_nick = User::get_instance_by_id($GP['uid'])->get_nick();
		$my_nick = $current_user->get_nick();
		/**
		 * Nachricht senden
		 *
		CommunityMessageSender::send_message(
			$offer_nick,
			str_replace("[USER]", $my_nick, LNG_MAKE_OFFER_MAIL_SUBJECT),
			str_replace(
				array(
					'[RECIPIENT]',
					'[USER]',
					'[KKR]',
					'[ITEM]',
					'[YES_NO_BUTTONS]'
				),
				array(
					$offer_nick,
					$my_nick,
					$GP['price'],
					ItemConstants::$ITEM_NAMES[$GP['type']][$GP['id']],
					"YES_NO"
				),
				LNG_MAKE_OFFER_MAIL_TEXT
			),
			CommunityMessageSender::SENDER_SYSTEM
		);*/

		$r->add_value(1);
		$r->add_value(str_replace(
			array("[USER]", "[ITEM]", "[KKR]"),
			array(
				User::get_main_part_of_nick($offer_nick),
				ItemConstants::$ITEM_NAMES[$GP['type']][$GP['id']],
				$GP['price']
			),
			LNG_MAKE_OFFER_OK)
		);
		$r->add_value($new_kkr);
		$r->add_value(0);	// keine Korrektur der Items in der Schatzkiste
		$r->add_value(User::get_main_part_of_nick(
			User::get_instance_by_id($GP['uid'])->get_nick()
		));
	}
	catch (Exception $e) {
		$r->add_value(0);
		switch ($e->getCode()) {
			case 1:
				$r->add_value(
					str_replace(
						array("[USER]", "[ITEM]"),
						array(
							User::get_main_part_of_nick(
								User::get_instance_by_id($GP['uid'])->get_nick()
							),
							ItemConstants::$ITEM_NAMES[$GP['type']][$GP['id']]
						),
						LNG_ITEM_DOES_NOT_EXIST_OFFER
					)
				);
				break;
			case 2:
				$r->add_value(
					str_replace(
						array("[LINK]", "[/LINK]"),
						array(
							'<a href="javascript: UIButtons.toggle_buy_klunker();">',
							'</a>'
						),
						LNG_NOT_ENOUGH_KLUNKER_OFFER
					)
				);
				break;
			default:
				error_log($e->getFile().', Line '.$e->getLine().': '.
					$e->getMessage());
				$r->add_value(LNG_STRANGE_ERROR_OFFER);
		}
	}
	catch (IllegalArgumentException $e) {
		$r->add_value(0);
		$r->add_value(LNG_STRANGE_ERROR_OFFER);
	}

	die($r);
}
else if (isset($GP['set_item_properties'])) {
	try {
		$result = Server::get_instance(CONF_PAYMENT_SERVER_URL,
			CONF_MAMMUN_AUTH_SECRET)
				->set_chest_item_props(
					$current_user->get_id(), $GP['type'], $GP['id'],
					$GP['sellable'], $GP['price'], $GP['exclude_direct']
				);
		$r->add_value(1);
		if ($GP['sellable']) {
			$r->add_value(str_replace("[ITEM]",
				ItemConstants::$ITEM_NAMES[$GP['type']][$GP['id']],
				LNG_PUT_ON_MP_OK)
			);
		}
		else {
			$r->add_value(str_replace("[ITEM]",
				ItemConstants::$ITEM_NAMES[$GP['type']][$GP['id']],
				LNG_REMOVE_FROM_MP_OK)
			);
		}
		$r->add_value($GP['type']);
		$r->add_value($GP['id']);
		$r->add_value($result[0]);
		$r->add_value($result[1]);
		$r->add_value($GP['price']);
		$r->add_value($GP['exclude_direct']);
	}
	catch (Exception $e) {
		$r->add_value(0);
		switch ($e->getCode()) {
			case 1:
				if ($GP['sellable']) {
					$r->add_value(str_replace("[ITEM]",
						ItemConstants::$ITEM_NAMES[$GP['type']][$GP['id']],
						LNG_ITEM_DOES_NOT_EXIST_PUT_ON_MP)
					);
				}
				else {
					$r->add_value(str_replace("[ITEM]",
						ItemConstants::$ITEM_NAMES[$GP['type']][$GP['id']],
						LNG_ITEM_DOES_NOT_EXIST_REMOVE_FROM_MP)
					);
				}
				break;
			default:
				$r->add_value(LNG_STRANGE_ERROR_PUT_ON_MP);
		}
	}
	catch (IllegalArgumentException $e) {
		$r->add_value(0);
		$r->add_value(LNG_STRANGE_ERROR_PUT_ON_MP);
	}
	die($r);
}
/**
 * Es soll ein Gegenstand auf den Marktplatz gelegt werden.
 * @deprecated Use set_item_properties instead
 */
else if (isset($GP['put_on_mp'])) {
	try {
		for ($i = 0; $i < $GP['put_on_mp']; $i++)
		{
			$result = Server::get_instance(CONF_PAYMENT_SERVER_URL,
				CONF_MAMMUN_AUTH_SECRET)->put_on_mp(
					$current_user->get_id(), $GP['type'], $GP['id']
				);
		}
		$r->add_value(1);
		$r->add_value(str_replace("[ITEM]",
			ItemConstants::$ITEM_NAMES[$GP['type']][$GP['id']],
			LNG_PUT_ON_MP_OK)
		);
		$r->add_value($GP['type']);
		$r->add_value($GP['id']);
		$r->add_value($result[0]);
		$r->add_value($result[1]);
	}
	catch (Exception $e) {
		$r->add_value(0);
		switch ($e->getCode()) {
			case 1:
				$r->add_value(str_replace("[ITEM]",
					ItemConstants::$ITEM_NAMES[$GP['type']][$GP['id']],
					LNG_ITEM_DOES_NOT_EXIST_PUT_ON_MP)
				);
				break;
			default:
				$r->add_value(LNG_STRANGE_ERROR_PUT_ON_MP);
		}
	}
	catch (IllegalArgumentException $e) {
		$r->add_value(0);
		$r->add_value(LNG_STRANGE_ERROR_PUT_ON_MP);
	}
	die($r);
}
/**
 * Es soll ein Gegenstand vom Marktplatz genommen werden.
 * @deprecated Use set_item_properties instead
 */
else if (isset($GP['remove_from_mp'])) {
	try {

		for ($i = 0; $i < $GP['remove_from_mp']; $i++)
		{
			$result = Server::get_instance(CONF_PAYMENT_SERVER_URL,
				CONF_MAMMUN_AUTH_SECRET)->remove_from_mp(
					$current_user->get_id(), $GP['type'], $GP['id']
				);
		}
		$r->add_value(1);
		$r->add_value(str_replace("[ITEM]",
			ItemConstants::$ITEM_NAMES[$GP['type']][$GP['id']],
			LNG_REMOVE_FROM_MP_OK)
		);
		$r->add_value($GP['type']);
		$r->add_value($GP['id']);
		$r->add_value($result[0]);
		$r->add_value($result[1]);
	}
	catch (Exception $e) {
		$r->add_value(0);
		switch ($e->getCode()) {
			case 1:
				$r->add_value(str_replace("[ITEM]",
					ItemConstants::$ITEM_NAMES[$GP['type']][$GP['id']],
					LNG_ITEM_DOES_NOT_EXIST_REMOVE_FROM_MP)
				);
				break;
			default:
				$r->add_value(LNG_STRANGE_ERROR_REMOVE_FROM_MP);
		}
	}
	catch (IllegalArgumentException $e) {
		$r->add_value(0);
		$r->add_value(LNG_STRANGE_ERROR_REMOVE_FROM_MP);
	}
	die($r);
}
/**
 * Es soll ein Angebot angenommen werden.
 */
else if (isset($GP['accept_offer'])) {
	try {
		$buyer_id = User::get_instance_by_nick($GP['accept_offer'])->get_id();
		$result = Server::get_instance(CONF_PAYMENT_SERVER_URL,
			CONF_MAMMUN_AUTH_SECRET)->accept_offer(
				$current_user->get_id(), $buyer_id, $GP['type'], $GP['id']
			);
		$r->add_value(1);
		$r->add_value(
			str_replace(
				array(
					"[ITEM]",
					"[USER]",
					"[KKR]"
				),
				array(
					ItemConstants::$ITEM_NAMES[$GP['type']][$GP['id']],
					User::get_main_part_of_nick($GP['accept_offer']),
					$result->price
				),
				LNG_ACCEPT_OFFER_OK
			)
		);
		$r->add_value($GP['type']);
		$r->add_value($GP['id']);
		$r->add_value($result->klunker);
		$r->add_value($GP['accept_offer']);

		$my_nick = $current_user->get_nick();

		/**
		 * Nun wird der Käufer informiert
		 */
		CommunityMessageSender::send_message(
			$GP['accept_offer'],
			str_replace(
				array(
					'[USER]',
					'[ITEM]'
				),
				array(
					$my_nick,
					ItemConstants::$ITEM_NAMES[$GP['type']][$GP['id']],
				),
				LNG_ACCEPT_OFFER_SUBJECT),
			str_replace(
				array(
					'[RECIPIENT]',
					'[USER]',
					'[KKR]',
					'[ITEM]',
					'[LINK]',
					'[/LINK]'
				),
				array(
					User::get_main_part_of_nick($GP['accept_offer']),
					User::get_main_part_of_nick($my_nick),
					$GP['price'],
					ItemConstants::$ITEM_NAMES[$GP['type']][$GP['id']],
					'<a href="javascript: UIButtons.toggle_treasure_chest();"',
					'</a>'
				),
				LNG_ACCEPT_OFFER_TEXT
			),
			CommunityMessageSender::SENDER_SYSTEM
		);

		/**
		 * Jetzt werden alle informiert, die leider nicht mehr bieten können,
		 * weil das Item nun weg ist.
		 */
		$result->users_to_inform = Converter::stdClassToArray(
			$result->users_to_inform
		);
		$n = count($result->users_to_inform);
		for ($i = 0; $i < $n; $i++) {
			if ($uid == $buyer_id)
				continue;
			$uid = $result->users_to_inform[$i]->uid;
			$kkr = $result->users_to_inform[$i]->price;
			$nick = User::get_instance_by_id($uid)->get_nick();
			/**
			 * Nachricht senden
			 */
			CommunityMessageSender::send_message(
				$nick,
				str_replace(
					array(
						'[USER]',
						'[ITEM]'
					),
					array(
						$my_nick,
						ItemConstants::$ITEM_NAMES[$GP['type']][$GP['id']],
					),
					LNG_ACCEPT_OFFER_INFORMATION_SUBJECT),
				str_replace(
					array(
						'[RECIPIENT]',
						'[USER]',
						'[KKR]',
						'[ITEM]'
					),
					array(
						User::get_main_part_of_nick($nick),
						User::get_main_part_of_nick($my_nick),
						$GP['price'],
						ItemConstants::$ITEM_NAMES[$GP['type']][$GP['id']]
					),
					LNG_ACCEPT_OFFER_INFORMATION_TEXT
				),
				CommunityMessageSender::SENDER_SYSTEM
			);
		}
	}
	catch (Exception $e) {
		$r->add_value(0);
		switch ($e->getCode()) {
			case 1:
				$r->add_value(str_replace(
					array("[ITEM]", '[USER]'),
					array(
						ItemConstants::$ITEM_NAMES[$GP['type']][$GP['id']],
						User::get_main_part_of_nick($GP['accept_offer'])
					),
					LNG_ITEM_DOES_NOT_EXIST_ACCEPT_OFFER)
				);
				break;
			default:
				$r->add_value(LNG_STRANGE_ERROR_ACCEPT_OFFER);
		}
	}
	catch (IllegalArgumentException $e) {
		$r->add_value(0);
		$r->add_value(LNG_STRANGE_ERROR_ACCEPT_OFFER);
	}
	die($r);
}

/**
 * Es soll ein Angebot zurückgezogen werden.
 */
else if (isset($GP['withdraw_offer'])) {
	try {
		$seller_id =User::get_instance_by_nick($GP['withdraw_offer'])->get_id();
		$result = Server::get_instance(CONF_PAYMENT_SERVER_URL,
			CONF_MAMMUN_AUTH_SECRET)->withdraw_offer(
				$current_user->get_id(), $seller_id, $GP['type'], $GP['id']
			);
		$r->add_value(1);
		$r->add_value(
			str_replace(
				array(
					"[ITEM]",
					"[USER]",
					"[KKR]"
				),
				array(
					ItemConstants::$ITEM_NAMES[$GP['type']][$GP['id']],
					User::get_main_part_of_nick($GP['withdraw_offer']),
					$result->price
				),
				LNG_REJECT_OFFER_OK
			)
		);
		$r->add_value($GP['type']);
		$r->add_value($GP['id']);
		$r->add_value($GP['withdraw_offer']);
		$r->add_value($result->klunker);

		$my_nick = $current_user->get_nick();

		$outgoing_offers = (array) Server::get_instance(CONF_PAYMENT_SERVER_URL,
			CONF_MAMMUN_AUTH_SECRET)
	        	->get_outgoing_offers($current_user->get_id());

		for($i = 0; $i < count($outgoing_offers); $i++)
		{
			$outgoing_offers[$i]->nick = User::get_instance_by_id(
				$outgoing_offers[$i]->seller_id)->get_nick();
			$outgoing_offers[$i]->time_left =
				Time::get_t_now() - $outgoing_offers[$i]->t_stamp;
			$outgoing_offers[$i]->name =
				ItemConstants::$ITEM_NAMES[$outgoing_offers[$i]->type]
													[$outgoing_offers[$i]->id];
		}
		$r->add_value(json_encode($outgoing_offers));
	}
	catch (Exception $e) {
		$r->add_value(0);
		switch ($e->getCode()) {
			case 1:
				$r->add_value(str_replace(
					array("[ITEM]", '[USER]'),
					array(
						ItemConstants::$ITEM_NAMES[$GP['type']][$GP['id']],
						User::get_main_part_of_nick($GP['withdraw_offer'])
					),
					LNG_ITEM_DOES_NOT_EXIST_REJECT_OFFER)
				);
				break;
			default:
				$r->add_value(LNG_STRANGE_ERROR_REJECT_OFFER);
		}
	}
	catch (IllegalArgumentException $e) {
		$r->add_value(0);
		$r->add_value(LNG_STRANGE_ERROR_REJECT_OFFER);
	}
	die($r);
}

/**
 * Es soll ein Angebot abgelehnt werden.
 */
else if (isset($GP['reject_offer'])) {
	try {
		$buyer_id = User::get_instance_by_nick($GP['reject_offer'])->get_id();
		$price = Server::get_instance(CONF_PAYMENT_SERVER_URL,
			CONF_MAMMUN_AUTH_SECRET)->reject_offer(
				$current_user->get_id(), $buyer_id, $GP['type'], $GP['id']
			);
		$r->add_value(1);
		$r->add_value(
			str_replace(
				array(
					"[ITEM]",
					"[USER]",
					"[KKR]"
				),
				array(
					ItemConstants::$ITEM_NAMES[$GP['type']][$GP['id']],
					User::get_main_part_of_nick($GP['reject_offer']),
					$price
				),
				LNG_REJECT_OFFER_OK
			)
		);
		$r->add_value($GP['type']);
		$r->add_value($GP['id']);
		$r->add_value($GP['reject_offer']);

		$my_nick = $current_user->get_nick();
		/**
		 * Nachricht senden
		 */
		CommunityMessageSender::send_message(
			$GP['reject_offer'],
			str_replace("[USER]", User::get_main_part_of_nick($my_nick),
				LNG_REJECT_OFFER_SUBJECT),
			str_replace(
				array(
					'[RECIPIENT]',
					'[USER]',
					'[KKR]',
					'[ITEM]'
				),
				array(
					$GP['reject_offer'],
					User::get_main_part_of_nick($my_nick),
					$price,
					ItemConstants::$ITEM_NAMES[$GP['type']][$GP['id']]
				),
				LNG_REJECT_OFFER_TEXT
			),
			CommunityMessageSender::SENDER_SYSTEM
		);
	}
	catch (Exception $e) {
		$r->add_value(0);
		switch ($e->getCode()) {
			case 1:
				$r->add_value(str_replace(
					array("[ITEM]", '[USER]'),
					array(
						ItemConstants::$ITEM_NAMES[$GP['type']][$GP['id']],
						User::get_main_part_of_nick($GP['reject_offer'])
					),
					LNG_ITEM_DOES_NOT_EXIST_REJECT_OFFER)
				);
				break;
			default:
				$r->add_value(LNG_STRANGE_ERROR_REJECT_OFFER);
		}
	}
	catch (IllegalArgumentException $e) {
		$r->add_value(0);
		$r->add_value(LNG_STRANGE_ERROR_REJECT_OFFER);
	}
	die($r);
}

if (isset($GP['make_a_gift'])) {
	$to_uid = User::get_instance_by_nick($GP['make_a_gift'])->get_id();
	try {
		$r->add_value(1);
		list($amount, $amount_mp) =
			Server::get_instance(CONF_PAYMENT_SERVER_URL,
				CONF_MAMMUN_AUTH_SECRET)->give_item_away(
					$current_user->get_id(), $GP['type'], $GP['id'], $to_uid
				);
		$r->add_value(
			str_replace(
				array(
					"[ITEM]",
					"[USER]",
				),
				array(
					ItemConstants::$ITEM_NAMES[$GP['type']][$GP['id']],
					User::beautify_nick($GP['make_a_gift']),
				),
				LNG_MAKE_A_GIFT_OK
			)
		);
		$r->add_value($GP['type']);
		$r->add_value($GP['id']);
		$r->add_value($amount);
		$r->add_value($amount_mp);
	}
	catch (Exception $e) {
		$r->add_value(0);
		switch ($e->getCode()) {
			case 1:
				$r->add_value(str_replace(
					array("[ITEM]", '[USER]'),
					array(
						ItemConstants::$ITEM_NAMES[$GP['type']][$GP['id']],
						User::beautify_nick($GP['make_a_gift'])
					),
					LNG_ITEM_DOES_NOT_EXIST_MAKE_A_GIFT)
				);
				break;
			default:
				$r->add_value(LNG_STRANGE_ERROR_MAKE_A_GIFT);
		}
	}
	catch (IllegalArgumentException $e) {
		$r->add_value(0);
		$r->add_value(LNG_STRANGE_ERROR_MAKE_A_GIFT);
	}
	die($r);
}



if ($GP['nick']) {
	try {
		$uid = User::get_instance_by_nick($GP['nick'])->get_id();
	}
	catch(Exception $e) {
		$uid = (isset($GP['uid'])) ? $GP['uid'] : $current_user->get_id();
	}
}
else {
	$uid = (isset($GP['uid'])) ? $GP['uid'] : $current_user->get_id();
}

/**
 * Schatzkistenbesuch beendet.
 */
if (isset($GP['unvisit'])) {
	TreasureChest::get_instance($uid)->unvisit();
	die();
}

/**
 * Items aus der Schatzkiste holen und den Besuch markieren.
 */
$treasure_chest = TreasureChest::get_instance($uid);
$all_items = $treasure_chest->get_items();
$all_items_json = Converter::xToJson($all_items);
if ($uid != $current_user->get_id())
	$treasure_chest->visit(); // Besuch der eigenen Schatzkiste ist uninteressant
$visitor_ids = $treasure_chest->get_visitors();
$visitor_amount = count($visitor_ids);

$has_items = count($all_items > 0);
if ($uid != $current_user->get_id()) {
	$nick = User::get_instance_by_id($uid)->get_nick();
	$officer_text = ($has_items)
		? str_replace(
			"[USER]", User::beautify_nick($nick),
					LNG_TC_INTRO_TEXT_NOT_YOUR_TC_HAS_TREASURES)
		: str_replace(
			"[USER]", User::beautify_nick($nick),
					LNG_TC_INTRO_TEXT_NOT_YOUR_TC_NO_TREASURES);

	/**
	 * Schatzkistenbesuch registrieren
	 */
	Server::get_instance(CONF_PAYMENT_SERVER_URL, CONF_MAMMUN_AUTH_SECRET)
		->visit_chest(
			$current_user->get_id(), $uid
		);
}
else {
	$officer_text = ($has_items)
		? LNG_TC_INTRO_TEXT_HAS_TREASURES
		: str_replace(
			array("[ASHOP]", "[/SHOP]"),
			array('<a href="javascript: '
				. 'UIButtons.UIButtons.toggle_shop();">',
				'</a>'),
			LNG_TC_INTRO_TEXT_NO_TREASURES
		);
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
 * Den Avatar des Schatzkistenbesitzers zeigen wir nur an, wenn der Spieler
 * nicht ich selbst bin. Wenn ich der Besitzer bin, dann sparen wir uns den
 * Server-Request.
 */
$avatar_nicks = array();
if ($uid != $current_user->get_id()) {
	$avatar_nicks[] = User::get_instance_by_id($uid)->get_nick();
}
/**
 * Wenn ich der Besitzer bin, dann kann ich die Angebote einsehen und wenn
 * es welche gibt, dann sollte ich die Avatare der User laden.
 *
 * Zudem werden hier alle Angebote aufsummiert ($offer_amount).
 */
else {
	$outgoing_offers = (array) Server::get_instance(CONF_PAYMENT_SERVER_URL,
		CONF_MAMMUN_AUTH_SECRET)
	        ->get_outgoing_offers($current_user->get_id());

	for($i = 0; $i < count($outgoing_offers); $i++)
	{
		$outgoing_offers[$i]->nick = User::get_instance_by_id(
			$outgoing_offers[$i]->seller_id)->get_nick();
		$avatar_nicks[] = $outgoing_offers[$i]->nick;
		$outgoing_offers[$i]->time_left =
			Time::get_t_now() - $outgoing_offers[$i]->t_stamp;
		$outgoing_offers[$i]->name =
			ItemConstants::$ITEM_NAMES[$outgoing_offers[$i]->type]
													[$outgoing_offers[$i]->id];
	}

	$offer_amount = 0;
	foreach ($all_items as $type => $items) {
		foreach ($items as $i => $data) {
			foreach ($data->offers as $j => $offer) {
				$avatar_nicks[] = $all_items[$type][$i]->offers[$j]->buyer_nick;
				$offer_amount++;
			}
		}
	}
}
/**
 * Die Abfrage hier ist sinnvoll, denn im Fall meiner eigenen Schatzkiste, die
 * keine eingehenden und keine ausgehenden Gebote hat (mit der häufigste Fall)
 * brauchen wir keinen Request an die Community wegen Avataren senden!
 */
if (!empty($avatar_nicks))
	$avatar_data = Avatar::get_avatars($avatar_nicks);

include('templates/treasure_chest/treasure_chest.tmpl.inc');


if ($AJAX_OUTPUT) {
	$r->add_value(ob_get_clean());
	echo $r;
}
else
	ob_flush();
?>