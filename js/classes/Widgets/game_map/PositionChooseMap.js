/**
 * Diese Karte dient zum Auswählen der Position vor Mapstart.
 */

if (typeof(PureFW.GameMap) != 'object')
	PureFW.GameMap = new Object();

/**
 * Erzeugt die Mammun-Spielkarte
 * 
 * Die Felderhöhe und Breite und somit der Blickwinkel wird durch die Angaben
 * der Höhe und Breite der Map, sowie der Anzahl der Felder in X- und Y-
 * Richtung bestimmt.
 * 
 * @param Node parent_node	An welchen Knoten die Map angehängt werden soll
 * @param int x				Wo sich die Map befinden soll in x-Richtung in px
 * @param int y				Wo sich die Map befinden soll in y-Richtung in px
 * @param int w				Wie breit die gesamte Map sein soll
 * @param int h				Wie hoch die gesamte Map sein soll
 * @param int num_fields_x	Wie viele Felder die Map haben soll in X-Richtung 
 * @param int num_fields_y	Wie viele Fleder die Map haben soll in Y-Richtung
 * @param bool no_scale		Nicht sklierbar (default: false)
 * 
 * @see PureFW.GameMap.MultilayerMap
 */
PureFW.GameMap.PositionChooseMap = function(parent_node, x, y, w, h, 
	num_fields_x, num_fields_y, no_scale)
{
	if (typeof(parent_node) !== 'undefined') {
		this.init(parent_node, x, y, w, h, num_fields_x, num_fields_y, no_scale);
		
		this.instance_num = this.constructor.num_instances;
		this.instance_name = "MammunUI.get_top_frame().PureFW.GameMap" +
				".PositionChooseMap.instances["	+this.instance_num+"]";
		
		this.id = "PureFW.GameMap.PositionChooseMap"+this.instance_num;
		this.img_id = "PureFW.GameMap.PositionChooseMap_img"+this.instance_num;
		this.map_id = 'PureFW.GameMap.PositionChooseMap_map'+this.instance_num;
		this.area_id_prefix = "PureFW.GameMap.PositionChooseMap_area"
			+this.instance_num;
		
		this.insert_into_dom_tree();
		this.constructor.num_instances++;
		this.constructor.instances[this.instance_num] = this;
		
		if (!this.no_scale)
			PureFW.WidgetManager.manager_all.register_widget(this);
	}
};
/********************************************************************
 * PureFW.GameMap.PositionChooseMap extends MultilayerMap
 ********************************************************************/
/**
 * TODO: Check all circumstances, when use of PseudoMultilayerMap is needed!
 */
PureFW.GameMap.PositionChooseMap.extend(PureFW.IsoMap.VisMap);

/********************************************************************
 * Statische Deklarationen
 ********************************************************************/
PureFW.GameMap.PositionChooseMap.num_instances = 0;		// Instanzüberwachung (anzahl)
PureFW.GameMap.PositionChooseMap.instances = new Array();// Instanzüberwachung (Instanzen)

PureFW.GameMap.PositionChooseMap.colors =
	['blue', 'green', 'orange', 'pink', 'purple', 'red', 'white', 'yellow'];

/********************************************************************
 * Prototyp-Deklarationen
 ********************************************************************/
PureFW.GameMap.PositionChooseMap.prototype.init = function(parent_node, x, y, 
	w, h, num_fields_x, num_fields_y, no_scale)
{
	PureFW.GameMap.PositionChooseMap.parent.init.call(this, parent_node, x, y, 
		w, h, num_fields_x, num_fields_y, no_scale);
	
	this.homesectors = new Object();
	this.used_colors = {
		'blue': false, 
		'green': false, 
		'orange': false, 
		'pink': false, 
		'purple': false, 
		'red': false, 
		'white': false, 
		'yellow': false
	};
	
	this.on_field_mouseover_functions = new Array();
	this.on_field_mouseout_functions = new Array();
	this.on_field_click_functions = new Array();
}
	

PureFW.GameMap.PositionChooseMap.prototype.add_homesector = function(x, y) {
	x = parseInt(x); y = parseInt(y);
	if (typeof(this.homesectors[x]) != 'object')
		this.homesectors[x] = new Object();
	this.homesectors[x][y] = new Object();
}

