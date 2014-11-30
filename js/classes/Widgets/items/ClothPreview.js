/**
 * Dieses Widget bietet eine Vorschau eines Items an einer Unit.
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
PureFW.Items.ClothPreview = function(parent_node, x, y, w, h, no_scale)
{
	if (typeof(parent_node) !== 'undefined') {
		this.init(parent_node, x, y, w, h, no_scale);
		
		this.instance_num = this.constructor.num_instances;
		this.instance_name = "MammunUI.get_top_frame().PureFW.Items" +
				".ClothPreview.instances["	+this.instance_num+"]";
		
		this.id = "PureFW.Items.ClothPreview"+this.instance_num;
		this.content_id 
			= "PureFW.Items.ClothPreview_cont"+this.instance_num;
		this.bg_img_id = 'PureFW.Items.ClothPreview_bg'+this.instance_num;
		
		this.insert_into_dom_tree();
		this.constructor.num_instances++;
		this.constructor.instances[this.instance_num] = this;
		
		if (!this.no_scale)
			PureFW.WidgetManager.manager_all.register_widget(this);
	}
};
/********************************************************************
 * PureFW.Items.ClothPreview extends Container
 ********************************************************************/
PureFW.Items.ClothPreview.extend(PureFW.Container);

/********************************************************************
 * Statische Deklarationen
 ********************************************************************/
PureFW.Items.ClothPreview.num_instances = 0;		// Instanzüberwachung (anzahl)
PureFW.Items.ClothPreview.instances = new Array();	// Instanzüberwachung (Instanzen)



/********************************************************************
 * Prototyp-Deklarationen
 ********************************************************************/
PureFW.Items.ClothPreview.prototype.init = function(parent_node, x, y, w, h,
	no_scale)
{
	PureFW.Items.ClothPreview.parent.init.call(this, parent_node, x, y, w, h,
		no_scale);

	this.item = new Object();
	this.item.type = 0;
	this.item.id = 0;
	this.unit_type = null;
	this.preview_cont = null;
};

PureFW.Items.ClothPreview.prototype.insert_into_dom_tree = function() {
	PureFW.Items.ClothPreview.parent.insert_into_dom_tree.call(this);
	
	this.set_bg_img('ui/backgrounds/Items/ClothPreview/bg_'+this.width+'x'
		+this.height+'.png');
	this.set_overflow('visible');
	
	this.preview_cont = this.add_widget(
		PureFW.Image,
		(this.width - 165) >> 1,
		(this.height - 289) >> 1,
		165, 289,
		'pattern/spacer.gif'
	);
	
	this.rareness_meter = this.add_widget(
		PureFW.Items.RarenessMeter,
		this.width - 18,
		(this.height - 246) >> 1,
		18, 246
	);
}

PureFW.Items.ClothPreview.prototype._refresh_pic_url = function() {
	if (this.item.type < 10) {
		var img_pic_path = 'ui/units/training/'+this.unit_type+'/'
													+this.unit_type+'_small_0_';
		for (var i = 0; i < this.item.type-1; i++) {
			img_pic_path += '0_';
		}
		img_pic_path += this.item.id;
		for (var i = this.item.type; i < 6; i++) {
			img_pic_path += '_0';
		}
		img_pic_path += '_0.png';
	}
	else if (this.item.type == 10) {
		img_pic_path = 'ui/elements/building_preview_boxes/item_detail_view/'
			+this.item.id + '.png';
	}
	this.preview_cont.set_pic_url(img_pic_path);
}

/**
 * Setzt das Item.
 * 
 * @param Object item
 */
PureFW.Items.ClothPreview.prototype.set_item = function(item) {
	this.item.id = item.id;
	this.item.type = item.type;
	this.rareness_meter.set_value(item.rareness_factor);
	if (item.xp > 0) {
		this.set_bg_img('ui/backgrounds/Items/ClothPreview/bg_xp_'
			+this.width+'x'+this.height+'.png'
		);
	}
	if (this.unit_type)
		this._refresh_pic_url();
}

/**
 * Setzt den Typ der Einheit. 
 * 
 * Gültige Werte sind 'club', 'blade', 'javelin' und 'bow'
 * 
 * @param String type
 */
PureFW.Items.ClothPreview.prototype.set_unit = function(type) {
	this.unit_type = type;
	if (this.item.type && this.item.id)
		this._refresh_pic_url();
}