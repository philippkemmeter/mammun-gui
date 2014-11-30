<script type="text/javascript">
<? /**
    * Damit Formulartexte funktionieren und dass die Steuerung der Karte nicht
    * beeinträchtigt wird, wird hier das Propagieren verhindert.
    */ ?>
TreasureChest.form_keyupdown = function(ev) {
	try {
		ev = PureFW.EventUtil.formatEvent(ev);
	}
	catch(e){
	}
	// Tastendruck nicht weitergeben an z.B. die Karte, damit das Tippen
	// keine Auswirkungen auf die Steuerung hat
	ev.stopPropagation();
}

<? /**
    * Diese Funktion zeigt die Details des angegebenen Items an. Im Dialog kann
    * dann entschieden werden, ob dieses Item gekauft werden soll.
    */ ?>
TreasureChest.show_details2 = function(item, chest_item)
{
	<? /* Wenn wider erwarten der details-container noch existiert, wird er
	    * hiermit zerstört. Wahrscheinlicher ist jedoch, dass details_container
	    * nicht "null" ist, aber das widget bereits zerstört wurde, was zu einem
	    * Fehler führte, weswegen hier ein silent-try-catch eingebaut ist.*/ ?>
	try {
		TreasureChest.item_details.details_container.destroy();
	}
	catch(e){}
	TreasureChest.item_details = new Object();

	TreasureChest.item_details.details_container = new PureFW.ConfirmationBox(
		document.body,
		((MammunUI.reference_width - 730)>>1) + 10,
		((MammunUI.reference_height - 460)>>1) - 10,
		730,460,
		null, true
	);
	TreasureChest.item_details.details_container.set_bg_img(
		'ui/backgrounds/confirm/info/info_730x460.png'
	);
	var destructor = function(ev) {
		TreasureChest.item_details.details_container
			.remove_event_handler("destroy",destructor);
		TreasureChest.item_details.details_container
			.remove_event_handler("change",destructor);
		destroy_object(TreasureChest.item_details);
	}

	<? /** Einheitenvorschau-"Widget" einfügen */ ?>
	TreasureChest.create_unit_preview(TreasureChest.item_details,
		TreasureChest.item_details.details_container, item, chest_item);

	TreasureChest.item_details.details_topic = new PureFW.Container(
		TreasureChest.item_details.details_container,
		300, 40,
		260, 60
	);
	TreasureChest.item_details.details_topic.set_font_size(1.4);
	TreasureChest.item_details.details_topic.set_content(
		item.name
	);
	TreasureChest.item_details.details_text = new PureFW.ScrollContainer(
		TreasureChest.item_details.details_container,
		TreasureChest.item_details.details_topic.position.x,
		TreasureChest.item_details.details_topic.position.y
			+ TreasureChest.item_details.details_topic.height,
		260, 0
	);
	TreasureChest.item_details.details_text.set_content(
		item.desc
	);
	TreasureChest.item_details.details_text.update_inner_height();
	TreasureChest.item_details.details_text.set_height(160);

	TreasureChest.item_details.quality_image = new PureFW.Image(
		TreasureChest.item_details.details_container,
		580, 40,
		110, 114,
		'ui/elements/treasure_chest/quality_'+item.shop_id+'.png');


	TreasureChest.item_details.details_options = new PureFW.Container(
		TreasureChest.item_details.details_container,
		289, 283,
		400, 153
	);
	TreasureChest.item_details.details_options.set_bg_img(
		'ui/backgrounds/treasure/details_options_bg-400x153.png');

	<? /**
		* Label zum Beschriften der details_options_box (überlappt)
		*/ ?>
	TreasureChest.item_details.details_options_label = new PureFW.Image(
		TreasureChest.item_details.details_container,
		TreasureChest.item_details.details_options.position.x - 9,
		TreasureChest.item_details.details_options.position.y - 23,
		152, 44,
		'ui/elements/texts/<?=$LNG?>/treasure/i_wonna_plate.png'
	);

	<? /**
		* Inhalt der Box
		*/ ?>

	<? /* Help */ ?>
	TreasureChest.item_details.options_help = new PureFW.Image(
		TreasureChest.item_details.details_options,
		364, 11,
		26, 26,
		'ui/icons/labels/treasure_chest/help.png');

	<? /* Buy-Direct-Radio mit Text */ ?>
	TreasureChest.item_details.items_direct_buy_radio = new PureFW.RadioButton(
		TreasureChest.item_details.details_options,
		16, 50,
		16, 16,
		'ui/elements/unit_training/special_radio_button.png');
	TreasureChest.item_details.items_direct_buy_text = new PureFW.Image(
		TreasureChest.item_details.details_options,
		44, TreasureChest.item_details.items_direct_buy_radio.position.y,
		225, 14,
		'ui/elements/texts/<?=$LNG?>/treasure/txt_direct_sell_for.png');
	TreasureChest.item_details.items_direct_buy_text.add_event_handler('click',
		function(ev) {
			TreasureChest.item_details.items_direct_buy_radio.on_click()
		});

	<? /* Klunkerpreisschild */?>
	TreasureChest.item_details.item_direct_buy_price_label = new PureFW.Container(
		TreasureChest.item_details.details_options,
		TreasureChest.item_details.items_direct_buy_text.position.x
			+ TreasureChest.item_details.items_direct_buy_text.width + 15,
		TreasureChest.item_details.items_direct_buy_text.position.y - 15,
		86, 47
	);
	TreasureChest.item_details.item_direct_buy_price_label.set_bg_img(
		'ui/backgrounds/treasure/preisschild.png'
	);
	TreasureChest.item_details.item_direct_buy_price_label.add_event_handler(
		'click',
		function(ev) {
			TreasureChest.item_details.items_direct_buy_radio.on_click()
		});

	<? /* Make-Offer-Radio mit Text */ ?>
	TreasureChest.item_details.items_make_offer_radio = new PureFW.RadioButton(
		TreasureChest.item_details.details_options,
		TreasureChest.item_details.items_direct_buy_radio.position.x,
		TreasureChest.item_details.items_direct_buy_radio.position.y
			+TreasureChest.item_details.items_direct_buy_radio.height + 30,
		16, 16,
		'ui/elements/unit_training/special_radio_button.png');
	TreasureChest.item_details.items_make_offer_text = new PureFW.Image(
		TreasureChest.item_details.details_options,
		TreasureChest.item_details.items_direct_buy_text.position.x,
		TreasureChest.item_details.items_make_offer_radio.position.y,
		264, 14,
		'ui/elements/texts/<?=$LNG?>/treasure/txt_make_offer.png');
	TreasureChest.item_details.items_make_offer_text.add_event_handler('click',
		function(ev) {
			TreasureChest.item_details.items_make_offer_radio.on_click()
		});

	<? /** Link Radio-Buttons */ ?>
	TreasureChest.item_details.items_direct_buy_radio.link_with_radio_button(
		TreasureChest.item_details.items_make_offer_radio);
	TreasureChest.item_details.items_make_offer_radio.link_with_radio_button(
		TreasureChest.item_details.items_direct_buy_radio);

	<? /** Klunkereingabe mit KKR-Label davor */ ?>
	TreasureChest.item_details.enter_offer_kkr = new PureFW.Image(
		TreasureChest.item_details.details_options,
		162,
		80,
		44, 44,
		'ui/icons/labels/resources/44/klunker.png'
	);
	TreasureChest.item_details.enter_offer_kkr.add_event_handler("click",
		function(ev) {
			TreasureChest.item_details.items_make_offer_radio.on_click()
		});
	TreasureChest.item_details.enter_offer_input = new PureFW.Container(
		TreasureChest.item_details.details_options,
		TreasureChest.item_details.enter_offer_kkr.position.x
			+ TreasureChest.item_details.enter_offer_kkr.width,
			TreasureChest.item_details.enter_offer_kkr.position.y
			+ (TreasureChest.item_details.enter_offer_kkr.height-25>>1)+1,
		56, 25
	);
	TreasureChest.item_details.enter_offer_input.set_bg_img(
		'ui/elements/form/input/input_text_56x25.png'
	);
	TreasureChest.item_details.enter_offer_input.add_event_handler("click",
		function(ev) {
			TreasureChest.item_details.items_make_offer_radio.on_click()
		});
	TreasureChest.item_details.enter_offer_input.set_content(
		'<form name="MammunShop_chest_item_list_form" \
			action="javascript:TreasureChest.form_keyupdown(event);">\
		<input type="text" id="TreasureChestOfferPriceInput" style="width:100%; \
			height: 100%; font-size:1.5em;\
	 	background:url(../pix/pattern/spacer.gif) 0 0 no-repeat transparent;\
	 	border: none; position:absolute; top:0;left:0;" \
	 	onkeydown="javascript: TreasureChest.form_keyupdown(event)"\
	 	onkeyup="javascript: TreasureChest.form_keyupdown(event)"/></form>'
	);

	<? /** Wenn price>0, dann Direktkauf anbieten und Kaufpreis anzeigen */?>
	if (item.price > 0) {
		TreasureChest.item_details.klunker = new PureFW.Container(
			TreasureChest.item_details.item_direct_buy_price_label,
			38, 10,
			45, 30
		);
		TreasureChest.item_details.klunker.set_font_size(2);
		TreasureChest.item_details.klunker.set_font_color('#303030');
		TreasureChest.item_details.klunker.set_text_align('center');
		TreasureChest.item_details.klunker.set_content(item.price);
		<? /* Direkt-Kauf-Radio direkt auswählen */ ?>
		TreasureChest.item_details.items_direct_buy_radio.on_click();
	}
	<? /** Sonst: Direktkauf deaktivieren und Trade auswählen */?>
	else {
		TreasureChest.item_details.items_direct_buy_radio.deactivate();
		TreasureChest.item_details.items_direct_buy_text.deactivate();
		TreasureChest.item_details.item_direct_buy_price_label.deactivate();
		TreasureChest.item_details.items_make_offer_radio.on_click();
	}

<?/*
	if (item.price > 0) {
		TreasureChest.buy_directly_button = new PureFW.Image(
			TreasureChest.item_details.details_container,
			500, 380,
			100, 20,
			'ui/elements/treasure_chest/quality_'+item.shop_id+'.png'
		);
		TreasureChest.buy_directly_button.add_event_handler("click",
			(function(_i) {
				return function(ev) {
					TreasureChest.buy_directly(_i);
					TreasureChest.item_details.details_container.destroy();
				}
			})(item)
		);
		TreasureChest.details_klunker_amount = new PureFW.Container(
			TreasureChest.item_details.details_container,
			300, 380,
			100, 100
		);


		TreasureChest.details_klunker_amount.set_content(
			'<table border="0" cellpadding="0" cellspacing="0" width="100%">'+
			'<tr><td>Klunker:</td>'+
			'<td width="60">'+
			'<input id="TreasureChestOfferPriceInput" '+
			' type="text" value="1" style="width:100%; height:100%" '+
			' onkeyup="javascript: TreasureChest.form_keyupdown(event);" '+
			' onkeydown="javascript: TreasureChest.form_keyupdown(event);"/>'+
			'</td></tr></table>'
		);
	}
	else {
		TreasureChest.details_klunker_amount = new PureFW.Container(
			TreasureChest.item_details.details_container,
			300, 380,
			100, 100
		);


		TreasureChest.details_klunker_amount.set_content(
			'<table border="0" cellpadding="0" cellspacing="0" width="100%">'+
			'<tr><td>Klunker:</td>'+
			'<td width="60">'+
			'<input id="TreasureChestOfferPriceInput" '+
			' type="text" value="1" style="width:100%; height:100%" '+
			' onkeyup="javascript: TreasureChest.form_keyupdown(event);" '+
			' onkeydown="javascript: TreasureChest.form_keyupdown(event);"/>'+
			'</td></tr></table>'
		);
	}

*/?>
	TreasureChest.item_details.details_container.add_event_handler('confirm',
		function(_i) {
			return function(ev) {
				if (TreasureChest.item_details.items_make_offer_radio.checked)
					TreasureChest.make_offer(_i);
				else
					TreasureChest.buy_directly(_i);
			}
		}(item));

	TreasureChest.item_details.details_container
		.add_event_handler("destroy",destructor);
	TreasureChest.item_details.details_container
		.add_event_handler("change",destructor);
	TreasureChest.item_details.race_icons[0].on_click();
}

