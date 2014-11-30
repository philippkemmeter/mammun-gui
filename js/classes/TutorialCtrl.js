/**
 * Die TutorialCtrl-Klasse für die Ablaufsteuerung von Tutorials verantwortlich.
 *
 * Da immer nur ein Tutorial gleichzeitig ablaufen kann, ist diese Klasse
 * gänzlich statisch.
 *
 * @author Phil
 */

TutorialCtrl = new Object();

TutorialCtrl.running = false;

/**
 * Hilfsfunktion. Gibt den aktuellen Schritt zurück.
 *
 * @param Object tutorial
 * @param uint index
 * @return Object
 */
TutorialCtrl.get_current_step = function (tutorial, index) {
	return tutorial.steps[tutorial.order[index]];
}

/**
 * Startet das angegebene Tutorial an der angegebenen Stelle.
 * 
 * @param Object tutorial
 * @param uint step
 */
TutorialCtrl.start = function(tutorial, step) {
	TutorialCtrl.running = true;
	tutorial.init(step);
	
	var current_step = TutorialCtrl.get_current_step(
		tutorial,
		tutorial.current_step_i
	);
	current_step.action();
	/**
	 * Tutorial verbergen, wenn Event angezeigt wird.
	 */
	if (EventMessage.is_showing_popup())
		TutorialCtrl.hide(tutorial);
}

/**
 * Stopp das angegebene Tutorial.
 * 
 * @param Object tutorial
 */
TutorialCtrl.stop = function(tutorial) {
	TutorialCtrl.running = false;
	tutorial.clean_up();
}

/**
 * Diese Funktion schließt den Aktuellen Schritt erfolgreich ab und geht zum
 * nächsten.
 * 
 * @param Object tutorial
 * @param Object exspected_current		Welcher Schritt der aktuelle sein soll.
 * 										Wenn dieser Parameter gesetzt ist, wird
 * 										geprüft, ob der aktuelle Schritt auch
 * 										der ist, dem erwarteten entspricht und
 * 										nur dann wird weitergeschritten.
 */
TutorialCtrl.next = function(tutorial, exspected_current) {
	if (tutorial.current_step_i < 0) { 
		TutorialCtrl.start(tutorial);
		return;
	}
	
	var current_step = TutorialCtrl.get_current_step(
		tutorial,
		tutorial.current_step_i
	);
	
	if ((typeof(exspected_current) != 'undefined') &&
		(exspected_current != current_step))
	{
		return;
	}
	if (current_step.flag_to_set > 0) {
		PureFW.AJAXClientServer.send_request('tutorial.php?tut_flag='
			+ current_step.flag_to_set , null, true);
	}
	current_step.approve();
	
	current_step = TutorialCtrl.get_current_step(
		tutorial,
		++tutorial.current_step_i
	);
	
	if (typeof(current_step) !== 'undefined')
		current_step.action();
	else {
		TutorialCtrl.stop(tutorial);
		/**
		 * Versuchen, das nächste Tutorial zu starten.
		 */
		var yes = false;
		var started = false;
		for (o in TutorialCtrl) {
			if (yes) {
				try {
					TutorialCtrl.start(TutorialCtrl[o]);
					started = true;
					break;
				}
				catch (e) {}
			}
			if (TutorialCtrl[o] == tutorial)
				yes = true;
		}
		
		if (!started)
			TutorialCtrl.running = false;
	}
}

/**
 * Diese Funktion schließt den Aktuellen Schritt NICHT erfolgreich ab und geht 
 * zum vorherigen zurück. 
 * 
 * @param Object tutorial
 * @param Object exspected_current		Welcher Schritt der aktuelle sein soll.
 * 										Wenn dieser Parameter gesetzt ist, wird
 * 										geprüft, ob der aktuelle Schritt auch
 * 										der ist, dem erwarteten entspricht und
 * 										nur dann wird zurückgeschritten.
 */
TutorialCtrl.previous = function(tutorial, expected_current) {
	if (tutorial.current_step_i < 0) { 
		TutorialCtrl.start(tutorial);
		return;
	}
	
	var current_step = TutorialCtrl.get_current_step(
		tutorial,
		tutorial.current_step_i
	);
	
	if ((typeof(exspected_current) != 'undefined') &&
		(exspected_current != current_step))
	{
		return;
	}
	
	current_step.cancel();
	
	current_step = TutorialCtrl.get_current_step(
		tutorial,
		--tutorial.current_step_i
	);
	
	if (typeof(current_step) !== 'undefined')
		current_step.action();
	else {
		TutorialCtrl.start(tutorial);
	}
}


/**
 * Alle Officers geben den Text wieder, der hier angegeben wird, komme was 
 * wolle.
 */
TutorialCtrl.hijack_officer = function(text) {
	TutorialCtrl.hijack_text = text;
	if (text && text.length > 0) {
		TutorialCtrl.officer_set_text = PureFW.Officer.prototype.set_text;
	
		PureFW.Officer.prototype.set_text = function() {
			TutorialCtrl.officer_set_text.call(
				this,
				TutorialCtrl.hijack_text
			);
		}
	}
}

/**
 * Die Officers sind wieder frei :)
 */
TutorialCtrl.free_officer = function() {
	if (typeof(TutorialCtrl.officer_set_text) == 'function')
		PureFW.Officer.prototype.set_text = TutorialCtrl.officer_set_text;
}

/**
 * Verbirgt das Tutorial
 */
TutorialCtrl.hide = function(tutorial) {
	if (!tutorial) {
		for (o in TutorialCtrl) {
			if (TutorialCtrl[o] && TutorialCtrl[o].steps) {
				TutorialCtrl.hide(TutorialCtrl[o]);
			}
		}
		return;
	}
	var current_step = TutorialCtrl.get_current_step(
		tutorial,
		tutorial.current_step_i
	);
	if (current_step)
		current_step.hide();
}

/**
 * Zeigt das Tutorial (wieder) an
 */
TutorialCtrl.show = function(tutorial) {
	if (!tutorial) {
		for (o in TutorialCtrl) {
			if (TutorialCtrl[o] && TutorialCtrl[o].steps) {
				TutorialCtrl.show(TutorialCtrl[o]);
			}
		}
		return;
	}
	var current_step = TutorialCtrl.get_current_step(
		tutorial,
		tutorial.current_step_i
	);
	if (current_step)
		current_step.show();
}

/**
 * Die Tutorial-Officer-Box erstellen mit entsprechendem Inhalt.
 * 
 * Diese Officer-Box ist dazu da, zusätzliche Hinweise anzuzeigen, ohne den 
 * Spielablauf zu stören. Ergänzende Informationen, die nicht bestätigt werden
 * müssen.
 * 
 * @param String text
 */
TutorialCtrl.tut_box = null;
TutorialCtrl.create_tut_officer_box = function(text) {
	if (TutorialCtrl.tut_box) {
		TutorialCtrl.destroy_tut_officer_box();
	}
	TutorialCtrl.tut_box = new PureFW.Container(
		document.body,
		(MammunUI.reference_width - 546) >> 1,
		100,
		546, 115
	);
	TutorialCtrl.tut_box.set_bg_img(
		'ui/backgrounds/tutorial/officer_tutorial_box_546x115.png'
	);
	TutorialCtrl.tut_box.set_css_class(
		'tutorial_box_small'
	);
	TutorialCtrl.tut_box_inner = new PureFW.Container(
		TutorialCtrl.tut_box,
		110, 10,
		TutorialCtrl.tut_box.width - 120,
		TutorialCtrl.tut_box.height - 20
	);
	TutorialCtrl.tut_box_inner.set_content(
		text
	);
}

/**
 * Zerstört die Tutorial-Officer_box
 * 
 * @see TutorialCtrl#create_tut_officer_box
 */
TutorialCtrl.destroy_tut_officer_box = function() {
	try {
		TutorialCtrl.tut_box_inner.destroy();
	}
	catch(e) {}
	TutorialCtrl.tut_box_inner = null;
	
	try {
		TutorialCtrl.tut_box.destroy();
	}
	catch(e) {}	
	TutorialCtrl.tut_box = null;
}

TutorialCtrl.hide_tut_officer_box = function() {
	TutorialCtrl.tut_box.hide();
}

TutorialCtrl.show_tut_officer_box = function() {
	TutorialCtrl.tut_box.show();
}


/**
 * Jedes Tutorial ist ein eigenes Objekt unterhalb von TutorialCtrl. Es besteht
 * prinzipiell aus einem Unterobjekt "steps", in dem alle Tutorial-Schritte als
 * einzelne Objekte abgelegt sind und einem Unter-Array "order", das die 
 * Reihenfolge der einzelnen Schritte bestimmt. Zusätzlich ist noch das Attribut
 * current_step_i erforderlich, um den aktuellen Schritt zu kennen.
 * 
 * Hinzu kommen die obligaten Funktionen 'init' und 'clean_up'.
 * 
 * Navigiert wird über die Oberklasse TutorialCtrl bzw. deren Funktion start,
 * stop, next und previous.
 */


/*****************************************************************************
 * 
 * 
 * 
 * Das Tutorial "erste Schritte".
 * 
 * 
 * 
 *****************************************************************************/
TutorialCtrl.first_steps = new Object();

/** Alle Schritte im Tutorial */
TutorialCtrl.first_steps.steps = {
	show_intro_text:{},
	show_slide_show:{},
	slide_show_outro:{},
	home_sector:{},
	choose_unit_move:{},
	move_unit:{},
	click_move_arrow:{},
	close_move_arrow:{},
	open_science:{},
	start_research:{},
	quit_research:{},
	home_sector2:{},
	choose_unit_move2:{},
	move_unit2:{},
	open_training:{},
	start_training:{},
	training_details:{},
	training_indivi:{},
	quit_training:{},
	show_outro_text:{},
	wait_until_arrived:{}
}

/** Reihenfolge der einzelnen Schritte */
TutorialCtrl.first_steps.order = [
	'show_intro_text',
	'show_slide_show',
	'slide_show_outro',
	'home_sector', 
	'choose_unit_move', 
	'move_unit',
	'click_move_arrow',
	'close_move_arrow',
	'open_science', 
	'start_research', 
	'quit_research', 
	'home_sector2',
	'choose_unit_move2',
	'move_unit2',
	'open_training', 
	'start_training', 
	'training_details', 
	'training_indivi',
	'quit_training',
	'show_outro_text',
	'wait_until_arrived'
];

TutorialCtrl.first_steps.current_step_i = -1;

/**
 * Initialisiert das Tutorial :)
 * 
 * @param Object step		Einsprungpunkt: Mit dem wievielten Schritt soll
 * 							begonnen werden?
 */
TutorialCtrl.first_steps.init = function(step_i) {
	GameMenu.close_game_menu();
	GameMenu.hide();
	try {
		UIButtons.bb1.hide();
		UIButtons.bb2.hide();
		UIButtons.bb3.hide();
	}
	catch(e){}
	
	TutorialCtrl.first_steps.current_step_i = step_i || 0;
}

/**
 * Deinitialisiert das Tutorial :)
 */
TutorialCtrl.first_steps.clean_up = function() {
	UIButtons.bb1.show();
	UIButtons.bb2.show();
	UIButtons.bb3.show();
	GameMenu.show();
}


/**
 * Jetzt kommen die einzelnen Tutorialschritte. Jeder Schritt ist ein Objekt
 * mit mindestens den drei Funktionen 'action', 'cancel' und 'approve'.
 * 
 * Zudem sind folgende Attribute vorgeschrieben: 'text' und 'flag_to_set'.
 * 'text' gibt dabei an, welcher Text in dem Schritt angezeigt werden soll
 * (manche Schritte können keinen Text anzeigen, dann ist das Setzen zwecklos),
 * und 'flag_to_set' gibt an, welcher Flag an den GUI-Server übermittelt werden
 * soll, wenn der Schritt erfolgreich abgeschlossen wurde.
 */

                                  
/**
 * Der Schritt zeigt den Introtext an.
 * 
 * In diesem Schritt wird einfach ein Intro-Text angezeigt, der den Spieler
 * willkommen heißen soll.
 */
TutorialCtrl.first_steps.steps.show_intro_text.text = '';
TutorialCtrl.first_steps.steps.show_intro_text.flag_to_set = 0;
TutorialCtrl.first_steps.steps.show_intro_text.action = function() {
	Overview.xp_bar.hide();
	TutorialCtrl.first_steps.steps.show_intro_text.bg = new PureFW.Container(
		document.body,
		0, 0,
		MammunUI.reference_width, MammunUI.reference_height
	);
	TutorialCtrl.first_steps.steps.show_intro_text.bg.set_bg_color('#e7eef6');
	TutorialCtrl.first_steps.steps.show_intro_text.bg.set_z_index(310);
	try {
		SectorDetails.this_window.destroy();
	} catch(e) {};
	TutorialCtrl.first_steps.steps.show_intro_text.box = new PureFW.OfficerBox(
		document.body,
		(MammunUI.reference_width - 546) >> 1,
		(MammunUI.reference_height - 278) >> 1,
		546, 278
	);
	TutorialCtrl.first_steps.steps.show_intro_text.box.set_bg_img(
		'ui/backgrounds/tutorial/officer_tutorial_box_546x278.png'
	);
	TutorialCtrl.first_steps.steps.show_intro_text.box.set_content(
		TutorialCtrl.first_steps.steps.show_intro_text.text
	);
	TutorialCtrl.first_steps.steps.show_intro_text.box.set_z_index(311);
	
	TutorialCtrl.first_steps.steps.show_intro_text.box.add_event_handler(
		"confirm",
		function() {
			TutorialCtrl.next(
				TutorialCtrl.first_steps,
				TutorialCtrl.first_steps.steps.show_intro_text
			);
		}
	);
	
	TutorialCtrl.first_steps.steps.show_intro_text.box.set_css_class(
		'tutorial_box_big'
	);
}
TutorialCtrl.first_steps.steps.show_intro_text.approve = function() {
	TutorialCtrl.first_steps.steps.show_intro_text.cancel();
}
TutorialCtrl.first_steps.steps.show_intro_text.cancel = function() {
	try {
		TutorialCtrl.first_steps.steps.show_intro_text.box.destroy();
		TutorialCtrl.first_steps.steps.show_intro_text.box = null;
	}
	catch(e) {}	// was removed already
	try {
		TutorialCtrl.first_steps.steps.show_intro_text.bg.destroy();
		TutorialCtrl.first_steps.steps.show_intro_text.bg = null;
	}
	catch(e) {}	// was removed already
	Overview.xp_bar.show();
}
TutorialCtrl.first_steps.steps.show_intro_text.hide = function() {
	try {
		TutorialCtrl.first_steps.steps.show_intro_text.box.hide();
	}
	catch(e) {}
	try {
		TutorialCtrl.first_steps.steps.show_intro_text.bg.hide();
	}
	catch(e) {}
}
TutorialCtrl.first_steps.steps.show_intro_text.show = function() {
	try {
		TutorialCtrl.first_steps.steps.show_intro_text.box.show();
	}
	catch(e) {}
	try {
		TutorialCtrl.first_steps.steps.show_intro_text.bg.show();
	}
	catch(e) {}
}
/**
 * Jetzt wird eine einleitende Slideshow angezeigt, die den Spieler 
 */
TutorialCtrl.first_steps.steps.show_slide_show.text = new Array();
TutorialCtrl.first_steps.steps.show_slide_show.pictures = new Array(
	'ui/backgrounds/tutorial/slideshow/island.jpg',
	'ui/backgrounds/tutorial/slideshow/research.jpg',
	'ui/backgrounds/tutorial/slideshow/training.jpg',
	'ui/backgrounds/tutorial/slideshow/constructing.jpg'
);
TutorialCtrl.first_steps.steps.show_slide_show.action = function() {
	Overview.xp_bar.hide();
	TutorialCtrl.first_steps.steps.show_slide_show.bg = new PureFW.Container(
		document.body,
		0, 0,
		MammunUI.reference_width, MammunUI.reference_height
	);
	TutorialCtrl.first_steps.steps.show_slide_show.bg.set_bg_color('#e7eef6');
	TutorialCtrl.first_steps.steps.show_slide_show.bg.set_z_index(310);
	
	TutorialCtrl.first_steps.steps.show_slide_show.box 
		= new PureFW.BubbleScroller
	(
		document.body,
		0, 0,
		MammunUI.reference_width, MammunUI.reference_height,
		1, 0
	);
	TutorialCtrl.first_steps.steps.show_slide_show.box.set_z_index(311);
	TutorialCtrl.first_steps.steps.show_slide_show.box.set_rflick_x_offset(-55);
	TutorialCtrl.first_steps.steps.show_slide_show.box.set_lflick_x_offset(10);
	TutorialCtrl.first_steps.steps.show_slide_show.box
		.set_flick_y_pos(50);
	

	var n = TutorialCtrl.first_steps.steps.show_slide_show.pictures.length;
	for (var i = 0, j = 0; j < n; i+=2, j++) {
		TutorialCtrl.first_steps.steps.show_slide_show.box.add_item(
			PureFW.Container,
			[MammunUI.reference_width, MammunUI.reference_height],
			(function (_pic, _caption, _text, _last) {
				return function() {
					this.set_bg_img(_pic);
					
					var title_cont = this.add_widget(
						PureFW.Container,
						65, 30,
						MammunUI.reference_width -
							(65 << 1),
						0, 0
					);
					title_cont.set_css_class("ShopCatalog_item_title");
					title_cont.set_font_color('#333');
					title_cont.set_content(_caption);
					
					var text_cont = this.add_widget(
						PureFW.Container,
						title_cont.position.x, 80,
						title_cont.width, 0
					);
					text_cont.set_css_class("Window_bright");
					text_cont.set_content(_text);
					
					if (_last) {
						var finish_button = this.add_widget(
							PureFW.Image,
							MammunUI.reference_width - 55,
							50,
							46, 44,
							'ui/elements/bubblebar/flick_right_46x44.png'
						);
						finish_button.add_event_handler(
							"click",
							function(ev) {
								TutorialCtrl.next(
									TutorialCtrl.first_steps,
									TutorialCtrl.first_steps.steps
										.show_slide_show
								);
							}
						);
					}
				}
			})(
				TutorialCtrl.first_steps.steps.show_slide_show.pictures[j],
				TutorialCtrl.first_steps.steps.show_slide_show.text[i],
				TutorialCtrl.first_steps.steps.show_slide_show.text[i+1],
				(j == (n-1))
			)
			
		);
	}
}
TutorialCtrl.first_steps.steps.show_slide_show.approve = function() {
	TutorialCtrl.first_steps.steps.show_slide_show.cancel();
}
TutorialCtrl.first_steps.steps.show_slide_show.cancel = function() {
	Overview.xp_bar.show();
	try {
		TutorialCtrl.first_steps.steps.show_slide_show.bg.destroy();
		TutorialCtrl.first_steps.steps.show_slide_show.bg = null;
		TutorialCtrl.first_steps.steps.show_slide_show.box.destroy();
		TutorialCtrl.first_steps.steps.show_slide_show.box = null;
	}
	catch(e){}
}
TutorialCtrl.first_steps.steps.show_slide_show.hide = function() {
	Overview.xp_bar.show();
	try {
		TutorialCtrl.first_steps.steps.show_slide_show.bg.hide();
	}
	catch(e){}
	try {
		TutorialCtrl.first_steps.steps.show_slide_show.box.hide();
	}
	catch(e){}
}
TutorialCtrl.first_steps.steps.show_slide_show.show = function() {
	Overview.xp_bar.show();
	try {
		TutorialCtrl.first_steps.steps.show_slide_show.bg.show();
	}
	catch(e){}
	try {
		TutorialCtrl.first_steps.steps.show_slide_show.box.show();
	}
	catch(e){}
}
/**
 * Übergang von Slideshow zum eigentlichen Tutorial. 
 */
