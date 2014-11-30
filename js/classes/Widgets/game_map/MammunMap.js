/**
 * Repräsentiert die Karte, auf der man bei Mammun spielt.
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
PureFW.GameMap.MammunMap = function(parent_node, x, y, w, h, num_fields_x, 
	num_fields_y, no_scale)
{
	if (typeof(parent_node) !== 'undefined') {
		this.init(parent_node, x, y, w, h, num_fields_x, num_fields_y, no_scale);
		
		this.instance_num = this.constructor.num_instances;
		this.instance_name = "MammunUI.get_top_frame().PureFW.GameMap" +
				".MammunMap.instances["	+this.instance_num+"]";
		
		this.id = "PureFW.GameMap.MammunMap"+this.instance_num;
		this.img_id = "PureFW.GameMap.MammunMap_img"+this.instance_num;
		this.map_id = 'PureFW.GameMap.MammunMap_map'+this.instance_num;
		this.area_id_prefix = "PureFW.GameMap.MammunMap_area"
			+this.instance_num;
		
		this.insert_into_dom_tree();
		this.constructor.num_instances++;
		this.constructor.instances[this.instance_num] = this;
		
		if (!this.no_scale)
			PureFW.WidgetManager.manager_all.register_widget(this);
	}
};
/********************************************************************
 * PureFW.GameMap.MammunMap extends MultilayerMap
 ********************************************************************/
/**
 * TODO: Check all circumstances, when use of PseudoMultilayerMap is needed!
 */
if ((navigator.userAgent.toLowerCase().indexOf("firefox") != -1))
	PureFW.GameMap.MammunMap.extend(PureFW.IsoMap.MultilayerMap);
else
	PureFW.GameMap.MammunMap.extend(PureFW.IsoMap.PseudoMultilayerMap);

/********************************************************************
 * Statische Deklarationen
 ********************************************************************/
PureFW.GameMap.MammunMap.num_instances = 0;		// Instanzüberwachung (anzahl)
PureFW.GameMap.MammunMap.instances = new Array();// Instanzüberwachung (Instanzen)

/********************************************************************
 * Prototyp-Deklarationen
 ********************************************************************/
PureFW.GameMap.MammunMap.prototype.init = function(parent_node, x, y, w, h, 
	num_fields_x, num_fields_y, no_scale)
{
	PureFW.GameMap.MammunMap.parent.init.call(this, parent_node, x, y, w, h, 
		num_fields_x, num_fields_y, no_scale);
	

	/**
	 * Hier werden die einzelnen Sektorbasierten Daten initialisiert
	 */
	this.sector_data_fields = new Array();
	for (var x = 0; x < this.num_fields_x; x++) {
		this.sector_data_fields[x] = new Array();
		for (var y = 0; y < this.num_fields_y; y++)
			this.sector_data_fields[x][y] = null;
	}
	
	/**
	 * Jetzt kommen die Kartendaten
	 */
	this.player_colors = new Object();
	
	/**
	 * Überwachung, welche Ebenen neu gezeichnet werden müssen beim nächsten
	 * Refresh
	 */
	this.layers_to_recreate = new Object();
	this.layers_to_recreate['sector_borders'] = false;
	this.layers_to_recreate['buildings'] = false;
	this.layers_to_recreate['units'] = false;
	this.layers_to_recreate['fight'] = false;
	this.layers_to_recreate['movements'] = false;
	
	/**
	 * Sektorgröße in Gebäudeeinheiten
	 */
	this.num_b_fields_x = 8;
	this.num_b_fields_y = 8;
	this.b_field_scale = 0.8;	// Gebäude zentriert im Sektor etwas verkleinert
	
	/**
	 * Klickflächenebene für die Bewegungspfeile.
	 */
	this.movement_click_areas = new Array();
	this.on_move_arrow_click_functions = new Array();
	this.on_move_arrow_mouseover_functions = new Array();
	this.on_move_arrow_mouseout_functions = new Array();
	
	/**
	 * Der Nick des Users aus dessen Sicht die Karte gezeichnet werden soll.
	 * Wenn leer, dann Beobachtersicht.
	 */
	this.my_nick = '';
};

PureFW.GameMap.MammunMap.prototype.insert_into_dom_tree = function() {
	PureFW.GameMap.MammunMap.parent.insert_into_dom_tree.call(this);
	
	this.add_layer('sector_borders', 0);
	this.add_layer('buildings', 5);
	this.add_layer('units', 10);
	this.add_layer('fight', 15);
	this.add_layer('movements', 20);
	this.add_layer('paths', 25);
};


