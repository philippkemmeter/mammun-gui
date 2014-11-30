/**
 * Diese Klasse nähert mithilfe von rechteckigen Klickflächen eine korrekte
 * ClickMap an. Sie wird vorallem dann genutzt werden müssen, wenn ein Browser
 * Bugs mit AREA-Tags hat.
 * 
 * Der bekannteste Bug ist, dass AREA-Tags in Chrome, Safari und IE nicht immer
 * korrekt innerhalb von Frames funktionieren. In dem Fall muss hierher 
 * ausgewichen werden.
 * 
 * Das das PureFW diese Fehler nicht selbst detektiert, muss der Entwickler
 * dafür sorgetragen, wann er diese Klasse benutzt und wann nicht. Dieses
 * Widget hier funktioniert jedenfalls immer!
 */

/**
 * Erzeugt die isometrische Pseudo-Clickmap.
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
PureFW.IsoMap.PseudoClickMap = function(parent_node, x, y, w, h, num_fields_x, 
	num_fields_y, no_scale)
{
	if (typeof(parent_node) !== 'undefined') {
		this.init(parent_node, x, y, w, h, num_fields_x, num_fields_y, no_scale);
		
		this.instance_num = this.constructor.num_instances;
		this.instance_name = "PureFW.IsoMap.PseudoClickMap.instances["
			+this.instance_num+"]";
		
		this.id = "PureFW.IsoMap.PseudoClickMap"+this.instance_num;
		
		this.insert_into_dom_tree();
		this.constructor.num_instances++;
		this.constructor.instances[this.instance_num] = this;
		
		if (!this.no_scale)
			PureFW.WidgetManager.manager_all.register_widget(this);
	}
};
/********************************************************************
 * PureFW.IsoMap.PseudoClickMap extends Map
 ********************************************************************/
PureFW.IsoMap.PseudoClickMap.extend(PureFW.IsoMap.Map);

/********************************************************************
 * Statische Deklarationen
 ********************************************************************/
PureFW.IsoMap.PseudoClickMap.num_instances = 0;		// Instanzüberwachung (anzahl)
PureFW.IsoMap.PseudoClickMap.instances = new Array();	// Instanzüberwachung (Instanzen)

/**
 * Struktur eines Feldes innerhalb der Iso-Click-Map
 */
/*Struct*/ PureFW.IsoMap.PseudoClickMap.Field = function() {
	this.enabled = true;
}
PureFW.IsoMap.PseudoClickMap.Field.prototype.destroy = function() {}

/********************************************************************
 * Prototyp-Deklarationen
 ********************************************************************/
PureFW.IsoMap.PseudoClickMap.prototype.init = function(parent_node, x, y, w, h, 
	num_fields_x, num_fields_y, no_scale)
{
	PureFW.IsoMap.PseudoClickMap.parent.init.call(this, parent_node, x, y, w, h,
		num_fields_x, num_fields_y, no_scale);

	for (var x = 0; x < this.num_fields_x; x++) {
		this.fields[x] = new Array();
		for (var y = 0; y < this.num_fields_y; y++) {
			this.fields[x][y] = null;
		}
	}
	
	this.field_widgets = new Array();
	this.field_widgets_z_index = 0;
	
	this.on_field_click_functions = new Array();
	this.on_field_mouseover_functions = new Array();
	this.on_field_mouseout_functions = new Array();

	/**
	 * Wenn Felder hinzugefügt oder entfernt wurden, wir das hier true. Dann
	 * und nur dann wird bei refresh die click-map neu aufgebaut.
	 */
	this.need_area_recreate = false;
};

PureFW.IsoMap.PseudoClickMap.prototype.insert_into_dom_tree = function() {
	var sector_inner_html =
		'<div id="'+this.id+'"'+
		' style="position: absolute; top: '+this.position.y+'px;'+
		' left: '+(this.position.x)+'px;'+
		' width:'+(this.width)+'px; height:'+(this.height)+'px; '+
		'z-index: '+(this.z_index)+';"></div>';
	this.parent_node.innerHTML = this.parent_node.innerHTML + sector_inner_html;
};

/**
 * Wrapperfunktion, um das Click-Event auszulösen mit ergänzenden Informationen
 * bzgl. des angeklickten Feldes zu versehen und dann erst zu feuern.
 * 
 * @param Event ev
 * @param uint i
 * @param uint j
 */
