<? include_once('private_choose_shared.tmpl.inc') ?>

<script type="text/javascript">
PrivateChooseMap = new Object();
<? /**
    * Verweis auf das Fenster, in dem diese Seite hier dargestellt wird.
    */ ?>
PrivateChooseMap.this_window = PureFW.ContainerAJAX.instances[<?=$this_win_id?>];
PrivateChooseMap.this_window.set_font_size(1.3);
PrivateChooseMap.this_window.set_font_color('#333');
PrivateChooseMap.this_window.set_text_shadow('#fff', 1, 0, 0);
LeaveMapWindow.button_bar.deactivate();

PrivateChooseMap.fb_friends = <?= json_encode($fb_friends) ?>;

PrivateChooseMap.MapPlayers = null;
PrivateChooseMap.InvPop = null;
PrivateChooseMap.mentor_cont = null;

<?
/**
 * Alle Kartentypen, die zur Auswahl stehen
 */
?>
PrivateChooseMap.choosable_map_templates = <?= json_encode($map_templates)?>;
PrivateChooseMap.chosen_template = <?= json_encode($chosen_template) ?>;
PrivateChooseMap.avatar_data = <?= json_encode($avatar_data) ?>;
PrivateChooseMap.map_invitations = <?= json_encode($map_invitations) ?>;
PrivateChooseMap.auto_choose_map_invitation = <?=$auto_choose_map_invitation?>;
PrivateChooseMap.invitation_containers = new Array();
PrivateChooseMap.my_level = <?= User::get_current_user()->get_cur_level() ?>;

<?
/**
 * There is a small Popup here to inform the player if the chosen map is
 * unaccessible. Bad auth_key or full island.
 */
?>
PrivateChooseMap.choose_by_auth_key = function(auth_key)
{
	PureFW.AJAXClientServer.send_request('choose_map.php?choose_by_auth_key='
		+auth_key, PrivateChooseMap.choose_by_auth_key_resp);
};

PrivateChooseMap.choose_by_auth_key_resp = function(response_arr)
{
	if(!parseInt(response_arr[0]))
	{
//		if(parseInt(response_arr[1])) // if errorCode == 1, not sure if relevant
//		{
			var info_cont = MammunUI.show_warning(
					360, 255,
					'<?= LNG_SETTLE_BUT_FULL_INTERNAL ?>',
					PureFW.ConfirmationBox.NO,
					false);
//		}
	}
};

<?
/**
 * Der Spieler möchte die angegebene Einladung annehmen. Das führt zu einem
 * Besuch der damit verbundenen Insel. MapVisitNavi ist dann dafür
 * verantwortlich, dass er dort siedeln kann.
 */
?>
PrivateChooseMap.visit_chosen_map = function(invitation) {
	if (PrivateChooseMap.InvPop) {
		destroy_object(PrivateChooseMap.InvPop);
		PrivateChooseMap.InvPop = null;
	}
	if (PrivateChooseMap.MapPlayers) {
		destroy_object(PrivateChooseMap.MapPlayers);
		PrivateChooseMap.MapPlayers = null;
	}
	MapVisitNavi.map_browser.choose_map_ids = [
		[ invitation.world_id, invitation.map_id ]
	];
	MapVisitNavi.map_browser.choose_map_hrefs = new Object();
	MapVisitNavi.map_browser.choose_map_hrefs[invitation.world_id]
		= new Object();
	MapVisitNavi.map_browser.choose_map_hrefs[invitation.world_id]
		[invitation.map_id] = invitation.choose_map_href.replace(
			'choose_map.php',
			'col_pos_choose.php'
		);	/** Wrap hacking yeah **/

	MapVisitNavi.map_browser.choose_mode = true;
	if (PrivateChooseMap.auto_choose_map_invitation > -1)
		MapVisitNavi.map_browser.direct_choose = true;
	MapVisitNavi.map_browser.show_players_on_map = true;
	MapVisitNavi.map_browser.show_back_to_my_map_link = false;

	MapUI.change_map(invitation.world_id, invitation.map_id);
}

