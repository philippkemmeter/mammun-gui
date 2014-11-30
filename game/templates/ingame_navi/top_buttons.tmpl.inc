<? Language::get_instance()->load_language_file('top_buttons.tmpl') ?>
<script type="text/javascript">
<?/*important_info_box = new PureFW.ConfirmationBox(
	document.body,
	(MammunUI.reference_width-350)>>1,
	(MammunUI.reference_height-200)>>1,
	350, 200,
	PureFW.ConfirmationBox.YES
);
important_info_box.set_bg_img(
	'ui/backgrounds/confirm/info/info_350x200.png'
);
important_info_box.set_content(
	"<div style='width:90%; left: 5%; top: 7%; width:90%;position:absolute;'>\
	Nur noch für kurze Zeit! Novemberpreise.<br/>\
	<br/>\
	Profitier noch bis zum 30. November 23.59 Uhr von unseren tollen\
	Novemberpreisen im <a href='javascript: UIButtons.toggle_shop()'>Shop</a>.\
	Hüte, Afros, Röcke und vieles mehr zu Top-Preisen!<br/>\
	Stöber' durch den <a href='javascript: UIButtons.toggle_shop()'>Shop</a>,\
	füll' deine Schatzkiste und handle dann mit deinen Freunden…<br/>\
	<br/>\
	Und für die Adventszeit? Na da wird's Edles günstig geben...</div>"
);*/?>

UIButtons = new Object();

UIButtons.fetch_unread_message_callback = function(response_arr) {
	if (response_arr) {
		UIButtons.tb1_overlay.set_content(response_arr[0]);
		if ((parseInt(response_arr[0]) <= 0) || (!response_arr[0]))
			UIButtons.tb1_overlay.hide();
		else
			UIButtons.tb1_overlay.show();
	}
	else {
		UIButtons.tb1_overlay.hide();
	}
};

UIButtons.fetch_unread_message_count = function() {
	PureFW.AJAXClientServer.send_request(
		'../community/gausus_wcf_hacks/pm_interface.php?get_unread_messages=<?
			echo urlencode(User::get_current_user()->get_nick()) ?>&s=<?
			echo $_SESSION['WCF_SID']?>&t=<?= $_SESSION['WCF_ST'] ?>',
			UIButtons.fetch_unread_message_callback
	);
}

UIButtons.open_window = function(url, title, z_index) {
	if (url.indexOf('profile.php') != -1) {
		var tmp = url.split('?');
		UIButtons.toggle_profile(tmp[1]);
		return;
	}
	if (get_cur_fixed_window_url().indexOf(url) >= 0) {
		close_fixed_window();
	}
	MapVisitNavi.map_browser_icon_label.hide();
	open_fixed_window(url, '', 980, 488, 10, 120);
	map_ui__fixed_window.set_z_index(z_index);
}
UIButtons.toggle_window = function(url, title, z_index, w, h, x, y) {
	try {SectorDetails.unit_popup.destroy();} catch(e){}
	try {SectorDetails.unit_details_box.destroy();} catch(e){}

	if (url.indexOf('profile.php') != -1) {
		var tmp = url.split('?');
		UIButtons.toggle_profile(tmp[1]);
		return;
	}

	if (typeof(z_index) === 'undefined')
		z_index = 90;
	if (get_cur_fixed_window_url().indexOf(url) >= 0) {
		close_fixed_window();
	}
	else
	{
		MapVisitNavi.map_browser_icon_label.hide();
		open_fixed_window(url,'', (w || 980), (h || 488), (x || 10), (y || 120));
		map_ui__fixed_window.set_z_index(z_index);
	}
}

UIButtons.open_new_window = function(url, z_index, x, y, w, h) {
	if (!w)
		w = 980;
	if (!h)
		h = 488;
	var window = new PureFW.WindowAJAX(
		document.body,
		x || 10, y || 120,
		w, h,
		url,
		''
	);
	window.set_bg_img(
			'ui/backgrounds/window/'+MapDataCtrl.my_color+
				'/window-'+w+'x'+h+'.png'
	);
	window.set_z_index(z_index || 200);
	return window;
}

