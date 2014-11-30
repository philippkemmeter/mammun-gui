<script type="text/javascript">
TreasureChest.keyup = function(ev) {
	try
	{
		ev = PureFW.EventUtil.formatEvent(ev);
	}
	catch(e){}
	ev.stopPropagation();
}


<?	/**
	 * Erzeugt den Dialog zum Verschenken von dem gewählten Item.
	 */ ?>
TreasureChest.show_make_a_gift_dialog = function(item) {
	<? /* Wenn wider Erwarten der make_gift-Container noch existiert, wird er
	    * hiermit zerstört. Wahrscheinlicher ist jedoch, dass make_gift-Cont
	    * nicht "null" ist, aber das widget bereits zerstört wurde, was zu einem
	    * Fehler führte, weswegen hier ein silent-try-catch eingebaut ist.*/ ?>
	try {
		destroy_object(TreasureChest.make_gift);
		TreasureChest.make_gift = null;
	}
	catch(e){}
	TreasureChest.make_gift = new Object();

	TreasureChest.make_gift.main_container = new PureFW.ConfirmationBox(
		document.body,
		(1000-360)>>1,
		(700-265)>>1,
		360,265,
		null, true
	);
	TreasureChest.make_gift.main_container.set_bg_img(
		'ui/backgrounds/treasure/bg_gift.png'
	);
	TreasureChest.make_gift.main_container.set_z_index(500);
	var destructor = function(ev) {
		TreasureChest.make_gift.main_container
			.remove_event_handler("destroy",destructor);
		TreasureChest.make_gift.main_container
			.remove_event_handler("change",destructor);
		destroy_object(TreasureChest.make_gift);
		TreasureChest.make_gift = null;
	};

	TreasureChest.make_gift.main_container.add_event_handler(
		"confirm",
		(function (_item) {
			return function (ev) {
				PureFW.AJAXClientServer.send_request(
					"<?= $_SERVER['PHP_SELF'] ?>"+
					"?make_a_gift="+
						document.getElemById("TreasureChestGiftNick").value+
					"&type="+_item.type+
					"&id="+_item.id,
					TreasureChest.make_a_gift_response
				);
			}
		})(item)
	);

	TreasureChest.make_gift.topic_cont = new PureFW.Container(
		TreasureChest.make_gift.main_container,
		0, 15,
		TreasureChest.make_gift.main_container.width,
		25
	);
	TreasureChest.make_gift.topic_cont.set_font_size(1.5);
	TreasureChest.make_gift.topic_cont.set_content(
		'<?= LNG_MAKE_A_GIFT ?>'
	);
	TreasureChest.make_gift.topic_cont.set_text_align('center');

	TreasureChest.make_gift.to_a_friend = new PureFW.Image(
		TreasureChest.make_gift.main_container,
		30,
		TreasureChest.make_gift.topic_cont.position.y
			+ TreasureChest.make_gift.topic_cont.height + 30,
		135, 15,
		'ui/elements/texts/<?=$LNG?>/treasure/txt_gift_to_a_friend.png'
	);

	TreasureChest.make_gift.friend_container = new PureFW.ScrollContainer(
		TreasureChest.make_gift.main_container,
		TreasureChest.make_gift.to_a_friend.position.x,
		TreasureChest.make_gift.to_a_friend.position.y
			+ TreasureChest.make_gift.to_a_friend.height,
		TreasureChest.make_gift.main_container.width
		 - (TreasureChest.make_gift.to_a_friend.position.x << 1),
		 60
	);
	var p = 44/48*100;
	var p_padding = (100 - p)/2;
	var x = 0;
	for (var cat in PauseMenu.buddies) {
		if ((cat != 'onlineFriends') && (cat != 'offlineFriends'))
			continue;
		var n = PauseMenu.buddies[cat].length;
		for (var i = 0; i < n; i++) {
			var temp = new Object();
			temp.avatar_container = new PureFW.Container(
				TreasureChest.make_gift.friend_container,
				x, (TreasureChest.make_gift.friend_container.height - 48)>>1,
				48, 48);
			temp.avatar_container.set_bg_color('#cccccc');
			temp.avatar_container.set_content(
				'<img border="0" title="'+PauseMenu.buddies[cat][i].nick+'" '
				+ 'src="'+PauseMenu.avatars[PauseMenu.buddies[cat][i].nick]
				            							.replace('SIZE',44)+'" '
				+ 'style="width:'+p+'%; height:'+p+'%; position: absolute;'
				+	'left:'+p_padding+'%; top:'+p_padding+'%;"/>'
			);
			temp.avatar_container.add_event_handler(
				"click",
				(function (_nick) {
					return function(ev) {
						document.getElemById("TreasureChestGiftNick").value
							= _nick;
					}
				})(PauseMenu.buddies[cat][i].nick)
			);
			x += 50;
		}
	}
	TreasureChest.make_gift.friend_container.set_inner_width(x);

	TreasureChest.make_gift.to_anybody = new PureFW.Image(
		TreasureChest.make_gift.main_container,
		TreasureChest.make_gift.to_a_friend.position.x,
		TreasureChest.make_gift.friend_container.position.y
		+ TreasureChest.make_gift.friend_container.height + 20,
		218, 14,
		'ui/elements/texts/<?=$LNG?>/treasure/txt_gift_to_anybody.png'
	);


	TreasureChest.make_gift.nick_input = new PureFW.Container(
		TreasureChest.make_gift.main_container,
		TreasureChest.make_gift.to_anybody.position.x,
		TreasureChest.make_gift.to_anybody.position.y
			+ TreasureChest.make_gift.to_anybody.height,
		298, 28
	);
	TreasureChest.make_gift.nick_input.set_bg_img(
		'ui/elements/form/input/input_text_294x28.png'
	);
	TreasureChest.make_gift.nick_input.set_content(
        '<form name="TreasureChest_make_gift_form" \
        action="javascript: MammunUI.form_keyupdown(event);">\
		<input type="text" id="TreasureChestGiftNick" \
		value="" style="width:100%; \
		        height: 100%; padding:4px; color: #366A9B; \
		 background:url(../pix/pattern/spacer.gif) 0 0 no-repeat transparent;\
		 border: none; position:absolute; top:0;left:0;" \
		 onkeydown="javascript: MammunUI.form_keyupdown(event)"\
		 onkeyup="javascript: MammunUI.form_keyupdown(event)"/></form>'
	);
}

