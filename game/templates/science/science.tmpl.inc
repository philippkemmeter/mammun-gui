<?
/**
 * ACHTUNG: Wegen automatischem Tagschließen von Browsern nach innerHTML darf
 * kein JS innerhalb von Tags stehen! Am besten JS am Anfang und am Ende der
 * Datei reinschreiben. Wenn innerhalb von Tabellen entstehen meist
 * Tabellenfehler!
 */
$this_id = rand(0,1000000);	// eindeutige ID
/**
 * ACHTUNG: Allen Variablen das Suffix $this_id anhängen, da es sonst passieren
 * kann, dass Werte von Variablen in der Funktion, die eval aufruft, geändert
 * werden. Beispiel:
 * function x() {
 *    var l = 1;
 *    eval("var l = 2");  // danach ist l überschrieben
 *    eval("var l<?=$this_id?> = 2"); // danach ist l NICHT überschrieben
 * }
 */
?>
<script type="text/javascript">
Science = new Object();

<? /**
    * Der Forschungstribe wird analog zu mini_tribes.tmpl.inc erzeugt.
    * Mehr Infos zu den einzelnen Eigenschaften sind dort zu sehen.
    *
    * @see mini_unit_production.tmpl.inc
    * @author Philipp Kemmeter
    */ ?>
Science.sc_tribe = new Object();
Science.sc_tribe.id = '<?= $scientist_tribe->get_id() ?>';
Science.sc_tribe.name = '<?= $scientist_tribe->get_name() ?>';
Science.sc_tribe.count = <?= $scientist_tribe->get_count() ?>;
Science.sc_tribe.raw_count = <?= $scientist_tribe->get_raw_count() ?>;
Science.sc_tribe.must_add = <?= $scientist_tribe->get_morphing_count() ?>;
Science.sc_tribe.add_stamp = <?= $scientist_tribe->get_add_stamp() ?>;
Science.sc_tribe.add_value = <?= $scientist_tribe->get_add_value() ?>;
Science.sc_tribe.is_morphing = '<?= $scientist_tribe->is_morphing() ?>';
Science.sc_tribe.pic_url = '<?= pic_path(str_replace('.gif', '_icon54x54.png',
 $scientist_tribe->get_pic_url())) ?>';
Science.sc_tribe.cost = <?= $scientist_tribe->get_cost()*1000 ?>;
Science.sc_tribe.time_needed = <?=
	($scientist_tribe->get_add_value() == 0)
		? $scientist_tribe->get_morph_duration()*1000
		: $scientist_tribe->get_morph_duration()
					*$scientist_tribe->get_add_value() ?>;
Science.tribe_icon = null;	// später

Science.scs_total = <?= floor($scientist_tribe->get_count()/1000) ?>;
Science.project = new Array();
Science.old_t_now = PureFW.Time.get_current_server_time();
Science.this_window = PureFW.ContainerAJAX.instances[<?=$this_win_id?>];

Science.ootext = '<?= $officer_text ?>';

Science.click_yes = null;
Science.click_no = null;

Science.available_sciences = new Object();
<? foreach($researchable_scs as $type => $scs) :?>
	Science.available_sciences.<?=$type?> = new Array();
	<? foreach($scs as $sc_id) : ?>
		n<?=$this_id?> = Science.available_sciences.<?=$type?>.length;
		Science.available_sciences.<?=$type?>[n<?=$this_id?>] = new Object();
		Science.available_sciences.<?=$type?>[n<?=$this_id?>].name =
			'<?= (in_array($sc_id, ScienceConstants::$VERY_IMPORTANT_SCS))
				? '<b>'.ScienceConstants::$SC_NAME[$sc_id].'</b>'
				: ScienceConstants::$SC_NAME[$sc_id] ?>';
		Science.available_sciences.<?=$type?>[n<?=$this_id?>].desc =
			'<?= ScienceConstants::$SC_DESC[$sc_id] ?>';
		Science.available_sciences.<?=$type?>[n<?=$this_id?>].dataX =
			'<?=$cryptObj->do_encryption(array('sc_id' => $sc_id))?>';

