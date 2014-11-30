/**
 * Ein Bild.
 * 
 * @author Phil
 */

/**
 * @param HTMLElement parent_node
 * @param int x
 * @param int y
 * @param uint w	(wenn <= 0, dann wird sie dynamisch bestimmt)
 * @param uint h	(wenn <= 0, dann wird sie dynamisch bestimmt)
 */
PureFW.Image = function(parent_node, x, y, w, h, pic_url, no_scale) {
	if (typeof(parent_node) !== 'undefined') {
		this.init(parent_node, x, y, w, h, pic_url, no_scale);
	
		this.instance_num = this.constructor.num_instances;
		this.instance_name = "PureFW.Image.instances["+this.instance_num+"]";
		
		this.id = "Image"+this.instance_num;
		
		this.insert_into_dom_tree();
		this.constructor.num_instances++;
		this.constructor.instances[this.instance_num] = this;
		
		if (!this.no_scale)
			PureFW.WidgetManager.manager_all.register_widget(this);
	}
};
/********************************************************************
 * Container Bar extends Widget
 ********************************************************************/
PureFW.Image.extend(PureFW.Widget);

/********************************************************************
 * Statische Deklarationen
 ********************************************************************/
PureFW.Image.num_instances = 0;
PureFW.Image.instances = new Array();

/********************************************************************
 * Prototyp-Deklarationen
 ********************************************************************/
PureFW.Image.prototype.init = function(parent_node, x, y, w, h, pic_url, 
	no_scale) 
{
	PureFW.Image.parent.init.call(this, parent_node, x, y, w, h, no_scale);
	this.on_change_functions = new Array();
	this.pic_url = pic_url || '';
};

PureFW.Image.prototype.insert_into_dom_tree = function() {
	PureFW.Image.parent.insert_into_dom_tree.call(this);

	this.node = document.createElement('img');
	this.node.style.position = 'absolute';
	this.node.style.width = this.get_width()+'px';
	this.node.style.height = this.get_height()+'px';
	this.node.style.left = this.get_x()+'px';
	this.node.style.top = this.get_y()+'px';
	this.node.style.backgroundRepeat = 'no-repeat';
	this.node.style.backgroundPosition = 'top left';
	this.node.style.overflow = 'hidden';
	this.node.src = this.add_pic_path(this.pic_url);
	this.node.id = this.id;
	
	this.parent_node.appendChild(this.node);
};

/**
 * Setzt die URL zum Bild, das dieses Widget repräsentieren werden soll.
 * 
 * @param string pic_url
 */
PureFW.Image.prototype.set_pic_url = function(pic_url) {
	if (this.pic_url == pic_url)
		return;
	this.pic_url = pic_url;
	if (this.get_node()) {
		this.get_node().src = this.add_pic_path(this.pic_url);
		this.on_change();
	}
}

/**
 * Gibt die URL zum Bild zurück, das dieses Widget repräsentiert.
 * 
 * @return string
 */
PureFW.Image.prototype.get_pic_url = function() {
	return this.pic_url;
}

/**
 * Fügt einen EventHandler hinzu. Funktion überschrieben, da zusätzlich
 * der Typ "change" unterstützt wird, welcher beim Verändern des Inhalts
 * ausgelöst wird (durch set_content oder add_content).
 * 
 * @param string type
 * @param Function fn
 * @see Widget.add_event_handler
 */
PureFW.Image.prototype.add_event_handler = function(type, fn) {
	if (type === "change")
		this.on_change_functions.push(fn);
	else
		PureFW.Image.parent.add_event_handler.call(this, type, fn);
};

/**
 * Entfernt einen EventHandler. Funktion überschrieben, da zusätzlich
 * der Typ "change" unterstützt wird, welcher beim Verändern des Inhalts
 * ausgelöst wird (durch set/add_content).
 * 
 * @param string type
 * @param Function fn
 * @see Container.prototype.remove_event_handler
 */
PureFW.Image.prototype.remove_event_handler = function(type, fn) {
	if (type === "change")
		this.on_change_functions.remove(fn);
	else
		PureFW.Image.parent.remove_event_handler.call(this, type, fn);
};

/**
 * Ruft das "on-change"-Event exlizit auf. Wird normalerweise aufgerufen,
 * wenn der Inhalt durch set_content oder add_content verändert wird.
 * 
 * @param Event ev [optional]
 */
PureFW.Image.prototype.on_change = function(ev) {
	if (!ev) {
		ev = this.create_event("change");
	}
	var n = this.on_change_functions.length;
	for (var i = 0; i < n; i++) {
		this.on_change_functions[i].call(this, ev);
	}
};

/**
 * Zerstört das Widget und alle enthaltenen Widgets.
 */
PureFW.Image.prototype.destroy = function() {
	while (this.on_change_functions.length)
		this.on_change_functions.pop();
	PureFW.Image.parent.destroy.call(this);
};
