/**
 * Ein Avatar mit Schattenwurf und ggf. farbigem Rahmen.
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
PureFW.Avatar.Avatar = function(parent_node, x, y, w, h, pic, nick, color,
	no_scale) 
{
	if (typeof(parent_node) !== 'undefined') 
	{
		this.init(parent_node, x, y, w, h, pic, nick, color, no_scale);
	
		this.instance_num = this.constructor.num_instances;
		this.instance_name = 
			"PureFW.Avatar.Avatar.instances["+this.instance_num+"]";
		
		this.id = "Avatar"+this.instance_num;
		this.bg_img_id = "AvatarBg"+this.instance_num;
		this.content_id = "AvatarCont"+this.instance_num;
		
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
PureFW.Avatar.Avatar.extend(PureFW.Container);

/********************************************************************
 * Statische Deklarationen
 ********************************************************************/
PureFW.Avatar.Avatar.num_instances = 0;
PureFW.Avatar.Avatar.instances = new Array();

/********************************************************************
 * Prototyp-Deklarationen
 ********************************************************************/
PureFW.Avatar.Avatar.prototype.init = function(parent_node, x, y, w, h, pic, 
	nick, frame_color, no_scale) 
{
	PureFW.Avatar.Avatar.parent.init.call(this, parent_node, x, y, w, h, 
		no_scale);

	this.pic = pic || null;
	this.frame_color = frame_color || null;
	
	this.avatar_img = null;

	this.inner_width = w ? w*44/48 : 0;
	this.inner_height = h ? h*44/48 : 0;
	this.inner_x = w ? w*2/48 : 0;
	this.inner_y = h ? h*1/48 : 0;
	
	this.nick = nick;
	
	this.frame_img = null;
};

PureFW.Avatar.Avatar.prototype.insert_into_dom_tree = function() 
{
	PureFW.Avatar.Avatar.parent.insert_into_dom_tree.call(this);
	
	if (this.width == 94) {
		this.set_bg_img(
			'ui/backgrounds/avatars/shadow_94.png'
		);
	}
	else {
		this.set_bg_img(
			'ui/backgrounds/avatars/shadow.png'
		);
	}

	this.avatar_img = new PureFW.Image(
		this,
		this.inner_x, this.inner_y,
		this.inner_width, this.inner_height,
		this.pic,
		this.no_scale
	);
	this.avatar_img.set_z_index(1);
		
	if (this.frame_color) {
		this.frame_img = new PureFW.Image(
			this,
			this.inner_x, this.inner_y,
			this.inner_width, this.inner_height,
			'ui/icons/labels/avatars/'+this.inner_width+'/frames/'
				+this.frame_color+'.png',
			this.no_scale
		);
		this.frame_img.set_z_index(2);
	}
	
	if (this.nick)
		this.set_nick(this.nick);
};

PureFW.Avatar.Avatar.prototype.add_event_handler = function(type, fn) {
	this.avatar_img.add_event_handler(type, fn);
}

PureFW.Avatar.Avatar.prototype.remove_event_handler = function(type, fn) {
	this.avatar_img.remove_event_handler(type, fn);
}

PureFW.Avatar.Avatar.prototype.set_tooltip = function(str) {
	if (this.frame_img)
		this.frame_img.set_tooltip(str);
	this.avatar_img.set_tooltip(str);
}

/**
 * Gibt den gesetzten Nick zurück.
 * 
 * @return String
 */
PureFW.Avatar.Avatar.prototype.get_nick = function() {
	return this.nick;
}

/**
 * Setzt den Nick nachträglich.
 * 
 * @param String str
 */
PureFW.Avatar.Avatar.prototype.set_nick = function(nick) {
	this.nick = nick;
	this.set_tooltip(MammunUI.get_nick_main(this.nick));
	if (this.frame_img) {
		this.frame_img.add_event_handler("click",
			(function (_instance) {
				return function(ev) {
					if (_instance.is_activated()) {
						UIButtons.open_window(
							'profile.php?nick='	+ _instance.nick, '', 500
						);
					}
				}
			})(this)
		)
	}
	else {
		this.avatar_img.add_event_handler("click",
			(function (_instance) {
				return function(ev) {
					if (_instance.is_activated()) {
						UIButtons.open_window(
							'profile.php?nick='	+ _instance.nick, '', 500
						);
					}
				}
			})(this)
		)
	}
}

/**
 * Setzt das Avatar-Bildchen nachträglich.
 * 
 * @param String str
 */
PureFW.Avatar.Avatar.prototype.set_pic_url = function(pic) {
	this.pic = pic;
	this.avatar_img.set_pic_url(pic);
}

PureFW.Avatar.Avatar.prototype.destroy = function() {
	PureFW.Avatar.Avatar.parent.destroy.call(this);
	if (this.avatar_img)
		this.avatar_img.destroy();
	if (this.item_shop_symbol_container)
		this.frame_img.destroy();
};