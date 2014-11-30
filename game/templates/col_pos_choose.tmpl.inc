<? Language::get_instance()->load_language_file("col_pos_choose") ?>
<script type="text/javascript">
ColPosChoose = new Object();
ColPosChoose.loaded = false;

ColPosChoose.this_window = PureFW.ContainerAJAX.instances[<?=$this_win_id?>];
ColPosChoose.this_window.set_font_size(1.3);
ColPosChoose.this_window.set_font_color('#333');
ColPosChoose.this_window.set_text_shadow('#fff', 1, 0, 0);

ColPosChoose.homesectors = <?= json_encode($homesectors) ?>;
ColPosChoose.players_on_map = <?= json_encode($players_on_map) ?>;
ColPosChoose.players_on_map_linear = <?= json_encode($players_on_map_linear) ?>;
ColPosChoose.player_me = <?= json_encode($player_me) ?>;
ColPosChoose.avatars = <?= json_encode($avatars) ?>;
ColPosChoose.colors =
	{
		"blue":		true,
		"green":	true,
		"orange":	true,
		"pink":		true,
		"purple":	true,
		"red":		true,
		"white":	true,
		"yellow":	true
	};

<? if (User::get_current_user()->get_state() == IUser::STATE_PLAYING) : ?>
ColPosChoose.chosen_home = new PureFW.Point(
	<?= User::get_current_user()->get_homesector()%1000?>,
	<?= floor((User::get_current_user()->get_homesector()%100)/10)?>
);
<? else : ?>
ColPosChoose.chosen_home = null;
<? endif; ?>
<? if ($invite_mode) : ?>
ColPosChoose.has_opened_invitation_screen = false
<? endif; ?>


ColPosChoose.home_click = function(ev) {
	<? /**
		* Wenn der Spieler noch nicht spielt, dann kann er den Heimatsektor
		* durch Klicken wählen. Sonst passiert hier nichts.
		*/ ?>
	<? if (User::get_current_user()->get_state() != IUser::STATE_PLAYING) : ?>
	var x = ev.detail.x; var y = ev.detail.y;
	if (ColPosChoose.preview_map.is_empty(x,y)) {
		ColPosChoose.preview_map.set_player(
			x, y,
			ColPosChoose.player_me.nick,
			ColPosChoose.player_me.color,
			ColPosChoose.avatars[ColPosChoose.player_me.nick].replace("SIZE",44)
		);
		ColPosChoose.chosen_home = new PureFW.Point(x,y);
		ColPosChoose.preview_map.refresh();
	}
	<? endif; ?>
}


