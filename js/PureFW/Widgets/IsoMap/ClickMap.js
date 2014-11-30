/**
 * Diese Klasse repräsentiert eine unsichtbare, isometrische Klick-Map. 
 * 
 * Um eine visualisierte isometrische klickbare Karte zu bekommen, bitte
 * PureFW.IsoMap.MultilayerMap benutzen.
 *  
 * @author Phil [based on former Grid-Widget by Alex]
 * @package PureFW.IsoMap
 */


/**
 * Erzeugt die isometrische Clickmap.
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
 */
PureFW.IsoMap.ClickMap = function(parent_node, x, y, w, h, num_fields_x, 
	num_fields_y, no_scale)
{
	if (typeof(parent_node) !== 'undefined') {
		this.init(parent_node, x, y, w, h, num_fields_x, num_fields_y, no_scale);
		
		this.instance_num = this.constructor.num_instances;
		this.instance_name = "PureFW.IsoMap.ClickMap.instances["
			+this.instance_num+"]";
		
		this.id = "PureFW.IsoMap.ClickMap"+this.instance_num;
		this.img_id = "PureFW.IsoMap.ClickMap_img"+this.instance_num;
		this.map_id = 'PureFW.IsoMap.ClickMap_map'+this.instance_num;
		this.area_id_prefix = "PureFW.IsoMap.ClickMap_area"+this.instance_num;
		
		this.insert_into_dom_tree();
		this.constructor.num_instances++;
		this.constructor.instances[this.instance_num] = this;
		
		if (!this.no_scale)
			PureFW.WidgetManager.manager_all.register_widget(this);
	}
};
/********************************************************************
 * PureFW.IsoMap.ClickMap extends IsoMap.PseudoClickMap
 ********************************************************************/
PureFW.IsoMap.ClickMap.extend(PureFW.IsoMap.PseudoClickMap);

/********************************************************************
 * Statische Deklarationen
 ********************************************************************/
PureFW.IsoMap.ClickMap.num_instances = 0;		// Instanzüberwachung (anzahl)
PureFW.IsoMap.ClickMap.instances = new Array();	// Instanzüberwachung (Instanzen)

/**
 * Struktur eines Feldes innerhalb der Iso-Click-Map
 */
/*Struct*/ PureFW.IsoMap.ClickMap.Field = function() {
	this.enabled = true;
}
PureFW.IsoMap.ClickMap.Field.prototype.destroy = function() {}

/********************************************************************
 * Prototyp-Deklarationen
 ********************************************************************/
PureFW.IsoMap.ClickMap.prototype.init = function(parent_node, x, y, w, h, 
	num_fields_x, num_fields_y, no_scale)
{
	PureFW.IsoMap.ClickMap.parent.init.call(this, parent_node, x, y, w, h,
		num_fields_x, num_fields_y, no_scale);

	for (var x = 0; x < this.num_fields_x; x++) {
		this.fields[x] = new Array();
		for (var y = 0; y < this.num_fields_y; y++) {
			this.fields[x][y] = null;
		}
	}
	
	this.on_field_click_functions = new Array();
	this.on_field_mouseover_functions = new Array();
	this.on_field_mouseout_functions = new Array();

	/**
	 * Wenn Felder hinzugefügt oder entfernt wurden, wir das hier true. Dann
	 * und nur dann wird bei refresh die click-map neu aufgebaut.
	 */
	this.need_area_recreate = false;
};

PureFW.IsoMap.ClickMap.prototype.insert_into_dom_tree = function() {
	var sector_inner_html =
		'<div id="'+this.id+'"'+
		' style="position: absolute; top: '+this.position.y+'px;'+
		' left: '+(this.position.x)+'px;'+
		' width:'+(this.width)+'px; height:'+(this.height)+'px; '+
		'z-index: '+(this.z_index)+';">';
	
	sector_inner_html += 
		'<img id="'+this.img_id+'" border="0" width="100%" height="100%"'+
		' src="../pix/pattern/spacer.gif" usemap="#'+this.map_id+'"'+
		' style="position: absolute; top: 0px; left: 0px; z-index: 0"/>';
	
	sector_inner_html += '<map id="'+this.map_id+'" name="'+this.map_id+'">'
		+ this._generate_map_inner_html() + '</map>';
	
	sector_inner_html += '</div>';
	
	this.parent_node.innerHTML = this.parent_node.innerHTML + sector_inner_html;
};

