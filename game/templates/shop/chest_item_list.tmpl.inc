<script>
MammunShop.chest_item_list = new Object();
MammunShop.chest_item_list.this_window
	= PureFW.ContainerAJAX.instances[<?=$this_win_id?>];
MammunShop.chest_item_list.avatar_data = <?= json_encode($avatar_data) ?>;
MammunShop.chest_item_list.chest_items = <?= json_encode($chest_items) ?>;

MammunShop.chest_item_list.keyup = function(ev)
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
 * Funktion führt Direktkauf aus.
 *
 * @param uint i		Index von MammunShop.chest_item_list.chest_items
 */ ?>
MammunShop.chest_item_list.buy_directly = function(i) {
	PureFW.AJAXClientServer.send_request(
		'treasure_chest.php?buy_directly=true&dataX='
			+ MammunShop.chest_item_list.chest_items[i].dataX
			+ '&id=<?=$GP['id']?>&type=<?=$GP['type']?>'
			+ '&price='+MammunShop.chest_item_list.chest_items[i].price,
		MammunShop.chest_item_list.buy_directly_response
	);
}

<?
/**
 * Erste Antwortstufe: Hat der Kauf geklappt? Dann wir ein weitere Request
 * rausgeschickt, um die neue Liste zu bekommen.
 *
 * @param string[] r_arr	Server-Antworten als Array
 */ ?>
MammunShop.chest_item_list.buy_directly_response = function(r_arr) {
	MammunShop.buy_item_response(r_arr);
	if (r_arr[0] == "1") {
		PureFW.AJAXClientServer.send_request(
			'shop.php?get_chest_item_list=true&id=<?=$GP['id']?>'
				+ '&type=<?=$GP['type']?>',
			MammunShop.chest_item_list.buy_directly_response_response
		);
	}
}

<?
/**
 * Zweite Antwortstufe: Nachdem der DirektKauf geklappt hat und die neue Liste
 * angefordert wurde, wird nun die erhaltene Liste angezeigt - die Kistenliste
 * also Aktuallisiert.
 *
 * @param string[] r_arr	Server-Antworten als Array
 */ ?>
MammunShop.chest_item_list.buy_directly_response_response = function(r_arr) {
	if (r_arr[0] == "1") {
		MammunShop.chest_item_list.chest_items = eval('('+r_arr[1]+')');
		MammunShop.chest_item_list.create_items();
		if (MammunShop.chest_item_list.chest_items.length > 1)
			MammunShop.show_offers();
	}
}

<?
/**
 * Diese Funktion öffnet eine Kontext-Box um ein Gebot abzugeben für das
 * entsprechende Item.
 *
 * @param uint i		Index von MammunShop.chest_item_list.chest_items
 */ ?>
MammunShop.chest_item_list.make_offer_dialog = function(i) {
	MammunShop.chest_item_list.enter_offer_box = new PureFW.ConfirmationBox(
		document.body,
		(MammunUI.reference_width - 404)>>1,
		(MammunUI.reference_height - 325)>>1,
		404, 325, null, true
	);
	MammunShop.chest_item_list.enter_offer_box.set_bg_img(
		'ui/backgrounds/confirm/info/info_404x325.png'
	);
	MammunShop.chest_item_list.enter_offer_box.set_z_index(300);
	MammunShop.chest_item_list.enter_offer_text = new PureFW.Container(
		MammunShop.chest_item_list.enter_offer_box,
		30, 15,
		MammunShop.chest_item_list.enter_offer_box.width - 60,
		MammunShop.chest_item_list.enter_offer_box.height -100
	);
	MammunShop.chest_item_list.enter_offer_text.set_content(
		'<div style="font-size: 1.5em;"><?= LNG_MAKE_OFFER_CAPTION ?></div><br/>'
		+'<?= str_replace('[ITEM]',
				ItemConstants::$ITEM_NAMES[$GP['type']][$GP['id']],
				LNG_MAKE_OFFER_TEXT) ?>'.replace(/\[USER\]/g,
				MammunShop.chest_item_list.chest_items[i].nick)

	);
	MammunShop.chest_item_list.enter_offer_kkr = new PureFW.Image(
		MammunShop.chest_item_list.enter_offer_box,
		30,
		MammunShop.chest_item_list.enter_offer_text.position.y
			+ MammunShop.chest_item_list.enter_offer_text.height,
		54, 51,
		'ui/icons/labels/resources/54/klunker.png'
	);
	MammunShop.chest_item_list.enter_offer_input = new PureFW.Container(
		MammunShop.chest_item_list.enter_offer_box,
		MammunShop.chest_item_list.enter_offer_kkr.position.x
			+ MammunShop.chest_item_list.enter_offer_kkr.width + 2,
		MammunShop.chest_item_list.enter_offer_kkr.position.y
			+ (MammunShop.chest_item_list.enter_offer_kkr.height-29>>1),
		70, 29
	);
	MammunShop.chest_item_list.enter_offer_input.set_bg_img(
		'ui/elements/form/input/input_text_70x29.png'
	);
	MammunShop.chest_item_list.enter_offer_input.set_content(
		'<form name="MammunShop_chest_item_list_form" \
			action="javascript:MammunShop.chest_item_list.keyup(event);">\
		<input type="text" name="kkr_amount" style="width:100%; height: 100%;\
	 	background:url(../pix/pattern/spacer.gif) 0 0 no-repeat transparent;\
	 	border: none; position:absolute; top:0;left:0;"\
	 	onkeydown="javascript: MammunShop.chest_item_list.keyup(event)"\
	 	onkeyup="javascript: MammunShop.chest_item_list.keyup(event)"/></form>'
	);
	MammunShop.chest_item_list.enter_offer_box.add_event_handler(
		"destroy",
		function() {
			try{MammunShop.chest_item_list.enter_offer_text.destroy(); }
			catch(e) {};
			try{MammunShop.chest_item_list.enter_offer_input.destroy();}
			catch(e){}
			try{MammunShop.chest_item_list.enter_offer_kkr.destroy();}
			catch(e){}
		}
	);

	MammunShop.chest_item_list.enter_offer_box.add_event_handler(
		"confirm",
		(function(_i) {
			return function (ev) {
				MammunShop.chest_item_list.make_offer(_i,
					document.forms.MammunShop_chest_item_list_form.kkr_amount.value
				);
			}
		})(i)
	);
}

