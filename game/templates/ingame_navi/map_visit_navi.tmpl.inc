<? Language::get_instance()->load_language_file("map_visit_navi.tmpl") ?>
<script type="text/javascript">
MapVisitNavi = new Object();

<?	/**
	 * Widget, das das Icon unten rechts zum Starten des Mapbrowsers
	 * repräsentiert
	 **/ ?>

MapVisitNavi.map_browser_icon = new PureFW.MapUI.MapVisitNaviIcon(
	document.body,
	MammunUI.reference_width - 236, MammunUI.reference_height - 67,
	226, 67
);
MapVisitNavi.map_browser_icon.set_z_index(299);

MapVisitNavi.map_browser_icon_label = new PureFW.Image(
	document.body,
	MapVisitNavi.map_browser_icon.position.x + 2,
	MapVisitNavi.map_browser_icon.position.y - 48,
	192, 58,
	'ui/elements/texts/<?= $LNG ?>/map_navi/icon_label.png'
);
MapVisitNavi.map_browser_icon_label.set_z_index(298);


<? foreach($avatar_data as $nick => $pic) : ?>
MapVisitNavi.map_browser_icon.add_avatar(
	'<?= $pic ?>'.replace('SIZE', 44),
	<?= $all_map_ids[$nick][0] ?>, <?= $all_map_ids[$nick][1] ?>
);
<? endforeach; ?>

<?	/**
	 * Statisches "Widget" das den unteren Mapbrowser repräsentiert.
	 */ ?>
MapVisitNavi.map_browser = new Object();

<? 	/** Alle Daten aller Karten, die bereits geladen sind */ ?>
MapVisitNavi.map_browser.map_data = new Array();
<?	/** Die ID der Welt, die gerade zentriert angezeigt wird */ ?>
MapVisitNavi.map_browser.current_world_id = 0;
<?	/** Die ID der Karte, die gerade zentriert angezeigt wird */ ?>
MapVisitNavi.map_browser.current_map_id = 0;
<?	/** Die ID der Karte, die gerade links angezeigt wird (vorherige) */ ?>
MapVisitNavi.map_browser.prev_map_id = 0;
<?	/** Die ID der Karte, die gerade rechts angezeigt wird (nächste) */ ?>
MapVisitNavi.map_browser.next_map_id = 0;
<?	/** Liste der Map-IDs der Freunds */ ?>
MapVisitNavi.map_browser.friend_map_ids = null;	// verzögert
<?	/** Liste der Choose-Map-IDs */ ?>
MapVisitNavi.map_browser.choose_map_ids = null;	// verzögert
<?	/** Hier sind die URLs drin, die zum Besuchen der Karte angesurft werden
		müssen */ ?>
MapVisitNavi.map_browser.choose_map_hrefs = null; // verzögert
<?	/** Ob die Liste der Spieler auf der Karte angezeigt werden sollen */ ?>
MapVisitNavi.map_browser.show_players_on_map = true;
<?	/** Ob der Link zurück zur eigenen Karte angezeigt werden soll */ ?>
MapVisitNavi.map_browser.show_back_to_my_map_link = true;

<?	/**
	 * Ob die Freundschaftskartentopologie benutzt werden soll.
	 */ ?>
MapVisitNavi.map_browser.friend_mode = false;
<?	/**
	 * Ob die Kartenwahlkartentolpologie benutzt werden soll.
	 */ ?>
MapVisitNavi.map_browser.choose_mode = false;

<? /** Nicht zerstören. sind wenig Daten, kann man halten */ ?>
MapVisitNavi.map_browser.destroy = function() { };
<?	/**
	 * Kartendaten setzen / hinzufügen / ersetzen
	 *
	 * @param uint world_id
	 * @param uint map_id
	 * @param uint template_id
	 * @param string[] players
	 * @param string[] avatars 		URIs zu den Avataren
	 */ ?>
MapVisitNavi.map_browser.set_map_data = function(world_id, map_id, template_id,
	players, avatars, max_players, is_invited)
{
	if (typeof(MapVisitNavi.map_browser.map_data[world_id]) == 'undefined')
		MapVisitNavi.map_browser.map_data[world_id] = new Array();

	if (typeof(MapVisitNavi.map_browser.map_data[world_id][map_id]
		== 'undefined'))
	{
		MapVisitNavi.map_browser.map_data[world_id][map_id] = new Object();
	}
	MapVisitNavi.map_browser.map_data[world_id][map_id].players = players;
	MapVisitNavi.map_browser.map_data[world_id][map_id].template_id = template_id;
	MapVisitNavi.map_browser.map_data[world_id][map_id].avatars = avatars;
	MapVisitNavi.map_browser.map_data[world_id][map_id].max_players = max_players;
	MapVisitNavi.map_browser.map_data[world_id][map_id].is_invited = is_invited;

	<? /**
	    * Hier folgt nun ein sehr komprimierter Code. Es werden alle
	    * Spieler auf der Karte durchgegangen und für jeden geschaut, ob sie
	    * einen gültigen Nick haben. Wenn der Nick '-' ist, dann ist es der
	    * Host, wenn der Nick '' ist, dann ist es kein Spieler, sondern ein
	    * freier Slot.
	    * Die Karte ist genau dann leer, wenn bis zu letzt kein richtiger
	    * Spieler gefunden wurde, i also gleich n ist.
	    */ ?>
	var n = players.length; var i = 0;
	while ((i < n) && ((players[i].nick == '-')||(players[i].nick == ''))) i++;

	MapVisitNavi.map_browser.map_data[world_id][map_id].map_is_empty = (i == n);
}

<?	/**
	 * Zeigt die vorherige Karte an, also die links von der aktuellen.
	 *
	 * Welche Karte hier angezeigt wird, entscheidet die übergebene map_id.
	 *
	 * @param unit world_id
	 * @param uint map_id
	 * @access private
	 */ ?>
