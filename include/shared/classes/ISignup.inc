<?php
/**
 * File created by Philipp Kemmeter on 05.02.2009 at 17:24:18
 */

/**
 *
 * @author Philipp Kemmeter
 */
interface ISignup {
	const ERR_NICK_ALREADY_REGISTERED = 1;
	const ERR_NICK_CONTAINS_INVALID_CHARS = 2;
	const ERR_NO_NICK = 3;
	const ERR_NICK_TOO_LONG = 4;
	const ERR_NICK_TOO_SHORT = 5;
	const ERR_EMAIL_ADRESS_INVALID = 6;
	const ERR_NO_EMAIL = 7;
	const ERR_EMAIL_ALREADY_REGISTERED = 8;
	const ERR_NO_FIRSTNAME = 9;
	const ERR_NO_SURNAME = 10;
	const ERR_RECRUIT_USER_DOES_NOT_EXIST = 11;
	const ERR_SERVER_FULL = 12;
	const ERR_NO_PASS = 13;
	const ERR_FB_ID_ALREADY_REGISTERED = 14;
	const ERR_VALIDATION_KEY_INVALID = 20;
	const ERR_USER_ALREADY_ACTIVATED = 21;
}
?>