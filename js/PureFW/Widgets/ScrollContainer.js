/**
 * Ein ScrollContainer, der HTML oder Widgets enthalten und durchscrollen kann.
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
 * 
 * FYI: The final width and height of the container need to be defined right
 * 		at initialization or else the scrollbars on the right and bottom won't
 * 		appear... This is a known bug and will be fixed eventually.
 */
PureFW.ScrollContainer = function(parent_node, x, y, w, h) { 
	if (typeof(parent_node) !== 'undefined')
	{
		this.init(parent_node, x, y, w, h);
	
		this.instance_num = PureFW.ScrollContainer.num_instances;
		this.instance_name = "PureFW.ScrollContainer.instances["
														+this.instance_num+"]";
		this.id = "ScrollContainer"+this.instance_num;
		this.bg_img_id = "ScrollContainer_bg_img"+this.instance_num;
		this.inner_container_id = "ScrollContent"+this.instance_num;
		this.vertical_scroller_id = "VertiScroller"+this.instance_num;
		this.horizontal_scroller_id = "HorizonScroller"+this.instance_num;

		this.insert_into_dom_tree();
		PureFW.ScrollContainer.num_instances++;
		PureFW.ScrollContainer.instances[this.instance_num] = this;

		PureFW.WidgetManager.manager_all.register_widget(this);
	}
};
/********************************************************************
 * ScrollContainer Bar extends Container
 ********************************************************************/
PureFW.ScrollContainer.extend(PureFW.Container);

/********************************************************************
 * Statische Deklarationen
 ********************************************************************/
PureFW.ScrollContainer.num_instances = 0;
PureFW.ScrollContainer.instances = new Array();
PureFW.ScrollContainer.active_scrolling_instance = null;
PureFW.ScrollContainer.active_dragging_instance = null;

PureFW.ScrollContainer.while_vertical_dragging = function(ev) 
{
	PureFW.ScrollContainer.active_dragging_instance._calculate_vertical_value(
		ev.detail);
};

PureFW.ScrollContainer.while_horizontal_dragging = function(ev) 
{
	PureFW.ScrollContainer.active_dragging_instance._calculate_horizontal_value(
		ev.detail);
};

PureFW.ScrollContainer.vertical_scroller_dragged = function(ev)
{
	PureFW.ScrollContainer.active_dragging_instance._start_vertical_dragging();
};

PureFW.ScrollContainer.vertical_scroller_dropped = function(ev)
{
	PureFW.ScrollContainer.active_dragging_instance._stop_vertical_dragging();
};

PureFW.ScrollContainer.horizontal_scroller_dragged = function(ev)
{
	PureFW.ScrollContainer.active_dragging_instance._start_horizontal_dragging();
};

PureFW.ScrollContainer.horizontal_scroller_dropped = function(ev)
{
	PureFW.ScrollContainer.active_dragging_instance._stop_horizontal_dragging();
};

PureFW.ScrollContainer.while_scrolling = function(delta)
{
	PureFW.ScrollContainer.active_scrolling_instance.scroll(delta);
};

PureFW.ScrollContainer.up_arrow_clicked = function()
{
	PureFW.ScrollContainer.active_scrolling_instance.scroll_up();
};

PureFW.ScrollContainer.down_arrow_clicked = function()
{
	PureFW.ScrollContainer.active_scrolling_instance.scroll_down();
};

PureFW.ScrollContainer.left_arrow_clicked = function()
{
	PureFW.ScrollContainer.active_scrolling_instance.scroll_left();
};

PureFW.ScrollContainer.right_arrow_clicked = function()
{
	PureFW.ScrollContainer.active_scrolling_instance.scroll_right();
};

/********************************************************************
 * Prototyp-Deklarationen
 ********************************************************************/
PureFW.ScrollContainer.prototype.init = function(parent_node, x, y, w, h) {
	PureFW.ScrollContainer.parent.init.call(this, parent_node, x, y, w, h);
	
	this.inner_container = null;
	this.vertical_scrolling_enabled = false;
	this.horizontal_scrolling_enabled = false;
	this.hiding_allowed = null;
	this.button_size = 0;
	this.scroller_color = null;
	
	this.up_arrow_container = null;
	this.down_arrow_container = null;
	this.side_scroller_container = null;
	this.side_scroller = null;
	
	this.left_arrow_container = null;
	this.right_arrow_container = null;
	this.bottom_scroller_container = null;
	this.bottom_scroller = null;
	
	this.internal_scroll_top = null;
	this.internal_scroll_middle_vertical = null;
	this.internal_scroll_bottom = null;
	
	this.internal_scroll_left = null;
	this.internal_scroll_middle_horizontal = null;
	this.internal_scroll_right = null;
	
	this.vertical_scroller_size = 0;
	this.vertical_scroller_position = 0;
	this.vertical_scroller_proportion = 0;
	this.vertical_scroll_interval = .2;
	
	this.horizontal_scroller_size = 0;
	this.horizontal_scroller_position = 0;
	this.horizontal_scroller_proportion = 0;
	this.horizontal_scroll_interval = .2;

	this.max_vertical_scroll = 0;
	this.max_horizontal_scroll = 0;
	this.on_scroll = null;
	this.slide_scroll_interval = null;
	
	this.inner_container_id = null;
	this.vertical_scroller_id = null;
	this.horizontal_scroller_id = null;
};

