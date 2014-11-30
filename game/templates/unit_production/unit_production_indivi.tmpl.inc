UnitIndividualScreen = new Object();
UnitProduction.items_selected = new Array();

<? /* Das Icon zum Abwählen. Wird später angelegt und immer wieder neu...*/?>
UnitIndividualScreen.reset_icon = null;
<?/*Aktuell angezeigten Icons von Gegenständen, die man im Shop kaufen kann*/?>
UnitIndividualScreen.items_shop_icons = new Array();
<?/*Aktuell angezeigten Icons von Gegenständen aus der Schatzkiste */?>
UnitIndividualScreen.items_treasure_chest_icons = new Array();
<?/*Aktuell angezeigten Overlay-Bilder für die ausgewählten Gegenstände*/?>
UnitIndividualScreen.overlay_pic = new Array();
<?/*Wo befinden sich die Schaltflächen, worüber man den Indivi-Typ aussucht*/?>
UnitIndividualScreen.hit_area_coords = new Array();
UnitIndividualScreen.hit_area_coords[<?=ItemConstants::TYPE_OUTERWEAR?>]
													= new PureFW.Point(145,140);
UnitIndividualScreen.hit_area_coords[<?=ItemConstants::TYPE_TROUSERS?>]
													= new PureFW.Point(135,200);
UnitIndividualScreen.hit_area_coords[<?=ItemConstants::TYPE_SHOES?>]
													= new PureFW.Point(0,0);
UnitIndividualScreen.hit_area_coords[<?=ItemConstants::TYPE_JEWELRY?>]
													= new PureFW.Point(0,0);
UnitIndividualScreen.hit_area_coords[<?=ItemConstants::TYPE_HAIR_HAT?>]
													= new PureFW.Point(150,55);
UnitIndividualScreen.hit_area_coords[<?=ItemConstants::TYPE_WEAPON?>]
													= new PureFW.Point(0,0);
<?/*Z-Indizes der Anziehsachen nach Typ*/ ?>
UnitIndividualScreen.type_z_index = new Array();
UnitIndividualScreen.type_z_index[<?=ItemConstants::TYPE_WEAPON?>] = 15;
UnitIndividualScreen.type_z_index[<?=ItemConstants::TYPE_HAIR_HAT?>] = 14;
UnitIndividualScreen.type_z_index[<?=ItemConstants::TYPE_JEWELRY?>] = 13;
UnitIndividualScreen.type_z_index[<?=ItemConstants::TYPE_OUTERWEAR?>] = 12;
UnitIndividualScreen.type_z_index[<?=ItemConstants::TYPE_TROUSERS?>] = 11;
UnitIndividualScreen.type_z_index[<?=ItemConstants::TYPE_SHOES?>] = 10;

