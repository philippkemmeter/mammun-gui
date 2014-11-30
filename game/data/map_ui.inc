<?php
/**
 * @author Philipp Kemmeter
 */
include_once('../include/classes/MapDescriptionData.inc');
include_once('../include/classes/map/MapPlayers.inc');
include_once('../include/shared/classes/IEventLog.inc');

Language::get_instance()->load_language_file('map_ui');

if (!$current_user->has_tutorial(User::TUT_FLAGGROUP_FIRST_STEPS
	| User::TUT_FLAGGROUP_BUILD_EXPAND))
{
	Language::get_instance()->load_language_file('tutorial');
}

if (!isset($world_id) || !isset($map_id)) {
	$world_id = User::get_current_user()->get_world_id();
	$map_id = User::get_current_user()->get_map_id();
}


$x = Server::get_instance(CONF_ENGINE_SERVER_URL)->get_template_data(
	$world_id, $map_id
);
MapDescriptionData::set_instance($x->world_id, $x->map_id, $x);
$template_id = MapDescriptionData::get_instance($x->world_id, $x->map_id)
	->get_template_id();
$world_id = $x->world_id;
$map_id = $x->map_id;
$auth_key = MapChooserPrivate::get_auth_key_for_current_map(
	$current_user->get_id()
);
$auth_key_ext = $auth_key.$current_user->get_id();
$map_start_time =
	(int)MapPlayers::get_instance($world_id, $map_id)->get_start_time(
		$current_user->get_id());

/**
 * DAS KANN MAN LÖSCHEN (glaub ich), WEIL ES KOMPLETT ÜBER DIE ENGINE UND
 * EVENTS GELÖST WIRD!
 */
if (isset($map_key_map_data)) {
	$inviting_nick = User::get_instance_by_id(
		$map_key_map_data->res_by_uid)->get_nick();

	/**
	 * Erste Karte
	 */
	if ($current_user->get_last_world_id() == 0) {
		/**
		 * Der Spieler bekommt seine Nachricht über eine EventMessage von
		 * der Engine, deswegen ist das folgendes nicht mehr nötig
		 */
		/*if ($map_key_map_data->map_id != $current_user->get_map_id()) {
			$auth_key_txt = str_replace(
				array('[NICK]'),
				array($inviting_nick),
				LNG_SETTLE_BUT_FULL_FIRST_MAP
			);
			$invitation_failed = true;
		}*/
	}
	/**
	 * (X>1)ste Karte
	 */
	else {
		if ($map_key_map_data->min_level > $current_user->get_cur_level()){
			$auth_key_txt = str_replace(
				array('[NICK]', '[NEED_XP]', '[HAVE_XP]'),
				array(
					User::get_main_part_of_nick($inviting_nick),
					$map_key_map_data->min_level,
					$current_user->get_cur_level()
				),
				LNG_SETTLE_BUT_TOO_LESS_EXP
			);
			$invitation_failed = true;
		}
		else if ($current_user->get_state() == User::STATE_PLAYING) {
			$auth_key_txt = str_replace('[NICK]',
				User::get_main_part_of_nick($inviting_nick),
				LNG_SETTLE_BUT_ON_MAP
			);
			$invitation_failed = false;
		}
		else if (count(MapPlayers::get_instance($world_id, $map_id)
			->get_uids_on_map()) >=
				MapDescriptionData::get_instance($world_id, $map_id)
					->get_max_players())
		{
			$auth_key_txt = str_replace('[NICK]',
				User::get_main_part_of_nick($inviting_nick),
				LNG_SETTLE_BUT_FULL
			);
			$invitation_failed = true;
		}
		else {
			$auth_key_txt = str_replace('[NICK]',
				User::get_main_part_of_nick($inviting_nick),
				LNG_WONNA_SETTLE_HERE
			);
			$invitation_failed = false;
		}
	}
}

// 128, 32, 16, 8, 4, 2, 1
/**
 * Tutorial
 */
