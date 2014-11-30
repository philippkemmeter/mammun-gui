/**
 * Diese Klasse ist für die Versorgung der MammunMap mit Daten zuständig.
 * Außerdem speichert es alle Core-Spiel-Daten, also alles, was die Game-Engine
 * so berechnet oder zur Berechnung braucht.
 * Das meiste bezieht sich auf die aktuelle Karte. Es werden keine Daten der
 * verschiedenen Karten, die ein Spieler besucht oder verwaltet, gecacht.
 * 
 * Außerdem werden alle Kartenaktionen hier bestimmt und behandelt.
 * 
 * TODO: PlayersOnMap zentral ablegen
 */

MapDataCtrl = new Object();	// es gibt nur eine zentrale Kontrolle

MapDataCtrl.world_id = 0;
MapDataCtrl.map_id = 0;
MapDataCtrl.my_world_id = 0;
MapDataCtrl.my_map_id = 0;
MapDataCtrl.template_id = 0;
MapDataCtrl.my_homesector = 0;
MapDataCtrl.my_homesector_field_pos = null;
MapDataCtrl.my_nick = '';
MapDataCtrl.my_color = '';
MapDataCtrl.my_is_mentor_map = false;	/** Bezieht sich auf meine Karte */
MapDataCtrl.my_is_public_map = false;	/** Bezieht sich auf meine Karte */
MapDataCtrl.my_map_is_closed = false;	/** Bezieht sich auf meine Karte */
MapDataCtrl.is_mentor_map = false;	/** Bezieht sich auf die aktuelle Karte */
MapDataCtrl.is_public_map = false;	/** Bezieht sich auf die aktuelle Karte */
MapDataCtrl.map_is_closed = false;	/** Bezieht sich auf die aktuelle Karte */
MapDataCtrl.auth_key = '';

MapDataCtrl.resources = null;
MapDataCtrl.dominators = null;
MapDataCtrl.buildings = null;
MapDataCtrl.player_colors = null;
MapDataCtrl.movements = null;
MapDataCtrl.units = null;
MapDataCtrl.fights = null;
MapDataCtrl.max_players = 0;			/**Bezieht sich auf die aktuelle Karte*/
MapDataCtrl.players_on_map_amount = 0;	/**Bezieht sich auf die aktuelle Karte*/
MapDataCtrl.my_max_players = 0;				/** Bezieht sich auf meine Karte */
MapDataCtrl.my_players_on_map_amount = 0;	/** Bezieht sich auf meine Karte */


MapDataCtrl.mammun_map = null;
MapDataCtrl.update_lock = false;

MapDataCtrl.Movement = new Object();	// Definitionen s.u.
MapDataCtrl.Movement.from_sector = -1;
MapDataCtrl.Movement.to_sector = -1;
MapDataCtrl.Movement.pathfinder = null;
MapDataCtrl.Movement.my_graph = new WGraph();
MapDataCtrl.Movement.chosen_units_to_move = new Array();
MapDataCtrl.Movement.move_successful_functions = new Array();
MapDataCtrl.Movement.move_unsuccessful_functions = new Array();

MapDataCtrl.LNG = new Object();
MapDataCtrl.LNG.WARNING_MOVE_TO_DARK_SECTOR = '';


/**
 * Setzt die Mammun-Karte
 */
MapDataCtrl.set_mammun_map = function(widget) {
	MapDataCtrl.mammun_map = widget;
	MapDataCtrl.mammun_map.add_layer("hover_layer", 1);
	
	MapDataCtrl.mammun_map.add_event_handler("field_click",
		MapDataCtrl.sector_click
	);

	MapDataCtrl.mammun_map.add_event_handler("field_mouseover",
		MapDataCtrl.sector_over
	);
	
	MapDataCtrl.mammun_map.add_event_handler("field_mouseout",
		MapDataCtrl.sector_out
	);
	
	MapDataCtrl.mammun_map.add_event_handler("move_arrow_click",
		MapDataCtrl.move_arrow_click
	);
	
	MapDataCtrl.mammun_map.my_nick = MapDataCtrl.my_nick;
}

/*****************************************************************************
 * 
 * 									MAP UPDATE
 * 
 *****************************************************************************/

