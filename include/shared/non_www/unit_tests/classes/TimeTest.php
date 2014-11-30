<?php
require_once(dirname(__FILE__).'/../extensions/PHPUnitSharedTestCase.inc');
require_once(CONF_SHARED_ROOT.'/classes/Time.inc');

/**
 * Testet die Time-Klasse.
 *
 * @author Philipp Kemmeter
 */
class TimeTest extends PHPUnitSharedTestCase {
	/**
	 * Testet, ob der initiale Getter ohne vorheriges Set funktioniert.
	 */
	public function test_get_t_now() {
		$this->assertEquals(time(), Time::get_t_now());
		sleep(1);
		$this->assertEquals(time()-1, Time::get_t_now());
	}

	/**
	 * Testet, ob man einen Timestamp setzen kann.
	 */
	public function test_set_t_now() {
		Time::set_t_now(12345);
		$this->assertEquals(12345, Time::get_t_now());
		Time::set_t_now(125);
		$this->assertEquals(125, Time::get_t_now());
	}

	/**
	 * Testet, dass Werte kleine 0 nicht gesetzt werden können.
	 *
	 * @expectedException IllegalArgumentException
	 */
	public function test_set_t_now_never_smaller_zero() {
		Time::set_t_now(-1);
	}
}


?>