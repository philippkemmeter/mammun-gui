<?php
define ('SC_TYPE_ABILS_START',			3010);
/**
 * Repräsentiert den Technologiebaum eines Users. Enthält alle Forschungen,
 * die der Nutzer bereits erforscht hat und alle Forschungen, die er als nächstes
 * zur Auswahl hat.
 */
class Techtree {
	const REQ_AND = 1;
	const REQ_OR = 2;

	private $uid;
	/**
	 * Erforschbare Forschungen als Liste, gruppiert
	 * @var uint[][]
	 */
	private $researchable_scs;

	/**
	 * Erforschbaren Forschungen als ROW gruppiert:
	 *
	 * $researchable_scs_row[category][sc_id] = $row
	 * @var stdClass[][]
	 */
	private $researchable_scs_row;
	/**
	 * Erforschbare Forschungen, aber in der Form $x[SCID]=(bool)
	 * @var bool[]
	 */
	private $researchable_scs_assoc;
	/**
	 * Redundante Speicherung der Daten der Forschungen, die er aktuell hat.
	 * (Level = cur_level)
	 *
	 * Gruppiert nach Kategorie: $cur_researched_scs[category][sc_id] = $row
	 * @var stdClass[][]
	 */
	private $cur_researched_scs;
	/**
	 * Alle Forschungen, die der User kennt, weil er sie irgendwann mal (nicht
	 * unbedingt auf der aktuellen Karte) erforscht hat.
	 * Assoziativ gespeichert: $x[SCID] = (bool)
	 * @var bool[]
	 */
	private $known_scs_assoc;

	private static $INSTANCES;

	private function __construct($uid) {
		$this->uid = $uid;
		$data = Server::get_instance(CONF_ENGINE_SERVER_URL)
			->get_techtree_data($uid);
		$this->researchable_scs = Converter::stdClassToArray(
			$data->researchable_scs);
		$this->researchable_scs_assoc = Converter::stdClassToArray(
			$data->researchable_scs_assoc);
		$this->cur_researched_scs = Converter::stdClassToArray(
			$data->cur_researched_scs);
		$this->researchable_scs_row = null;	// später

		foreach ($this->cur_researched_scs as $data) {
			foreach ($data as $sc_id => $jnk) {
				$this->known_scs_assoc[$sc_id] = true;
			}
		}
	}

	/**
	 * @return Techtree
	 */
	public static function get_instance($uid) {
		if (!self::$INSTANCES || !self::$INSTANCES[$uid])
			self::$INSTANCES[$uid] = new Techtree($uid);
		return self::$INSTANCES[$uid];
	}

	///// GETTER ////

	/**
	 * Gibt das maximal erreichbare Level einer Forschung zurück.
	 *
	 * @param uint $sc_id
	 * @return uint
	 */
	public static function get_science_max_level($sc_id) {
		if ($sc_id>ITEM_SCIENCE_ID)
			return 1;
		return ScienceConstants::$MAX_LEVELS[$sc_id];
	}

	/**
	 * Gibt die Forschung mit der ID $sc_id und aktuellem Level zurück, also
	 * soweit, wie der User sie aktuell geforscht hat.
	 *
	 * @param int $sc_id		Forschungs-ID
	 * @return stdClass Die Forschung als stdClass oder false im Fehlerfall
	 */
	public function get_science($sc_id) {
		$result = new stdClass();
		foreach ($this->cur_researched_scs as $type => $data) {
			if (isset($data->$sc_id)) {
				foreach ($data->$sc_id as $key => $value)
					$result->$key = $value;
				return $result;
			}
		}
		throw new Exception("Science not known");
	}


	public function get_science_cur_level($sc_id) {
		foreach ($this->cur_researched_scs as $type => $data) {
			if (isset($data->$sc_id))
				return $data->$sc_id->level;
		}
		return 0;
	}

