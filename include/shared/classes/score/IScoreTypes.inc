<?php
/**
 * Hauptsächlich geht's hierbei um gemeinsame Konstanten.
 * @author Philipp Kemmeter
 */
interface IScoreTypes {
	const TYPE_UNKNOWN = 0;
	const TYPE_RESSOURCE_GAIN = 1;
	const TYPE_RESSOURCE_USE = 2;
	const TYPE_SCIENCE_RESULT = 3;
	const TYPE_MEDAL = 4;

	const TYPE_MAX = 3;
}
?>