/**
 * Ein DropMenu, der viele Optionen (als Containers) enthalten kann.
 * 
 * @author Alex Arabadjiev
 */

/**
 * @param HTMLElement parent_node
 * @param int x
 * @param int y
 * @param uint w	(wenn <= 0, dann wird sie dynamisch bestimmt)
 * @param uint h	(wenn <= 0, dann wird sie dynamisch bestimmt)
 * @param uint l_w	(Breite des rechten Bildstücks)
 * @param uint r_w	(Breite des linken Bildstücks - und Pfeilbutton links)
 */
PureFW.DropMenu = function(parent_node, x, y, w, h, l_w, r_w, no_scale) 
{
	if (typeof(parent_node) !== 'undefined') 
	{
		this.init(parent_node, x, y, w, h, l_w, r_w, no_scale);
	
		this.instance_num = this.constructor.num_instances;
		this.instance_name = "PureFW.DropMenu.instances["+this.instance_num+"]";
		
		this.id = "DropMenu"+this.instance_num;
		this.content_id = "DropMenu_cont"+this.instance_num;
		this.bg_img_id = "DropMenu_bg"+this.instance_num;
		this.under_section_id = "DropMenuItems" + this.instance_num;
		
		this.insert_into_dom_tree();
		this.constructor.num_instances++;
		this.constructor.instances[this.instance_num] = this;
		
		PureFW.WidgetManager.manager_all.register_widget(this);
	}
};
/********************************************************************
 * DropMenu Bar extends Widget
 ********************************************************************/
PureFW.DropMenu.extend(PureFW.Container);

/********************************************************************
 * Statische Deklarationen
 ********************************************************************/
PureFW.DropMenu.num_instances = 0;
PureFW.DropMenu.instances = new Array();

/********************************************************************
 * Prototyp-Deklarationen
 ********************************************************************/
PureFW.DropMenu.prototype.init = function(parent_node, x, y, w, h, l_w, r_w, 
	no_scale) 
{
	PureFW.DropMenu.parent.init.call(this, parent_node, x, y, w, h, no_scale);
	
	this.current_item_container = null;
	this.under_section_container = null;
	this.under_section_items = new Array();
	
	this.under_background_top_left = null;
	this.under_background_top_middle = null;
	this.under_background_top_right = null;
	this.under_background_bottom_left = null;
	this.under_background_bottom_middle = null;
	this.under_background_bottom_right = null;
	
	this.left_part_container = null;
	this.middle_part_container = null;
	this.right_part_container = null;
	
	this.item_height = h - 10;
	this.max_visible_items = 5;
	
	this.left_width = l_w || 6;
	this.right_width = r_w || 16;
	
	this.menu_open = false;
};

/**
 * Fügt das Widget in den DOM-Baum ein
 * 
 * @see Widgets/Widget#insert_into_dom_tree
 */
PureFW.DropMenu.prototype.insert_into_dom_tree = function() 
{
	PureFW.DropMenu.parent.insert_into_dom_tree.call(this);
	
	this.node.style.overflow = 'visible';
	
	this.left_part_container = new PureFW.Container(
		this.get_node(),
		0, 0,
		this.left_width, this.height
	);
	this.left_part_container.set_bg_img(
		'ui/elements/form/drop_down/left_'
			+this.left_width+'x'+this.height+'.png'
	);
	this.left_part_container.get_node().style.cursor = 'pointer';
	this.left_part_container.add_event_handler("click",
		(function(_instance) {
			return function(ev) {
				_instance.toggle_drop_menu();
			}
		})(this)
	);
	this.left_part_container.disable_selection();
	this.left_part_container.parent_container = this;
	
	this.middle_part_container = new PureFW.Container(
		this.get_node(),
		this.left_part_container.width, 0,
		this.width - (this.left_width+this.right_width), 
		this.height
	);
	this.middle_part_container.set_bg_img(
		'ui/elements/form/drop_down/middle_DYNx'+this.height+'.png'
	);
	this.middle_part_container.get_node().style.cursor = 'pointer';
	this.middle_part_container.add_event_handler("click", 
		(function(_instance) {
			return function(ev) {
				_instance.toggle_drop_menu();
			}
		})(this)
	);
	this.middle_part_container.disable_selection();
	this.middle_part_container.parent_container = this;
	
	this.right_part_container = new PureFW.Container(
		this.get_node(),
		this.middle_part_container.width+this.left_part_container.width, 0,
		this.right_width, this.height);
	this.right_part_container.set_bg_img(
		'ui/elements/form/drop_down/right_'
			+this.right_width+'x'+this.height+'.png'
	);
	this.right_part_container.get_node().style.cursor = 'pointer';
	this.right_part_container.add_event_handler("click", 
		(function(_instance) {
			return function(ev) {
				_instance.toggle_drop_menu();
			}
		})(this)
	);
	this.right_part_container.disable_selection();
	this.right_part_container.parent_container = this;		
	
	this.current_item_container = new PureFW.Container(
			this.middle_part_container.get_node(),
			0, 6,
			this.middle_part_container.width, this.item_height);
	this.current_item_container.set_text_align('center');
	this.current_item_container.set_font_color('#366A9B');
	this.current_item_container.set_font_weight('bold');	
	this.current_item_container.parent_container = this.middle_part_container;
	
	this.under_section_container = new PureFW.ScrollContainer(
			this.get_node(),
			this.left_part_container.width, this.height, 
			this.width - 
				(this.left_part_container.width+
						this.right_part_container.width), 0);
	this.under_section_container.set_id(this.under_section_id);
	this.under_section_container.set_font_color('#366A9B');
	this.under_section_container.hide();
	this.under_section_container.set_z_index(40);
	this.under_section_container.parent_container = this;
	this.under_section_container.get_content_node().style.zIndex = 2;
	
	this.under_background_top_left = new PureFW.Image(
		this.under_section_container.get_node(),
		0, 0,
		5, 0,
		'ui/elements/form/drop_down/top_left.png');
	this.under_background_top_middle = new PureFW.Image(
		this.under_section_container.get_node(),
		5, 0,
		0, 0,
		'ui/elements/form/drop_down/top_middle.png');
	this.under_background_top_right = new PureFW.Image(
		this.under_section_container.get_node(),
		0, 0,
		5, 0,
		'ui/elements/form/drop_down/top_right.png');
	this.under_background_bottom_left = new PureFW.Image(
		this.under_section_container.get_node(),
		0, 0,
		5, 6,
		'ui/elements/form/drop_down/bottom_left.png');
	this.under_background_bottom_middle = new PureFW.Image(
		this.under_section_container.get_node(),
		5, 0,
		0, 6,
		'ui/elements/form/drop_down/bottom_middle.png');
	this.under_background_bottom_right = new PureFW.Image(
		this.under_section_container.get_node(),
		0, 0,
		5, 6,
		'ui/elements/form/drop_down/bottom_right.png');
	
	this.resize_backgrounds();
};