TutorialCtrl.first_steps.steps.slide_show_outro.text = new Array();
TutorialCtrl.first_steps.steps.slide_show_outro.action = function() {
	TutorialCtrl.first_steps.steps.slide_show_outro.box = new PureFW.OfficerBox(
		document.body,
		(MammunUI.reference_width - 546) >> 1,
		(MammunUI.reference_height - 278) >> 1,
		546, 278
	);
	TutorialCtrl.first_steps.steps.slide_show_outro.box.set_bg_img(
		'ui/backgrounds/tutorial/officer_tutorial_box_546x278.png'
	);
	TutorialCtrl.first_steps.steps.slide_show_outro.box.set_content(
		TutorialCtrl.first_steps.steps.slide_show_outro.text[0]
	);
	var icon = TutorialCtrl.first_steps.steps.slide_show_outro.box.add_widget(
		PureFW.Image,
		15, 75,
		24, 24,
		'ui/icons/labels/events/1.png'
	);
	var txt = TutorialCtrl.first_steps.steps.slide_show_outro.box.add_widget(
		PureFW.Container,
		icon.position.x + icon.width + 10, 
		icon.position.y + 3,
		0, 0
	);
	txt.set_content(TutorialCtrl.first_steps.steps.slide_show_outro.text[1]);
	icon = TutorialCtrl.first_steps.steps.slide_show_outro.box.add_widget(
		PureFW.Image,
		icon.position.x, icon.position.y + icon.height + 6,
		24, 24,
		'ui/icons/labels/events/10.png'
	);
	txt = TutorialCtrl.first_steps.steps.slide_show_outro.box.add_widget(
		PureFW.Container,
		icon.position.x + icon.width + 10, 
		icon.position.y + 3,
		0, 0
	);
	txt.set_content(TutorialCtrl.first_steps.steps.slide_show_outro.text[2]);
	icon = TutorialCtrl.first_steps.steps.slide_show_outro.box.add_widget(
		PureFW.Image,
		icon.position.x, icon.position.y + icon.height + 6,
		24, 24,
		'ui/icons/labels/events/21.png'
	);
	txt = TutorialCtrl.first_steps.steps.slide_show_outro.box.add_widget(
		PureFW.Container,
		icon.position.x + icon.width + 10, 
		icon.position.y + 3,
		0, 0
	);
	txt.set_content(TutorialCtrl.first_steps.steps.slide_show_outro.text[3]);
	
	txt = TutorialCtrl.first_steps.steps.slide_show_outro.box.add_widget(
		PureFW.Container,
		0, icon.position.y + icon.height + 45,
		0, 0
	);
	txt.set_content(TutorialCtrl.first_steps.steps.slide_show_outro.text[4]);
	
	TutorialCtrl.first_steps.steps.slide_show_outro.box.add_event_handler(
		"confirm",
		function() {
			TutorialCtrl.next(
				TutorialCtrl.first_steps,
				TutorialCtrl.first_steps.steps.slide_show_outro
			);
		}
	);
	
	TutorialCtrl.first_steps.steps.slide_show_outro.box.set_css_class(
		'tutorial_box_big'
	);	
}
TutorialCtrl.first_steps.steps.slide_show_outro.approve = function() {
	TutorialCtrl.first_steps.steps.slide_show_outro.cancel();
};
TutorialCtrl.first_steps.steps.slide_show_outro.cancel = function() {
	try {
		TutorialCtrl.first_steps.steps.slide_show_outro.box.destroy();
		TutorialCtrl.first_steps.steps.slide_show_outro.box = null;
	}
	catch(e){}
}
TutorialCtrl.first_steps.steps.slide_show_outro.hide = function() {
	TutorialCtrl.first_steps.steps.slide_show_outro.box.hide();
}
TutorialCtrl.first_steps.steps.slide_show_outro.show = function() {
	TutorialCtrl.first_steps.steps.slide_show_outro.box.show();
}


/**
 * Der Schritt führt den Home-Sector ein
 */
TutorialCtrl.first_steps.steps.home_sector.text = '';
TutorialCtrl.first_steps.steps.home_sector.flag_to_set = 0;
TutorialCtrl.first_steps.steps.home_sector.action = function(_this) {
	_this = _this || TutorialCtrl.first_steps.steps.home_sector;
	try {
		SectorDetails.this_window.destroy();
	} catch(e) {};
	var _top = MammunUI.get_top_frame();
	var arrow_parent = (MapUI.no_iframes) ? document.getElemById('mapx')
		: _top.mapx.document.getElementsByTagName('body')[0];
	
	_top.MapUI.scroll_to_hs();
	
	var mp = _top.MapDataCtrl.mammun_map;
	var p = mp.get_field_pixel_position(
		_top.MapDataCtrl.my_homesector_field_pos.x,
		_top.MapDataCtrl.my_homesector_field_pos.y
	);
	var w = mp.get_field_width() >> 1;
	var h = mp.get_field_height() >> 1
	
	_this.arrow = new PureFW.FilmAnimator(
		arrow_parent,
		MapDataCtrl.mammun_map.position.x + p.x + w - (81>>1),
		MapDataCtrl.mammun_map.position.y + p.y - 30,
		81, 56, true);
	_this.arrow.set_image(
		'ui/animations/animated-red-8bit-6frames-bouncing.png', 6);
	_this.arrow.set_z_index(30);
	_this.arrow.start_animation();

	/**
	 * Officer-Box
	 */
	TutorialCtrl.create_tut_officer_box(_this.text);
	
	/**
	 * Handler
	 */
	_this.sector_click_handler = (function(__this) {
		return function(ev)
		{
			var sector = ev.detail.x + 10*ev.detail.y;
			if (sector == MammunUI.get_top_frame().MapDataCtrl.my_homesector) {
				TutorialCtrl.next(
					TutorialCtrl.first_steps,
					__this
				);
			}
		}
	})(_this);
	_top.MapDataCtrl.mammun_map.add_event_handler("field_click",
		_this.sector_click_handler
	);
	
}
TutorialCtrl.first_steps.steps.home_sector.approve = function() {
	TutorialCtrl.first_steps.steps.home_sector.cancel();
}
TutorialCtrl.first_steps.steps.home_sector.cancel = function() {
	var _top = MammunUI.get_top_frame();
	_top.MapDataCtrl.mammun_map.remove_event_handler("field_click",
		TutorialCtrl.first_steps.steps.home_sector.sector_click_handler
	);
	try {
		TutorialCtrl.first_steps.steps.home_sector.arrow.destroy();
	} 
	catch(e) {}
	TutorialCtrl.first_steps.steps.home_sector.arrow = null;
	TutorialCtrl.destroy_tut_officer_box();
}
TutorialCtrl.first_steps.steps.home_sector.hide = function() {
	try {
		TutorialCtrl.first_steps.steps.home_sector.arrow.hide();
	}
	catch(e){}
	TutorialCtrl.hide_tut_officer_box();
}
TutorialCtrl.first_steps.steps.home_sector.show = function() {
	try {
		TutorialCtrl.first_steps.steps.home_sector.arrow.show();
	}
	catch(e){}
	TutorialCtrl.show_tut_officer_box();
}

/**
 * Bei diesem Schritt wählt der Spieler einen Mammun aus, den er bewegen will.
 */
TutorialCtrl.first_steps.steps.choose_unit_move.text = '';
TutorialCtrl.first_steps.steps.choose_unit_move.flag_to_set = 0;
TutorialCtrl.first_steps.steps.choose_unit_move.action = function(_this) {
	_this = _this || TutorialCtrl.first_steps.steps.choose_unit_move;
	var _top = MammunUI.get_top_frame();
	
	/**
	 * Officer hijacked!
	 * Egal, welcher Text welchem Officer auch immer gegeben wird, es wird
	 * unserer angezeigt.
	 */
	TutorialCtrl.hijack_officer(
		_this.text
	);
	
	if ( (typeof(SectorDetails) == 'undefined') || !SectorDetails) {
		var hs = _top.MapDataCtrl.my_homesector_field_pos;
		var ev = PureFW.EventUtil.create_event("click", 
			new PureFW.Point(hs.x, hs.y)
		);
		_top.MapDataCtrl.sector_click(ev);
		
		_this.waiting_interval =
			PureFW.Timeout.set_interval(
				(function(__this) {
					return function() {
						if ((typeof(SectorDetails) != 'undefined')
							&& SectorDetails && SectorDetails.loaded)
						{
							PureFW.Timeout.clear_interval(
								__this.waiting_interval
							);
							
							TutorialCtrl.first_steps.steps.choose_unit_move
								.action2(__this);
						}
					}
				})(_this), 500
			);
	}
	else {
		TutorialCtrl.first_steps.steps.choose_unit_move.action2(_this);
	}
}
TutorialCtrl.first_steps.steps.choose_unit_move.action2 = function(_this) {
	_this = _this || TutorialCtrl.first_steps.steps.choose_unit_move;

	var p;
	var units = SectorDetails.sector_zoom.get_unit_widgets();
	var n = units.length;
	for (var i = 0; i < n; i++) {
		if (units[i].is_my_unit) {
			p = units[i].position;
			w = units[i].width;
			h = units[i].height;
			break;
		}
	}
	
	if (typeof(p) == 'undefined') {
		/**
		 * Es wurde keine Einheit im HS gefunden, also kennt er den Schritt
		 * schon, denn er muss sie ja bewegt haben. Also können wir den Schritt 
		 * überspringen.
		 */
		TutorialCtrl.next(
			TutorialCtrl.first_steps,
			_this
		);
		return;
	}
	p.x += (parseInt(units[i].parent_node.style.left)
			/ PureFW.WidgetManager.manager_all.get_scale_factor())
		+ SectorDetails.sector_zoom.position.x
		+ SectorDetails.this_window.position.x;
	p.y += (parseInt(units[i].parent_node.style.top)
			/ PureFW.WidgetManager.manager_all.get_scale_factor())
		+ SectorDetails.sector_zoom.position.y
		+ SectorDetails.this_window.position.y;
	
	_this.arrow	= new PureFW.FilmAnimator(
		document.body,
		p.x + (w - 81>>1),
		p.y - 56,
		81, 56
	);
	
	_this.arrow.set_image(
		'ui/animations/animated-red-8bit-6frames-bouncing.png', 6);
	_this.arrow.set_z_index(
		SectorDetails.this_window.get_z_index() + 1);
	_this.arrow.start_animation();

	/**
	 * Handler
	 */
	_this.sector_close_handler = (function(__this) {
		return function(ev) 
			{
				PureFW.Timeout.clear_interval(TutorialCtrl.first_steps.steps
					.choose_unit_move.check_still_in_sector);
		
				if (MammunUI.get_top_frame()
					.MapDataCtrl.Movement.from_sector == -1) 
				{
					TutorialCtrl.previous(
						TutorialCtrl.first_steps,
						__this
					);
				}
				else {
					TutorialCtrl.next(
						TutorialCtrl.first_steps,
						__this
					);
				}
			}
		})(_this)
	
	SectorDetails.this_window.add_event_handler("destroy", 
		_this.sector_close_handler
	);

	/**
	 * Das ist eine Sicherung, falls der Destruktor aus irgendwelchen Gründen
	 * nicht ausgeführt wird. Beim Tutorial steht Stabilität ganz weit oben.
	 */
	TutorialCtrl.first_steps.steps.choose_unit_move.check_still_in_sector =
		PureFW.Timeout.set_interval((function(__this) {
			return function(ev) {
				if ((typeof(SectorDetails) == 'undefined')
					|| !SectorDetails || !SectorDetails.loaded)
				{
					__this.sector_close_handler(ev);
				}
			}
		})(_this), 500);
}
TutorialCtrl.first_steps.steps.choose_unit_move.approve = function() {
	TutorialCtrl.first_steps.steps.choose_unit_move.cancel();
}
TutorialCtrl.first_steps.steps.choose_unit_move.cancel = function() {
	try {
		TutorialCtrl.first_steps.steps.choose_unit_move.arrow.destroy();
		TutorialCtrl.first_steps.steps.choose_unit_move.arrow = null;
	}
	catch(e){}
	TutorialCtrl.free_officer();
}
TutorialCtrl.first_steps.steps.choose_unit_move.hide = function() {
	try {
		TutorialCtrl.first_steps.steps.choose_unit_move.arrow.hide();
	}
	catch(e) {}
}
TutorialCtrl.first_steps.steps.choose_unit_move.show = function() {
	try {
		TutorialCtrl.first_steps.steps.choose_unit_move.arrow.show();
	}
	catch(e) {}
}

/**
 * Bei diesem Schritt gibt der Spieler dem Mammun den Zielsektor an, zu welchem
 * er sich bewegen soll.
 */
TutorialCtrl.first_steps.steps.move_unit.text = '';
TutorialCtrl.first_steps.steps.move_unit.flag_to_set = 0;
TutorialCtrl.first_steps.steps.move_unit.action = function(_this) {
	_this = _this || TutorialCtrl.first_steps.steps.move_unit;
	try {
		SectorDetails.this_window.destroy();
	} catch(e) {};
	if (MammunUI.get_top_frame().MapDataCtrl.Movement.from_sector == -1) {
		/**
		 * Wir sind hier gelandet, aber keine Einheit ist gerade in der 
		 * Bewegung. Eigentlich wäre das ein Grund zurückzugehen, aber wir
		 * überspringen den Schritt und gehen zum nächsten, weil wir davon
		 * ausgehen, dass der Fall nur eintritt, wenn der Spieler den letzten
		 * Schritt übersprungen hat, weil er bereits Einheiten bewegt hat.
		 */
		TutorialCtrl.next(
			TutorialCtrl.first_steps,
			_this
		);
		return;
	}
		
	
	var _top = MammunUI.get_top_frame();
	
	_top.MapUI.scroll_to_hs();
	
	var mp = _top.MapDataCtrl.mammun_map;
	var hs = _top.MapDataCtrl.my_homesector;
	
	var arrow_parent = (MapUI.no_iframes) ? document.getElemById('mapx')
		: _top.mapx.document.getElementsByTagName('body')[0];
	_this.arrows = new Array();
	var cur_arrow = null;
	
	var movements = new Array();
	if (MapDataCtrl.movements[hs]) {
		for (var heading in MapDataCtrl.movements[hs]) {
			if (isNaN(heading))
				continue;
			switch(parseInt(heading)) {
			case 0:
				movements.push(hs+1);
				break;
			case 1:
				movements.push(hs-1);
				break;
			case 2:
				movements.push(hs+10);
				break;
			case 3:
				movements.push(hs-10);
				break;
			}
		}
	}
	var dirs = new Array(-1, -10, 1, 10);
	var n = dirs.length;
	for (var i = 0; i < n; i++) {
		var s = hs + dirs[i];
		if (MapDataCtrl.resources[s] && (!MapDataCtrl.dominators[s]
			|| (MapDataCtrl.dominators[s] == MapDataCtrl.my_nick))
			&& !movements.contains(s))
		{
			// Hier ist ein möglicher Ort, wo er seinen Mammun hinbewegen kann.
			var p = mp.get_field_pixel_position(s%10, Math.floor(s/10));
			var w = mp.get_field_width() >> 1;
			var h = mp.get_field_height() >> 1
			
			cur_arrow = new PureFW.FilmAnimator(
				arrow_parent,
				MapDataCtrl.mammun_map.position.x + p.x + w - (81>>1),
				MapDataCtrl.mammun_map.position.y + p.y - 30,
				81, 56,
				true
			);
			cur_arrow.set_image(
				'ui/animations/animated-red-8bit-6frames-bouncing.png', 6);
			cur_arrow.set_z_index(30);
			cur_arrow.start_animation();
			_this.arrows.push(cur_arrow);
		}
	}
	
	/**
	 * Officer-Box
	 */
	TutorialCtrl.create_tut_officer_box(
		_this.text
	);
	
	/**
	 * Handlers
	 */
	_this.unit_moved_handler = 
		(function(__this) 
		{
			return function(ev)
			{
				TutorialCtrl.next(
					TutorialCtrl.first_steps,
					__this
				);
			}
		})(_this);
	
	_this.unit_not_moved_handler =
		(function(__this) 
		{
			return function(ev)
			{
				TutorialCtrl.previous(
					TutorialCtrl.first_steps,
					__this
				);
			}
		})(_this);
	
	MapDataCtrl.Movement.add_event_handler("move_successful",
		_this.unit_moved_handler
	);
	MapDataCtrl.Movement.add_event_handler("move_unsuccessful",
		_this.unit_not_moved_handler
	);
}
TutorialCtrl.first_steps.steps.move_unit.approve = function() {
	TutorialCtrl.first_steps.steps.move_unit.cancel();
}
TutorialCtrl.first_steps.steps.move_unit.cancel = function() {
	try {
		MapDataCtrl.Movement.remove_event_handler("move_successful",
			TutorialCtrl.first_steps.steps.move_unit.unit_moved_handler
		);
		MapDataCtrl.Movement.remove_event_handler("move_unsuccessful",
			TutorialCtrl.first_steps.steps.move_unit.unit_not_moved_handler
		);
	}
	catch(e) {}
	try {
		TutorialCtrl.first_steps.steps.move_unit.arrows.destroy();
	}
	catch(e){}
	TutorialCtrl.first_steps.steps.move_unit.arrows = null;
	
	TutorialCtrl.destroy_tut_officer_box();
}
TutorialCtrl.first_steps.steps.move_unit.hide = function() {
	var n = TutorialCtrl.first_steps.steps.move_unit.arrows.length;
	for (var i = 0; i < n; i++) {
		try {
			TutorialCtrl.first_steps.steps.move_unit.arrows[i].hide();
		}
		catch(e) {}
	}
	TutorialCtrl.hide_tut_officer_box();
}
TutorialCtrl.first_steps.steps.move_unit.show = function() {
	var n = TutorialCtrl.first_steps.steps.move_unit.arrows.length;
	for (var i = 0; i < n; i++) {
		try {
			TutorialCtrl.first_steps.steps.move_unit.arrows[i].show();
		}
		catch(e) {}
	}
	TutorialCtrl.show_tut_officer_box();
}

