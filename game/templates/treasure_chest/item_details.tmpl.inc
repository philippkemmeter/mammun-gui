if (!((typeof(TreasureChest) != 'undefined') && TreasureChest
				&& (typeof(TreasureChest) == 'object')))
	TreasureChest = new Object();

TreasureChest.show_item_detail_history = function(item) {
	var conf_box = MammunUI.create_conf_box(
		437, 419, PureFW.ConfirmationBox.NO
	);

	var topic = conf_box.add_widget(
		PureFW.Image,
		20, 30,
		299, 29,
		'ui/elements/texts/<?=$LNG?>/treasure/txt_item_details_history_topic.png'
	);

	var intro = conf_box.add_widget(
		PureFW.Container,
		topic.position.x, topic.position.y + topic.height + 25,
		conf_box.width - (topic.position.x << 1), 50
	);
	intro.set_content('<?= LNG_INTRO_HISTORY ?> ');

	var anchor = intro;
	if (item.days_in_shop > 0) {
		var clock = conf_box.add_widget(
			PureFW.Image,
			topic.position.x, intro.position.y + intro.height + 15,
			44, 44,
			'ui/icons/labels/resources/44/wecker.png'
		);

		var clock_amt = conf_box.add_widget(
			PureFW.Container,
			clock.position.x + clock.width + 10,
			clock.position.y + ((clock.height - 20) >> 1),
			200, 20
		);
		clock_amt.set_font_weight('bold');
		clock_amt.set_content(
			item.days_in_shop + ' <?= LNG_DAYS_IN_SHOP ?>'
		);
		anchor = clock;
	}

	var further_info = conf_box.add_widget(
		PureFW.Container,
		topic.position.x, intro.position.y + intro.height + 140,
		conf_box.width - (topic.position.x << 1), 25
	);
	further_info.set_content('<?= LNG_FURTHER_INFORMATION ?>');
	further_info.set_font_weight('bold');
	further_info.set_font_size(1.1);

	var rareness = conf_box.add_widget(
		PureFW.Container,
		further_info.position.x, further_info.position.y + further_info.height
			+ 20,
		conf_box.width - (topic.position.x << 1), 0
	);
	var r2 = Math.round(Math.pow(10, item.rareness_factor));
	var r1 = 1;

	for (var z = 10; z < 100000000; z*=10) {
		if (r2 < z) {
			r1 = Math.round(z / r2);
			r2 = z;
			break;
		}
	}

	rareness.set_content(
		((r1 == r2)
			? '<?= LNG_RARENESS_TEXT_1_1 ?>'
			: '<?= LNG_RARENESS_TEXT ?>'.replace('[X]', r1).replace('[N]',r2)
		) + ' <?= LNG_RARENESS_VALUE_EXPLAIN ?>'.replace('[VALUE]',
			item.rareness_factor)
	);
}

<? /**
    * Diese Funktion zeigt die Details des angegebenen Items an. Im Dialog kann
    * dann entschieden werden, ob dieses Item gekauft werden soll.
    */ ?>