MapVisitNavi.map_browser.show_prev = function(world_id, map_id) {
	<? /* Daten nachladen, wenn noch nicht vorhanden */ ?>
	if ((typeof(MapVisitNavi.map_browser.map_data[world_id]) == 'undefined')
		|| (typeof(MapVisitNavi.map_browser.map_data[world_id][map_id])
				== 'undefined')
		|| !MapVisitNavi.map_browser.map_data[world_id][map_id])
	{
		PureFW.AJAXClientServer.send_request("map.php?get_preview_data=true&"
				+ "map_id="+map_id + '&world_id='+world_id,
			MapVisitNavi.map_browser.show_prev_response);
		return;
	}

	try {
		MapVisitNavi.navigation.prev_map_container_l.destroy();
		MapVisitNavi.navigation.prev_map_container_l = null;
		MapVisitNavi.navigation.prev_map_container.destroy();
		MapVisitNavi.navigation.prev_map_container = null;
		MapVisitNavi.navigation.prev_map_container_r.destroy();
		MapVisitNavi.navigation.prev_map_container_r = null;
		MapVisitNavi.navigation.prev_map_label.destroy();
		MapVisitNavi.navigation.prev_map_label = null;
	}
	catch (e) {}

	var max_players = MapVisitNavi.map_browser.map_data[world_id][map_id]
		.max_players;
	if (max_players == 0)
		return;

	if (!MapVisitNavi.map_browser.friend_mode
		&& !MapVisitNavi.map_browser.choose_mode)
	{
		<? /* Hier muss nun geschaut werden, dass keine leeren Karten beim
		      einfachen rumstöbern angezeigt werden! */ ?>

		var prev_map_id = map_id;
		while ((typeof(MapVisitNavi.map_browser.map_data[world_id][prev_map_id])
		                                        		!= 'undefined')
			&& MapVisitNavi.map_browser.map_data[world_id][prev_map_id]
				.map_is_empty)
		{
			prev_map_id--;
		}
		if (prev_map_id != map_id)
			return MapVisitNavi.map_browser.show_prev(world_id, prev_map_id);
	}


	MapVisitNavi.map_browser.prev_map_id = map_id;
	var players = MapVisitNavi.map_browser.map_data[world_id][map_id].players;

	var width = (max_players+1)*50;

	MapVisitNavi.navigation.prev_map_container_l = new PureFW.Image(
		MapVisitNavi.navigation.map_choose_main,
		MapVisitNavi.navigation.current_map_container_l.position.x
			- 30 - width - 5,
		12,
		5, 67,
		'ui/backgrounds/map_navi/unselected_map_l.png'
	);
	MapVisitNavi.navigation.prev_map_container_l.deactivate();
	MapVisitNavi.navigation.prev_map_container = new PureFW.Container(
		MapVisitNavi.navigation.map_choose_main,
		MapVisitNavi.navigation.current_map_container_l.position.x
			- 30 - width,
		12,
		width, 67
	);
	MapVisitNavi.navigation.prev_map_container.set_bg_img(
		'ui/backgrounds/map_navi/unselected_map_m.png'
	);
	MapVisitNavi.navigation.prev_map_container.deactivate();
	MapVisitNavi.navigation.prev_map_container_r = new PureFW.Image(
		MapVisitNavi.navigation.map_choose_main,
		MapVisitNavi.navigation.current_map_container_l.position.x
			- 30,
		12,
		5, 67,
		'ui/backgrounds/map_navi/unselected_map_r.png'
	);
	MapVisitNavi.navigation.prev_map_container_r.deactivate();

	MapVisitNavi.navigation.prev_map_label = new PureFW.Container(
		MapVisitNavi.navigation.prev_map_container,
		0, 50,
		100, 16
	);
	MapVisitNavi.navigation.prev_map_label.set_font_size(0.8);
	MapVisitNavi.navigation.prev_map_label.set_font_color('#333');
	MapVisitNavi.navigation.prev_map_label.set_content('#'+map_id);

	MapVisitNavi.map_browser.insert_map_pic_and_avatars(
		MapVisitNavi.navigation.prev_map_container, world_id, map_id
	);
}

<?	/**
	 * Funktion zum Nachladen von Kartendaten für die vorherige (linke) Karte.
	 *
	 * @param string[] response_arr
	 * @access private
	 */ ?>
MapVisitNavi.map_browser.show_prev_response = function(response_arr) {
	if (response_arr[0] == "1") {
		var world_id = parseInt(response_arr[1]);
		var map_id = parseInt(response_arr[2]);
		var template_id = parseInt(response_arr[3]);
		var max_players = parseInt(response_arr[4]);
		var players = eval('('+response_arr[5]+')');
		var avatars = eval('('+response_arr[6]+')');
		var is_invited = (parseInt(response_arr[7]) == 1);
		MapVisitNavi.map_browser.set_map_data(world_id, map_id, template_id,
			players, avatars, max_players, is_invited);

		<? /* Daten sind geladen, neuer Versuch :) */ ?>
		MapVisitNavi.map_browser.show_prev(world_id, map_id);
	}
	else if (response_arr[0] == "0") {
		var world_id = parseInt(response_arr[1]);
		var map_id = parseInt(response_arr[2]);
		MapVisitNavi.map_browser.set_map_data(world_id, map_id, 0, new Array(),
			new Array(), 0, false);
		MapVisitNavi.map_browser.show_prev(world_id, map_id);
	}
	else {
		throw new Error(response_arr);
	}
}

<?	/**
	 * Zeigt die angegebene Karte im "Next"-Container an.
	 *
	 * @param uint world_id
	 * @param unit map_id
	 * @access private
	 */ ?>
MapVisitNavi.map_browser.show_next = function(world_id, map_id) {
	<? /* Daten nachladen, wenn noch nicht vorhanden */ ?>
	if ((typeof(MapVisitNavi.map_browser.map_data[world_id]) == 'undefined')
		|| (typeof(MapVisitNavi.map_browser.map_data[world_id][map_id])
			== 'undefined')
		|| !MapVisitNavi.map_browser.map_data[world_id][map_id])
	{
		PureFW.AJAXClientServer.send_request("map.php?get_preview_data=true"
				+ "&map_id="+map_id + "&world_id="+world_id,
			MapVisitNavi.map_browser.show_next_response);
		return;
	}

	try {
		MapVisitNavi.navigation.next_map_container_l.destroy();
		MapVisitNavi.navigation.next_map_container_l = null;
		MapVisitNavi.navigation.next_map_container.destroy();
		MapVisitNavi.navigation.next_map_container = null;
		MapVisitNavi.navigation.next_map_container_r.destroy();
		MapVisitNavi.navigation.next_map_container_r = null;
		MapVisitNavi.navigation.next_map_label.destroy();
		MapVisitNavi.navigation.next_map_label = null;
	}
	catch (e) {}
	var max_players = MapVisitNavi.map_browser.map_data[world_id][map_id]
		.max_players;
	if (max_players == 0)
		return;

	if (!MapVisitNavi.map_browser.friend_mode
		&& !MapVisitNavi.map_browser.choose_mode)
	{
		<? /* Hier muss nun geschaut werden, dass keine leeren Karten beim
		      einfachen rumstöbern angezeigt werden! */ ?>

		var next_map_id = map_id;
		while ((typeof(MapVisitNavi.map_browser.map_data[world_id][next_map_id])
		                                        		!= 'undefined')
			&& MapVisitNavi.map_browser.map_data[world_id][next_map_id].map_is_empty)
		{
			next_map_id++;
		}
		if (next_map_id != map_id)
			return MapVisitNavi.map_browser.show_next(world_id, next_map_id);
	}

	MapVisitNavi.map_browser.next_map_id = map_id;
	var players = MapVisitNavi.map_browser.map_data[world_id][map_id].players;
	var max_players = MapVisitNavi.map_browser.map_data[world_id][map_id].max_players;

	var width = (max_players+1)*50;

	MapVisitNavi.navigation.next_map_container_l = new PureFW.Image(
		MapVisitNavi.navigation.map_choose_main,
		MapVisitNavi.navigation.current_map_container_r.position.x + 30,
		12,
		5, 67,
		'ui/backgrounds/map_navi/unselected_map_l.png'
	);
	MapVisitNavi.navigation.next_map_container_l.deactivate();
	MapVisitNavi.navigation.next_map_container = new PureFW.Container(
		MapVisitNavi.navigation.map_choose_main,
		MapVisitNavi.navigation.current_map_container_r.position.x + 30 + 5,
		12,
		width, 67
	);
	MapVisitNavi.navigation.next_map_container.set_bg_img(
		'ui/backgrounds/map_navi/unselected_map_m.png'
	);
	MapVisitNavi.navigation.next_map_container.deactivate();
	MapVisitNavi.navigation.next_map_container_r = new PureFW.Image(
		MapVisitNavi.navigation.map_choose_main,
		MapVisitNavi.navigation.current_map_container_r.position.x + 30 + 5
			+ width,
		12,
		5, 67,
		'ui/backgrounds/map_navi/unselected_map_r.png'
	);
	MapVisitNavi.navigation.next_map_container_r.deactivate();

	MapVisitNavi.navigation.next_map_label = new PureFW.Container(
		MapVisitNavi.navigation.next_map_container,
		0, 50,
		100, 16
	);
	MapVisitNavi.navigation.next_map_label.set_font_size(0.8);
	MapVisitNavi.navigation.next_map_label.set_font_color('#333');
	MapVisitNavi.navigation.next_map_label.set_content('#'+map_id);

	MapVisitNavi.map_browser.insert_map_pic_and_avatars(
		MapVisitNavi.navigation.next_map_container, world_id, map_id
	);
}

