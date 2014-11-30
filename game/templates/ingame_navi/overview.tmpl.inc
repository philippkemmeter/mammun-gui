<?
Language::get_instance()->load_language_file('overview.tmpl');
?>
<? if ($current_user->get_waiting_state() == IUser::STATE_MAP_IS_STARTING) : ?>
<?  /*
<!-- Count-Down -->
<div style="margin-left: 10px; margin-top:5px">
Count Down to Map Start:
<div id="count_down" style="font-size: 3em; color: #fff;">
<?= secInTime($current_user->get_waiting_finished() - Time::get_t_now()) ?>
</div>
</div>
*/ ?>
<script type="text/javascript">
rt_map_is_starting_count_down = function() {
	var t_now = PureFW.Time.get_current_server_time();
	var time_to_wait= <?= $current_user->get_waiting_finished() ?>- t_now;
	/*document.setElemInnerHTML('count_down',
		PureFW.Time.sec_in_time(time_to_wait));*/
	if (time_to_wait <= 0)
		location.href='ingame.php?start=true&dataX=<?=$cryptObj->do_encryption(array("relogin" => true))?>';
};
PureFW.Timeout.set_interval(rt_map_is_starting_count_down, 1000);
</script>
<? else :?>

<script type="text/javascript">
Overview = new Object();

Overview.fetch_treasure_offers_callback = function(response_arr) {
	if (response_arr) {
		Overview.items_overlay.set_content(response_arr[0]);
		if ((parseInt(response_arr[0]) <= 0) || (!response_arr[0]))
			Overview.items_overlay.hide();
		else
			Overview.items_overlay.show();
	}
	else {
		Overview.items_overlay.hide();
	}
}

Overview.fetch_treasure_offers_amount = function() {
	PureFW.AJAXClientServer.send_request(
		'treasure_chest.php?get_offers_amount=true',
		Overview.fetch_treasure_offers_callback
	);
}

Overview.button_size = new PureFW.Point(51, 70);
Overview.pic_size = new PureFW.Point(51, 54);
Overview.button_space = 10;
Overview.main_container = new PureFW.Container(
	document.body,
	0, 0,
	330,
	Overview.button_size.y+15
);
Overview.main_container.set_bg_img('ui/backgrounds/overview.png');
Overview.main_container.set_font_size(1.1);
Overview.main_container.set_z_index(190);
Overview.main_container.set_font_weight(700);

