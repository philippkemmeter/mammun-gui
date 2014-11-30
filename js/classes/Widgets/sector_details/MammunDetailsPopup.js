/**
 * Detailansicht eines Mammuns inklusive Aktivitäten, Attribute und
 * Fähigkeiten
 */


if (typeof(PureFW.SectorDetails) != 'object')
	PureFW.SectorDetails = new Object();

/**
 * @param Node parent_node	An welchen Knoten das Popup angehängt werden soll
 * @param int x				Wo sich das Popup befinden soll in x-Richtung in px
 * @param int y				Wo sich das Popup befinden soll in y-Richtung in px
 * @param int w				Breite des Popups.
 * @param int h				Höhe des Popups.
 * @param bool mammun_obj	Die Daten des mammuns, der Dargestellt werden soll
 * @param bool no_scale		Nicht sklierbar (default: false)
 */
PureFW.SectorDetails.MammunDetailsPopup = function(parent_node, x, y, w, h, 
	mammun_obj, no_scale)
{
	if (typeof(parent_node) !== 'undefined') {
		this.init(parent_node, x, y, w, h, mammun_obj, no_scale);
		
		this.instance_num = this.constructor.num_instances;
		this.instance_name = "MammunUI.get_top_frame().PureFW.SectorDetails" +
				".MammunDetailsPopup.instances["	+this.instance_num+"]";
		
		this.id = "PureFW.SectorDetails.MammunDetailsPopup"+this.instance_num;
		this.content_id 
			= "PureFW.SectorDetails.MammunDetailsPopup_cont"+this.instance_num;
		this.bg_img_id 
			= 'PureFW.SectorDetails.MammunDetailsPopup_bg'+this.instance_num;
		
		this.insert_into_dom_tree();
		this.constructor.num_instances++;
		this.constructor.instances[this.instance_num] = this;
		
		if (!this.no_scale)
			PureFW.WidgetManager.manager_all.register_widget(this);
	}
};
/********************************************************************
 * PureFW.SectorDetails.MammunDetailsPopup extends Container
 ********************************************************************/
PureFW.SectorDetails.MammunDetailsPopup.extend(PureFW.Container);

/********************************************************************
 * Statische Deklarationen
 ********************************************************************/
/**
 * Erzeugt ein MammunDetailsPopup zentriert zum Vaterknoten und bestimmt
 * Breite und Höhe selbstständig.
 */
PureFW.SectorDetails.MammunDetailsPopup.create_centered = function(
	parent_node, mammun_obj, no_scale) 
{
	
}

PureFW.SectorDetails.MammunDetailsPopup.num_instances = 0;		// Instanzüberwachung (anzahl)
PureFW.SectorDetails.MammunDetailsPopup.instances = new Array();	// Instanzüberwachung (Instanzen)



/********************************************************************
 * Prototyp-Deklarationen
 ********************************************************************/
PureFW.SectorDetails.MammunDetailsPopup.prototype.init = function(parent_node, 
	x, y, w, h, mammun_obj, no_scale)
{
	PureFW.SectorDetails.MammunDetailsPopup.parent.init.call(this, parent_node,
		x, y, w, h,	no_scale);

	this.mammun_obj = mammun_obj;
	this.name_container = null;
	this.pic_img = null;
	this.speech_text_container = null;
};

PureFW.SectorDetails.MammunDetailsPopup.prototype.insert_into_dom_tree 
	= function() 
{
	PureFW.SectorDetails.MammunDetailsPopup.parent.insert_into_dom_tree.call(
		this
	);
	
	this.name_container = this.add_widget(
		PureFW.Container,
		30, 25,
		0, 0
	);
	this.name_container.set_content(mammun_obj.name);
	var pic = MapObjects.Mammun.get_mammun_by_type_and_sex(mammun_obj.race)
					.pic_base;
	
	var n = mammun_obj.clothes.length;
	for (var i = 0; i < n; i++) {
		pic += '_';
		pic += this.clothes[i];
	}
	pic += '.png'; 
	this.pic_img = this.add_widget(
		PureFW.Image,
		10, 45,
		100, 175,
		pic
	);
	
	this.speech_text_container = this.add_widget(
		PureFW.Container,
		140, 50,
		this.width - 140 - 5,
		0
	);
	
	var health_icon = this.add_widget(
		PureFW.Image,
		this.pic_img.position.x + this.pic_img.width + 10,
		140,
		40, 40,
		'ui/icons/labels/attributes/40_dark/health.png'
	);
	
	/**TODO*/
//	this.health = this.add_widget(
//		
//	);
	
	
};

/**
 * Setzt den anzuzeigenden Namen des Mammuns.
 * 
 * @param String str
 */
PureFW.SectorDetails.MammunDetailsPopup.prototype.set_name = function(str) {
	this.name_container.set_content(str);
};

/**
 * Setzt den anzuzeigenden Spruch des Mammuns.
 * 
 * @param String str
 */
PureFW.SectorDetails.MammunDetailsPopup.prototype.set_speech = function(str) {
	this.speech_container.set_content('"'+str+'"');
};

/**
 * Setzt die aktuellen und maximalen Lebenspunkte.
 * 
 * @param uint hp
 * @param uint max_hp
 */
PureFW.SectorDetails.MammunDetailsPopup.prototype.set_health = function(
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
PureFW.SectorDetails.MammunDetailsPopup.prototype.set_recloth_tooltip 
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
PureFW.SectorDetails.MammunDetailsPopup.prototype.set_move_tooltip 
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
PureFW.SectorDetails.MammunDetailsPopup.prototype.set_details_tooltip 
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
PureFW.SectorDetails.MammunDetailsPopup.prototype.add_event_handler = function(
	type, fn) 
{
	if (type.substr(0, 8) == "details_")
		this.details_button.add_event_handler(type.substr(8), fn);
	else if (type.substr(0,5) === "move_")
		this.move_button.add_event_handler(type.substr(5), fn);
	else if (type.substr(0,8) == "recloth_")
		this.recloth_button.add_event_handler(type.substr(8), fn);
	else
		PureFW.SectorDetails.MammunDetailsPopup.parent.add_event_handler.call(
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
PureFW.SectorDetails.MammunDetailsPopup.remove_event_handler = function(
	type, fn) 
{
	if (type.substr(0, 8) == "details_")
		this.details_button.remove_event_handler(type.substr(8), fn);
	else if (type.substr(0,5) === "move_")
		this.move_button.remove_event_handler(type.substr(5), fn);
	else if (type.substr(0,8) == "recloth_")
		this.recloth_button.remove_event_handler(type.substr(8), fn);
	else
		PureFW.SectorDetails.MammunDetailsPopup.parent.remove_event_handler
			.call
		(
			this, type, fn
		);
};