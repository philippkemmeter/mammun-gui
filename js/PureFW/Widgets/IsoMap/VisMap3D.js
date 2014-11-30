/**
 * Dieses Widget repräsentiert eine Visualisierungskarte, auf der isometrische
 * 3D-Objekte platziert werden können.
 * 
 * Jedes dieser Objekte kann mehrere Felder belegen und hat eine zusätzliche
 * 3D-Höhe.
 * 
 * @author Phil
 * @package PureFW.IsoMap
 */

PureFW.IsoMap.VisMap3D = function(parent_node, x, y, w, h, num_fields_x,
	num_fields_y, no_scale)
{
	if (typeof(parent_node) !== 'undefined') {
		this.init(parent_node, x, y, w, h, num_fields_x, num_fields_y, 
			no_scale);
		
		this.instance_num = this.constructor.num_instances;
		this.instance_name = "PureFW.IsoMap.VisMap3D.instances["
			+this.instance_num+"]";
		
		this.id = "PureFW.IsoMap.VisMap3D"+this.instance_num;
		
		this.insert_into_dom_tree();
		this.constructor.num_instances++;
		this.constructor.instances[this.instance_num] = this;
		
		if (!this.no_scale)
			PureFW.WidgetManager.manager_all.register_widget(this);
	}
}


/********************************************************************
 * PureFW.IsoMap.VisMap3D extends IsoMap.VisMap
 ********************************************************************/
PureFW.IsoMap.VisMap3D.extend(PureFW.IsoMap.VisMap);

/********************************************************************
 * Statische Deklarationen
 ********************************************************************/
PureFW.IsoMap.VisMap3D.num_instances = 0;		// Instanzüberwachung (anzahl)
PureFW.IsoMap.VisMap3D.instances = new Array();	// Instanzüberwachung (Instanzen)

/**
 * Ein Objekt, das auf die Karte platziert wird.
 */
/*Struct */ PureFW.IsoMap.VisMap3D.MapObject = function(widget, w, h, proj_h) 
{
	this.widget = widget;
	this.info = new Object();
	this.info.size = new PureFW.Point(w,h);
	this.info.proj_h = proj_h;
}
PureFW.IsoMap.VisMap3D.MapObject.prototype.destroy = function() {
	try {
		this.widget.destroy();
	}
	catch(e){}
	try {
		this.info.size.destroy();
	}
	catch(e){}
	delete this.info;
}

/********************************************************************
 * Prototyp-Deklarationen
 ********************************************************************/
PureFW.IsoMap.VisMap3D.prototype.init = function(parent_node, x, y, w, h, 
	num_fields_x, num_fields_y, no_scale)
{
	PureFW.IsoMap.VisMap3D.parent.init.call(this, parent_node, x, y, w, h,
		num_fields_x, num_fields_y, no_scale);

	/**
	 * Hier werden alle 3D-Objekte, die auf der Karte sind, gespeichert.
	 * @see PureFW.IsoMap.VisMap3D.MapObject
	 */
	this.objects_3d = new Array();
	for (var i = 0; i < this.num_fields_x; i++) {
		this.objects_3d[i] = new Array();
		for(var j = 0; j < this.num_fields_y; j++)
			this.objects_3d[i][j] = null;
	}
	
};

PureFW.IsoMap.VisMap3D.prototype.insert_into_dom_tree = function() {
	PureFW.IsoMap.VisMap3D.parent.insert_into_dom_tree.call(this);
}

