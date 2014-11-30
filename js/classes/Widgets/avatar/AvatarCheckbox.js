/**
 * Die Avatar-Checkbox besteht aus einem Avatar, über den dann unten rechts
 * eine Checkbox gemalt wird. Sie funktioniert wie eine Checkbox.
 * 
 * @author Phil
 * @package Avatar
 */

if (typeof(PureFW.Avatar) != 'object')
	PureFW.Avatar = new Object();

/**
 * @param HTMLElement parent_node
 * @param int x
 * @param int y
 * @param uint w
 * @param uint h
 */
PureFW.Avatar.AvatarCheckbox = function(parent_node, x, y, w, h, pic, nick, 
	color, no_scale) 
{
	if (typeof(parent_node) !== 'undefined') 
	{
		this.init(parent_node, x, y, w, h, pic, nick, color, no_scale);
	
		this.instance_num = this.constructor.num_instances;
		this.instance_name = 
			"PureFW.Avatar.AvatarCheckbox.instances["+this.instance_num+"]";
		
		this.id = "AvatarCheckbox"+this.instance_num;
		this.bg_img_id = "AvatarCheckboxBg"+this.instance_num;
		this.content_id = "AvatarCheckboxCont"+this.instance_num;
		
		this.insert_into_dom_tree();
		this.constructor.num_instances++;
		this.constructor.instances[this.instance_num] = this;
		
		if (!this.no_scale)
			PureFW.WidgetManager.manager_all.register_widget(this);
	}
};
/********************************************************************
 * AvatarCheckbox extends Container
 ********************************************************************/
PureFW.Avatar.AvatarCheckbox.extend(PureFW.Container);

/********************************************************************
 * Statische Deklarationen
 ********************************************************************/
PureFW.Avatar.AvatarCheckbox.num_instances = 0;
PureFW.Avatar.AvatarCheckbox.instances = new Array();

/********************************************************************
 * Prototyp-Deklarationen
 ********************************************************************/
PureFW.Avatar.AvatarCheckbox.prototype.init = function(parent_node, x, y, w, h, 
	pic, nick, frame_color, no_scale) 
{
	PureFW.Avatar.AvatarCheckbox.parent.init.call(this, parent_node, x, y, w, 
		h, no_scale);
	
	this.pic = pic || '';
	this.nick = nick || '';
	this.frame_color = frame_color || '';
	
	this.avatar = null;
	this.checkbox = null;
}

PureFW.Avatar.AvatarCheckbox.prototype.insert_into_dom_tree = function() 
{
	PureFW.Avatar.AvatarCheckbox.parent.insert_into_dom_tree.call(this);
	this.set_overflow('visible');
	this.avatar = this.add_widget(
		PureFW.Avatar.Avatar,
		0, 0,
		this.width, this.height,
		this.pic, '', this.color,
		this.no_scale
	);
	this.set_tooltip(this.nick);
	
	this.checkbox = this.add_widget(
		PureFW.Checkbox,
		this.width - 19, this.height - 21,
		22, 19,
		'ui/elements/form/input/check_toggle_small.png'
	);
	this.checkbox.set_z_index(1);
	
	this.avatar.add_event_handler("click",
		(function(_instance) {
			return function (ev) {
				_instance.checkbox.on_click(ev)
			}
		})(this)
	);
}

/**
 * Hakt die Kiste an.
 */
PureFW.Avatar.AvatarCheckbox.prototype.check = function() {
	this.checkbox.check();
};

/**
 * Hakt die Kiste ab/aus oder wie auch immer.
 */
PureFW.Avatar.AvatarCheckbox.prototype.uncheck = function() {
	this.checkbox.uncheck();
};

/**
 * Hakt die Kiste an, wenn sie nicht angehakt ist, ansonsten wird die Kiste
 * in den unangehakten Zustand überführt.
 */
PureFW.Avatar.AvatarCheckbox.prototype.toggle = function() {
	this.checkbox.toggle();
};

/**
 * Gibt zurück, ob die Box gerade angehakt ist.
 * 
 * @return bool
 */
PureFW.Avatar.AvatarCheckbox.prototype.is_checked = function() {
	return this.checkbox.is_checked();
};

/**
 * Deaktiviert das Widget.
 * 
 * @see PureFW.Widget#deactivate
 */
PureFW.Avatar.AvatarCheckbox.prototype.deactivate = function() {
	this.avatar.deactivate();
	this.checkbox.deactivate();
}

/**
 * Aktiviert das Widget.
 * 
 * @see PureFW.Widget#activate
 */
PureFW.Avatar.AvatarCheckbox.prototype.activate = function() {
	this.avatar.activate();
	this.checkbox.activate();
}

/**
 * Fügt einen EventHandler hinzu. Funktion überschrieben, da zusätzlich
 * die Typen "check" und "uncheck" unterstützt werden, welche beim Anhaken
 * bzw. Abhaken ausgelöst werden.
 * 
 * @param string type
 * @param Function fn
 * @see PureFW.Checkbox#add_event_handler
 */
PureFW.Avatar.AvatarCheckbox.prototype.add_event_handler = function(type, fn) {
	if ((type === "check") || (type === "uncheck"))
		this.checkbox.add_event_handler(type, fn);
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
 * @see PureFW.Checkbox#remove_event_handler
 */
PureFW.Avatar.AvatarCheckbox.prototype.remove_event_handler = function(type, fn) {
	if ((type === "check") || (type === "uncheck"))
		this.checkbox.remove_event_handler(type, fn);
	else
		PureFW.Image.parent.remove_event_handler.call(this, type, fn);
};