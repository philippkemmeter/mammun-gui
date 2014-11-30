/**
 * Eine Box mit mehreren Tabs.
 * TabBox wird NICHT absolut positioniert, sondern "direkt" an den parent_node
 * angehängt. Absolute Positionierung möglich, indem parent_node derart 
 * positioniert ist.
 */

 
/**
 * Erstellt eine Tabbox initial ohne Tab. Weitere Tabs können
 * mit TabBox::add_tab hinzugefügt werden.
 * 
 * @param HTMLElement parent_node		Wo die TabBox eingehängt werden soll
 * @param uint x						x-Position der Box
 * @param uint y						y-Position   -"-
 * @param uint w						Breite       -"-
 * @param uint h						Höhe         -"-
 * @author Phil
 */
PureFW.TabBox = function (parent_node, x, y, w, h) {
	if (typeof(parent_node) !== 'undefined') {
		this.init();
		this.parent_node = parent_node || document.body;
		this.position = new PureFW.Point(x || 0, y || 0);
		this.width = w || 100;
		this.height = h || 100;
		this.instance_num = this.constructor.num_instances;
		this.instance_name = "PureFW.TabBox.instances["+this.instance_num+"]";
		
		this.id = "TabBox"+this.instance_num;
		this.content_id = "TabBox_cont"+this.instance_num;
		this.bg_img_id = "TabBox_img"+this.instance_num;
		
		this.bg_img = null;
		this.active_tab = -1;
		this.tabs = new Array();
	
		this.constructor.instances[this.instance_num] = this;
		this.constructor.num_instances++;
		this.insert_into_dom_tree();
		PureFW.WidgetManager.manager_all.register_widget(this);
	}
};

PureFW.TabBox.extend(PureFW.Container);

/********************************************************************
 * Statische Deklarationen
 ********************************************************************/
/*PRIVATE*/  PureFW.TabBox.num_instances = 0;		// Instanzüberwachung (Anzahl)
/*READONLY*/ PureFW.TabBox.instances = new Array(); // Instanzüberwachung

/********************************************************************
 * Prototype-Funktionen
 ********************************************************************/

// Keine direkte Content-Manipulation möglich
PureFW.TabBox.prototype.set_content = null;
PureFW.TabBox.prototype.add_content = null;

/**
 * Fügt einen neuen Tab in die Box hinzu.
 * 
 * @param uint x_offset		Offset in x-Richtung
 * @param uint y_offset		Offset in y-Richtung
 * @param uint button_w		Breite des Buttons, der den Tab aktiviert
 * @param uint button_h		Höhe des Buttons, der den Tab aktiviert
 * @return TabBox_Tab		Den eingefügten Tab
 */
PureFW.TabBox.prototype.add_tab = function(x_offset,y_offset,button_w,button_h) 
{
	var x, i;
	for (i = 0; i < this.tabs.length; i++) {
		this.tabs[i].deactivate();
	}
	if (this.tabs.length <= 0) {
		x = x_offset;
	}
	else {
		x = this.tabs[this.tabs.length-1].get_x() 
			+ this.tabs[this.tabs.length-1].get_width() + x_offset;
	}
	var index = this.tabs.length;
	this.tabs[index] = new PureFW.TabBox_Tab(
			this.get_content_node(), x, 
			y_offset, button_w, button_h, index, this);
	this.tabs[index].activate();
	this.active_tab = index;
	return this.tabs[index];
};

/**
 * Malt die Tabbox neu
 */
PureFW.TabBox.prototype.repaint = function() {
	PureFW.TabBox.parent.repaint.call(this);
	var n = this.tabs.length;
	for (var i = 0; i < n; i++)
		this.tabs[i].repaint();
};

PureFW.TabBox.prototype.scale = function(s) {
	PureFW.TabBox.parent.scale.call(this, s);
	var n = this.tabs.length;
	for (var i = 0; i < n; i++)
		this.tabs[i].scale(s);
};

/**
 * Gibt zurück, welcher Tab in der Liste der Tabs der aktive ist
 * 
 * @return uint
 * @see get_tabs
 */
PureFW.TabBox.prototype.get_active_tab_index = function() {
	return this.active_tab;
};