<?	/**
	 * Funktion zum Nachladen von Kartendaten für die nächsten (rechten) Karte.
	 *
	 * @param string[] response_arr
	 * @access private
	 */ ?>
MapVisitNavi.map_browser.show_next_response = function(response_arr) {
	if (response_arr[0] == "1") {
		var world_id = parseInt(response_arr[1]);
		var map_id = parseInt(response_arr[2]);
		var template_id = parseInt(response_arr[3]);
		var max_players = parseInt(response_arr[4]);
		var players = eval('('+response_arr[5]+')');
		var avatars = eval('('+response_arr[6]+')');
		var is_invited = (parseInt(response_arr[7]) == 1);
		MapVisitNavi.map_browser.set_map_data(world_id, map_id, template_id,
			players, avatars, max_players, is_invited);

		<? /* Daten sind geladen, neuer Versuch :) */ ?>
		MapVisitNavi.map_browser.show_next(world_id, map_id);
	}
	else if (response_arr[0] == "0") {
		var world_id = parseInt(response_arr[1]);
		var map_id = parseInt(response_arr[2]);
		MapVisitNavi.map_browser.set_map_data(world_id, map_id, 0, new Array(),
			new Array(), 0, false);
		MapVisitNavi.map_browser.show_next(world_id, map_id);
	}
	else {
		throw new Error(response_arr);
	}
}

<?	/**
	 * Zeigt die aktuelle Karte an.
	 *
	 * Welche Karte hier angezeigt wird, entscheidet die übergebene map_id.
	 *
	 * @param uint map_id
	 * @access private
	 */ ?>
MapVisitNavi.map_browser.show_map = function(world_id, map_id) {
	<? /* Daten nachladen, wenn noch nicht vorhanden */ ?>
	if ((typeof(MapVisitNavi.map_browser.map_data[world_id]) == 'undefined')
		|| (typeof(MapVisitNavi.map_browser.map_data[world_id][map_id])
				== 'undefined')
		|| !MapVisitNavi.map_browser.map_data[world_id][map_id])
	{
		PureFW.AJAXClientServer.send_request("map.php?get_preview_data=true"
				+"&map_id="+map_id+"&world_id="+world_id,
			function (response_arr) {
				if (response_arr[0] == "1") {
					var world_id = parseInt(response_arr[1]);
					var map_id = parseInt(response_arr[2]);
					var template_id = parseInt(response_arr[3]);
					var max_players = parseInt(response_arr[4]);
					var players = eval('('+response_arr[5]+')');
					var avatars = eval('('+response_arr[6]+')');
					var is_invited = (parseInt(response_arr[7]) == 1);
					MapVisitNavi.map_browser.set_map_data(world_id, map_id,
						template_id, players, avatars, max_players, is_invited
					);

					<? /* Daten sind geladen, neuer Versuch :) */ ?>
					MapVisitNavi.create_navigation();
				}
				else {
					throw new Error(response_arr);
				}
			}
		);
		return;
	}
	try {
		MapVisitNavi.navigation.current_map_container_l.destroy();
		MapVisitNavi.navigation.current_map_container_l = null;
		MapVisitNavi.navigation.current_map_container.destroy();
		MapVisitNavi.navigation.current_map_container = null;
		MapVisitNavi.navigation.current_map_container_r.destroy();
		MapVisitNavi.navigation.current_map_container_r = null;
		MapVisitNavi.navigation.map_label.destroy();
		MapVisitNavi.navigation.map_label = null;
	}
	catch (e) {}

	MapVisitNavi.map_browser.current_world_id = world_id;
	MapVisitNavi.map_browser.current_map_id = map_id;
	var players = MapVisitNavi.map_browser.map_data[world_id][map_id].players;
	var max_players = MapVisitNavi.map_browser.map_data[world_id][map_id]
		.max_players;
	var is_invited = MapVisitNavi.map_browser.map_data[world_id][map_id]
		.is_invited;

	var width = (max_players+1)*50;

	MapVisitNavi.navigation.current_map_container_l = new PureFW.Image(
		MapVisitNavi.navigation.map_choose_main,
		((MapVisitNavi.navigation.map_choose_main.width - width) >> 1) - 5,
		12,
		5, 81,
		'ui/backgrounds/map_navi/current_map_l.png'
	);
	MapVisitNavi.navigation.current_map_container = new PureFW.Container(
		MapVisitNavi.navigation.map_choose_main,
		(MapVisitNavi.navigation.map_choose_main.width - width) >> 1,
		12,
		width, 81
	);
	MapVisitNavi.navigation.current_map_container.set_bg_img(
		'ui/backgrounds/map_navi/current_map_m.png'
	);
	MapVisitNavi.navigation.current_map_container_r = new PureFW.Image(
		MapVisitNavi.navigation.map_choose_main,
		((MapVisitNavi.navigation.map_choose_main.width - width) >> 1)
			+ width,
		12,
		5, 81,
		'ui/backgrounds/map_navi/current_map_r.png'
	);

	MapVisitNavi.navigation.map_label = new PureFW.Container(
		MapVisitNavi.navigation.current_map_container,
		0, 50,
		100, 16
	);
	MapVisitNavi.navigation.map_label.set_font_size(0.8);
	MapVisitNavi.navigation.map_label.set_font_color('#333');
	MapVisitNavi.navigation.map_label.set_content('#'+map_id);

	try {
		MapVisitNavi.navigation.visit_current_map.destroy();
	}
	catch(e) {}

	var is_current_loaded_map = ((MapDataCtrl.world_id == world_id) &&
								(MapDataCtrl.map_id == map_id));

	MapVisitNavi.navigation.visit_current_map = new PureFW.Image(
		MapVisitNavi.navigation.map_choose_main,
		(MapVisitNavi.navigation.map_choose_main.width - 162) >> 1,
		65,
		162, 24,
		(is_current_loaded_map)
			? 'ui/elements/form/buttons/<?=$LNG?>/map_navi/join_this_map.png'
			: 'ui/elements/form/buttons/<?=$LNG?>/map_navi/visit_this_map.png'
	);
	MapVisitNavi.navigation.visit_current_map.set_z_index(10);
	if (is_current_loaded_map) {
		MapVisitNavi.navigation.visit_current_map.add_event_handler("click",
			function (ev) {
				MapVisitNavi.map_browser.open_join_dialog(world_id, map_id);
			}
		);
	}
	else {
		MapVisitNavi.navigation.visit_current_map.add_event_handler("click",
			function (ev) {
				try {
					ChooseWorld.destroyable = true;
					LeaveMapWindow.destroy();
					MapVisitNavi.map_browser.show_players_on_map = true;
				}
				catch(e){}
				MapUI.change_map(
					MapVisitNavi.map_browser.current_world_id,
					MapVisitNavi.map_browser.current_map_id
				);
			}
		);
	}

	MapVisitNavi.map_browser.insert_map_pic_and_avatars(
		MapVisitNavi.navigation.current_map_container, world_id, map_id
	);
}

<?	/**
	 * Funktion zum Nachladen von Kartendaten für die aktuelle Karte (Mitte).
	 *
	 * @param string[] response_arr
	 * @access private
	 */ ?>
