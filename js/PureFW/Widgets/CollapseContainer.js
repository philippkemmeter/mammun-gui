/**
 * A CollapseContainer contains two parts, a root and a body. The body can 
 * contain text, images, or another CollapseContainer.
 * 
 * @author Alex
 */

/**
 * @param HTMLElement parent_node
 * @param int x
 * @param int y
 * @param uint w	(wenn <= 0, dann wird sie dynamisch bestimmt)
 * @param uint h	(wenn <= 0, dann wird sie dynamisch bestimmt)
 * @param bool register_widget		Ob das Widget bei manager_all registriert
 * 									werden soll (default: TRUE)
 */
PureFW.CollapseContainer = function(parent_node, x, y, w, h) {
	if (typeof(parent_node) !== 'undefined') {
		this.init(parent_node, x, y, w, h);
	
		this.instance_num = this.constructor.num_instances;
		this.instance_name = "PureFW.CollapseContainer.instances["+this.instance_num+"]";
		
		this.id = "CollapseContainer"+this.instance_num;
		this.bg_img_id = "CollapseContainer_bg_img"+this.instance_num;
		this.content_id = "CollapseContainer_cont"+this.instance_num;
		
		this.insert_into_dom_tree();
		this.constructor.num_instances++;
		this.constructor.instances[this.instance_num] = this;
		
		PureFW.WidgetManager.manager_all.register_widget(this);
	}
};
/********************************************************************
 * CollapseContainer Bar extends Widget
 ********************************************************************/
PureFW.CollapseContainer.extend(PureFW.Container);

/********************************************************************
 * Statische Deklarationen
 ********************************************************************/
PureFW.CollapseContainer.num_instances = 0;
PureFW.CollapseContainer.instances = new Array();

/********************************************************************
 * Prototyp-Deklarationen
 ********************************************************************/
PureFW.CollapseContainer.prototype.init = function(parent_node, x, y, w, h) {
	PureFW.CollapseContainer.parent.init.call(this, parent_node, x, y, w, h);
	this.on_collapse_functions = new Array();
	this.on_expand_functions = new Array();
	this.arrow_button = null;
	this.root_container = null;
	this.body_container = null;
	
	this.big_arrow_url = 'ui/backgrounds/faq/arrows/arrow_big_';
	this.small_arrow_url = 'ui/backgrounds/faq/arrows/arrow_small_';
	this.current_arrow_url = this.big_arrow_url;
};

PureFW.CollapseContainer.prototype.insert_into_dom_tree = function() {
	PureFW.CollapseContainer.parent.insert_into_dom_tree.call(this);

	this.get_node().style.display = 'block';
	
	this.arrow_button = new PureFW.Image(
		this,
		0, 0,
		17, 11);
	this.arrow_button.set_pic_url(this.current_arrow_url + 'down.png');
	this.arrow_button.set_positioning('relative');
	this.arrow_button.set_float("left");
	this.arrow_button.parent_container = this;
	this.arrow_button.add_event_handler(
		"click",
		(function(_instance) {
			return function(ev) {
				_instance.toggle_expand_collapse();
			}
		})(this)
	);
	
	this.root_container = new PureFW.Container(
		this,
		0, 0,
		this.width - this.arrow_button.width, 20);
	this.root_container.set_positioning('relative');
	this.root_container.set_float('left');
	this.root_container.parent_container = this;
	this.root_container.add_event_handler(
		"click",
		(function(_instance) {
			return function(ev) {
				_instance.toggle_expand_collapse();
			}
		})(this)
	);
	
	this.body_container = new PureFW.Container(
		this,
		0, 0,
		this.width - this.arrow_button.width, 0);
//	this.body_container.set_bg_color("blue");
	this.body_container.set_positioning('relative');
	this.body_container.set_float('left');
	this.body_container.set_x(20);
	this.body_container.parent_container = this;
	
	this.collapse_body();
	this.get_node().style.clear='both';
};

/**
 * Returns the content_node of the body Container. This function is used as the 
 * parent_node when placing something into the CollapseContainer. 
 */
PureFW.CollapseContainer.prototype.get_body = function()
{
	return this.body_container.get_content_node();
}

/**
 * Calls expend, when collapsed and the other way around.
 */
PureFW.CollapseContainer.prototype.toggle_expand_collapse = function() {
	if (this.body_container.is_hidden())
		this.expand_body();
	else
		this.collapse_body();
}

/**
* Expands the body of the Container, showing all elements inside.
*/
PureFW.CollapseContainer.prototype.expand_body = function()
{
	this.arrow_button.set_pic_url(this.current_arrow_url + 'up.png');
	this.body_container.show();
	this.on_expand();
}

