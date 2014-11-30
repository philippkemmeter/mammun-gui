/**
 * Ein Item im Shop bzw. das Symbol, das dieses darstellt. Besteht aus Bild,
 * Namen, Information aus welchem Shop es stammt sowie Preis. Kann zusätzlich
 * ein Hintergrundbild erhalten.
 * 
 * @author Phil
 */

/**
 * @param HTMLElement parent_node
 * @param int x
 * @param int y
 * @param uint w
 * @param uint h
 */
PureFW.ShopItem = function(parent_node, x, y, w, h) {
	if (typeof(parent_node) !== 'undefined') {
		this.init(parent_node, x, y, w, h);
	
		this.instance_num = this.constructor.num_instances;
		this.instance_name = "PureFW.ShopItem.instances["+this.instance_num+"]";
		
		this.id = "ShopItem"+this.instance_num;
		this.bg_img_id = "ShopItem_bg_img"+this.instance_num;
		this.content_id = "ShopItem_cont"+this.instance_num;
		
		this.insert_into_dom_tree();
		this.constructor.num_instances++;
		this.constructor.instances[this.instance_num] = this;
		
		PureFW.WidgetManager.manager_all.register_widget(this);
	}
};
/********************************************************************
 * ShopItem Bar extends Container
 ********************************************************************/
PureFW.ShopItem.extend(PureFW.Container);

/********************************************************************
 * Statische Deklarationen
 ********************************************************************/
PureFW.ShopItem.num_instances = 0;
PureFW.ShopItem.instances = new Array();

PureFW.ShopItem.SHOP_BRONZE = 1;
PureFW.ShopItem.SHOP_SILVER = 2;
PureFW.ShopItem.SHOP_GOLD = 3;

/********************************************************************
 * Prototyp-Deklarationen
 ********************************************************************/
PureFW.ShopItem.prototype.init = function(parent_node, x, y, w, h) {
	PureFW.ShopItem.parent.init.call(this, parent_node, x, y, w, h);
	this.item_name = '';
	this.item_type = 0;
	this.item_id = 0;
	this.item_price = 0;
	this.item_shop = 0;
	this.item_amount = 0;
	this.item_discount = 0;
	
	this.item_name_container = null;
	this.item_pic_container = null;
	this.item_shop_symbol_container = null;
	this.item_price_container = null;
	this.item_price_container_inner = null;
	this.item_sold_out_img = null;
	this.item_amount_img = null;
	this.item_discount_img = null;
};

PureFW.ShopItem.prototype.insert_into_dom_tree = function() {
	PureFW.ShopItem.parent.insert_into_dom_tree.call(this);
};


PureFW.ShopItem.prototype.set_name = function(name) {
	if (!this.item_name_container) {
		this.item_name_container = new PureFW.Container(
				this,
				10,15,
				this.width-20, 40
		);
		this.item_name_container.set_font_size(1.2);
		this.item_name_container.set_font_weight('bold');
		this.item_name_container.set_text_align('center');
		this.item_name_container.set_z_index(1);
	}
	this.item_name = name;
	this.item_name_container.set_content(name);
};

PureFW.ShopItem.prototype.set_item = function(type, id) {
	if (!this.item_pic_container) {
		var size = 0;
		if (this.width > this.height)
			size = this.height;
		else
			size = this.width;
				
		size -= 40
		this.item_pic_container = new PureFW.Container(
				this,
				(this.width-size)>>1, this.height-size - 15,
				size, size
		);
		this.item_pic_container.set_z_index(0);
	}
	this.item_id = id;
	this.item_type = type;
	this.item_pic_container.set_content(
			'<img src="'+this.pic_path+'/ui/icons/labels/items/'+
				this.item_pic_container.width+'/'+type+'/'+id+'.png" '+
				'style="width: 100%; height: 100%" />'
	);
};

PureFW.ShopItem.prototype.set_shop = function(shop_id) {
	if ((shop_id < 0) || (shop_id > PureFW.ShopItem.SHOP_GOLD)) {
		throw new Error ("PureFW.ShopItem.prototype.set_shop("+shop_id+"): "+
				'shop_id must be in [0,1,2,3]!');
	}
	this.item_shop = shop_id;
};

