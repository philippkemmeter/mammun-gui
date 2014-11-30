<? Language::get_instance()->load_language_file('profile') ?>
<script type="text/javascript">
PauseMenu = new Object();
PauseMenu.loaded = false;
PauseMenu.statistics = new Array();
PauseMenu.buddies = <?= json_encode($buddies) ?>;
PauseMenu.world_users = <?= $random_user_json ?>;
PauseMenu.avatars = <?= json_encode($avatar_data) ?>;
PauseMenu.xps = <?= json_encode($xp_assoc) ?>;

<?/*This must be checked here because MammunUI.user_in_facebook isn't yet loaded.*/?>
MammunUI.user_in_facebook = <?	if(isset($_SESSION['fb']['fb_sig'])
										|| isset($_SESSION['fb']['fb_sig_post'])
										|| isset($_SESSION['fb']['session']))
									echo "true";
								else
									echo "false"; ?>;
<? /**
    * Verweis auf das Fenster, in dem diese Seite hier dargestellt wird.
    */ ?>
PauseMenu.this_window = PureFW.ContainerAJAX.instances[<?=$this_win_id?>];

PauseMenu.event_log = <?= json_encode($event_log) ?>;
PauseMenu.event_log_entries = new Array();
PauseMenu.refresh_active = true;

<? /**
    * Erstellen der Statistiken der Spieler auf der aktuellen Karte, also des
    * Rankings
    */ ?>
<? if (!defined('TEST_SERVER')) : ?>
<? for ($i = 0; $i < count($map_ranking); $i++) : ?>
<?	$user = User::get_instance_by_id($map_ranking[$i]->uid);
	$personal_stuff = $user->get_personal_data();
	$wcf_uid = $personal_stuff->id_wcf;
	$state = $obj_map_players->get_user_state_on_map($user->get_id());
	$color = MapPlayers::get_instance(
		$current_user->get_world_id(),
		$current_user->get_map_id()
	)->get_color($user->get_id());
	$nick = $user->get_nick();
	$value = $map_ranking[$i]->score; ?>
	PauseMenu.statistics[<?= $i ?>] = new Object();
	PauseMenu.statistics[<?= $i ?>].nick = "<?= $nick ?>";
	PauseMenu.statistics[<?= $i ?>].score = "<?= floor($value) ?>";
	PauseMenu.statistics[<?= $i ?>].color = "<?= $color ?>";
	PauseMenu.statistics[<?= $i ?>].wcf_uid = "<?= $wcf_uid ?>";
	PauseMenu.statistics[<?= $i ?>].is_alive =
		<? if($state != IMapPlayers::STATE_KILLED)
				echo "true";
		   else echo "false"; ?>;
	PauseMenu.statistics[<?= $i ?>].ui_score_gainer = new Gainer();
	PauseMenu.statistics[<?= $i ?>].ui_score_gainer.set_current_amount(
			<?= $map_ranking[$i]->score ?>);
	<? if (isset($map_score_gains->{$map_ranking[$i]->uid})
		&& false /*forecast deactivated: nicht genau genug!*/) : ?>
		PauseMenu.statistics[<?= $i ?>].ui_score_gainer.set_gain_per_second(
			<?= $map_score_gains->{$map_ranking[$i]->uid}/3600 ?>);
	<? else : ?>
		PauseMenu.statistics[<?= $i ?>].ui_score_gainer.set_gain_per_second(0);
	<? endif ?>

	<? if ($map_ranking[$i]->uid == User::get_current_user()->get_id()) : ?>
		PauseMenu.statistics.ui_my_score_gainer =
			PauseMenu.statistics[<?= $i ?>].ui_score_gainer;
	<? endif ?>
<? endfor ?>
<? endif ?>

//PauseMenu.refresh_buddies = function()
//{
//	PureFW.AJAXClientServer.send_request(
//		'../community/gausus_wcf_hacks/friend_interface.php?get_friends='+
//			'<?=$current_user->get_nick()?>&s=<?
//				echo $_SESSION['WCF_SID']?>&t=<?= $_SESSION['WCF_ST'] ?>'
//			, PauseMenu.refresh_callback
//	);
//}

PauseMenu.refresh_buddies = function()
{
	PureFW.AJAXClientServer.send_request(
		"pause_menu.php?get_buddies=true",
		PauseMenu.refresh_callback);
};

PauseMenu.refresh_callback = function(response_arr)
{
	destroy_object(PauseMenu.buddies);
	PauseMenu.buddies = null;
	eval('(PauseMenu.buddies = '+response_arr[0]+')');
//	alert("online_buddies: "+PauseMenu.buddies.onlineFriends);
	PauseMenu.create_buddies_section();
	<? /** Nochmal die Events grow/shrink feuern,
		damit die Sachen korrekt angeordnet werden. */?>
	if (PauseMenu.community_slide_table.get_col(1).is_open())
		PauseMenu.community_slide_table.get_col(1).on_grow();
	else
		PauseMenu.community_slide_table.get_col(1).on_shrink();
}

PauseMenu.refresh_event_log = function() {
	PureFW.AJAXClientServer.send_request('pause_menu.php?get_event_log=true',
		PauseMenu.refresh_event_log_callback);
}

PauseMenu.refresh_event_log_callback = function(response_arr) {
	if (response_arr[0] == 1) {
		PauseMenu.event_log = eval('('+response_arr[1]+')');
		PauseMenu.create_event_log(12);
	}
}

PauseMenu.rt_refresh = function() {
	if (PauseMenu.refresh_active) {
		PauseMenu.refresh_buddies();
		PauseMenu.refresh_event_log();
		var n = PauseMenu.statistics.length;
		for (var i = 0; i < n; i++) {
			if (PauseMenu.statistics[i].is_alive) {
				PauseMenu.diplomacy[i].player_score_container.set_content(
					number_format(
						PauseMenu.statistics[i].ui_score_gainer.get_cur_value(),
						0, ',', '.'
					)
				);
			}
		}
	}
}

