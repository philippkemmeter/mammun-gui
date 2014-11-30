<? include_once('private_choose_shared.tmpl.inc') ?>

<script type="text/javascript">
PrivateCreateNewMap = new Object();
<? /**
    * Verweis auf das Fenster, in dem diese Seite hier dargestellt wird.
    */ ?>
PrivateCreateNewMap.this_window = PureFW.ContainerAJAX.instances[<?=$this_win_id?>];
PrivateCreateNewMap.this_window.set_font_size(1.3);
PrivateCreateNewMap.this_window.set_font_color('#333');
PrivateCreateNewMap.this_window.set_text_shadow('#fff', 1, 0, 0);

LeaveMapWindow.button_bar.deactivate();
<?
/**
 * Alle Kartentypen, die zur Auswahl stehen
 */
?>
PrivateCreateNewMap.choosable_map_templates = <?= json_encode($map_templates)?>;
PrivateCreateNewMap.chosen_template = <?= json_encode($chosen_template) ?>;

<?
/**
 * Die Freunde vom Pausemenu umkonvertieren in eine einfache Array-Form
 */
?>
PrivateCreateNewMap.buddies = new Array();
//PrivateCreateNewMap.checked = new Array();
for (var x in PauseMenu.buddies.offlineFriends) {
	if (isNaN(x))
		continue;
	PrivateCreateNewMap.buddies.push(
		{
			'nick':PauseMenu.buddies.offlineFriends[x].nick,
			'checked':true
		}
	);
}
for (var x in PauseMenu.buddies.onlineFriends) {
	if (isNaN(x))
		continue;
	PrivateCreateNewMap.buddies.push(
		{
			'nick':PauseMenu.buddies.offlineFriends[x].nick,
			'checked':true			
		}
	);
}

PrivateCreateNewMap.avatars = PauseMenu.avatars;
PrivateCreateNewMap.xps = PauseMenu.xps;

PrivateCreateNewMap.my_xp = <?= User::get_current_user()->get_cur_level() ?>;

<?
/**
 * Alle Checkbox-Avatare, die erzeugt werden, werden hier gespeichert, damit man
 * sie durchiterieren kann.
 */
?>
PrivateCreateNewMap.avatar_cb_instances = new Array();

PrivateCreateNewMap.conf_box = null;




<?
/**
 * Erzeugt die Liste der Freunde, die der Spieler einladen kann.
 */
?>
PrivateCreateNewMap.create_friends_list = function() {
	try {
		PrivateCreateNewMap.avatar_cb_instances.destroy();
	}
	catch(e) {}
	PrivateCreateNewMap.avatar_cb_instances = new Array();

	var n = PrivateCreateNewMap.buddies.length;
	for (var i = 0; i < n; i+=3) {
		var avatar_args = new Array();
		for (var row = 0, y = 0; row < 3; row++, y+=50) {
			var b = PrivateCreateNewMap.buddies[i+row];
			if (!b)
				break;
			b = b.nick;
			avatar_args.push({
				x: 0, y: y,
				w: 150, h: 48,
				pic: PrivateCreateNewMap.avatars[b],
				nick: b,
				i: i+row
			});
		}
		PrivateCreateNewMap.friends_list_cont_bubble_scroller.add_item(
			PureFW.Container,
			[ 150, 150 ],
			(function(_init) {
				return function() {
					var n = _init.length;
					for (var i = 0; i < n; i++) {
						var args = _init[i];
						var tmp = this.add_widget(
							PureFW.Avatar.AvatarXP, args.x, args.y,
								args.w, args.h, args.pic, args.nick, null,
								true
						);
						tmp.set_xp_level(PrivateCreateNewMap.xps[args.nick]);
						tmp.add_event_handler("check",
							(
								function(_i)
								{								
									return function (ev) 
									{
										PrivateCreateNewMap.buddies[_i].checked = true;
										PrivateCreateNewMap.adjust_starting_level();										
									}
								}
							)
							(args.i)
						);
						tmp.add_event_handler("uncheck",
							(
								function(_i)
								{								
									return function (ev) 
									{
										PrivateCreateNewMap.buddies[_i].checked = false;
										PrivateCreateNewMap.adjust_starting_level();
									}
								}
							)
							(args.i)
						);
						tmp.check();
						PrivateCreateNewMap.avatar_cb_instances.push(tmp);
					}
				}
			})(avatar_args)
		);
	}
}

