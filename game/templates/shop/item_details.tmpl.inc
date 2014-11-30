if (!((typeof(MammunShop) != 'undefined') && (typeof(MammunShop) == 'object')))
	MammunShop = new Object();

<? /**
    * Diese Funktion zeigt die Details des angegebenen Items an. Im Dialog kann
    * dann entschieden werden, ob dieses Item gekauft werden soll.
    */ ?>
MammunShop.show_details = function(item, chest_item) {
	<? /* Wenn wider erwarten der details-container noch existiert, wird er
	    * hiermit zerstört. Wahrscheinlicher ist jedoch, dass details_container
	    * nicht "null" ist, aber das widget bereits zerstört wurde, was zu einem
	    * Fehler führte, weswegen hier ein silent-try-catch eingebaut ist.*/ ?>
	try {
		MammunShop.details_container.destroy();
		destroy_object(MammunShop.item_details);
	}
	catch(e){}
	MammunShop.item_details = new Object();
	MammunShop.details_container = new PureFW.ConfirmationBox(
		document.body,
		(MammunUI.reference_width-701)>>1,
		(MammunUI.reference_height-536)>>1,
		701,536,
		PureFW.ConfirmationBox.NO,
		true
	);
	MammunShop.details_container.set_bg_img(
		'ui/backgrounds/confirm/info/info_701x536.png'
	);
	MammunShop.details_container.add_event_handler("destroy",
		function(){
			try
			{
				destroy_object(MammunShop.item_details);
			}
			catch(e){}
		}
	);
	MammunShop.details_container.show();

	// Anchor Container !!!
	MammunShop.item_details.details_chooser =
		MammunShop.details_container.add_widget
	(
		PureFW.Container,
		MammunShop.details_container.width - 238 - 65, 95,
		236, 88
	);
	MammunShop.item_details.details_chooser.set_bg_img(
		'ui/backgrounds/shop/unit_choose_light.png'
	);

	MammunShop.item_details.try_on_caption =
		MammunShop.details_container.add_widget
	(
		PureFW.Image,
		MammunShop.item_details.details_chooser.position.x + 16,
		MammunShop.item_details.details_chooser.position.y - 35,
		203, 17,
		'ui/elements/texts/<?= $LNG ?>/treasure/txt_unit_chooser_caption.png'
	);

//	MammunShop.item_details.left_scroll_arrow =
//		MammunShop.details_container.add_widget(
//			PureFW.Image,
//			MammunShop.item_details.details_chooser.position.x - 50,
//			MammunShop.item_details.details_chooser.position.y + 23,
//			46, 44,
//			'ui/elements/bubblebar/flick_left_46x44.png');
//
//	MammunShop.item_details.right_scroll_arrow =
//		MammunShop.details_container.add_widget(
//			PureFW.Image,
//			MammunShop.item_details.details_chooser.position.x +
//				MammunShop.item_details.details_chooser.width + 4,
//			MammunShop.item_details.details_chooser.position.y + 23,
//			46, 44,
//			'ui/elements/bubblebar/flick_right_46x44.png');

	MammunShop.item_details.units = new Array();



	MammunShop.item_details.race_icons = new Array();
	var l = MammunShop.item_details.units.length;
	var icon_counter = 0;

	for (var unit_counter = 0; unit_counter < l; unit_counter++)
	{
		if( (MammunShop.item_details.units[unit_counter].type != "club") 	&&
			(MammunShop.item_details.units[unit_counter].type != "blade") 	&&
			(MammunShop.item_details.units[unit_counter].type != "bow") 	&&
			(MammunShop.item_details.units[unit_counter].type != "javelin"))
		{
			continue;
		}

		var temp_icon =
			MammunShop.item_details.details_chooser.add_widget
		(
			PureFW.Image,
			10, 7,
			55, 76,
			MammunShop.item_details.units[unit_counter].pic
		);
		temp_icon.set_tooltip(MammunShop.item_details.units[unit_counter].name);
		temp_icon.set_positioning('relative');
		temp_icon.set_float('left');

		temp_icon.add_event_handler(
			"click",
			(function(_unit, _icon, _item) {
				return function(ev) {
					MammunShop.select_race(_unit, _icon, _item);
				}
			})(unit_counter, icon_counter, item)
		);

		MammunShop.item_details.race_icons.push(temp_icon);
		icon_counter++;
	}
//	MammunShop.item_details.details_chooser.set_inner_width((icon_i-1)*67+89);

	MammunShop.item_details.details_pic =
		MammunShop.details_container.add_widget
	(
		PureFW.Container,
		MammunShop.item_details.details_chooser.position.x+3,
		MammunShop.item_details.details_chooser.position.y
			+ MammunShop.item_details.details_chooser.height + 5,
		224, 314
	);
	MammunShop.item_details.details_pic.set_bg_img(
			'ui/backgrounds/shop/unit_preview_light.png'
		);

	MammunShop.item_details.unit_preview_pic =
		MammunShop.item_details.details_pic.add_widget
	(
		PureFW.Image,
		(MammunShop.item_details.details_pic.width - 165) >> 1,
		0,
		165, 289
	);

	MammunShop.item_details.unit_preview_pic.set_pic_url('/pattern/spacer.gif');

	MammunShop.item_details.rareness_meter
		= MammunShop.details_container.add_widget
	(
		PureFW.Items.RarenessMeter,
		MammunShop.item_details.details_pic.position.x
			+ MammunShop.item_details.details_pic.width - 18,
		MammunShop.item_details.details_pic.position.y
			+ ((MammunShop.item_details.details_pic.height - 246) >> 1),
		18, 246
	);
	MammunShop.item_details.rareness_meter.set_value(
		item.rareness_factor
	);

	<?/* ANCHOR */?>
	MammunShop.item_details.details_topic =
		MammunShop.details_container.add_widget
	(
		PureFW.Container,
		30,	30,
		285, 60
	);

	MammunShop.item_details.details_topic.set_font_weight('bold');
	MammunShop.item_details.details_topic.set_font_size(1.4);
	MammunShop.item_details.details_topic.set_content(
		item.name
	);

//	MammunShop.item_details.related_items_container = new PureFW.Container(
//		MammunShop.details_container,
//		MammunShop.item_details.details_topic.position.x,
//		MammunShop.item_details.details_topic.position.y
//			+ MammunShop.item_details.details_topic.height,
//		285, 0);
//
//		MammunShop.item_details.related_items_container.set_content(
//			'<?=LNG_RELATED_ITEMS?><br/>' +
//			'<a href="javascript: alert(\'soon\');"> Item 1 </a><br/>' +
//			'<a href="javascript: alert(\'even sooner\');"> Item 2 </a>');




	MammunShop.item_details.details_text
		= MammunShop.details_container.add_widget
	(
		PureFW.ScrollContainer,
		MammunShop.item_details.details_topic.position.x,
		MammunShop.item_details.details_topic.position.y
			+ MammunShop.item_details.details_topic.height + 10,
		285, 0
	);
	if (item.available) {
		MammunShop.item_details.details_text.set_content(item.desc);
		MammunShop.item_details.details_text.update_inner_height();
		MammunShop.item_details.details_text.set_height(200);
	}
	else {
		var r2 = Math.round(Math.pow(10, item.rareness_factor));
		var r1 = 1;
		for (var z = 1; z < 8; z++) {
			if (r2 < 10*z) {
				r1 = Math.round(10*z / r2);
				r2 = 10*z;
				break;
			}
		}

		MammunShop.item_details.details_text.set_content(
			'<?= LNG_XP_WERE_NEEDED ?>'.replace('[AMT]', item.xp) +
			'<br/>'+
			((r1 == r2)
				? '<?= LNG_RARENESS_TEXT_1_1 ?>'
				: '<?= LNG_RARENESS_TEXT ?>'.replace('[X]', r1).replace('[N]',r2)
			)
		);
		MammunShop.item_details.details_text.update_inner_height();
		MammunShop.item_details.details_text.set_height(200);

		if (chest_item) {
		}
		else {
			MammunShop.item_details.chest_item_list
				= MammunShop.details_container.add_widget
			(
				PureFW.ContainerAJAX,
				MammunShop.item_details.details_text.position.x,
				MammunShop.item_details.details_text.position.y +
					MammunShop.item_details.details_text.height + 15,
				MammunShop.item_details.details_text.width,
				180,
				'<?= $_SERVER['PHP_SELF'] ?>?show_chest_item_list=true&type='
					+item.type+'&id='+item.id
			);
		}
	}



	MammunShop.item_details.race_icons[0].on_click();
};
