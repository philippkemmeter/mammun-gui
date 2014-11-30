/**
 * Dieses Widget repräsentiert ein Polaroid-Bild mit Platz auf dem unteren
 * Weiß zum Beschriften.
 */

PureFW.PolaroidContainer = function(parent_node, x, y, w, h, no_scale)
{
	if (typeof(parent_node) !== 'undefined') {
		this.init(parent_node, x, y, w, h, no_scale);
		
		this.instance_num = this.constructor.num_instances;
		this.instance_name = "PureFW.PolaroidContainer" +
				".instances["	+this.instance_num+"]";
		
		this.id = "PureFW.PolaroidContainer"+this.instance_num;
		this.content_id 
			= "PureFW.PolaroidContainer_cont"+this.instance_num;
		this.bg_img_id = 'PureFW.PolaroidContainer_bg'
			+this.instance_num;
		
		this.insert_into_dom_tree();
		this.constructor.num_instances++;
		this.constructor.instances[this.instance_num] = this;
		
		if (!this.no_scale)
			PureFW.WidgetManager.manager_all.register_widget(this);
	}
};

/********************************************************************
 * PureFW.PolaroidContainer extends Container
 ********************************************************************/
PureFW.PolaroidContainer.extend(PureFW.Container);

/********************************************************************
 * Statische Deklarationen
 ********************************************************************/
PureFW.PolaroidContainer.num_instances = 0;		// Instanzüberwachung (anzahl)
PureFW.PolaroidContainer.instances = new Array();// Instanzüberwachung (Instanzen)


/********************************************************************
 * Prototyp-Deklarationen
 ********************************************************************/
PureFW.PolaroidContainer.prototype.init = function(parent_node, 
	x, y, w, h, no_scale)
{
	PureFW.PolaroidContainer.parent.init.call(this, parent_node, 
		x, y, w, h, no_scale);

	this.picture = null;
	this.widget_label = null;
};

PureFW.PolaroidContainer.prototype.insert_into_dom_tree = function() 
{
	PureFW.PolaroidContainer.parent.insert_into_dom_tree.call(this);

	
	this.set_bg_img(
		'ui/backgrounds/PolaroidContainer/paper_'+this.width+'x'+this.height
			+ '.png'
	);
	
	this.picture = this.add_widget(
		PureFW.Image,
		(this.width - 150) >> 1, (this.width - 150) >> 1,
		150, 150,
		'pattern/spacer.gif',
		this.no_scale
	);
	
	this.widget_label = this.add_widget(
		PureFW.Container,
		10, this.picture.height + this.picture.position.y + 10,
		this.width - 20,
		this.height - this.picture.position.y - this.picture.height - 20,
		this.no_scale
	);
	this.widget_label.set_css_class('PolaroidContainer_widget_label');
};


/**
 * Setzt die URL für das Bild, das angezeigt werden soll
 */
PureFW.PolaroidContainer.prototype.set_pic_url = function(pic) {
	this.picture.set_pic_url(pic);
};

/**
 * Setzt den beschreibenden Text, der unter dem Bild so eingeblendet wird,
 * dass es als die Beschreibung des gesamten Widgets wahrgenommen wird.
 * 
 * @param String text
 */
PureFW.PolaroidContainer.prototype.add_label_content = function(text) {
	this.widget_label.add_content(text);
};

/**
 * Fügt ein neues durch den übergebenen Konstruktor definiertes Widget dem 
 * Label hinzu. Die Argumente werden entsprechend dem Widget weitergereicht.
 * 
 * Gibt das neue Widget zurück.
 * 
 * @param Constructor widget_constructor
 * @param uint x
 * @param uint y
 * @param uint w
 * @param uint h
 * @param mixed argN
 * @return PureFW.Widget
 * 
 * @see PureFW.Container#add_widget
 */
PureFW.PolaroidContainer.prototype.add_widget_to_label = function(
	widget_constructor, x, y, w, h, arg6, arg7, arg8, arg9, arg10, arg11, 
	arg12, arg13)
{
	return this.widget_label.add_widget(widget_constructor, 
		x, y, w, h, arg6, arg7, arg8, arg9, arg10, arg11, arg12, arg13);
};