TreasureChest.keyup = function(ev)
{
	try
	{
		ev = PureFW.EventUtil.formatEvent(ev);
	}
	catch(e){}
	ev.stopPropagation();
};
<?
/**
 * Diese Funktion öffnet eine Kontext-Box um ein Gebot abzugeben für das
 * entsprechende Item.
 *
 * @param Object item
 */ ?>
TreasureChest.make_offer_dialog = function(item) {
	TreasureChest.enter_offer_box = new PureFW.ConfirmationBox(
		document.body,
		(MammunUI.reference_width - 404)>>1,
		(MammunUI.reference_height - 325)>>1,
		404, 325, null, true
	);
	TreasureChest.enter_offer_box.set_bg_img(
		'ui/backgrounds/confirm/info/info_404x325.png'
	);
	TreasureChest.enter_offer_box.set_z_index(300);
	TreasureChest.enter_offer_text = new PureFW.Container(
		TreasureChest.enter_offer_box,
		30, 15,
		TreasureChest.enter_offer_box.width - 60,
		TreasureChest.enter_offer_box.height -100
	);
	TreasureChest.enter_offer_text.set_content(
		'<div style="font-size: 1.5em;"><?= LNG_MAKE_OFFER_CAPTION ?></div><br/>'
		+'<?= str_replace(array('[ITEM]', '[USER]'),
				array(ItemConstants::$ITEM_NAMES[$GP['type']][$GP['id']],
				User::get_instance_by_id($uid)->get_nick()),
				LNG_MAKE_OFFER_TEXT) ?>'
	);
	TreasureChest.enter_offer_kkr = new PureFW.Image(
		TreasureChest.enter_offer_box,
		30,
		TreasureChest.enter_offer_text.position.y
			+ TreasureChest.enter_offer_text.height,
		54, 51,
		'ui/icons/labels/resources/54/klunker.png'
	);
	TreasureChest.enter_offer_input = new PureFW.Container(
		TreasureChest.enter_offer_box,
		TreasureChest.enter_offer_kkr.position.x
			+ TreasureChest.enter_offer_kkr.width + 2,
		TreasureChest.enter_offer_kkr.position.y
			+ (TreasureChest.enter_offer_kkr.height-29>>1),
		70, 29
	);
	TreasureChest.enter_offer_input.set_bg_img(
		'ui/elements/form/input/input_text_70x29.png'
	);
	TreasureChest.enter_offer_input.set_content(
		'<form name="MammunShop_chest_item_list_form" \
			action="javascript:TreasureChest.keyup(event);">\
		<input type="text" name="kkr_amount" style="width:100%; height: 100%;\
	 	background:url(../pix/pattern/spacer.gif) 0 0 no-repeat transparent;\
	 	border: none; position:absolute; top:0;left:0;"\
	 	onkeydown="javascript: TreasureChest.keyup(event)"\
	 	onkeyup="javascript: TreasureChest.keyup(event)"/></form>'
	);
	TreasureChest.enter_offer_box.add_event_handler(
		"destroy",
		function() {
			try{TreasureChest.enter_offer_text.destroy(); }
			catch(e) {};
			try{TreasureChest.enter_offer_input.destroy();}
			catch(e){}
			try{TreasureChest.enter_offer_kkr.destroy();}
			catch(e){}
		}
	);

	TreasureChest.enter_offer_box.add_event_handler(
		"confirm",
		(function(_i) {
			return function (ev) {
				TreasureChest.make_offer(_i,
					document.forms.MammunShop_chest_item_list_form.kkr_amount.value
				);
			}
		})(item)
	);
}



