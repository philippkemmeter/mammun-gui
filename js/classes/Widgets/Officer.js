/**
 * Repräsentiert einen Berater mit Sprechblase. Das Beraterbild kann ausgetauscht,
 * der Sprechblasen Inhalt angegeben werden. Das gesamte Widget kann ein- und
 * ausgeblendet werden usw...
 * 
 * @author Phil
 */


/**
 * Erzeugt einen Officer.
 * @param officer	Muss ein Attribut von Officer.officers sein
 */
PureFW.Officer = function(parent_node, officer)
{
	if (typeof(parent_node) !== 'undefined') 
	{
		this.init(parent_node,0,0,0,0);
		
		this.officer = officer || this.constructor.OFFICERS.councilor;
		
		this.instance_num = this.constructor.num_instances;
		this.instance_name = "PureFW.Officer.instances["+this.instance_num+"]";

		this.officer_container = null;
		this.officer_url = this.officer.pic_left;
		this.officer_pos = new PureFW.Point(0,0);
		this.officer_width = this.officer.width;
		this.officer_height = this.officer.height;
		this.officer_id = 'Officer_pic' + this.instance_num;
		
		this.balloon_container = null;
		this.balloon_url = this.officer.balloon.pic_right;
		this.balloon_pos = new PureFW.Point(0,0);
		this.balloon_id = 'Officer_balloon' + this.instance_num;
		
		this.balloon_width = 668;
		this.balloon_height = 195;
		
		this.constructor.active_officer_stack.push(this);
		
		this.insert_into_dom_tree();
		this.constructor.num_instances++;
		this.constructor.instances[this.instance_num] = this;
		
		PureFW.WidgetManager.manager_all.register_widget(this);
	}
	
};

/********************************************************************
 * Officer extends Widget
 ********************************************************************/
PureFW.Officer.extend(PureFW.Widget);

/********************************************************************
 * Statische Deklarationen
 ********************************************************************/
/** 
 * Das ist die statische Definition der verschiedenen Offiziersbilder und
 * allem, was daran gekoppelt ist. Ein solches Element der Officer.OFFICERS
 * muss der Officer-Klasse übergeben werden, damit diese weiß, welchen Officer
 * sie darstellen soll
 */
PureFW.Officer.OFFICERS = {
	salesman:{// Klunker Verkaeufer
		pic_left:pic_path('ui/officers/salesman/salesman_bernard_left.png'),
		pic_right:pic_path('ui/officers/salesman/salesman_bernard_right.png'),
		width:118,
		height:350,
		balloon:{
			pic_left:pic_path('ui/officers/balloon/balloon_left.png'),
			pic_right:pic_path('ui/officers/balloon/balloon_right.png')
		}
	},	
	councilor:{// Berater
		pic_left:pic_path('ui/officers/councilor/bernard_left.png'),
		pic_right:pic_path('ui/officers/councilor/bernard_right.png'),
		width:134,
		height:350,
		balloon:{
			pic_left:pic_path('ui/officers/balloon/balloon_left.png'),
			pic_right:pic_path('ui/officers/balloon/balloon_right.png')
		}
	},
	councilor_small:{// Berater in klein
		pic_left:pic_path('ui/officers/councilor/bernard_left_103x266.png'),
		width:103,
		height:266,
		balloon:{
			pic_left:pic_path('ui/officers/balloon/balloon_left.png'),
			pic_right:pic_path('ui/officers/balloon/balloon_right.png')
		}
	},
	cwo:{	// Training
		pic_left:pic_path('ui/officers/cwo/grandpa_left.png'),
		pic_right:pic_path('ui/officers/cwo/grandpa_right.png'),
		width:194,
		height:350,
		balloon:{
			pic_left:pic_path('ui/officers/balloon/balloon_left.png'),
			pic_right:pic_path('ui/officers/balloon/balloon_right.png')
		}
	},				
	cso:{	// Forschung
		pic_left:pic_path('ui/officers/cso/grandma_left.png'),
		pic_right:pic_path('ui/officers/cso/grandma_right.png'),
		width:148,
		height:350,
		balloon:{
			pic_left:pic_path('ui/officers/balloon/balloon_left.png'),
			pic_right:pic_path('ui/officers/balloon/balloon_right.png')
		}
	},
	cco:{	// Gebäude
		pic_left:pic_path('ui/officers/cco/construction_bernard_left.png'),
		pic_right:pic_path('ui/officers/cco/construction_bernard_right.png'),
		width:178,
		height:350,
		balloon:{
			pic_left:pic_path('ui/officers/balloon/balloon_left.png'),
			pic_right:pic_path('ui/officers/balloon/balloon_right.png')
		}
	}
};
PureFW.Officer.num_instances = 0;
PureFW.Officer.instances = new Array();

