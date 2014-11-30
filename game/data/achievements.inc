<?php
/**
 * Diese Datei zeigt Achievements. AJAX geladen.
 */

require_once ('../include/std_include.inc');
if (isset($_GET['ajaxid'])) {// Ein ping-und-echo-Parameter für AJAX-Request-Zuweisungen
	$AJAX_OUTPUT = true;
	$r = new ClientServerResponse();
	$r->add_value($_GET['ajaxid']);
	$this_win_id = $_GET['ajaxid'];
}
ob_start();
$current_user = User::get_current_user();
Language::get_instance()->load_language_file('achievements');
require_once('../include/classes/Techtree.inc');

$expected_data = array (
	'ajaxwinid' 		=> 	'int',
	'sc_id'				=>	'eint',
	'set_bernard_txt'	=>	'string',
	'const_key'			=>	'string'
);
$GP = $cryptObj->load_GET_POST($expected_data);

if (isset($GP['set_bernard_txt'])) {
	$r = new ClientServerResponse();

	Language::get_instance()->set_var_value_of_lng_file(
		'ScienceConstants', strtoupper($GP['const_key'].'_bernard'),
		$GP['set_bernard_txt']
	);
	$r->add_value(1);
	die($r);
}

$techtree = Techtree::get_instance($current_user->get_id());

$researched_scs_org = $techtree->get_cur_researched_scs();
$researched_scs = array();
/**
 * Deepcopy all data for output
 */
foreach ($researched_scs_org as $type => $data) {
	$d = (array)$data;
	foreach ($d as $id => $row) {
		if (($id == 2) || ($id == 1)) {	/* Interne Forschungen nicht anzeigen*/
			continue;
		}
		$researched_scs[$type][$id]->name = ScienceConstants::$SC_NAME[$id];
		/* SC-IDs verschlüsseln */
		$researched_scs[$type][$id]->dataX = $cryptObj->do_encryption(
			array("sc_id" => $row->sc_id)
		);
		$researched_scs[$type][$id]->const_key = $row->const_key;
	}
}
$researched_scs_json = json_encode($researched_scs);

$activate_input_bernard =
	((strtolower($current_user->get_nick()) == 'bernard')
	&& defined('TEST_SERVER'));

if (!isset($GP['sc_id']))
	$GP['sc_id'] = 0;
if (isset($GP['sc_id'])) {
	/**
	 * TODO: Momentan ist $cur_level==$max_level_ever gegeben. Das wird
	 * irgendwann nicht mehr so sein, dann muss was besseres her!
	 */
	$sc_id = $GP['sc_id'];
	if ($sc_id) {
		$cur_level = $techtree->get_science_cur_level($sc_id);
		$science = $techtree->get_known_science_values($sc_id,true);
	}
	else {
		$cur_level = 0;
		$science = new stdClass();
		$science->name = '';
		$science->resource1 = 0;
		$science->resource2 = 0;
		$science->attack = 0;
		$science->defend = 0;
		$science->hitpoints = 0;
		$science->speed = 0;
		$science->is_req_of = array();
	}

	include('templates/achievements/science_details.tmpl.inc');

	if ($AJAX_OUTPUT) {
		$r->add_value(ob_get_clean());
		echo $r;
	}
	else
		ob_flush();
	die();
}


include ('templates/achievements/achievements.tmpl.inc');
if ($AJAX_OUTPUT) {
	$r->add_value(ob_get_clean());
	echo $r;
}
else
	ob_flush();
?>