UIButtons.toggle_unit_wizard = function(additional_post)
{
	if (additional_post)
		UIButtons.toggle_window('unit_production.php?'+additional_post);
	else
		UIButtons.toggle_window('unit_production.php');
}

UIButtons.toggle_im_window = function() {
	UIButtons.toggle_window('../community/wbb/index.php?page=PMListPlain&s=<?
		echo $_SESSION['WCF_SID']?>&t=<?= $_SESSION['WCF_ST'] ?>',
			"Nachrichtendienst", 110);
}

UIButtons.toggle_buy_klunker = function() {
	UIButtons.toggle_window('buy_klunker.php', '', 110);
}

UIButtons.toggle_treasure_chest = function(additional_post, as_popup) {
	if (typeof(additional_post) == 'undefined')
		additional_post = '';
	else
		additional_post = '?'+additional_post;

	if (as_popup) {
		var window = UIButtons.open_new_window(
			'treasure_chest.php'+additional_post, 300
		);
		window.set_bg_img(
			'ui/backgrounds/window/'
			+ MapDataCtrl.my_color + '/'
			+ 'window-980x488-treasure_chest.png'
		);
	}
	else {
		UIButtons.toggle_window('treasure_chest.php'+additional_post, '', 300);

		if (map_ui__fixed_window) {
			map_ui__fixed_window.set_bg_img(
				'ui/backgrounds/window/'
				+ MapDataCtrl.my_color + '/'
				+ 'window-980x488-treasure_chest.png'
			);
		}
	}
}

UIButtons.toggle_shop = function()
{
	UIButtons.toggle_window('shop.php', '', 110/*,976, 516*/);
	if (map_ui__fixed_window) {
		map_ui__fixed_window.set_bg_img(
			'ui/backgrounds/window/'
				+ MapDataCtrl.my_color + '/window-980x488-shop.png'
		);
	}
}

UIButtons.toggle_science_window = function()
{
	UIButtons.toggle_window('science.php', '');
}

UIButtons.toggle_buildings_window = function(additional_post)
{
	if (typeof(additional_post) == 'undefined')
		additional_post = '';
	else
		additional_post = '?'+additional_post;
	UIButtons.toggle_window('buildings.php'+additional_post, '');
}

UIButtons.toggle_game_menu = function()
{
	if (IngameNavi.pause_menu.box.is_hidden())
	{
		UIButtons.show_game_menu();
	}
	else
	{
		UIButtons.hide_game_menu();
	}
}

UIButtons.toggle_friends = function()
{
	UIButtons.toggle_window('../community/wbb/index.php', '');
}

UIButtons.hide_game_menu = function()
{
//	IngameNavi.pause_menu.box.hide();
	resume_game();
}
<? /*
UIButtons.toggle_board = function() {
	UIButtons.toggle_window('../community/wbb/index.php?page=Index', "");
} */ ?>
UIButtons.show_game_menu = function()
{
//	IngameNavi.pause_menu.box.show();
	pause_game();
}

UIButtons.toggle_notes = function()
{
	if (!UINotes.notes_cont.is_hidden()) {
		UINotes.notes_cont.hide();
		ui_notes_save();
	}
	else {
		UINotes.notes_cont.show();
	}
}

UIButtons.toggle_new_message = function(additional_post)
{
	if (typeof(additional_post) === 'undefined')
		additional_post = '';
	else
		additional_post = '&'+additional_post;

	if (typeof(UINewIM) === 'undefined') {
		UINewIM = new Object();
	}
	if (UINewIM.window) {
		try {
			UINewIM.window.destroy();
		}
		catch (e) {
		}
		UINewIM.window = null;
	}
	UINewIM.window = new PureFW.WindowAJAX(
			document.body,
			50, 85,
			492, 483,
			'../community/wbb/index.php?form=PMNewPlain&s=<?
					echo $_SESSION['WCF_SID']?>&t=<?= $_SESSION['WCF_ST'] ?>'
				+additional_post, ''
	);
	UINewIM.window.set_bg_img(
			'ui/backgrounds/window/'
				+ MapDataCtrl.my_color + '/window-492x483.png'
	);
	UINewIM.window.set_z_index(205);
}

