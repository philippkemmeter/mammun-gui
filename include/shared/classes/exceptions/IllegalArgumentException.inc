<?php


class IllegalArgumentException extends Exception {

	/**
	 * Konstrutor :)
	 * Der geerbte Standardkontruktor __construct($message, $code) ist darin
	 * implizit enthalten.
	 *
	 * @param string $varname
	 * @param string $expected_range
	 * @param mixed $given_value
	 */
	public function __construct($varname, $expected_range=false,
		$given_value=false)
	{
		if ($given_value === false) {
			/**
			 * Überladener Konstruktor ($message, $code) wird hier benutzt.
			 */
			parent::__construct($varname, (int)$expected_range);
			return;
		}
		/**
		 * Eigentlicher IllegalArgumentException-Konstruktor
		 */
		$expected_range = str_replace(
			array(
				'N1', 'N\{0}', 'N0', 'N', 'Z',
				'R', 'R+', 'R-', 'R0+', 'R0-',
				'INF'
			),
			array(
				'ℕ₁', 'ℕ₁','ℕ₀', 'ℕ', 'ℤ',
				'ℝ', 'ℝ⁺\{0}', 'ℝ⁻\{0}', 'ℝ₀⁺', 'ℝ₀⁻',
				'∞'
			),
			$expected_range
		);
		$message = "{$this->getFile()}:{$this->getLine()}: ".
			"\$$varname=='$given_value' ∉ $expected_range!";
		parent::__construct($message);
	}
}


?>