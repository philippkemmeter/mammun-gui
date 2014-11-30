/**
 * Dieses Widget repräsentiert eine unsichtbare Click-Map, auf der isometrische
 * 3D-Objekte platziert werden können.
 * 
 * Jedes dieser Objekte kann mehrere Felder belegen und hat eine zusätzliche
 * 3D-Höhe.
 * 
 * @author Phil
 * @package PureFW.IsoMap
 */


/**
 * Erzeugt die isometrische 3D-Clickmap.
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
PureFW.IsoMap.ClickMap3D = function(parent_node, x, y, w, h, num_fields_x, 
	num_fields_y, no_scale)
{
	if (typeof(parent_node) !== 'undefined') {
		this.init(parent_node, x, y, w, h, num_fields_x, num_fields_y, no_scale);
		
		this.instance_num = this.constructor.num_instances;
		this.instance_name = "PureFW.IsoMap.ClickMap3D.instances["
			+this.instance_num+"]";
		
		this.id = "PureFW.IsoMap.ClickMap3D"+this.instance_num;
		this.img_id = "PureFW.IsoMap.ClickMap3D_img"+this.instance_num;
		this.map_id = 'PureFW.IsoMap.ClickMap3D_map'+this.instance_num;
		this.area_id_prefix = "PureFW.IsoMap.ClickMap3D_area"+this.instance_num;
		
		this.insert_into_dom_tree();
		this.constructor.num_instances++;
		this.constructor.instances[this.instance_num] = this;
		
		if (!this.no_scale)
			PureFW.WidgetManager.manager_all.register_widget(this);
	}
};
/********************************************************************
 * PureFW.IsoMap.ClickMap3D extends IsoMap.ClickMap
 ********************************************************************/
PureFW.IsoMap.ClickMap3D.extend(PureFW.IsoMap.ClickMap);

/********************************************************************
 * Statische Deklarationen
 ********************************************************************/
PureFW.IsoMap.ClickMap3D.num_instances = 0;		// Instanzüberwachung (anzahl)
PureFW.IsoMap.ClickMap3D.instances = new Array();	// Instanzüberwachung (Instanzen)

/**
 * Struktur eines Objektes innerhalb der Iso-Click-Map-3D
 */
/*Struct*/ PureFW.IsoMap.ClickMap3D.MapObject= function(x, y, w, h, ph, tooltip) 
{
	this.enabled = true;
	this.size = new PureFW.Point(w,h);	// Größe in Feldern
	this.pos = new PureFW.Point(x,y);	// Position in Feldern
	this.proj_h = ph;					// Projektionshöhenfaktor
	this.tooltip = tooltip;
}
PureFW.IsoMap.ClickMap3D.MapObject.prototype.destroy = function() {
	delete this.size;
	this.size = null;
}

/********************************************************************
 * Prototyp-Deklarationen
 ********************************************************************/
PureFW.IsoMap.ClickMap3D.prototype.init = function(parent_node, x, y, w, h, 
	num_fields_x, num_fields_y, no_scale)
{
	PureFW.IsoMap.ClickMap3D.parent.init.call(this, parent_node, x, y, w, h,
		num_fields_x, num_fields_y, no_scale);

	this.objects_3d = new Array();
	for (var x = 0; x < this.num_fields_x; x++) {
		this.objects_3d[x] = new Array();
		for (var y = 0; y < this.num_fields_y; y++) {
			this.objects_3d[x][y] = null;
		}
	}
	
	this.on_object_click_functions = new Array();
	this.on_object_mouseover_functions = new Array();
	this.on_object_mouseout_functions = new Array();
	
	this.img_y = 0;	// y-Position des Bildes, das die Map enthält.
					// siehe dazu _generate_map_inner_html
};

