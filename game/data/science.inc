<?php
/**
 * Diese Datei zeigt Forschungsstatusse und Dialoge an. Per AJAX geladen.
 */

require_once ('../include/std_include.inc');
require_once ('../include/classes/Tribes.inc');
if (isset($_GET['ajaxid'])) {// Ein ping-und-echo-Parameter für AJAX-Request-Zuweisungen
	$AJAX_OUTPUT = true;
	$r = new ClientServerResponse();
	$r->add_value($_GET['ajaxid']);
	$this_win_id = $_GET['ajaxid'];
}
ob_start();
$current_user = User::get_current_user();
Language::get_instance()->load_language_file('science');
Language::get_instance()->load_language_file('ScienceConstants');

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
		if ($AJAX_OUTPUT) {
			$r->add_value(ob_get_clean());
			echo $r;
		}
		else
			ob_flush();
		die();
	}
	else
		kill_session();
}



include_once ('../include/classes/Science.inc');
$expected_data = array (
	'ajaxwinid' 	=> 	'int',
	'set_sc'		=> 	'(e)int',
	'sc_id'			=>  'eint',
	'cur_sc'		=> '(e)int'
);
$GP = $cryptObj->load_GET_POST($expected_data);


$sc_obj = Science::get_instance($current_user->get_id());
$techtree = $sc_obj->get_techtree();
//print_r($GP);
if (isset($GP['set_sc'])) {
	if (!$GP['sc_id'])
		die("strange");
	try {
		$projects = $sc_obj->get_science_projects();
		$old_count = count($projects);
		$sc_obj->start_research($GP['sc_id'], $GP['set_sc']*1000);
		$projects = $sc_obj->get_science_projects();
		if ($old_count < count($projects))
			$officer_text = str_replace('[PROJECT_NAME]',
				ScienceConstants::$SC_NAME[$GP['sc_id']],
				LNG_NEW_PROJECT_STARTED);
		else if ($old_count == count($projects)) {
			if ($GP['set_sc'] < $GP['cur_sc'])
				$officer_text = LNG_SCIENTISTS_REMOVED;
			else {
				/*if ($GP['set_sc'] > 14000)
					$officer_text = LNG_SCIENTISTS_ADDED_TOO_MANY;
				else*/
					$officer_text = LNG_SCIENTISTS_ADDED;
			}
		}
		else
			$officer_text = LNG_PROJECT_PAUSED;
	}
	catch(HackattackException $e) {
		echo $e->getMessage();
	}
	catch(Exception $e) {
		switch ($e->getCode()) {
			case Science::ERR_TOO_MANY_PROJ_PARALLEL:
				$officer_text = '<span class="error">'.
					LNG_TOO_MANY_PROJECT_PARALLEL.'</span><br/>'.
					LNG_TOO_MANY_PROJECT_PARALLEL_HINT;
				break;
			case Science::ERR_NO_PROJ_POSSIBLE_AT_ALL:
				echo LNG_NO_TC_UNDER_CONSTRUCTION_OR_NA;
				break;
			case Science::ERR_ALL_SCIENTISTS_AT_WORK:
				$officer_text = '<span class="error">'.
					LNG_ALL_SCIENTISTS_AT_WORK.'</span><br/>'.
					LNG_ALL_SCIENTISTS_AT_WORK_HINT;
				break;
			default:
				echo "Unknown error occured.";
				//errormail
				break;
		}
	}
}

$researchable_scs = $techtree->get_researchable_scs();
if (!isset($researchable_scs['general']))
	$researchable_scs['general'] = array();
if (!isset($researchable_scs['races']))
	$researchable_scs['races'] = array();
if (!isset($researchable_scs['abilities']))
	$researchable_scs['abilities'] = array();
if (!isset($researchable_scs['buildings']))
	$researchable_scs['buildings'] = array();
$science_projects = $sc_obj->get_science_projects();
$scientist_tribe
	= Tribes::get_instance($current_user->get_id())->get_tribes_by_race(2);
$scientist_tribe = $scientist_tribe[0];

$sc_total = $scientist_tribe->get_amount();
$sc_at_work = Science::get_instance(User::get_current_user()->get_id())
	->get_scientists_at_work();

if (empty($officer_text)) {
	if ($science_projects)
		$officer_text = LNG_INTRO_ALL_IS_RUNNING;
	else
		$officer_text = LNG_INTRO_NOTHING_TO_DO;
	$popup_officer_text = false;
}
else {
	$popup_officer_text = true;
}


include('templates/science/science.tmpl.inc');
if ($AJAX_OUTPUT) {
	$r->add_value(ob_get_clean());
	echo $r;
}
else
	ob_flush();
?>
