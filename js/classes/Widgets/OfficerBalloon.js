/**
 * Repräsentiert einen die Sprechblase eines Beraters. Sprechblasenbild kann
 * ausgetauscht werden, Sprechblase kann minimiert werden.
 * 
 * @author Phil
 */

/**
 * Erzeugt die Sprechblase
 * 
 * @param HTMLElement parent_node
 * @param uint x
 * @param uint y
 * @param uint w
 * @param uint h
 * @param string pic_path
 */
PureFW.OfficerBalloon = function(parent_node, x, y, w, h, bg_pic) {
	if (typeof(parent_node) !== 'undefined') 
	{
		this.init(parent_node, x, y, w, h, bg_pic);
		this.instance_num = this.constructor.num_instances;
		this.instance_name = 
			"PureFW.OfficerBalloon.instances["+this.instance_num+"]";
		this.id = "OfficerBalloon"+this.instance_num;
		this.bg_img_id = "OfficerBalloon_bg_img"+this.instance_num;
		this.content_id = "OfficerBalloon_cont"+this.instance_num;
		this.yes_id = "OfficerBalloon_yes"+this.instance_num;
		this.no_id = "OfficerBalloon_no"+this.instance_num;
		this.text_container = null;
		this.balloon_text_id = 'OfficerBalloon_text' + this.instance_num;
		this.text = '';
		
		this.insert_into_dom_tree();
		this.constructor.num_instances++;
		this.constructor.instances[this.instance_num] = this;
		PureFW.WidgetManager.manager_all.register_widget(this);
	}
};

/********************************************************************
 * OfficerBalloon extends Container
 ********************************************************************/
PureFW.OfficerBalloon.extend(PureFW.ConfirmationBox);

/********************************************************************
 * Statische Deklarationen
 ********************************************************************/
PureFW.OfficerBalloon.num_instances = 0;
PureFW.OfficerBalloon.instances = new Array();

/**
 * Wrapper fürs Wachsen.
 * 
 * @param Event ev
 * @return Function
 */
PureFW.OfficerBalloon.grow_wrapper = function(ev) {
	this.grow();
}

/********************************************************************
 * Prototyp-Deklarationen
 ********************************************************************/
/**
 * Initialisierung
 * 
 * @param HTMLElement parent_node
 * @param uint x
 * @param uint y
 * @param uint w
 * @param uint h
 * @param string pic_path
 */
PureFW.OfficerBalloon.prototype.init = function(parent_node, x, y, w, h, 
	bg_pic) 
{
	PureFW.OfficerBalloon.parent.init.call(this, parent_node, x, y, w, h);
	
	this.growed_width = w;
	this.growed_height = h;
	this.shrinked_width = 106;
	this.shrinked_height = 89;
	this.is_shrinked = false;
	this.reshow_yes_no_containers = false;	// Flag der fürs shrinken gebraucht
											// wird (s. shrink() / grow())
	
	this.bg_pic = bg_pic;
	this.grow_shrink_button = null;
	this.grow_shrink_button_width = 28;
	this.grow_shrink_button_height = 28;
	
	this.confirm_button_offset.y -= 30;
	this.confirm_button_offset.x += 30;
	
};

/**
 * Fügt die Bubble in den DOM-Baum ein.
 */
PureFW.OfficerBalloon.prototype.insert_into_dom_tree = function()
{
	PureFW.OfficerBalloon.parent.insert_into_dom_tree.call(this);
	this.set_bg_img(this.bg_pic);
	
	var tw = Math.floor(this.width/1.12);
	var th = Math.floor(this.height/1.65);
	
	this.text_container = new PureFW.Container(
		this,
		this.width-tw-15, this.height-th-60,
		tw, th
	);
	this.text_container.set_font_size(1.05);
	this.set_css_class('OfficerBalloon');

	if (this.yes_container) {
		this.yes_container.hide();
	}
	if (this.no_container) {
		this.no_container.hide();
	}
	
	/**
	 * Der Grow/Shrink-Button
	 */
	this.grow_shrink_button = new PureFW.Image(
		this.parent_node,
		this.position.x + this.width,
		this.position.y,
		this.grow_shrink_button_width,
		this.grow_shrink_button_height,
		'ui/elements/form/buttons/shrink.png'
	);
	this.grow_shrink_button.add_event_handler(
		"click",
		(function(_instance) {
			return function(ev) {
				_instance.grow_shrink_toggle();
			}
		})(this)
	);
	this.grow_shrink_button.set_z_index(this.get_z_index()+1);
};