UIButtons.toggle_profile = function(additional_post) {
	if (typeof(additional_post) === 'undefined')
		additional_post = '';
	else
		additional_post = '&'+additional_post;

	if (typeof(UINewProfile) === 'undefined') {
		UINewProfile = new Object();
	}
	if (UINewProfile.window) {
		try {
			UINewProfile.window.destroy();
		}
		catch (e) {
		}
		UINewProfile.window = null;
	}
	UINewProfile.window = new PureFW.WindowAJAX(
			document.body,
			10, 120, 980, 488,
			'profile.php?'+additional_post, ''
	);
	UINewProfile.window.set_bg_img(
			'ui/backgrounds/window/'
				+ MapDataCtrl.my_color + '/window-980x488.png'
	);
	UINewProfile.window.set_z_index(300);
}

UIButtons.open_leave_map_dialog = function(additional_post)
{
	if (typeof(additional_post) === 'undefined')
		additional_post = '';
	else
		additional_post = '?'+additional_post;

	try {
		LeaveMapWindow.destroy();
	}
	catch (e) {	}
	LeaveMapWindow = null;

	LeaveMapWindow = new PureFW.WindowAJAX(
			document.body,
			(MammunUI.reference_width - 856)>>1, 120,
			856, 473,
			'island_left.php'
				+additional_post, ''
	);
	LeaveMapWindow.set_bg_img(
			'ui/backgrounds/window/'
				+ MapDataCtrl.my_color + '/window-856x473.png'
	);
	LeaveMapWindow.set_z_index(90);
}

UIButtons.open_col_choose_dialog = function(additional_post) {
	if (typeof(additional_post) === 'undefined')
		additional_post = '';
	else
		additional_post = '?'+additional_post;
	UIButtons.toggle_window('col_pos_choose.php'+additional_post,
		'', 90, 980, 520, 10, 80);
}

UIButtons.open_friend_invitation_friend_popup = function() {
	try {
		InvitationWindowPopup.destroy();
	}
	catch (e) {	}
	InvitationWindowPopup = null;
	InvitationWindowPopup = new PureFW.WindowAJAX(
			document.body,
			(MammunUI.reference_width - 575)>>1, 120,
			575, 473,
			'invitation2.php?mammun_friends=true', ''
	);
	InvitationWindowPopup.set_bg_img(
		'ui/backgrounds/window/'
			+ MapDataCtrl.my_color + '/window-575x473.png'
	);
	InvitationWindowPopup.set_z_index(300);
}

UIButtons.open_friend_invitation = function(general) {
	try {
		InvitationWindow.destroy();
	}
	catch (e) {	}
	InvitationWindow = null;

	if (MapDataCtrl.is_mentor_map) {
		if (general) {
			InvitationWindow = new PureFW.WindowAJAX(
				document.body,
				(MammunUI.reference_width - 516)>>1, 120,
				516, 461,
				(general) ? 'invitation2.php?general_invitation=true'
					: 'invitation2.php', ''
			);
			InvitationWindow.set_bg_img(
				'ui/backgrounds/window/'
					+ MapDataCtrl.my_color + '/window-516x461.png'
			);
		}
		else {
			InvitationWindow = new PureFW.WindowAJAX(
					document.body,
					(MammunUI.reference_width - 717)>>1, 120,
					717, 484,
					'invitation2.php', ''
			);
			InvitationWindow.set_bg_img(
				'ui/backgrounds/window/'
					+ MapDataCtrl.my_color + '/window-717x484.png'
			);
		}
	}
	else {
		InvitationWindow = new PureFW.WindowAJAX(
				document.body,
				(MammunUI.reference_width - 575)>>1, 120,
				575, 473,
				'invitation2.php', ''
		);
		InvitationWindow.set_bg_img(
			'ui/backgrounds/window/'
				+ MapDataCtrl.my_color + '/window-575x473.png'
		);
	}
	InvitationWindow.set_z_index(300);
}

