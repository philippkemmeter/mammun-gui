if (!((typeof(ChestShopFilter) != 'undefined') && ChestShopFilter
				&& (typeof(ChestShopFilter) == 'object')))
	ChestShopFilter = new Object();

<? /* Filter für die Itemliste */ ?>
ChestShopFilter.cur_filter = new Object();
ChestShopFilter.cur_filter.shop = 0;
ChestShopFilter.cur_filter.type = 0;
ChestShopFilter.cur_filter.discount = false;
ChestShopFilter.cur_filter.color = 0;
ChestShopFilter.cur_filter.similar_items = -1;

ChestShopFilter.old_filter = new Object();
ChestShopFilter.old_filter.shop = 0;
ChestShopFilter.old_filter.type = 0;
ChestShopFilter.old_filter.discount = false;
ChestShopFilter.old_filter.color = 0;
ChestShopFilter.old_filter.similar_items = -1;

ChestShopFilter.on_filter_change_functions = new Array();

<? /**
    * Setzt den angegebenen Filter auf den angegebenen Wert. Für which sind die
    * Strings "type" und "shop" möglich. Für Filter die entsprechenden IDs
    */ ?>
ChestShopFilter.set_filter = function(which, filter) {
	if (filter == 'history_back') {
		ChestShopFilter.cur_filter[which] = ChestShopFilter.old_filter[which];
		filter = ChestShopFilter.cur_filter[which];
	}
	else {
		ChestShopFilter.old_filter[which] = ChestShopFilter.cur_filter[which];
		ChestShopFilter.cur_filter[which] = filter;
	}
	if (which == 'color') {
		ChestShopFilter.color_filter.select_item(filter);
	}
	else if (which == 'type') {
		switch(filter) {
			case 5:
				ChestShopFilter.category_filter.select_item(2);
				break;
			case 1:
				ChestShopFilter.category_filter.select_item(3);
				break;
			case 2:
				ChestShopFilter.category_filter.select_item(4);
				break;
			case 10:
				ChestShopFilter.category_filter.select_item(6);
				break;
			default:
				ChestShopFilter.category_filter.select_item(0);
		}
	}
	ChestShopFilter.on_filter_change();
};

ChestShopFilter.on_filter_change = function() {
	var n = ChestShopFilter.on_filter_change_functions.length;
	for (var i = 0; i < n; i++) {
		ChestShopFilter.on_filter_change_functions[i]();
	}
};