UnitIndividualScreen.choose_item_type = function(type) {
	UnitIndividualScreen.hit_areas[type].check();
	var n = UnitIndividualScreen.items_shop_icons.length;
	for (var i = 0; i < n; i++) {
		UnitIndividualScreen.items_shop_icons[i].destroy();
	}

	if (UnitProduction.items[type]) {
		<? /**
		    * Drei Items zufällig auswählen, die er nicht sowieso schon besitzt
		    */ ?>
		UnitProduction.items[type].shuffle();	<? /*mischen*/?>
		chosen_indizes = new Array();
		var n = UnitProduction.items[type].length;
		var m = (UnitProduction.my_items[type])
			? UnitProduction.my_items[type].length
			: 0;
		var num_selected = 0;
		var num_items_skipped = 0;
		var i = 0;
		<?/* die ersten drei (wenn möglich) finden, die er nicht besitzt*/?>
		while ((num_selected < 3) && (i < n)) {
			var index_valid = true;
			<? /** Index darf auf kein Item zeigen, das der User besitzt*/?>
			for (var j = 0; j < m; j++) {
				if (UnitProduction.my_items[type][j].id ==
					UnitProduction.items[type][i].id)
				{
					index_valid = false;
					break;
				}
			}
			if (index_valid) {
				chosen_indizes.push(i);
				num_selected++;
			}
			i++;
		}
		for (i = 0; i < num_selected; i++) {
			var index = chosen_indizes[i];
			UnitIndividualScreen.items_shop_icons[i] = new PureFW.Icon(
				UnitIndividualScreen.item_chooser_shop_list.get_content_node(),
				0, 8, 71, 69,
				'ui/icons/labels/items/65/'+type+'/'
					+UnitProduction.items[type][index].id+'.png'
			);
			UnitIndividualScreen.items_shop_icons[i].set_tooltip(
				UnitProduction.items[type][index].name
			);
			UnitIndividualScreen.items_shop_icons[i].set_color('darkblue');
			UnitIndividualScreen.items_shop_icons[i].set_positioning('relative');
			UnitIndividualScreen.items_shop_icons[i].set_float('left');
			document.getElemById(UnitIndividualScreen.items_shop_icons[i]
				.img_id).style.left = '12.5%';
			document.getElemById(UnitIndividualScreen.items_shop_icons[i]
				.img_id).style.top = '12.5%';
			document.getElemById(UnitIndividualScreen.items_shop_icons[i]
				.img_id).style.width = '75%';
			document.getElemById(UnitIndividualScreen.items_shop_icons[i]
				.img_id).style.height = '75%';

			UnitIndividualScreen.items_shop_icons[i].add_event_handler(
				"click",
				(function(n1,n2){
					return function(ev) {
						UnitIndividualScreen.select_item_shop(n1,n2);
					}
				})(UnitProduction.items[type][index], i)
			);
		}
	}
	UnitIndividualScreen.item_chooser_shop_list.set_inner_width();

	var n = UnitIndividualScreen.items_treasure_chest_icons.length;
	for (var i = 0; i < n; i++) {
		UnitIndividualScreen.items_treasure_chest_icons[i].destroy();
	}

	if (UnitIndividualScreen.reset_icon)
		UnitIndividualScreen.reset_icon.destroy();
	UnitIndividualScreen.reset_icon = new PureFW.Icon(
		UnitIndividualScreen.item_chooser_treasure_chest_list.get_content_node(),
		0, 8, 71, 69, 'pattern/spacer.gif'
	);
	UnitIndividualScreen.reset_icon.set_color('darkblue');
	UnitIndividualScreen.reset_icon.set_positioning('relative');
	UnitIndividualScreen.reset_icon.set_float('left');
	switch (type) {
		case 1:
			UnitIndividualScreen.reset_icon.set_tooltip(
				'<?= LNG_OUTERWEAR_NONE ?>'
			);
			break;
		case 2:
			UnitIndividualScreen.reset_icon.set_tooltip(
				'<?= LNG_TROUSERS_NONE ?>'
			);
			break;
		case 3:
			UnitIndividualScreen.reset_icon.set_tooltip(
				'<?= LNG_SHOES_NONE ?>'
			);
			break;
		case 4:
			UnitIndividualScreen.reset_icon.set_tooltip(
				'<?= LNG_JEWELRY_NONE ?>'
			);
			break;
		case 5:
			UnitIndividualScreen.reset_icon.set_tooltip(
				'<?= LNG_HAIR_HAT_NONE ?>'
			);
			break;
		case 6:
			UnitIndividualScreen.reset_icon.set_tooltip(
				'<?= LNG_WEAPON_NONE ?>'
			);
			break;
			break;
	}
	document.getElemById(UnitIndividualScreen.reset_icon.img_id).style.left
		= '12.5%';
	document.getElemById(UnitIndividualScreen.reset_icon.img_id).style.top
		= '12.5%';
	document.getElemById(UnitIndividualScreen.reset_icon.img_id).style.width
		= '75%';
	document.getElemById(UnitIndividualScreen.reset_icon.img_id).style.height
		= '75%';
	UnitIndividualScreen.reset_icon.add_event_handler(
		"click",
		(function(_type) {
			return function(ev) {
				UnitIndividualScreen.reset_selection(_type);
			}
		})(type)
	);

	if (UnitProduction.my_items[type]) {
		n = UnitProduction.my_items[type].length;
		for (i = 0; i < n; i++) {
			UnitIndividualScreen.items_treasure_chest_icons[i] = new PureFW.Icon(
				UnitIndividualScreen.item_chooser_treasure_chest_list.get_content_node(),
				0, 8, 71, 69,
				'ui/icons/labels/items/65/'+type+'/'
					+UnitProduction.my_items[type][i].id+'.png'
			);
			UnitIndividualScreen.items_treasure_chest_icons[i].set_tooltip(
				UnitProduction.my_items[type][i].name
			);
			UnitIndividualScreen.items_treasure_chest_icons[i]
				.set_color('darkblue');
			UnitIndividualScreen.items_treasure_chest_icons[i]
				.set_positioning('relative');
			UnitIndividualScreen.items_treasure_chest_icons[i].set_float('left');
			document.getElemById(UnitIndividualScreen.items_treasure_chest_icons[i]
				.img_id).style.left = '12.5%';
			document.getElemById(UnitIndividualScreen.items_treasure_chest_icons[i]
				.img_id).style.top = '12.5%';
			document.getElemById(UnitIndividualScreen.items_treasure_chest_icons[i]
				.img_id).style.width = '75%';
			document.getElemById(UnitIndividualScreen.items_treasure_chest_icons[i]
				.img_id).style.height = '75%';


			UnitIndividualScreen.items_treasure_chest_icons[i].add_event_handler(
				"click",
				(function(n1, n2){
					return function(ev) {
						UnitIndividualScreen.select_item_treasure_chest(n1,n2);
					}
				})(UnitProduction.my_items[type][i], i)
			);
		}
		UnitIndividualScreen.item_chooser_treasure_chest_list.set_inner_width(
			n*71+87
		);
	}
	else {
		UnitIndividualScreen.item_chooser_treasure_chest_list.set_inner_width(87);
	}

	UnitIndividualScreen.item_chooser_treasure_chest_desc.set_content('');
};