/**
 * Verkleinert die Blase, wenn sie groß ist, vergrößert sie, wenn sie klein ist.
 */
PureFW.OfficerBalloon.prototype.grow_shrink_toggle = function() {
	if (this.is_shrinked) {
		this.grow();
	}
	else {
		this.shrink();
	}
}
/**
 * Vergrößert die Blase auf ihre ganze Größe, wenn sie nicht bereits groß ist.
 */
PureFW.OfficerBalloon.prototype.grow = function() {
	if (!this.is_shrinked)
		return;
	this.set_width(this.growed_width);
	this.set_height(this.growed_height);
	this.set_y(this.position.y 
		- (this.growed_height - this.shrinked_height)
	);
	this.set_bg_img(this.bg_pic.replace(/_small_ex\.png/g, '.png'));
	this.text_container.show();
	this.grow_shrink_button.set_position(
		this.position.x + this.width - this.grow_shrink_button_width,
		this.position.y
	);
	this.grow_shrink_button.set_pic_url(
		'ui/elements/form/buttons/shrink.png'
	);
	if (this.reshow_yes_no_containers)
		this.show_confirm_dialog();
	
	/**
	 * Wenn die Blase angeklickt wird, dann nichts mehr tun.
	 */
	this.remove_event_handler(
		"click",
		PureFW.OfficerBalloon.grow_wrapper
	);
	
	this.is_shrinked = false;
}

/**
 * Verkleinert die Blase auf die minimierte Größe, wenn sie nicht schon klein 
 * ist.
 */
PureFW.OfficerBalloon.prototype.shrink = function() {
	if (this.is_shrinked)
		return;
	this.set_width(this.shrinked_width);
	this.set_height(this.shrinked_height);
	this.set_y(this.position.y 
		+ (this.growed_height - this.shrinked_height)
	);
	this.set_bg_img(this.bg_pic.replace(/\.png/g, '_small_ex.png'));
	this.text_container.hide();
	this.grow_shrink_button.set_position(
		this.position.x + this.width - this.grow_shrink_button_width,
		this.position.y
	);
	this.grow_shrink_button.set_pic_url(
		'ui/elements/form/buttons/grow.png'
	);
	
	if (!this.yes_container.is_hidden() || !this.no_container.is_hidden()) {
		this.hide_confirm_dialog();
		this.reshow_yes_no_containers = true;
	}
	
	/**
	 * Wenn die Blase angeklickt wird, dann vergrößern.
	 */
	this.add_event_handler(
		"click",
		PureFW.OfficerBalloon.grow_wrapper
	);
	this.is_shrinked = true;
}

/**
 * Setzt den Text, der in der Bubble stehn soll.
 * 
 * @param string str
 */
PureFW.OfficerBalloon.prototype.set_text = function(str) 
{
	this.text_container.set_content(str);
	this.text = str;
	this.grow();
};

/**
 * Synomym für ~.set_text
 * 
 * @param string str
 */
PureFW.OfficerBalloon.prototype.set_content = function(str) {
	this.set_text(str);
};

/**
 * Zeigt den Confirmation-Dialog mit dem angegebenen Text an. Wird kein Text 
 * angegeben, wird der Text nicht korrigiert, sondern nur die YES-,NO-Buttons
 * eingeblendet.
 * 
 * @param string new_text
 */
PureFW.OfficerBalloon.prototype.show_confirm_dialog = function(new_text)
{
	if(new_text)
	{
		this.set_text(new_text);
	}
	if (this.yes_container)
		this.yes_container.show();
	if (this.no_container)
		this.no_container.show();
};

PureFW.OfficerBalloon.prototype.hide_confirm_dialog = function()
{
	if (this.yes_container)
		this.yes_container.hide();
	if (this.no_container)
		this.no_container.hide();
}

/**
 * Versteckt die Blase.
 */