<?
/**
 * Der Spieler will die Einladung direkt annehmen, ohne vorher die Insel zu
 * besuchen.
 */
?>
PrivateChooseMap.accept_chosen_map = function(invitation) {
	if (PrivateChooseMap.InvPop) {
		destroy_object(PrivateChooseMap.InvPop);
		PrivateChooseMap.InvPop = null;
	}
	if (PrivateChooseMap.MapPlayers) {
		destroy_object(PrivateChooseMap.MapPlayers);
		PrivateChooseMap.MapPlayers = null;
	}

	var tmp = invitation.choose_map_href.split('?');
	UIButtons.open_col_choose_dialog(tmp[1]);
}

PrivateChooseMap.random_settle = function(force) {
	if (!force) {
		var warn_cont = MammunUI.show_warning(
			360, 255,
			'<?= LNG_RANDOM_SETTLE_WARNING ?>',
			PureFW.ConfirmationBox.YES | PureFW.ConfirmationBox.NO,
			false
		);
		warn_cont.add_event_handler("confirm",
			function(ev) {
				PrivateChooseMap.random_settle(true);
			}
		);
	}
	else {
		MapUI.ingame_show_loading();
		PureFW.AJAXClientServer.send_request('choose_map.php?dataX=<?=
			$cryptObj->do_encryption(array("choose_map_randomly" => true)) ?>',
				PrivateChooseMap.random_settle_response);
	}
}
PrivateChooseMap.random_settle_response = function(response_arr) {
	if (response_arr[0] == 1) {
		document.location.href = "ingame.php?start=true";
	}
}

PrivateChooseMap._configure_invitation = function(i) {
	var container = this;
	this.set_overlay_bg_img(
		'ui/backgrounds/PolaroidContainer/overlay_bg_ice.png'
	);

	this.set_pic_url(
		PrivateChooseMap.avatar_data[
			PrivateChooseMap.map_invitations[i].received_by
		].replace('SIZE', 150)
	);
	this.set_xp_level(
		((PrivateChooseMap.map_invitations[i].max_level >
			PrivateChooseMap.my_level)
				? PrivateChooseMap.my_level
				: PrivateChooseMap.map_invitations[i].max_level)
	);

	this.confirm_button =
		this.add_widget_to_label(
			PureFW.Container,
			0, this.widget_label.height
				- 20,
			this.widget_label.width,
			20
		);
	this.confirm_button.set_text_align(
		'center');
	this.confirm_button.set_content(
		'<b><a href="javascript: PrivateChooseMap.visit_chosen_map('
			+ 'PrivateChooseMap.map_invitations['+i+'])"><?=
			LNG_BTN_ACCEPT_INVITATION ?></a></b>'
	);

	this.add_label_content(
		'<?= LNG_USER_HAS_INVITED_YOU ?>'.replace("[USER]",
			'<a href="javascript: UIButtons.toggle_profile(\'nick='
				+ PrivateChooseMap.map_invitations[i].received_by + '\');">'
				+ MammunUI.beautify_nick(
					PrivateChooseMap.map_invitations[i].received_by)
				+ '</a>'
			)
	);

<?	/**
	 * Spieler auf der Karte.
	 */ ?>
	this.players_on_map_imgs = [];

	var n = PrivateChooseMap.map_invitations[i].players_on_map.length;
	for (var j = 0, x = 0, y = 0; j < n; j++) {
		if (PrivateChooseMap.map_invitations[i].players_on_map[j]
			!= PrivateChooseMap.map_invitations[i].received_by)
		{
			var img = this.add_widget_to_overlay(
				PureFW.Image,
				x, y,
				22, 22,
				PrivateChooseMap.avatar_data[
					PrivateChooseMap.map_invitations[i].players_on_map[j]
				].replace('SIZE', 22)
			);
			this.players_on_map_imgs.push(img);

			if (x >= 80) {
				x = 0;
				y += 23;
			}
			else {
				x += 23;
			}
		}
	}
}
<?
/**
 * Erzeugt die ersten zwei Einladungen und fügt sie in den Container
 * invitation_cont hinzu.
 */
