UnitSideInfo = new Object();

UnitSideInfo.information_container = new PureFW.Container(
	UnitProduction.this_window,
	750, 40,
	192, 400
);
UnitSideInfo.information_container.set_font_color('black'),
UnitSideInfo.information_container.set_font_size(1.5);
UnitSideInfo.information_container.set_font_weight(700);

	UnitSideInfo.t_n_label = new PureFW.Label(UnitSideInfo.information_container,
		0,0,0,0,'----');
	UnitSideInfo.t_n_label.set_pic('ui/icons/labels/resources/44/wecker.png',
		40,40);
	UnitSideInfo.t_n_label.set_positioning('relative');
	UnitSideInfo.t_n_label.set_title('<?= LNG_TIME_NEEDED ?>');

	UnitSideInfo.cost_label = new PureFW.Label(UnitSideInfo.information_container,
		0,0,0,0,'----');
	UnitSideInfo.cost_label.set_pic('ui/icons/labels/resources/44/food.png',
		40,40);
	UnitSideInfo.cost_label.set_positioning('relative');
	UnitSideInfo.cost_label.set_title('<?= LNG_PRICE_PER_UNIT ?>');

	UnitSideInfo.kkr_label = new PureFW.Label(UnitSideInfo.information_container,
		0,0,0,0,"----");
	UnitSideInfo.kkr_label.set_pic('ui/icons/labels/resources/44/klunker.png',
		40,40);
	UnitSideInfo.kkr_label.set_positioning('relative');
	UnitSideInfo.kkr_label.set_title('<?= LNG_KLUNKER_PER_UNIT ?>');

	UnitSideInfo.att_label = new PureFW.Label(
		UnitSideInfo.information_container,
		0,0,0,0,'----');
	UnitSideInfo.att_label.set_pic('ui/icons/labels/attributes/44/strength.png',
		40,40);
	UnitSideInfo.att_label.set_positioning('relative');
	UnitSideInfo.att_label.set_title('<?= LNG_ATTACK_ATTRIBUTE ?>');

	UnitSideInfo.def_label = new PureFW.Label(
		UnitSideInfo.information_container,
		0,0,0,0,'----');
	UnitSideInfo.def_label.set_pic('ui/icons/labels/attributes/44/dexterity.png',
		40,40);
	UnitSideInfo.def_label.set_positioning('relative');
	UnitSideInfo.def_label.set_title('<?= LNG_DEFENSE_ATTRIBUTE ?>');

	UnitSideInfo.hp_label = new PureFW.Label(
		UnitSideInfo.information_container,
		0,0,0,0,'----');
	UnitSideInfo.hp_label.set_pic('ui/icons/labels/attributes/44/health.png',
		40,40);
	UnitSideInfo.hp_label.set_positioning('relative');
	UnitSideInfo.hp_label.set_title('<?= LNG_HITPOINTS_ATTRIBUTE ?>');

	UnitSideInfo.speed_label = new PureFW.Label(
		UnitSideInfo.information_container,
		0,0,0,0,'----');
	UnitSideInfo.speed_label.set_pic('ui/icons/labels/attributes/44/speed.png',
		40,40);
	UnitSideInfo.speed_label.set_positioning('relative');
	UnitSideInfo.speed_label.set_title('<?= LNG_SPEED_ATTRIBUTE ?>');

UnitSideInfo.back_next_button_container = new PureFW.Container(
	UnitSideInfo.information_container, 0, 350, 192, 30
);

UnitSideInfo.destroy = function() {
	destroy_object(UnitSideInfo);
	UnitSideInfo = null;
}