$tutorials = null;
if (!$current_user->has_tutorial(User::TUT_FLAGGROUP_FIRST_STEPS)) {
	$tutorials = new stdClass();
	/**
	 * Das Tutorial, das angezeigt werden soll
	 * @var string
	 */
	$tutorials->first_steps = new stdClass();
	$tutorials->build_and_expand = new stdClass();
	/**
	 * Die Texte der einzelnen Tutorial-Schritte
	 * @var assoc
	 */
	$tutorials->first_steps->tutorial_texts = array (
		'show_intro_text'	=> LNG_TUT_INTRO,
		'show_slide_show'	=> array(
			LNG_TUT_SLIDE_EXPAND_CAPTION,
			LNG_TUT_SLIDE_EXPAND_TEXT,
			LNG_TUT_SLIDE_SCIENCE_CAPTION,
			LNG_TUT_SLIDE_SCIENCE_TEXT,
			LNG_TUT_SLIDE_TRAINING_CAPTION,
			LNG_TUT_SLIDE_TRAINING_TEXT,
			LNG_TUT_SLIDE_BUILD_CAPTION,
			LNG_TUT_SLIDE_BUILD_TEXT
		),
		'slide_show_outro'	=> array(
			LNG_TUT_SLIDE_OUTRO1,
			LNG_TUT_SLIDE_OUTRO1_INVENT,
			LNG_TUT_SLIDE_OUTRO1_TRAIN,
			LNG_TUT_SLIDE_OUTRO1_BUILD,
			LNG_TUT_SLIDE_OUTRO2
		),
		'home_sector'		=> LNG_TUT_CLICK_HS,
		'choose_unit_move'	=> LNG_TUT_IN_HS,
		'move_unit'			=> LNG_TUT_MOVE,
		'click_move_arrow'	=> LNG_TUT_CLICK_MOVE_ARROW,
		'close_move_arrow'	=> LNG_TUT_CLOSE_MOVE_ARROW,
		'open_science'		=> LNG_TUT_OPEN_SCIENCE,
		'start_research'	=> LNG_TUT_START_SCIENCE,
		'quit_research'		=> LNG_TUT_SCIENCE_OK,
		'open_training'		=> LNG_TUT_OPEN_TRAINING,
		'show_outro_text'	=> LNG_TUT_OUTRO,
		'home_sector2'		=> LNG_TUT_CLICK_HS2,
		'choose_unit_move2'	=> LNG_TUT_CHOOSE_SECOND,
		'move_unit2'		=> LNG_TUT_MOVE_SECOND
	);
	/**
	 * Diese Flags werden gesetzt, wenn der entsprechende Tutorial-Schritt
	 * beendet wurde
	 * @var assoc
	 */
	$tutorials->first_steps->tutorial_flags = array(
		'slide_show_outro'	=> User::TUT_FLAG_INTRO,
		'move_unit'			=> User::TUT_FLAG_MOVE_UNIT,
		'start_research'	=> User::TUT_FLAG_DO_SCIENCE,
		'training_indivi'	=> User::TUT_FLAG_DO_TRAINING,
		'move_unit2'		=> User::TUT_FLAG_MOVE_UNIT2,
		'show_outro_text'	=> User::TUT_FLAG_FIST_STEPS_DONE
	);
	/**
	 * Und nun wird anhand der aktuell gesetzten Flags geschaut, bei welchem
	 * Schritt wir starten müssen.
	 */
	if (!$current_user->has_tutorial(User::TUT_FLAG_INTRO))
		$tutorials->first_steps->tutorial_start_step = 'show_intro_text';
	else if (!$current_user->has_tutorial(User::TUT_FLAG_MOVE_UNIT))
		$tutorials->first_steps->tutorial_start_step = 'home_sector';
	else if (!$current_user->has_tutorial(User::TUT_FLAG_DO_SCIENCE))
		$tutorials->first_steps->tutorial_start_step = 'click_move_arrow';
	else if (!$current_user->has_tutorial(User::TUT_FLAG_MOVE_UNIT2))
		$tutorials->first_steps->tutorial_start_step = 'home_sector2';
	else if (!$current_user->has_tutorial(User::TUT_FLAG_DO_TRAINING))
		$tutorials->first_steps->tutorial_start_step = 'open_training';
	else
		$tutorials->first_steps->tutorial_start_step = 'show_outro_text';

}
/**
 * Wir müssen das Tutorial auch dann vorbereiten, wenn das vorherige ansteht,
 * deswegen hier kein ELSE IF, sondern IF.
 */
