<?
$sec_w = 138<<2;	// Sektor-Breite
$sec_h = 106<<2;	// Sektor-Höhe
$sec_w_2 = $sec_w>>1;	// halbe Breite
$sec_h_2 = $sec_h>>1;	// halbe Höhe
$x_offset = 224;		// wo der aktuelle Sektor erscheinen soll in x (absolut)
$y_offset = 200;	// wo der aktuelle Sektor erscheinen soll in y (absolut)
$sec_hover = 0;		// wie weit der aktuelle Sektor über dem Boden schweben soll
$unit_width = 50;
$unit_height = 50;
Language::get_instance()->load_language_file('sector_details.tmpl');
Language::get_instance()->load_language_file('buildings_action');
Language::get_instance()->load_language_file('profile');
$this_id = rand(0,1000000);	// eindeutige ID?>

<script type="text/javascript">
if((typeof(SectorDetails) != 'undefined') && (SectorDetails))
{
	SectorDetails.leave_sector();
};

SectorDetails = new Object();
SectorDetails.loaded = false;
SectorDetails.new_building = null;
SectorDetails.here_to_build = false
SectorDetails.unit_popup = null;
SectorDetails.unit_details = null;
SectorDetails.dominator_nick = '<?= $this_sector->get_dominator_nick() ?>';
SectorDetails.my_nick = '<?= $current_user->get_nick() ?>';

SectorDetails.visitor =
	<?= (($current_user->get_world_id() != $world_id) &&
		($current_user->get_map_id() != $map_id)) ? 'true' : 'false' ?>;
SectorDetails.own_sector =
	((SectorDetails.dominator_nick == SectorDetails.my_nick) &&
		!SectorDetails.visitor);

SectorDetails.my_unit_amount = 0;	<? /* Anzahl meiner Mammuns. Später. */ ?>
SectorDetails.my_building_amount = 0; <? /* Anzahl meiner Gebäude. Später. */ ?>

<? /** Event-Handler may register here for constrution_started-Events */ ?>
SectorDetails.construction_started_functions = new Array();

if((typeof(GlobalConstructionSite) != 'undefined') && (GlobalConstructionSite))
{
	SectorDetails.here_to_build = true;
	SectorDetails.pre_chosen_building = GlobalConstructionSite;
	delete GlobalConstructionSite;
}

<? if (isset($tutorial_texts)) { ?>
	SectorDetails.tutorial_texts = <?= json_encode($tutorial_texts) ?>;
<? } ?>

SectorDetails.user_interaction_enabled = <?
	echo ($current_user->get_waiting_state() == 0) ? "true" : "false" ?>

SectorDetails.this_window = FullWindowAJAX.instance;

SectorDetails.build_tab_inner_html_old = '';
SectorDetails.active_tab_old = -1;


<? /** X-Button zum Schließen der Seite. Das wird als aller erstes gemacht,
		damit der Spieler in jedem Fall den Sektor verlassen kann! **/ ?>
SectorDetails.close_button = new PureFW.Image(
	SectorDetails.this_window,
	'<?= $x_offset + (($sec_w - 54) >> 1) ?>',
	'<?= $y_offset+$sec_h-50?>',
	54,54,
	'ui/elements/form/buttons/confirm/confirm_no.png'
);
SectorDetails.close_button.set_z_index(5);

SectorDetails.close_button.add_event_handler(
	"click",
	function (ev) {
		SectorDetails.this_window.destroy();
	}
);

<? /* Destruktor definieren */?>
SectorDetails.leave_sector = function()
{
//	SectorDetails.stop_building();
	try
	{
		SectorDetails.this_window.remove_event_handler(
										"destroy", SectorDetails.leave_sector);
		PopupMenu__hide_all();
		ui_show_map_scroll_arrows();
		destroy_object(SectorDetails);
		delete SectorDetails;
	}
	catch(e){
		alert(e);
	}
	SectorDetails = null;		// <= Für den Internet Explorer
}
SectorDetails.this_window.add_event_handler("destroy",
	SectorDetails.leave_sector);

if(SectorDetails.user_interaction_enabled)
{
	SectorDetails.building_popup = new PopupMenu(0, '', document.body);
	SectorDetails.building_popup.add_entry(
		'<?= LNG_B_DESTROY ?>','javascript: SectorDetails.destroy();');
	SectorDetails.building_popup.add_entry(
		'<?= LNG_B_PAINT ?>',
		'javascript: UIButtons.toggle_buildings()'
	);

	if (SectorDetails.dominator_nick != SectorDetails.my_nick)
		SectorDetails.building_popup.deactivate_entry(1);

	SectorDetails.last_hit_unit = new Object();
	SectorDetails.last_hit_unit.id = 0;
	SectorDetails.last_hit_unit.time_per_sector = 0;

	SectorDetails.last_hit_building = null;
}

