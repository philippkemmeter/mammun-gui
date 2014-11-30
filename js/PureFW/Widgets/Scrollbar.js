/**
 * Eine Scrollbar. Sie beherrscht animiertes Scrollen, Step-Scrollen usw.
 * Die Scrollbar selbst, scrollt dabei nichts wirklich, sondern feuert nur
 * Events, die angeben, um wieviel in welche Richtung jetzt gescrollt werden
 * müsste. Somit kann diese Scrollbar auch für alles andere benutzt werden,
 * nicht nur zum Scrollen.
 * 
 * @author Alex
 */

/**
 * Erstellt eine neue Scrollbar.
 * 
 * @param HTMLElement parent_node	Hier wird sie in den DOM-Baum gehängt
 * @param uint x					x-Position
 * @param uint y					y-Position
 * @param uint w					Breite
 * @param uint l					Länge
 * @param bool vertical				Ob vertikal, sonst horizontal
 */
PureFW.Scrollbar = function (parent_node, x, y, w, l, vertical) {
	if (typeof(parent_node) !== 'undefined') {
		this.init();
		this.parent_node = parent_node || document.body;
		this.position = new PureFW.Point(x || 0, y || 0);
		this.width = w || 10;
		this.height = l || 100;
		this.vertical = vertical ? !!vertical : false;
		this.arrow1_pic_url = '';
		this.arrow2_pic_url = '';
		this.inst_num = this.constructor.num_instances;
		this.id = "Scrollbar"+this.inst_num;
		this.node = null;
		this.arrow1_node = this.arrow2_node = this.bar_node = null;
		
		this.document_size = false;	// Wie groß das Dokument ist, das hierüber gescrollt
								// werden soll.
		this.document_pos = 0;
		
		this.scroll_step = 5;	// um wie viel Pixel pro Scroll-Event 
								// (also pro Click) gescrollt werden soll
		this.scroll_interval = 20;	// Intervall zwischen den einzelnen Steps
									// Ist dieser Wert klein, wird das Halten
									// zum scrollen mehr unterstützt. ist er groß,
									// Gibt es "Schubweises" Scrollen pro Click
		this.scroll_plus_interval_handler = null;
		this.scroll_minus_interval_handler = null;
		
		// scroll_ani_interval wird benutzt, wenn scroll_step und scroll_interval
		// sehr hoch sind, um eine Animation zwischen den steps zu gewährleisten.
		// Wenn scroll_ani_interval == 0, dann wird nicht animiert.
		this.scroll_ani_step = 8;
		this.scroll_ani_interval = 0;
		this.scroll_ani_plus_interval_handler = null;
		this.scroll_ani_minus_interval_handler = null;
		this.scroll_ani_steps_left = 0;
		this.scroll_ani_is_running = false;
		
		this.on_scroll_functions = new Array();
		
		this.constructor.instances[this.inst_num] = this;
		this.constructor.num_instances++;
		this.insert_into_dom_tree();
		PureFW.WidgetManager.manager_all.register_widget(this);
	}
};

/********************************************************************
 * Scrollbar extends Widget
 ********************************************************************/
PureFW.Scrollbar.extend(PureFW.Widget);

/********************************************************************
 * Statische Deklarationen
 ********************************************************************/
PureFW.Scrollbar.num_instances = 0;
PureFW.Scrollbar.instances = new Array();

/********************************************************************
 * Prototype-Funktionen
 ********************************************************************/

/**
 * Fügt die Scrollbar in den DOM-Baum ein.
 * 
 * @throws Error
 */
