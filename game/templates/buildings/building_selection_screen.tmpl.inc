BuildingSelection = new Object();
<?/**
   * Das ist der Container oben Links, welcher die Häuserwahl enthält
   **/?>
BuildingSelection.buildings_container = Buildings.this_window.add_widget(
	PureFW.Container,
	210, 52,
	248, 91
);
BuildingSelection.buildings_container.set_bg_img(
	'ui/backgrounds/unitBgs/weapon_choose.png'
);

<? /**
    * Diese Scrollbox ist innerhalb des Gebäudewahl-Containers
    * (buildings_container) und enthält die wählbaren Gebäude. Diese werden
    * später hierhinein eingefügt.
    */ ?>

BuildingSelection.buildings_choose_scroll =
	BuildingSelection.buildings_container.add_widget
(
	PureFW.ScrollContainer,
	10, 0,
	BuildingSelection.buildings_container.width-20,
	BuildingSelection.buildings_container.height-10
);

<? /**
    * Der picture_container beinhaltet die Vorschau des Gebäudes, das man gerade
    * über die Gebäudewahl gewählt hat.
    */ ?>
BuildingSelection.picture_container = Buildings.this_window.add_widget(
	PureFW.Container,
	215, 145, 240, 274
);
BuildingSelection.picture_container.set_bg_img(
	'ui/backgrounds/unitBgs/chosen_unit.png'
);

<? /**
    * Der Preview-Container enthält in Wirklichkeit das Vorschaubild. Da der
    * Picture-Container bereits ein Hintergrundbild besitzt, müssen zwei Bilder
    * übereinander gelegt werden => zwei Container.
    */ ?>
BuildingSelection.preview_container
	= BuildingSelection.picture_container.add_widget
(
	PureFW.Container,
	0,
	0,
	BuildingSelection.picture_container.width,
	BuildingSelection.picture_container.height
);

BuildingSelection.amount_available
	= Buildings.this_window.add_widget
	(
	PureFW.QuantityDisplay,
	425,
	130,
	48,48
	);

BuildingSelection.preview_container.set_bg_img(
	'/ui/elements/building_preview_boxes/small_shadow.png'
);


<?/* Farbwahl */ ?>
BuildingSelection.indivi_container = Buildings.this_window.add_widget(
	PureFW.Container,
	BuildingSelection.picture_container.position.x
		+ BuildingSelection.picture_container.width + 20,
	BuildingSelection.picture_container.position.y,
	260,
	BuildingSelection.picture_container.height
);

BuildingSelection.paint_headline
	= BuildingSelection.indivi_container.add_widget
(
	PureFW.Image,
	0, 0,
	234, 49,
	'ui/elements/texts/<?= $LNG ?>/buildings/paint_it_as_you_like.png'
);

BuildingSelection.sub_title = BuildingSelection.indivi_container.add_widget(
	PureFW.Container,
	0, BuildingSelection.paint_headline.position.y
		+ BuildingSelection.paint_headline.height + 20,
	0, 30
);
BuildingSelection.sub_title.set_content(
	'<?= LNG_PAINTING_COSTS_KKR ?>'.replace('[KLUNKER]',
		Buildings.painting_costs)
);
BuildingSelection.sub_title.set_css_class('Window_bright');

BuildingSelection.kkr_symbol = BuildingSelection.indivi_container.add_widget(
	PureFW.Image,
	BuildingSelection.sub_title.position.x +
		(BuildingSelection.sub_title.get_width() /
			BuildingSelection.sub_title.scale_factor) + 3,
	BuildingSelection.sub_title.position.y - 2,
	23, 21,
	'ui/icons/labels/resources/21/klunker.png'
);

BuildingSelection.color_icons = new Array();
<?
$colors = array (
	IBuildings::COLOR_MAIN_RED		=>	'red',
	IBuildings::COLOR_MAIN_BLUE		=>	'blue',
	IBuildings::COLOR_MAIN_YELLOW	=>	'yellow',
	IBuildings::COLOR_MAIN_GREEN	=>	'green',
	IBuildings::COLOR_MAIN_WHITE	=>	'white',
	IBuildings::COLOR_MAIN_PURPLE	=>	'purple',
	IBuildings::COLOR_MAIN_ORANGE	=>	'orange',
	IBuildings::COLOR_MAIN_PINK		=>	'pink'
);
$i = 0;
$j = 0;
foreach ($colors as $code => $string) :
?>