TreasureChest.make_a_gift_response = function(response_arr) {
	if (response_arr[0] == "1") {
		var type = parseInt(response_arr[2]);
		var id = parseInt(response_arr[3]);
		var amount = parseInt(response_arr[4]);
		var amount_mp = parseInt(response_arr[5]);
		for (var i in TreasureChest.all_items[type])
		{
			if (TreasureChest.all_items[type][i].id == id)
			{
				TreasureChest.all_items[type][i].amount = amount;
				TreasureChest.all_items[type][i].amount_mp = amount_mp;
				break;
			}
		}
		TreasureChest.item_details.details_amt_cont.set_content(
			amount + amount_mp
		);
		if ((amount + amount_mp) <= 0) {
			try {
				destroy_object(TreasureChest.item_details);
				TreasureChest.item_details = null;
			}
			catch(e){}
		}
		TreasureChest.show_items();
		MammunUI.show_warning(350, 200, response_arr[1],
			PureFW.ConfirmationBox.YES);
		TreasureChest.update_amount_ui();
	}
	else if (response_arr[0] == "0") {
		MammunUI.show_warning(350, 200, response_arr[1],
			PureFW.ConfirmationBox.NO, true);
	}
	else {
		throw Error(response_arr);
	}
}


TreasureChest.commit_all_settings = function(item)
{
	/*if(isNaN(chosen_price))
	{
		alert("Price is not a number!");
		return;
	}
	if(isNaN(chosen_quantity))
	{
		alert("Quantity to exclude is not a number!");
		return;
	}*/

    var url = "<?= $_SERVER['PHP_SELF'] ?>?set_item_properties=true"+
        		"&id="+item.id+
        		"&type="+item.type+
        		"&sellable="+((item.amount_mp > 0)?'1':'0')+
    			"&price="+item.price+
				"&exclude_direct="+item.exclude_direct;
    PureFW.AJAXClientServer.send_request(url,
    	TreasureChest.commit_all_settings_response);
};

