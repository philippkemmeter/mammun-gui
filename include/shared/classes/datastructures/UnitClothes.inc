<?php
require_once(dirname(__FILE__).'/IIndividualization.inc');

/**
 * Ein Struct, welches die Kleidung von Einheiten beinhaltet.
 */

class UnitClothes implements IIndividualization
{
	const OUTERWEAR_NONE = 0;
	const OUTERWEAR_FUR1_NATURAL = 1;
	const OUTERWEAR_FUR1_RED = 2;
	const OUTERWEAR_FUR1_BLUE = 3;
	const OUTERWEAR_FUR1_YELLOW = 4;
	const OUTERWEAR_FUR1_GREEN = 5;
	const OUTERWEAR_FUR1_WHITE = 6;
	const OUTERWEAR_FUR1_BLACK = 7;
	const OUTERWEAR_FUR1_PURPLE = 8;
	const OUTERWEAR_FUR1_ORANGE = 9;
	const OUTERWEAR_FUR1_PINK = 10;

	const OUTERWEAR_FUR2_NATURAL = 101;
	const OUTERWEAR_FUR2_RED = 102;
	const OUTERWEAR_FUR2_BLUE = 103;
	const OUTERWEAR_FUR2_YELLOW = 104;
	const OUTERWEAR_FUR2_GREEN = 105;
	const OUTERWEAR_FUR2_WHITE = 106;
	const OUTERWEAR_FUR2_BLACK = 107;
	const OUTERWEAR_FUR2_PURPLE = 108;
	const OUTERWEAR_FUR2_ORANGE = 109;
	const OUTERWEAR_FUR2_PINK = 110;

	const OUTERWEAR_FUR3_NATURAL = 201;
	const OUTERWEAR_FUR3_RED = 202;
	const OUTERWEAR_FUR3_BLUE = 203;
	const OUTERWEAR_FUR3_YELLOW = 204;
	const OUTERWEAR_FUR3_GREEN = 205;
	const OUTERWEAR_FUR3_WHITE = 206;
	const OUTERWEAR_FUR3_BLACK = 207;
	const OUTERWEAR_FUR3_PURPLE = 208;
	const OUTERWEAR_FUR3_ORANGE = 209;
	const OUTERWEAR_FUR3_PINK = 210;

	const OUTERWEAR_BRA1_NATURAL = 301;
	const OUTERWEAR_BRA1_RED = 302;
	const OUTERWEAR_BRA1_BLUE = 303;
	const OUTERWEAR_BRA1_YELLOW = 304;
	const OUTERWEAR_BRA1_GREEN = 305;
	const OUTERWEAR_BRA1_WHITE = 306;
	const OUTERWEAR_BRA1_BLACK = 307;
	const OUTERWEAR_BRA1_PURPLE = 308;
	const OUTERWEAR_BRA1_ORANGE = 309;
	const OUTERWEAR_BRA1_PINK = 310;


	const OUTERWEAR_MAX = 320;

	const TROUSERS_NONE = 0;
	const TROUSERS_FUR1_NATURAL = 1;
	const TROUSERS_FUR1_RED = 2;
	const TROUSERS_FUR1_BLUE = 3;
	const TROUSERS_FUR1_YELLOW = 4;
	const TROUSERS_FUR1_GREEN = 5;
	const TROUSERS_FUR1_WHITE = 6;
	const TROUSERS_FUR1_BLACK = 7;
	const TROUSERS_FUR1_PURPLE = 8;
	const TROUSERS_FUR1_ORANGE = 9;
	const TROUSERS_FUR1_PINK = 10;

	const TROUSERS_FUR2_NATURAL = 101;
	const TROUSERS_FUR2_RED = 102;
	const TROUSERS_FUR2_BLUE = 103;
	const TROUSERS_FUR2_YELLOW = 104;
	const TROUSERS_FUR2_GREEN = 105;
	const TROUSERS_FUR2_WHITE = 106;
	const TROUSERS_FUR2_BLACK = 107;
	const TROUSERS_FUR2_PURPLE = 108;
	const TROUSERS_FUR2_ORANGE = 109;
	const TROUSERS_FUR2_PINK = 110;