PauseMenu.create_event_log = function(amount) {
	PauseMenu.event_log_entries.destroy();
	PauseMenu.event_log_entries = new Array();

	var max_entries = PauseMenu.event_log.length;
	var current_entries = PauseMenu.event_log_entries.length;
	var from = current_entries;
	var to = from + amount;
	for (var i = from, j = from; (i < max_entries) && (i<to); i++) {
		if (!PauseMenu.event_log[i])
			continue;

		var log_entry = PauseMenu.event_log[i];
		PauseMenu.event_log_entries[j] = new Object();
		PauseMenu.event_log_entries[j].destroy = (function(_j) {
			return function(ev) {
				destroy_object(PauseMenu.event_log_entries[_j]);
			}
		})(j);

		PauseMenu.event_log_entries[j].cover_container = new PureFW.Container(
			PauseMenu.activity_box_content,
			0, 0,
			PauseMenu.activity_box.width - 50, 0);
		PauseMenu.event_log_entries[j].cover_container.set_positioning(
				'relative');
		PauseMenu.event_log_entries[j].icon = new PureFW.Image(
			PauseMenu.event_log_entries[j].cover_container,
			0, 0,
			24, 24,
			'ui/icons/labels/events/'+log_entry.type+'.png'
		);
		PauseMenu.event_log_entries[j].icon.set_positioning('relative');
		PauseMenu.event_log_entries[j].icon.set_float('left');

		PauseMenu.event_log_entries[j].icon.get_node().style.margin = "3px";

		if(MammunUI.user_in_facebook)
		{
			PauseMenu.event_log_entries[j].facebook_publish_button = new PureFW.Container(
				PauseMenu.event_log_entries[j].cover_container,
				0, 0,
				16,16);
			PauseMenu.event_log_entries[j].facebook_publish_button.set_tooltip(
				"Publish on Facebook");
			PauseMenu.event_log_entries[j].facebook_publish_button
				.set_positioning('relative');
			PauseMenu.event_log_entries[j].facebook_publish_button.set_bg_img(
					'ui/elements/logos/facebook16x16.png');
			PauseMenu.event_log_entries[j].facebook_publish_button.add_event_handler(
				'click', function(content) {
							return function(ev)
							{
								PauseMenu.publish_facebook_story(content);
							}
				}(log_entry));
			PauseMenu.event_log_entries[j].facebook_publish_button.set_float('right');
			PauseMenu.event_log_entries[j].facebook_publish_button.set_padding(3);
		}

		PauseMenu.event_log_entries[j].message = new PureFW.Container(
			PauseMenu.event_log_entries[j].cover_container,
			0, 0,
			/*PauseMenu.event_log_entries[j].cover_container.width
				- (PauseMenu.event_log_entries[j].icon.position.x
					+ PauseMenu.event_log_entries[j].icon.width + 2)*/0,
			0);
		PauseMenu.event_log_entries[j].message.set_positioning('relative');
		var d = new Date((log_entry.t_stamp
			+ PureFW.Time.get_client_server_diff())*1000);

		PauseMenu.event_log_entries[j].message.set_content(
			log_entry.str+'<br/>'+'<span style="color: #555">'
			+d.formatDate("d.m.Y, H:i")+'</span>'
		);
		j++;
	}

	<?/*
	if(typeof(PauseMenu.more_activities_button) == 'undefined')
	{
		PauseMenu.more_activities_button = new PureFW.Container(
			PauseMenu.activity_box_content,
			0, 0,
			PauseMenu.activity_box_content.width, 20);
		PauseMenu.more_activities_button.set_content("<?=LNG_MORE_ACTIVITIES?>");
		PauseMenu.more_activities_button.set_text_align('center');
		PauseMenu.more_activities_button.add_event_handler('click',
			function ()
			{
				PauseMenu.create_event_log(12);
			});
	}

	PauseMenu.more_activities_button.set_y(i*30+3);
	*/?>
	var totalScrollHeight = 0;
	PauseMenu.activity_box_content.set_height(
		PauseMenu.activity_box.height-35
	);
	for(var i in PauseMenu.event_log_entries)
	{
		if(typeof(PauseMenu.event_log_entries[i]) == "function") continue;
		totalScrollHeight +=
			PauseMenu.event_log_entries[i].cover_container.get_height()
			/ PureFW.WidgetManager.manager_all.scale_factor;
	}
	PauseMenu.activity_box_content.set_inner_height(totalScrollHeight/*+23*/);
}

PauseMenu.publish_facebook_story = function(entry)
{
	entry.str = entry.str.replace(/<[^>]+>/g, "");
	MammunUI.facebook_publish_prompt(
		entry.publish_str, '', 'ui/icons/labels/events/'+entry.type+'.png',
		PauseMenu.publish_facebook_callback
	);
};
PauseMenu.publish_facebook_callback = function(post_id, exception)
{
   	if(post_id)
   	{
//           	alert("Success!");
//            post_to_my_server(post_id);
   	}
   	if(exception)
   	{
           	alert(exception);
   	}
   	else
   	{
//   	   	alert("something went wrong");
   	}
};

PauseMenu.create_diplomacy_section = function()
{
	PauseMenu.diplomacy = new Array();
	var n = PauseMenu.statistics.length;
	var y = 10;
	for(var i = 0; i < n; i++)
	{
		PauseMenu.diplomacy[i] = new Object();
		PauseMenu.diplomacy[i].player_pic_container = new PureFW.Avatar.Avatar(
			PauseMenu.community_slide_table.get_col(0).get_content_node(),
			17, y,
			48, 48,
			PauseMenu.avatars[PauseMenu.statistics[i].nick].replace('SIZE',44),
			PauseMenu.statistics[i].nick,
			PauseMenu.statistics[i].color
		);
		if (PauseMenu.statistics[i].nick == '<?= $current_user->get_nick() ?>')
		{
			PauseMenu.diplomacy[i].player_pic_container.avatar_img.set_id(
				"PauseMenu_my_avatar"
			);
		}

		PauseMenu.diplomacy[i].player_name_container = new PureFW.Container(
			PauseMenu.community_slide_table.get_col(0).get_content_node(),
			80, y,
			100, 20);

		PauseMenu.diplomacy[i].player_name_container.set_content(
			'<a style="color: #fff; font-weight: bold; " '
				+'href="javascript: UIButtons.toggle_window(\''
					+ 'profile.php?nick=' + PauseMenu.statistics[i].nick +
														'\', \'\', 110);">'
				+ MammunUI.get_nick_main(PauseMenu.statistics[i].nick) + '</a>'
		);

		PauseMenu.diplomacy[i].player_score_container = new PureFW.Container(
			PauseMenu.community_slide_table.get_col(0).get_content_node(),
			180, y,
			70, 20);
		if (PauseMenu.statistics[i].is_alive) {
			PauseMenu.diplomacy[i].player_score_container.set_content(
				number_format(
					PauseMenu.statistics[i].ui_score_gainer.get_cur_value(),
					0, ',', '.'
				)
			);
		}
		else {
			PauseMenu.diplomacy[i].player_score_container.set_content(
				'<?= LNG_LEFT_ISLAND ?>');
		}
		PauseMenu.diplomacy[i].player_score_container.get_node().style.textAlign
			 = 'right';

		PauseMenu.diplomacy[i].player_mail_image = new PureFW.Image(
			PauseMenu.community_slide_table.get_col(0).get_content_node(),
			80, y + 20,
			36, 37,
			'ui/icons/labels/communication/36x37/pm.png'
		);
		PauseMenu.diplomacy[i].player_mail_image.set_tooltip(
			'<?= LNG_TITLE_PM_BUTTON ?>'.replace(/\[NICK\]/g,
				PauseMenu.statistics[i].nick)
		);
		PauseMenu.diplomacy[i].player_mail_image.add_event_handler("click",
				(function(_i) {
					return function(ev)
					{
						UIButtons.toggle_new_message("recipients="
							+PauseMenu.statistics[_i].nick);
					}
				})(i));

		PauseMenu.diplomacy[i].player_chest = new PureFW.Image(
			PauseMenu.community_slide_table.get_col(0).get_content_node(),
			PauseMenu.diplomacy[i].player_mail_image.position.x
				+PauseMenu.diplomacy[i].player_mail_image.width,
			PauseMenu.diplomacy[i].player_mail_image.position.y,
			36, 37,
			'ui/icons/labels/communication/36x37/chest.png'
		);
		PauseMenu.diplomacy[i].player_chest.set_tooltip(
			'<?= LNG_TITLE_CHEST_BUTTON ?>'.replace(/\[NICK\]/g,
				PauseMenu.statistics[i].nick)
		);
		PauseMenu.diplomacy[i].player_chest.add_event_handler("click",
			(function(_i) {
				return function(ev)
				{
					UIButtons.toggle_treasure_chest("nick="
						+PauseMenu.statistics[_i].nick);
				}
			})(i));

		y += 58;
	}
	PauseMenu.community_slide_table.get_col(0).set_inner_height(y);
};