<? /**
    * Wählt ein Item aus der Liste der Items der eigenen Schatzkiste aus
    */ ?>
UnitIndividualScreen.select_item_treasure_chest = function(item,
	icon_list_index)
{
	UnitIndividualScreen.reset_icon.set_width(71);
	UnitIndividualScreen.reset_icon.set_height(69);
	UnitIndividualScreen.reset_icon.set_y(8);
	var n = UnitIndividualScreen.items_shop_icons.length;
	for (var i = 0; i < n; i++) {
		UnitIndividualScreen.items_shop_icons[i].set_width(71);
		UnitIndividualScreen.items_shop_icons[i].set_height(69);
		UnitIndividualScreen.items_shop_icons[i].set_y(8);
	}
	n = UnitIndividualScreen.items_treasure_chest_icons.length;
	for (i = 0; i < n; i++) {
		UnitIndividualScreen.items_treasure_chest_icons[i].set_width(71);
		UnitIndividualScreen.items_treasure_chest_icons[i].set_height(69);
		UnitIndividualScreen.items_treasure_chest_icons[i].set_y(8);
	}
	UnitIndividualScreen.items_treasure_chest_icons[icon_list_index].set_width(87);
	UnitIndividualScreen.items_treasure_chest_icons[icon_list_index].set_height(85);
	UnitIndividualScreen.items_treasure_chest_icons[icon_list_index].set_y(0);

	if (!UnitIndividualScreen.overlay_pic[item.type]) {
		UnitIndividualScreen.overlay_pic[item.type] = new PureFW.Container(
			UnitIndividualScreen.picture_container,
			UnitIndividualScreen.unit_container.position.x,
			UnitIndividualScreen.unit_container.position.y,
			UnitIndividualScreen.unit_container.width,
			UnitIndividualScreen.unit_container.height
		);
		UnitIndividualScreen.overlay_pic[item.type].set_z_index(
			UnitIndividualScreen.type_z_index[item.type]
		);
	}
	UnitIndividualScreen.overlay_pic[item.type].set_bg_img(
		UnitProduction.unit_selected.overlay_pic_path + ''
			+item.type+'/'+item.id+'.png'
	);

	UnitIndividualScreen.item_chooser_treasure_chest_desc.set_content(
		'<b>'+item.name+'</b><br/><br/>'+
		'<?= LNG_AMOUNT ?>:' +(parseInt(item.amount) + parseInt(item.amount_mp))
	);
	UnitProduction.items_selected[item.type] = item;
	UnitProduction.items_selected[item.type].from_chest = true;

	var cur_kkr = 0;
	for (i in UnitProduction.items_selected) {
		if (typeof(UnitProduction.items_selected[i]) != 'function') {
			<? /* Item Kostenlos? Nichts addieren, sonst entsteht NaN */ ?>
			if (UnitProduction.items_selected[i].cost)
				cur_kkr += parseInt(UnitProduction.items_selected[i].cost);
		}
	}
	if (cur_kkr != 0)
		UnitSideInfo.kkr_label.set_text(-cur_kkr);
	else
		UnitSideInfo.kkr_label.set_text(cur_kkr);
};


<? /**
    * Wählt ein Item aus der Liste der Items des Shops aus
    */ ?>
