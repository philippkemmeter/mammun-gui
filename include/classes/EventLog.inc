<?php
require_once(dirname(__FILE__).'/../shared/classes/IEventLog.inc');
require_once(dirname(__FILE__).'/Tribes.inc');
require_once(dirname(__FILE__).'/buildings/Buildings.inc');
require_once(dirname(__FILE__).'/../shared/classes/ItemConstants.inc');

Language::get_instance()->load_language_file('EventLog');
Language::get_instance()->load_language_file('facebook_publish');

/**
 * Diese Klasse ist verantwortlich für einen Log, der pro Spieler pro Karte die
 * Ereignisse auf der Karte protokolliert (Kämpfe gestartet / beendet,
 * Forschungen beendet usw).
 *
 * @author Philipp Kemmeter
 */
class EventLog implements IEventLog {
	/**
	 * Holt den Log des aktuellen Spielers vom Server von der angegebenen Karte.
	 * Der Log besteht aus einem Array von stdClass-Objekten nach Zeitstempel
	 * absteigend sortiert und folgenden Werten:
	 *
	 * <code>
	 * $o->type			Typ des Events (s. IEventLog-TYPE-Konstanten)
	 * $o->t_stamp		Zeitpunkt des Events (UNIX-Timestamp)
	 * $o->extra		Zustäzliche Informationen (Integer, event-spezifisch)
	 * $o->extra_str	Weitere zusätzliche Information (String, event-spez.)
	 * $o->num			ggf. Häufung eines selben Event zum selben Zeitpunkt
	 * $o->format_date	Formatierte Datumsanzeige des Zeitstempels
	 * $o->str			Stringrepräsentation des Events zur Nutzerausgabe
	 * </code>
	 *
	 * @param uint $world_id
	 * @param uint $map_id
	 * @return stdClass[]
	 */
	public static function get_my_log($world_id, $map_id) {
		$log = Server::get_instance(CONF_ENGINE_SERVER_URL)
			->get_event_log($world_id, $map_id);

		//$log = json_decode('[{"type":"51","t_stamp":"1279832069","extra":"5","extra_str":"-","num":"1"},{"type":"10","t_stamp":"1279831813","extra":"3011","extra_str":"bongo","num":"2"},{"type":"15","t_stamp":"1279830679","extra":"0","extra_str":"Matthias","num":"1"},{"type":"10","t_stamp":"1279829824","extra":"3009","extra_str":"bertel","num":"1"},{"type":"1","t_stamp":"1279817069","extra":"27","extra_str":"","num":"1"},{"type":"1","t_stamp":"1279817025","extra":"50","extra_str":"","num":"1"},{"type":"11","t_stamp":"1279813786","extra":"2","extra_str":"Denker","num":"1"},{"type":"20","t_stamp":"1279811465","extra":"63","extra_str":"7","num":"1"},{"type":"1","t_stamp":"1279809434","extra":"1007","extra_str":"","num":"1"},{"type":"20","t_stamp":"1279806922","extra":"10","extra_str":"2","num":"1"},{"type":"20","t_stamp":"1279806875","extra":"33","extra_str":"2","num":"1"},{"type":"20","t_stamp":"1279806857","extra":"33","extra_str":"2","num":"1"},{"type":"56","t_stamp":"1279806613","extra":"33","extra_str":"","num":"1"},{"type":"10","t_stamp":"1279733834","extra":"3011","extra_str":"yedi yeti","num":"2"},{"type":"10","t_stamp":"1279731852","extra":"3009","extra_str":"hintz","num":"2"},{"type":"51","t_stamp":"1279726448","extra":"10","extra_str":"-","num":"1"},{"type":"1","t_stamp":"1279724236","extra":"54","extra_str":"","num":"1"},{"type":"11","t_stamp":"1279715787","extra":"2","extra_str":"Denker","num":"1"},{"type":"20","t_stamp":"1279709078","extra":"91","extra_str":"2","num":"1"},{"type":"20","t_stamp":"1279709069","extra":"91","extra_str":"2","num":"1"},{"type":"20","t_stamp":"1279708967","extra":"2","extra_str":"2","num":"1"},{"type":"20","t_stamp":"1279708924","extra":"2","extra_str":"2","num":"1"},{"type":"20","t_stamp":"1279708892","extra":"29","extra_str":"2","num":"1"},{"type":"20","t_stamp":"1279708866","extra":"29","extra_str":"2","num":"1"},{"type":"56","t_stamp":"1279708732","extra":"2","extra_str":"","num":"1"},{"type":"10","t_stamp":"1279674448","extra":"3011","extra_str":"B-h\u00f6rnchen","num":"2"},{"type":"10","t_stamp":"1279672462","extra":"3009","extra_str":"C-h\u00f6rnchen","num":"2"},{"type":"51","t_stamp":"1279664915","extra":"29","extra_str":"-","num":"1"},{"type":"11","t_stamp":"1279663112","extra":"2","extra_str":"Denker","num":"1"},{"type":"51","t_stamp":"1279660499","extra":"91","extra_str":"-","num":"1"},{"type":"11","t_stamp":"1279656065","extra":"2","extra_str":"Denker","num":"1"},{"type":"20","t_stamp":"1279656043","extra":"73","extra_str":"2","num":"1"},{"type":"20","t_stamp":"1279655955","extra":"60","extra_str":"2","num":"1"},{"type":"20","t_stamp":"1279655947","extra":"60","extra_str":"2","num":"1"},{"type":"20","t_stamp":"1279655922","extra":"50","extra_str":"2","num":"1"},{"type":"20","t_stamp":"1279655908","extra":"50","extra_str":"2","num":"1"},{"type":"20","t_stamp":"1279655865","extra":"11","extra_str":"2","num":"1"},{"type":"20","t_stamp":"1279655857","extra":"11","extra_str":"2","num":"1"},{"type":"20","t_stamp":"1279655808","extra":"3","extra_str":"2","num":"1"},{"type":"20","t_stamp":"1279655794","extra":"3","extra_str":"2","num":"1"},{"type":"20","t_stamp":"1279655779","extra":"4","extra_str":"2","num":"1"},{"type":"20","t_stamp":"1279655745","extra":"4","extra_str":"2","num":"1"},{"type":"20","t_stamp":"1279655698","extra":"15","extra_str":"2","num":"1"},{"type":"56","t_stamp":"1279655627","extra":"4","extra_str":"","num":"1"},{"type":"56","t_stamp":"1279655627","extra":"3","extra_str":"","num":"1"},{"type":"56","t_stamp":"1279655627","extra":"15","extra_str":"","num":"1"},{"type":"20","t_stamp":"1279649322","extra":"40","extra_str":"2","num":"1"},{"type":"20","t_stamp":"1279649311","extra":"30","extra_str":"2","num":"1"},{"type":"20","t_stamp":"1279649294","extra":"30","extra_str":"2","num":"1"},{"type":"20","t_stamp":"1279649277","extra":"22","extra_str":"2","num":"1"},{"type":"20","t_stamp":"1279649205","extra":"14","extra_str":"2","num":"1"},{"type":"20","t_stamp":"1279649194","extra":"14","extra_str":"2","num":"1"},{"type":"20","t_stamp":"1279649151","extra":"70","extra_str":"2","num":"1"},{"type":"20","t_stamp":"1279649093","extra":"16","extra_str":"2","num":"1"},{"type":"56","t_stamp":"1279648987","extra":"16","extra_str":"","num":"1"},{"type":"56","t_stamp":"1279648987","extra":"14","extra_str":"","num":"1"},{"type":"51","t_stamp":"1279647738","extra":"70","extra_str":"-","num":"1"},{"type":"1","t_stamp":"1279647568","extra":"32","extra_str":"","num":"1"},{"type":"10","t_stamp":"1279642248","extra":"3011","extra_str":"it","num":"2"},{"type":"10","t_stamp":"1279640234","extra":"3009","extra_str":"a","num":"2"},{"type":"11","t_stamp":"1279624203","extra":"2","extra_str":"Denker","num":"1"},{"type":"51","t_stamp":"1279623992","extra":"2","extra_str":"-","num":"1"},{"type":"55","t_stamp":"1279623679","extra":"91","extra_str":"winnie","num":"1"},{"type":"20","t_stamp":"1279617387","extra":"71","extra_str":"2","num":"1"},{"type":"20","t_stamp":"1279617380","extra":"71","extra_str":"2","num":"1"},{"type":"20","t_stamp":"1279617366","extra":"53","extra_str":"2","num":"1"},{"type":"20","t_stamp":"1279617351","extra":"53","extra_str":"2","num":"1"},{"type":"20","t_stamp":"1279617324","extra":"85","extra_str":"2","num":"1"},{"type":"20","t_stamp":"1279617279","extra":"85","extra_str":"2","num":"1"},{"type":"20","t_stamp":"1279617058","extra":"17","extra_str":"2","num":"1"},{"type":"20","t_stamp":"1279617024","extra":"24","extra_str":"2","num":"1"},{"type":"20","t_stamp":"1279616954","extra":"94","extra_str":"2","num":"1"},{"type":"56","t_stamp":"1279616898","extra":"24","extra_str":"","num":"1"},{"type":"10","t_stamp":"1279600262","extra":"3011","extra_str":"zick zack bertl","num":"1"},{"type":"10","t_stamp":"1279598220","extra":"3009","extra_str":"j. daniels","num":"1"},{"type":"11","t_stamp":"1279582167","extra":"2","extra_str":"Denker","num":"1"},{"type":"51","t_stamp":"1279578178","extra":"94","extra_str":"-","num":"1"},{"type":"51","t_stamp":"1279576877","extra":"17","extra_str":"-","num":"1"},{"type":"20","t_stamp":"1279575584","extra":"75","extra_str":"2","num":"1"},{"type":"20","t_stamp":"1279575569","extra":"65","extra_str":"2","num":"1"},{"type":"20","t_stamp":"1279575472","extra":"57","extra_str":"2","num":"1"},{"type":"20","t_stamp":"1279575452","extra":"66","extra_str":"2","num":"1"},{"type":"20","t_stamp":"1279575407","extra":"66","extra_str":"2","num":"1"},{"type":"20","t_stamp":"1279575393","extra":"56","extra_str":"2","num":"1"},{"type":"20","t_stamp":"1279575311","extra":"20","extra_str":"2","num":"1"},{"type":"20","t_stamp":"1279575296","extra":"21","extra_str":"2","num":"1"},{"type":"20","t_stamp":"1279575283","extra":"22","extra_str":"2","num":"1"},{"type":"20","t_stamp":"1279575117","extra":"23","extra_str":"2","num":"1"},{"type":"51","t_stamp":"1279553548","extra":"23","extra_str":"-","num":"1"},{"type":"20","t_stamp":"1279536769","extra":"36","extra_str":"2","num":"1"},{"type":"51","t_stamp":"1279524466","extra":"36","extra_str":"-","num":"1"},{"type":"1","t_stamp":"1279515846","extra":"4001","extra_str":"","num":"1"},{"type":"1","t_stamp":"1279497333","extra":"33","extra_str":"","num":"1"},{"type":"10","t_stamp":"1279491152","extra":"3011","extra_str":"superman","num":"2"},{"type":"11","t_stamp":"1279476038","extra":"2","extra_str":"Denker","num":"1"},{"type":"20","t_stamp":"1279469009","extra":"1","extra_str":"2","num":"1"},{"type":"20","t_stamp":"1279468982","extra":"18","extra_str":"2","num":"1"},{"type":"56","t_stamp":"1279468959","extra":"18","extra_str":"","num":"1"},{"type":"56","t_stamp":"1279468959","extra":"1","extra_str":"","num":"1"},{"type":"11","t_stamp":"1279462229","extra":"2","extra_str":"Denker","num":"1"},{"type":"20","t_stamp":"1279455135","extra":"28","extra_str":"2","num":"1"},{"type":"20","t_stamp":"1279455102","extra":"37","extra_str":"2","num":"1"},{"type":"56","t_stamp":"1279455064","extra":"37","extra_str":"","num":"1"},{"type":"56","t_stamp":"1279455064","extra":"28","extra_str":"","num":"1"},{"type":"1","t_stamp":"1279374566","extra":"49","extra_str":"","num":"1"},{"type":"1","t_stamp":"1279374494","extra":"48","extra_str":"","num":"1"},{"type":"1","t_stamp":"1279312360","extra":"3017","extra_str":"","num":"1"},{"type":"10","t_stamp":"1279293316","extra":"3011","extra_str":"frederick","num":"1"},{"type":"10","t_stamp":"1279293315","extra":"3011","extra_str":"frederick","num":"1"},{"type":"10","t_stamp":"1279290299","extra":"3009","extra_str":"winnie","num":"1"},{"type":"20","t_stamp":"1279271378","extra":"62","extra_str":"2","num":"1"},{"type":"20","t_stamp":"1279271358","extra":"61","extra_str":"2","num":"1"},{"type":"20","t_stamp":"1279271214","extra":"38","extra_str":"2","num":"1"},{"type":"56","t_stamp":"1279271089","extra":"38","extra_str":"","num":"1"},{"type":"10","t_stamp":"1279249836","extra":"3011","extra_str":"xibits small brother","num":"2"},{"type":"10","t_stamp":"1279246872","extra":"3009","extra_str":"john walker","num":"2"},{"type":"11","t_stamp":"1279234922","extra":"2","extra_str":"Denker","num":"1"},{"type":"20","t_stamp":"1279228047","extra":"92","extra_str":"2","num":"1"},{"type":"20","t_stamp":"1279228034","extra":"93","extra_str":"2","num":"1"},{"type":"20","t_stamp":"1279227944","extra":"84","extra_str":"2","num":"1"},{"type":"20","t_stamp":"1279227715","extra":"39","extra_str":"2","num":"1"},{"type":"56","t_stamp":"1279227665","extra":"39","extra_str":"","num":"1"},{"type":"10","t_stamp":"1279209077","extra":"3011","extra_str":"flat eric","num":"1"},{"type":"10","t_stamp":"1279206120","extra":"3009","extra_str":"snoop doggy","num":"1"},{"type":"20","t_stamp":"1279187341","extra":"46","extra_str":"2","num":"1"},{"type":"20","t_stamp":"1279187321","extra":"56","extra_str":"2","num":"1"},{"type":"20","t_stamp":"1279187291","extra":"67","extra_str":"2","num":"1"},{"type":"20","t_stamp":"1279187269","extra":"57","extra_str":"2","num":"1"},{"type":"20","t_stamp":"1279187158","extra":"58","extra_str":"2","num":"1"},{"type":"20","t_stamp":"1279187018","extra":"59","extra_str":"2","num":"1"},{"type":"20","t_stamp":"1279186984","extra":"49","extra_str":"2","num":"1"},{"type":"1","t_stamp":"1279112170","extra":"18","extra_str":"","num":"1"},{"type":"1","t_stamp":"1279063222","extra":"9","extra_str":"","num":"1"},{"type":"55","t_stamp":"1279045051","extra":"10","extra_str":"johnny","num":"1"},{"type":"55","t_stamp":"1278958880","extra":"36","extra_str":"j\u00fcrgi","num":"1"},{"type":"56","t_stamp":"1278953875","extra":"49","extra_str":"","num":"1"},{"type":"1","t_stamp":"1278950443","extra":"1006","extra_str":"","num":"1"},{"type":"20","t_stamp":"1278931693","extra":"11","extra_str":"10","num":"1"},{"type":"20","t_stamp":"1278931669","extra":"59","extra_str":"10","num":"1"},{"type":"56","t_stamp":"1278931595","extra":"11","extra_str":"","num":"1"},{"type":"51","t_stamp":"1278924289","extra":"59","extra_str":"-","num":"1"},{"type":"20","t_stamp":"1278884013","extra":"22","extra_str":"10","num":"1"},{"type":"56","t_stamp":"1278883959","extra":"22","extra_str":"","num":"1"},{"type":"11","t_stamp":"1278870425","extra":"2","extra_str":"Denker","num":"1"},{"type":"1","t_stamp":"1278865118","extra":"25","extra_str":"","num":"1"},{"type":"20","t_stamp":"1278863406","extra":"21","extra_str":"10","num":"1"},{"type":"56","t_stamp":"1278863291","extra":"21","extra_str":"","num":"1"},{"type":"55","t_stamp":"1278854739","extra":"59","extra_str":"Bernd","num":"1"},{"type":"11","t_stamp":"1278848090","extra":"2","extra_str":"Denker","num":"1"},{"type":"20","t_stamp":"1278841168","extra":"20","extra_str":"10","num":"1"},{"type":"20","t_stamp":"1278841114","extra":"67","extra_str":"10","num":"1"},{"type":"56","t_stamp":"1278840250","extra":"20","extra_str":"","num":"1"},{"type":"51","t_stamp":"1278822248","extra":"67","extra_str":"-","num":"1"},{"type":"1","t_stamp":"1278794266","extra":"40","extra_str":"","num":"1"},{"type":"55","t_stamp":"1278770489","extra":"70","extra_str":"j\u00fcrgi","num":"1"},{"type":"11","t_stamp":"1278761155","extra":"2","extra_str":"Denker","num":"1"},{"type":"20","t_stamp":"1278754348","extra":"30","extra_str":"10","num":"1"},{"type":"20","t_stamp":"1278754239","extra":"58","extra_str":"10","num":"1"},{"type":"20","t_stamp":"1278754196","extra":"57","extra_str":"10","num":"1"},{"type":"1","t_stamp":"1278745716","extra":"7","extra_str":"","num":"1"},{"type":"1","t_stamp":"1278731295","extra":"30","extra_str":"","num":"1"},{"type":"1","t_stamp":"1278726534","extra":"3009","extra_str":"","num":"1"},{"type":"1","t_stamp":"1278724311","extra":"10","extra_str":"","num":"1"},{"type":"33","t_stamp":"1278696472","extra":"1","extra_str":"Clubber","num":"1"},{"type":"55","t_stamp":"1278696472","extra":"67","extra_str":"Clubber","num":"1"},{"type":"56","t_stamp":"1278693700","extra":"58","extra_str":"","num":"1"},{"type":"20","t_stamp":"1278670171","extra":"46","extra_str":"10","num":"1"},{"type":"20","t_stamp":"1278670115","extra":"71","extra_str":"10","num":"1"},{"type":"56","t_stamp":"1278670073","extra":"46","extra_str":"","num":"1"},{"type":"56","t_stamp":"1278670073","extra":"71","extra_str":"","num":"1"},{"type":"56","t_stamp":"1278670073","extra":"30","extra_str":"","num":"1"},{"type":"56","t_stamp":"1278670073","extra":"57","extra_str":"","num":"1"},{"type":"1","t_stamp":"1278665234","extra":"31","extra_str":"","num":"1"},{"type":"1","t_stamp":"1278611935","extra":"17","extra_str":"","num":"1"},{"type":"20","t_stamp":"1278590597","extra":"40","extra_str":"10","num":"1"},{"type":"20","t_stamp":"1278590525","extra":"56","extra_str":"10","num":"1"},{"type":"56","t_stamp":"1278590438","extra":"56","extra_str":"","num":"1"},{"type":"51","t_stamp":"1278588252","extra":"40","extra_str":"-","num":"1"},{"type":"1","t_stamp":"1278585162","extra":"24","extra_str":"","num":"1"},{"type":"20","t_stamp":"1278514978","extra":"50","extra_str":"10","num":"1"},{"type":"20","t_stamp":"1278514932","extra":"66","extra_str":"10","num":"1"},{"type":"56","t_stamp":"1278514903","extra":"66","extra_str":"","num":"1"},{"type":"56","t_stamp":"1278514903","extra":"50","extra_str":"","num":"1"},{"type":"20","t_stamp":"1278451413","extra":"65","extra_str":"10","num":"1"},{"type":"20","t_stamp":"1278451360","extra":"60","extra_str":"10","num":"1"},{"type":"56","t_stamp":"1278319061","extra":"60","extra_str":"","num":"1"},{"type":"56","t_stamp":"1278319061","extra":"65","extra_str":"","num":"1"},{"type":"1","t_stamp":"1278181802","extra":"14","extra_str":"","num":"1"},{"type":"1","t_stamp":"1278155893","extra":"8","extra_str":"","num":"1"},{"type":"1","t_stamp":"1278105319","extra":"29","extra_str":"","num":"1"},{"type":"20","t_stamp":"1278053594","extra":"61","extra_str":"10","num":"1"},{"type":"20","t_stamp":"1278053556","extra":"75","extra_str":"10","num":"1"},{"type":"56","t_stamp":"1278053518","extra":"61","extra_str":"","num":"1"},{"type":"51","t_stamp":"1278026894","extra":"75","extra_str":"-","num":"1"},{"type":"20","t_stamp":"1277998300","extra":"62","extra_str":"10","num":"1"},{"type":"20","t_stamp":"1277998283","extra":"53","extra_str":"10","num":"1"},{"type":"56","t_stamp":"1277998141","extra":"62","extra_str":"","num":"1"},{"type":"56","t_stamp":"1277998141","extra":"53","extra_str":"","num":"1"},{"type":"1","t_stamp":"1277984547","extra":"16","extra_str":"","num":"1"},{"type":"20","t_stamp":"1277976793","extra":"63","extra_str":"10","num":"1"},{"type":"1","t_stamp":"1277957680","extra":"3","extra_str":"","num":"1"},{"type":"51","t_stamp":"1277940739","extra":"63","extra_str":"-","num":"1"},{"type":"11","t_stamp":"1277857610","extra":"2","extra_str":"Denker","num":"1"},{"type":"1","t_stamp":"1277837851","extra":"22","extra_str":"","num":"1"},{"type":"10","t_stamp":"1277779122","extra":"3011","extra_str":"unter","num":"1"},{"type":"20","t_stamp":"1277757096","extra":"85","extra_str":"10","num":"1"},{"type":"1","t_stamp":"1277628284","extra":"4006","extra_str":"","num":"1"},{"type":"1","t_stamp":"1277579483","extra":"13","extra_str":"","num":"1"},{"type":"1","t_stamp":"1277459127","extra":"12","extra_str":"","num":"1"},{"type":"10","t_stamp":"1277411206","extra":"3011","extra_str":"johnny","num":"1"},{"type":"20","t_stamp":"1277392156","extra":"92","extra_str":"10","num":"1"},{"type":"56","t_stamp":"1277295933","extra":"92","extra_str":"","num":"1"},{"type":"56","t_stamp":"1277295933","extra":"85","extra_str":"","num":"1"},{"type":"33","t_stamp":"1277265005","extra":"1","extra_str":"Clubber","num":"1"},{"type":"55","t_stamp":"1277265005","extra":"63","extra_str":"Clubber","num":"1"},{"type":"1","t_stamp":"1277253573","extra":"4005","extra_str":"","num":"1"},{"type":"1","t_stamp":"1277180548","extra":"39","extra_str":"","num":"1"},{"type":"10","t_stamp":"1277168522","extra":"3011","extra_str":"j\u00fcrgi","num":"1"},{"type":"1","t_stamp":"1277153759","extra":"46","extra_str":"","num":"1"},{"type":"1","t_stamp":"1277153738","extra":"47","extra_str":"","num":"1"},{"type":"20","t_stamp":"1277151455","extra":"93","extra_str":"10","num":"1"},{"type":"20","t_stamp":"1277151382","extra":"73","extra_str":"10","num":"1"},{"type":"20","t_stamp":"1277151263","extra":"84","extra_str":"10","num":"1"},{"type":"56","t_stamp":"1277151066","extra":"93","extra_str":"","num":"1"},{"type":"10","t_stamp":"1277002966","extra":"3011","extra_str":"Bernd","num":"1"},{"type":"1","t_stamp":"1276989946","extra":"4","extra_str":"","num":"1"},{"type":"1","t_stamp":"1276989910","extra":"41","extra_str":"","num":"1"},{"type":"1","t_stamp":"1276989870","extra":"5","extra_str":"","num":"1"},{"type":"56","t_stamp":"1276985682","extra":"73","extra_str":"","num":"1"},{"type":"56","t_stamp":"1276985682","extra":"84","extra_str":"","num":"1"},{"type":"56","t_stamp":"1276960679","extra":"83","extra_str":"","num":"1"}]');
		return self::convert_event_log($log);
	}

