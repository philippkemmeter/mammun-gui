<?php
/**
 * Diese Datei zeigt Unit Production. AJAX geladen.
 */

require_once ('../include/std_include.inc');
require_once ('../include/classes/Tribes.inc');
require_once ('../include/classes/Resource.inc');
require_once ('../include/shared/classes/ItemConstants.inc');
if (isset($_GET['ajaxid'])) {// Ein ping-und-echo-Parameter für AJAX-Request-Zuweisungen
	$AJAX_OUTPUT = true;
	$r = new ClientServerResponse();
	$r->add_value($_GET['ajaxid']);
	$this_win_id = $_GET['ajaxid'];
}
ob_start();
$current_user = User::get_current_user();
Language::get_instance()->load_language_file('unit_production');

/**
 * Checken, ob der Inhalt der Seite überhaupt zutreffend ist. Wenn der User-State
 * nicht stimmt, wurde entweder versucht zu bescheißen, oder im Spielcode ist
 * ein Designfehler. Kann auch vorkommen, wenn sich der State im laufenden Spiel
 * ändert. In jedem Fall löst ein kill_session() die Situation.
 */
if (($current_user->get_state() != User::STATE_PLAYING ||
	$current_user->get_waiting_state() != 0))
{
	if ($current_user->get_waiting_state() == User::STATE_MAP_IS_STARTING) {
		$error_text = LNG_ERR_MAP_NOT_STARTED_YET;
		include ('templates/error_info/error.tmpl.inc');
		die();
	}
	else
		kill_session();
}
require_once('../include/classes/Techtree.inc');
require_once('../include/classes/Tribes.inc');
$expected_data = array (
	'release_locks'	=> 'eint',
	'ajaxwinid' 	=> 	'int',
	'chosen_race'	=> 	'seint',
	'chosen_name'	=> 	'string',
	'id'			=> '(e)int',
	'amount'		=> 'int',
	'action'		=> 'string',
	'%ability\d+'	=> 'seint',
	'%item\d+'		=> 'int',
	'id0'			=>  'seint_nosession',	// Unit-ID zum Umziehen
	'race_change'	=>	'int'				// Rassen-ID zum Umziehen
);
$GP = $cryptObj->load_GET_POST($expected_data);
$tribes = Tribes::get_instance($current_user->get_id());

if (isset($GP['release_locks'])) {
	Server::get_instance(CONF_PAYMENT_SERVER_URL, CONF_MAMMUN_AUTH_SECRET)
		->release_item_locks(
			$current_user->get_id()
		);
	die();
}

