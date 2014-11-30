/**
 * Diese Officer-Box ist ein spezieller Container, in dem der Officer im
 * Container angezeigt wird und der Officer-Text daneben steht (also keine
 * Sprechblase, sondern alles in einer Box).
 */

PureFW.OfficerBox = function (parent_node, x, y, h, w, officer, buttons) {
	if (typeof(parent_node) !== 'undefined') 
	{
		this.init(parent_node, x, y, h, w, officer, buttons);
		
		this.instance_num = this.constructor.num_instances;
		this.instance_name = 
			"PureFW.OfficerBox.instances[" + this.instance_num + "]";
		
		this.id = "OfficerBox"+this.instance_num;
		this.bg_img_id = "OfficerBox_bg"+this.instance_num
		this.content_id = "OfficerBox_cont"+this.instance_num;
		this.grey_layer_node_id ="OfficerBox_grey_layer"+this.instance_num;
		
		this.insert_into_dom_tree();
		this.constructor.num_instances++;
		this.constructor.instances[this.instance_num] = this;
		PureFW.WidgetManager.manager_all.register_widget(this);
	}
}

/********************************************************************
* OfficerBox extends ConfirmationBox
********************************************************************/
PureFW.OfficerBox.extend(PureFW.ConfirmationBox);

/********************************************************************
 * Statische Deklarationen
 ********************************************************************/
PureFW.OfficerBox.num_instances = 0;
PureFW.OfficerBox.instances = new Array();

/********************************************************************
 * Prototype-Deklarationen
 ********************************************************************/
/**
 * @see PureFW.Container.prototype#init
 */
PureFW.OfficerBox.prototype.init = function(parent_node, x, y, w, h, 
	officer, buttons) 
{
	PureFW.OfficerBox.parent.init.call(this, parent_node, x, y, w, h, 
		buttons || PureFW.ConfirmationBox.YES, false);
	
	this.officer = officer || PureFW.Officer.OFFICERS.councilor_small;
	
	this.officer_img = null;
	this.text_cont = null;
};

/**
 * @see PureFW.Container.prototype#insert_into_dom_tree
 */
PureFW.OfficerBox.prototype.insert_into_dom_tree = function() {
	PureFW.OfficerBox.parent.insert_into_dom_tree.call(this);
	
	this.officer_img = new PureFW.Image(
		this,
		10, (this.height - this.officer.height) >> 1,
		this.officer.width, this.officer.height,
		this.officer.pic_left
	);
	
	this.text_cont = new PureFW.Container(
		this,
		this.officer_img.position.x + this.officer_img.width + 10,
		10,
		this.width -(this.officer_img.position.x + this.officer_img.width + 20),
		this.height - 20
	);
}

/**
 * @see PureFW.Container.prototype#add_content
 * @param string text
 */
PureFW.OfficerBox.prototype.add_content = function(text) {
	this.text_cont.add_content(text);
}

PureFW.OfficerBox.prototype.add_widget = function(widget_constructor,
	a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12)
{
	return this.text_cont.add_widget(widget_constructor,
		a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12);
}

/**
 * @see PureFW.Container.prototype#set_content
 * @param string text
 */
PureFW.OfficerBox.prototype.set_content = function(text) {
	this.text_cont.set_content(text);
}

/**
 * Ändert den Officer, der angezeigt werden soll
 * 
 * @param PureFW.Officer.OFFICERS officer
 */
PureFW.OfficerBox.set_officer = function(officer) {
	this.officer = officer;
	this.officer_img.set_width(this.officer.width);
	this.officer_img.set_height(this.officer.height);
	this.officer_img.set_y((this.height - this.officer.height)>>1);
	this.officer_img.set_pic_url(this.officer.pic_left);
}