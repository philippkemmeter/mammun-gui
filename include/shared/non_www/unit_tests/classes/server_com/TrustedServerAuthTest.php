<?php
require_once(dirname(__FILE__).'/../../extensions/PHPUnitSharedTestCase.inc');
require_once(CONF_SHARED_ROOT.'/classes/server_com/TrustedServerAuth.inc');

/**
 * Testet die Funktionen von TrustedServerAuth
 *
 * @author Philipp Kemmeter
 */
class TrustedServerAuthTest extends PHPUnitSharedTestCase {
	private static $secret;
	private static $post;

	/**
	 * Initialisiert die Fixtures
	 */
	public static function setUpBeforeClass() {
		self::$secret = md5('lala 12837 ,sdmus');

		$o = new stdClass();
		$o->o = new stdClass();
		$o->o->m = 'mua';
		self::$post = array(
			'Muh'	=>	'mäh',
			'miau'	=>	1,
			'param'	=>	serialize(array('m', 's', 't')),
			'param2'=>	serialize($o),
		);
	}
	/**
	 * Kodiert eine Beispielanfrage und enkodiert sie wieder.
	 *
	 * Testet, ob das Ergebnis entsprechend gleich der Eingabe ist :)
	 */
	public function test_encode_decode() {
		$this->assertTrue(
			TrustedServerAuth::verify(
				TrustedServerAuth::create_trusted_request_array(
					self::$post, self::$secret
				),
				self::$secret
			)
		);
	}

	/**
	 * Testet, ob der timeout funktioniert.
	 */
	public function test_timeout() {
		$encode = TrustedServerAuth::create_trusted_request_array(
			self::$post, self::$secret
		);
		sleep(2);

		$this->assertFalse(
			TrustedServerAuth::verify(
				$encode,
				self::$secret,
				1
			)
		);
	}
}