if ($GP['action'] === 'morph') {
	$r = new ClientServerResponse();
	/**
	 * Wenn keine ID übermittelt wurde, schauen, ob anhand der Daten ein neuer
	 * Tribe erstellt oder gefunden werden kann.
	 */
	if (empty($GP['id'])) {
		if (empty($GP['chosen_name'])) {
			$r->add_value(0);
			$r->add_value('Bitte Namen eingeben');
			die($r);
		}
		try {
			$abilities = array();
			foreach ($GP as $key => $value)
				if (substr($key, 0, 7) === 'ability')
					$abilities[] = $value;
			$tribe = $tribes->insert_new_tribe($GP['chosen_race'],
				$GP['chosen_name'], $abilities);
		}
		catch (Exception $e) {
			if ($e->getCode() == Tribes::E_SAME_TRIBE_CONFIG_TWICE) {
				$GP['id'] = (int)$e->getMessage();
			}
			else {
				$r->add_value(0);
				$r->add_value($e->getCode().": ".$e->getMessage());
				die($r);
			}
		}
	}

	/**
	 * Wenn der Tribe gerade per $tribes->inser_new_tribe() erfolgreich
	 * hinzugefügt wurde, dann ist $tribe bereits gesetzt. Ansonsten muss man
	 * ihn anhand der ID erst finden.
	 */
	if (!$tribe)
		$tribe = $tribes->get_tribe_by_id($GP['id']);
	if (empty($tribe)) {
		$r->add_value(0);
		$r->add_value("Tribe not found");
		die($r);
	}
	try {
		$GP['amount'] *= 1000;

		$clothes = new UnitClothes(
			$GP['item1'],$GP['item2'],
			$GP['item3'],$GP['item4'],
			$GP['item5'],$GP['item6']
		);
		$klunker = $tribe->morph($GP['amount'], $GP['chosen_name'], $clothes);

		/**
		 * Wenn bislang kein Fehler aufgetreten ist, dann alle aktuellen infos
		 * der Unit zurückgeben.
		 */
		$uid = $current_user->get_id();
		$count = $tribe->get_count();
		$morph_add = $tribe->get_morphing_count();
		$r->add_value(1);
		$r->add_value($count);
		$r->add_value($morph_add);
		$r->add_value($tribe->get_add_stamp());
		$r->add_value($tribe->get_add_value());
		$r->add_value($tribe->get_time_needed()*$tribe->get_add_value());
		$r->add_value(Resource::get_instance($uid)->
			get_resource(Resource::RESOURCE2));
		$r->add_value(
			ScoreCache::get_map_score(
				$current_user->get_world_id(),
				$current_user->get_map_id(),
				$uid
			)
		);
		$r->add_value(
			ScoreCache::get_league_score(0, $uid)
		);
		$r->add_value($klunker);
		$r->add_value($tribe->get_pic_url());
		$r->add_value($GP['chosen_name']);
	}
	catch (HackattackException $e) {
		// errormail ?
		$r->add_value(0);
		die($r);
	}
	catch (Exception $e) {
		$r->add_value(0);
		switch ($e->getCode()) {
			case Tribes__TribeInMF::ERR_NOT_ENOUGH_BIO:
				$r->add_value(LNG_ERR_NOT_ENOUGH_FOOD);
				break;
			case Tribes__TribeInMF::ERR_UNIT_LIMIT_REACHED:
				$r->add_value(LNG_ERR_UNIT_LIMIT_REACHED);
				break;
			case Tribes__TribeInMF::ERR_NOT_ENOUGH_KLUNKER:
				$r->add_value(
					str_replace(
						array('[LINK]', '[/LINK]'),
						array(
							'<a href="javascript: '.
								'UIButtons.toggle_buy_klunker(); '.
								'MammunUI.destroy_warning();">',
							'</a>'
						),
						LNG_ERR_NOT_ENOUGH_KLUNKER
					)
				);
				break;
			default:
				$r->add_value(LNG_ERR_STRANGE_ERROR);
		}
	}
	die($r);
}
/**
 * Einheit entlassen
 */