/**
 * Berechnet die x-Norm des Wertes.
 * 
 * Die x-Norm eines Wertes wird genutzt, um Werte in x-Richtung zu normieren.
 * 
 * @param float value
 * @return float
 */
PureFW.GameMap.MammunMap.prototype.norm_x = function(value) {
	return value*this.field_width/138;
}

/**
 * Berechnet die y-Norm des Wertes.
 * 
 * Die y-Norm eines Wertes wird genutzt, um Werte in y-Richtung zu normieren.
 * @param float value
 * @return float
 */
PureFW.GameMap.MammunMap.prototype.norm_y = function(value) {
	return value*this.field_height/106;
}


/**
 * Aktuallisiert alle Ebenen, die aktuallisiert werden müssen aufgrund von
 * geänderten Daten.
 */
PureFW.GameMap.MammunMap.prototype.refresh = function() {
	PureFW.GameMap.MammunMap.parent.refresh.call(this);
	for (layer_name in this.layers_to_recreate) {
		if (this.layers_to_recreate[layer_name]) {
			switch (layer_name) {
				case 'sector_borders':
					this._update_sector_border_layer();
					break;
				case 'buildings':
					this._update_buildings_layer();
					break;
				case 'units':
					this._update_units_layer();
					break;
				case 'fight':
					this._update_fight_layer();
					break;
				case 'movements':
					this._update_movements_layer();
					break;
			}
			this.layers_to_recreate[layer_name] = false;
		}
	}
};

/**
 * Aktuallisiert die Gebietsgrenzenebene
 */
