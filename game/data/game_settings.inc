<?php
/**
 * Diese Datei zeigt Forschungsstatusse und Dialoge an. Per AJAX geladen.
 */
include('../include/shared/classes/ISO3166.inc');
require_once ('../include/std_include.inc');
if (isset($_GET['ajaxid'])) {// Ein ping-und-echo-Parameter für AJAX-Request-Zuweisungen
	$AJAX_OUTPUT = true;
	$r = new ClientServerResponse();
	$r->add_value($_GET['ajaxid']);
	$this_win_id = $_GET['ajaxid'];
}
ob_start();

Language::get_instance()->load_language_file('game_settings');
Language::get_instance()->load_language_file('countries_iso3166');

include_once('../include/shared/classes/ISO3166.inc');

$current_user = User::get_current_user(true);
$pd = $current_user->get_personal_data();

$birthday_array = explode('-', $pd->birthday);
$birth_year = $birthday_array[0];
$birth_month = $birthday_array[1];
$birth_day = $birthday_array[2];

$expected_data = array (
	'name' 			=> 	'string',
	'email' 		=> 	'string',
	'password' 		=> 	'string',
	'old_pass'		=> 	'string',
	'gender'		=> 	'int',
	'birth_day' 	=> 	'int',
	'birth_month' 	=> 	'int',
	'birth_year' 	=> 	'int',
	'gender' 		=> 	'string',
	'city' 			=> 	'string',
	'country' 		=> 	'string',
	'send'			=>	'bool',
	'delete_account'=>	'bool'
);
$GP = $cryptObj->load_GET_POST($expected_data);

if ($AJAX_OUTPUT) {
	$r->add_value(ob_get_clean());
	echo $r;
}
else
	ob_flush();

$all_countries = ISO_3166::$all_codes;
$my_country_array = array();
for ($i = 0; $i < count($all_countries); $i++)
{
	$my_country_array[$all_countries[$i]] =
		constant("LNG_ISO3166_".$all_countries[$i]);
}
asort($my_country_array, SORT_STRING);

if (isset($GP['delete_account'])) {
	$r = new ClientServerResponse();
	Server::get_instance(CONF_ENGINE_SERVER_URL)->cmd_accounting_delete();
	$r->add_relogin_command('');
	die($r);
}

if(isset($GP['password'])) {
	$r = new ClientServerResponse();
	try {
		Server::get_instance(CONF_ENGINE_SERVER_URL)
			->cmd_set_new_pass($GP['email'], $GP['password'], $GP['old_pass']);
		/**
		 * Community: Userdaten anpassen (Passwort)
		 */
		ClientServer::send_request(CONF_WCF_SERVER_URL
			.'/gausus_wcf_hacks/change_userdata_interface.php'
			.'?s='.$_SESSION['WCF_SID']
			.(isset($GP['password']) ? '&pass='.$GP['password'] : ''),
		true);

		$r->add_value(1);
		$r->add_value(LNG_PASSWORD_STORED);
	}
	catch(Exception $e)
	{
		$r->add_value(0);
		switch($e->getCode()) {
			case 1000:
				$r->add_value(LNG_OLD_PASSWORD_WRONG);
				break;
			case 1001:
				$r->add_value(LNG_PASSWORD_TOO_SHORT);
				break;
			default:
				$r->add_value(LNG_STRANGE_ERROR_PW_SET);
	    }
	}
	die($r);
}

if (isset($GP['send']))
{
	$user_name_array = explode(' ', $GP['name']);
	$user_first_name = $user_name_array[0];
	$user_last_names = '';
	for($i = 1; $i < count($user_name_array); $i++)
	{
		$user_last_names .= ' ' . $user_name_array[$i];
	}
	$user_last_names = trim($user_last_names);

	$user_birthday = $GP['birth_year'] . '-' .
					 $GP['birth_month'] . '-' .
					 $GP['birth_day'];

	$pd->firstnames = $user_first_name;
	$pd->surnames = $user_last_names;
	$pd->gender = $GP['gender'];
	$pd->birthday = $user_birthday;
	$pd->city = $GP['city'];
	$pd->country = $GP['country'];

	$r = new ClientServerResponse();
	try {
		/**
		 * Engine: Userdaten anpassen (E-Mail und Rest)
		 */
		Server::get_instance(CONF_ENGINE_SERVER_URL)
			->set_email_address($GP['email']);
		Server::get_instance(CONF_ENGINE_SERVER_URL)
			->set_user_personal($pd);
		/**
		 * Community: Userdaten anpassen (E-Mail)
		 */
		ClientServer::send_request(CONF_WCF_SERVER_URL
			.'/gausus_wcf_hacks/change_userdata_interface.php'
			.'?s='.$_SESSION['WCF_SID']
			.'&email='.$GP['email'],
		true);

		$r->add_value(1);
	}
	catch (Exception $e) {
		error_log($e->getMessage());
		$r->add_value(0);
		$r->add_value(LNG_STRANGE_ERROR_PW_SET);
	}
	die($r);
}
$avatar_data = ClientServer::send_request(CONF_WCF_SERVER_URL
	.'/gausus_wcf_hacks/avatar_interface.php?get_avatar=true'
	.'&s='.$_SESSION['WCF_SID']);
$avatar_size = $avatar_data[1];
if ($avatar_size->w && $avatar_size->h)
	$avatar_url = '../community/wcf/images/'.$avatar_data[0];
else
	$avatar_url = 'ui/icons/labels/avatars/150/default.png';
include('templates/game_settings/game_settings.tmpl.inc');
?>