/**
 * Repräsentiert die Anzeige der eigenen Brezeln oben in der Mitte
 */
if (typeof(PureFW.MapUI) != 'object')
	PureFW.MapUI = new Object();

/**
 * @param HTMLElement parent_node
 * @param int x
 * @param int y
 * @param uint w
 * @param uint h
 * @param bool no_scale=false
 */
PureFW.MapUI.BrezelAmt = function(parent_node, x, y, w, h, no_scale) {
	if (typeof(parent_node) !== 'undefined') {
		this.init(parent_node, x, y, w, h, no_scale);
	
		this.instance_num = this.constructor.num_instances;
		this.instance_name = 
			"PureFW.MapUI.BrezelAmt.instances["+this.instance_num+"]";
		
		this.id = "BrezelAmt"+this.instance_num;
		this.bg_img_id = "BrezelAmtBg"+this.instance_num;
		this.content_id = "BrezelAmtCont"+this.instance_num;
		
		this.insert_into_dom_tree();
		this.constructor.num_instances++;
		this.constructor.instances[this.instance_num] = this;
		
		if (!this.no_scale)
			PureFW.WidgetManager.manager_all.register_widget(this);
	}
};
/********************************************************************
* BrezelAmt extends Container
********************************************************************/
PureFW.MapUI.BrezelAmt.extend(PureFW.Container);

/********************************************************************
* Statische Deklarationen
********************************************************************/
PureFW.MapUI.BrezelAmt.num_instances = 0;
PureFW.MapUI.BrezelAmt.instances = new Array();

/********************************************************************
* Prototyp-Deklarationen
********************************************************************/
PureFW.MapUI.BrezelAmt.prototype.init = function(parent_node, x, y, w, h, 
	no_scale) 
{
	PureFW.MapUI.BrezelAmt.parent.init.call(this, parent_node, x, y, w, h, 
		no_scale);
	
	this.brezeln = 0;
	this.brezel_cont = null;
}

PureFW.MapUI.BrezelAmt.prototype.insert_into_dom_tree = function() {
	PureFW.MapUI.BrezelAmt.parent.insert_into_dom_tree.call(this);
	
	this.set_bg_img('ui/backgrounds/BrezelAmt/brezel.png');
	
	this.brezel_cont = this.add_widget(
		PureFW.Container,
		47, 15,
		46, 20
	);
	this.brezel_cont.set_css_class('BrezelAmt_cont');
}

/**
 * Setzt die Anzahl der Brezeln.
 * 
 * @param uint amt
 */
PureFW.MapUI.BrezelAmt.prototype.set_brezeln = function(amt) {
	this.brezeln = parseInt(amt);
	this.brezel_cont.set_content(this.brezeln);
}