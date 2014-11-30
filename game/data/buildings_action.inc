<?php
/**
 * File created by Philipp Kemmeter on 13.02.2009 at 14:25:03
 */

include_once ('../include/std_include.inc');
require_once ('../include/classes/Tribes.inc');
require_once ('../include/classes/buildings/Buildings.inc');
require_once ('../include/classes/map/Sector.inc');
//ClientServer::$debug = true;

Language::get_instance()->load_language_file('building_action');

$current_user = User::get_current_user();
/**
 * Checken, ob der Inhalt der Seite zutreffend ist. Wenn der User-State
 * nicht stimmt, wurde entweder versucht zu schmuen, oder im Spielcode ist
 * ein Designfehler. In jedem Fall behebt ein kill_session() alles ;-).
 */
if ($current_user->get_state() != User::STATE_PLAYING
	&& !$current_user->get_waiting_state())
{
	echo "Wrong State...";
	kill_session();
}

$expected_data = array (
	'x'				=> 'int',
	'y'				=> 'int',
	'type'			=> 'int',
	'sector_id'		=> 'int',
	'construct'		=> 'bool',
	'destroy'		=> 'bool',
	'change_color'	=> 'bool',
	'world_id'		=> 'int',
	'map_id'		=> 'int',
	'id'			=> 'seint_nosession',
	'color'			=>	'int'
);
$GP = $cryptObj->load_GET_POST($expected_data);

function get_sector_buildings($sector_id) {
	$data = Server::get_instance(CONF_ENGINE_SERVER_URL)->get_map_data(
		User::get_current_user()->get_id(),
		User::get_current_user()->get_world_id(),
		User::get_current_user()->get_map_id(),
		$sector_id
	);
	return Buildings::convert_server_buildings($data->building);
}

