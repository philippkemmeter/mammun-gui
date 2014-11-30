UnitWeaponScreen = new Object();

<? /**
    * Das ist die oben links, welche die Waffenwahl enthält
    */ ?>
UnitWeaponScreen.weapon_container = new PureFW.Container(
	UnitProduction.this_window,
	210, 52,
	248, 91
);
UnitWeaponScreen.weapon_container.set_bg_img(
	'ui/backgrounds/unitBgs/weapon_choose.png'
);
<? /**
    * Diese Scrollbox ist innerhalb des Waffelwahl-Containers (weapon_container)
    * und enthält die wählbaren Waffen. Diese werden später hierhinein
    * eingefügt.
    */ ?>
UnitWeaponScreen.weapon_choose_scroll = new PureFW.ScrollContainer(
	UnitWeaponScreen.weapon_container,
	10, 0,
	UnitWeaponScreen.weapon_container.width-20,
	UnitWeaponScreen.weapon_container.height-10
);

<? /**
    * Der picture_container beinhaltet die Vorschau der Einheit, die man gerade
    * über die Waffe gewählt hat.
    */ ?>
UnitWeaponScreen.picture_container = new PureFW.Container(
	UnitProduction.this_window,
	215, 145, 240, 274
);
UnitWeaponScreen.picture_container.set_bg_img(
	'ui/backgrounds/unitBgs/chosen_unit.png'
);
<? /**
    * Der Unit-Container enthält in Wirklichkeit das Vorschaubild. Da der
    * Picture-Container bereits ein Hintergrundbild besitzt, müssen zwei Bilder
    * übereinander gelegt werden => zwei Container.
    */ ?>
UnitWeaponScreen.unit_container = new PureFW.Container(
	UnitWeaponScreen.picture_container,
	0,
	0,
	240, 274
);
UnitWeaponScreen.unit_container.set_bg_img(
	'/ui/elements/unit_preview_boxes/small_shadow.png'
);

<? /**
    * Dieser Container enthält Namen- und Geschlechtwahl.
    */ ?>
UnitWeaponScreen.name_gender_container = new PureFW.Container(
	UnitProduction.this_window,
	UnitWeaponScreen.weapon_container.position.x+
		UnitWeaponScreen.weapon_container.width+7,
	UnitWeaponScreen.weapon_container.position.y,
	279,
	91
);
UnitWeaponScreen.name_gender_container.set_bg_img(
	'ui/backgrounds/unitBgs/name_gender.png'
);
<? /**
    * Hierüber kann der Name eingegeben werden. Ist innerhalb des
    * name_gender_containers
    */ ?>
UnitWeaponScreen.name_title_container = new PureFW.Container(
	UnitWeaponScreen.name_gender_container,
	20, 5, 233, 20
);
UnitWeaponScreen.name_title_container.set_content('<?= LNG_ENTER_NAME ?>:');

UnitWeaponScreen.name_input_container = new PureFW.Container(
	UnitWeaponScreen.name_gender_container,
	15,
	30,
	233,
	25
);
UnitWeaponScreen.name_input_container.set_bg_img(
	'ui/elements/form/input/input_text_233.png'
);
UnitWeaponScreen.name_input_container.set_content(
	'<form name="UnitProduction_form" \
		action="javascript:UnitProduction.keyup(event);">\
	<input type="text" name="chosen_name" \
	 style="width:100%; height: 100%;\
	 background:url(../pix/pattern/spacer.gif) 0 0 no-repeat transparent;\
	 font-size: 0.9em;\
	 border: none; position:absolute; top:0;left:0;"\
	 onkeydown="javascript: UnitProduction.keyup(event)"\
	 onkeyup="javascript: UnitProduction.keyup(event)"/></form>'
);

<?/* Geschlechterwahl noch nicht verfügbar, da keine Bilder vorhanden
UnitWeaponScreen.gender_container = new PureFW.Container(
	UnitWeaponScreen.name_gender_container.get_content_node(), 10, 50, 120, 20);
UnitWeaponScreen.gender_container.set_content("Gender:");
UnitWeaponScreen.female_pic_container = new PureFW.Container(
	UnitWeaponScreen.name_gender_container.get_content_node(), 180, 50, 20, 20);
UnitWeaponScreen.female_pic_container.get_node().style.background = 'pink';
UnitWeaponScreen.male_pic_container = new PureFW.Container(
	UnitWeaponScreen.name_gender_container.get_content_node(), 210, 50, 20, 20);
UnitWeaponScreen.male_pic_container.get_node().style.background = 'blue';*/?>

<? /**
    * Das ist der große Container neben dem Vorschau-Container. Er enthält
    * alle Fähigkeiten, die ausgewählt werden können für die gewählte Waffe.
    */ ?>
UnitWeaponScreen.abilities_large_container = new PureFW.Container(
	UnitProduction.this_window,
	UnitWeaponScreen.picture_container.position.x+
		UnitWeaponScreen.picture_container.width+9,
	UnitWeaponScreen.picture_container.position.y,
	279,
	278
);
UnitWeaponScreen.abilities_large_container.set_bg_img(
	'../pix/ui/backgrounds/unitBgs/abilities.png'
);
<? /**
    * Überschrift innerhalb des Ability-Containers
    */ ?>
UnitWeaponScreen.abilities_title_container = new PureFW.Container(
	UnitWeaponScreen.abilities_large_container.get_content_node(),
	20, 5, 120, 20);
UnitWeaponScreen.abilities_title_container.set_content('<?= LNG_ABILITIES ?>:');
<? /**
    * Scroll-Container zum durchsuchen der Abilities.
    */ ?>
UnitWeaponScreen.abilities_container = new PureFW.ScrollContainer(
	UnitWeaponScreen.abilities_large_container.get_content_node(),
	10, 30,
	UnitWeaponScreen.abilities_large_container.width-20,
	UnitWeaponScreen.abilities_large_container.height-40
);


<? /**
    * Der Zurück-Button
    */ ?>

UnitWeaponScreen.back_button_container = new PureFW.Container(
	UnitSideInfo.back_next_button_container,
	0, 0, 93, 30
);
UnitWeaponScreen.back_button_container.set_bg_img(
	'ui/elements/form/buttons/<?=$LNG?>/wizard/back.png'
);
UnitWeaponScreen.back_button_container.add_event_handler(
	"click",
	function (ev) {
		UnitProduction.show_first_page();
	}
);

<? /**
    * Der Weiter-Button
    */ ?>
UnitWeaponScreen.next_button_container = new PureFW.Container(
	UnitSideInfo.back_next_button_container,
	96, 0, 93, 30
);
UnitWeaponScreen.next_button_container.set_bg_img(
	'ui/elements/form/buttons/<?=$LNG?>/wizard/next.png'
);
UnitWeaponScreen.next_button_container.add_event_handler(
	"click",
	function (ev) {
		UnitProduction.chosen_name =
			document.forms.UnitProduction_form.chosen_name.value;
		UnitProduction.choose_inidiv_dialog();
	}
);
UnitWeaponScreen.next_button_container.get_node().style.cursor = 'pointer';

UnitWeaponScreen.destroy = function()
{
	destroy_object(UnitWeaponScreen);
	UnitWeaponScreen = null;
};