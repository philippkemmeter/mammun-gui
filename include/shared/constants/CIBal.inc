<?php
/**
 * Konstanten-Interface. Hier sind alle Balancing-Konstanten drin.
 *
 * Um am Balancing etwas zu ändern, muss also nur hier gedreht werden.
 *
 * @author Philipp Kemmeter
 */
interface CIBal {
	/**
	 * Anzahl Sekunden, die ein Tribe von einem Sektor in den benachbarten
	 * braucht bei Speed 100.
	 * @var int
	 */
	const MOVEMENT_TIME = 560000;

	/**
	 * Sekunden die ein Tribe mit Intelligenz 100 braucht, um einen Landfleck
	 * zu erkunden.
	 * @var int
	 */
	const EXPLORATION_TIME = 0;

	/**
	 * Wie viele Tribes maximal gleichzeitig gemorpht werden dürfen.
	 */
	const TRIBE_MORPH_SIMUL_MAX = 3;
}
?>