Overview.wood_label = new PureFW.Label(
	Overview.main_container,
	10,5,
	Overview.button_size.x,
	Overview.button_size.y,
	'<?= number_format ( $wood, 0, ',', '.' ) ?>'
);
Overview.wood_label.set_pic(
	'<?= pic_path("/ui/icons/labels/resources/54/wood.png") ?>',
	Overview.pic_size.x, Overview.pic_size.y,
	PureFW.Label.OVER
);
Overview.wood_label.set_title('<?= LNG_OWN_WOOD ?>');
var tmp = Overview.button_size.x+Overview.button_space;
var tmp_pos = tmp;
Overview.food_label = new PureFW.Label(
	Overview.main_container,
	10+tmp_pos, 5,
	Overview.button_size.x,
	Overview.button_size.y,
	'<?= number_format ( $food, 0, ',', '.' ) ?>'
);
Overview.food_label.set_pic(
	'<?= pic_path("/ui/icons/labels/resources/54/food.png") ?>',
	Overview.pic_size.x, Overview.pic_size.y,
	PureFW.Label.OVER
);
Overview.food_label.set_title('<?= LNG_OWN_FOOD ?>');
tmp_pos += tmp;
Overview.unit_limit_label = new PureFW.Label(
	Overview.main_container,
	10+tmp_pos, 5,
	Overview.button_size.x,
	Overview.button_size.y,
	'<?= $unit_limit[0] ?> / <?= $unit_limit[1] ?>'
);
Overview.unit_limit_label.set_pic(
	'<?= pic_path("/ui/icons/labels/resources/54/unit_limit.png")?>',
	Overview.pic_size.x, Overview.pic_size.y,
	PureFW.Label.OVER
);
Overview.unit_limit_label.set_title('<?= LNG_UNIT_LIMIT ?>');
tmp_pos += tmp;
Overview.klunker_label = new PureFW.Label(
	Overview.main_container,
	10+tmp_pos, 5,
	Overview.button_size.x,
	Overview.button_size.y,
	'<?= number_format ( $klunker, 0, ',', '.' ) ?>'
);
Overview.klunker_label.set_pic(
	'<?= pic_path("/ui/icons/labels/resources/54/klunker.png")?>',
	Overview.pic_size.x, Overview.pic_size.y,
	PureFW.Label.OVER
);
Overview.klunker_label.set_title('<?= LNG_OWN_KLUNKER ?>');
<? if (strtolower($current_user->get_nick()) == 'bernard') : ?>
Overview.klunker_label.add_event_handler(
	"click",
	function() {
		UIButtons.open_window('achievements.php');
	}
);
<? elseif (strtolower($current_user->get_nick()) == 'robert') : ?>
Overview.klunker_label.add_event_handler(
	"click",
	function() {
		UIButtons.toggle_navigation();
	}
);
<? endif; ?>
tmp_pos += tmp;
Overview.items_label = new PureFW.Label(
	Overview.main_container,
	10+tmp_pos, 5,
	Overview.button_size.x,
	Overview.button_size.y,
	'<?= number_format ( $items, 0, ',', '.' ) ?>'
);
Overview.items_label.set_pic(
	'<?= pic_path("/ui/icons/labels/resources/54/chest.png")?>',
	Overview.pic_size.x, Overview.pic_size.y,
	PureFW.Label.OVER
);
Overview.items_label.set_title('<?= LNG_OWN_ITEMS ?>');
Overview.items_label.add_event_handler(
	"click",
	UIButtons.toggle_treasure_chest
);

Overview.items_overlay = new PureFW.Container(
	Overview.main_container,
	Overview.items_label.position.x + Overview.items_label.width - 12,
	Overview.items_label.position.y - 3,
	25,
	25
);
Overview.items_overlay.set_bg_img(
	'ui/backgrounds/button_overlay_kringel/red/kringel.png'
);
Overview.items_overlay.set_z_index(2);
Overview.items_overlay.set_text_align('center');
Overview.items_overlay.set_font_size(1.2);
Overview.items_overlay.set_font_weight('bold');
Overview.items_overlay.get_content_node().style.paddingTop = '10%';
Overview.items_overlay.set_content(0);
Overview.items_overlay.hide();

Overview.xp_bar = new PureFW.MapUI.XPBar(
	document.body,
	((MammunUI.reference_width - 238) >> 1), 15,
	238, 60,
	210
);
Overview.xp_bar.set_z_index(2);

<? /**
	* Genie-Streich: XP-Bar soll ansich korrekt überlappen, aber wenn
	* ConfirmationBox-Widgets erstellt werden, dann soll es als einziges Widget
	* einen höheren z-Index bekommen, als der weißte Schleier, der alles
	* ausblendet. Deswegen überschreiben wir jetzt den Konstruktor und den
	* Destruktor von der ConfirmationBox-Klasse, dass der XP-Bar-z-Index immer
	* automatisch korrekt angepasst wird.
    **/ ?>
Overview.confbox_insert_into_dom_tree
	= PureFW.ConfirmationBox.prototype.insert_into_dom_tree;
PureFW.ConfirmationBox.prototype.insert_into_dom_tree = function() {
	Overview.confbox_insert_into_dom_tree.call(this);
	this.old_xp_bar_z_index = Overview.xp_bar.get_z_index();
	Overview.xp_bar.set_z_index(this.get_z_index()+2);
}
Overview.confbox_destroy = PureFW.ConfirmationBox.prototype.destroy;
PureFW.ConfirmationBox.prototype.destroy = function() {
	Overview.confbox_destroy.call(this);
	Overview.xp_bar.set_z_index(this.old_xp_bar_z_index);
}
<? /**
	* Ende des JS-Hacks :)
	*/ ?>

