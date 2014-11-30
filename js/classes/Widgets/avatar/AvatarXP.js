/**
 * Dieses Widget erweitert den Avatar um die Anzeige der XP des Spielers.
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
 * @param String pic	Avatar-Bild
 * @param String nick	Nutzername
 * @param String color	Farbe
 * @param bool checkbox	Ob Checkbox-Avatare benutzt werden sollen, oder normale
 * @param boo no_scale
 */
PureFW.Avatar.AvatarXP = function(parent_node, x, y, w, h, pic, nick, color, 
	checkbox, no_scale) 
{
	if (typeof(parent_node) !== 'undefined') 
	{
		this.init(parent_node, x, y, w, h, pic, nick, color, checkbox, 
			no_scale);
	
		this.instance_num = this.constructor.num_instances;
		this.instance_name = 
			"PureFW.Avatar.AvatarXP.instances["+this.instance_num+"]";
		
		this.id = "AvatarXP"+this.instance_num;
		this.bg_img_id = "AvatarXPBg"+this.instance_num;
		this.content_id = "AvatarXPCont"+this.instance_num;
		
		this.insert_into_dom_tree();
		this.constructor.num_instances++;
		this.constructor.instances[this.instance_num] = this;
		
		if (!this.no_scale)
			PureFW.WidgetManager.manager_all.register_widget(this);
	}
};
/********************************************************************
 * Avatar extends Container
 ********************************************************************/
PureFW.Avatar.AvatarXP.extend(PureFW.Container);

/********************************************************************
 * Statische Deklarationen
 ********************************************************************/
PureFW.Avatar.AvatarXP.num_instances = 0;
PureFW.Avatar.AvatarXP.instances = new Array();

/********************************************************************
 * Prototyp-Deklarationen
 ********************************************************************/
PureFW.Avatar.AvatarXP.prototype.init = function(parent_node, x, y, w, h, pic, 
	nick, frame_color, checkbox, no_scale) 
{
	PureFW.Avatar.AvatarXP.parent.init.call(this, parent_node, x, y, w, h,
		no_scale);

	this.pic = pic || null;
	this.frame_color = frame_color || null;
	this.nick = nick || null;
	
	this.cb = checkbox || false;
	this.avatar = null;
	this.nick_container = null;
	this.xp_img = null;
	this.xp_container = null;
	
	
	this.xp_level = 0;
};

PureFW.Avatar.AvatarXP.prototype.insert_into_dom_tree = function() 
{
	PureFW.Avatar.AvatarXP.parent.insert_into_dom_tree.call(this);
	
	if (this.cb) {
		this.avatar = this.add_widget(
			PureFW.Avatar.AvatarCheckbox,
			0, 0,
			this.height, this.height,
			this.pic,
			this.nick,
			this.frame_color
		);
	}
	else {
		this.avatar = this.add_widget(
			PureFW.Avatar.Avatar,
			0, 0,
			this.height, this.height,
			this.pic,
			this.nick,
			this.frame_color
		);
	}
	
	this.nick_container = this.add_widget(
		PureFW.Container,
		this.height + 3,
		0,
		this.width - this.height - 6,
		23
	);
	this.nick_container.set_text_align('left');
	this.nick_container.set_content(
		'<a href="javascript: UIButtons.open_window(\'profile.php?nick='
			+this.nick+'\');">'+MammunUI.beautify_nick(this.nick)+'</a>'
	);
	
	this.xp_img = this.add_widget(
		PureFW.Image,
		this.height +3,	this.height - 25,
		25, 25,
		'ui/icons/labels/xp_star/25/star.png'
	);
	
	this.xp_container = this.add_widget(
		PureFW.Container,
		this.xp_img.position.x + this.xp_img.width + 2,
		this.xp_img.position.y + 4,
		this.nick_container.width - 2 - this.xp_img.width
			- this.xp_img.position.x,
		20
	);
	
	this.xp_container.set_content(this.xp_level);
};

/**
 * Alle Ereignisse werden dem Avatarbild zugewiesen.
 * 
 * @param String type
 * @param Function fn
 * @see PureFW.Avatar.Avatar#add_event_handler
 */
PureFW.Avatar.AvatarXP.prototype.add_event_handler = function(type, fn) {
	this.avatar.add_event_handler(type, fn);
}

/**
 * Alle Ereignisse werden dem Avatarbild zugewiesen.
 * 
 * @param String type
 * @param Function fn
 * @see PureFW.Avatar.Avatar#add_event_handler
 */
PureFW.Avatar.AvatarXP.prototype.remove_event_handler = function(type, fn) {
	this.avatar.remove_event_handler(type, fn);
}

/**
 * Setzt den Tooltip des Avatars
 * 
 * @param String str
 * @see PureFW.Avatar.Avatar#set_tooltip
 */
PureFW.Avatar.AvatarXP.prototype.set_tooltip = function(str) {
	this.avatar.set_tooltip(str);
}

/**
 * Setzt das XP-Level, das angezeigt werden soll
 * 
 * @param uint xp
 */
PureFW.Avatar.AvatarXP.prototype.set_xp_level = function(xp) {
	this.xp_level = xp;
	this.xp_container.set_content(this.xp_level);
}

/**
 * Gibt das angezeigte XP-Level zurück
 * 
 * @return uint
 */
PureFW.Avatar.AvatarXP.prototype.get_xp_level = function() {
	return this.xp_level;
}

/**
 * Gibt zurück, ob der AvatarXP angehakt wurde.
 * 
 * Falls diese Instanz ohne Checkbox erzeugt wurde, wird immer FALSE 
 * zurückgegeben.
 * 
 * @return bool
 */
PureFW.Avatar.AvatarXP.prototype.is_checked = function() {
	return (this.cb) ? this.avatar.is_checked() : false;
}

/**
 * Hakt den AvatarXP explizit an.
 * 
 * Diese Funktion hat keine Auswirkung, wenn keine Checkbox erzeugt wurde.
 * @return
 */
PureFW.Avatar.AvatarXP.prototype.check = function() {
	if (this.cb)
		this.avatar.check();
}
/**
 * Hakt den AvatarXP explizit ab.
 * 
 * Diese Funktion hat keine Auswirkung, wenn keine Checkbox erzeugt wurde.
 * @return
 */
PureFW.Avatar.AvatarXP.prototype.uncheck = function() {
	if (this.cb)
		this.avatar.uncheck();
}

/**
 * Deaktiviert das Widget.
 * 
 * @see PureFW.Widget#deactivate
 */
PureFW.Avatar.AvatarXP.prototype.deactivate = function() {
	this.avatar.deactivate();
	this.xp_container.deactivate();
	this.xp_img.deactivate();
	this.nick_container.deactivate();
}

/**
 * Aktiviert das Widget.
 * 
 * @see PureFW.Widget#activate
 */
PureFW.Avatar.AvatarXP.prototype.activate = function() {
	this.avatar.activate();
	this.xp_container.activate();
	this.xp_img.activate();
	this.nick_container.activate();
}

/**
 * Gibt den Nick zurück
 * 
 * @return String
 */
PureFW.Avatar.AvatarXP.prototype.get_nick = function() {
	return this.nick;
}