UnitIndividualScreen.select_item_shop = function(item, icon_list_index) {
	UnitIndividualScreen.reset_icon.set_width(71);
	UnitIndividualScreen.reset_icon.set_height(69);
	UnitIndividualScreen.reset_icon.set_y(8);
	var n = UnitIndividualScreen.items_shop_icons.length;
	for (var i = 0; i < n; i++) {
		UnitIndividualScreen.items_shop_icons[i].set_width(71);
		UnitIndividualScreen.items_shop_icons[i].set_height(69);
		UnitIndividualScreen.items_shop_icons[i].set_y(8);
	}
	n = UnitIndividualScreen.items_treasure_chest_icons.length;
	for (i = 0; i < n; i++) {
		UnitIndividualScreen.items_treasure_chest_icons[i].set_width(71);
		UnitIndividualScreen.items_treasure_chest_icons[i].set_height(69);
		UnitIndividualScreen.items_treasure_chest_icons[i].set_y(8);
	}
	<? /**
	    * Wenn das nicht gesetzt ist, dann wurde es machinell ausgewählt, weil
	    * der User zurück und wieder vor benutzt hat. In dem Fall ist nicht
	    * klar, ob das Item aus der Schatzkiste oder nicht gekommen ist.
	    * Es ist nur insofern klar, dass das Schatzkisten-Item keine Klunker-
	    * kosten besitzt, sodass die Rekonstruktion völlig reibungslos passiert.
	    * In diesem Rekonstruktionsfall müssen die Items nicht vergrößert
	    * werden, als hätte der User das Icon gerade angeklickt.
	    */ ?>
	if (typeof(icon_list_index) !== 'undefined') {
		UnitIndividualScreen.items_shop_icons[icon_list_index].set_width(87);
		UnitIndividualScreen.items_shop_icons[icon_list_index].set_height(85);
		UnitIndividualScreen.items_shop_icons[icon_list_index].set_y(0);
	}

	if (!UnitIndividualScreen.overlay_pic[item.type]) {
		UnitIndividualScreen.overlay_pic[item.type] = new PureFW.Container(
			UnitIndividualScreen.picture_container,
			UnitIndividualScreen.unit_container.position.x,
			UnitIndividualScreen.unit_container.position.y,
			UnitIndividualScreen.unit_container.width,
			UnitIndividualScreen.unit_container.height
		);
		UnitIndividualScreen.overlay_pic[item.type].set_z_index(
			UnitIndividualScreen.type_z_index[item.type]
		);
	}
	UnitIndividualScreen.overlay_pic[item.type].set_bg_img(
		UnitProduction.unit_selected.overlay_pic_path + ''
			+item.type+'/'+item.id+'.png'
	);
	<? /**
	    * Im Rekonstruktionsfall soll die Beschreibung nicht erscheinen, als
	    * hätte der User das Item gerade angeklickt.
	    */ ?>
	if (typeof(icon_list_index) !== 'undefined') {
		UnitIndividualScreen.item_chooser_shop_desc_container.set_content(
			'<b>'+item.name+'</b><br/><br/>'+
			'Klunker:' +item.cost
		);
	}
	UnitProduction.items_selected[item.type] = item;

	var cur_kkr = 0;
	for (i in UnitProduction.items_selected) {
		if (typeof(UnitProduction.items_selected[i]) != 'function') {
			<? /* Item Kostenlos? Nichts addieren, sonst entsteht NaN */ ?>
			if (UnitProduction.items_selected[i].cost)
				cur_kkr += parseInt(UnitProduction.items_selected[i].cost);
		}
	}
	if (cur_kkr != 0)
		UnitSideInfo.kkr_label.set_text(-cur_kkr);
	else
		UnitSideInfo.kkr_label.set_text(cur_kkr);
};

<? /**
    * Wählt kein Item, sondern wählt das vorher gewählt einfach ab
    */ ?>
UnitIndividualScreen.reset_selection = function(type) {
	var n = UnitIndividualScreen.items_shop_icons.length;
	for (var i = 0; i < n; i++) {
		UnitIndividualScreen.items_shop_icons[i].set_width(71);
		UnitIndividualScreen.items_shop_icons[i].set_height(69);
		UnitIndividualScreen.items_shop_icons[i].set_y(8);
	}
	n = UnitIndividualScreen.items_treasure_chest_icons.length;
	for (i = 0; i < n; i++) {
		UnitIndividualScreen.items_treasure_chest_icons[i].set_width(71);
		UnitIndividualScreen.items_treasure_chest_icons[i].set_height(69);
		UnitIndividualScreen.items_treasure_chest_icons[i].set_y(8);
	}
	UnitIndividualScreen.reset_icon.set_width(87);
	UnitIndividualScreen.reset_icon.set_height(85);
	UnitIndividualScreen.reset_icon.set_y(0);

	if (UnitIndividualScreen.overlay_pic[type]) {
		UnitIndividualScreen.overlay_pic[type].destroy();
		UnitIndividualScreen.overlay_pic[type] = null;
	}

	UnitIndividualScreen.item_chooser_treasure_chest_desc.set_content('');

	delete UnitProduction.items_selected[type];
	var cur_kkr = 0;
	for (i in UnitProduction.items_selected) {
		if (typeof(UnitProduction.items_selected[i]) != 'function') {
			<? /* Item Kostenlos? Nichts addieren, sonst entsteht NaN */ ?>
			if (UnitProduction.items_selected[i].cost)
				cur_kkr += parseInt(UnitProduction.items_selected[i].cost);
		}
	}
	if (cur_kkr != 0)
		UnitSideInfo.kkr_label.set_text(-cur_kkr);
	else
		UnitSideInfo.kkr_label.set_text(cur_kkr);
};

<? /**
    * Diese Funktion vergrößert den Shop-Container und verkleinert den
    * Treasure-Chest-Container, damit man die Details der Shop-Items sehen kann.
    */ ?>
UnitIndividualScreen.make_shop_container_big = function() {
<? /**TODO	if (UnitIndividualScreen.show_kkr_warning) {

	} */ ?>
	UnitIndividualScreen.item_chooser_treasure_chest.set_width(279);
	UnitIndividualScreen.item_chooser_treasure_chest.set_height(91);
	UnitIndividualScreen.item_chooser_treasure_chest.set_x(
		UnitIndividualScreen.picture_container.position.x
		+UnitIndividualScreen.picture_container.width+8
	);
	UnitIndividualScreen.item_chooser_treasure_chest.set_bg_img(
		'ui/backgrounds/unitBgs/item_choose_small.png'
	);

	UnitIndividualScreen.item_chooser_shop.set_width(281);
	UnitIndividualScreen.item_chooser_shop.set_height(259);
	UnitIndividualScreen.item_chooser_shop.set_x(
		UnitIndividualScreen.picture_container.position.x
		+UnitIndividualScreen.picture_container.width+7
	);
	UnitIndividualScreen.item_chooser_shop.set_y(
		UnitIndividualScreen.picture_container.position.y
		+ UnitIndividualScreen.picture_container.height
		- 259
	);
	UnitIndividualScreen.item_chooser_shop.set_bg_img(
		'ui/backgrounds/unitBgs/item_choose_big.png'
	);
	UnitIndividualScreen.item_chooser_shop_icon.set_x(
		UnitIndividualScreen.item_chooser_shop.position.x - 10
	);
	UnitIndividualScreen.item_chooser_shop_icon.set_y(
		UnitIndividualScreen.item_chooser_shop.position.y - 10
	);

};

