<?php
require_once ('../include/std_include.inc');

$r = new ClientServerResponse();
$expected_data = array(
	'email'	=>	'string'
);

$GP = $cryptObj->load_GET_POST($expected_data);

if (!isset($GP['email'])) {
	$r->add_value(0);
	die($r);
}


try {
	Server::get_instance(CONF_ENGINE_SERVER_URL)
		->cmd_set_new_pass($GP['email']);
	$r->add_value(1);
}
catch (Exception $e) {
	$r->add_value(0);
}
die($r);
?>