	const TROUSERS_SKIRT1_NATURAL = 201;
	const TROUSERS_SKIRT1_RED = 202;
	const TROUSERS_SKIRT1_BLUE = 203;
	const TROUSERS_SKIRT1_YELLOW = 204;
	const TROUSERS_SKIRT1_GREEN = 205;
	const TROUSERS_SKIRT1_WHITE = 206;
	const TROUSERS_SKIRT1_BLACK = 207;
	const TROUSERS_SKIRT1_PURPLE = 208;
	const TROUSERS_SKIRT1_ORANGE = 209;
	const TROUSERS_SKIRT1_PINK = 210;

	const TROUSERS_MAX = 220;

	const SHOES_NONE = 0;
	const SHOES_MAX = 0;

	const JEWELRY_NONE = 0;
	const JEWELRY_MAX = 0;

	const HAIR_HAT_NORMAL = 0;
	const HAIR_HAT_HELMET1_NATURAL = 1;
	const HAIR_HAT_HELMET1_RED = 2;
	const HAIR_HAT_HELMET1_BLUE = 3;
	const HAIR_HAT_HELMET1_YELLOW = 4;
	const HAIR_HAT_HELMET1_GREEN = 5;
	const HAIR_HAT_HELMET1_WHITE = 6;
	const HAIR_HAT_HELMET1_BLACK = 7;
	const HAIR_HAT_HELMET1_PURPLE = 8;
	const HAIR_HAT_HELMET1_ORANGE = 9;
	const HAIR_HAT_HELMET1_PINK = 10;

	const HAIR_HAT_AFRO_NATURAL = 101;
	const HAIR_HAT_AFRO_RED = 102;
	const HAIR_HAT_AFRO_BLUE = 103;
	const HAIR_HAT_AFRO_YELLOW = 104;
	const HAIR_HAT_AFRO_GREEN = 105;
	const HAIR_HAT_AFRO_WHITE = 106;
	const HAIR_HAT_AFRO_BLACK = 107;
	const HAIR_HAT_AFRO_PURPLE = 108;
	const HAIR_HAT_AFRO_ORANGE = 109;
	const HAIR_HAT_AFRO_PINK = 110;

	const HAIR_HAT_BONNET_NATURAL = 201;
	const HAIR_HAT_BONNET_RED = 202;
	const HAIR_HAT_BONNET_BLUE = 203;
	const HAIR_HAT_BONNET_YELLOW = 204;
	const HAIR_HAT_BONNET_GREEN = 205;
	const HAIR_HAT_BONNET_WHITE = 206;
	const HAIR_HAT_BONNET_BLACK = 207;
	const HAIR_HAT_BONNET_PURPLE = 208;
	const HAIR_HAT_BONNET_ORANGE = 209;
	const HAIR_HAT_BONNET_PINK = 210;

	const HAIR_HAT_LONG_HAIR_NATURAL = 301;
	const HAIR_HAT_LONG_HAIR_RED = 302;
	const HAIR_HAT_LONG_HAIR_BLUE = 303;
	const HAIR_HAT_LONG_HAIR_YELLOW = 304;
	const HAIR_HAT_LONG_HAIR_GREEN = 305;
	const HAIR_HAT_LONG_HAIR_WHITE = 306;
	const HAIR_HAT_LONG_HAIR_BLACK = 307;
	const HAIR_HAT_LONG_HAIR_PURPLE = 308;
	const HAIR_HAT_LONG_HAIR_ORANGE = 309;
	const HAIR_HAT_LONG_HAIR_PINK = 310;

	const HAIR_HAT_CHRISTMAS_HAT_RED = 402;

	const HAIR_HAT_MAX = 320;

	const WEAPON_NORMAL_NATURAL = 0;
	const WEAPON_NORMAL_RED = 2;
	const WEAPON_NORMAL_BLUE = 3;
	const WEAPON_NORMAL_YELLOW = 4;
	const WEAPON_NORMAL_GREEN = 5;
	const WEAPON_NORMAL_WHITE = 6;
	const WEAPON_NORMAL_BLACK = 7;
	const WEAPON_NORMAL_PURPLE = 8;
	const WEAPON_NORMAL_ORANGE = 9;
	const WEAPON_NORMAL_PINK = 10;

	const WEAPON_MAX = 20;

	public $outerwear;
	public $trousers;
	public $shoes;
	public $jewelry;
	public $hairhat;
	public $weapon;