SectorDetails.move_unit = function(force)
{
	if (typeof(force) == 'undefined')
		force = false;
	if ((SectorDetails.my_unit_amount == 1) &&
		(SectorDetails.own_sector) &&
		(SectorDetails.my_building_amount == 0) && !force)
	{
		var tmp_confirm = MammunUI.show_warning(
			360,255,
			'<?= LNG_TEXT_SECTOR_SECTOR_WILL_BE_EMPTY_ON_CONTINUE ?>',
			PureFW.ConfirmationBox.YES | PureFW.ConfirmationBox.NO
		);
		tmp_confirm.add_event_handler("confirm",
			function(ev) {
				SectorDetails.move_unit(true);
			}
		);
		return;
	}
	var unit_array = new Array();
	unit_array[0] = new Object();
	unit_array[0].id = SectorDetails.last_hit_unit.id;
	unit_array[0].time_per_sector = SectorDetails.last_hit_unit.time_per_sector;
	unit_array[0].count = 1;
	MammunUI.get_top_frame().MapDataCtrl.Movement.units_move(
		<?= $sector_id ?>, unit_array
	);
	SectorDetails.this_window.destroy();
}


SectorDetails.open_unit_popup = function(unit_id, text) {
	var datas = SectorDetails.sector_zoom.get_unit_datas();
	for (var i in datas) {
		if (isNaN(i))
			continue;
		for (var j in datas[i]) {
			if (isNaN(j))
				continue;
			if (datas[i][j].id == unit_id) {
				SectorDetails.unit_clicked(
					PureFW.EventUtil.create_event("click", datas[i][j])
				);
				SectorDetails.unit_popup.speech_container.set_content(text);
				return;
			}
		}
	}
}

SectorDetails.unit_clicked = function(ev)
{
	var unit_info = ev.detail;
	if(SectorDetails.user_interaction_enabled)
	{
		SectorDetails.last_hit_unit.id = unit_info.id;
		SectorDetails.last_hit_unit.time_per_sector =
											<?= 100*7000*0.8?>/unit_info.speed;

		try {
			SectorDetails.unit_popup.destroy();
		}
		catch(e) {}
		SectorDetails.unit_popup = null;

		SectorDetails.unit_popup = new PureFW.SectorDetails.MammunSpeechBubble(
			document.body,
			SectorDetails.sector_zoom.position.x + unit_info.pos.x + 25,
			SectorDetails.sector_zoom.position.y + unit_info.pos.y - 183,
			199, 194,
			((unit_info.user_nick == '<?= $current_user->get_nick() ?>')
				&& !SectorDetails.visitor)
		);
		SectorDetails.unit_popup.set_z_index(200);

		SectorDetails.unit_popup.set_name(unit_info.name);

		<?	/**
			 * Meiner eigenen Einheit kann ich mehr Informationen entlocken und
			 * mit ihr kann ich Aktionen durchführen. Was hier also nur für
			 * meine Einheiten gilt ist:
			 * 1. Lebensenergieanzeige (exakt)
			 * 2. Dass er mich anspricht
			 * 3. Bewegungsbefehl.
			 *
			 * Aus Übersichts- und Stylegründen wird der Detailansicht-Button
			 * bei den fremden Einheiten woanders hinpositioniert, weswegen
			 * es einen Else-Fall gibt.
			 */ ?>
		if ((unit_info.user_nick == '<?= $current_user->get_nick() ?>')
			&& !SectorDetails.visitor)
		{
			SectorDetails.unit_popup.set_speech(
				MammunSpeech.get_speech(unit_info,
					MapDataCtrl.fights[<?=$sector_id?>], true)
			);
			SectorDetails.unit_popup.set_health(
				unit_info.amount/1000*unit_info.hp, unit_info.hp
			);

			SectorDetails.unit_popup.set_move_tooltip("<?= LNG_B_MOVE ?>")
			SectorDetails.unit_popup.add_event_handler("move_click",
				function () {
					SectorDetails.unit_popup.destroy();
					SectorDetails.move_unit();
				});

			SectorDetails.unit_popup.set_recloth_tooltip("<?=LNG_B_RECLOTH ?>");
			SectorDetails.unit_popup.add_event_handler("recloth_click",
				(function(_unit_info) {
					return function(ev) {
						UIButtons.toggle_unit_wizard(
							'id0=' + _unit_info.id + '&race_change='
								+ _unit_info.race + '&action=change_clothes'
						);
					}
				})(unit_info)
			);
		}
		<?	/**
			 * Andere Sprüche
			 */ ?>
		else {
			SectorDetails.unit_popup.set_speech(
				MammunSpeech.get_speech(unit_info,
					MapDataCtrl.fights[<?=$sector_id?>], false)
			);
		}

		SectorDetails.unit_popup.set_details_tooltip("<?=LNG_B_SHOW_BIG_PIC?>");
		SectorDetails.unit_popup.add_event_handler("details_click",
			function(_info)
			{
				return function(ev)
				{
					SectorDetails.unit_popup.destroy();
					_info.pic_url = _info.pic_url.replace('0.png', '0_0.png');
					SectorDetails.show_unit_details(_info);
				}
			}(unit_info)
		);
	}
}

