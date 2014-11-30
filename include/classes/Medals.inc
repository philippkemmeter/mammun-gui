<?php
require_once(dirname(__FILE__).'/../shared/classes/IMedals.inc');

class Medals implements IMedals {
	private function __construct() {}

	private static $medals;
	private static $user_medals;

	public static function get_medals($type=0, $subtype=0) {
		if ($type > 0) {
			if (!isset(self::$medals[$type])) {
				$tmp = Server::get_instance(
					CONF_ENGINE_SERVER_URL)->get_medals($type, 0);
				self::$medals[$type] = Converter::stdClassToArray($tmp->$type);
				foreach (self::$medals[$type] as $k => $v) {
					self::$medals[$type][$k] = Converter::stdClassToArray($v);
				}

			}
			if ($subtype > 0)
				return self::$medals[$type][$subtype];
			else
				return self::$medals[$type];
		}

		return Server::get_instance(CONF_ENGINE_SERVER_URL)->get_medals(0, 0);
	}

	public static function get_user_medals($uid, $type=0, $subtype=0) {
		if ($type > 0) {
			if (!isset(self::$user_medals[$uid][$type])) {
				$tmp = Server::get_instance(
					CONF_ENGINE_SERVER_URL)->get_user_medals($uid, $type, 0);
				self::$user_medals[$uid][$type]
					= Converter::stdClassToArray($tmp->$type);
				foreach (self::$user_medals[$uid][$type] as $k => $v) {
					self::$user_medals[$uid][$k]
						= Converter::stdClassToArray($v);
				}

			}
			if ($subtype > 0)
				return self::$user_medals[$uid][$type][$subtype];
			else
				return self::$user_medals[$uid][$type];
		}

		return Server::get_instance(CONF_ENGINE_SERVER_URL)->get_user_medals(
			$uid, 0, 0);
	}
}
?>