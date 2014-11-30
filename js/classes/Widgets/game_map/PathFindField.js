/**
 * Dieses Widget repräsentiert ein Sektoroverlay, um einen Schritt in einem
 * Pfad über die MammunMap darzustellen.
 * 
 * @author Phil
 */

if (typeof(PureFW.GameMap) != 'object')
	PureFW.GameMap = new Object();

/**
 * @param Node parent_node	An welchen Knoten die Map angehängt werden soll
 * @param int x				Wo sich die Map befinden soll in x-Richtung in px
 * @param int y				Wo sich die Map befinden soll in y-Richtung in px
 * @param int w				Wie breit die gesamte Map sein soll
 * @param int h				Wie hoch die gesamte Map sein soll
 * @param bool no_scale		Nicht sklierbar (default: false)
 * 
 * @see PureFW.GameMap.MultilayerMap
 */
PureFW.GameMap.PathFindField = function(parent_node, x, y, w, h, no_scale)
{
	if (typeof(parent_node) !== 'undefined') {
		this.init(parent_node, x, y, w, h, no_scale);
		
		this.instance_num = this.constructor.num_instances;
		this.instance_name = "PureFW.GameMap" +
				".PathFindField.instances["	+this.instance_num+"]";
		
		this.id = "PureFW.GameMap.PathFindField"+this.instance_num;
		this.content_id 
			= "PureFW.GameMap.PathFindField_cont"+this.instance_num;
		this.bg_img_id = 'PureFW.GameMap.PathFindField_bg'+this.instance_num;
		
		this.insert_into_dom_tree();
		this.constructor.num_instances++;
		this.constructor.instances[this.instance_num] = this;
		
		if (!this.no_scale)
			PureFW.WidgetManager.manager_all.register_widget(this);
	}
};
/********************************************************************
 * PureFW.GameMap.PathFindField extends Container
 ********************************************************************/
PureFW.GameMap.PathFindField.extend(PureFW.Container);

/********************************************************************
 * Statische Deklarationen
 ********************************************************************/
PureFW.GameMap.PathFindField.num_instances = 0;		// Instanzüberwachung (anzahl)
PureFW.GameMap.PathFindField.instances = new Array();// Instanzüberwachung (Instanzen)



/********************************************************************
 * Prototyp-Deklarationen
 ********************************************************************/
PureFW.GameMap.PathFindField.prototype.init = function(parent_node, x, y, 
	w, h, no_scale)
{
	PureFW.GameMap.PathFindField.parent.init.call(this, parent_node, x, y, 
		w, h, no_scale);

	
	this.t_stamp_cont = null;	// hier wird die Zeit angezeigt in dem Schritt
	this.t_stamp = 0;
};

PureFW.GameMap.PathFindField.prototype.insert_into_dom_tree = function() {
	PureFW.GameMap.PathFindField.parent.insert_into_dom_tree.call(this);
	
	this.get_node().style.overflow = 'visible';
	
	this.set_bg_img('map/waypoint.png');
	
	this.t_stamp_cont = new PureFW.Container(
		this,
		44, 17,
		50, 22,
		this.no_scale
	);
	this.t_stamp_cont.set_css_class('mammun_map_path_find_field');
}

/**
 * Setzt die Uhrzeit, die angezeigt werden soll (Angabe in Sekunden).
 * 
 * @param uint t
 */
PureFW.GameMap.PathFindField.prototype.set_t_stamp = function(t) {
	this.t_stamp = t;
	this.t_stamp_cont.set_content(
		PureFW.Time.sec_in_time(this.t_stamp, false)
	);
}
/**
 * @see PureFW.Container#destroy
 */
PureFW.GameMap.PathFindField.prototype.destroy = function() {
	PureFW.GameMap.PathFindField.parent.destroy.call(this);
	this.t_stamp_cont.destroy();
}