PureFW.ScrollContainer.prototype.insert_into_dom_tree = function()
{
	this.button_size = 16;
	this.scroller_color = 'default';

	PureFW.ScrollContainer.parent.insert_into_dom_tree.call(this);

	this.vertical_scrolling_enabled = true;
	this.horizontal_scrolling_enabled = true;
	this.hiding_allowed = true;
	
	this.node.style.overflow = 'hidden';

	this.set_id(this.id);
	
	this.inner_container = new PureFW.Container(
			this.node,
			0, 0,
			this.width, this.height);
	this.inner_container.set_id(this.inner_container_id);
	this.inner_container.parent_container = this;
	
	this.up_arrow_container = new PureFW.Image(
			this.node, 
			0, 0, 
			this.button_size, this.button_size,
		'ui/elements/scrollbars/'+ this.scroller_color +'/scrollarrowtop.png');
	this.up_arrow_container.set_positioning('absolute');
	this.up_arrow_container.set_x(this.width - this.button_size);
	this.up_arrow_container.set_z_index(10);
	this.up_arrow_container.add_event_handler(
			"click", PureFW.ScrollContainer.up_arrow_clicked);
	this.up_arrow_container.add_event_handler("mousedown",
		(function(_instance) {
			return function(ev) {
				_instance.slide_scroll_interval = PureFW.Timeout.set_interval(
					function() {
						_instance.scroll_up()
					}, 100);
			}
		})(this)
	);
	this.up_arrow_container.add_event_handler("mouseup",
		(function(_instance) {
			return function(ev) {
				PureFW.Timeout.clear_interval(_instance.slide_scroll_interval);
			}
		})(this)
	);
	this.up_arrow_container.add_event_handler("mouseout",
		(function(_instance) {
			return function(ev) {
				PureFW.Timeout.clear_interval(_instance.slide_scroll_interval);
			}
		})(this)
	);
	this.up_arrow_container.parent_container = this;
	
	this.down_arrow_container = new PureFW.Image(
			this.node, 
			0, 0, 
			this.button_size, this.button_size,
		'ui/elements/scrollbars/'+ this.scroller_color +'/scrollarrowbottom.png');
	this.down_arrow_container.set_positioning('absolute');
	this.down_arrow_container.set_x(this.width - this.button_size);
	this.down_arrow_container.set_y(this.height - this.button_size);
	this.down_arrow_container.set_z_index(10);
	this.down_arrow_container.add_event_handler(
			"click", PureFW.ScrollContainer.down_arrow_clicked);
	this.down_arrow_container.add_event_handler("mousedown",
		(function(_instance) {
			return function(ev) {
				_instance.slide_scroll_interval = PureFW.Timeout.set_interval(
					function() {
						_instance.scroll_down();
					}, 100
				);
			}
		})(this)
	);
	this.down_arrow_container.add_event_handler("mouseup",
		(function(_instance) {
			return function(ev) {
				PureFW.Timeout.clear_interval(_instance.slide_scroll_interval);
			}
		})(this)
	);
	this.down_arrow_container.add_event_handler("mouseout",
		(function(_instance) {
			return function(ev) {
				PureFW.Timeout.clear_interval(_instance.slide_scroll_interval);
			}
		})(this)
	);
	this.down_arrow_container.parent_container = this;

	this.side_scroller_container = new PureFW.Container(
			this.node, 
			0, 0, 
			this.button_size, this.height - 2*this.button_size); // when height is 0, this becomes -32
	this.side_scroller_container.set_positioning('absolute');
	this.side_scroller_container.set_x(this.width - this.button_size);
	this.side_scroller_container.set_y(this.button_size);
	this.side_scroller_container.parent_container = this;

	if(this.inner_container.height > 0)
	{
		this.vertical_scroller_size = 
			(this.side_scroller_container.height) * 
			(this.side_scroller_container.height / this.inner_container.height);
	}
	this.vertical_scroller_proportion = 1;
	
	this.side_scroller = new PureFW.Container(
			this.side_scroller_container.get_content_node(),
			0, 0, 
			this.button_size, this.vertical_scroller_size);
	
	this.side_scroller.set_z_index(9);
	this.side_scroller.set_id(this.vertical_scroller_id);
	this.side_scroller.add_event_handler(
		"mousedown",
		(function(_instance) {
			return function(ev) {
				_instance._start_vertical_dragging();
			}
		})(this)
	);
	this.side_scroller.add_event_handler(
		"mouseup",
		(function(_instance) {
			return function(ev) {
				_instance._stop_vertical_dragging();
			}
		})(this)
	);
	this.side_scroller_container.parent_container = this.side_scroller_container;
	
		this.internal_scroll_top = new PureFW.Container(
				this.side_scroller , 0, 0, this.button_size, 10);
		this.internal_scroll_top.set_bg_img(
				'ui/elements/scrollbars/'+ this.scroller_color +'/scrollbar_top.png');
		this.internal_scroll_top.parent_container = this.side_scroller;
	
		this.internal_scroll_middle_vertical = new PureFW.Container(
				this.side_scroller , 0, 0, this.button_size, 1);
		this.internal_scroll_middle_vertical.set_y(10);
		this.internal_scroll_middle_vertical.set_height(this.vertical_scroller_size - 20);
		this.internal_scroll_middle_vertical.set_bg_img(
				'ui/elements/scrollbars/'+ this.scroller_color +'/scrollbar_middle_vertical.png');
		this.internal_scroll_middle_vertical.parent_container = this.side_scroller;
	
		this.internal_scroll_bottom = new PureFW.Container(
				this.side_scroller , 0, 0, this.button_size, 10);
		this.internal_scroll_bottom.set_y(this.internal_scroll_middle_vertical.height + 10);
		this.internal_scroll_bottom.set_bg_img(
				'ui/elements/scrollbars/'+ this.scroller_color +'/scrollbar_bottom.png');
		this.internal_scroll_bottom.parent_container = this.side_scroller;

	this.left_arrow_container = new PureFW.Container(
			this.node, 
			0, 0, 
			this.button_size, this.button_size);
	this.left_arrow_container.set_bg_img(
			'ui/elements/scrollbars/'+ this.scroller_color +'/scrollarrowleft.png');
	this.left_arrow_container.set_positioning('absolute');
	this.left_arrow_container.set_y(this.height - this.button_size);
	this.left_arrow_container.set_z_index(10);
	this.left_arrow_container.add_event_handler(
			"click", PureFW.ScrollContainer.left_arrow_clicked);
	this.left_arrow_container.add_event_handler("mousedown",
		(function(_instance) {
			return function(ev) {
				_instance.slide_scroll_interval = PureFW.Timeout.set_timeout(
					function() {
						_instance.scroll_left()
					}, 100);
			}
		})(this)
	);
	this.left_arrow_container.add_event_handler("mouseup",
		(function(_instance) {
			return function(ev) {
				PureFW.Timeout.clear_interval(_instance.slide_scroll_interval);
			}
		})(this)
	);
	this.left_arrow_container.add_event_handler("mouseout",
		(function(_instance) {
			return function(ev) {
				PureFW.Timeout.clear_interval(_instance.slide_scroll_interval);
			}
		})(this)
	);
	this.left_arrow_container.parent_container = this;
	
	this.right_arrow_container = new PureFW.Container(
			this.node, 
			0, 0, 
			this.button_size, this.button_size);
	this.right_arrow_container.set_bg_img(
			'ui/elements/scrollbars/'+ this.scroller_color +'/scrollarrowright.png');
	this.right_arrow_container.set_positioning('absolute');
	this.right_arrow_container.set_x(this.width - 2*this.button_size);
	this.right_arrow_container.set_y(this.height - this.button_size);
	this.right_arrow_container.set_z_index(10);
	this.right_arrow_container.add_event_handler(
			"click", PureFW.ScrollContainer.right_arrow_clicked);
	this.right_arrow_container.add_event_handler("mousedown",
		(function(_instance) {
			return function(ev) {
				_instance.slide_scroll_interval = PureFW.Timeout.set_interval(
					function() {
						_instance.scroll_right();
					}, 100);
			}
		})(this)
	);
	this.right_arrow_container.add_event_handler("mouseup",
		(function(_instance) {
			return function(ev) {
				PureFW.Timeout.clear_interval(_instance.slide_scroll_interval);
			}
		})(this)
	);
	this.right_arrow_container.add_event_handler("mouseout",
		(function(_instance) {
			return function(ev) {
				PureFW.Timeout.clear_interval(_instance.slide_scroll_interval);
			}
		})(this)
	);
	this.right_arrow_container.parent_container = this;
	
	this.bottom_scroller_container = new PureFW.Container(
			this.node, 
			0, 0, 
			this.width - 3*this.button_size, this.button_size);
	this.bottom_scroller_container.set_positioning('absolute');
	this.bottom_scroller_container.set_x(this.button_size);
	this.bottom_scroller_container.set_y(this.height - this.button_size);
	this.bottom_scroller_container.parent_container = this;
	
	if(this.inner_container.width > 0)
	{
		this.horizontal_scroller_size = 
			(this.bottom_scroller_container.width) * 
			(this.bottom_scroller_container.width / this.inner_container.width);
	}
	this.horizontal_scroller_proportion = 1;
	
	this.bottom_scroller = new PureFW.Container(
			this.bottom_scroller_container ,
			0, 0, 
			this.horizontal_scroller_size, this.button_size);

	this.bottom_scroller.set_z_index(9);
	this.bottom_scroller.set_id(this.horizontal_scroller_id);
	this.bottom_scroller.add_event_handler(
		"mousedown",
		(function(_instance) {
			return function(ev) {
				_instance._start_horizontal_dragging();
			}
		})(this)
	);
	this.bottom_scroller.add_event_handler(
		"mouseup",
		(function(_instance) {
			return function(ev) {
				_instance._stop_horizontal_dragging();
			}
		})(this)
	);
	this.bottom_scroller.parent_container = this.bottom_scroller_container;
	
		this.internal_scroll_left = new PureFW.Container(
				this.bottom_scroller , 0, 0, 10, this.button_size);
		this.internal_scroll_left.set_bg_img(
				'ui/elements/scrollbars/'+ this.scroller_color +'/scrollbar_left.png');
		this.internal_scroll_left.parent_container = this.bottom_scroller;
	
		this.internal_scroll_middle_horizontal = new PureFW.Container(
				this.bottom_scroller, 0, 0, 1, this.button_size);
		this.internal_scroll_middle_horizontal.set_x(10);
		this.internal_scroll_middle_horizontal.set_width(this.horizontal_scroller_size - 20);
		this.internal_scroll_middle_horizontal.set_bg_img(
				'ui/elements/scrollbars/'+ this.scroller_color +'/scrollbar_middle_horizontal.png');
		this.internal_scroll_middle_horizontal.parent_container = this.bottom_scroller;
	
		this.internal_scroll_right = new PureFW.Container(
				this.bottom_scroller, 0, 0, 10, this.button_size);
		this.internal_scroll_right.set_x(this.internal_scroll_middle_horizontal.width + 10);
		this.internal_scroll_right.set_bg_img(
				'ui/elements/scrollbars/'+ this.scroller_color +'/scrollbar_right.png');
		this.internal_scroll_right.parent_container = this.bottom_scroller;

	this.add_event_handler(
		"mouseover", 
		(function(_instance) {
			return function(ev) {
				_instance.selected()
			}
		})(this)
	);
	this.add_event_handler(
		"mouseout", 
		(function(_instance) {
			return function(ev) {
				_instance.deselected()
			}
		})(this)
	);
	this.hide_all_vertical();
	this.hide_all_horizontal();

	this.update_vertical_scroller();
	this.update_horizontal_scroller();
	
	//this.disableSelection();	<= causes bugs
	
	this.side_scroller.disable_selection();
	this.side_scroller_container.disable_selection();
	this.down_arrow_container.disable_selection();
	this.up_arrow_container.disable_selection();
	
	this.bottom_scroller.disable_selection();
	this.bottom_scroller_container.disable_selection();
	this.right_arrow_container.disable_selection();
	this.left_arrow_container.disable_selection();
};