/**
 * Adds an item into the DropMenu. Can also specify a function to be called
 * when that menu item is selected.
 */

PureFW.DropMenu.prototype.add_item = function(name, fn)
{
	var place = this.under_section_items.length;
	var new_item = this.add_widget_as_item(
		PureFW.Container, [], fn
	);
	new_item.set_text_align('center');
	new_item.set_content(name);
	if(place == 0)
	{ //TODO: Another part of the workaround mentioned below
		this.current_item_container.set_content(
				this.under_section_items[place].get_content()
		);
	}
};

/**
 * Adds an item into the item LIST. Here a constructor is passed as an agrument
 * and an array of arguments.
 * 
 * The x and y positions are fix, and cannot be changed, as well as the width
 * and height. Those values are always 0,0 and 100%,100%.
 * 
 * Example:
 * <code>
 * // Stores an Image with PIC my_pic_url.png
 * bubble_scroller.add_item(
 * 		PureFW.Image,
 * 		[ 'my_pic_url.png' ]
 * );
 * 
 * // Stores a Container with width 300, height 20, adds an click event handler
 * // and sets the background color to red, when the item in the menu is 
 * // selected
 * bubble_scroller.add_item(
 * 		PureFW.Container,
 * 		[ ],
 * 		function() {
 * 			this.add_event_handler("click", function(ev) { alert('clicked')});
 * 			this.set_bg_color('red');
 * 		}
 * );
 * </code>
 * 
 * @param Function constructor
 * @param mixed[] args
 * @param Function on_click_callback_fn
 * @return PureFW.Widget
 */
PureFW.DropMenu.prototype.add_widget_as_item = function(constructor, args, 
	on_click_callback_fn)
{
	var place = this.under_section_items.length;
	
	this.under_section_items[place] = this.under_section_container.add_widget(
		constructor,
		0, this.item_height * place,
		this.under_section_container.width, this.item_height,
		args[0], args[1], args[2], args[3], args[4], args[5], args[6], args[7]
	);
	
	this.under_section_items[place].add_event_handler("click", 
			(function(z, instance)
			{
				return function(ev)
				{
					instance.select_item(z);
				}
			})(place, this)
	);
	
	if(this.under_section_items.length > this.max_visible_items)
	{
		this.under_section_container.set_height(this.item_height * 
				this.max_visible_items);
	}
	else
	{
		this.under_section_container.set_height(this.item_height * 
				this.under_section_items.length);
	}
	this.under_section_container.set_inner_height(this.item_height * 
			this.under_section_items.length);
	this.resize_backgrounds();
	
	if(on_click_callback_fn)
	{
		this.under_section_items[place].add_event_handler("click", 
			on_click_callback_fn);
	}
	
	return this.under_section_items[place];
}

PureFW.DropMenu.prototype.select_by_name = function(string)
{
	this.select_item(this.get_index_of(string));
};

PureFW.DropMenu.prototype.get_index_of = function(string)
{
	var n = this.under_section_items.length;
	for(var i = 0; i < n; i++)
	{
		var content = this.under_section_items[i].get_content();
		if(string == content)
		{
			return i;
		}
	}
};

PureFW.DropMenu.prototype.scroll_to_item = function(item)
{
	var item_index = this.get_index_of(item);
	this.under_section_container.scroll_to(0, item_index*this.item_height);
};

