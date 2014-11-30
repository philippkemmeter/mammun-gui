/**
 * @author Alex
 */

PureFW.UnitSlot = function(parent_node, x, y, w, h) 
{
	this.on_change_functions = new Array();
	this.bg_img = null;
	this.bg_img_node = null;
	this.node = null;
	this.content_node = null;
	
	if (typeof(parent_node) !== 'undefined') 
	{
		this.init(parent_node, x, y, w, h);
	
		this.instance_num = this.constructor.num_instances;
		this.instance_name = "PureFW.UnitSlot.instances["+this.instance_num+"]";
		
		this.id = "UnitSlot"+this.instance_num;
		this.bg_img_id = "UnitSlot_bg_img"+this.instance_num;
		this.content_id = "UnitSlot_cont"+this.instance_num;
		
		this.unit_name = null;
		//this.unit_description = null;
		this.unit_time = null;
		this.unit_pic_url = null;
		
		this.name_container = null;
		//this.desc_container = null;
		this.timer_container = null;
		this.pic_container = null;

		this.on_empty_function = null;
		
		this.insert_into_dom_tree();
		this.constructor.num_instances++;
		this.constructor.instances[this.instance_num] = this;
		
		PureFW.WidgetManager.manager_all.register_widget(this);
	}
};

/********************************************************************
 * UnitSlot Bar extends Widget
 ********************************************************************/
PureFW.UnitSlot.extend(PureFW.Container);

/********************************************************************
 * Statische Deklarationen
 ********************************************************************/
PureFW.UnitSlot.num_instances = 0;
PureFW.UnitSlot.instances = new Array();

/********************************************************************
 * Prototyp-Deklarationen
 ********************************************************************/
PureFW.UnitSlot.prototype.insert_into_dom_tree = function() 
{
	PureFW.UnitSlot.parent.insert_into_dom_tree.call(this);
	
	this.set_bg_img('ui/backgrounds/unitBgs/unit_slot.png');
	
	this.blank_container = new PureFW.Container(
			document.getElemById(this.id),
			this.x, this.y,
			this.width, this.height);
	this.blank_container.parent_container = this;
	this.blank_container.set_bg_img('ui/elements/unit_slots/shadow.png');
	this.blank_container.hide();
	
	this.pic_container = new PureFW.Container(
			document.getElemById(this.id),
			0, 0,
			this.width, this.height);
	this.pic_container.parent_container = this;
	
	this.pic_container.set_text_align('center');
	this.pic_container.set_font_size(1.5);

	this.name_container = new PureFW.Container(
			document.getElemById(this.id),
			10, this.height - 45,
			this.width - 20, 20);
	this.name_container.parent_container = this;
	this.name_container.set_font_weight('bold');
	this.name_container.set_font_size('1.1');
	
	/*this.desc_container = new PureFW.Container(
			document.getElemById(this.id),
			10, this.height - 60,
			this.width - 20, 15);
	this.desc_container.parent_container = this;
	this.desc_container.set_positioning('relative');
	this.desc_container.get_node().style.overflow = 'hidden';*/
	
	this.timer_container = new PureFW.Container(
			document.getElemById(this.id),
			10, this.height - 25,
			this.width - 20, 20);
	this.timer_container.parent_container = this;
	
	this.hide_content();
	
	this.parent_node.appendChild(this.node);
};

PureFW.UnitSlot.prototype.display_empty = function(on_empty)
{
	this.on_empty_function = on_empty;
	this.hide_content();
	this.blank_container.add_event_handler("click", on_empty);
	this.pic_container.add_event_handler("click", on_empty);
	this.blank_container.show();
};

PureFW.UnitSlot.prototype.hide_content = function()
{
	this.pic_container.hide();
	this.name_container.hide();
	//this.desc_container.hide();
	this.timer_container.hide();
	this.blank_container.hide();
};

PureFW.UnitSlot.prototype.show_content = function()
{
	if(this.on_empty_function)
	{
		this.blank_container.remove_event_handler("click", this.on_empty_function);
		this.pic_container.remove_event_handler("click", this.on_empty_function);
	}
	this.pic_container.show();
	this.name_container.show();
	//this.desc_container.show();
	this.timer_container.show();
	this.blank_container.hide();
};

/**
 * -----------------------------------------------------------------------------
 * Getters/Setters follow... ---------------------------------------------------
 * -----------------------------------------------------------------------------
 */

PureFW.UnitSlot.prototype.set_unit_name = function(name)
{
	this.unit_name = name;
	this.name_container.set_content(this.unit_name);
	this.name_container.repaint();
};

PureFW.UnitSlot.prototype.get_unit_name = function()
{
	return this.unit_name;
};
/*
PureFW.UnitSlot.prototype.set_unit_description = function(desc)
{
	this.unit_description = desc;
	this.desc_container.set_content(this.unit_description);
	this.desc_container.repaint();
};*/
/*
PureFW.UnitSlot.prototype.get_unit_description = function()
{
	return this.unit_description;
};*/

PureFW.UnitSlot.prototype.set_unit_time = function(t)
{
	this.unit_time = t;
	this.timer_container.set_content(this.unit_time);
	this.timer_container.repaint();
};

PureFW.UnitSlot.prototype.get_unit_time = function()
{
	return this.unit_time;
};

PureFW.UnitSlot.prototype.set_unit_pic_url = function(url)
{
	this.unit_pic_url = url;
	this.pic_container.set_bg_img(this.unit_pic_url);
	this.pic_container.repaint();
};

/**
 * Setzt einen String, der komplett über den gesamten Slot drüber geschrieben
 * werden soll. Damit kann ein leerer Slot mit der Aufforderung draufzuklicken
 * versehen werden.
 * 
 * @param String str
 */
PureFW.UnitSlot.prototype.set_overlay_text = function(str) {
	this.pic_container.set_content(str);
	this.pic_container.show();
};

PureFW.UnitSlot.prototype.repaint = function()
{
	PureFW.UnitSlot.parent.repaint.call(this);
};