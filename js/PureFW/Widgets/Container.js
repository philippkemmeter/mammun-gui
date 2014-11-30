/**
 * Ein Container, der HTML oder Widgets enthalten kann.
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
PureFW.Container = function(parent_node, x, y, w, h, no_scale) {
	if (typeof(parent_node) !== 'undefined') {
		this.init(parent_node, x, y, w, h, no_scale);
	
		this.instance_num = this.constructor.num_instances;
		this.instance_name = "PureFW.Container.instances["+this.instance_num+"]";
		
		this.id = "Container"+this.instance_num;
		this.bg_img_id = "Container_bg_img"+this.instance_num;
		this.content_id = "Container_cont"+this.instance_num;
		
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
PureFW.Container.extend(PureFW.Widget);

/********************************************************************
 * Statische Deklarationen
 ********************************************************************/
PureFW.Container.num_instances = 0;
PureFW.Container.instances = new Array();

/********************************************************************
 * Prototyp-Deklarationen
 ********************************************************************/
PureFW.Container.prototype.init = function(parent_node, x, y, w, h, no_scale)
{
	PureFW.Container.parent.init.call(this, parent_node, x, y, w, h, no_scale);
	this.on_change_functions = new Array();
	this.bg_img = null;
	this.bg_img_node = null;
	this.node = null;
	this.content_node = null;
	
	this.added_widgets = new Array();
};

PureFW.Container.prototype.insert_into_dom_tree = function() {
	PureFW.Container.parent.insert_into_dom_tree.call(this);

	this.node = document.createElement('div');
	this.node.style.position = 'absolute';
	if (this.width > 0)
		this.node.style.width = this.get_width()+'px';
//	alert(this.instance_name+".height = "+this.height);
	if (this.height > 0)
		this.node.style.height = this.get_height()+'px';
	this.node.style.left = this.get_x()+'px';
	this.node.style.top = this.get_y()+'px';
	this.node.style.backgroundRepeat = 'no-repeat';
	this.node.style.backgroundPosition = 'top left';
	this.node.style.overflow = 'hidden';
	this.node.id = this.id;
	
	this.parent_node.appendChild(this.node);
};

/**
 * Setzt die CSS-Overflow Eigenschaft. Gültige Werte sind 'visible', 'hidden',
 * 'scroll' und 'auto'.
 * 
 * @param String s
 */
PureFW.Container.prototype.set_overflow = function(s) {
	this.get_node().style.overflow = s;
	this._get_content_node().style.overflow = s;
}

/**
 * Setzt die Breite des Containers. Wenn w <= 0, dann wird sie dynamisch zur
 * Laufzeit anhand des Inhalts bestimmt.
 * 
 * @param int w
 * @param uint ms			Positionssetzung soll animiert werden mit
 * 							Motiontweening und so viele Millisekunden lang
 * 							dauern.
 * @param Function easing	Funktion zum glätten der Animation
 * @param Function on_finish Funktion, die am Ende der Animation aufgerufen
 * 							 werden soll
 */
PureFW.Container.prototype.set_width = function(w, ms, easing, on_finish) {
	if ((w > 0) || ms)
		PureFW.Widget.set_width.call(this, w, ms, easing, on_finish);
	else {
		this.width = w;
		this.get_node().style.width = '';
	}
};

/**
 * Gibt die Breite des Containers zurück.
 * 
 * @return uint
 */
PureFW.Container.prototype.get_width = function() {
	if (this.width > 0)
		return PureFW.Widget.get_width.call(this);
	else
		return parseInt(this.get_node().offsetWidth);
};

/**
 * Setzt die Höhe des Containers. Wenn h <= 0, dann wird sie dynamisch zur
 * Laufzeit anhand des Inhalts bestimmt.
 * 
 * @param int h
 * @param uint ms			Positionssetzung soll animiert werden mit
 * 							Motiontweening und so viele Millisekunden lang
 * 							dauern.
 * @param Function easing	Funktion zum glätten der Animation
 * @param Function on_finish Funktion, die am Ende der Animation aufgerufen
 * 							 werden soll
 */
