/**
 * Dieses Widget repräsentiert einen Sektor der MammunMap, der Gebäude enthält.
 * 
 * @author Phil
 */

if (typeof(PureFW.GameMap) != 'object')
	PureFW.GameMap = new Object();

/**
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
PureFW.GameMap.BuildingsField = function(parent_node, x, y, w, h, 
	num_fields_x, num_fields_y, no_scale)
{
	if (typeof(parent_node) !== 'undefined') {
		this.init(parent_node, x, y, w, h, num_fields_x, num_fields_y, no_scale);
		
		this.instance_num = this.constructor.num_instances;
		this.instance_name = "PureFW.GameMap" +
				".BuildingsField.instances["	+this.instance_num+"]";
		
		this.id = "PureFW.GameMap.BuildingsField"+this.instance_num;
		this.content_id 
			= "PureFW.GameMap.BuildingsField_cont"+this.instance_num;
		this.bg_img_id = 'PureFW.GameMap.BuildingsField_bg'+this.instance_num;
		
		this.insert_into_dom_tree();
		this.constructor.num_instances++;
		this.constructor.instances[this.instance_num] = this;
		
		if (!this.no_scale)
			PureFW.WidgetManager.manager_all.register_widget(this);
	}
};
/********************************************************************
 * PureFW.GameMap.BuildingsField extends Container
 ********************************************************************/
PureFW.GameMap.BuildingsField.extend(PureFW.Container);

/********************************************************************
 * Statische Deklarationen
 ********************************************************************/
PureFW.GameMap.BuildingsField.num_instances = 0;		// Instanzüberwachung (anzahl)
PureFW.GameMap.BuildingsField.instances = new Array();// Instanzüberwachung (Instanzen)



/********************************************************************
 * Prototyp-Deklarationen
 ********************************************************************/
PureFW.GameMap.BuildingsField.prototype.init = function(parent_node, x, y, 
	w, h, num_fields_x, num_fields_y, no_scale)
{
	PureFW.GameMap.BuildingsField.parent.init.call(this, parent_node, x, y, 
		w, h, no_scale);
	
	this.num_fields_x = num_fields_x;
	this.num_fields_y = num_fields_y;
	this.b_map_scale = 0.85;
	this.building_map = null;
};

PureFW.GameMap.BuildingsField.prototype.insert_into_dom_tree = function() {
	PureFW.GameMap.BuildingsField.parent.insert_into_dom_tree.call(this);
	
	this.get_node().style.overflow = 'visible';
	
	this.building_map = new PureFW.IsoMap.VisMap3D(
		this,
		this.width * (1-this.b_map_scale) >> 1,
		this.height * (1-this.b_map_scale) >> 1,
		this.width * this.b_map_scale,
		this.height * this.b_map_scale,
		this.num_fields_x,
		this.num_fields_y,
		this.no_scale
	);
}

/**
 * Fügt das angegebene Gebäude dem Sektor hinzu.
 * 
 * @param Object b_obj
 */
PureFW.GameMap.BuildingsField.prototype.add_building = function(b_obj) {
	var b_x = b_obj.ulc[0];
	var b_y = b_obj.ulc[1];
	
	object3d = this.building_map.get_object(b_x, b_y);
	if (!object3d) {
		object3d = this.building_map.create_object(
			b_x, 
			b_y, 
			b_obj.size, 
			b_obj.size,
			b_obj.height,
			PureFW.BuildingImage, b_obj, 
			this.no_scale
		);
		if (b_obj.xp_level > 0) {
			
		}
	}
	else {
		object3d.set_b_obj(b_obj);
	}
};

/**
 * Entfernt alle Gebäude.
 */
PureFW.GameMap.BuildingsField.prototype.clear_buildings = function() {
	for (var x = 0; x < this.num_fields_x; x++)
		for (var y = 0; y < this.num_fields_y; y++)
			this.building_map.destroy_object(x,y);
}

PureFW.GameMap.BuildingsField.prototype.get_field_width = function() {
	return this.building_map.get_field_width();
}

PureFW.GameMap.BuildingsField.prototype.get_field_height = function() {
	return this.building_map.get_field_height();
}

PureFW.GameMap.BuildingsField.prototype.get_num_fields_x = function() {
	return this.num_fields_x;
}

PureFW.GameMap.BuildingsField.prototype.get_num_fields_y = function() {
	return this.num_fields_y;
}

PureFW.GameMap.BuildingsField.prototype.get_building = function(x,y) {
	return this.building_map.get_object(x,y);
}

/**
 * @see PureFW.Container#destroy
 */
PureFW.GameMap.BuildingsField.prototype.destroy = function() {
	PureFW.GameMap.BuildingsField.parent.destroy.call(this);
	this.building_map.destroy();
}

