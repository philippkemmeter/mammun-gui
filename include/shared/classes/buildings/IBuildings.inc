<?php
/**
 * File created by Philipp Kemmeter on 06.05.2009 at 13:46:17
 */

/**
 *
 * @author Philipp Kemmeter
 */
interface IBuildings {
	/* Building-Type-IDs */
	const TECHCENTER = 1;
	const TENT = 2;
	const DWELLING = 3;
	const ADV_DWELLING = 4;
	const HUNTING_LODGE = 5;
	const ADV_HUNTING_LODGE = 6;
	const FARM = 7;
	const LUMBERJACKS_HUT = 8;
	const ADV_LUMBERJACKS_HUT = 9;
	const SHED = 10;
	const MOTHER_FUNGUS = 100;
	const NEUTRAL_BUILDINGS_BEGIN = 200;
	const CHRISTMAS_TREE = 200;
	const NORMAL_TREE = 201;
	const NEUTRAL_BUILDINGS_END = 299;

	const CUSTOM_BUILDINGS_BEGIN = 300;
	/**
	 * Custom Building-Types sind auch gleich der Item-ID. Deswegen müssen
	 * hier die Farbfiltercodes beachtet werden.
	 * @var unknown_type
	 */
	const C_STONE_BIG1 = 301;
	const C_STONE_BIG2 = 401;
	const C_STONE_BIG3 = 501;
	const C_STONE_SMALL1 = 601;
	const C_STONE_SMALL2 = 701;
	const C_OAK_TREE1 = 801;
	const C_FIR_TREE1 = 901;
	const C_PALM_TREE1= 1001;
	const C_OBILISK1 = 1101;
	const C_PYRAMIDE1 = 1201;
	const C_LETTER_TYPE1 = 1301;
	const C_STELA1 = 1401;
	const C_STONEHENGE1 = 1501;
	const C_DOUBLE_STELA1 = 1601;
	const C_STONE_PILLAR1 = 1701;
	const C_PHAROS = 1801;
	const C_TRIUMPHAL_ARCH = 1901;
	const C_MAYA_PYRAMIDE1 = 2001;
	const C_PILE_OF_STONES = 2101;
	const C_SUNDIAL = 2201;
	const C_GEYSER = 2301;
	const C_WATERFALL = 2401;
	const C_EIFFEL_TOWER = 2501;
	const C_HALLOWEEN_PUMPKIN = 2601;
	const C_HALLOWEEN_SCARECROW = 2701;
	const C_HALLOWEEN_BONEYARD = 2801;
	const C_CHRISTMAS_SCARECROW = 2901;
	const C_CHRISTMAS_TREE = 3001;
	const C_CHRISTMAS_TREE_DEVASTED = 3101;
	const C_CHRISTMAS_PRESENTS = 3201;
	const C_CHRISTMAS_PRESENTS_BAG = 3301;
	const C_CHRISTMAS_SLIDE = 3401;
	const C_CHRISTMAS_SLIDE_DEVASTED = 3501;
	const C_CHRISTMAS_SNOWMAN = 3601;

	const CUSTOM_BUILDINGS_END = 65535;

	/**
	 * Colors
	 */
	const COLOR_MAIN_NATURAL = 1;
	const COLOR_MAIN_RED = 2;
	const COLOR_MAIN_BLUE = 3;
	const COLOR_MAIN_YELLOW = 4;
	const COLOR_MAIN_GREEN = 5;
	const COLOR_MAIN_WHITE = 6;
	const COLOR_MAIN_BLACK = 7;
	const COLOR_MAIN_PURPLE = 8;
	const COLOR_MAIN_ORANGE = 9;
	const COLOR_MAIN_PINK = 10;

	/* Get_free_place Algo-Arten */
	const GFP_ROW_BY_ROW = 1;
	const GFP_SPIRAL = 2;
	const GFP_SCATTERED = 3;

	const ERR_UNKNOWN = 0;
	const ERR_NO_DOMINATION = 1;
	const ERR_CANT_BUILD_THERE = 2;
	const ERR_BUILDING_NOT_FOUND = 3;
	const ERR_IS_UPGRADING = 4;
	const ERR_MAX_LEVEL_REACHED = 5;
	const ERR_REQ_NOT_MET = 6;
	const ERR_NOT_ENOUGH_RESOURCES = 7;
	const ERR_COULD_NOT_BUILD_ALL = 8;
	const ERR_TOO_MANY_BUILDINGS_IN_SECTOR = 9;
	const ERR_DONT_DESTROY_MAIN_BUILDINGS = 10;
	const ERR_NOT_ENOUGH_KLUNKER = 11;
	const ERR_CANNOT_CHANGE_COLOR = 12;

	const KKR_PAINT = 5;
}
?>