/**
 * Holt neue Daten vom Server und leitet sie zur Darstellung weiter
 */
MapDataCtrl.update_map = function(callback) {
	if (MapDataCtrl.update_lock)
		return;
	var request_str = "ingame_update.php?"
		+ "map_data_update=true&my_event_log=true&my_score_level=true"
		+ "&world_id=" + MapDataCtrl.world_id + "&map_id=" + MapDataCtrl.map_id;
	if (typeof(callback) == 'function') {
		PureFW.AJAXClientServer.send_request(request_str,
			(function(_callback) {
				return function(response_arr) {
					MapDataCtrl.__update_map_response(response_arr);
					_callback(response_arr)
				}
			})(callback));
	}
	else {
		PureFW.AJAXClientServer.send_request(request_str, 
			MapDataCtrl.__update_map_response);	
	}
};
MapDataCtrl.__update_map_response = function(response_arr) {
	if (response_arr[0] != '1') {
		throw new Error(
			"Got wronge response in MapDataCtrl.__update_map_response: "
					+ response_arr);
	}
	
	if (MapDataCtrl.dominators)
		destroy_object(MapDataCtrl.dominators);
	MapDataCtrl.dominators = null;
	
	if (MapDataCtrl.buildings)
		destroy_object(MapDataCtrl.buildings);
	MapDataCtrl.buildings = null;
	
	if (MapDataCtrl.movements)
		destroy_object(MapDataCtrl.movements);
	MapDataCtrl.movements = null;
	
	if (MapDataCtrl.units)
		destroy_object(MapDataCtrl.units);
	MapDataCtrl.units = null;
	
	if (MapDataCtrl.fights)
		destroy_object(MapDataCtrl.fights);
	MapDataCtrl.fights = null;
	
	var map_data = eval('('+response_arr[1]+')');
	
	MapDataCtrl.dominators = map_data.dominator;
	MapDataCtrl.buildings = map_data.building;
	MapDataCtrl.movements = map_data.moving_units;
	MapDataCtrl.units = map_data.units;
	MapDataCtrl.fights = map_data.fight;
	if (map_data.user_colors) {
		if (MapDataCtrl.player_colors)
			destroy_object(MapDataCtrl.player_colors);
		MapDataCtrl.player_colors = map_data.user_colors;
	}
	/*if (map_data.bio) {
		if (MapDataCtrl.resources)
			destroy_object(MapDataCtrl.resources);
		MapDataCtrl.resources = map_data.bio;
	}*/

	if ((typeof(response_arr[2]) != 'undefined') && (response_arr[2])) {
		var event_log = eval('('+response_arr[2]+')');
		MammunUI.get_top_frame().EventMessage.handle_event_messages(event_log);
	}
	
	if ((typeof(response_arr[3]) != 'undefined') && (response_arr[3])){
		var score_obj = eval('('+response_arr[3]+')');
		try {
			PauseMenu.statistics.ui_my_score_gainer.set_current_amount(
				parseInt(score_obj.map_score)
			);
		}
		catch(e){}
		try {
			Overview.xp_bar.set_xp_score(score_obj.league_score);
			Overview.xp_bar.set_xp_level(score_obj.level);
			Overview.xp_bar.set_xp_score_next_level(score_obj.score_next_level);
			Overview.xp_bar.set_xp_score_last_level(score_obj.score_last_level);
		}
		catch(e){}
	}
	
	MapDataCtrl.refresh_mammun_map(false);
};

/**
 * Wechselt die Karte (Besuchen)
 * => Mehr Daten müssen geholt und verarbeitet werden, als beim einfachen update
 */