TreasureChest.commit_all_settings_response = function(response_arr)
{
	if (response_arr[0] == "1") {
		var type = parseInt(response_arr[2]);
		var id = parseInt(response_arr[3]);
		var amount = parseInt(response_arr[4]);
		var amount_mp = parseInt(response_arr[5]);
		var price = parseInt(response_arr[6]);
		if (isNaN(price))
			price = 0;
		var exclude_direct = parseInt(response_arr[7]);
		if (isNaN(exclude_direct))
			exclude_direct = 0;
		for (var i in TreasureChest.all_items[type])
		{
			if (TreasureChest.all_items[type][i].id == id)
			{
				TreasureChest.all_items[type][i].amount = amount;
				TreasureChest.all_items[type][i].amount_mp = amount_mp;
				TreasureChest.all_items[type][i].price = price;
				TreasureChest.all_items[type][i].exclude_direct = exclude_direct;
				break;
			}
		}
		TreasureChest.show_items();
		MammunUI.show_warning(350, 200, response_arr[1],
			PureFW.ConfirmationBox.YES);
		TreasureChest.update_amount_ui();
	}
	else if (response_arr[0] == "0")
	{
		MammunUI.show_warning(350, 200, response_arr[1],
			PureFW.ConfirmationBox.NO, true);
	}
	else
	{
		throw Error(response_arr);
	}
};

<? /**
	* Diese Funktion mach ein Update der UI. Der Overview Item Gainer wird
	* aktuallisiert, dass er die aktuelle Gesamtsumme aller Items anzeigt.
	*/ ?>
TreasureChest.update_amount_ui = function() {
	var amount = 0;
	for (var type in TreasureChest.all_items) {
		if (typeof (TreasureChest.all_items[type]) == 'function')
			continue;
		for (var item_i in TreasureChest.all_items[type]) {
			if (TreasureChest.all_items[type][item_i].amount)
				amount += parseInt(TreasureChest.all_items[type][item_i].amount);
			if (TreasureChest.all_items[type][item_i].amount_mp)
				amount += parseInt(TreasureChest.all_items[type][item_i].amount_mp);
		}
	}

	ui_items_gainer.set_current_amount(amount);
	ui_refresh();
}

<? /**
    * Diese Funktion führt dazu, dass das angegebene Angebot akzeptiert wird.
    */ ?>
TreasureChest.accept_offer = function(item, buyer_nick)
{
	post_data = "id="+item.id+"&type="+item.type+"&accept_offer="+buyer_nick;
	PureFW.AJAXClientServer.send_request("<?= $_SERVER['PHP_SELF'] ?>",
			TreasureChest.accept_offer_response, post_data);
}

<? /**
    * Diese Funktion behandelt die Serverantwort im Falle von Offer-ACcept und
    * Offer-Reject, da beide ähnliche Konsequenzen haben. Da sie nicht ganz
    * gleich sind, die Konsequenzen, muss der zweite Parameter angegeben werden.
    * Ist accept==true, wird der Accept-Fall angenommen, sonst Reject.
    */ ?>
TreasureChest.accept_reject_response = function(response_arr, accept)
{
	var no_items = null;
	var no_offers = null;
	if (response_arr[0] == "1")
	{
		//type and id for item just accepted/rejected
		var type = parseInt(response_arr[2]);
		var id = parseInt(response_arr[3]);

		if (accept) {
			var klunker = parseInt(response_arr[4]);
			var buyer_nick = response_arr[5];
		}
		else {
			var buyer_nick = response_arr[4];
		}
		var this_item = null;
		for (var i in TreasureChest.all_items[type]) {
			if (TreasureChest.all_items[type][i].id == id) {
				if (accept) <? /* Wenn Accept, dann hab ich eins weniger*/ ?>
				{
					TreasureChest.all_items[type][i].amount_mp -= 1;
					if (TreasureChest.all_items[type][i].amount_mp == 0)
					{
						TreasureChest.all_items[type][i].offers = new Array();
					}
					var allItemArray = TreasureChest.create_offers_item_array();
					if (allItemArray.length ===0)
					{
						no_items = true;
						no_offers = true;
					}
					else
					{
						no_items = false;
						for(var k = 0; k < allItemArray.length; k++)
						{
							if(allItemArray[k].offers.length === 0)
							{
								no_offers = true;
							}
						}
					}
				}
				<? /** Das Angebot vom Item entfernen */ ?>
				var n = TreasureChest.all_items[type][i].offers.length;
				for (var j = 0; j < n; j++)
				{
					if (TreasureChest.all_items[type][i].offers[j].buyer_nick
						== buyer_nick)
					{
						TreasureChest.all_items[type][i].offers.splice(j, 1);
						break;
					}
				}
//				if (TreasureChest.all_items[type][i].offers.length <= 0)
//				{
//					no_offers = true;
//				}
				this_item = TreasureChest.all_items[type][i];
				break;
			}
		}
		if (accept) {
			if (ui_klunker_gainer)
				ui_klunker_gainer.set_current_amount(klunker);
			ui_refresh();
		}
		if (!(!no_items && !no_offers))
		{
			TreasureChest.item_details.details_container.destroy();
		}
		else
		{
			TreasureChest.show_all_offers(this_item);
		}
		TreasureChest.show_items();
		MammunUI.show_warning(400, 300, response_arr[1],
			PureFW.ConfirmationBox.YES);
		var offer_amount = TreasureChest.get_current_offers();
		Overview.set_offers_amount(offer_amount);
		TreasureChest.offer_amount_circle.set_content(offer_amount);
		if (offer_amount <= 0)
			TreasureChest.offer_amount_circle.hide();

	}
	else if (response_arr[0] == "0") {
		MammunUI.show_warning(400, 300, response_arr[1],
			PureFW.ConfirmationBox.NO);
	}
	else {
		MammunUI.show_warning(400, 300, '<?=LNG_STRANGE_ERROR_ACCEPT_OFFER?>',
			PureFW.ConfirmationBox.NO, true);
		throw Error(response_arr);
	}
}

