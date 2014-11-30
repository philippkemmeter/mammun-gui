<?php
require_once ('../include/classes/Tribes.inc');
include_once('../include/classes/Science.inc');
$uid = User::get_current_user()->get_id();
$scientist_tribe
	= Tribes::get_instance($uid)->get_tribes_by_race(2);
$scientist_tribe = $scientist_tribe[0];
$sc_total = $scientist_tribe->get_amount();
$sc_at_work = Science::get_instance($uid)->get_scientists_at_work();

include('templates/ingame_navi/top_buttons.tmpl.inc');
?>