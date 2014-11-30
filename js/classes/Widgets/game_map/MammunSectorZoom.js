/**
 * Stellt einen einzelnen Sektor gezoomt und mit allen Details dar.
 * 
 * Es besteht prinzipiell aus folgenden Ebenen
 * - Gebäudebau-Clickmap
 * - Einheiten-Clickmap
 * - Gebäude-Clickmap
 * - Einheitendarstellung West
 * - Einheitendarstellung Nord
 * - Gebäudedarstellung
 * - Einheitendarstellung Ost
 * - Einheitendarstellung Süd
 * - Sektorgebäudefeldgitter
 */

if (typeof(PureFW.GameMap) != 'object')
	PureFW.GameMap = new Object();

/**
 * Erzeugt die Mammun-Detail-Ansicht.
 * 
 * Die Felderhöhe und Breite und somit der Blickwinkel wird durch die Angaben
 * der Höhe und Breite des Sektors, sowie der Anzahl der Felder in X- und Y-
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
PureFW.GameMap.MammunSectorZoom = function(parent_node, x, y, w, h, 
	num_fields_x, num_fields_y, no_scale)
{
	if (typeof(parent_node) !== 'undefined') {
		this.init(parent_node, x, y, w, h, num_fields_x, num_fields_y, no_scale);
		
		this.instance_num = this.constructor.num_instances;
		this.instance_name = "PureFW.GameMap" +
				".MammunSectorZoom.instances["	+this.instance_num+"]";
		
		this.id = "PureFW.GameMap.MammunSectorZoom"+this.instance_num;
		this.content_id 
			= "PureFW.GameMap.MammunSectorZoom_cont"+this.instance_num;
		this.bg_img_id = 'PureFW.GameMap.MammunSectorZoom_bg'+this.instance_num;
		
		this.insert_into_dom_tree();
		this.constructor.num_instances++;
		this.constructor.instances[this.instance_num] = this;
		
		if (!this.no_scale)
			PureFW.WidgetManager.manager_all.register_widget(this);
	}
};
/********************************************************************
 * PureFW.GameMap.MammunSectorZoom extends Container
 ********************************************************************/
PureFW.GameMap.MammunSectorZoom.extend(PureFW.GameMap.BuildingsField);

/********************************************************************
 * Statische Deklarationen
 ********************************************************************/
PureFW.GameMap.MammunSectorZoom.num_instances = 0;		// Instanzüberwachung (anzahl)
PureFW.GameMap.MammunSectorZoom.instances = new Array();// Instanzüberwachung (Instanzen)

/********************************************************************
 * Prototyp-Deklarationen
 ********************************************************************/
PureFW.GameMap.MammunSectorZoom.prototype.init = function(parent_node, x, y, 
	w, h, num_fields_x, num_fields_y, no_scale)
{
	PureFW.GameMap.MammunSectorZoom.parent.init.call(this, parent_node, x, y, 
		w, h, num_fields_x, num_fields_y, no_scale);
	
	this.unit_height = this.unit_width = 80;

	this.unit_layers = [null, null, null, null];
	this.units = [[],[],[],[]];	// Ein Array pro Kante
	this.unit_widgets = new Array();
	
	this.dominator = null;
	
	this.building_placement_click_layer = null;
	this.building_click_layer = null;
	this.unit_click_fields = new Array();
	this.building_placement_grid_map = null;
	this.building_grid_bg = null;
	
	this.placed_buildings = new Array();
	for (var x = 0; x < this.num_fields_x; x++) {
		this.placed_buildings[x] = new Array();
		for (var y = 0; y < this.num_fields_y; y++)
			this.placed_buildings[x][y] = false;
	}
	
	this.on_unit_click_functions = new Array();
	
	this.fog_img = null;
	this.fight = false;
};