?>
PrivateChooseMap.create_invitations = function() {
	PrivateChooseMap.invitation_containers.destroy();
	PrivateChooseMap.invitation_containers = new Array();

	for (var i = 0; i < 2 /* MAX 2 invitations in a row */; i++) {
		try {
			PrivateChooseMap.invitation_containers[i] =
				PrivateChooseMap.invitations_cont.add_widget
			(
				PureFW.ChooseMap.IslandPreview,
				i*190, 0,
				188, PrivateChooseMap.invitations_cont.height,
				PrivateChooseMap.map_invitations[i].template_id
			);
			PrivateChooseMap._configure_invitation.call(
				PrivateChooseMap.invitation_containers[i], i
			);
		}
		catch(e) {
			<? /** Gibt keine Einladung :) */ ?>
			PrivateChooseMap.invitation_containers[i] =
				PrivateChooseMap.invitations_cont.add_widget
			(
				PureFW.Image,
				i*190, 0,
				188, PrivateChooseMap.invitations_cont.height,
				'ui/backgrounds/choose_map/empty_invitation_slot.png'
			);
			PrivateChooseMap.invitation_containers[i].add_event_handler("click",
				function(ev) {
					PrivateChooseMap.show_mentor_dialog();
				}
			);
		}
	}

}

PrivateChooseMap.destroy = function() {
	PrivateChooseMap.this_window.remove_event_handler("destroy",
		PrivateChooseMap.destroy);
	PrivateChooseMap.this_window.remove_event_handler("change",
		PrivateChooseMap.destroy);
	destroy_object(PrivateChooseMap);
	PrivateChooseMap = null;
}

<?
/**
 * Zeigt alle Einladungen in einem Popup an
 */
?>
PrivateChooseMap.show_invitations = function()
{
	if(PrivateChooseMap.InvPop)
	{
		destroy_object(PrivateChooseMap.InvPop);
		PrivateChooseMap.InvPop = null;
	}
	PrivateChooseMap.InvPop = new Object();
	PrivateChooseMap.InvPop.box = new PureFW.ConfirmationBox(
		document.body,
		(MammunUI.reference_width - 530) >> 1,
		(MammunUI.reference_height - 437) >> 1,
		530, 437,
		PureFW.ConfirmationBox.NO
	);
	PrivateChooseMap.InvPop.box.set_bg_img(
			'ui/backgrounds/confirm/info/info_530x437.png'
	);
	PrivateChooseMap.InvPop.box.set_font_size(1.3);

	PrivateChooseMap.InvPop.title = new PureFW.Image(
		PrivateChooseMap.InvPop.box,
		PrivateChooseMap.InvPop.box.width/2 - 396/2, 22,
		396, 28,
		'ui/elements/texts/<?=$LNG?>/map_navi/all_invitations_396x28.png');

	PrivateChooseMap.InvPop.scroller = new PureFW.BubbleScroller(
		PrivateChooseMap.InvPop.box,
		20,
		PrivateChooseMap.InvPop.title.position.y
			+ PrivateChooseMap.InvPop.title.height + 15,
		PrivateChooseMap.InvPop.box.width - 40,
		PrivateChooseMap.InvPop.box.height
			- PrivateChooseMap.InvPop.title.position.y
			- PrivateChooseMap.InvPop.title.height - 35
		);
	for(var i = 0; i < PrivateChooseMap.map_invitations.length; i++)
	{
		PrivateChooseMap.InvPop.scroller.add_item(
			PureFW.ChooseMap.IslandPreview,
			[ 188, PrivateChooseMap.invitations_cont.height,
							  PrivateChooseMap.map_invitations[i].template_id ],
			(function(_i) {
				return function () {
					PrivateChooseMap._configure_invitation.call(this, _i);
				}
			})(i)
		);
	}
};

<?
/**
 * Zeigt alle Spieler auf einer Karte in einem Popup an
 */