<? /** Erweitern um die Eigenschaften des ScienceProjects, wenn gerade läuft */?>
	<? if (@$science_projects[$sc_id]) { ?>
		Science.available_sciences.<?=$type?>[n<?=$this_id?>].sc_count =
			<?= floor($science_projects[$sc_id]->get_sc_count()/1000) ?>;
		Science.available_sciences.<?=$type?>[n<?=$this_id?>].progress =
			<?= $science_projects[$sc_id]->get_progress() ?>;
		Science.available_sciences.<?=$type?>[n<?=$this_id?>].finish_time =
			<?= $science_projects[$sc_id]->get_finish_time() ?>;
		Science.available_sciences.<?=$type?>[n<?=$this_id?>].time_needed =
			<?= $science_projects[$sc_id]->get_time_needed() ?>;
		Science.available_sciences.<?=$type?>[n<?=$this_id?>].project_index
			= Science.project.length;
		Science.project.push(Science.available_sciences.<?=$type?>[n<?=$this_id?>]);
	<? } else {
		/**
		 * time_needed auf die Zeit setzen, die die Forschung brauchen würde,
		 * wenn alle Forscher des Spielers daran forschen würden.
		 */
		$sc_values = Techtree::get_instance($current_user->get_id())
			->get_known_science_values($sc_id, false, false);
		$time_needed = ceil(Science::get_instance($current_user->get_id())
			->science_time_needed($sc_values->grow,
				floor($scientist_tribe->get_count()/1000)*1000));
	?>
		Science.available_sciences.<?=$type?>[n<?=$this_id?>].sc_count = 0;
		Science.available_sciences.<?=$type?>[n<?=$this_id?>].progress = 0;
		Science.available_sciences.<?=$type?>[n<?=$this_id?>].finish_time = 0;
		Science.available_sciences.<?=$type?>[n<?=$this_id?>].time_needed =
			<?= $time_needed ?>;
	<? } ?>
<? endforeach ?>
<? endforeach ?>


slider<?=$this_id?> = new Array();
progress_bar<?=$this_id?> = new Array();
Science.researches = new Array();
active_research_names<?=$this_id?> = new Array();
active_research_progresses<?=$this_id?> = new Array();

<? /**
    * Diese Funktion eröffnet dem User die Wahl, einen neuen Forscher zu
    * trainieren, oder eben nicht. Confirmation halt :)
    */ ?>
Science.morph_confirmation = function() {
	if (Science.click_no)
		Science.officer.remove_event_handler("cancel", Science.click_no);
	if (Science.click_yes)
		Science.officer.remove_event_handler("confirm", Science.click_yes);

	Science.click_no =
		function(ev)
		{
			Science.click_yes = null;
			Science.click_no = null;
			Science.officer.remove_event_handler("confirm", Science.click_yes);
			Science.officer.remove_event_handler("cancel", Science.click_no);
			Science.officer.balloon_container.show();
			Science.officer.set_text(Science.ootext);
		};

	Science.click_yes =
		function(ev)
		{
			Science.click_no(ev);
			Science.morph_request(1);
		};
	Science.officer.add_event_handler("confirm", Science.click_yes);
	Science.officer.add_event_handler("cancel", Science.click_no);
	Science.officer.balloon_container.show();
	Science.officer.show_confirm_dialog(
		'<?= LNG_ONE_MORE_CONFIRMATION_TEXT?>'.replace(
			/\[NAHRUNG\]/,
			number_format(Science.sc_tribe.cost, 0, ',', '.')
		)
	);
}

<? /**
    * Sendet die Anfrage an den Server, dass amount Forscher produziert werden
    * sollen.
    *
    * @param uint amount	Wie viele produziert werden sollen
    * @author Philipp Kemmeter
    */ ?>
