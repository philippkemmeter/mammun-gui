<?php
require_once(dirname(__FILE__).'/../extensions/PHPUnitSharedTestCase.inc');
require_once(CONF_SHARED_ROOT.'/classes/ValueChecker.inc');

class ValueCheckerTest extends PHPUnitSharedTestCase {
	public function provider_string_range() {
		return array (
			array("hallo", false, 0, 5, '',						true),
			array("hallo", true, 0, 3, '',						false),
			array("welt", true, -1, 100, '',	 				true),
			array("welt", true, 10, 100, '',					false),
			array("hallo hallo", true, false, false, '',	 	true),
			array("", false, false, false, '',					false),
			array("hallo hallo", false, false, false, ' ',		false),
			array("", true, false, false, '',					true),
			array("muha", true, 100, 2,	'',						false),
			array("muha", true, false, false, 'h',				false),
			array(23.3, true, false, false, '',					true),
			array(23.3, true, false, false, '.',				false),
		);
	}
	/**
	 * Testet ValueChecker::string
	 *
	 * @param string $value
	 * @param bool $empty
	 * @param int $min
	 * @param int $max
	 * @param string $cs	verbotene Zeichen
	 * @param bool $valid	ob die Eingabe korrekt sind, oder (FALSE) ob eine
	 * 						IllegalArgumentException geworfen werden soll
	 * @dataProvider provider_string_range
	 */
	public function test_string($value, $empty, $min, $max, $cs, $valid) {
		$varname = uniqid();
		try {
			$v = ValueChecker::string($value, $varname, $empty, $min, $max, $cs);
			if (!$valid) {
				$this->fail("An exception hasn't been thrown, but should.");
			}
			$this->assertEquals((string)$value, $v);
		}
		catch (IllegalArgumentException $e) {
			if ($valid) {
				$this->fail(
					"An exception has been thrown, but shouldn't: ".
						$e->getMessage()
				);
			}
		}
	}


	public function provider_float() {
		return array(
			array(1, false, false,								true),
			array("1.2", false, false,							true),
			array("hallo", false, false,						false),
			array(4, 4, 4,										true),
			array(4, 4, 10,										true),
			array(4, 2, 4,										true),
			array(4, -1, 4,										true),
			array(-4.12, -5, 4,									true),
			array(0, 10, 1,										false)
		);
	}
	/**
	 * Testet ValueChecker::float
	 *
	 * @param float $value
	 * @param int $min
	 * @param int $max
	 * @param bool $valid	ob die Eingabe korrekt sind, oder (FALSE) ob eine
	 * 						IllegalArgumentException geworfen werden soll
	 * @dataProvider provider_float
	 */
	public function test_float($value, $min, $max, $valid) {
		$varname = uniqid();
		try {
			$v = ValueChecker::float($value, $varname, $min, $max);
			if (!$valid) {
				$this->fail("An exception hasn't been thrown, but should.");
			}
			$this->assertEquals((float)$value, $v);
		}
		catch (IllegalArgumentException $e) {
			if ($valid) {
				$this->fail(
					"An exception has been thrown, but shouldn't: ".
						$e->getMessage()
				);
			}
		}
	}


	public function provider_int() {
		return array(
			array(1, false, false,								true),
			array("1.2", false, false,							false),
			array("3523", false, false,							true),
			array("hallo", false, false,						false),
			array(4, 4, 4,										true),
			array(4, 4, 10,										true),
			array(4, 2, 4,										true),
			array(4, -1, 4,										true),
			array(-4, -4, 4,									true),
			array(0, 10, 1,										false)
		);
	}
	/**
	 * Testet ValueChecker::int
	 *
	 * @param mixed $value
	 * @param int $min
	 * @param int $max
	 * @param bool $valid	ob die Eingabe korrekt sind, oder (FALSE) ob eine
	 * 						IllegalArgumentException geworfen werden soll
	 * @dataProvider provider_int
	 */
	public function test_int($value, $min, $max, $valid) {
		$varname = uniqid();
		try {
			$v = ValueChecker::int($value, $varname, $min, $max);
			if (!$valid) {
				$this->fail("An exception hasn't been thrown, but should.");
			}
			$this->assertEquals((int)$value, $v);
		}
		catch (IllegalArgumentException $e) {
			if ($valid) {
				$this->fail(
					"An exception has been thrown, but shouldn't: ".
						$e->getMessage()
				);
			}
		}
	}


