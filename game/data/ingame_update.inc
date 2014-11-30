<?php
/**
 * Diese Datei ist die Schnittstelle der GUI, um Ingame-Sachen anzufragen.
 * Also Kartendaten, Punktestände usw. Alles, was Live benötigt wird.
 *
 * @author PhilippKemmeter
 */

require_once ('../include/std_include.inc');

$expected_data = array (
	'my_score_level'	=>	'bool',
	'map_ranking'		=>	'bool',
	'map_data_update'	=>	'bool',
	'map_data_full'		=>	'bool',
	'my_event_log'		=>	'bool',
	'xp_bar_data'		=>	'bool',
	'world_id'			=>	'int',
	'map_id'			=>	'int'
);

$GP = $cryptObj->load_GET_POST($expected_data);


$uid = User::get_current_user()->get_id();
if (isset($GP['map_id'])) {
	$map_id = $GP['map_id'];
	unset($GP['map_id']);
}
else {
	$map_id = User::get_current_user()->get_map_id();
}
if (isset($GP['world_id'])) {
	$world_id = $GP['world_id'];
	unset($GP['world_id']);
}
else {
	$world_id = User::get_current_user()->get_world_id();
}

try {
	$r = new ClientServerResponse();
	$r->add_value(1);
	foreach ($GP as $command => $bool) {
		switch ($command) {
			case 'my_score_level':
				require_once ('../include/classes/ScoreCache.inc');
				require_once (
					'../include/shared/classes/score/GameScoreCalculation.inc');
				$score_obj = new stdClass();
				$score_obj->map_score = ScoreCache::get_map_score(
					$world_id, $map_id, $uid, true);
				$score_obj->league_score = ScoreCache::get_league_score(
					0, $uid, true);
				$score_obj->level = GameScoreCalculation::calc_level_by_score(
					$score_obj->league_score);
				$score_obj->score_last_level
					= GameScoreCalculation::calc_score_by_level($score_obj->level);
				$score_obj->score_next_level
					= GameScoreCalculation::calc_score_by_level($score_obj->level+1);
				$r->add_value($score_obj);
				break;
			case 'map_ranking':
				break;
			case 'map_data_full':
				$map_data = Server::get_instance(CONF_ENGINE_SERVER_URL)
					->get_map_data($uid, $world_id, $map_id);
				$r->add_value($map_data);
				break;
			case 'map_data_update':
				$map_data = Server::get_instance(CONF_ENGINE_SERVER_URL)
					->get_map_data($uid, $world_id, $map_id);
				unset($map_data->mentor_map);
				unset($map_data->closed);
				unset($map_data->bio);
				$r->add_value($map_data);
				break;
			case 'my_event_log':
				require_once ('../include/classes/EventMessages.inc');
				$r->add_value(EventMessages::get_my_log());
				break;
			case 'xp_bar_data':
				require_once ('../include/shared/classes/score/'.
					'GameScoreCalculation.inc');
				$data = new stdClass();
				$data->lvl_up_present
					= Server::get_instance(CONF_ENGINE_SERVER_URL)
						->get_level_up_present(User::get_current_user()
							->get_cur_level(true)+1);
				$data->cur_level = User::get_current_user()
							->get_cur_level();
				$data->score_next_level =
					GameScoreCalculation::calc_score_by_level(
						User::get_current_user()->get_cur_level()+1);
				$data->score_last_level =
					GameScoreCalculation::calc_score_by_level(
						User::get_current_user()->get_cur_level());
				$data->score = User::get_current_user()->get_cur_score();
				$r->add_value($data);
				break;
		}
	}
}
catch(Exception $e) {
	$r = new ClientServerResponse();
	$r->add_value(0);
	$r->add_value($e->getMessage());
}
die($r);
?>