PureFW.ScrollContainer.prototype.selected = function()
{
	scrolling.cur_obj = this.inner_container.get_node();
	scrolling.max_vertical_scroll = this.inner_container.height - this.height;
	scrolling.max_horizontal_scroll = this.inner_container.width - this.width;
	scrolling.top = parseInt(this.inner_container.get_node().style.top);
	scrolling.while_scrolling = PureFW.ScrollContainer.while_scrolling;
	PureFW.ScrollContainer.active_scrolling_instance = this;
	this.show_all_vertical();
	this.show_all_horizontal();
};

PureFW.ScrollContainer.prototype.deselected = function()
{
	this.hide_all_vertical();
	this.hide_all_horizontal();
	scrolling.cur_obj = null;
	scrolling.max_vertical_scroll = 0;
	scrolling.max_horizontal_scroll = 0;
	scrolling.top = 0;
};

PureFW.ScrollContainer.prototype.update_vertical_scroller = function()
{
	if(this.height < this.inner_container.height)
	{
		this.vertical_scrolling_enabled = true;
		this.max_vertical_scroll = this.inner_container.height - this.height + 2;
		this.vertical_scroller_size = 
			(this.side_scroller_container.height) * 
			(this.side_scroller_container.height / this.inner_container.height);
		if(isNaN(this.vertical_scroller_size))
		{
			this.vertical_scroller_size = 0;
		}
		if (this.vertical_scroller_size < 20)
		{
			this.vertical_scroller_size = 20;
		}
		this.internal_scroll_middle_vertical.set_height(this.vertical_scroller_size - 20);
		this.internal_scroll_bottom.set_y(this.internal_scroll_middle_vertical.height + 10);
		this.side_scroller.set_height(this.vertical_scroller_size);
		this.vertical_scroller_proportion = 
			(this.side_scroller_container.height- this.vertical_scroller_size) 
			/ this.max_vertical_scroll;
	}
	else
	{
		this.vertical_scrolling_enabled = false;
		this.hide_all_vertical();
		this.set_inner_top(0);
	}
};