PureFW.GameMap.MammunSectorZoom.prototype.insert_into_dom_tree = function() {
	PureFW.GameMap.MammunSectorZoom.parent.insert_into_dom_tree.call(this);
	
	this.unit_layers[0] = new PureFW.Container(
		this,
		this.width >> 1, this.height >> 1,
		this.width >> 1, this.height >> 1,
		this.no_scale
	);
	this.unit_layers[0].get_node().style.overflow = 'visible';
	this.unit_layers[1] = new PureFW.Container(
		this,
		0, 0,
		this.width >> 1, this.height >> 1,
		this.no_scale
	);
	this.unit_layers[1].get_node().style.overflow = 'visible';
	this.unit_layers[2] = new PureFW.Container(
		this,
		0, this.height >> 1,
		this.width >> 1, this.height >> 1,
		this.no_scale
	);
	this.unit_layers[2].get_node().style.overflow = 'visible';
	this.unit_layers[3] = new PureFW.Container(
		this,
		this.width >> 1, 0,
		this.width >> 1, this.height >> 1,
		this.no_scale
	);
	this.unit_layers[3].get_node().style.overflow = 'visible';
	
	
	this.building_click_layer = new PureFW.IsoMap.ClickMap3D(
		this,
		this.width * (1-this.b_map_scale) >> 1,
		this.height * (1-this.b_map_scale) >> 1,
		this.width * this.b_map_scale,
		this.height * this.b_map_scale,
		this.num_fields_x,
		this.num_fields_y,
		this.no_scale
	);
	
	this.building_grid_bg = new PureFW.Image(
		this,
		this.width * (1-this.b_map_scale) >> 1,
		this.height * (1-this.b_map_scale) >> 1,
		this.width * this.b_map_scale,
		this.height * this.b_map_scale,
		'ui/backgrounds/sector_details/grid.png',
		this.no_scale
	);
	
	this.building_placement_click_layer = new PureFW.IsoMap.ClickMap(
		this,
		this.width * (1-this.b_map_scale) >> 1,
		this.height * (1-this.b_map_scale) >> 1,
		this.width * this.b_map_scale,
		this.height * this.b_map_scale,
		this.num_fields_x,
		this.num_fields_y,
		this.no_scale
	);

	for (var x = 0; x < this.num_fields_x; x++)
		for (var y = 0; y < this.num_fields_y; y++)
			this.building_placement_click_layer.create_field(x,y);
	this.building_placement_click_layer.refresh();
	
	this.building_placement_grid_map = new PureFW.IsoMap.VisMap(
		this,
		this.width * (1-this.b_map_scale) >> 1,
		this.height * (1-this.b_map_scale) >> 1,
		this.width * this.b_map_scale,
		this.height * this.b_map_scale,
		this.num_fields_x,
		this.num_fields_y,
		this.no_scale
	);
	
	this.building_grid_bg.set_z_index(0);
	this.building_placement_grid_map.set_z_index(1);
	this.unit_layers[3].set_z_index(2);
	this.unit_layers[1].set_z_index(3);
	this.building_map.set_z_index(4);
	this.unit_layers[0].set_z_index(5);
	this.unit_layers[2].set_z_index(6);
	// nebel-Ebene: z-index: 7 [wird on demand später erzeugt]
	this.building_click_layer.set_z_index(8);
	// unit_click_layer: z_index: 9	 [Aus Browser-Bug-Gründen kein Layer]
	this.building_placement_click_layer.set_z_index(10);
	
	
	this.hide_construction_grid();
}

/**
 * Setzt den Dominator des Sektors
 * 
 * @param String dom
 * @return
 */
PureFW.GameMap.MammunSectorZoom.prototype.set_dominator = function(dom) {
	this.dominator = dom;
};

/**
 * Fügt eine stationäre Einheit der Sektordarstellung hinzu.
 * 
 * @param unit Object 
 * @param unit edge
 */
PureFW.GameMap.MammunSectorZoom.prototype.add_unit = function(unit, edge) 
{
	if (!this.dominator)
		throw new Error("Dominator has to be set before calling add_unit!");
	if ((edge > 3) || (edge < 0))
		edge = 0;
	this.units[edge].push(unit);
};

/**
 * Entfernt alle Einheiten von der angegebenen Kante.
 * 
 * Ist <code>edge</code> <code>-1</code>(default), dann werden alle Einheiten 
 * aller Kanten entfernt.
 * 
 * @param uint edge
 */
PureFW.GameMap.MammunSectorZoom.prototype.clear_units = function(edge) {
	if (typeof(edge) == 'undefined')
		edge = -1;
	if (edge < 0) {
		for (var i = 0; i < 4; i++)
			this.clear_units(i);
	}
	else {
		if (edge > 3)
			edge = 0;
		this.units[edge].destroy();
		this.units[edge] = new Array();
	}
};

/**
 * Gibt alle Einheitendaten sortiert nach Kante zurück, die vorher hinzugefügt 
 * wurden.
 * 
 * @return Object
 */
PureFW.GameMap.MammunSectorZoom.prototype.get_unit_datas = function() {
	return this.units;
};

/**
 * Gibt alle Einheitenwidgets zurück
 * 
 * @return Person
 */
PureFW.GameMap.MammunSectorZoom.prototype.get_unit_widgets = function() {
	return this.unit_widgets;
};



/**
 * Aktuallisiert die Anzeige des Widgets. 
 * 
 * Muss nach jeder Änderung aufgerufen werden. Das Explizite Aufrufen von
 * refresh erhöht die Performance spürbar, weswegen kein automatischer Refresh
 * durchgeführt wird!
 */
PureFW.GameMap.MammunSectorZoom.prototype.refresh = function() {
	this._create_unit_click_areas();
	this.building_click_layer.refresh();
	
	var field;
	for (var x = 0; x < this.num_fields_x; x++) {
		for (var y = 0; y < this.num_fields_y; y++) {
			field = this.building_placement_grid_map.get_field(x,y);
			/**
			 * Wenn ein Gebäude an der Stelle platziert ist, dann ein rotes
			 * Feld
			 */
			if (this.placed_buildings[x] && this.placed_buildings[x][y]) {
				if (!field) {
					field = this.building_placement_grid_map.create_field(
						x, y,
						PureFW.Image, 
						'../pix/ui/backgrounds/sector_details/red.png',
						this.no_scale);
				}
				else {
					field.set_pic_url(
						'../pix/ui/backgrounds/sector_details/red.png'
					);
				}
				field.hide();
			}
			/**
			 * sonst ein weißes
			 */
			else {
				if (!field) {
					field = this.building_placement_grid_map.create_field(
						x, y,
						PureFW.Image, 
						'../pix/ui/backgrounds/sector_details/white.png',
						this.no_scale);
				}
				else {
					field.set_pic_url(
						'../pix/ui/backgrounds/sector_details/white.png'
					);
				}
				field.hide();
			}
		}
	}
}
/**
 * Fügt das angegebene Gebäude dem Sektor hinzu.
 * 
 * @param Object b_obj
 * @see PureFW.GameMap.BuildingsField#clear_buildings
 */
