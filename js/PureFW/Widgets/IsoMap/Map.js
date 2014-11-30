/**
 * Diese Klasse stellt eine abstrakte isometrische Karte dar, von der alle
 * isometrischen Karten erben sollen.
 * 
 * @author Phil
 * @package PureFW.IsoMap
 */

if (typeof(PureFW.IsoMap) != 'object') {
	PureFW.IsoMap = new Object();
}

PureFW.IsoMap.Map = new Object();	// abstrakt => statisch

/**
 * Abstract class IsoMap.Map extends abstract class Widget
 */
for (x in PureFW.Widget) PureFW.IsoMap.Map[x] = PureFW.Widget[x];
PureFW.IsoMap.Map.parent = PureFW.Widget;

PureFW.IsoMap.Map.init = function(parent_node, x, y, w, h, 
	num_fields_x, num_fields_y, no_scale)
{
	PureFW.IsoMap.Map.parent.init.call(this, parent_node, x, y, w, h,
		no_scale);
	
	if (!num_fields_x || (num_fields_x < 0))
		throw new Error("num_fields_x has to be in N1, "+ num_fields_x +"given");
	if (!num_fields_y || (num_fields_y < 0))
		throw new Error("num_fields_y has to be in N1, "+ num_fields_y +"given");
	
	this.num_fields_x = num_fields_x;
	this.num_fields_y = num_fields_y;
	this.field_width = (this.width / this.num_fields_x);
	this.field_height = (this.height / this.num_fields_y);
	
	/**
	 * Hält pro Feld ein Objekt, um darin spezielle Werte zu speichern.
	 * 
	 * Welche Objekte genau entscheidet dann die konkrete Klasse
	 */
	this.fields = new Array();
};

/**
 * Gibt die Breite in Pixeln eines einzelnen ClickMap-Feldes aus (Pixelgröße, 
 * wie aktuell dargestellt, also inklusive Scale-Faktor).
 * 
 * @return ufloat
 */
PureFW.IsoMap.Map.get_field_width = function() {
	return this.field_width * this.scale_factor;
};

/**
 * Gibt die Höhe in Pixeln eines einzelnen ClickMap-Feldes aus (Pixelgröße, 
 * wie aktuell dargestellt, also inklusive Scale-Faktor).
 * 
 * @return ufloat
 */
PureFW.IsoMap.Map.get_field_height = function() {
	return this.field_height * this.scale_factor;
};

/**
 * Gibt zurück, wie viele Felder die Map in x-Richtung besitzen kann.
 */
PureFW.IsoMap.Map.get_num_fields_x = function() {
	return this.num_fields_x;
}

/**
 * Gibt zurück, wie viele Felder die Map in y-Richtung besitzen kann.
 */
PureFW.IsoMap.Map.get_num_fields_y = function() {
	return this.num_fields_y;
}

/**
 * Gibt die Pixelposition des übergebenen Feldes zurück
 *
 * @param uint x
 * @param uint y
 * @return Point
 */
PureFW.IsoMap.Map.get_field_pixel_position = function(x, y) {
	result = 
		new PureFW.Point(
			this.width + this.field_width*(x-y-1)  >> 1,
			(this.field_height >> 1)*(x+y)
		);
	return result;
};