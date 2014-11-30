<?php
require_once(dirname(__FILE__).'/ScoreCache.inc');
require_once(dirname(__FILE__).'/../shared/classes/ITribes.inc');
require_once(dirname(__FILE__).'/../shared/classes/Tribe.inc');
require_once(dirname(__FILE__).'/../shared/classes/exceptions/IllegalArgumentException.inc');
require_once(dirname(__FILE__).'/../shared/classes/datastructures/UnitClothes.inc');
/**
 * Repräsentiert einen einzelnen Tribe im Hauptgebäude.
 *
 */
//ACHTUNG! Sollte in die Tribes-Klasse verschachtelt sein, aber nested Classes wird
//von PHP nicht mehr unterstützt ebenso wenig wir Namespaces!
//ACHTUNG: !!Darf NUR von Tribes-Klasse instanziiert werden!!
class Tribes__TribeInMF extends AbstractTribe implements ITribes__TribeInMF {
	public /*int*/ $amount;	// Anzahl Einheiten des Stammes noch im Hauptgebäude
						// ACHTUNG: Berücksichtigt NICHT die noch morphenden!!!
						// Diese Anzahl ist NICHT synchron und daher private
	private /*bool*/ $dropped_once;
	public /*int*/ $add_value;
	public /*int*/ $add_stamp;
	private /*bool*/ $autodrop;
	private /*int*/ $mercenary_id;	// Verweis auf Söldnereigeneschaften, wenn Söldner
	private /*int*/ $race_level;
	private /*int[5]*/ $ability_levels;
	private /*float*/ $morph_duration;

	//private, aber Tribes-Klasse ist Freund
	//Darf NUR von Tribes-Klase aufgerufen werden, der Konstruktor

	/**
	 * Konstruktor NUR VON Tribes-Klasse heraus aufrufen
	 * (nested-Classes oder Friends werden von PHP nicht unterstützt)
	 *
	 * @param array $db_data die DB-Zeile des Tribes
	 */
	public function __construct(array $db_data) {
		$this->init_from_db($db_data);
	}
	protected function init_from_db(array $db_data=null) {
		if (!$db_data)
			return;
		parent::init_from_db($db_data);
		$this->amount = $db_data['amount'];
		$this->dropped_once = $db_data['dropped_once'];
		$this->add_stamp = $db_data['add_stamp'];
		$this->add_value = $db_data['add_value'];
		$this->autodrop = $db_data['autodrop'];
		$this->mercenary_id = $db_data['mercenary_id'];
		$this->race_level = $db_data['race_level'];
		$this->ability_levels = $db_data['ability_levels'];
		$this->morph_duration = $db_data['morph_duration'];
	}

	/**
	 * Erzeugt neue Einheiten (so viele, wie angegeben) dieses Stammes.
	 *
	 * @param int $amount wieviele erzeugt werden sollen
	 * @param string $name	wie die Einheit auf der Karte heißen soll (wird
	 * 						bei Forschern ignoriert)
	 * @param UnitClothes $clothes	Welche Kleidung die Einheit tragen soll
	 * @return uint			Anzahl Klunker, die der User noch hat
	 */
	public function morph($amount, $name, UnitClothes $clothes) {
		$result = Server::get_instance(CONF_ENGINE_SERVER_URL)
			->cmd_tribe_morph($this->uid, $this->id, $amount, $name,
				$clothes->to_php_std_class());
		$this->init_from_db((array)$result->tribe_data);

		ScoreCache::set_map_score(
			User::get_instance_by_id($this->uid)->get_world_id(),
			User::get_instance_by_id($this->uid)->get_map_id(),
			$this->uid, $result->cur_map_score
		);
		ScoreCache::set_league_score(
			0, $this->uid, $result->cur_league_score
		);
		return $result->klunker;
	}

	/**
	 * Entfernt die angegebene Anzahl an Einheiten des Stammes aus dem
	 * Hauptgebäude. Macht bei der aktuellen Konfiguration nur bei Forschern
	 * Sinn, da alle anderen direkt auf die Karte gedroppt werden.
	 *
	 * Gibt zurück, wie viele Einheiten er danach noch hat.
	 *
	 * @param uint $amount
	 * @return uint
	 */
	public function unmorph($amount) {
		$result = Server::get_instance(CONF_ENGINE_SERVER_URL)
			->cmd_tribe_morph($this->uid, $this->id, $amount);

		$this->init_from_db((array)$result->tribe_data);

		ScoreCache::set_map_score(
			User::get_instance_by_id($this->uid)->get_world_id(),
			User::get_instance_by_id($this->uid)->get_map_id(),
			$this->uid, $result->cur_map_score
		);

		return $this->get_amount();
	}