/**
 * Bei diesem Schritt wird der Spieler angewiesen, den Bewegungspfeil 
 * anzuklicken, um zu sehen, wie lange sein Mammun exakt noch läuft.
 */
TutorialCtrl.first_steps.steps.click_move_arrow.text = '';
TutorialCtrl.first_steps.steps.click_move_arrow.flag_to_set = 0;
TutorialCtrl.first_steps.steps.click_move_arrow.action = function() {
	try {
		SectorDetails.this_window.destroy();
	} catch(e) {};
	
	
	var _top = MammunUI.get_top_frame();
	var mp = _top.MapDataCtrl.mammun_map;
	var hs = _top.MapDataCtrl.my_homesector;
	var hs_pos = _top.MapDataCtrl.my_homesector_field_pos;
	
	var moving = MapDataCtrl.movements[hs];
	/**
	 * Es wurde niemand bewegt. Eigentlich müssten wir jetzt einen Schritt
	 * zurück, aber die Policy "skip movement on error" zwingt uns zum
	 * Überspringen.
	 */
	if (!moving || moving.length == 0) {
		TutorialCtrl.next(
			TutorialCtrl.first_steps,
			TutorialCtrl.first_steps.steps.click_move_arrow
		);
		return;
	}
	
	
	var arrow_parent = (MapUI.no_iframes) ? document.getElemById('mapx')
		: _top.mapx.document.getElementsByTagName('body')[0];
	
	var p = mp.get_field_pixel_position(hs_pos.x, hs_pos.y);
	var w = mp.get_field_width();
	var h = mp.get_field_height();
	
	var x_offset = 0;
	var y_offset = 0;
	/**
	 * Achtung: Copy/Paste auf Mapprint.js
	 * 
	 * TODO: Position von MammunMap bekommen
	 */
	var arrow_attr = new Array(
		{x:w-40-17,			y:h-34-20},
		{x:17,				y:20},
		{x:17,				y:h-34-20},
		{x:w-40-17,			y:20}
	);
	var arrow_w = 40;
	var arrow_h = 34;
	
	var arrow_offset;
	for (var heading in moving) {
		if (isNaN(heading))
			continue;
		
		heading = parseInt(heading);
		break;
	}
	arrow_offset = arrow_attr[heading];

	TutorialCtrl.first_steps.steps.click_move_arrow.arrow 
			= new PureFW.FilmAnimator
	(
		arrow_parent,
		MapDataCtrl.mammun_map.position.x + p.x + arrow_offset.x 
			+ ((arrow_w - 81)>>1),
		MapDataCtrl.mammun_map.position.y + p.y + arrow_offset.y - 56,
		81, 56,
		true
	);
	TutorialCtrl.first_steps.steps.click_move_arrow.arrow.set_image(
		'ui/animations/animated-red-8bit-6frames-bouncing.png', 6);
	TutorialCtrl.first_steps.steps.click_move_arrow.arrow.set_z_index(30);
	TutorialCtrl.first_steps.steps.click_move_arrow.arrow.start_animation();
	
	/**
	 * Officer-Box
	 */
	TutorialCtrl.create_tut_officer_box(
		TutorialCtrl.first_steps.steps.click_move_arrow.text
	);
	
	/**
	 * Timer, der checkt, ob ShowMovingUnits-Dialog geöffnet wurde
	 */
	TutorialCtrl.first_steps.steps.click_move_arrow.check_timer =
		PureFW.Timeout.set_interval(
			function(ev) {
				var _top = MammunUI.get_top_frame();
				var _mapx = (MapUI.no_iframes) ? _top : _top.mapx;

				if ((typeof(_mapx.ShowMovingUnits) != 'undefined')
					&& _mapx.ShowMovingUnits)
				{
					PureFW.Timeout.clear_interval(
						TutorialCtrl.first_steps.steps.click_move_arrow
							.check_timer
					);
					TutorialCtrl.next(
						TutorialCtrl.first_steps,
						TutorialCtrl.first_steps.steps.click_move_arrow
					);
				}
			}, 500
		);
}
TutorialCtrl.first_steps.steps.click_move_arrow.approve = function() {
	TutorialCtrl.first_steps.steps.click_move_arrow.cancel();
}
TutorialCtrl.first_steps.steps.click_move_arrow.cancel = function() {
	try {
		TutorialCtrl.first_steps.steps.click_move_arrow.arrow.destroy();
	}
	catch(e){}
	TutorialCtrl.first_steps.steps.click_move_arrow.arrow = null;
	
	TutorialCtrl.destroy_tut_officer_box();
}
TutorialCtrl.first_steps.steps.click_move_arrow.hide = function() {
	try {
		TutorialCtrl.first_steps.steps.click_move_arrow.arrow.hide();
	}
	catch(e){}
	
	TutorialCtrl.hide_tut_officer_box();
}
TutorialCtrl.first_steps.steps.click_move_arrow.show = function() {
	try {
		TutorialCtrl.first_steps.steps.click_move_arrow.arrow.show();
	}
	catch(e){}
	
	TutorialCtrl.show_tut_officer_box();
}


/**
 * Bei diesem Schritt wird der Spieler angewiesen, den Move-Arrow-Dialog 
 * wieder zu schließen.
 */
TutorialCtrl.first_steps.steps.close_move_arrow.text = '';
TutorialCtrl.first_steps.steps.close_move_arrow.flag_to_set = 0;
TutorialCtrl.first_steps.steps.close_move_arrow.action = function() {
	try {
		SectorDetails.this_window.destroy();
	} catch(e) {};
	
	var _top = MammunUI.get_top_frame();
	var _mapx = (MapUI.no_iframes) ? _top : _top.mapx;
	var mp = _top.MapDataCtrl.mammun_map;
	var hs = _top.MapDataCtrl.my_homesector;
	
	var moving = MapDataCtrl.movements[hs];
	/**
	 * Wenn sich nichts bewegt, müssten wir eigentlich zurück, aber wir 
	 * überspringen, weil Truppenbewegungen als "überspringbar" eingestuft
	 * sind.
	 */
	if (!moving || moving.length == 0) {
		TutorialCtrl.next(
			TutorialCtrl.first_steps,
			TutorialCtrl.first_steps.steps.close_move_arrow
		);
		return;
	}

	/**
	 * Wenn das ShowMovingUnits-Objekt noch nicht bereit ist, dann warten...
	 */
	if ((typeof(_mapx.ShowMovingUnits) == 'undefined')
		|| _mapx.ShowMovingUnits)
	{
		TutorialCtrl.first_steps.steps.close_move_arrow.waiting_interval =
			PureFW.Timeout.set_interval(
				function() {
					if ((typeof(_mapx.ShowMovingUnits) != 'undefined')
						&& _mapx.ShowMovingUnits)
					{
						PureFW.Timeout.clear_interval(
							TutorialCtrl.first_steps.steps
								.close_move_arrow.waiting_interval
						);
						
						TutorialCtrl.first_steps.steps.close_move_arrow
							.action2();
					}
				}, 500
			);
	}
	else {
		TutorialCtrl.first_steps.steps.close_move_arrow.action2();
	}
}
TutorialCtrl.first_steps.steps.close_move_arrow.action2 = function() {
	var _top = MammunUI.get_top_frame();
	var _mapx = (MapUI.no_iframes) ? _top : _top.mapx;
	var p = MapDataCtrl.movement_array_info_balloon.get_position();
	var offset = MapDataCtrl.movement_array_info_balloon.close_button
		.get_position();
	var w = MapDataCtrl.movement_array_info_balloon.close_button.get_width();
	
	var arrow_parent = (MapUI.no_iframes) ? document.getElemById('mapx')
		: _top.mapx.document.getElementsByTagName('body')[0];
	
	TutorialCtrl.first_steps.steps.close_move_arrow.arrow 
											= new PureFW.FilmAnimator (
		arrow_parent,
		p.x + offset.x + ((w - 81)>>1),
		p.y + offset.y - 56,
		81, 56,
		true
	);

	TutorialCtrl.first_steps.steps.close_move_arrow.arrow.set_image(
		'ui/animations/animated-red-8bit-6frames-bouncing.png', 6);
	TutorialCtrl.first_steps.steps.close_move_arrow.arrow.set_z_index(30);
	TutorialCtrl.first_steps.steps.close_move_arrow.arrow.start_animation();
	/**
	* Officer-Box
	*/
	TutorialCtrl.create_tut_officer_box(
		TutorialCtrl.first_steps.steps.close_move_arrow.text
	);

	/**
	 * Timer, der checkt, ob ShowMovingUnits-Dialog wieder zu ist
	 */
	TutorialCtrl.first_steps.steps.close_move_arrow.check_timer =
		PureFW.Timeout.set_interval(
			function(ev) {
				if (!MapDataCtrl.movement_array_info_balloon
					|| MapDataCtrl.movement_array_info_balloon.is_hidden())
				{
					PureFW.Timeout.clear_interval(
						TutorialCtrl.first_steps.steps.close_move_arrow
							.check_timer
					);
					TutorialCtrl.next(
						TutorialCtrl.first_steps,
						TutorialCtrl.first_steps.steps.close_move_arrow
					);
				}
			}, 500
		);
}
TutorialCtrl.first_steps.steps.close_move_arrow.approve = function() {
	TutorialCtrl.first_steps.steps.close_move_arrow.cancel();
}
TutorialCtrl.first_steps.steps.close_move_arrow.cancel = function() {
	try {
		TutorialCtrl.first_steps.steps.close_move_arrow.arrow.destroy();
	}
	catch(e){}
	TutorialCtrl.first_steps.steps.close_move_arrow.arrow = null;
	
	TutorialCtrl.destroy_tut_officer_box();
}
TutorialCtrl.first_steps.steps.close_move_arrow.hide = function() {
	try {
		TutorialCtrl.first_steps.steps.close_move_arrow.arrow.hide();
	}
	catch(e){}
	
	TutorialCtrl.hide_tut_officer_box();
}
TutorialCtrl.first_steps.steps.close_move_arrow.show = function() {
	try {
		TutorialCtrl.first_steps.steps.close_move_arrow.arrow.show();
	}
	catch(e){}
	
	TutorialCtrl.show_tut_officer_box();
}


/**
 * Bei diesem Schritt wird der Spieler aufgefordert, den Forschungsdialog
 * zu öffnen.
 */
TutorialCtrl.first_steps.steps.open_science.text = '';
TutorialCtrl.first_steps.steps.open_science.flag_to_set = 0;
TutorialCtrl.first_steps.steps.open_science.action = function() {
	try {
		SectorDetails.this_window.destroy();
	} catch(e) {};
	UIButtons.bb3.show();
	TutorialCtrl.first_steps.steps.open_science.bb_z_index = 
		UIButtons.bottom_container.get_z_index();
	UIButtons.bottom_container.set_z_index(120);
	
	TutorialCtrl.first_steps.steps.open_science.arrow = new PureFW.FilmAnimator(
		document.body,
		UIButtons.bottom_container.position.x +
			UIButtons.bb3.position.x + ((UIButtons.bb3.width - 81)>>1),
		UIButtons.bottom_container.position.y +
			UIButtons.bb3.position.y - 46,
		81, 56);
	TutorialCtrl.first_steps.steps.open_science.arrow.set_image(
		'ui/animations/animated-red-8bit-6frames-bouncing.png', 6);
	TutorialCtrl.first_steps.steps.open_science.arrow.set_z_index(
		UIButtons.bottom_container.get_z_index()+1);
	TutorialCtrl.first_steps.steps.open_science.arrow.start_animation();

	/**
	 * Officer-Box
	 */
	TutorialCtrl.create_tut_officer_box(
		TutorialCtrl.first_steps.steps.open_science.text
	);
	
	/**
	 * Handlers
	 */
	TutorialCtrl.first_steps.steps.open_science.cso_click_handler =
		function(ev)
	{
		TutorialCtrl.next(
			TutorialCtrl.first_steps,
			TutorialCtrl.first_steps.steps.open_science
		);
	}
	
	UIButtons.bb3.add_event_handler("click",
		TutorialCtrl.first_steps.steps.open_science.cso_click_handler
	);
}

TutorialCtrl.first_steps.steps.open_science.approve = function() {
	TutorialCtrl.first_steps.steps.open_science.cancel();
}

TutorialCtrl.first_steps.steps.open_science.cancel = function() {
	try {
		TutorialCtrl.first_steps.steps.open_science.arrow.destroy();
	}
	catch(e) {}
	TutorialCtrl.first_steps.steps.open_science.arrow = null;
	
	TutorialCtrl.destroy_tut_officer_box();
	
	UIButtons.bb3.remove_event_handler("click",
		TutorialCtrl.first_steps.steps.open_science.cso_click_handler
	);
	UIButtons.bottom_container.set_z_index(
		TutorialCtrl.first_steps.steps.open_science.bb_z_index
	);
}
TutorialCtrl.first_steps.steps.open_science.hide = function() {
	try {
		TutorialCtrl.first_steps.steps.open_science.arrow.hide();
	}
	catch(e) {}
	
	TutorialCtrl.hide_tut_officer_box();
}
TutorialCtrl.first_steps.steps.open_science.show = function() {
	try {
		TutorialCtrl.first_steps.steps.open_science.arrow.show();
	}
	catch(e) {}
	
	TutorialCtrl.show_tut_officer_box();
}

/**
 * Bei diesem Schritt wird der Spieler aufgefordert, eine Forschung auszuwählen.
 */
TutorialCtrl.first_steps.steps.start_research.text = '';
TutorialCtrl.first_steps.steps.start_research.flag_to_set = 0;
TutorialCtrl.first_steps.steps.start_research.action = function() {
	UIButtons.bb3.show();
	
	/**
	 * Officer hijacked!
	 * Egal, welcher Text welchem Officer auch immer gegeben wird, es wird
	 * unserer angezeigt.
	 */
	TutorialCtrl.hijack_officer(
		TutorialCtrl.first_steps.steps.start_research.text
	);
	
	/**
	 * Wenn das Science-Objekt noch nicht bereit ist, dann warten...
	 */
	if ( (typeof(Science) == 'undefined') || !Science || !Science.loaded) {
		/**
		 * Wenn das Forschungsfenster noch nicht mal geöffnet wurde, dann
		 * öffnen
		 */
		if (get_cur_fixed_window_url().indexOf('science.php') < 0)
			UIButtons.open_window('science.php');
		
		TutorialCtrl.first_steps.steps.start_research.waiting_interval =
			PureFW.Timeout.set_interval(
				function() {
					if ((typeof(Science) != 'undefined')
						&& Science && Science.loaded)
					{
						PureFW.Timeout.clear_interval(
							TutorialCtrl.first_steps.steps
								.start_research.waiting_interval
						);
						
						TutorialCtrl.first_steps.steps.start_research
							.action2();
					}
				}, 500
			);
	}
	else {
		TutorialCtrl.first_steps.steps.start_research.action2();
	}
}
TutorialCtrl.first_steps.steps.start_research.action2 = function() {
	TutorialCtrl.first_steps.steps.start_research.arrow = new PureFW.FilmAnimator(
		document.body,
		240, 230-56,
		81, 56);
	TutorialCtrl.first_steps.steps.start_research.arrow.set_image(
		'ui/animations/animated-red-8bit-6frames-bouncing.png', 6);
	TutorialCtrl.first_steps.steps.start_research.arrow.set_z_index(200);
	TutorialCtrl.first_steps.steps.start_research.arrow.start_animation();
	
	/**
	 * Handlers
	 */
	TutorialCtrl.first_steps.steps.start_research.science_click_handler 
		= function(ev, chosen_sc)
	{
		PureFW.Timeout.clear_interval(
			TutorialCtrl.first_steps.steps.start_research.check_still_in_dialog
		);
		TutorialCtrl.next(
			TutorialCtrl.first_steps,
			TutorialCtrl.first_steps.steps.start_research
		);
	}
	TutorialCtrl.first_steps.steps.start_research.window_close_handler 
		= function(ev)
	{
		PureFW.Timeout.clear_interval(
			TutorialCtrl.first_steps.steps.start_research.check_still_in_dialog
		);
		TutorialCtrl.previous(
			TutorialCtrl.first_steps,
			TutorialCtrl.first_steps.steps.start_research
		);
	}

	Science.research_started_functions.push(
		TutorialCtrl.first_steps.steps.start_research.science_click_handler
	);
	
	Science.available_science_clicked_functions.push(
		function(ev) {
			TutorialCtrl.first_steps.steps.start_research.arrow.hide();
		}
	);
	
	Science.research_not_started_functions.push(
		function(ev) {
			TutorialCtrl.first_steps.steps.start_research.arrow.show();
		}
	);
	
	/**
	 * Destroy-Event-Handler haben einen Bug, wie's scheint. Deswegen läuft
	 * das hier über die bewährte Sicherung.
	 */
	TutorialCtrl.first_steps.steps.start_research.check_still_in_dialog =
		PureFW.Timeout.set_interval(
			function(ev) {
				if ((typeof(Science) == 'undefined')
					|| !Science || !Science.loaded)
				{
					TutorialCtrl.first_steps.steps.start_research
						.window_close_handler(ev);
				}
			}, 500
		);
}

