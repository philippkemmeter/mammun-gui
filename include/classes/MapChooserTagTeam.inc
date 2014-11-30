<?php
require_once(dirname(__FILE__).'/MapChooser.inc');
/**
 * Die Klasse ist zuständig dafür, dass der User eine FFA-Karte wählen kann, dass
 * diese sich füllt und, wenn voll, gestartet wird.
 */
class MapChooserTagTeam extends MapChooser {
	private static $INSTANCES;

	private $auth_key;

	/**
	 * @param uint $uid
	 * @return MapChooserTagTeam
	 */
	public static function get_instance($uid, $world_id=0) {
		if (@!self::$INSTANCES[$uid][$world_id])
			self::$INSTANCES[$uid][$world_id] = new self($uid, $world_id);

		return self::$INSTANCES[$uid][$world_id];
	}

	/**
	 * @param uint $uid
	 * @param uint $world_id
	 * @return MapChooserTagTeam
	 */
	private function __construct($uid, $world_id=0) {
		$this->uid = (int)$uid;
		$this->world_id = (int)$world_id;
		$this->init();
	}

	protected function init() {
		$user = User::get_instance_by_id($this->uid);
		$cur_level = $user->get_cur_level();
		$x = Server::get_instance(CONF_ENGINE_SERVER_URL)
			->get_map_chooser_tag_team_data($this->uid, true, $this->world_id);
		$this->available_maps = (array)$x->available_maps;
		$this->chosen_map_index = $x->chosen_map_index;
		$this->auth_key = $x->auth_key;
	}

	/**
	 * Versucht den Spieler auf die Karte zu setzen, die sein Auth-Key angibt.
	 *
	 * Je nach Auth-Key bewirbt sich der Spieler als Tag-Team-Partner eines
	 * Spielers. Er wurde also eingeladen. Dieser Tag-Team-Platz ist jedoch
	 * nicht sicher, da sich ggf. mehrere Leute auf den Platz beworben haben.
	 * In dem Fall wird die Exception "self::ERR_MAP_ALREADY_FULL" geworfen.
	 *
 	 * @param string $auth_key	Die Hälfte der Heimatsektoren sind reserviert.
	 * 							Der $auth_key entscheidet, für welchen dieser
	 * 							Sektoren er sich bewirbt (könnte schon belegt
	 * 							sein)
	 */
	public function choose_map_by_auth_key($auth_key) {
		$x = Server::get_instance(CONF_ENGINE_SERVER_URL)
			->cmd_map_chooser_tag_team_choose_map_by_auth_key($this->uid,
				$auth_key, $this->world_id);
		$this->available_maps = (array)$x->available_maps;
		$this->chosen_map_index = $x->chosen_map_index;
	}

	/**
	 * Setzt den User auf die Map mit der angegebenen ID
	 *
	 * @param int $map_index	Der User wird auf die
	 * 							Karte available_maps[$map_index] gesetzt
	 */
	public function choose_map($map_index) {
		$x = Server::get_instance(CONF_ENGINE_SERVER_URL)
			->cmd_map_chooser_tag_team_choose_map($this->uid, $map_index,
				$this->world_id);
		$this->available_maps = (array)$x->available_maps;
		$this->chosen_map_index = $x->chosen_map_index;
		$this->auth_key = $x->auth_key;
	}

	/**
	 * Mithilfe dieser Funktion wird der Spieler auf die nächste freie Karte
	 * gesetzt.
	 */
	public function choose_random_map() {
		$x = Server::get_instance(CONF_ENGINE_SERVER_URL)
			->cmd_map_chooser_tag_team_choose_random_map($this->uid,
				$this->world_id);
		$this->available_maps = (array)$x->available_maps;
		$this->chosen_map_index = $x->chosen_map_index;
		$this->auth_key = $x->auth_key;
	}

	/**
	 * Mach die Mapwahl rückgängig. Der Spieler wird somit wieder in den Zustand
	 * zurückgesetzt, als habe er bislang noch keine Karte gewählt.
	 */
	public function unchoose_map() {
		$x = Server::get_instance(CONF_ENGINE_SERVER_URL)
			->cmd_map_chooser_tag_team_unchoose_map($this->uid, $this->world_id);
		$this->available_maps = (array)$x->available_maps;
		$this->chosen_map_index = $x->chosen_map_index;
	}

	/**
	 * Gibt die Daten zurück, die dem entsprechenden Auth-Key zugeordnet sind.
	 *
	 * Die Rückgabe ist ein Standartobjekt folgender Struktor:
	 *
	 * $o->world_id
	 * $o->map_id		ID der Karte, für die der Key gilt
	 * $o->sector_ids	Liste der Sektoren, die in Frage kommen. Ist die Liste
	 * 					leer, sind entweder alle möglichen Sektoren belegt, oder
	 * 					die Reservierungsdauer ist ausgelaufen.
	 * 					Dann kann höchstens versucht werden, ohne Authkey normal
	 * 					der Karte beizutreten (falls sie nicht voll ist).
	 * $o->res_by_uid	ID des Users, der den Platz reserviert hat
	 * $o->min_level	Die Stufe, die der User mindestens braucht, um hier
	 * 					mitspielen zu dürfen
	 * $o->max_level	Die Stude, die der User maximal haben darf, um hier ohne
	 * 					Handicap mitspielen zu dürfen
	 *
	 * @param string $auth_key
	 * @return stdClass[]
	 */
	public static function get_data_by_auth_key($auth_key) {
		return Server::get_instance(CONF_ENGINE_SERVER_URL)
					->get_map_chooser_tag_team_data_by_auth_key($auth_key);
	}
}
?>