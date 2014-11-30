/**
 * Verwaltet die Daten eines Sektors einer Spielkarte
 */

if (typeof(PureFW.GameMap) != 'object')
	PureFW.GameMap = new Object();

/**
 * Erzeugt ein neues Sektor-Daten-Objekt.
 * 
 * @param uint x	Feldkoordinate
 * @param uint y	Feldkoordinate
 */
PureFW.GameMap.SectorData = function(x, y) {
	this.coordinates = new PureFW.Point(x,y);
	this.units = null;
	this.dominator = null;
	this.buildings = null;
	this.resources = null;
	this.movements = null;
	this.fight = false;
}