/**
* Collapses the body of the Container, hiding all elements inside. The stuff
* placed underneath should move itself up to compensate for the newly created
* empty space.
*/
PureFW.CollapseContainer.prototype.collapse_body = function()
{
	this.arrow_button.set_pic_url(this.current_arrow_url + 'down.png');
	this.body_container.hide();
	this.on_collapse();
}

/**
* 
*/
PureFW.CollapseContainer.prototype.set_arrow = function(arrow)
{
	if(arrow == 'small')
	{
		this.current_arrow_url = this.small_arrow_url;
		this.arrow_button.set_width(13);
		this.arrow_button.set_height(9);
		this.arrow_button.set_y(3);
	}
	else if(arrow == 'big')
	{
		this.current_arrow_url = this.big_arrow_url;
		this.arrow_button.set_width(17);
		this.arrow_button.set_height(11);
		this.arrow_button.set_y(0);
	}
}

/**
 * Setzt den Inhalt (HTML) des CollapseContainers. Wird interprete_js angegeben, 
 * wird ggf. mitgesandtes JS (innerhalb der Tags <script></script>) 
 * interpretiert!
 * 
 * @param string text
 * @param bool interprete_js [=false]
 */
PureFW.CollapseContainer.prototype.set_content = function(text, interprete_js) {
	this.on_change(this.create_event("change"));
	document.setElemInnerHTML(this.root_container.get_content_node(), 
			text, interprete_js);
};

/**
 * Fügt dem CollapseContainer Inhalt (HTML ohne Javascript) hinzu.
 * 
 * @param string text
 */
PureFW.CollapseContainer.prototype.add_content = function(text) {
	this.on_change(this.create_event("change"));
	document.setElemInnerHTML(this.root_container.get_content_node(), 
			this.root_container.get_content_node().innerHTML + text);
};
 
/**
 * Gibt den Inhalt (HTML) des CollapseContainers zurück.
 * 
 * @return string
 */
PureFW.CollapseContainer.prototype.get_content = function() {
	if (document.getElemById(this.content_id))
		this.root_container.content_node = document.getElemById(this.content_id);
	if(document.getElemById(this.id))
		this.node = document.getElemById(this.id);
	if (this.root_container.content_node)
	{
		return this.root_container.content_node.innerHTML;
	}
	else
	{
		return this.root_container.node.innerHTML;
	}
};

/**
 * Fügt einen EventHandler hinzu. Funktion überschrieben, da zusätzlich
 * die Typen "collapse" und "expand" unterstützt werden.
 * 
 * @param string type
 * @param Function fn
 * @see Widget.add_event_handler
 */
PureFW.CollapseContainer.prototype.add_event_handler = function(type, fn) {
	if (type === "collapse")
		this.on_collapse_functions.push(fn);
	else if (type === "expand")
		this.on_expand_functions.push(fn);
	else
		PureFW.Container.parent.add_event_handler.call(this, type, fn);
};

/**
 * Entfernt einen EventHandler. Funktion überschrieben, da zusätzlich
 * die Typen "collapse" und "expand" unterstützt werden.
 * 
 * @param string type
 * @param Function fn
 * @see Container.prototype.remove_event_handler
 */
PureFW.CollapseContainer.prototype.remove_event_handler = function(type, fn) {
	if (type === "collapse")
		this.on_collapse_functions.remove(fn);
	else if (type === "expand")
		this.on_expand_functions.remove(fn);
	else
		PureFW.Container.parent.remove_event_handler.call(this, type, fn);
};

/**
 * Ruft das "on-collapse"-Event exlizit auf. Wird normalerweise aufgerufen,
 * wenn der der CollapseContainer "eingeklappt", sein Body als unsichtbar wird.
 * 
 * @param Event ev [optional]
 */
PureFW.CollapseContainer.prototype.on_collapse = function(ev) {
	if (!ev) {
		ev = this.create_event("collapse");
	}
	var n = this.on_collapse_functions.length;
	for (var i = 0; i < n; i++) {
		this.on_collapse_functions[i].call(this, ev);
	}
};

/**
 * Ruft das "on-expand"-Event exlizit auf. Wird normalerweise aufgerufen,
 * wenn der der CollapseContainer "ausgefahren", sein Body als sichtbar wird.
 * 
 * @param Event ev [optional]
 */
PureFW.CollapseContainer.prototype.on_expand = function(ev) {
	if (!ev) {
		ev = this.create_event("expand");
	}
	var n = this.on_expand_functions.length;
	for (var i = 0; i < n; i++) {
		this.on_expand_functions[i].call(this, ev);
	}
};


/**
 * Zerstört das Widget und alle enthaltenen Widgets.
 */
PureFW.CollapseContainer.prototype.destroy = function() {
	while (this.on_expand_functions.length)
		this.on_expand_functions.pop();
	while (this.on_collapse_functions.length)
		this.on_collapse_functions.pop();
	PureFW.Container.parent.destroy.call(this);
};