<? /**
    * Wie UnitIndividualScreen.make_shop_container_big nur umgekehrt.
    */ ?>
UnitIndividualScreen.make_treasure_chest_container_big = function() {
	UnitIndividualScreen.item_chooser_treasure_chest.set_width(281);
	UnitIndividualScreen.item_chooser_treasure_chest.set_height(259);
	UnitIndividualScreen.item_chooser_treasure_chest.set_x(
		UnitIndividualScreen.picture_container.position.x
		+UnitIndividualScreen.picture_container.width+7
	);
	UnitIndividualScreen.item_chooser_treasure_chest.set_bg_img(
		'ui/backgrounds/unitBgs/item_choose_big.png'
	);

	UnitIndividualScreen.item_chooser_shop.set_width(279);
	UnitIndividualScreen.item_chooser_shop.set_height(91);
	UnitIndividualScreen.item_chooser_shop.set_x(
		UnitIndividualScreen.picture_container.position.x
		+UnitIndividualScreen.picture_container.width+8
	);
	UnitIndividualScreen.item_chooser_shop.set_y(
		UnitIndividualScreen.picture_container.position.y
		+ UnitIndividualScreen.picture_container.height
		- 91
	);
	UnitIndividualScreen.item_chooser_shop.set_bg_img(
		'ui/backgrounds/unitBgs/item_choose_small.png'
	);
	UnitIndividualScreen.item_chooser_shop_icon.set_x(
		UnitIndividualScreen.item_chooser_shop.position.x - 10
	);
	UnitIndividualScreen.item_chooser_shop_icon.set_y(
		UnitIndividualScreen.item_chooser_shop.position.y - 10
	);
};

<? /**
    * Diese Funktion öffnet ein neues Popup-Fenster, welches alle Items des
    * angegebenen Typs enthält. Der Nutzer kann dann diese hierüber direkt dem
    * Mammun anziehen.
    */ ?>
UnitIndividualScreen.show_all_shop_items = function(type) {

};


<? /**
    * Erzeugt die Hit-Flächen der Units, mit denen man die Kleidungstypen
    * aussucht.
    */ ?>
