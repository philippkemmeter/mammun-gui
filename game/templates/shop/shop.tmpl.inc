<script type="text/javascript">

if (PureFW.Officer.get_current_active_officer())
	PureFW.Officer.get_current_active_officer().hide();

MammunShop = new Object();
<? /* Das aktuelle Fenster, in dem diese Seite dargestellt wird */ ?>
MammunShop.this_window = PureFW.ContainerAJAX.instances[<?=$this_win_id?>];
MammunShop.this_window.set_css_class('ConfirmationBox');

<? /* Enthält alle Items, die man kaufen kann. */ ?>
MammunShop.all_items = <?= $all_items_json ?>;

<? /* Alle Waffen/Rassen, die der Spieler erforscht hat */ ?>
MammunShop.researched_races = new Array();
<? foreach ($researched_races as $i => $race) : ?>
<? $pic_core = substr(ScienceConstants::$PICS[$race], 0,
							strrpos(ScienceConstants::$PICS[$race],'.')); ?>
MammunShop.researched_races.push('<?=$pic_core?>');
 <? endforeach; ?>


<? /* Enthält alle Container, die ein Item in der Liste darstellen und auch
      gerade sichtbar sind. Siehe MammunShop.show_items für weiteres. */ ?>
MammunShop.item_containers = new Array();

<?
/**
 * This is a list of all special offers currently on the shop's "window." See
 * SpecialShopOffer.js for Widget details.
 */
?>
MammunShop.item_preview_containers = new Array();

<? /* Wie groß ein jedes Item dargestellt werden soll. Insgesamt mit allen
      eingeblendeten Infos */ ?>
MammunShop.item_size = new PureFW.Point(183,200);
<? /* Wie viel Platz zwischen den Items sein soll (in x und y) */ ?>
MammunShop.item_margin = new PureFW.Point(0,0);

<? /* Dieses Container enthält die Details des ausgewählten Widgets */ ?>
MammunShop.details_container = null;

MammunShop.show_preview_items = function()
{
	<? /** 1. Alle aktuell angezeigten Items-Container zerstören */ ?>
	var n = MammunShop.item_containers.length;
	for (var i = 0; i < n; i++) {
		MammunShop.item_containers[i].destroy();
	}
	var n = MammunShop.item_preview_containers.length;
	for (var i = 0; i < n; i++) {
		MammunShop.item_preview_containers[i].destroy();
	}

	var type = 1;
	var item = {id:1, name:"Super-Pants!", cost:21, pretzels:21};

	var x = 40;
	var y = 70;
	var spacing = 25;


	for(var i = 0; i < 4; i++)
	{
		MammunShop.item_preview_containers[i] =
			MammunShop.this_window.add_widget(PureFW.SpecialShopOffer, x, y);
		MammunShop.item_preview_containers[i].set_name(item.name);
		MammunShop.item_preview_containers[i].set_desc(
				"These pants are hardcore! You gotta buy em!");
		MammunShop.item_preview_containers[i].set_pretzels(item.pretzels);
		MammunShop.item_preview_containers[i].set_price(15);
		MammunShop.item_preview_containers[i].set_user(
				'ui/backgrounds/avatars/avatar-104x104.png', 'Jack');
//		MammunShop.item_preview_containers[i].set_timeout(-12);
		MammunShop.item_preview_containers[i].set_item(type, item.id);
		MammunShop.item_preview_containers[i].set_buy_link(
				'<a href="javascript:">'
				+'<?=LNG_SHOW_ITEM_DETAILS?>'
				+'</a>');
		MammunShop.item_preview_containers[i].buy_link.add_event_handler('click',
				function(_i)
				{
					return function(ev)
					{
						TreasureChest.show_details(_i, null,
							MammunShop.researched_races);
					}
				}(item));

		x = x + MammunShop.item_preview_containers[i].width + spacing;
	}

	MammunShop.title_container = MammunShop.this_window.add_widget(
			PureFW.Image,
			65, 15,
			600, 28,
			'ui/elements/texts/de/shop/headline.png');

	MammunShop.enter_shop_container = MammunShop.this_window.add_widget(
			PureFW.Container,
			60, MammunShop.this_window.height - 60,
			0,0);
		MammunShop.enter_shop_container.set_font_color('#303030');
		MammunShop.enter_shop_container.set_content(
				'<?=LNG_MORE_ITEMS_IN_THE?>'
				+'<a href="javascript: MammunShop.enter_shop();">'
				+'<b> <?=LNG_MARKET?></b></a>');

	MammunShop.catalogue_container = MammunShop.this_window.add_widget(
			PureFW.Container,
			MammunShop.this_window.width-220, MammunShop.this_window.height-60,
			0,0);
		MammunShop.catalogue_container.set_content(
				'<a href="javascript: MammunShop.open_catalogue();">'
				+'<b><?=LNG_SEE_CATALOGUE?></b></a>');
};