PureFW.ScrollContainer.prototype.update_horizontal_scroller = function()
{
	if(this.width < this.inner_container.width)
	{
		this.horizontal_scrolling_enabled = true;
		this.max_horizontal_scroll = this.inner_container.width - this.width + 2;
		this.horizontal_scroller_size = 
			(this.bottom_scroller_container.width) * 
			(this.bottom_scroller_container.width / this.inner_container.width);
		if(isNaN(this.horizontal_scroller_size))
		{
			this.horizontal_scroller_size = 0;
		}
		if (this.horizontal_scroller_size < 20)
		{
			this.horizontal_scroller_size = 20;
		}
		this.internal_scroll_middle_horizontal.set_width(this.horizontal_scroller_size - 20);
		this.internal_scroll_right.set_x(this.internal_scroll_middle_horizontal.width + 10);
		this.bottom_scroller.set_width(this.horizontal_scroller_size);
		this.horizontal_scroller_proportion = 
			(this.bottom_scroller_container.width
					- this.horizontal_scroller_size) / this.max_horizontal_scroll;
	}
	else
	{
		this.horizontal_scrolling_enabled = false;
		this.hide_all_horizontal();
		this.set_inner_left(0);
	}
};

/**
 * The scroll function is used only for vertical scrolling, to handle the scroll
 * events fired by the mousewheel and such...
 */