PureFW.GameMap.MammunSectorZoom.prototype.add_building = function(b_obj) {
	PureFW.GameMap.MammunSectorZoom.parent.add_building.call(this, b_obj);
	
	var b_x = parseInt(b_obj.ulc[0]);
	var b_y = parseInt(b_obj.ulc[1]);

	object3d = this.building_click_layer.get_object(b_x, b_y);
	/**
	 * Die Höhe der Klickfläche des Gebäudes soll bei großen Grundflächen
	 * auf die Grundfläche reduziert werden. Also die Klickfläche soll keine
	 * 3D-Höhe haben, height ist somit 1 (weil height ein Faktor ist).
	 * Bei kleineren Gebäuden ist die Verdeckungsgefahr wesentlich geringer,
	 * weswegen man hier mit der kompletten Höhe arbeiten kann.
	 * 
	 * Da gibt es noch genug zu tunen, weil die Regel wirkt noch etwas
	 * willkürlich.
	 */
	var height = (b_obj.size > 2) ? 1 : b_obj.height;
	if (!object3d) {
		object3d = this.building_click_layer.create_object(
			b_x, 
			b_y, 
			b_obj.size, 
			b_obj.size,
			height,
			b_obj.name 
		);
	}
	else {
		object3d.size.x = b_obj.size;
		object3d.size.y = b_obj.size;
		object3d.proj_h = height;
		object3d.name = b_obj.name;
		object3d.pos.x = b_x;
		object3d.pos.y = b_y;
	}
	var size = parseInt(b_obj.size);
	for (var i = b_x; i < b_x + size; i++) {
		for (var j = b_y; j < b_y + size; j++)
			this.placed_buildings[i][j] = true;
	}
}
/**
 * Entfernt alle Gebäude.
 * 
 * @see PureFW.GameMap.BuildingsField#clear_buildings
 */
PureFW.GameMap.MammunSectorZoom.prototype.clear_buildings = function() {
	PureFW.GameMap.MammunSectorZoom.parent.clear_buildings.call(this);
	for (var x = 0; x < this.num_fields_x; x++)
		for (var y = 0; y < this.num_fields_y; y++)
			this.building_click_layer.destroy_object(x,y);
}

/**
 * Blendet den Nebel ein oder aus.
 * 
 * @param String pic
 */
PureFW.GameMap.MammunSectorZoom.prototype.set_fog = function(pic) {
	if (pic) {
		if (!this.fog_img) {
			this.fog_img = this.add_widget(
				PureFW.Image,
				0, 0,
				this.width, this.height,
				pic
			);
			this.fog_img.set_z_index(7);
		}
		else {
			this.fog_img.set_pic_url(pic);
		}
	}
	else {
		this.fog_img.destroy();
		this.fog_img = null;
	}
}

/**
 * Hierüber kann angegeben werden, ob im Sektor gerade gekämpft wird, was die
 * Darstellung beeinflusst.
 * 
 * @param bool f
 */
PureFW.GameMap.MammunSectorZoom.prototype.set_fight = function(f) {
	this.fight = f;
}



