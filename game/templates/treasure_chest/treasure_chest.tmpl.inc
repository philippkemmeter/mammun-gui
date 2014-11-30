<script type="text/javascript">
TreasureChest = new Object();
<? /* Das aktuelle Fenster, in dem diese Seite dargestellt wird */ ?>
TreasureChest.this_window = PureFW.ContainerAJAX.instances[<?=$this_win_id?>];
TreasureChest.this_window.set_css_class('ConfirmationBox');

if (PureFW.Officer.get_current_active_officer())
	PureFW.Officer.get_current_active_officer().hide();

<? /* Enthält alle Items, die man kaufen kann. */ ?>
TreasureChest.all_items = <?= $all_items_json ?>;

<? /* Contains all offers made to other players */ ?>
TreasureChest.outgoing_offers = <?= json_encode($outgoing_offers) ?>;

<? /* Enthält alle Container, die ein Item in der Liste darstellen und auch
      gerade sichtbar sind. Siehe TreasureChest.show_items für weiteres. */ ?>
TreasureChest.item_containers = new Array();

<? /* Wie groß ein jedes Item dargestellt werden soll. Insgesamt mit allen
      eingeblendeten Infos */ ?>
TreasureChest.item_size = new PureFW.Point(183, 200);
<? /* Wie viel Platz zwischen den Items sein soll (in x und y) */ ?>
TreasureChest.item_margin = new PureFW.Point(0,0);

<? /* Dieses Container enthält die Details des ausgewählten Widgets */ ?>
TreasureChest.details_container = null;

<? /* Check if the TreasureChest belongs to the player or someone else.*/ ?>
TreasureChest.own_chest = <? if ($uid == $current_user->get_id())
							 { echo "true";}
							 else
							 {echo "false";}?>

<? /* Avatare, die angezeigt werden könnten (bei Geboten oder
	Schatzkistenbesitzer) */ ?>
TreasureChest.avatar_data = <?= json_encode($avatar_data) ?>;

<? /* Alle Waffen/Rassen, die der Spieler erforscht hat */ ?>
TreasureChest.researched_races = new Array();
<? foreach ($researched_races as $i => $race) : ?>
<? $pic_core = substr(ScienceConstants::$PICS[$race], 0,
							strrpos(ScienceConstants::$PICS[$race],'.')); ?>
TreasureChest.researched_races.push('<?=$pic_core?>');
 <? endforeach; ?>

<?
/**
 * Zeigt alle Items des Marktplatzes an.
 *
 * @return bool ob welche angezeigt werden
 */
