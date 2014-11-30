<?php
/**
 * Schnittstellendefinition für die User-Klasse.
 */
require_once (dirname(__FILE__).'/helper/IConvertablePHPAssocArray.inc');
require_once (dirname(__FILE__).'/helper/IConvertablePHPStdClass.inc');

interface IUser extends IConvertablePHPAssocArray, IConvertablePHPStdClass {
	const ERR_USER_DOES_NOT_EXIST = 1;
	const ERR_USER_IS_ALREADY_CONNECTED_TO_FB = 2;
	const ERR_USER_PASSWORD_AUTHENTIFICATION_FAILED = 3;

	const STATE_UNACTIVATED = 0;
	const STATE_NEW_PLAYER = 1;
	const STATE_CHOOSING_SCIENCES_LOST_LAST_MAP = 2;
	const STATE_CHOOSING_MAPS = 3;
	const STATE_WAITING_ON_MAP = 4;
	const STATE_PLAYING = 5;
	const STATE_CHOOSING_SCIENCES_WON_LAST_MAP = 6;
	const STATE_MAP_IS_STARTING = 7;
	const STATE_WAITING_FOR_CLANMAP = 8;
	const STATE_WAITING_ON_MAP_AND_READY = 9;

	const MODE_CLAN = 2;
	const MODE_FFA = 1;

	const TUT_FLAG_INTRO = 1;
	const TUT_FLAG_MOVE_UNIT = 2;
	const TUT_FLAG_DO_SCIENCE = 4;
	const TUT_FLAG_DO_TRAINING = 8;
	const TUT_FLAG_MOVE_UNIT2 = 16;
	const TUT_FLAG_FIST_STEPS_DONE = 32;
	const TUT_FLAGGROUP_FIRST_STEPS = 63;	// 1|2|4|8|16|32

	const TUT_FLAG_BEAM_UNITS = 128;

	const TUT_FLAG_DO_BUILDING = 512;
	const TUT_FLAG_MOVE_UNIT_AFTER_BUILD = 1024;
	const TUT_FLAG_BUILDINGS_DONE = 2048;
	const TUT_FLAG_MOVE_SECOND_UNIT_AFTER_BUILD = 4096;
	const TUT_FLAG_INVITE = 64;
	const TUT_FLAGGROUP_BUILD_EXPAND = 7744; // 512|1024|2048|4096|64

	const BANNED_DELETED_BY_HIMSELF = 1;
}
?>