PureFW.GameMap.MammunSectorZoom.prototype._create_unit_click_areas = function()
{
	this.unit_widgets.destroy();
	this.unit_widgets = new Array();
	var unit_widgets_per_edge = [[],[],[],[]];
	this.unit_click_fields.destroy();
	this.unit_click_fields = new Array();

	var pos, x, y, unit, pose;
	var attackers_per_edge = [ {}, {}, {}, {} ];
	var defenders_per_edge = [ {}, {}, {}, {} ];
	var attack_edges = [];
	var defend_edges = [];
	var att_edge_num = [];
	
	/**
	 * Der Algorithmus verläuft in mehreren Stufen.
	 * 
	 * Zuerst werden die Daten gesammelt, welche Partei wie viele Mammuns
	 * auf welcher Seite hat.
	 * 
	 * Dann werden die Mammuns ggf. umverteilt. Gründe kann ein Mehrparteien
	 * oder ein Mehrfrontenkampf sein mit wenig Gegenwehr.
	 * 
	 * Und dann werden am Ende die Mammun platziert - erst die unterlegene
	 * Partei, dann die überlegene.
	 */
	var max_att_edge = [-1, 0];
	for (var edge = 0; edge < 4; edge++) {
		var n = this.units[edge].length;
		var att_n = 0;
			
		for (var i = 0; i < n; i++) {
			var nick = this.units[edge][i].user_nick;
			
			if (this.units[edge][i].user_nick == this.dominator) {
				/** TODO: or is allied */
				if (!defenders_per_edge[edge][nick])
					defenders_per_edge[edge][nick] = new Array();
				defenders_per_edge[edge][nick].push(this.units[edge][i]);
				if (defend_edges.search(edge) == -1)
					defend_edges.push(edge);
			}
			else if (this.units[edge][i].user_nick != '-') {
				if (!attackers_per_edge[edge][nick])
					attackers_per_edge[edge][nick] = new Array();
				attackers_per_edge[edge][nick].push(this.units[edge][i]);
				if (attack_edges.search(edge) == -1)
					attack_edges.push(edge);
				att_n++;
			}
		}
		if (att_n > max_att_edge[1]) {
			max_att_edge[0] = edge;
			max_att_edge[1] = att_n;
		}
		att_edge_num[edge] = att_n;
	}
	
	if (attack_edges.length > defend_edges.length) {
		// TODO: Angreifer umverteilen auf die anderen Kanten, damit
		// keiner gegen die Luft kämpft
	}
	
	// Jetzt die Angreifer auf die Kanten besser verteilen, nicht strikt nach
	// Herkunft. Verteidiger sind bereits perfekt ausgeglichen vom Server
	var edges = [ max_att_edge[0], max_att_edge[0]^1 ];
	for (var ei = 0; ei < edges.length; ei++) {
		var edge = edges[ei];
		if (att_edge_num[edge] == 0)
			continue;
		var edge_v = edge^2;
		var neighbour_edges = (att_edge_num[edge_v] < att_edge_num[edge_v^1])
			? [ edge_v, edge_v^1 ]
			: [ edge_v^1, edge_v ];
		
		for (var ni = 0; ni < neighbour_edges.length; ni++) {
			if (att_edge_num[neighbour_edges[ni]] == 0)
				continue;
			var diff_2 
				= (att_edge_num[neighbour_edges[ni]] - att_edge_num[edge]) >> 1;
	
			
			for (var nick in attackers_per_edge[edge]) {
				var un = Math.abs(diff_2) * (attackers_per_edge[edge][nick].length 
													/ att_edge_num[edge]);
				
				for (var ui = 0; ui < un; ui++) {
					if (diff_2 < 0) {
						if (!attackers_per_edge[neighbour_edges[ni]][nick])
							attackers_per_edge[neighbour_edges[ni]][nick] 
																= new Array();
						attackers_per_edge[neighbour_edges[ni]][nick].push(
							attackers_per_edge[edge][nick].pop()
						);
					}
					else {
						if (!attackers_per_edge[edge][nick])
							attackers_per_edge[edge][nick] = new Array();
						attackers_per_edge[edge][nick].push(
							attackers_per_edge[neighbour_edges[ni]][nick].pop()
						);
					}
				}
			}
		}
	}
	
	var parties = null;
	for (var edge = 0; edge < 4; edge++) {
//		//edge = (edge < 2) ? edge : edge ^ 1;
//		// first count all defenders and attackers
		var def_n = 0;
		var att_n = 0;
		if (parties) {
			destroy_object(parties);
			parties = null;
		}
		parties = [[]];

		for (var nick in defenders_per_edge[edge]) {
			var n = defenders_per_edge[edge][nick].length;
			for (var i = 0; i < n; i++) {
				parties[0].push(defenders_per_edge[edge][nick][i]);
				def_n++;
			}
		}
		var k = 1;
		for (var nick in attackers_per_edge[edge]) {
			if (typeof(parties[k]) == 'undefined')
				parties[k] = [];
			var n = attackers_per_edge[edge][nick].length;
			for (var i = 0; i < n; i++) {
				parties[k].push(attackers_per_edge[edge][nick][i]);
				att_n++;
			}
			k++;
		}
		
		/**
		 * Zunächst alle Mammuns der Partei mit weniger Mammuns setzen. Dann
		 * kommt die anderen Partei und gruppiert sich um die erstere.
		 */
		if (((att_n > 0) && (att_n < def_n)) || (def_n <= 0)) {
			var tmp = parties[0];
			for (var i = 1; i < parties.length; i++) {
				parties[i-1] = parties[i];
			}
			parties[parties.length-1] = tmp;
		}
		var i_att = 0;
		var i_def = 0;
		var partner = null;
		var m = parties.length;
		var defending = false;
		for (var i = 0; i < m; i++) {
			var n = parties[i].length;
			for (var j = 0; j < n; j++) {
				unit = parties[i][j];
				if (i == 0) {
					if (unit.user_nick == this.dominator) {
						defending = true;
						pos = this._get_unit_position_def(
							edge, i_def, def_n
						);
						i_def++;
						pose = edge;
						
						if (this.fight) {
							pos.x += 
								(( MapObjects.Animal.WALK_DIRECTIONS[pose].x
								* MapObjects.Animal.PERPSECTIVE.x) >> 1);
							pos.y += 
								(( MapObjects.Animal.WALK_DIRECTIONS[pose].y
								* MapObjects.Animal.PERPSECTIVE.y) >> 1);
						}
					}
					else {
						defending = false;
						
						if (this.dominator == '-') {
							pose = Math.floor(Math.random()*4);
							pos = this._get_unit_position_dunkel(
								edge, i_att, att_n
							);
						}
						else {
							pose = edge ^ 1;
							pos = this._get_unit_position_att(
								edge, i_att, att_n
							);
							if (this.fight) {
								pos.x += 
									(( MapObjects.Animal.WALK_DIRECTIONS[pose].x
									* MapObjects.Animal.PERPSECTIVE.x) >> 1);
								pos.y += 
									(( MapObjects.Animal.WALK_DIRECTIONS[pose].y
									* MapObjects.Animal.PERPSECTIVE.y) >> 1);
							}
						}
						i_att++;
					}
					pos.x = pos.x-15 + Math.floor(Math.random()*15); 
					pos.y = pos.y + Math.floor(Math.random()*15);
				}
				else {
					// cur_n: Anzahl aller Mammuns bisherig angezeigten Parteien
					var cur_n = 0;
					for (var k = 0; k < i; k++)
						cur_n += parties[k].length;
					
					var rand = MathExt.Random.halton(i*n+j, 3
						//((edge | 1) == edge) ? 3 : 5
					);
					var index = Math.floor(cur_n*rand);
					partner = unit_widgets_per_edge[edge][index];
					if (partner) {
						
						// Kante suchen, wo der Partner am wenigsten Partner hat
						var edge_order = [];
						edge_order[0] = (defending) ? edge^1 : edge;
						edge_order[1] = edge_order[0]^2;
						edge_order[2] = edge_order[1]^1;
						edge_order[3] = edge_order[0]^1;
						var min_edge = [-1, Infinity];
						for (var eoi = 0; eoi < 4; eoi++) {
							edge2 = edge_order[eoi];
							var partners_partners 
								= partner.get_interaction_partners(edge2);
							
							//	partners_partners.length);
							if (partners_partners.length < min_edge[1]) {
								min_edge[0] = edge2;
								min_edge[1] = partners_partners.length;
							}
						}
	
						edge2 = min_edge[0];
						
						// edge2 ist die freie Seite des Partners, ergo ist "meine"
						// freie Seite edge2^1
						
						// Alle Partner an selber Seite verschieben, um Platz zu
						// schaffen
						var partners_partners 
							= partner.get_interaction_partners(edge2);
						
						var edge_v = edge2^2;	// vertical
						var pn = partners_partners.length;
						for (var pi = 0; pi < pn; pi++) {
							partners_partners[pi].set_position(
								partners_partners[pi].position.x
									- (( MapObjects.Animal.WALK_DIRECTIONS[edge_v].x
										* MapObjects.Animal.PERPSECTIVE.x) >> 3),
								partners_partners[pi].position.y
									- (( MapObjects.Animal.WALK_DIRECTIONS[edge_v].y
										* MapObjects.Animal.PERPSECTIVE.y) >> 3)
							);
						}
						
	
						// "Meine" Position, inklusive
						pos = new PureFW.Point(
							partner.position.x
								+ (( MapObjects.Animal.WALK_DIRECTIONS[edge2^1].x
									* MapObjects.Animal.PERPSECTIVE.x) >> 1)
								+ ((( MapObjects.Animal.WALK_DIRECTIONS[edge_v].x
									* MapObjects.Animal.PERPSECTIVE.x) >> 3) 
									* pn ),
							partner.position.y
								+ (( MapObjects.Animal.WALK_DIRECTIONS[edge2^1].y
									* MapObjects.Animal.PERPSECTIVE.y) >> 1)
								+ ((( MapObjects.Animal.WALK_DIRECTIONS[edge_v].y
									* MapObjects.Animal.PERPSECTIVE.y) >> 3) 
									* pn )
						);
					}
					else {	// if (!partner)
						edge2 = 0;
						pos = new PureFW.Point(0,0);
					}
					pose = edge2;
					//pose = (edge2 < 2) ? edge2 ^ 1: edge2;
				}
				
				x = pos.x;
				y = pos.y;
				
				var person = this.unit_layers[edge].add_widget(
					MapObjects.Mammun,
					x, y,
					this.unit_width, this.unit_height,
					MapObjects.Mammun.get_mammun_by_type_and_sex(unit.race),
					unit.clothes,
					MapDataCtrl.player_colors[unit.user_nick],
					this.no_scale
				);
				person.click_div.set_parent_node(this.get_node());
				person.click_div.set_width(50);
				person.click_div.set_height(50);
				if (edge == 0) {
					person.set_click_div_offset(
						this.width >> 1, this.height >> 1
					);
				}
				else if (edge == 2) {
					person.set_click_div_offset(
						0, this.height >> 1
					);
				}
				else if (edge == 3) {
					person.set_click_div_offset(
						this.width >> 1, 0
					);
				}
				person.set_hp(unit.amount);
				person.is_my_unit = (unit.user_nick == this.dominator);
				if (this.fight) {
					person._set_pose(pose);
					person.fight();
				}
				else {
					person._set_pose(Math.floor(Math.random()*4));
					person.set_walk_region(x-15, y, 15, 15);
				}
				person.set_z_index(Math.floor(y/5));
				
				unit.pos = new PureFW.Point(
					x + person.click_div_offset.x, 
					y + person.click_div_offset.y
				);
				person.add_event_handler("click", 
					(function(_instance, _unit) {
						return function(ev) {
							_instance.on_unit_click(
								_instance.create_event("unit_click", _unit));
						}
					})(this, unit)
				);
				
				this.unit_widgets.push(person);
				unit_widgets_per_edge[edge].push(person);
				
				if (partner) {
					MapObjects.Animal.link_interaction_partners(
						edge2, partner, person
					);
				}
			}
		}
	
//		var i_att = 0;
//		var i_def = 0;
//	
//		for (var i = 0; i < n; i++) {
//			unit = this.units[edge][i];
//			if (unit.user_nick == this.dominator) {
//				pos = this._get_unit_position_def(
//					edge, i_def, def_n
//				);
//				i_def++;
//				pose = (edge < 2) ? edge : edge ^ 1;
//			}
//			else {
//				if (this.dominator == '-') {
//					pos = this._get_unit_position_dunkel(
//						edge, i, n
//					);
//				}
//				else {
//					pos = this._get_unit_position_att(
//						edge, i_att, att_n
//					);
//					i_att++;
//				}
//				pose = (edge > 1) ? edge : edge ^ 1;
//			}
//			x = pos.x;
//			y = pos.y;
//			var c = Math.floor(2.5 * edge);
//			this.unit_layers[edge].set_bg_color(
//				'#'+c+''+c+''+c
//			);
//			var person = this.unit_layers[edge].add_widget(
//				MapObjects.Mammun,
//				x, y,
//				this.unit_width, this.unit_height,
//				MapObjects.Mammun.get_mammun_by_type_and_sex(unit.race),
//				//'map/units/'+unit.pic_url.replace('.png', '_'+pose+'.png'),
//				[0,0,0,0,0,0],//unit.clothes,
//				MapDataCtrl.player_colors[unit.user_nick],
//				this.no_scale
//			);
//			person.set_hp(unit.amount);
//			person.is_my_unit = (unit.user_nick == this.dominator);
//			if (this.fight) {
//				person._set_pose(pose);
//				person.fight();
//			}
//			else {
//				person._set_pose(Math.floor(Math.random()*4));
//				person.set_walk_region(x, y, 10, 10);
//			}
//			person.set_z_index(Math.floor(y/10));	
//			
//			this.unit_widgets.push(person);
//			/*
//			if (person.is_my_unit && (unit.amount > 600) && !this.fight) {
//				var tmp = this.unit_layers[edge].add_widget(
//					PureFW.FilmAnimator,
//					x + ((this.unit_width - 50) >> 1), y - 50,
//					50, 50
//				);
//				tmp.set_image(
//					'ui/animations/zzz/50/zzz.png', 4
//				);
//				tmp.set_frame_delay(200)
//				tmp.start_animation();
//			}*/
//			// TODO
//			/*if (unit.camouflaged)
//				person.set_opacity(0.5);*/
//			
//			if (edge == 0) {
//				x += this.width >> 1;
//				y += this.height >> 1;
//			}
//			else if (edge == 3) {
//				y += this.height >> 1;
//			}
//			else if (edge == 2) {
//				x += this.width >> 1;
//			}
//				/*
//			var tmp = new PureFW.Image(
//				this,
//				x, y,
//				this.unit_width,
//				this.unit_height,
//				'pattern/spacer.gif',
//				this.no_scale
//			);
//			tmp.set_z_index(9);
//			if (unit.hp) {
//				tmp.set_tooltip(unit.name + '(HP: '
//					+ Math.ceil(unit.amount/1000*unit.hp)
//					+ '/' + unit.hp + ')');
//			}
//			else {
//				tmp.set_tooltip(unit.name);
//			}
//			unit.pos = new PureFW.Point(x, y);
//			*/
//			person.add_event_handler("click", 
//				(function(_instance, _unit) {
//					return function(ev) {
//						_instance.on_unit_click(
//							_instance.create_event("unit_click", _unit));
//					}
//				})(this, unit)
//			);
//			/*
//			tmp.add_event_handler("mouseover",
//				(function(_person) {
//					return function(ev) {
//						_person.on_mouseover();
//					}
//				})(person));
//			tmp.add_event_handler("mouseout",
//				(function(_person) {
//					return function(ev) {
//						_person.on_mouseout();
//					}
//				})(person));
//			this.unit_click_fields.push(tmp);*/
//		}
	}
};

