<?php
@session_start();
if (!empty($_REQUEST['map_key'])) {
	$_SESSION['map_key'] = $_REQUEST['map_key'];
}
if (!empty($_REQUEST['visit'])) {
	$_SESSION['visit'] = $_REQUEST['visit'];
}
// Registration Logic Here...
require_once(dirname(__FILE__)."/../../include/shared/classes/server_com/ClientServer.inc");
require_once(dirname(__FILE__)."/../../include/_config.inc");
require_once(dirname(__FILE__)."/../../include/classes/MammunFB2Wrapper.inc");
require_once(dirname(__FILE__)."/../../include/classes/MammunFBWrapper.inc");
require_once(dirname(__FILE__)."/../../include/classes/Signup.inc");

if (isset($_REQUEST['fb_sig']) || isset($_REQUEST['fb_sig_post'])) {
	$facebook = MammunFBWrapper::getFacebook();
	$user_id = MammunFBWrapper::getUser();
	$session = null;
}
else {
	$facebook = MammunFB2Wrapper::getFacebook();
	$user_id = MammunFB2Wrapper::getUser();
	$session = $facebook->getSession();
}
/**
 * The Facebook API call requests all needed user data so that it can later be
 * filled in to our own forms and sent to the server for registration
 */

try
{
	if(isset($facebook))
	{
		$expected_data = array (
			'nick' 			=> 	'string',
			'pass' 			=> 	'string',
			'old_account'	=>	'int',
			'terms_accepted'=>	'int'
		);
		//$GP = $cryptObj->load_GET_POST($expected_data);


		//echo json_encode($_REQUEST);
		if(isset($_REQUEST['nick']))
		{
			if (isset($_REQUEST['fb_sig']) || isset($_REQUEST['fb_sig_post'])) {
				$user_details = $facebook->api_client->users_getInfo($user_id,
					'email, first_name, last_name');
				$user_first_name = $user_details[0]['first_name'];
				$user_last_name = $user_details[0]['last_name'];
				$user_email = $user_details[0]['email'];
			}
			else {
				$user_details = $facebook->api('me/?fields=email,first_name,last_name');
				$user_first_name = $user_details['first_name'];
				$user_last_name = $user_details['last_name'];
				$user_email = $user_details['email'];
			}

			$response = new ClientServerResponse();
			$nick = trim($_REQUEST['nick']);
			$pass = trim($_REQUEST['pass']);

			try
			{
				/**
				 * Two seperate types of login are required. The first is if the user
				 * already has a mammun account and wants to link his old account with
				 * his facebook. The second is a completely new registration which
				 * simultaneously creates a new account and links his facebook id to it.
				 */
				if(@$_REQUEST['old_account'])
				{
					Signup::connect_account_with_fb($user_id, $nick, $pass);
				}
				else
				{
					if (strlen($_SESSION['map_key']) > 6) {
						$tmp = $_SESSION['map_key'];
						$_SESSION['map_key'] = substr($tmp, 0, 6);
						$recruit = substr($tmp, 6);
					}
					$pass = Signup::register($nick, $user_email,
						$user_first_name, $user_last_name, $recruit, false,
						$pass,'', $user_id, $_SESSION['map_key'],
						'de', $_SESSION['visit']);
				}
				$response->add_value(1);
			}
			catch(Exception $e)
			{
				$old_account = $_REQUEST['old_account'];
				$response->add_value(0);
				$response->add_value($e->getMessage());
				$response->add_value($e->getCode());
				$response->add_value($old_account);
			}
			die($response);
		}
	}
	else
	{
		throw new Exception("\$facebook not set.");
	}
}
catch (Exception $e)
{
	unset($_SESSION);
//	header("Location: ".CONF_FACEBOOK_APP_URL); // This is a very bad idea. It causues the recursive frame scenario...
//	header("Location: http://www.mammun.com/");

	die("Facebook authentication failed! Please reload the application.");
}




?>
