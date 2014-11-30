<?php
/**
 * File created by Philipp Kemmeter on 11.02.2009 at 10:47:10
 */
 
/**
 * Verwaltet die Notizen des Spielers
 *
 * @author Philipp Kemmeter
 */
class Notes {
	private $uid;
	private $note;
	private $t_stamp;
	private $data_changed = false;
	
	private static $INSTANCES = array();
	
	/** 
	 * Gibt die Instanz zurück, die der UID zugeordnet ist (singleton)
	 * 
	 * @param uint $uid
	 * @return Notes
	 */
	public static function get_instance($uid) {
		if (!isset(self::$INSTANCES[$uid])) {
			self::$INSTANCES[$uid] = new Notes($uid);
		}
		return self::$INSTANCES[$uid];
	}
	
	/**
	 * 
	 * @param uint $uid
	 * @return Notes
	 */
	private function __construct($uid) {
		$this->uid = $uid;
		if (isset($_SESSION['Notes'][$uid])) {
			$this->note = $_SESSION['Notes'][$uid]['note'];
			$this->t_stamp = $_SESSION['Notes'][$uid]['t_stamp'];
		}
		else {
			$data = Server::get_instance(CONF_ENGINE_SERVER_URL)
				->get_notes_data($uid);
			if (is_object($data)) {
				$this->note = $data->note;
				$this->t_stamp = $data->t_stamp;
			}
			else {
				throw new Exception($data);
			}
		}
	}

	
	/**
	 * Setzt die Notiz des Spielers auf den entsprechenden Text
	 * 
	 * @param string $note
	 */
	public function set_note($note) {
		$data = Server::get_instance(CONF_ENGINE_SERVER_URL)
			->cmd_set_notes_data($this->uid, $note);
		/**
		 * Es kann passieren, dass nach dem Speichern die Daten aus welchen
		 * Gründen auch immer, nicht so gespeichert wurden, als gewünscht.
		 * Gerade der t_stamp kann nicht vorgegeben werden und wird
		 * höchstwahrscheinlich bis zu ein paar Sekunden abweichen.
		 * Daher:
		 */
		if (is_object($data)) {
			$this->note = $data->note;
			$this->t_stamp = $data->t_stamp;
		}
		else {
			throw new Exception($data);
		}
		$_SESSION['Notes'][$this->uid]['note'] = $this->note;
		$_SESSION['Notes'][$this->uid]['t_stamp'] = $this->t_stamp;
	}
	
	/**
	 * Gibt die Notiz des Spielers zurück
	 * 
	 * @return string
	 */
	public function get_note() {
		return $this->note;
	}
	
	/**
	 * Gibt zurück, wann die Notiz das letzte mal geändert wurde.
	 * 
	 * @return uint
	 */
	public function get_last_modified_time() {
		return $this->t_stamp;
	}
}
?>