MapDataCtrl.change_map = function(world_id, map_id, callback) {
	MapDataCtrl.update_lock = true;
	MapDataCtrl.world_id = world_id;
	MapDataCtrl.map_id = map_id;
	
	PureFW.AJAXClientServer.send_request("map.php?change_map=1"
		+"&world_id=" + MapDataCtrl.world_id + "&map_id=" + MapDataCtrl.map_id, 
		(function(_c) {
			return function(response_arr) {
				MapDataCtrl.__change_map_response(response_arr);
				if (_c)
					_c();
			}
		})(callback  || null)
	);
}
MapDataCtrl.__change_map_response = function(response_arr) {
	var o = eval('('+response_arr[0]+')');
	MapDataCtrl.template_id = o.template_id;
	MapDataCtrl.max_players = parseInt(o.max_players);
	MapDataCtrl.world_name = o.world_name;
	
	if (MapDataCtrl.dominators)
		destroy_object(MapDataCtrl.dominators);
	MapDataCtrl.dominators = null;
	
	if (MapDataCtrl.buildings)
		destroy_object(MapDataCtrl.buildings);
	MapDataCtrl.buildings = null;
	
	if (MapDataCtrl.movements)
		destroy_object(MapDataCtrl.movements);
	MapDataCtrl.movements = null;
	
	if (MapDataCtrl.units)
		destroy_object(MapDataCtrl.units);
	MapDataCtrl.units = null;
	
	if (MapDataCtrl.fights)
		destroy_object(MapDataCtrl.fights);
	MapDataCtrl.fights = null;
	
	if (MapDataCtrl.resources)
		destroy_object(MapDataCtrl.resources);
	MapDataCtrl.resources = null;
	
	if (MapDataCtrl.player_colors)
		destroy_object(MapDataCtrl.player_colors);
	MapDataCtrl.player_colors = null;
	
	destroy_object(MapDataCtrl.dominators);
	MapDataCtrl.dominators = null;
	destroy_object(MapDataCtrl.buildings);
	MapDataCtrl.buildings = null;
	destroy_object(MapDataCtrl.movements);
	MapDataCtrl.movements = null;
	destroy_object(MapDataCtrl.units);
	MapDataCtrl.units = null;
	destroy_object(MapDataCtrl.fights);
	MapDataCtrl.fights = null;
	destroy_object(MapDataCtrl.resources);
	MapDataCtrl.resources = null;
	destroy_object(MapDataCtrl.player_colors);
	MapDataCtrl.player_colors = null;
	
	MapDataCtrl.dominators = eval('('+response_arr[1]+')');
	MapDataCtrl.buildings = eval('('+response_arr[2]+')');
	MapDataCtrl.movements = eval('('+response_arr[3]+')');
	MapDataCtrl.units = eval('('+response_arr[4]+')');
	MapDataCtrl.fights = eval('('+response_arr[5]+')');
	MapDataCtrl.resources = eval('('+response_arr[6]+')');
	MapDataCtrl.player_colors = eval('('+response_arr[7]+')');
	MapDataCtrl.is_mentor_map = !!parseInt(response_arr[8]);
	MapDataCtrl.is_closed = !!parseInt(response_arr[9]);
	
	MapDataCtrl.refresh_mammun_map(true);
	MapDataCtrl.update_lock = false;
}
/**
 * Diese Funktion ist eine Hilfsfunktion von __insert_edge_by_domination.
 * 
 * @see MapDataCtrl#__insert_edge_by_domination
 */
MapDataCtrl.__insert_edge_by_array = function(x, y, i, data, compare_with) {
	if ((x > 0) && ((!compare_with && data[i-1])||(data[i-1] == compare_with))){
		if (!MapDataCtrl.Movement.my_graph.get_vertex(i-1))		// Terrain ? :)
			MapDataCtrl.Movement.my_graph.add_vertex(new Knoten(i-1, 1));	
		MapDataCtrl.Movement.my_graph.add_edge(i, i-1, false);
	}
	else
		MapDataCtrl.Movement.my_graph.remove_edge(i, i-1, false);
	
	if ((x < 9) && ((!compare_with && data[i+1])||(data[i+1] == compare_with))){
		if (!MapDataCtrl.Movement.my_graph.get_vertex(i+1))		// Terrain ? :)
			MapDataCtrl.Movement.my_graph.add_vertex(new Knoten(i+1, 1));
		MapDataCtrl.Movement.my_graph.add_edge(i, i+1, false);
	}
	else
		MapDataCtrl.Movement.my_graph.remove_edge(i, i+1, false);
	
	if ((y > 0) && ((!compare_with && data[i-10])||(data[i-10] == compare_with))){
		if (!MapDataCtrl.Movement.my_graph.get_vertex(i-10))	// Terrain ? :)
			MapDataCtrl.Movement.my_graph.add_vertex(new Knoten(i-10, 1));
		MapDataCtrl.Movement.my_graph.add_edge(i, i-10, false);
	}
	else
		MapDataCtrl.Movement.my_graph.remove_edge(i, i-10, false);
	
	if ((y < 9) && ((!compare_with && data[i+10])||(data[i+10] == compare_with))){
		if (!MapDataCtrl.Movement.my_graph.get_vertex(i+10))	// Terrain ? :)
			MapDataCtrl.Movement.my_graph.add_vertex(new Knoten(i+10, 1));	
		MapDataCtrl.Movement.my_graph.add_edge(i, i+10, false);
	}
	else
		MapDataCtrl.Movement.my_graph.remove_edge(i, i+10, false);
}

