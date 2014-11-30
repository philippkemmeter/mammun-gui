/**
 * Findet kürzeste Wege auf einer Karte mit unterschiedlichem Terrain
 * (gewichtete Knoten) Durch gewisse Portale kann es "Einbahnstraßen" geben - 
 * wir haben es also mit gerichteten Kanten zu tun.
 *
 * @author Phil
 */

var Pathfinder = function(graph) {
	if (typeof(graph) !== 'object' || typeof(graph.adjazenzliste) === 'undefined') {
		alert('Pathfinder: graph muss der Klasse WGraph angehören');
		return false;
	}
	if (typeof(MinHeap) == 'undefined') {
		alert('Pathfinder: MinHeap-Klasse nicht gefunden!');
		return false;
	}
	if (typeof(WGraph) == 'undefined') {
		alert('Pathfinder: WGraph-Klasse nicht gefunden!');
		return false;
	}
	this.graph = graph;
	this.last_s_id = null;
};

Pathfinder.prototype = {
	destroy:function() {
	},
	dijkstra:function(s_id) {
		this.last_s_id = s_id;	// für spätere Pfadrekonstruktion
		
		var pq = new MinHeap();	// Priority Queue
		for (var i = 0; i < this.graph.adjazenzliste.length; i++) {
			if (typeof(this.graph.adjazenzliste[i]) === 'undefined')
				continue;
			this.graph.adjazenzliste[i][0].dijkstra_w = 1 << 14;	// unendlich
			this.graph.adjazenzliste[i][0].dijkstra_p = null;
		}
		this.graph.adjazenzliste[s_id][0].dijkstra_w = 0;
		pq.insert(s_id);
		while (!pq.empty()) {
			var v = this.graph.adjazenzliste[pq.pop()][0];
			for (var j = 1; j < this.graph.adjazenzliste[v.id].length; j++) {
				var u = this.graph.adjazenzliste[this.graph.adjazenzliste[v.id][j]][0];
				var kanten_gewicht = v.weight + u.weight;
//				alert(v.id + '--('+kanten_gewicht+')-->' + u.id);
				if ((kanten_gewicht + v.dijkstra_w) < u.dijkstra_w) {
//					alert(kanten_gewicht +'+'+ v.dijkstra_w+'<'+u.dijkstra_w);
					u.dijkstra_p = v.id;
					u.dijkstra_w = (kanten_gewicht + v.dijkstra_w);
					pq.insert(u.id);
				}
//				else
//					alert(kanten_gewicht +'+'+ v.dijkstra_w+'>='+u.dijkstra_w);
			}
		}
	},
	get_path:function(g_id) {
		if (this.graph.adjazenzliste[g_id] === 'undefined')
			return false;
		var result = new Array();
		var s_id = this.last_s_id;
		if (!this.graph.adjazenzliste[g_id])
			return new Array();	// kein Weg möglich
		else
			var v = this.graph.adjazenzliste[g_id][0];
		
		while(v.id != s_id) {
			result[result.length] = v.id;
			if (!this.graph.adjazenzliste[v.dijkstra_p])
				return new Array();	// kein Weg möglich
			else
				v = this.graph.adjazenzliste[v.dijkstra_p][0];
		}
		result[result.length] = s_id;
		return result.reverse();
	},
	get_path_str:function(g_id) {
		if (this.graph.adjazenzliste[g_id] === 'undefined')
			return false;
		var s_id = this.last_s_id; 
		var str = '';
		if (!this.graph.adjazenzliste[g_id])
			return '';	// kein Weg möglich
		else
			var v = this.graph.adjazenzliste[g_id][0];
		do {
			str = '-' + v.id + str;
			if (!this.graph.adjazenzliste[v.dijkstra_p])
				return '';	//kein Weg möglich
			else
				v = this.graph.adjazenzliste[v.dijkstra_p][0];
		} while(v.id != s_id);
		return s_id+str;
	}
};