/**
 * Dieses Widget repräsentiert die Vorschau eines MapTemplates, welche beim
 * Insel-Hosting in der Liste zu wählender Inselformen erscheint.
 * 
 * @package ChooseMap
 */

if (typeof(PureFW.ChooseMap) != 'object')
	PureFW.ChooseMap = new Object();

PureFW.ChooseMap.MapTemplatePreview = function(parent_node, x, y, w, h, 
	template_id, no_scale)
{
	if (typeof(parent_node) !== 'undefined') {
		this.init(parent_node, x, y, w, h, template_id, no_scale);
		
		this.instance_num = this.constructor.num_instances;
		this.instance_name = "PureFW.ChooseMap" +
				".MapTemplatePreview.instances["	+this.instance_num+"]";
		
		this.id = "PureFW.ChooseMap.MapTemplatePreview"+this.instance_num;
		this.content_id 
			= "PureFW.ChooseMap.MapTemplatePreview_cont"+this.instance_num;
		this.bg_img_id = 'PureFW.ChooseMap.MapTemplatePreview_bg'
			+this.instance_num;
		
		this.insert_into_dom_tree();
		this.constructor.num_instances++;
		this.constructor.instances[this.instance_num] = this;
		
		if (!this.no_scale)
			PureFW.WidgetManager.manager_all.register_widget(this);
	}
};

/********************************************************************
 * PureFW.ChooseMap.MapTemplatePreview extends Container
 ********************************************************************/
PureFW.ChooseMap.MapTemplatePreview.extend(PureFW.PolaroidContainerOverlay);

/********************************************************************
 * Statische Deklarationen
 ********************************************************************/
PureFW.ChooseMap.MapTemplatePreview.num_instances = 0;		// Instanzüberwachung (anzahl)
PureFW.ChooseMap.MapTemplatePreview.instances = new Array();// Instanzüberwachung (Instanzen)


/********************************************************************
 * Prototyp-Deklarationen
 ********************************************************************/
PureFW.ChooseMap.MapTemplatePreview.prototype.init = function(parent_node, 
	x, y, w, h, template_id, no_scale)
{
	PureFW.ChooseMap.MapTemplatePreview.parent.init.call(this, parent_node, 
		x, y, w, h, no_scale);
	
	this.template_id = template_id;
	
	this.template_mini_img = null;
	this.template_label = null;
};

PureFW.ChooseMap.MapTemplatePreview.prototype.insert_into_dom_tree = function() 
{
	PureFW.ChooseMap.MapTemplatePreview.parent.insert_into_dom_tree.call(this);

	if (this.template_id) {
		this.picture.set_pic_url(
			'map/templates/'+this.template_id+'/_0_/template'+this.template_id
				+'_150_3d.jpg'
		);
	}
	
	this.set_overlay_height(48);
	
	this.template_mini_img = this.overlay.add_widget(
		PureFW.Image,
		2, 2,
		this.overlay.height - 4,
		this.overlay.height - 4,
		((this.template_id) 
			? 'map/templates/'+this.template_id+'/_0_/template'+this.template_id
				+'_44.jpg'
			: 'pattern/spacer.gif'),
		this.no_scale
	);
	
	this.template_label = this.overlay.add_widget(
		PureFW.Container,
		this.template_mini_img.position.x + this.template_mini_img.width + 7,
		this.template_mini_img.position.y,
		this.overlay.width - this.template_mini_img.position.x  
			- this.template_mini_img.width - 9,
		this.overlay.height - this.template_mini_img.position.y,
		this.no_scale
	);
	this.template_label.set_css_class(
		'ChooseMap_MapTemplatesPreview_template_label'
	);
};

/**
 * Setzt die ID des Templates, das angezeigt werden soll
 */
PureFW.ChooseMap.MapTemplatePreview.prototype.set_template_id = function(id) {
	this.template_id = id;
	this.picture.set_pic_url(
		'map/templates/'+this.template_id+'/_0_/template'+this.template_id
			+'_150_3d.jpg'
	);
	this.template_mini_img.set_pic_url(
		'map/templates/'+this.template_id+'/_0_/template'+this.template_id
		+'_44.jpg'
	);
};

PureFW.PolaroidContainerOverlay.prototype.add_overlay_content = function(s) {
	this.template_label.add_content(s);
};

/**
 * Fügt ein Widget dem Overlay hinzu.
 * 
 * @param Function widget_constructor
 * @param uint x
 * @param uint y
 * @param uint w
 * @param uint h
 * @param mixed argN
 * @return Widget
 * 
 * @see PureFW.Container#add_widget
 */
PureFW.PolaroidContainerOverlay.prototype.add_widget_to_overlay = function(
	widget_constructor, x, y, w, h, arg6, arg7, arg8, arg9, arg10, arg11, 
	arg12, arg13)
{
	return this.template_label.add_widget(
		widget_constructor, x, y, w, h, arg6, arg7, arg8, arg9, arg10, arg11, 
		arg12, arg13);
}