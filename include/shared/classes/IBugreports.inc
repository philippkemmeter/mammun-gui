<?php
interface IBugreports {
	const STATE_UNKNOWN = 0;
	const STATE_OPEN = 1;
	const STATE_FIXING = 2;
	const STATE_FIXED = 3;
	const STATE_TRASH = 4;
	
	const PRIORITY_UNKNOWN = 0;
	const PRIORITY_LOW = 1;
	const PRIORITY_NORMAL = 3;
	const PRIORITY_HIGH = 5;
	
	const CAT_UNKNOWN = 0;
	const CAT_CORE = 1;
	const CAT_COMMUNITY = 2;
	const CAT_DESIGN = 3;
	const CAT_LANGUAGE = 4;
	const CAT_WISHES = 5;
	const CAT_GUI_JS = 6;
}
?>