ColPosChoose.create_enemies_section = function() {
	ColPosChoose.players.remove_all_widgets();
	ColPosChoose.enemies = new Array();
	var n = ColPosChoose.homesectors.length-1;
	var x = 0;
	var y = 0;
	var cols_used = new Object();
	for (var i in ColPosChoose.colors)
	 	cols_used[i] = ColPosChoose.colors[i];

	for(var i = 0, l = 0; l < n; l++) {
		if (ColPosChoose.players_on_map_linear[l]) {
			var nick = ColPosChoose.players_on_map_linear[l].nick;
			var avatar = ColPosChoose.avatars[nick].replace("SIZE",44);
			var color = ColPosChoose.players_on_map_linear[l].color;
			ColPosChoose.colors[color] = false;
		}
		else {
			var avatar = 'pattern/spacer.gif'
			var nick = '';
			for (var c in cols_used) {
				if (cols_used[c]) {
					var color = c;
					break;
				}
			}
		}
		if (nick == '<?= User::get_current_user()->get_nick() ?>') {
			n++;
			continue;
		}

		ColPosChoose.enemies[i] = new Object();

		ColPosChoose.enemies[i].player_pic_container
			= ColPosChoose.players.add_widget
		(
			PureFW.Avatar.Avatar,
			x, y,
			48, 48,
			avatar, nick
		);

		ColPosChoose.enemies[i].player_color_container =
			ColPosChoose.players.add_widget
		(
			PureFW.Image,
			x+53,y+25,
			20,20,
			"ui/icons/labels/col_pos_choose/"+color+".png"
		);

		if (ColPosChoose.players_on_map_linear[l]) {
			ColPosChoose.enemies[i].player_name_container
				= ColPosChoose.players.add_widget
			(
				PureFW.Container,
				x+53, y,
				0, 0
			);

			ColPosChoose.enemies[i].player_name_container.set_content(
				'<a style="color: #333" '
				+ 'href="javascript: UIButtons.toggle_window(\''
				+ 'profile.php?nick=' + nick +
														'\', \'\', 110);">'
				+ MammunUI.beautify_nick(nick)
				+ "</a>"
			);
		}
		else {
			ColPosChoose.enemies[i].player_color_container.set_opacity(0.5);
			ColPosChoose.enemies[i].player_name_container
				= ColPosChoose.players.add_widget
			(
				PureFW.Image,
				x+53, y,
				149, 36,
				<? if ($invite_mode) : ?>
				'ui/elements/texts/<?=$LNG?>/col_pos_choose/invite_me.png'
				<? else : ?>
				'ui/elements/texts/<?=$LNG?>/col_pos_choose/free_place.png'
				<? endif; ?>
			);
			<? if ($invite_mode) : ?>
			ColPosChoose.enemies[i].player_name_container.add_event_handler(
				"click", function(ev) {
					ColPosChoose.has_opened_invitation_screen = true;
					ColPosChoose.letsago_button.set_pic_url(
						"ui/elements/form/buttons/<?=$LNG?>/col_pos_choose/go_on.png"
					);
					UIButtons.open_friend_invitation();
				}
			);
			ColPosChoose.enemies[i].player_pic_container.add_event_handler(
				"click", (function(_i) {
					return function(ev) {
						ColPosChoose.enemies[_i].player_name_container
								.on_click();
					}
				})(i)
			);
			<? endif; ?>
		}


		cols_used[color] = false;

		if ((i | 1) != i) {
			x = 210;
		}
		else {
			x = 0;
			y += 53;
		}
		i++;
	}

	if (ColPosChoose.player_me.color == 'neutral') {
		for (var c in ColPosChoose.colors) {
			if (ColPosChoose.colors[c]) {
				ColPosChoose.player_me.color = c;
				ColPosChoose.set_my_color(c);
				break;
			}
		}
	}

	<? /**
		* Wenn im Invite-Mode, dann sollen die leeren Avatare mit FB-Freunden
		* gefüllt werden.
		*/ ?>
	<? if ($invite_mode) : ?>
	PureFW.AJAXClientServer.send_request(
		"invitation.php?get_facebook_friends=true",
		function(response_arr) {
			var friends = eval("("+response_arr[0]+")");
			var n = ColPosChoose.enemies.length;
			for (var i = 0; i < n; i++) {
				if (ColPosChoose.enemies[i].player_pic_container.get_nick()=='')
				{
					var friend = friends.shift();
					ColPosChoose.enemies[i].player_pic_container.set_pic_url(
						friend.avatar.replace("SIZE", 44)
					);
				}
			}
		}
	);
	<? endif; ?>
};


ColPosChoose.create_homes = function() {
	for (var i in ColPosChoose.homesectors) {
		if (isNaN(i))
			continue;
		var hs = ColPosChoose.homesectors[i];

		var x = hs % 10;
		var y = Math.floor(hs/10);

		ColPosChoose.preview_map.add_homesector(x, y);
	}

	for (var x in ColPosChoose.players_on_map) {
		if (isNaN(x))
			continue;
		for (var y in ColPosChoose.players_on_map[x]) {
			if (isNaN(y))
				continue;
			var player = ColPosChoose.players_on_map[x][y];

			ColPosChoose.preview_map.set_player(
				x, y,
				player.nick,
				player.color,
				ColPosChoose.avatars[player.nick].replace("SIZE", 44)
			);
		}
	}
	ColPosChoose.preview_map.refresh();
}

ColPosChoose.set_my_color = function(c) {
	ColPosChoose.colors[ColPosChoose.player_me.color] = true;
	ColPosChoose.colors[c] = false;
	ColPosChoose.player_me.color = c;
	ColPosChoose.col_blib.set_pic_url(
		"ui/icons/labels/col_pos_choose/"+c+".png"
	);
	ColPosChoose.preview_map.change_color(
		ColPosChoose.player_me.nick, c
	);
	ColPosChoose.create_enemies_section();
	ColPosChoose.preview_map.refresh();
}

ColPosChoose.show_col_picker = function() {
	var confbox = MammunUI.create_conf_box(
		350, 200, PureFW.ConfirmationBox.NO
	);
	var col_blib_offset = 30;
	var col_box = confbox.add_widget(
		PureFW.Container,
		(confbox.width - (col_blib_offset << 2)) >> 1,
		(confbox.height - (col_blib_offset << 1)) >> 1,
		(col_blib_offset << 2),
		(col_blib_offset << 1)
	);
	var x = 0; var y = 0;
	for (c in ColPosChoose.colors) {
		var img = col_box.add_widget(
			PureFW.Image,
			x, y,
			20, 20,
			"ui/icons/labels/col_pos_choose/"+c+".png"
		);

		if (!ColPosChoose.colors[c]) {
			img.set_opacity(0.5);
			img.add_event_handler(
				"click",
				function (ev) {
					MammunUI.show_warning(
						350, 200, '<?= LNG_ERR_COLOR_NOT_AVAILABLE ?>',
						PureFW.ConfirmationBox.NO, true
					);
				}
			);
		}
		else {
			img.add_event_handler(
				"click",
				(function(_c, _confbox) {
					return function(ev){
						ColPosChoose.set_my_color(_c);
						_confbox.destroy();
					}
				})(c, confbox)
			);
		}

		if (x >= 3*col_blib_offset) {
			x = 0;
			y += col_blib_offset;
		}
		else
			x += col_blib_offset;
	}
}

