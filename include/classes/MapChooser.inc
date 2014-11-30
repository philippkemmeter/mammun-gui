<?php
require_once(dirname(__FILE__).'/../shared/classes/IMapChooser.inc');
/**
 * Abstrakte Klasse, die die Schnittstellen für MapChooser-Klassen, wie
 * MapChooserFFA, bereitstellt.
 */

abstract class MapChooser implements IMapChooser {
	protected $uid;	// Die User-ID
	protected $available_maps = array();
	protected $chosen_map_index = -1;
	protected $world_id;


//	abstract static public function get_instance($uid, $world_id=0);

	abstract public function choose_map($map_index);
	abstract public function unchoose_map();

	public function get_available_maps() {
		return $this->available_maps;
	}

	public function get_chosen_map() {
		return ($this->chosen_map_index < 0)
			? null : $this->available_maps[$this->chosen_map_index];
	}
}
?>