PureFW.Officer.active_officer_stack = new Array();

/**
 * Diese Funktion gibt den Officer zurück, der gerade angezeigt wird (es wird
 * immer nur ein Officer angezeigt zur gleichen Zeit).
 * 
 * @return PureFW.Officer
 */
PureFW.Officer.get_current_active_officer = function() {
	if (PureFW.Officer.active_officer_stack.length == 0)
		return null;
	
	return PureFW.Officer.active_officer_stack
		[PureFW.Officer.active_officer_stack.length-1];
};

PureFW.Officer.set_officer_z_index = function(z)
{
	if (PureFW.Officer.get_current_active_officer())
		PureFW.Officer.get_current_active_officer().set_z_index(z);
};

/********************************************************************
 * Prototyp-Deklarationen
 ********************************************************************/

PureFW.Officer.prototype.insert_into_dom_tree = function() 
{
	PureFW.Officer.parent.insert_into_dom_tree.call(this);
	this.position = new PureFW.Point(0,0);
	
	this.officer_container = new PureFW.Container(
			this.parent_node, 0, 0, this.officer_width, this.officer_height);
	this.officer_container.set_positioning('absolute');
	this.officer_container.set_bg_img(this.officer_url);
	this.officer_container.set_x(
			(this.officer_pos.x+this.balloon_width+this.balloon_pos.x));
	this.officer_container.set_y(
			(this.officer_pos.y+this.balloon_height-70+this.balloon_pos.y));
	this.officer_container.set_id(this.officer_id);
	this.officer_container.parent_container = this;
	this.id = this.officer_id;
	
	this.balloon_container = new PureFW.OfficerBalloon(
			this.parent_node, this.officer_pos.x, this.officer_pos.y, 
			this.balloon_width, this.balloon_height,
			this.balloon_url
	);
	this.balloon_container.set_id(this.balloon_id);
	
	var n = PureFW.Officer.active_officer_stack.length;
	for(var i = 0; i < n; i++)
	{
		PureFW.Officer.active_officer_stack[i].hide();
	}
	this.show();

	this.set_z_index(191); // just to be safe... can be set again later
};

PureFW.Officer.prototype.set_officer = function(officer) 
{
	if (!officer || !officer.pic_left || !officer.pic_right) 
	{
		alert ('Officer::set_officer erwartet eine Officerbeschreibung als ersten Parameter');
		return;
	}
	this.officer = officer; 
	//TODO Bilder austauschen und Größen und Position aktuallisieren.
};

PureFW.Officer.prototype.set_z_index = function(z) {
	PureFW.Officer.parent.set_z_index.call(this,z);
	this.balloon_container.set_z_index(z);
	this.officer_container.set_z_index(z);
};

/**
 * Synonym für ~.set_position
 * 
 * @param uint x
 * @param uint y
 */
PureFW.Officer.prototype.set_overall_position = function(x, y)
{
	this.set_position(x, y);
};

/**
 * Setzt die Bezugsposition von Officer und Balloon, d.h. die Position,
 * an denen sich Officer und Balloon relativ ausrichten.
 * 
 * @param uint x
 * @param uint y
 */
PureFW.Officer.prototype.set_position = function(x, y) {
	PureFW.Officer.parent.set_position.call(this, x, y);
	this._recalculate_position();
}

/**
 * Setzt die Position des Officers relativ zur Bezugsposition neu, welche durch
 * ~.set_position vorher gesetzt wurde. 
 * 
 * @param uint x
 * @param uint y
 */
PureFW.Officer.prototype.set_officer_position = function(x,y) 
{
	delete this.officer_pos;
	this.officer_pos = new PureFW.Point(x,y);
	this._recalculate_position();
};

/**
 * Setzt die Position der Bubble relativ zur Bezugsposition neu, welche durch
 * ~.set_position vorher gesetzt wurde. 
 * 
 * @param uint x
 * @param uint y
 */
PureFW.Officer.prototype.set_balloon_position = function(x,y) 
{
	delete this.balloon_pos;
	this.balloon_pos = new PureFW.Point(x,y);
	this._recalculate_position();
};

/**
 * Setzt die Breite des Officer-Bildes auf den angegebenen Wert (Pixel bei 
 * Scale 1)
 * 
 * @param uint width
 */
PureFW.Officer.prototype.set_pic_width = function(width) {
	this.officer_container.set_width(width);
};

/**
 * Setzt die Höhe des Officer-Bildes auf den angegebenen Wert (Pixel bei 
 * Scale 1)
 * 
 * @param uint height
 */
PureFW.Officer.prototype.set_pic_height = function(height) {
	this.officer_container.set_height(height);
};

/**
 * Setzt die Breite des Officer-Balloons auf den angegebenen Wert (Pixel bei 
 * Scale 1)
 * 
 * @param uint width
 */
