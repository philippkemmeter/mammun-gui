<?php
/**
 * File created by Philipp Kemmeter on 17.03.2009 at 11:21:52
 */
 
/**
 *
 * @author Philipp Kemmeter
 */
class HackattackException extends Exception {
	public function __construct($message, $code) {
		parent::__construct($message, $code);
	}
}
?>