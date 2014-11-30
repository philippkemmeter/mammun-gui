/**
 * Dieses Objekt beschreibt eine isometrische Karte mit beliebig vielen 
 * Darstellungsebenen unterhalb einer Klick-Map (natürlich alles skalierbar).
 * 
 * Allerdings ist die Click-Map durch Rechtecke angenähert.
 * 
 * @author Phil
 * @package PureFW.IsoMap
 */


/**
 * Erzeugt die isometrische, mehrschichtige Karte.
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
 * @see PureFW.IsoMap.PseudoClickMap
 */
PureFW.IsoMap.PseudoMultilayerMap = function(parent_node, x, y, w, h, 
	num_fields_x, num_fields_y, no_scale)
{
	if (typeof(parent_node) !== 'undefined') {
		this.init(parent_node, x, y, w, h, num_fields_x, num_fields_y, no_scale);
		
		this.instance_num = this.constructor.num_instances;
		this.instance_name = "PureFW.IsoMap.PseudoMultilayerMap.instances["
			+this.instance_num+"]";
		
		this.id = "PureFW.IsoMap.PseudoMultilayerMap"+this.instance_num;
		this.img_id = "PureFW.IsoMap.PseudoMultilayerMap_img"+this.instance_num;
		this.map_id = 'PureFW.IsoMap.PseudoMultilayerMap_map'+this.instance_num;
		this.area_id_prefix = "PureFW.IsoMap.PseudoMultilayerMap_area"
			+this.instance_num;
		
		this.insert_into_dom_tree();
		this.constructor.num_instances++;
		this.constructor.instances[this.instance_num] = this;
		
		if (!this.no_scale)
			PureFW.WidgetManager.manager_all.register_widget(this);
	}
};
/********************************************************************
 * PureFW.IsoMap.PseudoMultilayerMap extends PseudoClickMap
 ********************************************************************/
PureFW.IsoMap.PseudoMultilayerMap.extend(PureFW.IsoMap.PseudoClickMap);

/********************************************************************
 * Statische Deklarationen
 ********************************************************************/
PureFW.IsoMap.PseudoMultilayerMap.num_instances = 0;		// Instanzüberwachung (anzahl)
PureFW.IsoMap.PseudoMultilayerMap.instances = new Array();// Instanzüberwachung (Instanzen)

/********************************************************************
 * Prototyp-Deklarationen
 ********************************************************************/
PureFW.IsoMap.PseudoMultilayerMap.prototype.init = function(parent_node, x, y, w, h, 
	num_fields_x, num_fields_y, no_scale)
{
	PureFW.IsoMap.PseudoMultilayerMap.parent.init.call(this, parent_node, x, y, w, h, 
		num_fields_x, num_fields_y, no_scale);
	
	/**
	 * Die Visualisierungsebenen werden hier gespeichert.
	 */
	this.vis_layers = new Object();
};

PureFW.IsoMap.PseudoMultilayerMap.prototype.insert_into_dom_tree = function() {
	PureFW.IsoMap.PseudoMultilayerMap.parent.insert_into_dom_tree.call(this);
};

/**
 * Fügt eine neue Kartendarstellungsebene hinzu.
 * 
 * Der <code>identifier</code> muss angegeben werden, um der Ebene einen
 * eindeutigen Namen zu geben, unter dem diese später wieder abrufbar ist. 
 * Der angegebene z-Index entscheidet bei mehreren Ebenen, welche über welcher
 * angezeigt wird.
 * 
 * Gibt die neue Ebene zurück, so dass dort dann die einzelnen Felder
 * der Ebene angepasst werden können.
 * 
 * @param string identifier
 * @param uint z_index=0
 * @return PureFW.IsoMap.VisMap
 */
PureFW.IsoMap.PseudoMultilayerMap.prototype.add_layer = function(identifier, 
	z_index) 
{
	this.vis_layers[identifier] = new PureFW.IsoMap.VisMap(
		this,
		0, 0, 
		this.width, this.height,
		this.num_fields_x, this.num_fields_y,
		this.no_scale
	);
	/**
	 * Z-Index setzen, wenn einer angegeben wurde. Dabei darauf achten, dass
	 * das unsichtbare Click-Map-Bild immer einen höheren z-Index hat, als alle
	 * Layer.
	 */
	if (typeof(z_index) != 'undefined') {
		this.vis_layers[identifier].set_z_index(z_index);

		if (this.field_widgets_z_index <= z_index) {
			var n = this.field_widgets.length;
			this.field_widgets_z_index = z_index+1;
			for (var i = 0; i < n; i++)
				this.field_widgets[i].set_z_index(z_index+1);
		}
	}
	return this.vis_layers[identifier];
}

/**
 * Entfernt die durch <code>identifier</code> angegebene Visualisierungsebene.
 * 
 * @param string identifier
 */
PureFW.IsoMap.PseudoMultilayerMap.prototype.remove_layer = function(identifier) {
	if (typeof(this.vis_layers[identifier]) != 'undefined') {
		try {
			this.vis_layers[identifier].destroy();
		}
		catch(e) {}
		this.vis_layers[identifier] = null;
	}
}

/**
 * Gibt die durch <code>identifier</code> angegebene Visualisierungsebene
 * zurück.
 * 
 * @param identifier
 * @return PureFW.IsoMap.VisMap
 */
PureFW.IsoMap.PseudoMultilayerMap.prototype.get_layer = function(identifier) {
	return this.vis_layers[identifier];
}