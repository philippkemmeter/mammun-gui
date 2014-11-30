<?php
require_once (dirname(__FILE__).'/helper/IConvertablePHPAssocArray.inc');
require_once (dirname(__FILE__).'/helper/IConvertablePHPStdClass.inc');

/**
 * @author Philipp Kemmeter
 */
interface IMapPlayers
	extends IConvertablePHPStdClass, IConvertablePHPAssocArray
{
	const STATE_STILL_ALIVE = 1;
	const STATE_KILLED = 2;
	const STATE_VICTOR = 3;
	const STATE_WAS_NEVER_ON_MAP = 4;

	const PLAYER_COLOR0 = 'black';
	const PLAYER_COLOR1 = 'blue';
	const PLAYER_COLOR2 = 'green';
	const PLAYER_COLOR3 = 'orange';
	const PLAYER_COLOR4 = 'pink';
	const PLAYER_COLOR5 = 'purple';
	const PLAYER_COLOR6 = 'red';
	const PLAYER_COLOR7 = 'white';
	const PLAYER_COLOR8 = 'yellow';
	const PLAYER_COLOR_MAX = 8;
}
?>