<?php
include_once ('../include/std_include.inc');

$current_user = User::get_current_user();

$expected_data = array(
	'action'		=> 	'string'
);

$GP=$cryptObj->load_GET_POST($expected_data);

if (isset($GP['delete_account'])) {
	Server::get_instance(CONF_ENGINE_SERVER_URL)->cmd_accounting_delete();
}
?>