/**
 * Dieses Widget repräsentiert die Vorschau einer Insel, welche bei
 * Inselwahl als Option erscheint.
 * 
 * @package ChooseMap
 */

if (typeof(PureFW.ChooseMap) != 'object')
	PureFW.ChooseMap = new Object();

PureFW.ChooseMap.IslandPreview = function(parent_node, x, y, w, h, 
	template_id, no_scale)
{
	if (typeof(parent_node) !== 'undefined') {
		this.init(parent_node, x, y, w, h, template_id, no_scale);
		
		this.instance_num = this.constructor.num_instances;
		this.instance_name = "PureFW.ChooseMap" +
				".IslandPreview.instances["	+this.instance_num+"]";
		
		this.id = "PureFW.ChooseMap.IslandPreview"+this.instance_num;
		this.content_id 
			= "PureFW.ChooseMap.IslandPreview_cont"+this.instance_num;
		this.bg_img_id = 'PureFW.ChooseMap.IslandPreview_bg'
			+this.instance_num;
		
		this.insert_into_dom_tree();
		this.constructor.num_instances++;
		this.constructor.instances[this.instance_num] = this;
		
		if (!this.no_scale)
			PureFW.WidgetManager.manager_all.register_widget(this);
	}
};

/********************************************************************
 * PureFW.ChooseMap.IslandPreview extends MapTemplatePreview
 ********************************************************************/
PureFW.ChooseMap.IslandPreview.extend(PureFW.ChooseMap.MapTemplatePreview);

/********************************************************************
 * Statische Deklarationen
 ********************************************************************/
PureFW.ChooseMap.IslandPreview.num_instances = 0;		// Instanzüberwachung (anzahl)
PureFW.ChooseMap.IslandPreview.instances = new Array();// Instanzüberwachung (Instanzen)


/********************************************************************
 * Prototyp-Deklarationen
 ********************************************************************/
PureFW.ChooseMap.IslandPreview.prototype.init = function(parent_node, 
	x, y, w, h, template_id, no_scale)
{
	PureFW.ChooseMap.IslandPreview.parent.init.call(this, parent_node, 
		x, y, w, h, template_id, no_scale);
	
	this.xp_level = 0;
	this.xp_cont = null;
	this.xp_cont_inner = null;
};

PureFW.ChooseMap.IslandPreview.prototype.insert_into_dom_tree = function() 
{
	PureFW.ChooseMap.IslandPreview.parent.insert_into_dom_tree.call(this);

	this.get_node().style.overflow = 'visible';
	
	this.xp_cont = this.add_widget(
		PureFW.Container,
		-7, -12,
		77, 77,
		this.no_scale
	);
	this.xp_cont.set_bg_img('ui/icons/labels/xp_star/77/star.png');
	this.xp_cont.set_z_index(2);
	
	this.xp_cont_inner = this.xp_cont.add_widget(
		PureFW.Container,
		0, 28,
		this.xp_cont.width, 0
	);
	this.xp_cont_inner.set_css_class('ChooseMap_IslandPreview_xp_cont_inner');
	this.xp_cont_inner.set_content(this.xp_level);
};


/**
 * Setzt die ID des Templates, das angezeigt werden soll
 */
PureFW.ChooseMap.IslandPreview.prototype.set_template_id = function(id) {
	this.template_id = id;
	this.template_mini_img.set_pic_url(
		'map/templates/'+this.template_id+'/_0_/template'+this.template_id
		+'_44.jpg'
	);
};

/**
 * Setzt die XP-Anzahl in Leveln, die mit dem Widget assoziiert werden soll.
 * 
 * @param uint xp
 */
PureFW.ChooseMap.IslandPreview.prototype.set_xp_level = function(xp) {
	this.xp_level = xp;
	this.xp_cont_inner.set_content(xp);
};