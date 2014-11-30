/**
 * Kombination aus Radiobutton und einem Container, der die Beschriftung
 * des Buttons enthalten kann.
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
 * @param uint w_radio
 * @param string pic_url	Pfad zum Checkbox-Doppelbild [oben unchecked, 
 * 															unten checked]
 */
PureFW.RadioLabel = function(parent_node, x, y, w, h, w_radio, pic_url) {
	if (typeof(parent_node) !== 'undefined') {
		this.init(parent_node, x, y, w, h, w_radio, pic_url);
	
		this.instance_num = this.constructor.num_instances;
		this.instance_name = 
				"PureFW.RadioLabel.instances["+this.instance_num+"]";
	
		this.insert_into_dom_tree();
		this.constructor.num_instances++;
		this.constructor.instances[this.instance_num] = this;
		
		PureFW.WidgetManager.manager_all.register_widget(this);
	}
};

/********************************************************************
 * RadioLabel extends Widget
 ********************************************************************/
PureFW.RadioLabel.extend(PureFW.Widget);

/********************************************************************
 * Statische Deklarationen
 ********************************************************************/
PureFW.RadioLabel.num_instances = 0;
PureFW.RadioLabel.instances = new Array();

/********************************************************************
 * Prototyp-Deklarationen
 ********************************************************************/
PureFW.RadioLabel.prototype.init = function(parent_node, x, y, w, h, w_radio, 
	pic_url) 
{
	PureFW.RadioLabel.parent.init.call(this, parent_node, x, y, w, h);
	this.radio_pic_url = pic_url;
	this.radio_w = w_radio;
	this.radio_button = null;
	this.label_container = null;
};

PureFW.RadioLabel.prototype.insert_into_dom_tree = function() {
	PureFW.RadioLabel.parent.insert_into_dom_tree.call(this);

	this.radio_button = new PureFW.RadioButton(
		this.parent_node,
		this.position.x,
		this.position.y,
		this.radio_w, this.radio_w,
		this.radio_pic_url
	);
	this.label_container = new PureFW.Container(
		this.parent_node,
		this.radio_button.position.x +
			this.radio_button.width + 5,
		this.radio_button.position.y,
		this.width - this.radio_button.width - 5,
		this.height
	);
	this.label_container.add_event_handler(
		"click",
		(function (_instance) {
			return function(ev) {
				_instance.radio_button.on_click();
			}
		})(this)
	);
};

PureFW.RadioLabel.prototype.get_label_container = function() {
	return this.label_container;
};

PureFW.RadioLabel.prototype.get_radio_button = function() {
	return this.radio_button;
};