PureFW.Scrollbar.prototype.insert_into_dom_tree = function() {
	PureFW.Scrollbar.parent.insert_into_dom_tree.call(this);
	this.node = document.createElement('div');
	this.node.style.position = 'absolute';
	this.node.style.left = this.get_x() + "px";
	this.node.style.top = this.get_y() + "px";
	this.node.id = this.id;
	
	this.arrow1_node = document.createElement('div');
	this.arrow1_node.setAttribute("onmousedown", "javascript: PureFW.Scrollbar."+
				"instances["+this.inst_num+"]._scroll_minus_start();");
	this.arrow1_node.setAttribute("onmouseup", "javascript: PureFW.Scrollbar."+
				"instances["+this.inst_num+"]._scroll_minus_stop();");
	this.arrow1_node.setAttribute("onmouseout", "javascript: PureFW.Scrollbar."+
				"instances["+this.inst_num+"]._scroll_minus_stop();");
	this.arrow1_node.style.width = this.get_width() + "px";
	this.arrow1_node.style.height = this.get_width() + "px";
	this.arrow1_node.style.backgroundRepeat = 'no-repeat';
	this.arrow1_node.style.backgroundPosition = 'center';
	this.arrow1_node.style.position = 'absolute';
	this.arrow1_node.style.cursor = 'pointer';
	
	this.bar_node = document.createElement('div');
	this.bar_node.style.position = 'absolute';
	
	this.arrow2_node = document.createElement('div');
	this.arrow2_node.setAttribute("onmousedown", "javascript: PureFW.Scrollbar."+
				"instances["+this.inst_num+"]._scroll_plus_start();");
	this.arrow2_node.setAttribute("onmouseup", "javascript: PureFW.Scrollbar."+
				"instances["+this.inst_num+"]._scroll_plus_stop();");
	this.arrow2_node.setAttribute("onmouseout", "javascript: PureFW.Scrollbar."+
				"instances["+this.inst_num+"]._scroll_plus_stop();");
	this.arrow2_node.style.width = this.get_width() + "px";
	this.arrow2_node.style.height = this.get_width() + "px";
	this.arrow2_node.style.backgroundRepeat = 'no-repeat';
	this.arrow2_node.style.backgroundPosition = 'center';
	this.arrow2_node.style.position = 'absolute';
	this.arrow2_node.style.cursor = 'pointer';

	if (this.vertical) {
		this.node.style.width = this.get_width() + "px";
		this.node.style.height = this.get_height() + "px";
		
		this.arrow1_node.innerHTML = '⇑';
		this.arrow1_node.style.left = "0px";
		this.arrow1_node.style.top = "0px";
		
		this.bar_node.style.width = this.get_width() + "px";
		this.bar_node.style.height = this.get_height() - 
			(this.get_width()<<1) + "px";
		this.bar_node.style.left = "0px";
		this.bar_node.style.top = this.get_width() + "px";
		
		this.arrow2_node.innerHTML = '⇓';
		this.arrow2_node.style.left = "0px";
		this.arrow2_node.style.top = (this.get_height() - this.get_width()) + "px";
	}
	else {
		this.node.style.width = this.get_height() + "px";
		this.node.style.height = this.get_width() + "px";
		
		this.arrow1_node.innerHTML = '⇐';
		this.arrow1_node.style.left = "0px";
		this.arrow1_node.style.top = "0px";

		this.bar_node.style.width = this.get_height() - 
			(this.get_width()<<1) + "px";
		this.bar_node.style.height = this.get_width() + "px";
		this.bar_node.style.left = this.get_width() + "px";
		this.bar_node.style.top =  "0px";
		
		this.arrow2_node.innerHTML = '⇒';
		this.arrow2_node.style.left = (this.get_height()- this.get_width()) + "px";
		this.arrow2_node.style.top = "0px";
	}
	
	this.node.appendChild(this.arrow1_node);
	this.node.appendChild(this.bar_node);
	this.node.appendChild(this.arrow2_node);
	this.parent_node.appendChild(this.node);
};

/**
 * Setzt die Bilder für die Scrollbar-Pfeile. first_arrow ist dabei der Pfeil, 
 * der oben bei vertikaler, und links bei horizontaler Richtung zu sehen ist,
 * second_arrow ist der andere.
 * 
 * @param URI first_arrow_pic_url
 * @param URI second_arrow_pic_url
 */
PureFW.Scrollbar.prototype.set_arrow_pics = function(first_arrow_pic_url, 
		second_arrow_pic_url)
{
	this.arrow1_pic_url = first_arrow_pic_url;
	this.arrow2_pic_url = second_arrow_pic_url;
	this.repaint();
};

/**
 * Setzt den Schritt, um den gesprungen werden soll, wenn der Scrollbutton
 * 1x angeklickt wird.
 * 
 * @param uint step
 */
