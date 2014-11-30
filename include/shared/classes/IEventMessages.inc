<?php
require_once(dirname(__FILE__).'/IEventLog.inc');

/**
 * Konstantendefinition für den EventMessages
 *
 * @author Philipp Kemmeter
 */

interface IEventMessages extends IEventLog { /** Nachrichten Typen synchen */
	const FLAG_NEW_MESSAGE = 1;
}