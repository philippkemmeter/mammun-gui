<script type="text/javascript">
GameMenu = new Object();
GameMenu.game_menu_open = false;
GameMenu.UIButtons_bottom_container_vis = true;
GameMenu.hidden = false;
GameMenu.open_game_menu = function(ev) {
	if(GameMenu.game_menu_open || GameMenu.hidden) {
		GameMenu.close_game_menu();
	}
	else {
		try { ui_hide_map_scroll_arrows() } catch(e) {}
		GameMenu.toggle_button.set_y(92-GameMenu.toggle_button_size.y, 500);
		GameMenu.decoration_left.set_y(92-GameMenu.decoration_left_size.y, 500);
		GameMenu.decoration_right.set_y(92-GameMenu.decoration_right_size.y, 500);
		if (!GameMenu.content_container) {
			GameMenu.content_container = new PureFW.ContainerAJAX(
				document.body,
				0,92,
				MammunUI.reference_width, MammunUI.reference_height-92,
				"pause_menu.php"
			);
			GameMenu.content_container.set_z_index(100);
			GameMenu.content_container.set_bg_img(
				"ui/backgrounds/pause_menu/background.png");
		}
		else {
			GameMenu.content_container.show();
		}
		GameMenu.content_container.content_node.style.display = 'none';
		GameMenu.content_container.set_height(
			MammunUI.reference_height-92, 500, null,
			function () {
				PureFW.Timeout.set_timeout(
					function() {
						GameMenu.content_container.content_node.style.display
							= 'block';
					}, 300
				);
			}
		);
		GameMenu.content_container.set_y(92, 500);
		if (map_ui__fixed_window) {
			map_ui__fixed_window.set_z_index(90);
		}
		try {
			MapVisitNavi.map_browser_icon.hide();
			MapVisitNavi.map_browser_icon_label.hide();
			TutorialCtrl.hide();
		}
		catch(e) {}
		PureFW.Officer.set_officer_z_index(91);
		GameMenu.toggle_title.set_pic_url(
			'ui/elements/texts/<?=$LNG?>/pause_menu/to_the_island.png');
		try {
			GameMenu.UIButtons_bottom_container_vis =
				!UIButtons.bottom_container.is_hidden();
			UIButtons.bottom_container.hide();
		}
		catch(e){}
		GameMenu.game_menu_open = true;
		try { PauseMenu.refresh_active = true; } catch(e){}
		try { PauseMenu.rt_refresh(); } catch(e){}
	}
};
GameMenu.close_game_menu = function() {
	try{ PauseMenu.refresh_active = false; } catch(e) {}
	try { ui_show_map_scroll_arrows() } catch(e) {}
	GameMenu.toggle_button.set_y(660-GameMenu.toggle_button_size.y, 500);
	GameMenu.decoration_left.set_y(660-GameMenu.decoration_left_size.y, 500);
	GameMenu.decoration_right.set_y(660-GameMenu.decoration_right_size.y, 500);
	GameMenu.content_container.set_y(660, 500);
	GameMenu.content_container.content_node.style.display = 'none';
	GameMenu.content_container.set_height(
		1, 500, null, function() { GameMenu.content_container.hide(); }
	);
	GameMenu.toggle_title.set_pic_url(
		'ui/elements/texts/<?=$LNG?>/pause_menu/more.png');
	try {
		if (GameMenu.UIButtons_bottom_container_vis)
			UIButtons.bottom_container.show();
		else
			UIButtons.bottom_container.hide();
	}
	catch(e){}
	try {
		MapVisitNavi.map_browser_icon.show();
		MapVisitNavi.map_browser_icon_label.show();
		TutorialCtrl.show();
	}
	catch(e) {}
	GameMenu.game_menu_open = false;
	PureFW.Officer.set_officer_z_index(191);
}
GameMenu.hide = function() {
	GameMenu.decoration_left.hide();
	GameMenu.decoration_right.hide();
	GameMenu.toggle_button.hide();
	GameMenu.close_game_menu();
	GameMenu.hidden = true;
}

GameMenu.show = function() {
	GameMenu.decoration_left.show();
	GameMenu.decoration_right.show();
	GameMenu.toggle_button.show();
	GameMenu.hidden = false;
}

GameMenu.toggle_button_size = new PureFW.Point(158, 35);
GameMenu.decoration_left_size = new PureFW.Point(414, 15);
GameMenu.decoration_right_size = new PureFW.Point(428, 14);
GameMenu.toggle_button = new PureFW.Container(
	document.body,
	GameMenu.decoration_left_size.x,
	660-GameMenu.toggle_button_size.y,
	GameMenu.toggle_button_size.x,
	GameMenu.toggle_button_size.y
);
GameMenu.toggle_button.set_z_index(100);
GameMenu.toggle_button.set_bg_img(
		'ui/elements/pause_menu/' + MapDataCtrl.my_color + '/button_bg.png'
);
GameMenu.toggle_button.add_event_handler("click", GameMenu.open_game_menu);
GameMenu.toggle_title = new PureFW.Image(
	GameMenu.toggle_button,
	0, 0,
	GameMenu.toggle_button.width, GameMenu.toggle_button.height
);
GameMenu.toggle_title.set_pic_url(
	'ui/elements/texts/<?=$LNG?>/pause_menu/more.png'
);

GameMenu.decoration_left = new PureFW.Container(
	document.body,
	0,
	660-GameMenu.decoration_left_size.y,
	GameMenu.decoration_left_size.x,
	GameMenu.decoration_left_size.y
);
GameMenu.decoration_left.set_z_index(100);
GameMenu.decoration_left.set_bg_img(
		'ui/elements/pause_menu/' + MapDataCtrl.my_color + '/border_left.png'
);

GameMenu.decoration_right = new PureFW.Container(
	document.body,
	GameMenu.decoration_left_size.x+GameMenu.toggle_button_size.x,
	660-GameMenu.decoration_right_size.y,
	GameMenu.decoration_right_size.x,
	GameMenu.decoration_right_size.y
);
GameMenu.decoration_right.set_z_index(100);
GameMenu.decoration_right.set_bg_img(
		'ui/elements/pause_menu/' + MapDataCtrl.my_color + '/border_right.png'
);

GameMenu.content_container = new PureFW.ContainerAJAX(
	document.body,
	0,92,
	MammunUI.reference_width, MammunUI.reference_height-92,
	"pause_menu.php"
);
GameMenu.content_container.hide();
GameMenu.content_container.set_z_index(100);
GameMenu.content_container.set_bg_img(
	"ui/backgrounds/pause_menu/background.png");

GameMenu.update_color = function() {
	GameMenu.decoration_right.set_bg_img(
		'ui/elements/pause_menu/' + MapDataCtrl.my_color + '/border_right.png'
	);
	GameMenu.decoration_left.set_bg_img(
		'ui/elements/pause_menu/' + MapDataCtrl.my_color + '/border_left.png'
	);
	GameMenu.toggle_button.set_bg_img(
		'ui/elements/pause_menu/' + MapDataCtrl.my_color + '/button_bg.png'
	);
}
</script>