PureFW.Container.prototype.set_height = function(h, ms, easing, on_finish) {
	if ((h > 0) || ms)
		PureFW.Widget.set_height.call(this, h, ms, easing, on_finish);
	else {
		this.height = h;
		this.get_node().style.height = '';
	}
};

/**
 * Gibt die Höhe des Containers zurück.
 * 
 * @return uint
 */
PureFW.Container.prototype.get_height = function() {
	/*if (this.id=="SlideTableColContent0")
		alert(this.get_node().offsetHeight);*/
	if (this.height > 0)
		return PureFW.Widget.get_height.call(this);
	else
		return this.get_node().offsetHeight;
};

/**
 * Setzt die Hintergrundfarbe des Widgets. CSS-typische Werte wie "#FF8833" oder
 * "red" sind möglich.
 */
PureFW.Container.prototype.set_bg_color = function(col) {
	this._get_content_node().style.backgroundColor = col;
};

/**
 * Setzt die Ausrichtung des Textes. Gültige Werte sind CSS-üblich 'left',
 * 'right', 'justify', 'center'.
 */
PureFW.Container.prototype.set_text_align = function(align) {
	this._get_content_node().style.textAlign = align;
};

/**
 * Setzt das Hintergrundbild des Containers. Wenn Strech angegeben wird, dann 
 * wird das Hintergrundbild auf die Größe des Containers gestreckt 
 * (Grundeinstellung). Das ist auch die Vorraussetzung, dass der Hintergrund
 * korrekt mitskaliert.
 * Wird als img ein Leerstring übergeben, wird das Hintergrundbild gelöscht.
 * 
 * @param string img
 * @param bool strech		[default: true]
 */
PureFW.Container.prototype.set_bg_img = function(img, strech) {
	this.bg_img = img;
	if (typeof(strech) === 'undefined')
		strech = true;
	if (this.bg_img) {
		var img_uri = (this.bg_img.indexOf('://') > -1)
			? this.bg_img
			: this.pic_path+this.bg_img;
	}
	else
		img_uri = false

	if (this.node) {
		if (!strech) {
			// The simple case
			if (img_uri)
				this.get_node().style.backgroundImage = 'url('+img_uri+')';
			else
				this.get_node().style.backgroundImage = '';	
		}
		else {
			if (img_uri) {
				var temp_s = this.get_node().innerHTML;
				if (!this.content_node) {
					this.node.innerHTML = '';
					this.content_node = document.createElement('div');
					this.content_node.style.width = "100%";
					this.content_node.style.height = "100%";
					this.content_node.style.position = "absolute";
					this.content_node.style.top = '0';
					this.content_node.style.left = '0';
					this.content_node.id = this.content_id;
					this.content_node.innerHTML = temp_s;
					this.content_node.style.zIndex = 1;
					this.get_node().appendChild(this.content_node);
				}
				else
					this.content_node = document.getElemById(this.content_id);
				if (!this.bg_img_node) {
					this.bg_img_node = document.createElement('img');
					this.bg_img_node.style.width = "100%";
					this.bg_img_node.style.height = "100%";
					this.bg_img_node.style.position = 'absolute';
					this.bg_img_node.style.top = '0';
					this.bg_img_node.style.left = '0';
					this.bg_img_node.style.zIndex = 0;
					this.bg_img_node.id = this.bg_img_id;
					this.get_node().appendChild(this.bg_img_node);
				}
				else
					this.bg_img_node = document.getElemById(this.bg_img_id);
				
				if (this.bg_img_node)
					this.bg_img_node.src = img_uri;
			}
			else {
				if (this.bg_img_node = document.getElemById(this.bg_img_id))
					this.get_node().removeChild(this.bg_img_node);
				this.bg_img_node = null;
			}
		}
		
	}
};