/**
 * Diese Funktion fügt Kanten im Graph ein, wenn der Sektor von MIR
 * dominiert wird. Welche Kanten das sind, hängt von verschiedenen Umständen
 * ab: Kampf && Nachbardominanz oder Nachbarresource 
 */
MapDataCtrl.__insert_edge_for_domination = function(x, y, i) {
	if (!MapDataCtrl.fights || !MapDataCtrl.dominators
		|| !MapDataCtrl.resources)
	{
		throw new Error("figts, resources and dominators have to be "+
			"initialized before calling _insert_edge_for_domination!");
	}

	if (MapDataCtrl.dominators[i] == MapDataCtrl.my_nick) {
		if (!MapDataCtrl.Movement.my_graph.get_vertex(i))	// Terrain ? :)
			MapDataCtrl.Movement.my_graph.add_vertex(new Knoten(i, 1));
		
		if (MapDataCtrl.fights[i]) {
			MapDataCtrl.__insert_edge_by_array(x, y, i, MapDataCtrl.dominators,
				MapDataCtrl.my_nick);
		}
		else {
			MapDataCtrl.__insert_edge_by_array(x, y, i, MapDataCtrl.resources);
		}
	}
	else {
		MapDataCtrl.Movement.my_graph.remove_edge(i, i-1, false);
		MapDataCtrl.Movement.my_graph.remove_edge(i, i+1, false);
		MapDataCtrl.Movement.my_graph.remove_edge(i, i-10, false);
		MapDataCtrl.Movement.my_graph.remove_edge(i, i+10, false);
	}
};

/**
 * Diese Funktion leitet die Daten der Karte an die MammunMap-Klasse weiter
 */
MapDataCtrl.refresh_mammun_map = function(map_change) {
	map_change = map_change || false;
	var mp = MapDataCtrl.mammun_map;
	if (!mp)
		throw new Error("Cannot refresh Mammun Map, mammun_map is NULL!");
	
	for (var i = 0, y = 0; y < 10; y++) {
		for (var x = 0; x < 10; x++, i++) {
			if (MapDataCtrl.dominators && MapDataCtrl.dominators[i]) {
				if (!map_change)
					MapDataCtrl.__insert_edge_for_domination(x, y, i);
				mp.set_dominator(x, y, MapDataCtrl.dominators[i]);
			}
			else {
				mp.set_dominator(x, y, null);
			}
			if (MapDataCtrl.buildings && MapDataCtrl.buildings[i]) {
				mp.set_buildings(x, y, MapDataCtrl.buildings[i]);
			}
			else {
				mp.set_buildings(x, y, null);
			}
			if (MapDataCtrl.movements && MapDataCtrl.movements[i]) {
				mp.set_movements(x, y, MapDataCtrl.movements[i]);
			}
			else {
				mp.set_movements(x, y, null);
			}
			if (MapDataCtrl.units && MapDataCtrl.units[i]) {
				mp.set_units(x, y, MapDataCtrl.units[i]);
			}
			else {
				mp.set_units(x, y, null);
			}
			if (MapDataCtrl.fights && MapDataCtrl.fights[i]) {
				mp.set_fight(x, y, MapDataCtrl.fights[i]);
			}
			else {
				mp.set_fight(x, y, null);
			}
			if (MapDataCtrl.resources && MapDataCtrl.resources[i]) {
				mp.create_field(x, y);
				mp.set_resources(x, y, MapDataCtrl.resources[i]);
				MapDataCtrl.mammun_map.get_layer("hover_layer")
					.create_field(x, y, PureFW.Image, 'pattern/spacer.gif', true);
			}
			else {
				mp.destroy_field(x, y);
			}
		}
	}
	if (MapDataCtrl.player_colors) {
		for (var i in MapDataCtrl.player_colors) {
			mp.set_player_color(i, MapDataCtrl.player_colors[i]);
		}
	}

	mp.refresh();
}