PauseMenu.create_buddies_section = function()
{
	try {PauseMenu.buddies_section_invitingHeader.destroy() } catch(e){}
	try {PauseMenu.buddies_section_onlineHeader.destroy() } catch(e){}
	try {PauseMenu.buddies_section_offlineHeader.destroy() } catch(e){}
	try {PauseMenu.buddies_section_invitedHeader.destroy() } catch(e) {}
	try {
		var n = PauseMenu.buddies_section.length;
		for (var i = 0; i < n; i++) {
			try {PauseMenu.buddies_section[i].avatar_container.destroy();}
			catch(e){}
			try {PauseMenu.buddies_section[i].player_name_container.destroy();}
			catch(e){}
			try {PauseMenu.buddies_section[i].player_mail_image.destroy();}
			catch(e){}
			try {PauseMenu.buddies_section[i].player_chest.destroy();}
			catch(e){}
			try {PauseMenu.buddies_section[i].player_accept.destroy();}
			catch(e){}
			try {PauseMenu.buddies_section[i].player_decline.destroy();}
			catch(e){}
			try {PauseMenu.buddies_section[i].player_remove.destroy;}
			catch(e){}
		}
	}catch(e){}


	PauseMenu.buddies_section = new Array();
	var y = 10;
	PauseMenu.buddies_section = new Array();
	var n = PauseMenu.buddies.invitingFriends.length;
	if (n > 0) {
		PauseMenu.buddies_section_invitingHeader = new PureFW.Container
		(
			PauseMenu.community_slide_table.get_col(1).get_content_node(),
			8, y,
			63, 15
		);
		PauseMenu.buddies_section_invitingHeader.set_bg_img
		(
			'ui/elements/texts/<?=$LNG?>/pause_menu/requests.png'
		);
		y+=25;
	}

	for(var i = 0; i < n; i++)
	{
		var temp = new Object();
		temp.avatar_container = new PureFW.Avatar.Avatar(
			PauseMenu.community_slide_table.get_col(1).get_content_node(),
			17, y,
			48, 48,
			PauseMenu.avatars[PauseMenu.buddies.invitingFriends[i].nick]
			      			.replace('SIZE',44),
      		PauseMenu.buddies.invitingFriends[i].nick
		);

		temp.player_name_container = new PureFW.Container(
			PauseMenu.community_slide_table.get_col(1).get_content_node(),
			80, y,
			100, 20);
		temp.player_name_container.set_content(
			'<a style="color: #fff; font-weight: bold; " '
				+'href="javascript: UIButtons.toggle_window(\''
					+'profile.php?nick='+
						PauseMenu.buddies.invitingFriends[i].nick + '\', \'\', 110);">'
				+ MammunUI.get_nick_main(PauseMenu.buddies.invitingFriends[i].nick) + '</a>'
		);

		temp.player_mail_image = new PureFW.Image(
			PauseMenu.community_slide_table.get_col(1).get_content_node(),
			80, y + 20,
			36, 37,
			'ui/icons/labels/communication/36x37/pm.png'
		);
		temp.player_mail_image.set_tooltip(
			'<?= LNG_TITLE_PM_BUTTON ?>'.replace(/\[NICK\]/g,
				PauseMenu.buddies.invitingFriends[i].nick)
		);
		temp.player_mail_image.add_event_handler("click",
				(function(_i) {
					return function(ev)
					{
						UIButtons.toggle_new_message("recipients="
							+PauseMenu.buddies.invitingFriends[_i].nick);
					}
				})(i));
		temp.player_chest = new PureFW.Image(
			PauseMenu.community_slide_table.get_col(1).get_content_node(),
			temp.player_mail_image.position.x
				+temp.player_mail_image.width,
				temp.player_mail_image.position.y,
			36, 37,
			'ui/icons/labels/communication/36x37/chest.png'
		);
		temp.player_chest.set_tooltip(
			'<?= LNG_TITLE_CHEST_BUTTON ?>'.replace(/\[NICK\]/g,
				PauseMenu.buddies.invitingFriends[i].nick)
		);
		temp.player_chest.add_event_handler("click",
			(function(_i) {
				return function(ev)
				{
					UIButtons.toggle_treasure_chest("nick="
						+PauseMenu.buddies.invitingFriends[_i].nick);
				}
			})(i));

		temp.player_accept = new PureFW.Image(
				PauseMenu.community_slide_table.get_col(1).get_content_node(),
				200, y,
				14, 12,
				'ui/icons/labels/pause_menu/friendship_accept.png'
			);
		temp.player_accept.add_event_handler("click",
				(function(_i) {
					return function(ev)
					{
						PureFW.AJAXClientServer.send_request(
							"../community/wbb/index.php?form=WhiteListEdit&accept="
								+ PauseMenu.buddies.invitingFriends[_i].wcf_uid
								+ "&s=<?= $_SESSION['WCF_SID']?>"
								+ "&t=<?= $_SESSION['WCF_ST'] ?>",
							PauseMenu.refresh_buddies, null, true
						);
					}
				})(i));

		temp.player_decline = new PureFW.Image(
				PauseMenu.community_slide_table.get_col(1).get_content_node(),
				220, y,
				14, 12,
				'ui/icons/labels/pause_menu/friendship_decline.png'
			);
		temp.player_decline.add_event_handler("click",
				(function(_i) {
					return function(ev)
					{
						PureFW.AJAXClientServer.send_request(
							"../community/wbb/index.php?form=WhiteListEdit&decline="
								+ PauseMenu.buddies.invitingFriends[_i].wcf_uid
								+ "&s=<?= $_SESSION['WCF_SID']?>"
								+ "&t=<?= $_SESSION['WCF_ST'] ?>",
							PauseMenu.refresh_buddies, null, true
						);
					}
				})(i));

		PauseMenu.buddies_section.push(temp);
		y += 58;
	}

	var n = PauseMenu.buddies.onlineFriends.length;
	if (n > 0) {
		PauseMenu.buddies_section_onlineHeader = new PureFW.Container
		(
			PauseMenu.community_slide_table.get_col(1).get_content_node(),
			20, y,
			39, 14
		);
		PauseMenu.buddies_section_onlineHeader.set_bg_img
		(
				'ui/icons/labels/pause_menu/online.png'
		);
		y+=25;
	}
	for(var i = 0; i < n; i++)
	{
		var temp = new Object();
		temp.avatar_container = new PureFW.Avatar.Avatar(
			PauseMenu.community_slide_table.get_col(1).get_content_node(),
			17, y,
			48, 48,
			PauseMenu.avatars[PauseMenu.buddies.onlineFriends[i].nick]
			      			.replace('SIZE',44),
      		PauseMenu.buddies.onlineFriends[i].nick
		);

		temp.player_name_container = new PureFW.Container(
			PauseMenu.community_slide_table.get_col(1).get_content_node(),
			80, y,
			100, 20);
		temp.player_name_container.set_content(
			'<a style="color: #fff; font-weight: bold; " '
				+'href="javascript: UIButtons.toggle_window(\''
					+'profile.php?nick='+
						PauseMenu.buddies.onlineFriends[i].nick + '\', \'\', 110);">'
				+ MammunUI.get_nick_main(PauseMenu.buddies.onlineFriends[i].nick)
				+ '</a>'
		);

		temp.player_mail_image = new PureFW.Image(
			PauseMenu.community_slide_table.get_col(1).get_content_node(),
			80, y + 20,
			36, 37,
			'ui/icons/labels/communication/36x37/pm.png'
		);
		temp.player_mail_image.set_tooltip(
			'<?= LNG_TITLE_PM_BUTTON ?>'.replace(/\[NICK\]/g,
				PauseMenu.buddies.onlineFriends[i].nick)
		);
		temp.player_mail_image.add_event_handler("click",
				(function(_i) {
					return function(ev)
					{
						UIButtons.toggle_new_message("recipients="
							+PauseMenu.buddies.onlineFriends[_i].nick);
					}
				})(i));
		temp.player_chest = new PureFW.Image(
			PauseMenu.community_slide_table.get_col(1).get_content_node(),
			temp.player_mail_image.position.x
				+temp.player_mail_image.width,
				temp.player_mail_image.position.y,
			36, 37,
			'ui/icons/labels/communication/36x37/chest.png'
		);
		temp.player_chest.set_tooltip(
			'<?= LNG_TITLE_CHEST_BUTTON ?>'.replace(/\[NICK\]/g,
				PauseMenu.buddies.onlineFriends[i].nick)
		);
		temp.player_chest.add_event_handler("click",
			(function(_i) {
				return function(ev)
				{
					UIButtons.toggle_treasure_chest("nick="
						+PauseMenu.buddies.onlineFriends[_i].nick);
				}
			})(i));

		temp.player_remove = new PureFW.Image(
				PauseMenu.community_slide_table.get_col(1).get_content_node(),
				220, y,
				14, 12,
				'ui/icons/labels/pause_menu/friends_remove.png'
			);
		temp.player_remove.add_event_handler("click",
				(function(_i) {
					return function(ev)
					{
						PureFW.AJAXClientServer.send_request(
							"../community/wbb/index.php?form=WhiteListEdit&remove="
								+ PauseMenu.buddies.onlineFriends[_i].wcf_uid
								+ "&u=<?=$WCF_user_ID?>"
								+ "&s=<?= $_SESSION['WCF_SID']?>"
								+ "&t=<?= $_SESSION['WCF_ST'] ?>",
							PauseMenu.refresh_buddies, null, true
						);
					}
				})(i));

		y += 58;
		PauseMenu.buddies_section.push(temp);
	}


	var n = PauseMenu.buddies.offlineFriends.length;
	if (n > 0) {
		PauseMenu.buddies_section_offlineHeader = new PureFW.Container
		(
			PauseMenu.community_slide_table.get_col(1).get_content_node(),
			18, y,
			48, 15
		);
		PauseMenu.buddies_section_offlineHeader.set_bg_img
		(
				'/ui/icons/labels/pause_menu/offline.png'
		);
		y+=25;
	}

	for(var i = 0; i < n; i++)
	{
		var temp = new Object();
		temp.avatar_container = new PureFW.Avatar.Avatar(
			PauseMenu.community_slide_table.get_col(1).get_content_node(),
			17, y,
			48, 48,
			PauseMenu.avatars[PauseMenu.buddies.offlineFriends[i].nick]
			      			.replace('SIZE',44),
      		PauseMenu.buddies.offlineFriends[i].nick
		);

		temp.player_name_container = new PureFW.Container(
			PauseMenu.community_slide_table.get_col(1).get_content_node(),
			80, y,
			100, 20);
		temp.player_name_container.set_content(
			'<a style="color: #fff; font-weight: bold; " '
				+'href="javascript: UIButtons.toggle_window(\''
					+'profile.php?nick='+
						PauseMenu.buddies.offlineFriends[i].nick + '\', \'\', 110);">'
				+ MammunUI.get_nick_main(PauseMenu.buddies.offlineFriends[i].nick)
				+ '</a>'
		);

		temp.player_mail_image = new PureFW.Image(
			PauseMenu.community_slide_table.get_col(1).get_content_node(),
			80, y + 20,
			36, 37,
			'ui/icons/labels/communication/36x37/pm.png'
		);
		temp.player_mail_image.set_tooltip(
			'<?= LNG_TITLE_PM_BUTTON ?>'.replace(/\[NICK\]/g,
				PauseMenu.buddies.offlineFriends[i].nick)
		);
		temp.player_mail_image.add_event_handler("click",
				(function(_i) {
					return function(ev)
					{
						UIButtons.toggle_new_message("recipients="
							+PauseMenu.buddies.offlineFriends[_i].nick);
					}
				})(i));
		temp.player_chest = new PureFW.Image(
			PauseMenu.community_slide_table.get_col(1).get_content_node(),
			temp.player_mail_image.position.x
				+temp.player_mail_image.width,
				temp.player_mail_image.position.y,
			36, 37,
			'ui/icons/labels/communication/36x37/chest.png'
		);
		temp.player_chest.set_tooltip(
			'<?= LNG_TITLE_CHEST_BUTTON ?>'.replace(/\[NICK\]/g,
				PauseMenu.buddies.offlineFriends[i].nick)
		);
		temp.player_chest.add_event_handler("click",
			(function(_i) {
				return function(ev)
				{
					UIButtons.toggle_treasure_chest("nick="
						+PauseMenu.buddies.offlineFriends[_i].nick);
				}
			})(i));

		temp.player_remove = new PureFW.Image(
			PauseMenu.community_slide_table.get_col(1).get_content_node(),
			220, y,
			14, 12,
			'ui/icons/labels/pause_menu/friends_remove.png'
		);
		temp.player_remove.add_event_handler("click",
				(function(_i) {
					return function(ev)
					{
						PureFW.AJAXClientServer.send_request(
							"../community/wbb/index.php?form=WhiteListEdit&remove="
								+ PauseMenu.buddies.offlineFriends[_i].wcf_uid
								+"&u=<?=$WCF_user_ID?>"
								+ "&s=<?= $_SESSION['WCF_SID']?>"
								+ "&t=<?= $_SESSION['WCF_ST'] ?>",
							PauseMenu.refresh_buddies, null, true
						);
					}
				})(i));

		y += 58;
		PauseMenu.buddies_section.push(temp);
	}

	if (n > 0) {
		var n = PauseMenu.buddies.invitedFriends.length;
		PauseMenu.buddies_section_invitedHeader = new PureFW.Container
		(
			PauseMenu.community_slide_table.get_col(1).get_content_node(),
			1, y,
			79, 15
		);
		PauseMenu.buddies_section_invitedHeader.set_bg_img
		(
			'ui/elements/texts/<?=$LNG?>/pause_menu/pending.png'
		);
		y+=25;
	}
	for(var i = 0; i < n; i++)
	{
		var temp = new Object();
		temp.avatar_container = new PureFW.Avatar.Avatar(
			PauseMenu.community_slide_table.get_col(1).get_content_node(),
			17, y,
			48, 48,
			PauseMenu.avatars[PauseMenu.buddies.invitedFriends[i].nick]
			      			.replace('SIZE',44),
      		PauseMenu.buddies.invitedFriends[i].nick
		);

		temp.player_name_container = new PureFW.Container(
			PauseMenu.community_slide_table.get_col(1).get_content_node(),
			80, y,
			100, 20);
		temp.player_name_container.set_content(
			'<a style="color: #fff; font-weight: bold; " '
				+'href="javascript: UIButtons.toggle_window(\''
					+'profile.php?nick='+
						PauseMenu.buddies.invitedFriends[i].nick + '\', \'\', 110);">'
				+ MammunUI.get_nick_main(PauseMenu.buddies.invitedFriends[i].nick)
				+ '</a>'
		);

		temp.player_mail_image = new PureFW.Image(
			PauseMenu.community_slide_table.get_col(1).get_content_node(),
			80, y + 20,
			36, 37,
			'ui/icons/labels/communication/36x37/pm.png'
		);
		temp.player_mail_image.set_tooltip(
			'<?= LNG_TITLE_PM_BUTTON ?>'.replace(/\[NICK\]/g,
				PauseMenu.buddies.invitedFriends[i].nick)
		);
		temp.player_mail_image.add_event_handler("click",
				(function(_i) {
					return function(ev)
					{
						UIButtons.toggle_new_message("recipients="
							+PauseMenu.buddies.invitedFriends[_i].nick);
					}
				})(i));
		temp.player_chest = new PureFW.Image(
			PauseMenu.community_slide_table.get_col(1).get_content_node(),
			temp.player_mail_image.position.x
				+temp.player_mail_image.width,
				temp.player_mail_image.position.y,
			36, 37,
			'ui/icons/labels/communication/36x37/chest.png'
		);
		temp.player_chest.set_tooltip(
			'<?= LNG_TITLE_CHEST_BUTTON ?>'.replace(/\[NICK\]/g,
				PauseMenu.buddies.invitedFriends[i].nick)
		);
		temp.player_chest.add_event_handler("click",
			(function(_i) {
				return function(ev)
				{
					UIButtons.toggle_treasure_chest("nick="
						+PauseMenu.buddies.invitedFriends[_i].nick);
				}
			})(i));

		temp.player_cancel = new PureFW.Image(
				PauseMenu.community_slide_table.get_col(1).get_content_node(),
				220, y,
				14, 12,
				'ui/icons/labels/pause_menu/friend_request_cancel.png'
			);
		temp.player_cancel.add_event_handler("click",
				(function(_i) {
					return function(ev)
					{
						PureFW.AJAXClientServer.send_request(
							"../community/wbb/index.php?form=WhiteListEdit&cancel="
								+ PauseMenu.buddies.invitedFriends[_i].wcf_uid
								+"&t=<?=$_SESSION['WCF_ST']?>"
								+ "&s=<?= $_SESSION['WCF_SID']?>"
								+ "&t=<?= $_SESSION['WCF_ST'] ?>",
							PauseMenu.refresh_buddies, null, true
						);
					}
				})(i));

		y += 58;
		PauseMenu.buddies_section.push(temp);
	}
	PauseMenu.community_slide_table.get_col(1).set_inner_height(y);
};


