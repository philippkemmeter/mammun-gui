<?php
require_once(dirname(__FILE__).'/../../include/shared/classes/facebook2/facebook.php');
require_once(dirname(__FILE__).'/../../include/_config.inc');

class MammunFB2Wrapper
{
	protected static $facebook_instance;
	protected static $user_instance;
	
	public static function getFacebook()
	{
		if(self::$facebook_instance == NULL)
		{
			self::$facebook_instance = new Facebook2(array(
  				'appId' => CONF_FACEBOOK_APP_ID,
  				'secret' => CONF_FACEBOOK_API_SECRET,
 				'cookie' => false,
			));;
		}
		return self::$facebook_instance;
	}

	public static function getUser()
	{
		if(self::$user_instance == NULL)
		{
			try 
			{
  				self::$user_instance = self::getFacebook()->getUser();
			}
			catch (FacebookApiException $e) 
			{
    			error_log($e);
			}
		return self::$user_instance;
		}
	
	}
	
}
?>
