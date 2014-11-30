/**
 * Ein Item im Schatzkiste bzw. das Symbol, das dieses darstellt. Besteht aus Bild,
 * Namen, und andere Information. Kann zusätzlich
 * ein Hintergrundbild erhalten.
 * 
 * @author Alex, Phil
 */


/**
 * @param HTMLElement parent_node
 * @param int x
 * @param int y
 * @param uint w
 * @param uint h
 */
PureFW.ChestItem = function(parent_node, x, y, w, h) 
{
	if (typeof(parent_node) !== 'undefined') 
	{
		this.init(parent_node, x, y, w, h);
	
		this.instance_num = this.constructor.num_instances;
		this.instance_name = "PureFW.ChestItem.instances["+this.instance_num+"]";
		
		this.id = "ChestItem"+this.instance_num;
		this.bg_img_id = "ChestItem_bg_img"+this.instance_num;
		this.content_id = "ChestItem_cont"+this.instance_num;
		
		this.insert_into_dom_tree();
		this.constructor.num_instances++;
		this.constructor.instances[this.instance_num] = this;
		
		PureFW.WidgetManager.manager_all.register_widget(this);
	}
};
/********************************************************************
 * ChestItem Bar extends Container
 ********************************************************************/
PureFW.ChestItem.extend(PureFW.Container);

/********************************************************************
 * Statische Deklarationen
 ********************************************************************/
PureFW.ChestItem.num_instances = 0;
PureFW.ChestItem.instances = new Array();

/********************************************************************
 * Prototyp-Deklarationen
 ********************************************************************/
PureFW.ChestItem.prototype.init = function(parent_node, x, y, w, h) 
{
	PureFW.ChestItem.parent.init.call(this, parent_node, x, y, w, h);
	this.item_amount = 0;
	this.item_amount_mp = 0;
	
	this.amount_container = null;
	this.amount_text = null;
	this.label = null;
	this.label_text = null;
};

PureFW.ChestItem.prototype.insert_into_dom_tree = function() 
{
	PureFW.ChestItem.parent.insert_into_dom_tree.call(this);
	
	this.set_bg_img(
		'ui/backgrounds/PolaroidContainer/paper_178x193.png'
	);
	this.set_overflow('visible');
	
	this.amount_container = this.add_widget(
		PureFW.Container,
		this.width-38, -10,
		48,48
	);
	this.amount_container.set_bg_img(
		'ui/backgrounds/ChestItem/amount_bg.png'
	);
	this.amount_container.set_css_class('ChestItem_amount')
	this.amount_text = this.amount_container.add_widget(
		PureFW.Container,
		0, 12,
		this.amount_container.width, 20
	);
	this.amount_container.hide();
	
	
	this.label = this.add_widget(
		PureFW.Container,
		(178 - 168) >> 1, 137, 
		169, 50
	);
	this.label.set_css_class('ChestItem_label')
	this.label_text = this.label.add_widget(
		PureFW.Container,
		60, 15,
		60, 20
	);
	
	this.sub_title = this.add_widget(
		PureFW.Container,
		0, this.height - 40,
		this.width, 30
	);
	this.sub_title.set_text_align('center');
	this.sub_title.set_font_weight('bold');
};

PureFW.ChestItem.prototype.set_item = function(item) {
	this.item_type = item.type;
	this.item_id = item.id;
	this.item_amount_total = parseInt(item.amount) + parseInt(item.amount_mp);
	this.item_amount_mp = item.amount_mp;
	this.cost = item.price;
	
	this.set_bg_img(
		'ui/backgrounds/ChestItem/items/'+item.type+'/'+item.id+'.png'
	);
	
	if (this.item_amount_mp <= 0) {
		this.label.set_bg_img(
			'ui/elements/texts/'+MammunUI.LNG+'/ChestItem/label_mine.png'
		);
		this.label_text.set_content('');
	}
	else if (this.cost <= 0) {
		this.label.set_bg_img(
			'ui/backgrounds/ChestItem/label_bit.png'
		);
		this.label_text.set_content('');
	}
	else {
		this.label.set_bg_img(
			'ui/backgrounds/ChestItem/klunker_bg.png'
		);
		this.label_text.set_content(this.cost);
	}
	
	this.amount_text.set_content(this.item_amount_total);
	if (this.item_amount_total > 0)
		this.amount_container.show();
	else
		this.amount_container.hide();
};

PureFW.ChestItem.prototype.set_amount = function(amt) {
	this.item_amount = amt;

	if (this.item_amount <= 0) {
		this.label.set_bg_img(
			'ui/elements/texts/de/ChestItem/label_mine.png'
		);
		this.label_text.set_content('');
	}
	this.amount_text.set_content(this.item_amount);
	if (this.item_amount > 0)
		this.amount_container.show();
	else
		this.amount_container.hide();
	
}

PureFW.ChestItem.prototype.set_sub_title = function(text)
{
	this.sub_title.set_content(text);
};