Science.morph_request = function(amount) {
	var url = "unit_production.php?action=morph"+
		"&id=<?=$scientist_tribe->get_id()?>"+
		"&amount="+amount;
	PureFW.AJAXClientServer.send_request(url, Science.morph_response);
	ui_refresh();
}
<? /**
    * Hier wird die Serverantwort der Morph-Anfrage abgefangen und behandelt.
    * Es wird entschieden, ob der Morph als Erfolgreich vom Server angenommen
    * wurde, oder nicht, und getan, was daraus folgt (Aktuallisierung des
    * Tribes oder Auswurf eines Fehlers etc).
    *
    * @param String response_text	Die Antwortwerte in einem Array
    * @see JS:Science.morph_request
    * @author Philipp Kemmeter
    */ ?>
Science.morph_response = function(response_arr) {
	if (response_arr[0] == 1) {	// successful morph
		<? /* Tribe aktualisieren */ ?>
		Science.sc_tribe.add_stamp = parseInt(response_arr[3]);
		Science.sc_tribe.is_morphing = true;

		<? /* Essensmenge korrigieren (das morphen ist nicht umsonst) */ ?>
		if (ui_food_gainer)
			ui_food_gainer.set_current_amount(parseInt(response_arr[6]));
		<? /* Einheitenlimit anpassen */ ?>
		Overview.increase_unit_produced(1);

		<? /* Punkte anpassen */ ?>
		if (PauseMenu.statistics.ui_my_score_gainer) { // Map-Score
			PauseMenu.statistics.ui_my_score_gainer.set_current_amount(
				parseInt(response_arr[7])
			);
			PauseMenu.rt_refresh();
		}
		//ui_refresh();
	}
	else {
		MammunUI.show_warning(350, 200,
			'<font color="red">'+response_arr[1]+'</font>',
			PureFW.ConfirmationBox.NO);
	}
}

<? /**
    * Diese Funktion eröffnet dem User die Wahl, einen Forscher zu entlassen
    * oder eben nicht. Confirmation halt :)
    * @author Philipp Kemmeter
    */ ?>
Science.unmorph_confirmation = function() {
	if (Science.click_no)
		Science.officer.remove_event_handler("cancel", Science.click_no);
	if (Science.click_yes)
		Science.officer.remove_event_handler("confirm", Science.click_yes);
	Science.click_no =
		function(ev)
		{
			Science.click_yes = null;
			Science.click_no = null;
			Science.officer.remove_event_handler("confirm", Science.click_yes);
			Science.officer.remove_event_handler("cancel", Science.click_no);
			Science.officer.balloon_container.show();
			Science.officer.set_text(Science.ootext);
		};

	Science.click_yes =
		function(ev)
		{
			Science.click_no(ev);
			Science.unmorph_request(1);
		};
	Science.officer.add_event_handler("confirm", Science.click_yes);
	Science.officer.add_event_handler("cancel", Science.click_no);
	Science.officer.show_confirm_dialog(
		'<?= LNG_ONE_LESS_CONFIRMATION_TEXT?>'
	);
}

<? /**
    * Sendet die Anfrage an den Server, dass amount Forscher entlassen werden
    * sollen.
    *
    * @param uint amount
    * @author Philipp Kemmeter
    */ ?>
Science.unmorph_request = function(amount) {
	var url = "unit_production.php?action=unmorph"+
		"&id=<?=$scientist_tribe->get_id()?>"+
		"&amount="+amount;
	PureFW.AJAXClientServer.send_request(url, Science.unmorph_response);
}

<? /**
    * Hier wird die Serverantwort der Morph-Anfrage abgefangen und behandelt.
    * Es wird entschieden, ob der Morph als Erfolgreich vom Server angenommen
    * wurde, oder nicht, und getan, was daraus folgt (Aktuallisierung des
    * Tribes oder Auswurf eines Fehlers etc).
    *
    * @param String response_text	Die Antwortwerte in einem Array
    * @see JS:Science.morph_request
    * @author Philipp Kemmeter
    */ ?>
