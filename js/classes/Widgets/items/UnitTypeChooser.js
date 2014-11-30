/**
 * Dieses Widget ist zum Auswählen eine Einheitenart gedacht.
 */

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
PureFW.Items.UnitTypeChooser = function(parent_node, x, y, w, h, no_scale)
{
	if (typeof(parent_node) !== 'undefined') {
		this.init(parent_node, x, y, w, h, no_scale);
		
		this.instance_num = this.constructor.num_instances;
		this.instance_name = "MammunUI.get_top_frame().PureFW.Items" +
				".UnitTypeChooser.instances["	+this.instance_num+"]";
		
		this.id = "PureFW.Items.UnitTypeChooser"+this.instance_num;
		this.content_id 
			= "PureFW.Items.UnitTypeChooser_cont"+this.instance_num;
		this.bg_img_id = 'PureFW.Items.UnitTypeChooser_bg'+this.instance_num;
		
		this.insert_into_dom_tree();
		this.constructor.num_instances++;
		this.constructor.instances[this.instance_num] = this;
		
		if (!this.no_scale)
			PureFW.WidgetManager.manager_all.register_widget(this);
	}
};
/********************************************************************
 * PureFW.Items.UnitTypeChooser extends Container
 ********************************************************************/
PureFW.Items.UnitTypeChooser.extend(PureFW.Container);

/********************************************************************
 * Statische Deklarationen
 ********************************************************************/
PureFW.Items.UnitTypeChooser.num_instances = 0;		// Instanzüberwachung (anzahl)
PureFW.Items.UnitTypeChooser.instances = new Array();	// Instanzüberwachung (Instanzen)



/********************************************************************
 * Prototyp-Deklarationen
 ********************************************************************/
PureFW.Items.UnitTypeChooser.prototype.init = function(parent_node, x, y, w, h,
	no_scale)
{
	PureFW.Items.UnitTypeChooser.parent.init.call(this, parent_node, x, y, w, h,
		no_scale);

	this.unit_types = ['club'];
	this.unit_type_chosen = 'club';
	
	this.icons = new Object();
	
	this.caption_cont = null;
	this.main_cont = null;
	this.on_select_functions = new Array();
};

PureFW.Items.UnitTypeChooser.prototype.insert_into_dom_tree = function() {
	PureFW.Items.UnitTypeChooser.parent.insert_into_dom_tree.call(this);
	
	this.set_bg_img('ui/backgrounds/Items/UnitTypeChooser/bg_'+this.width+'x'
		+this.height+'.png');
	this.set_overflow('visible');
	
	this.caption_cont = this.add_widget(
		PureFW.Container,
		0, 0,
		this.width,
		29
	);
	this.main_cont = this.add_widget(
		PureFW.Container,
		0, 29,
		this.width,
		this.height - 29
	);
}

/**
 * Erstellt die Icons, die die Einheiten zur Wahl darstellen
 */
PureFW.Items.UnitTypeChooser.prototype._create_choices = function() {
	var n = this.unit_types.length;

	for (var i = 0; i < n; i++) {
		if( (this.unit_types[i] != "club") 	&&
			(this.unit_types[i] != "blade") 	&&
			(this.unit_types[i] != "bow") 	&&
			(this.unit_types[i] != "javelin"))
		{
			continue;
		}

		this.icons[this.unit_types[i]] = this.main_cont.add_widget(
			PureFW.Image,
			20, 12,
			43, 59,
			'ui/icons/labels/items/UnitTypeChooser/'+this.unit_types[i]+'.png'
		);
		this.icons[this.unit_types[i]].set_positioning('relative');
		this.icons[this.unit_types[i]].set_float('left');

		this.icons[this.unit_types[i]].add_event_handler(
			"click",
			(function(_unit_type, _instance) {
				return function(ev) {
					for (var x in _instance.icons) {
						_instance.icons[x].set_width(43);
						_instance.icons[x].set_height(59);
						_instance.icons[x].set_y(12);
					}
					_instance.icons[_unit_type].set_width(55);
					_instance.icons[_unit_type].set_height(76);
					_instance.icons[_unit_type].set_y(5);
					
					_instance.on_select(
						_instance.create_event("select",_unit_type)
					);
				}
			})(this.unit_types[i], this)
		);
		/**
		 * Erstes Icon anklicken
		 */
		if (i == 0)
			this.icons[this.unit_types[i]].on_click();
	}
}

/**
 * Setzt den Typ der Einheit. 
 * 
 * Gültige Werte sind 'club', 'blade', 'javelin' und 'bow'
 * 
 * @param String[] type
 */
PureFW.Items.UnitTypeChooser.prototype.set_unit_types = function(types) {
	this.unit_types = types;
	this._create_choices();
}

/**
 * Setzt die Überschrift auf den angegebenen Wert.
 * 
 * @param String text
 */
PureFW.Items.UnitTypeChooser.prototype.set_caption = function(text) {
	this.caption_cont.set_content(text);
}

/**
 * Fügt das angegebene Widget der Überschrift hinzu.
 * 
 * @param Constructor widget_constructor
 * @param uint x
 * @param uint y
 * @param uint w
 * @param uint h
 * @param mixed argN
 * @return PureFW.Widget
 */
PureFW.Items.UnitTypeChooser.prototype.add_widget_to_caption = function(
	widget_constructor,
	x, y, w, h, arg6, arg7, arg8, arg9, arg10, arg11, arg12, arg13) 
{
	return this.caption_cont.add_widget(widget_constructor,
		x, y, w, h, arg6, arg7, arg8, arg9, arg10, arg11, arg12, arg13
	);
}

/**
 * Fügt einen EventHandler hinzu. Funktion überschrieben, da zusätzlich
 * der Typ "select" unterstützt wird, welcher beim Auswählen eines Typs
 * ausgelöst wird.
 * 
 * @param string type
 * @param Function fn
 * @see Widget.add_event_handler
 */
PureFW.Items.UnitTypeChooser.prototype.add_event_handler = function(type, fn) {
	if (type === "select")
		this.on_select_functions.push(fn);
	else
		PureFW.Items.UnitTypeChooser.parent.add_event_handler.call(
			this, type, fn);
};

/**
 * Entfernt einen EventHandler. Funktion überschrieben, da zusätzlich
 * der Typ "select" unterstützt wird, welcher beim Auswählen eines Typs
 * ausgelöst wird.
 * 
 * @param string type
 * @param Function fn
 * @see Widget.remove_event_handler
 */
PureFW.Items.UnitTypeChooser.prototype.remove_event_handler = function(type, fn) 
{
	if (type === "select")
		this.on_select_functions.remove(fn);
	else
		PureFW.Items.UnitTypeChooser.parent.remove_event_handler.call(
			this, type, fn);
};

/**
 * Ruft das "on-select"-Event exlizit auf. Wird normalerweise aufgerufen,
 * wenn ein Typ ausgewählt wurde.
 * 
 * @param Event ev [optional]
 */
PureFW.Items.UnitTypeChooser.prototype.on_select = function(ev) {
	if (!ev) {
		ev = this.create_event("select");
	}
	var n = this.on_select_functions.length;
	for (var i = 0; i < n; i++) {
		this.on_select_functions[i].call(this, ev);
	}
};



/*	MammunShop.item_details.try_on_caption =
		MammunShop.details_container.add_widget
	(
		PureFW.Image,
		MammunShop.item_details.details_chooser.position.x + 16,
		MammunShop.item_details.details_chooser.position.y - 35,
		203, 17,
		'ui/elements/texts/<?= $LNG ?>/shop/txt_einfach_mal_anprobieren_203x17.png'
	);*/