Overview.xp_bar.set_xp_level(
	<?= User::get_current_user()->get_cur_level() ?>
);
Overview.xp_bar.set_xp_score_next_level(
	<?= GameScoreCalculation::calc_score_by_level(
		User::get_current_user()->get_cur_level()+1) ?>
);
Overview.xp_bar.set_xp_score_after_next_level(
	<?= GameScoreCalculation::calc_score_by_level(
		User::get_current_user()->get_cur_level()+2) ?>
);
Overview.xp_bar.set_xp_score_last_level(
	<?= GameScoreCalculation::calc_score_by_level(
		User::get_current_user()->get_cur_level()) ?>
);
Overview.xp_bar.set_xp_score(
	<?= User::get_current_user()->get_cur_score() ?>
);
Overview.xp_bar.set_tooltip(
	'<?= LNG_TOOLTIP_XP_LEVEL ?>'
);

Overview.unit_produced = <?= $unit_limit[0] ?>;
Overview.unit_limit = <?= $unit_limit[1] ?>;
Overview.increase_unit_produced = function(by_amount) {
	if (typeof(by_amount) == 'undefined')
		by_amount = 1;
	Overview.unit_produced += by_amount;
	Overview.refresh_cur_unit_limit();
};
Overview.increase_unit_limit = function(by_amount) {
	if (typeof(by_amount) == 'undefined')
		by_amount = 1;
	Overview.unit_limit += by_amount;
	Overview.refresh_cur_unit_limit();
};
Overview.set_unit_produced_and_limit = function(prod, limit) {
	prod = parseInt(prod);
	if (prod == Number.NaN)
		throw new Error('Given amount is NaN');
	if (prod <= 0)
		throw new Error('Given amount is negative');
	limit = parseInt(limit);
	if (limit == Number.NaN)
		throw new Error('Given amount is NaN');
	if (limit <= 0)
		throw new Error('Given amount is negative');
	Overview.unit_produced = prod;
	Overview.unit_limit = limit;
	Overview.refresh_cur_unit_limit();
};
Overview.refresh_cur_unit_limit = function()
{
	var divider;
	if(!(!(Overview.unit_produced > 99) && !(Overview.unit_limit > 99)))
	{
		divider = '/';
	}
	else
	{
		divider = ' / ';
	}
	Overview.unit_limit_label.set_text(
		Overview.unit_produced + divider + Overview.unit_limit
	);
};
Overview.get_unit_produced = function() {
	return parseInt(Overview.unit_produced);
};
Overview.get_unit_limit = function() {
	return parseInt(Overview.unit_limit);
};

Overview.set_item_amount = function(amount) {
	ui_items_gainer.set_current_amount(amount);
};
Overview.increase_item_amount = function(amount) {
	ui_items_gainer.set_current_amount(
		ui_items_gainer.get_cur_value() + amount
	);
}
Overview.set_offers_amount = function(amount) {
	Overview.items_overlay.set_content(amount);
	if (!amount)
		Overview.items_overlay.hide();
}

ui_food_gainer.set_current_amount(<?= $food ?>);
ui_food_gainer.set_gain_per_second(<?= ($food_gain)/3600 ?>);
ui_wood_gainer.set_current_amount(<?= $wood ?>);
ui_wood_gainer.set_gain_per_second(<?= ($wood_gain)/3600 ?>);
ui_klunker_gainer.set_current_amount(<?= $klunker ?>);
ui_klunker_gainer.set_gain_per_second(<?= ($klunker_gain)/3600 ?>);
ui_items_gainer.set_current_amount(<?= $items ?>);
ui_items_gainer.set_gain_per_second(<?= $items_gain/3600 ?>);
Overview.refresh_cur_unit_limit();

Overview.fetch_treasure_offers_amount();
PureFW.Timeout.set_interval(Overview.fetch_treasure_offers_amount, 10000);
</script>
<? endif; ?>
