/**
 * Dieses Widget repräsentiert eine unsichtbare Click-Ebene, die an 
 * ausgezeichneten Stellen rechteckige Klickflächen erhalten kann.
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
PureFW.GameMap.ClickRectMap = function(parent_node, x, y, w, h, no_scale)
{
	if (typeof(parent_node) !== 'undefined') {
		this.init(parent_node, x, y, w, h, no_scale);
		
		this.instance_num = this.constructor.num_instances;
		this.instance_name = "MammunUI.get_top_frame().PureFW.GameMap" +
				".ClickRectMap.instances["	+this.instance_num+"]";
		
		this.id = "PureFW.GameMap.ClickRectMap"+this.instance_num;
		this.content_id 
			= "PureFW.GameMap.ClickRectMap_cont"+this.instance_num;
		this.bg_img_id = 'PureFW.GameMap.ClickRectMap_bg'+this.instance_num;
		
		this.insert_into_dom_tree();
		this.constructor.num_instances++;
		this.constructor.instances[this.instance_num] = this;
		
		if (!this.no_scale)
			PureFW.WidgetManager.manager_all.register_widget(this);
	}
};
/********************************************************************
 * PureFW.GameMap.ClickRectMap extends Container
 ********************************************************************/
PureFW.GameMap.ClickRectMap.extend(PureFW.Container);

/********************************************************************
 * Statische Deklarationen
 ********************************************************************/
PureFW.GameMap.ClickRectMap.num_instances = 0;		// Instanzüberwachung (anzahl)
PureFW.GameMap.ClickRectMap.instances = new Array();// Instanzüberwachung (Instanzen)



/********************************************************************
 * Prototyp-Deklarationen
 ********************************************************************/
PureFW.GameMap.ClickRectMap.prototype.init = function(parent_node, x, y, 
	w, h, no_scale)
{
	PureFW.GameMap.ClickRectMap.parent.init.call(this, parent_node, x, y, 
		w, h, no_scale);

	this.on_field_click_functions = new Array();
	this.on_field_mouseover_functions = new Array();
	this.on_field_mouseout_functions = new Array();
	
	this.click_areas = new Object();
};

PureFW.GameMap.ClickRectMap.prototype.insert_into_dom_tree = function() {
	PureFW.GameMap.ClickRectMap.parent.insert_into_dom_tree.call(this);
	
	this.get_node().style.overflow = 'visible';
}

/**
 * Fügt eine Click-Fläche der Ebene hinzu.
 * 
 * @param uint x
 * @parem uint y
 * @param uint w
 * @param uint h
 */
PureFW.GameMap.ClickRectMap.prototype.add_click_area = function(x, y, w, 
	h) 
{
	var new_area = new PureFW.Container(
		this,
		x, y,
		w, h,
		this.no_scale
	);
	new_area.add_event_handler("click",
		(function(_instance) {
			return function(ev) {
				_instance.on_field_click(ev);
			}
		})(this)
	);
	new_area.add_event_handler("mouseover",
		(function(_instance) {
			return function(ev) {
				_instance.on_field_mouseover(ev);
			}
		})(this)
	);
	new_area.add_event_handler("mouseup",
		(function(_instance) {
			return function(ev) {
				_instance.on_field_mouseup(ev);
			}
		})(this)
	);
	if (!this.click_areas[x])
		this.click_areas[x] = new Object();
	this.click_areas[x][y] = new_area;
	
	return new_area;
}

/**
 * Entfernt eine Klickfläche
 * 
 * @param uint x
 * @parem uint y
 */
PureFW.GameMap.ClickRectMap.prototype.remove_click_area = function(x, y) {
	if (this.click_areas[x] && this.click_areas[x][y]) {
		try {
			this.click_areas[x][y].destroy();
		}
		catch(e){}
		this.click_areas[x][y] = null;
	}
}

/**
 * Erweitert das Hinzufügen von Ereignissen um die Ereignisse "field_click",
 * "field_mouseover", "field_mouseout"
 * 
 * @see Widget#add_event_handler
 * @param string type
 * @param Function fn
 */
PureFW.GameMap.ClickRectMap.prototype.add_event_handler = function(type, fn) {
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
		PureFW.GameMap.ClickRectMap.parent.add_event_handler.call(this, type, fn);
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
PureFW.GameMap.ClickRectMap.prototype.remove_event_handler = function(type, fn) {
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
		PureFW.GameMap.ClickRectMap.parent.remove_event_handler.call(this, type, fn);
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
PureFW.GameMap.ClickRectMap.prototype.on_field_click = function(ev, i, j) {
	if (ev) {
		try {
			ev = PureFW.EventUtil.formatEvent(ev);
			ev.detail = new PureFW.Point(i,j);
			if (typeof(ev.detail) != 'object')
				throw "Event is not changable";
		}
		catch(e) {
			ev = this.create_event("click");
			ev.detail = new PureFW.Point(i,j);
		}
	}
	else {
		ev = this.create_event("click");
		ev.detail = new PureFW.Point(i,j);
	}
	
	var n = this.on_field_click_functions.length;
	for (var i = 0; i < n; i++) {
		this.on_field_click_functions[i].call(this, ev);
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
PureFW.GameMap.ClickRectMap.prototype.on_field_mouseover = function(ev, i, j) {
	if (ev) {
		try {
			ev = PureFW.EventUtil.formatEvent(ev);
			ev.detail = new PureFW.Point(i,j);
			if (typeof(ev.detail) != 'object')
				throw "Event is not changable";
		}
		catch(e) {
			ev = this.create_event("mouseover");
			ev.detail = new PureFW.Point(i,j);
		}
	}
	else {
		ev = this.create_event("mouseover");
		ev.detail = new PureFW.Point(i,j);
	}
	
	var n = this.on_field_mouseover_functions.length;
	for (var i = 0; i < n; i++)
		this.on_field_mouseover_functions[i].call(this, ev);
}

/**
 * Wrapperfunktion, um das MouseOver-Event auszulösen mit ergänzenden 
 * Informationen bzgl. des angeklickten Feldes zu versehen und dann erst zu 
 * feuern.
 * 
 * @param Event ev
 * @param IsoMouseOverMap.Field field
 */
PureFW.GameMap.ClickRectMap.prototype.on_field_mouseout = function(ev, i, j) {
	if (ev) {
		try {
			ev = PureFW.EventUtil.formatEvent(ev);
			ev.detail = new PureFW.Point(i,j);
			if (typeof(ev.detail) != 'object')
				throw "Event is not changable";
		}
		catch(e) {
			ev = this.create_event("mouseout");
			ev.detail = new PureFW.Point(i,j);
		}
	}
	else {
		ev = this.create_event("mouseout");
		ev.detail = new PureFW.Point(i,j);
	}
	
	var n = this.on_field_mouseout_functions.length;
	for (var i = 0; i < n; i++)
		this.on_field_mouseout_functions[i].call(this, ev);
}

/**
 * @see PureFW.Container#destroy
 */
PureFW.GameMap.ClickRectMap.prototype.destroy = function() {
	PureFW.GameMap.ClickRectMap.parent.destroy.call(this);
	this.click_areas.destroy();
}