/**
 * Diese Hilfsfunktion generiert den Inner-HTML-String des Map-Tags neu.
 * 
 * Dieser Inner-HTML-Krams ist notwendig, weil vor allem der Internet-Explorer
 * lauter Bugs erzeugt bei dynamisch generierten Area-Tags als Knoten und z.B.
 * die Area dann einfach ignoriert...
 * 
 * @return String
 * @access private
 * @see PureFW.IsoMap.ClickMap.prototype#insert_into_dom_tree
 * @see PureFW.IsoMap.ClickMap.prototype#repaint
 * 
 */
PureFW.IsoMap.ClickMap.prototype._generate_map_inner_html = function() {
	/**
	 * Wir speichern hier Hilfswerte um Multiplikationen und Shift-Operationen
	 * zu sparen.
	 */
	var field_width = this.field_width;
	var field_height = this.field_height;
	var field_width_scaled = field_width*this.scale_factor;
	var field_height_scaled = field_height*this.scale_factor;
	var field_half_height_scaled = field_height_scaled >> 1;
	var field_half_width_scaled = field_width_scaled >> 1;

	var x, y;
	var area_string = '';
	for (var j = 0; j < this.num_fields_y; j++)	{
		for (var i = 0; i < this.num_fields_x; i++)	{
			if (this.fields[i][j]) {
				var x = ((this.width >> 1) + (((i-j-1)*field_width) >> 1))
					* this.scale_factor;
				var y = (((i+j)*field_height) >> 1)	* this.scale_factor;
				
				area_string += '<area id="'+this.area_id_prefix+'_'+i+'_'+j+'"';
				
				area_string += ' href="javascript: ;"';
				area_string += ' onclick="javascript: '+this.instance_name+
					'.on_field_click(event, '+i+','+j+');"';
				area_string += ' onmouseover="javascript: '+this.instance_name+
					'.on_field_mouseover(event, '+i+','+j+');"';
				area_string += ' onmouseout="javascript: '+this.instance_name+
				'.on_field_mouseout(event, '+i+','+j+');"';
	
				area_string += ' shape="poly" coords="'+
					(x)+','+(y+field_half_height_scaled)+', '+
					(x+field_half_width_scaled)+','+(y)+', '+
					(x+field_width_scaled)+','+(y+field_half_height_scaled)+', '+
					(x+field_half_width_scaled)+','+(y+field_height_scaled)+'"/>';
			}
		}
	}
	return area_string;
}

/**
 * Zerstört das PureFW.IsoMap.ClickMap und alle seine Felder.
 * @see PureFW.Widget#destroy
 */
PureFW.IsoMap.ClickMap.prototype.destroy = function() 
{
	PureFW.IsoMap.ClickMap.parent.destroy.call(this);
	this.fields.destroy();
};

/**
 * Nach jeder Änderung der existierenden Felder muss explizit diese Funktion
 * aufgerufen werden, um die Änderungen wirksam zu machen.
 * 
 * Der DOM-Baum wird dann entsprechend angepasst. Würde diese Funktion 
 * automatisch bei jedem eingefügten Feld aufgerufen werden, würde dies die
 * Performance stark negativ beeinträchtigen.
 */
PureFW.IsoMap.ClickMap.prototype.refresh = function() {
	if (this.need_area_recreate) {
		document.setElemInnerHTML(this.map_id, this._generate_map_inner_html());
		this.need_area_recreate = false;
	}
}