PureFW.ScrollContainer.prototype.scroll = function(delta)
{
	if(this.vertical_scrolling_enabled)
	{
		if (delta < 0)
	    {
	    	this.scroll_down();
	    }
	    else
	    {
	    	this.scroll_up();
	    }
	}
	else if(!this.vertical_scrolling_enabled && this.horizontal_scrolling_enabled)
	{
		if (delta < 0)
	    {
	    	this.scroll_right();
	    }
	    else
	    {
	    	this.scroll_left();
	    }
	}
};

PureFW.ScrollContainer.prototype.scroll_up = function(amt)
{
	if((this.inner_container.position.y <= 0) && this.vertical_scrolling_enabled)
	{
		var relative_distance = this.height*this.vertical_scroll_interval;
		this.set_inner_top(this.inner_container.position.y + 
				(amt || relative_distance));
        if(this.inner_container.position.y > 0)
        {
        	this.set_inner_top(0);
        }
	}
};

PureFW.ScrollContainer.prototype.scroll_down = function(amt)
{
	this.max_vertical_scroll = this.inner_container.height - this.height;
	if((-this.inner_container.position.y <= this.max_vertical_scroll) 
			&& this.vertical_scrolling_enabled)
	{
		var relative_distance = this.height*this.vertical_scroll_interval;
		this.set_inner_top(this.inner_container.position.y - 
				(amt || relative_distance));
		if(-this.inner_container.position.y > this.max_vertical_scroll)
		{
			this.set_inner_top(-this.max_vertical_scroll);
		}
	}
};

PureFW.ScrollContainer.prototype.scroll_left = function(amt)
{
	if((this.inner_container.position.x <= 0) && this.horizontal_scrolling_enabled)
	{
		var relative_distance = this.width*this.horizontal_scroll_interval;
		this.set_inner_left(this.inner_container.position.x + 
				(amt || relative_distance));
        if(this.inner_container.position.x > 0)
        {
        	this.set_inner_left(0);
        }
	}
};

PureFW.ScrollContainer.prototype.scroll_right = function(amt)
{
	this.max_horizontal_scroll = this.inner_container.width - this.width;
	if((-this.inner_container.position.x <= this.max_horizontal_scroll) 
			&& this.horizontal_scrolling_enabled)
	{
		var relative_distance = this.width*this.horizontal_scroll_interval;
		this.set_inner_left(this.inner_container.position.x - 
				(amt || relative_distance));
		if(-this.inner_container.position.x > this.max_horizontal_scroll)
		{
			this.set_inner_left(-this.max_horizontal_scroll);
		}
	}
};