PauseMenu.create_world_section = function()
{
	PauseMenu.world_section = new Array();
	var n = PauseMenu.world_users.length;
	var y = 10;
	for(var i = 0; i < n; i++)
	{
		PauseMenu.world_section[i] = new Object();
		PauseMenu.world_section[i].avatar_container = new PureFW.Avatar.Avatar(
			PauseMenu.community_slide_table.get_col(2).get_content_node(),
			17, y,
			48, 48,
			PauseMenu.avatars[PauseMenu.world_users[i].nick].replace('SIZE',44),
			PauseMenu.world_users[i].nick
		);

		PauseMenu.world_section[i].player_name_container = new PureFW.Container(
			PauseMenu.community_slide_table.get_col(2).get_content_node(),
			80, y,
			100, 20);
		PauseMenu.world_section[i].player_name_container.set_content(
			'<a style="color: #fff; font-weight: bold; " '
				+'href="javascript: UIButtons.toggle_window(\''
					+'profile.php?nick='+
						PauseMenu.world_users[i].nick + '\', \'\', 110);">'
				+ MammunUI.get_nick_main(PauseMenu.world_users[i].nick)
				+ '</a>'
		);

		PauseMenu.world_section[i].player_mail_image = new PureFW.Image(
			PauseMenu.community_slide_table.get_col(2).get_content_node(),
			80, y + 20,
			36, 37);
		PauseMenu.world_section[i].player_mail_image.set_pic_url(
				'ui/icons/labels/communication/36x37/pm.png');
		PauseMenu.world_section[i].player_mail_image.set_tooltip(
			'<?= LNG_TITLE_PM_BUTTON ?>'.replace(/\[NICK\]/g,
				PauseMenu.world_users[i].nick)
		);
		PauseMenu.world_section[i].player_mail_image.add_event_handler("click",
				(function(_i) {
					return function(ev)
					{
						UIButtons.toggle_new_message("recipients="
							+PauseMenu.world_users[_i].nick);
					}
				})(i));
		PauseMenu.world_section[i].player_chest = new PureFW.Image(
			PauseMenu.community_slide_table.get_col(2).get_content_node(),
			PauseMenu.world_section[i].player_mail_image.position.x
				+PauseMenu.world_section[i].player_mail_image.width,
				PauseMenu.world_section[i].player_mail_image.position.y,
			36, 37,
			'ui/icons/labels/communication/36x37/chest.png'
		);
		PauseMenu.world_section[i].player_chest.set_tooltip(
			'<?= LNG_TITLE_CHEST_BUTTON ?>'.replace(/\[NICK\]/g,
				PauseMenu.world_users[i].nick)
		);
		PauseMenu.world_section[i].player_chest.add_event_handler("click",
			(function(_i) {
				return function(ev)
				{
					UIButtons.toggle_treasure_chest("nick="
						+PauseMenu.world_users[_i].nick);
				}
			})(i));

		y += 58;
	}
	PauseMenu.community_slide_table.get_col(2).set_inner_height(y);
};
<? /**
    * Funktion zum Anzeigen des Forums
    */ ?>