MammunShop.enter_shop = function()
{
	var n = MammunShop.item_preview_containers.length;
	for (var i = 0; i < n; i++) {
		MammunShop.item_preview_containers[i].destroy();
	}
	MammunShop.title_container.destroy();
	MammunShop.enter_shop_container.destroy();
	MammunShop.catalogue_container.destroy();

	MammunShop.create_navi(MammunShop.LeftContainer);

	MammunShop.show_items();
};

MammunShop.limited_items_in_catalog = function() {
	for (var type in MammunShop.all_items) {
		if (typeof (MammunShop.all_items[type]) == 'function')
			continue;

		for (var item_i in MammunShop.all_items[type]) {
			var item = MammunShop.all_items[type][item_i];
			if (typeof(item) == 'function')
				continue;

			if (item.available) {
				if (item.av_until > 0) {
					return true;
				}
			}
		}
	}
	return false;
}


MammunShop.open_catalog = function() {
	var catalog = new PureFW.Items.ShopCatalog(
		document.body,
		(MammunUI.reference_width - 964) >> 1,
		(MammunUI.reference_height - 608) >> 1,
		964, 608,
		true
	);
	catalog.add_event_handler("buy",
		function (ev) {
			MammunShop.buy_item(ev.detail);
		}
	);
	PureFW.Items.ShopCatalog.LNG_TOOLTIP_CATALOG_TIME =
		'<?= LNG_TOOLTIP_CATALOG_TIME ?>'
	PureFW.Items.ShopCatalog.LNG_SOLD_OUT = '<?= LNG_SOLD_OUT ?>';
	PureFW.Items.ShopCatalog.LNG = '<?= $LNG ?>';

	var items_to_add = new Array();
	for (var type in MammunShop.all_items)
	{
		if (typeof (MammunShop.all_items[type]) == 'function')
			continue;

		for (var item_i in MammunShop.all_items[type])
		{
			var item = MammunShop.all_items[type][item_i];
			if (typeof(item) == 'function')
				continue;

			if (item.available) {
<?				/**
				 * Zunächst nur die Items hinzufügen, die limitiert sind
				 */ ?>
				if (item.av_until > 0) {
					catalog.add_item(item);
				}
<?				/**
				 * Die anderen merken und später ans Ende dazuheften
				 */ ?>
				else {
					items_to_add.push(item);
				}
			}
		}
	}
<?	/**
	 * Hier kommen die unlimitierten Items :)
	 */ ?>
	var n = items_to_add.length;
	for (var i = 0; i < n; i++)
		catalog.add_item(items_to_add[i]);
}

<?
/**
 * Zeigt alle Items des Marktplatzes an.
 *
 * @return bool ob welche angezeigt werden
 */