PureFW.ScrollContainer.prototype.set_vertical_scroll_interval = function(amount)
{
	if((!isNaN(amount) && (amount>0 && amount<=1)))
	{
		this.vertical_scroll_interval = amount;
	}
}

PureFW.ScrollContainer.prototype.set_horizontal_scroll_interval = function(amount)
{
	if((!isNaN(amount) && (amount>0 && amount<=1)))
	{
		this.horizontal_scroll_interval = amount;
	}
}

PureFW.ScrollContainer.prototype.scroll_to = function(x,y)
{
	if (x > this.max_horizontal_scroll)
		x = this.max_horizontal_scroll;
	else if (x < 0)
		x = 0;
	if (y > this.max_vertical_scroll)
		y = this.max_vertical_scroll;
	else if (y < 0)
		y = 0;
	this.set_inner_left(-x);
	this.set_inner_top(-y);
}

PureFW.ScrollContainer.prototype.set_inner_top = function(t, movescroller)
{
	if(typeof(movescroller) == 'undefined')
	{
		movescroller = true;
	}
	if(t || t===0)
	{
		this.inner_container.set_y(t);
	}
	if(movescroller == true)
	{
		this.set_scroller_vertical_position(-t);
	}
};

PureFW.ScrollContainer.prototype.set_inner_left = function(l, movescroller)
{
	if(typeof(movescroller) == 'undefined')
	{
		movescroller = true;
	}
	if(l || l===0)
	{
		this.inner_container.set_x(l);
	}
	if(movescroller == true)
	{
		this.set_scroller_horizontal_position(-l);
	}
};

/**
 * Wenn der Inhalt des Scroll-Containers eine dynamische Höhe aufweist und ihm
 * somit initial keine Höhe übergeben wurde, muss diese Funktion aufgerufen 
 * werden nach jeder Änderung des Inhalts.
 */
PureFW.ScrollContainer.prototype.update_inner_height = function() {
	this.inner_container.set_height(
		this.inner_container.get_height() / this.scale_factor
	);
	this.update_vertical_scroller();	
}

PureFW.ScrollContainer.prototype.set_inner_height = function(h)
{
	if(h >= 0)
	{
		this.inner_container.set_height(h);
		this.update_vertical_scroller();
	}
};

PureFW.ScrollContainer.prototype.set_inner_width = function(w)
{
	if(w >= 0)
	{
		this.inner_container.set_width(w);
		this.update_horizontal_scroller();
	}
};

PureFW.ScrollContainer.prototype.get_button_size = function()
{
	return (this.button_size * this.scale_factor);
};

PureFW.ScrollContainer.prototype.set_button_size = function(w) 
{
	if(w > 0)
	{
		this.button_size = w;
	}
};

PureFW.ScrollContainer.prototype.set_color = function(col)
{
	this.scroller_color = col;
	
	this.up_arrow_container.set_bg_img('ui/elements/scrollbars/'+ this.scroller_color +'/scrollarrowtop.png');
	this.down_arrow_container.set_bg_img('ui/elements/scrollbars/'+ this.scroller_color +'/scrollarrowbottom.png');
	this.internal_scroll_top.set_bg_img('ui/elements/scrollbars/'+ this.scroller_color +'/scrollbar_top.png');
	this.internal_scroll_middle_vertical.set_bg_img('ui/elements/scrollbars/'+ this.scroller_color +'/scrollbar_middle_vertical.png');
	this.internal_scroll_bottom.set_bg_img('ui/elements/scrollbars/'+ this.scroller_color +'/scrollbar_bottom.png');
	
	this.left_arrow_container.set_bg_img('ui/elements/scrollbars/'+ this.scroller_color +'/scrollarrowleft.png');
	this.right_arrow_container.set_bg_img('ui/elements/scrollbars/'+ this.scroller_color +'/scrollarrowright.png');
	this.internal_scroll_left.set_bg_img('ui/elements/scrollbars/'+ this.scroller_color +'/scrollbar_left.png');
	this.internal_scroll_middle_horizontal.set_bg_img('ui/elements/scrollbars/'+ this.scroller_color +'/scrollbar_middle_horizontal.png');
	this.internal_scroll_right.set_bg_img('ui/elements/scrollbars/'+ this.scroller_color +'/scrollbar_right.png');
	
	this.repaint();
};

PureFW.ScrollContainer.prototype.get_vertical_scroller_size = function()
{
	return this.vertical_scroller_size * this.scale_factor;
};