<? /* FILTER*/ ?>
ChestShopFilter.create_filter_widgets = function(parent_widget, x, y, chest)
{
	ChestShopFilter.filter_label_container = parent_widget.add_widget(
		PureFW.Image,
		x, y,
		50, 17,
		'ui/elements/texts/<?=$LNG?>/shop/txt_filter-50x17.png'
	);

	ChestShopFilter.drop_menus = new Array();
	ChestShopFilter.drop_menus.close_all = function(exclude) {
		for (var i = 0; i < this.length; i++) {
			if (this[i] != exclude)
				this[i].close_drop_menu();
		}
	}

	ChestShopFilter.category_filter = parent_widget.add_widget(
		PureFW.DropMenu,
		ChestShopFilter.filter_label_container.position.x + 70,
		ChestShopFilter.filter_label_container.position.y - 8,
		170, 32,
		6, 24);
	ChestShopFilter.category_filter.set_maximum_visible_items(14);

	ChestShopFilter.category_filter.add_item(
		"&nbsp;<b><?= LNG_FILTER_TYPE_ALL ?></b>",
		function() {
			ChestShopFilter.set_filter('similar_items', -1);
			ChestShopFilter.set_filter('type', 0);
		}
	);
	ChestShopFilter.category_filter.add_item(
		"&nbsp;<b><?= LNG_FILTER_TYPE_CLOTHES ?></b>"
	);
	ChestShopFilter.category_filter.add_item(
		"&nbsp;&nbsp;&nbsp;&nbsp;<?= LNG_FILTER_TYPE_HAIR_HAT ?>",
		function() {
			ChestShopFilter.set_filter('similar_items', -1);
			ChestShopFilter.set_filter('type', 5);
		}
	);
	ChestShopFilter.category_filter.add_item(
		"&nbsp;&nbsp;&nbsp;&nbsp;<?= LNG_FILTER_TYPE_OUTERWEAR ?>",
		function() {
			ChestShopFilter.set_filter('similar_items', -1);
			ChestShopFilter.set_filter('type', 1);
		}
	);
	ChestShopFilter.category_filter.add_item(
		"&nbsp;&nbsp;&nbsp;&nbsp;<?= LNG_FILTER_TYPE_TROUSERS ?>",
		function() {
			ChestShopFilter.set_filter('similar_items', -1);
			ChestShopFilter.set_filter('type', 2);
		}
	);
	ChestShopFilter.category_filter.add_item(
		"&nbsp;&nbsp;<b><?= LNG_FILTER_TYPE_BUILDINGS ?></b>"
	);
	ChestShopFilter.category_filter.add_item(
		"&nbsp;&nbsp;&nbsp;&nbsp;<?= LNG_FILTER_TYPE_STONES_TREES ?>",
		function() {
			ChestShopFilter.set_filter('similar_items', -1);
			ChestShopFilter.set_filter('type', 10);
		}
	);
	ChestShopFilter.category_filter.set_text_align('left');

	ChestShopFilter.category_filter.add_event_handler("click",
		function(ev) {
			ChestShopFilter.drop_menus.close_all(
				ChestShopFilter.category_filter
			)
		}
	);

	ChestShopFilter.drop_menus.push(ChestShopFilter.category_filter);




	ChestShopFilter.color_filter = parent_widget.add_widget(
		PureFW.DropMenu,
		ChestShopFilter.category_filter.position.x + 188,
		ChestShopFilter.category_filter.position.y,
		170, 32,
		6, 24);
	ChestShopFilter.color_filter.set_maximum_visible_items(14);

	ChestShopFilter.color_filter_add_helper = function(pic_main, name,
		color_filter)
	{
		var tmp = ChestShopFilter.color_filter.add_widget_as_item(
			PureFW.Container, [],
			function () {
				ChestShopFilter.set_filter('similar_items', -1);
				ChestShopFilter.set_filter('color', color_filter);
			}
		);
		tmp.add_widget(
			PureFW.Image,
			5, (tmp.height - 20) >> 1,
			20, 20,
			'ui/icons/labels/colors/20/'+pic_main+'.png'
		);
		var tmp2 = tmp.add_widget(
			PureFW.Container,
			30, (tmp.height - 20) >> 1,
			tmp.width - 35, 20
		);
		tmp2.set_content(name);
	}
	ChestShopFilter.color_filter_add_helper('all',
		'<?= LNG_FILTER_COLOR_ALL ?>', 0);
	ChestShopFilter.color_filter_add_helper('natural',
		'<?= LNG_FILTER_COLOR_NATURE ?>', 1);
	ChestShopFilter.color_filter_add_helper('red',
		'<?= LNG_FILTER_COLOR_RED ?>', 2);
	ChestShopFilter.color_filter_add_helper('blue',
		'<?= LNG_FILTER_COLOR_BLUE ?>', 3);
	ChestShopFilter.color_filter_add_helper('yellow',
		'<?= LNG_FILTER_COLOR_YELLOW ?>', 4);
	ChestShopFilter.color_filter_add_helper('green',
		'<?= LNG_FILTER_COLOR_GREEN ?>', 5);
	ChestShopFilter.color_filter_add_helper('white',
		'<?= LNG_FILTER_COLOR_WHITE ?>', 6);
	ChestShopFilter.color_filter_add_helper('black',
		'<?= LNG_FILTER_COLOR_BLACK ?>', 7);
	ChestShopFilter.color_filter_add_helper('purple',
		'<?= LNG_FILTER_COLOR_PURPLE ?>', 8);
	ChestShopFilter.color_filter_add_helper('orange',
		'<?= LNG_FILTER_COLOR_ORANGE ?>', 9);
	ChestShopFilter.color_filter_add_helper('pink',
		'<?= LNG_FILTER_COLOR_PINK ?>', 10);
	ChestShopFilter.color_filter.select_item(0);
	ChestShopFilter.color_filter.set_text_align('left');

	ChestShopFilter.color_filter.add_event_handler("click",
		function(ev) {
			ChestShopFilter.drop_menus.close_all(
				ChestShopFilter.color_filter
			)
		}
	);

	ChestShopFilter.drop_menus.push(ChestShopFilter.color_filter);

	if (chest) {
		ChestShopFilter.group_filter_cb = parent_widget.add_widget(
			PureFW.Checkbox,
			ChestShopFilter.color_filter.position.x + 188,
			ChestShopFilter.color_filter.position.y,
			36, 31,
			'ui/elements/form/input/check_toggle.png'
		);
		ChestShopFilter.group_filter_cb.check();
		ChestShopFilter.group_filter_cb.add_event_handler("check",
			function (ev) {
				ChestShopFilter.set_filter('similar_items', -1)
			}
		);
		ChestShopFilter.group_filter_cb.add_event_handler("uncheck",
			function (ev) {
				ChestShopFilter.set_filter('similar_items', -2)
			}
		);

		ChestShopFilter.group_filter_name = parent_widget.add_widget(
			PureFW.Image,
			ChestShopFilter.group_filter_cb.position.x
				+ ChestShopFilter.group_filter_cb.width + 3,
			ChestShopFilter.group_filter_cb.position.y + 10,
			155, 20,
			'ui/elements/texts/<?=$LNG?>/treasure/txt_group_treasures.png'
		);
		ChestShopFilter.group_filter_name.add_event_handler("click",
			function(ev) {
				ChestShopFilter.group_filter_cb.toggle();
			}
		);

	}
	else {
		ChestShopFilter.player_filter = parent_widget.add_widget(
			PureFW.DropMenu,
			ChestShopFilter.color_filter.position.x + 188,
			ChestShopFilter.color_filter.position.y,
			210, 32,
			6, 24
		);

		ChestShopFilter.player_filter.add_item(
			'<?= LNG_FILTER_PLAYER_ALL ?>'
		);

		ChestShopFilter.player_filter.add_event_handler("click",
			function(ev) {
				ChestShopFilter.drop_menus.close_all(
					ChestShopFilter.player_filter
				)
			}
		);

		ChestShopFilter.drop_menus.push(ChestShopFilter.player_filter);
	}
};