PureFW.IsoMap.ClickMap3D.prototype.insert_into_dom_tree = function() {
	PureFW.IsoMap.ClickMap3D.parent.insert_into_dom_tree.call(this);
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
 * @see PureFW.IsoMap.ClickMap3D.prototype#insert_into_dom_tree
 * @see PureFW.IsoMap.ClickMap3D.prototype#repaint
 * 
 */
PureFW.IsoMap.ClickMap3D.prototype._generate_map_inner_html = function() {
	var x, y;
	var area_string = '';
	/**
	 * min_y enthält am Ende den kleinsten y-Wert. Dieser wird benötigt, um
	 * das Image, in dem die Map eingebunden wird, korrekt zu positionieren, da
	 * es durchaus sein kann, dass ein 3DObjekt über die y-Position 0 
	 * hinausragt, woraus folgt, dass auch die Clickmap dann verschoben wäre,
	 * wenn das Bild nicht korrekt bei min_y positioniert wird.
	 */
	var min_y = Infinity;
	for (var j = 0; j < this.num_fields_y; j++)	{
		for (var i = 0; i < this.num_fields_x; i++)	{
			if (this.objects_3d[i][j]) {
				
				var y = ((this.field_height*(i + j) >> 1)
					-this.objects_3d[i][j].size.y*this.field_height
						*(this.objects_3d[i][j].proj_h-1)) * this.scale_factor;
				
				// 2D-projizierte Höhe der Grundfläche des Gebäudes in px
				var bbaseh = this.objects_3d[i][j].size.y * this.field_height
					* this.scale_factor;
				// 2D-projizierte Höhe des Geb. in px
				var bh = this.objects_3d[i][j].size.y * 
					this.objects_3d[i][j].proj_h * this.field_height
					* this.scale_factor;
				
				this.objects_3d[i][j].tmp_y = y;			// zwischenspeichern
				this.objects_3d[i][j].tmp_bbaseh = bbaseh;	// zwischenspeichern
				this.objects_3d[i][j].tmp_bh = bh;			// zwischenspeichern
				
				y -= bh - (bbaseh >> 1);
				if (y < min_y)
					min_y = y;
			}
		}
	}
	// jetzt kenne ich das min_y und es kann losgehn.
	for (var j = 0; j < this.num_fields_y; j++)	{
		for (var i = 0; i < this.num_fields_x; i++)	{
			if (this.objects_3d[i][j]) {
				// 2D-projizierte Breite des Gebäudes in px
				var bw = this.objects_3d[i][j].size.x * this.field_width;
				// 2D-projizierte Höhe des Geb. in px
				var bh = this.objects_3d[i][j].tmp_bh; 
				// 2D-projizierte Höhe der Grundfläche des Gebäudes in px
				var bbaseh = this.objects_3d[i][j].tmp_bbaseh;
				var x = ((this.width + this.field_width
					*(i - j - this.objects_3d[i][j].size.x)) >> 1) 
					* this.scale_factor;
				var y = this.objects_3d[i][j].tmp_y - min_y /* HIER, min_y !!! */;
				
				/**
				 * Bei überlappenden Areas wird die ERSTE nach vorne geholt
				 * (also im Gegenteil zur Renderreihenfolge)
				 * Deswegen wird die neuste Area vor die andern geschrieben
				 */
				area_string = '<area id="'+this.area_id_prefix+'_'+i+'_'+j+'"'
							+ ' href="javascript: ;"'
							+ ' onclick="javascript: '+this.instance_name+
								'.on_object_click(event, '+i+','+j+');"'
							+ ' onmouseover="javascript: '+this.instance_name+
								'.on_object_mouseover(event, '+i+','+j+');"'
							+ ' onmouseout="javascript: '+this.instance_name+
								'.on_object_mouseout(event, '+i+','+j+');"'
							+ ' shape="poly" coords="'+
								x + ', ' + (y+(bbaseh >> 1)) + ', ' +
								(x+(bw >> 1)) + ', ' + y + ', ' +
								(x+bw) + ', ' + (y+(bbaseh >> 1)) + ', '+
								(x+bw) + ', ' + (y+(bh - (bbaseh >> 1))) + ', '+
								(x+(bw >> 1)) + ', ' + (y+bh) + ', '+
								x+ ', ' + (y+(bh - (bbaseh >> 1))) + '"/>'
					+ area_string;
			}
		}
	}
	this.img_y = min_y;

	return area_string;
}

/**
 * @see PureFW.Widget#repaint
 */
PureFW.IsoMap.ClickMap3D.prototype.repaint = function() {
	PureFW.IsoMap.ClickMap3D.parent.repaint.call(this);
	
	this.refresh();
}

/**
 * Wrapperfunktion, um das Click-Event auszulösen mit ergänzenden Informationen
 * bzgl. des angeklickten Feldes zu versehen und dann erst zu feuern.
 * 
 * @param Event ev
 * @param uint i
 * @param uint j
 */
PureFW.IsoMap.ClickMap3D.prototype.on_object_click = function(ev, i, j) {
	if (!this.objects_3d[i][j].enabled)
		return;
	var n = this.on_object_click_functions.length;
	if (n == 0)
		return;
	
	if (ev) {
		try {
			ev = PureFW.EventUtil.formatEvent(ev);
			ev.detail = this.objects_3d[i][j];
			if (typeof(ev.detail) != 'object')
				throw "Event is not changable";
		}
		catch(e) {
			ev = this.create_event("click");
			ev.detail = this.objects_3d[i][j];
		}
	}
	else {
		ev = this.create_event("click");
		ev.detail = this.objects_3d[i][j];
	}
	
	for (var i = 0; i < n; i++) {
		this.on_object_click_functions[i].call(this, ev);
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
PureFW.IsoMap.ClickMap3D.prototype.on_object_mouseover = function(ev, i, j) {
	if (!this.objects_3d[i][j].enabled)
		return;
	var n = this.on_object_mouseover_functions.length;
	if (n == 0)
		return;
	
	if (ev) {
		try {
			ev = PureFW.EventUtil.formatEvent(ev);
			ev.detail = this.objects_3d[i][j];
			if (typeof(ev.detail) != 'object')
				throw "Event is not changable";
		}
		catch(e) {
			ev = this.create_event("mouseover");
			ev.detail = this.objects_3d[i][j];
		}
	}
	else {
		ev = this.create_event("mouseover");
		ev.detail = this.objects_3d[i][j];
	}
	
	for (var i = 0; i < n; i++)
		this.on_object_mouseover_functions[i].call(this, ev);
}

/**
 * Wrapperfunktion, um das MouseOver-Event auszulösen mit ergänzenden 
 * Informationen bzgl. des angeklickten Feldes zu versehen und dann erst zu 
 * feuern.
 * 
 * @param Event ev
 * @param IsoMouseOverMap.Field field
 */
PureFW.IsoMap.ClickMap3D.prototype.on_object_mouseout = function(ev, i, j) {
	if (!this.objects_3d[i][j].enabled)
		return;
	var n = this.on_object_mouseout_functions.length;
	if (n == 0)
		return;
	
	if (ev) {
		try {
			ev = PureFW.EventUtil.formatEvent(ev);
			ev.detail = this.objects_3d[i][j];
			if (typeof(ev.detail) != 'object')
				throw "Event is not changable";
		}
		catch(e) {
			ev = this.create_event("mouseout");
			ev.detail = this.objects_3d[i][j];
		}
	}
	else {
		ev = this.create_event("mouseout");
		ev.detail = this.objects_3d[i][j];
	}
	
	for (var i = 0; i < n; i++)
		this.on_object_mouseout_functions[i].call(this, ev);
}

/**
 * Erweitert das Hinzufügen von Ereignissen um die Ereignisse "field_click",
 * "field_mouseover", "field_mouseout"
 * 
 * @see Widget#add_event_handler
 * @param string type
 * @param Function fn
 */
PureFW.IsoMap.ClickMap3D.prototype.add_event_handler = function(type, fn) {
	if (type == "object_mouseover") {
		this.on_object_mouseover_functions.push(fn);
	}
	else if (type == "object_mouseout") {
		this.on_object_mouseout_functions.push(fn);
	}
	else if (type == "object_click") {
		this.on_object_click_functions.push(fn);
	}
	else {
		PureFW.IsoMap.ClickMap3D.parent.add_event_handler.call(this, type, fn);
	}
}

/**
 * Erweitert das Entfernen von Ereignissen um die Ereignisse "object_click",
 * "object_mouseover", "object_mouseout"
 * 
 * @see Widget#add_event_handler
 * @param string type
 * @param Function fn
 */
PureFW.IsoMap.ClickMap3D.prototype.remove_event_handler = function(type, fn) {
	if (type == "object_mouseover") {
		this.on_object_mouseover_functions.remove(fn);
	}
	else if (type == "object_mouseout") {
		this.on_object_mouseout_functions.remove(fn);
	}
	else if (type == "object_click") {
		this.on_object_click_functions.remove(fn);
	}
	else {
		PureFW.IsoMap.ClickMap3D.parent.remove_event_handler.call(this, type, fn);
	}
}

/**
 * Zerstört das PureFW.IsoMap.ClickMap3D und alle seine Felder.
 * @see PureFW.Widget#destroy
 */
PureFW.IsoMap.ClickMap3D.prototype.destroy = function() 
{
	PureFW.IsoMap.ClickMap3D.parent.destroy.call(this);
	this.objects_3d.destroy();
};

/**
 * Erzeugt ein 3D-Objekt auf der Karte.
 * 
 * <code>
 *   /\  Man stelle sich ein Feld als eine Raute wie links zu sehen ist vor. Die
 *   \/  Die Höhe eines 3D-Objektes wird als Projektionsgröße angegeben, und 
 *       zwar gibt sie an, wie viel mal das Objekt, das eingefügt werden soll,
 * von der unteren Spitze der Raute zur obereren reicht. Ein Wert von proj_h
 * von 1 würde also angeben, dass das Objekt genau von unten bis oben reicht, 
 * eine Wert von 0.5 die Hälfte, einer von 1.5 die Hälfte mehr usw.
 * </code>
 * 
 * Es kann also keine 3D-Höhe im eigentlichen Sinne angegeben werden, weil
 * die Objekte ja bereits projiziert/gerendert vorliegen in 2D! Es wird somit
 * nur die 2D-Objekthöhe über den Wert proj_h ermittelt.
 * 
 * Achtung: Überlappungen werden NICHT geprüft. Es handelt sich hierbei nur
 * um eine Visualisierung, daher werden die Daten auch nicht geprüft.
 *  
 * @param uint x			x-Position in Feldkoordinaten (obere linke Ecke)
 * @param uint y 			y-Position in Feldkoordinaten (obere linke Ecke)
 * @param uint size_x		Wie viele Felder in x-Richtung das Objekt einnimmt
 * @param uint size_y		Wie viele Felder in y-Richtung das Objekt einnimmt
 * @param ufloat proj_h		Projizierte Höhe des Objekts als 
 *							Feldhöhenkoordinaten. Die Höhe 1 bedeutet, dass das
 * 							Objekt von der unteren Spitze der Isometriefeldraute
 * 							zur oberen Spitze ragt. Kommazahlen erlaubt.
 * @param strint tooltip	Tooltip, der angezeigt werden soll, wenn man über
 * 							dem Objekt mit der Maus eine Zeit lang verweilt
 * @return Object
 */
PureFW.IsoMap.ClickMap3D.prototype.create_object = function (x, y, size_x,
	size_y, proj_h, tooltip) 
{
	if (!this.objects_3d[x][y]) {
		this.objects_3d[x][y] = new PureFW.IsoMap.ClickMap3D.MapObject(
			x, y, size_x, size_y, proj_h, tooltip
		);
		this.need_area_recreate = true;
	}
	return this.objects_3d[x][y];
}

/**
 * Zerstört ein Object in der Click-Map.
 * 
 * @param x
 * @param y
 * @return
 */
PureFW.IsoMap.ClickMap3D.prototype.destroy_object = function (x, y) {
	if (this.objects_3d[x][y]) {
		try {
			this.objects_3d[x][y].destroy();
		}
		catch(e){}
		this.objects_3d[x][y] = null;
		this.need_area_recreate = true;
	}
}

PureFW.IsoMap.ClickMap3D.prototype.get_object = function(x,y) {
	return (this.objects_3d[x][y]) ? this.objects_3d[x][y] : null;
}

/**
 * Nach jeder Änderung der existierenden Felder muss explizit diese Funktion
 * aufgerufen werden, um die Änderungen wirksam zu machen.
 * 
 * Der DOM-Baum wird dann entsprechend angepasst. Würde diese Funktion 
 * automatisch bei jedem eingefügten Feld aufgerufen werden, würde dies die
 * Performance stark negativ beeinträchtigen.
 */
PureFW.IsoMap.ClickMap3D.prototype.refresh = function() {
	if (this.need_area_recreate) {
		PureFW.IsoMap.ClickMap3D.parent.refresh.call(this);
		document.getElemById(this.img_id).height = this.height - this.img_y;
		document.getElemById(this.img_id).style.top = this.img_y + "px";
	}
}

/**
 * Aktiviert das angegebenen Objekt.
 * 
 * 
 * @param uint x
 * @param uint y
 */
PureFW.IsoMap.ClickMap3D.prototype.activate_object = function(x, y)
{
	if (x < 0 || x >= this.num_fields_x)
		throw new Error("x has to be in {0,1, .., "+this.num_fields_x+
			". '"+x+"' given.");
	if (y < 0 || y >= this.num_fields_y)
		throw new Error("y has to be in {0,1, .., "+this.num_fields_y+
			". '"+y+"' given.");
	
	this.objects_3d[x][y].enabled = true;
};

/**
 * Deaktiviert das durch die Koordinaten bestimmte Objekt der ClickMap3D.
 * 
 * @param uint x
 * @param uint y
 */
PureFW.IsoMap.ClickMap3D.prototype.deactivate_object = function(x,y) 
{
	if (x < 0 || x >= this.num_fields_x)
		throw new Error("x has to be in {0,1, .., "+this.num_fields_x+
			". '"+x+"' given.");
	if (y < 0 || y >= this.num_fields_y)
		throw new Error("y has to be in {0,1, .., "+this.num_fields_y+
			". '"+y+"' given.");
	
	this.objects_3d[x][y].enabled = false;
};