?>
TreasureChest.show_items = function()
{
 	TreasureChest.trade_item_scroller.remove_all_items();

 	<?
 	/**
 	 * Create all items. TODO:BOOKMARK
 	 */
 	?>
 	<?
 	/**
 	 * Erste Schleife: Wir wollen herausfinden, welche Itemähnlichkeitsgruppen
 	 * nur ein Mitglied haben. Eine solche Gruppe beinhaltet Items gleichen
 	 * Schnitts, aber ggf. unterschiedlicher Farbe.
 	 */
 	?>
 	var similar_group = new Array();
 	var no_items = true;
 	for (var type in TreasureChest.all_items)
 	{
 		if (typeof (TreasureChest.all_items[type]) == 'function')
 			continue;

 		similar_group[type] = new Array();
 		if (ChestShopFilter.cur_filter.type
 			&& (ChestShopFilter.cur_filter.type != type))
 		{
 			continue;
 		}
 		for (var item_i in TreasureChest.all_items[type])
 		{
 			var item = TreasureChest.all_items[type][item_i];
 			if (typeof(item) == 'function')
 				continue;

 			if(item.available)
 				continue;

 			if (ChestShopFilter.cur_filter.shop &&
 				(item.shop_id != ChestShopFilter.cur_filter.shop))
 			continue;

 			if (ChestShopFilter.cur_filter.color &&
 					((item.id % 100) != ChestShopFilter.cur_filter.color))
 				continue;

 			if (!similar_group[type][Math.floor(item.id/100)])
 				similar_group[type][Math.floor(item.id/100)] = 1;
 			else
 				similar_group[type][Math.floor(item.id/100)]++;
 			no_items = false;
 		}
 	}
 	if (no_items)
 		return false;	// Keine Items, nix zu tun.
 	<?
 	/**
 	 * Zweite Schleife:
 	 * Jetzt wird gebaut. ;-)
 	 */
 	?>
 	var similar_items_shown = new Array();
 	var no_items = true;
 	for (var type in TreasureChest.all_items)
 	{
 		similar_items_shown[type] = new Array();
 		if (typeof (TreasureChest.all_items[type]) == 'function')
 		{
 			continue;
 		}
 		if (ChestShopFilter.cur_filter.type
 	 		&& (ChestShopFilter.cur_filter.type != type))
 		{
 			continue;
 		}
 		for (var item_i in TreasureChest.all_items[type])
 		{
 			var item = TreasureChest.all_items[type][item_i];
 			if (typeof(item) == 'function')
 				continue;

 			if ((item.amount == 0) && (item.amount_mp == 0))
				continue;

 			if (ChestShopFilter.cur_filter.shop &&
 					(item.shop_id != ChestShopFilter.cur_filter.shop))
 				continue;

 			if (ChestShopFilter.cur_filter.color &&
 					((item.id % 100) != ChestShopFilter.cur_filter.color))
 				continue;

 			if (ChestShopFilter.cur_filter.similar_items == -1) {
 			<?	/**
 				 * Keine gleichen Items anzeigen, also nur ähnlicher Art
 				 * (gruppierte Ansicht)
 				 */ ?>
 				if (similar_items_shown[type][Math.floor(item.id/100)])
 					continue;
 			}
 			else if (ChestShopFilter.cur_filter.similar_items == -2) {
 	 		<?	/**
 	 			 * Keine Gruppierung und keine Filterung
 	 			 */ ?>
 			}
 			else {
 			<?	/**
 				 * Nur gleiche Items anzeigen, also nur ähnlicher Art
 				 * (nicht gruppiert, aber gefiltert)
 				 */ ?>
 			 	if (ChestShopFilter.cur_filter.similar_items !=
 				 									Math.floor(item.id/100))
 			 		continue;
 			}

 			similar_items_shown[type][Math.floor(item.id/100)] = true;
 			TreasureChest.trade_item_scroller.add_item(PureFW.ChestItem,
 	 			[178, 235],
				function(_item, _sim_filter, _related_amount)
 				{
 					return function()
 					{
 						this.set_item(_item);
 						if ((_sim_filter == -1) && (_related_amount > 1)) {
							this.add_event_handler("click",
								function(ev) {
									ChestShopFilter.set_filter(
										'similar_items',
										Math.floor(_item.id/100)
									);
									ChestShopFilter.set_filter(
										'type', _item.type
									);
									ChestShopFilter.set_filter(
										'color', 0
									);
									TreasureChest.history_back.show();
								}
							);
							this.set_sub_title(
								'<a href="javascript: ;">'
									+ '<?= LNG_B_SHOW_RELATED_ITEMS ?>'
								+'</a>'
							);
						}
						else {
							this.add_event_handler("click",
								(function(_instance) {
									return function(ev) {
										TreasureChest.show_details(
											_item, _instance,
		 									TreasureChest.researched_races
		 								);
									}
								})(this)
							);
							this.set_sub_title(
								'<a href="javascript: ;">'
									+ '<?= LNG_B_SHOW_DETAILS ?>'
								+'</a>'
							);
						}
 					}
 				}(item, ChestShopFilter.cur_filter.similar_items,
					similar_group[type][Math.floor(item.id/100)])
 			);
 			no_items = false;
 		}
 	}
 	return !no_items;
}


<?
/**
 * Diese Funktion zeigt die Items an, welche dem Filter ChestShopFilter.cur_filter
 * entsprechen. Für jedes Item wird ein entsprechendes Widget erzeugt und dem
 * TreasureChest.RightContainer zugewiesen.
 */ ?>
TreasureChest.show_items_old = function()
{
	<? /** 1. Alle aktuell angezeigten Items-Container zerstören */ ?>
	var n = TreasureChest.item_containers.length;
	for (var i = 0; i < n; i++)
	{
		TreasureChest.item_containers[i].destroy();
	}

	<? /** 2. Neu aufbauen */ ?>
	/*var items_per_row =
		Math.floor(
			(TreasureChest.RightContainer.width + TreasureChest.item_margin.x) /
			(TreasureChest.item_size.x + TreasureChest.item_margin.x)
		);*/
	var x = 0;
	var y = 10;
	for (var type in TreasureChest.all_items) {
		if (typeof (TreasureChest.all_items[type]) == 'function')
			continue;
		if (ChestShopFilter.cur_filter.type && (ChestShopFilter.cur_filter.type != type))
			continue;
		for (var item_i in TreasureChest.all_items[type]) {
			var item = TreasureChest.all_items[type][item_i];
			if (typeof(item) == 'function')
				continue;
			if ((item.amount == 0) && (item.amount_mp == 0)) {
				continue;
			}
			if (ChestShopFilter.cur_filter.shop &&
					(item.shop_id != ChestShopFilter.cur_filter.shop))
				continue;

			var new_container = new PureFW.ChestItem(
				TreasureChest.RightContainer,
				x, y,
				TreasureChest.item_size.x, TreasureChest.item_size.y);

			new_container.set_name(item.name);
			if (!((typeof(item.shop_id) != 'undefined') && item.shop_id))
				item.shop_id = 1;
			new_container.set_shop();
			new_container.set_item(type, item.id);
			if (TreasureChest.own_chest) {
				new_container.set_amount(
					parseInt(item.amount) + parseInt(item.amount_mp)
				);
			}
			new_container.set_amount_mp(item.amount_mp);
			new_container.set_cost(item.price);
			if (TreasureChest.own_chest) {
				new_container.item_direct_or_not_img.hide();
			}
			new_container.set_bg_img(
				'ui/backgrounds/shop/item_bg_'+(item.shop_id-1)+'.png');

			new_container.add_event_handler("click",
				function(_i, _c_i) {
					return function(ev) {
						TreasureChest.show_details(_i, _c_i,
							TreasureChest.researched_races);
					};
				}(item, new_container)
			);
			TreasureChest.item_containers.push(new_container);
			if (x >= (TreasureChest.RightContainer.width-2*TreasureChest.item_size.x)) {
				x = 0;
				y += TreasureChest.item_size.y + TreasureChest.item_margin.y;
				TreasureChest.RightContainer.set_inner_height(y);
			}
			else
			{
				x += TreasureChest.item_size.x + TreasureChest.item_margin.x;
				TreasureChest.RightContainer.set_inner_height(y + new_container.height);
			}
		}
	}
};

