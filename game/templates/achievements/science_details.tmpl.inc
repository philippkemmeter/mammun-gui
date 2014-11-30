<script type="text/javascript">
Achievements = new Object();
Achievements.this_window = PureFW.ContainerAJAX.instances[<?=$this_win_id?>];
Achievements.researched_sciences = <?= $researched_scs_json ?>;
Achievements.cur_science = <?= json_encode($science) ?>;

Achievements.bernard_size_small = new PureFW.Point(280, 233);
Achievements.bernard_size_large = new PureFW.Point(620, 338);

Achievements.fill_main_box = function() {
	var cur_science = Achievements.cur_science;
	Achievements.main_box_title.set_content(cur_science.name);

	if (cur_science.resource1 > 0)
		Achievements.wood_value.set_content('+ '+cur_science.resource1+'/h');
	else
		Achievements.wood_value.set_content(0);
	if (cur_science.resource2 > 0) {
		if ((cur_science.type != 30) && (cur_science.type < 100))
			Achievements.food_value.set_content('+ '+cur_science.resource2+'/h');
		else
			Achievements.food_value.set_content('- '+(cur_science.resource2*1000)+' g');
	}
	else
		Achievements.food_value.set_content(0);
	if (cur_science.attack > 0)
		Achievements.strength_value.set_content('+ '+cur_science.attack);
	else
		Achievements.strength_value.set_content(0);
	if (cur_science.defend > 0)
		Achievements.dexterity_value.set_content('+ '+cur_science.defend);
	else
		Achievements.dexterity_value.set_content(0);
	if (cur_science.hitpoints > 0)
		Achievements.health_value.set_content('+ '+cur_science.hitpoints);
	else
		Achievements.health_value.set_content(0);
	if (cur_science.speed > 0)
		Achievements.speed_value.set_content('+ '+cur_science.speed);
	else
		Achievements.speed_value.set_content(0);

	for (var type in Achievements.researched_sciences) {
		for (var sc_id in Achievements.researched_sciences[type])
		{
			Achievements.list_all.add_content(
				'<a style="color: #f2f7ff" '
					+ 'href="javascript: Achievements.this_window.set_url(\''
					+ '<?= $_SERVER['PHP_SELF']?>?dataX='
					+ Achievements.researched_sciences[type][sc_id].dataX +
					'\');">'+
				Achievements.researched_sciences[type][sc_id].name + '</a><br/>'
			);
		}
	}
	Achievements.list_all.update_inner_height();
	Achievements.list_all.set_height(
		Achievements.main_box.height - /*220*/160
	);
}

Achievements.fill_lu_box = function() {
	var cur_science = Achievements.cur_science;
	for (var i = 1; i <= 3; i++) {
		if (typeof(cur_science['need_sc'+i]) == 'object') {
			Achievements.lu_cont_box.add_content(
				'<a style="color:#fff" href="javascript: Achievements.this_window.set_url(\''
					+ '<?= $_SERVER['PHP_SELF']?>?dataX='
					+ encodeURI(cur_science['need_sc'+i].dataX) + '\');">'
					+ cur_science['need_sc'+i].name + '</a><br/>'
			);
		}
	}
	Achievements.lu_cont_box.set_y(
		((Achievements.lu_box.get_height() - Achievements.lu_cont_box.get_height())
			>> 1) / Achievements.lu_box.scale_factor
	);
}