PauseMenu.show_board = function () {
	alert('Not implemented, yet');
}


<? /**
    * Funktion zum anzeigen der Einstellungen / Optionen.
    */ ?>
PauseMenu.show_options = function () {
	alert('Not implemented, yet');
}

<? /**
    * Verlassen der Karte. Es poppt zunächst eine ConfirmationBox auf, welche
    * dann bestätigt werden muss. Dann wird die Insel unwiderruflich verlassen.
    */ ?>
PauseMenu.leave_map = function() {
	<? if ($i_have_won) : ?>
	var text = '<?= LNG_LEAVE_MAP_CONFIRMATION_WON ?>';
	<? else : ?>
	var text = '<?= LNG_LEAVE_MAP_CONFIRMATION ?>';
	<? endif; ?>
	PauseMenu.leave_map_confirmation =
		MammunUI.show_warning(360, 255, text, null);

	PauseMenu.leave_map_confirmation.add_event_handler("confirm",
		function() {
			PureFW.AJAXClientServer.send_request(
					"ingame.php?dataX=<?=$cryptObj->do_encryption(
						array("leave_map"=>true))?>",
						PauseMenu.leave_map_fetch_result
			);
		}
	);


}

<? /**
    * Nach Verlassen der Insel wird die Server-Antwort hier verarbeitet.
    */ ?>