UIButtons.toggle_friend_invitation = function(general) {
	if (typeof(InvitationWindow) == 'undefined')
		InvitationWindow = null;
	if (!InvitationWindow)
		UIButtons.open_friend_invitation(general);
	else {
		try {
			InvitationWindow.destroy();
		}
		catch (e) {
			InvitationWindow = null;
			UIButtons.open_friend_invitation(general);
		}
		InvitationWindow = null;
	}
}

UIButtons.toggle_faq_window = function()
{
	UIButtons.toggle_window("faq.php", '', 120);
	/*if (get_cur_fixed_window_url()
				.indexOf("faq.php") >= 0)
	{
		close_fixed_window();
	}
	else {
		open_fixed_window(
			"faq.php", '', 980, 488, 10, 120
		);
		map_ui__fixed_window.set_z_index(120);
	}*/
}
UIButtons.toggle_settings_window = function()
{
	if (typeof(UISettings) === 'undefined')
	{
		UISettings = new Object();
	}
	if (UISettings.window)
	{
		try
		{
			UISettings.window.destroy();
		}
		catch (e)
		{
		}
		UISettings.window = null;
	}
//	UIButtons.toggle_window('game_settings.php', 'Settings', 120,
//			717, 484, 160, 140);
	UISettings.window = new PureFW.WindowAJAX(
			document.body,
			160, 140,
			717, 484,
			'game_settings.php'
	);
	UISettings.window.set_bg_img(
			'ui/backgrounds/window/'
				+ MapDataCtrl.my_color + '/window-717x484.png'
	);
	UISettings.window.set_z_index(120);
};

 /*
UIButtons.toggle_forum_window = function()
{
	UIButtons.toggle_window('../community/wbb/index.php?', "Forum")
}*/

UIButtons.toggle_bugreport_window = function()
{
	UIButtons.toggle_window('bugreport.php', '');
}

UIButtons.show_link_to_island_help = function() {
	MammunUI.show_warning(
		360, 255, '<?= str_replace("'", "\'", LNG_LINK_TO_ISLAND_HELP) ?>',
		PureFW.ConfirmationBox.YES
	);
}

UIButtons.navigation_hidden = false;
UIButtons.toggle_navigation = function() {
	if (UIButtons.navigation_hidden) {
		UIButtons.navigation_hidden = false;
		UIButtons.top_container.show();
		UIButtons.bottom_container.show();
		UIButtons.bb1.show();
		UIButtons.bb2.show();
		UIButtons.bb3.show();
		Overview.main_container.show();
		Overview.xp_bar.show();
		GameMenu.show();
		MapVisitNavi.map_browser_icon_label.show();
		MapVisitNavi.map_browser_icon.show();
	}
	else {
		UIButtons.navigation_hidden = true;
		UIButtons.top_container.hide();
		UIButtons.bottom_container.hide();
		UIButtons.bb1.hide();
		UIButtons.bb2.hide();
		UIButtons.bb3.hide();
		Overview.main_container.hide();
		Overview.xp_bar.hide();
		GameMenu.hide();
		MapVisitNavi.map_browser_icon_label.hide();
		MapVisitNavi.map_browser_icon.hide();
	}
}