/**
 * Erzeugt ein 3D-Objekt auf der Karte.
 * 
 * <code>
 *   /\  Man stelle sich ein Feld als eine Raute wie links zu sehen ist vor. Die
 *   \/  Die Höhe eines 3D-Objektes wird als Projektionsgröße angegeben, und 
 *       zwar gibt sie an, wie viel mal das Objekt, das eingefügt werden soll,
 * von der unteren Spitze der Raute zur obereren reicht. Ein Wert von proj_h
 * von 1 würde also angeben, dass das Objekt genau von unten bis oben reicht, 
 * eine Wert von 0.5 die Hälfte, einer von 1.5 die Hälfte mehr usw.
 * </code>
 * 
 * Es kann also keine 3D-Höhe im eigentlichen Sinne angegeben werden, weil
 * die Objekte ja bereits projiziert/gerendert vorliegen in 2D! Es wird somit
 * nur die 2D-Objekthöhe über den Wert proj_h ermittelt.
 * 
 * Achtung: Überlappungen werden NICHT geprüft. Es handelt sich hierbei nur
 * um eine Visualisierung, daher werden die Daten auch nicht geprüft.
 *  
 * @param uint x			x-Position in Feldkoordinaten (obere linke Ecke)
 * @param uint y 			y-Position in Feldkoordinaten (obere linke Ecke)
 * @param uint size_x		Wie viele Felder in x-Richtung das Objekt einnimmt
 * @param uint size_y		Wie viele Felder in y-Richtung das Objekt einnimmt
 * @param ufloat proj_h		Projizierte Höhe des Objekts als 
 *							Feldhöhenkoordinaten. Die Höhe 1 bedeutet, dass das
 * 							Objekt von der unteren Spitze der Isometriefeldraute
 * 							zur oberen Spitze ragt. Kommazahlen erlaubt.
 * @param Widget-Constructor widget_constructor
 * 							Welchs Widget erzeugt werden soll.
 * @param mixed add_argN	Zusätzliche Argumente, die dem Konstruktor übergeben
 * 							werden sollen. Die ersten fünf (parent_node, x, y
 * 							w, h) sind fest und werden hier sowieso gefüllt.
 * 							Hierüber können ggf. weitere definiert werden.
 * 
 * @return PureFW.Widget	Das erzeugte Widget
 */
PureFW.IsoMap.VisMap3D.prototype.create_object = function(x, y, size_x, size_y, 
	proj_h, widget_constructor, w_arg6, w_arg7, w_arg8, w_arg9, w_arg10, 
	w_arg11, w_arg12, w_arg13) 
{
	this.destroy_object(x,y);

	var x_pos = (this.width + this.field_width*(x - y - size_y)) >> 1;
	var y_pos = (this.field_height*(x + y) >> 1)
		-size_y*this.field_height*(proj_h-1);
	
	var width = this.field_width*size_x;
	var height = this.field_height*size_y*proj_h;
	
	widget = new widget_constructor(
		this,
		x_pos, y_pos,
		width, height,
		w_arg6, w_arg7, w_arg8, w_arg9, w_arg10, w_arg11, w_arg12, w_arg13
	);
	/**
	 * Der z-Index muss noch geschickt gewählt werden. Ein Objekt, das 
	 * weiter "vorne" (im 3D-Sinne) steht, muss über einem anderen liegen.
	 * Durch Rotation der Karte, so dass ein jedes Feld eine Raute 
	 * darstellt, gilt, dass in jede horizontalen Reihe von Rauten denselben
	 * z-Index erhalten kann. Dazu bräuchte man jetzt ein Bild ^^
	 * Wenn man sich mal etwas damit auseinandersetzt ist der eindeutige
	 * und möglichst kleine z-Index, der keine Sortierung der Objekte
	 * benötigt x+y.
	 * Dazu muss noch der maximale z-Index der Felder addiert werden,
	 * welche nach demselben System vergeben wurden und somit
	 * this.num_fields_x+this.num_fields_y ist.
	 */
	widget.set_z_index(x+y+this.num_fields_x+this.num_fields_y);
	this.objects_3d[x][y] = new PureFW.IsoMap.VisMap3D.MapObject(
		widget, size_x, size_y, proj_h
	);
	
	return widget;
};

/**
 * Zerstört das angegebene Objekt.
 * 
 * @param uint x
 * @param uint y
 * @return
 */
PureFW.IsoMap.VisMap3D.prototype.destroy_object = function(x, y) {
	if (this.objects_3d[x][y]) {
		try {
			this.objects_3d[x][y].destroy();
		}
		catch(e) {
		}
		this.objects_3d[x][y] = null;
	}
}

/**
 * Gibt ein Objekt zurück.
 * 
 * Wir full_data angegeben, wird ein Objekt mit folgenden Attributen 
 * zurückgegeben:
 * <code>
 * o.widget PureFW.Widget		Das verknüpfte Widget
 * o.info.size.x uint			Größe in Feldern in x-Richtung des Objektes
 * o.info.size.y uint			Größe in Feldern in y-Richtung des Objektes
 * o.info.proj_h uint			Projektionshöhenfaktor
 * </code>
 * 
 * Ansonsten wird nur das Widget zurückgegeben (Standardeinstellung).
 * 
 * @param uint x
 * @param uint y
 * @param bool full_data=false
 * @return PureFW.Widget / Object
 */
PureFW.IsoMap.VisMap3D.prototype.get_object = function(x,y, full_data) {
	if (full_data)
		return (this.objects_3d[x][y]) ? this.objects_3d[x][y] : null;
	else
		return (this.objects_3d[x][y]) ? this.objects_3d[x][y].widget : null;
};