<?
/**
 * Mithilfe der Funktion wird das Angebot an den Server geschickt, um es
 * dem User nun auch tatsächlich zu machen.
 *
 * @param uint i		Index von MammunShop.chest_item_list.chest_items
 * @param uint price	Angebotspreis
 */?>
MammunShop.chest_item_list.make_offer = function(i, price) {
	PureFW.AJAXClientServer.send_request(
		'treasure_chest.php?make_offer=true&dataX='
			+ MammunShop.chest_item_list.chest_items[i].dataX
			+ '&id=<?=$GP['id']?>&type=<?=$GP['type']?>'
			+ '&price='+price,
		MammunShop.chest_item_list.make_offer_response
	);
}

<?
/**
 * Serveranworten werden verarbeitet nach Angebotsübersendung.
 */?>
MammunShop.chest_item_list.make_offer_response = function(r_arr) {
	if (r_arr[0] == "1") {
<?		// TODO: CHECK THIS OVERRIDE!?>
		r_arr[1] = '<?= str_replace('[ITEM]',
              				ItemConstants::$ITEM_NAMES[$GP['type']][$GP['id']],
            				LNG_MAKE_OFFER_OK) ?>'.
            replace(/\[USER\]/g, r_arr[4]);
	}
	MammunShop.buy_item_response(r_arr);
}


<?
/**
 * Zeigt die Angebote an, die für das Item verfübar sind, also alle genau
 * gleichen Items, und wer sie verkauft.
 */