Achievements.fill_ru_box = function() {
	var cur_science = Achievements.cur_science;
	var n = cur_science.is_req_of.length;
	var update_height = function ()
	{
		var n = (parseInt(Achievements.ru_cont_box.parent_node.style.height));
		if(Achievements.ru_cont_box.get_height() < n)
		{
			Achievements.ru_cont_box.set_height(
				Achievements.ru_cont_box.get_height() + 15);
		}
		else
		{
			Achievements.ru_cont_box.update_inner_height();
		}
	}
	for (var i = 0; i < n; i++) {
		if (cur_science.is_req_of[i].known) {
			update_height();
			Achievements.ru_cont_box.inner_container.add_content(
				'<a style="color:#fff" href="javascript: Achievements.this_window.set_url(\''
				+ '<?= $_SERVER['PHP_SELF']?>?dataX='
					+ encodeURI(cur_science.is_req_of[i].dataX) + '\');">'
					+ cur_science.is_req_of[i].name + '</a><br/>'
			);
		}
		else {
			update_height();
			Achievements.ru_cont_box.inner_container.add_content(
				'<span style="color:#bcd">'+ cur_science.is_req_of[i].name + '</span><br/>'
			);
		}
	}

	Achievements.ru_cont_box.set_y(
		((Achievements.ru_box.get_height() - Achievements.ru_cont_box.get_height())
			>> 1) / Achievements.ru_box.scale_factor
	);
}

Achievements.fill_granny = function() {
<? if ($sc_id == 0) : ?>
	return;
<? else : ?>
	var cur_science = Achievements.cur_science;
	var known_string = '';

	var n = cur_science.is_req_of.length;
	var first = true;
	for (var i = 0; i < n; i++) {
		if (first)
			first = false
		else
			known_string += ', ';
		if (cur_science.is_req_of[i].known) {
			known_string +=
				'„<a href="javascript: Achievements.this_window.set_url(\'';
			known_string += '<?= $_SERVER['PHP_SELF']?>?dataX=';
			known_string += encodeURI(cur_science.is_req_of[i].dataX);
			known_string += '\');">';
			known_string += cur_science.is_req_of[i].name;
			known_string += '</a>“';
		}
		else {
			known_string += '„';
			known_string += cur_science.is_req_of[i].name;
			known_string += '“';
		}
	}
	if (n > 0) {
		if (cur_science.desc_known) {
			Achievements.granny_cont.set_content(
				cur_science.desc_known + '<br/><br/>' +
				'<?= LNG_GRANNY_TEXT_MASK ?>'
				.replace("[SCIENCE]", cur_science.name)
				.replace("[NEW_SCIENCES]", known_string));
		}
		else {
			Achievements.granny_cont.set_content(
				'<?= LNG_GRANNY_TEXT_MASK ?>'
				.replace("[SCIENCE]", cur_science.name)
				.replace("[NEW_SCIENCES]", known_string));
		}
	}
	else {
		if (cur_science.desc_known) {
			Achievements.granny_cont.set_content(cur_science.desc_known);
		}
		else {
			Achievements.granny_cont.set_content(
				'<?= LNG_GRANNY_TEXT_MASK_NO_PROGRESS ?>'
					.replace("[SCIENCE]", cur_science.name));
		}
	}
<? endif; ?>
}

Achievements.fill_bernard = function() {
<? if ($sc_id == 0) : ?>
	return;
<? else : ?>
	var cur_science = Achievements.cur_science;
	var h1 = Achievements.bernard_cont.height;
<? if ($activate_input_bernard) { ?>
	Achievements.bernard_cont.set_content(
		cur_science.desc_bernard.replace(/<br\/>/g, "\n")
	);
<? } else { ?>
	Achievements.bernard_cont.set_content(cur_science.desc_bernard);
	Achievements.bernard_cont.set_height(0);
	var h = Achievements.bernard_cont.inner_container.get_height() /
		Achievements.bernard_cont.inner_container.scale_factor;
	Achievements.bernard_cont.set_height(h1);
<? } ?>
<? endif; ?>
}

