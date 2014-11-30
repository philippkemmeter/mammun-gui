/**
 * Repräsentiert einen einfachen Min-Heap. Laufzeiten für's Einfügen: O(log(n)),
 * Kleinstes Element entfernen: O(log(n)). Datenstruktur: Binärbaum als 1D-Array.
 * 
 * @author Phil
 */
 
var MinHeap = function() {
	// Die Heap-Items sind wie folgt organisiert: Ein 1D-Array, wobei die Position
 	// des Vaters eines Knotens im Heap [Position des Kindes]/2 ist. Die Wurzel
 	// ist an Position 1 (nicht 0!), damit das einwandfrei funktioniert.
 	this.items = new Array();
 	this.items.length++;	// Pos 0 überspringen
};
 
MinHeap.prototype={
	insert:function(value) {
 		var child_pos = this.items.length;
 		this.items[child_pos] = value;	// push
 		
 		// Heap-Bedingung prüfen
 		var parent_pos = child_pos >> 1;
 		while ((parent_pos >= 1) && (this.items[parent_pos] > value)) {
 			// Verletzung der Heap-Bed.
 			this.items[child_pos] = this.items[parent_pos]
 			child_pos = parent_pos;
 			parent_pos = child_pos >> 1;
 		}
 		this.items[child_pos] = value;
 	},
 	empty:function() {
 		if (this.items.length < 1)	// dürfte nie passieren
 			this.items.length = 1;
 		return (this.items.length == 1);
 	},
 	pop:function() {
 		if (this.empty())
 			return false;
 			
 		var result = this.items[1];	// pop
 		this.items[1] = this.items[this.items.length-1];
 		this.items.length--;
 		
 		// Heap-Bedingung prüfen
 		var parent_pos = 1;
 		var value = this.items[parent_pos];
 		var child_pos = parent_pos << 1;
 		while (child_pos <= this.items.length-1) {
 			if (child_pos < this.items.length-1)
 				if (this.items[child_pos+1] < this.items[child_pos])
 					child_pos++;	// rechter Sohn ist kleiner
			if (value > this.items[child_pos])	{	// Verletzung der Heap-Bed.
				this.items[parent_pos] = this.items[child_pos];
				parent_pos = child_pos;
				child_pos = parent_pos << 1;
			}
			else
				break;
			this.items[parent_pos] = value;
		}
		return result;
	},
	print:function() {
		var s = '';
		for (var i = 0; i < this.items.length; i++) {
			s += this.items[i];
			s += ' ';
		}
		alert(s);
	}
};