	public static $cloth_type_strings = array(
		'outerwear',
		'trousers',
		'shoes',
		'jewelry',
		'hairhat',
		'weapon'
	);

	/**
	 * Erzeugt eine neue Kleidungskombination.
	 * @param uint $outerwear
	 * @param uint $trousers
	 * @param uint $shoes
	 * @param uint $jewelry
	 * @param uint $hairhat
	 * @param uint $weapon
	 * @throws IllegalArgumentException
	 */
	public function __construct($outerwear, $trousers=0, $shoes=0, $jewelry=0,
		$hairhat=0, $weapon=0)
	{
		if (is_object($outerwear)) {
			$data_o = $outerwear;
			$outerwear = (int)$data_o->outerwear;
			$trousers = (int)$data_o->trousers;
			$shoes = (int)$data_o->shoes;
			$jewelry = (int)$data_o->jewelry;
			$hairhat = (int)$data_o->hairhat;
			$weapon = (int)$data_o->weapon;
		}
		else {
			$outerwear = (int)$outerwear;
			$trousers = (int)$trousers;
			$shoes = (int)$shoes;
			$jewelry = (int)$jewelry;
			$hairhat = (int)$hairhat;
			$weapon = (int)$weapon;
		}
		if (($outerwear < 0) || ($outerwear > self::OUTERWEAR_MAX))
			throw new IllegalArgumentException("outerwear",
				"[0..".self::OUTERWEAR_MAX."]", $outerwear);
		if (($trousers < 0) || ($trousers > self::TROUSERS_MAX))
			throw new IllegalArgumentException("trousers",
				"[0..".self::TROUSERS_MAX."]", $trousers);
		if (($shoes < 0) || ($shoes > self::SHOES_MAX))
			throw new IllegalArgumentException("shoes",
				"[0..".self::SHOES_MAX."]", $shoes);
		if (($jewelry < 0) || ($jewelry > self::JEWELRY_MAX))
			throw new IllegalArgumentException("jewelry",
				"[0..".self::JEWELRY_MAX."]", $jewelry);
		if (($hairhat < 0) || ($hairhat > self::HAIR_HAT_MAX))
			throw new IllegalArgumentException("hairhat",
				"[0..".self::HAIR_HAT_MAX."]", $hairhat);
		if (($weapon < 0) || ($weapon > self::WEAPON_MAX))
			throw new IllegalArgumentException("weapon",
				"[0..".self::WEAPON_MAX."]", $weapon);

		$this->outerwear = $outerwear;
		$this->trousers = $trousers;
		$this->shoes = $shoes;
		$this->jewelry = $jewelry;
		$this->hairhat = $hairhat;
		$this->weapon = $weapon;
	}

	/**
	 * Konvertiert eine stdClasse zurück zu einem UnitClothes Objekt.
	 *
	 * @param stdClass $data
	 * @return UnitClothes
	 */
	public static function convert_std_class_to_unit_clothes(stdClass $data) {
		return new self($data->outerwear, $data->trousers, $data->shoes,
			$data->jewelry, $data->hairhat, $data->weapon);
	}

	/**
	 * Konvertiert das Objekt in ein Standardobjekt
	 * @see include/shared/classes/helper/IConvertablePHPStdClass#to_php_std_class()
	 */
	public function to_php_std_class() {
		$x = new stdClass();
		$x->outerwear = $this->outerwear;
		$x->trousers = $this->trousers;
		$x->shoes = $this->shoes;
		$x->jewelry = $this->jewelry;
		$x->hairhat = $this->hairhat;
		$x->weapon = $this->weapon;
		return $x;
	}

	/**
	 * Konvertiert das Objekt in ein php Array
	 * @see include/shared/classes/helper/IConvertablePHPArray#to_php_array()
	 */
	public function to_php_array() {
		$x = array();
		$x[self::TYPE_OUTERWEAR] = $this->outerwear;
		$x[self::TYPE_TROUSERS] = $this->trousers;
		$x[self::TYPE_SHOES] = $this->shoes;
		$x[self::TYPE_JEWELRY] = $this->jewelry;
		$x[self::TYPE_HAIR_HAT] = $this->hairhat;
		$x[self::TYPE_WEAPON] = $this->weapon;
		return $x;
	}
}
?>