?>
PrivateChooseMap.show_map_players = function(i)
{
	if(PrivateChooseMap.MapPlayers)
	{
		destroy_object(PrivateChooseMap.MapPlayers);
		PrivateChooseMap.MapPlayers = null;
	}
	PrivateChooseMap.MapPlayers = new Object();
	PrivateChooseMap.MapPlayers.box = new PureFW.ConfirmationBox(
		document.body,
		(MammunUI.reference_width - 404) >> 1,
		(MammunUI.reference_height - 325) >> 1,
		404, 325,
		PureFW.ConfirmationBox.NO,
		true
	);
	PrivateChooseMap.MapPlayers.box.set_bg_img(
		'ui/backgrounds/confirm/info/info_404x325.png'
	);
	PrivateChooseMap.MapPlayers.box.add_event_handler("confirm",
		function(ev)
		{
			destroy_object(PrivateChooseMap.MapPlayers);
		}
	);
	PrivateChooseMap.MapPlayers.title = new PureFW.Image(
		PrivateChooseMap.MapPlayers.box,
		PrivateChooseMap.MapPlayers.box.width/2 - 277/2, 22,
		277, 28,
		'ui/elements/texts/<?=$LNG?>/map_navi/players_on_map_title_277x28.png');
	PrivateChooseMap.MapPlayers.playerContainers = new Array();
	var x = 0;
	var y = 0;
	for(var j=0;j<PrivateChooseMap.map_invitations[i].players_on_map.length;j++)
	{
		//if x is odd, move one over, else stay at 20
		x = (j%2)*((PrivateChooseMap.MapPlayers.box.width/2 - 30)+20) + 20;
		y = (j%2) ? y : y + 50 + 10; // if j is even, go to the next line
		var player = PrivateChooseMap.map_invitations[i].players_on_map[j];
		if(typeof(player) == "function")
		{
			continue;
		}
		PrivateChooseMap.MapPlayers.playerContainers[j] = new PureFW.Container(
			PrivateChooseMap.MapPlayers.box,
			x, y,
			PrivateChooseMap.MapPlayers.box.width/2 - 30, 50);
		PrivateChooseMap.MapPlayers.playerContainers[j].set_content(
			'<div style="padding-left:60px">'
				+MammunUI.beautify_nick(player)+'</div>');
		PrivateChooseMap.MapPlayers.playerContainers[j].add_widget(
			PureFW.Avatar,
			0, 0, 48, 48,
			PrivateChooseMap.avatar_data.player, player);
	}
};


<?
/**
 * Wählt das Template für die Mentorenkarte aus.
 *
 * @param uint template_id
 */
?>
PrivateChooseMap.mentor_choose_template = function(template_id) {
	var n = PrivateChooseMap.choosable_map_templates.length;
	for (var i = 0; i < n; i++) {
		if (PrivateChooseMap.choosable_map_templates[i].template_id
			== template_id)
		{
			PrivateChooseMap.chosen_template
				= PrivateChooseMap.choosable_map_templates[i];
			break;
		}
	}
	PrivateChooseMap.show_mentor_dialog(template_id);
}
<?
/**
 * Zeigt den Dialog an, der die Einstellungen für eine Mentorenkarte vornimmt.
 */
?>
PrivateChooseMap.show_mentor_dialog = function(template_id) {
	if (PrivateChooseMap.mentor_cont) {
		try {
			PrivateChooseMap.mentor_cont.destroy();
		}
		catch(e) {}
		PrivateChooseMap.mentor_cont = null;
	}
	PrivateChooseMap.mentor_cont = MammunUI.create_conf_box(530, 352);

	var mentor_cont = PrivateChooseMap.mentor_cont;

	var template_id = (template_id) ? template_id
		: PrivateChooseMap.chosen_template.template_id;

	PrivateChooseMap.new_island_preview = mentor_cont.add_widget(
		PureFW.ChooseMap.NewIslandPreview,
		mentor_cont.width - 230,
		50,
		192, 222,
		template_id
	);
	var new_island_label
		= PrivateChooseMap.new_island_preview.add_widget_to_label
	(
		PureFW.Container,
		0, 0,
		PrivateChooseMap.new_island_preview.widget_label.width,
		20
	);
	new_island_label.set_text_align('center');
	new_island_label.add_content(
		'<b><a href="javascript: ;">'
			+ '<?= LNG_BTN_CHOOSE_MAP_TEMPLATE ?></a></b>'
	);
	PrivateChooseMap.new_island_preview.set_xp_level(5);

	PrivateChooseMap.new_island_preview.add_event_handler("click",
		function (ev) {
			PrivateChooseMapShared.choose_template_dialog(
				PrivateChooseMap.choosable_map_templates,
				PrivateChooseMap.mentor_choose_template
			);
		}
	);

	var description = mentor_cont.add_widget(
		PureFW.Container,
		40, 30,
		240, mentor_cont.height - 60
	);
	description.set_content('<?= LNG_MENTOR_WARNING ?>');


	mentor_cont.add_event_handler("confirm",
		PrivateChooseMap.start_mentor_map
	);
}