?>
MammunShop.show_offers = function() {
	var n = MammunShop.chest_item_list.chest_items.length;
	try
	{
		MammunShop.offers_container.destroy();
	}
	catch(e) {}

	MammunShop.offers_container = MammunUI.create_conf_box(
		437, 419,
		PureFW.ConfirmationBox.NO
	);

<?	/*NOTE TO&BY ALEX: USE NEW CONVENTION FOR CREATING WITHING A FOR LOOP!*/ ?>
	var SPACING = 15;
	var MARGIN_TOP = 40;
	var MARGIN_LEFT = 30;

	for(var i = 0; i < n; i++)
	{
		var tempAvatar = MammunShop.offers_container.add_widget(
			PureFW.Avatar.Avatar,
			MARGIN_LEFT, i*48 + i*SPACING + MARGIN_TOP,
			48, 48,
			MammunShop.chest_item_list.avatar_data[
				MammunShop.chest_item_list.chest_items[i].nick
			].replace('SIZE',44),
			MammunShop.chest_item_list.chest_items[i].nick
		);
		var tempName = MammunShop.offers_container.add_widget(
			PureFW.Container,
			MARGIN_LEFT + tempAvatar.width + 10, tempAvatar.position.y,
			0, 0
		);
		tempName.set_content(
			'<a href="javascript: UIButtons.toggle_window(\''
				+ 'profile.php?nick='
					+ MammunShop.chest_item_list.chest_items[i].nick
				+ '\', \'\', 500);">'
					+ MammunUI.beautify_nick(
						MammunShop.chest_item_list.chest_items[i].nick
					)
				+ '</a>'
		);
		var tempPrice = MammunShop.offers_container.add_widget(
			PureFW.Container,
			MARGIN_LEFT + tempName.position.x + 100, tempAvatar.position.y,
			110, 49
		);
		var tempBuy = MammunShop.offers_container.add_widget(
			PureFW.Image,
			MARGIN_LEFT + tempPrice.position.x + 100, tempAvatar.position.y,
			44, 44
		);

		if (MammunShop.chest_item_list.chest_items[i].price > 0) {
			tempPrice.set_bg_img('ui/icons/labels/shop/klunker_price_110x49.png');
			tempPrice.set_content(
				MammunShop.chest_item_list.chest_items[i].price
			);
			tempPrice.set_font_size(1.2);
			tempPrice.set_font_weight('bold');
			tempPrice.set_text_align('center');
			tempPrice.set_font_color('#303030');
			tempPrice.get_content_node().style.margin = "14px";
			tempBuy.set_pic_url(
				'ui/icons/labels/shop/icn_buy.png');
			tempBuy.add_event_handler("click",
				function(_i) {
					return function(ev) {
						MammunShop.chest_item_list.buy_directly(_i);
					}
				}(i)
			);
		}
		else//make an offer
		{
			tempPrice.set_bg_img(
				'ui/icons/labels/shop/klunker_price_unknown_110x49.png');
			tempBuy.set_pic_url(
				'ui/icons/labels/shop/icn_bid.png');
			tempBuy.add_event_handler("click",
					function(_i) {
						return function(ev) {
							MammunShop.chest_item_list.make_offer_dialog(_i);
						}
					}(i)
				);
		}
	}
	tempAvatar = tempName = tempPrice = tempBuy = null; // clear the pointers

}


<?
/**
 * Erstellt die Liste der Items, die man kaufen oder auf die man bieten kann.
 */?>