SectorDetails.show_unit_details = function(info)
{
	if(!typeof(info) == 'undefined')
		return;
	MammunUI.show_unit_details(info);
}

SectorDetails.building_clicked = function(ev)
{
	if(SectorDetails.user_interaction_enabled)
	{
		SectorDetails.last_hit_building = ev.detail;
		SectorDetails.building_popup.set_tooltip(ev.detail.tooltip);
		var p = PureFW.MouseFeatures.get_cursor_pos();
		SectorDetails.building_popup.set_position(
			p.x, p.y
		);
		var clicked_building = null;
		for (i in MapDataCtrl.buildings[<?= $sector_id ?>]) {
			if (isNaN(i))
				continue;
			var b = MapDataCtrl.buildings[<?= $sector_id ?>][i];
			var ulc = b.ulc;
			if ((ulc[0] == SectorDetails.last_hit_building.pos.x)
				&& (ulc[1] == SectorDetails.last_hit_building.pos.y))
			{
				clicked_building = b;
				break;
			}
		}
		if (!clicked_building.custom && !clicked_building.neutral
			&& (MapDataCtrl.world_id == MapDataCtrl.my_world_id)
			&& (MapDataCtrl.map_id == MapDataCtrl.my_map_id))
		{
			if ((SectorDetails.dominator_nick != SectorDetails.my_nick)
				&& (b.color != 0))
			{
				SectorDetails.building_popup.activate_entry(2,
					'javascript: SectorDetails.officer.set_text(\'' +
						'<?= str_replace('[NICK]',
							$this_sector->get_dominator_nick(),
							LNG_ERR_CANNOT_PAINT_OTHERS_PAINTED) ?>\');'
				);
			}
			else {
				SectorDetails.building_popup.activate_entry(2,
					'javascript: UIButtons.toggle_buildings_window(\'' +
						'sector_id=<?=$sector_id?>&' +
						'x='+clicked_building.ulc[0]+'&' +
						'y='+clicked_building.ulc[1]+'&' +
						'type='+b.type+'&' +
						'color='+b.color+'&' +
						'nick='+SectorDetails.dominator_nick+'&' +
						'change_mode=true' +
					'\');'
				);
			}
		}
		else
			SectorDetails.building_popup.deactivate_entry(2);

		if (SectorDetails.own_sector) {
			if (clicked_building.neutral) {
				SectorDetails.building_popup.change_entry(
					1, '<?= LNG_B_DESTROY ?>'
				);
			}
			else {
				if (clicked_building.custom) {
					SectorDetails.building_popup.change_entry(
						1, '<?= LNG_B_PUT_IN_CHEST ?>',
						'javascript: SectorDetails.destroy();'
					);
				}
				else {
					SectorDetails.building_popup.change_entry(
						1, '<?= LNG_B_DESTROY ?>',
						'javascript: SectorDetails.destroy();'
					);
				}
			}
		}
		SectorDetails.last_hit_building.neutral = clicked_building.neutral;
		SectorDetails.last_hit_building.custom = clicked_building.custom;

		SectorDetails.building_popup.show();
	}
}

SectorDetails.building_mouseover = function(ev)
{
	if(SectorDetails.user_interaction_enabled)
	{
		SectorDetails.last_hit_building = ev.detail;

		var hover_building = null;
		for (i in MapDataCtrl.buildings[<?= $sector_id ?>]) {
			if (isNaN(i))
				continue;
			var b = MapDataCtrl.buildings[<?= $sector_id ?>][i];
			var ulc = b.ulc;
			if ((ulc[0] == SectorDetails.last_hit_building.pos.x)
				&& (ulc[1] == SectorDetails.last_hit_building.pos.y))
			{
				hover_building = b;
				break;
			}
		}
		if (hover_building.const_key == 'C_GEYSER') {
			var widget = SectorDetails.sector_zoom.get_building(
				hover_building.ulc[0], hover_building.ulc[1]);

			if (widget.b_ani) {
				widget.b_ani.set_strip(2);
			}
			else {
				var ani = widget.set_ani_as_img(
					'map/buildings/geyser_ani.png', 9, 50, 3
				);
				ani.set_frame_range(0, 2, 1);
				ani.set_frame_range(0, 1, 2);
				ani.add_event_handler("loop", function(ev) {
					if (this.strip == 0)
						this.set_strip(1);
					else if (this.strip == 2)
						this.destroy();
				});
			}
		}
	}
}