MapVisitNavi.map_browser.show_map_response = function(response_arr) {
	if (response_arr[0] == "1") {
		var world_id = parseInt(response_arr[1]);
		var map_id = parseInt(response_arr[2]);
		var template_id = parseInt(response_arr[3]);
		var max_players = parseInt(response_arr[4]);
		var players = eval('('+response_arr[5]+')');
		var avatars = eval('('+response_arr[6]+')');
		var is_invited = (parseInt(response_arr[7]) == 1);
		MapVisitNavi.map_browser.set_map_data(world_id, map_id, template_id,
			players, avatars, max_players, is_invited);

		<? /* Daten sind geladen, neuer Versuch :) */ ?>
		//MapVisitNavi.map_browser.show_map(world_id, map_id);
		MapVisitNavi.create_navigation();
	}
	else {
		throw new Error(response_arr);
	}
}

<?	/**
	 * Hilfsfunktion. Fügt die Avatare und das Kartenbild in den angegebenen
	 * Container ein. Wird benutzt, um die einzelnen Kartenbeschreibungsdivs
	 * zu füllen. Da die Füllung ansich bei allen DIVS gleich ist, ist das
	 * hierher ausgelagert.
	 *
	 * @param PureFW.Container parent_container
	 * @param uint world_id
	 * @param uint map_id
	 * @access private
	 */ ?>
MapVisitNavi.map_browser.insert_map_pic_and_avatars = function(parent_container,
	world_id, map_id)
{
	var players = MapVisitNavi.map_browser.map_data[world_id][map_id].players;
	var template_id = MapVisitNavi.map_browser.map_data[world_id][map_id].template_id;
	var avatars = MapVisitNavi.map_browser.map_data[world_id][map_id].avatars;
	var max_players = MapVisitNavi.map_browser.map_data[world_id][map_id].max_players;
	try {
		MapVisitNavi.navigation.loaded_minimaps[world_id][map_id].destroy();
		MapVisitNavi.navigation.loaded_minimaps[world_id][map_id] = null;
		MapVisitNavi.navigation.loaded_avatars[world_id][map_id].destroy();
		MapVisitNavi.navigation.loaded_avatars[world_id][map_id] = null;
	}
	catch (e) {}

	if (typeof(MapVisitNavi.navigation.loaded_minimaps[world_id]) == 'undefined')
		MapVisitNavi.navigation.loaded_minimaps[world_id] = new Array();
	MapVisitNavi.navigation.loaded_minimaps[world_id][map_id]= new PureFW.Image(
		parent_container,
		3, 6,
		44, 44,
		'map/templates/'+template_id+'/_0_/template'+template_id+'_44.jpg'
	);
	var x = 51;
	var n = players.length;
	if (typeof(MapVisitNavi.navigation.loaded_avatars[world_id]) == 'undefined')
		MapVisitNavi.navigation.loaded_avatars[world_id] = new Array();
	MapVisitNavi.navigation.loaded_avatars[world_id][map_id] = new Array();
	var j = 0;
	for (var i = 0; i < n; i++) {
		if (players[i].nick == '-')
			continue;

		MapVisitNavi.navigation.loaded_avatars[world_id][map_id][j]
		= new PureFW.Avatar.Avatar(
			parent_container,
			x, 5,
			48, 48,
			((players[i].nick == '')
				? 'ui/icons/labels/map_navi/free_slot/44/'
						+players[i].color+'.png'
				: avatars[players[i].nick].replace('SIZE',44)),
			players[i].nick
		);
		if (!players[i].alive) {
			MapVisitNavi.navigation.loaded_avatars[world_id][map_id][j]
				.set_opacity(0.4);
		}
		if (players[i].nick != '') {
			if (!players[i].alive) {
				MapVisitNavi.navigation.loaded_avatars[world_id][map_id][j]
					.set_tooltip
				(
					players[i].nick + ' (<?= LNG_LEFT_ISLAND ?>)'
				);
			}
		}

		j++;
		x+=50;
	}
}

<?	/**
	 * Gibt die map_id, die in der aktuellen Topographie die nächst mögliche
	 * darstellt.
	 *
	 * Es wird ein Karten-ID-Trippel zurückgegeben, wobei das mittlere dem
	 * übergebenen am nächsten ist. Die Nachbarn der Mitte sind Modulo der
	 * Topologiegröße ausgewählt, so dass es also ein zyklische Verbindung
	 * darstellt.
	 *
	 * @param uint world_id
	 * @param uint map_id
	 * @return uint[3]
	 */ ?>
MapVisitNavi.get_closest_match = function(world_id, map_id) {
	var result = new Array();
	if (MapVisitNavi.map_browser.choose_mode) {
		<? /** Kartenwahltopologie */ ?>
		var key = MapVisitNavi.map_browser.choose_map_ids.search(
			[ world_id, map_id ], false
		);
		var n = MapVisitNavi.map_browser.choose_map_ids.length;
		if (key < 0) {
			<? /** Karte nicht gefunden. Index suchen, der dem nahe kommt. */ ?>
			for (var i = n-1; i > 0; i--) {
				if ((MapVisitNavi.map_browser.choose_map_ids[i][0] == world_id)
					&& (MapVisitNavi.map_browser.choose_map_ids[i][1] > map_id))
				{
					key = i;
					break;
				}
			}
			if (key < 1) {
				for (var i = n-1; i > 0; i--) {
					if (MapVisitNavi.map_browser.choose_map_ids[i][0]
							> world_id)
					{
						key = i;
						break;
					}
				}
			}
			if (key < 1)
				key = 1;
		}
		if ((key-1) < 0) {
			result[0] = MapVisitNavi.map_browser.choose_map_ids[n-1];
		}
		else {
			result[0] = MapVisitNavi.map_browser.choose_map_ids[key-1];
		}
		result[1] = MapVisitNavi.map_browser.choose_map_ids[key];
		result[2] = MapVisitNavi.map_browser.choose_map_ids[(key+1) % n];
	}
	else if (MapVisitNavi.map_browser.friend_mode) {
		<? /** Freundschaftskartentopologie */ ?>
		if (!MapVisitNavi.map_browser.friend_map_ids) {
			MapVisitNavi.map_browser.load_friend_maps();
			return false;
		}
		var key = MapVisitNavi.map_browser.friend_map_ids.search(
			[ world_id, map_id ], false);

		var n = MapVisitNavi.map_browser.friend_map_ids.length;
		if (key < 0) {
			<? /** Karte nicht gefunden. Index suchen, der dem nahe kommt. */ ?>
			for (var i = n-1; i > 0; i--) {
				if ((MapVisitNavi.map_browser.friend_map_ids[i][0] == world_id)
					&& (MapVisitNavi.map_browser.friend_map_ids[i][1] > map_id))
				{
					key = i;
					break;
				}
			}
			if (key < 0) {
				for (var i = n-1; i > 0; i--) {
					if (MapVisitNavi.map_browser.friend_map_ids[i][0]
						> world_id)
					{
						key = i;
						break;
					}
				}
			}
			if (key < 0)
				key = 1;
		}
		if ((key-1) < 0) {
			result[0] = MapVisitNavi.map_browser.friend_map_ids[n-1];
		}
		else {
			result[0] = MapVisitNavi.map_browser.friend_map_ids[key-1];
		}
		result[1] = MapVisitNavi.map_browser.friend_map_ids[key];
		result[2] = MapVisitNavi.map_browser.friend_map_ids[(key+1) % n];
	}
	else {
		<? /** Weltkartentopologie. Einfach Karte +/- 1 ;-) */ ?>
		if (world_id == 0) {
			result[0] = [ 12, 50 ];
			result[1] = [ 12, 51 ];
			result[2] = [ 12, 52 ];
		}
		else {
			result[0] = [ world_id, map_id-1 ];
			result[1] = [ world_id, map_id ];
			result[2] = [ world_id, map_id+1 ];
		}
	}
	for (var i = 0; i < 3; i++)
		if (typeof(result[i]) == 'undefined')
			result[i] = [ 0, 0 ];
	return result;
}