else if ($GP['action'] === 'unmorph') {
	$r = new ClientServerResponse();
	$tribe = $tribes->get_tribe_by_id($GP['id']);
	if (!$tribe) {
		$r->add_value(0);
		$r->add_value("NOT_EXISTING");	// CODE: Tribe does not exist
	}
	else {
		try {
			$tribe->unmorph($GP['amount']*1000);
			$r->add_value(1);
		}
		catch (HackattackException $e) {
			$r->add_value(0);
		}
		catch (Exception $e) {
			$r->add_value(0);
			$r->add_value($e->getCode);
		}
	}
	die($r);
}
else if ($GP['action'] === 'change_clothes') {
	/**
	 * Fall 1: Spieler hat die Kleider schon gewählt
	 */
	if (isset($GP['item1']) || isset($GP['item2']) || isset($GP['item3'])
		|| isset($GP['item4']) || isset($GP['item5']) || isset($GP['item6']))
	{
		$r = new ClientServerResponse();
		$clothes = new UnitClothes(
			$GP['item1'],$GP['item2'],
			$GP['item3'],$GP['item4'],
			$GP['item5'],$GP['item6']
		);
		try {
			$kkr = Server::get_instance(CONF_ENGINE_SERVER_URL)->
							cmd_tribe_change_clothes(
								$GP['id0'],
								$clothes->to_php_std_class()
							);

			$r->add_value(1);
			$r->add_value($_REQUEST['id0']);
			$r->add_value($kkr);
		}
		catch (Exception $e) {
			$r->add_value(0);
			switch ($e->getCode()) {
				case Tribes__TribeInMF::ERR_NOT_ENOUGH_KLUNKER:
					$r->add_value(
						str_replace(
							array('[LINK]', '[/LINK]'),
							array(
								'<a href="javascript: '.
									'UIButtons.toggle_buy_klunker(); '.
									'MammunUI.destroy_warning();">',
								'</a>'
							),
							LNG_ERR_NOT_ENOUGH_KLUNKER
						)
					);
					break;
				default:
					$r->add_value(LNG_ERR_STRANGE_ERROR);
			}
		}
		die($r);
	}
	/**
	 * Fall 2: Er muss sie noch wählen
	 */
	else {
		// weiter unten...
	}
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

$researched_abilities = array();	// enthält gleich alle erforschten
									// Fähigkeiten gruppiert nach Typ
									// (general, offensive etc)



$all_items = Converter::stdClassToArray(
	Server::get_instance(CONF_PAYMENT_SERVER_URL, CONF_MAMMUN_AUTH_SECRET)
	->get_all_direct_trade_items(
		array(
			User::get_current_user()->get_id(),
			0,
			3,
			true,
			UnitClothes::TYPE_OUTERWEAR,
			UnitClothes::TYPE_TROUSERS,
			UnitClothes::TYPE_SHOES,
			UnitClothes::TYPE_JEWELRY,
			UnitClothes::TYPE_HAIR_HAT,
			UnitClothes::TYPE_WEAPON
		)
	)
);
foreach ($all_items as $type => $items) {
	foreach ($items as $i => $data) {
		$all_items[$type][$i]->name
			= ItemConstants::$ITEM_NAMES[$type][$data->id];
		$all_items[$type][$i]->desc
			= ItemConstants::$ITEM_DESCS[$type][$data->id];
		$all_items[$type][$i]->type = $type;
		// Rückwärtskompatibilitätsumkopiererei:
		$all_items[$type][$i]->cost = $all_items[$type][$i]->price;
	}
}

$all_items_json = Converter::xToJson($all_items);

$my_items = Converter::stdClassToArray(
	Server::get_instance(CONF_PAYMENT_SERVER_URL, CONF_MAMMUN_AUTH_SECRET)
		->get_my_items(
			$current_user->get_id(),
			UnitClothes::TYPE_OUTERWEAR,
			UnitClothes::TYPE_TROUSERS,
			UnitClothes::TYPE_SHOES,
			UnitClothes::TYPE_JEWELRY,
			UnitClothes::TYPE_HAIR_HAT,
			UnitClothes::TYPE_WEAPON
		)
);

/**
 * Die erhaltenen Items für die Ausgabe konvertieren. Regeln sind:
 * 1. Keine Offers
 * 2. Name und Beschreibung der Items hinzufügen
 */
foreach ($my_items as $type => $items) {
	foreach ($items as $i => $data) {
		unset($my_items[$type][$i]->offers);
		$my_items[$type][$i]->name
			= ItemConstants::$ITEM_NAMES[$type][$data->id];
		$my_items[$type][$i]->desc
			= ItemConstants::$ITEM_DESCS[$type][$data->id];
	}
}


$my_items_json = Converter::xToJson($my_items);

/**
 * Das hier ist jetzt Fall 2 von change_clothes: Er muss noch die Kleider wählen
 */
if ($GP['action'] === 'change_clothes') {
	$init_clothes = UnitClothes::convert_std_class_to_unit_clothes(
		Server::get_instance(CONF_ENGINE_SERVER_URL)->
				get_tribe_clothes($GP['id0']))->to_php_array();

	/**
	 * $init_clothes hat der Mammun gerade an. Diese schonmal in die Schatzkiste
	 * legen, damit der Anprobierdialog normal funktioniert.
	 * Zudem werden sie als selektiert markiert.
	 */
	$items_selected = array();
	foreach ($init_clothes as $type => $id) {
		$create_new_item = true;
		if ($my_items[$type]) {
			foreach ($my_items[$type] as $i => $item) {
				if ($item->id == $id) {
					$my_items[$type][$i]->amount++;
					$create_new_item = false;
					break;
				}
			}
		}
		if ($create_new_item) {
			$item = new stdClass();
			$item->amount = 1;
			$item->amount_mp = 0;
			$item->id = $id;
			$item->type = $type;
			$item->name = ItemConstants::$ITEM_NAMES[$type][$data->id];
			$item->desc	= ItemConstants::$ITEM_DESCS[$type][$data->id];
			$my_items[$type][] = $item;
		}
		$items_selected[] = $item;
	}
	$my_items_json = Converter::xToJson($my_items);

	$change_clothes = true;
	$change_clothes_race = $GP['race_change'];
	$officer_text_select_indivi = LNG_INTRO_TEXT_CHANGE_CLOTHES;
	include('templates/unit_production/unit_production.tmpl.inc');
	if ($AJAX_OUTPUT) {
		$r->add_value(ob_get_clean());
		echo $r;
	}
	else
		ob_flush();
	die();
}


if (!empty($all_researched_sciences['abilities'])) {
	foreach ($all_researched_sciences['abilities'] as $sc_id => $data) {
		if ($data->level > 0) {	// sollte ohnehin so sein
			$researched_abilities[$data->type][] = $sc_id;
		}
	}
}

$morphing_tribes =
	Tribes::get_instance($current_user->get_id())->get_morphing_tribes();

$morphing_tribes_by_race = array();
$morphing_scientists_amount = 0;
foreach ($morphing_tribes as $t) {
	if ($t->get_race() == 2) {
		$morphing_scientists_amount++;
	}
	$morphing_tribes_by_race[$t->get_race()] = $t;
}

$morphing_amount = count($morphing_tribes) - $morphing_scientists_amount;
$officer_text_intro = LNG_INTRO_TEXT."<br/>";
$officer_text_intro .= ($morphing_amount == 0)
	? LNG_INTRO_TEXT_NO_TRAINING
	: (($morphing_amount > 1)
		? str_replace(
			array(
				"[AMOUNT]",
				"[MAMMUN]",
				"[WERDEN]"
			),
			array(
				$morphing_amount,
				LNG_MAMMUN_PLURAL,
				LNG_WERDEN_PLURAL
			),
			LNG_INTRO_TEXT_PROGRESS
		)
		: str_replace(
			array(
				"[AMOUNT]",
				"[MAMMUN]",
				"[WERDEN]"
			),
			array(
				$morphing_amount,
				LNG_MAMMUN_SINGULAR,
				LNG_WERDEN_SINGULAR
			),
			LNG_INTRO_TEXT_PROGRESS
		)
	);

$officer_text_unit_limit_reached = LNG_ERR_UNIT_LIMIT_REACHED."<br/>";
$tent_sc_id = 1006;
$officer_text_unit_limit_reached .=
	(isset($all_researched_sciences['buildings']->$tent_sc_id))
	? LNG_ERR_UNIT_LIMIT_REACHED_TENT
	: LNG_ERR_UNIT_LIMIT_REACHED_NO_TENT;

$officer_text_select_unit_options = LNG_SELECT_UNIT_OPTIONS_TEXT;
$officer_text_select_abils = (empty($researched_abilities))
	? LNG_SELECT_UNIT_OPTIONS_TEXT_NO_ABILS
	: LNG_SELECT_UNIT_OPTIONS_TEXT_CHOOSE_ABILS;

$officer_text_select_indivi = LNG_SELECT_INDIVI_TEXT;

$officer_text_not_enough_food_for_anything = LNG_ERR_NOT_ENOUGH_FOOD_AT_ALL;

$items_selected = array();

include('templates/unit_production/unit_production.tmpl.inc');
if ($AJAX_OUTPUT) {
	$r->add_value(ob_get_clean());
	echo $r;
}
else
	ob_flush();
?>