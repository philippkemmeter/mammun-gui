/**
 * @author Phil
 */

PureFW.ProgressBar = function(parent_node, x, y, w, h) 
{
	if (typeof(parent_node) !== 'undefined') 
	{
		this.init();
		this.parent_node = parent_node || document;
		this.position = new PureFW.Point(x || 0, y || 0);
		this.width = w || 142;
		this.height = h || 15;
		
		this.instance_num = this.constructor.num_instances;
		this.instance_name = "PureFW.ProgressBar.instances["+this.instance_num+"]";
		
		this.id = "ProgressBar"+this.instance_num;
		this.bg_img_id = "ProgressBar_bg_img"+this.instance_num;
		this.content_id = "ProgressBar_cont"+this.instance_num;
		
		this.node = null;
		this.empty_bar_container = null;
		this.background_url = 'ui/elements/progressbars/empty_142x15.png';
		this.progress_container = null;
		this.progress_value = null;
		this.progress_url = 'ui/elements/progressbars/full_142x15.png';
		this.timer_container = null;
		this.reference_value = 100;
		
		this.insert_into_dom_tree();
		this.constructor.num_instances++;
		this.constructor.instances[this.instance_num] = this;
		
		PureFW.WidgetManager.manager_all.register_widget(this);
	}
};

/********************************************************************
* ProgressBar extends Widget
********************************************************************/

PureFW.ProgressBar.extend(PureFW.Container);

/********************************************************************
 * Statische Deklarationen
 ********************************************************************/
PureFW.ProgressBar.num_instances = 0;
PureFW.ProgressBar.instances = new Array();

/********************************************************************
 * Prototype-Funktionen
 ********************************************************************/

PureFW.ProgressBar.prototype.insert_into_dom_tree = function() 
{
	PureFW.ProgressBar.parent.insert_into_dom_tree.call(this);
			
	this.empty_bar_container = new PureFW.Container(
			this.node, 0, 0, this.width, this.height);
	this.empty_bar_container.set_bg_img(this.background_url);
	this.empty_bar_container.set_z_index(2);
	
	this.progress_container = new PureFW.Container(
			this.node, 0, 0, 0, this.height);
	this.progress_container.set_z_index(3);
	this.progress_container.get_node().style.overflow = 'hidden';
	
	this.progress_bar_container = new PureFW.Container(
			this.progress_container.get_content_node(), 0, 0, 
			this.width, this.height);
	this.progress_bar_container.set_bg_img(this.progress_url);
	
	this.timer_container = new PureFW.Container(
			this.node, 0, 0, this.width, this.height);
	this.timer_container.get_node().style.textAlign = 'center';
	this.timer_container.set_font_weight('bold');
	this.timer_container.set_z_index(10);

	this.parent_node.appendChild(this.node);
	
	this.disable_selection();
};
/**
 * Setzt den Wert
 *
 * @param uint v
 */
PureFW.ProgressBar.prototype.set_value = function(v)
{
	this.progress_value = v;
	this._draw_current_progress();
};
 
/**
 * Gibt den Wert zurück
 *
 * @return uint
 */
PureFW.ProgressBar.prototype.get_value = function()
{
	return this.progress_value;
};
 
/**
 * Reference value
 *
 * @param uint v
 */
PureFW.ProgressBar.prototype.set_reference_value = function(v)
{
	this.reference_value = v;
	this._draw_current_progress();
};

PureFW.ProgressBar.prototype.set_time = function(t)
{
	this.timer_container.set_content(t);
	this.timer_container.repaint();
}

 
PureFW.ProgressBar.prototype._draw_current_progress = function()
{
	var percent = this.progress_value / this.reference_value;
	this.progress_container.set_width(percent*this.width);
	this.repaint();
};