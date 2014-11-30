<?php
/**
 * File created by Philipp Kemmeter on 17.03.2009 at 11:15:55
 */

/**
 *
 * @author Philipp Kemmeter
 */
interface IHackattackException {
	const CODE_ADMIN_FUNCTION = 1;
	const CODE_MAY_NOT_RESEARCH = 2;
	const CODE_SCIENTISTS_DROP = 3;
	const CODE_MERC_MORPH = 4;
	const CODE_MORPH_UNRESEARCHED = 5;
	const CODE_ILLEGAL_ABIL_FOR_RACE = 6;
	const CODE_DESTROY_BUILDING_OF_OTHER = 7;
	const CODE_LEAVING_MAP_WHILE_NOT_PLAYING = 8;
	const CODE_TRANSACTION_ID_WRONG = 9;
	const CODE_OVERPAINTIN_OTHERS_BUILDINGS = 10;
}
?>