	/// GETTER
	/**
	 * Gibt die aktuelle Anzahl der Einheiten des Tribes im MF zurück
	 *
	 * @return int die aktuelle Anzahl :)
	 */
	public function get_count() {
		return $this->amount;
	}

	public function get_amount() {
		return $this->get_count();
	}

	/**
	 * Gibt zurück wie lange es dauert eine Einheit davon zu morphen in Sekunden.
	 * @return float Dauer in Sekunden
	 */
	public function get_morph_duration() {
		return $this->morph_duration;
	}
	/**
	 * Gibt zurück, wie viele Einheiten noch gemorpht werden (also wie viele
	 * noch nicht gemorpht worden sind und in der Warteschlange zum Morphen stehen)
	 * @return int Anzahl der zu morphenden Einheiten
	 */
	public function get_morphing_count() {
		/**
		 * Gibt es nicht mehr, da man während des Morphens die Zahl nicht mehr
		 * erhöhen kann. Sprich: Es wird immer 0 zurückgegeben.
		 */
		return 0;
	}

	public function get_morphing_amount() {
		return $this->get_morphing_count();
	}

	public function get_add_value() {
		return $this->add_value ? $this->add_value  : 0;
	}
	public function get_add_stamp() {
		return $this->add_stamp ? $this->add_stamp : 0;
	}
	public function get_raw_amount() {
		return $this->amount ? $this->amount : 0;
	}
	public function get_raw_count() {
		return $this->get_raw_amount() ? $this->get_raw_amount() : 0;
	}
	public function is_morphing() {
		return ($this->add_value != 0);
	}
	public function is_mercenary() {
		return ($this->mercenary_id != 0);
	}
	public function get_race_level() {
		if (!$this->race_level)
			$this->init_cur_levels();
		return $this->race_level;
	}
	/**
	 * Gibt das Level der gewählten Ability zurück
	 *
	 * @param uint $which_ability	Das Level welcher Ability
	 * @return int das entsprechende Level
	 */
	public function get_ability_level($which_ability) {
		if (!$this->ability_levels)
			$this->init_cur_levels();
		return isset($this->ability_levels[$which_ability])
			? $this->ability_levels[$which_ability] : 0;
	}

	public function to_php_assoc_array() {
		$result = array (
			'abilities' => $this->abilities,
			'ability_levels' => $this->ability_levels,
			'add_stamp' => $this->add_stamp,
			'add_value' => $this->add_value,
			'amount' => $this->get_amount(),
			'autodrop' => $this->autodrop,
			'att' => $this->att,
			'cost' => $this->cost,
			'def' => $this->def,
			'dropped_once' => $this->dropped_once,
			'extra' => $this->extra,
			'hp' => $this->hp,
			'id' => $this->id,
			'mercenary_id' => $this->mercenary_id,
			'morph_duration' => $this->get_morph_duration(),
			'name' => $this->name,
			'pic_url' => $this->pic_url,
			'race' => $this->race,
			'race_level' => $this->race_level,
			'speed' => $this->speed,
			'state' => $this->state,
			'success' => $this->success,
			'uid' => $this->uid
		);
		for ($i = 0; isset($this->abilities[$i]); $i++) {
			$result['ability_'.$i] = $this->abilities[$i];
		}
		return $result;
	}

	public function to_php_std_class() {
		$x = new stdClass();
		$x->abilities = $this->abilities;
		$x->ability_levels = $this->ability_levels;
		$x->add_stamp = $this->add_stamp;
		$x->add_value = $this->add_value;
		$x->amount = $this->amount;
		$x->autodrop = $this->autodrop;
		$x->att = $this->att;
		$x->cost = $this->cost;
		$x->def = $this->def;
		$x->dropped_once = $this->dropped_once;
		$x->extra = $this->extra;
		$x->hp = $this->hp;
		$x->id = $this->id;
		$x->mercenary_id = $this->mercenary_id;
		$x->name = $this->name;
		$x->pic_url = $this->pic_url;
		$x->race = $this->race;
		$x->race_level = $this->race_level;
		$x->speed = $this->speed;
		$x->state = $this->state;
		$x->success = $this->success;
		$x->uid = $this->uid;
		for ($i = 0; isset($this->abilities[$i]); $i++) {
			$x->{'ability_'.$i} = $this->abilities[$i];
		}
		return $x;
	}
}

//////////// ENDE VON DER KLASSE TRIBE //////////////


