<?php
/**
 * Diese Klasse repräsentiert den Datensatz eines Gebäudes, jedoch ohne
 * jegliche Funktionalität.
 * Hiervon erben dann alle möglichen Gebäudetypen, also konkret die Klasse
 * <code>Buildings</code>, welche ein baubares Gebäude repräsentiert, und
 * die Klasse <code>BuildingOnMap</code>, welches ein Gebäude auf der Karte
 * repräsentiert.
 *
 * @author Philipp Kemmeter
 */
abstract class AbstractBuilding {
	protected $sp = 0;
	protected $att = 0;
	protected $def = 0;
	protected $name = '';
	protected $desc = '';
	protected $const_key = '';
	protected $size = 0;
	protected $height = 0;

	protected $level = 0;
	protected $type = 0;

	protected $pic = '';
	protected $grow = 0;
	protected $cost = 0;
	protected $req_sc_id = 0;
	protected $req_level = 0;
	protected $extra = 0;

	public function get_sp() {
		return $this->sp;
	}

	public function get_att() {
		return $this->att;
	}

	public function get_def() {
		return $this->def;
	}

	public function get_name() {
		return $this->name;
	}

	public function get_desc() {
		return $this->desc;
	}

	public function get_const_key() {
		return $this->const_key;
	}

	public function get_size() {
		return ($this->size) ? $this->size : 0;
	}

	public function get_height() {
		return $this->height ? $this->height : 0;
	}

	public function get_level() {
		return ($this->level) ? $this->level : 0;
	}

	public function get_type() {
		return ($this->type) ? $this->type : 0;
	}

	public function get_pic() {
		return $this->pic;
	}

	public function get_grow() {
		return $this->grow;
	}

	public function get_req_sc_id() {
		return ($this->req_sc_id) ? $this->req_sc_id : 0;
	}

	public function get_req_level() {
		return ($this->req_level) ? $this->req_level : 0;
	}

	public function get_cost() {
		return ($this->cost) ? $this->cost : 0;
	}

	public function get_extra() {
		return $this->extra;
	}

}
?>