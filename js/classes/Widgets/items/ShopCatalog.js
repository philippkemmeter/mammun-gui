/**
 * Dieses Widget ist ein besonderes Popup-Fenster, dass im Shop aufgeht und
 * die aktuellen Angebote anzeigt. Man kann sie dann durchblättern usw.
 */



if (typeof(PureFW.Items) != 'object')
	PureFW.Items = new Object();

/**
 * @param Node parent_node	An welchen Knoten die Map angehängt werden soll
 * @param int x				Wo sich die Map befinden soll in x-Richtung in px
 * @param int y				Wo sich die Map befinden soll in y-Richtung in px
 * @param int w				Wie breit die gesamte Map sein soll
 * @param int h				Wie hoch die gesamte Map sein soll
 * @param bool condensed	Ob eine komprimierte Version angezeigt werden soll
 * @param bool no_scale		Nicht sklierbar (default: false)
 */
PureFW.Items.ShopCatalog = function(parent_node, x, y, w, h, grey, no_scale)
{
	if (typeof(parent_node) !== 'undefined') {
		this.init(parent_node, x, y, w, h, grey, no_scale);
		
		this.instance_num = this.constructor.num_instances;
		this.instance_name = "MammunUI.get_top_frame().PureFW.Items" +
				".ShopCatalog.instances["	+this.instance_num+"]";
		
		this.id = "PureFW.Items.ShopCatalog"+this.instance_num;
		this.content_id 
			= "PureFW.Items.ShopCatalog_cont"+this.instance_num;
		this.bg_img_id = 'PureFW.Items.ShopCatalog_bg'+this.instance_num;
		this.grey_layer_node_id ="PureFW.Items.ShopCatalog_grey_layer"
			+this.instance_num;
		
		this.insert_into_dom_tree();
		this.constructor.num_instances++;
		this.constructor.instances[this.instance_num] = this;
		
		if (!this.no_scale)
			PureFW.WidgetManager.manager_all.register_widget(this);
	}
};
/********************************************************************
 * PureFW.Items.ShopCatalog extends Container
 ********************************************************************/
PureFW.Items.ShopCatalog.extend(PureFW.ConfirmationBox);

/********************************************************************
 * Statische Deklarationen
 ********************************************************************/
PureFW.Items.ShopCatalog.num_instances = 0;		// Instanzüberwachung (anzahl)
PureFW.Items.ShopCatalog.instances = new Array();	// Instanzüberwachung (Instanzen)
PureFW.Items.ShopCatalog.LNG_TOOLTIP_CATALOG_TIME = '';
PureFW.Items.ShopCatalog.LNG_SOLD_OUT = '';
PureFW.Items.ShopCatalog.LNG = '';


/********************************************************************
 * Prototyp-Deklarationen
 ********************************************************************/
PureFW.Items.ShopCatalog.prototype.init = function(parent_node, x, y, w, h,
	grey, no_scale)
{
	PureFW.Items.ShopCatalog.parent.init.call(this, parent_node, x, y, w, h,
		PureFW.ConfirmationBox.NO_BUTTONS, grey, no_scale);

	this.items = new Array();
	this.current_page_index = 0;
	
	this.bubble_scroller = null;
	this.on_buy_functions = new Array();
	
	this.time_conts = new Array();
	this.time_update_timeout_handler = 0;
};

PureFW.Items.ShopCatalog.prototype.insert_into_dom_tree = function() {
	PureFW.Items.ShopCatalog.parent.insert_into_dom_tree.call(this);
	
	this.set_bg_img('ui/backgrounds/shop/catalog/catalog_bg.png');
	
	var close_btn = this.add_widget(
		PureFW.Image,
		this.width - 32, 0,
		32, 32,
		'ui/elements/window/close_window.png'
	);
	close_btn.add_event_handler("click",
		(function (_instance){ 
			return function(ev){
					_instance.destroy();
				} 
		})(this)
	);
	
	this.bubble_scroller = this.add_widget(
		PureFW.BubbleScroller,
		28, 17,
		907, 566,
		1, 0
	);
	this.bubble_scroller.hide_bubble_bar();
	this.bubble_scroller.set_flick_y_pos(this.height - 100)
	this.bubble_scroller.set_rflick_x_offset(-55);
	this.bubble_scroller.set_lflick_x_offset(20);
	this.time_update_timeout_handler =
		PureFW.Timeout.set_interval(
			(function(_instance) {
				return function() {
					_instance.update_time_conts();
				}
			})(this), 
			1000
		);
}

/**
 * Fügt ein Item in den Katalog ein.
 * 
 * @param Object item
 */
