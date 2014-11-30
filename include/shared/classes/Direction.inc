<?php
/**
 * Repräsentiert ein Enum, welches mögliche Richtungen links, rechts, oben, unten
 * und Kombinationen unterstützt (Flags).
 */

class Direction {	// ENUM-Klasse
	const MIN = 0;

	const NONE = 0;		// keine Richtung
	const LEFT = 1;
	const RIGHT = 2;
	const UP = 4;
	const DOWN = 8;
	const ALL = 15;

	const TELEPORT = 16;	// keine spezifizierbare Richtung, da Teleport

	const MAX = 31;

	private function __construct(){}	// Konstruktor nicht möglich => Enum-Klasse
}
?>