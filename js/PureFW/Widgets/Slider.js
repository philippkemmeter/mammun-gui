/**
 * Repräsentiert einen Slider, mit welchem man Werte komfortabel einstellen kann.
 *
 * [===[]========]  <= also so etwas :) 
 *
 * @author Phil
 */
 
/**
 * Erstellt eine Instanz des Sliders
 * 
 * @param HTMLElement parent_node		Wo der Slider eingehängt werden soll
 * @param int x		[default: 0]		x-Position (in Pixeln)
 * @param int y		[default: 0]		y-Position (in Pixeln)
 * @param uint w	[default: 100]		Breite (in Pixeln)
 * @param uint h	[default: 19]		Höhe (in Pixeln)
 * @param uint min	[default: 0]		Minimalwert, der eingestellt werden kann
 * @param uint max	[default: 1000]		Maximalwert, der eingestellt werden kann
 * @param uint value[default: 0]		Initialwert
 */
PureFW.Slider = function(parent_node, x, y, w, h, min, max, value) {
	this.on_value_change_functions = new Array();
	this.on_slide_functions = new Array();
	if (typeof(parent_node) != 'undefined') {
		this.init();
		this.parent_node = parent_node || document.body;
		this.width = w || 100;
//		this.width = parent_node.style.width;
		this.height = h || 19;
		this.position = new PureFW.Point(x || 0, y || 0);
		this.min = min || 0;
		this.max = max || 1000;
		
		
		this.instance_num = this.constructor.num_instances;
		this.instance_name = "PureFW.Slider.instances["+this.instance_num+"]";
		this.node = null;
		this.id = 'Slider'+this.instance_num;
		this.slider_button_id = 'SliderB'+this.instance_num;
		this.value = value || 0;
		this.slider_pos = 0;
		this.old_value = this.value;
		
		this.left_arrow_container = null;
		this.left_arrow_url = 'ui/elements/sliders/arrow_left.png';
		
		this.bg_img_left_node = null;
		this.bg_img_left_url = 'ui/elements/sliders/bgl.png';
		this.bg_img_left_id = 'SliderBGL'+this.instance_num;
		this.bg_img_node = null;
		this.bg_img_url = 'ui/elements/sliders/bg.png';
		this.bg_img_id = 'SliderBG'+this.instance_num;
		this.bg_img_right_node = null;
		this.bg_img_right_url = 'ui/elements/sliders/bgr.png';
		this.bg_img_right_id = 'SliderBGR'+this.instance_num;
		
		this.right_arrow_container = null;
		this.right_arrow_url = 'ui/elements/sliders/arrow_right.png';
		
		this.slider_button_container = null;
		this.slider_button_url = 'ui/elements/sliders/button_19x19.png';
		
		this.insert_into_dom_tree();
		this._calculate_slider_pos();
		
		this.constructor.instances[this.instance_num] = this;
		this.constructor.num_instances++;
		PureFW.WidgetManager.manager_all.register_widget(this);
	}
};

/********************************************************************
* Slider extends Widget
********************************************************************/
PureFW.Slider.extend(PureFW.Widget);

/********************************************************************
 * Statische Deklarationen
 ********************************************************************/
PureFW.Slider.num_instances = 0;			   // Instanzüberwachung (Anzahl)
PureFW.Slider.instances = new Array();	       // Instanzüberwachung (Instanzen)
PureFW.Slider.active_dragging_instance = null; // Welche Instanz gerade draggt

PureFW.Slider.while_dragging = function(ev) {
	PureFW.Slider.active_dragging_instance.slider_pos = ev.detail.x;
	PureFW.Slider.active_dragging_instance._calculate_value();
	PureFW.Slider.active_dragging_instance.on_slide(
			PureFW.Slider.active_dragging_instance.create_event("slide", 
			PureFW.Slider.active_dragging_instance.get_value())
	);
};

PureFW.Slider.left_arrow_clicked = function(ev)
{
	this.parent_container.set_value(this.parent_container.value - 1);
}

PureFW.Slider.right_arrow_clicked = function(ev)
{
	this.parent_container.set_value(this.parent_container.value + 1);
}

