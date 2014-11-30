<?php
interface IMapChooser {
	const ERR_MAP_DOES_NOT_EXIST = 1;
	const ERR_MAP_ALREADY_FULL = 2;
	const ERR_USER_IS_OR_WAS_ON_MAP = 3;
	const ERR_NO_HS_FOUND = 4;
	const ERR_WORLD_NEEDS_HIGHER_LEVEL = 5;
	const ERR_HS_NOT_AVAILABLE = 6;
}