/**
 * Gibt die x- und y-Position, die eine Einheit mit angegebenener Kante hat, 
 * wenn sie den Sektor verteidigt.
 * amt gibt dabei an, wie viele Einheiten an dieser Kante ausgerichtet werden
 * (loop-max) und i ist der Inkrement der Schleife.
 *
 * @param {0,..,3} edge
 * @param uint i
 * @param uint amt
 * @return { x:uint, y:uint }
 */
PureFW.GameMap.MammunSectorZoom.prototype._get_unit_position_def = function(
	edge, i, amt) 
{
	var result = new PureFW.Point();
	i++; amt++;
	switch (edge) {
		case 0:
			result.x = i*(this.width>>1)/amt - (this.width>>4);
			result.y = (this.height>>1) - i*(this.height>>1)/amt + 16
				- (this.height>>4);
			break;			
		case 1:
			result.x = i*(this.width>>1)/amt;
			result.y = (this.height>>1) - i*(this.height>>1)/amt + 16;
			break;
		case 2:
			result.x = i*(this.width>>1)/amt;
			result.y = i*(this.height>>1)/amt;
			break;			
		case 3:
			result.x = i*(this.width>>1)/amt - (this.width>>4);
			result.y = i*(this.height>>1)/amt + (this.height>>4);
			break;
		default:
			result.x = result.y = 0;
	};
	result.y -= this.unit_height;
	
	return result;
};


