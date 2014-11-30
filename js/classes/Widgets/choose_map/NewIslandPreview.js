/**
 * Dieses Widget repräsentiert die Vorschau der neuen Insel, wenn ein Spieler
 * eine neue Insel erstellen will und Leute drauf einlädt usw.
 * 
 * @see private_create_new_map.tmpl.inc
 * @package ChooseMap
 */
if (typeof(PureFW.ChooseMap) != 'object')
	PureFW.ChooseMap = new Object();

PureFW.ChooseMap.NewIslandPreview = function(parent_node, x, y, w, h, 
	template_id, no_scale, no_lvl)
{
	if (typeof(no_lvl) == 'undefined')
		no_lvl=false;
	if (typeof(parent_node) !== 'undefined') {
		this.init(parent_node, x, y, w, h, template_id, no_scale, no_lvl);
		
		this.instance_num = this.constructor.num_instances;
		this.instance_name = "PureFW.ChooseMap" +
				".NewIslandPreview.instances["	+this.instance_num+"]";
		
		this.id = "PureFW.ChooseMap.NewIslandPreview"+this.instance_num;
		this.content_id 
			= "PureFW.ChooseMap.NewIslandPreview_cont"+this.instance_num;
		this.bg_img_id = 'PureFW.ChooseMap.NewIslandPreview_bg'
			+this.instance_num;
		
		this.insert_into_dom_tree();
		this.constructor.num_instances++;
		this.constructor.instances[this.instance_num] = this;
		
		if (!this.no_scale)
			PureFW.WidgetManager.manager_all.register_widget(this);
	}
};

/********************************************************************
 * PureFW.ChooseMap.NewIslandPreview extends Container
 ********************************************************************/
PureFW.ChooseMap.NewIslandPreview.extend(PureFW.Container);

/********************************************************************
 * Statische Deklarationen
 ********************************************************************/
PureFW.ChooseMap.NewIslandPreview.num_instances = 0;		// Instanzüberwachung (anzahl)
PureFW.ChooseMap.NewIslandPreview.instances = new Array();// Instanzüberwachung (Instanzen)


/********************************************************************
 * Prototyp-Deklarationen
 ********************************************************************/
PureFW.ChooseMap.NewIslandPreview.prototype.init = function(parent_node, 
	x, y, w, h, template_id, no_scale, no_lvl)
{
	PureFW.ChooseMap.NewIslandPreview.parent.init.call(this, parent_node, 
		x, y, w, h, no_scale);

	this.template_id = template_id;
	this.picture = null;
	this.widget_label = null;
	this.xp_level = 0;
	this.xp_cont = null;
	this.xp_cont_inner = null;
	this.no_lvl = no_lvl || false;
};

PureFW.ChooseMap.NewIslandPreview.prototype.insert_into_dom_tree = function() 
{
	PureFW.ChooseMap.NewIslandPreview.parent.insert_into_dom_tree.call(this);
	this.set_overflow('visible');

	this.picture = this.add_widget(
		PureFW.Image,
		(this.width - 150) >> 1, (this.width - 150) >> 1,
		150, 150,
		((this.template_id) 
			? 'map/templates/'+this.template_id+'/_0_/template'+this.template_id
				+'_150_3d.jpg'
			: 'pattern/spacer.gif'),
		this.no_scale
	);
	
	this.polaroid_overlay = this.add_widget(
		PureFW.Image,
		0,0,
		this.width,this.height,
		'ui/backgrounds/choose_map/island_preview_overlay.png'
	);
	this.polaroid_overlay.set_z_index(1);
	
	this.widget_label = this.add_widget(
		PureFW.Container,
		10, this.picture.height + this.picture.position.y + 10,
		this.width - 20,
		this.height - this.picture.position.y - this.picture.height - 20,
		this.no_scale
	);
	this.widget_label.set_z_index(2);
	
	if(!this.no_lvl)
	{
		this.xp_cont = this.add_widget(
			PureFW.Container,
			-15, -20,
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
		this.xp_cont_inner.set_css_class('ChooseMap_NewIslandPreview_xp_cont_inner');
		this.xp_cont_inner.set_content(this.xp_level);
	}
};

/**
 * Setzt die ID des Templates, das angezeigt werden soll
 */
PureFW.ChooseMap.NewIslandPreview.prototype.set_template_id = function(id) {
	this.template_id = id;
	this.picture.set_pic_url(
		'map/templates/'+this.template_id+'/_0_/template'+this.template_id
		+'_150_3d.jpg'
	);
};

/**
 * Setzt den beschreibenden Text, der unter dem Bild so eingeblendet wird,
 * dass es als die Beschreibung des gesamten Widgets wahrgenommen wird.
 * 
 * @param String text
 */
PureFW.ChooseMap.NewIslandPreview.prototype.add_label_content = function(text) {
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
PureFW.ChooseMap.NewIslandPreview.prototype.add_widget_to_label = function(
	widget_constructor, x, y, w, h, arg6, arg7, arg8, arg9, arg10, arg11, 
	arg12, arg13)
{
	return this.widget_label.add_widget(widget_constructor, 
		x, y, w, h, arg6, arg7, arg8, arg9, arg10, arg11, arg12, arg13);
};

/**
 * Setzt die XP-Anzahl in Leveln, die mit dem Widget assoziiert werden soll.
 * 
 * @param uint xp
 */
PureFW.ChooseMap.NewIslandPreview.prototype.set_xp_level = function(xp) {
	this.xp_level = xp;
	if (!this.no_lvl)
		this.xp_cont_inner.set_content(xp);
};

PureFW.ChooseMap.NewIslandPreview.prototype.get_xp_level = function() {
	return this.xp_level;
};