<?php
/**
 * Mithilfe dieser Klasse lässt sich herausfinden, welche Spieler auf welcher
 * Karte gerade spielen (also nicht warten, sondern wirklich aktiv spielen),
 * und welchen Zustand die Spieler in Bezug auf diese Karte haben - z.B. ob
 * sie noch auf der Karte sind oder verdrängt wurden, oder ob sie vielleicht
 * sogar gewonnen haben :)
 */

require_once (dirname(__FILE__).'/../../shared/classes/IMapPlayers.inc');
require_once (dirname(__FILE__).'/../User.inc');

class MapPlayers implements IMapPlayers {
	private /*array*/ $player_data = array();
	private /*uint*/ $world_id;
	private /*uint*/ $map_id;
	private /*bool*/ $map_is_over;
	private /*bool*/ $map_is_full;
	private static $INSTANCES = array();

	/**
	 * @param uint $world_id
	 * @param uint $map_id
	 * @param bool $force
	 * @return MapPlayers
	 */
	public static function get_instance($world_id, $map_id, $force=false) {
		$force = true;
		if (!isset(self::$INSTANCES[$world_id][$map_id]))
			self::$INSTANCES[$world_id][$map_id] = new MapPlayers($world_id,
				$map_id, $force);
		return self::$INSTANCES[$world_id][$map_id];
	}

	protected function __construct($world_id, $map_id, $force=false) {
		$this->map_id = $map_id;
		$this->world_id = $world_id;

		if (!$world_id) {
			$this->player_data = new stdClass();
			$this->map_id_over = true;
		}
		else {
			$data = Server::get_instance(CONF_ENGINE_SERVER_URL)
				->get_map_players_data($world_id, $map_id);
			$this->player_data = $data->player_data;

			$this->map_is_over = $data->map_is_over;
			$this->map_id = $data->map_id;
			$this->world_id = $data->world_id;
			$this->map_is_full = $data->map_is_full;
		}
	}

	/**
	 * Gibt zurück, wie der Zustand des Users auf der Karte gerade ist.
	 * Mögliche Rückgabewerte sind MapPlayers::STATE_STILL_ALIVE,
	 * MapPlayers::STATE_KILLED, MapPlayers::STATE_VICTOR,
	 * MapPlayers::STATE_WAS_NEVER_ON_MAP.
	 *
	 * @param uint $uid		Die ID des Users
	 * @return MapPlayers::STATE_...
	 */
	public function get_user_state_on_map($uid) {
		if (!isset($this->player_data->$uid))
			return self::STATE_WAS_NEVER_ON_MAP;
		if ($this->player_data->$uid->killer_id)
			return self::STATE_KILLED;
		if ($this->map_is_over)
			return self::STATE_VICTOR;
		else if ($this->map_is_full) {
			/**
			 * Wenn die Karte voll ist, dann wird geschaut, ob $uid der einzig
			 * noch lebende ist. Wenn ja, dann hat er gewonnen, wenn wir noch
			 * einen anderen lebenden finden, dann ist der Status einfach nur
			 * "am Leben"
			 */
			foreach ($this->player_data as $uid2 => $data) {
				if ($uid2 == $uid)
					continue;
				if (!$data->killer_id)
					return self::STATE_STILL_ALIVE;
			}
			return self::STATE_VICTOR;
		}
		else
			return self::STATE_STILL_ALIVE;
	}

	/**
	 * Gibt die ID des Killers zurück, der den Spieler gekillt hat oder FALSE
	 * wenn er noch am Leben ist oder gar nicht auf der Karte war.
	 *
	 * @param uint $uid		Die ID des Users
	 * @return uint/FALSE
	 */
	public function get_killer_id($uid) {
		if (empty($this->player_data->$uid) ||
			!($this->player_data->$uid->killer_id))
		{
			return false;
		}
		else
			return $this->player_data->$uid->killer_id;
	}

	/**
	 * Gibt zurück, welchen Heimatsektor der User auf dieser Karte aktuell hat
	 * oder mal hatte. Sprich: Dieser Wert bleibt auch nach Verlassen der Karte
	 * gültig im Gegensatz zu dem, den das User-Objekt liefert.
	 * Es wird FALSE zurückgeliefert, falls der User niemals auf dieser Karte
	 * gespielt hat.
	 *
	 * @param uint $uid
	 * @return [0,1,...99]/FALSE
	 */
	public function get_homesector($uid) {
		if (empty($this->player_data->$uid) ||
			!($this->player_data->$uid->homesector))
		{
			return false;
		}
		else
			return $this->player_data->$uid->homesector;
	}