/**
 * Setzt den Backgrond-Repeat. Erlaubt sind CSS-übliche Werte wie no-repeat,
 * repeat usw.
 * !Das geht aber nur, wenn das Bild nicht sowieso schon als strech-Bild
 * eingefügt wurde!
 * 
 * @param string repeat
 */
PureFW.Container.prototype.set_bg_repeat = function(repeat) {
	this.get_node().style.backgroundRepeat = repeat;
};

/**
 * Setzt die Backgrond-Position. Erlaubt sind CSS-übliche Werte wie "top left",
 * "50% 50%" usw.
 * !Das geht aber nur, wenn das Bild nicht sowieso schon als strech-Bild
 * eingefügt wurde!
 * 
 * @param string pos
 */
PureFW.Container.prototype.set_bg_position = function(pos) {
	this.get_node().style.backgroundPosition = pos;
};
 
/**
 * Gibt das Hintergrundbild des Containers zurück
 * 
 * @return string
 */
PureFW.Container.prototype.get_bg_img = function() {
	return this.bg_img;
};

/**
 * Malt den Container neu und aktuallisiert den scale_factor, 
 * falls er verloren ging
 */
PureFW.Container.prototype.repaint = function() {
	var elem = this.get_node();
	if ((typeof(this.scale_factor) == 'undefined') && !this.no_scale)
	{
		this.scale_factor = PureFW.WidgetManager.manager_all.scale_factor;
	}
	if (elem) {
		if (this.width > 0)
			elem.style.width = this.get_width() + 'px';
		if (this.height > 0)
			elem.style.height = this.get_height() + 'px';
		elem.style.left = this.get_x() + 'px';
		elem.style.top = this.get_y() + 'px';
	}
	this.set_bg_img(this.get_bg_img(), !!this.bg_img_node);
};
 
/**
 * Setzt das Padding (innenabstand) des Containers in Pixel
 * 
 * @param int p_all
 */
PureFW.Container.prototype.set_padding = function(p_all) {
	this._get_content_node().style.padding = p_all + 'px';
};

/**
 * Setzt das linke Padding (innenabstand) des Containers in Pixel
 * 
 * @param int p
 */
PureFW.Container.prototype.set_padding_left = function(p) {
	this._get_content_node().style.paddingLeft = p + 'px';
};

/**
 * Setzt das rechte Padding (innenabstand) des Containers in Pixel
 * 
 * @param int p
 */
PureFW.Container.prototype.set_padding_right = function(p) {
	this._get_content_node().style.paddingRight = p + 'px';
};

/**
 * Setzt das obere Padding (innenabstand) des Containers in Pixel
 * 
 * @param int p
 */
PureFW.Container.prototype.set_padding_top = function(p) {
	this._get_content_node().style.paddingTop = p + 'px';
};

/**
 * Setzt das untere Padding (innenabstand) des Containers in Pixel
 * 
 * @param int p
 */
PureFW.Container.prototype.set_padding_bottom = function(p) {
	this._get_content_node().style.paddingBottom = p + 'px';
};

/**
 * Setzt den Inhalt (HTML) des Containers. Wird interprete_js angegeben, wird
 * ggf. mitgesandtes JS (innerhalb der Tags <script></script>) interpretiert!
 * 
 * @param string text
 * @param bool interprete_js [=false]
 */
PureFW.Container.prototype.set_content = function(text, interprete_js) {
	this.remove_all_widgets();
	this.on_change(this.create_event("change"));
	document.setElemInnerHTML(this._get_content_node(), text, interprete_js);
};

/**
 * Fügt ein neues durch den übergebenen Konstruktor definiertes Widget dem 
 * Content hinzu. Die Argumente werden entsprechend dem Widget weitergereicht.
 * 
 * Beispielnutzung:
 * <code>
 * x.add_widget(
 *  PureFW.Image, 0, 0, 12, 12, 'my_icon.png', x.no_scale
 * );
 * </code>
 * Dies fügt dem Container <code>x</code> ein neues Bild an Position (0;0) mit 
 * Größe (12;12) hinzu, das das Bild 'my_icon.png' anzeigt und nur dann skalier,
 * wenn auch x skaliert.
 * 
 * Gibt das neue Widget zurück.
 * 
 * @param Constructor widget_constructor
 * @param uint x
 * @param uint y
 * @param uint w
 * @param uint h
 * @param mixed argN
 * @return PureFW.Widget
 */