PureFW.Scrollbar.prototype.set_scroll_step = function(step) {
	this.scroll_step = step;
};

/**
 * Setzt, nach wie vielen Millisekunden das Scroll-Event wieder aufgerufen
 * werden soll, wenn der Nutzer die Maustaste gedrückt hält.
 * 
 * @param uint interval
 */
PureFW.Scrollbar.prototype.set_scroll_interval = function(interval) {
	this.scroll_interval = interval;
};

/**
 * Ob die Scrollsprünge zusätzlich animiert werden sollen. Macht nur Sinn,
 * wenn scroll_interval und scroll_step entsprechend hoch sind. So kann man
 * animiert zu einem gewissen Punkt springen.
 * 
 * @param bool b
 */
PureFW.Scrollbar.prototype.set_animate_scroll = function(b) {
	this.scroll_ani_interval = (b) ? 20 : 0;
};

/**
 * Setzt, wie groß das zu scrollende Dokument in Pixeln ist. Wenn dieser Wert
 * angegeben wird, dann wird automatisch darauf geachtet, dass nicht zu weit 
 * gescrollt werden kann. Zudem sind dann Aussagen über die aktuelle
 * Position im Dokument i.a. möglich.
 * 
 * @param uint size
 */
PureFW.Scrollbar.prototype.set_document_size = function(size) {
	this.document_size = size;
};
/**
 * Zeichnet die Scrollbar neu.
 */
PureFW.Scrollbar.prototype.repaint = function() {
	this.node.style.left = this.get_x() + "px";
	this.node.style.top = this.get_y() + "px";
	
	this.arrow1_node.style.width = this.get_width() + "px";
	this.arrow1_node.style.height = this.get_width() + "px";
	if (this.arrow1_pic_url)
		this.arrow1_node.style.backgroundImage = 'url('+this.add_pic_path( 
			this.arrow1_pic_url)+')';
	else {
		this.arrow1_node.style.backgroundImage = '';
		this.arrow1_node.innerHTML = (this.vertical) ? '⇑' : '⇐';
	}
	
	this.arrow2_node.style.width = this.get_width() + "px";
	this.arrow2_node.style.height = this.get_width() + "px";
	if (this.arrow2_pic_url)
		this.arrow1_node.style.backgroundImage = 'url('+this.add_pic_path( 
			this.arrow2_pic_url)+')';
	else {
		this.arrow2_node.style.backgroundImage = '';
		this.arrow2_node.innerHTML = (this.vertical) ? '⇓' : '⇒';
	}
	
	if (this.vertical) {
		this.node.style.width = this.get_width() + "px";
		this.node.style.height = this.get_height() + "px";
		
		this.arrow1_node.style.left = "0px";
		this.arrow1_node.style.top = "0px";
		
		this.bar_node.style.width = this.get_width() + "px";
		this.bar_node.style.height = this.get_height() - 
			(this.get_width()<<1) + "px";
		this.bar_node.style.left = "0px";
		this.bar_node.style.top = this.get_width() + "px";
		
		this.arrow2_node.style.left = "0px";
		this.arrow2_node.style.top = (this.get_height() - this.get_width()) + "px";
	}
	else {
		this.node.style.width = this.get_height() + "px";
		this.node.style.height = this.get_width() + "px";
		
		this.arrow1_node.style.left = "0px";
		this.arrow1_node.style.top = "0px";
		
		this.bar_node.style.width = this.get_height() - 
			(this.get_width()<<1) + "px";
		this.bar_node.style.height = this.get_width() + "px";
		this.bar_node.style.left = this.get_width() + "px";
		this.bar_node.style.top =  "0px";
		
		this.arrow2_node.style.left = (this.get_height()- this.get_width()) + "px";
		this.arrow2_node.style.top = "0px";
	}
};

/**
 * Triggert das Scrollen um einen Schritt nach unten/rechts. Wird normalerweise 
 * nicht direkt aufgerufen, sondern durch anklicken der entsprechenden 
 * Scroll-Buttons.
 */