UnitIndividualScreen.create_hit_areas = function() {
	UnitIndividualScreen.hit_areas = new Array();
	UnitIndividualScreen.hit_area_circles = new Array();
	UnitIndividualScreen.connection_lines = new Array();
	UnitIndividualScreen.category_names = new Array();
	var first_active = null;
	var last_active = null;
	for (var i = 1; i <= 6; i++) {
		if (UnitIndividualScreen.hit_area_coords[i].x == 0)
			continue;

		if(!first_active)
		{
			first_active = i;
		}

		UnitIndividualScreen.hit_areas[i] = new PureFW.RadioButton(
			UnitIndividualScreen.picture_container,
			UnitIndividualScreen.hit_area_coords[i].x,
			UnitIndividualScreen.hit_area_coords[i].y,
			16, 16, 'ui/elements/unit_training/special_radio_button.png'
		);
		UnitIndividualScreen.hit_area_circles[i] = new PureFW.Image(
			UnitIndividualScreen.picture_container,
			UnitIndividualScreen.hit_area_coords[i].x - 4,
			UnitIndividualScreen.hit_area_coords[i].y - 3,
			23, 23, 'ui/elements/unit_training/circle.png'
		);
		UnitIndividualScreen.hit_area_circles[i].set_z_index(90);
		UnitIndividualScreen.connection_lines[i] = new PureFW.Image(
			UnitIndividualScreen.picture_container,
			UnitIndividualScreen.hit_area_circles[i].position.x +
				UnitIndividualScreen.hit_area_circles[i].width - 2,
			UnitIndividualScreen.hit_area_circles[i].position.y +
				UnitIndividualScreen.hit_area_circles[i].height/2 - 2,
			UnitIndividualScreen.picture_container.width -
				(UnitIndividualScreen.hit_area_circles[i].position.x +
				UnitIndividualScreen.hit_area_circles[i].width),
			2,
			'ui/elements/unit_training/line.png');
		UnitIndividualScreen.connection_lines[i].set_z_index(85);

		UnitIndividualScreen.category_names[i] = new PureFW.Container(
			UnitIndividualScreen.picture_container,
			UnitIndividualScreen.connection_lines[i].position.x + 20,
			UnitIndividualScreen.connection_lines[i].position.y - 18,
			UnitIndividualScreen.connection_lines[i].width, 20);
		UnitIndividualScreen.category_names[i].set_z_index(85);
		UnitIndividualScreen.category_names[i].set_font_color("#333333");
		UnitIndividualScreen.category_names[i].set_font_weight("bold");
		UnitIndividualScreen.category_names[i].add_event_handler(
			"click",
			(function(n1){
				return function(ev) {
					UnitIndividualScreen.hit_areas[n1].on_click();
				};
			})(i)
		);
		switch (i)
		{
			case 1 :
				UnitIndividualScreen.category_names[i].set_content('<?=LNG_TYPE_OUTERWEAR?>');
				break;
			case 2 :
				UnitIndividualScreen.category_names[i].set_content('<?=LNG_TYPE_TROUSERS?>');
				break;
			case 3 :
				UnitIndividualScreen.category_names[i].set_content('<?=LNG_TYPE_SHOES?>');
				break;
			case 4 :
				UnitIndividualScreen.category_names[i].set_content('<?=LNG_TYPE_JEWELRY?>');
				break;
			case 5 :
				UnitIndividualScreen.category_names[i].set_content('<?=LNG_TYPE_HAIR_HAT?>');
				break;
			case 6 :
				UnitIndividualScreen.category_names[i].set_content('<?=LNG_TYPE_WEAPON?>');
				break;
			default :
				UnitIndividualScreen.category_names[i].set_content('?');
		}


		UnitIndividualScreen.hit_areas[i].set_z_index(100);
		UnitIndividualScreen.hit_areas[i].add_event_handler(
			"click",
			(function(n1){
				return function(ev) {
					UnitIndividualScreen.choose_item_type(n1);
				};
			})(i)
		);
		if(last_active)
		{
			UnitIndividualScreen.hit_areas[i].link_with_radio_button(
				UnitIndividualScreen.hit_areas[last_active]);
		}
		last_active = i;
	}
	var x = UnitIndividualScreen.hit_areas.length;
	UnitIndividualScreen.hit_areas[first_active].link_with_radio_button(
		UnitIndividualScreen.hit_areas[last_active]);
};

UnitIndividualScreen.apply_random_clothes = function()
{
	for (var item_type = 1; item_type <= 6; item_type++)
	{
		if (UnitIndividualScreen.hit_area_coords[item_type].x == 0)
			continue;

		var item_source = Math.round(Math.random());
		if(item_source == 1)
		{
			if(!(UnitProduction.my_items[item_type]))
			{
				continue;
			}
			var n = UnitProduction.my_items[item_type].length;
			var random_item = Math.round(Math.random()*n);
			if(random_item == n)
			{
				UnitIndividualScreen.reset_selection(item_type);
			}
			else
			{
				UnitIndividualScreen.select_item_treasure_chest(
					UnitProduction.my_items[item_type][random_item], random_item);
			}
		}
		else
		{
			if(UnitProduction.items[item_type])
			{
				var n = UnitProduction.items[item_type].length;
				var m = (UnitProduction.my_items[item_type])
					? UnitProduction.my_items[item_type].length
					: 0;

				var index_is_valid = true;
				var index = Math.floor(Math.random() * n);

				<? /** Index darf auf kein Item zeigen, das der User besitzt*/?>
				for (var i = 0; i < m; i++)
				{
					if (UnitProduction.my_items[item_type][i].id ==
						UnitProduction.items[item_type][index].id)
					{
						index = Math.abs(index - 1);
						UnitIndividualScreen.select_item_treasure_chest(
							UnitProduction.my_items[item_type][index], index);
						index_is_valid = false;
					}
				}
				if(index_is_valid)
				{
					try
					{
						UnitIndividualScreen.select_item_shop(
							UnitProduction.items[item_type][index]);
					}
					catch(e)
					{
						//UnitProduction.officer.set_text(e);
						var i = UnitIndividualScreen.items_shop_icons.length;
						UnitIndividualScreen.items_shop_icons[i] = new PureFW.Icon(
							UnitIndividualScreen.item_chooser_shop_list.get_content_node(),
							0, 8, 71, 69,
							'ui/icons/labels/items/65/'+item_type+'/'
								+UnitProduction.items[item_type][index].id+'.png'
						);
						UnitIndividualScreen.items_shop_icons[i].set_tooltip(
							UnitProduction.items[item_type][i].name
						);
						UnitIndividualScreen.items_shop_icons[i].set_color('darkblue');
						UnitIndividualScreen.items_shop_icons[i].set_positioning('relative');
						UnitIndividualScreen.items_shop_icons[i].set_float('left');
						document.getElemById(UnitIndividualScreen.items_shop_icons[i]
							.img_id).style.left = '12.5%';
						document.getElemById(UnitIndividualScreen.items_shop_icons[i]
							.img_id).style.top = '12.5%';
						document.getElemById(UnitIndividualScreen.items_shop_icons[i]
							.img_id).style.width = '75%';
						document.getElemById(UnitIndividualScreen.items_shop_icons[i]
							.img_id).style.height = '75%';

						UnitIndividualScreen.items_shop_icons[i].add_event_handler(
							"click",
							(function(n1,n2){
								return function(ev) {
									UnitIndividualScreen.select_item_shop(n1,n2);
								}
							})(UnitProduction.items[item_type][index], i)
						);
					}
				}
				UnitIndividualScreen.item_chooser_shop_list.set_inner_width();
			}
		}
	}
};