PureFW.GameMap.MammunMap.prototype._update_sector_border_layer = function() {
	var layer = this.get_layer('sector_borders');
	var field, cur_building, object3d;
	
	var border_attr = new Array(
		{w: this.norm_x(68), h: this.norm_y(52),  
		 x: this.norm_x(35), y:0},
		{w: this.norm_x(35), h: this.norm_y(104), 
		 x: 0,               y: 0},
		{w: this.norm_x(35), h: this.norm_y(104), 
		 x: this.norm_x(103),y:0},
		{w: this.norm_x(68), h: this.norm_y(52),  
		 x: this.norm_x(35), y:this.norm_y(52)},
		{w: this.norm_x(70), h: this.norm_y(64),
		 x: this.norm_x(34), y:this.norm_y(-51)},
		{w: this.norm_x(48), h: this.norm_y(128),
		 x: this.norm_x(-34),y:this.norm_y(-12)},
		{w: this.norm_x(48), h: this.norm_y(128),
		 x: this.norm_x(124),y:this.norm_y(-12)},
		{w: this.norm_x(70), h: this.norm_y(64),
		 x: this.norm_x(34), y:this.norm_y(91)}
	);
	
	for (var x = 0; x < this.num_fields_x; x++) {
		for (var y = 0; y < this.num_fields_y; y++) {
			if (this.sector_data_fields[x][y] && 
				this.sector_data_fields[x][y].dominator)
			{
				/**
				 * Um den Code halbwegs übersichtlich zu halten, brauchen wir
				 * ein paar Abkürzungen.
				 */
				var mx = this.num_fields_x-1;
				var my = this.num_fields_y-1;
				/**
				 * Die Dominatoren des 3x3 Sektor-Blocks werden hier 
				 * umgespeichert, um spätere Range-Abfragen zu vermeiden, sowie
				 * den Code bei den Abfragen möglichst kurz zu halten.
				 */
				var doms = {
					'-1': {
						'-1': ((x>0)&&(y>0)&&this.sector_data_fields[x-1][y-1])?
								this.sector_data_fields[x-1][y-1].dominator : '',
						'0': ((x>0) && this.sector_data_fields[x-1][y]) ?
								this.sector_data_fields[x-1][y].dominator : '',
						'1': ((x>0)&&(y<my)&&this.sector_data_fields[x-1][y+1])?
								this.sector_data_fields[x-1][y+1].dominator : ''
					},
					'0': {
						'-1': ((y>0) && this.sector_data_fields[x][y-1]) ?
							this.sector_data_fields[x][y-1].dominator : '',
						'0': (this.sector_data_fields[x][y]) ?
								this.sector_data_fields[x][y].dominator : '',
						'1': ((y<my) && this.sector_data_fields[x][y+1]) ?
								this.sector_data_fields[x][y+1].dominator : ''
					},
					'1': {
						'-1':((x<mx)&&(y>0)&&this.sector_data_fields[x+1][y-1])?
								this.sector_data_fields[x+1][y-1].dominator :'',
						'0': ((x<mx) && this.sector_data_fields[x+1][y]) ?
								this.sector_data_fields[x+1][y].dominator : '',
						'1':((x<mx)&&(y<my)&&this.sector_data_fields[x+1][y+1])?
								this.sector_data_fields[x+1][y+1].dominator : ''
					}
				}
				
				var pic = new Array();
				
				var dom_color = 
					this.player_colors[this.sector_data_fields[x][y].dominator];
				if (!dom_color)
					dom_color = 'black';
				/**
				 * Jetzt folgt der Kern der Kiste: Welche Sektoren sind mit 
				 * welchen durch die Dominanz gruppiert, woraus folgt, welche
				 * Grenzstücke erzeugt werden müssen für den Sektor.
				 */
				
				/**
				 * Zunächst wird geschaut, ob der rechte Nachbarsektor 
				 * unterschiedlich vom aktuellen ist.
				 * Jede Border ist dabei prinzipiell in zwei Teile getrennt.
				 * Es wird dann geschaut, wie die oberen und unteren Nachbarn
				 * aussehen, um zu entscheiden, ob ein Geraden- und ein Kurven-
				 * stück nötig ist.
				 */
				if (doms['0']['0'] != doms['-1']['0']) {
					if (doms['0']['0'] != doms['0']['-1']) {
						pic[0] = '/map/sector_borders/' + dom_color
													+ '/corn_topleft_inner.png';
					}
					else if (doms['0']['0'] != doms['-1']['-1']) {// geradenstück
						pic[0] = '/map/sector_borders/' + dom_color
											+ '/line_topleft_left_inner.png';
					}
					if (doms['0']['0'] != doms['0']['1']) {
						pic[1] = '/map/sector_borders/' + dom_color
												+ '/corn_bottomleft_inner.png';
					}
					else if (doms['0']['0'] != doms['-1']['1']) {// geradenstück
						pic[1] = '/map/sector_borders/' + dom_color
										+ '/line_bottomleft_left_inner.png';
					}
				}
				/**
				 * Der linke Nachbar ist gleich. Jetzt wird geschaut, ob 
				 * oben und/oder unten ein Geradenstück nötig ist, oder eine
				 * Außenkurve oder gar nichts :)
				 */
				else {
					if (doms['0']['0'] != doms['-1']['-1']) {
						if (doms['0']['0'] != doms['0']['-1']) {
							pic[0] = '/map/sector_borders/' + dom_color
								 				+ '/line_topleft_top_inner.png';
						}
						else {
							pic[4] = '/map/sector_borders/' + dom_color
												+ '/corn_bottomright_outer.png';
						}
					}
					if (doms['0']['0'] != doms['-1']['1']) {
						if (doms['0']['0'] != doms['0']['1']) {
							pic[1] = '/map/sector_borders/' + dom_color
										+ '/line_bottomleft_bottom_inner.png';
						}
						else {
							pic[5] = '/map/sector_borders/' + dom_color 
												+ '/corn_topright_outer.png';
						}
					}
				}
				/**
				 * Dasselbe passiert nun mit dem rechten Nachbarsektor
				 */
				if (doms['0']['0'] != doms['1']['0']) {
					if (doms['0']['0'] != doms['0']['-1']) {
						pic[2] = '/map/sector_borders/' + dom_color
												+ '/corn_topright_inner.png';
					}
					else if (doms['0']['0'] != doms['1']['-1']) {// geradenstück
						pic[2] = '/map/sector_borders/' + dom_color
										+ '/line_topright_right_inner.png';
					}
					if (doms['0']['0'] != doms['0']['1']) {
						pic[3] = '/map/sector_borders/' + dom_color
												+ '/corn_bottomright_inner.png';
					}
					else if (doms['0']['0'] != doms['1']['1']) { // geradenstück
						pic[3] = '/map/sector_borders/' + dom_color
									+ '/line_bottomright_right_inner.png';
					}
				}
				/**
				 * und dem entsprechenden else-case wie oben
				 */
				else {
					if (doms['0']['0'] != doms['1']['-1']) {
						if (doms['0']['0'] != doms['0']['-1']) {
							pic[2] = '/map/sector_borders/' + dom_color
											+ '/line_topright_top_inner.png';
						}
						else {
							pic[6] = '/map/sector_borders/' + dom_color
												+ '/corn_bottomleft_outer.png';
						}
					}
					if (doms['0']['0'] != doms['1']['1']) {
						if (doms['0']['0'] != doms['0']['1']) {
							pic[3] = '/map/sector_borders/' + dom_color
										+ '/line_bottomright_bottom_inner.png';
						}
						else {
							pic[7] = '/map/sector_borders/' + dom_color +
												'/corn_topleft_outer.png';
						}
					}
				}
				
				/**
				 * Okay, nachdem wir jetzt wissen, welche Bilder wohin müssen,
				 * müssen wir sie nur noch erzeugen.
				 */
				var n = pic.length ;
				if (n == 0) {
					layer.destroy_field(x,y);
				}
				else {
					field = layer.get_field(x,y);
					if (!field) {
						field = layer.create_field(x,y,
							PureFW.Container, this.no_scale);
						field.destroy = function() {
							PureFW.Container.prototype.destroy.call(this);
							field.borders.destroy();
							field.borders = null;
						}
						field.get_node().style.overflow = 'visible';
					}
					if (!field.borders)
						field.borders = [null, null, null, null, 
						                 null, null, null, null];
					for (var i = 0; i < 8; i++) {
						if (pic[i] && (pic[i].length > 0)) {
							if (!field.borders[i]) {
								field.borders[i] = new PureFW.Image(
									field,
									border_attr[i].x, border_attr[i].y,
									border_attr[i].w, border_attr[i].h,
									pic[i],
									this.no_scale
								);
							}
							else {
								field.borders[i].set_pic_url(pic[i]);
							}
						}
						else {
							if (field.borders[i]) {
								try {
									field.borders[i].destroy();
								}
								catch(e) {
								}
								field.borders[i] = null;
							}
						}
					}
				}
			}
			else {
				layer.destroy_field(x,y);
			}
		}
	}
};

