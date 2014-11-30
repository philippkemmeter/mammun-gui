/**
 * Ein Radio-Button (gruppierbar). Wird einer angehakt, werden die anderen
 * abgehakt. Jeder Radio-Button feuert dabei ein oncheck / onuncheck event.
 * 
 * @author Phil
 */

/**
 * Ein einzelner Radiobutton. Dieser soll nur von Hand erstellt werden, sondern
 * immer eine Gruppe von Buttons, da ein einzelner Button sonst keinen Nutzen
 * hat.
 * 
 * @param HTMLElement parent_node
 * @param int x
 * @param int y
 * @param uint w
 * @param uint h
 * @param string pic_url	Pfad zum Checkbox-Doppelbild [oben unchecked, 
 * 															unten checked]
 */
PureFW.RadioButton = function(parent_node, x, y, w, h, pic_url) {
	if (typeof(parent_node) !== 'undefined') {
		this.init(parent_node, x, y, w, h, pic_url);
	
		this.instance_num = this.constructor.num_instances;
		this.instance_name = 
				"PureFW.RadioButton.instances["+this.instance_num+"]";
		
		this.id = "RadioButton"+this.instance_num;
		this.pic_id = "RadioButton_pic"+this.instance_num;
		
		this.insert_into_dom_tree();
		this.constructor.num_instances++;
		this.constructor.instances[this.instance_num] = this;
		
		PureFW.WidgetManager.manager_all.register_widget(this);
	}
};

/********************************************************************
 * RadioButton extends Checkbox
 ********************************************************************/
PureFW.RadioButton.extend(PureFW.Checkbox);

/********************************************************************
 * Statische Deklarationen
 ********************************************************************/
PureFW.RadioButton.num_instances = 0;
PureFW.RadioButton.instances = new Array();

/********************************************************************
 * Prototyp-Deklarationen
 ********************************************************************/
PureFW.RadioButton.prototype.init = function(parent_node, x, y, w, h, pic_url) {
	PureFW.RadioButton.parent.init.call(this, parent_node, x, y, w, h, pic_url);
	this.radio_button_links = new Array();
	this.group_visit_counter = 0;
};
PureFW.RadioButton.prototype.insert_into_dom_tree = function() {
	PureFW.RadioButton.parent.insert_into_dom_tree.call(this);

	/**
	 * Die on_click-Funktionen des Buttons überschreiben, dass das
	 * Widget nicht getogglet, sondern angehakt wird.
	 * Da die genaue Signatur der Registrierten Funktion nicht bekannt ist,
	 * werden nun einfach (HACK!) alle Funktionen deregistriert und alles
	 * neu aufgebaut.
	 */
	while (this.on_click_functions.length)
		this.on_click_functions.pop();
	this.add_event_handler(
		"click",
		(function(instance) {
			return function(ev) {
				instance.group_visit_counter = 1;
				instance.uncheck_all_in_group();
				instance.check();
			}
		})(this)
	);
};

/**
 * Erstellt eine Verbindung mit einem anderen RadioButton. Alle Verbindungen
 * des nun verbundenen RadioButtons werden dann bei der Traversierung 
 * mitberücksichtigt, so dass eine geschlossene Gruppe entsteht. Somit ist es
 * möglich, das typische Radio-Button-Verhalten ohne ein weiteres Verwaltungs-
 * Objekt zu realisieren. 
 * 
 * !! Die Radio-Buttons müssen Zyklisch verknüpft werden !!
 * 
 * @param radio_button
 */
PureFW.RadioButton.prototype.link_with_radio_button = function(radio_button) {
	this.radio_button_links.push(radio_button);
};

/**
 * Diese Funktion hakt alle Radiobuttons ab, die durch die Verknüpfungsketten
 * zu finden sind.
 * 
 * !! Die Radio-Buttons müssen Zyklisch verknüpft werden !!
 */
PureFW.RadioButton.prototype.uncheck_all_in_group = function() {
	if (this.group_visit_counter == 1) {
		this.group_visit_counter = 2;
	}
	else if (this.group_visit_counter > 1) {
		this.group_visit_counter = 0;
		return;
	}
	var n = this.radio_button_links.length;
	for (var i = 0; i < n; i++) {
		this.radio_button_links[i].uncheck();
		this.radio_button_links[i].uncheck_all_in_group();
	}
};