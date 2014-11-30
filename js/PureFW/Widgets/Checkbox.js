/**
 * Eine Checkbox. Sie kann angehakt oder nicht angehakt sein und beim 
 * Statuswechsel Events feuern.
 * 
 * @author Phil
 */

/**
 * @param HTMLElement parent_node
 * @param int x
 * @param int y
 * @param uint w
 * @param uint h
 * @param string pic_url	Pfad zum Checkbox-Doppelbild [oben unchecked, 
 * 															unten checked]
 */
PureFW.Checkbox = function(parent_node, x, y, w, h, pic_url, no_scale) {
	if (typeof(parent_node) !== 'undefined') {
		this.init(parent_node, x, y, w, h, pic_url, no_scale);
	
		this.instance_num = this.constructor.num_instances;
		this.instance_name = "PureFW.Checkbox.instances["+this.instance_num+"]";
		
		this.id = "Checkbox"+this.instance_num;
		this.pic_id = "Checkbox_pic"+this.instance_num;
		
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
PureFW.Checkbox.extend(PureFW.Widget);

/********************************************************************
 * Statische Deklarationen
 ********************************************************************/
PureFW.Checkbox.num_instances = 0;
PureFW.Checkbox.instances = new Array();

/********************************************************************
 * Prototyp-Deklarationen
 ********************************************************************/
PureFW.Checkbox.prototype.init = function(parent_node, x, y, w, h, pic_url,
	no_scale) 
{
	PureFW.Checkbox.parent.init.call(this, parent_node, x, y, w, h, no_scale);
	this.pic_url = pic_url;
	this.on_check_functions = new Array();
	this.on_uncheck_functions = new Array();
	this.node = null;
	this.pic_node = null;
	this.checked = false;
};

PureFW.Checkbox.prototype.insert_into_dom_tree = function() {
	PureFW.Checkbox.parent.insert_into_dom_tree.call(this);

	this.node = document.createElement('div');
	this.node.style.position = 'absolute';
	this.node.style.width = this.get_width()+'px';
	this.node.style.height = this.get_height()+'px';
	this.node.style.left = this.get_x()+'px';
	this.node.style.top = this.get_y()+'px';
	this.node.style.backgroundRepeat = 'no-repeat';
	this.node.style.backgroundPosition = 'top left';
	this.node.style.overflow = 'hidden';
	this.node.id = this.id;
	
	this.pic_node = document.createElement('img');
	this.pic_node.style.position = 'absolute';
	this.pic_node.style.width = '100%';
	this.pic_node.style.height = '200%';
	this.pic_node.style.left = 0;
	this.pic_node.style.top = 0;
	this.pic_node.src = this.add_pic_path(this.pic_url);
	this.pic_node.id = this.pic_id;
	
	this.parent_node.appendChild(this.node);
	this.node.appendChild(this.pic_node);
	
	this.add_event_handler(
		"click",
		(function(instance) {
			return function(ev) {
				instance.toggle()
			}
		})(this)
	);
};

PureFW.Checkbox.prototype.get_pic_node = function() {
	if (!this.pic_id && this.pic_node)
		this.pic_id = this.pic_node.id;
	
	if (this.pic_id) {
		var tmp = document.getElemById(this.pic_id);
		if (tmp)
			this.pic_node = tmp;
	}
	return this.pic_node;
};


/**
 * Setzt die URL zum Bild, das dieses Widget repräsentieren werden soll.
 * 
 * @param string pic_url
 */
PureFW.Checkbox.prototype.set_pic_url = function(pic_url) {
	this.pic_url = pic_url;
	this.get_pic_node().src = this.add_pic_path(this.pic_url);
};

/**
 * Hakt die Kiste an.
 */
PureFW.Checkbox.prototype.check = function() {
	if (!this.activated)
		return;
	this.get_pic_node().style.top = -this.get_height()+"px";
	this.checked = true;
	this.on_check();
};

/**
 * Hakt die Kiste ab/aus oder wie auch immer.
 */
PureFW.Checkbox.prototype.uncheck = function() {
	if (!this.activated)
		return;
	this.get_pic_node().style.top = 0;
	this.checked = false;
	this.on_uncheck();
};

/**
 * Hakt die Kiste an, wenn sie nicht angehakt ist, ansonsten wird die Kiste
 * in den unangehakten Zustand überführt.
 */
PureFW.Checkbox.prototype.toggle = function() {
	if (this.checked)
		this.uncheck();
	else
		this.check();
};

/**
 * Gibt zurück, ob die Box gerade angehakt ist.
 * 
 * @return bool
 */
PureFW.Checkbox.prototype.is_checked = function() {
	return this.checked;
};

/**
 * Fügt einen EventHandler hinzu. Funktion überschrieben, da zusätzlich
 * die Typen "check" und "uncheck" unterstützt werden, welche beim Anhaken
 * bzw. Abhaken ausgelöst werden.
 * 
 * @param string type
 * @param Function fn
 * @see PureFW.Container#add_event_handler
 */
PureFW.Checkbox.prototype.add_event_handler = function(type, fn) {
	if (type === "check")
		this.on_check_functions.push(fn);
	else if (type === "uncheck")
		this.on_uncheck_functions.push(fn);
	else
		PureFW.Image.parent.add_event_handler.call(this, type, fn);
};

/**
 * Entfernt einen EventHandler. Funktion überschrieben, da zusätzlich
 * die Typen "check" und "uncheck" unterstützt werden, welche beim Anhaken
 * bzw. Abhaken ausgelöst werden.
 * 
 * @param string type
 * @param Function fn
 * @see PureFW.Container#remove_event_handler
 */
PureFW.Checkbox.prototype.remove_event_handler = function(type, fn) {
	if (type === "check")
		this.on_check_functions.remove(fn);
	else if (type === "uncheck")
		this.on_uncheck_functions.remove(fn);
	else
		PureFW.Image.parent.remove_event_handler.call(this, type, fn);
};

/**
 * Ruft das "on-check"-Event exlizit auf. Wird normalerweise aufgerufen,
 * wenn die Checkbox angehakt wird.
 * 
 * @param Event ev [optional]
 */
PureFW.Checkbox.prototype.on_check = function(ev) {
	if (!ev) {
		ev = this.create_event("check");
	}
	var n = this.on_check_functions.length;
	for (var i = 0; i < n; i++) {
		this.on_check_functions[i].call(this, ev);
	}
};

/**
 * Ruft das "on-uncheck"-Event exlizit auf. Wird normalerweise aufgerufen,
 * wenn die Checkbox unangehakt wird.
 * 
 * @param Event ev [optional]
 */
PureFW.Checkbox.prototype.on_uncheck = function(ev) {
	if (!ev) {
		ev = this.create_event("uncheck");
	}
	var n = this.on_uncheck_functions.length;
	for (var i = 0; i < n; i++) {
		this.on_uncheck_functions[i].call(this, ev);
	}
};

/**
 * Zerstört das Widget und alle enthaltenen Widgets.
 */
PureFW.Checkbox.prototype.destroy = function() {
	while (this.on_check_functions.length)
		this.on_check_functions.pop();
	while (this.on_uncheck_functions.length)
		this.on_uncheck_functions.pop();
	PureFW.Checkbox.parent.destroy.call(this);
};