Science.unmorph_response = function(response_arr) {
	if (response_arr[0] == 1) {	// successful unmorph
		<? /* Tribe aktualisieren */ ?>
		Science.sc_tribe.count -= parseInt(response_arr[2]);
		Science.sc_tribe.raw_count -= parseInt(response_arr[2]);

		<? /* Einheitenlimit anpassen */ ?>
		Overview.increase_unit_produced(-1);
		ui_refresh();
	}
	else
		throw response_arr[1];
}


<? /**
    * Refresht den aktuellen Forschungsstand aller Projekte.
    *
    * @see JS: Science.rtRefresh
    * @author Philipp Kemmeter
    */ ?>
Science.rtResearch = function()
{
	if (!Science.this_window)
	{
		Science.destroy();
		return;
	}

	var t_now = PureFW.Time.get_current_server_time();

	for (var i = 0; i < Science.project.length; i++)
	{
		if (Science.project[i].finish_time - t_now < 0) {
			Science.this_window.set_url('science.php');
			return;
		}

		var progress = Science.project[i].progress +
			(t_now - Science.old_t_now) / Science.project[i].time_needed;

		var prozent = Math.round(progress*10000)/100 + '';
		<? /* Länge der progressbar (grüne) wird hier bestimmt */ ?>
		if(progress_bar<?=$this_id?>[i])
			progress_bar<?=$this_id?>[i].set_value(prozent);
		var length = (prozent.indexOf('.') == 2)
			? prozent.length : prozent.length+1;
		switch (length) {
			case 2:
				prozent += ".";
			case 3:
				prozent += ".0";
			case 4:
				prozent += "0";
		}
		prozent = Math.floor(prozent);
		if (isNaN(prozent))
			prozent = 0;
		if(active_research_progresses<?=$this_id?>[i])
			active_research_progresses<?=$this_id?>[i].set_content(prozent+'%');

		if(progress_bar<?=$this_id?>[i])
			progress_bar<?=$this_id?>[i].set_time(
				PureFW.Time.sec_in_time(
					Science.project[i].finish_time - t_now)
				);
	}
}

<? /**
	* Aktualisiert den ggf. morphenden Scientist-Tribe.
	*
	* @see JS: mini_tribes__rtMorphing();
	* @author Philipp Kemmeter
	*/ ?>
Science.rtMorph = function()
{
	if (!Science.sc_tribe.is_morphing)
		return;
	var j = new Date();
	var t_now = PureFW.Time.get_current_server_time();

	var secs_left = Science.sc_tribe.time_needed
		- (t_now - Science.sc_tribe.add_stamp);
	var secintime = PureFW.Time.sec_in_time(secs_left, true, false);

	document.getElemById('ScienceMorphText').innerHTML = secintime;

	if (secs_left <= 0) {
		; //TODO aktuelle Tribes vom Server holen (reload - zur Sicherheit)
		Science.sc_tribe.is_morphing = false;
	}
}

<? /**
    * Refresht alles, was in Echtzeit aktuallisiert werden soll
    *
    * @author Philipp Kemmeter
    */ ?>
Science.rtRefresh = function() {
	Science.rtResearch();
	Science.rtMorph();
}

Science.interval = PureFW.Timeout.set_interval(Science.rtRefresh, 1000);

<? /**
    * Hier kann man dann sich für das Ereignis registrieren, wenn man an dieses
    * Array eine Funktion anhängt.
    */ ?>