	/**
	 * Gibt alle Werte der Forschung zurück, die der Spieler kennt. Das sind
	 * prinzipiell alle, wenn er die Forschung kennt, außer ggf. ein paar
	 * Vorraussetzungen (bei ODER-Beziehungen), sowie wozu die Forschung
	 * führen kann (bei UND-Beziehungen).
	 *
	 * Wenn <code>$for_output</code> gesetzt ist, dann werden alle Daten zum
	 * Ausgeben präpariert (Namen und Beschreibungen der Forschungen werden
	 * bereitgestellt, sowie sc_id verschlüsselt und in dataX gepackt).
	 *
	 * Wird <code>$include_reqs</code> auf <code>false</code> gesetzt, dann
	 * werden keine Requirements der Forschung und auch keine Nachfolger
	 * generiert, sondern nur die Daten der angegebenen Forschung zurückgegeben.
	 *
	 * @param uint $sc_id
	 * @param bool $for_output
	 * @param bool $include_reqs
	 * @return stdClass
	 */
	public function get_known_science_values($sc_id, $for_output=false,
		$include_reqs=true)
	{
		global $cryptObj;
		if (empty($this->known_scs_assoc[$sc_id])
			&& empty($this->researchable_scs_assoc[$sc_id]))
		{
			return new stdClass();
		}

		/**
		 * TODO Cache
		 */
		if (!$this->researchable_scs_row) {
			$this->researchable_scs_row =
				Server::get_instance(CONF_ENGINE_SERVER_URL)
												->get_researchable_scs_row();
		}
		try {
			$science = $this->get_science($sc_id);
		}
		catch (Exception $e) {
			/**
			 * Forschung noch nicht erfoscht, aus $this->researchable_scs_row
			 * entnehmen.
			 */
			foreach ($this->researchable_scs_row as $cat => $data) {
				if (isset($data->$sc_id)) {
					$science = $data->$sc_id;
					break;
				}
			}
		}
		for ($i = 1; $i <= 3; $i++) {
			if ((!$science->{"need_sc$i"}) || !$include_reqs ||
				empty($this->known_scs_assoc[$science->{"need_sc$i"}]))
			{
				unset($science->{"need_sc$i"});
				unset($science->{"need_sc$i".'_lev'});
			}
			else {
				$row = $this->get_science($science->{"need_sc$i"});
				if ($for_output) {
					$row->dataX = $cryptObj->do_encryption(
						array("sc_id" => $row->sc_id)
					);
					$row->name = ScienceConstants::$SC_NAME[$row->sc_id];
					$row->desc = ScienceConstants::$SC_DESC[$row->sc_id];
					$row->desc_known
						= ScienceConstants::$SC_DESC_KNOWN[$row->sc_id];
					$row->desc_bernard
						= ScienceConstants::$SC_DESC_BERNARD[$row->sc_id];
					unset($row->need_sc1);
					unset($row->need_sc1_lev);
					unset($row->need_sc2);
					unset($row->need_sc2_lev);
					unset($row->need_sc3);
					unset($row->need_sc3_lev);
					unset($row->need_i1);
					unset($row->sc1_2_op);
					unset($row->sc2_3_op);
					unset($row->sc_id);
				}
				$science->{"need_sc$i"} = $row;
			}
		}
		if ($for_output) {
			$science->dataX = $cryptObj->do_encryption(
				array("sc_id" => $science->sc_id)
			);
			$science->name = ScienceConstants::$SC_NAME[$sc_id];
			$science->desc = ScienceConstants::$SC_DESC[$sc_id];
			$science->desc_known = ScienceConstants::$SC_DESC_KNOWN[$sc_id];
			$science->desc_bernard = ScienceConstants::$SC_DESC_BERNARD[$sc_id];
			unset($science->sc_id);
		}
		unset($science->need_i1);	// not used

		$arrays_to_check = array(
			'cur_researched_scs'	=> $this->cur_researched_scs,
			'researchable_scs'		=> $this->researchable_scs_row
		);
		/**
		 * Alle bekannten Forschungen durchgehen und schauen, ob sie diese
		 * Forschung als Vorraussetzung haben und ob der Nutzer sie bereits
		 * Nutzen kann.
		 */
		if ($include_reqs) {
			$science->is_req_of = array();
			foreach ($arrays_to_check as $key => $array) {
				foreach ($array as $cat => $data) {
					foreach ($data as $row) {
						if (isset($row->need_sc1)
							&& ($row->need_sc1 == $sc_id))
						{
							if (($row->sc1_2_op == self::REQ_AND)
								&&
								(empty($this->known_scs_assoc[$row->need_sc2])))
							{
								/**
								 * Forschung nicht hinzufügen, wenn es noch eine
								 * weitere UND-Abhängigkeit gibt, die nicht
								 * erfüllt wurde.
								 */
								continue;
							}
						}
						else if (isset($row->need_sc2)
							&& ($row->need_sc2 == $sc_id))
						{
							if (($row->sc1_2_op == self::REQ_AND) &&
								empty($this->known_scs_assoc[$row->need_sc1]))
							{
								continue;	// s.o.
							}
							if (($row->sc2_3_op == self::REQ_AND) &&
								empty($this->known_scs_assoc[$row->need_sc3]))
							{
								continue;	// s.o.
							}
						}
						else if (isset($row->need_sc3)
							&& ($row->need_sc3 == $sc_id))
						{
							if (($row->sc2_3_op == self::REQ_AND) &&
								empty($this->known_scs_assoc[$row->need_sc2]))
							{
								continue;	// s.o.
							}
						}
						else
							continue;

						$row->known = ($key == 'cur_researched_scs');
						if ($for_output) {
							$row->dataX = $cryptObj->do_encryption(
								array("sc_id" => $row->sc_id)
							);
							$row->name
								= ScienceConstants::$SC_NAME[$row->sc_id];
							$row->desc
								= ScienceConstants::$SC_DESC[$row->sc_id];
							if ($row->known) {
								$row->desc_known = ScienceConstants
												::$SC_DESC_KNOWN[$row->sc_id];
								$row->desc_bernard
									= ScienceConstants
												::$SC_DESC_BERNARD[$row->sc_id];
							}
							unset($row->need_sc1);
							unset($row->need_sc1_lev);
							unset($row->need_sc2);
							unset($row->need_sc2_lev);
							unset($row->need_sc3);
							unset($row->need_sc3_lev);
							unset($row->need_i1);
							unset($row->sc1_2_op);
							unset($row->sc2_3_op);
							unset($row->sc_id);
						}
						$science->is_req_of[] = $row;
					}
				}
			}
		}
		return $science;
	}

