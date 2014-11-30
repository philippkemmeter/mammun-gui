/**
 * Sprechblase eine (angeklickten) Mammuns mit Befehlicons.
 */



if (typeof(PureFW.SectorDetails) != 'object')
	PureFW.SectorDetails = new Object();

/**
 * @param Node parent_node	An welchen Knoten die Map angehängt werden soll
 * @param int x				Wo sich die Map befinden soll in x-Richtung in px
 * @param int y				Wo sich die Map befinden soll in y-Richtung in px
 * @param int w				Wie breit die gesamte Map sein soll
 * @param int h				Wie hoch die gesamte Map sein soll
 * @param bool my_mammun	Ob es mein eigener Mammun ist, oder ein anderer
 * @param bool no_scale		Nicht sklierbar (default: false)
 */
PureFW.SectorDetails.MammunSpeechBubble = function(parent_node, x, y, w, h, 
		my_mammun, no_scale)
{
	if (typeof(parent_node) !== 'undefined') {
		this.init(parent_node, x, y, w, h, my_mammun, no_scale);
		
		this.instance_num = this.constructor.num_instances;
		this.instance_name = "MammunUI.get_top_frame().PureFW.SectorDetails" +
				".MammunSpeechBubble.instances["	+this.instance_num+"]";
		
		this.id = "PureFW.SectorDetails.MammunSpeechBubble"+this.instance_num;
		this.content_id 
			= "PureFW.SectorDetails.MammunSpeechBubble_cont"+this.instance_num;
		this.bg_img_id 
			= 'PureFW.SectorDetails.MammunSpeechBubble_bg'+this.instance_num;
		
		this.insert_into_dom_tree();
		this.constructor.num_instances++;
		this.constructor.instances[this.instance_num] = this;
		
		if (!this.no_scale)
			PureFW.WidgetManager.manager_all.register_widget(this);
	}
};
/********************************************************************
 * PureFW.SectorDetails.MammunSpeechBubble extends Container
 ********************************************************************/
PureFW.SectorDetails.MammunSpeechBubble.extend(PureFW.Container);

/********************************************************************
 * Statische Deklarationen
 ********************************************************************/
PureFW.SectorDetails.MammunSpeechBubble.num_instances = 0;		// Instanzüberwachung (anzahl)
PureFW.SectorDetails.MammunSpeechBubble.instances = new Array();	// Instanzüberwachung (Instanzen)



/********************************************************************
 * Prototyp-Deklarationen
 ********************************************************************/
PureFW.SectorDetails.MammunSpeechBubble.prototype.init = function(parent_node, 
	x, y, w, h, my_mammun, no_scale)
{
	PureFW.SectorDetails.MammunSpeechBubble.parent.init.call(this, parent_node,
		x, y, w, h,	no_scale);

	this.my_mammun = my_mammun;
	this.name_container = null;
	this.speech_container = null;
	this.health_img = null;
	this.health_cont = null;
	this.details_button = null;
	this.recloth_button = null;
	this.move_button = null;
};

PureFW.SectorDetails.MammunSpeechBubble.prototype.insert_into_dom_tree 
	= function() 
{
	PureFW.SectorDetails.MammunSpeechBubble.parent.insert_into_dom_tree.call(
		this
	);
	this.set_bg_img(
		'ui/backgrounds/info_balloon/info_balloon_single_'+this.width+'x'
			+ this.height+'.png'
	);
	this.set_css_class("MammunSpeechBubble_cont");
	
	var close_button = this.add_widget(
		PureFW.Image,
		this.width - 28, 0,
		28, 28,
		'ui/elements/form/buttons/btn_no.png'
	);
	
	close_button.add_event_handler("click",
		(function(_instance) {
			return function(ev) {
				_instance.destroy();
			}
		})(this)
	);
	close_button.set_z_index(2);
	
	this.name_container = this.add_widget(
		PureFW.Container,
		13, 15,
		this.width - 20, 20
	);
	this.name_container.set_css_class("MammunSpeechBubble_name");
	
	this.speech_container = this.add_widget(
		PureFW.Container,
		this.name_container.position.x,
		this.name_container.position.y + this.name_container.height + 5,
		this.width - 20, 40
	);
	this.speech_container.set_css_class("MammunSpeechBubble_speech");
	
	if (this.my_mammun) {
		this.health_img = this.add_widget(
			PureFW.Image,
			50, 91,
			26, 26,
			'ui/icons/labels/attributes/26/health.png'
		);
		this.health_cont = this.add_widget(
			PureFW.Container,
			77, 91,
			66, 20
		);
		
		this.details_button = this.add_widget(
			PureFW.Image,
			((this.width - 32) >> 1) + 5, 125,
			32, 32,
			'ui/icons/labels/attributes/details.png'
		);
		
		this.move_button = this.add_widget(
			PureFW.Image,
			this.details_button.position.x - 32 - 15, 
			this.details_button.position.y,
			32, 32,
			'ui/icons/labels/unit_orders/move.png'
		);
		
		this.recloth_button = this.add_widget(
			PureFW.Image,
			this.details_button.position.x + 
				this.details_button.width + 20,
			this.details_button.position.y - 5,
			22, 41,
			'ui/icons/labels/unit_orders/change_clothes.png'
		);
	}
	else {
		this.details_button = this.add_widget(
			PureFW.Image,
			(this.width - 32) >> 1, 110,
			32, 32,
			'ui/icons/labels/attributes/details.png'
		);
	}
};