TutorialCtrl.first_steps.steps.start_research.approve = function() {
	TutorialCtrl.first_steps.steps.start_research.cancel();
}

TutorialCtrl.first_steps.steps.start_research.cancel = function() {
	try {
		Science.research_started_functions.remove(
			TutorialCtrl.first_steps.steps.start_research.science_click_handler
		);
	}
	catch(e) {}
	
	TutorialCtrl.free_officer();
	try {
		TutorialCtrl.first_steps.steps.start_research.arrow.destroy()
	}
	catch(e) {}
}
TutorialCtrl.first_steps.steps.start_research.hide = function() {
	try {
		TutorialCtrl.first_steps.steps.start_research.arrow.hide()
	}
	catch(e) {}
}
TutorialCtrl.first_steps.steps.start_research.show = function() {
	try {
		TutorialCtrl.first_steps.steps.start_research.arrow.show()
	}
	catch(e) {}
}

/**
 * Dieser Schritt schließt das Forschen ab. Der Spieler wird aufgefordert, den
 * Screen zu verlassen.
 */
TutorialCtrl.first_steps.steps.quit_research.text = '';
TutorialCtrl.first_steps.steps.quit_research.flag_to_set = 0;
TutorialCtrl.first_steps.steps.quit_research.action = function() {
	UIButtons.bb3.show();
	
	/**
	 * Officer hijacked!
	 * Egal, welcher Text welchem Officer auch immer gegeben wird, es wird
	 * unserer angezeigt.
	 */
	TutorialCtrl.hijack_officer(
		TutorialCtrl.first_steps.steps.quit_research.text
	);
	
	/**
	 * Wenn das Science-Objekt noch nicht bereit ist, dann warten...
	 */
	if ( (typeof(Science) == 'undefined') || !Science || !Science.loaded) {
		/**
		 * Wenn das Forschungsfenster noch nicht mal geöffnet wurde, dann
		 * öffnen
		 */
		if (get_cur_fixed_window_url().indexOf('science.php') < 0)
			UIButtons.open_window('science.php');
		
		TutorialCtrl.first_steps.steps.quit_research.waiting_interval =
			PureFW.Timeout.set_interval(
				function() {
					if ((typeof(Science) != 'undefined')
						&& Science && Science.loaded)
					{
						PureFW.Timeout.clear_interval(
							TutorialCtrl.first_steps.steps
								.quit_research.waiting_interval
						);
						
						TutorialCtrl.first_steps.steps.quit_research
							.action2();
					}
				}, 500
			);
	}
	else {
		TutorialCtrl.first_steps.steps.quit_research.action2();
	}
}
TutorialCtrl.first_steps.steps.quit_research.action2 = function() {
	TutorialCtrl.first_steps.steps.quit_research.arrow = new PureFW.FilmAnimator(
		document.body,
		930, 122-56,
		81, 56);
	TutorialCtrl.first_steps.steps.quit_research.arrow.set_image(
		'ui/animations/animated-red-8bit-6frames-bouncing.png', 6);
	TutorialCtrl.first_steps.steps.quit_research.arrow.set_z_index(200);
	TutorialCtrl.first_steps.steps.quit_research.arrow.start_animation();
	
	/**
	 * Handlers
	 */
	TutorialCtrl.first_steps.steps.quit_research.window_close_handler 
		= function(ev)
	{
		PureFW.Timeout.clear_interval(
			TutorialCtrl.first_steps.steps.quit_research.check_still_in_dialog
		);
		TutorialCtrl.next(
			TutorialCtrl.first_steps,
			TutorialCtrl.first_steps.steps.quit_research
		);
	}

	/**
	 * Destroy-Event-Handler haben einen Bug, wie's scheint. Deswegen läuft
	 * das hier über die bewährte Sicherung.
	 */
	TutorialCtrl.first_steps.steps.quit_research.check_still_in_dialog =
		PureFW.Timeout.set_interval(
			function(ev) {
				if ((typeof(Science) == 'undefined')
					|| !Science || !Science.loaded)
				{
					TutorialCtrl.first_steps.steps.quit_research
						.window_close_handler(ev);
				}
			}, 1500
		);
}

TutorialCtrl.first_steps.steps.quit_research.approve = function() {
	TutorialCtrl.first_steps.steps.quit_research.cancel();
}

TutorialCtrl.first_steps.steps.quit_research.cancel = function() {
	try {
		Science.research_started_functions.remove(
			TutorialCtrl.first_steps.steps.quit_research.science_click_handler
		);
	}
	catch(e) {}
	try {
		TutorialCtrl.first_steps.steps.quit_research.arrow.destroy();
	}
	catch(e) {}
	
	TutorialCtrl.free_officer();	
}
TutorialCtrl.first_steps.steps.quit_research.hide = function() {
	try {
		TutorialCtrl.first_steps.steps.quit_research.arrow.hide();
	}
	catch(e) {}
}
TutorialCtrl.first_steps.steps.quit_research.show = function() {
	try {
		TutorialCtrl.first_steps.steps.quit_research.arrow.show();
	}
	catch(e) {}
}

/**
 * Bei diesem Schritt wird der Spieler aufgefordert, den Trainingsdialog
 * zu öffnen.
 */
TutorialCtrl.first_steps.steps.open_training.text = '';
TutorialCtrl.first_steps.steps.open_training.flag_to_set = 0;
TutorialCtrl.first_steps.steps.open_training.action = function() {
	try {
		SectorDetails.this_window.destroy();
	} catch(e) {};
	UIButtons.bb1.show();
	TutorialCtrl.first_steps.steps.open_training.bb_z_index = 
		UIButtons.bottom_container.get_z_index();
	UIButtons.bottom_container.set_z_index(120);
	
	TutorialCtrl.first_steps.steps.open_training.arrow = new PureFW.FilmAnimator(
		document.body,
		UIButtons.bottom_container.position.x +
			UIButtons.bb1.position.x + ((UIButtons.bb1.width - 81)>>1),
		UIButtons.bottom_container.position.y +
			UIButtons.bb1.position.y - 46,
		81, 56);
	TutorialCtrl.first_steps.steps.open_training.arrow.set_image(
		'ui/animations/animated-red-8bit-6frames-bouncing.png', 6);
	TutorialCtrl.first_steps.steps.open_training.arrow.set_z_index(
		UIButtons.bottom_container.get_z_index()+1);
	TutorialCtrl.first_steps.steps.open_training.arrow.start_animation();
	
	/**
	 * Officer-Box
	 */
	TutorialCtrl.create_tut_officer_box(
		TutorialCtrl.first_steps.steps.open_training.text
	);
	
	/**
	 * Handlers
	 */
	TutorialCtrl.first_steps.steps.open_training.cwo_click_handler =
		function(ev)
	{
		TutorialCtrl.next(
			TutorialCtrl.first_steps,
			TutorialCtrl.first_steps.steps.open_training
		);
	}
	
	UIButtons.bb1.add_event_handler("click",
		TutorialCtrl.first_steps.steps.open_training.cwo_click_handler
	);
}

TutorialCtrl.first_steps.steps.open_training.approve = function() {
	TutorialCtrl.first_steps.steps.open_training.cancel();
}

TutorialCtrl.first_steps.steps.open_training.cancel = function() {
	try {
		TutorialCtrl.first_steps.steps.open_training.arrow.destroy();
	}
	catch(e) {}
	TutorialCtrl.first_steps.steps.open_training.arrow = null;
	
	TutorialCtrl.destroy_tut_officer_box();
	
	UIButtons.bb1.remove_event_handler("click",
		TutorialCtrl.first_steps.steps.open_training.cwo_click_handler
	);
	UIButtons.bottom_container.set_z_index(
		TutorialCtrl.first_steps.steps.open_training.bb_z_index
	);
}
TutorialCtrl.first_steps.steps.open_training.hide = function() {
	try {
		TutorialCtrl.first_steps.steps.open_training.arrow.hide();
	}
	catch(e) {}
	
	TutorialCtrl.hide_tut_officer_box();
}
TutorialCtrl.first_steps.steps.open_training.show = function() {
	try {
		TutorialCtrl.first_steps.steps.open_training.arrow.show();
	}
	catch(e) {}
	
	TutorialCtrl.show_tut_officer_box();
}

/**
 * Bei diesem Schritt wird der Spieler aufgefordert, einen Slot auszuwählen,
 * um mit dem Training zu beginnen.
 */
TutorialCtrl.first_steps.steps.start_training.text = '';
TutorialCtrl.first_steps.steps.start_training.flag_to_set = 0;
TutorialCtrl.first_steps.steps.start_training.action = function() {
	UIButtons.bb1.show();
	
	/**
	 * Officer hijacked!
	 * Egal, welcher Text welchem Officer auch immer gegeben wird, es wird
	 * unserer angezeigt.
	 */
	TutorialCtrl.hijack_officer(
		TutorialCtrl.first_steps.steps.start_training.text
	);
	
	/**
	 * Wenn das UnitProduction-Objekt noch nicht bereit ist, dann warten...
	 */
	if ( (typeof(UnitProduction) == 'undefined') || !UnitProduction || 
			!UnitProduction.loaded) 
	{
		/**
		 * Wenn das Forschungsfenster noch nicht mal geöffnet wurde, dann
		 * einen Schritt zurück
		 */
		if (get_cur_fixed_window_url().indexOf('unit_production.php') < 0) {
			TutorialCtrl.previous(
				TutorialCtrl.first_steps,
				TutorialCtrl.first_steps.steps.start_training
			);
			return;
		}
		
		TutorialCtrl.first_steps.steps.start_training.waiting_interval =
			PureFW.Timeout.set_interval(
				function() {
					if ((typeof(UnitProduction) != 'undefined')
						&& UnitProduction && UnitProduction.loaded)
					{
						PureFW.Timeout.clear_interval(
							TutorialCtrl.first_steps.steps
								.start_training.waiting_interval
						);
						
						TutorialCtrl.first_steps.steps.start_training
							.action2();
					}
				}, 500
			);
	}
	else {
		TutorialCtrl.first_steps.steps.start_training.action2();
	}
}
TutorialCtrl.first_steps.steps.start_training.action2 = function() {
	TutorialCtrl.first_steps.steps.start_training.arrow = new PureFW.FilmAnimator(
		document.body,
		290, 250-56,
		81, 56);
	TutorialCtrl.first_steps.steps.start_training.arrow.set_image(
		'ui/animations/animated-red-8bit-6frames-bouncing.png', 6);
	TutorialCtrl.first_steps.steps.start_training.arrow.set_z_index(
		120);
	TutorialCtrl.first_steps.steps.start_training.arrow.start_animation();
	/**
	 * Handlers
	 */
	TutorialCtrl.first_steps.steps.start_training.training_started_handler
		= function(ev)
	{
		PureFW.Timeout.clear_interval(
			TutorialCtrl.first_steps.steps.start_training.check_still_in_dialog
		);
		TutorialCtrl.next(
			TutorialCtrl.first_steps,
			TutorialCtrl.first_steps.steps.start_training
		);
	}
	TutorialCtrl.first_steps.steps.start_training.window_close_handler 
		= function(ev)
	{
		PureFW.Timeout.clear_interval(
			TutorialCtrl.first_steps.steps.start_training.check_still_in_dialog
		);
		TutorialCtrl.previous(
			TutorialCtrl.first_steps,
			TutorialCtrl.first_steps.steps.start_training
		);
	}
	
	TutorialCtrl.first_steps.steps.start_training.check_where_we_are 
		= function(ev)
	{
		if ((typeof(UnitProduction) == 'undefined')
			|| !UnitProduction || !UnitProduction.loaded)
		{
			TutorialCtrl.first_steps.steps.start_training
				.window_close_handler(ev);
			return;
		}
		if (UnitProduction.slots.length == 0) {
			TutorialCtrl.first_steps.steps.start_training
				.training_started_handler(ev);
			return;
		}
		var n = UnitProduction.slots.length;
		if (n == 0) {
			/**
			 * Wenn es keine Unit-Slots (mehr) gibt, wir aber noch im Trainings-
			 * dialog sind, dann sind wir schon einen Schritt weiter. 
			 */
			TutorialCtrl.first_steps.steps.start_training
				.training_started_handler(ev);
			return;
		}
		for (var i = 0; i < n; i++) {
			/**
			 * Es wird bereits jemand trainiert => weiter
			 */
			if (UnitProduction.slots[i].current_unit) {
				TutorialCtrl.first_steps.steps.start_training
					.training_started_handler(ev);
				return;
			}
		}
	}
	
	/**
	 * Destroy-Event-Handler haben einen Bug, wie's scheint. Deswegen läuft
	 * das hier über die bewährte Sicherung.
	 */
	TutorialCtrl.first_steps.steps.start_training.check_still_in_dialog =
		PureFW.Timeout.set_interval(
			function(ev) {
				TutorialCtrl.first_steps.steps.start_training
					.check_where_we_are(ev)
			}, 500
		);
}

TutorialCtrl.first_steps.steps.start_training.approve = function() {
	TutorialCtrl.first_steps.steps.start_training.cancel();
}

TutorialCtrl.first_steps.steps.start_training.cancel = function() {
	try {
		UnitProduction.training_started_functions.remove(
			TutorialCtrl.first_steps.steps.start_training
				.training_started_handler
		);
	}
	catch(e) {}
	try {
		TutorialCtrl.first_steps.steps.start_training.arrow.destroy();
	}
	catch(e) {}
	try {
		var n = UnitProduction.slots.length;
		for (var i = 0; i < n; i++) {
			try {
				UnitProduction.slots[i].remove_event_handler("click", 
					TutorialCtrl.first_steps.steps.start_training
						.training_started_handler
				);
			}
			catch(e) {}
		}
	}
	catch(e){}
	
	// Free hijacked officer
	TutorialCtrl.free_officer();	
}
TutorialCtrl.first_steps.steps.start_training.hide = function() {
	try {
		TutorialCtrl.first_steps.steps.start_training.arrow.hide();
	}
	catch(e) {}
}
TutorialCtrl.first_steps.steps.start_training.show = function() {
	try {
		TutorialCtrl.first_steps.steps.start_training.arrow.show();
	}
	catch(e) {}
}

/**
 * Bei diesem Schritt wird der Spieler aufgefordert, den Mammun zu konfigurieren.
 */
