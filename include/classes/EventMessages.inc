<?php
require_once(dirname(__FILE__).'/../shared/classes/IEventMessages.inc');
require_once(dirname(__FILE__).'/../shared/classes/buildings/IBuildings.inc');
require_once(dirname(__FILE__).'/../shared/classes/ILevelUpPresents.inc');
require_once(dirname(__FILE__).'/../shared/classes/ScienceConstants.inc');
require_once(dirname(__FILE__).'/Medals.inc');
require_once(dirname(__FILE__).'/Avatar.inc');

Language::get_instance()->load_language_file('EventMessages');
Language::get_instance()->load_language_file('medals');
Language::get_instance()->load_language_file('facebook_publish');

/**
 * Diese Klasse ist verantwortlich für die aufpoppenden Ereignisnachrichten
 * pro Spieler.
 *
 * @author Philipp Kemmeter
 */
class EventMessages implements IEventMessages {
	/**
	 * Holt den Log des aktuellen Spielers vom Server von der angegebenen Karte.
	 * Der Log besteht aus einem Array von stdClass-Objekten nach Zeitstempel
	 * absteigend sortiert und folgenden Werten:
	 *
	 * <code>
	 * $o->type			Typ des Events (s. IEventLog-TYPE-Konstanten)
	 * $o->t_stamp		Zeitpunkt des Events (UNIX-Timestamp)
	 * $o->num			Spezifische ID des Nachricht
	 * $o->str			Stringrepräsentation des Events zur Nutzerausgabe
	 * $o->pic			URL zum Bild, das zum Text angezeigt werden soll
	 * $o->score		Wie viel Punkte das Ereignis bringt (wird beim Löschen
	 * 					der Nachricht gutgeschrieben)
	 * </code>
	 *
	 * @return stdClass[]
	 */
	public static function get_my_log() {
		$messages = Server::get_instance(CONF_ENGINE_SERVER_URL)
			->get_event_messages(false);
		return self::convert_event_messages($messages);
	}

	/**
	 * Fügt einen Eintrag in den Log ein. Wird $t_stamp nicht gesetzt, wird
	 * JETZT als Zeitpunkt gewählt. Ansonsten können frei Zeitpunkte (auch
	 * vergangene oder zukünftige) gewählt werden.
	 *
	 * @param uint $type
	 * @param uint $extra
	 * @param string $extra_str
	 * @param uint $t_stamp
	 * @param uint $score
	 * @param uint $medal_id
	 */
	public static function add_entry($type, $extra=0, $extra_str='', $t_stamp=0,
		$score=0, $medal_id=0)
	{
		Server::get_instance(CONF_ENGINE_SERVER_URL)
			->add_event_message($type, $extra, $extra_str, $t_stamp, $score,
				$medal_id);
	}

	/**
	 * Löscht die Nachricht mit der Angegebenen Nummer.
	 *
	 * @param uint $num
	 * @return float[]	Neue Punktzahlen (Liga; Map) des Spielers
	 */
	public static function delete_message($num) {
		return Server::get_instance(CONF_ENGINE_SERVER_URL)
					->delete_event_message($num);
	}

	private static function medal_id_to_pic_url_array($a) {
		$a = my_array_flatten($a);
		foreach ($a as $k => &$v) {
			$v = array(
				'pic_url' => pic_path("ui/icons/labels/medals/50x54/$v.png"),
				'tooltip' => constant("LNG_MEDAL_{$v}_TOOLTIP"),
				'deactivated' => true
			);
		}
		return $a;
	}

