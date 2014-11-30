<?php
require_once(dirname(__FILE__).'/MapChooser.inc');
/**
 * Die Klasse ist zuständig dafür, dass der User eine FFA-Karte wählen kann, dass
 * diese sich füllt und, wenn voll, gestartet wird.
 */
class MapChooserFFA extends MapChooser {
	private static $INSTANCES;

	/**
	 * @param uint $uid
	 * @return MapChooserFFA
	 */
	public static function get_instance($uid) {
		if (!self::$INSTANCES[$uid])
			self::$INSTANCES[$uid] = new self($uid);

		return self::$INSTANCES[$uid];
	}

	/**
	 * @param uint $uid
	 * @return MapChooserFFA
	 */
	private function __construct($uid) {
		$this->uid = (int)$uid;
		$this->init();
	}

	protected function init() {
		$user = User::get_instance_by_id($this->uid);
		$cur_level = $user->get_cur_level();

		$x = Server::get_instance(CONF_ENGINE_SERVER_URL)
			->get_map_chooser_ffa_data($this->uid);
		$this->available_maps = (array)$x->available_maps;
		$this->chosen_map_index = $x->chosen_map_index;
	}

	/**
	 * Setzt den User auf die Map mit der angegebenen ID
	 *
	 * @param int $map_index		Der User wird auf die
	 * 							Karte available_maps[$map_index] gesetzt
	 */
	public function choose_map($map_index) {
		$x = Server::get_instance(CONF_ENGINE_SERVER_URL)
			->cmd_map_chooser_ffa_choose_map($this->uid, $map_index);
		$this->available_maps = (array)$x->available_maps;
		$this->chosen_map_index = $x->chosen_map_index;
	}

	/**
	 * Mach die Mapwahl rückgängig. Der Spieler wird somit wieder in den Zustand
	 * zurückgesetzt, als habe er bislang noch keine Karte gewählt.
	 */
	public function unchoose_map() {
		$x = Server::get_instance(CONF_ENGINE_SERVER_URL)
			->cmd_map_chooser_ffa_unchoose_map($this->uid);
		$this->available_maps = (array)$x->available_maps;
		$this->chosen_map_index = $x->chosen_map_index;
	}

	/**
	 * Wenn ein Spieler auf einer Karte wartet, kann er sich als "bereit" markieren
	 * lassen. Sind alle Spieler bereit, startet die Karte automatisch.
	 * Mit Aufruf dieser Funktion wird der Spieler als "bereit" markiert, wenn er
	 * es noch nicht war, ansonsten wird dieser Bereit-Status wieder entzogen.
	 * Wartet der User gerade gar nicht auf einer Map, so wird eine
	 * Hackattack-Exception geworfen
	 */
	public function toggle_ready_state() {
		$x = Server::get_instance(CONF_ENGINE_SERVER_URL)
			->cmd_map_chooser_ffa_toggle_ready_state($this->uid);
		$this->available_maps = (array)$x->available_maps;
		$this->chosen_map_index = $x->chosen_map_index;
	}
}
?>