/*****************************************************************************
 * 
 * 							MAP EVENT HANDLERS
 * 
 *****************************************************************************/

/**
 * Behandelt einen Klick auf die Karte
 */
MapDataCtrl.sector_click = function(ev) {
	var x = ev.detail.x; var y = ev.detail.y; 
	var secnr = y*10+x;
	MapDataCtrl.sector_click_(secnr);
}
MapDataCtrl.sector_click_ = function(secnr) {
	var _top = MammunUI.get_top_frame();
	if (MapDrag.dragging) {
		MapDrag.dragging = false;
		return;
	}

	if (MapDataCtrl.Movement.from_sector == -1) {
		_top.open_dialog(
			'sector_details.php?sector_id=' + secnr +
				'&world_id=' + MapDataCtrl.world_id +
				'&map_id=' + MapDataCtrl.map_id, 
			'', 1024, 768, true
		);
	}
	else {
		if (MapDataCtrl.dominators[secnr] == '-') {
			var tmp = MammunUI.show_warning(
				350, 200, MapDataCtrl.LNG.WARNING_MOVE_TO_DARK_SECTOR,
				PureFW.ConfirmationBox.YES | PureFW.ConfirmationBox.NO
			);
			tmp.add_event_handler("confirm",
				(function (_sector) {
					return function(ev) {
						MapDataCtrl.Movement.move_to_here(_sector);
					}
				})(secnr)
			);
			tmp.add_event_handler("cancel",
				function (ev) {
					MapDataCtrl.Movement.move_end();
				}
			);
		}
		else {
			MapDataCtrl.Movement.move_to_here(secnr);
		}
	}
};

/**
 * Behandelt das Bewegen der Maus über einen Sektor
 */
MapDataCtrl.sector_over = function(ev) {
	var x = ev.detail.x; var y = ev.detail.y; 
	var secnr = y*10+x;

	if (MapDataCtrl.Movement.from_sector > -1) {
		var path = MapDataCtrl.Movement.pathfinder.get_path(secnr);
		var date = new Date();
		var timeInSec = date.getHours() * 3600 
			+ date.getMinutes() * 60 + date.getSeconds();
		MapDataCtrl.mammun_map.show_path(path, 
			MapDataCtrl.Movement.chosen_units_to_move[0].time_per_sector, 
			0, timeInSec
		);
	}
	else {
		var layer = MapDataCtrl.mammun_map.get_layer("hover_layer");
		layer.get_field(x, y).set_pic_url('map/sector_hover.png');
		/**
		 * Kein refresh nötig, da direkt das Feld geändert.
		 */
	}
};

/**
 * Behandelt das Bewegen der Maus über einen Sektor
 */
MapDataCtrl.sector_out = function(ev) {
	var x = ev.detail.x; var y = ev.detail.y; 
	var secnr = y*10+x;

	if (MapDataCtrl.Movement.from_sector == -1) {
		var layer = MapDataCtrl.mammun_map.get_layer("hover_layer");
		for (var i = 0; i < 10; i++) {
			for (var j = 0; j < 10; j++) {
				if (layer.get_field(i,j))
					layer.get_field(i,j).set_pic_url('pattern/spacer.gif');
			}
		}
	}
};

/**
 * Behandelt das Klicken auf einen Bewegungspfeil
 */