?>
MammunShop.show_items = function()
{
	MammunShop.trade_item_scroller.remove_all_items();

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
	for (var type in MammunShop.all_items)
	{
		if (typeof (MammunShop.all_items[type]) == 'function')
			continue;

		similar_group[type] = new Array();
		if (ChestShopFilter.cur_filter.type
			&& (ChestShopFilter.cur_filter.type != type))
		{
			continue;
		}
		for (var item_i in MammunShop.all_items[type])
		{
			var item = MammunShop.all_items[type][item_i];
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
	for (var type in MammunShop.all_items)
	{
		if (typeof (MammunShop.all_items[type]) == 'function')
			continue;

		similar_items_shown[type] = new Array();
		if (ChestShopFilter.cur_filter.type
			&& (ChestShopFilter.cur_filter.type != type))
		{
			continue;
		}
		for (var item_i in MammunShop.all_items[type])
		{
			var item = MammunShop.all_items[type][item_i];
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

			if (ChestShopFilter.cur_filter.similar_items < 0) {
			<?	/**
				 * Keine gleichen Items anzeigen, also keine ähnlicher Art
				 */ ?>
				if (similar_items_shown[type][Math.floor(item.id/100)])
					continue;
			}
			else {
			<?	/**
				 * Nur gleiche Items anzeigen, also nur ähnlicher Art
				 */ ?>
			 	if (ChestShopFilter.cur_filter.similar_items !=
				 									Math.floor(item.id/100))
			 		continue;
			}

			similar_items_shown[type][Math.floor(item.id/100)] = true;
			MammunShop.trade_item_scroller.add_item(PureFW.TradeItem, [178, 250],
				function(_item, _sim_filter, _related_amount)
				{
					return function()
					{
						<? /** Try-Catch, weil es u.U. ein falsche Nick sein kann
							* oder ein Avatar nicht initialisiert ist. Dann
							* einfach gar nix anzeigen
							*/ ?>
						try {
							this.set_avatar(
								PauseMenu.avatars[_item.user_nick].replace('SIZE', 44),
								_item.user_nick
							);
							this.set_text(
								'<a href="javascript: UIButtons.open_window('
									+ '\'profile.php?nick=' + _item.user_nick + '\''
									+ ');">'+MammunUI.beautify_nick(_item.user_nick)
								+'</a><br/><?=LNG_HAS_TREASURE?>'
							);
						}
						catch(e) {}
						this.set_item(_item);
						if ((_sim_filter < 0) && (_related_amount > 1)) {
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
									MammunShop.history_back.show();
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
								function(ev) {
									TreasureChest.show_details(_item, null,
										MammunShop.researched_races);
								}
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

<? include ('templates/treasure_chest/item_details.tmpl.inc') ?>

MammunShop.select_race = function(which_unit, which_icon, item)
{
	if(!MammunShop.item_details)
		return;
	MammunShop.unit_selected = MammunShop.item_details.units[which_unit];
	chosen_unit = MammunShop.unit_selected;

	var n = MammunShop.item_details.race_icons.length;
	for (var i = 0; i < n; i++)
	{
		MammunShop.item_details.race_icons[i].set_width(43);
		MammunShop.item_details.race_icons[i].set_height(59);
		MammunShop.item_details.race_icons[i].set_y(15);
	}
	MammunShop.item_details.race_icons[which_icon].set_width(55);
	MammunShop.item_details.race_icons[which_icon].set_height(76);
	MammunShop.item_details.race_icons[which_icon].set_y(7);

	var img_pic_path = 'ui/units/training/'
		+MammunShop.item_details.units[which_unit].type+'/'
		+MammunShop.item_details.units[which_unit].type+'_small_0_';
	for (var i = 0; i < item.type-1; i++) {
		img_pic_path += '0_';
	}
	img_pic_path += item.id;
	for (var i = item.type; i < 6; i++) {
		img_pic_path += '_0';
	}
	img_pic_path += '_0.png';
	MammunShop.item_details.unit_preview_pic.set_pic_url(img_pic_path);
};

MammunShop.buy_item = function(item) {
	post_data = "id="+item.id+"&type="+item.type+"&buy=true&amount="+item.amount
	PureFW.AJAXClientServer.send_request("<?= $_SERVER['PHP_SELF'] ?>",
			MammunShop.buy_item_response, post_data);
};

MammunShop.buy_item_response = function(response_arr) {
	<? /**
		* Fall "Super": Es hat geklappt.
		*/ ?>
	if (response_arr[0] == "1") {
		MammunShop.allright_cont = MammunUI.show_warning(
			490, 220,
			response_arr[1],
			PureFW.ConfirmationBox.YES
		);
		MammunShop.allright_cont.add_event_handler(
			"destroy",
			function() {
				try {MammunShop.allright_cont_text.destroy(); } catch(e) {};
			}
		);
		if (ui_klunker_gainer)
			ui_klunker_gainer.set_current_amount(parseInt(response_arr[2]));
		if (response_arr[3]) <? /* nur setzen, wenn > 0, sonst wurde keine
								Korrektur vom Server geschickt */ ?>
			ui_items_gainer.set_current_amount(parseInt(response_arr[3]));
		ui_refresh();
	}
	<? /**
		* Fehlerfall: Irgendwas ist schiefgegangen. Fehlermeldung anbei.
		*/ ?>
	else if (response_arr[0] == "0") {
		MammunShop.allright_cont = MammunUI.show_warning(
			490, 220,
			response_arr[1],
			PureFW.ConfirmationBox.YES,
			true
		);
		MammunShop.allright_cont.add_event_handler(
			"destroy",
			function() {
				try {MammunShop.allright_cont_text.destroy(); } catch(e) {};
			}
		);
	}
	<? /**
		* Brutaler Fehler: Irgendwas ist schiefgegangen, aber Server weiß nicht
		* was (Unexpected Error). Standardfehlermeldung senden.
		*/ ?>
	else {
		MammunShop.allright_cont = MammunUI.show_warning(
			490, 220,
			'<?=LNG_ITEM_BOUGHT_ERROR?>',
			PureFW.ConfirmationBox.YES,
			true
		);
		MammunShop.allright_cont.add_event_handler(
			"destroy",
			function() {
				try {MammunShop.allright_cont_text.destroy(); } catch(e) {};
			}
		);
		throw Error(response_arr);
	}
};

<? /**
    * Der Destruktor der Seite. Wird aufgerufen, wenn die Seite geschlossen
    * wird (Registern des Events am Ende dieser Datei).
    */ ?>
MammunShop.destroy = function() {
	MammunShop.this_window.set_css_class('');
	MammunShop.this_window.remove_event_handler("destroy", MammunShop.destroy);
	MammunShop.this_window.remove_event_handler("change", MammunShop.destroy);
	for (var x in MammunShop) {
		<?/* Nicht das aktuelle Fenster zerstören. Das zerstört sich entweder
		   * selbst, oder es möchte nicht zerstört werden - ganz einfach :)
		   * Wir haben hier nicht die Berechtigung dort einzugreifen!*/?>
		if (x == 'this_window')
			continue;
		try {
			MammunShop[x].destroy();
		}
		catch (e) {
			delete MammunShop[x];
		}
	}
	MammunShop = null;
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
MammunShop.title_container = MammunShop.this_window.add_widget(
	PureFW.Image,
	30, 10,
	587, 29,
	'ui/elements/texts/<?=$LNG?>/shop/headline_basar.png');

<? /* Filter */ ?>
<? include('templates/treasure_chest/filter.tmpl.inc'); ?>
ChestShopFilter.create_filter_widgets(
	MammunShop.this_window,
	MammunShop.title_container.position.x,
	MammunShop.title_container.position.y
		+ MammunShop.title_container.height + 40
);
ChestShopFilter.on_filter_change_functions.push(
	function() {
		MammunShop.history_back.hide();
		MammunShop.show_items();
	}
);

<? /* Link zum Katalog */ ?>
MammunShop.catalog_icon = MammunShop.this_window.add_widget(
	PureFW.Image,
	MammunShop.this_window.width - 122 - 20, 0,
	122, 80,
	'ui/icons/labels/shop/catalog.png'
);
MammunShop.catalog_icon.add_event_handler("click",
	function(ev) {
		MammunShop.open_catalog();
	}
);
MammunShop.catalog_icon.set_tooltip('<?= LNG_TOOLTIP_OPEN_CATALOG ?>');

<? /* Items */ ?>
MammunShop.trade_item_scroller = MammunShop.this_window.add_widget(
	PureFW.BubbleScroller,
	MammunShop.title_container.position.x + 16, 110,
	MammunShop.this_window.width - 100, 320,
	4
);

if (!MammunShop.show_items()) {
	MammunShop.warning_info = MammunShop.this_window.add_widget(
		PureFW.Container,
		MammunShop.trade_item_scroller.position.x
			+ ((MammunShop.trade_item_scroller.width - 500) >> 1),
		MammunShop.trade_item_scroller.position.y
			+ ((MammunShop.trade_item_scroller.height - 200) >> 1),
		500, 200
	);
	MammunShop.warning_info.set_content('<?=LNG_ERR_NO_ITEMS_IN_SHOP?>');
	MammunShop.warning_info.set_css_class('ConfirmationBox');
	MammunShop.warning_info.set_z_index(2);
}
<? /* Katalog automatisch aufpoppen lassen, wenn was limitiertes im Katalog ist */ ?>
if (MammunShop.limited_items_in_catalog())
	MammunShop.open_catalog();
else if (<?= ($open_catalog) ? 'true' : 'false' ?>)
	MammunShop.open_catalog();

<? /* Zurück-Button */ ?>
MammunShop.history_back = MammunShop.this_window.add_widget(
	PureFW.Image,
	MammunShop.title_container.position.x, MammunShop.this_window.height - 70,
	93, 30,
	'ui/elements/form/buttons/<?=$LNG?>/wizard/back.png'
);
MammunShop.history_back.add_event_handler("click",
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
MammunShop.history_back.hide();

<? /* Abschluss */ ?>
MammunShop.fit_label = MammunShop.this_window.add_widget(
	PureFW.Image,
	MammunShop.this_window.width - 380,
	MammunShop.this_window.height - 60,
	347, 25,
	'ui/elements/texts/<?=$LNG?>/shop/txt_footnote_on-fits-all-347x25.png'
);

<? /* Destruktor registrieren */ ?>
MammunShop.this_window.add_event_handler("destroy",
	function(ev) {
		if (PureFW.Officer.get_current_active_officer())
			PureFW.Officer.get_current_active_officer().show();
	}
);
MammunShop.this_window.add_event_handler("destroy", MammunShop.destroy);
MammunShop.this_window.add_event_handler("change", MammunShop.destroy);
</script>