<?	/**
	 * Scrollen zur angegebenen Karte. Diese wird dann in der Mitte angezeigt,
	 * die Container "Prev" und "Next" werden dann entsprechend gefüllt
	 * (abhängig von der angegebenen Topologie)
	 *
	 * @param uint world_id
	 * @param uint map_id
	 */ ?>
MapVisitNavi.map_browser.goto_map = function(world_id, map_id) {
	world_id = parseInt(world_id);
	map_id = parseInt(map_id);
	var map_ids = MapVisitNavi.get_closest_match(world_id, map_id);
	if (!map_ids)
		return;

	<? /**
		* Es konnte keine Insel gefunden werden => Anzeige, dass es keine
		* Insel gibt.
		* Wenn Freundschaftsmodus, anzeigen, dass er Freunde braucht.
		*/ ?>
	if (!map_ids[0] && !map_ids[1] && !map_ids[2]) {
		if (MapVisitNavi.map_browser.friend_mode) {
			MapVisitNavi.map_browser.show_preview_map_warning(
				'<?= LNG_MAP_VISIT_NAVI_NO_FRIEND_MAPS ?>'.replace(
					'[LINK]',
					''
					/*'<a href="javascript: UIButtons.toggle_friend_invitation();">'*/
				).replace('[/LINK]', ''/*'</a>'*/)
			);
			UIButtons.open_friend_invitation()
		}
		else {
			<? /** Nichts machen... */ ?>
		}
	}
	else {
		MapVisitNavi.map_browser.show_map(map_ids[1][0], map_ids[1][1]);
		MapVisitNavi.map_browser.show_prev(map_ids[0][0], map_ids[0][1]);
		MapVisitNavi.map_browser.show_next(map_ids[2][0], map_ids[2][1]);
	}
}

<?	/**
	 * Zeigt eine Warnung an der Stelle an, wo normalerweise die Kartenvorschau
	 * angezeigt wird.
	 */ ?>
MapVisitNavi.map_browser.show_preview_map_warning = function(text) {
	try {
		MapVisitNavi.navigation.current_map_container_l.destroy();
		MapVisitNavi.navigation.current_map_container_l = null;
		MapVisitNavi.navigation.current_map_container.destroy();
		MapVisitNavi.navigation.current_map_container = null;
		MapVisitNavi.navigation.current_map_container_r.destroy();
		MapVisitNavi.navigation.current_map_container_r = null;
		MapVisitNavi.navigation.map_label.destroy();
		MapVisitNavi.navigation.map_label = null;
	}
	catch (e) {}
	try {
		MapVisitNavi.navigation.next_map_container_l.destroy();
		MapVisitNavi.navigation.next_map_container_l = null;
		MapVisitNavi.navigation.next_map_container.destroy();
		MapVisitNavi.navigation.next_map_container = null;
		MapVisitNavi.navigation.next_map_container_r.destroy();
		MapVisitNavi.navigation.next_map_container_r = null;
		MapVisitNavi.navigation.next_map_label.destroy();
		MapVisitNavi.navigation.next_map_label = null;
	}
	catch (e) {}
	try {
		MapVisitNavi.navigation.prev_map_container_l.destroy();
		MapVisitNavi.navigation.prev_map_container_l = null;
		MapVisitNavi.navigation.prev_map_container.destroy();
		MapVisitNavi.navigation.prev_map_container = null;
		MapVisitNavi.navigation.prev_map_container_r.destroy();
		MapVisitNavi.navigation.prev_map_container_r = null;
		MapVisitNavi.navigation.prev_map_label.destroy();
		MapVisitNavi.navigation.prev_map_label = null;
	}
	catch (e) {}

	var width = 450;

	if (!MapVisitNavi.navigation) {
		return;
	}
	MapVisitNavi.navigation.current_map_container_l = new PureFW.Image(
		MapVisitNavi.navigation.map_choose_main,
		((MapVisitNavi.navigation.map_choose_main.width - width) >> 1) - 5,
		12,
		5, 81,
		'ui/backgrounds/map_navi/current_map_l.png'
	);
	MapVisitNavi.navigation.current_map_container = new PureFW.Container(
		MapVisitNavi.navigation.map_choose_main,
		(MapVisitNavi.navigation.map_choose_main.width - width) >> 1,
		12,
		width, 81
	);
	MapVisitNavi.navigation.current_map_container.set_bg_img(
		'ui/backgrounds/map_navi/current_map_m.png'
	);
	MapVisitNavi.navigation.current_map_container.set_content(text);
	MapVisitNavi.navigation.current_map_container.set_css_class('Window_bright');
	MapVisitNavi.navigation.current_map_container_r = new PureFW.Image(
		MapVisitNavi.navigation.map_choose_main,
		((MapVisitNavi.navigation.map_choose_main.width - width) >> 1)
			+ width,
		12,
		5, 81,
		'ui/backgrounds/map_navi/current_map_r.png'
	);

	if (typeof(MapVisitNavi.navigation.visit_current_map) != 'undefined')
		MapVisitNavi.navigation.visit_current_map.hide();
}

<?	/**
	 * Lädt die Karten der Freunde vom Server nach.
	 *
	 * @access private
	 */ ?>
MapVisitNavi.map_browser.load_friend_maps = function() {
	if (typeof(MapVisitNavi.map_browser.load_friend_maps_resp) == 'undefined') {
		MapVisitNavi.map_browser.load_friend_maps_resp = function(response_arr){
			if (response_arr[0] == "1") {
				MapVisitNavi.map_browser.friend_map_ids =
					eval('('+response_arr[1]+')');
				<? /* schon sortiert. */ ?>
				//MapVisitNavi.map_browser.friend_map_ids.sort();
				MapVisitNavi.map_browser.goto_map(
					MapVisitNavi.map_browser.current_world_id,
					MapVisitNavi.map_browser.current_map_id
				);
			}
			else {
				throw new Error(response_arr);
			}
		}
	}

	PureFW.AJAXClientServer.send_request("map.php?get_friend_maps=true",
		MapVisitNavi.map_browser.load_friend_maps_resp);
}

<?	/**
	 * Wechselt zum Freundkartenbrowsermodus (Freundschaftskartentopologie)
	 */ ?>
MapVisitNavi.map_browser_switch_to_friend_mode = function() {
	if (MapVisitNavi.map_browser.friend_mode)
		return;
	MapVisitNavi.map_browser.friend_mode = true;
	if (!MapVisitNavi.map_browser.friend_map_ids)
		MapVisitNavi.map_browser.load_friend_maps();
	else
		MapVisitNavi.map_browser.goto_map(
			MapVisitNavi.map_browser.current_world_id,
			MapVisitNavi.map_browser.current_map_id
		);
}

<?	/**
	 * Wechselt zum Weltkartenbrowsemodus (Weltkartentopologie)
	 */ ?>
MapVisitNavi.map_browser_switch_to_world_mode = function() {
	MapVisitNavi.map_browser.friend_mode = false;
	MapVisitNavi.map_browser.goto_map(
		MapVisitNavi.map_browser.current_world_id,
		MapVisitNavi.map_browser.current_map_id
	);
}