/**
 * Aktuallisiert die Gebäudeebene
 */
PureFW.GameMap.MammunMap.prototype._update_buildings_layer = function() {
	var layer = this.get_layer('buildings');
	var field;
	for (var x = 0; x < this.num_fields_x; x++) {
		for (var y = 0; y < this.num_fields_y; y++) {
			if (this.sector_data_fields[x][y] && 
				this.sector_data_fields[x][y].buildings)
			{
				field = layer.get_field(x,y);
				if (!field) {
					field = layer.create_field(x,y,
						PureFW.GameMap.BuildingsField, this.num_b_fields_x, 
						this.num_b_fields_y, this.no_scale);
				}
				var n = this.sector_data_fields[x][y].buildings.length;
				field.clear_buildings();
				for (var i = 0; i < n; i++) {
					field.add_building(
						this.sector_data_fields[x][y].buildings[i]
					);
				}
			}
			else {
				layer.destroy_field(x,y);
			}
		}
	}
};


PureFW.GameMap.MammunMap.prototype._add_move_arrow_click_area = function(
	x, y, w, h, data)
{
	var new_area = new PureFW.Image(
		this,
		x, y,
		w, h,
		'pattern/spacer.gif',
		this.no_scale
	);
	new_area.set_z_index(30);	// über allem
	new_area.instance_name = "MammunUI.get_top_frame()."+new_area.instance_name;
	new_area.add_event_handler("click",
		(function(_instance, _data) {
			return function(ev) {
				_instance.on_move_arrow_click(ev, _data);
			}
		})(this, data)
	);
	new_area.add_event_handler("mouseover",
		(function(_instance, _data) {
			return function(ev) {
				_instance.on_move_arrow_mouseover(ev, _data);
			}
		})(this, data)
	);
	new_area.add_event_handler("mouseout",
		(function(_instance, _data) {
			return function(ev) {
				_instance.on_move_arrow_mouseout(ev, _data);
			}
		})(this, data)
	);
	
	this.movement_click_areas.push(new_area);
}
/**
 * Aktuallisiert die Ebene der sich bewegenden Einheiten
 */