	/**
	 * Gibt die erforschbaren Sciences unguppiert als assoziatives Array zurück.
	 * Und zwar in der Form $array[$sc_id] = true, wenn erforschbar. Ansonsten
	 * undefiniert. Zum direkten abfragen, ob eine gewisse Forschung erforschbar
	 * ist, ist diese Darstellung also die erste Wahl, da keine Arraysuche erfolgen
	 * muss.
	 *
	 * @return array($sc_id => true)
	 */
	public function get_researchable_scs_assoc() {
		return $this->researchable_scs_assoc;
	}

	/**
	 * Gibt die erforschbaren Sciences gruppiert nach Typ zurück. Aufgebaut ist
	 * das Array wie folgt:
	 * $array['general'] enthält als array alle sc_ids, der erforschbaren General-Scs
	 * $array['races'] enthält als array alle sc_ids, der erforschbaren Races
	 * $array['abilities'] enthält als array alle sc_ids, der erforschbaren Abilities
	 * $array['buildings'] enthält als array alle sc_ids, der erforschbaren Gebäude
	 * $array['artifacts'] enthält als array alle sc_ids, der erforschbaren Gebäude
	 * Diese Darstellung ist also die erste Wahl, wenn man alle erforschbaren
	 * Forschungen ausgeben will.
	 *
	 * @return array('general'=>int[], 'races'=>int[], 'abilities'=>int[], 'buildings'=>int[], 'artifacts'=>int[])
	 */
	public function get_researchable_scs() {
		return $this->researchable_scs;
	}

	/**
	 * Gibt die Forschungen  zurück, die vom User auf der aktuellen
	 * Karte erforscht wurden und somit zur Verfügung stehen.
	 *
	 * @return array('general'=>stdClass[], 'races'=>stdClass[],
	 * 		'abilities'=>stdClass[], 'buildings'=>stdClass[],
	 * 		'artifacts'=>stdClass[])
	 */
	public function get_cur_researched_scs() {
		return $this->cur_researched_scs;
	}


	/**
	 * Gibt die Anzahl der aktuell vom Spieler erforschten Forschungen zurück.
	 *
	 * @return uint
	 */
	public function get_researched_sciences_count() {
		return $this->sciences_researched;
	}



	/**
	 * Gibt zurück, mit welchen Fähigkeitstypen der gewählte Einheitentyp
	 * ausgestattet werden darf.
	 * @param int $sc_id_of_race		Die Science-ID des gewählten Typs
	 * @return int[]	Welche Fähigkeitstypen wie oft
	 */
	public static function get_allowed_abilities_of_race($sc_id_of_race) {
		if (!in_array($sc_id_of_race, ScienceConstants::$ALL_RACES))
			return false;	// keine Rasse => keine Abilities
		if (isset($_SESSION['Techtree']['race_abilities'][$sc_id_of_race]))
			return $_SESSION['Techtree']['race_abilities'][$sc_id_of_race];

		$result = Server::get_instance(CONF_ENGINE_SERVER_URL)
					->get_allowed_abilities_of_race($sc_id_of_race);
		$_SESSION['Techtree']['race_abilities'][$sc_id_of_race] = $result;
		return $result;
	}
}
?>