PureFW.Container.prototype.add_widget = function(widget_constructor,
	x, y, w, h, arg6, arg7, arg8, arg9, arg10, arg11, arg12, arg13)
{
	var w = new widget_constructor(
		this._get_content_node(),
		x,y,w,h, 
		arg6, arg7, arg8, arg9, arg10, arg11, arg12, arg13
	);
	this.added_widgets.push(w);
	this.on_change(this.create_event("change"));
	return w;
};

/**
 * Entfernt alle hinzugefügten Widgets
 */
PureFW.Container.prototype.remove_all_widgets = function() {
	this.on_change(this.create_event("change"));
	this.added_widgets.destroy();
}

/**
 * Fügt dem Container Inhalt (HTML ohne Javascript) hinzu.
 * 
 * @param string text
 */
PureFW.Container.prototype.add_content = function(text) {
	this.on_change(this.create_event("change"));
	document.setElemInnerHTML(this._get_content_node(), 
			this._get_content_node().innerHTML + text);
};
 
/**
 * Gibt den Inhalt (HTML) des Containers zurück.
 * 
 * @return string
 */
PureFW.Container.prototype.get_content = function() {
	if (document.getElemById(this.content_id))
		this.content_node = document.getElemById(this.content_id);
	if(document.getElemById(this.id))
		this.node = document.getElemById(this.id);
	return (this.content_node) ? this.content_node.innerHTML 
			: this.node.innerHTML;
};

/**
 * Gibt den Knoten zurück, der den sichtbaren Inhalt des Containers enthält.
 * Möchte man z.B. in den Container ein weiteres Widget integrieren, erhält man
 * hierüber den nötigen Vaterknoten für den Konstruktor des Widgets.
 * 
 * @return HTMLElement
 * @deprecated
 */
PureFW.Container.prototype.get_content_node = function() {
	if (document.getElemById(this.content_id))
		this.content_node = document.getElemById(this.content_id);
	if (document.getElemById(this.id))
		this.node = document.getElemById(this.id);
	
	return (this.content_node) ? this.content_node : this.node;
};

/**
 * Gibt den Knoten zurück, der den sichtbaren Inhalt des Containers enthält.
 * Möchte man z.B. in den Container ein weiteres Widget integrieren, erhält man
 * hierüber den nötigen Vaterknoten für den Konstruktor des Widgets.
 * 
 * @return HTMLElement
 * @access private
 */
PureFW.Container.prototype._get_content_node = function() {
	return this.get_content_node();
};

/**
 * Fügt einen EventHandler hinzu. Funktion überschrieben, da zusätzlich
 * der Typ "change" unterstützt wird, welcher beim Verändern des Inhalts
 * ausgelöst wird (durch set_content oder add_content).
 * 
 * @param string type
 * @param Function fn
 * @see Widget.add_event_handler
 */
PureFW.Container.prototype.add_event_handler = function(type, fn) {
	if (type === "change")
		this.on_change_functions.push(fn);
	else
		PureFW.Container.parent.add_event_handler.call(this, type, fn);
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
PureFW.Container.prototype.remove_event_handler = function(type, fn) {
	if (type === "change")
		this.on_change_functions.remove(fn);
	else
		PureFW.Container.parent.remove_event_handler.call(this, type, fn);
};

/**
 * Ruft das "on-change"-Event exlizit auf. Wird normalerweise aufgerufen,
 * wenn der Inhalt durch set_content oder add_content verändert wird.
 * 
 * @param Event ev [optional]
 */
PureFW.Container.prototype.on_change = function(ev) {
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
PureFW.Container.prototype.destroy = function() {
	this.on_change_functions.destroy();
	this.added_widgets.destroy();
	PureFW.Container.parent.destroy.call(this);
};