TutorialCtrl.first_steps.steps.training_details.text = '';
TutorialCtrl.first_steps.steps.training_details.flag_to_set = 0;
TutorialCtrl.first_steps.steps.training_details.action = function() {
	UIButtons.bb1.show();
	
	/**
	 * Officer hijacked!
	 * Egal, welcher Text welchem Officer auch immer gegeben wird, es wird
	 * unserer angezeigt.
	 */
	TutorialCtrl.hijack_officer(
		TutorialCtrl.first_steps.steps.training_details.text
	);
	
	/**
	 * Wenn das UnitProduction-Objekt noch nicht bereit ist, dann warten...
	 */
	if ( (typeof(UnitProduction) == 'undefined') || !UnitProduction || 
			!UnitProduction.loaded) 
	{
		/**
		 * Wenn das UnitProduction-fenster noch nicht mal geöffnet wurde, dann
		 * einen Schritt zurück
		 */
		if (get_cur_fixed_window_url().indexOf('unit_production.php') < 0) {
			TutorialCtrl.previous(
				TutorialCtrl.first_steps,
				TutorialCtrl.first_steps.steps.training_details
			);
			return;
		}
		
		TutorialCtrl.first_steps.steps.training_details.waiting_interval =
			PureFW.Timeout.set_interval(
				function() {
					if ((typeof(UnitProduction) != 'undefined')
						&& UnitProduction && UnitProduction.loaded)
					{
						PureFW.Timeout.clear_interval(
							TutorialCtrl.first_steps.steps
								.training_details.waiting_interval
						);
						
						TutorialCtrl.first_steps.steps.training_details
							.action2();
					}
				}, 500
			);
	}
	else {
		TutorialCtrl.first_steps.steps.training_details.action2();
	}
}
TutorialCtrl.first_steps.steps.training_details.action2 = function() {
	TutorialCtrl.first_steps.steps.training_details.arrow1 
		= new PureFW.FilmAnimator
	(
		document.body,
		235, 215-56,
		81, 56
	);
	TutorialCtrl.first_steps.steps.training_details.arrow1.set_image(
		'ui/animations/animated-red-8bit-6frames-bouncing.png', 6);
	TutorialCtrl.first_steps.steps.training_details.arrow1.set_z_index(
		300);
	TutorialCtrl.first_steps.steps.training_details.arrow1.start_animation();
	
	TutorialCtrl.first_steps.steps.training_details.arrow2 
		= new PureFW.FilmAnimator
	(
		document.body,
		575, 215-56,
		81, 56
	);
	TutorialCtrl.first_steps.steps.training_details.arrow2.set_image(
		'ui/animations/animated-red-8bit-6frames-bouncing.png', 6);
	TutorialCtrl.first_steps.steps.training_details.arrow2.set_z_index(
		300);
	TutorialCtrl.first_steps.steps.training_details.arrow2.start_animation();
	
	TutorialCtrl.first_steps.steps.training_details.arrow3
		= new PureFW.FilmAnimator
	(
		document.body,
		875, 540-56,
		81, 56
	);
	TutorialCtrl.first_steps.steps.training_details.arrow3.set_image(
		'ui/animations/animated-red-8bit-6frames-bouncing.png', 6);
	TutorialCtrl.first_steps.steps.training_details.arrow3.set_z_index(
		300);
	TutorialCtrl.first_steps.steps.training_details.arrow3.start_animation();
	/**
	 * Handlers
	 */
	TutorialCtrl.first_steps.steps.training_details.details_chosen
		= function(ev)
	{
		PureFW.Timeout.clear_interval(
			TutorialCtrl.first_steps.steps.training_details.check_still_in_dialog
		);
		TutorialCtrl.next(
			TutorialCtrl.first_steps,
			TutorialCtrl.first_steps.steps.training_details
		);
	}
	TutorialCtrl.first_steps.steps.training_details.window_close_handler 
		= function(ev)
	{
		PureFW.Timeout.clear_interval(
			TutorialCtrl.first_steps.steps.training_details.check_still_in_dialog
		);
		TutorialCtrl.previous(
			TutorialCtrl.first_steps,
			TutorialCtrl.first_steps.steps.training_details
		);
	}

	TutorialCtrl.first_steps.steps.training_details.check_where_we_are 
		= function(ev) 
	{
		if ((typeof(UnitProduction) == 'undefined')
			|| !UnitProduction || !UnitProduction.loaded)
		{
			TutorialCtrl.first_steps.steps.training_details
				.window_close_handler(ev);
			return;
		}
		var n = UnitProduction.slots.length;
		for (var i = 0; i < n; i++) {
			/**
			 * Es wird bereits jemand trainiert => weiter
			 */
			
			if (UnitProduction.slots[i].current_unit) {
				TutorialCtrl.first_steps.steps.training_details
					.details_chosen(ev);
				return;
			}
		}
		/**
		 * Es sind Slots da, aber keiner wird Trainiert, dann sind wir eigentlich
		 * im Schritt vorher => zurück
		 */
		if (n > 0) {
			TutorialCtrl.first_steps.steps.training_details
				.window_close_handler(ev);
			return;
		}
		
		if ((typeof(UnitIndividualScreen) != 'undefined') 
			&& UnitIndividualScreen)
		{
			TutorialCtrl.first_steps.steps.training_details
				.details_chosen(ev);
		}
	}
	
	/**
	 * Destroy-Event-Handler haben einen Bug, wie's scheint. Deswegen läuft
	 * das hier über die bewährte Sicherung.
	 */
	TutorialCtrl.first_steps.steps.training_details.check_still_in_dialog =
		PureFW.Timeout.set_interval(
			function(ev) {
				TutorialCtrl.first_steps.steps.training_details
					.check_where_we_are(ev);
			}, 500
		);
	
	TutorialCtrl.first_steps.steps.training_details.check_where_we_are();
}

TutorialCtrl.first_steps.steps.training_details.approve = function() {
	TutorialCtrl.first_steps.steps.training_details.cancel();
}

TutorialCtrl.first_steps.steps.training_details.cancel = function() {
	try {
		UnitProduction.training_started_functions.remove(
			TutorialCtrl.first_steps.steps.training_details
				.training_started_handler
		);
	}
	catch(e) {}
	try {
		TutorialCtrl.first_steps.steps.training_details.arrow1.destroy();
		TutorialCtrl.first_steps.steps.training_details.arrow2.destroy();
		TutorialCtrl.first_steps.steps.training_details.arrow3.destroy();
	}
	catch(e) {}
	
	// Free hijacked officer
	TutorialCtrl.free_officer();	
}
TutorialCtrl.first_steps.steps.training_details.hide = function() {
	try {
		TutorialCtrl.first_steps.steps.training_details.arrow1.hide();
		TutorialCtrl.first_steps.steps.training_details.arrow2.hide();
		TutorialCtrl.first_steps.steps.training_details.arrow3.hide();
	}
	catch(e) {}
}
TutorialCtrl.first_steps.steps.training_details.show = function() {
	try {
		TutorialCtrl.first_steps.steps.training_details.arrow1.show();
		TutorialCtrl.first_steps.steps.training_details.arrow2.show();
		TutorialCtrl.first_steps.steps.training_details.arrow3.show();
	}
	catch(e) {}
}

/**
 * Bei diesem Schritt wird der Spieler aufgefordert, den Mammun zu 
 * individualisieren
 */
TutorialCtrl.first_steps.steps.training_indivi.text = '';
TutorialCtrl.first_steps.steps.training_indivi.flag_to_set = 0;
TutorialCtrl.first_steps.steps.training_indivi.action = function() {
	UIButtons.bb1.show();
	
	/**
	 * Officer hijacked!
	 * Egal, welcher Text welchem Officer auch immer gegeben wird, es wird
	 * unserer angezeigt.
	 */
	TutorialCtrl.hijack_officer(
		TutorialCtrl.first_steps.steps.training_indivi.text
	);
	
	/**
	 * Wenn das UnitProduction-Objekt noch nicht bereit ist, dann warten...
	 */
	if ( (typeof(UnitProduction) == 'undefined') || !UnitProduction || 
			!UnitProduction.loaded) 
	{
		/**
		 * Wenn das Forschungsfenster noch nicht mal geöffnet wurde, dann
		 * einen Schritt zurück
		 */
		if (get_cur_fixed_window_url().indexOf('unit_production.php') < 0) {
			TutorialCtrl.previous(
				TutorialCtrl.first_steps,
				TutorialCtrl.first_steps.steps.training_indivi
			);
			return;
		}
		
		TutorialCtrl.first_steps.steps.training_indivi.waiting_interval =
			PureFW.Timeout.set_interval(
				function() {
					if ((typeof(UnitProduction) != 'undefined')
						&& UnitProduction && UnitProduction.loaded)
					{
						PureFW.Timeout.clear_interval(
							TutorialCtrl.first_steps.steps
								.training_indivi.waiting_interval
						);
						
						TutorialCtrl.first_steps.steps.training_indivi
							.action2();
					}
				}, 500
			);
	}
	else {
		TutorialCtrl.first_steps.steps.training_indivi.action2();
	}
}
TutorialCtrl.first_steps.steps.training_indivi.action2 = function() {
	/**
	 * Handlers
	 */
	TutorialCtrl.first_steps.steps.training_indivi.training_started_handler
		= function(ev)
	{
		PureFW.Timeout.clear_interval(
			TutorialCtrl.first_steps.steps.training_indivi.check_still_in_dialog
		);
		TutorialCtrl.next(
			TutorialCtrl.first_steps,
			TutorialCtrl.first_steps.steps.training_indivi
		);
	}
	TutorialCtrl.first_steps.steps.training_indivi.window_close_handler 
		= function(ev)
	{
		PureFW.Timeout.clear_interval(
			TutorialCtrl.first_steps.steps.training_indivi.check_still_in_dialog
		);
		TutorialCtrl.previous(
			TutorialCtrl.first_steps,
			TutorialCtrl.first_steps.steps.training_indivi
		);
	}

	UnitProduction.training_started_functions.push(
		TutorialCtrl.first_steps.steps.training_indivi.training_started_handler
	);
	
	TutorialCtrl.first_steps.steps.training_indivi.check_where_we_are 
		= function(ev) 
	{
		if ((typeof(UnitProduction) == 'undefined')
			|| !UnitProduction || !UnitProduction.loaded)
		{
			TutorialCtrl.first_steps.steps.training_indivi
				.window_close_handler(ev);
			return;
		}
		var n = UnitProduction.slots.length;
		for (var i = 0; i < n; i++) {
			/**
			 * Es wird bereits jemand trainiert => weiter
			 */
			
			if (UnitProduction.slots[i].current_unit) {
				TutorialCtrl.first_steps.steps.training_indivi
					.training_started_handler(ev);
				return;
			}
		}
		if ((typeof(UnitIndividualScreen) == 'undefined') 
			|| !UnitIndividualScreen)
		{
			TutorialCtrl.first_steps.steps.training_indivi
				.window_close_handler(ev);
			throw new Error("prev @ indivi");
			return;
		}
	}
	
	/**
	 * Destroy-Event-Handler haben einen Bug, wie's scheint. Deswegen läuft
	 * das hier über die bewährte Sicherung.
	 */
	TutorialCtrl.first_steps.steps.training_indivi.check_still_in_dialog =
		PureFW.Timeout.set_interval(
			function(ev) {
				TutorialCtrl.first_steps.steps.training_indivi
					.check_where_we_are();
			}, 500
		);
	
	TutorialCtrl.first_steps.steps.training_indivi.check_where_we_are();
}

TutorialCtrl.first_steps.steps.training_indivi.approve = function() {
	TutorialCtrl.first_steps.steps.training_indivi.cancel();
}
TutorialCtrl.first_steps.steps.training_indivi.cancel = function() {
	try {
		UnitProduction.training_started_functions.remove(
			TutorialCtrl.first_steps.steps.training_indivi
				.training_started_handler
		);
	}
	catch(e) {}
	
	// Free hijacked officer
	TutorialCtrl.free_officer();	
}
TutorialCtrl.first_steps.steps.training_indivi.hide = function() {}
TutorialCtrl.first_steps.steps.training_indivi.show = function() {}




/**
 * Dieser Schritt schließt das Trainieren ab. Der Spieler wird aufgefordert, den
 * Screen zu verlassen.
 */
TutorialCtrl.first_steps.steps.quit_training.text = '';
TutorialCtrl.first_steps.steps.quit_training.flag_to_set = 0;
TutorialCtrl.first_steps.steps.quit_training.action = function() {
	UIButtons.bb1.show();
	
	/**
	 * Officer hijacked!
	 * Egal, welcher Text welchem Officer auch immer gegeben wird, es wird
	 * unserer angezeigt.
	 */
	TutorialCtrl.hijack_officer(
		TutorialCtrl.first_steps.steps.quit_training.text
	);
	
	/**
	 * Wenn das UnitProduction-Objekt noch nicht bereit ist, dann warten...
	 */
	if ( (typeof(UnitProduction) == 'undefined') || !UnitProduction || 
			!UnitProduction.loaded) 
	{
		/**
		 * Wenn das Forschungsfenster noch nicht mal geöffnet wurde, dann
		 * einen Schritt zurück
		 */
		if (get_cur_fixed_window_url().indexOf('unit_production.php') < 0) {
			TutorialCtrl.previous(
				TutorialCtrl.first_steps,
				TutorialCtrl.first_steps.steps.quit_training
			);
			return;
		}
		
		TutorialCtrl.first_steps.steps.quit_training.waiting_interval =
			PureFW.Timeout.set_interval(
				function() {
					if ((typeof(UnitProduction) != 'undefined')
						&& UnitProduction && UnitProduction.loaded)
					{
						PureFW.Timeout.clear_interval(
							TutorialCtrl.first_steps.steps
								.quit_training.waiting_interval
						);
						
						TutorialCtrl.first_steps.steps.quit_training
							.action2();
					}
				}, 500
			);
	}
	else {
		TutorialCtrl.first_steps.steps.quit_training.action2();
	}
}
TutorialCtrl.first_steps.steps.quit_training.action2 = function() {
	TutorialCtrl.first_steps.steps.quit_training.arrow = new PureFW.FilmAnimator(
		document.body,
		930, 122-56,
		81, 56);
	TutorialCtrl.first_steps.steps.quit_training.arrow.set_image(
		'ui/animations/animated-red-8bit-6frames-bouncing.png', 6);
	TutorialCtrl.first_steps.steps.quit_training.arrow.set_z_index(200);
	TutorialCtrl.first_steps.steps.quit_training.arrow.start_animation();
	/**
	 * Handlers
	 */
	TutorialCtrl.first_steps.steps.quit_training.window_close_handler 
		= function(ev)
	{
		PureFW.Timeout.clear_interval(
			TutorialCtrl.first_steps.steps.quit_training.check_still_in_dialog
		);
		TutorialCtrl.next(
			TutorialCtrl.first_steps,
			TutorialCtrl.first_steps.steps.quit_training
		);
	}

	/**
	 * Destroy-Event-Handler haben einen Bug, wie's scheint. Deswegen läuft
	 * das hier über die bewährte Sicherung.
	 */
	TutorialCtrl.first_steps.steps.quit_training.check_still_in_dialog =
		PureFW.Timeout.set_interval(
			function(ev) {
				if ((typeof(UnitProduction) == 'undefined')
					|| !UnitProduction || !UnitProduction.loaded)
				{
					TutorialCtrl.first_steps.steps.quit_training
						.window_close_handler(ev);
				}
			}, 500
		);
}

TutorialCtrl.first_steps.steps.quit_training.approve = function() {
	TutorialCtrl.first_steps.steps.quit_training.cancel();
}

TutorialCtrl.first_steps.steps.quit_training.cancel = function() {
	try {
		UnitProduction.research_started_functions.remove(
			TutorialCtrl.first_steps.steps.quit_training.science_click_handler
		);
	}
	catch(e) {}
	try {
		TutorialCtrl.first_steps.steps.quit_training.arrow.destroy();
	}
	catch(e) {}
	
	TutorialCtrl.free_officer();	
}
TutorialCtrl.first_steps.steps.quit_training.hide = function() {
	try {
		TutorialCtrl.first_steps.steps.quit_training.arrow.hide();
	}
	catch(e) {}
}
TutorialCtrl.first_steps.steps.quit_training.show = function() {
	try {
		TutorialCtrl.first_steps.steps.quit_training.arrow.show();
	}
	catch(e) {}
}


/**
 * Jetzt soll der Spieler erneut auf seinen Homesector klicken, um die zweite
 * Einheit zu bewegen
 */
TutorialCtrl.first_steps.steps.home_sector2.text = '';
TutorialCtrl.first_steps.steps.home_sector2.flag_to_set = 0;
TutorialCtrl.first_steps.steps.home_sector2.action = function() {
	/**
	 * Im endeffekt genau dasselbe, wie home_sector
	 */
	TutorialCtrl.first_steps.steps.home_sector.action(
		TutorialCtrl.first_steps.steps.home_sector2
	);
}
TutorialCtrl.first_steps.steps.home_sector2.approve = function() {
	TutorialCtrl.first_steps.steps.home_sector2.cancel();
}
TutorialCtrl.first_steps.steps.home_sector2.cancel = function() {
	var _top = MammunUI.get_top_frame();
	_top.MapDataCtrl.mammun_map.remove_event_handler("field_click",
		TutorialCtrl.first_steps.steps.home_sector2.sector_click_handler
	);
	try {
		TutorialCtrl.first_steps.steps.home_sector2.arrow.destroy();
	} 
	catch(e) {}
	TutorialCtrl.first_steps.steps.home_sector2.arrow = null;
	TutorialCtrl.destroy_tut_officer_box();
}
TutorialCtrl.first_steps.steps.home_sector2.hide = function() {
	try {
		TutorialCtrl.first_steps.steps.home_sector2.arrow.hide();
	} 
	catch(e) {}
	TutorialCtrl.hide_tut_officer_box();
}
TutorialCtrl.first_steps.steps.home_sector2.show = function() {
	try {
		TutorialCtrl.first_steps.steps.home_sector2.arrow.show();
	} 
	catch(e) {}
	TutorialCtrl.show_tut_officer_box();
}

/**
 * Bei diesem Schritt wählt der Spieler den zweiten Mammun aus, den er bewegen 
 * soll.
 */
TutorialCtrl.first_steps.steps.choose_unit_move2.text = '';
TutorialCtrl.first_steps.steps.choose_unit_move2.flag_to_set = 0;
TutorialCtrl.first_steps.steps.choose_unit_move2.action = function() {
	/**
	 * Im endeffekt genau dasselbe, wie choose_unit_move
	 */
	TutorialCtrl.first_steps.steps.choose_unit_move.action(
		TutorialCtrl.first_steps.steps.choose_unit_move2
	);
}
TutorialCtrl.first_steps.steps.choose_unit_move2.approve = function() {
	TutorialCtrl.first_steps.steps.choose_unit_move2.cancel();
}
TutorialCtrl.first_steps.steps.choose_unit_move2.cancel = function() {
	try {
		TutorialCtrl.first_steps.steps.choose_unit_move2.arrow.destroy();
		TutorialCtrl.first_steps.steps.choose_unit_move2.arrow = null;
	}
	catch(e){}
	TutorialCtrl.free_officer();
}
TutorialCtrl.first_steps.steps.choose_unit_move2.hide = function() {
	try {
		TutorialCtrl.first_steps.steps.choose_unit_move2.arrow.hide();
	}
	catch(e){}
}
TutorialCtrl.first_steps.steps.choose_unit_move2.show = function() {
	try {
		TutorialCtrl.first_steps.steps.choose_unit_move2.arrow.show();
	}
	catch(e){}
}

