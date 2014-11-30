<?php
require_once(dirname(__FILE__).'/../shared/classes/IBugreports.inc');
/**
 * File created by Philipp Kemmeter on 07.02.2009 at 13:24:35
 */

/**
 * Behandelt Bug-Reports (Wrapper für Server-Funktion)
 * 
 * @author Philipp Kemmeter
 */

class Bugreport implements IBugreports {
	private function __construct() {}
	
	/**
	 * Fügt einen neuen Fehlerbericht in die Datenbank ein und sendet den
	 * Verantwortlichen eine E-Mail
	 * 
	 * @param uint $user_id
	 * @param string $browser
	 * @param string $os
	 * @param string $resolution
	 * @param string $window_size
	 * @param uint $bug_source
	 * @param string $repro_desc
	 * @param string $bug_comment
	 */
	static public function insert_new_report($user_id, $browser, $os, 
		$resolution, $window_size, $bug_source, $repro_desc, $bug_comment)
	{
		$bug_id = Server::get_instance(CONF_ENGINE_SERVER_URL)
			->cmd_report_bug($user_id, $browser, $os, $resolution, $window_size,
				$bug_source, $repro_desc, $bug_comment);

		if (isset($_SESSION['Bugreports']['attachments'])) {
			foreach ($_SESSION['Bugreports']['attachments'] as $attach) {
				Server::get_instance(CONF_ENGINE_SERVER_URL)
					->cmd_add_bug_attachment($bug_id, $attach->data, 
							$attach->name);				
			}
		}
		unset($_SESSION['Bugreports']['attachments']);
		return 1;
	}
	
	/**
	 * Gibt alle Bugs aus der Datenbank als ein Array von stdClass-Objekten
	 * aufsteigend sortiert nach Datum zurück 
	 * (also ein direktes Abbild der bugreports-DB) 
	 * Der Parameter $limit bezeichnet den LIMIT-Parameter in Mysql (Beispiel:
	 * '4' oder '10, 20')
	 * Wenn $open_bugs_only gesetzt wird, werden nur die Bugs zurückgegeben,
	 * die weder gefixt, noch gerade in Bearbeitung sind.
	 * 
	 * @param string $limit
	 * @param bool $open_bugs_only
	 * @return stdClass[]
	 */
	static public function get_reports($limit=false, $open_bugs_only=false) 
	{
		return Server::get_instance(CONF_ENGINE_SERVER_URL)
			->get_bug_reports($limit, $open_bugs_only);
	}
	
	/**
	 * Verbindet zwei Bugs, indem der Sub-Bug dem Main-Bug untergeordnet wird 
	 * (z.B. weil der Sub-Bug dasselbe Beschreibt wie der Main-Bug)
	 * @param uint $main_bug_id
	 * @param uint $sub_bug_id
	 */
	static public function join_bugs($main_bug_id, $sub_bug_id) {
		return Server::get_instance(CONF_ENGINE_SERVER_URL)
			->cmd_join_bugs($main_bug_id, $sub_bug_id);
	}
	
	/**
	 * Gibt den Bugreport zurück, den die ID $id angibt.
	 * 
	 * @param uint $id
	 * @return stdClass
	 */
	static public function get_report($id) {
		return Server::get_instance(CONF_ENGINE_SERVER_URL)
			->get_bug_report($id);
	}

	/**
	 * Setzt den Status des Bugs mit der angegebenen ID auf $state. $comment
	 * kann dabei angegeben werden.
	 * 
	 * @param uint $id
	 * @param uint $state
	 * @param string $comment
	 * @throws HackattackException wenn User das nicht darf
	 */
	static public function set_state($id, $state, $comment='') {
		Server::get_instance(CONF_ENGINE_SERVER_URL)
			->cmd_set_bug_state($id, $state, $comment);
	}
	
	/**
	 * Setzt die Priorität des Bugs mit der angegebenen ID entsprechend.
	 * 
	 * @param uint $id
	 * @param ubyte $priority
	 * @throws HackattackException wenn User das nicht darf
	 */
	static public function set_priority($id, $priority) {
		Server::get_instance(CONF_ENGINE_SERVER_URL)
			->cmd_set_bug_priority($id, $priority);
	}
	
	/**
	 * Setzt die Kategorie-ID, der der Bug mit der angegebenen ID angehören 
	 * soll.
	 * 
	 * @param uint $id
	 * @param ubyte $cat_id
	 * @throws HackattackException wenn User das nicht darf
	 */
	static public function set_cat_id($id, $cat_id) {
		Server::get_instance(CONF_ENGINE_SERVER_URL)
			->cmd_set_bug_cat_id($id, $cat_id);
	}
	
	/**
	 * Fügt dem Bug mit der angegebenen ID ein Attachment hinzu. Wird als ID 0
	 * übergeben, werden die Daten gepuffert und dem nächsten Bug hinzugefügt,
	 * der mit der aktuellen Session berichtet wird.
	 * Gibt zurück, ob das Attachment hinzugefügt werden konnte. Bei gleichem 
	 * Dateinamen und ID=0 schlägt dies fehl.
	 * 
	 * @param uint $id
	 * @param string $data
	 * @param string $filename
	 * @param bool
	 */
	static public function add_attachment($id, $data, $filename) {
		if ($id) {
			Server::get_instance(CONF_ENGINE_SERVER_URL)
				->cmd_add_bug_attachment($id, $data);
		}
		else {
			if (!isset($_SESSION['Bugreports']['attachments']))
				$_SESSION['Bugreports']['attachments'] = array();
			foreach ($_SESSION['Bugreports']['attachments'] as $attach)
				if ($attach->name == $filename)
					return false;
					
			$a = new stdClass();
			$a->name = $filename;
			$a->data = $data;
			$_SESSION['Bugreports']['attachments'][] = $a;
		}
		return true;
	}
	
	/**
	 * Entfernt eines der aktuellen Attachments. Gibt zurück, ob das zu 
	 * löschende Attachment überhaupt existierte.
	 * 
	 * @param string name
	 * @return bool
	 */
	static public function remove_current_attachment($name) {
		foreach ($_SESSION['Bugreports']['attachments'] as $key => $attach) {
			if ($attach->name == $name) {
				unset($_SESSION['Bugreports']['attachments'][$key]);
				return true;
			}
		}
		return false;
	}
	
	/**
	 * Gibt die Attachments zurück, die gepuffert sind, um sie dem nächsten 
	 * Bugreport direkt anzufügen.
	 * 
	 * @return stdClass[]
	 */
	static public function get_current_attachments() {
		return isset($_SESSION['Bugreports']['attachments']) ?
			$_SESSION['Bugreports']['attachments'] : array();
	}
	
	/**
	 * Gibt das angegebene Attachment des angegebenen Bugs zurück. Wird keine
	 * $attach_id angegeben, werden alle Attachments des Bugs zurückgeliefert.
	 * 
	 * Form ist dabei jeweils ein stdObjekt, welches in name den Dateinamen und
	 * in data den Inhalt beinhaltet.
	 * 
	 * @param uint $bug_id
	 * @param uint $attach_id
	 * @return stdClass / stdClass[]
	 */
	static public function get_attachment($bug_id, $attach_id) {
		$param = urlencode(serialize(array($bug_id, $attach_id)));
		return ClientServer::send_request(CONF_ENGINE_SERVER_URL.
			'?cmd_get_bug_attachment='.$param,true);
//		return Server::get_instance(CONF_ENGINE_SERVER_URL)
//			->cmd_get_bug_attachment($bug_id, $attach_id);
	}
}
 
?>