	/**
	 * Diese Funktion konvertiert den Event-Log, dass er in eine
	 * Hier werden die Event-Texte erstellt und die Ausgabe der Events somit
	 * vorbereitet.
	 * Erzeugt die zusätzlichen Attribute "str", "pic", "pic_width" und
	 * "pic_height"
	 *
	 * @param stdClass[] $messages
	 * @return stdClass[]
	 */
	private static function convert_event_messages($messages) {
		$current_user = User::get_current_user();
		$n = count($messages);
		$indizes_to_delete = array();
		for ($i = 0; $i < $n; $i++) {
			$messages[$i]->type = (int)$messages[$i]->type;
			switch ($messages[$i]->type) {
				case self::TYPE_FOUND_EASTER_EGG_IN_DARK:
					$messages[$i]->str = LNG_EVM_EASTER_EGGS_IN_THE_DARK;
					$messages[$i]->pic = array (
						'pic_url' => pic_path(
						"ui/special_situations/event_messages/easter_eggs.png"
						),
						'width'		=> 234,
						'height'	=> 150,
						'tooltip'	=> ''
					);
					/* TODO
					$messages[$i]->publish_str
						= LNG_FB_SHARE_EASTER_EGGS_IN_THE_DARK;
						*/
					break;
				case self::TYPE_FIRST_MAP_INVIT_FAILED:
					$messages[$i]->str = str_replace(
							'[NICK]',
							'<b>'.User::get_instance_by_id($messages[$i]->extra)
								->get_nick_main().'</b>',
							LNG_EVM_FIRST_MAP_INVIT_FAILED
						);
					$nick = User::get_instance_by_id($messages[$i]->extra)
								->get_nick();
					$av = Avatar::get_avatars(array($nick));
					$avatar = str_replace('SIZE',150, $av->$nick);
					$messages[$i]->pic = array(
						'pic_url' => $avatar,
						'width' => 150,
						'height' => 150,
						'tooltip' => $nick,
						'pos'	=> 0
					);
					$messages[$i]->width = 530;
					$messages[$i]->height = 321;
					break;
				case self::TYPE_VISIT_MAP_FAILED:
					$messages[$i]->str = str_replace(
						"[NICK]",
						$messages[$i]->extra_str,
						LNG_EVM_VISIT_MAP_FAILED
					);
					$messages[$i]->on_yes = "function(ev) { ".
						"MapDataCtrl.change_map(".
							User::get_current_user()->get_world_id().', '.
							(User::get_current_user()->get_map_id()).
							", MapUI.change_map_callback".
						"); ".
					"}";
					$messages[$i]->on_no = "function(ev){}";
					break;
				case self::TYPE_ALL_ENEMIES_DEFEATED:
					$messages[$i]->str = LNG_EVM_ALL_ENEMIES_DEFEATED;
					$messages[$i]->on_yes = "function(ev) { ".
						"PauseMenu.leave_map(); }";
					$messages[$i]->on_no = "function(ev){}";
					/*$messages[$i]->publish_str =
						LNG_FB_SHARE_ALL_ENEMIES_DEFEATED;*/
					break;
				case self::TYPE_SC_FINISHED:
					$sc_id = $messages[$i]->extra;
					require_once(dirname(__FILE__).'/Techtree.inc');

					Language::get_instance()->load_language_file(
						'achievements');

					$techtree =	Techtree::get_instance(
						User::get_current_user()->get_id()
					);
					$science = $techtree->get_known_science_values($sc_id,true);
					$m = isset($science->is_req_of) ?
						count($science->is_req_of) : 0;

					if ($messages[$i]->medal_id) {
						if ($m > 0) {
							$known_sciences_str =
								"„{$science->is_req_of[0]->name}“";
							for ($j = 1; $j < $m; $j++) {
								$known_sciences_str .=
										", „{$science->is_req_of[$j]->name}“";
							}
						}
						else
							$known_sciences_str = '';
						$messages[$i]->str = str_replace(
							array('[SCIENCE]', '[NEW_SCIENCES]'),
							array(
								ScienceConstants::$SC_NAME[$sc_id],
								$known_sciences_str
							),
							constant(
								'LNG_MEDAL_'.$messages[$i]->medal_id.'_TEXT'
							)
						);
						/*$m1 = Medals::get_medals(
							IMedals::TYPE_SCIENCE_RESEARCHED_CATEGORY
						);
						$m2 = Medals::get_medals(
							IMedals::TYPE_SCIENCE_RESEARCHED_LEVEL
						);
						$messages[$i]->additional_pics =
							self::medal_id_to_pic_url_array(
								array_merge($m1, $m2)
						);*/
					}
					else {
						if ($m > 0) {
							$known_sciences_str =
								"„{$science->is_req_of[0]->name}“";
							for ($j = 1; $j < $m; $j++) {
								$known_sciences_str .=
										", „{$science->is_req_of[$j]->name}“";
							}
							$messages[$i]->str = str_replace(
								array('[SCIENCE]', '[NEW_SCIENCES]'),
								array(
									ScienceConstants::$SC_NAME[$sc_id],
									$known_sciences_str
								),
								LNG_EVM_SCIENCE_FINISHED1
							);
							if (ScienceConstants::$SC_DESC_KNOWN[$sc_id]) {
								$messages[$i]->str .= '<br/>'.
									ScienceConstants::$SC_DESC_KNOWN[$sc_id];
							}
							$messages[$i]->str .= '<br/>'. str_replace(
								array('[SCIENCE]', '[NEW_SCIENCES]'),
								array(
									ScienceConstants::$SC_NAME[$sc_id],
									$known_sciences_str
								),
								LNG_EVM_SCIENCE_FINISHED2
							);

						}
						else {
							$messages[$i]->str = str_replace(
								'[SCIENCE]',
								ScienceConstants::$SC_NAME[$sc_id],
								LNG_EVM_SCIENCE_FINISHED_NO_PROGRESS1
							).'<br/>';
							if (ScienceConstants::$SC_DESC_KNOWN[$sc_id]) {
								$messages[$i]->str .=
									ScienceConstants::$SC_DESC_KNOWN[$sc_id];
							}
							else {
								$messages[$i]->str .= str_replace(
									'[SCIENCE]',
									ScienceConstants::$SC_NAME[$sc_id],
									LNG_EVM_SCIENCE_FINISHED_NO_PROGRESS2
								);
							}
							$messages[$i]->str .= '<br/><br/>'.str_replace(
								'[SCIENCE]',
								ScienceConstants::$SC_NAME[$sc_id],
								LNG_EVM_SCIENCE_FINISHED_NO_PROGRESS3
							);
						}
						$messages[$i]->template = 'research';
						$messages[$i]->headline
							= "„".ScienceConstants::$SC_NAME[$sc_id]."“";
						$messages[$i]->publish_title =
							LNG_FB_SHARE_SCIENCE_RESEARCHED_TITLE;
						$messages[$i]->publish_str = str_replace(
							"[SCIENCE]",
							"„".ScienceConstants::$SC_NAME[$sc_id]."“",
							LNG_FB_SHARE_SCIENCE_RESEARCHED_DESC
						);
						$messages[$i]->publish_action =
							LNG_FB_SHARE_SCIENCE_RESEARCHED_LINK;
					}
					break;
				case self::TYPE_TRAINING_FINISHED:
					list($name, $pic_base) =
						explode('-', $messages[$i]->extra_str);
					if ($messages[$i]->medal_id) {
						$str_base =	constant(
							'LNG_MEDAL_'.$messages[$i]->medal_id.'_TEXT'
						);
						/*$m = Medals::get_medals(
							IMedals::TYPE_UNITS_TRAINED
						);
						unset($m[2]);
						$messages[$i]->additional_pics =
							self::medal_id_to_pic_url_array($m);*/
					}
					else {
						$str_base =	LNG_EVM_TRAINING_FINISHED;
						$messages[$i]->pic = array(
							'pic_url'
									=>'ui/officers/cwo/grandpa_left_95x171.png',
							'width'	=> 95,
							'height'=> 171,
							'pos'	=> 0
						);
					}

					$messages[$i]->str = str_replace(
						array('[NAME]'),
						array(
							'<b>'.ucfirst($name).'</b>'
						),
						$str_base
					);

					break;
				case self::TYPE_TRAINING_FINISHED_SCIENTIST:
					list($name, $pic_base) =
						explode('-', $messages[$i]->extra_str);
					if ($messages[$i]->medal_id) {
						$str_base =	constant(
							'LNG_MEDAL_'.$messages[$i]->medal_id.'_TEXT'
						);
						/*$m = Medals::get_medals(
							IMedals::TYPE_UNITS_TRAINED
						);
						$messages[$i]->additional_pics =
							self::medal_id_to_pic_url_array($m[2]);*/
					}
					else {
						$str_base =	LNG_EVM_TRAINING_FINISHED_SCIENTISTS;
						$messages[$i]->pic = array(
							'pic_url'
									=>'ui/officers/cso/grandma_left_93x217.png',
							'width'	=> 93,
							'height'=> 217,
							'pos'	=> 0
						);
						$messages[$i]->publish_title =
							LNG_FB_SHARE_TRAINING_FINISHED_SCIENTIST_TITLE;
						$messages[$i]->publish_str =
							LNG_FB_SHARE_TRAINING_FINISHED_SCIENTIST_DESC;
							$messages[$i]->publish_action =
							LNG_FB_SHARE_TRAINING_FINISHED_SCIENTIST_LINK;
					}

					$messages[$i]->str = str_replace(
						array('[MAMMUN]'),
						array($name),
						$str_base
					);
					break;
				case self::TYPE_MAP_INVITATION_RECEIVED:
					list($nick, $world_id)
						= explode('-', $messages[$i]->extra_str);
					$xp = User::get_instance_by_nick($nick)->get_cur_level();
					$map_id = ($messages[$i]->extra > 0)
						? $messages[$i]->extra
						: User::get_instance_by_nick($messages[$i]->extra_str)
							->get_map_id();

					if (!$world_id)
						$world_id = floor($map_id/100);
					$map_id = $map_id%100;

					$messages[$i]->str = str_replace(
						array('[INVITOR]', '[EXP]'),
						array(User::beautify_nick($nick), $xp),
						LNG_EVM_MAP_INVITATION_RECEIVED
					);

					$av = Avatar::get_avatars(array($nick));
					$avatar = str_replace('SIZE',150, $av->$nick);
					$messages[$i]->pic = array(
						'pic_url'	=> $avatar,
						'width'		=> 150,
						'height'	=> 150,
						'pos'		=> 0
					);

					$messages[$i]->on_yes = "function(ev) { ".
						"MapUI.change_map($world_id, $map_id); }";
					$messages[$i]->on_no = "function(ev){}";
					break;
				case self::TYPE_MAP_INVITATION_REQUEST:
					/**
					 * Wenn der Spieler schon auf der Insel ist, z.B. weil er
					 * einer die Anfrage schon positiv beantwortet hat und
					 * der Spieler auf die Einladung reagierte, bevor der
					 * aktuelle Spieler sich einloggte, dann brauchen wir
					 * dem Spieler auch nicht mehr die Anfrage zeigen.
					 * Das wird jetzt getestet.
					 */
					require_once(dirname(__FILE__).'/map/MapPlayers.inc');
					$mp = MapPlayers::get_instance(
						User::get_current_user()->get_world_id(),
						User::get_current_user()->get_map_id()
					);
					$uids_on_map = $mp->get_uids_on_map();
					$already_on_map = false;
					foreach ($uids_on_map as $uid) {
						if ($messages[$i]->extra == $uid) {
							$already_on_map = true;
							break;
						}
					}
					if ($already_on_map)
						break;

					/**
					 * Ab hier dann der Fall, dass er noch nicht auf der Insel
					 * ist.
					 */
					$user = User::get_instance_by_id($messages[$i]->extra);
					$nick = $user->get_nick();

					$messages[$i]->str = str_replace(
						'[REQUEST_NICK]', $nick,
						LNG_EVM_MAP_INVITATION_REQUEST
					);

					$av = Avatar::get_avatars(array($nick));
					$avatar = str_replace('SIZE', 150, $av->$nick);
					$avatar = str_replace('SIZE',150, $av->$nick);
					$messages[$i]->pic = array(
						'pic_url'	=> $avatar,
						'width'		=> 150,
						'height'	=> 150,
						'pos'		=> 0
					);
					$messages[$i]->on_yes = "function(ev) { ".
						"PureFW.AJAXClientServer.send_request(".
							"'invitation.php?send=true&chosen_friends=$nick',".
							"function (response_arr) {".
								"MammunUI.show_warning(350, 200,".
									"'".str_replace(
										'[REQUEST_NICK]', $nick,
										LNG_EVM_MAP_INVITATION_REQUEST_CONFIRM
									)
									."', PureFW.ConfirmationBox.YES);".
							"}".
						"); }";
					$messages[$i]->on_no = "function(ev){}";
					break;
				case self::TYPE_UPGRADE_COMPLETED:
				case self::TYPE_CONSTRUCTION_COMPLETED:
					require_once(dirname(__FILE__)
							.'/buildings/Buildings.inc');
					if ($messages[$i]->medal_id) {
						$str_base =	constant(
							'LNG_MEDAL_'.$messages[$i]->medal_id.'_TEXT'
						);
						/*$m = Medals::get_medals(
							IMedals::TYPE_BUILDING_CONSTRUCTED
						);
						$messages[$i]->additional_pics =
							self::medal_id_to_pic_url_array($m);*/
					}
					else {
						$type = (int)$messages[$i]->extra_str;
						switch ($type) {
							case IBuildings::SHED:
								$str_base = LNG_EVM_CONSTRUCTION_COMPLETE_SHED;
								break;
							case IBuildings::TENT:
								$str_base = LNG_EVM_CONSTRUCTION_COMPLETE_TENT;
								break;
							case IBuildings::FARM:
								$str_base = LNG_EVM_CONSTRUCTION_COMPLETE_FARM;
								break;
							case IBuildings::DWELLING:
								$str_base = LNG_EVM_CONSTRUCTION_COMPLETE_HUT;
								break;
							default:
								$str_base = '';
						}
						$messages[$i]->pic = array(
							'pic_url'
									=>'ui/officers/cco/'.
										'construction_bernard_95x187.png',
							'width'	=> 95,
							'height'=> 187,
							'pos'	=> 0
						);
						$messages[$i]->publish_str =
							str_replace(
								array('[SECTOR]','[BUILDING]'),
								array(
									$messages[$i]->extra,
									Buildings::get_b_name($type)
								),
								LNG_FB_SHARE_CONSTRUCTION_COMPLETE_DESC
							);
						$messages[$i]->publish_title
							= LNG_FB_SHARE_CONSTRUCTION_COMPLETE_TITLE;
						$messages[$i]->publish_action
							= LNG_FB_SHARE_CONSTRUCTION_COMPLETE_LINK;

					}
					$messages[$i]->str = str_replace(
						array('[SECTOR]', '[BUILDING]'),
						array(
							$messages[$i]->extra,
							Buildings::get_b_name($type)
						),
						$str_base
					);

					break;
				case self::TYPE_BUILDING_HAS_BEEN_PAINTED:
					require_once(dirname(__FILE__)
							.'/buildings/Buildings.inc');
					list($world_id, $map_id, $sector_id, $x, $y, $color_int) =
						explode('-', $messages[$i]->extra_str);
					$uid = $messages[$i]->extra;
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
						// not supported
						self::delete_message($messages[$i]->num);
						$indizes_to_delete[] = $i;
						$messages[$i] = null;
						break;
					}
					$nick = User::get_instance_by_id($uid)->get_nick();
					$messages[$i]->str = str_replace(
						array(
							'[SECTOR]', '[NICK]', '[B_NAME]', '[LINK]', '[/LINK]'
						),
						array(
						 	$sector_id,
							User::beautify_nick($nick),
							Buildings::get_b_name($building->get_type()),
							'<a href="javascript: UIButtons.toggle_new_message('
								."'recipients=$nick&subject="
						.urlencode(LNG_EVM_PAINTING_THANK_YOU_DEFAULT_TITLE)
								."&text="
						.urlencode(str_replace(
							array('[B_NAME]', '[SECTOR]', '[NICK]'),
							array(
								Buildings::get_b_name($building->get_type()),
								$sector_id,
								$nick
							),
							LNG_EVM_PAINTING_THANK_YOU_DEFAULT_TEXT
						))."');".
							'UINewIM.window.set_z_index(301);">',
							'</a>'
						),
						LNG_EVM_BUILDING_HAS_BEEN_PAINTED
					);

					$messages[$i]->publish_str = str_replace(
						array('[SECTOR]', '[NICK]', '[B_NAME]'),
						array(
						 	$sector_id,
							$nick,
							Buildings::get_b_name($building->get_type())
						),
						LNG_FB_SHARE_BUILDING_HAS_BEEN_PAINTED_DESC
					);
					$messages[$i]->publish_title
						= LNG_FB_SHARE_BUILDING_HAS_BEEN_PAINTED_TITLE;
					$messages[$i]->publish_action
						= LNG_FB_SHARE_BUILDING_HAS_BEEN_PAINTED_LINK;


					$pic = ($color_int > 0)
						? str_replace('.png', $color_int.'.png',
							$building->get_pic())
						: $building->get_pic();

					$messages[$i]->pic = array(
						'pic_url'
								=> "ui/elements/building_preview_boxes/$pic",
						'width'	=> 120,
						'height'=> 137,
						'pos'	=> 0
					);
					break;
				case self::TYPE_REPAINTED_HIS_MY_COLORED_BUILDING:
					require_once(dirname(__FILE__)
							.'/buildings/Buildings.inc');
					list($world_id, $map_id, $sector_id, $x, $y, $color_int) =
						explode('-', $messages[$i]->extra_str);
					$uid = $messages[$i]->extra;
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
						// not supported
						self::delete_message($messages[$i]->num);
						$indizes_to_delete[] = $i;
						$messages[$i] = null;
						break;
					}

					$nick = User::get_instance_by_id($uid)->get_nick();
					$messages[$i]->str = str_replace(
						array('[SECTOR]', '[NICK]', '[B_NAME]', '[LINK]',
							'[/LINK]'),
						array(
						 	$sector_id,
							User::beautify_nick($nick),
							Buildings::get_b_name($building->get_type()),
							'<a href="javascript: UIButtons.toggle_new_message('
								."'recipients=$nick');".
							'UINewIM.window.set_z_index(301);">',
							'</a>'
						),
						LNG_EVM_REPAINTED_HIS_MY_COLORED_BUILDING
					);

					$messages[$i]->publish_str = str_replace(
						array('[SECTOR]', '[NICK]', '[B_NAME]'),
						array(
						 	$sector_id,
							$nick,
							Buildings::get_b_name($building->get_type())
						),
						LNG_FB_SHARE_REPAINTED_HIS_MY_COLORED_BUILDING_DESC
					);
					$messages[$i]->publish_title
						= LNG_FB_SHARE_REPAINTED_HIS_MY_COLORED_BUILDING_TITLE;
					$messages[$i]->publish_action
						= LNG_FB_SHARE_REPAINTED_HIS_MY_COLORED_BUILDING_LINK;


					$pic = ($color_int > 0)
						? str_replace('.png', $color_int.'.png',
							$building->get_pic())
						: $building->get_pic();

					$messages[$i]->pic = array(
						'pic_url'
								=> "ui/elements/building_preview_boxes/$pic",
						'width'	=> 120,
						'height'=> 137,
						'pos'	=> 0
					);
					break;
				case self::TYPE_LEVELUP_PRESENT_GAINED_OLD:
					require_once(dirname(__FILE__).'/buildings/Buildings.inc');
					if ($messages[$i]->extra > 0)
					{
						$type = (int)$messages[$i]->extra_str;
						$messages[$i]->str = str_replace(
							'[BUILDING]',
							Buildings::get_b_name($type),
							LNG_EVM_LEVELUP_BUILDING_OLD
						);
						$messages[$i]->publish_str = str_replace(
							array('[LEVEL]', '[BUILDING]'),
							array(
								User::get_current_user()->get_cur_level(true),
								Buildings::get_b_name($type)
							),
							LNG_FB_SHARE_LEVELUP_BUILDING_DESC
						);
						$messages[$i]->publish_title = str_replace(
							array('[LEVEL]', '[BUILDING]'),
							array(
								User::get_current_user()->get_cur_level(true),
								Buildings::get_b_name($type)
							),
							LNG_FB_SHARE_LEVELUP_BUILDING_TITLE
						);
						$messages[$i]->publish_action
							= LNG_FB_SHARE_LEVELUP_BUILDING_LINK;



						$messages[$i]->pic = array(
							'pic_url'
									=> 'ui/elements/building_preview_boxes/'
										. 'item_detail_view/'.$type.'.png',
							'width'	=> 165,
							'height'=> 289,
							'pos'	=> 0
						);
						$messages[$i]->width = 530;
						$messages[$i]->height = 352;
						$messages[$i]->on_yes = "function(ev) { ".
							"Overview.increase_item_amount(1); }";
						$messages[$i]->on_no = "function(ev) { ".
							"Overview.increase_item_amount(1); }";
					}
					break;
				case self::TYPE_LEVELUP_PRESENT_GAINED:
					require_once(dirname(__FILE__).'/buildings/Buildings.inc');
					if ($messages[$i]->extra > 0)
					{
						$type = (int)$messages[$i]->extra_str;
						$messages[$i]->str = str_replace(
							array('[LEVEL]', '[BUILDING]'),
							array(
								User::get_current_user()->get_cur_level(true),
								Buildings::get_b_name($type)
							),
							LNG_EVM_LEVELUP_BUILDING
						);
						$messages[$i]->publish_str = str_replace(
							array('[LEVEL]', '[BUILDING]'),
							array(
								User::get_current_user()->get_cur_level(true),
								Buildings::get_b_name($type)
							),
							LNG_FB_SHARE_LEVELUP_BUILDING_DESC
						);
						$messages[$i]->publish_title
							= LNG_FB_SHARE_LEVELUP_BUILDING_TITLE;
						$messages[$i]->publish_action
							= LNG_FB_SHARE_LEVELUP_BUILDING_LINK;


						$messages[$i]->pic = array(
							'pic_url'
									=> 'ui/elements/building_preview_boxes/'
										. 'item_detail_view/'.$type.'.png',
							'width'	=> 165,
							'height'=> 289,
							'pos'	=> 0
						);
						$messages[$i]->width = 530;
						$messages[$i]->height = 352;
						$messages[$i]->on_yes = "function(ev) { ".
							"Overview.increase_item_amount(1); }";
						$messages[$i]->on_no = "function(ev) { ".
							"Overview.increase_item_amount(1); }";
					}
					else {
						$type = (int)$messages[$i]->extra_str;
						$messages[$i]->str = str_replace(
							array('[LEVEL]', '[BUILDING]'),
							array(
								User::get_current_user()->get_cur_level(true),
								Buildings::get_b_name($type)
							),
							LNG_EVM_LEVELUP_BLUEPRINT
						);
						$messages[$i]->publish_str = str_replace(
							array('[LEVEL]', '[BUILDING]'),
							array(
								User::get_current_user()->get_cur_level(true),
								Buildings::get_b_name($type)
							),
							LNG_FB_SHARE_LEVELUP_BLUEPRINT_DESC
						);
						$messages[$i]->publish_title
							= LNG_FB_SHARE_LEVELUP_BLUEPRINT_TITLE;
						$messages[$i]->publish_action
							= LNG_FB_SHARE_LEVELUP_BLUEPRINT_LINK;


						$messages[$i]->pic = array(
							'pic_url'
									=> 'ui/elements/building_preview_boxes/'
										. 'item_detail_view/'.$type.'_b.jpg',
							'width'	=> 300,
							'height'=> 300,
							'pos'	=> 0
						);
						$messages[$i]->width = 620;
						$messages[$i]->height = 350;
					}
					break;
				case self::TYPE_SECTOR_BATTLE_WON:
					if (!$messages[$i]->medal_id) {
						self::delete_message($messages[$i]->num);
						$indizes_to_delete[] = $i;
						$messages[$i] = null;
						/* not yet
						$messages[$i]->publish_str =
								LNG_FB_SHARE_SECTOR_BATTLE_WON;*/
					}
					else {
						/*$m = Medals::get_medals(
							IMedals::TYPE_ATTACK_SUCCESSFUL
						);
						$messages[$i]->additional_pics =
							self::medal_id_to_pic_url_array(
								array_merge($m[2], $m[3])
						);*/
						$sector_id = $messages[$i]->extra;
						$enemy_nick = $messages[$i]->extra_str;
						$str_base = constant(
							'LNG_MEDAL_'.$messages[$i]->medal_id.'_TEXT'
						);
						$messages[$i]->str = str_replace(
							array('[SECTOR]', '[PLAYER]'),
							array($sector_id, User::beautify_nick($enemy_nick)),
							$str_base
						);
					}
					break;
				case self::TYPE_SECTOR_WON_NO_BATTLE:
					if (!$messages[$i]->medal_id) {
						self::delete_message($messages[$i]->num);
						$indizes_to_delete[] = $i;
						$messages[$i] = null;
						/* not yet
						$messages[$i]->publish_str =
								LNG_FB_SHARE_SECTOR_WON_NO_BATTLE;*/
					}
					else {
						/*$m = Medals::get_medals(
							IMedals::TYPE_ATTACK_SUCCESSFUL
						);
						$messages[$i]->additional_pics =
							self::medal_id_to_pic_url_array($m[1]);*/
						$sector_id = $messages[$i]->extra;
						$str_base =	constant(
							'LNG_MEDAL_'.$messages[$i]->medal_id.'_TEXT'
						);
						$messages[$i]->str = str_replace(
							array('[SECTOR]'),
							array($sector_id),
							$str_base
						);
					}
					break;
				case self::TYPE_USER_RECRUITED:
					$nick = $messages[$i]->extra_str;

					if ($messages[$i]->medal_id) {
						$str_base =	constant(
							'LNG_MEDAL_'.$messages[$i]->medal_id.'_TEXT'
						);
						/*$m = Medals::get_medals(
							IMedals::TYPE_PREZELS_GAINED
						);
						$messages[$i]->additional_pics =
							self::medal_id_to_pic_url_array($m[0]);*/
					}
					else {
						$str_base =	LNG_EVM_USER_RECRUITED;
						$av = Avatar::get_avatars(array($nick));
						$avatar = str_replace('SIZE',150, $av->$nick);
						$messages[$i]->pic = array(
							'pic_url' => $avatar,
							'width'	=> 150,
							'height'=> 150,
							'pos'	=> 0
						);
					}

					$messages[$i]->str = str_replace(
						array('[NICK]'),
						array(
							User::beautify_nick($messages[$i]->extra_str)
						),
						$str_base
					);
					break;
				case self::TYPE_GIFT_RECEIVED:
					require_once(dirname(__FILE__).
						'/../shared/classes/ItemConstants.inc');
					$from_nick = User::get_instance_by_id($messages[$i]->extra)
						->get_nick();
					list($type, $id) = explode('-', $messages[$i]->extra_str);
					$messages[$i]->str = str_replace(
						array('[NICK]', '[ITEM]'),
						array(
							User::beautify_nick($from_nick),
							ItemConstants::$ITEM_NAMES[$type][$id]
						),
						LNG_EVM_GIFT_RECEIVED
					);
					$messages[$i]->publish_str = str_replace(
						array('[OTHER_NICK]', '[ITEM]'),
						array(
							User::beautify_nick($from_nick),
							ItemConstants::$ITEM_NAMES[$type][$id]
						),
						LNG_FB_SHARE_GIFT_RECEIVED_DESC
					);
					$messages[$i]->pic = array(
						'pic_url'	=> 'ui/icons/labels/items/168x150/'
							.$type.'/'.$id.'.png',
						'width'		=> 168,
						'height'	=> 150,
						'tooltip'	=> ItemConstants::$ITEM_NAMES[$type][$id]
					);
					$messages[$i]->publish_title
						= LNG_FB_SHARE_GIFT_RECEIVED_TITLE;
					$messages[$i]->publish_action
						= LNG_FB_SHARE_GIFT_RECEIVED_LINK;

					break;
				case self::TYPE_USE_FACEBOOK_SHARE:
					$messages[$i]->str = LNG_EVM_USE_FB_SHARE;
					$messages[$i]->pic = array(
						'pic_url'	=>
							'ui/icons/labels/facebook_share/symbol_109x107.png',
						'width'		=> 109,
						'height'	=> 107,
						'pos'		=> 0
					);
					$messages[$i]->template = 'fb_share';
					break;
				case self::TYPE_NEW_CATALOG:
					$messages[$i]->str = LNG_EVM_NEW_CATALOG;
					$messages[$i]->on_yes = "function(ev) { ".
						"UIButtons.toggle_shop(); }";
					break;
				case self::TYPE_SHOW_ANNOUNCEMENT:
					$messages[$i]->str = LNG_EVM_ACCOUNCEMENT;
					break;
				default:
					if (($messages[$i]->type < self::TYPE_SPECIAL_MIN) ||
						($messages[$i]->type > self::TYPE_SPECIAL_MAX))
					{
						// not supported
						self::delete_message($messages[$i]->num);
						$indizes_to_delete[] = $i;
						$messages[$i] = null;
					}
			}
			if ($messages[$i]) {
				if ($messages[$i]->score > 0) {
					$messages[$i]->score_str = '+'.$messages[$i]->score.' '.
						LNG_EVM_POINTS;
				}
				if ($messages[$i]->medal_id > 0) {
					$messages[$i]->headline = constant(
						'LNG_MEDAL_'.$messages[$i]->medal_id.'_HEADLINE'
					);
					$messages[$i]->add_str = constant(
						'LNG_MEDAL_'.$messages[$i]->medal_id.'_TOOLTIP'
					);
					$messages[$i]->publish_str = str_replace(
						'[MY_NICK]',
						User::get_current_user()->get_nick_main(),
						constant(
							'LNG_FB_SHARE_MEDAL_'.$messages[$i]->medal_id
								.'_DESC'
						)
					);
					$messages[$i]->publish_title = str_replace(
						'[MY_NICK]',
						User::get_current_user()->get_nick_main(),
						constant(
							'LNG_FB_SHARE_MEDAL_'.$messages[$i]->medal_id
								.'_TITLE'
						)
					);
					$messages[$i]->publish_action = str_replace(
						'[MY_NICK]',
						User::get_current_user()->get_nick_main(),
						constant(
							'LNG_FB_SHARE_MEDAL_'.$messages[$i]->medal_id
								.'_LINK'
						)
					);

				}
			}
		}
		/**
		 * Jetzt nichtunterstützte Events rauslöschen, das Array dabei aber
		 * konsistent halten, so dass die Indizes weiterhin klar durchgezählt
		 * sind.
		 */
		foreach ($indizes_to_delete as $i) {
			array_splice($messages, $i, 1);
		}
		return $messages;
	}
}