/**
 * Bei diesem Schritt gibt der Spieler dem Mammun den Zielsektor an, zu welchem
 * er sich bewegen soll.
 */
TutorialCtrl.first_steps.steps.move_unit2.text = '';
TutorialCtrl.first_steps.steps.move_unit2.flag_to_set = 0;
TutorialCtrl.first_steps.steps.move_unit2.action = function() {
	/**
	 * Im endeffekt genau dasselbe, wie move_unit
	 */
	TutorialCtrl.first_steps.steps.move_unit.action(
		TutorialCtrl.first_steps.steps.move_unit2
	);
}
TutorialCtrl.first_steps.steps.move_unit2.approve = function() {
	TutorialCtrl.first_steps.steps.move_unit2.cancel();
}
TutorialCtrl.first_steps.steps.move_unit2.cancel = function() {
	try {
		MapDataCtrl.Movement.remove_event_handler("move_successful",
			TutorialCtrl.first_steps.steps.move_unit2.unit_moved_handler
		);
		MapDataCtrl.Movement.remove_event_handler("move_unsuccessful",
			TutorialCtrl.first_steps.steps.move_unit2.unit_not_moved_handler
		);
	}
	catch(e) {}
	try {
		TutorialCtrl.first_steps.steps.move_unit2.arrows.destroy();
	}
	catch(e){}
	TutorialCtrl.first_steps.steps.move_unit2.arrows = null;
	
	TutorialCtrl.destroy_tut_officer_box();
}
TutorialCtrl.first_steps.steps.move_unit2.hide = function() {
	var n = TutorialCtrl.first_steps.steps.move_unit2.arrows.length;
	for (var i = 0; i < n; i++) {
		try {
			TutorialCtrl.first_steps.steps.move_unit2.arrows[i].hide();
		}
		catch(e){}
	}
	TutorialCtrl.hide_tut_officer_box();
}
TutorialCtrl.first_steps.steps.move_unit2.show = function() {
	var n = TutorialCtrl.first_steps.steps.move_unit2.arrows.length;
	for (var i = 0; i < n; i++) {
		try {
			TutorialCtrl.first_steps.steps.move_unit2.arrows[i].show();
		}
		catch(e){}
	}
	TutorialCtrl.show_tut_officer_box();
}



/**
 * Der Schritt zeigt den Outro-Text an.
 * 
 * In diesem Schritt wird der abschließende Text, dass der Spieler das Tutorial
 * geschafft habe, angezeigt.
 */
TutorialCtrl.first_steps.steps.show_outro_text.text = '';
TutorialCtrl.first_steps.steps.show_outro_text.flag_to_set = 0;
TutorialCtrl.first_steps.steps.show_outro_text.action = function() {
	GameMenu.show();
	try {
		SectorDetails.this_window.destroy();
	} catch(e) {};
	TutorialCtrl.first_steps.steps.show_outro_text.box = new PureFW.OfficerBox(
		document.body,
		(MammunUI.reference_width - 546) >> 1,
		(MammunUI.reference_height - 278) >> 1,
		546, 278
	);
	TutorialCtrl.first_steps.steps.show_outro_text.box.set_bg_img(
		'ui/backgrounds/tutorial/officer_tutorial_box_546x278.png'
	);
	TutorialCtrl.first_steps.steps.show_outro_text.box.set_css_class(
		'tutorial_box_big'
	)
	TutorialCtrl.first_steps.steps.show_outro_text.box.set_content(
		TutorialCtrl.first_steps.steps.show_outro_text.text
	);
	
	TutorialCtrl.first_steps.steps.show_outro_text.box.add_event_handler(
		"confirm",
		function() {
			TutorialCtrl.next(
				TutorialCtrl.first_steps,
				TutorialCtrl.first_steps.steps.show_outro_text
			);
		}
	);
}
TutorialCtrl.first_steps.steps.show_outro_text.approve = function() {
	TutorialCtrl.first_steps.steps.show_outro_text.cancel();
}
TutorialCtrl.first_steps.steps.show_outro_text.cancel = function() {
	try {
		TutorialCtrl.first_steps.steps.show_outro_text.box.destroy();
	}
	catch(e) {}	// was removed already
	TutorialCtrl.first_steps.steps.show_outro_text.box = null;
}
TutorialCtrl.first_steps.steps.show_outro_text.hide = function() {
	try {
		TutorialCtrl.first_steps.steps.show_outro_text.box.hide();
	}
	catch(e) {}	// was removed already
}
TutorialCtrl.first_steps.steps.show_outro_text.show = function() {
	try {
		TutorialCtrl.first_steps.steps.show_outro_text.box.show();
	}
	catch(e) {}	// was removed already
}


/**
 * Der Schritt macht nichts, außer dafür zu sorgen, dass der Bau-Officer noch
 * versteckt bleibt, bis sich das Gebiet vergrößert hat.
 */
TutorialCtrl.first_steps.steps.wait_until_arrived.text = '';
TutorialCtrl.first_steps.steps.wait_until_arrived.flag_to_set = 0;
TutorialCtrl.first_steps.steps.wait_until_arrived.action = function() {
	UIButtons.bb1.show();
	UIButtons.bb2.hide();
	UIButtons.bb3.show();
	GameMenu.show();
	TutorialCtrl.running = false;	// Eigentlich läuft jetzt kein Tutorial mehr
	TutorialCtrl.first_steps.steps.wait_until_arrived.interval 
		= PureFW.Timeout.set_interval
	(
		function(ev) {
			var sector_amt = 0;
			for (i in MapDataCtrl.dominators) {
				if (isNaN(i))
					continue;
				if (MapDataCtrl.dominators[i] == MapDataCtrl.my_nick)
					sector_amt++;
			}
			/**
			 * User hat sich bereits ausgebreitet.
			 */
			if (sector_amt > 1) {
				PureFW.Timeout.clear_interval(
					TutorialCtrl.first_steps.steps.wait_until_arrived.interval
				);
				TutorialCtrl.next(
					TutorialCtrl.first_steps,
					TutorialCtrl.first_steps.steps.wait_until_arrived
				);
			}
		}, 1000
	);
	
	/**
	 * Beaming Magic
	 */
	PureFW.Timeout.set_timeout(function() {
		PureFW.AJAXClientServer.send_request('tutorial.php?magic=1',function(){
			MapDataCtrl.update_map();
			PureFW.Timeout.clear_interval(
				TutorialCtrl.first_steps.steps.wait_until_arrived.interval
			);
			TutorialCtrl.next(
				TutorialCtrl.first_steps,
				TutorialCtrl.first_steps.steps.wait_until_arrived
			);
		}, null, true);
	},1000);
}
TutorialCtrl.first_steps.steps.wait_until_arrived.approve = function() {
	TutorialCtrl.first_steps.steps.wait_until_arrived.cancel();
}
TutorialCtrl.first_steps.steps.wait_until_arrived.cancel = function() {
	UIButtons.bb2.show();
}
TutorialCtrl.first_steps.steps.wait_until_arrived.hide = function() {}
TutorialCtrl.first_steps.steps.wait_until_arrived.show = function() {}


/*****************************************************************************
 * 
 * 
 * 
 * Das Tutorial "Bauen und ausbreiten".
 * 
 * 
 * 
 *****************************************************************************/
TutorialCtrl.build_and_expand = new Object();

/** Alle Schritte im Tutorial */
TutorialCtrl.build_and_expand.steps = {
	open_buildings:{},
	choose_building:{},
	confirm_building:{},
	choose_sector_build:{},
	place_building:{},
	choose_unit_move:{},
	move_unit:{},
	move_second_unit:{},
	show_outro_text:{},
	invite_friends:{}
}

/** Reihenfolge der einzelnen Schritte */
TutorialCtrl.build_and_expand.order = [
	'open_buildings', 
	'choose_building',
	'confirm_building',
	'choose_sector_build',
	'place_building', 
	'choose_unit_move', 
	'move_unit', 
	'move_second_unit', 
	'show_outro_text',
	'invite_friends'
];

TutorialCtrl.build_and_expand.current_step_i = -1;

/**
 * Initialisiert das Tutorial :)
 * 
 * @param Object step		Einsprungpunkt: Mit dem wievielten Schritt soll
 * 							begonnen werden?
 */
TutorialCtrl.build_and_expand.init = function(step_i) {
	GameMenu.close_game_menu();
	TutorialCtrl.build_and_expand.current_step_i = step_i || 0;
}

/**
 * Deinitialisiert das Tutorial :)
 */
TutorialCtrl.build_and_expand.clean_up = function() {
}


/**
 * Jetzt kommen die einzelnen Tutorialschritte. Jeder Schritt ist ein Objekt
 * mit mindestens den drei Funktionen 'action', 'cancel' und 'approve'.
 * 
 * Zudem sind folgende Attribute vorgeschrieben: 'text' und 'flag_to_set'.
 * 'text' gibt dabei an, welcher Text in dem Schritt angezeigt werden soll
 * (manche Schritte können keinen Text anzeigen, dann ist das Setzen zwecklos),
 * und 'flag_to_set' gibt an, welcher Flag an den GUI-Server übermittelt werden
 * soll, wenn der Schritt erfolgreich abgeschlossen wurde.
 */

/**
 * Bei diesem Schritt wird der Spieler aufgefordert, den Bau-Dialog zu öffnen.
 */
TutorialCtrl.build_and_expand.steps.open_buildings.text = '';
TutorialCtrl.build_and_expand.steps.open_buildings.flag_to_set = 0;
TutorialCtrl.build_and_expand.steps.open_buildings.action = function() {
	try {
		SectorDetails.this_window.destroy();
	} catch(e) {};
	try {
		UIButtons.bb2.show();
	} catch (e){};
	TutorialCtrl.build_and_expand.steps.open_buildings.bb_z_index = 
		UIButtons.bottom_container.get_z_index();
	UIButtons.bottom_container.set_z_index(120);
	
	TutorialCtrl.build_and_expand.steps.open_buildings.arrow 
		= new PureFW.FilmAnimator
	(
		document.body,
		UIButtons.bottom_container.position.x +
			UIButtons.bb2.position.x + ((UIButtons.bb2.width - 81)>>1),
		UIButtons.bottom_container.position.y +
			UIButtons.bb2.position.y - 46,
		81, 56
	);
	TutorialCtrl.build_and_expand.steps.open_buildings.arrow.set_image(
		'ui/animations/animated-red-8bit-6frames-bouncing.png', 6);
	TutorialCtrl.build_and_expand.steps.open_buildings.arrow.set_z_index(
		UIButtons.bottom_container.get_z_index()+1);
	TutorialCtrl.build_and_expand.steps.open_buildings.arrow.start_animation();

	/**
	 * Officer-Box
	 */
	TutorialCtrl.create_tut_officer_box(
		TutorialCtrl.build_and_expand.steps.open_buildings.text
	);
	
	/**
	 * Handlers
	 */
	TutorialCtrl.build_and_expand.steps.open_buildings.cco_click_handler =
		function(ev)
	{
		TutorialCtrl.next(
			TutorialCtrl.build_and_expand,
			TutorialCtrl.build_and_expand.steps.open_buildings
		);
	}
	
	UIButtons.bb2.add_event_handler("click",
		TutorialCtrl.build_and_expand.steps.open_buildings.cco_click_handler
	);
}

TutorialCtrl.build_and_expand.steps.open_buildings.approve = function() {
	TutorialCtrl.build_and_expand.steps.open_buildings.cancel();
}

TutorialCtrl.build_and_expand.steps.open_buildings.cancel = function() {
	try {
		TutorialCtrl.build_and_expand.steps.open_buildings.arrow.destroy();
	}
	catch(e) {}
	TutorialCtrl.build_and_expand.steps.open_buildings.arrow = null;

	TutorialCtrl.destroy_tut_officer_box();
	
	UIButtons.bb2.remove_event_handler("click",
		TutorialCtrl.build_and_expand.steps.open_buildings.cco_click_handler
	);
	UIButtons.bottom_container.set_z_index(
		TutorialCtrl.build_and_expand.steps.open_buildings.bb_z_index
	);
}
TutorialCtrl.build_and_expand.steps.open_buildings.hide = function() {
	try {
		TutorialCtrl.build_and_expand.steps.open_buildings.arrow.hide();
	}
	catch(e) {}

	TutorialCtrl.hide_tut_officer_box();
}
TutorialCtrl.build_and_expand.steps.open_buildings.show = function() {
	try {
		TutorialCtrl.build_and_expand.steps.open_buildings.arrow.show();
	}
	catch(e) {}

	TutorialCtrl.show_tut_officer_box();
}


/**
 * Bei diesem Schritt wird der Spieler aufgefordert, ein Gebäude auszuwählen.
 */
TutorialCtrl.build_and_expand.steps.choose_building.text = '';
TutorialCtrl.build_and_expand.steps.choose_building.flag_to_set = 0;
TutorialCtrl.build_and_expand.steps.choose_building.action = function() {
	try {
		SectorDetails.this_window.destroy();
	} catch(e) {};
	
	/**
	 * Wenn das BuildingSelection-Objekt noch nicht bereit ist, dann warten...
	 */
	if ( (typeof(BuildingSelection) == 'undefined') || !BuildingSelection || 
			!BuildingSelection.next_button_container) 
	{
		/**
		 * Wenn das Bau-Fenster noch nicht mal geöffnet wurde, dann öffnen
		 */
		if (get_cur_fixed_window_url().indexOf('buildings.php') < 0)
			UIButtons.open_window('buildings.php');
		
		TutorialCtrl.build_and_expand.steps.choose_building.waiting_interval =
			PureFW.Timeout.set_interval(
				function() {
					if ((typeof(BuildingSelection) != 'undefined')
						&& BuildingSelection && BuildingSelection
													.next_button_container)
					{
						PureFW.Timeout.clear_interval(
							TutorialCtrl.build_and_expand.steps
								.choose_building.waiting_interval
						);
						
						TutorialCtrl.build_and_expand.steps.choose_building
							.action2();
					}
				}, 500
			);
	}
	else {
		TutorialCtrl.build_and_expand.steps.choose_building.action2();
	}
}
TutorialCtrl.build_and_expand.steps.choose_building.action2 = function() {
	/**
	 * Pfeil
	 */
	TutorialCtrl.build_and_expand.steps.choose_building.arrow 
		= new PureFW.FilmAnimator
	(
		document.body,
		245, 215-56,
		81, 56
	);
	TutorialCtrl.build_and_expand.steps.choose_building.arrow.set_image(
		'ui/animations/animated-red-8bit-6frames-bouncing.png', 6);
	TutorialCtrl.build_and_expand.steps.choose_building.arrow.set_z_index(
		300);
	TutorialCtrl.build_and_expand.steps.choose_building.arrow.start_animation();
	/**
	 * Handlers
	 */
	TutorialCtrl.build_and_expand.steps.choose_building.building_chosen_handler=
		function(ev)
	{
		TutorialCtrl.next(
			TutorialCtrl.build_and_expand,
			TutorialCtrl.build_and_expand.steps.choose_building
		);
	}
	
	TutorialCtrl.build_and_expand.steps.choose_building.window_close_handler 
		= function(ev)
	{
		PureFW.Timeout.clear_interval(
			TutorialCtrl.build_and_expand.steps.choose_building
				.check_still_in_dialog
		);
		TutorialCtrl.previous(
			TutorialCtrl.build_and_expand,
			TutorialCtrl.build_and_expand.steps.choose_building
		);
	}
	Buildings.all_buildings[0].add_event_handler("click",
		TutorialCtrl.build_and_expand.steps.choose_building
			.building_chosen_handler
	);
	
	/**
	 * Destroy-Event-Handler haben einen Bug, wie's scheint. Deswegen läuft
	 * das hier über die bewährte Sicherung.
	 */
	TutorialCtrl.build_and_expand.steps.choose_building.check_still_in_dialog =
		PureFW.Timeout.set_interval(
			function(ev) {
				if ((typeof(BuildingSelection) == 'undefined')
					|| !BuildingSelection)
				{
					TutorialCtrl.build_and_expand.steps.choose_building
						.window_close_handler(ev);
				}
			}, 500
		);
}

TutorialCtrl.build_and_expand.steps.choose_building.approve = function() {
	TutorialCtrl.build_and_expand.steps.choose_building.cancel();
}

TutorialCtrl.build_and_expand.steps.choose_building.cancel = function() {
	try {
		Buildings.all_buildings[0].remove_event_handler("click",
			TutorialCtrl.build_and_expand.steps.choose_building
				.building_chosen_handler
		);
	}
	catch(e) {}
	PureFW.Timeout.clear_interval(
		TutorialCtrl.build_and_expand.steps.choose_building
			.check_still_in_dialog
	);
	try {
		TutorialCtrl.build_and_expand.steps.choose_building.arrow.destroy();
	}
	catch(e) {}
}
TutorialCtrl.build_and_expand.steps.choose_building.hide = function() {
	try {
		TutorialCtrl.build_and_expand.steps.choose_building.arrow.hide();
	}
	catch(e) {}
}
TutorialCtrl.build_and_expand.steps.choose_building.show = function() {
	try {
		TutorialCtrl.build_and_expand.steps.choose_building.arrow.show();
	}
	catch(e) {}
}

/**
 * Bei diesem Schritt wird der Spieler aufgefordert, seine Gebäudewahl zu
 * bestätigen durch Klicken auf "next".
 */