PureFW.Scrollbar.prototype.scroll_plus = function() {
	if (this._step_animation_enabled()) {
		// Animation gewünscht, aber der Aufruf wurde nicht von der Animation
		// heraus getätigt bzw. nicht innerhalb einer Animation.
		if (!this.scroll_ani_is_running) {
			this._scroll_ani_plus_start();	// Animation wird gestartet
			return;
		}
		if (this.scroll_ani_steps_left <= 0) {	// Animation zu Ende
			this._scroll_ani_plus_stop();
			return;
		}
		var step = this.scroll_ani_step;
		this.scroll_ani_steps_left -= step;
	}
	else {	// keine Animation: direkter Sprung
		var step = this.scroll_step;
	}
	
	this.document_pos++;
	if ((this.document_size !== false) && 
			(this.document_size >= this.document_pos)) 
	{
		this.document_pos = this.document.size();
		this.stop_scroll();
	}
	
	this.on_scroll(this.create_event("scroll", step));
};
/**
 * Triggert das Scrollen um einen Schritt nach oben/links. Wird normalerweise 
 * nicht direkt aufgerufen, sondern durch anklicken der entsprechenden 
 * Scroll-Buttons.
 */
PureFW.Scrollbar.prototype.scroll_minus = function() {
	if (this._step_animation_enabled()) {
		// Animation gewünscht, aber der Aufruf wurde nicht von der Animation
		// heraus getätigt bzw. nicht innerhalb einer Animation.
		if (!this.scroll_ani_is_running) {
			this._scroll_ani_minus_start();	// Animation wird gestartet
			return;
		}
		if (this.scroll_ani_steps_left <= 0) {	// Animation zu Ende
			this._scroll_ani_minus_stop();
			return;
		}
		var step = this.scroll_ani_step;
		this.scroll_ani_steps_left -= step;
	}
	else {	// keine Animation: direkter Sprung
		var step = this.scroll_step;
	}
	
	this.document_pos--;
	if ((this.document_size !== false) &&
			(this.document_size <= 0)) 
	{
		this.document_pos = 0;
		this.stop_scroll();
	}
	
	this.on_scroll(this.create_event("scroll", step));
	var content_style = document.getElemById(this.content_id).style;
};

/**
 * Stoppt das Scrollen direkt. Sollte aufgerufen werden, wenn das Dokument, das
 * gescrollt wird, am Ende angelangt ist.
 */
PureFW.Scrollbar.prototype.stop_scroll = function() {
	this._scroll_minus_stop();
	this._scroll_ani_minus_stop();
	this._scroll_plus_stop();
	this._scroll_ani_plus_stop();
};

/**
 * Fügt einen EventHandler hinzu. Funktion überschrieben, da zusätzlich
 * der Typ "scroll" unterstützt wird, welcher beim Scrollen ausgelöst wird.
 * Unter event.detail ist der Pixelwert, um den gescrollt werden soll,
 * abgelegt.
 * 
 * @param string type
 * @param Function fn
 * @see Widget.add_event_handler
 */
PureFW.Scrollbar.prototype.add_event_handler = function(type, fn) {
	if (type === "scroll")
		this.on_scroll_functions.push(fn);
	else
		PureFW.Container.parent.add_event_handler.call(this, type, fn);
};

/**
 * Entfernt einen EventHandler. Funktion überschrieben, da zusätzlich
 * der Typ "scroll" unterstützt wird,  welcher beim Scrollen ausgelöst wird.
 * 
 * @param string type
 * @param Function fn
 * @see Scrollbar.prototype.remove_event_handler
 */
PureFW.Scrollbar.prototype.remove_event_handler = function(type, fn) {
	if (type === "change")
		this.on_change_functions.remove(fn);
	else
		PureFW.Container.parent.remove_event_handler.call(this, type, fn);
};

/**
 * Ruft das "on-scroll"-Event exlizit auf. Wird normalerweise aufgerufen,
 * wenn durch anklicken der Scrollbuttons ein Scroll-Event entsteht.
 * 
 * @param Event ev [optional]
 */
PureFW.Scrollbar.prototype.on_scroll = function(ev) {
	if (!ev) {
		ev = this.create_event("scroll");
	}
		
	var n = this.on_scroll_functions.length;
	for (var i = 0; i < n; i++) {
		this.on_scroll_functions[i].call(this, ev);
	}
};

