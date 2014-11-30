<?php
require_once(dirname(__FILE__)
	. '/../../shared/classes/buildings/AbstractBuilding.inc');

/**
 * Jedes Gebäude hat gewisse Eigenschaften, die jetzt durch ein Objekt vereint
 * werden sollen (z.B. Att/Def/SP etc.. aber auch die Größe usw).
 * Das Building-Objekt repräsentiert hierbei ein Gebäude i.a., das aber keiner
 * Instanz auf der Karte zu Grund liegt, sondern die Eigenschaften gewisser
 * Gebäude-Typen bei bestimmten Stufen i.a. darstellt.
 */

class Building extends AbstractBuilding {
	protected $upgrade_cost = 0;
	protected $custom = false;

	/**
	 * @param stdClass $data
	 * @return Building
	 */
	public function __construct(stdClass $data) {
		$this->init($data);
	}

	protected function init(stdClass $data) {
		$this->sp = (int)$data->sp;
		$this->att = (int)$data->att;
		$this->def = (int)$data->def;
		$this->const_key = $data->const_key;
		$this->size = (int)$data->size;
		$this->height = (float)$data->height;
		$this->level = (int)$data->level;
		$this->type = (int)$data->type;
		$this->pic = $data->pic;
		$this->grow = (float)$data->grow;
		$this->cost = (int)$data->cost;
		$this->upgrade_cost = (int)$data->upgrade_cost;
		$this->req_sc_id = (int)$data->req_sc_id;
		$this->req_level = (int)$data->req_level;
		$this->extra = $data->extra;
		$this->custom = (bool)$data->custom;

		$this->name = @constant('LNG_B_'.$data->const_key);
		$this->desc = @constant('LNG_B_'.$data->const_key.'_DESC');
	}

	public function get_upgrade_cost() {
		return ($this->upgrade_cost) ? $this->upgrade_cost : 0;
	}

	public function is_custom_building() {
		return $this->custom;
	}
}
?>
