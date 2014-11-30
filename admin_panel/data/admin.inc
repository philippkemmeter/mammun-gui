<?php
/**
 * File created by Philipp Kemmeter on 27.02.2009 at 15:47:20
 */

include_once ('../include/std_include.inc');
$current_user = User::get_current_user();

$lower_nick = strtolower($current_user->get_nick());
if (!in_array($lower_nick, array('philipp', 'robert')))
	die();

$expected_data = array (
	'reset'					=> 'bool',
	'gen_science_data'		=> 'bool',
	'get_user_statistics'	=> 'bool'
);
$GP = $cryptObj->load_GET_POST($expected_data);

if (!empty($GP)) {
	$admin_server = Server::get_instance(
		CONF_ENGINE_SERVER_MAIN_URL.'/admin_interface_for_gui.php'
	);
	if ($GP['reset']) {
		if ($lower_nick !== 'philipp')
			die();

		echo "Resetting. This may take a while...";
		flush();
		ob_flush();
		$result = $admin_server->reset_all_accounts(1);
		if ($result == 1)
			echo "OK<br/>\n";
		else {
			echo "Error<br/>\n";
			print_r($result);
		}
		die();
	}
	else if ($GP['gen_science_data']) {
		if ($lower_nick !== 'philipp')
			die();
		die($admin_server->generate_science_data(1));
	}
	else if ($GP['get_user_statistics']) {
		$statistics = $admin_server->get_user_statistics();
		die(
			"Users registered in total: {$statistics->registered_user_amount}<br/>".
			"Users registered with account connected with fb: ".
				"{$statistics->registered_fb_user_amount}<br/>".
			"<br/>".
			"Users active in total (a login this week and more than 5 logins): ".
				"{$statistics->active_user_amount}<br/>".
			"Users active with account connected with fb ".
				"(a login this week and more than 5 logins): ".
				"{$statistics->active_fb_user_amount}<br/>".
			"<br/>".
			"Users online last 60 seconds: {$statistics->online_user_amount}"
		);
	}

}

include('templates/admin.tmpl.inc');
?>