if(SectorDetails.user_interaction_enabled)
{
	SectorDetails.cur_new_building = null;
	SectorDetails.visible_fields = new Array();

	SectorDetails.hide_visible_fields = function()
	{
		var n = SectorDetails.visible_fields.length;
		for(var i = 0; i < n; i++)
		{
			SectorDetails.visible_fields[i].hide();
		}
	};

	SectorDetails.field_over_to_build = function(ev) {
		var coords = ev.detail;
		SectorDetails.set_new_building_coords(coords.x, coords.y);
	}

	SectorDetails.set_new_building_coords = function(field_x, field_y)
	{
		var building_size = SectorDetails.cur_new_building.size[0];
		var grid_width = SectorDetails.sector_zoom.get_num_fields_x();
		var grid_height = SectorDetails.sector_zoom.get_num_fields_y();
		var may_continue = true;

		SectorDetails.hide_visible_fields();
		/*SectorDetails.visible_fields.destroy();
		SectorDetails.visible_fields = new Array();*/

		if ((field_x + building_size) > grid_width)
		{
			field_x = grid_width - building_size;
		}
		if ((field_y + building_size) > grid_height)
		{
			field_y = grid_height - building_size;
		}

		var placed_building_matrix = SectorDetails.sector_zoom
			.get_placed_buildings();

		var grid_field;
		for (var b_x = field_x; b_x < building_size + field_x; b_x++)
		{
			for (var b_y = field_y;	b_y < building_size + field_y; b_y++)
			{
				if (placed_building_matrix[b_x]
				           				&& placed_building_matrix[b_x][b_y])
				{
					SectorDetails.new_building.hide();
					may_continue = false;
					// no return here, but abort later
				}
				grid_field = SectorDetails.sector_zoom
					.get_building_placement_grid_map().get_field(b_x, b_y);

				grid_field.show();
				SectorDetails.visible_fields.push(grid_field);
			}
		}

		if(!may_continue)
		{
			return;
		}
		grid_field = SectorDetails.sector_zoom
			.get_building_placement_grid_map().get_field(field_x, field_y);


		var pos_x = grid_field.position.x +
			(((1-SectorDetails.sector_zoom.b_map_scale)
				*SectorDetails.sector_zoom.width) >> 1);
		var pos_y = grid_field.position.y +
			(((1-SectorDetails.sector_zoom.b_map_scale)
				*SectorDetails.sector_zoom.height) >> 1);

		SectorDetails.new_building.set_x(
			pos_x - (((building_size-1)*grid_field.width) >> 1)
		);
		SectorDetails.new_building.set_y(
			pos_y - SectorDetails.new_building.height
						+ (building_size)*grid_field.height
		);
		SectorDetails.new_building.show();
	}


	SectorDetails.field_click_to_build = function(ev) {
		var coords = ev.detail;
		SectorDetails.build(coords.x, coords.y);
	}

<?	/**
	 * Sendet die Anfrage an den Server, dass an den übergebenen Daten das
	 * cur_new_building gebaut werden soll.
	 *
	 * @param uint field_x	die X-Koordinate
	 * @param uint field_y	die Y-Koordinate
	 * @see JS: fetch_build_response
	 */?>
	SectorDetails.build = function(field_x, field_y)
	{
		if ((field_x + SectorDetails.cur_new_building.size[0])
			 > SectorDetails.sector_zoom.get_num_fields_x())
		{
			field_x = SectorDetails.sector_zoom.get_num_fields_x()
				- SectorDetails.cur_new_building.size[0];
		}
		if ((field_y + SectorDetails.cur_new_building.size[0])
			 > SectorDetails.sector_zoom.get_num_fields_y())
		{
			field_y = SectorDetails.sector_zoom.get_num_fields_y()
				- SectorDetails.cur_new_building.size[0];
		}

		var placed_building_matrix = SectorDetails.sector_zoom
			.get_placed_buildings();
		for (var b_x = field_x;
			b_x < SectorDetails.cur_new_building.size[0] + field_x; b_x++)
		{
			for (var b_y = field_y;
				b_y < SectorDetails.cur_new_building.size[0] + field_y; b_y++)
			{
				if (placed_building_matrix[b_x]
				           				&& placed_building_matrix[b_x][b_y])
				{
					return;
				}
			}
		}

		PureFW.AJAXClientServer.send_request(
			"buildings_action.php?construct=true&x="+field_x+"&y="
			+field_y+"&sector_id=<?= $sector_id ?>&world_id=<?=$world_id?>"
			+"&map_id=<?=$map_id?>&type="
			+SectorDetails.cur_new_building.type
			+"&color="+SectorDetails.cur_new_building.color,
			SectorDetails.fetch_build_response
		);
		SectorDetails.stop_building();
		SectorDetails.hide_visible_fields();
	}

	<? /**
	    * Fängt die Serverantwort der Bauanfrage auf und behandelt sie.
	    *
	    * @param Array/Error response		Die Antwort
	    */ ?>
	SectorDetails.fetch_build_response = function(response)
	{
		if(isArray(response)) // successful construction
		{
			response_text_arr = response;
			if (response_text_arr.length == 1)
			{
				SectorDetails.officer.set_text('<br/>'+response_text_arr[0]);
			}
			else
			{
				if (ui_wood_gainer)	// Holzmenge korrigieren
					ui_wood_gainer.set_current_amount(parseInt(response_text_arr[0]));
				if (ui_food_gainer)	// Essensmenge korrigieren
					ui_food_gainer.set_current_amount(parseInt(response_text_arr[1]));
				Overview.set_unit_produced_and_limit(parseInt(response_text_arr[2]),
					parseInt(response_text_arr[3]));
				<? /**
					* Wurde ein Custom-Building gebaut wurde, dann ist es jetzt
					* nicht mehr in der Schatzkiste, also verringern wir den
					* entsprechenden Zähler um eins
					*/ ?>
				if (SectorDetails.cur_new_building.custom)
					ui_items_gainer.add_current_amount(-1);

				// Interface updaten
				MapDataCtrl.buildings[<?= $sector_id ?>]
					= eval('('+response_text_arr[4]+')');
				var n = MapDataCtrl.buildings[<?= $sector_id ?>].length;
				SectorDetails.sector_zoom.clear_buildings();
				SectorDetails.add_all_buildings();

				MapDataCtrl.refresh_mammun_map();
				SectorDetails.sector_zoom.refresh();

				/**
				 * Punkte anpassen
				 */
				if (PauseMenu.statistics.ui_my_score_gainer) { // Map-Score
					PauseMenu.statistics.ui_my_score_gainer.set_current_amount(
						parseInt(response_text_arr[5])
					);
					PauseMenu.rt_refresh();
				}
				Overview.xp_bar.set_xp_score(parseInt(response_text_arr[6]));
				if (typeof(response_text_arr[7]) !== 'undefined') {
					ui_klunker_gainer.set_current_amount(
						parseInt(response_text_arr[7])
					);
				}

				ui_refresh();

				var n = SectorDetails.construction_started_functions.length;
				for (var i = 0; i < n; i++) {
					SectorDetails.construction_started_functions[i].call(this,
						PureFW.Widget.create_event("construction_started"));
				}
			}
		}
		else
		{
			SectorDetails.officer.set_text(response);
		}
		PureFW.Timeout.set_timeout(EventMessage.get_new_events_now, 1000);
	}


	SectorDetails.start_building = function(building, force)
	{
		if (typeof(force) == 'undefined')
			force = false;
		if (building.type == 10) {
			for (var i in MapDataCtrl.buildings[<?= $sector_id ?>]) {
				if (isNaN(i))
					continue;

			}
		}

		if(!SectorDetails.own_sector)
		{
			SectorDetails.officer.set_text('<?=LNG_ERR_NO_DOMINATION?>');
			return;
		}
		SectorDetails.sector_zoom.show_construction_grid();
		SectorDetails.sector_zoom.add_event_handler("placement_grid_mouseover",
			SectorDetails.field_over_to_build
		);
		SectorDetails.sector_zoom.add_event_handler("placement_grid_click",
			SectorDetails.field_click_to_build
		);
		SectorDetails.new_building.hide();
		SectorDetails.new_building.set_pic_url(building.pic);
		SectorDetails.new_building.set_width(
			SectorDetails.sector_zoom.get_field_width()
			*building.size[0]
			/PureFW.WidgetManager.manager_all.get_scale_factor()
		);
		SectorDetails.new_building.set_height(
			SectorDetails.sector_zoom.get_field_height()
			*building.size[0]*building.size[1]
			/PureFW.WidgetManager.manager_all.get_scale_factor()
		);
		SectorDetails.cur_new_building = building;
	}

	SectorDetails.stop_building = function()
	{
		SectorDetails.sector_zoom.hide_construction_grid();
		SectorDetails.sector_zoom.remove_event_handler("placement_grid_mouseover",
			SectorDetails.field_over_to_build
		);
		SectorDetails.sector_zoom.remove_event_handler("placement_grid_click",
			SectorDetails.field_click_to_build
		);
		SectorDetails.new_building.hide();
//		SectorDetails.new_building = null;
	}

	SectorDetails.destroy = function() {
		upper_left_corner = SectorDetails.last_hit_building.pos;
		PureFW.AJAXClientServer.send_request(
			"buildings_action.php?destroy=true&sector_id=<?= $sector_id ?>" +
			"&x="+upper_left_corner.x+"&y="+upper_left_corner.y,
			SectorDetails.fetch_destroy_response
		);
	<?/*	alert("buildings_action.php?destroy=true&sector_id=<?= $sector_id ?>" +
			"&id="+SectorDetails.last_hit_building.get_bid());
			alert('Das Zerstören von Gebäuden ist aktuell deaktiviert.');*/?>
	}

	SectorDetails.fetch_destroy_response = function(response) {
		if(isArray(response)) { // successful construction
			response_text_arr = response;
			if (response_text_arr.length == 1) {
				SectorDetails.officer.set_text('<br/>'+response_text_arr[0]);
			}
			else if (response_text_arr[0] == '!') {	// kritischer Fehler
				SectorDetails.officer.set_text(response_text_arr[1]);
			}
			else // Everything went well
			{
				if (ui_wood_gainer)	// Holzmenge korrigieren
					ui_wood_gainer.set_current_amount(parseInt(response_text_arr[0]));
				if (ui_food_gainer)	// Essensmenge korrigieren
					ui_food_gainer.set_current_amount(parseInt(response_text_arr[1]));
				<? /**
					* Wurde ein Custom-Building abgerissen, dann ist es jetzt
					* in der Schatzkiste, also erhöhen wir den entsprechenden
					* Zähler um eins
					*/ ?>
				if (SectorDetails.last_hit_building.custom)
					ui_items_gainer.add_current_amount(1);

				Overview.set_unit_produced_and_limit(parseInt(response_text_arr[2]),
					parseInt(response_text_arr[3]));
				// Interface updaten
				MapDataCtrl.buildings[<?= $sector_id ?>]
					= eval('('+response_text_arr[4]+')');
				var n = MapDataCtrl.buildings[<?= $sector_id ?>].length;
				SectorDetails.sector_zoom.clear_buildings();
				SectorDetails.add_all_buildings();

				if (ui_my_score_gainer)	// Punkte anpassen
					ui_my_score_gainer
							.set_current_amount(parseInt(response_text_arr[5]));
				MapDataCtrl.dominators[<?=$sector_id?>, response_text_arr[6]];
				MapDataCtrl.refresh_mammun_map();
				SectorDetails.sector_zoom.refresh();
				ui_refresh();
			}
		}
		else
		{
			SectorDetails.officer.set_text(response);
		}
	}
}

