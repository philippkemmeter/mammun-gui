/**
 * Repräsentiert eine Person auf der Karte.
 * Personen werden immer absolut positioniert (relativ zum parent_node).
 * 
 * @author Phil
 */
Person = function(parent_node, x, y, w, h, pic, color) {
	if (typeof(parent_node) !== 'undefined') {
	 	this.init(parent_node, x, y, w, h, pic, color);
	 	this.instance_num = this.constructor.num_instances;
		this.instance_name = "Person.instances["+this.instance_num+"]";

		this.id = 'Person'+this.instance_num;
		this.bg_img_id = 'PersonBg'+this.instance_num;
		this.content_id = 'PersonCont'+this.instance_num;
		
		Person.instances[this.instance_num] = this;
		this.constructor.num_instances++;
		this.insert_into_dom_tree();
		
		PureFW.WidgetManager.manager_all.register_widget(this);
	}
};

/********************************************************************
 * Person EXTENDS Container
 ********************************************************************/
Person.extend(PureFW.Container);

/********************************************************************
 * Statische Deklarationen
 ********************************************************************/
Person.num_instances = 0;			// Instanzüberwachung (Anzahl)
Person.instances = new Array();

/********************************************************************
 * Prototyp-Deklarationen
 ********************************************************************/
Person.prototype.init = function(parent_node, x, y, w, h, pic, color) {
	Person.parent.init.call(this, parent_node, x, y, w, h);
	this.color = color || 'neutral';
	this.pic_url = pic;

	this.color_circle_pic_url = '../pix/map/units/markers/normal/'
		+ this.color + '.png';
	this.color_circle_pic_url_hover = '../pix/map/units/markers/hover/'
		+ this.color + '.png';
	
	this.hp = 1;
	this.max_hp = 1000;
	
	this.hp_bar = null;
	this.img = null;
};

Person.prototype.insert_into_dom_tree = function() {
	Person.parent.insert_into_dom_tree.call(this);

	this.set_bg_img(
		this.color_circle_pic_url
	);
	
	this.img = new PureFW.Image(
		this,
		0, 0,
		this.width, this.height,
		this.pic_url
	);
	
	this.hp_bar = new HealthBar(this.parent_node, 
		this.position.x, this.position.y + this.height, this.width, 3);
	this.hp_bar.set_value(this.hp);
	this.hp_bar.set_max_value(this.max_hp);
	this.hp_bar.set_z_index(this.get_z_index());
	
	this.add_event_handler("mouseover", function(ev) {
		this.set_bg_img(this.color_circle_pic_url_hover);
	});
	this.add_event_handler("mouseout", function(ev) {
		this.set_bg_img(this.color_circle_pic_url);
	});
};

/**
 * Setzt die Lebenspunkte, die die Person aktuell hat.
 * 
 * @param uint h
 */
Person.prototype.set_hp = function(h) {
	if (h > this.max_hp)
		this.hp = this.max_hp;
	else if (h < 1)
		this.hp = 1;
	else
		this.hp = h;
	
	this.hp_bar.set_value(this.hp);
};

/**
 * @see PureFW.Widget#set_z_index
 */
Person.prototype.set_z_index = function(z) {
	Person.parent.set_z_index.call(this, z);
	this.hp_bar.set_z_index(z);
}