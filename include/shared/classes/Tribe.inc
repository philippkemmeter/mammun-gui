<?php
require_once(dirname(__FILE__) . '/ScienceConstants.inc');

abstract class AbstractTribe {
	protected $id = 0;
	protected $uid = 0;
	protected $name = '';
	protected $race = 0;
	protected $pic_url = '';
	protected $cost = 0;
	protected $state = 0;
	protected $abilities = array();
	protected $hp = 0;
	protected $att = 0;
	protected $def = 0;
	protected $speed = 0;
	protected $extra = 0;	// Rassenabhängige Extras
	protected $amount = 0;
	protected $success = 0;	// Erfolg wenn mit Tribe eine Gruppe im Kampf
							// gemeint ist

	protected function init_from_db(array $db_data=null) {
		if (!$db_data)
			return;
		$this->id = isset($db_data['id']) ? $db_data['id'] : 0;
		$this->uid = isset($db_data['uid']) ? $db_data['uid'] : 0;
		$this->name = isset($db_data['name']) ? $db_data['name'] : '';
		$this->race = isset($db_data['race']) ? $db_data['race'] : 0;
		$this->pic_url = 'units/'.(isset($db_data['pic_url']) ?
									$db_data['pic_url'] : '');
		$this->cost = isset($db_data['cost']) ? $db_data['cost'] : 0;
		$this->state = isset($db_data['state']) ? $db_data['state'] : 0;
		$this->abilities = array();
		for ($i = 0; isset($db_data['ability_'.$i]); $i++)
			$this->abilities[$i] = $db_data['ability_'.$i];
		$this->hp = isset($db_data['hp']) ? $db_data['hp'] :
			(isset($db_data['hitpoints']) ? $db_data['hitpoints'] : 0);
		$this->att = isset($db_data['att']) ? $db_data['att'] :
			(isset($db_data['attack']) ? $db_data['attack'] : 0);
		$this->def = isset($db_data['def']) ? $db_data['def'] :
			(isset($db_data['defend']) ? $db_data['defend'] : 0);
		$this->speed = isset($db_data['speed']) ? $db_data['speed'] : 0;
		$this->extra = isset($db_data['extra']) ? $db_data['extra'] : 0;
		$this->amount = isset($db_data['count']) ? $db_data['count'] : 0;
		$this->success = isset($db_data['success']) ? $db_data['success'] : 0;
	}

	public function get_id() {
		return $this->id;
	}
	public function get_uid() {
		return $this->uid;
	}
	public function get_name() {
		return ($this->race == 2 /*Scientist*/)
			? ScienceConstants::$SC_NAME[2]
			: $this->name;
	}
	public function get_race() {
		return $this->race;
	}
	public function get_pic_url() {
		return $this->pic_url;
	}
	public function get_cost() {
		return ($this->cost >= 1) ? $this->cost : 1;	// Mindestkosten sind 1
	}
	public function get_state() {
		return $this->state;
	}
	public function get_abilities() {
		return $this->abilities;
	}
	public function get_hp() {
		return $this->hp;
	}
	public function get_att() {
		return $this->att;
	}
	public function get_def() {
		return $this->def;
	}
	public function get_speed() {
/*		$mapsettings = Mapsettings::get_instance();*/
		$time_factor = 1; //$mapsettings->get_timefactor();
		return $this->speed*$time_factor;
	}
	public function get_extra() {
		return $this->extra;
	}
	public function get_count() {
		return $this->amount;
	}
	public function get_amount() {
		return $this->get_count();
	}
	public function get_success() {
		return $this->success;
	}

	public function set_success($s) {
		$this->success = $s;
	}

	/**
	 * Gibt zurück, wie lange es dauert eine Einheit eines solchen Tribes zu
	 * produzieren.
	 * (hypothetisch, da der konfigurierte Tribe ja noch nicht existiert).
	 *
	 * @return uint
	 */
	public function get_time_needed() {
		return self::calculate_needed_time($this->get_cost());
	}

	/**
	 * Berechnet die Dauer die ein Tribe mit den angegebenen Kosten zu Morphen
	 * braucht (in Sekunden).
	 *
	 * @param uint $cost
	 * @return uint
	 */
	public static function calculate_needed_time($cost) {
		return floor(5*pow($cost, 1.15));
	}
}
/**
 * Allgemeine Tribes-Klasse. Macht keinen eigenen Datenbankzugriff.
 *
 * @author Philipp Kemmeter
 */
class Tribe extends AbstractTribe {
	public function __construct(array $db_data) {
		$this->init_from_db($db_data);
	}
}
?>