SectorDetails.sector_zoom = null;	// später

<?
/******************************************************************************
 *                  /\     .---.    -----    -     .---.   .  .               *
 *                 /__\    |          |      |     |   |   |`.|               *
 *                /    \   `---'      -      -     `---'   '  '               *
 *                                                                            *
 *                             A  C  T  I  O  N                            PK *
 ******************************************************************************/
?>

<?/** Zunächst die neusten Daten propagieren */ ?>
MapDataCtrl.movements[<?=$sector_id?>] = <?= json_encode($map_data->moving_units) ?>;
MapDataCtrl.units[<?=$sector_id?>] = <?= json_encode($map_data->units) ?>;
MapDataCtrl.fights[<?=$sector_id?>] = <?= json_encode($map_data->fight) ?>;
MapDataCtrl.dominators[<?=$sector_id?>] = <?= json_encode($map_data->dominator) ?>;
MapDataCtrl.buildings[<?=$sector_id?>] = <?= json_encode(Buildings::convert_server_buildings($map_data->building)) ?>;
MapDataCtrl.refresh_mammun_map();

<? /* Hintergrundbilder anhand der Template-ID laden */?>
SectorDetails.background_img = new PureFW.Image(
	SectorDetails.this_window,
	0, -40,
	1024, 768,
	'map/templates/'
	+ MammunUI.get_top_frame().MapDataCtrl.template_id +
		'/_0_/sector<?= ($sector_id<10?0:'').$sector_id?>_bg_'
	+ MammunUI.get_top_frame().MapDataCtrl.template_id + '.jpg'
);

