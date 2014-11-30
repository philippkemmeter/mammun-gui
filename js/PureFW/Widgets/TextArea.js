/**
 * Ein TextArea, die Text enthalten und durchscrollen kann.
 * 
 * @author Alex
 * 
 * @param HTMLElement parent_node
 * @param int x
 * @param int y
 * @param uint w	(wenn <= 0, dann wird sie dynamisch bestimmt)
 * @param uint h	(wenn <= 0, dann wird sie dynamisch bestimmt)
 * @param bool register_widget		Ob das Widget bei manager_all registriert
 * 									werden soll (default: TRUE)
 * 
 * FYI: This version of the TextArea will have major problems later with 
 * resizing. The node used to contain the text might cause scaling issues.
 */
PureFW.TextArea = function(parent_node, x, y, w, h) { 
	if (typeof(parent_node) !== 'undefined')
	{
		this.init(parent_node, x, y, w, h);
	
		this.instance_num = this.constructor.num_instances;
		this.instance_name = "PureFW.TextArea.instances["
														+this.instance_num+"]";
		this.id = "TextArea"+this.instance_num;
		this.bg_img_id = "TextArea_bg_img"+this.instance_num;
		this.inner_container_id = "TextScrollContent"+this.instance_num;
		this.vertical_scroller_id = "TextVertiScroller"+this.instance_num;
		this.horizontal_scroller_id = "TextHorizonScroller"+this.instance_num;
		this.text_id = "TextAreaContent"+this.instance_num;

		this.insert_into_dom_tree();
		this.constructor.instances[this.instance_num] = this;
		this.constructor.num_instances++;
		
		PureFW.WidgetManager.manager_all.register_widget(this);
	}
};
/********************************************************************
 * TextArea Bar extends Container
 ********************************************************************/
PureFW.TextArea.extend(PureFW.ScrollContainer);

/********************************************************************
 * Statische Deklarationen
 ********************************************************************/
PureFW.TextArea.num_instances = 0;
PureFW.TextArea.instances = new Array();

/********************************************************************
 * Prototyp-Deklarationen
 ********************************************************************/
PureFW.TextArea.prototype.init = function(parent_node, x, y, w, h) {
	PureFW.TextArea.parent.init.call(this, parent_node, x, y, w, h);
	
	this.text_node = null;
};

PureFW.TextArea.prototype.insert_into_dom_tree = function()
{
	PureFW.TextArea.parent.insert_into_dom_tree.call(this);
	
	this.text_node = document.createElement('textarea');
	this.text_node.style.position = 'absolute';
	this.text_node.style.width = this.width - 10+'px';
	this.text_node.style.height = this.height - 10+'px';
	this.text_node.style.border = "0";
	this.text_node.style.backgroundColor = 'transparent';
	this.text_node.setAttribute("onkeyup",
			"javascript: " + this.instance_name + ".key_up(event);");
	this.text_node.setAttribute("onkeydown",
			"javascript: " + this.instance_name + ".key_down(event);");
	this.text_node.setAttribute("onchange",
			"javascript: " + this.instance_name + ".keep_one_space_ie(event);");
	this.text_node.id = this.text_id;
	
	this.get_content_node().appendChild(this.text_node);
};

/**
 * Setzt den Inhalt des Textfeldes auf den angegebenen Wert.
 * 
 * @param string content
 */
PureFW.TextArea.prototype.set_content = function(content) {
	document.setElemInnerHTML(this.get_text_element(), content);
	this.get_text_element().value = content;
	
	this.keep_one_space_ie();
};

/**
 * Fügt den angegebenen Inhalt zum Inhalt des Textfeldes hinzu.
 * 
 * @param string content
 */
PureFW.TextArea.prototype.add_content = function(content) {
	document.setElemInnerHTML(this.get_text_element(), 
		this.get_text_element().innerHTML+content);
	this.get_text_element().value = content;
};

/**
 * Gibt den Inhalt des Textfeldes wider.
 */
PureFW.TextArea.prototype.get_text = function()
{
	return this.get_text_element().value;
};

/**
 * Gibt den Inhalt des Textfeldes wider.
 * 
 * @see TextArea#get_text
 */
PureFW.TextArea.prototype.get_content = function()
{
	return this.get_text();
};


/**
 * Setzt das Name-Attribut des Textfeldes auf den angegebenen Wert. Das kann
 * für Formulare interessant sein.
 * 
 * @param name
 */
PureFW.TextArea.prototype.set_name = function(name) {
	this.text_node.setAttribute("name", name);
}

PureFW.TextArea.prototype.key_up = function(ev) 
{
	try 
	{
		ev = PureFW.EventUtil.formatEvent(ev); 
	} 
	catch(e){}
	// Tastendruck nicht weitergeben an z.B. die Karte, damit das Tippen
	// keine Auswirkungen auf die Steuerung hat
	ev.stopPropagation();

	// Größe anpassen

	if (parseInt(this.text_node.scrollHeight) > this.height) // on overflow
	{
		this.text_node.style.height = parseInt(this.text_node.scrollHeight)+"px";
		this.set_inner_height(parseInt(this.text_node.style.height));
		this.scroll_down(20);
	}
	else // when text fits inside the window BUGGED!!!
	{
		this.text_node.style.height = this.height+"px";
		this.set_inner_height(this.height);
		this.scroll_up(20);
	}
	
}

PureFW.TextArea.prototype.key_down = function(ev) 
{
	try 
	{ 
		ev = PureFW.EventUtil.formatEvent(ev); 
	} 
	catch(e){}
	// Tastendruck nicht weitergeben an z.B. die Karte, damit das Tippen
	// keine Auswirkungen auf die Steuerung hat
	ev.stopPropagation();

	// Größe anpassen
	if (parseInt(this.text_node.scrollHeight) > this.height)
		this.text_node.style.height = parseInt(this.text_node.scrollHeight)+"px";
	else
		this.text_node.style.height = this.height+"px";
	this.set_inner_height(parseInt(this.text_node.style.height));
}

/**
 * Diese Funktion ist ein hässlicher Hack für den IE, der Textareas mit 
 * transparentem Hintergrund nicht anklickbar macht, wenn nichts drin steht.
 * 
 * Deswegen wird hier ein Space hinzugefügt, wenn IE und leer, damit immer
 * was drinsteht.
 * 
 * @param Event ev
 */
PureFW.TextArea.prototype.keep_one_space_ie = function(ev) {
	if ((this.get_content() == '')
		&& (navigator.userAgent.toLowerCase().indexOf("explorer") > -1))
	{
		this.set_content(' ');
	}
}

PureFW.TextArea.prototype.get_text_element = function()
{
	if (document.getElemById(this.text_id)) {
		this.text_node = document.getElemById(this.text_id);
	}
	return this.text_node;
};