PureFW.Items.ShopCatalog.prototype.add_item = function(item) {
	this.items.push(item);
	this.bubble_scroller.add_item(
		PureFW.Container,
		[907, 566],
		(function (_item, _instance) {
			return function () {
				var merry_christmas = ((_item.id == 3001) || (_item.id == 3201) || 
					(_item.id == 3301) || (_item.id == 3401) ||
					(_item.id == 3601));
				var creepy_christmas = ((_item.id == 2901) || 
					(_item.id == 3101) || (_item.id == 3501));
				var halloween = ((_item.id == 2601) || (_item.id == 2701) ||
					(_item.id == 2801));
				
				this.set_bg_img(
					'ui/backgrounds/shop/catalog/items/'
						+_item.type+'/'+_item.id+'.jpg'
				);
				
				var y_offset = (merry_christmas || creepy_christmas) ? 77 : 0;
				
				var title_cont = this.add_widget(
					PureFW.Container,
					506, 26 + y_offset,
					385, 85
				);
				
				title_cont.set_css_class("ShopCatalog_item_title");
				title_cont.set_content(_item.name);
				
				var desc_cont = this.add_widget(
					PureFW.ScrollContainer,
					title_cont.position.x, 
					title_cont.position.y + title_cont.height + 20,
					title_cont.width,
					0
				);
				/**
				 * Ausnahme für Items mit dunklem Hintergrund müssen manuell
				 * hier eingefügt werden.
				 */
				if (creepy_christmas || halloween)
					desc_cont.set_css_class("ShopCatalog_item_desc_bright");
				else
					desc_cont.set_css_class("ShopCatalog_item_desc");
				desc_cont.set_content(_item.desc);
				desc_cont.update_inner_height();
				desc_cont.set_height(220 - y_offset);
				
				var time_img = this.add_widget(
					PureFW.Image,
					desc_cont.position.x + 5,
					desc_cont.position.y + desc_cont.height+54,
					44, 44,
					'ui/icons/labels/resources/44/wecker.png'
				);
				
				var time_cont = this.add_widget(
					PureFW.Container,
					time_img.position.x + time_img.width+15,
					time_img.position.y + 13,
					300, 24
				);
				time_cont.set_css_class('ShopCatalog_time_cont');
				if (_item.av_until > 0) {
					time_img.set_tooltip(
						PureFW.Items.ShopCatalog.LNG_TOOLTIP_CATALOG_TIME
					)
					time_cont.set_tooltip(
						PureFW.Items.ShopCatalog.LNG_TOOLTIP_CATALOG_TIME
					)
					time_cont.set_content(
						"-"+PureFW.Time.sec_in_time(
							_item.av_until -
							PureFW.Time.get_current_server_time()
						)
					);
				}
				else {
					time_img.hide();
					time_cont.hide();
				}
				
				var buy_button = this.add_widget(
					PureFW.Image,
					desc_cont.position.x,
					time_img.position.y + time_img.height + 10,
					201, 77,
					'ui/elements/form/buttons/'	+ PureFW.Items.ShopCatalog.LNG
						+ '/shop/btn_kaufen_201x77.png'
				);
				
				buy_button.add_event_handler("click",
					function (ev) {
						_instance.on_buy(_instance.create_event("buy", _item));
					}
				);
				
				/**
				 * Das wird in update_time_conts benutzt, um die Zeit in RT 
				 * runterzuzählen.
				 */
				_instance.time_conts.push(
					{
						cont: time_cont,
						t_stamp: _item.av_until
					}
				);
			}
		})(item, this)
	);
};


PureFW.Items.ShopCatalog.prototype.update_time_conts = function() {
	var n = this.time_conts.length;
	for (var i = 0; i < n; i++) {
		var cont = this.time_conts[i].cont;
		var av_until = this.time_conts[i].t_stamp;
		var t_now = PureFW.Time.get_current_server_time();
		var count_down = av_until - t_now;
		if (count_down > 0)
			cont.set_content("-"+PureFW.Time.sec_in_time(av_until - t_now));
		else {
			cont.set_content(PureFW.Items.ShopCatalog.LNG_SOLD_OUT);
		}
	}
}

/**
 * Fügt einen EventHandler hinzu. Funktion überschrieben, da zusätzlich
 * der Typ "buy" unterstützt wird, welcher beim Klicken auf den Kaufenknopf
 * angeklickt wird.
 * 
 * @param string type
 * @param Function fn
 * @see Widget.add_event_handler
 */
PureFW.Items.ShopCatalog.prototype.add_event_handler = function(type, fn) {
	if (type === "buy")
		this.on_buy_functions.push(fn);
	else
		PureFW.Items.ShopCatalog.parent.add_event_handler.call(this, type, fn);
};

/**
 * Entfernt einen EventHandler. Funktion überschrieben, da zusätzlich
 * der Typ "buy" unterstützt wird, welcher beim Klicken auf den Kaufenknopf
 * angeklickt wird.
 * 
 * @param string type
 * @param Function fn
 * @see Items.ShopCatalog.prototype.remove_event_handler
 */
PureFW.Items.ShopCatalog.prototype.remove_event_handler = function(type, fn) {
	if (type === "buy")
		this.on_buy_functions.remove(fn);
	else
		PureFW.Items.ShopCatalog.parent.remove_event_handler.call(this, type, fn);
};

/**
 * Ruft das "on-buy"-Event exlizit auf. Wird normalerweise aufgerufen,
 * wenn der Kaufenknopf angeklickt wird.
 * 
 * @param Event ev [optional]
 */
PureFW.Items.ShopCatalog.prototype.on_buy = function(ev) {
	if (!ev) {
		ev = this.create_event("buy");
	}
	var n = this.on_buy_functions.length;
	for (var i = 0; i < n; i++) {
		this.on_buy_functions[i].call(this, ev);
	}
};


PureFW.Items.ShopCatalog.prototype.destroy = function() {
	PureFW.Items.ShopCatalog.parent.destroy.call(this);
	PureFW.Timeout.clear_interval(this.time_update_timeout_handler);
}