PureFW.GameMap.MammunMap.prototype._update_movements_layer = function() {
	var layer = this.get_layer('movements');
	var field;
	var arrow_w = this.norm_x(40);	// dient zur Normierung
	var arrow_h = this.norm_y(34); // dient zur Normierung
	var x_l = this.norm_x(17);
	var x_r = this.field_width - arrow_w - x_l;
	var y_l = this.norm_y(20);
	var y_r = this.field_height - arrow_h - y_l;
	var arrow_attr = new Array(
		{x:x_l,		y:y_l,	dir:-1},
		{x:x_r,		y:y_r,	dir:1},
		{x:x_r,		y:y_l,	dir:-10},
		{x:x_l,		y:y_r,	dir:10}
	);
	for (var x = 0; x < this.num_fields_x; x++) {
		for (var y = 0; y < this.num_fields_y; y++) {
			if (this.sector_data_fields[x][y] && 
				this.sector_data_fields[x][y].movements)
			{
				for (var heading in this.sector_data_fields[x][y].movements) {
					if (isNaN(heading))
						continue;
					
					heading = parseInt(heading);
					
					var unit_data = 
						this.sector_data_fields[x][y].movements[heading];
					field = layer.get_field(x,y);
					if (!field) {
						field = layer.create_field(x,y, PureFW.Container, 
							this.no_scale);
						field.destroy = function() {
							PureFW.Container.prototype.destroy.call(this);
							field.movement_arrows.destroy();
							field.movement_arrows = null;
						}
					}
					
					var sec_to = new PureFW.Point();
					switch (heading) {
						case 0:
							sec_to.x = x+1;
							sec_to.y = y;
							break;
						case 1:
							sec_to.x = x-1;
							sec_to.y = y;
							break;
						case 2:
							sec_to.x = x;
							sec_to.y = y+1;
							break;
						case 3:
							sec_to.x = x;
							sec_to.y = y-1;
							break;
						default:
							sec_to.x = x;
							sec_to.y = y;
					}
					
					/**
					 * Die Bilder und Codes scheinen etwas vermischt zu sein,
					 * deswegen hier eine Korrektur. kA, warum da eine
					 * Vertauschung stattfinden muss, um ehrlich zu sein ^^
					 */
					heading ^= 1;
					
					if (!field.movement_arrows)
						field.movement_arrows = new Array();
					if (!field.movement_arrows[heading]) {
						field.movement_arrows[heading] = new PureFW.Image(
							field,
							arrow_attr[heading].x,
							arrow_attr[heading].y,
							arrow_w,
							arrow_h,
							'map/arrows/arrow_green_'+heading+'.png',
							this.no_scale
						);
					}
					
					var p = this.get_field_pixel_position(x, y);
					/**
					 * Jetzt noch die Klickflächen hinzufügen.
					 */
					this._add_move_arrow_click_area(
						p.x + arrow_attr[heading].x,
						p.y + arrow_attr[heading].y,
						arrow_w,
						arrow_h,
						{ 
							'unit_data': unit_data, 
							'sec_from': {'x': x, 'y':y}, 
							'sec_to' : {'x': sec_to.x, 'y':sec_to.y}
						}
					);
				}
			}
			else {
				layer.destroy_field(x,y);
			}
		}
	}
	
};
/**
 * Aktuallisiert die Ebene der stationären Einheiten.
 */
PureFW.GameMap.MammunMap.prototype._update_units_layer = function() {
	var layer = this.get_layer('units');
	var field, unit_pic, min_unit_hp_per_cent = Number.MAX_VALUE, my_unit, 
		unit_amount = 0;
	for (var x = 0; x < this.num_fields_x; x++) {
		for (var y = 0; y < this.num_fields_y; y++) {
			if (this.sector_data_fields[x][y] && 
				this.sector_data_fields[x][y].units && 
				!this.sector_data_fields[x][y].fight)
			{
				unit_amount = 0;
				min_unit_hp_per_cent = Number.MAX_VALUE;
				for(var nick in this.sector_data_fields[x][y].units) {
					if (nick == '-')	/* Einheiten des Dunkel nicht anzeigen*/
						continue;

					for (var edge in this.sector_data_fields[x][y].units[nick]){
						if (isNaN(edge))
							continue;
						var n = this.sector_data_fields[x][y].units
															[nick][edge].length;
						
						for (var j = 0; j < n; j++) {
							var cur_unit_data 
								= this.sector_data_fields[x][y].units
															[nick][edge][j];
							unit_pic = '../pix/map/units/'
								+ cur_unit_data.pic_url.replace(/\.png/g, 
									"_2.png");
							if (cur_unit_data.amount < min_unit_hp_per_cent)
								min_unit_hp_per_cent = cur_unit_data.amount;
							my_unit = (nick == this.my_nick);
						}
						unit_amount += n;
					}
				}
				
				if (unit_amount == 0) {	// Nur Einheiten des Dunkel :)
					field = layer.destroy_field(x,y);
					continue;
				}
				field = layer.get_field(x,y);
				if (!field) {
					field = layer.create_field(x,y,
						PureFW.Container, this.no_scale);
				}
				if (!field.mammun_image) {
					var size = this.norm_y(25);
					field.mammun_image = field.add_widget(
						PureFW.Image,
						(this.field_width >> 1) - (size >> 1),
						this.field_height - size - (size >> 1),
						size, size,
						unit_pic,
						this.no_scale
					);
					field.mammun_sub = field.add_widget(
						PureFW.Container,
						(this.field_width >> 1) - (size >> 1),
						this.field_height - (size >> 1),
						size, 0,
						this.no_scale
					);
					field.mammun_sub.set_font_size(0.8);
					field.mammun_sub.set_content(unit_amount);
					field.mammun_sub.set_font_color('white');
					field.mammun_sub.set_text_align('center');
					if ((min_unit_hp_per_cent > 600) && 
						(min_unit_hp_per_cent < Number.MAX_VALUE) &&
						my_unit)
					{
						field.zzz = field.add_widget(
							PureFW.FilmAnimator,
							field.mammun_image.position.x
								+ 5,
							field.mammun_image.position.y
								- 30,
							30, 30,
							this.no_scale
						);
						field.zzz.set_image(
							'ui/animations/zzz/30/zzz.png', 4
						);
						field.zzz.set_frame_delay(200)
						field.zzz.start_animation();
					}
				}
				else {
					field.mammun_image.set_pic_url(unit_pic);
					field.mammun_sub.set_content(unit_amount);
				}
			}
			else {
				layer.destroy_field(x,y);
			}
		}
	}
};
/**
 * Aktuallisiert die Kampfebene.
 */
