/**
 * SlideTable ist ein Widget, welches eine Tabelle darstellt, deren Spalten-
 * breiten verändert werden können. Dabei slidet die Tabelle animiert ihre
 * Spalten, so, dass die angeforderte neue Spaltenbreitenkonfiguration entsteht.
 * Eine Spalte ist dabei immer die geöffnete (die breiteste), alle anderen
 * sind geschlossen (gleich schmal).
 * 
 * @author Phil
 */

/**
 * Hilfs-Widget: Eine einzelne Spalte in der SlideTabelle
 */
SlideTableCol = function(parent_node, x, y, w, h) {
	if (typeof(parent_node) !== 'undefined') {
		this.init(parent_node, x, y, w, h);
		
		this.instance_num = this.constructor.num_instances;
		this.instance_name = "SlideTableCol.instances["+this.instance_num+"]";
		this.id = "SlideTableCol"+this.instance_num;
		this.content_id = "SlideTableCol_cont"+this.instance_num;
		this.bg_img_id = "SlideTableCol_bg_img" + this.instance_num;
		
		this.insert_into_dom_tree();
		
		this.constructor.num_instances++;
		this.constructor.instances[this.instance_num] = this;
		
		PureFW.WidgetManager.manager_all.register_widget(this);
	}
};


/********************************************************************
* SlideTableCol extends PureFW.Container
********************************************************************/
SlideTableCol.extend(PureFW.Container);

/********************************************************************
 * Statische Deklarationen
 ********************************************************************/
SlideTableCol.num_instances = 0;
SlideTableCol.instances = new Array();

/********************************************************************
 * Prototyp-Deklarationen
 ********************************************************************/

/**
 * Initialisiert alles, wie bei allen Widgets :)
 * 
 * @param HTMLElement parent_node
 * @param uint x
 * @param uint y
 * @param uint w
 * @param uint h
 * @see Widget.js#init
 */
SlideTableCol.prototype.init = function(parent_node, x, y, w, h) 
{
	SlideTableCol.parent.init.call(this, parent_node, x, y, w, h);
	
	this.on_shrink_functions = new Array();
	this.on_grow_functions = new Array();
	
	this.bg_deselected_url = 'ui/backgrounds/pause_menu/slide_table/bright-112x464.png';
	this.bg_selected_url = 'ui/backgrounds/pause_menu/slide_table/dark-281x464.png';
	this.open = false;
}

SlideTableCol.prototype.insert_into_dom_tree = function()
{
	SlideTableCol.parent.insert_into_dom_tree.call(this);
	
	this.inner_scroller = new PureFW.ScrollContainer(
			this.get_node(),
			15, 15,
			this.width - 28, this.height - 28);
	this.inner_scroller.parent_container = this;
};

SlideTableCol.prototype.open_column = function(w)
{
	this.open = true;
	this.set_bg_img(this.bg_selected_url);
	this.get_node().style.cursor = '';
	this.get_content_node().style.cursor = '';
	this.set_width(w);
	this.on_grow();
};

SlideTableCol.prototype.close_column = function(w)
{
	this.open = false;
	this.set_bg_img(this.bg_deselected_url);
	this.get_node().style.cursor = 'pointer';
	this.get_content_node().style.cursor = 'pointer';
	this.set_width(w);
	this.on_shrink();
};

SlideTableCol.prototype.is_open = function() {
	return this.open;
}

SlideTableCol.prototype.set_inner_height = function(h)
{
	this.inner_scroller.set_inner_height(h);
};

SlideTableCol.prototype.get_scroller_node = function()
{
	return this.inner_scroller.get_node();
};

SlideTableCol.prototype.get_content_node = function()
{
	return this.inner_scroller.get_content_node();
};

/**
 * Setzt die Breite des Widgets.
 * 
 * @param uint w
 * @see PureFW.Container#set_width
 */
SlideTableCol.prototype.set_width = function(w) 
{
	SlideTableCol.parent.set_width.call(this,w);
//	this.set_bg_img(this.bg_img, true, this.bg_img_l, this.bg_img_r, 
//				this.bg_img_l_w, this.bg_img_r_w);
	this.inner_scroller.set_width(w - 28);
	this.inner_scroller.set_inner_width(w - 28);
};


/**
 * Fügt einen EventHandler hinzu. Funktion überschrieben, da zusätzlich
 * der Typ "shrink", "grow" unterstützt wird.
 * 
 * @param string type
 * @param Function fn
 * @see Widget.add_event_handler
 */
SlideTableCol.prototype.add_event_handler = function(type, fn) {
	if (type === "shrink")
		this.on_shrink_functions.push(fn);
	else if (type === "grow")
		this.on_grow_functions.push(fn);
	else
		SlideTableCol.parent.add_event_handler.call(this, type, fn);
};

/**
 * Entfernt einen EventHandler. Funktion überschrieben, da zusätzlich
 * der Typen "shrink", "grow" unterstützt wird.
 * 
 * @param string type
 * @param Function fn
 * @see Container.prototype.remove_event_handler
 */
SlideTableCol.prototype.remove_event_handler = function(type, fn) {
	if (type === "shrink")
		this.on_shrink_functions.remove(fn);
	else if (type === "grow")
		this.on_grow_functions.remove(fn);
	else
		SlideTableCol.parent.remove_event_handler.call(this, type, fn);
};

/**
 * Ruft das "on-shrink"-Event exlizit auf. Wird normalerweise aufgerufen,
 * wenn die Spalte verkleinert wird.
 * 
 * @param Event ev [optional]
 */