/**
 * Gibt die x- und y-Position, die eine Einheit mit angegebenener Kante hat, 
 * wenn sie den Sektor angreift.
 * amt gibt dabei an, wie viele Einheiten an dieser Kante ausgerichtet werden
 * (loop-max) und i ist der Inkrement der Schleife.
 *
 * @param {0,..,3} edge
 * @param uint i
 * @param uint amt
 * @return { x:uint, y:uint}
 */
PureFW.GameMap.MammunSectorZoom.prototype._get_unit_position_att = function(
	edge, i, amt) 
{
	i++; amt++;
	var result = new PureFW.Point();
	switch (edge) {
		case 0:
			result.x = i*(this.width>>1)/amt;
			result.y = (this.height>>1) - i*(this.height>>1)/amt + 16;
			break;
		case 1:
			result.x = i*(this.width>>1)/amt - (this.width>>4);
			result.y = (this.height>>1) - i*(this.height>>1)/amt + 16
				- (this.height>>4);
			break;
		case 2:
			result.x = i*(this.width>>1)/amt - (this.width>>4);
			result.y = i*(this.height>>1)/amt + (this.height>>4);
			break;
		case 3:
			result.x = i*(this.width>>1)/amt;
			result.y = i*(this.height>>1)/amt;
			break;
		default:
			result.x = result.y = 0;
	};
	result.y -= this.unit_height;
	return result;
}