if(SectorDetails.user_interaction_enabled)
{
	<?/* Alle Einheiten als Männchen darstellen */ ?>
	SectorDetails.create_units = function(r) {
		SectorDetails.sector_zoom.clear_units();
		SectorDetails.my_unit_amount = 0;
		var _top = MammunUI.get_top_frame();
		var user,unit,temp,n,cur_unit_data,x,y,m,dir,pose,heading;
		for (user in _top.MapDataCtrl.units[<?=$sector_id?>]){
			if (user == '-')
				continue;
			for (heading in _top.MapDataCtrl.units[<?=$sector_id?>][user]) {
				if (isNaN(heading))
					continue;

				m = _top.MapDataCtrl.units[<?=$sector_id?>][user][heading].length;
				for (unit = 0; unit < m; unit++) {
					cur_unit_data =
						_top.MapDataCtrl.units[<?=$sector_id?>][user][heading][unit];

					var unit_data = new Object();
					for (var xx in cur_unit_data) {
						unit_data[xx] = cur_unit_data[xx];
					}
					unit_data.user_nick = user;

					var edge = (heading < 2) ? (heading ^ 1) : heading;

					SectorDetails.sector_zoom.add_unit(unit_data, edge);

					if (user == '<?= User::get_current_user()->get_nick()?>') {
						SectorDetails.my_unit_amount++;
					}
				}
			}
		}
	}
}