UnitIndividualScreen.random_clothes_button = new PureFW.Image(
	UnitProduction.this_window,
	15, 0,
	182, 62,
	'ui/elements/form/buttons/<?=$LNG?>/unit_training/dress_my_mammun-182x62.png');

UnitIndividualScreen.random_clothes_button.add_event_handler('click',
	UnitIndividualScreen.apply_random_clothes);


UnitIndividualScreen.categories_image = new PureFW.Image(
	UnitProduction.this_window,
	320, 30,
	134, 42,
	'ui/elements/texts/<?=$LNG?>/unit_training/label_categories-134x42.png');
UnitIndividualScreen.categories_image.set_z_index(5);

UnitIndividualScreen.shop_chest_connector_image = new PureFW.Image(
	UnitProduction.this_window,
	450, 95,
	16, 280,
	'ui/elements/unit_training/line-2.png');
UnitIndividualScreen.shop_chest_connector_image.set_z_index(5);

<? /**
    * Der picture_container enthält die Vorschau der Einheit, die man gerade
    * gewählt hat und nun anziehen kann.
    */ ?>
UnitIndividualScreen.picture_container = new PureFW.Container(
	UnitProduction.this_window,
	215, 52, 240, 366
);
UnitIndividualScreen.picture_container.set_bg_img(
	'ui/backgrounds/unitBgs/chosen_unit_indivi.png'
);
UnitIndividualScreen.unit_container = new PureFW.Container(
	UnitIndividualScreen.picture_container, 0, 0, 240, 366
);
UnitIndividualScreen.unit_container.set_z_index(5);

UnitIndividualScreen.unit_container.set_bg_img(
	UnitProduction.unit_selected.pic_big
);


<? /**
    * Hierüber werden die Gegenstände ausgewählt, die der User besitzt. Das ist
    * also ein Link zu seiner Schatzkiste. Alles, was er hier wählt, wird direkt
    * anprobiert und im Vorschaufenster angezeigt.
    */ ?>
UnitIndividualScreen.item_chooser_treasure_chest = new PureFW.Container(
	UnitProduction.this_window,
	UnitIndividualScreen.picture_container.position.x+
		UnitIndividualScreen.picture_container.width+7,
	UnitIndividualScreen.picture_container.position.y,
	281, 259
);
UnitIndividualScreen.item_chooser_treasure_chest.set_bg_img(
	'ui/backgrounds/unitBgs/item_choose_big.png'
);
UnitIndividualScreen.item_chooser_treasure_chest.add_event_handler("click",
	UnitIndividualScreen.make_treasure_chest_container_big
);
<? /* Overlay-Label-Icon */ ?>
UnitIndividualScreen.item_chooser_chest_icon = new PureFW.Image(
	UnitProduction.this_window,
	UnitIndividualScreen.item_chooser_treasure_chest.position.x-10,
	UnitIndividualScreen.item_chooser_treasure_chest.position.y-10,
	40,
	40,
	'ui/icons/labels/resources/40/chest.png'
);
UnitIndividualScreen.item_chooser_chest_icon.set_z_index(2);

<? /**
    * Unter-Container: Enthält die Liste aller Items in der Schatzkiste, welche
    * man jetzt hier verwenden darf.
    */ ?>
UnitIndividualScreen.item_chooser_treasure_chest_list
	= new PureFW.ScrollContainer
(
	UnitIndividualScreen.item_chooser_treasure_chest,
	25,5,
	UnitIndividualScreen.item_chooser_treasure_chest.width-50,
	85
);

<? /**
    * Unter-Container 2: Enthält die Daten des zuletzt gewählten Gegenstandes.
    */ ?>
UnitIndividualScreen.item_chooser_treasure_chest_desc = new PureFW.Container(
	UnitIndividualScreen.item_chooser_treasure_chest,
	UnitIndividualScreen.item_chooser_treasure_chest_list.position.x + 30,
	UnitIndividualScreen.item_chooser_treasure_chest_list.position.y
		+UnitIndividualScreen.item_chooser_treasure_chest_list.height
		+10,
	UnitIndividualScreen.item_chooser_treasure_chest_list.width - 60,
	UnitIndividualScreen.item_chooser_treasure_chest.height
		- UnitIndividualScreen.item_chooser_treasure_chest_list.height
		- 10
);
UnitIndividualScreen.item_chooser_treasure_chest_desc.set_font_size(1.2);



