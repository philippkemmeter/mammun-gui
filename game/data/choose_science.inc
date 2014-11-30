<?php
include_once ('../include/std_include.inc');
if (isset($_GET['ajaxid'])) {// Ein ping-und-echo-Parameter für AJAX-Request-Zuweisungen
	$AJAX_OUTPUT = true;
	$r = new ClientServerResponse();
	$r->add_value($_GET['ajaxid']);
	$this_win_id = $_GET['ajaxid'];
	ob_start();
}
else
	$AJAX_OUTPUT = false;
$current_user = User::get_current_user();
/**
 * Checken, ob der Inhalt der Seite überhaupt zutreffend ist. Wenn der User-State
 * nicht stimmt, wurde entweder versucht zu bescheißen, oder im Spielcode ist
 * ein Designfehler. Kann auch vorkommen, wenn sich der State im laufenden Spiel
 * ändert. In jedem Fall löst ein kill_session() die Situation.
 */
if (($current_user->get_state() != User::STATE_CHOOSING_SCIENCES_LOST_LAST_MAP) &&
	($current_user->get_state() != User::STATE_CHOOSING_SCIENCES_WON_LAST_MAP))
{
	kill_session();
}
include_once ('../include/classes/Techtree.inc');
include_once ('../include/classes/map/MapPlayers.inc');
require_once('/../include/shared/classes/score/GameScoreCalculation.inc');
$techtree = Techtree::get_instance($current_user->get_id());
$score = MapPlayers::get_instance(
	$current_user->get_world_id(),
	$current_user->get_map_id()
)->get_last_score($current_user->get_id());
$last_score = $current_user->get_last_score();
		$new_score = $current_user->get_cur_score();
		$amount = GameScoreCalculation::calc_level_by_score($new_score)-
			GameScoreCalculation::calc_level_by_score($last_score);
die();

include_once ('../include/classes/ScienceChooser.inc');
include_once ('../include/classes/helper/Converter.inc');
Language::get_instance()->load_language_file('choose_science');
$expected_data = array (
	'sc_id'		=>	'seint',
	'finish'	=>	'ebool',
	'reset'		=>	'ebool'
);

$GP = $cryptObj->load_GET_POST($expected_data);


$science_chooser = ScienceChooser::get_instance($current_user->get_id());




function encrypt_scs_array($array, $filter=false) {
	global $cryptObj;
	if (!empty($filter))
		$techtree = Techtree::get_instance($current_user->get_id());

	$result = array();
	$i = 0;
	foreach ($array as $sc_id) {

		if (empty($filter)
			|| ($techtree->get_science_cur_level($sc_id) < $filter[$sc_id]))
		{
			$result[$i]['id'] = $cryptObj->do_encryption(array('sc_id' => $sc_id));
			$result[$i]['name'] = ScienceConstants::$SC_NAME[$sc_id];
			$i++;
		}
	}
	return $result;
}
function get_current_selectable_scs_encrypted() {
	global $cryptObj;
	$techtree = Techtree::get_instance($current_user->get_id());
	$researchable_scs = $techtree->get_researchable_scs();
	$known_scs = ScienceChooser::get_instance($current_user->get_id())->get_known_sciences();
	$result = array();
	foreach($researchable_scs as $type => $data) {
		$result[$type] = encrypt_scs_array($data, $known_scs);
	}
	return $result;
}

/**
 * User hat alles gewählt und bestätigt, dass er diese Forschungen mitnehmen will.
 */
if ($GP['finish']) {
	$science_chooser->finish();
	User::get_instance_by_id($current_user->get_id())->set_state(User::STATE_CHOOSING_MAPS);
	echo "1";
	die();
}

/**
 * User möchte alles bisher gewählte verwerfen.
 */
if ($GP['reset']) {
	$scs_left = $science_chooser->reset();
	$selectable_sciences = get_current_selectable_scs_encrypted();
	$selected_sciences = encrypt_scs_array($science_chooser->get_selected_sciences());
	echo "1#$scs_left#"
			.Converter::arrayToJSON($selected_sciences)."#"
			.Converter::arrayToJSON($selectable_sciences);
	die();
}

/**
 * User hat die Forschung mit der ID $GP['sc_id'] gerade zum mitnehmen ausgewählt.
 */