PauseMenu.leave_map_fetch_result = function(response_arr) {
	if (!(response_arr[0] != '1' && response_arr[0] != '0')) {
		ui_auto_logout = false;
		var add_str = '';
		if (response_arr[1] && response_arr[2]) {
			add_str = '&join=true&world_id=' + response_arr[1]
				+ '&map_id=' + response_arr[2];
		}
			
		document.location.href = 'ingame.php?start=true' + add_str;
	}
	else if (response_arr[0] == '!') {
		throw "Fehler: " + response_arr[1];
	}
	else
		throw response_arr;
}

<? /**
    * Funktion zum Ausloggen des Spielers.
    */ ?>
PauseMenu.logout = function () {
	document.location.href = "logout.php";
}

<? /**
    * Destruktor. Wird aufgerufen,wenn das Fenster geschlossen wird und räumt
    * alles an Objekten hinter sich auf.
    */ ?>
PauseMenu.destroy = function() {
	PauseMenu.this_window.remove_event_handler("destroy", PauseMenu.destroy);
	PauseMenu.this_window.remove_event_handler("change", PauseMenu.destroy);
	for (var x in PauseMenu) {
		<?/* Nicht das aktuelle Fenster zerstören. Das zerstört sich entweder
		   * selbst, oder es möchte nicht zerstört werden - ganz einfach :)
		   * Wir haben hier nicht die Berechtigung dort einzugreifen!*/?>
		if (x == 'this_window')
			continue;
		try {
			PauseMenu[x].destroy();
		}
		catch (e) {
			delete PauseMenu[x];
		}
	}
	delete PauseMenu;
}

<?
/******************************************************************************
 *                  /\     .---.    -----    -     .---.   .  .               *
 *                 /__\    |          |      |     |   |   |`.|               *
 *                /    \   `---'      -      -     `---'   '  '               *
 *                                                                            *
 *                             A  C  T  I  O  N                            PK *
 ******************************************************************************/
?>
<? /* AKTIVITÄTEN-Liste */ ?>
PauseMenu.activity_box = new PureFW.Container(
	PauseMenu.this_window,
	25, 45,
	424, 193
);
PauseMenu.activity_box.set_bg_img(
	'ui/backgrounds/pause_menu/activity_bg.png'
);


PauseMenu.activity_box_content = new PureFW.ScrollContainer(
	PauseMenu.activity_box,
	10, 20,
	PauseMenu.activity_box.width-20, 0
);
PauseMenu.activity_box_content.set_font_color('#333333');

PauseMenu.activity_box_header = new PureFW.Image(
	PauseMenu.this_window,
	PauseMenu.activity_box.position.x,
	PauseMenu.activity_box.position.y - 20,
	132, 40,
	'ui/elements/texts/<?=$LNG?>/pause_menu/th_activities.png'
);
PauseMenu.activity_box_header.set_z_index(
	PauseMenu.activity_box.get_z_index() + 1
);
PauseMenu.create_event_log(12);