Science.research_started_functions = new Array();
Science.available_science_clicked_functions = new Array();
Science.research_not_started_functions = new Array();
Science.available_science_click = function(type, nr)
{
	var chosen_sc = Science.available_sciences[type][nr];
	var meta_science = '<u>'+chosen_sc.name+'</u> '
		+ '(<?= LNG_TIME_NEEDED_ALL_SCIENTISTS ?>'.replace(
			/\[TIME\]/g, PureFW.Time.sec_in_time(chosen_sc.time_needed)
		)+')'+
		'<br/><br/>'+chosen_sc.desc;

	if (Science.click_no)
		Science.officer.remove_event_handler("cancel", Science.click_no);
	if (Science.click_yes)
		Science.officer.remove_event_handler("confirm", Science.click_yes);

	Science.click_no =
		function(ev)
		{
			Science.click_yes = null;
			Science.click_no = null;
			var n = Science.research_not_started_functions.length;
			for (var i = 0; i < n; i++) {
				Science.research_not_started_functions[i].call(this,
					PureFW.Widget.create_event('research_not_started'),
					chosen_sc);
			}
			Science.officer.balloon_container.show();
			Science.officer.set_text(Science.ootext);
			Science.officer.remove_event_handler("cancel", Science.click_no);
			Science.officer.remove_event_handler("confirm", Science.click_yes);
			Science.officer.balloon_container.set_css_class('OfficerBalloon');
		};

	Science.click_yes =
		function(ev)
		{
			var n = Science.research_started_functions.length;
			for (var i = 0; i < n; i++) {
				Science.research_started_functions[i].call(this,
					PureFW.Widget.create_event('science_started'), chosen_sc);
			}
			Science.click_no(ev);
			Science.this_window.set_url(
				'<?=$_SERVER['PHP_SELF']?>?dataX='+chosen_sc.dataX
					+'&set_sc=1000000&cur_sc='+chosen_sc.sc_count);

		};
	Science.officer.add_event_handler("confirm", Science.click_yes);
	Science.officer.add_event_handler("cancel", Science.click_no);
	Science.officer.balloon_container.show();
	Science.officer.show_confirm_dialog(meta_science);
	Science.officer.balloon_container.set_css_class('OfficerBalloon_small');

	var n = Science.available_science_clicked_functions.length;
	for (var i = 0; i < n; i++) {
		Science.available_science_clicked_functions[i].call(this,
			PureFW.Widget.create_event('available_science_clicked'),
			chosen_sc);
	}
}