MapDataCtrl.move_arrow_click = function(ev) {
	var unit_data = ev.detail.unit_data;
	var sec_from = ev.detail.sec_from;
	var sec_to = ev.detail.sec_to;

	var mp = MapDataCtrl.mammun_map;
	
	var p = mp.get_field_pixel_position(sec_from.x, 
		sec_from.y);
	p.x += mp.position.x;
	p.y += mp.position.y;
	
	var sec_w = mp.get_field_width();
	var sec_h = mp.get_field_height();
	var sec_from_mid = new PureFW.Point(p.x + (sec_w>>1), p.y + (sec_h>>1));
	
	p = mp.get_field_pixel_position(sec_to.x, sec_to.y);
	p.x += mp.position.x;
	p.y += mp.position.y;
	var sec_to_mid = new PureFW.Point(p.x + (sec_w>>1), p.y + (sec_h>>1));
	
	// Der Pfeil wird in der Mitte der Kante, die die angrenzenden 
	// Sektoren trennt, angenommen
	var arrow_pos = new PureFW.Point(
		sec_from_mid.x + (sec_to_mid.x - sec_from_mid.x)/2,
		sec_from_mid.y + (sec_to_mid.y - sec_from_mid.y)/2
	);
	
	sec_from = sec_from.x + 10*sec_from.y;
	sec_to = sec_to.x + 10*sec_to.y;
	MapDataCtrl.movement_array_info_balloon.set_url(
		'movement.php?moving_in_sec='+sec_from+'&moving_to_sec='+sec_to
	);
	MapDataCtrl.movement_array_info_balloon.set_z_index(30);
	MapDataCtrl.movement_array_info_balloon.show();
	p = MapDataCtrl.movement_array_info_balloon.get_arrowhead_pos();
	MapDataCtrl.movement_array_info_balloon.set_position(
		arrow_pos.x-p.x, arrow_pos.y-p.y);
}

/*****************************************************************************
 * 
 * 							GETTER AND SETTER
 * 
 *****************************************************************************/

MapDataCtrl.get_neutral_buildings = function(sector_id) {
	var result = new Array();
	var n = MapDataCtrl.buildings[sector_id].length, cur_b;
	for (var i = 0; i < n; i++) {
		cur_b = MapDataCtrl.buildings[sector_id][i];
		if (cur_b.neutral)
			result.push(cur_b);
	}
	return result;
}

MapDataCtrl.get_non_neutral_buildings = function(sector_id) {
	var result = new Array();
	var n = MapDataCtrl.buildings[sector_id].length, cur_b;
	for (var i = 0; i < n; i++) {
		cur_b = MapDataCtrl.buildings[sector_id][i];
		if (!cur_b.neutral)
			result.push(cur_b);
	}
	return result;
}


/*****************************************************************************
 * 
 * 								MOVEMENT
 * 
 *****************************************************************************/

MapDataCtrl.Movement.units_move = function(sector, units) {
	if (!units)
		return false;
	/*if (MammunUI.get_top_frame().officer)
		MammunUI.get_top_frame().officer.hide();*/
	MapDataCtrl.Movement.chosen_units_to_move = units;
	MapDataCtrl.Movement.move_from_here(sector);
	return true;
};

MapDataCtrl.Movement.move_from_here = function(from) {
	MammunUI.get_top_frame().ui_refresh_active = false;

	if (MapDataCtrl.Movement.pathfinder) {
		try {
			MapDataCtrl.Movement.destroy();
		}
		catch(e){}
		MapDataCtrl.Movement.pathfinder = null;
	}
	MapDataCtrl.Movement.pathfinder = new Pathfinder(
		MapDataCtrl.Movement.my_graph
	);
	MapDataCtrl.Movement.pathfinder.dijkstra(from);
	MapDataCtrl.Movement.from_sector = from;
};

MapDataCtrl.Movement.move_to_here = function(to) {
	var p = MapDataCtrl.Movement.pathfinder.get_path_str(to);
	var chosen_units_to_move_str = '';
	
	for (var i = 0; i < MapDataCtrl.Movement.chosen_units_to_move.length; i++) 
	{
		chosen_units_to_move_str += '&id'+i+'='
			+MapDataCtrl.Movement.chosen_units_to_move[i].id;
		
		if (MapDataCtrl.Movement.chosen_units_to_move[i].count)
				chosen_units_to_move_str += '&count'+i+'='
					+MapDataCtrl.Movement.chosen_units_to_move[i].count;
	}
	//top.open_dialog('movement.php?path='+p+chosen_units_to_move_str, '', 400, 300);
	PureFW.AJAXClientServer.send_request('movement.php?path='+p+
			chosen_units_to_move_str, MapDataCtrl.Movement.move_result);
	
	// Direct Feedback:
	MapDataCtrl.Movement.move_end();
}