<?
/**
 * "Mein" Start-Level wird hierüber angepasst an alle User, die angeklickt
 * wurden. Dabei sind die Welten in 5er-Abschnitten unterteilt, außer Welt 0,
 * also:
 * 0-5,
 * 6-10,
 * 11-15,
 * 16-20
 * ...
 * Von diesen Bereichen wird immer das höchste Level gewählt.
 */
?>

PrivateCreateNewMap.adjust_starting_level = function() {
	var n = PrivateCreateNewMap.buddies.length;
	var min = Infinity;
	for (var x in PrivateCreateNewMap.buddies) {
		var a = PrivateCreateNewMap.buddies[x];
		if (a.checked==true) {
			if (a.xps < min)
				min = PrivateCreateNewMap.xps[a.nick];
		}
	}

	var level = (min < 6) ? 5 : Math.ceil(min/5)*5;
	if (level > PrivateCreateNewMap.my_xp)
		level = PrivateCreateNewMap.my_xp;

	PrivateCreateNewMap.new_island_preview.set_xp_level(level);
}

<?
/**
 * Wählt das Template aus.
 *
 * @param uint template_id
 */
?>
PrivateCreateNewMap.choose_template = function(template_id) {
	var n = PrivateCreateNewMap.choosable_map_templates.length;
	for (var i = 0; i < n; i++) {
		if (PrivateCreateNewMap.choosable_map_templates[i].template_id
			== template_id)
		{
			PrivateCreateNewMap.chosen_template
				= PrivateCreateNewMap.choosable_map_templates[i];
			break;
		}
	}
	PrivateCreateNewMap.new_island_preview.set_template_id(template_id);
}

<?
/**
 * Sendet die Anfrage ab.
 */