PureFW.GameMap.MammunMap.prototype._update_fight_layer = function() {
	var layer = this.get_layer('fight');
	var field;
	for (var x = 0; x < this.num_fields_x; x++) {
		for (var y = 0; y < this.num_fields_y; y++) {
			if (this.sector_data_fields[x][y] && 
				this.sector_data_fields[x][y].fight) 
			{
				field = layer.get_field(x,y);
				if (!field) {
					layer.create_field(x,y,
						PureFW.Image, 'map/fight_red.png', this.no_scale);
				}
				else {
					field.set_pic_url('map/fight_red.png');
				}
			}
			else {
				layer.destroy_field(x,y);
			}
		}
	}
}


/**
 * Zeigt einen Pfad auf der Karte an (mit schönen Fähnchen :) )
 * 
 * @param int[] path			Pfad, der angezeigt werden soll
 * @param uint time_per_sector	Zeit in Sekunden, die pro Sektor gebraucht wird
 * @param uint time_elapsed=0	Zeit, die die Einheite schon
 *								gelaufen ist, die den Pfad benutzt.
 * @param int offset=0			Allgemeiner Zeitshift.
 */
PureFW.GameMap.MammunMap.prototype.show_path = function(path, time_per_sector, 
	time_elapsed, offset) 
{
	if (typeof(time_elapsed) == 'undefined')
		time_elapsed = 0;
	if (typeof(offset) == 'undefined')
		offset = 0;
	var t = -time_elapsed;
	var layer = this.get_layer("paths");
	for (var i = 0; i < 10; i++) {
		for (var j = 0; j < 10; j++) {
			if (layer.get_field(i,j))
				layer.destroy_field(i,j);
		}
	}
	for (var i = 0; i < path.length; i++) {
		if (t >= 0) {
			var secnr = path[i];
			var x = secnr % 10;
			var y = Math.floor(secnr/10);
			var field = layer.get_field(x, y);
			if (!field)
				field = layer.create_field(x, y, PureFW.GameMap.PathFindField, 
					this.no_scale);
			
			field.set_t_stamp((t+offset) % 86400);
		}
		t += time_per_sector;
	}
}


/**
 * Erweitert das Hinzufügen von Ereignissen um die Ereignisse "move_arrow_click",
 * "move_arrow_mouseover", "move_arrow_mouseout"
 * 
 * @see Widget#add_event_handler
 * @param string type
 * @param Function fn
 */
PureFW.GameMap.MammunMap.prototype.add_event_handler = function(type, fn) {
	if (type == "move_arrow_mouseover") {
		this.on_move_arrow_mouseover_functions.push(fn);
	}
	else if (type == "move_arrow_mouseout") {
		this.on_move_arrow_mouseout_functions.push(fn);
	}
	else if (type == "move_arrow_click") {
		this.on_move_arrow_click_functions.push(fn);
	}
	else {
		PureFW.GameMap.MammunMap.parent.add_event_handler.call(this, type, fn);
	}
}

/**
 * Erweitert das Entfernen von Ereignissen um die Ereignisse "move_arrow_click",
 * "move_arrow_mouseover", "move_arrow_mouseout"
 * 
 * @see Widget#remove_event_handler
 * @param string type
 * @param Function fn
 */
