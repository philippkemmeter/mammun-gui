/**
 * Diese Klasse repräsentiert ein Icon, welches Freunde des Spielers anzeigt,
 * und wenn man draufklickt, kommt man zur MapVisitNavi/Inselbrowser
 * 
 * @author Philipp
 */

if (typeof(PureFW.MapUI) != 'object')
	PureFW.MapUI = new Object();

/**
 * @param HTMLElement parent_node
 * @param int x
 * @param int y
 * @param uint w
 * @param uint h
 * @param bool no_scale=false
 */
PureFW.MapUI.MapVisitNaviIcon = function(parent_node, x, y, w, h, no_scale) {
	if (typeof(parent_node) !== 'undefined') {
		this.init(parent_node, x, y, w, h, no_scale);
	
		this.instance_num = this.constructor.num_instances;
		this.instance_name = 
			"PureFW.MapUI.MapVisitNaviIcon.instances["+this.instance_num+"]";
		
		this.id = "MapVisitNaviIcon"+this.instance_num;
		this.bg_img_id = "MapVisitNaviIconBg"+this.instance_num;
		this.content_id = "MapVisitNaviIconCont"+this.instance_num;
		
		this.insert_into_dom_tree();
		this.constructor.num_instances++;
		this.constructor.instances[this.instance_num] = this;
		
		if (!this.no_scale)
			PureFW.WidgetManager.manager_all.register_widget(this);
	}
};


/********************************************************************
* MapVisitNaviIcon extends Container
********************************************************************/
PureFW.MapUI.MapVisitNaviIcon.extend(PureFW.Container);

/********************************************************************
* Statische Deklarationen
********************************************************************/
PureFW.MapUI.MapVisitNaviIcon.num_instances = 0;
PureFW.MapUI.MapVisitNaviIcon.instances = new Array();

/********************************************************************
* Prototyp-Deklarationen
********************************************************************/
PureFW.MapUI.MapVisitNaviIcon.prototype.init = function(parent_node, x, y, w, h, 
	no_scale) 
{
	PureFW.MapUI.MapVisitNaviIcon.parent.init.call(this, parent_node, x, y, w, 
		h, no_scale);
	
	this.avatar_spacing = 5;
	this.avatar_amount = 4;
	this.avatar_size = new PureFW.Point(48, 48);
	this.avatar_container = null;
	this.avatars = new Array();
};

PureFW.MapUI.MapVisitNaviIcon.prototype.insert_into_dom_tree = function() {
	PureFW.MapUI.MapVisitNaviIcon.parent.insert_into_dom_tree.call(this);
	
	this.set_bg_img('ui/backgrounds/MapVisitNaviIcon/bg_'+this.width+'x'
		+this.height+'.png');
	
	var width = (this.avatar_amount*this.avatar_size.x 
		+ (this.avatar_amount-1)*this.avatar_spacing);
	var offset = ((this.width - width) >> 1);
	
	this.avatar_container = this.add_widget(
		PureFW.Container,
		offset, offset,
		width, this.height - offset,
		this.no_scale
	);
};


/**
 * Fügt einen Avatar hinzu
 * 
 * @param String pic	URL des anzuzeigenden Bildes
 * @param bool external	Ob das angezeigte Bild einem Menschen außerhalb von
 * 						Mammun gehört (z.B. einem FB-Freund, der noch nicht
 * 						Mammun spielt)
 */
PureFW.MapUI.MapVisitNaviIcon.prototype.add_avatar = function(pic, 
	world_id, map_id, external) {
	/**
	 * Nicht mehr hinzufügen als angekündigt.
	 */
	if (this.avatars.length >= this.avatar_amount)
		return;
	
	external = external || false;
	world_id = world_id || 0;
	map_id = map_id || 0;
	var a = this.avatar_container.add_widget(
		PureFW.Avatar.Avatar,
		this.avatars.length * (this.avatar_size.x + this.avatar_spacing),
		0,
		this.avatar_size.x, this.avatar_size.y,
		pic,
		null,
		null,
		this.no_scale
	);
	
	if (external) {
		//@TODO Andere markierung für externe freune statt Brezel.
		/*var b = this.avatar_container.add_widget(
			PureFW.Image,
			a.position.x + ((a.width - 42) >> 1),
			a.position.y + a.height - 15,
			42, 24,
			'ui/icons/labels/brezel/brezel_plus.png'
		);
		b.set_z_index(2);*/
		
		a.add_event_handler("click",
			function(ev) {
				UIButtons.open_friend_invitation(true);
			}
		);
	}
	else {
		a.add_event_handler("click", (function(_world_id, _map_id) {
			return function(ev) {
				MapVisitNavi.map_browser.friend_map_ids = null;
				MapVisitNavi.map_browser.choose_map_ids = null;
				MapVisitNavi.map_browser.choose_map_hrefs = null;
				MapVisitNavi.map_browser.map_data = new Array();
				MapVisitNavi.map_browser.friend_mode = false;
				MapVisitNavi.map_browser.choose_mode = false;
				MapVisitNavi.map_browser.show_back_to_my_map_link = true;
				MapUI.change_map(_world_id, _map_id, true, true);
			}
		})(world_id, map_id));
	}
	
	this.avatars.push(a);
};