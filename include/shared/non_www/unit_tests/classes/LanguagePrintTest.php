<?php
require_once(dirname(__FILE__).'/../extensions/PHPUnitSharedTestCase.inc');
require_once(CONF_SHARED_ROOT.'/classes/LanguagePrint.inc');

class LanguagePrintTest extends PHPUnitSharedTestCase {
	public function provider_list() {
		return array(
			array(array('Hund', 'Katze', 'Maus'), 'de','Hund, Katze und Maus'),
			array(array('Hund', 'Maus'), 'en', 'Hund and Maus'),
			array(array('La rue'), 'fr', 'La rue'),
			array(array('Hund', 'Maus'), 'fr', 'Hund et Maus'),
			array(array(), 'de', '')
		);
	}

	/**
	 * @dataProvider provider_list
	 */
	public function test_listing($values, $lng, $expected_result) {
		Language::get_instance()->set_language($lng);
		$this->assertEquals(
			$expected_result,
			LanguagePrint::listing($values)
		);
	}
}
?>
