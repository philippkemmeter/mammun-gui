/**
 * A Bar containing bubbles, where only one is 'active' at any given time. The
 * bar can dynamically grow and shrink.
 * 													         	(((
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
PureFW.BubbleBar = function(parent_node, x, y, w, h) { 
	if (typeof(parent_node) !== 'undefined')
	{
		this.init(parent_node, x, y, w, h);
	
		this.instance_num = PureFW.BubbleBar.num_instances;
		this.instance_name = "PureFW.BubbleBar.instances["
														+this.instance_num+"]";
		this.id = "BubbleBar"+this.instance_num;
		this.content_id = "BubbleBar_contents"+this.instance_num;
		this.bg_img_id = 'BubbleBar_bg'+this.instance_num;

		this.insert_into_dom_tree();
		PureFW.BubbleBar.num_instances++;
		PureFW.BubbleBar.instances[this.instance_num] = this;

		PureFW.WidgetManager.manager_all.register_widget(this);
	}
};
/********************************************************************
 * BubbleBar Bar extends Container
 ********************************************************************/
PureFW.BubbleBar.extend(PureFW.Container);

/********************************************************************
 * Statische Deklarationen
 ********************************************************************/
PureFW.BubbleBar.num_instances = 0;
PureFW.BubbleBar.instances = new Array();

/********************************************************************
 * Prototyp-Deklarationen
 ********************************************************************/
PureFW.BubbleBar.prototype.init = function(parent_node, x, y, w, h) 
{
	PureFW.BubbleBar.parent.init.call(this, parent_node, x, y, w, h);
	
	this.bubble_list = null;
	this.active_bubble_index = 0;
	this.bubble_size = 0;
	this.bubble_spacing = 0;
	this.active_bubble_url = null;
	this.inactive_bubble_url = null;
};

PureFW.BubbleBar.prototype.insert_into_dom_tree = function()
{
	PureFW.BubbleBar.parent.insert_into_dom_tree.call(this);

	this.set_id(this.id);
	
	this.bubble_list = new Array();
	this.bubble_size = 14;
	this.bubble_spacing = 6;
	this.active_bubble_url = 'ui/elements/bubblebar/bubble_active_14x14.png';
	this.inactive_bubble_url = 'ui/elements/bubblebar/bubble_inactive_14x14.png';
};

/**
 * Adds a new bubble to the end of the bar, even if none exists. The position of 
 * the next bubble to be added is determined by the current amount of bubbles 
 * and the spaces in between. The first bubble created is automatically 
 * activated. Upon creation of a new bubble, the currently active bubble remains 
 * unchanged.
 * 
 * @throws Error when there are more bubbles than can be fit in the width of the
 * Container. This is just for safety to alert the developer of overflow.
 * 
 * @return PureFW.Image		The newly created Bubble
 */
PureFW.BubbleBar.prototype.add_bubble = function()
{
	var next_bubble_position = (this.bubble_list.length * this.bubble_size)
	if(this.bubble_list.length > 0)
	{
		next_bubble_position += 
			((this.bubble_list.length) * this.bubble_spacing);
	}
	this.bubble_list[this.bubble_list.length] = this.add_widget(
		PureFW.Image,
		next_bubble_position, 0,
		this.bubble_size, this.bubble_size,
		this.inactive_bubble_url
	);
	if(this.bubble_list.length == 1)
	{
		this.activate_bubble(0);
	}
	if((next_bubble_position + this.bubble_size) > this.width)
	{
/*		throw new Error(this.instance_name+" is overflowing its width. "+
				"More bubbles cannot be shown.");*/
	}
	
	return this.bubble_list[this.bubble_list.length-1];
};

/**
 * Removes the last bubble from the bar. If the last bubble was active, the
 * bubble before it is activated before removal.
 */
PureFW.BubbleBar.prototype.remove_bubble = function()
{
	if(this.bubble_list.length === 0)
	{
		return;
	}
	if(this.active_bubble_index == (this.bubble_list.length - 1))
	{
		this.previous();
	}
	this.bubble_list.pop().destroy();
};

PureFW.BubbleBar.prototype.remove_all_bubbles = function() {
	this.bubble_list.destroy();
	this.bubble_list = new Array();
	this.active_bubble_index = 0;
}

/**
 * Activates the next bubble in the bar. If the last bubble is already active,
 * nothing happens.
 */
PureFW.BubbleBar.prototype.next = function()
{
	if(((this.bubble_list.length - 1) == this.active_bubble_index) || 
			this.bubble_list.length == 0)
	{
		return;
	}
	else
	{
		this.bubble_list[this.active_bubble_index].set_pic_url(
			this.inactive_bubble_url);
		this.active_bubble_index++;
		this.bubble_list[this.active_bubble_index].set_pic_url(
			this.active_bubble_url);
	}
};

/**
 * Activates the bubble previous to the one currently active. If the first
 * bubble in the bar is active, nothing happens.
 */
PureFW.BubbleBar.prototype.previous = function()
{
	if(this.active_bubble_index === 0)
	{
		return;
	}
	else
	{
		this.bubble_list[this.active_bubble_index].set_pic_url(
			this.inactive_bubble_url);
		this.active_bubble_index--;
		this.bubble_list[this.active_bubble_index].set_pic_url(
			this.active_bubble_url);
	}
};

/**
 * Activates a given bubble and deactivates the currently active one. Do not use 
 * this function directly. Use next() instead.
 */
PureFW.BubbleBar.prototype.activate_bubble = function(bubble_index)
{
	this.bubble_list[this.active_bubble_index].set_pic_url(
		this.inactive_bubble_url);
	this.bubble_list[bubble_index].set_pic_url(this.active_bubble_url);
	this.active_bubble_index = bubble_index;
};

/**
 * Sets the square size of the bubbles in the bar.
 * 
 * @param size: Size, in pixels, of the bubbles
 */
PureFW.BubbleBar.prototype.set_bubble_size = function(size)
{
	this.bubble_size = size;
	this.repaint();
};

/**
 * Returns the size of the bubbles (width = height)
 */
PureFW.BubbleBar.prototype.get_bubble_size = function()
{
	return this.bubble_size * this.scale_factor;
};

/**
 * Sets the spacing between the bubbles. Default is 6px.
 * 
 * @param spacing: The pixel spacing of the bubbles.
 */
PureFW.BubbleBar.prototype.set_bubble_spacing = function(spacing)
{
	this.bubble_spacing = spacing;
	this.repaint();
};

/**
 * Returns the spacing between the bubbles.
 */
PureFW.BubbleBar.prototype.get_bubble_spacing = function()
{
	return this.bubble_spacing * this.scale_factor;
};

/**
 * Returns the number of bubbles in the bar.
 */
PureFW.BubbleBar.prototype.get_bubble_amount = function()
{
	return this.bubble_list.length;
};

/**
 * ENTFERNEN HIER NICHT UNTERSTÜTZ :(
 * @param string type
 * @param Function fn
 * @see Container.prototype.remove_event_handler
 */
PureFW.BubbleBar.prototype.remove_event_handler = null;


/**
 * No direct access to the functions below.
 */
PureFW.BubbleBar.prototype.set_content = null;
PureFW.BubbleBar.prototype.get_content = null
