/**
 * Der AJAXInfoBalloon repräsentiert eine Blase, die auf ein Objekt zeigt, und dieses
 * näher erläutert. Er lädt seinen Inhalt per AJAX in sich hinein :)
 * Die Breite ist dabei fest und die Höhe wird automatisch bestimmt.
 * 
 * @author Phil
 */

/**
 * @param HTMLElement parent_node
 * @param int x			x-Position
 * @param int y			y-Position
 * @param string url	URL
 */
AJAXInfoBalloon = function(parent_node, x,y,url) {
	this.init(parent_node, x, y, 200, 0, true);
	this.url = url;
	this.parent_node = parent_node;
	
	this.instance_num = this.constructor.num_instances;
	this.instance_name = "AJAXInfoBalloon.instances["+this.instance_num+"]";
	
	this.id = "AJAXInfoBalloonWin"+this.instance_num;
	this.bg_img_id = "AJAXInfoBalloon_bg_img"+this.instance_num;
	this.content_id = "AJAXInfoBalloon_cont"+this.instance_num;

	this.position = new PureFW.Point(x||0,y||0);
	this.top = null;
	this.mid = null;
	this.body = null;
	this.bot = null;
		
	if (this.parent_node)
		this.insert_into_dom_tree();

	this.constructor.instances[this.instance_num] = this;
	this.constructor.num_instances++;
};

/********************************************************************
 * AJAXInfoBalloon extends PureFW.Container
 ********************************************************************/
AJAXInfoBalloon.extend(PureFW.Container);

/********************************************************************
 * Statische Deklarationen
 ********************************************************************/
AJAXInfoBalloon.num_instances = 0;
AJAXInfoBalloon.instances = new Array();

/********************************************************************
 * Prototyp-Deklarationen
 ********************************************************************/

/**
 * Zerstört das Widget und alle enthaltenen Widgets.
 */
AJAXInfoBalloon.prototype.destroy = function() {
	AJAXInfoBalloon.parent.destroy.call(this);
	this.body.destroy();
	this.top.destroy();
	this.mid.destroy();
	this.bot.destroy();
	this.body = null;
	this.top = null;
	this.mid = null;
	this.bot = null;
	this.constructor.instances.remove(this);
};

/**
 * Setzt die URL, die das AJAXInfoBalloon ansurfen soll
 * 
 * @param string url
 */
AJAXInfoBalloon.prototype.set_url = function(url) {
	this.body.set_url(url);
};

/**
 * Gibt die zuletzt angesurfte URL an
 * 
 * @return string
 */
AJAXInfoBalloon.prototype.get_url = function() {
	return this.body.get_url();
};
/**
 * Fügt den AJAXInfoBalloon in den DOM-Baum ein
 */
AJAXInfoBalloon.prototype.insert_into_dom_tree = function() {
	AJAXInfoBalloon.parent.insert_into_dom_tree.call(this);
	this.get_content_node().style.overflow = 'visible';
	this.get_node().style.overflow = 'visible';

	this.top = new PureFW.Container(
		this.node,
		0, 0,
		this.width,	82, true
	);
	this.top.set_id("AJAXInfoBalloon_top"+this.instance_num);
	this.top.set_bg_img( 
		'ui/backgrounds/info_balloon/info_balloon_top'+this.width+'.png', false
	);
	
	this.mid = new PureFW.Container(
		this.node,
		0, 82,
		this.width, 0, true
	);
	this.mid.set_id("AJAXInfoBalloon_mid"+this.instance_num);
	this.mid.set_bg_img(
			'ui/backgrounds/info_balloon/info_balloon_mid'+this.width+'.png', false
	);
	this.mid.set_bg_repeat('repeat-y');
	
	this.body = new PureFW.ContainerAJAX(
		this.node,
		7, 7,
		this.width-50, 0,
		this.url, true
	);
	this.body.set_id("AJAXInfoBalloon_body"+this.instance_num);
	this.body.set_z_index(2);
	
	this.bot = new PureFW.Container(
		this.node,
		0, 82,
		this.width, 24, true
	);
	this.bot.set_id("AJAXInfoBalloon_bot"+this.instance_num);
	this.bot.set_bg_img(
		'ui/backgrounds/info_balloon/info_balloon_bot'+this.width+'.png', false
	);
	
	this.close_button = new PureFW.Image(
		this,
		this.width - 65, -5,
		28, 28, 
		'ui/elements/form/buttons/btn_no.png', true);
	this.close_button.add_event_handler('click', 
		(function(_instance) {
			return function(ev) {
				_instance.hide();
			}
		})(this)
	);
	this.close_button.set_z_index(10);
	
	this.hide();
	this.body.add_event_handler("load", 
		(function(n1){
			return function(){AJAXInfoBalloon.instances[n1].adjust_height()}
		})(this.instance_num)
	);
};

/**
 * Korrigiert die Höhe des Balloons. Muss i.d.R. nicht von Hand
 * aufgerufen werden.
 */
AJAXInfoBalloon.prototype.adjust_height = function() {
	var h = this.body.get_height() - 75;
	if (h <= 0)
		h = 0;
	this.mid.set_height(h);
	this.bot.set_y(82+h);
};

/**
 * Gibt zurück, wo der Schniepel der Blase ist
 */
AJAXInfoBalloon.prototype.get_arrowhead_pos = function() {
	return new PureFW.Point(this.get_width(), 82);
};