MapDataCtrl.Movement.move_result = function(response_arr) {
	var _top = MammunUI.get_top_frame();
	if (response_arr[0] == '1') {
		// Einheiten wurde abgezogen. Entsprechend die sich bewegenen Einheiten
		// aktualisieren, und den Dominator, um die Borders anzupassen - Rest
		// aktuallisiert sich selbst.
		var path = eval('('+response_arr[5]+')');
		var move_from_sector = path[0];
		
		MapDataCtrl.movements[move_from_sector] = eval('('+response_arr[1]+')');
		MapDataCtrl.fights[move_from_sector] = response_arr[3];
		MapDataCtrl.dominators[move_from_sector] = response_arr[2];
		MapDataCtrl.units[move_from_sector] = eval('('+response_arr[4]+')'); 

		MapDataCtrl.refresh_mammun_map();
		MapDataCtrl.Movement.on_move_successful(
			PureFW.EventUtil.create_event("move_successful", new PureFW.Point(
				move_from_sector%10, Math.floor(move_from_sector/10)))
		);
	}
	else if (response_arr[0] == '0') {
		var tmp = MammunUI.show_warning(
			350, 200, response_arr[1], PureFW.ConfirmationBox.YES
		);
		
		MapDataCtrl.Movement.on_move_unsuccessful(
			PureFW.EventUtil.create_event("move_unsuccessful", new PureFW.Point(
				move_from_sector%10, Math.floor(move_from_sector/10)))
		);
	}
};

MapDataCtrl.Movement.move_end = function() {
	MapDataCtrl.Movement.from_sector = -1;
	MammunUI.get_top_frame().ui_refresh_active = true;
	MapDataCtrl.mammun_map.show_path(new Array());
};

MapDataCtrl.Movement.cancel = function() {
	if (MapDataCtrl.Movement.from_sector)
		MapDataCtrl.Movement.move_end();
};

MapDataCtrl.Movement.add_event_handler = function(type, fn) {
	if (typeof (type) !== 'string')
		throw new Error("MapDataCtrl.Movement::add_event_handler: "+
		"First argument 'type' has to be of type 'string'");
	if (typeof (fn) !== 'function')
		throw new Error("MapDataCtrl.Movement::add_event_handler: "+
		"Second argument 'fn' has to be of type 'function'");
	if (type == "move_successful") {
		MapDataCtrl.Movement.move_successful_functions.push(fn);
	}
	else if (type == "move_unsuccessful") {
		MapDataCtrl.Movement.move_unsuccessful_functions.push(fn);
	}
};

MapDataCtrl.Movement.remove_event_handler = function(type, fn) {
	if (typeof (type) !== 'string')
		throw new Error("MapDataCtrl.Movement::remove_event_handler: "+
		"First argument 'type' has to be of type 'string'");
	if (typeof (fn) !== 'function')
		throw new Error("MapDataCtrl.Movement::remove_event_handler: "+
		"Second argument 'fn' has to be of type 'function'");
	if (type == "move_successful") {
		MapDataCtrl.Movement.move_successful_functions.remove(fn);
	}
	else if (type == "move_unsuccessful") {
		MapDataCtrl.Movement.move_unsuccessful_functions.remove(fn);
	}
}

MapDataCtrl.Movement.on_move_successful = function(ev) {
	if (!ev)
		ev = PureFW.EventUtil.create_event("move_successful");
	
	var n = MapDataCtrl.Movement.move_successful_functions.length;
	for (var i = 0; i < n; i++)
		MapDataCtrl.Movement.move_successful_functions[i].call(this, ev);
}

MapDataCtrl.Movement.on_move_unsuccessful = function(ev) {
	if (!ev)
		ev = PureFW.EventUtil.create_event("move_unsuccessful");
	
	var n = MapDataCtrl.Movement.move_unsuccessful_functions.length;
	for (var i = 0; i < n; i++)
		MapDataCtrl.Movement.move_unsuccessful_functions[i].call(this, ev);
}