UIButtons.destroy_all_windows = function() {
	try
	{
		UISettings.window.destroy();
		UISettings.window = null;
	} catch (e) {}
	try
	{
		UIInvitation.window.destroy();
		UIInvitation.window = null;
	} catch (e) {}
	try {
		LeaveMapWindow.destroy();
		LeaveMapWindow = null;
	} catch (e) {}
	try {
		UINewIM.window.destroy();
		UINewIM.window = null;
	} catch (e) {}
	try {
		UINewProfile.window.destroy();
		UINewProfile.window = null;
	} catch (e) {}
	close_fixed_window();
}


UIButtons.top_buttons_size = new PureFW.Point(51, 54);
UIButtons.top_buttons_space = 10;
<?/**
 * Der top_container enthält alle Buttons, die oben zur Navigation dienen.
 * @var PureFW.Container
 */?>
UIButtons.top_container = new PureFW.Container(
	document.body,
	MammunUI.reference_width-330,
	0,
	330,
	85
);
UIButtons.top_container.set_z_index(190);
UIButtons.top_container.set_bg_img('ui/backgrounds/map_ui_top/background.png');

UIButtons.tb1 = new PureFW.Image(
	UIButtons.top_container, 15, 10,
	UIButtons.top_buttons_size.x, UIButtons.top_buttons_size.y,
	'ui/icons/labels/top_navi/pm.png'
);
UIButtons.tb1.set_tooltip('<?= LNG_TITLE_PM_BUTTON ?>');
UIButtons.tb1.add_event_handler("click", UIButtons.toggle_im_window);
UIButtons.tb1_overlay = new PureFW.Container(
	UIButtons.top_container,
	UIButtons.tb1.position.x + UIButtons.tb1.width - 12,
	UIButtons.tb1.position.y - 8,
	25,
	25
);
UIButtons.tb1_overlay.set_bg_img(
	'ui/backgrounds/button_overlay_kringel/red/kringel.png'
);
UIButtons.tb1_overlay.set_z_index(2);
UIButtons.tb1_overlay.set_text_align('center');
UIButtons.tb1_overlay.set_font_size(1.2);
UIButtons.tb1_overlay.set_font_weight('bold');
UIButtons.tb1_overlay.get_content_node().style.paddingTop = '10%';
UIButtons.tb1_overlay.set_content(0);
UIButtons.tb1_overlay.hide();

var tmp = UIButtons.top_buttons_space+UIButtons.top_buttons_size.x;
var tmp_xpos = tmp+UIButtons.tb1.position.x;

UIButtons.tb2 = new PureFW.Image(
	UIButtons.top_container,
	tmp_xpos, UIButtons.tb1.position.y,
	UIButtons.top_buttons_size.x, UIButtons.top_buttons_size.y,
	'ui/icons/labels/top_navi/buy-klunker.png'
);
UIButtons.tb2.set_tooltip('<?= LNG_TITLE_KKR_BUTTON ?>');
UIButtons.tb2.add_event_handler("click", UIButtons.toggle_buy_klunker);

tmp_xpos += tmp;
UIButtons.tb3 = new PureFW.Image(
	UIButtons.top_container,
	tmp_xpos, UIButtons.tb1.position.y,
	UIButtons.top_buttons_size.x, UIButtons.top_buttons_size.y,
	'ui/icons/labels/top_navi/shop.png'
);
UIButtons.tb3.set_tooltip('<?= LNG_TITLE_SHOP_BUTTON ?>');
UIButtons.tb3.add_event_handler("click", UIButtons.toggle_shop);

tmp_xpos += tmp;
UIButtons.tb4 = new PureFW.Image(
	UIButtons.top_container,
	tmp_xpos, UIButtons.tb1.position.y,
	UIButtons.top_buttons_size.x, UIButtons.top_buttons_size.y,
	'ui/icons/labels/top_navi/notes.png'
);
UIButtons.tb4.set_tooltip('<?= LNG_TITLE_NOTES_BUTTON ?>');
UIButtons.tb4.add_event_handler("click", UIButtons.toggle_notes);