PureFW.Slider.stop_dragging_wrapper = function(ev) {
	PureFW.Slider.active_dragging_instance._stop_dragging(ev.detail);
};


/********************************************************************
 * Prototype-Funktionen
 ********************************************************************/

/**
 * Fügt den Slider in den DOM-Baum ein
 */
PureFW.Slider.prototype.insert_into_dom_tree = function() {
	this.node = document.createElement('div');
	this.node.style.width = this.get_width() + "px";
	this.node.style.height = this.get_height() + "px";
	this.node.style.overflow = 'hidden';
	this.node.style.position = 'absolute';
	this.node.style.left = this.get_x()+"px";
	this.node.style.top = this.get_y()+"px";
	this.node.id=this.id;

	this.left_arrow_container = new PureFW.Container(
				this.node, 0, 0, this.height, this.height);
	this.left_arrow_container.set_bg_img(this.left_arrow_url);
	this.left_arrow_container.add_event_handler(
				"click", PureFW.Slider.left_arrow_clicked);
	this.left_arrow_container.set_z_index(2);
	this.left_arrow_container.parent_container = this;
	
	this.bg_img_left_node = document.createElement('img');
	this.bg_img_left_node.src = this.add_pic_path(this.bg_img_left_url);
	this.bg_img_left_node.style.position = 'absolute';
	this.bg_img_left_node.style.width = 4*this.scale_factor+"px";
	this.bg_img_left_node.style.height = "100%";
	this.bg_img_left_node.style.left = this.get_height() + "px";
	this.bg_img_left_node.setAttribute("onmousedown", 
			"javascript: return false;");
	this.bg_img_left_node.id = this.bg_img_left_id;
	
	this.bg_img_node = document.createElement('img');
	this.bg_img_node.src = this.add_pic_path(this.bg_img_url);
	this.bg_img_node.style.height = "100%";
	this.bg_img_node.style.width = (this.get_width() - 8*this.scale_factor
			- (this.get_height() << 1)) + "px";
	this.bg_img_node.style.position = 'absolute';
	this.bg_img_node.style.left = this.get_height() + 4*this.scale_factor + "px";
	this.bg_img_node.setAttribute("onmousedown", "javascript: return false;");
	this.bg_img_node.id = this.bg_img_id;
	
	this.bg_img_right_node = document.createElement('img');
	this.bg_img_right_node.src = this.add_pic_path(this.bg_img_right_url);
	this.bg_img_right_node.style.position = 'absolute';
	this.bg_img_right_node.style.width = 4*this.scale_factor+"px";
	this.bg_img_right_node.style.height = "100%";
	this.bg_img_right_node.style.left = (
			this.get_width() - this.get_height() - 4*this.scale_factor) + "px";
	this.bg_img_right_node.setAttribute("onmousedown",
		"javascript: return false;");
	this.bg_img_right_node.id = this.bg_img_right_id;

	this.right_arrow_container = new PureFW.Container(
			this.node, 0, 0, this.height, this.height);
	this.right_arrow_container.set_bg_img(this.right_arrow_url);
	this.right_arrow_container.add_event_handler(
			"click", PureFW.Slider.right_arrow_clicked);
	this.right_arrow_container.set_z_index(2);
	this.right_arrow_container.set_x(this.width  - this.height);
	this.right_arrow_container.parent_container = this;
	
	this.slider_button_container = new PureFW.Container(this.node, 0, 0, 
				this.height, this.height);
	this.slider_button_container.set_id(this.slider_button_id);
	this.slider_button_container.set_bg_img(this.slider_button_url);
	this.slider_button_container.set_content(this.get_value());
	this.slider_button_container.add_event_handler(
		"mousedown",
		(function(_instance) {
			return function(ev) {
				_instance._start_dragging();
			}
		})(this)
	);
	this.slider_button_container.get_node().style.cursor = "pointer";
	this.slider_button_container.set_z_index(3);
	this.slider_button_container.parent_container = this;
	this.slider_button_container.get_node().style.textAlign = 'center';
		
	this.parent_node.appendChild(this.node);
	this.node.appendChild(this.bg_img_left_node);
	this.node.appendChild(this.bg_img_node);
	this.node.appendChild(this.bg_img_right_node);

	this.disable_selection();
	PureFW.Widget.disable_selection(this.bg_img_node);
	PureFW.Widget.disable_selection(this.bg_img_right_node);
	PureFW.Widget.disable_selection(this.bg_img_left_node);
};

