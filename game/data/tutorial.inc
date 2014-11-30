<?php
include ('../include/std_include.inc');
$r = new ClientServerResponse();
$current_user = User::get_current_user();
include_once ('../include/shared/classes/Language.inc');
Language::get_instance()->load_language_file('tutorial');

$expected_data = array (
	'tut_flag'	=> '(e)int',
	'magic'		=> '(e)int'
);
$GP = $cryptObj->load_GET_POST($expected_data);
if (isset($GP['tut_flag'])) {
	$current_user->add_tutorial($GP['tut_flag']);
}
else if (isset($GP['magic'])) {
	if ($GP['magic'] > 0 && $GP['magic'] < 2) {
		$fn = "tutorial_magic{$GP['magic']}";
		Server::get_instance(CONF_ENGINE_SERVER_URL)->$fn(0);
	}
}
?>