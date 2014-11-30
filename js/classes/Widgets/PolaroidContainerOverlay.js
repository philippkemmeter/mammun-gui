/**
 * Dieses Widget ist eine Erweiterung des PolaroidContainers. Hier können nun
 * auch Über das Bild zusätzliche Beschriftungen angebracht werden.
 */

PureFW.PolaroidContainerOverlay = function(parent_node, x, y, w, h, no_scale)
{
	if (typeof(parent_node) !== 'undefined') {
		this.init(parent_node, x, y, w, h, no_scale);
		
		this.instance_num = this.constructor.num_instances;
		this.instance_name = "PureFW.PolaroidContainerOverlay" +
				".instances["	+this.instance_num+"]";
		
		this.id = "PureFW.PolaroidContainerOverlay"+this.instance_num;
		this.content_id 
			= "PureFW.PolaroidContainerOverlay_cont"+this.instance_num;
		this.bg_img_id = 'PureFW.PolaroidContainerOverlay_bg'
			+this.instance_num;
		
		this.insert_into_dom_tree();
		this.constructor.num_instances++;
		this.constructor.instances[this.instance_num] = this;
		
		if (!this.no_scale)
			PureFW.WidgetManager.manager_all.register_widget(this);
	}
};

/********************************************************************
 * PureFW.PolaroidContainerOverlay extends Container
 ********************************************************************/
PureFW.PolaroidContainerOverlay.extend(PureFW.PolaroidContainer);

/********************************************************************
 * Statische Deklarationen
 ********************************************************************/
PureFW.PolaroidContainerOverlay.num_instances = 0;		// Instanzüberwachung (anzahl)
PureFW.PolaroidContainerOverlay.instances = new Array();// Instanzüberwachung (Instanzen)


/********************************************************************
 * Prototyp-Deklarationen
 ********************************************************************/
PureFW.PolaroidContainerOverlay.prototype.init = function(parent_node, 
	x, y, w, h, no_scale)
{
	PureFW.PolaroidContainerOverlay.parent.init.call(this, parent_node, 
		x, y, w, h, no_scale);

	this.overlay = null;
};

PureFW.PolaroidContainerOverlay.prototype.insert_into_dom_tree = function() 
{
	PureFW.PolaroidContainerOverlay.parent.insert_into_dom_tree.call(this);

	this.overlay = this.add_widget(
		PureFW.Container,
		this.picture.position.x,
		this.picture.position.y + (this.picture.height - 48),
		this.picture.width,
		48,
		this.no_scale
	);
	this.overlay.set_z_index(3);
};

/**
 * Setzt die Höhe des Overlays.
 * 
 * @param uint h
 */
PureFW.PolaroidContainerOverlay.prototype.set_overlay_height = function(h) {
	this.overlay.set_height(h);
	this.overlay.set_y(this.picture.position.y + (this.picture.height - h));
};

PureFW.PolaroidContainerOverlay.prototype.add_overlay_content = function(s) {
	this.overlay.add_content(s);
};

PureFW.PolaroidContainerOverlay.prototype.set_overlay_bg_img = function(s) {
	this.overlay.set_bg_img(s);
}

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
	return this.overlay.add_widget(
		widget_constructor, x, y, w, h, arg6, arg7, arg8, arg9, arg10, arg11, 
		arg12, arg13);
}