/**
 * Setzt den Minimalwert, der über den Slider eingestellt werden kann. Bei 
 * diesem Wert ist der Slider also ganz links.
 * Ist der hier übergebene Wert größer als das eingestellte Maximum, so wird
 * das Maximum entsprechend erhöht, so dass immer gilt min < max.
 * Ist der eingestellte aktuelle Wert hierdurch kleiner als das Minimum, 
 * wird dieser ebenso korrigiert, so dass immer gilt min <= value.
 * 
 * @param uint min
 */
PureFW.Slider.prototype.set_min = function(min) {
	if (min >= this.max)
		this.set_max(min+1);
	if (this.value < min)
		this.set_value(min);
	this.min = min;
	this._calculate_value();
};

/**
 * Setzt den Maximalwert, der über den Slider eingestellt werden kann. Bei 
 * diesem Wert ist der Slider also ganz rechts.
 * Ist der hier übergebene Wert kleiner als das eingestellte Minimum, so wird
 * das Minimum entsprechend verringert, so dass immer gilt min < max.
 * Ist der eingestellte aktuelle Wert hierdurch größer als das Maximum,
 * wird dieser ebenso korrigiert, so dass immer gilt max >= value.
 * 
 * @param uint max
 */
PureFW.Slider.prototype.set_max = function(max) {
	if (max < this.min)
		this.set_min(max-1);
	this.max = max;
	this._calculate_value();		
};

/**
 * Setzt den aktuell eingestellten Wert.
 * 
 * @param uint value
 */
PureFW.Slider.prototype.set_value = function(value) {
	this.old_value = this.value;
	this.value = value;
	this._calculate_slider_pos();
	this.on_value_change(this.create_event("value_change", value));
};

/**
 * Fügt dem Slider eine Ereignisbehandlungsroutine hinzu, die beim Ereignis type
 * aufgerufen wird. Zusätzlich zu den Werten für type, die PureFW.Widget
 * unterstützt, wird "value_change" unterstützt. Das "Value-Change"-Event wird
 * dabei aufgerufen, wenn sich der Wert des Sliders ändert.
 * 
 * @param string type
 * @param funtion fn
 * @see PureFW.Widget.add_event_handler
 */
PureFW.Slider.prototype.add_event_handler = function(type, fn) {
	if (type === "value_change") {
		this.on_value_change_functions.push(fn);
	}
	else if (type === "slide") {
		this.on_slide_functions.push(fn);
	}
	else {
		PureFW.Slider.parent.add_event_handler.call(this, type, fn);
	}
};

/**
 * Entfernt dem Slider den angegebenen Event Handler , der beim Ereignis type
 * aufgerufen wird. Zusätzlich zu den Werten für type, die PureFW.Widget
 * unterstützt, wird "value_change" unterstützt. Das "Value-Change"-Event wird
 * dabei aufgerufen, wenn sich der Wert des Sliders ändert.
 * 
 * @param string type
 * @param funtion fn
 * @see PureFW.Widget.remove_event_handler
 */
PureFW.Slider.prototype.remove_event_handler = function(type, fn) {
	if (type === "value_change") {
		this.on_value_change_functions.remove(fn);
	}
	else if (type === "slide") {
		this.on_slide_functions.remove(fn);
	}
	else {
		PureFW.Slider.parent.remove_event_handler.call(this, type, fn);
	}
};

/**
 * Feuert das on_value_change-Event an alle registrierten Funktionen.
 * 
 * @param Event ev [optional]
 */
PureFW.Slider.prototype.on_value_change = function(ev) {
	if (!ev)
		ev = this.create_event("value_change");
	var n = this.on_value_change_functions.length;
	for (var i = 0; i < n; i++)
		this.on_value_change_functions[i](ev);
};

/**
 * Feuert das slide-Event an alle registrierten Funktionen.
 * 
 * @param Event ev [optional]
 */