Science.destroy = function() {
	Science.research_general.destroy();
	Science.bg_general.destroy();
	Science.research_abilities.destroy();
	Science.bg_abilities.destroy();
	Science.bg_races.destroy();
	Science.research_races.destroy();
	Science.bg_buildings.destroy();
	Science.research_buildings.destroy();

	PureFW.Timeout.clear_interval(Science.interval);
	Science.officer.destroy();
	Science.this_window.remove_event_handler("destroy", Science.destroy);
	Science.this_window.remove_event_handler("change", Science.destroy);
	var n = slider<?=$this_id?>.length;
	for (var i = 0; i < n; i++)
	{
		slider<?=$this_id?>[i].destroy();
		progress_bar<?=$this_id?>[i].destroy();
		Science.researches[i].destroy();
		active_research_names<?=$this_id?>[i].destroy();
		active_research_progresses<?=$this_id?>[i].destroy();
	}
	Science = null;
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
var cur_sc_at_work = 0;

<? /* Destruktor registrieren. */ ?>
Science.scientist_container = new PureFW.Container(
	Science.this_window,
	10,0,
	200, 60
);

Science.scientist_container.set_text_align('center');
Science.scientist_container.set_font_color('black');
Science.scientist_container.set_content(
	'<b><?= LNG_YOU_HAVE ?> ' +
		Math.floor(Science.sc_tribe.count/1000) +
		' <?= floor($scientist_tribe->get_count()/1000) == 1 ?
			LNG_SCIENTISTS_SG : LNG_SCIENTISTS_PL ?></b><br/>'+
	'<span id="ScienceMorphText">'+
		'<a href="javascript: Science.morph_confirmation()" style="color: #000">'+
			'<?= LNG_ONE_MORE_SC ?>'+
		'</a>'+
	'</span><br/>'<?/*+
	'<a href="javascript: Science.unmorph_confirmation()" style="color: #000">'+
		'<?= LNG_ONE_LESS_SC ?>'+
	'</a>'*/?>
);


send_request<?=$this_id?> = new Array();
on_slide<?=$this_id?> = new Array();

on_click<?=$this_id?> = new Array();

Science.bg_general = new PureFW.Container(
	Science.this_window,
	220, 65,
	225, 360
);

Science.bg_general.set_content(
	'<h3 style="text-align: center; width: 100%;"><?= LNG_GENERAL ?></h3>'
);
Science.bg_general.set_bg_img(
	"ui/backgrounds/scienceBgs/project_general_background.png"
);
Science.bg_general.get_node().style.overflow = 'hidden';

Science.research_general = new PureFW.ScrollContainer(
	Science.bg_general.get_content_node(),
	0, 18,
	Science.bg_general.width, Science.bg_general.height - 21);

Science.bg_races = new PureFW.Container(
	Science.this_window,
	Science.bg_general.position.x + Science.bg_general.width + 10,
	Science.bg_general.position.y,
	225, 175
);

Science.bg_races.set_content(
	'<h3 style="text-align: center; width: 100%;"><?= LNG_RACES ?></h3>'
);
Science.bg_races.set_bg_img(
	"ui/backgrounds/scienceBgs/project_races_background.png"
);
Science.bg_races.get_node().style.overflow = 'hidden';

Science.research_races = new PureFW.ScrollContainer(
	Science.bg_races.get_content_node(),
	0, 18,
	Science.bg_races.width, Science.bg_races.height - 21);

Science.bg_buildings = new PureFW.Container(
	Science.this_window,
	Science.bg_races.position.x,
	Science.bg_races.position.y + Science.bg_races.height + 10,
	225, 175
);

Science.bg_buildings.set_content(
	'<h3 style="text-align: center; width: 100%;"><?= LNG_BUILDINGS ?></h3>'
);
Science.bg_buildings.set_bg_img(
	"ui/backgrounds/scienceBgs/project_buildings_background.png"
);
Science.bg_buildings.get_node().style.overflow = 'hidden';

Science.research_buildings = new PureFW.ScrollContainer(
	Science.bg_buildings.get_content_node(),
	0, 18,
	Science.bg_buildings.width, Science.bg_buildings.height - 21);

Science.bg_abilities = new PureFW.Container(
	Science.this_window,
	Science.bg_races.position.x + Science.bg_races.width + 10,
	Science.bg_general.position.y,
	225, 360
);

Science.bg_abilities.set_content(
	'<h3 style="text-align: center; width: 100%;"><?= LNG_ABILITIES ?></h3>'
);
Science.bg_abilities.set_bg_img(
	"ui/backgrounds/scienceBgs/project_abilities_background.png"
);
Science.bg_abilities.get_node().style.overflow = 'hidden';

Science.research_abilities = new PureFW.ScrollContainer(
	Science.bg_abilities.get_content_node(),
	0, 18,
	Science.bg_abilities.width, Science.bg_abilities.height - 21);




general_researches<?=$this_id?> = new Array();
abilities_researches<?=$this_id?> = new Array();
races_researches<?=$this_id?> = new Array();
buildings_researches<?=$this_id?> = new Array();

<? /* DIVs für die Forschungswahl werden jetzt generiert und zugewiesen */ ?>
{
	var y_position = 20;
	for (var type in Science.available_sciences)
	{
		var n = Science.available_sciences[type].length;
		for (var i = 0; i < n; i++)
		{
			if(Science.available_sciences[type][i].sc_count <= 0)
			{
				var inactive_science_height = 17; <?/* Change this to edit height
																	of all! */?>
				switch (type)
				{
					case 'general':
					{
						Science.researches[i] = new PureFW.Container(
							Science.research_general.get_content_node(),
							5, 2,
							Science.research_general.width - 13,
							inactive_science_height);
						general_researches<?=$this_id?>.push(Science.researches[i]);
						break;
					}
					case 'abilities':
					{
						Science.researches[i] = new PureFW.Container(
							Science.research_abilities.get_content_node(),
							5, 2,
							Science.research_abilities.width - 13,
							inactive_science_height);
						abilities_researches<?=$this_id?>.push(
							Science.researches[i]);
						break;
					}
					case 'races':
					{
						Science.researches[i] = new PureFW.Container(
							Science.research_races.get_content_node(),
							5, 2,
							Science.research_races.width - 13,
							inactive_science_height);
						races_researches<?=$this_id?>.push(
							Science.researches[i]);
						break;
					}
					case 'buildings':
					{
						Science.researches[i] = new PureFW.Container(
							Science.research_buildings.get_content_node(),
							5, 2,
							Science.research_buildings.width - 13,
							inactive_science_height);
						buildings_researches<?=$this_id?>.push(
							Science.researches[i]);
						break;
					}
				}

				Science.researches[i].set_content(
					Science.available_sciences[type][i].name);
				Science.researches[i].get_node().style.overflow = 'hidden'
				Science.researches[i].set_positioning('relative');

				on_click<?=$this_id?>[i] = function(_type, _i)
				{
					return function(ev)
					{
						Science.available_science_click(_type, _i);
					}
				}(type, i);

				Science.researches[i].add_event_handler(
					"click", on_click<?=$this_id?>[i]);
			}
			else
			{
				var current_index
					= Science.available_sciences[type][i].project_index;
				var active_science_height = 60; <?/* Change this to edit height of all! */?>
				switch (type)
				{
					case 'general':
						Science.researches[current_index]
											= new PureFW.Container(
							Science.research_general.get_content_node(),
							0, 2,
							Science.research_general.width, active_science_height);
						general_researches<?=$this_id?>.push(
							Science.researches[current_index]);
						break;
					case 'abilities':
						Science.researches[current_index]
											= new PureFW.Container(
							Science.research_abilities.get_content_node(),
							0, 2,
							Science.research_abilities.width, active_science_height);
						abilities_researches<?=$this_id?>.push(
							Science.researches[current_index]);
						break;
					case 'races':
						Science.researches[current_index]
											= new PureFW.Container(
							Science.research_races.get_content_node(),
							0, 2,
							Science.research_races.width, active_science_height);
						races_researches<?=$this_id?>.push(
							Science.researches[current_index]);
						break;
					case 'buildings':
						Science.researches[current_index] = new PureFW.Container(
							Science.research_buildings.get_content_node(),
							0, 2,
							Science.research_buildings.width,
							active_science_height);
						buildings_researches<?=$this_id?>.push(
							Science.researches[current_index]);
						break;
				}
				Science.researches[current_index].set_positioning('relative');
				Science.researches[current_index].set_bg_img(
					"ui/backgrounds/scienceBgs/project_bg.png");

				active_research_names<?=$this_id?>[current_index]
				                   				= new PureFW.Container(
					Science.researches[current_index].get_content_node(),
					5, 0,
					Science.researches[current_index].width - 33, 16);
				active_research_names<?=$this_id?>[current_index].set_content(
					Science.available_sciences[type][i].name);
				active_research_names<?=$this_id?>[current_index].get_node()
					.style.overflow = 'hidden';
				active_research_names<?=$this_id?>[current_index]
				                   				.set_font_weight('bold');

				active_research_progresses<?=$this_id?>[current_index]
				                        				= new PureFW.Container(
					Science.researches[current_index].get_content_node(),
					Science.researches[current_index].width - 33, 0,
					23, 17);
				active_research_progresses<?=$this_id?>[current_index]
				                        		.set_positioning('absolute');
				active_research_progresses<?=$this_id?>[current_index]
				                        		.set_font_weight('bold');

				progress_bar<?=$this_id?>[current_index]
				          							= new PureFW.ProgressBar(
					Science.researches[current_index].get_content_node(),
					13, 20,
					Science.researches[current_index].width - 27, 16);
				progress_bar<?=$this_id?>[current_index]
				          				.set_positioning('relative');

				send_request<?=$this_id?>[current_index] =
					(function(_i, _cur_sc) {
						return function(ev) {
							Science.this_window.set_url(
								'science.php?dataX='+Science.
								project[_i].dataX+'&set_sc='+ev.detail
								+'&cur_sc='+_cur_sc);
						}
					})(current_index, Science.project[current_index].sc_count);

				slider<?=$this_id?>[current_index] = new PureFW.Slider(
					Science.researches[current_index].get_content_node(),
					8, 36,
					Science.researches[current_index].width - 17, 20,
					0, Science.scs_total,
					Science.project[current_index].sc_count);
				slider<?=$this_id?>[current_index].add_event_handler(
					"value_change", send_request<?=$this_id?>[current_index]);
				<?php /* Declarations of progress_bar, researches,
				and slider Arrays are in the science_js_decl.tmp file */?>
			}
		}
	}
}
Science.calculate_height = function(arr)
{
	if(arr)
	{
		var total = 0;
		var size = arr.length;
		for(var x = 0; x < size; x++)
		{
			total = total + arr[x].height;
		}
		return total;
	}
	return 0;
}

Science.research_general.set_inner_height(
	Science.calculate_height(general_researches<?=$this_id?>));
Science.research_races.set_inner_height(
	Science.calculate_height(races_researches<?=$this_id?>));
Science.research_abilities.set_inner_height(
	Science.calculate_height(abilities_researches<?=$this_id?>));
Science.research_buildings.set_inner_height(
	Science.calculate_height(buildings_researches<?=$this_id?>));


<? if ($LNG == 'en') : ?>
Science.achievements_button = new PureFW.Image(
	Science.this_window,
	825, 0,
	93, 52,
	'ui/elements/form/buttons/<?=$LNG?>/science/achievements.png'
);
<? else : ?>
Science.achievements_button = new PureFW.Image(
	Science.this_window,
	830, 0,
	82, 62,
	'ui/elements/form/buttons/<?=$LNG?>/science/achievements.png'
);
<? endif; ?>

Science.achievements_button.add_event_handler(
	"click",
	function (ev) {
		UIButtons.toggle_window('achievements.php');
	}
);

<? /* jetzt wurde alles gesetzt und rtRefresh kann beginnen :) */ ?>
Science.rtRefresh();

<? /* Punkteanpassen, da ggf. Forschungen fertig geworden sind auf dem Server,
	was zu einem Punktschub führt. Deswegen werden hier die Punkte immer
	nochmal neu gesetzt */ ?>
if (ui_my_score_gainer)
	ui_my_score_gainer.set_current_amount(<?=
		ScoreCache::get_map_score(
			$current_user->get_world_id(),
			$current_user->get_map_id(),
			$current_user->get_id()
		) ?>);
Science.officer = new PureFW.Officer(document.body,PureFW.Officer.OFFICERS.cso);
Science.officer.set_position(50,50);
Science.officer.set_officer_position(0, 50);
Science.officer.set_balloon_position(-20, 0);
Science.officer.show();
<? if ($popup_officer_text) : ?>
MammunUI.show_warning(350, 200, '<?=$officer_text?>', PureFW.ConfirmationBox.YES);
Science.officer.balloon_container.hide();
<? else : ?>
Science.officer.set_text('<?=$officer_text?>');
<? endif; ?>

<? /** TODO: Das ist etwas overkill, ein update der Eventmessages würde reichen.
	  Dazu muss MapDataCtrl erweitert werden, um nur das zu erfragen. TODO */?>
MapDataCtrl.update_map();

Science.this_window.add_event_handler("destroy", Science.destroy);
Science.this_window.add_event_handler("change", Science.destroy);

<? if ($sc_total <= $sc_at_work) : ?>
UIButtons.bb3_zzz.stop_animation();
UIButtons.bb3_zzz.hide();
<? else : ?>
UIButtons.bb3_zzz.start_animation();
UIButtons.bb3_zzz.show();
<? endif; ?>

Science.loaded = true;
</script>