TutorialCtrl.build_and_expand.steps.confirm_building.text = '';
TutorialCtrl.build_and_expand.steps.confirm_building.flag_to_set = 0;
TutorialCtrl.build_and_expand.steps.confirm_building.action = function() {
	try {
		SectorDetails.this_window.destroy();
	} catch(e) {};
	
	/**
	 * Wenn das BuildingSelection-Objekt noch nicht bereit ist, dann warten...
	 */
	if ( (typeof(BuildingSelection) == 'undefined') || !BuildingSelection || 
			!BuildingSelection.next_button_container) 
	{
		/**
		 * Wenn das Bau-Fenster noch nicht mal geöffnet wurde, dann öffnen
		 */
		if (get_cur_fixed_window_url().indexOf('buildings.php') < 0)
			UIButtons.open_window('buildings.php');
		
		TutorialCtrl.build_and_expand.steps.confirm_building.waiting_interval =
			PureFW.Timeout.set_interval(
				function() {
					if ((typeof(BuildingSelection) != 'undefined')
						&& BuildingSelection && BuildingSelection
													.next_button_container)
					{
						PureFW.Timeout.clear_interval(
							TutorialCtrl.build_and_expand.steps
								.confirm_building.waiting_interval
						);
						
						TutorialCtrl.build_and_expand.steps.confirm_building
							.action2();
					}
				}, 500
			);
	}
	else {
		TutorialCtrl.build_and_expand.steps.confirm_building.action2();
	}
}
TutorialCtrl.build_and_expand.steps.confirm_building.action2 = function() {
	/**
	 * Pfeil
	 */
	TutorialCtrl.build_and_expand.steps.confirm_building.arrow 
		= new PureFW.FilmAnimator
	(
		document.body,
		870, 550-56,
		81, 56
	);
	TutorialCtrl.build_and_expand.steps.confirm_building.arrow.set_image(
		'ui/animations/animated-red-8bit-6frames-bouncing.png', 6);
	TutorialCtrl.build_and_expand.steps.confirm_building.arrow.set_z_index(
		300);
	TutorialCtrl.build_and_expand.steps.confirm_building.arrow
		.start_animation();
	
	/**
	 * Handlers
	 */
	TutorialCtrl.build_and_expand.steps.confirm_building.next_click_handler =
		function(ev)
	{
		TutorialCtrl.next(
			TutorialCtrl.build_and_expand,
			TutorialCtrl.build_and_expand.steps.confirm_building
		);
	}
	
	TutorialCtrl.build_and_expand.steps.confirm_building.window_close_handler 
		= function(ev)
	{
		PureFW.Timeout.clear_interval(
			TutorialCtrl.build_and_expand.steps.confirm_building
				.check_still_in_dialog
		);
		TutorialCtrl.previous(
			TutorialCtrl.build_and_expand,
			TutorialCtrl.build_and_expand.steps.confirm_building
		);
	}
	
	BuildingSelection.next_button_container.add_event_handler("click",
		TutorialCtrl.build_and_expand.steps.confirm_building.next_click_handler
	);
	
	/**
	 * Destroy-Event-Handler haben einen Bug, wie's scheint. Deswegen läuft
	 * das hier über die bewährte Sicherung.
	 */
	TutorialCtrl.build_and_expand.steps.confirm_building.check_still_in_dialog =
		PureFW.Timeout.set_interval(
			function(ev) {
				if ((typeof(BuildingSelection) == 'undefined')
					|| !BuildingSelection)
				{
					TutorialCtrl.build_and_expand.steps.confirm_building
						.window_close_handler(ev);
				}
			}, 500
		);
}

TutorialCtrl.build_and_expand.steps.confirm_building.approve = function() {
	TutorialCtrl.build_and_expand.steps.confirm_building.cancel();
}

TutorialCtrl.build_and_expand.steps.confirm_building.cancel = function() {
	try {
		BuildingSelection.next_button_container.remove_event_handler("click",
			TutorialCtrl.build_and_expand.steps.confirm_building
				.next_click_handler
		);
	}
	catch(e) {}
	PureFW.Timeout.clear_interval(
		TutorialCtrl.build_and_expand.steps.confirm_building
			.check_still_in_dialog
	);
	try {
		TutorialCtrl.build_and_expand.steps.confirm_building.arrow.destroy();
	}
	catch(e) {}
}
TutorialCtrl.build_and_expand.steps.confirm_building.hide = function() {
	try {
		TutorialCtrl.build_and_expand.steps.confirm_building.arrow.hide();
	}
	catch(e) {}
}
TutorialCtrl.build_and_expand.steps.confirm_building.show = function() {
	try {
		TutorialCtrl.build_and_expand.steps.confirm_building.arrow.show();
	}
	catch(e) {}
}


/**
 * Der Spieler soll nun einen Sektor wählen wo ein Mammun drin steht, um im
 * nächsten Schritt dort einen Unterstand zu bauen.
 */
TutorialCtrl.build_and_expand.steps.choose_sector_build.text = '';
TutorialCtrl.build_and_expand.steps.choose_sector_build.flag_to_set = 0;
TutorialCtrl.build_and_expand.steps.choose_sector_build.action = function(_this)
{
	_this = _this || TutorialCtrl.build_and_expand.steps.choose_sector_build;
	try {
		SectorDetails.this_window.destroy();
	} catch(e) {};
	var _top = MammunUI.get_top_frame();
	var arrow_parent = (MapUI.no_iframes) ? document.getElemById('mapx')
		: _top.mapx.document.getElementsByTagName('body')[0];
	
	_top.MapUI.scroll_to_hs();
	
	/**
	 * Wir nehmen den Mammun in einem Nachbarsektor des Heimatsektors an, so
	 * müssen wir nur vier Sektoren durchsuchen. Die Annahme ist korrekt, wenn
	 * der Spieler das Tutorial korrekt durchgespielt hat. Wird kein Mammun
	 * gefunden in den Nachbarsektoren, dann wird kein Pfeil angezeigt, das
	 * Tutorial läuft aber weiter.
	 */
	
	var mp = _top.MapDataCtrl.mammun_map;
	var w = mp.get_field_width() >> 1;
	var h = mp.get_field_height() >> 1
	var hs = _top.MapDataCtrl.my_homesector;
	
	var dirs = new Array(-1, -10, 1, 10);
	var n = dirs.length;
	_this.arrows = new Array();
	for (var i = 0; i < n; i++) {
		var s = hs + dirs[i];
		if (MapDataCtrl.resources[s] 
			&& MapDataCtrl.units[s]
			&& MapDataCtrl.units[s][MapDataCtrl.my_nick])
		{
			var ok = false;
			for (heading in _top.MapDataCtrl.units[s][MapDataCtrl.my_nick]) {
				if (isNaN(heading))
					continue;
				ok = true;
			}
			if (!ok)
				continue;
			
			// Hier steht einer meiner Mammuns drin
			var p = mp.get_field_pixel_position(s%10, Math.floor(s/10));
			
			cur_arrow = new PureFW.FilmAnimator(
				arrow_parent,
				MapDataCtrl.mammun_map.position.x + p.x + w - (81>>1),
				MapDataCtrl.mammun_map.position.y + p.y - 30,
				81, 56,
				true
			);
			cur_arrow.set_image(
				'ui/animations/animated-red-8bit-6frames-bouncing.png', 6);
			cur_arrow.set_z_index(30);
			cur_arrow.start_animation();
			_this.arrows.push(cur_arrow);
		}
	}
	
	/**
	 * Officer-Box
	 */
	TutorialCtrl.create_tut_officer_box(_this.text);
	
	/**
	 * Handler
	 */
	_this.sector_click_handler = (function(__this) {
		return function(ev)
		{
			TutorialCtrl.next(
				TutorialCtrl.build_and_expand,
				__this
			);
		}
	})(_this);
	_top.MapDataCtrl.mammun_map.add_event_handler("field_click",
		_this.sector_click_handler
	);
	
}
TutorialCtrl.build_and_expand.steps.choose_sector_build.approve = function() {
	TutorialCtrl.build_and_expand.steps.choose_sector_build.cancel();
}
TutorialCtrl.build_and_expand.steps.choose_sector_build.cancel = function() {
	var _top = MammunUI.get_top_frame();
	_top.MapDataCtrl.mammun_map.remove_event_handler("field_click",
		TutorialCtrl.build_and_expand.steps.choose_sector_build
			.sector_click_handler
	);
	try {
		TutorialCtrl.build_and_expand.steps.choose_sector_build.arrows.destroy();
	} 
	catch(e) {}
	TutorialCtrl.build_and_expand.steps.choose_sector_build.arrow = null;
	TutorialCtrl.destroy_tut_officer_box();
}
TutorialCtrl.build_and_expand.steps.choose_sector_build.hide = function() {
	var n = TutorialCtrl.build_and_expand.steps.choose_sector_build.arrows;
	for (var i = 0; i < n; i++) {
		try {
			TutorialCtrl.build_and_expand.steps.choose_sector_build.arrows[i]
				.hide();
		} 
		catch(e) {}
	}
	TutorialCtrl.hide_tut_officer_box();
}
TutorialCtrl.build_and_expand.steps.choose_sector_build.show = function() {
	var n = TutorialCtrl.build_and_expand.steps.choose_sector_build.arrows
		.length;
	for (var i = 0; i < n; i++) {
		try {
			TutorialCtrl.build_and_expand.steps.choose_sector_build.arrows[i]
				.show();
		} 
		catch(e) {}
	}
	TutorialCtrl.show_tut_officer_box();
}


/**
 * In diesem Schritt setzt der Spieler das gewählte Häuschen.
 */
TutorialCtrl.build_and_expand.steps.place_building.text = '';
TutorialCtrl.build_and_expand.steps.place_building.flag_to_set = 0;
TutorialCtrl.build_and_expand.steps.place_building.action = function() {
	var _top = MammunUI.get_top_frame();
	
	/**
	 * Officer hijacked!
	 * Egal, welcher Text welchem Officer auch immer gegeben wird, es wird
	 * unserer angezeigt.
	 */
	TutorialCtrl.hijack_officer(
		TutorialCtrl.build_and_expand.steps.place_building.text
	);
	
	if ( (typeof(SectorDetails) == 'undefined') || !SectorDetails) {
		/**
		 * Wir können nicht wissen, welchen Sektor der Spieler anwählen will.
		 * Wir können nur warten und nach einem Timeout das Warten beenden und
		 * zurücksteppen.
		 */
		TutorialCtrl.build_and_expand.steps.place_building.max_ticks = 20;
		TutorialCtrl.build_and_expand.steps.place_building.ticks = 0;
		
		TutorialCtrl.build_and_expand.steps.place_building.waiting_interval =
			PureFW.Timeout.set_interval(
				function() {
					/** Timeout-Check */
					if (TutorialCtrl.build_and_expand.steps.place_building
						.ticks > TutorialCtrl.build_and_expand.steps
									.place_building.max_ticks)
					{
						PureFW.Timeout.clear_interval(
							TutorialCtrl.build_and_expand.steps
								.place_building.waiting_interval
						);
						
						TutorialCtrl.previous(
							TutorialCtrl.build_and_expand,
							TutorialCtrl.build_and_expand.steps.place_building
						);
						return;
					}
					
					/** Loading-Finished-Check */
					TutorialCtrl.build_and_expand.steps.place_building.ticks++;
					if ((typeof(SectorDetails) != 'undefined')
						&& SectorDetails && SectorDetails.loaded)
					{
						PureFW.Timeout.clear_interval(
							TutorialCtrl.build_and_expand.steps
								.place_building.waiting_interval
						);
						
						TutorialCtrl.build_and_expand.steps.place_building
							.action2();
					}
				}, 500
			);
	}
	else {
		TutorialCtrl.build_and_expand.steps.place_building.action2();
	}
}
TutorialCtrl.build_and_expand.steps.place_building.action2 = function() {
	if (!SectorDetails.here_to_build) {
		/**
		 * Es wurde zwar ein Landfleck angewählt, aber nicht zum Bauen. Wir
		 * steppen also wieder einen Schritt zurück.
		 */
		TutorialCtrl.previous(
			TutorialCtrl.build_and_expand,
			TutorialCtrl.build_and_expand.steps.place_building
		);
		return;
	}
	
	/**
	 * Handler
	 */
	TutorialCtrl.build_and_expand.steps.place_building
		.construction_started_handler = function(ev) 
	{
		TutorialCtrl.next(
			TutorialCtrl.build_and_expand,
			TutorialCtrl.build_and_expand.steps.place_building
		);		
	}
	TutorialCtrl.build_and_expand.steps.place_building.sector_close_handler =
		function(ev) 
	{
		PureFW.Timeout.clear_interval(TutorialCtrl.build_and_expand.steps
			.place_building.check_still_in_sector);

		TutorialCtrl.previous(
			TutorialCtrl.build_and_expand,
			TutorialCtrl.build_and_expand.steps.place_building
		);
	}
	
	SectorDetails.construction_started_functions.push(
		TutorialCtrl.build_and_expand.steps.place_building
			.construction_started_handler
	);
	
	SectorDetails.this_window.add_event_handler("destroy", 
		TutorialCtrl.build_and_expand.steps.place_building.sector_close_handler
	);

	/**
	 * Das ist eine Sicherung, falls der Destruktor aus irgendwelchen Gründen
	 * nicht ausgeführt wird. Beim Tutorial steht Stabilität ganz weit oben.
	 */
	TutorialCtrl.build_and_expand.steps.place_building.check_still_in_sector =
		PureFW.Timeout.set_interval(
			function(ev) {
				if ((typeof(SectorDetails) == 'undefined')
					|| !SectorDetails || !SectorDetails.loaded)
				{
					TutorialCtrl.build_and_expand.steps.place_building
						.sector_close_handler(ev);
				}
			}, 500
		);
}
TutorialCtrl.build_and_expand.steps.place_building.approve = function() {
	TutorialCtrl.build_and_expand.steps.place_building.cancel();
}
TutorialCtrl.build_and_expand.steps.place_building.cancel = function() {
	try {
		TutorialCtrl.build_and_expand.steps.place_building.arrow.destroy();
		TutorialCtrl.build_and_expand.steps.place_building.arrow = null;
	}
	catch(e){}
	TutorialCtrl.free_officer();
	
	PureFW.Timeout.clear_interval(TutorialCtrl.build_and_expand.steps
		.place_building.check_still_in_sector);
	
	try {
		SectorDetails.construction_started_functions.remove(
			TutorialCtrl.build_and_expand.steps.place_building
				.construction_started_handler
		);
	}
	catch(e) {}
}
TutorialCtrl.build_and_expand.steps.place_building.hide = function() {
	try {
		TutorialCtrl.build_and_expand.steps.place_building.arrow.hide();
	}
	catch(e){}
}
TutorialCtrl.build_and_expand.steps.place_building.show = function() {
	try {
		TutorialCtrl.build_and_expand.steps.place_building.arrow.show();
	}
	catch(e){}
}



/**
 * In diesem Schritt wähl der Spieler einen Mammun aus, um ihn zu bewegen.
 */
TutorialCtrl.build_and_expand.steps.choose_unit_move.text = '';
TutorialCtrl.build_and_expand.steps.choose_unit_move.flag_to_set = 0;
TutorialCtrl.build_and_expand.steps.choose_unit_move.action = function() {
	var _top = MammunUI.get_top_frame();
	
	/**
	 * Officer hijacked!
	 * Egal, welcher Text welchem Officer auch immer gegeben wird, es wird
	 * unserer angezeigt.
	 */
	TutorialCtrl.hijack_officer(
		TutorialCtrl.build_and_expand.steps.choose_unit_move.text
	);
	
	if ( (typeof(SectorDetails) == 'undefined') || !SectorDetails) {
		var hs = _top.MapDataCtrl.my_homesector_field_pos;
		var ev = PureFW.EventUtil.create_event("click", 
			new PureFW.Point(hs.x, hs.y)
		);
		_top.MapDataCtrl.sector_click(ev);
		
		TutorialCtrl.build_and_expand.steps.choose_unit_move.waiting_interval =
			PureFW.Timeout.set_interval(
				function() {
					if ((typeof(SectorDetails) != 'undefined')
						&& SectorDetails && SectorDetails.loaded)
					{
						PureFW.Timeout.clear_interval(
							TutorialCtrl.build_and_expand.steps
								.choose_unit_move.waiting_interval
						);
						
						TutorialCtrl.build_and_expand.steps.choose_unit_move
							.action2();
					}
				}, 500
			);
	}
	else {
		TutorialCtrl.build_and_expand.steps.choose_unit_move.action2();
	}
}
TutorialCtrl.build_and_expand.steps.choose_unit_move.action2 = function() {
	SectorDetails.officer.set_text();
	
	var p;
	var p;
	var units = SectorDetails.sector_zoom.get_unit_widgets();
	var n = units.length;
	for (var i = 0; i < n; i++) {
		if (units[i].is_my_unit) {
			p = units[i].position;
			w = units[i].width;
			h = units[i].height;
			break;
		}
	}
	
	if (typeof(p) == 'undefined') {
		/**
		 * Es wurde keine Einheit im HS gefunden, also kennt er den Schritt
		 * schon, denn er muss sie ja bewegt haben. Also können wir den Schritt 
		 * überspringen.
		 */
		TutorialCtrl.next(
			TutorialCtrl.build_and_expand,
			TutorialCtrl.build_and_expand.steps.choose_unit_move
		);
		return;
	}
	p.x += (parseInt(units[i].parent_node.style.left)
				/ PureFW.WidgetManager.manager_all.get_scale_factor())
		+ SectorDetails.sector_zoom.position.x
		+ SectorDetails.this_window.position.x;
	p.y += (parseInt(units[i].parent_node.style.top)
				/ PureFW.WidgetManager.manager_all.get_scale_factor())
		+ SectorDetails.sector_zoom.position.y
		+ SectorDetails.this_window.position.y;

	TutorialCtrl.build_and_expand.steps.choose_unit_move.arrow 
		= new PureFW.FilmAnimator
	(
		document.body,
		p.x + (w - 81>>1),
		p.y - 56,
		81, 56
	);
	
	TutorialCtrl.build_and_expand.steps.choose_unit_move.arrow.set_image(
		'ui/animations/animated-red-8bit-6frames-bouncing.png', 6);
	TutorialCtrl.build_and_expand.steps.choose_unit_move.arrow.set_z_index(
		SectorDetails.this_window.get_z_index() + 1);
	TutorialCtrl.build_and_expand.steps.choose_unit_move.arrow.start_animation();

	/**
	 * Handler
	 */
	TutorialCtrl.build_and_expand.steps.choose_unit_move.sector_close_handler =
		function(ev) 
	{
		PureFW.Timeout.clear_interval(TutorialCtrl.build_and_expand.steps
			.choose_unit_move.check_still_in_sector);

		if (MammunUI.get_top_frame().MapDataCtrl.Movement.from_sector == -1) {
			TutorialCtrl.previous(
				TutorialCtrl.build_and_expand,
				TutorialCtrl.build_and_expand.steps.choose_unit_move
			);
		}
		else {
			TutorialCtrl.next(
				TutorialCtrl.build_and_expand,
				TutorialCtrl.build_and_expand.steps.choose_unit_move
			);
		}
	}
	
	SectorDetails.this_window.add_event_handler("destroy", 
		TutorialCtrl.build_and_expand.steps.choose_unit_move.sector_close_handler
	);

	/**
	 * Das ist eine Sicherung, falls der Destruktor aus irgendwelchen Gründen
	 * nicht ausgeführt wird. Beim Tutorial steht Stabilität ganz weit oben.
	 */
	TutorialCtrl.build_and_expand.steps.choose_unit_move.check_still_in_sector =
		PureFW.Timeout.set_interval(
			function(ev) {
				if ((typeof(SectorDetails) == 'undefined')
					|| !SectorDetails || !SectorDetails.loaded)
				{
					TutorialCtrl.build_and_expand.steps.choose_unit_move
						.sector_close_handler(ev);
				}
			}, 500
		);
}
TutorialCtrl.build_and_expand.steps.choose_unit_move.approve = function() {
	TutorialCtrl.build_and_expand.steps.choose_unit_move.cancel();
}
TutorialCtrl.build_and_expand.steps.choose_unit_move.cancel = function() {
	try {
		TutorialCtrl.build_and_expand.steps.choose_unit_move.arrow.destroy();
		TutorialCtrl.build_and_expand.steps.choose_unit_move.arrow = null;
	}
	catch(e){}
	TutorialCtrl.free_officer();
}
TutorialCtrl.build_and_expand.steps.choose_unit_move.hide = function() {
	try {
		TutorialCtrl.build_and_expand.steps.choose_unit_move.arrow.hide();
	}
	catch(e){}
}
TutorialCtrl.build_and_expand.steps.choose_unit_move.show = function() {
	try {
		TutorialCtrl.build_and_expand.steps.choose_unit_move.arrow.show();
	}
	catch(e){}
}