PureFW.ShopItem.prototype.set_discount = function(discount) {
	if (discount <= 0) {
		if (this.item_discount_img)
			this.item_discount_img.destroy();
	}
	else {
		if (!this.item_discount_img) {
			this.item_discount_img = new PureFW.Image(
				this,
				7,5,
				67,113,
				'ui/elements/texts/de/shop/discount/'+discount+'.png'
			);
			this.item_discount_img.set_z_index(2);
		}
		else {
			this.item_discount_img.set_pic_url(
				'ui/elements/texts/de/shop/discount/'+discount+'.png');
		}
	}
	this.item_discount = discount;
}

PureFW.ShopItem.prototype.set_amount = function(amount) {
	/**
	 * Das Item ist nicht (mehr) verfügbar, sondern ausverkauft.
	 */
	if (amount <= 0) {
		if (!this.item_sold_out_img) {
			// Wenn keine mehr Verfügbar, dann Ausverkauft-Label anzeigen.
			this.item_sold_out_img = new PureFW.Image(
				this,
				0, this.height-70,
				126, 64,
				'ui/elements/texts/de/shop/chests_only.png'
			);
			this.item_sold_out_img.set_z_index(1);
			if (this.on_click_functions.length > 0) {
				this.item_sold_out_img.add_event_handler("click",
					(function(_instance) {
						return function(ev) {
							_instance.on_click();
						}
					})(this)
				)
			}
		}
		if (this.item_amount_img)
			this.item_amount_img.destroy();
		this.set_cost('??');
	}
	/**
	 * Das Item ist verfügbar (amount)
	 */
	else {
		if (!this.item_amount_img) {
			if (amount > 1) {
				// Wenn nur einer, dann kein Label
				// Sonst neues Label erstellen
				this.item_amount_img = new PureFW.Image(
					this,
					9, this.height-50,
					105,50,
					'ui/elements/texts/de/shop/packs/'+amount+'.png'
				);
				this.item_amount_img.set_z_index(1);
			}
		}
		else {
			if (amount == 1) {
				// Wenn nur einer, dann kein Label
				this.item_amount_img.destroy();
			}
			else {
				// Sonst Label aktuallisieren
				this.item_amount_img.set_pic_url(
					'ui/elements/texts/de/shop/packs/'+amount+'.png');
			}
			
		}
	}
	this.item_amount = amount;
}

PureFW.ShopItem.prototype.set_cost = function(cost) {
	if (!this.item_price_container) {
		this.item_price_container = new PureFW.Container(
			this,
			this.width-84, this.height-34,
			72, 22
		);
		this.item_price_container.set_z_index(2);
		this.item_price_container.set_bg_img(
			'ui/backgrounds/shop/bg_preis.png'
		);
		this.item_price_container_inner = new PureFW.Container(
			this.item_price_container,
			36, 3,
			this.item_price_container.width-46,
			this.item_price_container.height-6
		);
		this.item_price_container_inner.set_text_align('right');
		this.item_price_container_inner.set_font_weight('bold');
		this.item_price_container_inner.set_font_size(1.15);
	}
	this.item_price = cost;
	this.item_price_container_inner.set_content(cost);
};

PureFW.ShopItem.prototype.add_event_handler = function(type, fn) {
	PureFW.ShopItem.parent.add_event_handler.call(this, type, fn);
	if (this.item_sold_out_img && type == "click") {
		this.item_sold_out_img.add_event_handler("click",
			(function(_instance) {
				return function(ev) {
					_instance.on_click();
				}
			})(this)
		)
	}
}

PureFW.ShopItem.prototype.destroy = function() {
	PureFW.ShopItem.parent.destroy.call(this);
	if (this.item_price_container)
		this.item_price_container.destroy();
	if (this.item_shop_symbol_container)
		this.item_shop_symbol_container.destroy();
	if (this.item_pic_container)
		this.item_pic_container.destroy();
	if (this.item_name_container)
		this.item_name_container.destroy();
	if (this.item_amount_img)
		this.item_amount_img.destroy();
	if (this.item_sold_out_img)
		this.item_sold_out_img.destroy();
	if (this.item_discount_img)
		this.item_discount_img.destroy();
};