$b_obj = Buildings::get_instance($current_user->get_id());
$r = new ClientServerResponse();
if ($GP['construct']) {
	if (!isset($GP['x']) || !isset($GP['y']) || !isset($GP['type']) ||
		!isset($GP['sector_id']))
	{
		$r->add_value("!");
		$r->add_value("#");
		die($r);// Hackattack
	}

	try {
		$GP['sector_id'] %= 100;
		$sector_c = new WorldCoordinates($GP['world_id'], $GP['map_id'],
			$GP['sector_id']);
		$klunker = $b_obj->construct($sector_c, $GP['x'], $GP['y'], $GP['type'],
			$GP['color']);
		$r_obj = Resource::get_instance($current_user->get_id());
		$unit_limit = Tribes::get_instance($current_user->get_id())
			->get_unit_limit();
		$sector_buildings =	get_sector_buildings($GP['sector_id']);
		$r->add_value($r_obj->get_resource(Resource::RESOURCE1));
		$r->add_value($r_obj->get_resource(Resource::RESOURCE2));
		$r->add_value($unit_limit[0]);
		$r->add_value($unit_limit[1]);
		$r->add_value($sector_buildings);
		$r->add_value(ScoreCache::get_map_score(
			$current_user->get_world_id(),
			$current_user->get_map_id(),
			$current_user->get_id()
		));
		$r->add_value(ScoreCache::get_league_score(0, $current_user->get_id()));
		if ($klunker !== false)
			$r->add_value($klunker);
	}
	catch (Exception $e) {
		switch($e->getCode()) {
			case Buildings::ERR_CANT_BUILD_THERE:
				$r->add_value(LNG_ERR_CANT_BUILD_THERE);
				break;
			case Buildings::ERR_NOT_ENOUGH_RESOURCES:
				$r_obj = Resource::get_instance($current_user->get_id());
				$buildable = Buildings::get_instance($current_user->get_id())
					->get_buildable_buildings();
				$cost = 0;
				foreach ($buildable as $b) {
					if ($b->get_type() == $GP['type']) {
						$cost = $b->get_cost();
						break;
					}
				}
				unset($buildable);
				$r->add_value(
					str_replace(array('[HAVE]', '[NEED]'),
								array(
									'<b>'.
										$r_obj->get_resource(Resource::RESOURCE1).
									'</b>',
									'<b>'.$cost.'</b>'),
								LNG_ERR_NOT_ENOUGH_RESOURCES
					)
				);
				break;
			case Buildings::ERR_NO_DOMINATION:
				$r->add_value(LNG_ERR_NO_DOMINATION);
				break;
			case Buildings::ERR_TOO_MANY_BUILDINGS_IN_SECTOR:
				$r->add_value(LNG_ERR_TOO_MANY_BUILDINGS);
				break;
			default:
				$r->add_value(LNG_ERR_STRANGE_ERROR);
		}
	}
}
else if ($GP['destroy']) {
	if(!isset($GP['sector_id'])) {
		$r->add_value("!");
		$r->add_value("#");
		die($r);// Hackattack
	}

	$GP['sector_id'] %= 100;

	try {
		$sector_c = new WorldCoordinates(
			$current_user->get_world_id(),
			$current_user->get_map_id(),
			$GP['sector_id']
		);
		$b_obj->destroy($sector_c, $GP['x'], $GP['y']);
		$sector_buildings =	get_sector_buildings($GP['sector_id']);
		$unit_limit = Tribes::get_instance($current_user->get_id())
			->get_unit_limit();
		$r_obj = Resource::get_instance($current_user->get_id());
		$r->add_value($r_obj->get_resource(Resource::RESOURCE1));
		$r->add_value($r_obj->get_resource(Resource::RESOURCE2));
		$r->add_value($unit_limit[0]);
		$r->add_value($unit_limit[1]);
		$r->add_value($sector_buildings);
		$world_id = $current_user->get_world_id();
		$map_id = $current_user->get_map_id();
		$r->add_value(ScoreCache::get_map_score(
			$world_id,
			$map_id,
			$current_user->get_id()
		));
		/**
		 * Der Dominator kann sich geändert haben, wenn die Regel aktiv ist,
		 * dass leere Sektoren immer niemandem gehören.
		 */
		$r->add_value(
			Sector::get_instance($sector_c)->get_dominator_nick()
		);
	}
	catch (HackattackException $e) {
		$r->add_exception(new Exception($e->getMessage(), $e->getCode()));
	}
	catch (Exception $e) {
		switch ($e->getCode()) {
			case Buildings::ERR_DONT_DESTROY_MAIN_BUILDINGS:
				$r->add_value(LNG_ERR_DONT_DESTROY_MAIN_BUILDINGS);
				break;
			default:
				error_log($e->getFile().":".$e->getLine().": ".$e->getMessage().", ".$e->getCode());
				$r->add_value(LNG_ERR_STRANGE_ERROR);
		}
	}
}
else if ($GP['change_color']) {
	if(!isset($GP['sector_id'])) {
		$r->add_value("!");
		$r->add_value("#");
		die($r);// Hackattack
	}

	$GP['sector_id'] %= 100;
	$sector_c = new WorldCoordinates($GP['world_id'], $GP['map_id'],
			$GP['sector_id']);
	try {
		$klunker = $b_obj->change_color($sector_c, $GP['x'], $GP['y'],
			$GP['color']);
		$sector_buildings =	get_sector_buildings($GP['sector_id']);
		$r_obj = Resource::get_instance($current_user->get_id());
		$r->add_value(1);
		$r->add_value($sector_buildings);
		if ($klunker !== false)
			$r->add_value($klunker);
	}
	catch (HackattackException $e) {
		$r->add_exception(new Exception($e->getMessage(), $e->getCode()));
	}
	catch (Exception $e) {
		error_log($e->getFile().":".$e->getLine().": ".$e->getMessage().", ".$e->getCode());
		$r->add_value(LNG_ERR_STRANGE_ERROR);
	}
}
die($r);
?>