?>
PrivateCreateNewMap.confirm_all_settings = function() {
	if (PrivateCreateNewMap.conf_box) {
		try {
			PrivateCreateNewMap.conf_box.destroy();
		}
		catch(e) {}
		PrivateCreateNewMap.conf_box = null;
	}
	var n = PrivateCreateNewMap.buddies.length;

	var checked = new Array();
	for (var x in PrivateCreateNewMap.buddies) {
		if (PrivateCreateNewMap.buddies[x].checked==true) {
			checked.push(PrivateCreateNewMap.buddies[x].nick);
		}
	}
	if (checked.length == 0) {
		PrivateCreateNewMap.conf_box = new PureFW.ConfirmationBox(
			document.body,
			(MammunUI.reference_width - 404) >> 1,
			(MammunUI.reference_height - 325) >> 1,
			404, 325,
			PureFW.ConfirmationBox.NO,
			true
		);
		PrivateCreateNewMap.conf_box.set_bg_img(
			'ui/backgrounds/confirm/alert/alert_404x325.png'
		);
		PrivateCreateNewMap.conf_box.set_css_class(
			'ConfirmationBox'
		);
		if (n == 0) {
			PrivateCreateNewMap.conf_box.set_content(
				'<div style="padding: 5%">'
					+ '<?= LNG_NO_FRIENDS_ON_SUBMIT ?>'
				+'</div>'
			);
		}
		else {
			PrivateCreateNewMap.conf_box.set_content(
				'<div style="padding: 5%">'
					+ '<?= LNG_TOO_FEW_FRIENDS_CHOSEN ?>'
				+'</div>'
			);
		}
	}
	<? /** Zusammenfassungsscreen anzeigen */ ?>
	else {
		PrivateCreateNewMap.conf_box = new PureFW.ConfirmationBox(
			document.body,
			(MammunUI.reference_width - 530) >> 1,
			(MammunUI.reference_height - 530) >> 1,
			530, 530,
			PureFW.ConfirmationBox.NO | PureFW.ConfirmationBox.YES,
			true
		);
		PrivateCreateNewMap.conf_box.set_bg_img(
			'ui/backgrounds/confirm/info/info_530x530.png'
		);
		PrivateCreateNewMap.conf_box.set_css_class(
			'ConfirmationBox'
		);

		var headline1 = PrivateCreateNewMap.conf_box.add_widget(
			PureFW.Image,
			30, 20,
			293, 28,
			'ui/elements/texts/<?=$LNG?>/choose_map/confirm_island.png'
		);

		var map_cont = PrivateCreateNewMap.conf_box.add_widget(
			PureFW.Container,
			(PrivateCreateNewMap.conf_box.width - 440) >> 1,
			headline1.position.x + headline1.height + 5,
			440,
			170
		);
		var template_id = PrivateCreateNewMap.chosen_template.template_id;
		var map_img = map_cont.add_widget(
			PureFW.Image,
			7, 6,
			150, 150,
			'map/templates/'+template_id+'/_0_/template'+template_id+'_150_3d.jpg'
		);

		var headline_small = map_cont.add_widget(
			PureFW.Container,
			(map_img.position.x + map_img.width + 6),
			map_img.position.y,
			map_cont.width - (map_img.position.x + map_img.width + 6) - 6,
			25
		);
		headline_small.set_content('<b><?= LNG_ALL_PLAYERS_LVL_MAX ?></b>');

		var xp_star = map_cont.add_widget(
			PureFW.Container,
			headline_small.position.x + ((headline_small.width - 133) >> 1),
			headline_small.position.y + headline_small.height,
			133, 133
		);
		xp_star.set_bg_img('ui/icons/labels/xp_star/133/star.png');
		var xp_star_inner = xp_star.add_widget(
			PureFW.Container,
			0, ((133-40)>>1),
			133, 40
		);
		xp_star_inner.set_font_size(2.3);
		xp_star_inner.set_text_align('center')
		xp_star_inner.set_font_weight('bold');
		xp_star_inner.set_content(
			PrivateCreateNewMap.new_island_preview.get_xp_level());

		var headline2 = PrivateCreateNewMap.conf_box.add_widget(
			PureFW.Image,
			headline1.position.x,
			map_cont.position.x + map_cont.height + 10,
			294, 22,
			'ui/elements/texts/<?=$LNG?>/choose_map/confirm_invitations.png'
		);

		<? /** Scroller mit allen ausgewählten Avataren */ ?>
		var scroller = PrivateCreateNewMap.conf_box.add_widget(
			PureFW.BubbleScroller,
			headline2.position.x,
			headline2.position.y
				+ headline2.height + 15,
			PrivateCreateNewMap.conf_box.width - (headline2.position.x << 1),
			260
		);

		var n = checked.length;
		for (var i = 0; i < n; i+=3) {
			var avatar_args = new Array();
			for (var row = 0, y = 0; row < 3; row++, y+=50) {
				var b = checked[i+row];
				if (!b)
					break;
				avatar_args.push({
					x: 0, y: y,
					w: 150, h: 48,
					pic: PrivateCreateNewMap.avatars[b],
					nick: b
				});
			}
			scroller.add_item(
				PureFW.Container,
				[ 150, 150 ],
				(function(_init) {
					return function() {
						var n = _init.length;
						for (var i = 0; i < n; i++) {
							var args = _init[i];
							var tmp = this.add_widget(
								PureFW.Avatar.AvatarXP, args.x, args.y,
									args.w, args.h, args.pic, args.nick
							);
							tmp.set_xp_level(PrivateCreateNewMap.xps[args.nick]);
						}
					}
				})(avatar_args)
			);
		}

		<? /** Abgehen */?>
		var str = checked[0];
		for (var i = 1; i < n; i++)
			str += ','+checked[i];
		PrivateCreateNewMap.conf_box.add_event_handler("confirm",
				(function(_checked_str, _template_id) {
					return function(ev) {
<?						/*
						MapUI.ingame_show_loading();
						PureFW.AJAXClientServer.send_request(
							"choose_map.php?dataX=<?= $cryptObj->do_encryption(array("start_new_map" => true)); ?>&template_id="
								+ _template_id
								+ "&invitations="+_checked_str,
							PrivateCreateNewMap.confirm_result
						);*/?>
						UIButtons.open_col_choose_dialog(
							"dataX=<?= $cryptObj->do_encryption(
								array("start_new_map" => true)); ?>&template_id="
							+ _template_id
							+ "&invitations="+_checked_str
						);
						<?/*
						PrivateCreateNewMap.this_window.set_url(
							"col_pos_choose.php?dataX=<?= $cryptObj->do_encryption(array("start_new_map" => true)); ?>&template_id="
							+ _template_id
							+ "&invitations="+_checked_str)*/?>
					}
				})(str, template_id)
		);
	}
}