/**
 * Gibt die Tabs zurück, welche in der Box sind
 * 
 * @return TabBox_Tab[]
 */
PureFW.TabBox.prototype.get_tabs = function() {
	return this.tabs;
};

/**
 * Aktiviert den angegebenen Tab (und deaktiviert alle andere)
 * 
 * @param uint index
 */
PureFW.TabBox.prototype.activate_tab = function(index) {
	var n = this.tabs.length;
	for (var i = 0; i < n; i++) {
		this.tabs[i].deactivate();
	}
	this.tabs[index].activate();
	this.active_tab = index;
};

/**
 * Setzt das horizontale Padding der Inhaltsbox alles Tabs
 * 
 * @param uint padding
 */
PureFW.TabBox.prototype.set_tab_content_padding_horz = function(padding) {
	var n = this.tabs.length;
	for (var i = 0; i < n; i++) {
		this.tabs[i].set_content_padding_horz(padding);
	}
};

/**
 * Hilfsklasse: ein einzelner Tab (Button + Content)
 * 
 * @param HTMLElement parent_node		Wo eingehängt
 * @param uint x						x-Position der Box
 * @param uint y						y-Position   -"-
 * @param uint w						Breite       -"-
 * @param uint h						Höhe         -"-
 * @param uint index					Der wievielte Tab in der Box es ist
 **/
PureFW.TabBox_Tab = function (parent_node, x, y, w, h, index, parent_tabbox) {
	if (typeof(parent_node) !== 'undefined') {
		if (!parent_node) {
			throw new Error('TabBox_Tab: parent_node have to be set in constructor');
		}
		if (!parent_tabbox) {
			throw new Error('TabBox_Tab: parent_tabbox have to be set in constructor');
		}
		this.width = w || 100;
		this.height = h || 20;
		this.position = new PureFW.Point(x || 0, y || 0);
		this.parent_node = parent_node;
		this.parent_tabbox = parent_tabbox;
		this.css_class_suffix = "";
		this.instance_num = this.constructor.num_instances;
		this.instance_name =
			"PureFW.TabBox_Tab.instances["+this.instance_num+"]";
		this.id = 'Tab_'+this.instance_num;
		
		this.init();
		
		this.bg_img_active = null;
		this.bg_img_inactive = null;
		
		this.tab_button = null;
		this.tab_content = null;
		
		this.internal_index = index || 0;
		this.active = false;
		this.padding_h = 0;	// horizontales Padding (left+right)
	
		
		this.insert_into_dom_tree();
		
		this.constructor.instances[this.instance_num] = this;
		this.constructor.num_instances++;
	}
};

/********************************************************************
 * TabBox extends Widget
 ********************************************************************/
PureFW.TabBox_Tab.extend(PureFW.Widget);

/********************************************************************
 * Statische Funktionen
 ********************************************************************/
PureFW.TabBox_Tab.num_instances = 0;
PureFW.TabBox_Tab.instances = new Array(); // Instanzüberwachung

PureFW.TabBox_Tab.tab_button_clicked = function(ev) {
	// this verweist auf den angeklickten Button
	var tab = PureFW.TabBox_Tab.instances[this.tab_instance_num];
	tab.parent_tabbox.activate_tab(tab.internal_index);
};

/********************************************************************
 * Prototype-Funktionen
 ********************************************************************/
PureFW.TabBox_Tab.prototype.insert_into_dom_tree = function() {
	PureFW.TabBox_Tab.parent.insert_into_dom_tree.call(this);

	this.tab_button = new PureFW.Container(this.parent_node, this.get_x(), 
			this.get_y(), this.get_width(), this.get_height());
	this.tab_button.tab_instance_num = this.instance_num;
	this.tab_button.add_event_handler("click", 
			PureFW.TabBox_Tab.tab_button_clicked);
	
	this.tab_content = new PureFW.Container(this.parent_node, 0, 
			this.get_y()+this.get_height(), this.parent_tabbox.get_width(),
			this.parent_tabbox.get_height() - this.get_height());
	this.tab_content.hide();
};

/**
 * Malt den Tab neu
 */
PureFW.TabBox_Tab.prototype.repaint = function() {
	PureFW.TabBox_Tab.parent.repaint.call(this);
};