	public function provider_id() {
		return array(
			array(1, false, 									true),
			array("1.2", false, 								false),
			array("3523", true,									true),
			array("hallo", false,								false),
			array(-10, false,									false),
			array(-10, true,									false),
			array(0, true,										true),
			array(0, false,										false)
		);
	}
	/**
	 * Testet ValueChecker::id
	 *
	 * @param mixed $value
	 * @param bool $zero_allowed
	 * @param bool $valid	ob die Eingabe korrekt sind, oder (FALSE) ob eine
	 * 						IllegalArgumentException geworfen werden soll
	 * @dataProvider provider_id
	 */
	public function test_id($value, $zero_allowed, $valid) {
		$varname = uniqid();
		try {
			$v = ValueChecker::id($value, $varname, $zero_allowed);
			if (!$valid) {
				$this->fail("An exception hasn't been thrown, but should.");
			}
			$this->assertEquals((int)$value, $v);
		}
		catch (IllegalArgumentException $e) {
			if ($valid) {
				$this->fail(
					"An exception has been thrown, but shouldn't: ".
						$e->getMessage()
				);
			}
		}
	}


	public function provider_values() {
		return array(
			array(1, array(1,2,3),								true),
			array("1.2", array(1, 1.2, 3),						true),
			array("hallo", array("hallo", "lala"),				true),
			array("hallo", array(),								false),
			array(-10, array(1),								false)
		);
	}

	/**
	 * Testet ValueChecker::values
	 *
	 * @param mixed $value
	 * @param array $values
	 * @param bool $valid	ob die Eingabe korrekt sind, oder (FALSE) ob eine
	 * 						IllegalArgumentException geworfen werden soll
	 * @dataProvider provider_values
	 */
	public function test_values($value, array $values, $valid) {
		$varname = uniqid();
		try {
			$v = ValueChecker::values($value, $varname, $values);
			if (!$valid) {
				$this->fail("An exception hasn't been thrown, but should.");
			}
			$this->assertEquals($value, $v);
		}
		catch (IllegalArgumentException $e) {
			if ($valid) {
				$this->fail("An exception has been thrown, but shouldn't.");
			}
		}
	}


	public function provider_bool() {
		return array(
			array(true, true),
			array(false, false),
			array(1, true),
			array(0, false),
			array(NULL, null),
			array("muh", null),
			array(2, null),
			array(0.1, null),
			array('', null),
			array('true', true),
			array('false', false)
		);
	}

	/**
	 * Testet ValueChecker::bool
	 *
	 * @param mixed $value
	 * @param bool $valid	ob die Eingabe korrekt sind, oder (FALSE) ob eine
	 * 						IllegalArgumentException geworfen werden soll
	 * @dataProvider provider_bool
	 */
	public function test_bool($value, $result) {
		$varname = uniqid();
		try {
			$v = ValueChecker::bool($value, $varname);
			if ($result === null) {
				$this->fail("An exception hasn't been thrown, but should.");
			}
			$this->assertEquals($result, $v);
		}
		catch (IllegalArgumentException $e) {
			if ($result !== null) {
				$this->fail("An exception has been thrown, but shouldn't.");
			}
		}
	}

	public function provider_email() {
		return array(
			array('a@b.d', true, 						'a@b.d'),
			array('a@b.d', false,						'a@b.d'),
			array('phil.kem@gmail.de', true, 			'phil.kem@gmail.de'),
			array('phil.kem@gmail.de', false, 			'phil.kem@gmail.de'),
			array('a', true, 							null),
			array('a', false, 							null),
			array('9278sada@asd', false, 				null),
			array('9278sada@asd', true,					null),
			array(02312, false, 						null),
			array(02312, true, 							null),
			array('827289@shdh.fr', false, 				'827289@shdh.fr'),
			array('827289@shdh.fr', true, 				'827289@shdh.fr'),
			array('@sdjdhasjkd.com', false,				null),
			array('@sdjdhasjkd.com', true, 				null),
			array('sdjdhasjkd.com',	false,				null),
			array('sdjdhasjkd.com',	true,				null),
			array('Phil Kem    <phil@gmail.com>  ', false,
												'Phil Kem <phil@gmail.com>'),
			array('Phil <phil@gmail.com>', true,		null),
		);
	}