tmp_xpos += tmp;
UIButtons.tb5 = new PureFW.Image(
	UIButtons.top_container,
	tmp_xpos, UIButtons.tb1.position.y,
	UIButtons.top_buttons_size.x, UIButtons.top_buttons_size.y,
	'ui/icons/labels/top_navi/help.png'
);
UIButtons.tb5.set_tooltip('<?= LNG_TITLE_FAQ_BUTTON ?>');
UIButtons.tb5.add_event_handler("click", UIButtons.toggle_faq_window);

<? if ($current_user->get_state() == IUser::STATE_PLAYING) : ?>
UIButtons.bottom_buttons_size = new PureFW.Point(88, 106);
UIButtons.bottom_buttons_space = 5;
<?/**
 * Der bottom_container enthält alle Buttons, die unten zur Navigation dienen.
 * @var PureFW.Container
 */?>
UIButtons.bottom_container = new PureFW.Container(
	document.body,
	50,
	660-UIButtons.bottom_buttons_size.y,
	UIButtons.bottom_buttons_size.x*3+UIButtons.bottom_buttons_space*2,
	UIButtons.bottom_buttons_size.y
);
UIButtons.bottom_container.set_z_index(91);
UIButtons.bottom_container.set_overflow('visible');

UIButtons.bb1 = new PureFW.Image(
	UIButtons.bottom_container,
	0,0,
	UIButtons.bottom_buttons_size.x, UIButtons.bottom_buttons_size.y,
	'ui/icons/labels/bottom_navi/grandpa.png'
);
UIButtons.bb1.set_tooltip('<?= LNG_TITLE_TRAINING_BUTTON ?>');
UIButtons.bb1.add_event_handler("click", UIButtons.toggle_unit_wizard);

tmp = UIButtons.bottom_buttons_space + UIButtons.bottom_buttons_size.x;
tmp_xpos = tmp;
UIButtons.bb2 = new PureFW.Image(
	UIButtons.bottom_container,
	tmp_xpos, 0,
	UIButtons.bottom_buttons_size.x, UIButtons.bottom_buttons_size.y,
	'ui/icons/labels/bottom_navi/builder.png'
);
UIButtons.bb2.set_tooltip('<?= LNG_TITLE_BUILDINGS_BUTTON ?>');
UIButtons.bb2.add_event_handler("click", UIButtons.toggle_buildings_window);

tmp_xpos += tmp;
UIButtons.bb3 = new PureFW.Image(
	UIButtons.bottom_container,
	tmp_xpos, 0,
	UIButtons.bottom_buttons_size.x, UIButtons.bottom_buttons_size.y,
	'ui/icons/labels/bottom_navi/grandma.png'
);
UIButtons.bb3.set_tooltip('<?= LNG_TITLE_SCIENCE_BUTTON ?>');
UIButtons.bb3.add_event_handler("click", UIButtons.toggle_science_window);

UIButtons.bb3_zzz = new PureFW.FilmAnimator(
	UIButtons.bottom_container,
	UIButtons.bb3.position.x + ((UIButtons.bb3.width - 80)>>1),
	UIButtons.bb3.position.y - 70,
	80, 80
);
UIButtons.bb3_zzz.set_image(
	'ui/animations/zzz/80/zzz.png', 4
);
UIButtons.bb3_zzz.set_frame_delay(200);
UIButtons.bb3_zzz.set_z_index(UIButtons.bb3.get_z_index()+1);
<? if ($sc_total <= $sc_at_work) : ?>
UIButtons.bb3_zzz.hide();
<? else : ?>
UIButtons.bb3_zzz.start_animation();
<? endif; ?>

UIButtons.bb3.add_event_handler("hide", function(ev) {
	UIButtons.bb3_zzz.hide();
});
/*UIButtons.bb3.add_event_handler("show", function(ev) {
	UIButtons.bb3_zzz.show();
});*/
<? endif; ?>

UIButtons.fetch_unread_message_count();
PureFW.Timeout.set_interval(UIButtons.fetch_unread_message_count, 10000);
</script>