<? /**
    * Antwort des Servers, wenn ein Angebot akzeptiert wurde, wird hier
    * verarbeitet.
    */ ?>
TreasureChest.accept_offer_response = function(response_arr) {
	TreasureChest.accept_reject_response(response_arr, true);
}

<? /**
    * Diese Funktion führt dazu, dass das angegebene Angebot abgelehnt wird.
    */ ?>
TreasureChest.reject_offer = function(item, buyer_nick) {
	post_data = "id="+item.id+"&type="+item.type+"&reject_offer="+buyer_nick;
	PureFW.AJAXClientServer.send_request("<?= $_SERVER['PHP_SELF'] ?>",
			TreasureChest.reject_offer_response, post_data);
}

<? /**
    * Antwort des Servers, wenn ein Angebot abgelehnt wurde, wird hier
    * verarbeitet.
    */ ?>
TreasureChest.reject_offer_response = function(response_arr) {
	TreasureChest.accept_reject_response(response_arr, false);
}

<?
	/**
	 * This function returns an array of all proper items from the
	 * TreasureChest.all_items array and allows them to be used without any further
	 * manipulation. used in TreasureChest.show_all_offers().
	 */
?>

TreasureChest.create_offers_item_array = function()
{
	var validItemArray = new Array();
	for (var type in TreasureChest.all_items)
	{
		if (typeof (TreasureChest.all_items[type]) == 'function')
			continue;
		for (var item_i in TreasureChest.all_items[type])
		{
			var item = TreasureChest.all_items[type][item_i];
			if (typeof(item) == 'function')
			{
				continue;
			}
				if (item.offers.length == 0)
				{
					continue;
				}
			validItemArray.push(item);
		}
	}
	return validItemArray;
};

TreasureChest.create_outgoing_offers_item_array = function()
{
	var validItemArray = new Array();
	TreasureChest.outgoing_offers.sort(TreasureChest.timeSort);

	for (var i = 0;i < TreasureChest.outgoing_offers.length; i++)
	{
		var item = new Object();
		item.type = TreasureChest.outgoing_offers[i].type;
		item.id = TreasureChest.outgoing_offers[i].id;
		validItemArray.push(item);
	}
	return validItemArray;
};

<?
/**
 * Diese Funktion erzeugt den click-Event-Handler der einzelnen Container,
 * welche expandieren können. Da diese Closure etwas komplex ist und auch im
 * Speicher einiges wiegt, wird sie hier einmal definiert.
 */?>
