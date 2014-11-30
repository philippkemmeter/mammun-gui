<?
require_once(dirname(__FILE__).'/../shared/classes/IWorld.inc');
require_once(dirname(__FILE__).'/../shared/classes/exceptions/'.
												'IllegalArgumentException.inc');
Language::get_instance()->load_language_file('Worlds');

class Worlds implements IWorld {
	/**
	 * Gibt alle Welten zurück als Objekte folgender Struktur:
	 *
	 * <code>
	 * $o->id               uint    Welt-ID
	 * $o->minimum_level    uint    Welches Level minimal benötigt wird
	 * $o->maximum_level    uint    Welches Level maximal erlaubt ist
	 * $o->name             string  Wie die Welt bezeichnet wird
	 * </code>
	 *
	 * $type muss eine der MAP_TYPE-Konstanten sein (s. IWorld)
	 *
	 * @param uint $type
	 * @return stdClass[]
	 */
	public static function get_worlds($type) {
		if (($type < 0) || ($type > self::MAP_TYPE_MAX)) {
			throw new IllegalArgumentException("type",
				"{0,...,".self::MAP_TYPE_MAX.'}', $type);
		}

		if (empty($_SESSION['Worlds']['worlds'][$type])) {
			$w =
				Server::get_instance(CONF_ENGINE_SERVER_URL)->get_worlds($type);
			foreach ($w as $world) {
				$_SESSION['Worlds']['worlds'][$type][$world->id] = $world;
				$_SESSION['Worlds']['worlds'][$type][$world->id]->name
					= self::get_world_name($world->id, $type);
			}
		}

		return $_SESSION['Worlds']['worlds'][$type];
	}

	/**
	 * Die Welt Welten zurück, die dem übergebenen Level entspricht:
	 *
	 * <code>
	 * $o->id               uint    Welt-ID
	 * $o->minimum_level    uint    Welches Level minimal benötigt wird
	 * $o->maximum_level    uint    Welches Level maximal erlaubt ist
	 * $o->name             string  Wie die Welt bezeichnet wird
	 * </code>
	 *
	 * $type muss eine der MAP_TYPE-Konstanten sein (s. IWorld)
	 *
	 * @param uint $level
	 * @param uint $type
	 * @return stdClass
	 */
	public static function get_world_data_by_level($level, $type) {
		if (($type < 0) || ($type > self::MAP_TYPE_MAX)) {
			throw new IllegalArgumentException("type",
				"{0,...,".self::MAP_TYPE_MAX.'}', $type);
		}
		if (empty($_SESSION['Worlds']['worlds'][$type])) {
			$w =
				Server::get_instance(CONF_ENGINE_SERVER_URL)->get_worlds($type);
			foreach ($w as $world) {
				$_SESSION['Worlds']['worlds'][$type][$world->id] = $world;
				$_SESSION['Worlds']['worlds'][$type][$world->id]->name
					= self::get_world_name($world->id, $type);
			}
		}
		foreach ($_SESSION['Worlds']['worlds'][$type] as $id => $world) {
			if (($world->minimum_level <= $level) &&
				($world->maximum_level >= $level))
			{
				return $world;
			}
		}
	}

	/**
	 * Gibt die Daten der entsprechenden Welt zurück:
	 *
	 * <code>
	 * $o->id               uint    Welt-ID
	 * $o->minimum_level    uint    Welches Level minimal benötigt wird
	 * $o->maximum_level    uint    Welches Level maximal erlaubt ist
	 * $o->name             string  Wie die Welt bezeichnet wird
	 * </code>
	 *
	 * @param uint $id
	 * @param uint $type
	 * @return stdClass
	 */
	public static function get_world_data_by_id($id, $type) {
		if (($type < 0) || ($type > self::MAP_TYPE_MAX)) {
			throw new IllegalArgumentException("type",
				"{0,...,".self::MAP_TYPE_MAX.'}', $type);
		}
		if (empty($_SESSION['Worlds']['worlds'][$type])) {
			$w =
				Server::get_instance(CONF_ENGINE_SERVER_URL)->get_worlds($type);
			foreach ($w as $world) {
				$_SESSION['Worlds']['worlds'][$type][$world->id] = $world;
				$_SESSION['Worlds']['worlds'][$type][$world->id]->name
					= self::get_world_name($world->id, $type);
			}
		}
		return $_SESSION['Worlds']['worlds'][$type][$id];
	}

	/**
	 * Gibt den Bezeichner der angegebenen Welt zurück
	 *
	 * @param uint $id
	 * @return string
	 */
	public static function get_world_name($id, $type=self::MAP_TYPE_FFA) {
		if (empty($_SESSION['Worlds']['worlds'][$type])) {
			self::get_worlds($type);
		}
		return @constant('LNG_WORLD_NAME'.
			(int)$_SESSION['Worlds']['worlds'][$type][$id]->name_index);
	}

}

?>