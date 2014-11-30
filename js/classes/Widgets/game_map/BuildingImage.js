/**
 * Diese Klasse stellt die Abbildung eines Gebäudes dar.
 */

/**
 * @param HTMLElement parent_node
 * @param int x
 * @param int y
 * @param uint w
 * @param uint h
 * @param string url
 */
PureFW.BuildingImage = function(parent_node, x, y, w, h, b_obj, no_scale) {
	if (typeof(parent_node) !== 'undefined') {
		this.init(parent_node, x, y, w, h, b_obj, no_scale);
		
		this.instance_num = this.constructor.num_instances;
		this.instance_name = "PureFW.BuildingImage.instances["+this.instance_num+"]";
		this.id = "BuildingImage"+this.instance_num;
		this.bg_img_id = "BuildingImage_bg_img"+this.instance_num;
		this.content_id = "BuildingImage_cont"+this.instance_num;
		
		this.insert_into_dom_tree();

		this.constructor.instances[this.instance_num] = this;
		this.constructor.num_instances++;
		if (!this.no_scale)
			PureFW.WidgetManager.manager_all.register_widget(this);
	}
};
/********************************************************************
* BuildingImage extends Container
********************************************************************/
PureFW.BuildingImage.extend(PureFW.Container);

/********************************************************************
 * Statische Deklarationen
 ********************************************************************/
PureFW.BuildingImage.num_instances = 0;
PureFW.BuildingImage.instances = new Array();

/********************************************************************
 * Prototype-Deklarationen
 ********************************************************************/
PureFW.BuildingImage.prototype.init = function(parent_node, x, y, w, h,
	b_obj, no_scale) 
{
	PureFW.BuildingImage.parent.init.call(this, parent_node, x, y, w, h, 
		no_scale);
	this.b_obj = b_obj;
	this.b_img = null;
	this.b_ani = null;
	this.xp_flag = null;
};

/**
 * Fügt das Widget in den DOM-Baum ein
 * 
 * @see Widgets/Widget#insert_into_dom_tree
 */
PureFW.BuildingImage.prototype.insert_into_dom_tree = function() {
	PureFW.BuildingImage.parent.insert_into_dom_tree.call(this);
	
	this.b_img = this.add_widget(
		PureFW.Image,
		0, 0,
		this.width, this.height,
		'map/buildings/'+this.b_obj.pic_url,
		this.no_scale
	);
	
	if (this.b_obj.xp_level > 0) {
		this._create_flag();
	}
}

/**
 * Erzeugt die XP-Flagge am Gebäude und zeigt sie an.
 * 
 * @access private
 */
PureFW.BuildingImage.prototype._create_flag = function() {
	return;	
	
	/**
	 * Flagge sah nicht gut aus, Feature deaktiviert.
	 */
	
	this.flag_size = (this.width / this.b_obj.size)*0.75;
	this.xp_flag = this.add_widget(
		PureFW.Image,
		this.width >> 1, 
		this.height - this.flag_size,
		this.flag_size, this.flag_size,
		'map/buildings/xp_flag.png',
		this.no_scale
	);
	this.xp_flag.set_z_index(this.b_img.get_z_index() + 1);
}

/**
 * Setzt die Gebäudedaten durch das Gebäude-Objekt <code>b_obj</code>.
 * 
 * @param Object b_obj
 */
PureFW.BuildingImage.prototype.set_b_obj = function(b_obj) {
	this.b_obj = b_obj;
	this.b_img.set_pic_url('map/buildings/'+this.b_obj.pic_url);
	if (this.b_obj.xp_level > 0) {
		if (!this.xp_flag) {
			this._create_flag();
		}
	}
	else {
		if (this.xp_flag) {
			this.xp_flag.destroy();
			this.xp_flag = null;
		}
	}
}

/**
 * Setzt eine Animation zum Abspielen, die statt des Gebäudebildes angezeigt
 * werden soll.
 * 
 * @param String pic_url		URL zum Filmstreifen
 * @param uint frames			Anzahl der Einzelbilder im Streifen
 * @param uint frame_delay		Pause zwischend den Einzelbildern
 * @param uint strips			Wieviele Filmstreifen die Animation hat
 * @return Widget				Zeiger auf das FilmAnimator-Widget
 */
PureFW.BuildingImage.prototype.set_ani_as_img = function(pic_url, 
	frames, frame_delay, strips)
{
	if (this.b_ani) {
		this.b_ani.destroy();
		this.b_ani = null;	// ist zwar schon im Konstruktor, aber sicherer so
	}
	this.b_img.hide();
	
	this.b_ani = this.add_widget(
		PureFW.FilmAnimator,
		0, 0,
		this.width, this.height,
		this.no_scale
	);
	this.b_ani.set_image(
		pic_url, frames, strips
	);
	this.b_ani.set_frame_delay(frame_delay);
	this.b_ani.add_event_handler("destroy", (function(_this) {
		return function (ev) {
			_this.b_img.show();
			_this.b_ani = null;
		}
	})(this));
	
	this.b_ani.start_animation();
	return this.b_ani;
}