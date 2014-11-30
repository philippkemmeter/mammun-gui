/**
 * Ein Container, der seinen Inhalt per AJAX laden kann.
 * 
 * @author Phil
 */

/**
 * @param HTMLElement parent_node
 * @param int x
 * @param int y
 * @param uint w
 * @param uint h
 * @param string url
 */
PureFW.ContainerAJAX = function(parent_node, x, y, w, h, url, no_scale) {
	if (typeof(parent_node) !== 'undefined') {
		this.init(parent_node, x, y, w, h, no_scale);
		this.url = url || '';
		
		this.instance_num = this.constructor.num_instances;
		this.instance_name = "PureFW.ContainerAJAX.instances["+this.instance_num+"]";
		this.id = "ContainerAJAX"+this.instance_num;
		this.bg_img_id = "ContainerAJAX_bg_img"+this.instance_num;
		this.content_id = "ContainerAJAX_cont"+this.instance_num;
		
		this.insert_into_dom_tree();
		if (this.url)
			this.load_content();
		this.constructor.instances[this.instance_num] = this;
		this.constructor.num_instances++;
		PureFW.WidgetManager.manager_all.register_widget(this);
	}
};
/********************************************************************
* ContainerAJAX extends Container
********************************************************************/
PureFW.ContainerAJAX.extend(PureFW.Container);

/********************************************************************
 * Statische Deklarationen
 ********************************************************************/
PureFW.ContainerAJAX.num_instances = 0;
PureFW.ContainerAJAX.instances = new Array();

PureFW.ContainerAJAX.fetch_request_result = function(response_arr) {
	if ((typeof(response_arr) != 'object') || response_arr.constructor != Array)
		throw response_arr;
	if (isNaN(response_arr[0])) {
		throw response_arr;
	}
	var instance_num = parseInt(response_arr[0]);
	PureFW.ContainerAJAX.instances[instance_num].set_content(response_arr[1], 	
			true);
	PureFW.ContainerAJAX.instances[instance_num].on_load(
			PureFW.ContainerAJAX.instances[instance_num].create_event("load")
	);
};

/********************************************************************
 * Prototype-Deklarationen
 ********************************************************************/
PureFW.ContainerAJAX.prototype.init = function(parent_node, x, y, w, h,
	no_scale) 
{
	PureFW.ContainerAJAX.parent.init.call(this, parent_node, x, y, w, h, 
		no_scale);
	this.on_load_functions = new Array();
	this.bg_img = null;
};
/**
 * Setzt eine URL, von welcher aus der Inhalt des Containers geladen werden 
 * soll.
 * 
 * @param string URL
 */
PureFW.ContainerAJAX.prototype.set_url = function(url) {
	this.url = url;
	this.load_content();
};

/**
 * Gibt die URL zurück, deren Inhalt aktuell geladen ist.
 * 
 * @return string
 */
PureFW.ContainerAJAX.prototype.get_url = function() {
	return this.url;
};
 
/**
 * Lädt den Inhalt - sprich, es wird das geladen, was die URL vorgibt.
 */
PureFW.ContainerAJAX.prototype.load_content = function() {
	if (this.url) {
		this.set_content('&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;loading...');
		var url = (this.url.indexOf('?') == -1) ? this.url+'?' : this.url+'&';
		PureFW.AJAXClientServer.send_request(url+'ajaxid='+this.instance_num, 
			 	this.constructor.fetch_request_result);
	}
};

/**
 * Ruft das "on-load"-Event exlizit auf. Wird normalerweise aufgerufen,
 * wenn der Inhalt fertig geladen ist.
 * 
 * @param Event ev [optional]
 */
PureFW.ContainerAJAX.prototype.on_load = function(ev) {
	if (!ev) {
		this.create_event("load")
	}
	var n = this.on_load_functions.length;
	for (var i = 0; i < n; i++) {
		this.on_load_functions[i].call(this, ev);
	}
};

/**
 * Fügt einen EventHandler hinzu. Funktion überschrieben, da zusätzlich
 * der Typ "load" unterstützt wird.
 * 
 * @param string type
 * @param Function fn
 * @see Widget.add_event_handler
 */
PureFW.ContainerAJAX.prototype.add_event_handler = function(type, fn) {
	if (type === "load")
		this.on_load_functions.push(fn);
	else
		PureFW.ContainerAJAX.parent.add_event_handler.call(this, type, fn);
};

/**
 * Entfernt einen EventHandler. Funktion überschrieben, da zusätzlich
 * der Typ "load" unterstützt wird.
 * 
 * @param string type
 * @param Function fn
 * @see ContainerAJAX.prototype.remove_event_handler
 */
PureFW.ContainerAJAX.prototype.remove_event_handler = function(type, fn) {
	if (type === "load")
		this.on_load_functions.remove(fn);
	else
		PureFW.ContainerAJAX.parent.remove_event_handler.call(this, type, fn);
};	

/**
 * Zerstört den ContainerAJAX
 */
PureFW.ContainerAJAX.prototype.destroy = function() {
	this.constructor.instances.remove(this);
	PureFW.ContainerAJAX.parent.destroy.call(this);
	this.on_load_functions.destroy();
}