TreasureChest.create_expand_container_click_function =
	function(_item, _o_count, _i_count)
{
	return function(ev)
	{
		try
		{
		   	ev = PureFW.EventUtil.formatEvent(ev);
			ev.stopPropagation();
		}
		catch(e) {}
		if(!_item.collapsed) // To shrink the Offers
		{
			// First close the main Container of the item and
			// set the first offer's background to normal
			TreasureChest.item_details.item_offer_containers[_i_count].set_height(30);

			// Change the picture of the expansion button
			TreasureChest.item_expand_containers[_i_count].set_pic_url("ui/elements/form/buttons/grow_down.png");
			// And increase the inner height of the scroll container to allow for eventual scrolling
			TreasureChest.item_details.details_scroller.set_inner_height(
					(TreasureChest.item_details.details_scroller.inner_container.get_height() /
					TreasureChest.item_details.details_scroller.inner_container.scale_factor) -_o_count*30 + 30);
			// Finally go down the list of all items below the current
			// and move them down to make way for the newly shown offers.
			for (var j = _i_count + 1; j < TreasureChest.item_details.item_offer_containers.length; j++)
			{
				TreasureChest.item_details.item_offer_containers[j].set_y(
						TreasureChest.item_details.item_offer_containers[j].get_y() - _o_count*30+30);
			}
			_item.collapsed = true;
		}
		else // To expand the Offers
		{
			// First open the ItemContainer of all the offers remove the normal background of the first
			TreasureChest.item_details.item_offer_containers[_i_count].set_height(_o_count*30/* + 30*/);

			// Change the picture of the expansion button
			TreasureChest.item_expand_containers[_i_count].set_pic_url("ui/elements/form/buttons/shrink_up.png");
			// And decrease the inner height of the scroll container for proper (if any) scrolling
			TreasureChest.item_details.details_scroller.set_inner_height(
					(TreasureChest.item_details.details_scroller.inner_container.get_height() /
						TreasureChest.item_details.details_scroller.inner_container.scale_factor)
						+_o_count*30 - 30);
			// Finally go down the list of all items below the current
			// and move them up to their proper positions
			for (var j = _i_count + 1; j < TreasureChest.item_details.item_offer_containers.length; j++)
			{
				TreasureChest.item_details.item_offer_containers[j].set_y(
						TreasureChest.item_details.item_offer_containers[j].get_y() + _o_count*30-30);
			}
			_item.collapsed = false;
		}
	}
}


<?
	/**
	 * Creates a new Details window with a ScrollContainer inside.
	 * Takes every item and creates a Container for offers to be stored inside.
	 * Inside this container are places all the offers of the item. Each
	 * Container has the capability of shrinking or expanding, showing all or
	 * only one of its offers. The offer with the highest bid price is shown
	 * in first place without need for collapsing.
	 */
?>

