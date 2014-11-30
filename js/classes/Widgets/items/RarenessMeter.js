/**
 * Dieses Widget stellt den Balken zur Repräsentierung des Seltenheitsfaktors
 * dar.
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
PureFW.Items.RarenessMeter = function(parent_node, x, y, w, h, condensed, 
	no_scale)
{
	if (typeof(parent_node) !== 'undefined') {
		this.init(parent_node, x, y, w, h, condensed, no_scale);
		
		this.instance_num = this.constructor.num_instances;
		this.instance_name = "MammunUI.get_top_frame().PureFW.Items" +
				".RarenessMeter.instances["	+this.instance_num+"]";
		
		this.id = "PureFW.Items.RarenessMeter"+this.instance_num;
		this.content_id 
			= "PureFW.Items.RarenessMeter_cont"+this.instance_num;
		this.bg_img_id = 'PureFW.Items.RarenessMeter_bg'+this.instance_num;
		
		this.insert_into_dom_tree();
		this.constructor.num_instances++;
		this.constructor.instances[this.instance_num] = this;
		
		if (!this.no_scale)
			PureFW.WidgetManager.manager_all.register_widget(this);
	}
};
/********************************************************************
 * PureFW.Items.RarenessMeter extends Container
 ********************************************************************/
PureFW.Items.RarenessMeter.extend(PureFW.Container);

/********************************************************************
 * Statische Deklarationen
 ********************************************************************/
PureFW.Items.RarenessMeter.num_instances = 0;		// Instanzüberwachung (anzahl)
PureFW.Items.RarenessMeter.instances = new Array();	// Instanzüberwachung (Instanzen)



/********************************************************************
 * Prototyp-Deklarationen
 ********************************************************************/
PureFW.Items.RarenessMeter.prototype.init = function(parent_node, x, y, w, h,
	condensed, no_scale)
{
	PureFW.Items.RarenessMeter.parent.init.call(this, parent_node, x, y, w, h,
		no_scale);
	
	this.condensed = condensed || false;
	
	this.rareness_value = 0;
	this.rareness_max = 4;
	
	this.arrow_cont = null;
	this.arrow_cont_inner = null;
};

PureFW.Items.RarenessMeter.prototype.insert_into_dom_tree = function() {
	PureFW.Items.RarenessMeter.parent.insert_into_dom_tree.call(this);
	
	this.set_bg_img('ui/backgrounds/Items/RarenessMeter/bg_'+this.width+'x'
		+this.height+'.png');
	this.set_overflow('visible');
	
	/**
	 * Komprimierte Version hat nur einen Pfeil, der auf der Skala auf die
	 * entsprechende Stelle zeigt.
	 */
	if (this.condensed) {
		this.arrow_cont = this.add_widget(
			PureFW.Image,
			0, 0,
			0, 0,
			''		// TODO
		);
	}
	/**
	 * Unkomprimierte Version dagegen hat 
	 */
	else {
		this.arrow_cont = this.add_widget(
			PureFW.Container,
			this.width >> 1, 0,
			52, 32
		);
		this.arrow_cont.set_bg_img(
			'ui/backgrounds/Items/RarenessMeter/arrow_bg_big.png'
		);
		this.arrow_cont_inner = this.arrow_cont.add_widget(
			PureFW.Container,
			18, 6,
			27, 20
		);
		this.arrow_cont_inner.set_text_align('center');
		this.arrow_cont_inner.set_content(this.rareness_value);
	}
	
	this._update_arrow_pos();
};

/**
 * Diese Funktion berechnet die Position des Pfeiles zur Anzeige des Wertes
 * auf der Skala und setzt den Pfeil entsprechend.
 */
PureFW.Items.RarenessMeter.prototype._update_arrow_pos = function() {
	var y = this.height 
				- (this.rareness_value * this.height / (this.rareness_max+1))
				- (this.arrow_cont.height >> 1) - 2;	// zentrieren
	this.arrow_cont.set_y(y);
};

/**
 * Setzt den Seltenheitswert, der auf der Skala angezeigt werden soll.
 * 
 * @param ufloat v
 */
PureFW.Items.RarenessMeter.prototype.set_value = function(v) {
	this.rareness_value = Math.round(v*10)/10;	// 1 Nachkommastelle
	this._update_arrow_pos();
	if (this.arrow_cont_inner) {
		var rareness_value_str = 
			(parseInt(this.rareness_value) == this.rareness_value)
				? this.rareness_value + '.0' : this.rareness_value;
		this.arrow_cont_inner.set_content(rareness_value_str);
	}
};