TreasureChest.show_details = function(item, chest_item, researched_races) {
	try {
		TreasureChest.officer.hide();
	}
	catch(e){}

	<? /* Wenn wider erwarten der details-container noch existiert, wird er
	    * hiermit zerstört. Wahrscheinlicher ist jedoch, dass details_container
	    * nicht "null" ist, aber das widget bereits zerstört wurde, was zu einem
	    * Fehler führte, weswegen hier ein silent-try-catch eingebaut ist.*/ ?>
	try {
		destroy_object(TreasureChest.item_details);
		TreasureChest.item_details = null;
	}
	catch(e){}
	TreasureChest.item_details = new Object();

	TreasureChest.item_details.details_container = MammunUI.create_conf_box(
		810, 540,
		<? if ($uid == $current_user->get_id()) : ?>
		PureFW.ConfirmationBox.YES
		<? else : ?>
		PureFW.ConfirmationBox.NO
		<? endif; ?>
	);

	<? /** Überschrift und Beschreibungstext */ ?>

	TreasureChest.item_details.details_topic
		= TreasureChest.item_details.details_container.add_widget
	(
		PureFW.Container,
		30, 25,
		390, 60
	);
	TreasureChest.item_details.details_topic.set_content(
		item.name
	);
	TreasureChest.item_details.details_topic.set_font_weight('bold');
	TreasureChest.item_details.details_topic.set_font_size(1.4);

	TreasureChest.item_details.details_text
		= TreasureChest.item_details.details_container.add_widget
	(
		PureFW.ScrollContainer,
		TreasureChest.item_details.details_topic.position.x,
		TreasureChest.item_details.details_topic.position.y
			+ TreasureChest.item_details.details_topic.height,
		390, 0
	);
	TreasureChest.item_details.details_text.set_content(
		item.desc
	);
	TreasureChest.item_details.details_text.update_inner_height();
	TreasureChest.item_details.details_text.set_height(139);

	<? /* HISTORY-Link */ ?>
	TreasureChest.item_details.history_link_cont
		= TreasureChest.item_details.details_container.add_widget
	(
		PureFW.Container,
		TreasureChest.item_details.details_topic.position.x, 250,
		200, 20
	);
	TreasureChest.item_details.history_link_cont.set_content(
		'<a href="javascript: ;"><?= LNG_B_SHOW_ITEM_HISTORY ?></a>'
	);
	TreasureChest.item_details.history_link_cont.add_event_handler("click",
		(function(_item) {
			return function(ev) {
				TreasureChest.show_item_detail_history(_item);
			}
		})(item)
	);

<? if ($uid == $current_user->get_id()) : ?>
	<? /**
		* NUR IN MEINER SCHATZKISTE
		* Anzeige der Stückzahl
		*/ ?>
	TreasureChest.item_details.details_amt_img
		= TreasureChest.item_details.details_container.add_widget
	(
		PureFW.Image,
		TreasureChest.item_details.details_topic.position.x,
		300,
		58,17,
		'ui/elements/texts/<?=$LNG?>/treasure/txt_details_amount.png'
	);
	TreasureChest.item_details.details_amt_cont
		= TreasureChest.item_details.details_container.add_widget
	(
		PureFW.Container,
		TreasureChest.item_details.details_amt_img.position.x
			+ TreasureChest.item_details.details_amt_img.width + 10,
		TreasureChest.item_details.details_amt_img.position.y - 3,
		50,20
	);
	TreasureChest.item_details.details_amt_cont.set_font_weight('bold');
	TreasureChest.item_details.details_amt_cont.set_font_color('#777');
	TreasureChest.item_details.details_amt_cont.set_content(
		parseInt(item.amount)+parseInt(item.amount_mp)
	);


	<? /**
		* NUR IN MEINER SCHATZKISTE!
		* Detail-Box mit (fast) allen Optionen zu dem Item.
		*/ ?>
	TreasureChest.item_details.details_options_bg
		= TreasureChest.item_details.details_container.add_widget
	(
		PureFW.Container,
		TreasureChest.item_details.details_topic.position.x,
		350,
		400, 153
	);
	TreasureChest.item_details.details_options_bg.set_bg_img(
		'ui/backgrounds/treasure/details_options_bg_400x153.png'
	);

	<? /**
		* Label zum Beschriften der details_options_box (überlappt)
		*/ ?>
	TreasureChest.item_details.details_options_label
		= TreasureChest.item_details.details_container.add_widget
	(
		PureFW.Image,
		TreasureChest.item_details.details_options_bg.position.x - 9,
		TreasureChest.item_details.details_options_bg.position.y - 23,
		152, 44,
		'ui/elements/texts/<?=$LNG?>/treasure/my_items_plate.png'
	);

	<? /**
		* Inhalt der Optionsbox wird nun Stückweise erstellt.
		*/ ?>
	TreasureChest.item_details.details_options
		= TreasureChest.item_details.details_options_bg.add_widget
	(
		PureFW.Container,
		0, 0,
		TreasureChest.item_details.details_options_bg.width,
		TreasureChest.item_details.details_options_bg.height
	);
	TreasureChest.item_details.details_options.set_bg_img(
		'ui/backgrounds/treasure/details_options_bg_400x153_conf_overlay.png'
	);

	<? /* Help Button */ ?>
	<?/*
	TreasureChest.item_details.options_help = new PureFW.Image(
		TreasureChest.item_details.details_options,
		364, 11,
		26, 26,
		'ui/icons/labels/treasure_chest/help.png');
	/*TreasureChest.item_details.options_help.add_event_handler("click",
		function() { alert("Go help yourself!!!"); } );*/?>

	<? /* In-der-Schatzkiste-bleiben-Button mit Text */?>
	TreasureChest.item_details.items_stay_radio
		= TreasureChest.item_details.details_options.add_widget
	(
		PureFW.RadioButton,
		16, 24,
		16, 16,
		'ui/elements/unit_training/special_radio_button.png');
	TreasureChest.item_details.items_stay_radio.add_event_handler('click',
		function(_i)
		{
			return function(ev){
				_i.amount = parseInt(_i.amount) + parseInt(_i.amount_mp);
				_i.amount_mp = 0;
				TreasureChest.item_details.items_sell_check.deactivate();
				TreasureChest.item_details.price_input.deactivate();
				TreasureChest.item_details.item_quantity_input.deactivate();
				TreasureChest.item_details.price_text.deactivate();
				TreasureChest.item_details.buy_from_price_text.deactivate();
			}
		}(item, chest_item)
	);
	TreasureChest.item_details.items_stay_text
		= TreasureChest.item_details.details_options.add_widget
	(
		PureFW.Image,
		44, TreasureChest.item_details.items_stay_radio.position.y,
		135, 16,
		'ui/elements/texts/<?=$LNG?>/treasure/txt_item_no_trade.png'
	);
	TreasureChest.item_details.items_stay_text.add_event_handler('click',
			function(ev) {
				TreasureChest.item_details.items_stay_radio.on_click()
			});
	TreasureChest.item_details.items_stay_desc
		= TreasureChest.item_details.details_options.add_widget
	(
		PureFW.Container,
		200, TreasureChest.item_details.items_stay_radio.position.y + 2,
		200, 14
	);
	TreasureChest.item_details.items_stay_desc.set_font_color('#777');
	TreasureChest.item_details.items_stay_desc.set_font_size(0.7);
	TreasureChest.item_details.items_stay_desc.set_content(
		'<?= LNG_OPT_ITEM_NO_TRADE_DESC ?>'
	);

	<? /* Steht-zum-Verkauf-Button mit Text */?>
	TreasureChest.item_details.items_sell_radio
		= TreasureChest.item_details.details_options.add_widget
	(
		PureFW.RadioButton,
		TreasureChest.item_details.items_stay_radio.position.x, 49,
		16, 16,
		'ui/elements/unit_training/special_radio_button.png');
	TreasureChest.item_details.items_sell_radio.add_event_handler('click',
		function(_i)
		{
			return function(ev){
				_i.amount_mp = parseInt(_i.amount_mp) + parseInt(_i.amount);
				_i.amount = 0;
				TreasureChest.item_details.items_sell_check.activate();
				if (TreasureChest.item_details.items_sell_check.checked)
					TreasureChest.item_details.items_sell_check.check();
				else
					TreasureChest.item_details.items_sell_check.uncheck();
			}
		}(item)
	);
	TreasureChest.item_details.items_sell_text
		= TreasureChest.item_details.details_options.add_widget
	(
		PureFW.Image,
		TreasureChest.item_details.items_stay_text.position.x,
		TreasureChest.item_details.items_sell_radio.position.y,
		137, 15,
		'ui/elements/texts/<?=$LNG?>/treasure/txt_item_trade.png'
	);
	TreasureChest.item_details.items_sell_text.add_event_handler('click',
			function(ev) {
				TreasureChest.item_details.items_sell_radio.on_click()
			});
	TreasureChest.item_details.items_sell_desc
		= TreasureChest.item_details.details_options.add_widget
	(
		PureFW.Container,
		200, TreasureChest.item_details.items_sell_radio.position.y + 2,
		200, 14
	);
	TreasureChest.item_details.items_sell_desc.set_font_color('#777');
	TreasureChest.item_details.items_sell_desc.set_font_size(0.7);
	TreasureChest.item_details.items_sell_desc.set_content(
		'<?= LNG_OPT_ITEM_TRADE_DESC ?>'
	);

	<? /* Linking the radio buttons */ ?>
	TreasureChest.item_details.items_stay_radio.link_with_radio_button(
		TreasureChest.item_details.items_sell_radio);
	TreasureChest.item_details.items_sell_radio.link_with_radio_button(
		TreasureChest.item_details.items_stay_radio);

	<? /* Direkt-Verkauf-Checkbox */ ?>
	TreasureChest.item_details.items_sell_check
		= TreasureChest.item_details.details_options.add_widget
	(
		PureFW.Checkbox,
		39, 68,
		36, 31,
		'ui/elements/form/input/check_toggle.png');
	TreasureChest.item_details.items_sell_check.add_event_handler('click',
		function (ev) {
			TreasureChest.item_details.items_sell_radio.on_click();
		}
	);
	TreasureChest.item_details.items_sell_check.add_event_handler('check',
		(function (_i) {
			return function (ev) {
				var price;
				var exclude;
				if(typeof(_i.exclude_direct) == 'undefined')
					exclude = 0;
				else
					exclude = _i.exclude_direct;
				if (!_i.price)
					price = _i.shop_price;
				else
					price = _i.price;

				TreasureChest.item_details.price_input.set_content(price);
				TreasureChest.item_details.item_quantity_input.set_content(
					exclude);
				TreasureChest.item_details.price_input.activate();
				TreasureChest.item_details.item_quantity_input.activate();
				TreasureChest.item_details.price_text.activate();
				TreasureChest.item_details.buy_from_price_text.activate();
			}
		}(item))
	);
	TreasureChest.item_details.items_sell_check.add_event_handler('uncheck',
		(function (_i) {
			return function (ev) {
				TreasureChest.item_details.price_input.deactivate();
				TreasureChest.item_details.item_quantity_input.deactivate();
				TreasureChest.item_details.price_text.deactivate();
				TreasureChest.item_details.buy_from_price_text.deactivate();
			}
		}(item))
	);


	TreasureChest.item_details.price_text
		= TreasureChest.item_details.details_options.add_widget
	(
		PureFW.Image,
		71, TreasureChest.item_details.items_sell_check.position.y,
		275, 36,
		'ui/elements/texts/<?=$LNG?>/treasure/txt_item_direct_price.png');

	TreasureChest.item_details.price_input
		= TreasureChest.item_details.details_options.add_widget
	(
		PureFW.Container,
		131, TreasureChest.item_details.items_sell_check.position.y+5,
		56, 25);
	TreasureChest.item_details.price_input.set_content(
		'<form name="TreasureChest_form_price" \
			action="javascript:TreasureChest.keyup(event);">\
		<input type="text" name="price" style="width:100%; height: 100%;\
		 background:url(../pix/pattern/spacer.gif) 0 0 no-repeat transparent;\
		 font-size: 1em; text-align: center; color: #333;\
		 border: none; position:absolute; top:0;left:0;"\
		 onkeydown="javascript: TreasureChest.keyup(event);"\
		 onkeyup="javascript: TreasureChest.keyup(event);"/></form>'
	);
	TreasureChest.item_details.price_input.set_bg_img(
		'ui/elements/form/input/input_text_56x25.png');
	TreasureChest.item_details.price_input.set_content = function(value) {
		document.forms.TreasureChest_form_price.price.value = value;
	}
	TreasureChest.item_details.price_input.get_content = function() {
		return document.forms.TreasureChest_form_price.price.value;
	}
	TreasureChest.item_details.price_input.deactivate = function() {
		PureFW.Container.parent.deactivate.call(this);
		document.forms.TreasureChest_form_price.price.disabled = true;
	}
	TreasureChest.item_details.price_input.activate = function() {
		PureFW.Container.parent.activate.call(this);
		document.forms.TreasureChest_form_price.price.disabled = false;
	}

	TreasureChest.item_details.buy_from_price_text
		= TreasureChest.item_details.details_options.add_widget
	(
		PureFW.Image,
		16, 111,
		353, 18,
		'ui/elements/texts/<?=$LNG?>/treasure/txt_stop_direct_trade.png');

	TreasureChest.item_details.item_quantity_input
		= TreasureChest.item_details.details_options.add_widget
	(
		PureFW.Container,
		44, 105,
		56, 25);
	TreasureChest.item_details.item_quantity_input.set_content(
		'<form name="TreasureChest_form_quantity" \
			action="javascript:TreasureChest.keyup(event);">\
		<input type="text" name="quantity" style="width:100%; height: 100%;\
		 background:url(../pix/pattern/spacer.gif) 0 0 no-repeat transparent;\
		 font-size: 1em; text-align: center; color: #333;\
		 border: none; position:absolute; top:0;left:0;"\
		 onkeydown="javascript: TreasureChest.keyup(event);"\
		 onkeyup="javascript: TreasureChest.keyup(event);"/></form>'
	);
	TreasureChest.item_details.item_quantity_input.set_bg_img(
		'ui/elements/form/input/input_text_56x25.png');
	TreasureChest.item_details.item_quantity_input.set_content = function(value) {
		document.forms.TreasureChest_form_quantity.quantity.value = value;
	}
	TreasureChest.item_details.item_quantity_input.get_content = function() {
		return document.forms.TreasureChest_form_quantity.quantity.value;
	}
	TreasureChest.item_details.item_quantity_input.deactivate = function() {
		PureFW.Container.parent.deactivate.call(this);
		document.forms.TreasureChest_form_quantity.quantity.disabled = true;
	}
	TreasureChest.item_details.item_quantity_input.activate = function() {
		PureFW.Container.parent.activate.call(this);
		document.forms.TreasureChest_form_quantity.quantity.disabled = false;
	}


	if(item.amount_mp > 0)
	{
		TreasureChest.item_details.items_sell_radio.on_click();
	}
	else
	{
		TreasureChest.item_details.items_stay_radio.on_click();
	}
	if (item.price > 0) {
		TreasureChest.item_details.items_sell_check.check();
	}
	else {
		TreasureChest.item_details.items_sell_check.uncheck();
	}
	chest_item.set_amount(parseInt(item.amount) + parseInt(item.amount_mp));

	TreasureChest.item_details.details_container.add_event_handler('confirm',
		function(_i) {
			return function(ev) {

				if (TreasureChest.item_details.price_input.is_activated())
					_i.price = TreasureChest.item_details.price_input.get_content();
				else
					_i.price = 0;

				if (TreasureChest.item_details.item_quantity_input.is_activated())
					_i.exclude_direct = TreasureChest.item_details
											.item_quantity_input.get_content();
				else
					_i.exclude_direct = 0;
				TreasureChest.commit_all_settings(_i)
			}
		}(item));
<? else : ?>
<?
	/**
	 * NUR SCHATZKISTE DER ANDEREN:
	 * Anzeige, ob ICH das Item habe.
	 */


	/**
	 *  Wenn es eine fremde Schatzkiste ist, dann zeigen wir jetzt die
	 *  Kaufmodalitäten an, statt dem Konfigurator für das Item.
	 */
?>
	if (item.my_instance) {
		TreasureChest.item_details.my_instance_text
				= TreasureChest.item_details.details_container.add_widget
		(
			PureFW.Container,
			TreasureChest.item_details.details_topic.position.x, 280,
			TreasureChest.item_details.details_container.width, 40
		);
		TreasureChest.item_details.my_instance_text.set_font_style('italic');
		TreasureChest.item_details.my_instance_text.set_font_weight('bold');

		var my_amount = parseInt(item.my_instance.amount);
		var my_amount_mp = parseInt(item.my_instance.amount_mp);
		var my_amount_total = my_amount + my_amount_mp;
		TreasureChest.item_details.my_instance_text.set_content(
			'<?= LNG_YOU_HAVE_THIS_TREASURE ?>'.replace('[AMOUNT]',
				my_amount_total)
		);
		if (my_amount_mp <= 0) {
			TreasureChest.item_details.my_instance_text.add_content(
				'<?= LNG_AND_IT_S_JUST_FOR_YOU ?>'
			);
		}
		else {
			if (item.my_instance.price > 0) {
				TreasureChest.item_details.my_instance_text.add_content(
					'<?= LNG_AND_OFFERING_IT_FOR_KKR ?>'.replace('[KKR]',
						item.my_instance.price)
				);
			}
			else {
				TreasureChest.item_details.my_instance_text.add_content(
					'<?= LNG_AND_OFFERING_FOR_TRADE ?>'
				);
			}
		}

	}
	if (!chest_item) {
		<? /**
			* In dem Fall bezieht es sich nicht auf ein Item, sondern auf
			* alle Items in allen Schatzkisten dieses Typs.
			* Dann ist es etwas komplex und wir holen die Liste der Verkäufer
			* bzw. potentiellen Verkäufer vom Server neu, um das nicht vorladen
			* zu müssen.
			*/ ?>
		TreasureChest.item_details.chest_item_list
				= TreasureChest.item_details.details_container.add_widget
			(
				PureFW.ContainerAJAX,
				TreasureChest.item_details.details_topic.position.x, 330,
				TreasureChest.item_details.details_container.width,	180,
				'<?= $_SERVER['PHP_SELF'] ?>?show_chest_item_list=true&type='
					+item.type+'&id='+item.id
			);
	}
	else {
		<? /**
			* Sonst normal nur ein Item, dann ist es einfach mit dem Bieten
			* und Kaufen.
			*/ ?>

		TreasureChest.item_details.one_offer_container
			= TreasureChest.item_details.details_container.add_widget
		(
			PureFW.Container,
			TreasureChest.item_details.details_topic.position.x,
			330,
			TreasureChest.item_details.width, 50
		);

		TreasureChest.item_details.one_offer_container.set_content(
			'<b><?=LNG_ITEM_SOLD_BY?><b><br />' +
			'<a href="javascript: UIButtons.toggle_window(\''
			+ 'profile.php?nick=<?= User::get_instance_by_id($uid)->get_nick() ?>'
			+ '\', \'\', 110);">'
				+ MammunUI.beautify_nick(
					'<?= User::get_instance_by_id($uid)->get_nick() ?>'
				)
			+ '</a>'
		);

		TreasureChest.item_details.seller_avatar
			= TreasureChest.item_details.details_container.add_widget
		(
			PureFW.Avatar.Avatar,
			TreasureChest.item_details.one_offer_container.position.x,
			TreasureChest.item_details.one_offer_container.position.y + 50,
			48, 48,
			TreasureChest.avatar_data[
				'<?= User::get_instance_by_id($uid)->get_nick() ?>'
			],
			'<?= User::get_instance_by_id($uid)->get_nick() ?>'
		);
		<?
		/**
		 * Wenn ein Preis feststeht, dann diesen anzeigen und einen Kaufen-
		 * Button mit entsprechender Funktion einbinden.
		 */?>
		if (item.price > 0) {
			TreasureChest.item_details.price_container
				= TreasureChest.item_details.details_container.add_widget
			(
			 	PureFW.Container,
				TreasureChest.item_details.seller_avatar.position.x + 52,
				TreasureChest.item_details.seller_avatar.position.y,
				110, 49
			);
			TreasureChest.item_details.price_container.set_bg_img(
				'ui/icons/labels/shop/klunker_price_110x49.png'
			);
			TreasureChest.item_details.price_container_inner =
				TreasureChest.item_details.price_container.add_widget
			(
				PureFW.Container,
				50, 15,
				45, 20
			);
			TreasureChest.item_details.price_container_inner.set_font_weight(
				'bold'
			);
			TreasureChest.item_details.price_container_inner.set_text_align(
				'center'
			);
			TreasureChest.item_details.price_container_inner.set_content(
				item.price
			);

			TreasureChest.item_details.buy_button
				= TreasureChest.item_details.details_container.add_widget
			(
				PureFW.Image,
				TreasureChest.item_details.one_offer_container.position.x,
				TreasureChest.item_details.price_container.position.y
					+ TreasureChest.item_details.price_container.height,
				201, 77,
				'ui/elements/form/buttons/<?=$LNG?>/shop/btn_kaufen_201x77.png'
			);
			TreasureChest.item_details.buy_button.add_event_handler("click",
				(function (_i) {
					return function(ev) {
						TreasureChest.buy_directly(_i);
					}
				})(item)
			);
		}
		<? /**
			* Sonst, wenn kein Preis feststeht, dann keinen anzeigen und einen
			* Biete-Button mit entsprechender Funktion einbinden
			*/ ?>
		else {
			TreasureChest.item_details.price_container
				= TreasureChest.item_details.details_container.add_widget
			(
			 	PureFW.Image,
				TreasureChest.item_details.seller_avatar.position.x + 52,
				TreasureChest.item_details.seller_avatar.position.y,
				110, 49,
				'ui/icons/labels/shop/klunker_price_unknown_110x49.png'
			);
			TreasureChest.item_details.buy_button
				= TreasureChest.item_details.details_container.add_widget
			(
				PureFW.Image,
				TreasureChest.item_details.one_offer_container.position.x,
				TreasureChest.item_details.price_container.position.y
					+ TreasureChest.item_details.price_container.height,
				200, 74,
				'ui/elements/form/buttons/<?=$LNG?>/shop/btn_bieten_200x74.png'
			);
			TreasureChest.item_details.buy_button.add_event_handler("click",
				(function (_i) {
					return function(ev) {
						TreasureChest.make_offer_dialog(_i);
					}
				})(item)
			);
		}
	}
<? endif; ?>

	<?
	/**
	 * Jetzt kommt die rechte Seite: Anprobieren des Items.
	 */
	?>
	TreasureChest.item_details.unit_chooser
		= TreasureChest.item_details.details_container.add_widget
	(
		PureFW.Items.UnitTypeChooser,
		490, 15,
		238, 115
	);
	TreasureChest.item_details.unit_chooser.add_widget_to_caption(
		PureFW.Image,
		(238 - 203) >> 1, 10,
		203, 17,
		'ui/elements/texts/<?=$LNG?>/treasure/txt_unit_chooser_caption.png'
	);
	if (item.type > <?= IIndividualization::TYPE_CLOTHES_MAX ?>)
		TreasureChest.item_details.unit_chooser.hide();

	TreasureChest.item_details.unit_preview
		= TreasureChest.item_details.details_container.add_widget
	(
		PureFW.Items.ClothPreview,
		TreasureChest.item_details.unit_chooser.position.x
			+ ((TreasureChest.item_details.unit_chooser.width-224)>>1),
		TreasureChest.item_details.unit_chooser.position.y
			+ TreasureChest.item_details.unit_chooser.height + 10,
		224, 314
	);
	TreasureChest.item_details.unit_preview.set_item(
		item
	);
	TreasureChest.item_details.unit_preview.set_z_index(2);
	TreasureChest.item_details.rarer_img
		= TreasureChest.item_details.details_container.add_widget
	(
		PureFW.Image,
		TreasureChest.item_details.unit_preview.position.x
			+ TreasureChest.item_details.unit_preview.width + 3,
		TreasureChest.item_details.unit_preview.position.y
			+ ((TreasureChest.item_details.unit_preview.height
				- 160) >> 1),
		18, 160,
		'ui/elements/texts/<?=$LNG?>/treasure/txt_rare_arrow.png'
	);

	<? /** Erst jetzt wird der Unit-Chooser konfiguriert, weil Preview vorher
		*  existieren muss */ ?>
	TreasureChest.item_details.unit_chooser.add_event_handler("select",
		function(ev) {
			TreasureChest.item_details.unit_preview.set_unit(ev.detail);
		}
	);
	TreasureChest.item_details.unit_chooser.set_unit_types(
		researched_races
	);

	<?
	/**
	 * Verschenken-Button (nur anzeigen, wenn meine Schatzkiste)
	 */
	?>
	<? if ($uid == $current_user->get_id()) : ?>
	TreasureChest.item_details.make_a_gift_btn
		= TreasureChest.item_details.details_container.add_widget
	(
		PureFW.Image,
		TreasureChest.item_details.unit_preview.position.x
			+ ((TreasureChest.item_details.unit_preview.width - 201) >> 1),
		TreasureChest.item_details.unit_preview.position.y
			+ TreasureChest.item_details.unit_preview.height,
		201, 61,
		'ui/elements/form/buttons/<?=$LNG?>/treasure_chest/make_a_gift.png'
	);
	TreasureChest.item_details.make_a_gift_btn.add_event_handler("click",
		(function(_item) {
			return function(ev) {
				TreasureChest.show_make_a_gift_dialog(_item);
			}
		})(item)
	);
	<? endif; ?>


	TreasureChest.item_details.details_container.add_event_handler('destroy',
		function (ev) {
			try {
				TreasureChest.officer.show();
			}
			catch(e){}
			TreasureChest.show_items();
		}
	);

};