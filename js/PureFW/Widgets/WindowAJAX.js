/**
 * Ein JS-Fenster mit Titlleiste etc. Kann verschoben werden, geschlossen und
 * all der Kram. Lädt den Inhalt per AJAX.
 *
 * @author Phil
 */

/**
 * Constructor
 */

PureFW.WindowAJAX = function(parent_node, x, y, w, h, url, title) {
	if (typeof(this.parent_node) != 'undefined') {
		this.init(parent_node, x, y, w, h);
		this.title = title || '';
		this.bg_img = null;
		this.url = url || '';
		this.frame_size = new PureFW.Point(15,0);	// Breite des Rahmens um den Kontent
		this.button_bar_width = 32;	// Breite des Teils der Titelleiste mit dem X-Knopf
		this.title_bar_height = 32;	// Breite der Titelleiste
	
		this.instance_num = this.constructor.num_instances;
		this.instance_name = "PureFW.WindowAJAX.instances["+this.instance_num+"]";
		
		this.id = "WindowAJAX"+this.instance_num;
		this.bg_img_id = "WindowAJAX_img"+this.instance_num;
		this.content_id = "WindowAJAX_cont"+this.instance_num;
		this.body = null;
		this.title_bar = null;
		this.button_bar = null;
		if (this.parent_node)
			this.insert_into_dom_tree();
		
		this.constructor.num_instances++;
		this.constructor.instances[this.instance_num] = this;
		PureFW.WidgetManager.manager_all.register_widget(this);
	}
};

/********************************************************************
 * WindowAJAX extends Container
 ********************************************************************/
PureFW.WindowAJAX.extend(PureFW.Container);

/********************************************************************
 * Statische Deklarationen
 ********************************************************************/
PureFW.WindowAJAX.num_instances = 0;			// Instanzüberwachung (anzahl)
PureFW.WindowAJAX.instances = new Array();		// Instanzüberwachung (Instanzen)
	
/**
 * Fensterinhalt durch Inhalt der angegebenen Ziel-URL neu füllen (url
 * ansurfen)
 * 
 * @param String str
 * @see PureFW.ContainerAJAX.set_url
 */
PureFW.WindowAJAX.prototype.set_url = function(url) {
	this.body.set_url(url);
};
	
/**
 * Gibt die zuletzt geladene URL zurück
 * 
 * @return String
 * @see PureFW.ContainerAJAX.get_url
 */
PureFW.WindowAJAX.prototype.get_url = function() {
	return this.body.get_url();
}

/**
 * Setzt den Titel des Fensters
 * 
 * @param String str
 */
PureFW.WindowAJAX.prototype.set_title = function(str) {
	this.title = str;
	document.setElemInnerHTML(document.getElemById(this.title_bar.get_id()), str);
};

/**
 * @TODO : Resizing mit Berücksichtigung aller nested Components :)
 */

/**
 * Zeichnet das Fenster neu
 */
PureFW.WindowAJAX.prototype.repaint = function() {
	PureFW.WindowAJAX.parent.repaint.call(this);
	try {
		this.body.repaint();
		this.title_bar.repaint();
		this.button_bar.repaint();
	}
	catch(e) {}
}
/**
 * Fügt das AJAX-Fenster in den DOM-Baum ein.
 */
PureFW.WindowAJAX.prototype.insert_into_dom_tree = function() {
	PureFW.WindowAJAX.parent.insert_into_dom_tree.call(this);
	this.set_z_index(10);

	var this_elem = document.getElemById(this.id);
	
	this.title_bar = new PureFW.Container(
			this_elem,
			this.frame_size.x,
			this.frame_size.y,
			this.width - (this.frame_size.x << 1) - this.button_bar_width,
			this.title_bar_height
	);
	this.title_bar.set_content(this.title);
	this.title_bar.set_font_size(1.1);
	this.title_bar.set_font_weight('bold');
	this.title_bar.set_padding_top(((this.title_bar_height - 14) >> 1) - 1);
	this.title_bar.set_padding_left(this.frame_size.x + 3);
	
	this.button_bar = new PureFW.Container(
			this_elem,
			this.width - 4 -this.button_bar_width,
			1,
			this.button_bar_width,
			this.title_bar_height
	);
	this.button_bar.set_bg_img('../pix/ui/elements/window/close_window.png');
	this.destroy_caller = 
		(function asd(n1){ 
			return function(ev){
					PureFW.WindowAJAX.instances[n1].destroy();
				} 
		})(this.instance_num);
	this.button_bar.add_event_handler('click', this.destroy_caller);
	
	this.body = new PureFW.ContainerAJAX(
			this_elem,
			this.frame_size.x,
			this.frame_size.y + this.title_bar_height,
			this.width - (this.frame_size.x << 1),
			this.height - (this.frame_size.y << 1) - this.title_bar_height,
			this.url
	);
};

/**
 * Setzt den Inhalt (HTML) des Containers. Wird interprete_js angegeben, wird
 * ggf. mitgesandtes JS (innerhalb der Tags <script></script>) interpretiert!
 * 
 * @param string text
 * @param bool interprete_js [=false]
 * @see PureFW.Container#set_content
 */
PureFW.WindowAJAX.prototype.set_content = function(content, interprete_js) {
	this.body.set_content(content, interprete_js);
}

/**
 * Disables the action on clicking the close button, removes the event
 * handler. Can only be done once and cannot be undone.
 */

PureFW.WindowAJAX.prototype.disable_close = function() 
{
	this.button_bar.remove_event_handler('click', this.destroy_caller);
	this.button_bar.hide();
}

/**
 * Zerstört das Fenster
 * @see Widget.destroy
 */
PureFW.WindowAJAX.prototype.destroy = function() {
	this.title_bar.destroy();
	this.button_bar.destroy();
	this.body.destroy();
	PureFW.WindowAJAX.parent.destroy.call(this);
}

/**
 * EventHandler hinzufügen (überladen, um das "change"-Event zu erlauben).
 */
PureFW.WindowAJAX.prototype.add_event_handler = function(type, fn) {
	if (type === "change") {
		this.body.add_event_handler(type, fn);
	}
	else {
		PureFW.WindowAJAX.parent.add_event_handler.call(this, type, fn);
	}
}
