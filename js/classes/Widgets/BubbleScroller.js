/**
 * A Container for items which can be cycled through. A BubbleBar on the bottom 
 * represents in which 'frame' the scroller currently is.
 * 													         	 
 * @author Alex
 */

/**
 * @param HTMLElement parent_node
 * @param int x
 * @param int y
 * @param uint w	(wenn <= 0, dann wird sie dynamisch bestimmt)
 * @param uint h	(wenn <= 0, dann wird sie dynamisch bestimmt)
 * @param bool register_widget		Ob das Widget bei manager_all registriert
 * 									werden soll (default: TRUE)
 */
PureFW.BubbleScroller = function(parent_node, x, y, w, h, items_per_page, 
	border_size, no_scale)
{ 
	if (typeof(parent_node) !== 'undefined')
	{
		this.init(parent_node, x, y, w, h, items_per_page, border_size,
			no_scale);
	
		this.instance_num = PureFW.BubbleScroller.num_instances;
		this.instance_name = "PureFW.BubbleScroller.instances["
														+this.instance_num+"]";
		this.id = "BubbleScroller"+this.instance_num;
		this.content_id = "BubbleScroller_contents"+this.instance_num;
		this.bg_img_id = 'BubbleScroller_bg'+this.instance_num;

		this.insert_into_dom_tree();
		PureFW.BubbleScroller.num_instances++;
		PureFW.BubbleScroller.instances[this.instance_num] = this;

		PureFW.WidgetManager.manager_all.register_widget(this);
	}
};
/********************************************************************
 * BubbleScroller Bar extends Container
 ********************************************************************/
PureFW.BubbleScroller.extend(PureFW.Container);

/********************************************************************
 * Statische Deklarationen
 ********************************************************************/
PureFW.BubbleScroller.num_instances = 0;
PureFW.BubbleScroller.instances = new Array();

/********************************************************************
 * Prototyp-Deklarationen
 ********************************************************************/
PureFW.BubbleScroller.prototype.init = function(parent_node, x, y, w, h,
	items_per_page, border_size, no_scale) 
{
	PureFW.BubbleScroller.parent.init.call(this, parent_node, x, y, w, h,
		no_scale);

	this.item_list = null;
	this.item_cache = null;
	this.frame_index = 0;
	this.frame_capacity = items_per_page || 2;
	this.border_size = (typeof(border_size) == 'undefined') ? 50 : border_size;
};

PureFW.BubbleScroller.prototype.insert_into_dom_tree = function()
{
	PureFW.BubbleScroller.parent.insert_into_dom_tree.call(this);
	
	/**
	 * @important: There is a significant difference between the cache and the
	 * list. The list contains data in the form of a constructor and arguments.
	 * Data in the list is simply in the form of an associated array. When the
	 * item is shown for the first time, it is placed in the cache as an actual
	 * Object, and instantiated there as well. There it is accessible at any 
	 * time as a normal Object, with all known functions. Once in the cache, the 
	 * item exists simultaneously in the cache and the list.
	 */
	this.item_list = new Array();
	this.item_cache = new Array();
	
	this.frame_index = 0;
	
	this.left_flick_container = new PureFW.Image(
		this,
		2, 
		(this.height - this.border_size - 44)>>1,
		46, 44);
	this.left_flick_container.set_pic_url(
			'ui/elements/bubblebar/flick_left_46x44.png');
	this.left_flick_container.add_event_handler('click',
			function(_instance)
			{
				return function(ev)
				{
					_instance.show_previous_set();
				}
			}(this));
	this.left_flick_container.set_z_index(2);
	
	this.right_flick_container = new PureFW.Image(
		this,
		this.width - this.border_size + 2, 
		(this.height - this.border_size - 44)>>1,
		46, 44);
	this.right_flick_container.set_pic_url(
			'ui/elements/bubblebar/flick_right_46x44.png');
	this.right_flick_container.add_event_handler('click',
		function(_instance)
		{
			return function(ev)
			{
				_instance.show_next_set();
			}
		}(this));
	this.right_flick_container.set_z_index(2);
	
	this.bubble_bar = this.add_widget(
		PureFW.BubbleBar,
		this.width/2, this.height - this.border_size + (this.border_size/2 - 7),
		0, 14);
	/**
	 * The inner container must be declared last because it takes on the fancy
	 * content_id and becomes the new go-to container when creating a widget
	 * using 'this' as parent.
	 */
	this.inner_container = this.add_widget(
		PureFW.Container,
		this.border_size, 0,
		this.width - 2*this.border_size,
		this.height - this.border_size);
	this.inner_container.set_id(this.content_id);
	
	this.left_flick_container.hide();
	this.right_flick_container.hide();
};