SectorDetails.add_all_buildings = function() {
	var n = MapDataCtrl.buildings[<?= $sector_id ?>].length;
	for (var i = 0; i < n; i++) {
		SectorDetails.sector_zoom.add_building(
			MapDataCtrl.buildings[<?= $sector_id ?>][i]
		);
		if (MapDataCtrl.buildings[<?= $sector_id ?>][i].const_key
				== "C_WATERFALL")
		{
			var widget = SectorDetails.sector_zoom.get_building(
				MapDataCtrl.buildings[<?= $sector_id ?>][i].ulc[0],
				MapDataCtrl.buildings[<?= $sector_id ?>][i].ulc[1]
			);
			var ani = widget.set_ani_as_img(
				'map/buildings/waterfall_ani.png', 6, 100
			);
		}
	}

	SectorDetails.my_building_amount =
		MapDataCtrl.get_non_neutral_buildings(<?= $sector_id ?> )
			.length;
}

SectorDetails.print_sector = function() {
	SectorDetails.sector_zoom = new PureFW.GameMap.MammunSectorZoom(
		SectorDetails.this_window,
		<?= $x_offset?>, <?= $y_offset-$sec_hover?>-40,
		552, 424,
		8, 8
	);
	SectorDetails.sector_zoom.set_bg_img('map/sector_big_border.png');

	SectorDetails.add_all_buildings();

	SectorDetails.sector_zoom.set_dominator(
		MapDataCtrl.dominators[<?= $sector_id?>]
	);

	SectorDetails.sector_zoom.add_event_handler("unit_click",
		SectorDetails.unit_clicked
	);
	SectorDetails.sector_zoom.add_event_handler("building_click",
		SectorDetails.building_clicked
	);
	SectorDetails.sector_zoom.add_event_handler("building_mouseover",
		SectorDetails.building_mouseover
	);
	SectorDetails.create_units();
	SectorDetails.sector_zoom.set_fight(MapDataCtrl.fights[<?=$sector_id?>]);
	SectorDetails.sector_zoom.refresh();
}
SectorDetails.print_sector();


<?/*body_size<?=$this_id?> = getBodySize();
getElemStyle('sector<?=$this_win_id?>').left =
	((body_size<?=$this_id?>[0]>>1)-<?=$sec_w_2?>)+'px'
getElemStyle('sector_sh<?=$this_win_id?>').left =
	((body_size<?=$this_id?>[0]>>1)-<?=$sec_w_2?>)+'px'
*/?>

<? /**
	* Anzeige, wem der Sektor gehört inklusive Uplinks zu Schatzkiste und
	* Profil. Nur anzeigen, wenn es auch einen Dominator gibt.
	**/ ?>
<? if ($dominator_user) : ?>
	<? if ($dominator_user->get_nick() !== '-') : /* Host-User hat kein Bild usw..*/ ?>
		SectorDetails.owner_details = new PureFW.Container(
			document.body,
			MammunUI.reference_width-141, 200,
			141, 85
		);
		SectorDetails.owner_details.set_z_index(89);
		SectorDetails.owner_details.set_bg_img(
			'ui/backgrounds/sector_details/owner_bg.png'
		);

		SectorDetails.owner_details_name = new PureFW.Container(
			SectorDetails.owner_details,
			0, 5,
			SectorDetails.owner_details.width, 20
		);
		SectorDetails.owner_details_name.set_content(
			'<a style="color: #fff;" '
				+'href="javascript: UIButtons.toggle_window(\''
					+'profile.php?nick='+
						'<?= $dominator_user->get_nick() ?>\', \'\', 110);">'
				+ '<?= $dominator_user->get_nick_main() ?>'
				+ '</a>'
		);
		SectorDetails.owner_details_name.set_font_size(1.2);
		SectorDetails.owner_details_name.set_text_align('center');


		try {
			SectorDetails.tmp
				= PauseMenu.avatars['<?= $dominator_user->get_nick()?>']
				    				.replace('SIZE',44);
		}
		catch(e) {
			try {
				SectorDetails.tmp =
					MapVisitNavi.map_browser.map_data[MapDataCtrl.world_id]
						[MapDataCtrl.map_id].avatars
						['<?= $dominator_user->get_nick()?>'].replace('SIZE',44);
			}
			catch(e) {
				SectorDetails.tmp = '';
			}
		}
		SectorDetails.owner_details_avatar = new PureFW.Avatar.Avatar(
			SectorDetails.owner_details,
			10, 25,
			48, 48,
			SectorDetails.tmp,
			'<?= $dominator_user->get_nick()?>',
			MapDataCtrl.player_colors['<?= $dominator_user->get_nick()?>']
		);

		SectorDetails.owner_details_mail_img = new PureFW.Image(
			SectorDetails.owner_details,
			SectorDetails.owner_details_avatar.position.x
				+ SectorDetails.owner_details_avatar.width + 10,
			SectorDetails.owner_details_avatar.position.y
				+ 5,
			36, 37,
			'ui/icons/labels/communication/36x37/pm.png'
		);
		SectorDetails.owner_details_mail_img.set_tooltip(
			'<?= str_replace('[NICK]', $dominator_user->get_nick() ,
			   				LNG_TITLE_PM_BUTTON) ?>'
		);
		SectorDetails.owner_details_mail_img.add_event_handler("click",
			(function(_i) {
				return function(ev)
				{
					UIButtons.toggle_new_message(
						"recipients=<?= $dominator_user->get_nick()?>"
					);
				}
			})(i));

		SectorDetails.owner_details_chest_img = new PureFW.Image(
			SectorDetails.owner_details,
			SectorDetails.owner_details_mail_img.position.x
				+ SectorDetails.owner_details_mail_img.width,
			SectorDetails.owner_details_mail_img.position.y,
			36, 37,
			'ui/icons/labels/communication/36x37/chest.png'
		);
		SectorDetails.owner_details_chest_img.set_tooltip(
			'<?= str_replace('[NICK]', $dominator_user->get_nick() ,
				LNG_TITLE_CHEST_BUTTON) ?>'
		);
		SectorDetails.owner_details_chest_img.add_event_handler("click",
			(function(_i) {
				return function(ev)
				{
					UIButtons.toggle_treasure_chest(
						"nick=<?= $dominator_user->get_nick()?>"
					);
				}
			})(i));
	<? else : ?>
		SectorDetails.sector_zoom.set_fog(
			'ui/backgrounds/sector_details/nebel.png'
		);
	<? endif; ?>
	<? if ((date('m', time()) == 10) && (date('d', time()) == 31)) : ?>
		SectorDetails.sector_zoom.set_fog(
			'ui/backgrounds/sector_details/nebel.png'
		);
	<? endif; ?>