/**
 * Schreibt den angegebenen Text in den Tab-Button als Beschriftung.
 * 
 * @param string text
 */
PureFW.TabBox_Tab.prototype.set_title = function(text) {
	this.tab_button.set_content(text);
};
/**
 * Setzt den Inhalt des Tabs auf den angebebenen Wert (HTML wird 
 * interpretiert)
 * 
 * @param HTMLString text
 */
PureFW.TabBox_Tab.prototype.set_content = function(text) {
	this.tab_content.set_content(text);
};

/**
 * Setzt die Position des Inhalt-Teils des Tabs
 * 
 * @param uint padding
 */
PureFW.TabBox_Tab.prototype.set_content_padding_horz = function(padding) {
	this.padding_h = padding;
	this.tab_content.set_x(this.get_content_padding_horz());
	this.tab_content.set_width(this.parent_tabbox.get_width()
			- (this.get_content_padding_horz()<<1));
};

/**
 * Gibt das horizontale Padding (left+right) des Kontents zurück
 * 
 * @return uint
 */
PureFW.TabBox_Tab.prototype.get_content_padding_horz = function() {
	return this.padding_h * this.scale_factor;
};

/**
 * Fügt dem Tab ein Ereignis zu. Wird dem Ereignis "button_" vorangestellt, also
 * z.B. "button_click", bezieht sich das Ereignis auf die Schaltfäche, ansonsten
 * auf den Inhaltsanzeigenden Bereich.
 * 
 * @see PureFW.Container.add_event_handler
 */
PureFW.TabBox_Tab.prototype.add_event_handler = function(type, fn) {
	if (type.substr(0, 7) === "button_") {
		this.tab_button.add_event_handler(type.substr(7), fn);
	}
	else {
		this.tab_content.add_event_handler(type, fn);
	}
};

/**
 * Entfernt ein Ereignis. Wird dem Ereignis "button_" vorangestellt, also
 * z.B. "button_click", bezieht sich das Ereignis auf die Schaltfäche, ansonsten
 * auf den Inhaltsanzeigenden Bereich.
 * 
 * @see PureFW.Container.remove_event_handler
 */
PureFW.TabBox_Tab.prototype.remove_event_handler = function(type, fn) {
	if (type.substr(0, 7) === "button_") {
		this.tab_button.remove_event_handler(type.substr(7), fn);
	}
	else {
		this.tab_content.remove_event_handler(type, fn);
	}
};

/**
 * Gibt die ID des Inhalts zurück, so dass der Inhalt direkt modifiziert
 * werden kann.
 * 
 * @return string
 */
PureFW.TabBox_Tab.prototype.get_content_id = function() {
	return this.tab_content.get_content_node().id;
};

/**
 * Gibt die Breite des Reiterknopfes zurück
 * 
 * @param uint
 */
PureFW.TabBox_Tab.prototype.get_button_width = function() {
	return this.get_width();
};

/**
 * Setzt das Bild, das der Tab-Button zeigen soll, wenn der Tab aktiv ist
 * 
 * @param string img
 */
PureFW.TabBox_Tab.prototype.set_active_img = function(img) {
	this.bg_img_active = img;
	if (this.active)
		this.activate();
};

/**
 * Setzt das Bild, das der Tab-Button zeigen soll, wenn der Tab aktiv ist
 * 
 * @param string img
 */
PureFW.TabBox_Tab.prototype.set_inactive_img = function(img) {
	this.bg_img_inactive = img;
	if (!this.active)
		this.deactivate();
};

/**
 * Aktiviert das Tab.
 */
PureFW.TabBox_Tab.prototype.activate = function() {
	this.tab_content.show();
	if (this.bg_img_active)
		this.tab_button.set_bg_img(this.bg_img_active);
	else
		this.tab_button.set_bg_img('');
	this.active = true;
};

/**
 * Deaktiviert das Tab. Es wird kein anderes Tab dafür aktiviert.
 */
PureFW.TabBox_Tab.prototype.deactivate = function() {
	this.tab_content.hide();
	if (this.bg_img_inactive)
		this.tab_button.set_bg_img(this.bg_img_inactive);
	else
		this.tab_button.set_bg_img('');
	this.active = true;
};