/**
 * Die Tribes-Klasse repräsentiert die Sammlung aller Stämme eines Users,
 * die NICHT auf der Karte (also noch im Hauptgebäude) sind.
 * Der Motherfungus hat einen Link hierauf,
 * aber es kann auch eigenständig benutzt werden (aus Effizienzgründen, da
 * nicht immer alle Informationen des MF benötigt werden, wenn die Tribes
 * gebraucht werden)
 *
 * !Singleton (nur eine Instanz pro UID)
 */
class Tribes implements ITribes {
	private /*Tribes__TribeInMF[]*/ $tribes; //all Tribes als "tribe_id => Tribe"
	private /*int*/ $uid;
	private $morph_speed;
	private $unit_limit;

	static private $INSTANCE = array();	// instanzüberwachung. Nur eine Instanz
										// pro UID. Konstruktor ist private.
										// Objekt statisch über get_instance($uid)
 										// anfordern! (s.u.)

 	/**
	 * Diese Statische Funtion als Konstruktor benutzen !
	 *
	 * @param uint $uid
	 * @return Tribes
	 **/
 	public static function get_instance($uid) {
 		if (((int)$uid) <= 0)
 			throw new IllegalArgumentException("\$uid", "N+\{0}", $uid);
 		if (!self::$INSTANCE || !self::$INSTANCE[$uid]) {
 			self::$INSTANCE[$uid] = new Tribes($uid);
 		}
 		return self::$INSTANCE[$uid];
 	}
	private function __construct($uid) {
		$this->uid = $uid;
		$this->init();
	}

	private function init() {
		$tribes_data = Server::get_instance(CONF_ENGINE_SERVER_URL)
			->get_tribes_data($this->uid);
		foreach ((array)($tribes_data->tribes) as $tribe_id => $tribe)
			$this->tribes[$tribe_id] = new Tribes__TribeInMF((array)$tribe);
		$this->morph_speed = $tribes_data->morph_speed;
		$this->unit_limit = $tribes_data->unit_limit;
	}

	/**
	 * Gibt alle Tribes des Users zurück.
	 *
	 * @return Tribes__TribeInMF[] alle Tribes des Users als "tribe_id => Tribe"
	 */
	public function get_tribes() {
		return $this->tribes;
	}

	/**
	 * Gibt den Tribe des Users zurück, welcher die übergebene ID besitzt.
	 *
	 * @param uint $id
	 * @param bool $force
	 * @return Tribes__TribeInMF der gewünschte Tribe
	 */
	public function get_tribe_by_id($id, $force=false) {
		if ($force) {
			$this->init();
		}
		return $this->tribes[$id];
	}

	/**
	 * Gibt die Tribes mit der angegebenen Rasse zurück
	 *
	 * @param int $race						Die Rassen-ID
	 * @return Tribes__TribeInMF[]/bool		Die Tribes in einem Array oder false,
	 * 										wenn kein Hit.
	 */
	public function get_tribes_by_race($race) {
		$result = array();
		foreach ($this->tribes as $tribe) {
			if ($tribe->get_race() == $race)
				$result[] = $tribe;
		}
		return (count($result) > 0) ? $result : false;
	}


	/**
	 * Gibt die Tribes zurück, die gerade morphen
	 *
	 * @return Tribes__TribeInMF[] die morphenden Tribes
	 */
	public function get_morphing_tribes() {
		$result = array();
		foreach ($this->tribes as $tribe) {
			if ($tribe->is_morphing())
				$result[] = $tribe;
		}
		return $result;
	}

	/**
	 * Fügt einen neuen Tribe zu den produzierbaren Tribes im MF ein.
	 *
	 * @param int $race			Grundtyp/Rasse
	 * @param string $name		Name, den der Tribe bekommen soll
	 * @param int[5] $abilities	Fähigkeiten, die der Tribe haben soll
	 * @return Tribes__TribeInMF 	Den hinzugefügten Tribe als Objekt
	 */
	public function insert_new_tribe($race, $name, array $abilities=array()) {
		$new_tribe_data = Server::get_instance(CONF_ENGINE_SERVER_URL)
			->cmd_tribes_insert_new_tribe($this->uid, $race, $name, $abilities);
		$new_tribe = new Tribes__TribeInMF((array)$new_tribe_data);
		$this->tribes[] = $new_tribe;
		return $new_tribe;
	}

	/**
	 * Gibt ein Array zurück, dessen erster Wert angibt, wie viele Einheiten
	 * bereits produziert wurden und vom Limit abgezogen werden müssen, und
	 * dessen zweiter Wert die höhe des Limits angibt.
	 *
	 * @return uint[2]
	 */
	public function get_unit_limit() {
		return $this->unit_limit;
	}

	public function get_morph_speed() {
		return $this->morph_speed;
	}
}

?>