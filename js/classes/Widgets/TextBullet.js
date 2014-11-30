/**
 * A text piece, in bullet format.
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
PureFW.TextBullet = function(parent_node, x, y, w, h) {
	if (typeof(parent_node) !== 'undefined') {
		if ((typeof(x) == 'undefined'))
		{
			this.init(parent_node, 0, 0, 0, 0);
		}
		else
		{
			this.init(parent_node, x, y, w, h);
		}
	
		this.instance_num = this.constructor.num_instances;
		this.instance_name = "PureFW.TextBullet.instances["+this.instance_num+"]";
		
		this.id = "TextBullet"+this.instance_num;
		this.bg_img_id = "TextBullet_bg_img"+this.instance_num;
		this.content_id = "TextBullet_cont"+this.instance_num;
		
		this.insert_into_dom_tree();
		this.constructor.num_instances++;
		this.constructor.instances[this.instance_num] = this;
		
		PureFW.WidgetManager.manager_all.register_widget(this);
	}
};
/********************************************************************
 * TextBullet Bar extends Widget
 ********************************************************************/
PureFW.TextBullet.extend(PureFW.Container);

/********************************************************************
 * Statische Deklarationen
 ********************************************************************/
PureFW.TextBullet.num_instances = 0;
PureFW.TextBullet.instances = new Array();

/********************************************************************
 * Prototyp-Deklarationen
 ********************************************************************/
PureFW.TextBullet.prototype.init = function(parent_node, x, y, w, h) {
	PureFW.TextBullet.parent.init.call(this, parent_node, x, y, w, h);
	this.on_change_functions = new Array();
	this.bg_img = null;
	this.bg_img_node = null;
	
	this.bullet_container = null;
	this.text_container = null;
};

PureFW.TextBullet.prototype.insert_into_dom_tree = function() {
	PureFW.TextBullet.parent.insert_into_dom_tree.call(this);

	this.set_positioning('relative');
	
	this.bullet_container = new PureFW.Image(
		this,
		0, 4,
		7, 7);
	this.bullet_container.set_positioning('relative');
//	this.bullet_container.set_y(4);
	this.bullet_container.set_float('left');
	this.bullet_container.set_pic_url('ui/backgrounds/faq/square_bullet.png');
	
	this.text_container = new PureFW.Container(
		this,
		0, 0,
		0, 0);
//	this.text_container.set_x(10);
	this.text_container.set_padding_left(10);
	this.text_container.set_padding_right(30); // cheap workaround
	this.text_container.set_float('left');
	this.text_container.get_node().style.cursor = 'pointer';
	this.text_container.add_event_handler(
		"mouseover",
		(function(_instance) {
			return function(ev) {
				_instance.set_font_color('#990000');
			};
		})(this.text_container)
	);
	this.text_container.add_event_handler(
		"mouseout",
		(function(_instance) {
			return function(ev) {
				_instance.set_font_color('#366A9B');
			};
		})(this.text_container)
	);
	this.text_container.set_positioning('relative');
	this.text_container.set_font_color('#366A9B');
	
	this.get_node().style.clear='both';
};


/**
 * Outsourcing the functions to the Text Container
 */
PureFW.TextBullet.prototype.set_content = function(text, interprete_js)
{
	this.text_container.set_content(text, interprete_js);
};

PureFW.TextBullet.prototype.add_content = function(text) 
{
	this.text_container.add_content(text);
};

PureFW.TextBullet.prototype.get_content = function() 
{
	this.text_container.get_content();
};

//PureFW.TextBullet.prototype.get_content_node = function() 
//{
//	this.text_container.get_content_node();
//};