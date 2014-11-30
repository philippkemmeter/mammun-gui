<?php
require_once(dirname(__FILE__).'/../../include/shared/classes/facebook/facebook.php');
require_once(dirname(__FILE__).'/../../include/_config.inc');
/**
 * MammunFBWrapper packs the needed Facebook API functionality into a singleton-
 * type of class. This allows us to declare the API keys only once and have only
 * one intance in the code where we actually create a new Facebook object.
 * @author alex
 *
 */
class MammunFBWrapper
{
	protected static $facebook_instance;
	protected static $user_instance;
	/*
	public static $facebook_keys = Array(
					'fb_sig_in_iframe', 
					'fb_sig_iframe_key',
					'fb_sig_locale',
					'fb_sig_in_new_facebook',
					'fb_sig_time',
					'fb_sig_added',
					'fb_sig_profile_update_time',
					'fb_sig_expires',
					'fb_sig_user',
					'fb_sig_session_key',
					'fb_sig_ss',
					'fb_sig_cookie_sig',
					'fb_sig_ext_perms',
					'fb_sig_api_key',
					'fb_sig_app_id',
					'fb_sig',
					CONF_FACEBOOK_API_KEY.'_expires',
					CONF_FACEBOOK_API_KEY.'_session_key',
					CONF_FACEBOOK_API_KEY.'_ss',
					CONF_FACEBOOK_API_KEY.'_user',
					CONF_FACEBOOK_API_KEY,
					'fbsetting_'.CONF_FACEBOOK_API_KEY);*/
					
	private static $api_key = CONF_FACEBOOK_API_KEY;
	private static $api_secret = CONF_FACEBOOK_API_SECRET;

	public static function getFacebook()
	{
		if(self::$facebook_instance == NULL)
		{
			self::$facebook_instance = new Facebook(self::$api_key, self::$api_secret);
		}
		return self::$facebook_instance;
	}

	public static function getUser()
	{
		if(self::$user_instance == NULL)
		{
			self::$user_instance = self::getFacebook()->require_login(
				$required_permissions = 'email');
		}
		return self::$user_instance;
	}
}
?>