<?	/**
	 * Zeigt den Dialog an, um mitzuspielen. Je nach dem, ob er eine Einladung
	 * besitzt, wird ein "ich will mitspielen"-Anfragedialog oder ein
	 * "ich spiel jetzt mit"-Direkt-Siedeln-Dialog gezeigt
	 */ ?>
MapVisitNavi.settle_here_conf = null;
MapVisitNavi.map_browser.open_join_dialog = function(world_id, map_id) {
	var error = false;
	var msg = '';
	var on_confirm = null;
	if (MapVisitNavi.map_browser.map_data[world_id][map_id].is_invited) {
		msg = '<?= LNG_LEAVE_MAP_TO_JOIN ?>';
		on_confirm = (function(_world_id, _map_id) {
			return function (ev) {
				PureFW.AJAXClientServer.send_request(
					"ingame.php?dataX=<?=$cryptObj->do_encryption(
						array("leave_map"=>true))?>&join_map=true&map_id="
							+_map_id+"&world_id="+_world_id,
						PauseMenu.leave_map_fetch_result
				);
			}
		})(world_id, map_id);
	}
	else {
		if (MapDataCtrl.is_closed) {
			msg = '<?= LNG_ERR_MAP_CLOSED ?>';
			error = true;
		}
		/*else if (MapDataCtrl.is_mentor_map) {
		}*/
		else {
			var players = MapVisitNavi.map_browser.map_data[world_id][map_id]
				.players;
			var n = players.length;
			var all_dead = true;
			for (var i = 0; i < n; i++) {
				if (players[i].alive)
					all_dead = false;
			}
			if (all_dead) {
				msg = '<?= LNG_ERR_MAP_CLOSED ?>';
				error = true;
			}
			else {
				msg = '<?= LNG_SEND_INVITATION_REQUEST ?>';
				on_confirm = (function(_world_id, _map_id) {
					return function (ev) {
						PureFW.AJAXClientServer.send_request(
							'map.php?request_invitation=true&map_id='+_map_id
								+'&world_id='+_world_id,
							function (response_arr) {
								if (response_arr[0] == 1) {
									MammunUI.show_warning(350, 200,
										'<?= LNG_REQUEST_SENT ?>',
										PureFW.ConfirmationBox.YES);
								}
								else {
									MammunUI.show_warning(350, 200,
										'<?= LNG_ERR_MAP_CLOSED ?>',
										PureFW.ConfirmationBox.NO, true);
								}
							}
						);
					}
				})(world_id, map_id);
			}
		}
	}

	if (error) {
		var box_type = 'info';
		var buttons = PureFW.ConfirmationBox.NO;
	}
	else {
		var box_type = 'info';
		var buttons = PureFW.ConfirmationBox.YES | PureFW.ConfirmationBox.NO;
	}

	try {
		MapVisitNavi.settle_here_conf.destroy();
	}
	catch(e) {}

	MapVisitNavi.settle_here_conf = new PureFW.ConfirmationBox(
		document.body,
		(MammunUI.reference_width - 450) >> 1,
		100,
		450, 130,
		buttons
	);
	MapVisitNavi.settle_here_conf.set_bg_img(
		'ui/backgrounds/confirm/'+box_type+'/'+box_type+'_450x130.png'
	);
	MapVisitNavi.settle_here_conf.set_content(
		'<div style="position: absolute; top:10%; left: 5%; width: 90%; '
		+ 'text-align: center">'
		+ msg
		+'</div>'
	);
	if (on_confirm) {
		MapVisitNavi.settle_here_conf.add_event_handler(
			"confirm",
			on_confirm
		);
	}
}


<?	/**
 	 * Erstellt die Navigation, um zwischen den Inseln zu browsen. Die
 	 * Navigation wird dabei jedesmal _neu_ erstellt.
 	 */ ?>
