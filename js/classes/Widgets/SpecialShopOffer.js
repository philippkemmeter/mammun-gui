/**
 * This widget is used exclusively in the Shop, when special offers need to be
 * shown. It extends the standard PolaroidContanier
 */

PureFW.SpecialShopOffer = function(parent_node, x, y, w, h, no_scale)
{
	if (typeof(parent_node) !== 'undefined') {
		this.init(parent_node, x, y, w, h, no_scale);
		
		this.instance_num = this.constructor.num_instances;
		this.instance_name = "PureFW.SpecialShopOffer" +
				".instances["	+this.instance_num+"]";
		
		this.id = "PureFW.SpecialShopOffer"+this.instance_num;
		this.content_id 
			= "PureFW.SpecialShopOffer_cont"+this.instance_num;
		this.bg_img_id = 'PureFW.SpecialShopOffer_bg'
			+this.instance_num;
		
		this.insert_into_dom_tree();
		this.constructor.num_instances++;
		this.constructor.instances[this.instance_num] = this;
		
		if (!this.no_scale)
			PureFW.WidgetManager.manager_all.register_widget(this);
	}
};

/********************************************************************
 * PureFW.SpecialShopOffer extends PolaroidContainer
 ********************************************************************/
PureFW.SpecialShopOffer.extend(PureFW.Container);

/********************************************************************
 * Statische Deklarationen
 ********************************************************************/
PureFW.SpecialShopOffer.num_instances = 0;		// Instanzüberwachung (anzahl)
PureFW.SpecialShopOffer.instances = new Array();// Instanzüberwachung (Instanzen)

/********************************************************************
 * Prototyp-Deklarationen
 ********************************************************************/
PureFW.SpecialShopOffer.prototype.init = function(parent_node, x, y, w, h, no_scale)
{
	PureFW.SpecialShopOffer.parent.init.call(this, parent_node, x, y, (w || 198), (h || 338), no_scale);
	
	this.title_container = null;
	this.drop_shadow = null;
	this.price_container = null;
	this.pretzel_container = null;
	this.item_desc_containter = null;
	this.avatar_container = null;
	this.nick_container = null;
	this.buy_link = null;
	// Here down...
	this.picture = null;
	this.widget_label = null;
};

PureFW.SpecialShopOffer.prototype.insert_into_dom_tree = function() 
{
	PureFW.SpecialShopOffer.parent.insert_into_dom_tree.call(this);
	

	// HERE DOWN
	this.set_bg_img(
		'ui/backgrounds/PolaroidContainer/paper_'+this.width+'x'+this.height
			+ '.png'
	);
	
	this.picture = this.add_widget(
		PureFW.Image,
		(this.width - 150) >> 1, (this.width - 150) >> 1,
		150, 150,
		'pattern/spacer.gif',
		this.no_scale
	);
	
	this.widget_label = this.add_widget(
		PureFW.Container,
		10, this.picture.height + this.picture.position.y + 10,
		this.width - 20,
		this.height - this.picture.position.y - this.picture.height - 20,
		this.no_scale
	);
	this.widget_label.set_css_class('PolaroidContainer_widget_label');
	// HERE UP
	
	// Don't ask the reason for all this math... I don't know...
	this.widget_label.set_height(this.widget_label.height - 38);
	
	this.title_container = this.add_widget(PureFW.Container,
					0, 10,
					this.width, 40);
			this.title_container.set_text_align('center');
			this.title_container.set_font_size(1.2);
			this.title_container.set_font_weight('bold');
			this.title_container.set_text_align('center');
			this.title_container.set_font_color('#303030');
			this.picture.set_y(this.title_container.position.y 
					+ this.title_container.height + 10);
			this.widget_label.set_y(this.picture.height 
					+ this.picture.position.y + 10);
	
	this.price_container = this.add_widget(PureFW.Container,
					this.picture.position.x - 20,
					this.picture.position.y + this.picture.height - 49,
					110, 49);
			this.price_container.set_bg_img(
					'ui/icons/labels/shop/klunker_price_110x49.png');
			this.price_container.set_font_size(1.2);
			this.price_container.set_font_weight('bold');
			this.price_container.set_text_align('center');
			this.price_container.set_font_color('#303030');
			this.price_container.get_content_node().style.margin = "14px";
	
	this.pretzel_container = this.add_widget(PureFW.Container,
					this.picture.position.x + this.picture.width - 86 + 15,
					this.price_container.position.y,
					86, 47);
			this.pretzel_container.set_bg_img(
					'ui/icons/labels/shop/pretzel_86x47.png');
			this.pretzel_container.set_font_size(1.2);
			this.pretzel_container.set_font_weight('bold');
			this.pretzel_container.set_text_align('center');
			this.pretzel_container.set_font_color('#303030');
			this.pretzel_container.get_content_node().style.margin = "14px";
			this.pretzel_container.hide();
	
	this.item_desc_container = this.add_widget_to_label(PureFW.Container,
					0, 0,
					this.widget_label.width, 0);
				
	// The avatar container is just a placeholder at this point.
	this.avatar_container = 
				{
					position: {x:0, y: this.widget_label.height - 48},
					width:48, height: 48
				};
			
	this.nick_container = this.add_widget_to_label(PureFW.Container,
					this.avatar_container.position.x + this.avatar_container.width + 5,
					this.avatar_container.position.y,
					this.widget_label.width - this.avatar_container.width - 10,
					0);
			
	this.buy_link = this.add_widget_to_label(PureFW.Container,
					this.nick_container.position.x,
					this.nick_container.position.y + 25,
					this.widget_label.width - this.avatar_container.width - 10,
					0);
			this.buy_link.set_font_weight('bold');
			this.buy_link.set_content('LINK_ME!');
};