<? if ($invite_mode) : ?>
ColPosChoose.go_on = function() {
	if (!ColPosChoose.has_opened_invitation_screen) {
		MammunUI.show_warning(
			350, 200, '<?= LNG_INFO_REMIND_LATER ?>',
			PureFW.ConfirmationBox.YES
		);
	}
	map_ui__fixed_window.destroy();
}
<? elseif (User::get_current_user()->get_state() == IUser::STATE_PLAYING) : ?>
ColPosChoose.change_color = function() {
	PureFW.AJAXClientServer.send_request(
		"col_pos_choose.php?color="+ColPosChoose.player_me.color+
			"&home="+ColPosChoose.chosen_home.y+''
			+ColPosChoose.chosen_home.x,
		function(response_arr) {
			if (response_arr[0] == 1) {
				MapDataCtrl.my_color = ColPosChoose.player_me.color;
				GameMenu.decoration_right.set_bg_img(
					'ui/elements/pause_menu/' + MapDataCtrl.my_color +
						'/border_right.png'
				);
				GameMenu.decoration_left.set_bg_img(
					'ui/elements/pause_menu/' + MapDataCtrl.my_color +
						'/border_left.png'
				);
				GameMenu.toggle_button.set_bg_img(
					'ui/elements/pause_menu/' + MapDataCtrl.my_color +
						'/button_bg.png'
				);
				close_fixed_window();
				MapDataCtrl.player_colors['<?=
									User::get_current_user()->get_nick() ?>']
					= MapDataCtrl.my_color
				MapDataCtrl.refresh_mammun_map();
			}
			else {
				throw response_arr;
			}
		}
	);
}
<? else : ?>
ColPosChoose.start_map = function()
{
	if (!ColPosChoose.chosen_home) {
		MammunUI.show_warning(
			350, 200, '<?= LNG_ERR_NO_HOME_CHOSEN ?>',
			PureFW.ConfirmationBox.NO, true
		);
		return;
	}

	MapUI.ingame_show_loading();
	PureFW.AJAXClientServer.send_request(
		"choose_map.php?color="+ColPosChoose.player_me.color+
			"&home="+ColPosChoose.chosen_home.y+''
			+ColPosChoose.chosen_home.x+"<?= $get_appendix ?>",
		function(response_arr) {
			if (response_arr[0] == 1) {
				document.location.href = "ingame.php?start=true";
			}
			else {
				throw response_arr;
			}
		}
	);
}
<? endif; ?>

ColPosChoose.destroy = function() {
	MapVisitNavi.map_browser_icon_label.show();
	ColPosChoose.this_window.remove_event_handler("destroy",
		ColPosChoose.destroy);
	ColPosChoose.this_window.remove_event_handler("change",
		ColPosChoose.destroy);
	ColPosChoose = null;
}
<?
/******************************************************************************
 *                  /\     .---.    -----    -     .---.   .  .               *
 *                 /__\    |          |      |     |   |   |`.|               *
 *                /    \   `---"      -      -     `---"   "  "               *
 *                                                                            *
 *                             A  C  T  I  O  N                            PK *
 ******************************************************************************/
?>



ColPosChoose.headline = ColPosChoose.this_window.add_widget(
	PureFW.Image,
	40, 0,
	410, 29,
<? if ($invite_mode) : ?>
	"ui/elements/texts/<?=$LNG?>/col_pos_choose/heading_invite_mode.png"
<? else : ?>
	"ui/elements/texts/<?=$LNG?>/col_pos_choose/heading.png"
<? endif; ?>
);

ColPosChoose.intro = ColPosChoose.this_window.add_widget(
	PureFW.Container,
	ColPosChoose.headline.position.x,40,
	415, 0
);
<? if ($invite_mode) : ?>
ColPosChoose.intro.set_content('<?= LNG_CHOOSE_COL_POS_INVITE_INTRO ?>');
<? else : ?>
ColPosChoose.intro.set_content('<?= LNG_CHOOSE_COL_POS_INTRO ?>');

ColPosChoose.col_blib = ColPosChoose.this_window.add_widget(
	PureFW.Image,
	ColPosChoose.intro.position.x,
	ColPosChoose.intro.position.y + 60,
	20, 20,
	"ui/icons/labels/col_pos_choose/"+ColPosChoose.player_me.color+".png"
);

ColPosChoose.col_choose_txt = ColPosChoose.this_window.add_widget(
	PureFW.Container,
	ColPosChoose.col_blib.position.x + ColPosChoose.col_blib.width + 10,
	ColPosChoose.col_blib.position.y,
	0, 0
);
ColPosChoose.col_choose_txt.set_content(
	'<?= str_replace(
			array('[LINK]', '[/LINK]'),
			array('<a href="javascript: ColPosChoose.show_col_picker();">',
					'</a>'),
			LNG_CHOOSE_COL_POS_YOUR_COLOR
	) ?>'
);

