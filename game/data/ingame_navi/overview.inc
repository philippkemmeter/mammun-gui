<?php
require_once ('../include/classes/Resource.inc');
require_once ('../include/classes/Tribes.inc');
require_once ('../include/classes/payment/Payment.inc');
require_once ('../include/shared/classes/score/GameScoreCalculation.inc');
$uid = $current_user->get_id();
if (($current_user->get_state() == IUser::STATE_PLAYING)
	&& ($current_user->get_waiting_state() == 0))
{
	$r = Resource::get_instance($uid);
	$wood = $r->get_resource(0);
	$wood_gain = $r->get_resource_gain(0);
	$food = $r->get_resource(1);
	$food_gain = $r->get_resource_gain(1);
	$unit_limit = Tribes::get_instance($uid)->get_unit_limit();
}
else {
	$wood = 0;
	$wood_gain = 0;
	$food = 0;
	$food_gain = 0;
	$unit_limit = array(0,0);
}
$klunker = Payment::get_instance($uid)->get_klunker_amount();
$klunker_gain = 0;

$my_items = Converter::stdClassToArray(
	Server::get_instance(CONF_PAYMENT_SERVER_URL,
		CONF_MAMMUN_AUTH_SECRET)->get_my_items($uid, 0)
);

$items = 0;
foreach ($my_items as $datas) {
	foreach ($datas as $data) {
		$items += $data->amount;
		$items += $data->amount_mp;
	}
}

$items_gain = 0;

include ('templates/ingame_navi/overview.tmpl.inc');
?>