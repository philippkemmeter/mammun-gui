<?
/**
 * Verwaltet die Rohstoffe eines Spielers.
 * In jedem Sektor wird bei dominanz eine Ressource namens Bio abgebaut. Diese
 * ist nur intern und kommt dem Spieler nie zu Gesicht. Das Bio wird direkt in
 * Resourcen umgewandelt, so dass die verschiedenen Resourcen, die aus Spielersicht
 * in einem Sektor zu sammeln sind, gänzlich voneinander abhängig sind.
 */

class Resource {
	/**
	 * Enthält die Ressourcen, die der User aktuell besitzt
	 *
	 * @var float[self::NUM_RESOURCES]
	 */
	private $resources = array();

	/**
	 * Enthält die Summe der Ressource-Gains [Gebäude beschr. + Gebäude ub. +
	 * 	Forschung]
	 * @var float[self::NUM_RESOURCES]
	 */
	private $gains_sum = array();

	/**
	 * Enthält die Ressource-Gains nach Art aufgeteilt [Gebäude mit beschränkung,
	 * Gebäude ohne Beschränkung (Farm), Forschungsbonus]
	 *
	 * @var float[self::NUM_RESOURCES][100][3]
	 */
	private $gains_split = array();

	/**
	 * User-ID
	 *
	 * @var uint
	 */
	private $uid;

	const NUM_RESOURCES = 2;
	const RESOURCE1 = 0;
	const RESOURCE2 = 1;

	static private $INSTANCE = array();	// instanzüberwachung. Nur eine Instanz pro UID innerhalb von PV
 										// Konstruktor ist private. Objekt statisch über get_instance($uid)
 										// anfordern! (s.u.)

 	/**
	 * Diese Statische Funtion als Konstruktor benutzen !
	 *
	 * @param uint $uid
	 * @return Resource
	 */
 	public static function get_instance($uid) {
 		if (!isset(self::$INSTANCE[$uid])) {
 			self::$INSTANCE[$uid] = new self($uid);
 		}
 		return self::$INSTANCE[$uid];
 	}

 	private function __construct($uid) {
 		$this->uid = (int)$uid;

 		$response = Server::get_instance(CONF_ENGINE_SERVER_URL)
				->multi_call(
					array(
						'get_resource_gains'=>
										array($this->uid, 'all_array', true),
						'get_resources'		=>	$this->uid
					)
				);
		$this->gains_split = $response[0];
		foreach ($this->gains_split as $resource => $data) {
			foreach ($data as $sector => $amount_array) {
				$this->gains_sum[$resource][$sector] = array_sum($amount_array);
			}
		}
 		$this->resources = $response[1];
 	}
 	/**
 	 * Liefert die gewählte Ressource aus dem resource-Array zurück.
 	 *
 	 * @param int $which
 	 * @throws IllegalArgumentExcetion
 	 */
 	public function get_resource($which=0) {
 		if ($which < 0 || $which >= self::NUM_RESOURCES)
 			throw new IllegalArgumentException('which',
 				'[0,..,'.(self::NUM_RESOURCES-1).']', $which);

		return $this->resources[$which];
 	}

	/**
	 * Gibt zurück, wieviel Resource des übergebenen Typs der User vorrausichtlich
	 * für den übergebnen Sektor pro Stunde bekommen wird. Wird 'all' als Sektor
	 * angegeben, so wird die Summe über alle Sektoren zurückgegeben.
	 * @param uint $which			Welche Resource gewünscht ist
	 * @param [0,1,...99] $sector	Resourcen aus dem Sektor ('all' für alle Sektoren)
	 * @param bool $split			Wenn TRUE wird ein Array
	 * 								zurückgegeben, wobei:
	 *							$return[0]: das vom Sektormax beschränkte Gain
	 *							$return[1]: das unbeschränkte Gain
	 *							$return[2]: das Forschungsgain
	 * 							(Sprich: Jagdhaus vs. Bauernhof vs. Bodenbrüter)
	 *
	 * @todo Summe über alle Sektorresourcen cachen
	 */
	public function get_resource_gain($which=0, $sector='all_sum', $split=false)
	{
		if ($which < 0 || $which >= self::NUM_RESOURCES)
 			throw new IllegalArgumentException('which',
 				'[0,..,'.(self::NUM_RESOURCES-1).']', $which);
 		if ($split) {
 			if (($sector === 'all') || ($sector === 'all_sum'))
				return array_sum($this->gains_split[$which]);
			else if ($sector === 'all_array')
				return $this->gains_split[$which];
			else
				return $this->gains_split[$which][$sector];
 		}
 		else {
	 		if (($sector === 'all') || ($sector === 'all_sum'))
				return array_sum($this->gains_sum[$which]);
			else if ($sector === 'all_array')
				return $this->gains_sum[$which];
			else
				return $this->gains_sum[$which][$sector];
 		}
	}

	/**
	 * Gibt zurück, zu welcher Rate Bio in Resource1 oder in Resource2
	 * umgewandelt wird (abhängig von $which). Die Rate ist ein Faktor, dass
	 * Bio/h*Resource-Rate=Resource/h ergibt.
	 *
	 * @param uint $which
	 * @return ufloat
	 */
	public static function get_bio_to_resource_rate($which = 0) {
		if ($which < 0 || $which >= self::NUM_RESOURCES)
 			throw new ExceptionWithErrormail("Ressource-Error", $this->uid, 'All',
 				"Trying to access invalid ressources",
 				"\$which = $which in Resource::get_resource_modifier");
 		if ($which == 0)
 			return self::RESOURCE1_RATE;
 		else if ($which == 1)
 			return self::RESOURCE2_RATE;
 		else
 			return self::RESOURCE3_RATE;
	}
}
?>