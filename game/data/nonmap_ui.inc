<?php
require_once ('../include/std_include.inc');
$current_user = User::get_current_user();
$expected_data = array (
	'ajax_get_next_page'		=> 	'ebool'
);

// Diese Seiten werden in der Reihenfolge per Next angezeigt
$pages = array ('island_left.php', 'choose_map.php');

$state = $current_user->get_state();
switch ($state) {
	case User::STATE_CHOOSING_MAPS:
		$cur_page = 'choose_map.php';
		break;
	case User::STATE_CHOOSING_SCIENCES_LOST_LAST_MAP:
	case User::STATE_CHOOSING_SCIENCES_WON_LAST_MAP:
		$cur_page = 'island_left.php';
		break;
	default:
		kill_session();
}

$cur_page_index = array_search($cur_page, $pages);

if (isset($GP['ajax_get_next_page'])) {
	echo $pages[$cur_page_index+1];
	die();
}

include('templates/nonmap_ui.tmpl.inc');
?>