MapVisitNavi.create_navigation = function(friend_mode) {
	var map_ids = MapVisitNavi.get_closest_match(
		MapDataCtrl.world_id, MapDataCtrl.map_id
	);
	if (friend_mode)
		MapVisitNavi.map_browser_switch_to_friend_mode();

	<? /* Daten nachladen, wenn noch nicht vorhanden */ ?>
	if ((typeof(MapVisitNavi.map_browser.map_data[map_ids[1][0]])=='undefined')
		|| (typeof(MapVisitNavi.map_browser.map_data[map_ids[1][0]]
											[map_ids[1][1]]) == 'undefined')
		|| !MapVisitNavi.map_browser.map_data[map_ids[1][0]][map_ids[1][1]])
	{
		PureFW.AJAXClientServer.send_request(
			"map.php?get_preview_data=true&map_id="+map_ids[1][1]
				+ '&world_id='+map_ids[1][0],
			MapVisitNavi.map_browser.show_map_response
		);
		return;
	}

	var world_id = MapDataCtrl.world_id;//map_ids[1][0]
	var map_id = MapDataCtrl.map_id;//map_ids[1][1];


	try {
		destroy_object(MapVisitNavi.navigation);
		MapVisitNavi.navigation = null;
	}
	catch (e) {}

	MapVisitNavi.navigation = new Object();
	MapVisitNavi.navigation.loaded_minimaps = new Array();
	MapVisitNavi.navigation.loaded_avatars = new Array();

<?	/**
	 * 1. Spieler auf der Karte anzeigen
	 */ ?>

	if (MapVisitNavi.map_browser.show_players_on_map
		&& MapVisitNavi.map_browser.map_data[world_id]
		&& MapVisitNavi.map_browser.map_data[world_id][map_id]
		&& world_id > 0)
	{
		MapVisitNavi.navigation.map_players_container_top = new PureFW.Image(
			document.body,
			MammunUI.reference_width-179, 200,
			179,28,
			'ui/elements/texts/<?=$LNG?>/map_navi/players_on_map_caption.png'
		);
		MapVisitNavi.navigation.map_players_container_top.set_z_index(91);

		MapVisitNavi.navigation.map_players_containers = new Array();
		MapVisitNavi.navigation.map_players_avatars = new Array();
		MapVisitNavi.navigation.map_players_nicks = new Array();
		var players = MapVisitNavi.map_browser.map_data[world_id][map_id]
			.players;
		var avatars = MapVisitNavi.map_browser.map_data[world_id][map_id]
			.avatars;
		var n = players.length;
		var y = 0;
		for (var i = 0, j = 0; i < n; i++) {
			if (players[i].nick == '-')
				continue;

			MapVisitNavi.navigation.map_players_containers[j]
			                                   			= new PureFW.Container(
				document.body,
				MapVisitNavi.navigation.map_players_container_top.position.x,
				MapVisitNavi.navigation.map_players_container_top.position.y
					+ MapVisitNavi.navigation.map_players_container_top.height
					+ y,
				MapVisitNavi.navigation.map_players_container_top.width,
				26
			);
			MapVisitNavi.navigation.map_players_containers[j].set_z_index(91);
			MapVisitNavi.navigation.map_players_containers[j].set_bg_img(
				'ui/backgrounds/map_navi/players_on_map_mid.png'
			);
			/*MapVisitNavi.navigation.map_players_containers[j].add_event_handler(
				"click",
				(function (_nick) {
					return function(ev) {
						UIButtons.open_window(
							'profile.php?nick='	+ _nick, '', 110
						);
					}
				})(players[i].nick)
			);*/
			if (players[i].nick != '') {
				MapVisitNavi.navigation.map_players_avatars[j]
				                            			 = new PureFW.Avatar.Avatar(
					MapVisitNavi.navigation.map_players_containers[j],
					10, 2,
					24, 24,
					avatars[players[i].nick].replace('SIZE',22),
					players[i].nick,
					MapDataCtrl.player_colors[players[i].nick]
				);
				if (!players[i].alive) {
					MapVisitNavi.navigation.map_players_avatars[j]
						.set_opacity(0.4);
					MapVisitNavi.navigation.map_players_avatars[j].set_tooltip(
						players[i].nick + ' (<?= LNG_LEFT_ISLAND ?>)'
					);
				}

				MapVisitNavi.navigation.map_players_nicks[j]
				                          				= new PureFW.Container (
					MapVisitNavi.navigation.map_players_containers[j],
					40, 4,
					135, 20
				);
				MapVisitNavi.navigation.map_players_nicks[j].set_font_size(1.3);
				MapVisitNavi.navigation.map_players_nicks[j].set_content(
					MammunUI.get_nick_main(players[i].nick)
				);
				if (!players[i].alive) {
					MapVisitNavi.navigation.map_players_nicks[j]
						.get_content_node().style.textDecoration
							= 'line-through';
				}
				MapVisitNavi.navigation.map_players_nicks[j].add_event_handler(
					"click",
					(function (_nick) {
						return function(ev) {
							UIButtons.open_window(
								'profile.php?nick='	+ _nick, '', 110
							);
						}
					})(players[i].nick)
				);
			}
			else {	// nick == ''
				MapVisitNavi.navigation.map_players_avatars[j]
				                            			= new PureFW.Container(
					MapVisitNavi.navigation.map_players_containers[j],
					10, 2,
					24, 24
				);
				MapVisitNavi.navigation.map_players_avatars[j].set_bg_img(
					'ui/icons/labels/map_navi/free_slot/24/'
						+players[i].color+'.png'
				);
				if (i == (n-1)) {
					MapVisitNavi.navigation.map_players_nicks[j]
					      				= new PureFW.Container (
						MapVisitNavi.navigation.map_players_containers[j],
						40, 4,
						135, 20
					);
					/*
					MapVisitNavi.navigation.map_players_nicks[j].set_content(
						'<a href="javascript: '
							+'MapVisitNavi.map_browser.open_join_dialog('+map_id+');">'
							+ 'Mitspielen</a>'
					);
					*/
				}
			}

			j++;
			y+=26;
		}
		MapVisitNavi.navigation.map_players_container_bot = new PureFW.Image(
			document.body,
			MapVisitNavi.navigation.map_players_container_top.position.x,
			MapVisitNavi.navigation.map_players_container_top.position.y
				+ MapVisitNavi.navigation.map_players_container_top.height + y,
			MapVisitNavi.navigation.map_players_container_top.width,
			18,
			'ui/backgrounds/map_navi/players_on_map_bot.png'
		);
		MapVisitNavi.navigation.map_players_container_bot.set_z_index(91);
	}

	if (!((world_id == MapDataCtrl.world_id) &&
		(map_id == MapDataCtrl.map_id) &&
		(MapVisitNavi.map_browser.choose_mode)))
		{
	<?	/**
		 * 2. Untere Leiste mit den Karten
		 */ ?>
		MapVisitNavi.navigation.map_choose_main = new PureFW.Container(
			document.body,
			0, MammunUI.reference_height - 131,
			MammunUI.reference_width, 131
		);
		MapVisitNavi.navigation.map_choose_main.set_bg_img(
			'ui/backgrounds/map_navi/bottom_bar.png'
		);
		MapVisitNavi.navigation.map_choose_main.set_z_index(91);
	
		MapVisitNavi.map_browser.goto_map(world_id, map_id);
	
	<?	/**
		 * 3. Navigationspfeile einblenden
		 */ ?>
		MapVisitNavi.navigation.prev_map = new PureFW.Image(
			MapVisitNavi.navigation.map_choose_main,
			30, 30,
			32, 32,
			'ui/icons/labels/map_navi/prev_arrow.png'
		);
		MapVisitNavi.navigation.prev_map.set_z_index(10);
		MapVisitNavi.navigation.prev_map.add_event_handler("click",
			function (ev) {
				MapVisitNavi.map_browser.goto_map(
					MapVisitNavi.map_browser.current_world_id,
					MapVisitNavi.map_browser.prev_map_id
				);
			}
		);
	
		MapVisitNavi.navigation.next_map = new PureFW.Image(
			MapVisitNavi.navigation.map_choose_main,
			MapVisitNavi.navigation.map_choose_main.width - 32 - 30, 30,
			32, 32,
			'ui/icons/labels/map_navi/next_arrow.png'
		);
		MapVisitNavi.navigation.next_map.set_z_index(10);
		MapVisitNavi.navigation.next_map.add_event_handler("click",
			function (ev) {
				MapVisitNavi.map_browser.goto_map(
					MapVisitNavi.map_browser.current_world_id,
					MapVisitNavi.map_browser.next_map_id
				);
			}
		);
	
	<?	/**
		 * 4. Checkbox für Freundschaftsmodustoggle
		 */ ?>
		MapVisitNavi.navigation.friend_mode_cb = new PureFW.Checkbox(
			MapVisitNavi.navigation.map_choose_main,
			20, 90,
			36, 31,
			'ui/elements/form/input/check_toggle.png'
		);
		if (MapVisitNavi.map_browser.friend_mode)
			MapVisitNavi.navigation.friend_mode_cb.check();
		MapVisitNavi.navigation.friend_mode_cb.add_event_handler("check",
			function (ev) {
				MapVisitNavi.map_browser_switch_to_friend_mode();
			}
		);
		MapVisitNavi.navigation.friend_mode_cb.add_event_handler("uncheck",
			function (ev) {
				MapVisitNavi.map_browser_switch_to_world_mode();
			}
		);
		if (MapVisitNavi.map_browser.choose_mode)
			MapVisitNavi.navigation.friend_mode_cb.hide();
	
		MapVisitNavi.navigation.friend_mode_txt = new PureFW.Image(
			MapVisitNavi.navigation.map_choose_main,
			MapVisitNavi.navigation.friend_mode_cb.position.x
				+ MapVisitNavi.navigation.friend_mode_cb.width + 5,
			MapVisitNavi.navigation.friend_mode_cb.position.y + 7,
			280, 18,
			'ui/elements/texts/<?=$LNG?>/map_navi/show_friend_maps_only.png'
		);
		MapVisitNavi.navigation.friend_mode_txt.add_event_handler("click",
			function(ev) {
				MapVisitNavi.navigation.friend_mode_cb.toggle(ev);
			}
		);
	
		if (MapVisitNavi.map_browser.choose_mode)
			MapVisitNavi.navigation.friend_mode_txt.hide();

}
<?	/**
	 * 5. Kartennamenanzeige und "zurück zu meiner Insel"-Button
	 */ ?>
	if (MapVisitNavi.map_browser.show_back_to_my_map_link) {
		MapVisitNavi.navigation.current_map_name = new PureFW.Container(
			document.body,
			Overview.main_container.position.x + Overview.main_container.width,
			5,
			UIButtons.top_container.position.x -
				(Overview.main_container.position.x + Overview.main_container.width),
			0
		);
		MapVisitNavi.navigation.current_map_name.set_z_index(91);
		MapVisitNavi.navigation.current_map_name.set_text_align('center');
		MapVisitNavi.navigation.current_map_name.set_font_size(1.5);
		if (world_id > 0) {
			MapVisitNavi.navigation.current_map_name.set_content(
				MapDataCtrl.world_name + ' #' + map_id
			);
		}
		<? if (User::get_current_user()->get_state() == User::STATE_PLAYING) : ?>
		MapVisitNavi.navigation.back_to_my_map = new PureFW.Image(
			document.body,
			MapVisitNavi.navigation.current_map_name.position.x +
				((MapVisitNavi.navigation.current_map_name.width - 209) >> 1),
			MapVisitNavi.navigation.current_map_name.position.y
				+ MapVisitNavi.navigation.current_map_name.height + 20,
			209, 30,
			'ui/elements/form/buttons/<?=$LNG?>/map_navi/back_to_my_map.png'
		);
		MapVisitNavi.navigation.back_to_my_map.set_z_index(91);
		MapVisitNavi.navigation.back_to_my_map.add_event_handler("click",
			function (ev) {
				MapUI.change_map(
					<?= User::get_current_user()->get_world_id()?>,
					<?= User::get_current_user()->get_map_id()?>
				);
			}
		);
		<? else : ?>
		MapVisitNavi.navigation.back_to_my_map = new PureFW.Image(
				document.body,
				MapVisitNavi.navigation.current_map_name.position.x +
					((MapVisitNavi.navigation.current_map_name.width - 255) >> 1),
				MapVisitNavi.navigation.current_map_name.position.y
					+ MapVisitNavi.navigation.current_map_name.height + 20,
				252, 30,
				'ui/elements/form/buttons/<?=$LNG?>/map_navi/back_to_suggested_islands.png'
			);
			MapVisitNavi.navigation.back_to_my_map.set_z_index(91);
			MapVisitNavi.navigation.back_to_my_map.add_event_handler("click",
				function (ev) {
					UIButtons.open_leave_map_dialog();
					MapVisitNavi.destroy_navigation();
				}
			);
	<? endif; ?>
	}

<?	/**
	 * 6. Wenn der User dabei ist, eine Insel auszuwählen, auf der er siedeln
	 * will, dann kommt jetzt direkt die Conf-Box, ob er hier jetzt siedeln
	 * will. Bei JA folgt Siedeln, bei NEIN wird die Weltwahl wieder gezeigt.
	 */ ?>
	<? if (User::get_current_user()->get_state() != User::STATE_PLAYING) : ?>
	if ((world_id == MapDataCtrl.world_id) &&
		(map_id == MapDataCtrl.map_id) &&
		(MapVisitNavi.map_browser.choose_mode))
	{
		<? 	/**
			 * Wenn die Map-ID nicht korrigiert wurde, dann gilt
			 * (map_id == MapDataCtrl.map_id) und somit, dass hier gesiedelt
			 * werden darf, weil angefragte Map-ID MapDataCtrl.map_id gleich der
			 * korrigierten map_id ist.
			 */ ?>
		MapVisitNavi.navigation.settle_here_conf = new PureFW.ConfirmationBox(
			document.body,
			(MammunUI.reference_width - 450) >> 1,
			100,
			450, 130
		);
		MapVisitNavi.navigation.settle_here_conf.set_bg_img(
			'ui/backgrounds/confirm/info/info_450x130.png'
		);
		MapVisitNavi.navigation.settle_here_conf.set_font_size(2.6);
		MapVisitNavi.navigation.settle_here_conf.set_font_weight('bold');
		MapVisitNavi.navigation.settle_here_conf.set_content(
			'<div style="position: absolute; top:25%; left: 5%; width: 90%; '
														+ 'text-align: center">'
				+'<?= LNG_JOIN_THIS_MAP ?>'
			+'</div>'
		);

		MapVisitNavi.navigation.settle_here_conf.add_event_handler(
			"confirm",
			function (ev) {
				UIButtons.open_leave_map_dialog();
				var tmp = MapVisitNavi.map_browser.choose_map_hrefs[world_id]
															[map_id].split('?');
				UIButtons.open_col_choose_dialog(tmp[1]);
				MapVisitNavi.destroy_navigation();

/*				PureFW.AJAXClientServer.send_request(
					MapVisitNavi.map_browser.choose_map_hrefs[world_id][map_id],
					function (response_arr) {
						var _top = MammunUI.get_top_frame();
						_top.ui_auto_logout = false;
						_top.location.href = 'ingame.php?start=true';
					}
				);*/
			}
		);
		MapVisitNavi.navigation.settle_here_conf.add_event_handler(
			"cancel",
			function (ev) {
				UIButtons.open_leave_map_dialog();
				MapVisitNavi.destroy_navigation();
			}
		);
	}
	<? else : /** Wenn der Spieler gerade spielt **/ ?>

<?	/**
	 * 7. Spieler will die Insel besuchen und die er hat eine Einladung auf die
	 * Insel bekommen. Dann wird ihm gleich oben die Option zum Siedeln #
	 * angezeigt.
	 **/ ?>
	//if (MapVisitNavi.map_browser.map_data[world_id][map_id].is_invited) {
		MapVisitNavi.map_browser.open_join_dialog(world_id, map_id);
	//}

	<? endif; ?>
	try {
		MapVisitNavi.map_browser_icon.hide();
		MapVisitNavi.map_browser_icon_label.hide();
	}
	catch(e) {}
}