TreasureChest.show_all_offers = function(outgoing)
{
	if (typeof(outgoing) == 'undefined')
		outgoing = false;
	<? /* Wenn wider erwarten der details-container noch existiert, wird er
	    * hiermit zerstört. Wahrscheinlicher ist jedoch, dass details_container
	    * nicht "null" ist, aber das widget bereits zerstört wurde, was zu einem
	    * Fehler führte, weswegen hier ein silent-try-catch eingebaut ist.*/ ?>
	try {
		destroy_object(TreasureChest.item_details);
	}
	catch(e){}
	<? /** TreasureChest.item_details wird überschrieben und neugesetzt. Der
		* Name ist offensichtlich ungünstig gewählt, es wäre besser
		* "TreasureChest.current_dialog" gewesen, aber damals gab's nur den
		* einen...*/ ?>
	TreasureChest.item_details = new Object();
	TreasureChest.item_details.details_container = MammunUI.show_warning(
		730,460,
		'',
		PureFW.ConfirmationBox.NO
	);

	TreasureChest.item_details.details_scroller_bg = new PureFW.Container(
		TreasureChest.item_details.details_container,
		(TreasureChest.item_details.details_container.width - 659)>>1,
		(TreasureChest.item_details.details_container.height-362)>>1,
		659, 362
	);
	TreasureChest.item_details.details_scroller_bg.set_bg_img(
		'ui/backgrounds/treasure/offer_list_bg_659x362.png'
	);
	TreasureChest.item_details.details_scroller_bg.set_font_color('black');
	<? /** Beschriftung des Containers */ ?>
	if(!outgoing)
	{
		TreasureChest.item_details.offers_in_label = new PureFW.Image(
			TreasureChest.item_details.details_container,
			TreasureChest.item_details.details_scroller_bg.position.x - 10,
			TreasureChest.item_details.details_scroller_bg.position.y - 20,
			178, 44,
			'ui/elements/texts/<?=$LNG?>/treasure/offers_in.png'
		);
	}
	else
	{
		TreasureChest.item_details.offers_out_label = new PureFW.Image(
			TreasureChest.item_details.details_container,
			TreasureChest.item_details.details_scroller_bg.position.x - 10,
			TreasureChest.item_details.details_scroller_bg.position.y - 20,
			131, 44,
			'ui/elements/texts/<?=$LNG?>/treasure/offers_out.png'
		);
	}

	TreasureChest.item_details.details_scroller = new PureFW.ScrollContainer(
		TreasureChest.item_details.details_scroller_bg,
		(TreasureChest.item_details.details_scroller_bg.width-607)>>1, 45,
		607,
		TreasureChest.item_details.details_scroller_bg.height - 70
	);

	<? /** Über der Liste, die im Scroller entstehen wird, kommen nun noch
			Icons, um die Tabelle zu beschriften*/?>
	TreasureChest.item_details.kkr_ico = new PureFW.Image(
		TreasureChest.item_details.details_scroller_bg,
		390, 20,
		23, 21,
		'ui/icons/labels/resources/21/klunker.png'
	);
	TreasureChest.item_details.kkr_ico.set_tooltip(
		'<?= LNG_TOOLTIP_OFFER_KKR ?>'
	);
	TreasureChest.item_details.time_ico = new PureFW.Image(
		TreasureChest.item_details.details_scroller_bg,
		490, 20,
		20, 21,
		'ui/icons/labels/resources/21/wecker.png'
	);
	TreasureChest.item_details.time_ico.set_tooltip(
		'<?= LNG_TOOLTIP_OFFER_TIME ?>'
	);

	// All the crap to be destroyed on close. This function is attached to
	// the X button on the bottom of the box.
	var destructor = function()
	{
		TreasureChest.item_details.details_container
			.remove_event_handler("destroy",destructor);
		TreasureChest.item_details.details_container
			.remove_event_handler("change",destructor);
		destroy_object(TreasureChest.item_details);
	};


	TreasureChest.item_details.item_offer_containers = new Array();	// One per Item
//	TreasureChest.item_details.item_expand_containers = new Array();// For expansion buttons
	var y = 0;

	// Contains all the items to be used (see above function)
	var itemArray = (outgoing) ?
			TreasureChest.create_outgoing_offers_item_array() :
			TreasureChest.create_offers_item_array();

	for(var item_counter = 0; item_counter < itemArray.length; item_counter++)
	{
		var item = itemArray[item_counter];
		// Build a Container to store all the offers of the current item.
		TreasureChest.item_details.item_offer_containers[item_counter]
														= new PureFW.Container(
			TreasureChest.item_details.details_scroller.get_content_node(),
			0, y,
			TreasureChest.item_details.details_scroller.width, 30
		);

		<? /** jedes ungerade Angebot dunkel darstellen **/ ?>
		var is_dark_row = (item_counter != (item_counter | 1));

		<?
		/**
	    * Tabelle der Angebote erstellen. Die letzte Spalte enthält die Aktionen
	    * "Annehmen"/"Ablehnen". Die Aktionsbuttons werden wegen Skalierung
	    * aber erst später erstellt (s.u.)
	    *
	    * Each offer the item is stored in has its own Container. The
	    * item.sub_offers array hold all of these offer Containers.
	    */
		?>
		var n = (outgoing) ?
				1 :
				item.offers.length;
		item.sub_offers = new Array();
		item.collapsed = false;
		if(!outgoing)
		{
			item.offers.sort(TreasureChest.priceSort);
		}
		for (var i = 0; i < n; i++) {
			var offer = (outgoing) ?
					TreasureChest.outgoing_offers[item_counter] :
					item.offers[i];
			item.sub_offers[i] = new Object();
			item.sub_offers[i].destroy = (function(_o) {
				return function (ev) {
					destroy_object(_o);
				}
			})(item.sub_offers[i]);

			item.sub_offers[i].main_cont = new PureFW.Container(
				TreasureChest.item_details.item_offer_containers[item_counter]
					.get_content_node(),
				0, i*25+1,
				TreasureChest.item_details.details_scroller.width, 24
			);
			if (is_dark_row) {
				item.sub_offers[i].main_cont.set_bg_img(
					'ui/backgrounds/treasure/row_dark.png'
				);
			}

			item.sub_offers[i].item_pic = new PureFW.Image(
				item.sub_offers[i].main_cont,
				5, 0,
				24, 24
			);
			if (i==0) {
				item.sub_offers[i].item_pic.set_pic_url(
					'ui/icons/labels/items/65/'+item.type+'/'+item.id+'.png'
				);
			}
			else {
				item.sub_offers[i].item_pic.set_pic_url(
					'pattern/spacer.gif'
				);
			}

			item.sub_offers[i].name_cont = new PureFW.Container(
				item.sub_offers[i].main_cont,
				item.sub_offers[i].item_pic.position.x
					+ item.sub_offers[i].item_pic.width + 5,
				(item.sub_offers[i].main_cont.height - 16) >> 1,
				200,
				16
			);
			if (i==0)
			{
				if(outgoing)
				{
					item.sub_offers[i].name_cont.set_content(
						offer.name.replace('<br/>', '')
					);
				}
				else
				{
					item.sub_offers[i].name_cont.set_content(
						item.name.replace('<br/>', '')
					);
				}
			}

			var nick = (outgoing) ? offer.nick : offer.buyer_nick;
			item.sub_offers[i].avatar_cont
				= item.sub_offers[i].main_cont.add_widget
			(
				PureFW.Avatar.Avatar,
				item.sub_offers[i].name_cont.position.x
					+ item.sub_offers[i].name_cont.width,
				0,
				24, 24,
				TreasureChest.avatar_data[nick],
				nick
			);

			item.sub_offers[i].nick_cont = new PureFW.Container(
				item.sub_offers[i].main_cont,
				item.sub_offers[i].avatar_cont.position.x
					+ item.sub_offers[i].avatar_cont.width + 16,
				item.sub_offers[i].name_cont.position.y,
				80,
				item.sub_offers[i].name_cont.height
			);
			item.sub_offers[i].nick_cont.set_content(
				nick
			);

			item.sub_offers[i].price_cont = new PureFW.Container(
				item.sub_offers[i].main_cont,
				item.sub_offers[i].nick_cont.position.x
					+ item.sub_offers[i].nick_cont.width,
				item.sub_offers[i].nick_cont.position.y,
				40,
				item.sub_offers[i].name_cont.height
			);
			item.sub_offers[i].price_cont.set_content(offer.price);
			item.sub_offers[i].price_cont.set_font_weight('bold');
			if(outgoing)
			{
				item.sub_offers[i].price_cont.set_text_align('center');
			}
			else
			{
				item.sub_offers[i].price_cont.set_text_align('right');
			}

			item.sub_offers[i].time_cont = new PureFW.Container(
				item.sub_offers[i].main_cont,
				item.sub_offers[i].price_cont.position.x
					+ item.sub_offers[i].price_cont.width + 25,
				item.sub_offers[i].price_cont.position.y,
				110,
				item.sub_offers[i].name_cont.height
			);
			if(outgoing)
			{
				item.sub_offers[i].time_cont.set_content(
					PureFW.Time.sec_in_time(offer.time_left)
				);
				item.sub_offers[i].time_cont.set_text_align('center');
			}
			else
			{
				item.sub_offers[i].time_cont.set_content(offer.t_stamp);
				item.sub_offers[i].time_cont.set_text_align('right');
			}

			item.sub_offers[i].button_yes = new PureFW.Image(
				item.sub_offers[i].main_cont,
				item.sub_offers[i].time_cont.position.x
					+ item.sub_offers[i].time_cont.width+25,
				(item.sub_offers[i].main_cont.height-19)>>1,
				24,19,
				'ui/icons/labels/treasure_chest/offer_yes.png'
			);
			item.sub_offers[i].button_yes.hide();

			item.sub_offers[i].button_no = new PureFW.Container(
				item.sub_offers[i].main_cont,
				item.sub_offers[i].button_yes.position.x
					+ item.sub_offers[i].button_yes.width,
				item.sub_offers[i].button_yes.position.y,
				22,19
			);
			item.sub_offers[i].button_no.set_bg_img(
				'ui/icons/labels/treasure_chest/offer_no.png'
			);

			if(!outgoing)
			{
				item.sub_offers[i].button_yes.show();
				item.sub_offers[i].button_yes.add_event_handler(
					"click",
					(function(_n1,_n2) {
						return function(ev) {
							TreasureChest.accept_offer(_n1,_n2);
						}
					})(item, item.offers[i].buyer_nick)
				);
				item.sub_offers[i].button_yes.set_tooltip(
					'<?= LNG_TOOLTIP_OFFER_O ?>'
				);
				item.sub_offers[i].button_no.add_event_handler(
					"click",
					(function(_n1,_n2) {
						return function(ev) {
							TreasureChest.reject_offer(_n1,_n2);
						}
					})(item, item.offers[i].buyer_nick)
				);
				item.sub_offers[i].button_no.set_tooltip(
					'<?= LNG_TOOLTIP_OFFER_X ?>'
				);
			}
			else
			{
				item.sub_offers[i].button_no.set_x(item.sub_offers[i].button_no.position.x - 10);
				item.sub_offers[i].button_no.add_event_handler(
					"click",
					(function(_n1,_n2) {
						return function(ev) {
							TreasureChest.withdraw_offer(_n1,_n2);
						}
					})(item, offer.nick)
				);
				item.sub_offers[i].button_no.set_tooltip(
					'<?= LNG_TOOLTIP_MY_OFFER_X ?>'
				);
			}
		}

		// Place the Container, containing all the offers, in the proper place.
		// Position depends on the number of items already created.
		TreasureChest.item_details.item_offer_containers[item_counter]
		                                         		.set_height((n-1)*25+30);
//		// If there is more than one offer per item.
//		if(n > 1)
//		{
//			// Create an expansion button with the below function
//			TreasureChest.item_details.item_expand_containers[item_counter]
//			                                      			= new PureFW.Image(
//				TreasureChest.item_details.item_offer_containers[item_counter]
//				                                 			.get_content_node(),
//				0, 0,
//				28, 28,
//				"ui/elements/form/buttons/grow_down.png"
//			);
//			TreasureChest.item_details.item_expand_containers[item_counter]
//				.add_event_handler(
//					"click",
//					TreasureChest.create_expand_container_click_function(
//							item, item.offers.length, item_counter)
//			);
//		}

		TreasureChest.item_details.show_offer_action_buttons = new Array();
		y = y + TreasureChest.item_details.item_offer_containers[item_counter].get_height();

		<?
		/**
		 * The idea here is to collape all of the items after they have been
		 * created. There is currently no function to do this.
		 */
		?>
		/*for (var j = 0; j < TreasureChest.item_details.item_offer_containers.length; j++)
		{
			TreasureChest.item_details.item_offer_containers[j].set_height(30);
			TreasureChest.item_details.item_offer_containers[j].set_y(j*30);
		}*/
	}
	TreasureChest.item_details.details_scroller.set_inner_height(y);
	TreasureChest.item_details.details_container.add_event_handler("change",
		destructor);
	TreasureChest.item_details.details_container.add_event_handler("destroy",
		destructor);
};