/**
 * Adds an item into the item LIST. Here a constructor is passed as an agrument
 * and an array of arguments.
 * 
 * The x and y values will be calculated and cannot be set! So the first 
 * arguments are width and height.
 * 
 * Example:
 * <code>
 * // Stores an Image with width and height 100 and PIC my_pic_url.png
 * bubble_scroller.add_item(
 * 		PureFW.Image,
 * 		[ 100, 100, 'my_pic_url.png' ]
 * );
 * 
 * // Stores a Container with width 300, height 20, adds an click event handler
 * // and sets the background color to red after creation
 * bubble_scroller.add_item(
 * 		PureFW.Container,
 * 		[ 300, 20 ],
 * 		function() {
 * 			this.add_event_handler("click", function(ev) { alert('clicked')});
 * 			this.set_bg_color('red');
 * 		}
 * );
 * </code>
 * 
 * @param Function constructor
 * @param mixed[] args
 * @param Function[] on_create_callback_fn
 */
PureFW.BubbleScroller.prototype.add_item = function(constructor, args, 
	on_create_callback_fn)
{
	this.item_list.push(
		{
			constructor: constructor, 
			args: args, 
			on_create_callback_fn: on_create_callback_fn
		}
	);
	if(this.item_list.length <= this.frame_index+this.frame_capacity)
	{
		this.show_item(this.item_list.length - 1);
	}
	var b = Math.ceil(this.item_list.length / this.frame_capacity);
	if(this.bubble_bar.get_bubble_amount() < b)
	{
		this.bubble_bar.set_width(
			this.bubble_bar.width + 
			this.bubble_bar.bubble_size + 
			this.bubble_bar.bubble_spacing
		);
		this.bubble_bar.set_x(this.width/2 - this.bubble_bar.width/2);
		
		var b = this.bubble_bar.add_bubble();
		b.add_event_handler("click",
			function(_index, _scroller) 
			{
				return function(ev)
				{
					// Be Careful of infinite Loops...
					while(_scroller.frame_index < _index)
					{
						_scroller.show_next_set();
					}
					while(_scroller.frame_index > _index)
					{
						_scroller.show_previous_set();
					}
				}
			}((this.bubble_bar.bubble_list.length-1)*this.frame_capacity, this)
		);
		
		if (this.bubble_bar.get_bubble_amount() > 1)
			this.right_flick_container.show();
	}
};

PureFW.BubbleScroller.prototype.remove_all_items = function() {
	this.item_list.destroy();
	this.item_list = new Array();
	this.item_cache.destroy();
	this.item_cache = new Array();
	this.bubble_bar.remove_all_bubbles();
	this.bubble_bar.set_width(0);
	this.frame_index = 0;
	
	for(var i = 0; i < this.frame_capacity; i++)
	{// Hide all items in the current frame
		this.hide_item(this.frame_index + i); 
	}
	this.right_flick_container.hide();
	this.left_flick_container.hide();
};

/**
 * Shows an item from the list of all items. In case the item is already is
 * already in the cache, it is shown. Otherwise, the item is instantiated here
 * and any applicable arguments are applied. The result is placed in the proper
 * position in the cache.
 */
PureFW.BubbleScroller.prototype.show_item = function(item_index)
{
	if(!this.item_cache[item_index])
	{
		var args = this.item_list[item_index].args;
		var col_w = this.inner_container.width / this.frame_capacity;
		var col_h = this.inner_container.height;
		var w = args[0] || col_w;
		var h = args[1] || col_h;
		var x = ((item_index % this.frame_capacity) * col_w);
					+ ((col_w - w) >> 1);
		var y = ((col_h - h) >> 1);
		
		this.item_cache[item_index] = 
			this.inner_container.add_widget
		(
			this.item_list[item_index].constructor,
			x, y,
			w, h,
			args[2], args[3], args[4], args[5], args[6], args[7], args[8], 
			args[9], args[10], args[11], args[12], args[13], args[14]
		);
		
		if(this.item_list[item_index].on_create_callback_fn)
		{
			this.item_list[item_index].on_create_callback_fn.call(
				this.item_cache[item_index]);
		}
		
	}
	else
	{
		this.item_cache[item_index].show();
	}
};