Achievements.expand_bernard = function(ev)
{
	ev.stopPropagation();
	var move_horizontal = Achievements.bernard_size_large.x
											- Achievements.bernard_size_small.x;
	var move_vertical = Achievements.bernard_size_large.y
											- Achievements.bernard_size_small.y;
	Achievements.bernard.set_x(Achievements.bernard.position.x-move_horizontal);
	Achievements.bernard.set_y(Achievements.bernard.position.y - move_vertical);
	Achievements.bernard.set_width(Achievements.bernard_size_large.x);
	Achievements.bernard.set_height(Achievements.bernard_size_large.y);
	Achievements.bernard.set_bg_img(
			'ui/backgrounds/achievements/bernard_bg_large.png');

	Achievements.bernard_label.set_x(Achievements.bernard_label.position.x
															- move_horizontal);
	Achievements.bernard_label.set_y(Achievements.bernard_label.position.y
															- move_vertical);

	Achievements.bernard_cont.set_width(Achievements.bernard_size_large.x-20);
	Achievements.bernard_cont.set_height(Achievements.bernard_size_large.y-40);

	try
	{	// Innerhalb von try-catch weil bernard_cont auch ein TextArea sein kann
		Achievements.bernard_cont.vertical_scrolling_enabled = true;
		Achievements.bernard_cont.update_inner_height();
	}
	catch(e){}
	Achievements.bernard_cont.remove_event_handler('click',
												Achievements.expand_bernard);
	Achievements.bernard_label.remove_event_handler('click',
			   									Achievements.expand_bernard);
	Achievements.bernard_cont.inner_container.add_event_handler('click',
												Achievements.shrink_bernard);
	Achievements.bernard_label.add_event_handler('click',
												Achievements.shrink_bernard);

	Achievements.bernard_cont.on_mouseover();
	Achievements.bernard_cont.set_tooltip('');
}