TreasureChest.withdraw_offer = function(item, buyer_nick)
{
	post_data = "id="+item.id+
				"&type="+item.type+
				"&withdraw_offer="+buyer_nick;

	PureFW.AJAXClientServer.send_request("<?= $_SERVER['PHP_SELF'] ?>",
			TreasureChest.withdraw_offer_response, post_data);
};

TreasureChest.withdraw_offer_response = function(response_arr)
{
	if (response_arr[0] == "1")
	{
//		alert(	"0: "+response_arr[0]+
//				"\n1: "+response_arr[1]+
//				"\n2: "+response_arr[2]+
//				"\n3: "+response_arr[3]+
//				"\n4: "+response_arr[4]);
		if (ui_klunker_gainer)
			ui_klunker_gainer.set_current_amount(parseInt(response_arr[5]));
		TreasureChest.outgoing_offers = eval('('+response_arr[6]+')');
		TreasureChest.show_all_offers(true);
		ui_refresh();
	}
	else if (response_arr[0] == "0")
	{
		MammunUI.show_warning(400, 300, response_arr[1],
			PureFW.ConfirmationBox.NO, true);
	}
	else
	{
		MammunUI.show_warning(400, 300, '<?=LNG_STRANGE_ERROR_OFFER?>',
			PureFW.ConfirmationBox.NO, true);
		throw Error(response_arr);
	}
};

<?
/**
 * Specifies how to sort the items in a given array.
 */
?>

TreasureChest.priceSort = function(a, b)
{
	return b.price - a.price;
}

TreasureChest.timeSort = function(a, b)
{
	return b.t_stamp - a.t_stamp;
}
</script>