PureFW.OfficerBalloon.prototype.hide = function() {
	PureFW.Widget.hide.call(this);
	this.grow_shrink_button.hide();
}

/**
 * Zeigt die Blase wieder an.
 */
PureFW.OfficerBalloon.prototype.show = function() {
	PureFW.Widget.show.call(this);
	this.grow_shrink_button.show();
}

/**
 * Setzt die x-Position des Widgets.
 * 
 * @param uint x
 */
PureFW.OfficerBalloon.prototype.set_x = function(x) {
	PureFW.OfficerBalloon.parent.set_x.call(this, x);
	this.grow_shrink_button.set_x(
		this.position.x + this.width - this.grow_shrink_button_width
	);
}

/**
 * Setzt die y-Position des Widgets.
 * 
 * @param uint y
 */
PureFW.OfficerBalloon.prototype.set_y = function(y) {
	PureFW.OfficerBalloon.parent.set_y.call(this, y);
	this.grow_shrink_button.set_y(
		this.position.y
	);
}

/**
 * Setzt die Breite des Widgets.
 * 
 * @param uint w
 */
PureFW.OfficerBalloon.prototype.set_width = function(w) {
	PureFW.OfficerBalloon.parent.set_width.call(this, w);
	var tw = Math.floor(this.width/1.12);
	this.text_container.set_width(tw);
	this.text_container.set_x(this.width-tw-10);
}

PureFW.OfficerBalloon.prototype.set_height = function(h) {
	PureFW.OfficerBalloon.parent.set_height.call(this, h);
	var th = Math.floor(this.height/1.7);
	var y_off = Math.floor(this.height/3.25);
	this.text_container.set_height(th);
	this.text_container.set_y(this.height-th-y_off);
}

/**
 * Setzen der Breite des Widgets, wenn ausgefahren (groß)
 * 
 * @param uint w
 */
PureFW.OfficerBalloon.prototype.set_growed_width = function(w) {
	this.growed_width = w;
	if (!this.is_shrinked) {
		this.set_width(w);
		this.grow_shrink_button.set_x(
			this.position.x + this.width - this.grow_shrink_button_width
		);
	}
}

/**
 * Setzt die Höhe des Widgets, wenn ausgefahren (groß)
 * 
 * @param uint h
 */
PureFW.OfficerBalloon.prototype.set_growed_height = function(h) {
	this.growed_height = h;
	if (!this.is_shrinked) {
		this.set_height(h);
	}
}

/**
 * Setzt den z-Index des Widgets
 * 
 * @param uint z
 */
PureFW.OfficerBalloon.prototype.set_z_index = function(z) {
	try {
		this.grow_shrink_button.set_z_index(z + 1);
		PureFW.OfficerBalloon.parent.set_z_index.call(this, z);
	}
	catch(e) {}
}


/**
 * Ruft das "confirm"-Event exlizit auf. Wird normalerweise aufgerufen,
 * wenn die Box bestätigt wurde.
 * 
 * @param Event ev [optional]
 */
PureFW.OfficerBalloon.prototype.on_confirm = function(ev) {
	if (!ev) {
		this.create_event("confirm");
	}
	var n = this.on_confirm_functions.length;
	for (var i = 0; i < n; i++) {
		this.on_confirm_functions[i].call(this, ev);
	}
	this.hide_confirm_dialog();
}

/**
 * Ruft das "cancel"-Event explizit auf. Wird normalerweise aufgerufen,
 * wenn die Box geschlossen wurde, ohne Bestätigung (Drücken von (X)).
 *
 * @param Event ev [optional]
 */
PureFW.OfficerBalloon.prototype.on_cancel = function(ev) {
	if (!ev) {
		this.create_event("cancel");
	}
	var n = this.on_cancel_functions.length;
	for (var i = 0; i < n; i++) {
		this.on_cancel_functions[i].call(this,ev);
	}
	this.hide_confirm_dialog();
}


/**
 * Zerstört die Blubberblase...
 */
PureFW.OfficerBalloon.prototype.destroy = function() {
	PureFW.OfficerBalloon.parent.destroy.call(this);
	this.grow_shrink_button.destroy();
}