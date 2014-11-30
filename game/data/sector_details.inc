<?php
/**
 * Diese Datei zeigt den Inhalt eines Sektors an. Wird i.d.R. per AJAX geladen.
 */
include ('../include/std_include.inc');
if (isset($_GET['ajaxid'])) {// Ein ping-und-echo-Parameter für AJAX-Request-Zuweisungen
	$AJAX_OUTPUT = true;
	$r = new ClientServerResponse();
	$r->add_value($_GET['ajaxid']);
	$this_win_id = $_GET['ajaxid'];
}
else
	$AJAX_OUTPUT = false;
ob_start();

$current_user = User::get_current_user();


//error_reporting(E_ALL);
include_once ('../include/classes/buildings/Buildings.inc');
include_once ('../include/classes/map/Sector.inc');
include_once ('../include/shared/classes/Language.inc');
include_once ('../include/classes/MapDescriptionData.inc');

Language::get_instance()->load_language_file('sector_details');

$expected_data = array (
	'world_id'	=>	'int',
	'map_id'	=>	'int',
	'sector_id' =>	'(e)int'
);
$GP = $cryptObj->load_GET_POST($expected_data);
$sector_id = (int) $GP['sector_id'];
if ($sector_id > 99) {
	if ($sector_id == 100)	// TODO: UGLY!
		$sector_id = $current_user->get_homesector() % 100;
	else
		$sector_id = $sector_id % 100;
}

if ((isset($GP['world_id']) && ($GP['world_id'] > 0)) && (isset($GP['map_id'])))
{
	$world_id = $GP['world_id'];
	$map_id = $GP['map_id'];
}
else {
	$map_id = $current_user->get_map_id();
	$world_id = $current_user->get_world_id();
}

$sector_c = new WorldCoordinates($world_id, $map_id, $sector_id);
/** Nötige Objekte erzeugen */
$this_sector = Sector::get_instance($sector_c);
$bObj = Buildings::get_instance($current_user->get_id());

/** Hilfsvariablen (2) */
$sector_resource_gain = $this_sector->get_resource_gain($current_user->get_id());
$sector_resource_gain_split = $this_sector->get_resource_gain(
	$current_user->get_id(), false, true
);
$sector_resource_gain_w_max = array(
	$sector_resource_gain_split[0][0] + $sector_resource_gain_split[0][2],
	$sector_resource_gain_split[1][0] + $sector_resource_gain_split[1][2]
);
$sector_resource_gain_no_max = array(
	$sector_resource_gain_split[0][1],
	$sector_resource_gain_split[1][1]
);
$sector_resource_max = $this_sector->get_max_resources();
for ($i = 0; $i < 2; $i++)
 	$sector_resource_max[$i] += $sector_resource_gain_split[$i][2];

/** Baubare Gebäude */
$buildable_buildings = $bObj->get_buildable_buildings();

/**
 * Wir holen die aktuellsten Daten vom Sektor ab. Diese werden explizit neu
 * berechnet, damit man hier immer auf dem neusten Stand ist. Diese
 * Informationen werden dann an die Karte propagiert.
 *
 * Computing on Demand :)
 */
$map_data = Server::get_instance(CONF_ENGINE_SERVER_URL)->get_map_data(
	$current_user->get_id(), $world_id, $map_id, $sector_id, true
);


/** Den Officer-Text zusammenbauen */
$officer_text = '';
$search = array(
	'[SECTOR_NUM]',
	'[DOMINATOR]',
	'[WOOD]',
	'[WOOD_ADD]',
	'[WOOD_MAX]',
	'[FOOD]',
	'[FOOD_ADD]',
	'[FOOD_MAX]'
);
$replace = array (
	$sector_id,
	User::get_main_part_of_nick($this_sector->get_dominator_nick()),
	'<b>'.$sector_resource_gain_w_max[0].'</b>',
	'<b>'.$sector_resource_gain_no_max[0].'</b>',
	'<b>'.$sector_resource_max[0].'</b>',
	'<b>'.$sector_resource_gain_w_max[1].'</b>',
	'<b>'.$sector_resource_gain_no_max[1].'</b>',
	'<b>'.$sector_resource_max[1].'</b>'
);
/* FIXME: get_homesector() is obsolete */
if (($sector_id == ($current_user->get_homesector()%100))
	&& ($this_sector->get_dominator_id() != $current_user->get_id()))
{
	$officer_text .= str_replace($search, $replace,
		LNG_TEXT_SECTOR_DESC_HOME_OCCUPIED);
}
else {
	if ($this_sector->get_dominator_id() == $current_user->get_id()) {
		if ($this_sector->get_fight()) {
			$officer_text .= str_replace($search, $replace,
				LNG_TEXT_SECTOR_DESC_YOU_DEFENDING);
		}
		else {
			/* FIXME: get_homesector() is obsolete */
			if ($sector_id == ($current_user->get_homesector()%100)) {
				$officer_text .= str_replace($search, $replace,
					LNG_TEXT_SECTOR_DESC_YOURS_PART1_HOME);
			}
			else {
				$officer_text .= str_replace($search, $replace,
					LNG_TEXT_SECTOR_DESC_YOURS_PART1);
			}

			if ($sector_resource_gain_w_max[0] < 1) {
				$officer_text .= str_replace($search, $replace,
					LNG_TEXT_SECTOR_DESC_YOURS_PART2_WOOD1);
			}
			else {
				$officer_text .= str_replace($search, $replace,
					LNG_TEXT_SECTOR_DESC_YOURS_PART2_WOOD2);
			}
			$officer_text .= str_replace($search, $replace,
				LNG_TEXT_SECTOR_DESC_YOURS_PART2_FOOD2);


			if (($sector_resource_gain_w_max[0] < $sector_resource_max[0]) ||
				($sector_resource_gain_w_max[0] < $sector_resource_max[1]))
			{
				$officer_text .= str_replace($search, $replace,
					LNG_TEXT_SECTOR_DESC_YOURS_PART3);
			}
			else
				$officer_text .= str_replace($search, $replace,
					LNG_TEXT_SECTOR_DESC_YOURS_PART3_FULL_RES);


			if ($sector_resource_gain_no_max[1] > 0) {
				$officer_text .= ' '.str_replace($search, $replace,
					LNG_TEXT_SECTOR_DESC_YOURS_PART4);
			}
		}
	}
	else {
		if (!$this_sector->get_dominator_nick())
			$officer_text .= str_replace($search, $replace,
				LNG_TEXT_SECTOR_DESC_NOBODY);
		else if ($this_sector->get_dominator_nick() === '-') {
			if ($this_sector->get_fight()) {
				$officer_text .= str_replace($search, $replace,
					LNG_TEXT_SECTOR_DESC_HOST_FIGHT);
			}
			else {
				$officer_text .= str_replace($search, $replace,
					LNG_TEXT_SECTOR_DESC_HOST);
			}
		}
		else {
			if ($this_sector->get_fight()) {
				$officer_text .= str_replace($search, $replace,
					LNG_TEXT_SECTOR_DESC_FIGHT_OTHER);
			}
			else {
				$officer_text .= str_replace($search, $replace,
					LNG_TEXT_SECTOR_DESC_HOSTILE);
			}
		}
	}
}

if ($this_sector->get_dominator_id())
	$dominator_user = User::get_instance_by_id($this_sector->get_dominator_id());
else
	$dominator_user = null;

include ('templates/sector_details/sector_details.tmpl.inc');
if ($AJAX_OUTPUT) {
	$r->add_value(ob_get_clean());
	echo $r;
}
else
	ob_flush();
?>