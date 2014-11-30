/** 
 * Zählt eine abstrakte Menge hoch.
 * Die abstrakte Menge könnte z.B. eine Resource sein, welche permanent "Zuwachs"
 * bekommt (konkreteres Beispiel: Holz, das gesammelt wird).
 * 
 * @author Phil
 */
 
 
/**
 * Konstruktor der Gainer-Klasse.
 *
 * @param float current_amount		Wieviel von der Menge aktuell vorhanden ist
 * @param float gain_per_second		Wieviel Holz pro Sekunde dazu kommt
 */
var Gainer = function(current_amount, gain_per_second) {
	var j = new Date();
	this.start_time = Math.floor(j.getTime()/1000);
	this.start_amount = current_amount ? current_amount : 0;
	this.gain = gain_per_second ? gain_per_second : 0;
	this.max_capacity = 0;
};

Gainer.prototype = {
	/**
	 * Setzt eine Obergrenze. Es wird ab Erreichen der Obergrenze nicht
	 * mehr weiterhochgezählt und als Markierung des Erreichens die Anzahl
	 * rot ausgegeben (wenn Outputelements definiert)
	 *
	 * @param float max		Maximale Anzahl
	 */
	set_max:function(max) {
		this.max_capacity = max;
	},
	/**
	 * Wenn sich die Menge sprunghaft auf einen Wert gesetzt werden soll, muss
	 * diese Funktion aufgerufen werden. Bsp.: Man hat etwas von der Menge
	 * ausgegeben oder plötzlich dazugewonnen.
	 */
	set_current_amount:function(value) {
		var j = new Date();
		this.start_time = Math.floor(j.getTime()/1000);
		this.start_amount = parseInt(value);
	},
	add_current_amount:function(value) {
		var j = new Date();
		this.start_time = Math.floor(j.getTime()/1000);
		this.start_amount = this.get_cur_value()+parseInt(value);
	},
	/**
	 * Setzt den sekündlichen Dazugewinn der Menge
	 */
	set_gain_per_second:function(gain) {
		this.start_amount = this.get_cur_value();
		var j = new Date();
		this.start_time = Math.floor(j.getTime()/1000);
		this.gain = gain;
	},
	/**
	 * Gibt den aktuellen Wert zurück
	 */
	get_cur_value:function() {
		var j = new Date();
		var t_now = Math.floor(j.getTime()/1000);
		var cur_value = Math.round(this.start_amount + (t_now - this.start_time)*this.gain);
		if (this.max_capacity && (cur_value >= this.max_capacity))
			cur_value = this.max_capacity;
		return cur_value;
	}
};