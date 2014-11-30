/**
 * Verwaltete alle bei ihm registrierten Widgets. So können an zentraler Stelle
 * Funktionen für alle Widgets ausgeführt werden (z.B. Skalieren usw)
 * 
 * Dies hilft somit, Widgets zu gruppieren :)
 * 
 * @author Phil
 */

PureFW.WidgetManager = function (initial_font_size) {
	this.initial_font_size = initial_font_size || 13;
	this.scale_factor = 1;
}

PureFW.WidgetManager.scale_reference_value = 1000;
PureFW.WidgetManager.page_aspect_ratio = 0;

/********************************************************************
 * PROTOTYPE
 ********************************************************************/
PureFW.WidgetManager.prototype.registered_widgets = new Array();
	
/**
 * Registriert das angegebene Widget zur Scale-Überwachung. Sobald ein neuer 
 * Wert dem WidgetScaler gegeben wird, wird jedes registrierte Widget e
 * ntsprechend skaliert.
 * 
 * @param Widget widget
 */
PureFW.WidgetManager.prototype.register_widget = function(widget) {
	if (!widget.instance_of(PureFW.Widget)) {
		throw new Error("First argument has to be a Widget-Object in "+
				"WidgetScalar::register_widget"+arguments.callee.caller);
	}
	
	this.registered_widgets.push(widget);
};
	
/**
 * Entfernt das angegebene Widget von der Scale-Überwachung. 
 * 
 * @param Widget widget
 */
PureFW.WidgetManager.prototype.unregister_widget = function(widget) {
	if (!widget.instance_of(PureFW.Widget)) {
		throw new Error("First argument has to be a Widget-Object in "+
			"WidgetScalar::unregister_widget");
	}
	
	this.registered_widgets.remove(widget);
};
	
/**
 * Setzt die Qualität der Bilder entsprechend der Angabe.
 * 
 * @param low|high		qual
 */
PureFW.WidgetManager.prototype.set_quality = function(qual) {
	var i, n = this.registered_widgets.length;
	for (i = 0; i < n; i++) {
		if (this.registered_widgets[i]) {
			this.registered_widgets[i].set_quality(qual);
		}
	}
};

/**
 * Durch den Aufruf der Funktion werden alle registrierten Widgets anhand der
 * übergebenen Größe und der statisch definierten Referenzgröße Skaliert.
 * @param value
 */
PureFW.WidgetManager.prototype.set_scale_value = function(value) {
	this.scale(value / this.constructor.scale_reference_value);
};

PureFW.WidgetManager.prototype.scale = function(factor) {
//		return;		//deactivated
	if (factor == this.scale_factor)
		return;	// nothing to do
	this.scale_factor = factor;
	var i, n = this.registered_widgets.length;
	for (i = 0; i < n; i++) {
		if (this.registered_widgets[i]) {
			this.registered_widgets[i].scale(factor);
		}
	}
	document.body.style.fontSize = (this.initial_font_size*factor)+"px";
};

PureFW.WidgetManager.prototype.refresh_fonts = function() {
	document.body.style.fontSize = 
		(this.initial_font_size*this.scale_factor)+"px";
}

PureFW.WidgetManager.prototype.get_scale_factor = function() {
	return this.scale_factor;
};

PureFW.WidgetManager.prototype.set_pic_path = function(path) {
	var i, n = this.registered_widgets.length;
	for (i = 0; i < n; i++) {
		if (this.registered_widgets[i]) {
			this.registered_widgets[i].set_pic_path(path);
		}
	}
};

PureFW.WidgetManager.prototype.repaint = function() {
	var i, n = this.registered_widgets.length;
	for (i = 0; i < n; i++) {
		if (this.registered_widgets[i]) {
			this.registered_widgets[i].repaint();
		}
	}
};

PureFW.WidgetManager.prototype.insert_into_dom_tree = function() {
	var i, n = this.registered_widgets.length;
	for (i = 0; i < n; i++) {
		if (this.registered_widgets[i]) {
			this.registered_widgets[i].insert_into_dom_tree();
		}
	}
};

/**
 * Nun wird eine Instanz erzeugt, die dazu da ist, ALLE Widgets zu vereinen.
 * Genannt ist sie PureFW.WidgetManager.manager_all.
 * 
 * Dieser Namenskonvention folgend könnte man z.B. für Icons eine eigene Gruppe
 * unter PureFW.WidgetManager.manager_icon anlegen.
 */
PureFW.WidgetManager.manager_all = new PureFW.WidgetManager();