if ($GP['sc_id']) {
	try {
		$scs_left = $science_chooser->select_science($GP['sc_id']);
		$selectable_sciences = get_current_selectable_scs_encrypted();
		$selected_sciences = encrypt_scs_array($science_chooser->get_selected_sciences());
		echo "1#$scs_left#"
			.Converter::arrayToJSON($selected_sciences)."#"
			.Converter::arrayToJSON($selectable_sciences);
		if (!$scs_left) {
			echo "#".LNG_TEXT_ALL_SCS_CHOSEN;
		}
		else if (empty($selectable_sciences)) {
			echo "#".str_replace("[TOTAL_AMOUNT]", $scs_left,LNG_TEXT_NOT_ENOUGH_RESEARCHED);
		}
		else {
			$all_empty = true;
			foreach ($selectable_sciences as $key => $data) {
				if (!empty($data)) {
					$all_empty = false;
					break;
				}
			}
			if ($all_empty)
				echo "#".str_replace("[TOTAL_AMOUNT]", $scs_left,LNG_TEXT_NOT_ENOUGH_RESEARCHED);
		}
	}
	catch (Exception $e) {
		echo "!#";
		switch ($e->getCode()) {
			case ScienceChooser::ERR_MAX_SELECTABLE_SCIENCES_REACHED:
				echo LNG_ERR_MAX_SELECTABLE_SCIENCES_REACHED;
				break;
			case ScienceChooser::ERR_SCIENCE_NEVER_KNOWN:
				echo LNG_ERR_SCIENCE_NEVER_KNOWN;
				break;
			case ScienceChooser::ERR_MAY_NOT_RESEARCH_SCIENCE:
				echo LNG_ERR_MAY_NOT_RESEARCH_SCIENCE;
				break;
			default:
				echo LNG_ERR_SCIENCE_ERROR;
		}
	}
	die();
}

$scs_left = $science_chooser->get_sel_scs_amount_total() - $science_chooser->get_selected_sciences_amount();
$selected_sciences = encrypt_scs_array($science_chooser->get_selected_sciences());
$selectable_sciences = get_current_selectable_scs_encrypted();

$sel_scs_last_map = $science_chooser->get_sel_scs_amount_from_last_map();
$sel_scs_older_maps = $science_chooser->get_sel_scs_amount_from_older_maps();

/**
 * Den richtigen Eröffnungs-Officer-Spruch wählen
 */

if (!$scs_left) {
	$officer_text = LNG_TEXT_ALL_SCS_CHOSEN;
}
else if (empty($selectable_sciences)) {
	$officer_text = str_replace("[TOTAL_AMOUNT]", $scs_left,LNG_TEXT_NOT_ENOUGH_RESEARCHED);
}
else {
	$all_empty = true;
	foreach ($selectable_sciences as $key => $data) {
		if (!empty($data)) {
			$all_empty = false;
			break;
		}
	}
	if ($all_empty) {
		$officer_text = str_replace("[TOTAL_AMOUNT]", $scs_left,LNG_TEXT_NOT_ENOUGH_RESEARCHED);
	}
	else {
		if ($science_chooser->get_last_map_research_amount() > 0) {
			if (!$sel_scs_last_map) {
				if ($sel_scs_older_maps)
					$officer_text =
						str_replace(
							'[SAVED_AMOUNT]',
							$sel_scs_older_maps,
							LNG_INTRO_TEXT_NO_NEW_BUT_OLD
						);
				else
					$officer_text = LNG_INTRO_TEXT_NO_NEW_NO_OLD;
			}
			else {
				if ($sel_scs_older_maps) {
					$officer_text =
						str_replace(
							array('[AMOUNT]', '[SAVED_AMOUNT]', '[TOTAL_AMOUNT]'),
							array($sel_scs_last_map, $sel_scs_older_maps, $sel_scs_last_map + $sel_scs_older_maps),
							LNG_INTRO_TEXT_NEW_AND_OLD
						);
				}
				else {
					$officer_text =
						str_replace(
							'[AMOUNT]',
							$sel_scs_last_map,
							LNG_INTRO_TEXT_NEW_BUT_NO_OLD
						);
				}
			}
		}
		else {
			$officer_text =
				str_replace(
					'[TOTAL_AMOUNT]',
					$sel_scs_last_map+$sel_scs_older_maps,
					LNG_INTRO_TEXT_NONE_RESEARCHED
				);
		}
	}
}



//$sc_obj = Science::get_instance($current_user->get_id());

include ('templates/choose_science/choose_science.tmpl.inc');
if ($AJAX_OUTPUT) {
	$r->add_value(ob_get_clean());
	echo $r;
}
?>