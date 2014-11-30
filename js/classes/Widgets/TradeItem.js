/**
 * An Item which is being traded by players, but not sold in the shop.
 */

PureFW.TradeItem = function(parent_node, x, y, w, h, no_scale)
{
	if (typeof(parent_node) !== 'undefined') {
		this.init(parent_node, x, y, w, h, no_scale);
		
		this.instance_num = this.constructor.num_instances;
		this.instance_name = "PureFW.TradeItem" +
				".instances["	+this.instance_num+"]";
		
		this.id = "PureFW.TradeItem"+this.instance_num;
		this.content_id 
			= "PureFW.TradeItem_cont"+this.instance_num;
		this.bg_img_id = 'PureFW.TradeItem_bg'
			+this.instance_num;
		
		this.insert_into_dom_tree();
		this.constructor.num_instances++;
		this.constructor.instances[this.instance_num] = this;
		
		if (!this.no_scale)
			PureFW.WidgetManager.manager_all.register_widget(this);
	}
};

/********************************************************************
 * PureFW.TradeItem extends Container
 ********************************************************************/
PureFW.TradeItem.extend(PureFW.Container);

/********************************************************************
 * Statische Deklarationen
 ********************************************************************/
PureFW.TradeItem.num_instances = 0;		// Instanzüberwachung (anzahl)
PureFW.TradeItem.instances = new Array();// Instanzüberwachung (Instanzen)


/********************************************************************
 * Prototyp-Deklarationen
 ********************************************************************/
PureFW.TradeItem.prototype.init = function(parent_node, 
	x, y, w, h, no_scale)
{
	PureFW.TradeItem.parent.init.call(this, parent_node, 
		x, y, (w || 178), (h || 245), no_scale);

	this.item_polaroid = null;
	
	this.avatar_container = null;
	this.name_container = null;
};

PureFW.TradeItem.prototype.insert_into_dom_tree = function() 
{
	PureFW.TradeItem.parent.insert_into_dom_tree.call(this);

	this.item_polaroid = this.add_widget(
		PureFW.Container,
		0, 0,
		178, 193
	);
	this.item_polaroid.set_bg_img(
		'ui/backgrounds/PolaroidContainer/paper_178x193.png'
	);
	
	
	this.sub_title = this.item_polaroid.add_widget(
		PureFW.Container,
		0, this.item_polaroid.height - 30,
		this.item_polaroid.width, 30
	);
	this.sub_title.set_text_align('center');
	this.sub_title.set_font_weight('bold');
	
	this.name_container = this.add_widget(
		PureFW.Container,
		52, 
		this.height - 44,
		0, 0);
};

PureFW.TradeItem.prototype.set_avatar = function(pic, nick)
{
	try {
		this.avatar_container.destroy();
	}
	catch(e) {}
	this.avatar_container = this.add_widget(
		PureFW.Avatar.Avatar,
		2, this.height - 48,
		48, 48,
		pic,
		nick
	);
};

PureFW.TradeItem.prototype.set_sub_title = function(text)
{
	this.sub_title.set_content(text);
};

PureFW.TradeItem.prototype.set_item = function(item) {
	this.item_polaroid.set_bg_img(
		'/ui/backgrounds/TradeItem/items/'+item.type+'/'+item.id+'.png'
	);
};

PureFW.TradeItem.prototype.set_text = function(txt)
{
	this.name_container.set_content(txt);
};