/********************************************************************
 * Ab hier alles Private!
 ********************************************************************/


/**
 * Startet das Plus-Scrollen per durch mehrfachen Aufruf von scroll_plus()
 * im Interval this.scroll_interval;
 */
PureFW.Scrollbar.prototype._scroll_plus_start = function() {
	this.scroll_plus();
	this.scroll_plus_interval_handler = 
		window.setInterval(
			"PureFW.Scrollbar.instances["+this.inst_num+"].scroll_plus()",
			this.scroll_interval
		);
};
/**
 * Stoppt das Plus-Scrollen.
 */
PureFW.Scrollbar.prototype._scroll_plus_stop = function() {
	try {
		window.clearInterval(this.scroll_plus_interval_handler);
	}
	catch (e) {}
};
/**
 * Startet das Minus-Scrollen per durch mehrfachen Aufruf von scroll_minus()
 * im Interval this.scroll_interval;
 */
PureFW.Scrollbar.prototype._scroll_minus_start = function() {
	this.scroll_minus();
	this.scroll_minus_interval_handler = 
		window.setInterval(
			"PureFW.Scrollbar.instances["+this.inst_num+"].scroll_minus()",
			this.scroll_interval
		);
};
/**
 * Stoppt das Minus-Scrollen.
 */
PureFW.Scrollbar.prototype._scroll_minus_stop = function() {
	try {
		window.clearInterval(this.scroll_minus_interval_handler);
	}
	catch (e) {}
};
/**
 * Startet eine Animation zum Überbrücken der Plus-Scroll-Ereignisse
 */
PureFW.Scrollbar.prototype._scroll_ani_plus_start = function() {
	if (this._step_animation_enabled()) 
	{
		this.scroll_ani_steps_left = this.scroll_step;
		this.scroll_ani_is_running = true;
		this.scroll_ani_plus_interval_handler =
			window.setInterval(
				"PureFW.Scrollbar.instances["+this.inst_num+"].scroll_plus()",
				this.scroll_ani_interval
			);
	}
};
/**
 * Stoppt die Animation zum Überbrücken der Plus-Scroll-Ereignisse
 */
PureFW.Scrollbar.prototype._scroll_ani_plus_stop = function() {
	this.scroll_ani_steps_left = 0;
	this.scroll_ani_is_running = false;
	try {
		window.clearInterval(this.scroll_ani_plus_interval_handler);
	}
	catch (e) {}
};
/**
 * Startet eine Animation zum Überbrücken der Minus-Scroll-Ereignisse
 */
PureFW.Scrollbar.prototype._scroll_ani_minus_start = function() {
	if (this._step_animation_enabled()) 
	{
		this.scroll_ani_steps_left = this.scroll_step;
		this.scroll_ani_is_running = true;
		this.scroll_ani_minus_interval_handler =
			window.setInterval(
				"PureFW.Scrollbar.instances["+this.inst_num+"].scroll_minus()",
				this.scroll_ani_interval
			);
	}
};
/**
 * Stoppt die Animation zum Überbrücken der Minus-Scroll-Ereignisse
 */
PureFW.Scrollbar.prototype._scroll_ani_minus_stop = function() {
	this.scroll_ani_steps_left = 0;
	this.scroll_ani_is_running = false;
	try {
		window.clearInterval(this.scroll_ani_minus_interval_handler);
	}
	catch (e) {}
};
/**
 * Gibt zurück, ob eine Animation gewünscht und sinnvoll ist. Wenn das Scroll-
 * Interval oder der Scroll-Step sehr klein sind, macht es keinen Sinn diese
 * Einzelereignisse mit einer Animation zu überbrücken. Bei großen Werten ist
 * das eher Sinnvoll. Beispiel:
 * Scroll-Step ist 100 (also Pro Click soll 100Pixel weit gescrollt werden).
 * Diesen Sprung kann man sehr gut mit einer Animation überbrücken.
 */
PureFW.Scrollbar.prototype._step_animation_enabled = function() {
	return ((this.scroll_ani_interval > 0) 
		&& ((this.scroll_ani_step*this.scroll_ani_interval+100) < 
							(this.scroll_interval*this.scroll_step)) );
}