BuildingSelection.tmp_icon = BuildingSelection.indivi_container.add_widget(
	PureFW.Image,
	<?= $i ?>*56,
	BuildingSelection.sub_title.position.y
		+ BuildingSelection.sub_title.height
		+ <?= $j ?>*56,
	56, 56,
	'ui/icons/labels/buildings/color/<?=$string?>.png'
);
BuildingSelection.tmp_icon.add_event_handler("click",
	function(ev) {
		Buildings.select_color(<?= $code ?>);
	}
);

BuildingSelection.color_icons.push(
	BuildingSelection.tmp_icon
);
<?
$i++;
if ($i >= 4) {
	$i = 0;
	$j++;
}
endforeach;
?>

BuildingSelection.btn_no_color = BuildingSelection.indivi_container.add_widget(
	PureFW.Image,
	0, BuildingSelection.indivi_container.height - 30,
	178, 30,
	'ui/elements/form/buttons/<?=$LNG?>/buildings/btn_no_color.png'
);
BuildingSelection.btn_no_color.add_event_handler("click",
	function(ev) {
		Buildings.select_color(0);
	}
);

BuildingSelection.indivi_container.hide();

<?/* Info Section */?>

BuildingSelection.information_container = Buildings.this_window.add_widget(
	PureFW.Container,
	750, 60,
	192, 400
);
BuildingSelection.information_container.set_font_color('black'),
BuildingSelection.information_container.set_font_size(2);
BuildingSelection.information_container.set_font_weight(700);

	BuildingSelection.grow_label
		= BuildingSelection.information_container.add_widget
	(
		PureFW.Label,
		0,0,0,0,'----'
	);
	BuildingSelection.grow_label.set_pic(
		'ui/icons/labels/resources/44/wecker.png', 44,44
	);
	BuildingSelection.grow_label.set_positioning('relative');
	BuildingSelection.grow_label.set_title('<?= LNG_TIME_NEEDED ?>');

	BuildingSelection.cost_label
		= BuildingSelection.information_container.add_widget
	(
		PureFW.Label,
		0,0,0,0, '----'
	);
	BuildingSelection.cost_label.set_title('<?=LNG_BUILDING_COST?>');
	BuildingSelection.cost_label.set_pic(
		'ui/icons/labels/resources/44/wood.png', 44,44
	);
	BuildingSelection.cost_label.set_positioning('relative');

	BuildingSelection.kkr_label
		= BuildingSelection.information_container.add_widget
	(
		PureFW.Label,
		0,0,0,0,"----"
	);
	BuildingSelection.kkr_label.set_pic(
		'ui/icons/labels/resources/44/klunker.png', 44,44
	);
	BuildingSelection.kkr_label.set_positioning('relative');
	BuildingSelection.kkr_label.set_title('<?= LNG_KLUNKER_PER_BUILDING ?>');

	BuildingSelection.unit_limit_label
		= BuildingSelection.information_container.add_widget
	(
		PureFW.Label,
		0,0,0,0,"----"
	);
	BuildingSelection.unit_limit_label.set_pic(
		'ui/icons/labels/resources/44/unit_limit.png', 44,44
	);
	BuildingSelection.unit_limit_label.set_positioning('relative');
	BuildingSelection.unit_limit_label.set_title('<?= LNG_UNIT_LIMIT_INCREASE ?>');

<?/* Buttons Section */?>
BuildingSelection.back_next_button_container = new PureFW.Container(
	BuildingSelection.information_container, 0, 340, 192, 30
);
<? /**
    * Der Weiter-Button
    */ ?>
BuildingSelection.next_button_container
	= BuildingSelection.back_next_button_container.add_widget
(
	PureFW.Container,
	90, 0, 93, 30
);
<? if ($change_mode) : ?>
BuildingSelection.next_button_container.set_bg_img(
	'ui/elements/form/buttons/<?=$LNG?>/wizard/finish.png'
);
BuildingSelection.next_button_container.add_event_handler(
	"click",
	function (ev) {
		Buildings.change_color();
	}
);
<? else : ?>
BuildingSelection.next_button_container.set_bg_img(
	'ui/elements/form/buttons/<?=$LNG?>/wizard/next.png'
);
BuildingSelection.next_button_container.add_event_handler(
	"click",
	function (ev) {
		Buildings.start_building();
	}
);
<? endif; ?>
BuildingSelection.next_button_container.hide();

BuildingSelection.destroy = function()
{
	destroy_object(BuildingSelection);
	BuildingSelection = null;
};