/**
 * Setzt den anzuzeigenden Namen des Mammuns.
 * 
 * @param String str
 */
PureFW.SectorDetails.MammunSpeechBubble.prototype.set_name = function(str) {
	this.name_container.set_content(str);
};

/**
 * Setzt den anzuzeigenden Spruch des Mammuns.
 * 
 * @param String str
 */
PureFW.SectorDetails.MammunSpeechBubble.prototype.set_speech = function(str) {
	this.speech_container.set_content('"'+str+'"');
};

/**
 * Setzt die aktuellen und maximalen Lebenspunkte.
 * 
 * @param uint hp
 * @param uint max_hp
 */
PureFW.SectorDetails.MammunSpeechBubble.prototype.set_health = function(
	hp, max_hp)
{
	hp = parseInt(hp); max_hp = parseInt(max_hp);
	this.health_cont.set_content(hp + "/" + max_hp);
}

/**
 * Setzt den Tooltip des Recloth-Buttons
 * 
 * @param String str
 */
PureFW.SectorDetails.MammunSpeechBubble.prototype.set_recloth_tooltip 
	= function(str)
{
	if (this.recloth_button)
		this.recloth_button.set_tooltip(str);
};

/**
 * Setzt den Tooltip des Move-Buttons
 * 
 * @param String str
 */
PureFW.SectorDetails.MammunSpeechBubble.prototype.set_move_tooltip 
	= function(str)
{
	if (this.move_button)
		this.move_button.set_tooltip(str);
};

/**
 * Setzt den Tooltip des Details-Buttons
 * 
 * @param String str
 */
PureFW.SectorDetails.MammunSpeechBubble.prototype.set_details_tooltip 
	= function(str)
{
	if (this.details_button)
		this.details_button.set_tooltip(str);
};

/**
 * Fügt einen EventHandler hinzu. Funktion überschrieben, da zusätzlich
 * wrapper für die einzelnen Buttons existieren. Präfixe sind "details_",
 * "move_" und "recloth_"
 * 
 * @param string type
 * @param Function fn
 * @see Widget#add_event_handler
 */
PureFW.SectorDetails.MammunSpeechBubble.prototype.add_event_handler = function(
	type, fn) 
{
	if (type.substr(0, 8) == "details_")
		this.details_button.add_event_handler(type.substr(8), fn);
	else if (type.substr(0,5) === "move_")
		this.move_button.add_event_handler(type.substr(5), fn);
	else if (type.substr(0,8) == "recloth_")
		this.recloth_button.add_event_handler(type.substr(8), fn);
	else
		PureFW.SectorDetails.MammunSpeechBubble.parent.add_event_handler.call(
			this, type, fn
		);
};

/**
 * Entfernt einen EventHandler. Funktion überschrieben, da zusätzlich
 * wrapper für die einzelnen Buttons existieren. Präfixe sind "details_",
 * "move_" und "recloth_"
 * 
 * @param string type
 * @param Function fn
 * @see Widget#remove_event_handler
 */
PureFW.SectorDetails.MammunSpeechBubble.remove_event_handler = function(
	type, fn) 
{
	if (type.substr(0, 8) == "details_")
		this.details_button.remove_event_handler(type.substr(8), fn);
	else if (type.substr(0,5) === "move_")
		this.move_button.remove_event_handler(type.substr(5), fn);
	else if (type.substr(0,8) == "recloth_")
		this.recloth_button.remove_event_handler(type.substr(8), fn);
	else
		PureFW.SectorDetails.MammunSpeechBubble.parent.remove_event_handler
			.call
		(
			this, type, fn
		);
};