/**
 * Gibt die x- und y-Position, die eine Einheit mit angegebenener Kante hat, 
 * wenn sie im Sektor des Dunkels ist (egal ob Angriff oder Verteidigung, da 
 * das Dunkel den ganzen Sektor als Nebel einnimmt).
 * amt gibt dabei an, wie viele Einheiten an dieser Kante ausgerichtet werden
 * (loop-max) und i ist der Inkrement der Schleife.
 *
 * @param {0,..,3} edge
 * @param uint i
 * @param uint amt
 * @return {x:uint, y:uint}
 */
PureFW.GameMap.MammunSectorZoom.prototype._get_unit_position_dunkel = function(
	edge, i, amt) 
{
	i++; amt++;
	var result = new PureFW.Point();
	switch (edge) {
		case 0:
			result.x = MathExt.Random.halton(i, 3) * (this.width>>2);
			result.y = MathExt.Random.halton(i, 5) * (this.height>>2);
			break;
		case 1:
			result.x = MathExt.Random.halton(i, 3) * (this.width>>2)
				+ (this.width>>2);
			result.y = MathExt.Random.halton(i, 5) * (this.height>>2)
				+ (this.height>>2);
			break;
		case 2:
			result.x = MathExt.Random.halton(i, 3) * (this.width>>2)
				+ (this.width>>2);
			result.y = MathExt.Random.halton(i, 5) * (this.height>>2);
			break;
		case 3:
			result.x = MathExt.Random.halton(i, 3) * (this.width>>2);
			result.y = MathExt.Random.halton(i, 5) * (this.height>>2)
				+ (this.height>>2);
			break;
		default:
			result.x = result.y = 0;
	};
	result.y -= this.unit_height;
	return result;
};


/**
 * Hinzufügen eines Events.
 * 
 * Erweitert um "unit_click", das beim Anklicken einer Einheit ausgelöst wird.
 * 
 * @param string type
 * @param Funktion fn
 * @see PureFW.Widget#add_event_handler
 */