<? endif; ?>

<?/* Ein noch unsichtbares Gebäudebild, das aktiv wird, wenn man ein Gebäude
	setzen will (als Preview) */?>
SectorDetails.new_building = new PureFW.Image(
	SectorDetails.sector_zoom,
	0,0,
	100,100,
	'pattern/spacer.gif'
);
SectorDetails.new_building.set_z_index(8);
SectorDetails.new_building.hide();

<? /**
	* Hirsch zeigen
	*/ ?>
<? if (!$dominator_user || ($dominator_user->get_nick() !== '-')) : ?>
<? if ((date('m', time()) == 12) && (date('d', time()) < 27)) : ?>
(function () {
	var i = <?= $sector_id?> - 10;
	if ((i >= 0) && (MapDataCtrl.resources[i]) && ((i | 1) == i)) {
		var rect = new Object();
		rect.x = 630;
		rect.y = 130;
		rect.w = 280;
		rect.h = 70;

		var megaloceros = SectorDetails.this_window.add_widget(
			MapObjects.Animal,
			rect.x + Math.floor(Math.random()*rect.w),
			rect.y + Math.floor(Math.random()*rect.h),
			MapObjects.Animal.animals.megaloceros.width,
			MapObjects.Animal.animals.megaloceros.height,
			MapObjects.Animal.animals.megaloceros,
			[0,0,0,0,402,0]
		);
	<?/*	var cont = SectorDetails.this_window.add_widget(
			PureFW.Container,
			rect.x, rect.y,
			rect.w, rect.h
		);
		cont.set_bg_color('red');

		/*cont.add_event_handler("click",
			(function(_megaloceros) {
				return function(ev) {
					_megaloceros.walk_to(ev.pageX, ev.pageY);
				}
			})(megaloceros)
		);*/?>
		megaloceros.set_walk_region(
			rect.x, rect.y,
			rect.w, rect.h
		);
		SectorDetails.megaloceros = megaloceros;
	}
})()
<? endif; ?>
<? endif; ?>


<? /** Die Pfeile zum Scrollen ausblenden */ ?>
try { ui_hide_map_scroll_arrows() } catch(e) {}

<?/* Jetzt den zuständigen Offizier zeigen mit Sprechblase */?>
SectorDetails.officer = new PureFW.Officer(document.body);
SectorDetails.officer.set_position(50,20);
SectorDetails.officer.set_officer_position(0, 50);
SectorDetails.officer.set_balloon_position(0, -0);
<? if (isset($tutorial_texts)) { ?>
	SectorDetails.set_tutorial_text(0);
<? } else { ?>
	SectorDetails.officer.set_text('<?= $officer_text ?>');
<? } ?>

if(SectorDetails.here_to_build)
{
	SectorDetails.start_building(SectorDetails.pre_chosen_building);
}
SectorDetails.loaded = true;
</script>