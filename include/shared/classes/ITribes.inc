<?php
/**
 * Repräsentiert Schnittstelle eines einzelnen Tribe im Hauptgebäude.
 */
interface ITribes__TribeInMF
{
	const ERR_NOT_ENOUGH_BIO = 1;
	const ERR_UNIT_LIMIT_REACHED = 2;
	const ERR_NOT_ENOUGH_KLUNKER = 3;
	const ERR_UNMORPH_SCIENTISTS_ONLY = 4;
	const ERR_MOPPH_TOO_MANY_SIMUL = 5;
}

/**
 * Repräsentiert Schnittstelle der Tribes-Verwaltungsklasse
 */
interface  ITribes
{
	const E_SAME_NAME_TWICE = 1;
	const E_SAME_TRIBE_CONFIG_TWICE = 2;
	const E_NAME_EMPTY = 3;
}
?>