	/**
	 * Diese Funktion konvertiert den Event-Log, dass er in eine
	 * Hier werden die Event-Texte erstellt und die Ausgabe der Events somit
	 * vorbereitet.
	 * Erzeugt die zusätzlichen Attribute "str" und "format_date"
	 *
	 * @param stdClass[] $event_log
	 * @return stdClass[]
	 */
	private static function convert_event_log($event_log) {
		global $cryptObj;
		$current_user = User::get_current_user();
		$n = count($event_log);
		for ($i = 0; $i < $n; $i++) {
			switch ($event_log[$i]->type) {
				case self::TYPE_SC_FINISHED:
					$sc_id = $event_log[$i]->extra;
					$event_log[$i]->str = str_replace(
						array('[A]', '[/A]', '[SCIENCE]'),
						array('<a href="javascript: GameMenu.open_game_menu(); '.
							'UIButtons.toggle_science_window()">',
							'</a>',
							'<a href="javascript: GameMenu.open_game_menu(); '.
							'UIButtons.toggle_window(\'achievements.php?dataX='.
								$cryptObj->do_encryption(array('sc_id'=>$sc_id)).
								'\');">'.
							ScienceConstants::$SC_NAME[$sc_id].
							'</a>'),
						LNG_EV_SCS_RESEARCHED
					);
					$event_log[$i]->publish_str = str_replace(
						array('[A]', '[/A]', '[SCIENCE]'),
						array('', '', ScienceConstants::$SC_NAME[$sc_id]),
						LNG_FB_SHARE_SCS_RESEARCHED_DESC
					);
					break;
				case self::TYPE_TRAINING_FINISHED_SCIENTIST:
					list($name, $pic_base) =
						explode('-', $event_log[$i]->extra_str);
					$event_log[$i]->str = str_replace(
							'[UNIT]',
							'<a href="javascript: GameMenu.open_game_menu(); '.
								'UIButtons.toggle_science_window();">'
								. $name . '</a>',
							LNG_EV_TRAINING_FINISHED
						);
						$event_log[$i]->publish_str = str_replace(
							'[UNIT]',
							$name,
							LNG_FB_SHARE_TRAINING_FINISHED_DESC
						);
					break;
				case self::TYPE_TRAINING_FINISHED:
					list($name, $pic_base) =
						explode('-', $event_log[$i]->extra_str);
					$event_log[$i]->str = str_replace(
						'[UNIT]',
						'<a href="javascript: GameMenu.open_game_menu(); '.
						'MammunUI.get_top_frame().MapDataCtrl.sector_click_('.
							($current_user->get_homesector()%100).');">'
							. $name . '</a>',
						LNG_EV_TRAINING_FINISHED
					);

					$event_log[$i]->publish_str = str_replace(
						'[UNIT]',
						$name,
						LNG_FB_SHARE_TRAINING_FINISHED_DESC
					);
					break;
				case self::TYPE_REPAINTED_HIS_MY_COLORED_BUILDING:
					list($world_id, $map_id, $sector_id, $x, $y, $color_int) =
						explode('-', $event_log[$i]->extra_str);
					$uid = $event_log[$i]->extra;
					$buildings_in_sec = Buildings::get_buildings_in_sector(
						new WorldCoordinates($world_id, $map_id, $sector_id)
					);
					$building = NULL;
					foreach ($buildings_in_sec as $b) {
						$ulc = $b->get_ulc();
						if (($ulc->x == $x) && ($ulc->y == $y)) {
							$building = $b;
							break;
						}
					}
					if (!$building) {
						$event_log[$i] = null;
						break;
					}
					$event_log[$i]->str = str_replace(
						array('[SECTOR]', '[NICK]', '[B_NAME]', '[A]', '[/A]'),
						array(
						 	$sector_id,
							User::get_instance_by_id($uid)->get_nick(),
							Buildings::get_b_name($building->get_type()),
							'<a href="javascript: GameMenu.open_game_menu(); '.
							'MammunUI.get_top_frame().MapDataCtrl.sector_click_('
								.$sector_id.');">',
							'</a>'
						),
						LNG_EV_REPAINTED_HIS_MY_COLORED_BUILDING
					);

					$event_log[$i]->publish_str = str_replace(
						array('[SECTOR]', '[NICK]', '[B_NAME]', '[A]', '[/A]'),
						array(
						 	$sector_id,
							User::get_instance_by_id($uid)->get_nick(),
							Buildings::get_b_name($building->get_type()),
							'<a href="javascript: GameMenu.open_game_menu(); '.
							'MammunUI.get_top_frame().MapDataCtrl.sector_click_('
								.$sector_id.');">',
							'</a>'
						),
						LNG_FB_SHARE_REPAINTED_HIS_MY_COLORED_BUILDING_DESC
					);
					break;
				case self::TYPE_BUILDING_HAS_BEEN_PAINTED:
					list($world_id, $map_id, $sector_id, $x, $y, $color_int) =
						explode('-', $event_log[$i]->extra_str);
					$uid = $event_log[$i]->extra;
					$buildings_in_sec = Buildings::get_buildings_in_sector(
						new WorldCoordinates($world_id, $map_id, $sector_id)
					);
					$building = NULL;
					foreach ($buildings_in_sec as $b) {
						$ulc = $b->get_ulc();
						if (($ulc->x == $x) && ($ulc->y == $y)) {
							$building = $b;
							break;
						}
					}
					if (!$building) {
						$event_log[$i] = null;
						break;
					}
					$event_log[$i]->str = str_replace(
						array('[SECTOR]', '[NICK]', '[B_NAME]', '[A]', '[/A]'),
						array(
						 	$sector_id,
							User::get_instance_by_id($uid)->get_nick(),
							Buildings::get_b_name($building->get_type()),
							'<a href="javascript: GameMenu.open_game_menu(); '.
							'MammunUI.get_top_frame().MapDataCtrl.sector_click_('
								.$sector_id.');">',
							'</a>'
						),
						LNG_EV_BUILDING_HAS_BEEN_PAINTED
					);

					$event_log[$i]->publish_str = str_replace(
						array('[SECTOR]', '[NICK]', '[B_NAME]', '[A]', '[/A]'),
						array(
						 	$sector_id,
							User::get_instance_by_id($uid)->get_nick(),
							Buildings::get_b_name($building->get_type()),
							'<a href="javascript: GameMenu.open_game_menu(); '.
							'MammunUI.get_top_frame().MapDataCtrl.sector_click_('
								.$sector_id.');">',
							'</a>'
						),
						LNG_FB_SHARE_BUILDING_HAS_BEEN_PAINTED_DESC
					);
					break;
				case self::TYPE_MAP_INVITATION_RECEIVED:
					$map_id = ($event_log[$i]->extra > 0)
						? $event_log[$i]->extra
						: User::get_instance_by_nick($event_log[$i]->extra_str)
							->get_map_id();

					list($nick, $world_id)
						= explode('-', $event_log[$i]->extra_str);

					if (!$world_id)
						$world_id = floor($map_id/100);
					$map_id = $map_id%100;

					$event_log[$i]->str = str_replace(
						array('[NICK]', '[A]', '[/A]'),
						array(
							$nick,
							'<a href="javascript: MapUI.change_map('.
									"$world_id, $map_id".
								');">',
							'</a>'
						),
						LNG_EV_MAP_INVITATION_RECEIVED
					);
					$event_log[$i]->publish_str = str_replace(
						array('[NICK]', '[A]', '[/A]'),
						array(
							$nick,
							'',
							''
							),
						LNG_FB_SHARE_MAP_INVITATION_RECEIVED_DESC
					);
					break;
				case self::TYPE_SECTOR_BATTLE_WON:
					$sector_id = $event_log[$i]->extra;
					$enemy_nick = $event_log[$i]->extra_str;
					$event_log[$i]->str = str_replace(
						array('[A]', '[/A]', '[SECTOR]', '[ENEMY_NICK]'),
						array('<a href="javascript: GameMenu.open_game_menu(); '.
							'MammunUI.get_top_frame().MapDataCtrl.sector_click_('
								.$sector_id.');">',
							'</a>', $sector_id,
							'<a href="javascript: UIButtons.open_window('.
							"'profile.php?nick=$enemy_nick', '', 110);\">".
							User::beautify_nick($enemy_nick)
							."</a>"),
						LNG_EV_SECTOR_BATTLE_WON
					);
					$event_log[$i]->publish_str = str_replace(
						array('[A]', '[/A]', '[SECTOR]', '[ENEMY_NICK]'),
						array('','', $sector_id, User::beautify_nick($enemy_nick)),
						LNG_FB_SHARE_SECTOR_BATTLE_WON_DESC
					);
					break;
				case IEventLog::TYPE_SECTOR_BATTLE_LOST:
					$sector_id = $event_log[$i]->extra;
					$enemy_nick = $event_log[$i]->extra_str;
					$event_log[$i]->str = str_replace(
						array('[A]', '[/A]', '[SECTOR]', '[ENEMY_NICK]'),
						array('<a href="javascript: GameMenu.open_game_menu(); '.
							'MammunUI.get_top_frame().MapDataCtrl.sector_click_('
								.$sector_id.');">',
							'</a>', $sector_id,
							'<a href="javascript: UIButtons.open_window('.
							"'profile.php?nick=$enemy_nick', '', 110);\">".
							User::beautify_nick($enemy_nick).
							"</a>"),
						LNG_EV_SECTOR_BATTLE_LOST
					);
					$event_log[$i]->publish_str = str_replace(
						array('[A]', '[/A]', '[SECTOR]', '[ENEMY_NICK]'),
						array('', '', $sector_id, User::beautify_nick($enemy_nick)),
						LNG_FB_SHARE_SECTOR_BATTLE_LOST_DESC
					);
					break;
				case self::TYPE_SECTOR_WON_NO_BATTLE:
					$sector_id = $event_log[$i]->extra;
					$event_log[$i]->str = str_replace(
						array('[A]', '[/A]', '[SECTOR]'),
						array('<a href="javascript: GameMenu.open_game_menu(); '.
							'MammunUI.get_top_frame().MapDataCtrl.sector_click_('.$sector_id.');">',
							'</a>', $sector_id),
						LNG_EV_SECTOR_WON_NO_BATTLE
					);
					$event_log[$i]->publish_str = str_replace(
						array('[A]', '[/A]', '[SECTOR]'),
						array('', '', $sector_id),
						LNG_FB_SHARE_SECTOR_WON_NO_BATTLE_DESC
					);
					break;
				case self::TYPE_TRIBE_KILLED:
					$sector_id = $event_log[$i]->extra;
					$name = $event_log[$i]->extra_str;
					$event_log[$i]->str = str_replace(
						array('[A]', '[/A]', '[SECTOR]', '[TRIBE_NAME]'),
						array('<a href="javascript: GameMenu.open_game_menu(); '.
							'MammunUI.get_top_frame().MapDataCtrl.sector_click_('
								.$sector_id.');">',
							'</a>', $sector_id, $name),
						LNG_EV_TRIBE_KILLED
					);
					$event_log[$i]->publish_str = str_replace(
						array('[A]', '[/A]', '[SECTOR]', '[TRIBE_NAME]'),
						array('', '', $sector_id, $name),
						LNG_FB_SHARE_TRIBE_KILLED_DESC
					);
					break;
				case self::TYPE_TRIBE_DIED:
					$sector_id = $event_log[$i]->extra;
					$name = $event_log[$i]->extra_str;
					$event_log[$i]->str = str_replace(
						array('[A]', '[/A]', '[SECTOR]', '[TRIBE_NAME]'),
						array('<a href="javascript: GameMenu.open_game_menu(); '.
							'MammunUI.get_top_frame().MapDataCtrl.sector_click_('
								.$sector_id.');">',
							'</a>', $sector_id, $name),
						LNG_EV_TRIBE_DIED
					);
					$event_log[$i]->publish_str = str_replace(
						array('[A]', '[/A]', '[SECTOR]', '[TRIBE_NAME]'),
						array('', '', $sector_id, $name),
						LNG_FB_SHARE_TRIBE_DIED_DESC
					);
					break;
				case self::TYPE_CONSTRUCTION_COMPLETED:
					$sector_id = $event_log[$i]->extra;
					$type_str =
						Buildings::get_b_name((int)$event_log[$i]->extra_str);
					$event_log[$i]->str = str_replace(
						array('[A]', '[/A]', '[SECTOR]', '[TYP]'),
						array('<a href="javascript: GameMenu.open_game_menu(); '.
							'MammunUI.get_top_frame().MapDataCtrl.sector_click_('
								.$sector_id.');">',
							'</a>', $sector_id, $type_str),
						LNG_EV_CONSTRUCTION_COMPLETE
					);
					$event_log[$i]->publish_str = str_replace(
						array('[A]', '[/A]', '[SECTOR]', '[TYP]'),
						array('', '', $sector_id, $type_str),
						LNG_FB_SHARE_CONSTRUCTION_COMPLETE_DESC
					);
					break;
				case self::TYPE_CLOTHES_OF_UNIT_GAINED:
					$item_amount = $event_log[$i]->extra;
					$tribe_name = $event_log[$i]->extra_str;
					$event_log[$i]->str = str_replace(
						array('[A]', '[/A]', '[AMOUNT]', '[TRIBE_NAME]'),
						array('<a href="javascript: '.
								'UIButtons.toggle_treasure_chest();">',
							'</a>', $item_amount, $tribe_name),
						LNG_EV_CLOTHES_OF_UNIT_GAINED
					);
					$event_log[$i]->publish_str = str_replace(
						array('[A]', '[/A]', '[AMOUNT]', '[TRIBE_NAME]'),
						array('', '', $item_amount, $tribe_name),
						LNG_FB_SHARE_CLOTHES_OF_UNIT_GAINED_DESC
					);
					break;
				case self::TYPE_GIFT_RECEIVED:
					$from_nick = User::get_instance_by_id($event_log[$i]->extra)
						->get_nick();
					list($type, $id) = explode('-', $event_log[$i]->extra_str);
					$event_log[$i]->str = str_replace(
						array('[NICK]', '[ITEM]'),
						array('<a href="javascript: UIButtons.open_window('.
							"'profile.php?nick=$from_nick', '', 110);\">".
							User::beautify_nick($from_nick).
							"</a>",
							'<a href="javascript: '.
								'UIButtons.toggle_treasure_chest();">'.
								ItemConstants::$ITEM_NAMES[$type][$id]
							.'</a>'
						),
						LNG_EV_GIFT_RECEIVED
					);
					$event_log[$i]->publish_str = str_replace(
						array('[NICK]', '[ITEM]'),
						array(User::beautify_nick($from_nick), ItemConstants::$ITEM_NAMES[$type][$id]
						),
						LNG_FB_SHARE_GIFT_RECEIVED_DESC
					);
					break;
				case self::TYPE_ITEM_SOLD_DIRECTLY:
					$to_nick = User::get_instance_by_id($event_log[$i]->extra)
						->get_nick();
					list($type, $id, $price)
						= explode('-', $event_log[$i]->extra_str);
					$event_log[$i]->str = str_replace(
						array('[NICK]', '[ITEM]', '[KKR]'),
						array('<a href="javascript: UIButtons.open_window('.
							"'profile.php?nick=$to_nick', '', 110);\">".
							User::beautify_nick($to_nick).
							"</a>",
							'<a href="javascript: '.
								'UIButtons.toggle_treasure_chest();">'.
								ItemConstants::$ITEM_NAMES[$type][$id]
							.'</a>',
							$price
						),
						LNG_EV_ITEM_SOLD
					);
					$event_log[$i]->publish_str = str_replace(
						array('[NICK]', '[ITEM]', '[KKR]'),
						array(User::beautify_nick($to_nick), ItemConstants::$ITEM_NAMES[$type][$id], $price
						),
						LNG_FB_SHARE_ITEM_SOLD_DESC
					);
					break;
				case self::TYPE_TRADE_ACCEPTED:
					$nick = User::get_instance_by_id($event_log[$i]->extra)
						->get_nick();
					$event_log[$i]->str = str_replace(
						array('[NICK]', '[A]', '[/A]'),
						array('<a href="javascript: UIButtons.open_window('.
							"'profile.php?nick=$nick', '', 110);\">".
							User::beautify_nick($nick).
							"</a>",
							'<a href="javascript: '.
								'UIButtons.toggle_im_window();">',
							'</a>'
						),
						LNG_EV_OFFER_ACCEPTED
					);
					$event_log[$i]->publish_str = str_replace(
						array('[NICK]', '[A]', '[/A]'),
						array(User::beautify_nick($nick), '', ''),
						LNG_FB_SHARE_OFFER_ACCEPTED_DESC
					);
					break;
				case self::TYPE_TRADE_DENIED:
					$nick = User::get_instance_by_id($event_log[$i]->extra)
						->get_nick();
					$event_log[$i]->str = str_replace(
						array('[NICK]', '[A]', '[/A]'),
						array('<a href="javascript: UIButtons.open_window('.
							"'profile.php?nick=$nick', '', 110);\">".
							User::beautify_nick($nick).
							"</a>",
							'<a href="javascript: '.
								'UIButtons.toggle_im_window();">',
							'</a>'
						),
						LNG_EV_OFFER_DENIED
					);
					$event_log[$i]->publish_str = str_replace(
						array('[NICK]', '[A]', '[/A]'),
						array(User::beautify_nick($nick), '', ''),
						LNG_FB_SHARE_OFFER_DENIED_DESC
					);
					break;
				default:	// not supported
					$event_log[$i] = null;

			}
			// Nur, wenn dieses Event überhaupt ausgegeben werden soll.
			if ($event_log[$i]) {
				$event_log[$i]->format_date
						= date("d.m.Y, H:i", $event_log[$i]->t_stamp);
				$event_log[$i]->t_stamp = (int)$event_log[$i]->t_stamp;
			}
		}
		return $event_log;
	}
}