PureFW.GameMap.PositionChooseMap.prototype.set_player  = function(x, y, nick, 
	color, avatar)
{
	/**
	 * Falls der User schon einen Platz hat, dann entfernen wir ihn
	 */
	this.remove_player(nick);
	
	if (typeof(this.homesectors[x]) != 'object')
		this.homesectors[x] = new Object();
	
	if (typeof(this.homesectors[x][y]) == 'object') {
		this.homesectors[x][y].nick = nick;
		this.homesectors[x][y].color = color;
		this.homesectors[x][y].avatar = avatar;
	}
	else {
		this.homesectors[x][y] = {
			'nick': nick,
			'color': color,
			'avatar': avatar
		};
	}
	
	this.used_colors[color] = true;
}

PureFW.GameMap.PositionChooseMap.prototype.change_color = function(nick, color)
{
	for (var x in this.homesectors) {
		for (var y in this.homesectors[x]) {
			if (this.homesectors[x][y].nick && 
				(this.homesectors[x][y].nick == nick))
			{
				this.used_colors[this.homesectors[x][y].color] = false;
				this.homesectors[x][y].color = color;
				this.used_colors[this.homesectors[x][y].color] = true;
				break;
			}
		}
	}
}

PureFW.GameMap.PositionChooseMap.prototype.remove_player = function(x, y) {
	if (isNaN(x)) {
		var nick = x;
		/**
		 * Nick ist gemeint
		 */
		for (var x in this.homesectors) {
			for (var y in this.homesectors[x]) {
				if (this.homesectors[x][y].nick && 
					(this.homesectors[x][y].nick == nick))
				{
					this.used_colors[this.homesectors[x][y].color] = false;
					delete this.homesectors[x][y];
					this.homesectors[x][y] = null;	// IE 4tw...
					this.homesectors[x][y] = new Object();
					break;
				}
			}
		}
	}
	else {
		if (typeof(this.homesectors[x]) != 'object')
			this.homesectors[x] = new Object();
		if (typeof(this.homesectors[x][y]) == 'object') {
			this.used_colors[this.homesectors[x][y].color] = false;
			delete this.homesectors[x][y];
			this.homesectors[x][y] = null;	// IE 4tw...
			this.homesectors[x][y] = new Object();
		}		
	}
}


PureFW.GameMap.PositionChooseMap.prototype.is_empty = function(x, y) {
	return ((typeof(this.homesectors[x]) != 'object') ||
				(typeof(this.homesectors[x][y]) != 'object') ||
					!this.homesectors[x][y].nick);
}


PureFW.GameMap.PositionChooseMap.prototype.refresh = function() {
	var empty_chosen_cols = new Object();
	for (var x in this.homesectors) {
		if (isNaN(x))
			continue;
		for (var y in this.homesectors[x]) {
			if (isNaN(y))
				continue;
			
			var player = this.homesectors[x][y];

			x = parseInt(x);
			y = parseInt(y);
			var field =	this.create_field (
				x, y,
				PureFW.Container
			);
			field.set_x(field.position.x + ((field.width - 66) >> 1));
			field.set_y(field.position.y + ((field.height - 49) >> 1));
			field.set_width(66);
			field.set_height(49);
			if (!player.color) {
				var col = '';
				for (var c in this.used_colors) {
					if (!this.used_colors[c] && !empty_chosen_cols[c]) {
						col = c;
						empty_chosen_cols[c] = true;
						break;
					}
				}
			}
			else {
				col = player.color;
			}
			field.set_bg_img(
				'map/sector_borders/' + col + '/square_66x49.png'
			);
			field.add_event_handler('click', (function(_instance, _x, _y) {
				return function(ev) {
					_instance.on_field_click(ev, _x, _y);
				}
			})(this, x, y));
			
			field.add_event_handler('mouseover', (function(_instance, _x, _y) {
				return function(ev) {
					_instance.on_field_mouseover(ev, _x, _y);
				}
			})(this, x, y));
			
			field.add_event_handler('mouseover', (function(_instance, _x, _y) {
				return function(ev) {
					_instance.on_field_mouseover(ev, _x, _y);
				}
			})(this, x, y));

			if (player.avatar) {
				field.add_widget(
					PureFW.Image,
					0, 0,
					field.width, field.height,
					player.avatar.replace(/\.(png|jpg|gif)/, '_3d.png')
				);
			}
		}
	}
}


/**
 * Wrapperfunktion, um das Click-Event auszulösen mit ergänzenden Informationen
 * bzgl. des angeklickten Feldes zu versehen und dann erst zu feuern.
 * 
 * @param Event ev
 * @param uint i
 * @param uint j
 */