<? /* NAVI-LINKS (ja Button + Text + OnHover-Tooltip-Box) */ ?>
<? /* Settings */ ?>
PauseMenu.button_settings = new PureFW.Image(
	PauseMenu.this_window,
	30, 300,
	67, 48,
	'ui/icons/labels/pause_menu/i_settings.png'
);
//PauseMenu.button_settings.add_event_handler("click", PauseMenu.settings);
PauseMenu.button_settings.add_event_handler("mouseover",
	function(ev) {
		PauseMenu.tip_settings.show()
	}
);
PauseMenu.button_settings.add_event_handler("mouseout",
	function(ev) {
		PauseMenu.tip_settings.hide()
	}
);
PauseMenu.button_settings.add_event_handler("click", UIButtons.toggle_settings_window);

PauseMenu.text_settings = new PureFW.Image(
	PauseMenu.this_window,
	PauseMenu.button_settings.position.x
		+ PauseMenu.button_settings.width,
	PauseMenu.button_settings.position.y,
	181,
	PauseMenu.button_settings.height,
	'ui/elements/texts/<?=$LNG?>/pause_menu/settings.png'
);

PauseMenu.text_settings.add_event_handler("click", UIButtons.toggle_settings_window);

//PauseMenu.text_settings.add_event_handler("click", PauseMenu.settings);
PauseMenu.text_settings.add_event_handler("mouseover",
	function(ev) {
		PauseMenu.tip_settings.show()
	}
);
PauseMenu.text_settings.add_event_handler("mouseout",
	function(ev) {
		PauseMenu.tip_settings.hide()
	}
);


PauseMenu.tip_settings = new PureFW.Container(
	PauseMenu.this_window,
	PauseMenu.button_settings.position.x + 75,
	PauseMenu.button_settings.position.y
		+ PauseMenu.button_settings.height-5,
	170,
	35
);
PauseMenu.tip_settings.set_text_align('center');
PauseMenu.tip_settings.set_font_color('#333333');
PauseMenu.tip_settings.set_content('<?= LNG_CHANGE_SETTINGS_HERE ?>');
PauseMenu.tip_settings.hide();
PauseMenu.text_settings.set_z_index(PauseMenu.tip_settings.get_z_index()+1);

<? /* Logout */?>
<? /*PauseMenu.button_logout = new PureFW.Image(
	PauseMenu.this_window,
	40, 370,
	37, 47,
	'ui/icons/labels/pause_menu/i_logout.png'
);
PauseMenu.button_logout.add_event_handler("click", PauseMenu.logout);

PauseMenu.text_logout = new PureFW.Image(
		PauseMenu.this_window,
		PauseMenu.button_logout.position.x
			+ PauseMenu.button_logout.width,
		PauseMenu.button_logout.position.y,
		157,
		PauseMenu.button_logout.height,
		'ui/elements/texts/<?=$LNG?>/pause_menu/logout.png'
);
PauseMenu.text_logout.add_event_handler("click", PauseMenu.logout);*/?>

<?/* Surrender */?>
<? if (User::get_current_user()->get_state() == User::STATE_PLAYING) : ?>
PauseMenu.button_surrender = new PureFW.Image(
	PauseMenu.this_window,
	40, 480,
	36, 43,
	'ui/icons/labels/pause_menu/i_leave_island.png'
);
PauseMenu.button_surrender.add_event_handler("click", PauseMenu.leave_map);
PauseMenu.button_surrender.add_event_handler("mouseover",
	function (ev) {
		PauseMenu.tip_surrender.show()
	}
);
PauseMenu.button_surrender.add_event_handler("mouseoout",
	function (ev) {
		PauseMenu.tip_surrender.hide()
	}
);

<? if ($i_have_won) : ?>
PauseMenu.text_surrender = new PureFW.Image(
	PauseMenu.this_window,
	PauseMenu.button_surrender.position.x
		+ PauseMenu.button_surrender.width,
		PauseMenu.button_surrender.position.y,
	281,
	PauseMenu.button_surrender.height,
	'ui/elements/texts/<?=$LNG?>/pause_menu/leave_island_won.png'
);
<? else : ?>
PauseMenu.text_surrender = new PureFW.Image(
	PauseMenu.this_window,
	PauseMenu.button_surrender.position.x
		+ PauseMenu.button_surrender.width + 20,
		PauseMenu.button_surrender.position.y,
	150,
	PauseMenu.button_surrender.height,
	'ui/elements/texts/<?=$LNG?>/pause_menu/new_island.png'
);
<? endif; ?>
PauseMenu.text_surrender.add_event_handler("click", PauseMenu.leave_map);
PauseMenu.text_surrender.add_event_handler("mouseover",
	function (ev) {
		PauseMenu.tip_surrender.show()
	}
);
PauseMenu.text_surrender.add_event_handler("mouseout",
	function (ev) {
		PauseMenu.tip_surrender.hide()
	}
);

PauseMenu.tip_surrender = new PureFW.Container(
	PauseMenu.this_window,
	PauseMenu.button_surrender.position.x + 75,
	PauseMenu.button_surrender.position.y
		+ PauseMenu.button_surrender.height-10,
	170,
	35
);
PauseMenu.tip_surrender.set_z_index(PauseMenu.text_surrender.get_z_index()+1);
PauseMenu.tip_surrender.set_text_align('center');
PauseMenu.tip_surrender.set_font_color('#333333');
PauseMenu.tip_surrender.set_content('<?= LNG_LEAVE_ISLAND ?>');
PauseMenu.tip_surrender.hide();

<? endif; ?>

<? /* Community-Slide-Tabelle rechts */ ?>

PauseMenu.community_slide_table = new SlideTable(
		PauseMenu.this_window,
		460,
		50,
		530,
		464
);
PauseMenu.community_slide_table.set_font_size(1.1);
PauseMenu.community_slide_table.set_closed_col_width(112);
PauseMenu.community_slide_table.add_col();
PauseMenu.community_slide_table.add_col();

PauseMenu.community_slide_table.get_node().style.overflow = 'visible';
PauseMenu.community_slide_table.get_col(0).get_node().style.overflow = 'visible';
PauseMenu.community_slide_table.get_col(1).get_node().style.overflow = 'visible';
PauseMenu.community_slide_table.get_col(2).get_node().style.overflow = 'visible';

PauseMenu.community_slide_table.get_col(0).add_event_handler(
		"shrink",
		function()
		{
			var n = PauseMenu.statistics.length;
			for(var i = 0; i < n; i++)
			{
				PauseMenu.diplomacy[i].player_pic_container.set_x(17);
				PauseMenu.diplomacy[i].player_name_container.hide();
				try{PauseMenu.diplomacy[i].player_score_container.hide();}
				catch(e){}
				PauseMenu.diplomacy[i].player_mail_image.hide();
				PauseMenu.diplomacy[i].player_pic_container.deactivate();
			}
		}
);