PureFW.SpecialShopOffer.prototype.set_user = function(pic, nick)
{
	this.avatar_mode = true;
	if(this.avatar_container.destroy)
		this.avatar_container.destroy();
	
	this.avatar_container = this.add_widget_to_label(PureFW.Avatar.Avatar,
		0, this.widget_label.height - 48,
		48, 48,
		pic,
		nick);
	this.nick_container.set_font_weight('normal');
	this.nick_container.set_font_color('black');
	this.nick_container.set_content(/*MammunUI.beautify_nick(*/nick/*)*/);
};

PureFW.SpecialShopOffer.prototype.set_timeout = function(t)
{
	if(this.avatar_mode)
	{
		if(this.avatar_container.destroy)
			this.avatar_container.destroy();
		this.avatar_container = this.add_widget_to_label(PureFW.Container,
			0, this.widget_label.height - 44,
			44, 44);
		this.avatar_container.set_bg_img("ui/icons/labels/resources/44/wecker.png");
		this.nick_container.set_font_weight('bold');
		this.nick_container.set_font_color('red');
	}
	this.nick_container.set_content(t);
};

PureFW.SpecialShopOffer.prototype.set_desc = function(text)
{
	this.item_desc_container.set_content(text);	
};

PureFW.SpecialShopOffer.prototype.set_name = function(str)
{
	this.title_container.set_content(str);
};

PureFW.SpecialShopOffer.prototype.set_price = function(kkr)
{
	if(kkr < 0)
	{
		// Set the picture just in case...
		this.price_container.set_bg_img(
				'ui/icons/labels/shop/offer_only_147x50.png');
		this.price_container.set_width(147);
		this.price_container.set_height(50);
		this.price_container.set_content("");
		this.pretzel_container.hide();
	}
	else
	{
		this.price_container.set_bg_img(
				'ui/icons/labels/shop/klunker_price_110x49.png');
		this.price_container.set_width(110);
		this.price_container.set_height(49);
		this.price_container.set_content(kkr);
	}
};

PureFW.SpecialShopOffer.prototype.set_pretzels = function(b)
{
	this.pretzel_container.set_content(b);
	this.pretzel_container.show();
};

PureFW.SpecialShopOffer.prototype.set_buy_link = function(link)
{
	this.buy_link.set_content(link);
};

/**
 * Modified version of the function in ShopItem.js 
 */
PureFW.SpecialShopOffer.prototype.set_item = function(type, id) 
{
	var item_id = id;
	var item_type = type;
	this.set_pic_url('/ui/icons/labels/items/'
			+/*this.picture.width+*/143+'/'+type+'/'+id+'.png');
};

//HERE DOWN -> Inheritence Methods

/**
 * Setzt die URL für das Bild, das angezeigt werden soll
 */
PureFW.SpecialShopOffer.prototype.set_pic_url = function(pic) {
	this.picture.set_pic_url(pic);
};

/**
 * Setzt den beschreibenden Text, der unter dem Bild so eingeblendet wird,
 * dass es als die Beschreibung des gesamten Widgets wahrgenommen wird.
 * 
 * @param String text
 */
PureFW.SpecialShopOffer.prototype.add_label_content = function(text) {
	this.widget_label.add_content(text);
};

/**
 * Fügt ein neues durch den übergebenen Konstruktor definiertes Widget dem 
 * Label hinzu. Die Argumente werden entsprechend dem Widget weitergereicht.
 * 
 * Gibt das neue Widget zurück.
 * 
 * @param Constructor widget_constructor
 * @param uint x
 * @param uint y
 * @param uint w
 * @param uint h
 * @param mixed argN
 * @return PureFW.Widget
 * 
 * @see PureFW.Container#add_widget
 */
PureFW.SpecialShopOffer.prototype.add_widget_to_label = function(
	widget_constructor, x, y, w, h, arg6, arg7, arg8, arg9, arg10, arg11, 
	arg12, arg13)
{
	return this.widget_label.add_widget(widget_constructor, 
		x, y, w, h, arg6, arg7, arg8, arg9, arg10, arg11, arg12, arg13);
};