	/**
	 * Testet ValueChecker::email
	 *
	 * @param mixed $value
	 * @param bool $valid	ob die Eingabe korrekt sind, oder (FALSE) ob eine
	 * 						IllegalArgumentException geworfen werden soll
	 * @dataProvider provider_email
	 */
	public function test_email($value, $strict, $result) {
		$varname = uniqid();
		try {
			$v = ValueChecker::email($value, $varname, $strict);
			if (!$result) {
				$this->fail("An exception hasn't been thrown, but should.");
			}
			$this->assertEquals($result, $v);
		}
		catch (IllegalArgumentException $e) {
			if ($result) {
				$this->fail("An exception has been thrown, but shouldn't.");
			}
		}
	}


	public function provider_mysqldate() {
		return array(
			array('2000-12-12', false, false, false,			'2000-12-12'),
			array('0000-00-00', false, false, false,			null),
			array('1900-01-42', false, false, false,			null),
			array('1983-02-15', true, '1983-02-14', '1983-02-16','1983-02-15'),
			array('1983-02-15', true, '1983-02-15', '1983-02-15','1983-02-15'),
			array('0000-00-00', true, '1200-10-22', false,      '0000-00-00'),
			array('1928-1-1', false, false, false,              null)
		);
	}

	/**
	 * Testet ValueChecker::mysqldate
	 *
	 * @param mixed $value					Der zu prüfende Wert
	 * @param bool $zero_date_allowed       Ob 0000-00-00 erlaubt ist.
	 * @param string $min                   Kleinstes erlaubtes Datum.
	 * @param string $max                   Größtes erlaubtes Datum.
	 * @param string $result                Erwartetes Ergebnis. Bei NULL wird
	 *                                      eine IllegalArgumentException
	 *                                      erwartet.
	 * @dataProvider provider_mysqldate
	 */
	public function test_mysqdate($value, $zero_date_allowed, $min, $max,
		$result)
	{
		$varname = uniqid();
		try {
			$v = ValueChecker::mysqldate($value, $varname, $zero_date_allowed,
				$min, $max, $result);
			if (!$result) {
				$this->fail("An exception hasn't been thrown, but should.");
			}
			$this->assertEquals($result, $v);
		}
		catch (IllegalArgumentException $e) {
			if ($result) {
				$this->fail("An exception has been thrown, but shouldn't.");
			}
		}

	}


	public function provider_url() {
		return array(
			array('http://www.mammun.com', true),
			array('https://abc.de/?asdf=asdasd&mdasu,1', true),
			array('Hallo', false),
			array('mammun.com', false),
			array('ftp://asdf.asdf.asdfgha.de/public/asdgb-as02isd38$E', false),
			array('ftp://asdf.asdf.asdfgha.de/public/asdgb-as02isd38#E', true),
			array('http://www.mammun.com.', false)
		);
	}

	/**
	 * Testet ValueChecker::url.
	 *
	 * @param string $value
	 * @param bool $shall_pass Ob der angegebene Wert korrekt sein soll.
	 * @dataProvider provider_url
	 */
	public function test_url($value, $shall_pass) {
		$varname = uniqid();
		try {
			$v = ValueChecker::url($value, $varname);
			if (!$shall_pass) {
				$this->fail("An exception hasn't been thrown, but should.");
			}
			$this->assertEquals($value, $v);
		}
		catch(IllegalArgumentException $e) {
			if ($shall_pass) {
				$this->fail("An exception has been thrown, but shouldn't.");
			}
		}
	}


	public function provider_colhex() {
		return array(
			array('#123456', true, true),
			array('8fa3d4', false, true),
			array('7da3221', false, false),
			array('a46d3u', false, false),
			array('hallos', false, false),
			array('#hallos', true, false),
			array('872f63', true, false)
		);
	}

	/**
	 * Testet ValueChecker::colhex
	 *
	 * @param string $value
	 * @param bool $leading_hash Ob ein # vorangestellt werden soll
	 * @param bool $shall_pass   Ob der angegebene Wert korrekt sein soll.
	 * @dataProvider provider_colhex
	 */
	public function text_colhex($value, $leading_hash, $shall_pass) {
		$varname = uniqid();

		try {
			$v = ValueChecker::colhex($value, $varname, $leading_hash);
			if (!$shall_pass) {
				$this->fail("An exception hasn't been thrown, but should.");
			}
			$this->assertEquals($value, $v);
		}
		catch (IllegalArgumentException $e) {
			if ($shall_pass) {
				$this->fail("An exception has been thrown, but shouldn't.");
			}
		}
	}

}
?>