/**
 * Bei diesem Schritt gibt der Spieler dem Mammun den Zielsektor an, zu welchem
 * er sich bewegen soll.
 */
TutorialCtrl.build_and_expand.steps.move_unit.text = '';
TutorialCtrl.build_and_expand.steps.move_unit.flag_to_set = 0;
TutorialCtrl.build_and_expand.steps.move_unit.action = function() {
	try {
		SectorDetails.this_window.destroy();
	} catch(e) {};
	if (MammunUI.get_top_frame().MapDataCtrl.Movement.from_sector == -1) {
		/**
		 * Wir sind hier gelandet, aber keine Einheit ist gerade in der 
		 * Bewegung. Eigentlich wäre das ein Grund zurückzugehen, aber wir
		 * überspringen den Schritt und gehen zum nächsten, weil wir davon
		 * ausgehen, dass der Fall nur eintritt, wenn der Spieler den letzten
		 * Schritt übersprungen hat, weil er bereits Einheiten bewegt hat.
		 */
		TutorialCtrl.next(
			TutorialCtrl.build_and_expand,
			TutorialCtrl.build_and_expand.steps.move_unit
		);
		return;
	}
		
	
	var _top = MammunUI.get_top_frame();

	try {
		// das geht nur, wenn Karte in Frame angezeigt wird!
		_top.mapx.scroll_to_hs();
	}
	catch(e) {
	}
	
	/**
	 * Officer-Box
	 */
	TutorialCtrl.build_and_expand.steps.move_unit.box = new PureFW.Container(
		document.body,
		(MammunUI.reference_width - 546) >> 1,
		100,
		546, 115
	);
	TutorialCtrl.build_and_expand.steps.move_unit.box.set_bg_img(
		'ui/backgrounds/tutorial/officer_tutorial_box_546x115.png'
	);
	TutorialCtrl.build_and_expand.steps.move_unit.box.set_css_class(
		'tutorial_box_small'
	);
	TutorialCtrl.build_and_expand.steps.move_unit.box_inner = new PureFW.Container(
		TutorialCtrl.build_and_expand.steps.move_unit.box,
		110, 10,
		TutorialCtrl.build_and_expand.steps.move_unit.box.width - 120,
		TutorialCtrl.build_and_expand.steps.move_unit.box.height - 20
	);
	TutorialCtrl.build_and_expand.steps.move_unit.box_inner.set_content(
		TutorialCtrl.build_and_expand.steps.move_unit.text
	);
	
	/**
	 * Handlers
	 */
	TutorialCtrl.build_and_expand.steps.move_unit.unit_moved_handler
		= function(ev)
	{
		TutorialCtrl.next(
			TutorialCtrl.build_and_expand,
			TutorialCtrl.build_and_expand.steps.move_unit
		);
	}
	TutorialCtrl.build_and_expand.steps.move_unit.unit_not_moved_handler
		= function(ev)
	{
		TutorialCtrl.previous(
			TutorialCtrl.build_and_expand,
			TutorialCtrl.build_and_expand.steps.move_unit
		);
	}
	MapDataCtrl.Movement.add_event_handler("move_successful",
		TutorialCtrl.build_and_expand.steps.move_unit.unit_moved_handler
	);
	MapDataCtrl.Movement.add_event_handler("move_unsuccessful",
		TutorialCtrl.build_and_expand.steps.move_unit.unit_not_moved_handler
	);
}
TutorialCtrl.build_and_expand.steps.move_unit.approve = function() {
	TutorialCtrl.build_and_expand.steps.move_unit.cancel();
}
TutorialCtrl.build_and_expand.steps.move_unit.cancel = function() {
	try {
		MapDataCtrl.Movement.remove_event_handler("move_successful",
			TutorialCtrl.build_and_expand.steps.move_unit.unit_moved_handler
		);
		MapDataCtrl.Movement.remove_event_handler("move_unsuccessful",
			TutorialCtrl.build_and_expand.steps.move_unit.unit_not_moved_handler
		);
	}
	catch(e){}
	try {
		TutorialCtrl.build_and_expand.steps.move_unit.arrows.destroy();
	}
	catch(e){}
	TutorialCtrl.build_and_expand.steps.move_unit.arrows = null;
	try {
		TutorialCtrl.build_and_expand.steps.move_unit.box_inner.destroy();
	}
	catch(e) {}
	TutorialCtrl.build_and_expand.steps.move_unit.box_inner = null;
	
	try {
		TutorialCtrl.build_and_expand.steps.move_unit.box.destroy();
	}
	catch(e) {}
	TutorialCtrl.build_and_expand.steps.move_unit.box = null;
}
TutorialCtrl.build_and_expand.steps.move_unit.hide = function() {
	try {
		var n = TutorialCtrl.build_and_expand.steps.move_unit.arrows.length;
	}
	catch(e) {
		var n = 0;
	}
	for (var i = 0; i < n; i++) {
		try {
			TutorialCtrl.build_and_expand.steps.move_unit.arrows[i].hide();
		}
		catch(e){}
	}
	try {
		TutorialCtrl.build_and_expand.steps.move_unit.box_inner.hide();
	}
	catch(e) {}
	
	try {
		TutorialCtrl.build_and_expand.steps.move_unit.box.hide();
	}
	catch(e) {}
}
TutorialCtrl.build_and_expand.steps.move_unit.show = function() {
	try {
		var n = TutorialCtrl.build_and_expand.steps.move_unit.arrows.length;
	}
	catch(e) {
		var n = 0;
	}
	for (var i = 0; i < n; i++) {
		try {
			TutorialCtrl.build_and_expand.steps.move_unit.arrows[i].show();
		}
		catch(e){}
	}
	try {
		TutorialCtrl.build_and_expand.steps.move_unit.box_inner.show();
	}
	catch(e) {}
	
	try {
		TutorialCtrl.build_and_expand.steps.move_unit.box.show();
	}
	catch(e) {}
}

/**
 * Jetzt soll er noch eine Einheit bewegen, damit sich das System bei ihm
 * verfestigt. Diesmal ohne Pfeile und mit fast keinen Hilfen.
 */

TutorialCtrl.build_and_expand.steps.move_second_unit.text = '';
TutorialCtrl.build_and_expand.steps.move_second_unit.flag_to_set = 0;
TutorialCtrl.build_and_expand.steps.move_second_unit.action = function() {
	TutorialCtrl.create_tut_officer_box(
		TutorialCtrl.build_and_expand.steps.move_second_unit.text
	);

	/**
	 * Wir checken wo wir sind und blenden ggf. die Hilfstextbox aus. Somit
	 * sparen wir uns Unmengen an Schritten und Branches usw.
	 */
	TutorialCtrl.build_and_expand.steps.move_second_unit.check_interval =
		PureFW.Timeout.set_interval(
			function() {
				/** Loading-Finished-Check */
				if ((typeof(SectorDetails) != 'undefined')
					&& SectorDetails && SectorDetails.loaded)
				{
					TutorialCtrl.tut_box.hide();
				}
				else {
					TutorialCtrl.tut_box.show();
				}
			}, 500
		);
	/**
	 * Handlers
	 */
	TutorialCtrl.build_and_expand.steps.move_second_unit.unit_moved_handler
		= function(ev)
	{
		TutorialCtrl.next(
			TutorialCtrl.build_and_expand,
			TutorialCtrl.build_and_expand.steps.move_second_unit
		);
	}
	MapDataCtrl.Movement.add_event_handler("move_successful",
		TutorialCtrl.build_and_expand.steps.move_second_unit.unit_moved_handler
	);
}
TutorialCtrl.build_and_expand.steps.move_second_unit.approve = function() {
	TutorialCtrl.build_and_expand.steps.move_second_unit.cancel();
}
TutorialCtrl.build_and_expand.steps.move_second_unit.cancel = function() {
	TutorialCtrl.destroy_tut_officer_box();
	PureFW.Timeout.clear_interval(
		TutorialCtrl.build_and_expand.steps.move_second_unit.check_interval
	);
}
TutorialCtrl.build_and_expand.steps.move_second_unit.hide = function() {
	TutorialCtrl.hide_tut_officer_box();
}
TutorialCtrl.build_and_expand.steps.move_second_unit.show = function() {
	TutorialCtrl.show_tut_officer_box();
}

/**
 * Der Schritt zeigt den Outro-Text an.
 * 
 * In diesem Schritt wird der abschließende Text, dass der Spieler das Tutorial
 * geschafft habe, angezeigt.
 */
TutorialCtrl.build_and_expand.steps.show_outro_text.text = '';
TutorialCtrl.build_and_expand.steps.show_outro_text.flag_to_set = 0;
TutorialCtrl.build_and_expand.steps.show_outro_text.action = function() {
	try {
		SectorDetails.this_window.destroy();
	} catch(e) {};
	TutorialCtrl.build_and_expand.steps.show_outro_text.box 
		= new PureFW.OfficerBox
	(
		document.body,
		(MammunUI.reference_width - 546) >> 1,
		(MammunUI.reference_height - 278) >> 1,
		546, 278
	);
	TutorialCtrl.build_and_expand.steps.show_outro_text.box.set_bg_img(
		'ui/backgrounds/tutorial/officer_tutorial_box_546x278.png'
	);
	TutorialCtrl.build_and_expand.steps.show_outro_text.box.set_css_class(
		'tutorial_box_big'
	)
	TutorialCtrl.build_and_expand.steps.show_outro_text.box.set_content(
		TutorialCtrl.build_and_expand.steps.show_outro_text.text
	);
	
	TutorialCtrl.build_and_expand.steps.show_outro_text.box.add_event_handler(
		"confirm",
		function() {
			TutorialCtrl.next(
				TutorialCtrl.build_and_expand,
				TutorialCtrl.build_and_expand.steps.show_outro_text
			);
		}
	);
}
TutorialCtrl.build_and_expand.steps.show_outro_text.approve = function() {
	TutorialCtrl.build_and_expand.steps.show_outro_text.cancel();
}
TutorialCtrl.build_and_expand.steps.show_outro_text.cancel = function() {
	try {
		TutorialCtrl.build_and_expand.steps.show_outro_text.box.destroy();
		TutorialCtrl.build_and_expand.steps.show_outro_text.box = null;
	}
	catch(e) {}	// was removed already
}
TutorialCtrl.build_and_expand.steps.show_outro_text.hide = function() {
	try {
		TutorialCtrl.build_and_expand.steps.show_outro_text.box.hide();
	}
	catch(e) {}	// was removed already
}
TutorialCtrl.build_and_expand.steps.show_outro_text.show = function() {
	try {
		TutorialCtrl.build_and_expand.steps.show_outro_text.box.show();
	}
	catch(e) {}	// was removed already
}



/**
 * Öffnet den Einladungsdialog
 */
TutorialCtrl.build_and_expand.steps.invite_friends.text = '';
TutorialCtrl.build_and_expand.steps.invite_friends.flag_to_set = 0;
TutorialCtrl.build_and_expand.steps.invite_friends.action = function() {
	if ((MapDataCtrl.players_on_map_amount >= MapDataCtrl.max_players)
		|| (MapDataCtrl.map_is_closed))
	{
		// Karte ist voll => Niemand muss eingeladen werden
		TutorialCtrl.next(
			TutorialCtrl.build_and_expand,
			TutorialCtrl.build_and_expand.steps.invite_friends
		);
		return;
	}
	
	TutorialCtrl.build_and_expand.steps.invite_friends.box = 
		new PureFW.OfficerBox
	(
		document.body,
		(MammunUI.reference_width - 546) >> 1,
		(MammunUI.reference_height - 278) >> 1,
		546, 278
	);
	TutorialCtrl.build_and_expand.steps.invite_friends.box.set_bg_img(
		'ui/backgrounds/tutorial/officer_tutorial_box_546x278.png'
	);
	TutorialCtrl.build_and_expand.steps.invite_friends.box.set_z_index(311);
	
	TutorialCtrl.build_and_expand.steps.invite_friends.box.set_css_class(
		'tutorial_box_big'
	);
	
	TutorialCtrl.build_and_expand.steps.invite_friends.box.set_content(
		TutorialCtrl.build_and_expand.steps.invite_friends.text
	);
	
	TutorialCtrl.build_and_expand.steps.invite_friends.box.add_event_handler(
		"confirm",
		function(ev) {
			UIButtons.open_col_choose_dialog("invite=true");
			/**
			 * Wenn das ColPosChoose-Objekt noch nicht bereit ist, dann warten...
			 */
			if ( (typeof(ColPosChoose) == 'undefined') || !ColPosChoose || 
				!ColPosChoose.loaded) 
			{
				TutorialCtrl.build_and_expand.steps.invite_friends.waiting_interval =
					PureFW.Timeout.set_interval(
						function() {
							if ((typeof(ColPosChoose) != 'undefined')
								&& ColPosChoose && ColPosChoose.loaded)
							{
								PureFW.Timeout.clear_interval(
									TutorialCtrl.build_and_expand.steps
									.invite_friends.waiting_interval
								);
								
								TutorialCtrl.build_and_expand.steps.invite_friends
								.action2();
							}
						}, 500
					);
			}
			else {
				TutorialCtrl.build_and_expand.steps.invite_friends.action2();
			}
		}
	);
}
TutorialCtrl.build_and_expand.steps.invite_friends.action2 = function() {
	/**
	 * Destroy-Event-Handler haben einen Bug, wie's scheint. Deswegen läuft
	 * das hier über die bewährte Sicherung.
	 */
	TutorialCtrl.build_and_expand.steps.invite_friends.trys = 0;
	TutorialCtrl.build_and_expand.steps.invite_friends.check_still_in_dialog =
		PureFW.Timeout.set_interval(
			function(ev) {
				if ((typeof(ColPosChoose) == 'undefined')
					|| !ColPosChoose || !ColPosChoose.loaded)
				{
					if (TutorialCtrl.build_and_expand.steps.invite_friends.trys < 2)
						TutorialCtrl.build_and_expand.steps.invite_friends.trys++;
					else {
						PureFW.Timeout.clear_interval(
							TutorialCtrl.build_and_expand.steps
							.invite_friends.check_still_in_dialog
						);
						
						TutorialCtrl.next(
							TutorialCtrl.build_and_expand,
							TutorialCtrl.build_and_expand.steps.invite_friends
						);
					}
				}
			}, 500
		);
}
TutorialCtrl.build_and_expand.steps.invite_friends.approve = function() {
	TutorialCtrl.build_and_expand.steps.invite_friends.cancel();
}
TutorialCtrl.build_and_expand.steps.invite_friends.cancel = function() {
	TutorialCtrl.destroy_tut_officer_box();
}
TutorialCtrl.build_and_expand.steps.invite_friends.hide = function() {
	TutorialCtrl.hide_tut_officer_box();
}
TutorialCtrl.build_and_expand.steps.invite_friends.show = function() {
	TutorialCtrl.show_tut_officer_box();
}