SlideTableCol.prototype.on_shrink = function(ev) {
	if (!ev) {
		ev = this.create_event("shrink");
	}
	var n = this.on_shrink_functions.length;
	for (var i = 0; i < n; i++) {
		this.on_shrink_functions[i].call(this, ev);
	}
};

/**
 * Ruft das "on-grow"-Event exlizit auf. Wird normalerweise aufgerufen,
 * wenn die Spalte vergrößert wird (passiert beim Anklicken).
 * 
 * @param Event ev [optional]
 */
SlideTableCol.prototype.on_grow = function(ev) {
	if (!ev) {
		ev = this.create_event("grow");
	}
	var n = this.on_grow_functions.length;
	for (var i = 0; i < n; i++) {
		this.on_grow_functions[i].call(this, ev);
	}
};

/**
 * Konstruktor. Parameter, wie bei allen Widgets.
 * 
 * @see PureFW.Widget
 */
SlideTable = function(parent_node, x, y, w, h) {
	if (typeof(parent_node) !== 'undefined') {
		this.init(parent_node, x, y, w, h);
		
		this.instance_num = this.constructor.num_instances;
		this.instance_name = "SlideTable.instances["+this.instance_num+"]";
		this.id = "SlideTable"+this.instance_num;

		// Breite einer jeden geschlossenen Spalte (offene Spalten automatisch!)
//		this.closed_col_width = parseInt(w/10);
		this.closed_col_width = 112;
		// Alle Spalten (initial: keine)
		this.cols = new Array();
		
		// Spalten können einen offset haben, damit über den Spalten noch 
		// Überschriften hinpassen
		this.col_y_offset = 0;
		
		// Welche Spalte ist die offene (index von cols)
		this.col_open_index = -1;
		
		// Welche Spalte als letztes angeklickt wurde
		this.last_clicked_col_index = -1;
		
		this.insert_into_dom_tree();
		
		this.constructor.num_instances++;
		this.constructor.instances[this.instance_num] = this;
		
		PureFW.WidgetManager.manager_all.register_widget(this);
	}
};

/********************************************************************
* SlideTable extends PureFW.Container
********************************************************************/
SlideTable.extend(PureFW.Container);

/********************************************************************
 * Statische Deklarationen
 ********************************************************************/
SlideTable.num_instances = 0;
SlideTable.instances = new Array();

SlideTable.click_wrapper = function(inst_num, col_index) {
	SlideTable.instances[inst_num].open_col(col_index);
};

/********************************************************************
 * Prototyp-Deklarationen
 ********************************************************************/
SlideTable.prototype.init = function(parent_node, x, y, w, h) {
	SlideTable.parent.init.call(this, parent_node, x, y, w, h);
}

/**
 * Fügt die Tabelle in den DOM-Baum ein
 */
SlideTable.prototype.insert_into_dom_tree = function() {
	SlideTable.parent.insert_into_dom_tree.call(this);
	
	// Eine Spalte direkt hinzufügen
	this.add_col();
};

/**
 * Keinen direkten Zugriff auf den Kontent!
 */
SlideTable.prototype.set_content = null;
SlideTable.prototype.add_content = null;
SlideTable.prototype.get_content = null;

/**
 * Fügt der Tabelle eine Spalte hinzu. Gibt die hinzugefügte Spalte zurück.
 * 
 * @return SlideTableCol
 */
SlideTable.prototype.add_col = function() {
	var n = this.cols.length;
	if ((n+1)*this.closed_col_width >= this.width)
		throw Error("SlideTable: Cannot add another Col, no space left!");

	this.cols[n] = new SlideTableCol(
		this,
		this.width - n*this.closed_col_width,
		this.col_y_offset,
		this.closed_col_width,
		this.height
	);
	this.cols[n].set_bg_img(this.cols[n].bg_deselected_url);
	this.cols[n].add_event_handler("click",
		(function(_inst_num, _col_index) {
			return function() {
				SlideTable.click_wrapper(_inst_num, _col_index)
			};
		})(this.instance_num, n)
	);
	if (this.col_open_index < 0)
		this.open_col(0);
	else
		this.open_col(this.col_open_index);
	
	return this.cols[n];
};

/**
 * Gibt die angegebene Spalte zurück.
 *
 * @param uint i
 * @return PureFW.Container
 */
SlideTable.prototype.get_col = function(i) {
	return this.cols[i];
};

/**
 * Setzt die Breite der geschlossenen Spalten.
 * 
 * @param uint w
 */
SlideTable.prototype.set_closed_col_width = function(w) {
	this.closed_col_width = w;
	this.open_col(this.col_open_index);
};

/**
 * Öffnet die Spalte mit dem angegebenen Index. Alle anderen werden geschlossen.
 * 
 * @param uint i
 */
SlideTable.prototype.open_col = function(i) 
{
//	var open_col_w = this.width - (this.cols.length-1)*this.closed_col_width;
	var open_col_w = 281;
	
	var n = this.cols.length;
	for (var j = 0; j < i; j++) 
	{
		this.cols[j].close_column(this.closed_col_width);
		this.cols[j].set_x(this.closed_col_width*j);
	}
	this.cols[j].open_column(open_col_w);
	this.cols[j].set_x(this.closed_col_width*j);
	for (j = i+1; j < n; j++) 
	{
		this.cols[j].close_column(this.closed_col_width);
		this.cols[j].set_x(this.closed_col_width*(j-1)+open_col_w);
	}
	this.col_open_index = i;
};

