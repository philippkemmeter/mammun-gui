<?php
/**
 * Wird von jedem sprachspezifischen Test eingebunden
 */
require_once(dirname(__FILE__).'/../../extensions/PHPUnitMammunTestCase.inc');
require_once(CONF_GAME_ROOT . '/include/shared/classes/Language.inc');
if (!isset($language_to_test)) {
	trigger_error("Keine Sprache zum Testen angegeben!", E_USER_ERROR);
}
Language::get_instance()->set_language($language_to_test);
require_once(CONF_GAME_ROOT . '/include/classes/EventMessages.inc');

class EventMessages_LNG_Test extends PHPUnitMammunTestCase {

	public static function tearDownAfterClass() {
		self::install_db_dump("pvgame__m_event__clear");
	}

	public function provider_convert_event_messages() {
		self::install_db_dump("pvgame__m_event__all_events_for_philipp3760");
		self::login_user("Philipp");
		/**
		 * Dump einlesen, wo Philipp alle nur möglichen Event-Messages bekommen
		 * hat, damit man später alle parsen lassen kann und somit mit einem
		 * Test alle testet.
		 */
		$messages = EventMessages::get_my_log();
		$result = array();
		for ($i = 0, $n = count($messages); $i < $n; $i++) {
			$result[] = array ($messages[$i]);
		}

		return $result;
	}

	private function assert_lng_string($str, $type) {
		$this->assertFalse(
			(strpos($str, 'LNG') !== false),
			"LNG-Konstante nicht aufgelöst: $str (Typ: $type)"
		);
		$this->assertFalse(
			((strpos($str, '[') !== false)
				|| (strpos($str, ']') !== false)),
			"[]-Variable nicht aufgelöst: $str (Typ: $type)"
		);
	}

	/**
	 * @dataProvider provider_convert_event_messages
	 */
	public function test_convert_event_messages($message) {
		$type = $message->type;
		$this->assert_lng_string($message->str, $type);

		if (isset($message->publish_str) || isset($message->publish_title)
			|| isset($message->publish_action))
		{
			$this->assertFalse(
				empty($message->publish_str),
				"\$message->publish_str ist leer, aber publish_xxx ist nicht ".
					"leer. (Typ: $type)"
			);
			$this->assertFalse(
				empty($message->publish_title),
				"\$message->publish_title ist leer, aber publish_xxx ist nicht ".
					"leer. (Typ: $type)"
			);
			$this->assertFalse(
				empty($message->publish_action),
				"\$message->publish_action ist leer, aber publish_xxx ist nicht ".
					"leer. (Typ: $type)"
			);
			$this->assert_lng_string($message->publish_str, $type);
			$this->assert_lng_string($message->publish_title, $type);
			$this->assert_lng_string($message->publish_action, $type);
		}
	}
}
?>