PureFW.GameMap.MammunMap.prototype.remove_event_handler = function(type, fn) {
	if (type == "move_arrow_mouseover") {
		this.on_move_arrow_mouseover_functions.remove(fn);
	}
	else if (type == "move_arrow_mouseout") {
		this.on_move_arrow_mouseout_functions.remove(fn);
	}
	else if (type == "move_arrow_click") {
		this.on_move_arrow_click_functions.remove(fn);
	}
	else {
		PureFW.GameMap.MammunMap.parent.remove_event_handler.call(this,type,fn);
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
PureFW.GameMap.MammunMap.prototype.on_move_arrow_click = function(ev, 
	unit_data) 
{
	if (ev) {
		try {
			ev = PureFW.EventUtil.formatEvent(ev);
			ev.detail = unit_data;
			if (typeof(ev.detail) != 'object')
				throw "Event is not changable";
		}
		catch(e) {
			ev = this.create_event("click");
			ev.detail = unit_data;
		}
	}
	else {
		ev = this.create_event("click");
		ev.detail = unit_data;
	}
	
	var n = this.on_move_arrow_click_functions.length;
	for (var i = 0; i < n; i++) {
		this.on_move_arrow_click_functions[i].call(this, ev);
	}
}

/**
 * Wrapperfunktion, um das MouseOver-Event auszulösen mit ergänzenden 
 * Informationen bzgl. des angeklickten Feldes zu versehen und dann erst zu 
 * feuern.
 * 
 * @param Event ev
 * @param Object unit_data
 */
PureFW.GameMap.MammunMap.prototype.on_move_arrow_mouseover = function(ev, 
	unit_data) 
{
	if (ev) {
		try {
			ev = PureFW.EventUtil.formatEvent(ev);
			ev.detail = unit_data;
			if (typeof(ev.detail) != 'object')
				throw "Event is not changable";
		}
		catch(e) {
			ev = this.create_event("mouseover");
			ev.detail = unit_data;
		}
	}
	else {
		ev = this.create_event("mouseover");
		ev.detail = unit_data;
	}
	
	var n = this.on_move_arrow_mouseover_functions.length;
	for (var i = 0; i < n; i++)
		this.on_move_arrow_mouseover_functions[i].call(this, ev);
}

/**
 * Wrapperfunktion, um das MouseOver-Event auszulösen mit ergänzenden 
 * Informationen bzgl. des angeklickten Feldes zu versehen und dann erst zu 
 * feuern.
 * 
 * @param Event ev
 * @param Object unit_data
 */
PureFW.GameMap.MammunMap.prototype.on_move_arrow_mouseout = function(ev, 
	unit_data) 
{
	if (ev) {
		try {
			ev = PureFW.EventUtil.formatEvent(ev);
			ev.detail = unit_data;
			if (typeof(ev.detail) != 'object')
				throw "Event is not changable";
		}
		catch(e) {
			ev = this.create_event("mouseout");
			ev.detail = unit_data;
		}
	}
	else {
		ev = this.create_event("mouseout");
		ev.detail = unit_data;
	}
	
	var n = this.on_move_arrow_mouseout_functions.length;
	for (var i = 0; i < n; i++)
		this.on_move_arrow_mouseout_functions[i].call(this, ev);
}


/******************************************************************************
 *            Unspannende Getter und Setter für die Sektordaten               *
 ******************************************************************************/

/**
 * Setzt den Dominator im Sektor.
 *
 * @param String dominator
 * @param uint x
 * @param uint y
 */
PureFW.GameMap.MammunMap.prototype.set_dominator = function(x, y, dominator) {
	if (!this.sector_data_fields[x][y])
		this.sector_data_fields[x][y] = new PureFW.GameMap.SectorData(x,y);
	if (this.sector_data_fields[x][y].dominator != dominator) {
		this.sector_data_fields[x][y].dominator = dominator;
		this.layers_to_recreate['sector_borders'] = true;
	}
};

/**
 * Gibt den Dominator im Sektor zurück.
 * 
 * @param uint x
 * @param uint y
 * @return String
 */
PureFW.GameMap.MammunMap.prototype.get_dominator = function(x, y) {
	return this.sector_data_fields[x][y].dominator;
};

/**
 * Setzt den Einheiten im Sektor.
 *
 * @param String dominator
 * @param uint x
 * @param uint y
 */
PureFW.GameMap.MammunMap.prototype.set_units = function(x, y, units) {
	if (!this.sector_data_fields[x][y])
		this.sector_data_fields[x][y] = new PureFW.GameMap.SectorData(x,y);
	if (this.sector_data_fields[x][y].units != units) {
		this.sector_data_fields[x][y].units = units;
		this.layers_to_recreate['units'] = true;
	}
};

/**
 * Gibt den Einheiten im Sektor zurück.
 * 
 * @param uint x
 * @param uint y
 * @return String
 */
PureFW.GameMap.MammunMap.prototype.get_units = function(x, y) {
	return this.sector_data_fields[x][y].units;
};


/**
 * Setzt den Gebäude im Sektor.
 *
 * @param String dominator
 * @param uint x
 * @param uint y
 */
PureFW.GameMap.MammunMap.prototype.set_buildings = function(x, y, buildings) {
	if (!this.sector_data_fields[x][y])
		this.sector_data_fields[x][y] = new PureFW.GameMap.SectorData(x,y);
	
	if (this.sector_data_fields[x][y].buildings != buildings) {
		this.sector_data_fields[x][y].buildings = buildings;
		this.layers_to_recreate['buildings'] = true;
	}
};

/**
 * Gibt den Gebäude im Sektor zurück.
 * 
 * @param uint x
 * @param uint y
 * @return String
 */
PureFW.GameMap.MammunMap.prototype.get_buildings = function(x, y) {
	return this.sector_data_fields[x][y].buildings;
};

/**
 * Setzt den Ressourcen im Sektor.
 *
 * @param String dominator
 * @param uint x
 * @param uint y
 */
PureFW.GameMap.MammunMap.prototype.set_resources = function(x, y, resources) {
	if (!this.sector_data_fields[x][y])
		this.sector_data_fields[x][y] = new PureFW.GameMap.SectorData(x,y);
	if (this.sector_data_fields[x][y].resources != resources) {
		this.sector_data_fields[x][y].resources = resources;
	}
};

/**
 * Gibt den Ressourcen im Sektor zurück.
 * 
 * @param uint x
 * @param uint y
 * @return String
 */
PureFW.GameMap.MammunMap.prototype.get_resources = function(x, y) {
	return this.sector_data_fields[x][y].resources;
};

/**
 * Setzt den Truppenbewegungen im Sektor.
 *
 * @param String dominator
 * @param uint x
 * @param uint y
 */
PureFW.GameMap.MammunMap.prototype.set_movements = function(x, y, movements) {
	if (!this.sector_data_fields[x][y])
		this.sector_data_fields[x][y] = new PureFW.GameMap.SectorData(x,y);
	if (this.sector_data_fields[x][y].movements != movements) {
		this.sector_data_fields[x][y].movements = movements;
		this.layers_to_recreate['movements'] = true;
	}
};

/**
 * Gibt den Truppenbewegungen im Sektor zurück.
 * 
 * @param uint x
 * @param uint y
 * @return String
 */
PureFW.GameMap.MammunMap.prototype.get_movements = function(x, y) {
	return this.sector_data_fields[x][y].movements;
};

/**
 * Setzt den Kämpfe im Sektor.
 *
 * @param String dominator
 * @param uint x
 * @param uint y
 */
PureFW.GameMap.MammunMap.prototype.set_fight = function(x, y, fight) {
	if (!this.sector_data_fields[x][y])
		this.sector_data_fields[x][y] = new PureFW.GameMap.SectorData(x,y);
	if (this.sector_data_fields[x][y].fight != fight) {
		this.sector_data_fields[x][y].fight = fight;
		this.layers_to_recreate['fight'] = true;
	}
};

/**
 * Gibt den Kämpfe im Sektor zurück.
 * 
 * @param uint x
 * @param uint y
 * @return String
 */
PureFW.GameMap.MammunMap.prototype.get_fight = function(x, y) {
	return this.sector_data_fields[x][y].fight;
};

/**
 * Setzt die Farben des Spielers auf der Karte.
 *
 * @param String player
 * @param {'black', 'blue', 'green', 'orange', 'pink', 'purple', 'red', 
 * 			'white', 'yellow'} color
 */
PureFW.GameMap.MammunMap.prototype.set_player_color = function(player, color) {
	if (this.player_colors[player] != color) {
		this.player_colors[player] = color;
		this.layers_to_recreate['sector_borders'] = true;
	}
};

/**
 * Gibt die Farben des Spielers auf der Karte zurück.
 *
 * @param String player
 * @return {'black', 'blue', 'green', 'orange', 'pink', 'purple', 'red', 
 * 			'white', 'yellow'} 
 */
PureFW.GameMap.MammunMap.prototype.get_player_color = function(player) {
	return this.player_colors[player];
};