PureFW.GameMap.MammunSectorZoom.prototype.add_event_handler = function(
	type, fn) 
{
	if (typeof (type) !== 'string')
		throw new Error("PureFW.GameMap.MammunSectorZoom::add_event_handler: "+
				"First argument 'type' has to be of type 'string'");
	if (typeof (fn) !== 'function')
		throw new Error("PureFW.GameMap.MammunSectorZoom::add_event_handler: "+
				"Second argument 'fn' has to be of type 'function'");
	if (type == "unit_click") {
		this.on_unit_click_functions.push(fn);
	}
	else if (type == "building_click") {
		this.building_click_layer.add_event_handler("object_click", fn);
	}
	else if (type == "building_mouseover") {
		this.building_click_layer.add_event_handler("object_mouseover", fn);
	}
	else if (type == "building_mouseout") {
		this.building_click_layer.add_event_handler("object_mouseout", fn);
	}
	else if (type == "placement_grid_mouseover") {
		this.building_placement_click_layer.add_event_handler(
			"field_mouseover", fn);
	}
	else if (type == "placement_grid_mouseout") {
		this.building_placement_click_layer.add_event_handler(
			"field_mouseout", fn);
	}
	else if (type == "placement_grid_click") {
		this.building_placement_click_layer.add_event_handler("field_click", fn);
	}
	else {
		PureFW.GameMap.MammunSectorZoom.parent.add_event_handler.call(
			this, type, fn);
	}
};

/**
 * Entfernen eines Events.
 * 
 * Erweitert um "unit_click", das beim Anklicken einer Einheit ausgelöst wird.
 * 
 * @param string type
 * @param Funktion fn
 * @see PureFW.Widget#remove_event_handler
 */
PureFW.GameMap.MammunSectorZoom.prototype.remove_event_handler = function(
	type, fn) 
{
	if (typeof (type) !== 'string')
		throw new Error("PureFW.GameMap.MammunSectorZoom::remove_event_handler: "+
				"First argument 'type' has to be of type 'string'");
	if ((fn) && (typeof (fn) !== 'function'))
		throw new Error("PureFW.GameMap.MammunSectorZoom::remove_event_handler: "+
		"Second argument 'fn' has to be of type 'function'");
	
	if (type == "unit_click") {
		if (fn)
			this.on_unit_click_functions.remove(fn);
		else {
			this.on_unit_click_functions.destroy();
			this.on_unit_click_functions = new Array();
		}
	}
	else if (type == "building_click") {
		this.building_click_layer.remove_event_handler("object_click", fn);
	}
	else if (type == "building_mouseover") {
		this.building_click_layer.remove_event_handler("object_mouseover", fn);
	}
	else if (type == "building_mouseout") {
		this.building_click_layer.remove_event_handler("object_mouseout", fn);
	}
	else if (type == "placement_grid_mouseover") {
		this.building_placement_click_layer.remove_event_handler(
			"field_mouseover", fn);
	}
	else if (type == "placement_grid_mouseout") {
		this.building_placement_click_layer.remove_event_handler(
			"field_mouseout", fn);
	}
	else if (type == "placement_grid_click") {
		this.building_placement_click_layer.remove_event_handler(
			"field_click", fn);
	}
	else {
		PureFW.GameMap.MammunSectorZoom.parent.remove_event_handler.call(this, 
			type, fn);
	}
};

/**
 * Löst das "unit_click"-Event explizit aus.
 * 
 * Wir normalerweise beim Anklicken eine Einheit ausgelöst.
 */
PureFW.GameMap.MammunSectorZoom.prototype.on_unit_click = function(ev) {
	var n = this.on_unit_click_functions.length;
	for (var i = 0; i < n; i++)
		this.on_unit_click_functions[i].call(this, ev);
}

/**
 * Verbirgt das Grid zum Platzieren von Gebäuden, um diese zu errichten.
 */
PureFW.GameMap.MammunSectorZoom.prototype.hide_construction_grid = function() {
	this.building_placement_click_layer.hide();
	this.building_grid_bg.hide();
	this.building_placement_grid_map.hide();
}
/**
 * Zeigt das Grid zum Platzieren von Gebäuden, um diese zu errichten, an.
 */
PureFW.GameMap.MammunSectorZoom.prototype.show_construction_grid = function() {
	this.building_placement_click_layer.show();
	this.building_grid_bg.show();
	this.building_placement_grid_map.show();
}

/**
 * Gibt ein 2D-Array zurück, das nicht unbedingt voll besetzt sein muss, und
 * an den Stellen TRUE ist, wo ein Gebäude einen Platz belegt.
 * 
 * @return bool[][]
 */
PureFW.GameMap.MammunSectorZoom.prototype.get_placed_buildings = function() {
	return this.placed_buildings;
}

/**
 * Gibt eine VisMap zurück, die rote und weiße Felder enthält, je nachdem,
 * ob an der Stelle ein Gebäude steht, oder nicht.
 * 
 * Sie ist teil der Construction-Grid-Geschichte und wird somit von den
 * Funktionen <code>show_construction_grid</code> und 
 * <code>hide_construction_grid</code> mitbeeinflusst.l
 * 
 * @return PureFW.IsoMap.VisMap
 */
PureFW.GameMap.MammunSectorZoom.prototype.get_building_placement_grid_map = 
	function()
{
	return this.building_placement_grid_map;
}

/**
 * @see PureFW.Container#destroy
 */
PureFW.GameMap.MammunSectorZoom.prototype.destroy = function() {
	PureFW.GameMap.MammunSectorZoom.parent.destroy.call(this);
	this.unit_widgets.destroy();
	this.units.destroy();
	this.unit_layers.destroy();
}

