if (!((typeof(MammunShop) != 'undefined') && MammunShop))
	MammunShop = new Object();

<? /**
    * Erstellt die Shop-Navi. Als erstes Argument muss der Vaterknoten übergeben
    * werden, an den die Navi eingehängt werden soll.
    *
    * Damit man sie auch in der Schatzkiste verwenden
    * kann, kann man als zweites Argument ein Objekt übergeben, dessen Kinder die
    * erstellen Objekte sein sollen. Dieses Objekt muss eine Funktion
    * "set_filter" besitzen, welche dann durch Anklicken der Radiobuttons
    * aufgerufen wird.
    *
    * @param HTMLNode parent_node
    * @param Object o [optional]
    */ ?>
MammunShop.create_navi = function(parent_node, o) {
	if (typeof(o) == 'undefined')
		o = MammunShop;

	o.selection_filter_container = new PureFW.Container
	(
		parent_node,
		2, 5,
		166, 379
	);
	o.selection_filter_container.set_bg_img(
			'ui/backgrounds/shop/filter_bg.png');


	<? /** Qualität- / Shop-Wahl */?>
	o.caption_quality = new PureFW.Image(
		o.selection_filter_container,
		(o.selection_filter_container.width -
			140) >> 1,
		5,
		140, 48,
		'ui/elements/texts/<?=$LNG?>/shop/shops.png'
	);

	<? /** Radiobuttons für die Qualitäts-/Shop-Wahl */ ?>
	o.quality_options = new Array();
	<? /* RBQ 0 */?>
	o.quality_options[0] = new PureFW.RadioLabel(
		o.selection_filter_container,
		o.caption_quality.position.x,
		o.caption_quality.position.y +
			o.caption_quality.height,
		o.caption_quality.width, 16,
		14, 'ui/elements/form/input/radio_toggle_small.png'
	);
	o.quality_options[0].radio_button.check();
	o.quality_options[0].radio_button.add_event_handler("check",
		function() {
			o.set_filter('shop', 0);
		}
	);
	o.quality_options[0].label_container.set_font_weight('bold');
	o.quality_options[0].label_container.set_font_size(1);
	o.quality_options[0].label_container.set_content('Alle anzeigen');

	<? /* RBQ 1 */ ?>
	o.quality_options[1] = new PureFW.RadioLabel(
		o.selection_filter_container,
		o.caption_quality.position.x,
		o.quality_options[0].label_container.position.y +
			o.quality_options[0].label_container.height,
		o.caption_quality.width, 16,
		14,	'ui/elements/form/input/radio_toggle_small.png'
	);
	o.quality_options[1].radio_button.link_with_radio_button(
		o.quality_options[0].radio_button
	);
	o.quality_options[1].radio_button.add_event_handler("check",
		function() {
			o.set_filter('shop', 1);
		}
	);
	o.quality_options[1].label_container.set_font_weight('bold');
	o.quality_options[1].label_container.set_font_size(1);
	o.quality_options[1].label_container.set_content('Schön & Gut');

	<? /* RBQ 2 */ ?>
	o.quality_options[2] = new PureFW.RadioLabel(
		o.selection_filter_container,
		o.caption_quality.position.x,
		o.quality_options[1].label_container.position.y +
			o.quality_options[1].label_container.height,
		o.caption_quality.width, 16,
		14, 'ui/elements/form/input/radio_toggle_small.png'
	);
	o.quality_options[2].radio_button.link_with_radio_button(
		o.quality_options[1].radio_button
	);
	o.quality_options[2].radio_button.add_event_handler("check",
		function() {
			o.set_filter('shop', 2);
		}
	);
	o.quality_options[2].label_container.set_font_weight('bold');
	o.quality_options[2].label_container.set_font_size(1);
	o.quality_options[2].label_container.set_content('Farbe & Mehr');

	<? /* RBQ 3 */ ?>
	o.quality_options[3] = new PureFW.RadioLabel(
		o.selection_filter_container,
		o.caption_quality.position.x,
		o.quality_options[2].label_container.position.y +
			o.quality_options[2].label_container.height,
		o.caption_quality.width, 16,
		14, 'ui/elements/form/input/radio_toggle_small.png'
	);
	o.quality_options[3].radio_button.link_with_radio_button(
		o.quality_options[2].radio_button
	);
	o.quality_options[3].radio_button.add_event_handler("check",
		function() {
			o.set_filter('shop', 3);
		}
	);
	o.quality_options[3].label_container.set_font_weight('bold');
	o.quality_options[3].label_container.set_font_size(1);
	o.quality_options[3].label_container.set_content('Schick & Selten');

	<?/* RBQ 0 Nachtrag (Zirkuläre Verbindung der Radiobuttons) */ ?>
	o.quality_options[0].radio_button.link_with_radio_button(
		o.quality_options[o.quality_options.length-1].radio_button
	);


	<? /** Kategoriewahl **/ ?>
	o.tmp = o.quality_options[o.quality_options.length-1];

	o.caption_categories = new PureFW.Image(
		o.selection_filter_container,
		(o.selection_filter_container.width -
			140) >> 1,
		o.tmp.position.y + o.tmp.height,
		140, 48,
		'ui/elements/texts/<?=$LNG?>/shop/categories.png'
	);
	<? /** Kategorieoptionen */ ?>
	o.categories_options = new Array();
	<? /* RBC 0 */?>
	o.categories_options[0] = new PureFW.RadioLabel(
		o.selection_filter_container,
		o.caption_categories.position.x,
		o.caption_categories.position.y +
			o.caption_categories.height,
		o.caption_categories.width, 16,
		14, 'ui/elements/form/input/radio_toggle_small.png'
	);
	o.categories_options[0].radio_button.check();
	o.categories_options[0].radio_button.add_event_handler("check",
		function() {
			o.set_filter('type', 0);
		}
	);
	o.categories_options[0].label_container.set_font_weight('bold');
	o.categories_options[0].label_container.set_font_size(1);
	o.categories_options[0].label_container.set_content('Alle anzeigen');

	<? /* RBC 1 */ ?>
	o.categories_options[1] = new PureFW.RadioLabel(
		o.selection_filter_container,
		o.caption_categories.position.x,
		o.categories_options[0].label_container.position.y +
			o.categories_options[0].label_container.height,
		o.caption_categories.width, 16,
		14,	'ui/elements/form/input/radio_toggle_small.png'
	);
	o.categories_options[1].radio_button.link_with_radio_button(
		o.categories_options[0].radio_button
	);
	o.categories_options[1].radio_button.add_event_handler("check",
		function() {
			o.set_filter('type', <?=ItemConstants::TYPE_HAIR_HAT?>);
		}
	);
	o.categories_options[1].label_container.set_font_weight('bold');
	o.categories_options[1].label_container.set_font_size(1);
	o.categories_options[1].label_container.set_content('Auf\'m Kopf');

	<? /* RBC 2 */ ?>
	o.categories_options[2] = new PureFW.RadioLabel(
		o.selection_filter_container,
		o.caption_categories.position.x,
		o.categories_options[1].label_container.position.y +
			o.categories_options[1].label_container.height,
		o.caption_categories.width, 16,
		14, 'ui/elements/form/input/radio_toggle_small.png'
	);
	o.categories_options[2].radio_button.link_with_radio_button(
		o.categories_options[1].radio_button
	);
	o.categories_options[2].radio_button.add_event_handler("check",
		function() {
			o.set_filter('type', <?=ItemConstants::TYPE_OUTERWEAR?>);
		}
	);
	o.categories_options[2].label_container.set_font_weight('bold');
	o.categories_options[2].label_container.set_font_size(1);
	o.categories_options[2].label_container.set_content('Um den Bauch');

	<? /* RBC 3 */ ?>
	o.categories_options[3] = new PureFW.RadioLabel(
		o.selection_filter_container,
		o.caption_categories.position.x,
		o.categories_options[2].label_container.position.y +
			o.categories_options[2].label_container.height,
		o.caption_categories.width, 16,
		14, 'ui/elements/form/input/radio_toggle_small.png'
	);
	o.categories_options[3].radio_button.link_with_radio_button(
		o.categories_options[2].radio_button
	);
	o.categories_options[3].radio_button.add_event_handler("check",
		function() {
			o.set_filter('type', <?=ItemConstants::TYPE_TROUSERS?>);
		}
	);
	o.categories_options[3].label_container.set_font_weight('bold');
	o.categories_options[3].label_container.set_font_size(1);
	o.categories_options[3].label_container.set_content('Für die Beine');

	<?/* RBC 0 Nachtrag (Zirkuläre Verbindung der Radiobuttons) */ ?>
	o.categories_options[0].radio_button.link_with_radio_button(
		o.categories_options[o.categories_options.length-1].radio_button
	);
};