ColPosChoose.col_players_txt = ColPosChoose.this_window.add_widget(
	PureFW.Container,
	ColPosChoose.intro.position.x,
	ColPosChoose.col_blib.position.y + 40,
	415, 0
);
ColPosChoose.col_players_txt.set_content(
	'<?= LNG_CHOOSE_COL_POS_PLAYERS ?>'
);
<? endif; ?>

ColPosChoose.players_headline = ColPosChoose.this_window.add_widget(
	PureFW.Container,
	ColPosChoose.headline.position.x,
<? if ($invite_mode) : ?>
	150,
<? else : ?>
	200,
<? endif; ?>
	415, 0
);
ColPosChoose.players_headline.set_font_size(1.5);
ColPosChoose.players_headline.set_content(
	'<b><?= LNG_CHOOSE_COL_POS_CO_PLAYERS ?></b>'
);

ColPosChoose.players = ColPosChoose.this_window.add_widget(
	PureFW.Container,
	ColPosChoose.players_headline.position.x,
	ColPosChoose.players_headline.position.y
		+ 35,
	415, 212
);

<? if ($invite_mode) : ?>
ColPosChoose.resettle = ColPosChoose.this_window.add_widget(
	PureFW.Container,
	ColPosChoose.players.position.x,
	ColPosChoose.players.position.y
		+ ColPosChoose.players.height,
	0, 0
);
ColPosChoose.resettle.set_content(
	'<?= str_replace(array('[LINK]', '[/LINK]'),
		array('<a href="javascript: PauseMenu.leave_map()">', '</a>'),
		LNG_CHOOSE_COL_POS_INVITE_OTHER) ?>'
);
<? endif; ?>


ColPosChoose.isle = ColPosChoose.this_window.add_widget(
	PureFW.Container,
	490, 10,
	375, 375
);
ColPosChoose.isle.set_bg_img(
	"map/templates/<?= $template_id ?>/_0_/template<?= $template_id?>_375.jpg"
);

ColPosChoose.isle_overlay = ColPosChoose.isle.add_widget(
	PureFW.Image,
	0,0,
	375,375,
	"ui/backgrounds/col_pos_choose/overlay.png"
);


ColPosChoose.letsago_button = ColPosChoose.this_window.add_widget(
	PureFW.Image,
<? if ($invite_mode) : ?>
	670, 410,
	190, 30,
	"ui/elements/form/buttons/<?=$LNG?>/col_pos_choose/remind_me_later.png"
<? else : ?>
	720, 410,
	139, 30,
	"ui/elements/form/buttons/<?=$LNG?>/col_pos_choose/lets_go.png"
<? endif; ?>
);

ColPosChoose.letsago_button.add_event_handler("click",
	function(ev) {
<? if($invite_mode) : ?>
		ColPosChoose.go_on();
<? elseif (User::get_current_user()->get_state() == IUser::STATE_PLAYING) : ?>
		ColPosChoose.change_color();
<? else : ?>
		ColPosChoose.start_map();
<? endif; ?>
	}
);


ColPosChoose.preview_map = ColPosChoose.isle.add_widget(
	PureFW.GameMap.PositionChooseMap,
	ColPosChoose.isle.width-350, 50,
	320, 278,
	10, 10
);
ColPosChoose.preview_map.set_z_index(2);

ColPosChoose.preview_map.add_event_handler("field_click",
	ColPosChoose.home_click);

ColPosChoose.create_homes();
ColPosChoose.create_enemies_section();

MapVisitNavi.map_browser_icon_label.hide()

if(ColPosChoose.chosen_home == null)
{
	ColPosChoose.empty_hs = new Array();
	for (i in ColPosChoose.homesectors)
	{
		if (isNaN(i))
			continue;
		var s = ColPosChoose.homesectors[i];
		if (ColPosChoose.preview_map.is_empty(s%10, Math.floor(s/10)))
		{
			ColPosChoose.empty_hs.push(s);
		}
	}
	var random_pos = ColPosChoose.empty_hs.random();

	ColPosChoose.home_click(PureFW.EventUtil.create_event("click",
		new PureFW.Point(random_pos%10, random_pos/10)
	));
}


ColPosChoose.this_window.add_event_handler("destroy", ColPosChoose.destroy);
ColPosChoose.this_window.add_event_handler("change", ColPosChoose.destroy);

ColPosChoose.loaded = true;
</script>