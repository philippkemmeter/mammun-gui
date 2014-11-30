/**
 * Ein QuantityDisplay, der HTML oder Widgets enthalten kann.
 * 
 * @author David, Phil
 */

/**
 * @param HTMLElement parent_node
 * @param int x
 * @param int y
 * @param uint w	(wenn <= 0, dann wird sie dynamisch bestimmt)
 * @param uint h	(wenn <= 0, dann wird sie dynamisch bestimmt)
 */
PureFW.QuantityDisplay = function(parent_node, x, y, w, h, no_scale) {
	if (typeof(parent_node) !== 'undefined') {
		this.init(parent_node, x, y, w, h, no_scale);
	
		this.instance_num = this.constructor.num_instances;
		this.instance_name = "PureFW.QuantityDisplay.instances["+this.instance_num+"]";
		
		this.id = "QuantityDisplay"+this.instance_num;
		this.bg_img_id = "QuantityDisplay_bg_img"+this.instance_num;
		this.content_id = "QuantityDisplay_cont"+this.instance_num;
		
		this.insert_into_dom_tree();
		this.constructor.num_instances++;
		this.constructor.instances[this.instance_num] = this;
		
		if (!this.no_scale)
			PureFW.WidgetManager.manager_all.register_widget(this);
	}
};
/********************************************************************
 * QuantityDisplay Bar extends Widget
 ********************************************************************/
PureFW.QuantityDisplay.extend(PureFW.Container);

/********************************************************************
 * Statische Deklarationen
 ********************************************************************/
PureFW.QuantityDisplay.num_instances = 0;
PureFW.QuantityDisplay.instances = new Array();

/********************************************************************
 * Prototyp-Deklarationen
 ********************************************************************/
PureFW.QuantityDisplay.prototype.init = function(parent_node, x, y, w, h, no_scale)
{
	PureFW.QuantityDisplay.parent.init.call(this, parent_node, x, y, w, h, no_scale);
	this.on_change_functions = new Array();
	this.bg_img = null;
	this.bg_img_node = null;
	this.node = null;
	this.content_node = null;
	this.added_widgets = new Array();
	this.item_amount = 0;
	this.item_amount_mp = 0;
	
	this.amount_container = null;
	this.amount_text = null;
	this.label = null;
	this.label_text = null;
};

PureFW.QuantityDisplay.prototype.insert_into_dom_tree = function() 
{
	PureFW.QuantityDisplay.parent.insert_into_dom_tree.call(this);
	
	//@TODO verschiedene positionierungen der zahl ne nach anzahl der stellen.
	this.amount_text = this.add_widget(
		PureFW.Container,
		0, 12,
		this.width, 20
	);
	this.amount_text.set_css_class('ChestItem_amount')
	this.hide();
	this.set_bg_img('ui/backgrounds/ChestItem/amount_bg.png');
	
}

PureFW.QuantityDisplay.prototype.set_quantity = function(quantity)
{
	this.item_amount = quantity;

	if (this.item_amount <= 0) {
		this.set_content('');
	}
	this.amount_text.set_content(this.item_amount);
	if (this.item_amount > 0)
		this.show();
	else
		this.hide();
}