PauseMenu.community_slide_table.get_col(0).add_event_handler(
		"grow",
		function()
		{
			var n = PauseMenu.statistics.length;
			for(var i = 0; i < n; i++)
			{
				PauseMenu.diplomacy[i].player_pic_container.set_x(0);
				PauseMenu.diplomacy[i].player_name_container.show();
				try{PauseMenu.diplomacy[i].player_score_container.show();}
				catch(e){}
				PauseMenu.diplomacy[i].player_mail_image.show();
				PauseMenu.diplomacy[i].player_pic_container.activate();
			}
		}
);

PauseMenu.community_slide_table.get_col(1).add_event_handler(
		"shrink",
		function()
		{
			if(!(typeof(PauseMenu.buddies_section_onlineHeader)=='undefined'))
				PauseMenu.buddies_section_onlineHeader.set_x(20);
			if(!(typeof(PauseMenu.buddies_section_offlineHeader)=='undefined'))
				PauseMenu.buddies_section_offlineHeader.set_x(18);
			if(!(typeof(PauseMenu.buddies_section_invitingHeader)=='undefined'))
				PauseMenu.buddies_section_invitingHeader.set_x(8);
			if(!(typeof(PauseMenu.buddies_section_invitedHeader)=='undefined'))
				PauseMenu.buddies_section_invitedHeader.set_x(1);
			var n = PauseMenu.buddies_section.length;
			for(var i = 0; i < n; i++)
			{
				PauseMenu.buddies_section[i].avatar_container.set_x(17);
				PauseMenu.buddies_section[i].avatar_container.deactivate();
				PauseMenu.buddies_section[i].player_name_container.hide();
				PauseMenu.buddies_section[i].player_mail_image.hide();
			}
		}
);

PauseMenu.community_slide_table.get_col(1).add_event_handler(
		"grow",
		function()
		{
			if(!(typeof(PauseMenu.buddies_section_onlineHeader)=='undefined'))
				PauseMenu.buddies_section_onlineHeader.set_x(109);
			if(!(typeof(PauseMenu.buddies_section_offlineHeader)=='undefined'))
				PauseMenu.buddies_section_offlineHeader.set_x(108);
			if(!(typeof(PauseMenu.buddies_section_invitingHeader)=='undefined'))
				PauseMenu.buddies_section_invitingHeader.set_x(92);
			if(!(typeof(PauseMenu.buddies_section_invitedHeader)=='undefined'))
				PauseMenu.buddies_section_invitedHeader.set_x(88);
			var n = PauseMenu.buddies_section.length;
			var n = PauseMenu.buddies_section.length;
			for(var i = 0; i < n; i++)
			{
				PauseMenu.buddies_section[i].avatar_container.set_x(0);
				PauseMenu.buddies_section[i].avatar_container.activate();
				PauseMenu.buddies_section[i].player_name_container.show();
				PauseMenu.buddies_section[i].player_mail_image.show();
			}
		}
);

PauseMenu.community_slide_table.get_col(2).add_event_handler(
	"shrink",
	function()
	{
		var n = PauseMenu.world_section.length;
		for(var i = 0; i < n; i++)
		{
			PauseMenu.world_section[i].avatar_container.set_x(17);
			PauseMenu.world_section[i].avatar_container.deactivate();
			PauseMenu.world_section[i].player_name_container.hide();
			PauseMenu.world_section[i].player_mail_image.hide();
		}
	}
);

PauseMenu.community_slide_table.get_col(2).add_event_handler(
	"grow",
	function()
	{
		var n = PauseMenu.world_section.length;
		for(var i = 0; i < n; i++)
		{
			PauseMenu.world_section[i].avatar_container.set_x(0);
			PauseMenu.world_section[i].avatar_container.activate();
			PauseMenu.world_section[i].player_name_container.show();
			PauseMenu.world_section[i].player_mail_image.show();
		}
	}
);

PauseMenu.slide_table_playmates_image = new PureFW.Image(
		PauseMenu.community_slide_table.get_col(0).get_node(),
		0, -30,
		112, 40,
		'ui/elements/texts/<?=$LNG?>/pause_menu/th_playmates.png');

PauseMenu.slide_table_friends_image = new PureFW.Image(
		PauseMenu.community_slide_table.get_col(1).get_node(),
		0, -30,
		112, 48,
		'ui/elements/texts/<?=$LNG?>/pause_menu/th_friends.png');

PauseMenu.slide_table_worldwide_image = new PureFW.Image(
		PauseMenu.community_slide_table.get_col(2).get_node(),
		0, -25,
		113, 39,
		'ui/elements/texts/<?=$LNG?>/pause_menu/th_worldwide.png');

PauseMenu.create_diplomacy_section();
PauseMenu.create_buddies_section();
PauseMenu.create_world_section();
<? if ((User::get_current_user()->get_state() == IUser::STATE_PLAYING)
	|| (User::get_current_user()->get_waiting_state() == 0)) : ?>
PauseMenu.community_slide_table.open_col(0);
<? else :?>
PauseMenu.community_slide_table.open_col(2);
<? endif; ?>

<?
/**
 * Freunde-Einladen-Button plus Fähnchen/Label.
 */ ?>

PauseMenu.invite_friends_label = new PureFW.Image(
	PauseMenu.this_window,
	PauseMenu.community_slide_table.position.x+3,
	PauseMenu.community_slide_table.position.y
		+ PauseMenu.community_slide_table.height - 17,
	205, 43,
	'ui/elements/texts/<?=$LNG?>/pause_menu/invite_friends_button_label.png'
);
PauseMenu.invite_friends_label.set_z_index(
	PauseMenu.community_slide_table.get_z_index() + 1
);
PauseMenu.invite_friends_label.add_event_handler("click",
	function(ev) {
		UIButtons.toggle_friend_invitation();
	}
);
PauseMenu.invite_friends = new PureFW.Image(
	PauseMenu.this_window,
	PauseMenu.community_slide_table.position.x+5,
	PauseMenu.community_slide_table.position.y
		+PauseMenu.community_slide_table.height + 8,
	202, 30,
	'ui/elements/form/buttons/<?=$LNG?>/pause_menu/invite_friends.png'
);
PauseMenu.invite_friends.set_z_index(
	PauseMenu.invite_friends_label.get_z_index() + 1
);
PauseMenu.invite_friends.add_event_handler("click",
	function(ev) {
		UIButtons.toggle_friend_invitation();
	}
);

PureFW.Timeout.set_interval(PauseMenu.rt_refresh, 10000);
PauseMenu.this_window.add_event_handler("destroy", PauseMenu.destroy);
PauseMenu.this_window.add_event_handler("change", PauseMenu.destroy);
PauseMenu.loaded = true;
</script>
