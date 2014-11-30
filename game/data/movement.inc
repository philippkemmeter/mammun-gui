<?php
include ('../include/std_include.inc');
$r = new ClientServerResponse();
if (isset($_GET['ajaxid'])) {// Ein ping-und-echo-Parameter für AJAX-Request-Zuweisungen
	$r->add_value($_GET['ajaxid']);
	$this_win_id = $_GET['ajaxid'];
}

$current_user = User::get_current_user();
/**
 * Checken, ob der Inhalt der Seite überhaupt zutreffend ist. Wenn der User-State
 * nicht stimmt, wurde entweder versucht zu bescheißen, oder im Spielcode ist
 * ein Designfehler. Kann auch vorkommen, wenn sich der State im laufenden Spiel
 * ändert. In jedem Fall löst ein kill_session() die Situation.
 */
if (($current_user->get_state() != User::STATE_PLAYING)
	|| ($current_user->get_waiting_state() != 0))
{
	kill_session();
}
require_once ('../include/shared/classes/Language.inc');
Language::get_instance()->load_language_file('movement');
include_once('../include/classes/map/Sector.inc');
include_once('../include/shared/classes/helper/Converter.inc');
include_once('../include/shared/classes/stdClassGS.inc');
$expected_data = array (
	'ajaxwinid' 	=> 	'int',
	'path'			=> 	'(e)string',
	'id0'			=>  'seint_nosession',
	'count0'		=>	'int',
	'moving_in_sec'	=>	'int',
	'moving_to_sec'	=>	'int',
	'stop'			=>	'bool'
);
$GP = $cryptObj->load_GET_POST($expected_data);
$world_id = $current_user->get_world_id();
$map_id = $current_user->get_map_id();

/**
 * Stoppen der Einheitn
 */
if (isset($GP['stop'])) {
	try {
		$x = Server::get_instance(CONF_ENGINE_SERVER_URL)
			->cmd_stop_tribe($GP['id0']);

		$sector_id = $x->sector_id;
		$moving_data = $x->moving_units;
		$dom = $x->dominator;
		$sector_unit_data = $x->units;
		$fight = $x->fight;
		$buildings = $x->building;
	}
	catch (IllegalArgumentException $e) {
		$r->add_value(0);
		$r->add_value($e->getMessage());
		die($r);
	}

	$r->add_value(1);
	$r->add_value($sector_id);
	$r->add_value($moving_data);
	$r->add_value($dom);
	$r->add_value($fight);
	$r->add_value($sector_unit_data);
	$r->add_value($buildings);
	die($r);
}

/**
 * Anforderung, alle Einheiten aufgelistet zu bekommen, die gerade von $GP['moving_in_sec']
 * nach $GP['moving_to_sec'] ziehen. Ist $GP['moving_to_sec'] nicht gesetzt,
 * werden alle Nachbarsektoren berücksichtigt.
 */
if (isset($GP['moving_in_sec'])) {
	if (($GP['moving_in_sec'] > 99) || ($GP['moving_in_sec'] < 0))
		;//hackattack
	$direction = Direction::ALL;
	if (isset($GP['moving_to_sec'])) {
		$diff = $GP['moving_to_sec'] - $GP['moving_in_sec'];
		if ($diff == 1)
			$direction = 0;
		else if ($diff == -1)
			$direction = 1;
		else if ($diff == 10)
			$direction = 2;
		else if ($diff == -10)
			$direction = 3;
	}/*
	try {
		$moving_data = Server::get_instance(CONF_ENGINE_SERVER_URL)
			->get_moving_units($map_id, $GP['moving_in_sec'], $direction);
	}
	catch (IllegalArgumentException $e) {
		die($e->getMessage());
	}
	$moving_units = array();
	for ($i = 0; $i < count($moving_data); $i++) {
		$moving_units[] = new stdClassGS((array)$moving_data);
	}*/
	$start_sector_id = $GP['moving_in_sec'];
	ob_start();
	include('templates/movement/show_moving_units.tmpl.inc');
	$r->add_value(ob_get_clean());
	die($r);
}
/**
 * Bewegungsbefehl erteilen. Die Einheiten mit ID $GP['id0']-$GP['idX'] sollen
 * den Pfad $GP['path'] nehmen.
 */
if (!$GP['path'] || !$GP['id0']) {
	// Da ist was merkwürdiges am Laufen. Es müsste immer ein Pfad und
	// mindestens eine ID übergeben werden, damit hier überhaupt was passieren
	// kann.
	// Tritt regulär dann auf, wenn der User die Einheiten wohin bewegen will,
	// wohin kein Pfad gefunden wurde. Wir gehen daher davon aus:

	$r->add_value(0);
	$r->add_value(LNG_NO_PATH_TO_DESTINATION);
	die($r);
}

if (!isset($GP['count0'])) {	// Spieler hat noch nicht angegeben,
								// wie viele er bewegen will
	$r->add_value(0);
	$r->add_value(LNG_NO_PATH_TO_DESTINATION);
	die($r);
}
else {	// Jetzt wird wirklich bewegt
	$path = explode('-',$GP['path']);
	for ($i = 0; $i < count($path); $i++)
		$path[$i] = (int)$path[$i];
	try {
		$x = Server::get_instance(CONF_ENGINE_SERVER_URL)
			->cmd_move_tribe($GP['id0'], $path);

		$moving_data = $x->moving_units;
		$dom = $x->dominator;
		$sector_unit_data = $x->units;
		$fight = $x->fight;
		$buildings = $x->building;
	}
	catch (IllegalArgumentException $e) {
		$r->add_value(0);
		$r->add_value($e->getMessage());
		die($r);
	}

	$r->add_value(1);
	$r->add_value($moving_data);
	$r->add_value($dom);
	$r->add_value($fight);
	$r->add_value($sector_unit_data);
	$r->add_value($path);
	$r->add_value($buildings);
	die($r);
}
?>