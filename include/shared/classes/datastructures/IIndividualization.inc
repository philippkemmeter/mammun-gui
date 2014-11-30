<?php
require_once(dirname(__FILE__).'/../helper/IConvertablePHPStdClass.inc');
require_once(dirname(__FILE__).'/../helper/IConvertablePHPArray.inc');

interface IIndividualization extends IConvertablePHPStdClass,
	IConvertablePHPArray
{
	const TYPE_CLOTHES_MIN = 1;
	const TYPE_OUTERWEAR = 1;
	const TYPE_TROUSERS = 2;
	const TYPE_SHOES = 3;
	const TYPE_JEWELRY = 4;
	const TYPE_HAIR_HAT = 5;
	const TYPE_WEAPON = 6;
	const TYPE_CLOTHES_MAX = 6;

	const TYPE_ITEMS_MIN = 10;
	const TYPE_BUILDING = 10;
	const TYPE_ITEMS_MAX = 10;
}
?>