PureFW.Officer.prototype.set_balloon_width = function(width) {
	this.balloon_container.set_growed_width(width);
};

/**
 * Setzt die Höhe des Officer-Balloons auf den angegebenen Wert (Pixel bei 
 * Scale 1)
 * 
 * @param uint height
 */
PureFW.Officer.prototype.set_balloon_height = function(height) {
	this.balloon_container.set_growed_height(height);
};

PureFW.Officer.prototype.set_officer_on_click = function(fn)
{
	this.officer_container.add_event_handler('click', fn);
};
	
PureFW.Officer.prototype.set_balloon_on_click = function(str) 
{
	this.balloon_container.add_event_handler('click', fn);
};

/**
 * Setzt den Text, der in der Bubble stehn soll auf den angegebenen Wert.
 * 
 * @param string str
 */
PureFW.Officer.prototype.set_text = function(str) {
	this.balloon_container.set_text(str);
};

/**
 * Zeigt den Confirm-Dialog der Blubberblase an.
 * 
 * @param string str
 */
PureFW.Officer.prototype.show_confirm_dialog = function(str) {
	this.balloon_container.show_confirm_dialog(str);
};

/**
 * Versteckt den Confirm-Dialog der Blubberblase.
 */
PureFW.Officer.prototype.hide_confirm_dialog = function() {
	this.balloon_container.hide_confirm_dialog();
};

/**
 * Zeigt den Officer und die Blase an.
 */
PureFW.Officer.prototype.show = function()
{	
	this.balloon_container.show();
	this.officer_container.show();
};

/**
 * Macht den Officer mit Bubble unsichtbar.
 */
PureFW.Officer.prototype.hide = function() 
{
	this.balloon_container.hide();
	this.officer_container.hide();
};

/**
 * Fügt einen EventHandler hinzu. Funktion überschrieben, da zusätzlich
 * der Typ "confirm" unterstützt wird, welcher beim Bestätigen des Fensters
 * ausgelöst wird (also beim Klick auf "YES"), sowie "cancel" bei Klick auf 
 * "NO".
 * Abgesehen von "confirm" und "cancel" beziehen sich alle Events auf sowohl
 * Officer, als auch balloon_container.
 * 
 * @param string type
 * @param Function fn
 * @see PureFW.Widget#add_event_handler
 */
PureFW.Officer.prototype.add_event_handler = function(type, fn) {
	this.balloon_container.add_event_handler(type, fn);
	if ((type !== "confirm") && (type !== "cancel"))
		this.officer_container.add_event_handler(type, fn);
};

/**
 * Entfernt einen EventHandler hinzu. Funktion überschrieben, da zusätzlich
 * der Typ "confirm" unterstützt wird, welcher beim Bestätigen des Fensters
 * ausgelöst wird (also beim Klick auf "YES"), sowie "cancel" bei Klick auf 
 * "NO".
 * Abgesehen von "confirm" und "cancel" beziehen sich alle Events auf sowohl
 * Officer, als auch balloon_container.
 * 
 * @param string type
 * @param Function fn
 * @see PureFW.Widget#remove_event_handler
 */
PureFW.Officer.prototype.remove_event_handler = function(type, fn) {
	this.balloon_container.remove_event_handler(type, fn);
	if ((type !== "confirm") && (type !== "cancel"))
		this.officer_container.remove_event_handler(type, fn);
};


/***** AB HIER ALLES PRIVATE *****/

/**
 * Berechnet die Positionen von Officer und Bubble neu und setzt sie
 * entsprechend (abhängig von der Overall-Position und den relativen).
 */
PureFW.Officer.prototype._recalculate_position = function() 
{
	this.officer_container.set_x(
		this.position.x + this.officer_pos.x
	);
	this.officer_container.set_y(
		this.position.y + 
		this.officer_pos.y + this.balloon_height-85
	);
	this.balloon_container.set_x(
		this.position.x + this.balloon_pos.x
		+ this.officer_container.width + 10
	);
	this.balloon_container.set_y(
		this.position.y + this.balloon_pos.y
	);
};

/**
 * Zeichnet das Widget neu
 */
PureFW.Officer.prototype.repaint = function() {
	this.balloon_container.repaint();
	this.officer_container.repaint();
};

/**
 * Zerstört das Widget
 */
PureFW.Officer.prototype.destroy = function()
{
	PureFW.Officer.active_officer_stack.pop();
	if(PureFW.Officer.active_officer_stack.length > 0)
	{
		var n = PureFW.Officer.active_officer_stack.length;
		PureFW.Officer.active_officer_stack[n - 1].show();
	}
	this.balloon_container.destroy();
	this.officer_container.destroy();
	PureFW.Officer.parent.destroy.call(this);
};