PureFW.ScrollContainer.prototype.get_horizontal_scroller_size = function()
{
	return this.horizontal_scroller_size * this.scale_factor;
};

PureFW.ScrollContainer.prototype.set_scroller_vertical_position = function(p)
{
	if(p || p===0)
	{
		this.vertical_scroller_position = p;
		this.side_scroller.set_y(p * this.vertical_scroller_proportion);
	}
};

PureFW.ScrollContainer.prototype.get_scroller_vertical_position = function()
{
	return (this.vertical_scroller_position);
};

PureFW.ScrollContainer.prototype.set_scroller_horizontal_position = function(p)
{
	if(p || p===0)
	{
		this.horizontal_scroller_position = p;
		this.bottom_scroller.set_x(p * this.horizontal_scroller_proportion);
	}
};

PureFW.ScrollContainer.prototype.get_scroller_horizontal_position = function()
{
	return (this.horizontal_scroller_position);
};

PureFW.ScrollContainer.prototype.show_all_vertical = function()
{
	if(this.vertical_scrolling_enabled)
	{
		this.up_arrow_container.show();
		this.side_scroller_container.show();
		this.down_arrow_container.show();
	}
};

PureFW.ScrollContainer.prototype.hide_all_vertical = function()
{
	if(this.hiding_allowed)
	{
		this.up_arrow_container.hide();
		this.side_scroller_container.hide();
		this.down_arrow_container.hide();
	}
};

PureFW.ScrollContainer.prototype.show_all_horizontal = function()
{
	if(this.horizontal_scrolling_enabled)
	{
		this.left_arrow_container.show();
		this.bottom_scroller_container.show();
		this.right_arrow_container.show();
	}
};

PureFW.ScrollContainer.prototype.hide_all_horizontal = function()
{
	if(this.hiding_allowed)
	{
		this.left_arrow_container.hide();
		this.bottom_scroller_container.hide();
		this.right_arrow_container.hide();
	}
};

/**
 * PRIVATE: Startet mit dem Draggen des Slider-Buttons.
 */
PureFW.ScrollContainer.prototype._start_vertical_dragging = function() 
{
	PureFW.MouseFeatures.Dragging.start_dragging(
		document.getElemById(this.vertical_scroller_id)
	);
	PureFW.MouseFeatures.Dragging.restrict_direction(
		PureFW.MouseFeatures.Dragging.RESTRICT_MOVING_Y
	);
	PureFW.MouseFeatures.Dragging.set_pos_limits(
		false,	// min_x
		0,		// min_y
		false,	// max_x
		(this.side_scroller_container.height -	 // max_y
			this.vertical_scroller_size)		
	);
	PureFW.MouseFeatures.Dragging.add_event_handler(
		"dragging",
		this.constructor.while_vertical_dragging
	);
	PureFW.MouseFeatures.Dragging.add_event_handler(
		"drop",
		this.constructor.vertical_scroller_dropped
	);
	this.constructor.active_dragging_instance = this;
	this.hiding_allowed = false;
};

/**
 * PRIVATE: Startet mit dem Draggen des Slider-Buttons.
 */
PureFW.ScrollContainer.prototype._start_horizontal_dragging = function() 
{
	PureFW.MouseFeatures.Dragging.start_dragging(
		document.getElemById(this.horizontal_scroller_id)
	);
	PureFW.MouseFeatures.Dragging.restrict_direction(
		PureFW.MouseFeatures.Dragging.RESTRICT_MOVING_X
	);
	PureFW.MouseFeatures.Dragging.set_pos_limits(
		0,		// min_x
		false,	// min_y
		(this.bottom_scroller_container.width - 
			this.horizontal_scroller_size),	// max_x
		false	// max_y		
	);
	PureFW.MouseFeatures.Dragging.add_event_handler(
		"dragging",
		this.constructor.while_horizontal_dragging
	);
	PureFW.MouseFeatures.Dragging.add_event_handler(
		"drop",
		this.constructor.horizontal_scroller_dropped
	);
	this.constructor.active_dragging_instance = this;
	this.hiding_allowed = false;
};

/**
 * PRIVATE: Stoppt mit dem Draggen des Slider-Buttons.
 */
PureFW.ScrollContainer.prototype._stop_horizontal_dragging = function() 
{
	PureFW.ScrollContainer.active_dragging_instance = null;
	PureFW.MouseFeatures.Dragging.remove_event_handler(
		"dragging",
		this.constructor.while_horizontal_dragging
	);
	PureFW.MouseFeatures.Dragging.remove_event_handler(
		"drop",
		this.constructor.horizontal_scroller_dropped
	);
	this.hiding_allowed = true;
	this.hide_all_horizontal();
};

