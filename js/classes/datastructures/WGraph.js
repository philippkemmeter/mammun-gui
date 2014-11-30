/**
 * Knotengewichteter, gerichteter Graph.
 * 
 * @author Phil
 */

// Hilfsklasse Knoten (Felder der Karte)
Knoten = function(id, w) {
	this.neighbours = new Array();
	this.id = parseInt(id);
	this.weight = w > 0 ? w : 1;// Hier sind die Felder gewichtet (Terrain)!
	this.dijkstra_w = 1 << 14;	// Gewicht des Knotens in Bezug auf Dijkstra-Algo
	this.dijkstra_p = null;		// Zeiger auf Nächsten Knoten im Pfad (Dijkstra)
}
Knoten.prototype.destroy = function() {
	this.neighbours.destroy();
}


WGraph = function() {
	this.adjazenzliste = new Array();
};

WGraph.prototype.add_vertex = function(v1) {
	if (this.adjazenzliste[v1.id]) {
		try {
			this.adjazenzliste[v1.id].destroy();
		}
		catch(e){}
		this.adjazenzliste[v1.id] = null;
	}
	this.adjazenzliste[v1.id] = new Array(v1);
};

WGraph.prototype.get_vertex = function(v1_id) {
	return (this.adjazenzliste[v1_id]) ? this.adjazenzliste[v1_id][0] : null;
};

/**
 * Kante zwischen zwei Knoten ziehen. Knoten durch die Knoten-ID auswählen.
 * Ist eine der Knoten-IDs nicht vorhanden, wird KEINE Kante gezogen!
 * 
 * @param int v1_id Startknoten-ID
 * @param int v2_id Endknoten-ID
 * @param bool bidirectional Ob auch eine Rückwärtskante gezogen werden soll
 * @result bool ob eine Kante gezogen werden konnte, oder nicht
 */
WGraph.prototype.add_edge = function(v1_id, v2_id, bidirectional) {
	if (typeof(this.adjazenzliste[v1_id]) === 'undefined') 
		return false;
	if (typeof(this.adjazenzliste[v2_id]) === 'undefined')
		return false;
	
	// Kante ziehen, wenn noch nicht bereits vorhanden
	if (!this.adjazenzliste[v1_id].contains(v2_id))
		this.adjazenzliste[v1_id][this.adjazenzliste[v1_id].length] = v2_id;
	if (bidirectional && !this.adjazenzliste[v2_id].contains(v1_id))
		this.adjazenzliste[v2_id][this.adjazenzliste[v2_id].length] = v1_id;
	return true;
};
/**
 * Löscht eine gezogenene Kante zwischen zwei Knoten
 * 
 * @param int v1_id		Startknoten-ID
 * @param int v2_id		Endknoten-ID
 * @param bool bidirectional	Ob auch die Rückkante gelöscht werden soll
 * @return bool			ob erfolgreich
 */
WGraph.prototype.remove_edge = function(v1_id, v2_id, bidirectional) {
	if (typeof(this.adjazenzliste[v1_id]) === 'undefined') 
		return false;
	if (typeof(this.adjazenzliste[v2_id]) === 'undefined')
		return false;
	
	this.adjazenzliste[v1_id].remove(v2_id);
	if (bidirectional)
		this.adjazenzliste[v2_id].remove(v1_id);
	return true;
};

/**
 * Gibt die Stringrepräsentation des Graphen zurück
 * @return string
 */
WGraph.prototype.to_string = function() {
	var s = '';
	for (var i = 0; i < this.adjazenzliste.length; i++) {
		if (typeof(this.adjazenzliste[i]) !== 'undefined') {
			for (var j = 1; j < this.adjazenzliste[i].length; j++) {
				var v = this.adjazenzliste[i][0];
				var u = this.adjazenzliste[this.adjazenzliste[i][j]][0];
				var w = v.weight + u.weight;
				s += i + ' --(' + w + ')--> ' + this.adjazenzliste[i][j] + "\n";
			}
		}
	}
	return s;
}