	/**
	 * Gibt zurück, wann der Spieler gestorben ist, oder FALSE, wenn er noch
	 * lebt oder gar nicht auf der Karte jemals war.
	 *
	 * @param uint $uid		Die ID des Users
	 * @return uint/FALSE	Zeitpunkt des Todes
	 */
	public function get_kill_time($uid) {
		if (empty($this->player_data->$uid) ||
			!($this->player_data->$uid->kill_time))
		{
			return false;
		}
		else
			return $this->player_data->$uid->kill_time;
	}

	/**
	 * Gibt zurück, wieviele Punkte der Spieler am Ende der Karte hatte oder
	 * FALSE, wenn der Spieler noch auf der Karte ist oder niemals auf der
	 * Karte war.
	 *
	 * @param uint $uid		Die ID des Users
	 * @return int/FALSE
	 */
	public function get_last_score($uid) {
		if (empty($this->player_data->$uid) ||
			!($this->player_data->$uid->last_map_score))
		{
			return false;
		}
		else
			return $this->player_data->$uid->last_map_score;
	}

	/**
	 * Gibt zurück, wann der Spieler die Karte betreten hat.
	 *
	 * Gibt FALSE zurück, wenn der Spieler noch auf der Karte ist oder
	 * niemals auf der Karte war.
	 *
	 * @param uint $uid
	 * @return uint/FALSE
	 */
	public function get_start_time($uid) {
		if (empty($this->player_data->$uid) ||
			!($this->player_data->$uid->start_time))
		{
			return false;
		}
		else
			return $this->player_data->$uid->start_time;
	}

	/**
	 * Gibt alle User auf der Karte zurück. Wird $only_alive_ones=true gesetzt,
	 * werden die bereits gestorbenen Spieler ignoriert.
	 *
	 * @param bool $only_alive_ones=false	Nur die lebenden Spieler
	 * @return User[]
	 */
	public function get_users_on_map($only_alive_ones=false) {
		$result = array();
		foreach ($this->player_data as $uid => $jnk) {
			if (!$only_alive_ones ||
				($this->get_user_state_on_map($uid) != self::STATE_KILLED))
			{
				$result[] = User::get_instance_by_id($uid);
			}
		}
		return $result;
	}

	public function get_uids_on_map($only_alive_ones=false) {
		$result = array();
		foreach ($this->player_data as $uid => $jnk) {
			if (!$only_alive_ones ||
				($this->get_user_state_on_map($uid) != self::STATE_KILLED))
			{
				$result[] = $uid;
			}
		}
		return $result;
	}

	public function change_color($uid, $color) {
		$this->player_data->$uid->color =
			Server::get_instance(CONF_ENGINE_SERVER_URL)
				->cmd_change_player_color($uid, $this->world_id,
					$this->map_id, $color);
	}

	/**
	 * Gibt die Farbe des Users mit der angegebenen ID zurück, die er auf dieser
	 * Karte hat oder hatte.
	 * Gibt FALSE zurück, wenn der User niemals auf dieser Karte war.
	 *
	 * @param uint $uid
	 * @param bool $return_color_codes
	 * @return  {'blue','green','orange','pink','purple','red','white','yellow'}
	 * 						/ FALSE / string
	 */
	public function get_color($uid, $return_color_codes=false) {
		if (empty($this->player_data->$uid) ||
			!($this->player_data->$uid->color))
		{
			return false;
		}
		else if ($return_color_codes) {
			switch ($this->player_data->$uid->color) {
				case 'blue':	return '#0099FF';
				case 'green':	return '#00CC00';
				case 'orange':	return '#FF9900';
				case 'pink':	return '#FF00FF';
				case 'purple':	return '#9900CC';
				case 'red':		return '#FF0000';
				case 'white':	return '#FFFFFF';
				case 'yellow':	return '#FFFF00';
			}
		}
		else
			return $this->player_data->$uid->color;
	}

	public function to_php_std_class() {
		$x = new stdClass();
		$x->world_id = $this->world_id;
		$x->map_id = $this->map_id;
		$x->player_data = $this->player_data;
		$x->map_is_over = $this->map_is_over;
		return $x;
	}

	public function to_php_assoc_array() {
		return array (
			'world_id'		=>	$this->world_id,
			'map_id'		=>	$this->map_id,
			'player_data'	=>	$this->player_data,
			'map_is_over'	=>	$this->map_is_over
		);
	}
}
?>