Achievements.shrink_bernard = function(ev)
{
	ev.stopPropagation();
	var move_horizontal = Achievements.bernard_size_large.x
											- Achievements.bernard_size_small.x;
	var move_vertical = Achievements.bernard_size_large.y
											- Achievements.bernard_size_small.y;
	Achievements.bernard.set_x(Achievements.bernard.position.x+move_horizontal);
	Achievements.bernard.set_y(Achievements.bernard.position.y + move_vertical);
	Achievements.bernard.set_width(Achievements.bernard_size_small.x);
	Achievements.bernard.set_height(Achievements.bernard_size_small.y);
	Achievements.bernard.set_bg_img(
			'ui/backgrounds/achievements/bernard_bg_large.png');
	Achievements.bernard_label.set_x(Achievements.bernard_label.position.x
															+ move_horizontal);
	Achievements.bernard_label.set_y(Achievements.bernard_label.position.y
															+ move_vertical);

	Achievements.bernard_cont.set_width(Achievements.bernard_size_small.x - 20);
	Achievements.bernard_cont.set_height(Achievements.bernard_size_small.y-40);
	try
	{	// Innerhalb von try-catch weil bernard_cont auch ein TextArea sein kann
		Achievements.bernard_cont.vertical_scrolling_enabled = false;
		Achievements.bernard_cont.horizontal_scrolling_enabled = false;
		Achievements.bernard_cont.set_inner_width(
										Achievements.bernard_size_small.x - 20);
	}
	catch(e){}
	Achievements.bernard_cont.inner_container.remove_event_handler('click',
												Achievements.shrink_bernard);
	Achievements.bernard_label.remove_event_handler('click',
												Achievements.shrink_bernard);
	Achievements.bernard_cont.add_event_handler('click',
												Achievements.expand_bernard);
	Achievements.bernard_label.add_event_handler('click',
												Achievements.expand_bernard);

	Achievements.bernard_cont.on_mouseout();
	Achievements.bernard_cont.set_tooltip('<?= LNG_CLICK_TO_EXPAND_BERNARD ?>');
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

<?
/**
 * 5 Boxen. In der Mitte die Hauptbox, links oben die Vorrausetzungen (vorherige
 * Stufe im Techtree), rechts oben die Forschungen, deren Vorraussetzung die
 * aktuelle Forschung ist (also nächste Stufe im Techtree), links unten Omas
 * Erklärung, rechts unten Bernards Beitrag.
 *
 * Hier wird nur das Design bestimmt. Der Inhalt wird durch oben angegebene
 * Funktionen erzeugt. Das macht es flexibler, Daten per Ajax nachzuladen.
 */
?>

<? /* Mitte */ ?>
Achievements.main_box = new PureFW.Container(
	Achievements.this_window,
	(Achievements.this_window.width - 280) >> 1, 10,
	280, 390
);
Achievements.main_box.set_bg_img('ui/backgrounds/achievements/middle_bg.png');
<? /* Mitte: Title */ ?>
Achievements.main_box_title = new PureFW.Container(
	Achievements.main_box,
	10, 10,
	Achievements.main_box.width - 20, 25
);
Achievements.main_box_title.set_font_size(1.4);
Achievements.main_box_title.set_text_align('center');

<? /* Mitte: wood icon and value */ ?>
Achievements.wood_ico = new PureFW.Image(
	Achievements.main_box,
	20,
	Achievements.main_box_title.position.y
		+ Achievements.main_box_title.height + 15,
	22, 22,
	'ui/icons/labels/resources/22/wood.png'
);

Achievements.wood_value = new PureFW.Container(
	Achievements.main_box,
	Achievements.wood_ico.position.x + Achievements.wood_ico.width + 5,
	Achievements.wood_ico.position.y + 3,
	55, 16
);

<? /* Mitte: food icon and value */ ?>
Achievements.food_ico = new PureFW.Image(
	Achievements.main_box,
	Achievements.wood_ico.position.x,
	Achievements.wood_ico.position.y
		+ Achievements.wood_ico.height + 10,
	22, 22,
	'ui/icons/labels/resources/22/food.png'
);
Achievements.food_value = new PureFW.Container(
	Achievements.main_box,
	Achievements.food_ico.position.x + Achievements.food_ico.width + 5,
	Achievements.food_ico.position.y + 3,
	55, 16
);

<? /* Mitte: strength icon and value */ ?>
Achievements.strength_ico = new PureFW.Image(
	Achievements.main_box,
	Achievements.wood_value.position.x + Achievements.wood_value.width,
	Achievements.wood_ico.position.y,
	22, 22,
	'ui/icons/labels/attributes/22/strength.png'
);
Achievements.strength_value = new PureFW.Container(
	Achievements.main_box,
	Achievements.strength_ico.position.x + Achievements.strength_ico.width + 5,
	Achievements.strength_ico.position.y + 3,
	55, 16
);

<? /* Mitte: dexterity icon and value */ ?>
Achievements.dexterity_ico = new PureFW.Image(
	Achievements.main_box,
	Achievements.strength_ico.position.x,
	Achievements.strength_ico.position.y
		+ Achievements.strength_ico.height + 10,
	22, 22,
	'ui/icons/labels/attributes/22/dexterity.png'
);
Achievements.dexterity_value = new PureFW.Container(
	Achievements.main_box,
	Achievements.dexterity_ico.position.x + Achievements.dexterity_ico.width + 5,
	Achievements.dexterity_ico.position.y + 3,
	55, 16
);

<? /* Mitte: health icon and value */ ?>
Achievements.health_ico = new PureFW.Image(
	Achievements.main_box,
	Achievements.strength_value.position.x + Achievements.strength_value.width,
	Achievements.strength_ico.position.y,
	22, 22,
	'ui/icons/labels/attributes/22/health.png'
);
Achievements.health_value = new PureFW.Container(
	Achievements.main_box,
	Achievements.health_ico.position.x + Achievements.health_ico.width + 5,
	Achievements.health_ico.position.y + 3,
	55, 16
);

<? /* Mitte: speed icon and value */ ?>
Achievements.speed_ico = new PureFW.Image(
	Achievements.main_box,
	Achievements.health_ico.position.x,
	Achievements.health_ico.position.y
		+ Achievements.health_ico.height + 10,
	22, 22,
	'ui/icons/labels/attributes/22/speed.png'
);
Achievements.speed_value = new PureFW.Container(
	Achievements.main_box,
	Achievements.speed_ico.position.x + Achievements.speed_ico.width + 5,
	Achievements.speed_ico.position.y + 3,
	55, 16
);

<? /** Suchdialog
Achievements.search_txt = new PureFW.Image(
	Achievements.main_box,
	(Achievements.main_box.width - 128) >> 1,
	140,
	128, 13,
	'ui/elements/texts/<?=$LNG?>/achievements/search_for_science.png'
);

Achievements.search_input = new PureFW.Container(
	Achievements.main_box,
	(Achievements.main_box.width - 250) >> 1,
	Achievements.search_txt.position.y + Achievements.search_txt.height + 5,
	250, 25
);
Achievements.search_input.set_bg_img(
	'ui/elements/form/input/input_text_250x25.png'
);*/ ?>

<? /** Liste aller Forschungen */ ?>
Achievements.list_all = new PureFW.ScrollContainer(
	Achievements.main_box,
	(Achievements.main_box.width - 250) >> 1,
	/*200*/140,
	250,
	0
);
<? // Alles füllen ?>
Achievements.fill_main_box();




<? /* links oben */ ?>
Achievements.lu_box = new PureFW.Container(
	Achievements.this_window,
	20,10,
	280, 120
);
Achievements.lu_box.set_bg_img('ui/backgrounds/achievements/reqs_bg.png');

Achievements.lu_cont_box = new PureFW.Container(
	Achievements.lu_box,
	10, 0,
	Achievements.lu_box.width - 20, 0
);
Achievements.lu_cont_box.get_node().className='confirmation_box';
Achievements.fill_lu_box();

<? /** Pfeil von links oben zur Mitte */ ?>

Achievements.arrow_1 = new PureFW.Image(
	Achievements.this_window,
	<? /* Position arrow in the middle between both boxes (X-Coords) */ ?>
	((Achievements.lu_box.position.x + Achievements.lu_box.width) +
		(Achievements.main_box.position.x - (Achievements.lu_box.position.x +
			Achievements.lu_box.width + 25)>> 1)),
	<? /* Center the arrow vertically with respect to lu_box */ ?>
	Achievements.lu_box.position.y +
		((Achievements.lu_box.height - 74) >> 1),
	25, 74,
	'ui/icons/labels/achievements/arrow.png'
);

<? /* rechts oben */ ?>
Achievements.ru_box = new PureFW.Container(
	Achievements.this_window,
	Achievements.this_window.width - 300,10,
	280, 120
);
Achievements.ru_box.set_bg_img('ui/backgrounds/achievements/reqs_bg.png');

Achievements.ru_cont_box = new PureFW.ScrollContainer(
	Achievements.ru_box,
	10, 0,
	Achievements.ru_box.width - 20, 0
);
Achievements.ru_cont_box.get_node().className='confirmation_box';
Achievements.fill_ru_box();

<?/*
Achievements.ru_box = new PureFW.Container(
	Achievements.this_window,
	Achievements.this_window.width - 300,10,
	280, 120
);
Achievements.ru_box.set_bg_img('ui/backgrounds/achievements/reqs_bg.png');

Achievements.ru_cont_box = new PureFW.Container(
	Achievements.ru_box,
	10, 0,
	Achievements.ru_box.width - 20, 0
);
Achievements.ru_cont_box.get_node().className='confirmation_box';
Achievements.fill_ru_box();
*/?>
<? /** Pfeil von Mitte nach rechts oben */ ?>

Achievements.arrow_1 = new PureFW.Image(
	Achievements.this_window,
	<? /* Position arrow in the middle between both boxes (X-Coords) */ ?>
	((Achievements.main_box.position.x + Achievements.main_box.width) +
		(Achievements.ru_box.position.x	- (Achievements.main_box.position.x +
			 Achievements.main_box.width + 25)>> 1)),
	<? /* Center the arrow vertically with respect to ru_box */ ?>
	Achievements.ru_box.position.y +
		((Achievements.ru_box.height - 74) >> 1),
	25, 74,
	'ui/icons/labels/achievements/arrow.png'
);

<? /* links unten */ ?>
Achievements.granny = new PureFW.Container(
	Achievements.this_window,
	Achievements.lu_box.position.x,
	Achievements.main_box.position.y + Achievements.main_box.height
		- 233,
	280, 233
);
Achievements.granny.set_bg_img('ui/backgrounds/achievements/oma_bg.png');

Achievements.granny_cont = new PureFW.ScrollContainer(
	Achievements.granny,
	10, 20,
	Achievements.granny.width - 20,
	Achievements.granny.height - 40
);
Achievements.granny_cont.set_font_color('#000');
Achievements.fill_granny();

<? /* Granny-Caption */ ?>
Achievements.granny_label = new PureFW.Image(
	Achievements.this_window,
	Achievements.granny.position.x - 5,
	Achievements.granny.position.y - 20,
	189, 35,
	'ui/elements/texts/<?=$LNG?>/achievements/granny_desc_caption.png'
);

<? /* rechts unten */ ?>
Achievements.bernard = new PureFW.Container(
	Achievements.this_window,
	Achievements.ru_box.position.x,
	Achievements.main_box.position.y + Achievements.main_box.height
		- 233,
	280, 233
);
Achievements.bernard.set_z_index(10);
Achievements.bernard.set_bg_img(
		'ui/backgrounds/achievements/bernard_bg_small.png');

<? if ($activate_input_bernard) { ?>
Achievements.bernard_cont = new PureFW.TextArea(
	Achievements.bernard,
	10, 20,
	Achievements.bernard.width - 20,
	Achievements.bernard.height - 40
);
Achievements.bernard_cont.set_z_index(10);
Achievements.save = new PureFW.Container(
	Achievements.this_window,
	Achievements.bernard.position.x + Achievements.bernard.width
		- 100,
	Achievements.bernard.position.y + Achievements.bernard.height,
	100, 25
);
Achievements.save.set_bg_color('red');
Achievements.save.set_content('Speichern');
Achievements.save.set_text_align('center');

Achievements.save.add_event_handler('click',
	function(ev) {
		alert("Hallo Bernard! Ich hab das mit dem direkten einspeichern "+
			"deaktiviert, per Word wäre es mir doch lieber, weil ich sonst "
			"doppelt setzen muss. Bitte schick mir den Text also einfach per "+
			"Mail, das wäre lieb :)"+
			"\n\nPhilipp");
	}
);
<? } else { ?>
Achievements.bernard_cont = new PureFW.ScrollContainer(
	Achievements.bernard,
	10, 20,
	Achievements.bernard.width - 20,
	Achievements.bernard.height - 40
);
Achievements.bernard_cont.set_z_index(10);
<? } ?>
Achievements.bernard_cont.set_font_color('#000');
Achievements.fill_bernard();
Achievements.bernard_cont.add_event_handler('click', Achievements.expand_bernard);
Achievements.bernard_cont.set_tooltip('<?= LNG_CLICK_TO_EXPAND_BERNARD ?>');

<? /* Bernard-Caption */ ?>
Achievements.bernard_label = new PureFW.Image(
	Achievements.this_window,
	Achievements.bernard.position.x - 5,
	Achievements.bernard.position.y - 20,
	189, 35,
	'ui/elements/texts/<?=$LNG?>/achievements/bernard_desc_caption.png'
);
Achievements.bernard_label.set_z_index(10);
Achievements.bernard_label.add_event_handler('click',
	Achievements.expand_bernard);
</script>