<? /**
    * Hierüber werden die Gegenstände ausgewählt, die der User nicht besitzt,
    * sondern im Shop gekauft werden können. Alles, was er hier wählt, wird
    * direkt anprobiert und im Vorschaufenster angezeigt, am Ende, wenn er
    * alles bestätigt hat, wird bezahlt.
    */ ?>
UnitIndividualScreen.item_chooser_shop = new PureFW.Container(
	UnitProduction.this_window,
	UnitIndividualScreen.picture_container.position.x
		+ UnitIndividualScreen.picture_container.width+8,
	UnitIndividualScreen.picture_container.position.y
		+ UnitIndividualScreen.picture_container.height
		- 91,
	281, 259
);
UnitIndividualScreen.item_chooser_shop.set_bg_img(
	'ui/backgrounds/unitBgs/item_choose_small.png'
);

UnitIndividualScreen.item_chooser_shop.add_event_handler("click",
	UnitIndividualScreen.make_shop_container_big
);

<? /* Overlay-Label-Icon */ ?>
UnitIndividualScreen.item_chooser_shop_icon = new PureFW.Image(
	UnitProduction.this_window,
	UnitIndividualScreen.item_chooser_shop.position.x-10,
	UnitIndividualScreen.item_chooser_shop.position.y-10,
	40,
	40,
	'ui/icons/labels/top_navi/40/shop.png'
);
UnitIndividualScreen.item_chooser_shop_icon.set_z_index(2);

<? /**
    * Unter-Container, welcher die wählbaren Elemente aus dem Shop enthält.
    */ ?>
UnitIndividualScreen.item_chooser_shop_list = new PureFW.ScrollContainer
(
	UnitIndividualScreen.item_chooser_shop,
	25,5,
	UnitIndividualScreen.item_chooser_shop.width-50,
	85
);
UnitIndividualScreen.item_chooser_shop_list.set_text_align = 'center';

<? /**
    * 2. Unter-Container, welcher die Beschreibung enthält. Anfangs ist er
    * nicht sichtbar, da item_chooser_shop zu klein ist, um diesen
    * desc_container anzuzeigen: er wird abgeschnitten.
    */ ?>
UnitIndividualScreen.item_chooser_shop_desc_container = new PureFW.Container(
	UnitIndividualScreen.item_chooser_shop,
	UnitIndividualScreen.item_chooser_shop_list.position.x + 30,
	UnitIndividualScreen.item_chooser_shop_list.position.y
		+UnitIndividualScreen.item_chooser_shop_list.height
		+10,
	UnitIndividualScreen.item_chooser_shop_list.width - 60,
	UnitIndividualScreen.item_chooser_shop.height
		- UnitIndividualScreen.item_chooser_shop_list.height
		- 60
);
UnitIndividualScreen.item_chooser_shop_desc_container.set_font_size(1.2);

UnitIndividualScreen.make_treasure_chest_container_big();

<? /**
	* TODO
    * Der Button, welcher zum Shop führt, d.h. genauer zur kompletten Liste
    * aller Items des Typs, die man im Shop kaufen kann.
    *

UnitIndividualScreen.item_chooser_shop_btn_more = new PureFW.Image(
	UnitIndividualScreen.item_chooser_shop,
	UnitIndividualScreen.item_chooser_shop.width - 182,
	UnitIndividualScreen.item_chooser_shop_desc_container.position.y
		+UnitIndividualScreen.item_chooser_shop_desc_container.height
		+2,
	152, 28,
	'ui/elements/form/buttons/more_in_shop.png'
);

 */?>

<? /**
    * Der Zurück-Button
    */ ?>
<? if (!$change_clothes) : /* Nur anzeigen, wenn "normal".
								Nicht beim Kleiderwechsel */?>
UnitIndividualScreen.back_button_container = new PureFW.Container(
	UnitSideInfo.back_next_button_container,
	0, 0, 93, 30
);
UnitIndividualScreen.back_button_container.set_bg_img(
	'ui/elements/form/buttons/<?=$LNG?>/wizard/back.png'
);
UnitIndividualScreen.back_button_container.add_event_handler(
	"click",
	function (ev) {
		UnitProduction.choose_race_dialog()
	}
);
<? endif; ?>

<? /**
    * Der Weiter-Button
    */ ?>
UnitIndividualScreen.next_button_container = new PureFW.Container(
	UnitSideInfo.back_next_button_container,
	96, 0, 93, 30
);
UnitIndividualScreen.next_button_container.set_bg_img(
	'ui/elements/form/buttons/<?=$LNG?>/wizard/finish.png'
);
UnitIndividualScreen.next_button_container.add_event_handler(
	"click",
	function (ev) {
		UnitProduction.submit_all_choices()
	}
);
UnitIndividualScreen.create_hit_areas();
UnitIndividualScreen.choose_item_type(1);

UnitIndividualScreen.destroy = function() {
	destroy_object(UnitIndividualScreen);
	UnitIndividualScreen = null;
	UnitProduction.officer.balloon_container.set_css_class('OfficerBalloon');
};