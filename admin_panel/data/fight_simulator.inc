<?php
include_once ('../include/std_include.inc');
$current_user = User::get_current_user();

if (strtolower($current_user->get_nick()) !== 'philipp')
	die();
	
$expected_data = array (
	'att1'	=>	'int',
	'att2'	=>	'int',
	'def1'	=>	'int',
	'def2'	=>	'int',
	'cost1'	=>	'int',
	'cost2'	=>	'int',
	'hp1'	=>	'int',
	'hp2'	=>	'int',
	'cycle'	=>	'int',
	'food'	=>	'int',
	'submitok'	=>	'bool'
);
$GP = $cryptObj->load_GET_POST($expected_data);

include('templates/header.tmpl.inc');


$att1 = $GP['att1'] ? $GP['att1'] : 0;
$att2 = $GP['att2'] ? $GP['att2'] : 0;
$def1 = $GP['def1'] ? $GP['def1'] : 1;
$def2 = $GP['def2'] ? $GP['def2'] : 1;
$cost1 = $GP['cost1'] ? ($GP['cost1']/1000) : 1;
$cost2 = $GP['cost2'] ? ($GP['cost2']/1000) : 1;
$hp1 = $GP['hp1'] ? $GP['hp1'] : 1;
$hp2 = $GP['hp2'] ? $GP['hp2'] : 1;
$cycle = $GP['cycle'] ? $GP['cycle'] : 3600;
$food_invested = $GP['food'] ? $GP['food'] : 10000;
if ($GP['submitok']) {	
	$viruses=$food_invested/$cost1;
    $enemies=$food_invested/$cost2;
    $total=$viruses+$enemies;
    $vipo1=$viruses*$att1/$enemies/$def2;
    $vipo2=$enemies*$att2/$viruses/$def1;
    $vipo11=pow($vipo1,0.75);
    $vipo22=pow($vipo2,0.75);
    $dead1=$vipo11*$enemies*$cycle/$hp2/500;
    $dead2=$vipo22*$viruses*$cycle/$hp1/500;
    if ($dead1>$enemies) $dead1=$enemies;
    if ($dead2>$viruses) $dead2=$viruses;
    $bio1=$dead1*$cost2;
    $bio2=$dead2*$cost1;
    if ($bio1>$bio2)
    {
		$winner=1;
		$ratio=$bio1/$bio2;
		$ratio2=$dead1/$dead2;
    }
    if ($bio2>$bio1)
    {
		$winner=2;
		$ratio=$bio2/$bio1;
		$ratio2=$dead2/$dead1;
    }
    
    include('templates/fight_simulator/fight_simulator_result.tmpl.inc');
}
include('templates/fight_simulator/fight_simulator.tmpl.inc');

include('templates/footer.tmpl.inc');
?>