PureFW.IsoMap.PseudoClickMap.prototype.on_field_click = function(ev, i, j) {
	if (!this.fields[i][j].enabled)
		return;
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
PureFW.IsoMap.PseudoClickMap.prototype.on_field_mouseover = function(ev, i, j) {
	if (!this.fields[i][j].enabled)
		return;
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
PureFW.IsoMap.PseudoClickMap.prototype.on_field_mouseout = function(ev, i, j) {
	if (!this.fields[i][j].enabled)
		return;
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
PureFW.IsoMap.PseudoClickMap.prototype.add_event_handler = function(type, fn) {
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
		PureFW.IsoMap.PseudoClickMap.parent.add_event_handler.call(this, type, fn);
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
PureFW.IsoMap.PseudoClickMap.prototype.remove_event_handler = function(type, fn) {
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
		PureFW.IsoMap.PseudoClickMap.parent.remove_event_handler.call(this, type, fn);
	}
}

/**
 * Zerstört das PureFW.IsoMap.PseudoClickMap und alle seine Felder.
 * @see PureFW.Widget#destroy
 */
PureFW.IsoMap.PseudoClickMap.prototype.destroy = function() 
{
	PureFW.IsoMap.PseudoClickMap.parent.destroy.call(this);
	this.field_widgets.destroy();
	this.fields.destroy();
};

/**
 * Erzeugt ein Feld in der Click-Map.
 * 
 * @param x
 * @param y
 * @return
 */
PureFW.IsoMap.PseudoClickMap.prototype.create_field = function (x, y) {
	if (!this.fields[x][y]) {
		this.fields[x][y] = new PureFW.IsoMap.PseudoClickMap.Field();
		this.need_area_recreate = true;
	}
}

/**
 * Zerstört ein Feld in der Click-Map.
 * 
 * @param x
 * @param y
 * @return
 */
PureFW.IsoMap.PseudoClickMap.prototype.destroy_field = function (x, y) {
	if (this.fields[x][y]) {
		try {
			this.fields[x][y].destroy();
		}
		catch(e){}
		this.fields[x][y] = null;
		this.need_area_recreate = true;
	}
}

/**
 * Nach jeder Änderung der existierenden Felder muss explizit diese Funktion
 * aufgerufen werden, um die Änderungen wirksam zu machen.
 * 
 * Der DOM-Baum wird dann entsprechend angepasst. Würde diese Funktion 
 * automatisch bei jedem eingefügten Feld aufgerufen werden, würde dies die
 * Performance stark negativ beeinträchtigen.
 */
PureFW.IsoMap.PseudoClickMap.prototype.refresh = function() {
	if (this.need_area_recreate) {
		this.field_widgets.destroy();
		for (var i = 0; i < this.num_fields_x; i++) {
			for (var j = 0; j < this.num_fields_y; j++) {
				if (!this.fields[i][j])
					continue;
				
				var x = (this.width >> 1) + 
					(((i-j-1)*this.field_width + (this.field_width >> 1)) >> 1);
				
				var y = (((i+j)*this.field_height + (this.field_height>>1))>>1);
				
				var tmp = new PureFW.Image(
					this,
					x, y,
					this.field_width >> 1,
					this.field_height >> 1,
					'pattern/spacer.gif',
					this.no_scale
				);
				tmp.instance_name = "MammunUI.get_top_frame()."
					+ tmp.instance_name;
				tmp.add_event_handler("click",
					(function (_instance, _i, _j) {
						return function (ev) {
							_instance.on_field_click(ev, _i, _j)
						}
					})(this, i, j)
				);
				tmp.add_event_handler("mouseover",
					(function (_instance, _i, _j) {
						return function (ev) {
							_instance.on_field_mouseover(ev, _i, _j)
						}
					})(this, i, j)
				);
				tmp.add_event_handler("mouseout",
					(function (_instance, _i, _j) {
						return function (ev) {
							_instance.on_field_mouseout(ev, _i, _j)
						}
					})(this, i, j)
				);
				tmp.set_z_index(this.field_widgets_z_index);
				this.field_widgets.push(tmp);
			}
		}
	}
};

/**
 * Aktiviert die angegebenen Felder.
 * 
 * 
 * @param uint x
 * @param uint y
 */
PureFW.IsoMap.PseudoClickMap.prototype.activate_field = function(x, y)
{
	if (x < 0 || x >= this.num_fields_x)
		throw new Error("x has to be in {0,1, .., "+this.num_fields_x+
			". '"+x+"' given.");
	if (y < 0 || y >= this.num_fields_y)
		throw new Error("y has to be in {0,1, .., "+this.num_fields_y+
			". '"+y+"' given.");
	
	this.fields[x][y].enabled = true;
};

/**
 * Deaktiviert das durch die Koordinaten bestimmte Feld der PseudoClickMap.
 * 
 * @param uint x
 * @param uint y
 */
PureFW.IsoMap.PseudoClickMap.prototype.deactivate_field = function(x,y) 
{
	if (x < 0 || x >= this.num_fields_x)
		throw new Error("x has to be in {0,1, .., "+this.num_fields_x+
			". '"+x+"' given.");
	if (y < 0 || y >= this.num_fields_y)
		throw new Error("y has to be in {0,1, .., "+this.num_fields_y+
			". '"+y+"' given.");
	
	this.fields[x][y].enabled = false;
};