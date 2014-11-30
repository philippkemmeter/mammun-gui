/**
 * Dieses Widget ist eine Erweiterung des PolaroidContainers. Hier können nun
 * auch Über das Bild zusätzliche Beschriftungen angebracht werden.
 */

PureFW.PolaroidMultiPic = function(parent_node, x, y, w, h, no_scale)
{
	if (typeof(parent_node) !== 'undefined') {
		this.init(parent_node, x, y, w, h, no_scale);
		
		this.instance_num = this.constructor.num_instances;
		this.instance_name = "PureFW.PolaroidMultiPic" +
				".instances["	+this.instance_num+"]";
		
		this.id = "PureFW.PolaroidMultiPic"+this.instance_num;
		this.content_id 
			= "PureFW.PolaroidMultiPic_cont"+this.instance_num;
		this.bg_img_id = 'PureFW.PolaroidMultiPic_bg'
			+this.instance_num;
		
		this.insert_into_dom_tree();
		this.constructor.num_instances++;
		this.constructor.instances[this.instance_num] = this;
		
		if (!this.no_scale)
			PureFW.WidgetManager.manager_all.register_widget(this);
	}
};

/********************************************************************
 * PureFW.PolaroidMultiPic extends PolaroidContainerOverlay
 ********************************************************************/
PureFW.PolaroidMultiPic.extend(PureFW.PolaroidContainer);

/********************************************************************
 * Statische Deklarationen
 ********************************************************************/
PureFW.PolaroidMultiPic.num_instances = 0;		// Instanzüberwachung (anzahl)
PureFW.PolaroidMultiPic.instances = new Array();// Instanzüberwachung (Instanzen)


/********************************************************************
 * Prototyp-Deklarationen
 ********************************************************************/
PureFW.PolaroidMultiPic.prototype.init = function(parent_node, 
	x, y, w, h, no_scale)
{
	PureFW.PolaroidMultiPic.parent.init.call(this, parent_node, 
		x, y, w, h, no_scale);

	this.layout_dimensions = null;
	this.pic_array = null;
	this.spacing = 0;
};

PureFW.PolaroidMultiPic.prototype.insert_into_dom_tree = function() 
{
	PureFW.PolaroidMultiPic.parent.insert_into_dom_tree.call(this);

	/**
	 * Including a seperate property for dimensions here in case we want to 
	 * extend this widget later to support different picture layouts. Will only
	 * implement a 3x2 layout for now.
	 */
	this.layout_dimensions = new PureFW.Point(3, 2);
	this.pic_amount = this.layout_dimensions.x * this.layout_dimensions.y;
	this.pic_array = new Array();
	this.spacing = 1;
	
	var x = 0;
	var y = 0;
	var pic_width= this.picture.width/this.layout_dimensions.x - this.spacing*2;
	var pic_height = pic_width;//this.picture.height/this.layout_dimensions.y - this.spacing*2;
	
	for(var i = 0; i < this.pic_amount; i++)
	{
		this.pic_array[i] = this.add_widget(
			PureFW.Avatar.Avatar,
			this.picture.position.x+this.spacing +(x*(pic_width+this.spacing*2)), 
			this.picture.position.y+this.spacing +(y*(pic_height+this.spacing*2)), 
			pic_width, pic_height);
		x++;
		if (x >= this.layout_dimensions.x)
		{
			x = 0;
			y++;
		}
	}
	this.picture.set_pic_url('pattern/spacer.gif');
};

PureFW.PolaroidMultiPic.prototype.set_chosen_pic_url = function(index, url)
{
	this.pic_array[index].avatar_img.set_pic_url(url);
};