/**
 * PRIVATE: Stoppt mit dem Draggen des Slider-Buttons.
 */
PureFW.ScrollContainer.prototype._stop_vertical_dragging = function() 
{
	PureFW.MouseFeatures.Dragging.remove_event_handler(
		"dragging",
		this.constructor.while_vertical_dragging
	);
	PureFW.MouseFeatures.Dragging.remove_event_handler(
		"drop",
		this.constructor.vertical_scroller_dropped
	);
	PureFW.ScrollContainer.active_dragging_instance = null;
	this.hiding_allowed = true;
	this.hide_all_vertical();
};

/**
 * PRIVATE: Berechnet den aktuellen Wert anhand der Sliderposition. Es wird 
 * dabei KEIN Event gefeuert!
 */
PureFW.ScrollContainer.prototype._calculate_vertical_value = function(obj_pos) 
{
	var value = obj_pos.y / this.vertical_scroller_proportion;
	this.set_inner_top(-value, false);
};

/**
 * PRIVATE: Berechnet den aktuellen Wert anhand der Sliderposition. Es wird 
 * dabei KEIN Event gefeuert!
 */
PureFW.ScrollContainer.prototype._calculate_horizontal_value = function(obj_pos) 
{
	var value = obj_pos.x / this.horizontal_scroller_proportion;
	this.set_inner_left(-value, false);
};

/**
 * Setzt den Inhalt (HTML) des Containers. Wird interprete_js angegeben, wird
 * ggf. mitgesandtes JS (innerhalb der Tags <script></script>) interpretiert!
 * 
 * @param string text
 * @param bool interprete_js [=false]
 */
PureFW.ScrollContainer.prototype.set_content = function(text, interprete_js) {
	this.on_change(this.create_event("change"));
	this.inner_container.set_content(text, interprete_js);
};
 
/**
 * Gibt den Inhalt (HTML) des Containers zurück.
 * 
 * @return string
 */
PureFW.ScrollContainer.prototype.get_content = function() {
	return this.get_content_node().innerHTML;
};

/**
 * Gibt den Knoten zurück, der den sichtbaren Inhalt des Containers enthält.
 * Möchte man z.B. in den Container ein weiteres Widget integrieren, erhält man
 * hierüber den nötigen Vaterknoten für den Konstruktor des Widgets.
 * 
 * @return HTMLElement
 */
PureFW.ScrollContainer.prototype.get_content_node = function() {
		return this.inner_container.get_node();
};

/**
 * Setzt die Breite des ScrollContainers. Wenn w <= 0, dann wird sie dynamisch zur
 * Laufzeit anhand des Inhalts bestimmt.
 * 
 * @param int w
 */
PureFW.ScrollContainer.prototype.set_width = function(w) {
	PureFW.ScrollContainer.parent.set_width.call(this, w);
	if ((w >= this.inner_container.width || w == 0) 
		&& (this.inner_container.width != 0))
	{
		this.set_inner_width(w);
	}
};

/**
 * Setzt die Höhe des ScrollContainers. Wenn h <= 0, dann wird sie dynamisch zur
 * Laufzeit anhand des Inhalts bestimmt.
 * 
 * @param int h
 */
PureFW.ScrollContainer.prototype.set_height = function(h) {
	PureFW.ScrollContainer.parent.set_height.call(this, h);
	if ((h >= this.inner_container.height || h == 0)
		&& (this.inner_container.height != 0))
	{
		this.set_inner_height(h);
	}
};

PureFW.ScrollContainer.prototype.repaint = function()
{
	PureFW.ScrollContainer.parent.repaint.call(this);
	
	var left = 0;
	var right = this.width - this.button_size;
	var top = 0;
	var bottom = this.height - this.button_size;
	
	this.left_arrow_container.set_y(bottom);
	this.bottom_scroller_container.set_x(this.button_size);
	this.bottom_scroller_container.set_width(this.width - 3*this.button_size);
	this.bottom_scroller_container.set_y(bottom);
	this.right_arrow_container.set_x(right - this.button_size);//so it doesn't cover the down arrow
	this.right_arrow_container.set_y(bottom);
	
	this.up_arrow_container.set_x(right);
	this.side_scroller_container.set_x(right);
	this.side_scroller_container.set_height(this.height - 2*this.button_size);
	this.side_scroller_container.set_y(this.button_size);
	this.down_arrow_container.set_x(right);
	this.down_arrow_container.set_y(bottom);
	
	this.update_vertical_scroller();
	this.update_horizontal_scroller();
};

PureFW.ScrollContainer.prototype.scale = function()
{
	PureFW.ScrollContainer.parent.scale.call(this);

	this.set_inner_left(0);
	this.set_inner_top(0);	
};