PureFW.GameMap.PositionChooseMap.prototype.on_field_click = function(ev, i, j) {
	if (n == 0)
		return;
	var n = this.on_field_click_functions.length;
	ev2 = null;
	if (ev) {
		try {
			ev = PureFW.EventUtil.formatEvent(ev);
			ev.detail = new PureFW.Point(i,j);
			if (typeof(ev.detail) != 'object')
				throw "Event is not changable";
		}
		catch(e) {
			ev2 = this.create_event("click");
			ev2.detail = new PureFW.Point(i,j);
		}
	}
	else {
		ev = this.create_event("click");
		ev.detail = new PureFW.Point(i,j);
	}
	
	for (var i = 0; i < n; i++) {
		this.on_field_click_functions[i].call(this, ev2 || ev);
	}
}

/**
 * Wrapperfunktion, um das MouseOver-Event auszulösen mit ergänzenden 
 * Informationen bzgl. des angeklickten Feldes zu versehen und dann erst zu 
 * feuern.
 * 
 * @param Event ev
 * @param uint i
 * @param uint j
 */
PureFW.GameMap.PositionChooseMap.prototype.on_field_mouseover = function(ev, i, 
	j) 
{
	var n = this.on_field_mouseover_functions.length;
	if (n == 0)
		return;
	
	ev2 = null;
	if (ev) {
		try {
			ev = PureFW.EventUtil.formatEvent(ev);
			ev.detail = new PureFW.Point(i,j);
			if (typeof(ev.detail) != 'object')
				throw "Event is not changable";
		}
		catch(e) {
			ev2 = this.create_event("mouseover");
			ev2.detail = new PureFW.Point(i,j);
		}
	}
	else {
		ev = this.create_event("mouseover");
		ev.detail = new PureFW.Point(i,j);
	}
	
	for (var i = 0; i < n; i++)
		this.on_field_mouseover_functions[i].call(this, ev2 || ev);
}

/**
 * Wrapperfunktion, um das MouseOver-Event auszulösen mit ergänzenden 
 * Informationen bzgl. des angeklickten Feldes zu versehen und dann erst zu 
 * feuern.
 * 
 * @param Event ev
 * @param IsoMouseOverMap.Field field
 */
PureFW.GameMap.PositionChooseMap.prototype.on_field_mouseout = function(ev, i, 
	j) 
{
	var n = this.on_field_mouseout_functions.length;
	if (n == 0)
		return;
	ev2 = null;
	if (ev) {
		try {
			ev = PureFW.EventUtil.formatEvent(ev);
			ev.detail = new PureFW.Point(i,j);
			if (typeof(ev.detail) != 'object')
				throw "Event is not changable";
		}
		catch(e) {
			ev2 = this.create_event("mouseout");
			ev2.detail = new PureFW.Point(i,j);
		}
	}
	else {
		ev = this.create_event("mouseout");
		ev.detail = new PureFW.Point(i,j);
	}
	
	for (var i = 0; i < n; i++)
		this.on_field_mouseout_functions[i].call(this, ev2 || ev);
}



/**
 * Erweitert das Hinzufügen von Ereignissen um die Ereignisse "field_click",
 * "field_mouseover", "field_mouseout"
 * 
 * @see Widget#add_event_handler
 * @param string type
 * @param Function fn
 */
PureFW.GameMap.PositionChooseMap.prototype.add_event_handler = function(type, 
	fn) 
{
	if (type == "field_mouseover") {
		this.on_field_mouseover_functions.push(fn);
	}
	else if (type == "field_mouseout") {
		this.on_field_mouseout_functions.push(fn);
	}
	else if (type == "field_click") {
		this.on_field_click_functions.push(fn);
	}
	else {
		PureFW.GameMap.PositionChooseMap.parent.add_event_handler.call(this, 
			type, fn);
	}
}

/**
 * Erweitert das Entfernen von Ereignissen um die Ereignisse "field_click",
 * "field_mouseover", "field_mouseout"
 * 
 * @see Widget#add_event_handler
 * @param string type
 * @param Function fn
 */
PureFW.GameMap.PositionChooseMap.prototype.remove_event_handler = function(type, 
	fn) 
{
	if (type == "field_mouseover") {
		this.on_field_mouseover_functions.remove(fn);
	}
	else if (type == "field_mouseout") {
		this.on_field_mouseout_functions.remove(fn);
	}
	else if (type == "field_click") {
		this.on_field_click_functions.remove(fn);
	}
	else {
		PureFW.GameMap.PositionChooseMap.parent.remove_event_handler.call(this, 
			type, fn);
	}
}