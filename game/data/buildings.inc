<?php
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
/**
 * Checken, ob der Inhalt der Seite überhaupt zutreffend ist. Wenn der User-State
 * nicht stimmt, wurde entweder versucht zu bescheißen, oder im Spielcode ist
 * ein Designfehler. Kann auch vorkommen, wenn sich der State im laufenden Spiel
 * ändert. In jedem Fall löst ein kill_session() die Situation.
 */
if ($current_user->get_state() != User::STATE_PLAYING)
	kill_session();

$expected_data = array (
	'change_mode'	=> 'bool',
	'x'				=> 'int',
	'y'				=> 'int',
	'sector_id'		=> 'int',
	'type'			=> 'int',
	'color'			=> 'int',
	'nick'			=> 'string'
);
$GP = $cryptObj->load_GET_POST($expected_data);

include_once ('../include/classes/buildings/Buildings.inc');
$bObj = Buildings::get_instance($current_user->get_id());

/** Baubare Gebäude */
if (@$GP['change_mode']) {
	$buildable_buildings = Buildings::get_all_non_custom_buildings();
	$all_buildings = array();
}
else {
	$buildable_buildings = $bObj->get_buildable_buildings();
	$b = Buildings::get_instance($current_user->get_id());
	$all_buildings = json_encode($b->get_treasure_buildings());
}

Language::get_instance()->load_language_file('buildings_');
Language::get_instance()->load_language_file('facebook_publish');

$change_mode = (bool)@$GP['change_mode'];
$type = (int)@$GP['type'];
$color = (int)@$GP['color'];
$world_id = User::get_current_user()->get_world_id();
$map_id = User::get_current_user()->get_map_id();
$sector_id = (int)@$GP['sector_id'];
$x = (int)@$GP['x'];
$y = (int)@$GP['y'];
$nick = @$GP['nick'];

include ('templates/buildings/buildings.tmpl.inc');

if ($AJAX_OUTPUT) {
	$r->add_value(ob_get_clean());
	echo $r;
}
else
	ob_flush();
?>