TreasureChest.get_current_offers = function()
{
	var total = 0;
	for (var type in TreasureChest.all_items)
	{
		if (typeof (TreasureChest.all_items[type]) == 'function')
			continue;
		if (ChestShopFilter.cur_filter.type && (ChestShopFilter.cur_filter.type != type))
			continue;
		for (var item_i in TreasureChest.all_items[type])
		{
			var item = TreasureChest.all_items[type][item_i];
			if (typeof(item) == 'function')
				continue;
			total += item.offers.length;
		}
	}
	return total;
}

<? include('item_details.tmpl.inc') ?>
</script>
<? if ($uid == $current_user->get_id()) : ?>
	<? include('details_own_put_on_mp.tmpl.inc') ?>
<? else : ?>
	<? include('details_other_make_offer.tmpl.inc') ?>
<? endif; ?>
<script type="text/javascript">

<? /**
    * Der Destruktor der Seite. Wird aufgerufen, wenn die Seite geschlossen
    * wird (Registern des Events am Ende dieser Datei).
    */ ?>
TreasureChest.destroy = function() {
	TreasureChest.this_window.set_css_class('');
	if (PureFW.Officer.get_current_active_officer())
		PureFW.Officer.get_current_active_officer().show();
	TreasureChest.this_window.remove_event_handler("destroy", TreasureChest.destroy);
	TreasureChest.this_window.remove_event_handler("change", TreasureChest.destroy);
	PureFW.AJAXClientServer.send_request(
		"<?= $_SERVER['PHP_SELF'] ?>?dataX=<?=
			$cryptObj->do_encryption(array("uid" => $uid))?>&unvisit=true"
	);
	destroy_object(TreasureChest);
	TreasureChest = null;
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


<? /* Überschrift */ ?>
if (TreasureChest.own_chest)
{
	TreasureChest.title_container = TreasureChest.this_window.add_widget(
		PureFW.Image,
		30, 10,
		227, 29,
		'ui/elements/texts/<?=$LNG?>/treasure/my_treasure_chest.png'
	);
}
else
{
	TreasureChest.title_container = TreasureChest.this_window.add_widget(
		PureFW.Image,
		30, 10,
		225, 23,
		'ui/elements/texts/<?=$LNG?>/treasure/treasure_chest_of.png'
	);

	TreasureChest.title_nick_cont = TreasureChest.this_window.add_widget(
		PureFW.Container,
		TreasureChest.title_container.position.x
			+ TreasureChest.title_container.width + 5,
		TreasureChest.title_container.position.y - 7,
		200, 30
	);
	TreasureChest.title_nick_cont.add_content(
		MammunUI.beautify_nick(
			'<?= User::get_instance_by_id($uid)->get_nick() ?>'
		)
	);
	TreasureChest.title_nick_cont.set_font_size(1.5);
}

<? /* Filter */ ?>
<? include('templates/treasure_chest/filter.tmpl.inc'); ?>
ChestShopFilter.create_filter_widgets(
	TreasureChest.this_window,
	TreasureChest.title_container.position.x,
	TreasureChest.title_container.position.y
		+ TreasureChest.title_container.height + 20,
	true
);

ChestShopFilter.on_filter_change_functions.push(
	function() {
		TreasureChest.history_back.hide();
		TreasureChest.show_items();
	}
);

<? /* Items */ ?>
TreasureChest.trade_item_scroller = TreasureChest.this_window.add_widget(
	PureFW.BubbleScroller,
	TreasureChest.title_container.position.x + 16, 110,
	TreasureChest.this_window.width - 100, 310,
	4
);

if (!TreasureChest.show_items()) {
	TreasureChest.warning_info = TreasureChest.this_window.add_widget(
		PureFW.Container,
		TreasureChest.trade_item_scroller.position.x
			+ ((TreasureChest.trade_item_scroller.width - 500) >> 1),
		TreasureChest.trade_item_scroller.position.y
			+ ((TreasureChest.trade_item_scroller.height - 200) >> 1),
		500, 200
	);
	//TreasureChest.warning_info.set_content('<?=LNG_ERR_NO_ITEMS_IN_SHOP?>');
	TreasureChest.warning_info.set_css_class('ConfirmationBox');
	TreasureChest.warning_info.set_z_index(2);
}

<? /* Zurück-Button */ ?>
TreasureChest.history_back = TreasureChest.this_window.add_widget(
	PureFW.Image,
	TreasureChest.title_container.position.x,
	TreasureChest.this_window.height - 70,
	105, 32,
	'ui/elements/form/buttons/<?=$LNG?>/wizard/back_shadow.png'
);
TreasureChest.history_back.add_event_handler("click",
	function (ev) {
		ChestShopFilter.set_filter(
			'similar_items', 'history_back'
		);
		ChestShopFilter.set_filter(
			'type', 'history_back'
		);
		ChestShopFilter.set_filter(
			'color', 'history_back'
		);
	}
);
TreasureChest.history_back.hide();

<? /* Abschluss */ ?>
TreasureChest.fit_label = TreasureChest.this_window.add_widget(
	PureFW.Image,
	TreasureChest.this_window.width - 380,
	TreasureChest.this_window.height - 60,
	347, 25,
	'ui/elements/texts/<?=$LNG?>/shop/txt_footnote_on-fits-all-347x25.png'
);

<? /* Zusatzinfo und Funktionen abhängig vom Kistenbesitzer */ ?>
TreasureChest.show_items();
if (TreasureChest.own_chest) {
	TreasureChest.show_outgoing_offers_button
		= TreasureChest.this_window.add_widget
	(
		PureFW.Image,
		ChestShopFilter.group_filter_cb.position.x,
		10,
		150, 30,
		'ui/elements/form/buttons/<?=$LNG?>/treasure_chest/btn_offers_out.png'
	);
	TreasureChest.show_outgoing_offers_button.add_event_handler('click',
		function (ev) {
			TreasureChest.show_all_offers(true)
		}
	);

	TreasureChest.show_incoming_offers_button
		= TreasureChest.this_window.add_widget
	(
		PureFW.Image,
		TreasureChest.show_outgoing_offers_button.position.x
			+ TreasureChest.show_outgoing_offers_button.width + 10,
			TreasureChest.show_outgoing_offers_button.position.y,
		190, 30,
		'ui/elements/form/buttons/<?=$LNG?>/treasure_chest/btn_offers_in.png'
	);
	TreasureChest.show_incoming_offers_button.add_event_handler('click',
		function (ev) {
			TreasureChest.show_all_offers()
		}
	);

<? if ($offer_amount > 0) : ?>
	TreasureChest.offer_amount_circle = TreasureChest.this_window.add_widget(
		PureFW.Container,
		TreasureChest.show_incoming_offers_button.position.x
			+ TreasureChest.show_incoming_offers_button.width - 12,
			TreasureChest.show_incoming_offers_button.position.y - 10,
		25,	25
	);
	TreasureChest.offer_amount_circle.set_bg_img(
		'ui/backgrounds/button_overlay_kringel/red/kringel.png'
	);
	TreasureChest.offer_amount_circle.set_z_index(2);
	TreasureChest.offer_amount_circle.set_text_align('center');
	TreasureChest.offer_amount_circle.set_font_size(0.95);
	TreasureChest.offer_amount_circle.set_font_color('#fff');
	TreasureChest.offer_amount_circle.get_content_node().style.paddingTop='10%';
	TreasureChest.offer_amount_circle.set_content(<?= $offer_amount ?>);
<? endif; ?>

}
else {
	TreasureChest.owner_avatar = TreasureChest.this_window.add_widget(
		PureFW.Avatar.Avatar,
		TreasureChest.this_window.width - 94 - 20,
		5,
		94,	94,
		TreasureChest.avatar_data[
			'<?= User::get_instance_by_id($uid)->get_nick() ?>'
		],
		'<?= User::get_instance_by_id($uid)->get_nick() ?>'
	);
}


<? /* Destruktor registrieren */ ?>
TreasureChest.this_window.add_event_handler("destroy", TreasureChest.destroy);
TreasureChest.this_window.add_event_handler("change", TreasureChest.destroy);

</script>