<?
/**
 * Startet eine Mentorenkarte
 */
?>
PrivateChooseMap.start_mentor_map = function() {
	<?/*
	MapUI.ingame_show_loading();
	PureFW.AJAXClientServer.send_request('map_choose.php?dataX=<?=
		$cryptObj->do_encryption(array("start_mentor_map" => true)) ?>'
		+ '&template_id='+PrivateChooseMap.chosen_template.template_id,
			PrivateChooseMap.random_settle_response);
	*/?>
//	LeaveMapWindow.destroy();
	UIButtons.open_col_choose_dialog('dataX=<?=
		$cryptObj->do_encryption(array("start_mentor_map" => true)) ?>'
		+ '&template_id='+PrivateChooseMap.chosen_template.template_id);
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



<? /** Sonst bauen wir ganz normal den Screen auf */ ?>

<?
/**
 * Die Überschrift des Screens
 */
?>
PrivateChooseMap.topic_img
	= PrivateChooseMap.this_window.add_widget
(
	PureFW.Image,
	40, 5,
	365, 30,
	'ui/elements/texts/<?=$LNG?>/choose_map/which_island.png'
);

<?
/**
 * Dieser Container enthält die Einladungen, die ein Spieler bekommen hat.
 */
?>
PrivateChooseMap.invitations_cont
	= PrivateChooseMap.this_window.add_widget
(
	PureFW.Container,
	PrivateChooseMap.topic_img.position.x,
	PrivateChooseMap.topic_img.position.y
		+ PrivateChooseMap.topic_img.height
		+ 10,
	188*2 + 2,
	288
);
PrivateChooseMap.invitations_cont.set_overflow('visible');

<? /** Inhalt des Containers wird dynamisch erzeugt. */ ?>
PrivateChooseMap.create_invitations();

<?
/**
 * Dieser Container enthält die anderen Auswahlmöglichkeiten, die ein Spieler
 * hat, die genauso dargestellt werden, wie die Einladungen (mit fremden
 * Spielen, mit FB-Spieler spielen
 */
?>
PrivateChooseMap.other_main_choices_cont
	= PrivateChooseMap.this_window.add_widget
(
	PureFW.Container,
	PrivateChooseMap.invitations_cont.position.x
		+ PrivateChooseMap.invitations_cont.width + 2,
	PrivateChooseMap.invitations_cont.position.y,
	188*2 + 7,
	288
);
PrivateChooseMap.other_main_choices_cont.set_overflow('visible');
<? /** INHALT des Containers */ ?>

<? /* ZUFÄLLIG siedeln */ ?>
PrivateChooseMap.choice_random = PrivateChooseMap.other_main_choices_cont.
	add_widget
(
	PureFW.ChooseMap.IslandPreview,
	0, 0,
	188,
	PrivateChooseMap.other_main_choices_cont.height
);
PrivateChooseMap.choice_random.set_pic_url(
	'ui/backgrounds/choose_map/random_settle_pic.png'
);
PrivateChooseMap.choice_random.set_xp_level(
	<?= User::get_current_user()->get_cur_level() ?>
);
PrivateChooseMap.choice_random.confirm_button =
	PrivateChooseMap.choice_random.add_widget_to_label(
		PureFW.Container,
		0, PrivateChooseMap.choice_random.widget_label.height
			- 20,
		PrivateChooseMap.choice_random.widget_label.width,
		20
	);
PrivateChooseMap.choice_random.confirm_button.set_text_align(
	'center');
PrivateChooseMap.choice_random.confirm_button.set_content(
	'<b><a href="javascript: PrivateChooseMap.random_settle();">'
		+'<?=LNG_BTN_CONFIRM_RANDOM?></a></b>'
);
PrivateChooseMap.choice_random.add_overlay_content(
	'<div style="padding-top: 8%"><?= LNG_PLAY_VS_RANDOM_PEOPLE ?></div>'
);

PrivateChooseMap.choice_random.add_label_content(
	'<?= LNG_PLAY_VS_RANDOM_PEOPLE_DESC ?>'
);

<? /* FB-Wahl */ ?>
PrivateChooseMap.choice_fb = PrivateChooseMap.other_main_choices_cont.
	add_widget(
	PureFW.ChooseMap.IslandPreview,
	190, 0,
	188,
	PrivateChooseMap.other_main_choices_cont.height
);
PrivateChooseMap.choice_fb.set_pic_url(
	'ui/backgrounds/choose_map/invite_new_players.png'
);

PrivateChooseMap.choice_fb.set_xp_level(
	(PrivateChooseMap.my_level < 5) ? PrivateChooseMap.my_level : 5
);
PrivateChooseMap.choice_fb.confirm_button =
	PrivateChooseMap.choice_fb.add_widget_to_label(
		PureFW.Container,
		0, PrivateChooseMap.choice_fb.widget_label.height
			- 20,
		PrivateChooseMap.choice_fb.widget_label.width,
		20
	);
PrivateChooseMap.choice_fb.confirm_button.set_text_align(
	'center');
PrivateChooseMap.choice_fb.confirm_button.set_content(
	'<b><a href="javascript: PrivateChooseMap.show_mentor_dialog();"><?=
		LNG_BTN_CONFIRM_MENTOR ?></a></b>'
);
PrivateChooseMap.choice_fb.add_overlay_content(
	'<div style="padding-top: 15%"><?= LNG_MENTOR_ISLAND ?></div>'
);
PrivateChooseMap.choice_fb.add_label_content(
	'<?= LNG_MENTOR_ISLAND_DESC ?>'
);

PrivateChooseMap.choice_fb.fb_friends =
	PrivateChooseMap.choice_fb.add_widget(
		PureFW.PolaroidMultiPic,
		PrivateChooseMap.choice_fb.picture.position.x,
		PrivateChooseMap.choice_fb.picture.position.y,
		PrivateChooseMap.choice_fb.picture.width,
		PrivateChooseMap.choice_fb.picture.height
	);
//PrivateChooseMap.choice_fb.xp_cont.destroy();
(function() {	<? /** local stack :) */ ?>
for (var x = 0, i = 0; x < 3; x++) {
	for (var y = 0; y < 2; y++, i++) {
		PrivateChooseMap.choice_fb.fb_friends.set_chosen_pic_url(i,
			PrivateChooseMap.fb_friends[i].avatar
		);
	}
}
})()

<?
/*
$n = count($fb_friends);
$chosen = array();
for ($i = 0; $i < $n && $i < 6; $i++) :
	$j = rand(0, $n);
	while(in_array($j, $chosen))
	{
		$j = rand(0, $n);
	}
	$chosen[] = $j;
	if ($fb_friends[$j]->avatar) : ?>
		PrivateChooseMap.choice_fb.fb_friends.set_chosen_pic_url(<?=$i?>,
				'<?=$fb_friends[$j]->avatar?>');
	<?
	endif;
endfor*/?>
<?
/**
 * Jetzt kommt die untere Zeile, die einem ggf. weiteres vorschlägt.
 */ ?>

PrivateChooseMap.pending_invitations_cont = PrivateChooseMap.this_window.
	add_widget
(
	PureFW.Container,
	PrivateChooseMap.invitations_cont.position.x,
	PrivateChooseMap.invitations_cont.position.y
		+ PrivateChooseMap.invitations_cont.height
		+ 15,
	PrivateChooseMap.invitations_cont.position.width,
	50
);

if (PrivateChooseMap.map_invitations &&
	(PrivateChooseMap.map_invitations.length > 2))
{
	PrivateChooseMap.pending_invitations_cont.set_content(
		'<?= LNG_YOU_HAVE_PENDING_INVITATIONS ?> '.replace('[AMOUNT]',
			PrivateChooseMap.map_invitations.length - 2)
			+ '<a href="javascript: PrivateChooseMap.show_invitations();">'
			+ '<?= LNG_BTN_SHOW_ALL_INVITATIONS ?>'
			+ '</a>.'
	);
}
else {
	PrivateChooseMap.pending_invitations_cont.set_content(
		'<?= LNG_YOU_HAVE_NO_PENDING_INVIS ?> '
			/*+ '<a href="javascript: PrivateChooseMap.show_mentor_dialog();">'
			+ '<?= LNG_BTN_CONFIRM_MENTOR ?>'
			+ '</a>.'*/
	);
}

PrivateChooseMap.start_new_map_question_cont = PrivateChooseMap.this_window.
	add_widget
(
	PureFW.Container,
	PrivateChooseMap.other_main_choices_cont.position.x,
	PrivateChooseMap.other_main_choices_cont.position.y
		+ PrivateChooseMap.other_main_choices_cont.height
		+ 15,
	PrivateChooseMap.other_main_choices_cont.width,
	50
);

PrivateChooseMap.start_new_map_question_cont.set_content(
	'<?= LNG_NOTHING_FITS_TEXT ?><br/>'
	+ '<b><a href="javascript: PrivateChooseMap.this_window.set_url(\''+
		'choose_map.php?dataX=<?=
			$cryptObj->do_encryption(array('start_new_map' => true)) ?>\');">'
	+ '<?= LNG_BTN_CREATE_A_NEW_ISLAND ?>'
	+ '</a>.</b>'
);

PrivateChooseMap.this_window.add_event_handler("destroy",
	PrivateChooseMap.destroy);
PrivateChooseMap.this_window.add_event_handler("change",
	PrivateChooseMap.destroy);

<? if ($current_user->get_state() == IUser::STATE_NEW_PLAYER) :?>
<? /**
	* Ein neuer Spieler soll das Interface noch nicht gleich komplett sehen:
	* grauer schleier drüber
	*/ ?>
PrivateChooseMap.bg = new PureFW.Container(
	document.body,
	0, 0,
	MammunUI.reference_width, MammunUI.reference_height
);
PrivateChooseMap.bg.set_bg_color('#e7eef6');
PrivateChooseMap.bg.set_z_index(299);

<? /**
	* Da ein neuer Spieler unfähig ist, zu entscheiden, was er will, wählen
	* wir für ihnn
	*/ ?>
if (PrivateChooseMap.map_invitations &&
								(PrivateChooseMap.map_invitations.length > 0))
	PrivateChooseMap.accept_chosen_map(PrivateChooseMap.map_invitations[0]);
else
	PrivateChooseMap.start_mentor_map();
<?
/**
 *  TODO: accept_chosen_map schägt fehl: Meldung schicken und
 *  start_mentor_map ausführen!
 */
?>

map_ui__fixed_window.set_z_index(300);


<? endif; ?>

<?
/**
 * Wenn der User vorher beim Verlassen der Insel bereits angegeben hat, wohin
 * er will, dann überspringen wir den Dialog und lassen ihn gleich zu der
 * Insel hüpfen, wohin er wollte.
 *
 * Mehr zu dieser Magic in ingame.inc: $_SESSION['join_map'] bzw. in
 * map_visit_navi.tmpl.inc und im PauseMenu::Leave_map_response
 */ ?>
if (PrivateChooseMap.auto_choose_map_invitation > -1) {
	PrivateChooseMap.accept_chosen_map(
		PrivateChooseMap.map_invitations[
			PrivateChooseMap.auto_choose_map_invitation
		]
	);
}
</script>
