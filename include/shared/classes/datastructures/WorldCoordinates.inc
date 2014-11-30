<?php
require_once(dirname(__FILE__).'/../exceptions/IllegalArgumentException.inc');
require_once(dirname(__FILE__).'/../helper/IConvertablePHPStdClass.inc');
/**
 * Diese Klasse repräsentiert ein 3-Tuppel bestehend als World-ID, Map-ID
 * und Sector-ID zur Identifikation eines Sektors in Weltkoordinaten
 */

class WorldCoordinates implements IConvertablePHPStdClass {
	private $world_id;
	private $map_id;
	private $sector_id;

	/**
	 * Erzeugt eine neue Weltkoordinate eines Sektors
	 *
	 * @param uint $world_id
	 * @param uint $map_id
	 * @param [0,...,99] $sector_id
	 */
	public function __construct($world_id, $map_id, $sector_id) {
		if ($world_id < 0) {
			error_log(print_r(debug_backtrace(false),true));
			throw new IllegalArgumentException("world_id", "N", $world_id);
		}
		if ($map_id < 0) {
			error_log(print_r(debug_backtrace(false),true));
			throw new IllegalArgumentException("map_id", "N", $map_id);
		}
		if (($sector_id < 0) || ($sector_id > 99)) {
			error_log(print_r(debug_backtrace(false),true));
			throw new IllegalArgumentException("sector_id", "[0,..,99]",
				$sector_id);
		}
		$this->world_id = $world_id;
		$this->map_id = $map_id;
		$this->sector_id = $sector_id;
	}

	public function get_world_id() {
		return $this->world_id;
	}

	public function get_map_id() {
		return $this->map_id;
	}

	public function get_sector_id() {
		return $this->sector_id;
	}

	public function to_php_std_class() {
		$result = new stdClass();
		$result->world_id = $this->world_id;
		$result->map_id = $this->map_id;
		$result->sector_id = $this->sector_id;
		return $result;
	}
}
?>