if (!$current_user->has_tutorial(User::TUT_FLAGGROUP_BUILD_EXPAND)) {
	if (!$tutorials)
		$tutorials = new stdClass();
	/**
	 * Wir wollen das Baumenü erst dann einführen, wenn der User bereits Land
	 * gut gemacht hat und lernen soll, dass man Bauen muss, bevor man
	 * weiterzieht.
	 *
	 * Dazu prüfen wir ganz einfach, wie viele Sektoren er aktuell besitzt.
	 * Sind es mehr als einer, dann hat er sich ausgebreitet und wir zeigen
	 * den Dialog an, sonst passiert nichts.
	 */
	$x = Server::get_instance(CONF_ENGINE_SERVER_URL)
		->get_dominated_sectors(0);

	$sectors_dominated = count($x);

	/**
	 * Wenn das Tutorial FIRST_STEPS noch nicht gesehen wurden, dann ist
	 * alles bereit oben konfiguriert worden im vorherigen IF-Block.
	 *
	 * Andernfalls müssen wir den Startpunkt des Tutorials ermitteln.
	 */
	if ($current_user->has_tutorial(User::TUT_FLAGGROUP_FIRST_STEPS)) {
		if ($sectors_dominated <= 1) {
			$tutorials->first_steps->tutorial_start_step = 'wait_until_arrived';
		}
		else if ($sectors_dominated > 1) {
			/**
			 * Und nun wird anhand der aktuell gesetzten Flags geschaut, bei
			 * welchem Schritt wir starten müssen.
			 */
			if (!$current_user->has_tutorial(User::TUT_FLAG_DO_BUILDING))
				$tutorials->build_and_expand->tutorial_start_step = 'open_buildings';
			else if (!$current_user->has_tutorial(User::TUT_FLAG_MOVE_UNIT_AFTER_BUILD))
				$tutorials->build_and_expand->tutorial_start_step = 'choose_unit_move';
			else if (!$current_user->has_tutorial(User::TUT_FLAG_MOVE_SECOND_UNIT_AFTER_BUILD))
				$tutorials->build_and_expand->tutorial_start_step = 'move_second_unit';
			else if (!$current_user->has_tutorial(User::TUT_FLAG_BUILDINGS_DONE))
				$tutorials->build_and_expand->tutorial_start_step
					= 'show_outro_text';
			else
				$tutorials->build_and_expand->tutorial_start_step
					= 'invite_friends';
		}
	}
	/**
	 * Die Texte der einzelnen Tutorial-Schritte für das
	 * @var assoc
	 */
	$tutorials->build_and_expand->tutorial_texts = array (
		'open_buildings'	=> LNG_TUT_OPEN_BUILDINGS,
		'choose_sector_build' => LNG_TUT_CHOOSE_SECTOR_TO_PLACE,
		'place_building'	=> LNG_TUT_PLACE_BUILDING,
		'choose_unit_move'	=> LNG_TUT_CHOOSE_UNIT_AFTER_BUILD,
		'move_unit'			=> LNG_TUT_MOVE_UNIT_AFTER_BUILD,
		'move_second_unit'	=> LNG_TUT_MOVE_SECOND_UNIT_AFTER_BUILD,
		'invite_friends'	=> LNG_TUT_INVITE,
		'show_outro_text'	=> LNG_TUT_BUILDINGS_OUTRO
	);
	/**
	 * Diese Flags werden gesetzt, wenn der entsprechende Tutorial-Schritt
	 * beendet wurde
	 * @var assoc
	 */
	$tutorials->build_and_expand->tutorial_flags = array(
		'place_building'	=> User::TUT_FLAG_DO_BUILDING,
		'move_unit'			=> User::TUT_FLAG_MOVE_UNIT_AFTER_BUILD,
		'move_second_unit'	=> User::TUT_FLAG_MOVE_SECOND_UNIT_AFTER_BUILD,
		'invite_friends'	=> User::TUT_FLAG_INVITE,
		'show_outro_text'	=> User::TUT_FLAG_BUILDINGS_DONE
	);
}
include ('templates/map_ui.tmpl.inc');
?>