<?	/**
	 * Das ist der Destruktor der Navigation...
	 */ ?>
MapVisitNavi.destroy_navigation = function() {
	try {
		destroy_object(MapVisitNavi.navigation);
		MapVisitNavi.navigation = null;
	}
	catch (e) {}
	try {
		MapVisitNavi.map_browser_icon.show();
		MapVisitNavi.map_browser_icon_label.show();
	}
	catch(e) {}
}

<?	/**
	 * Zeigt die Navigation wieder an, nachdem sie versteckt wurde. Existiert
	 * keine Navigation oder wurde sie nicht verborgen, passiert nichts...
	 */ ?>
MapVisitNavi.show_navigation = function() {
	try {
		MapVisitNavi.navigation.map_players_container_top.show();
	}
	catch (e){}
	try {
		var n = MapVisitNavi.navigation.map_players_containers.length;
		for (var i = 0; i < n; i++) {
			try {
				MapVisitNavi.navigation.map_players_containers[i].show();
			} catch(e){}
		}
	}
	catch(e){}
	try {
		MapVisitNavi.navigation.map_players_container_bot.show();
	} catch(e){}
	try {
		MapVisitNavi.navigation.map_choose_main.show();
	} catch(e){}
	try {
		MapVisitNavi.navigation.current_map_name.show();
	} catch(e){}
	try {
		MapVisitNavi.navigation.back_to_my_map.show();
	} catch(e){}
	try {
		MapVisitNavi.map_browser_icon.hide();
		MapVisitNavi.map_browser_icon_label.hide();
	}
	catch(e) {}
	MapVisitNavi.navigation.hidden = false;
}

<?	/**
	 * Versteckt die Navigation.
	 */ ?>
MapVisitNavi.hide_navigation = function() {
	try {
		MapVisitNavi.navigation.map_players_container_top.hide();
	} catch(e){}
	try {
		var n = MapVisitNavi.navigation.map_players_containers.length;
		for (var i = 0; i < n; i++) {
			try {
				MapVisitNavi.navigation.map_players_containers[i].hide();
			} catch(e){}
		}
	}
	catch(e){}
	try {
		MapVisitNavi.navigation.map_players_container_bot.hide();
	} catch(e){}
	try {
		MapVisitNavi.navigation.map_choose_main.hide();
	} catch(e){}
	try {
		MapVisitNavi.navigation.current_map_name.hide();
	} catch(e){}
	try {
		MapVisitNavi.navigation.back_to_my_map.hide();
	} catch(e){}
	try {
		MapVisitNavi.map_browser_icon.show();
		MapVisitNavi.map_browser_icon_label.show();
	}
	catch(e) {}
	MapVisitNavi.navigation.hidden = true;
}
</script>