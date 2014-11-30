<script type="text/javascript">
IslandLeft = new Object();

<?	/**
	 * Verweis auf das Fenster, in dem diese Seite hier dargestellt wird.
	 */ ?>
IslandLeft.this_window = PureFW.ContainerAJAX.instances[<?=$this_win_id?>];
IslandLeft.this_window.set_font_size(1.3);
IslandLeft.this_window.set_font_color('#333');
IslandLeft.this_window.set_text_shadow('#fff', 1, 0, 0);
<?	/**
	 * Der Spieler möchte eine neue Welt besiedeln.
	 */ ?>
IslandLeft.conquer_new_world = function() {
	IslandLeft.this_window.set_url('island_left.php?dataX=<?
		echo $cryptObj->do_encryption(array('choose_sciences'=>true)) ?>');
}

<?	/**
	 * Der Spieler möchte nochmal eine erste Karte spielen.
	 */ ?>
<?/* GIBBET NIMMER
IslandLeft.first_map_again = function() {
	PureFW.AJAXClientServer.send_request(
		'island_left.php?dataX=<?= $cryptObj->do_encryption(
				array(
					'choose_sciences' => true,
					'first_map_again' => true
				)) ?>',
		function (response_arr) {
			var _top = MammunUI.get_top_frame()
			_top.ui_auto_logout = false;
			_top.location.href = 'ingame.php?start=true';
		}
	);
}
*/?>

IslandLeft.destroy = function(ev) {
	IslandLeft.this_window.remove_event_handler("change", IslandLeft.destroy);
	IslandLeft.this_window.remove_event_handler("destroy", IslandLeft.destroy);
	LeaveMapWindow.set_bg_img(LeaveMapWindow.old_bg_img);
	if (ev.type == 'destroy') {
		UIButtons.open_leave_map_dialog();
	}
	destroy_object(IslandLeft);
	IslandLeft = null;
};


<?
/******************************************************************************
 *                  /\     .---.    -----    -     .---.   .  .               *
 *                 /__\    |          |      |     |   |   |`.|               *
 *                /    \   `---'      -      -     `---'   '  '               *
 *                                                                            *
 *                             A  C  T  I  O  N                            PK *
 ******************************************************************************/
?>

<? /* Dieser Container enthält alle Infos zentriert */ ?>
IslandLeft.main_container = new PureFW.Container(
	IslandLeft.this_window,
	0, 0,
	852, 469
);
LeaveMapWindow.old_bg_img = LeaveMapWindow.get_bg_img();

LeaveMapWindow.set_bg_img('ui/backgrounds/island_left/island_left_bg.png');

IslandLeft.caption = IslandLeft.main_container.add_widget(
		PureFW.Image,
		40,
		10,
		350,
		22,
		'ui/elements/texts/<?=$LNG?>/island_left/island_left_caption.png'
		);

IslandLeft.map_container = new PureFW.Container(
	IslandLeft.main_container,
	40,
	65,
	200, 225,
	'ui/elements/texts/<?=$LNG?>/island_left/last_island.png'
);

IslandLeft.map_img = IslandLeft.map_container.add_widget(
	PureFW.ChooseMap.NewIslandPreview,
	2, 1,
	192, 222,
	<?= $data->template_id; ?>,
	false,
	true
);
IslandLeft.map_label = new PureFW.Image(
	IslandLeft.main_container,
	IslandLeft.map_container.position.x - 15,
	IslandLeft.map_container.position.y - 15,
	112, 40,
	'ui/elements/texts/<?=$LNG?>/island_left/last_island.png'
);
IslandLeft.map_label.set_z_index(2);

IslandLeft.you_reached = IslandLeft.main_container.add_widget(
		PureFW.Container,
		50,
		285,
		250,
		50
		);

IslandLeft.you_reached.set_content('<?= LNG_ACHIEVEMENTS_TEXT?>');

IslandLeft.level = IslandLeft.main_container.add_widget(
		PureFW.Container,
		50,
		320,
		77,
		77
);
IslandLeft.level.set_bg_img('ui/icons/labels/xp_star/77/star.png');

IslandLeft.level_number = IslandLeft.level.add_widget(
		PureFW.Container,
		0,
		28,
		77,
		0
);
IslandLeft.level_number.set_text_align('center');
IslandLeft.level_number.set_content(
	'<b><?= User::get_current_user()->get_cur_level() ?></b>'
);

IslandLeft.score = IslandLeft.main_container.add_widget(
		PureFW.Container,
		120,
		350,
		100,
		50
		);
<? $score_diff = round(User::get_current_user()->get_cur_score()
-User::get_current_user()->get_last_score(), 0); ?>
IslandLeft.score.set_content('+<?= ($score_diff == '-0') ? '0' : $score_diff
	?> <?= LNG_POINTS_ACHIEVED_TEXT?>');


IslandLeft.text = IslandLeft.main_container.add_widget(
		PureFW.Container,
		450,
		50,
		350,0
		);

IslandLeft.text.set_content('<?= $text ?>');

IslandLeft.btn_conquer_new_world = new PureFW.Image(
	IslandLeft.main_container,
	IslandLeft.main_container.width-300,
	IslandLeft.main_container.height-130,
	252, 50,
	'ui/elements/input/buttons/<?=$LNG?>/island_left/new_island.png'
);
IslandLeft.btn_conquer_new_world.add_event_handler("click",
	IslandLeft.conquer_new_world
);

IslandLeft.this_window.add_event_handler("change", IslandLeft.destroy);
IslandLeft.this_window.add_event_handler("destroy", IslandLeft.destroy);
</script>