MammunShop.chest_item_list.create_items = function() {
	MammunShop.chest_item_list.this_window.remove_all_widgets();

	var n = MammunShop.chest_item_list.chest_items.length;

	if (n == 0)
		return;

<?	/**
	 * Nur ein Angebot: Dieses wird direkt angezeigt und es kann direkt gekauft
	 * werden.
	 *
	 * Wenn mehr, dann wird eine Liste von Avataren angezeigt und die Details
	 * sind in einem anderen Screen dahinter zu erreichen
	 * (s. u. den Else-Fall)
	 **/ ?>
	if (n == 1) {
		MammunShop.chest_item_list.one_offer_container
			= MammunShop.chest_item_list.this_window.add_widget
		(
			PureFW.Container,
			0, 0,
			MammunShop.chest_item_list.this_window.width, 50
		);

		MammunShop.chest_item_list.one_offer_container.set_content(
			'<b><?=LNG_ITEM_SOLD_BY?><b><br />' +
			'<a href="javascript: UIButtons.toggle_window(\''
			+ 'profile.php?nick='
				+ MammunShop.chest_item_list.chest_items[0].nick
			+ '\', \'\', 110);">'
				+ MammunUI.beautify_nick(
					MammunShop.chest_item_list.chest_items[0].nick
				)
			+ '</a>'
		);

		MammunShop.chest_item_list.seller_avatar
			= MammunShop.chest_item_list.this_window.add_widget
		(
			PureFW.Avatar.Avatar,
			MammunShop.chest_item_list.one_offer_container.position.x,
			MammunShop.chest_item_list.one_offer_container.position.y + 50,
			48, 48,
			MammunShop.chest_item_list.avatar_data[
				MammunShop.chest_item_list.chest_items[0].nick
			].replace('SIZE',44),
			MammunShop.chest_item_list.chest_items[0].nick
		);
		<?
		/**
		 * Wenn ein Preis feststeht, dann diesen anzeigen und einen Kaufen-
		 * Button mit entsprechender Funktion einbinden.
		 */?>
		if (MammunShop.chest_item_list.chest_items[0].price > 0) {
			MammunShop.chest_item_list.price_container
				= MammunShop.chest_item_list.this_window.add_widget
			(
			 	PureFW.Container,
				MammunShop.chest_item_list.seller_avatar.position.x + 52,
				MammunShop.chest_item_list.seller_avatar.position.y,
				110, 49
			);
			MammunShop.chest_item_list.price_container.set_bg_img(
				'ui/icons/labels/shop/klunker_price_110x49.png'
			);
			MammunShop.chest_item_list.price_container_inner =
				MammunShop.chest_item_list.price_container.add_widget
			(
				PureFW.Container,
				50, 15,
				45, 20
			);
			MammunShop.chest_item_list.price_container_inner.set_font_weight(
				'bold'
			);
			MammunShop.chest_item_list.price_container_inner.set_text_align(
				'center'
			);
			MammunShop.chest_item_list.price_container_inner.set_content(
				MammunShop.chest_item_list.chest_items[0].price
			);

			MammunShop.chest_item_list.buy_button
				= MammunShop.chest_item_list.this_window.add_widget
			(
				PureFW.Image,
				MammunShop.chest_item_list.one_offer_container.position.x,
				MammunShop.chest_item_list.price_container.position.y
					+ MammunShop.chest_item_list.price_container.height,
				201, 77,
				'ui/elements/form/buttons/<?=$LNG?>/shop/btn_kaufen_201x77.png'
			);
			MammunShop.chest_item_list.buy_button.add_event_handler("click",
				function(ev) {
					MammunShop.chest_item_list.buy_directly(0);
				}
			);
		}
		<? /**
			* Sonst, wenn kein Preis feststeht, dann keinen anzeigen und einen
			* Biete-Button mit entsprechender Funktion einbinden
			*/ ?>
		else {

			MammunShop.chest_item_list.price_container
				= MammunShop.chest_item_list.this_window.add_widget
			(
			 	PureFW.Image,
				MammunShop.chest_item_list.seller_avatar.position.x + 52,
				MammunShop.chest_item_list.seller_avatar.position.y,
				110, 49,
				'ui/icons/labels/shop/klunker_price_unknown_110x49.png'
			);
			MammunShop.chest_item_list.buy_button
				= MammunShop.chest_item_list.this_window.add_widget
			(
				PureFW.Image,
				MammunShop.chest_item_list.one_offer_container.position.x,
				MammunShop.chest_item_list.price_container.position.y
					+ MammunShop.chest_item_list.price_container.height,
				200, 74,
				'ui/elements/form/buttons/<?=$LNG?>/shop/btn_bieten_200x74.png'
			);
			MammunShop.chest_item_list.buy_button.add_event_handler("click",
				function(ev) {
					MammunShop.chest_item_list.make_offer_dialog(0);
				}
			);
		}
	}
	<? /** Mehrere User verkaufen das Item */ ?>
	else {
		MammunShop.chest_item_list.multi_offer_container =
			MammunShop.chest_item_list.this_window.add_widget
		(
			PureFW.Container,
			0, 0,
			MammunShop.chest_item_list.this_window.width, 50
		);

		MammunShop.chest_item_list.multi_offer_container.set_content(
				'<b><?=LNG_ITEM_OFFERED_BY?><b>'
		);

		MammunShop.chest_item_list.avatar_container
			= MammunShop.chest_item_list.this_window.add_widget
		(
			PureFW.Container,
			MammunShop.chest_item_list.multi_offer_container.position.x,
			MammunShop.chest_item_list.multi_offer_container.position.y
				+ 50,
			48*5+4*5, 48
		);
		try {
			for (var i = 0; i < n; i++) {
				MammunShop.chest_item_list.avatar_container.add_widget(
					PureFW.Avatar.Avatar,
					i*48 + i*4, 0,
					48, 48,
					MammunShop.chest_item_list.avatar_data[
						MammunShop.chest_item_list.chest_items[i].nick
					].replace('SIZE',44),
					MammunShop.chest_item_list.chest_items[i].nick);
			}
		}
		catch(e) {}
		MammunShop.chest_item_list.view_offers_button
			= MammunShop.chest_item_list.this_window.add_widget
		(
			PureFW.Image,
			MammunShop.chest_item_list.avatar_container.position.x + 3,
			MammunShop.chest_item_list.avatar_container.position.y +
				MammunShop.chest_item_list.avatar_container.height + 10,
			250, 57,
			'ui/elements/form/buttons/<?=$LNG?>/shop/btn_show_offers-250x57.png'
		);
		MammunShop.chest_item_list.view_offers_button.add_event_handler("click",
			function(ev) {
				MammunShop.show_offers();
			}
		);
	}
}


<? /** Destruktor */ ?>
MammunShop.chest_item_list.destroy = function() {
	destroy_object(MammunShop.chest_item_list);
	MammunShop.chest_item_list = null;
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

MammunShop.chest_item_list.create_items();
</script>