<? /**
    * Diese Funktion setzt das Item auf den Marktplatz, also in das Schaufenster
    * des Spielers, woraus folgt, dass andere ihm Gebote machen werden. Sprich:
    * Wenn er das Ding auf den MP setzt, dann will er es verkaufen.
    */ ?>
TreasureChest.make_offer = function(item, price) {
	post_data = "id="+item.id+"&type="+item.type+"&make_offer=true"
		+"&price="+price
		+"&dataX=<? echo $cryptObj->do_encryption(array("uid" => $uid)) ?>";
	PureFW.AJAXClientServer.send_request("<?= $_SERVER['PHP_SELF'] ?>",
			TreasureChest.make_offer_response, post_data);
}

TreasureChest.make_offer_response = function(response_arr)
{
	if (response_arr[0] == "1")
	{
		MammunUI.show_warning(400, 300,
			response_arr[1],
			PureFW.ConfirmationBox.YES
		);
		ui_klunker_gainer.set_current_amount(parseInt(response_arr[2]));
		if (response_arr[3] && parseInt(response_arr[3]))
			ui_items_gainer.set_current_amount(parseInt(response_arr[3]));

		if (response_arr[4] && response_arr[5]) {
			var type = parseInt(response_arr[4]);
			var id = parseInt(response_arr[5]);
			var amount_mp = 0;
			for (var i in TreasureChest.all_items[type]) {
				if (TreasureChest.all_items[type][i].id == id) {
					amount_mp = --TreasureChest.all_items[type][i].amount_mp;
					break;
				}
			}
		}
		TreasureChest.show_items();
		ui_refresh();
		TreasureChest.item_details.details_container.destroy();
	}
	else if (response_arr[0] == "0")
	{
		MammunUI.show_warning(404, 325,
			response_arr[1],
			PureFW.ConfirmationBox.NO,
			true
		);
		TreasureChest.item_details.details_container.destroy();
	}
	else
	{
		MammunUI.show_warning(404, 325,
			'<?=LNG_STRANGE_ERROR_OFFER?>',
			PureFW.ConfirmationBox.NO,
			true
		);
		TreasureChest.item_details.details_container.destroy();
		throw Error(response_arr);
	}
}
TreasureChest.buy_directly = function(item) {
	TreasureChest.last_item = item;
	post_data = "id="+item.id+"&type="+item.type+"&buy_directly=true"
		+"&price="+item.price
		+"&dataX=<? echo $cryptObj->do_encryption(array("uid" => $uid)) ?>";
	PureFW.AJAXClientServer.send_request("<?= $_SERVER['PHP_SELF'] ?>",
			TreasureChest.make_offer_response, post_data);
}

</script>