/**
 * Returns the next available position for an item to be placed. If this is 
 * called at the time when the Widget in question is added, the function MUST be
 * called and cannot be simply passed.
 */
PureFW.BubbleScroller.prototype.get_next_pos = function()
{
	var pos = this.item_list.length % this.frame_capacity;
	var x = pos*Math.round(this.inner_container.width / this.frame_capacity);
	var y = 0; // Will think of somehing nice here
	return new PureFW.Point(x, y);
}

/**
 * NOTE: Special fancy hiding effects can be inserted here.
 */
PureFW.BubbleScroller.prototype.hide_item = function(item_index)
{
	if(this.item_cache[item_index])
	{
		this.item_cache[item_index].hide();
	}
}

/**
 * Hides the items in the current frame and shows the items in the next one.
 */
PureFW.BubbleScroller.prototype.show_next_set = function()
{
	if((this.frame_index + this.frame_capacity) < this.item_list.length)
	{
		for(var i = 0; i < this.frame_capacity; i++)
		{// Hide all items in the current frame
			this.hide_item(this.frame_index + i); 
		}
		this.frame_index = this.frame_index + this.frame_capacity;
		this.left_flick_container.show();
		if((this.frame_index + this.frame_capacity) >= this.item_list.length)
		{
			this.right_flick_container.hide();
		}
		else
		{
			this.right_flick_container.show();
		}
		for(var i = 0; i < this.frame_capacity; i++)
		{
			try
			{
				this.show_item(this.frame_index + i);
			}
			catch(e) {} // May wanna do something here...
		}
	}
	this.bubble_bar.next();
}

/**
 * Hides the items in the current frame and shows the items in the previous one.
 */
PureFW.BubbleScroller.prototype.show_previous_set = function()
{
	if(this.frame_index > 0)
	{
		for(var i = 0; i < this.frame_capacity; i++)
		{// Hide all items in the current frame
			this.hide_item(this.frame_index + i); 
		}
		this.frame_index = this.frame_index - this.frame_capacity;
		this.right_flick_container.show();
		if(this.frame_index < 1)
		{
			this.left_flick_container.hide();
		}
		else
		{
			this.left_flick_container.show();
		}
		for(var i = 0; i < this.frame_capacity; i++)
		{
			try
			{
				this.show_item(this.frame_index + i);
			}
			catch(e) {} // May wanna do something here...
		}
	}
	this.bubble_bar.previous();
}

/**
 * Hides the Bubble Bar containing the bubbles indecating which page is 
 * currently shown.
 */
PureFW.BubbleScroller.prototype.hide_bubble_bar = function() {
	this.bubble_bar.hide();
}

/**
 * Hides the Bubble Bar containing the bubbles indecating which page is 
 * currently shown.
 */
PureFW.BubbleScroller.prototype.show_bubble_bar = function() {
	this.bubble_bar.show();
}

/**
 * Setzt die y-Position der Navipfeile. 
 * 
 * Initial sind sie zentriert auf die Höhe zentriert. Hier kann die vertikele
 * Position direkt gesetzt werden.
 * 
 * @param uint y
 */
PureFW.BubbleScroller.prototype.set_flick_y_pos = function(y) {
	this.left_flick_container.set_y(y);
	this.right_flick_container.set_y(y);
}

/**
 * Verschiebt den linken Pfeil-Button um die angegebene Anzahl Pixel in
 * x-Richtung relativ zur aktuellen Position 
 *  
 * @param uint x
 */
PureFW.BubbleScroller.prototype.set_lflick_x_offset = function(x) {
	this.left_flick_container.set_x(
		this.left_flick_container.position.x + x
	);
	
}

/**
 * Verschiebt den rechten Pfeil-Button um die angegebene Anzahl Pixel in
 * x-Richtung relativ zur aktuellen Position 
 *  
 * @param uint x
 */
PureFW.BubbleScroller.prototype.set_rflick_x_offset = function(x) {
	this.right_flick_container.set_x(
		this.right_flick_container.position.x + x
	);
}

/**
 * No Direct access to the below functions.
 */
PureFW.BubbleScroller.prototype.set_content = null;
 
PureFW.BubbleScroller.prototype.get_content = null;