PureFW.DropMenu.prototype.resize_backgrounds = function()
{
	if((this.under_section_container.height < this.under_background_bottom_left.height))
	{
		return;
	}
	this.under_background_top_left.set_height((this.under_section_container.height - this.under_background_bottom_left.height));
	
	this.under_background_top_middle.set_height((this.under_section_container.height - this.under_background_bottom_left.height));
	this.under_background_top_middle.set_width(
			this.under_section_container.width - 
				(this.under_background_top_left.width +
				 this.under_background_top_right.width));
	
	this.under_background_top_right.set_height((this.under_section_container.height - this.under_background_bottom_left.height));
	this.under_background_top_right.set_x(
		this.under_background_top_middle.width + this.under_background_top_left.width);
	
	this.under_background_bottom_left.set_y(this.under_background_top_left.height);
	
	this.under_background_bottom_middle.set_y(this.under_background_top_left.height);
	this.under_background_bottom_middle.set_width(
			this.under_section_container.width - 
			(this.under_background_top_left.width +
			 this.under_background_top_right.width));
	
	this.under_background_bottom_right.set_y(this.under_background_top_left.height);
	this.under_background_bottom_right.set_x(
			this.under_background_top_middle.width + this.under_background_top_left.width);
};

/**
 * Sets the maximum number of items visible in the dropped section of the menu.
 * The rest of the items will be beneath and will have to be scrolled to.
 */
PureFW.DropMenu.prototype.set_maximum_visible_items = function(max)
{
	this.max_visible_items = max;
	
	if(this.max_visible_items > this.under_section_items.length)
	{
		this.under_section_container.set_height(this.item_height * 
				this.max_visible_items);
	}
	else
	{
		this.under_section_container.set_height(this.item_height * 
				this.under_section_items.length);
	}
};

/**
 * Togglet die Sichtbarkeit des Drop-Menüs: Wenn das Menü aktuell offen ist,
 * wird es geschlossen und umgekehrt.
 */
PureFW.DropMenu.prototype.toggle_drop_menu = function() {
	if (this.menu_open)
		this.close_drop_menu();
	else
		this.open_drop_menu();
}

/**
 * Öffnet das DropDown-Menü (klappt es aus).
 */
PureFW.DropMenu.prototype.open_drop_menu = function()
{
	this.under_section_container.show();
	this.scroll_to_item(this.current_item_container.get_content());
	this.menu_open = true;
};

/**
 * Schließt das DropDown-Menü (klappt es ein).
 */
PureFW.DropMenu.prototype.close_drop_menu = function()
{
	this.under_section_container.hide();
	this.under_section_container.set_inner_top(0);
	this.menu_open = false;
};

/**
 * TODO: @alex I will fix this, this is a cheap workaround used to avoid the 
 * problem caused by a mouse event being thrown around, starting from the 
 * closure at instance.item_select(...), and not being caught.
 */

PureFW.DropMenu.prototype.select_item = function(which)
{
	try
	{
		this.current_item_container.set_content(
				this.under_section_items[which].get_content()
		);
		this.close_drop_menu(); // this isn't working for some reason... SEE ABOVE!!!
	}
	catch(e)
	{
//		alert(e);
	}
};

/**
 * Setzt die Breite des DropMenus. Calls the set_width of Container
 * and then changes the width of the under section accordingly.
 * 
 * @param int w
 */
PureFW.DropMenu.prototype.set_width = function(w) 
{
	PureFW.Container.set_width.call(this, w);
	this.under_section_container.set_width(w - 25);
	this.middle_part_container.set_width(w - 39);
	this.right_part_container.set_x(this.middle_part_container.width + 26);
	var n = this.under_section_items.length;
	for(var i = 0; i < n; i++)
	{
		this.under_section_items[i].set_width(w - 25);
	}
};


/**
 * Setzt die Höhe des DropMenus. Calls the set_height of Container
 * and of all the lower sections.
 * 
 * @param int h
 */
PureFW.DropMenu.prototype.set_height = function(h) 
{
	PureFW.Container.set_height.call(this, h);
	item_height = h - 10;
	this.under_section_container.set_height(this.item_height);
	var n = this.under_section_items.length;
	for(var i = 0; i < n; i++)
	{
		this.under_section_items[i].set_height(this.item_height);
	}
};

PureFW.DropMenu.prototype.set_item_padding = function(padding)
{
	this.current_item_container.set_padding_left(padding);
	this.current_item_container.set_padding_right(padding);
	var n = this.under_section_items.length;
	for(var i = 0; i < n; i++)
	{
		this.under_section_items[i].set_padding_left(padding);
		this.under_section_items[i].set_padding_right(padding);
	}	
};

/**
 * Setzt die Ausrichtung des Textes.
 * 
 * @param {'left', 'right', 'center', 'justify'} 	align 
 */
PureFW.DropMenu.prototype.set_text_align = function(align) {
	this.current_item_container.set_text_align(align);
	var n = this.under_section_items.length;
	for(var i = 0; i < n; i++)
	{
		this.under_section_items[i].set_text_align(align);
	}
}