PureFW.Slider.prototype.on_slide = function(ev) {
	if (!ev)
		ev = this.create_event("slide");
	var n = this.on_slide_functions.length;
	for (var i = 0; i < n; i++)
		this.on_slide_functions[i](ev);
};

/**
 * Gibt den Wert zurück, den der Slider gerade repräsentiert.
 * 
 * @return uint
 */
PureFW.Slider.prototype.get_value = function() {
	return this.value;
};

/**
 * Gibt den Wert zurück, den der Slider vor der letzten Änderung hatte. Dies
 * ist z.B. nützlich, wenn man wissen will, um wieviel sich der Slider durch
 * die letzte Aktion bewegt hat, ohne dass man dies von außen kontrollieren
 * müsste.
 * 
 * @return uint
 */
PureFW.Slider.prototype.get_old_value = function() {
	return this.old_value;
};

PureFW.Slider.prototype.repaint = function() {
	try {
		PureFW.Slider.parent.repaint.call(this);
		
		this.bg_img_left_node = document.getElemById(this.bg_img_left_id);
		this.bg_img_left_node.src = this.add_pic_path(this.bg_img_left_url);
		this.bg_img_left_node.style.width = 4*this.scale_factor+"px";
		this.bg_img_left_node.style.left = this.get_height() + "px";
		
		this.bg_img_node = document.getElemById(this.bg_img_id);
		this.bg_img_node.src = this.add_pic_path(this.bg_img_url);
		this.bg_img_node.style.width = (this.get_width() - 8*this.scale_factor
				- (this.get_height() << 1)) + "px";
		this.bg_img_node.style.left = this.get_height() + 4*this.scale_factor 
			+ "px";
	
		this.bg_img_right_node = document.getElemById(this.bg_img_right_id);
		this.bg_img_right_node.src = this.add_pic_path(this.bg_img_right_url);
		this.bg_img_right_node.style.width = 4*this.scale_factor+"px";
		this.bg_img_right_node.style.left = (this.get_width()  -
					this.get_height() - 4*this.scale_factor) + "px";
		
		this._calculate_slider_pos();
	}
	catch(e){
		delete this;
	}
};

/**
 * PRIVATE: Startet mit dem Draggen des Slider-Buttons.
 */
PureFW.Slider.prototype._start_dragging = function() {
	PureFW.MouseFeatures.Dragging.start_dragging(
		document.getElemById(this.slider_button_id)
	);
	PureFW.MouseFeatures.Dragging.restrict_direction(
		PureFW.MouseFeatures.Dragging.RESTRICT_MOVING_X
	);
	PureFW.MouseFeatures.Dragging.set_pos_limits(
		this.get_height() - this.scale_factor,
		false,
		this.get_width()-19*this.scale_factor - 
			this.get_height() - this.scale_factor,
		false
	);
	PureFW.MouseFeatures.Dragging.add_event_handler("dragging",
		this.constructor.while_dragging
	);
	PureFW.MouseFeatures.Dragging.add_event_handler("drop",
		this.constructor.stop_dragging_wrapper
	);
	this.constructor.active_dragging_instance = this;
};

/**
 * PRIVATE: Stoppt mit dem Draggen des Slider-Buttons.
 */
PureFW.Slider.prototype._stop_dragging = function() {
	this._calculate_value();
	this.on_value_change(this.create_event("value_change", this.value));
};

/**
 * PRIVATE: Berechnet den aktuellen Wert anhand der Sliderposition. Es wird 
 * dabei KEIN Event gefeuert!
 */
PureFW.Slider.prototype._calculate_value = function() {
	this.old_value = this.value;
	this.value = Math.round((this.slider_pos+ this.min - this.height)/
			(this.width - 19 - (this.height<<1))
			*(this.max - this.min) / this.scale_factor
		);
	this.slider_button_container.set_content(this.value);
};

/**
 * PRIVATE: Berechnet und setzt die Position des Slider-Buttons anhand des
 * Wertes.
 */
PureFW.Slider.prototype._calculate_slider_pos = function() {
	this.slider_pos = this.height + Math.round(this.value/(this.max-this.min)*
			(this.width- 19 - (this.height<<1))
			- this.min);
	this.slider_button_container.set_x(this.slider_pos);
};