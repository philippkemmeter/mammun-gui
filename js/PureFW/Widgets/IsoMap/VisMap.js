/**
 * Dieses Widget repräsentiert eine Darstellungsebene einer isometrischen Karte
 * mit mehreren Feldern, denen Inhalte zugewiesen werden können.
 * 
 * @author Phil
 * @package PureFW.IsoMap
 */

PureFW.IsoMap.VisMap = function(parent_node, x, y, w, h, num_fields_x, 
	num_fields_y, no_scale)
{
	if (typeof(parent_node) !== 'undefined') {
		this.init(parent_node, x, y, w, h, num_fields_x, num_fields_y, no_scale);
		
		this.instance_num = this.constructor.num_instances;
		this.instance_name = "PureFW.IsoMap.VisMap.instances["
			+this.instance_num+"]";
		
		this.id = "PureFW.IsoMap.VisMap"+this.instance_num;
		
		this.insert_into_dom_tree();
		this.constructor.num_instances++;
		this.constructor.instances[this.instance_num] = this;
		
		if (!this.no_scale)
			PureFW.WidgetManager.manager_all.register_widget(this);
	}
}

/********************************************************************
 * PureFW.IsoMap.VisMap extends IsoMap.Map
 ********************************************************************/
PureFW.IsoMap.VisMap.extend(PureFW.IsoMap.Map);

/********************************************************************
 * Statische Deklarationen
 ********************************************************************/
PureFW.IsoMap.VisMap.num_instances = 0;			// Instanzüberwachung (anzahl)
PureFW.IsoMap.VisMap.instances = new Array();	// Instanzüberwachung (Instanzen)

/********************************************************************
 * Prototyp-Deklarationen
 ********************************************************************/
PureFW.IsoMap.VisMap.prototype.init = function(parent_node, x, y, w, h, 
	num_fields_x, num_fields_y, no_scale)
{
	PureFW.IsoMap.VisMap.parent.init.call(this, parent_node, x, y, w, h,
		num_fields_x, num_fields_y, no_scale);

	for (var x = 0; x < this.num_fields_x; x++) {
		this.fields[x] = new Array();
		for (var y = 0; y < this.num_fields_y; y++) {
			this.fields[x][y] = null;
		}
	}
};

PureFW.IsoMap.VisMap.prototype.insert_into_dom_tree = function() {
	PureFW.IsoMap.VisMap.parent.insert_into_dom_tree.call(this);
	
	this.node = document.createElement('div');
	this.node.style.position = 'absolute';
	this.node.style.width = this.get_width()+'px';
	this.node.style.height = this.get_height()+'px';
	this.node.style.left = this.get_x()+'px';
	this.node.style.top = this.get_y()+'px';
	this.node.style.overflow = 'visible';
	this.node.id = this.id;
	
	this.parent_node.appendChild(this.node);
}

/**
 * Erzeugt ein Feld in der Karte.
 * 
 * @param uint x			x-Position in Feldkoordinaten
 * @param uint y 			y-Position in Feldkoordinaten
 * @param Widget-Constructor widget_constructor
 * 							Welchs Widget erzeugt werden soll.
 * @param mixed add_argN	Zusätzliche Argumente, die dem Konstruktor übergeben
 * 							werden sollen. Die ersten fünf (parent_node, x, y
 * 							w, h) sind fest und werden hier sowieso gefüllt.
 * 							Hierüber können ggf. weitere definiert werden.
 * @return PureFW.Widget	Das erzeugte Widget
 */
PureFW.IsoMap.VisMap.prototype.create_field = function(x, y, widget_constructor,
	w_arg6, w_arg7, w_arg8, w_arg9, w_arg10, w_arg11, w_arg12, w_arg13) 
{
	this.destroy_field(x, y);
	
	var x_pos = (this.width + this.field_width*(x-y-1)) >> 1;
	var y_pos = this.field_height*(x+y) >> 1;

	this.fields[x][y] = new widget_constructor(
		this,
		x_pos, y_pos,
		this.field_width, this.field_height,
		w_arg6, w_arg7, w_arg8, w_arg9, w_arg10, w_arg11, w_arg12, w_arg13
	);
	
	/**
	 * Der z-Index muss noch geschickt gewählt werden. Ein Feld, das 
	 * weiter "vorne" (im 3D-Sinne) steht, muss über einem anderen liegen.
	 * Durch Rotation der Karte, so dass ein jedes Feld eine Raute 
	 * darstellt, gilt, dass in jede horizontalen Reihe von Rauten denselben
	 * z-Index erhalten kann. Dazu bräuchte man jetzt ein Bild ^^
	 * Wenn man sich mal etwas damit auseinandersetzt ist der eindeutige
	 * und möglichst kleine z-Index, der keine Sortierung der Felder
	 * benötigt x+y.
	 * Bei einer 10x10-Karte brauchen wir somit statt 100 z-Indizes nur noch
	 * 18 (=9+9).
	 */
	this.fields[x][y].set_z_index(x+y);
	
	return this.fields[x][y];
}

/**
 * Gibt das durch x und y bestimmte Feld zurück.
 * 
 * @return PureFW.Widget
 */
PureFW.IsoMap.VisMap.prototype.get_field = function(x, y) {
	return this.fields[x][y];
}

/**
 * Zerstört durch x und y bestimmte Feld.
 * 
 * @return PureFW.Widget
 */
PureFW.IsoMap.VisMap.prototype.destroy_field = function(x, y) {
	if (this.fields[x][y]) {
		try {
			this.fields[x][y].destroy();
		}
		catch(e){}
		this.fields[x][y] = null;
	}
}