PrivateCreateNewMap.confirm_result = function(response_arr) {
	if (response_arr[0] == 1) {
		document.location.href = "ingame.php?start=true";
	}
}

PrivateCreateNewMap.destroy = function() {
	PrivateCreateNewMap.this_window.remove_event_handler("destroy",
		PrivateCreateNewMap.destroy);
	PrivateCreateNewMap.this_window.remove_event_handler("change",
		PrivateCreateNewMap.destroy);
	destroy_object(PrivateCreateNewMap);
	PrivateCreateNewMap = null;
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

PrivateCreateNewMap.topic_img
	= PrivateCreateNewMap.this_window.add_widget
(
	PureFW.Image,
	30, 5,
	764, 27,
	'ui/elements/texts/<?=$LNG?>/choose_map/choose_friends_to_invite_on_new_map.png'
);

<? /** Container mit den Freunden drin, die er einladen kann */ ?>
PrivateCreateNewMap.friends_list_cont
	= PrivateCreateNewMap.this_window.add_widget
(
	PureFW.Container,
	25,
	PrivateCreateNewMap.topic_img.position.y
		+ PrivateCreateNewMap.topic_img.height
		+ 20,
	496, 300
);
PrivateCreateNewMap.friends_list_cont.set_bg_img(
	'ui/backgrounds/choose_map/friends_list_new_map.png'
);
PrivateCreateNewMap.friends_list_cont.set_text_align('center');
PrivateCreateNewMap.friends_list_cont.add_content(
	'<br/><span style="color: #aaa; font-size: 0.95em"><?= LNG_PLAYER_LEVEL_LIKE_WEAKEST ?></span>'
);
PrivateCreateNewMap.friends_list_cont_bubble_scroller
	= PrivateCreateNewMap.friends_list_cont.add_widget
(
	PureFW.BubbleScroller,
	(PrivateCreateNewMap.friends_list_cont.width - 480)>>1,
	30,
	480, 260
);


<? /* Zurückbutton */?>
PrivateCreateNewMap.back
	= PrivateCreateNewMap.this_window.add_widget
(
	PureFW.Image,
	PrivateCreateNewMap.friends_list_cont.position.x,
	PrivateCreateNewMap.friends_list_cont.position.y
		+ PrivateCreateNewMap.friends_list_cont.height
		+ 10,
	105, 32,
	'ui/elements/form/buttons/<?=$LNG?>/wizard/back_shadow.png'
);
PrivateCreateNewMap.back.add_event_handler("click",
	function(ev) {
		PrivateCreateNewMap.this_window.set_url('choose_map.php');
	}
);

<? /** Kurzer Beschreibungstext für die linke Spalte */ ?>
PrivateCreateNewMap.right_col_desc_cont
	= PrivateCreateNewMap.this_window.add_widget
(
	PureFW.Container,
	PrivateCreateNewMap.friends_list_cont.position.x
		+ PrivateCreateNewMap.friends_list_cont.width
		+ 25,
	PrivateCreateNewMap.friends_list_cont.position.y,
	260, 50
);
PrivateCreateNewMap.right_col_desc_cont.set_content(
	'<b><?=LNG_COMPETE_W_FRIEND_W_XP?></b>'
);

<? /** Vorschau der aktuell ausgewählten Insel */ ?>
PrivateCreateNewMap.new_island_preview
	= PrivateCreateNewMap.this_window.add_widget
(
	PureFW.ChooseMap.NewIslandPreview,
	PrivateCreateNewMap.friends_list_cont.position.x
		+ PrivateCreateNewMap.friends_list_cont.width
		+ 60,
	PrivateCreateNewMap.right_col_desc_cont.position.y
		+ PrivateCreateNewMap.right_col_desc_cont.height
		+ 10,
	192, 222,
	PrivateCreateNewMap.chosen_template.template_id
);
PrivateCreateNewMap.new_island_label
	= PrivateCreateNewMap.new_island_preview.add_widget_to_label
(
	PureFW.Container,
	0, 0,
	PrivateCreateNewMap.new_island_preview.widget_label.width,
	20
);
PrivateCreateNewMap.new_island_label.set_text_align('center');
PrivateCreateNewMap.new_island_label.add_content(
	'<b><a href="javascript: ;">'
		+ '<?= LNG_BTN_CHOOSE_MAP_TEMPLATE ?></a></b>'
);
PrivateCreateNewMap.new_island_preview.set_xp_level(
	PrivateCreateNewMap.my_xp
);
PrivateCreateNewMap.new_island_preview.add_event_handler("click",
	function (ev) {
		PrivateChooseMapShared.choose_template_dialog(
			PrivateCreateNewMap.choosable_map_templates,
			PrivateCreateNewMap.choose_template
		);
	}
);

<? /** Confirmation button */ ?>
PrivateCreateNewMap.confirm_button
	= PrivateCreateNewMap.this_window.add_widget
(
	PureFW.Image,
	PrivateCreateNewMap.friends_list_cont.position.x
		+ PrivateCreateNewMap.friends_list_cont.width
		+ 40,
	PrivateCreateNewMap.new_island_preview.position.y
		+ PrivateCreateNewMap.new_island_preview.height
		+ 10,
	226, 54,
	'ui/elements/form/buttons/<?=$LNG?>/choose_map/create_new_island_confirm.png'
);
PrivateCreateNewMap.confirm_button.add_event_handler("click",
	PrivateCreateNewMap.confirm_all_settings
);

<? /** Nothing Fits? */?>
PrivateCreateNewMap.nothing_fits_cont
	= PrivateCreateNewMap.this_window.add_widget
(
	PureFW.Container,
	PrivateCreateNewMap.friends_list_cont.position.x
		+ PrivateCreateNewMap.friends_list_cont.width
		- 380,
	PrivateCreateNewMap.friends_list_cont.position.y
		+ PrivateCreateNewMap.friends_list_cont.height
		+ 3,
	380,
	60
);
PrivateCreateNewMap.nothing_fits_cont.set_content(
	'<?= LNG_NOT_ENOUGH_FRIENDS_TEXT ?><br/>'
	+ '<b><a href="javascript: '
		+ 'PrivateCreateNewMap.this_window.set_url(\'choose_map.php\');"><?=
			LNG_BTN_PLAY_A_RANDOM_MAP
		?></a></b>'
);

<? /** Füllung des Containers mit den Freunden */?>
PrivateCreateNewMap.create_friends_list();

PrivateCreateNewMap.this_window.add_event_handler("destroy",
	PrivateCreateNewMap.destroy);
PrivateCreateNewMap.this_window.add_event_handler("change",
	PrivateCreateNewMap.destroy);
</script>
