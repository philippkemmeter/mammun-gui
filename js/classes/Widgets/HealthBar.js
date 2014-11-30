/**
 * Repräsentiert einen Fortschrittsbalken. Dieser kann aber auch zur 
 * Visualisierung von anderen prozentualen Angaben genutzt werden.
 * Die HealthBar wird absolut (relativ zu parent_node) positioniert.
 * 
 * @author Alex, Phil
 */
HealthBar = function(parent_node, x, y, w, h) {
	if (typeof(parent_node) !== 'undefined') {
		this.init(parent_node, x, y, w, h);
		
		this.instance_num = this.constructor.num_instances;
		this.instance_name = "HealthBar.instances["+this.instance_num+"]";
		
		this.id = 'HealthBar'+this.instance_num;
		this.bg_img_id = 'HealthBarBg'+this.instance_num;
		this.content_id = 'HealthBarCont'+this.instance_num;
		
		this.direction = this.constructor.HORIZONTAL;
		
		this.insert_into_dom_tree();
		this.constructor.num_instances++;
		this.constructor.instances[this.instance_num] = this;
		
		PureFW.WidgetManager.manager_all.register_widget(this);
	}
};

/********************************************************************
 * HealthBar EXTENDS Container
 ********************************************************************/
HealthBar.extend(PureFW.Container);

/****************************************************************
 * Statische Deklarationen
 ****************************************************************/
HealthBar.VERTICAL = 0;
HealthBar.HORIZONTAL = 1;
HealthBar.num_instances = 0;
HealthBar.instances = new Array();


/****************************************************************
 * Prototyp-Funktionen
 ****************************************************************/
HealthBar.prototype.init = function(parent_node, x, y, w, h) {
	HealthBar.parent.init.call(this, parent_node, x, y, w, h);
	this.value = 0;
	this.max_value = 100;
	this.bar = null;
}

/**
 * Fügt den Fortschrittsbalken in den DOM-Baum ein. Falls das Objekt bereits
 * im Baum eingehängt ist (oder ein anderes mit der ID dieses Objekts...),
 * wird ein Fehler geworfen.
 * 
 * @throws Error
 */
HealthBar.prototype.insert_into_dom_tree = function() {
	HealthBar.parent.insert_into_dom_tree.call(this);

	this.bar = new PureFW.Container(
		this,
		0, 0,
		this.get_bar_length(), this.height
	);
	this.bar.set_bg_color('#'+this.get_bar_color());
};


/**
 * Setzt den Wert des Fortschrittbalkens auf den angegebenen Wert.
 * 
 * @param ufloat v
 */
HealthBar.prototype.set_value = function(v) {
	this.value = v;
	if (this.bar) {
		this.bar.set_width(this.get_bar_length());
		this.bar.set_bg_color('#'+this.get_bar_color());
	}
};

/**
 * Gibt den aktuellen Wert des Fortschrittbalkens zurück.
 * 
 * @return ufloat
 */
HealthBar.prototype.get_value = function() {
	return this.value;
}

/**
 * Setzt den maximalen Wert, also wann 100% erreicht sein soll.
 * 
 * @param ufloat v
 */
HealthBar.prototype.set_max_value = function(v) {
	this.max_value = v;
	if (this.bar) {
		this.bar.set_width(this.get_bar_length());
		this.bar.set_bg_color('#'+this.get_bar_color());
	}
}

/**
 * Gibt den maximalen Wert zurück.
 * 
 * @return ufloat
 */
HealthBar.prototype.get_max_value = function() {
	return this.max_value;
}

/**
 * Gibt die Länge in Pixeln zurück, die der Fortschrittsbalken aktuell hat
 * (NICHT die Länge, die der Balken maximal haben kann, was get_width() 
 * zurück liefern würde).
 * 
 * @return uint
 */
HealthBar.prototype.get_bar_length = function() {
	return Math.ceil((this.value*this.width)/this.max_value);
}

/**
 * Gibt den aktuellen Fortschritt zurück. Dieser Wert mal 100 würde somit
 * die Prozentanzeige angeben.
 * 
 * @return ufloat
 */
HealthBar.prototype.get_health = function() {
	return this.value/this.max_value;
}

/**
 * Gibt die Farbe als 3fach-Hexcode zurück (also z.B. FFCC00), welchen der
 * Balken aktuell hat.
 * 
 * @return string
 */
HealthBar.prototype.get_bar_color = function() {
	var p = this.get_health()*255;
	if (p < 0)
		p = 0;
	if (p > 255)
		p = 255;
	var green = Number(parseInt(p)).toString(16);
	var red = Number(parseInt(255-p)).toString(16);
	if (green.length < 2)
		green = '0'+green;
	if (red.length < 2)
		red = '0'+red;
	return red+green+'00';		
}
