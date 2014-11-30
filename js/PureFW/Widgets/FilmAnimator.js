/**
 * Ein Container, der Filmstreifen abspielt, wie eine alte Filmrolle. Die
 * Einzelbilder liegen dabei untereinander in einer Bilddatei die durchrotiert
 * wird. Um mehrere Animationen von einem FilmAnimator verwalten zu lassen,
 * z.B. weil ein Objekt mehrere Animationen erfahren kann und man nicht
 * die Rolle wechseln möchte, kann man mehrere Streifen nebeneinander legen
 * und zwischen ihnen hin und her wechseln.
 * 
 * Außerdem kann der Framebereich eingegrenzt werden, was vor allem dann
 * nötig wird, wenn ein Filmstreifen länger ist, als der andere, damit keine
 * Leerbilder gezeigt werden.
 * 
 * @author Alex, Philipp
 */

/**
 * @param HTMLElement parent_node
 * @param int x
 * @param int y
 * @param uint w	(wenn <= 0, dann wird sie dynamisch bestimmt)
 * @param uint h	(wenn <= 0, dann wird sie dynamisch bestimmt)
 */
PureFW.FilmAnimator = function(parent_node, x, y, w, h, no_scale) 
{
	if (typeof(parent_node) !== 'undefined') 
	{
		this.init(parent_node, x, y, w, h, no_scale);
		this.width = w || 40;
		this.height = h || 40;
			
		this.instance_num = this.constructor.num_instances;
		this.instance_name = "PureFW.FilmAnimator.instances["+this.instance_num+"]";

		this.id = "FilmAnimator"+this.instance_num;
		this.content_id = "FilmAnimatorCont"+this.instance_num;
		this.bg_id = "FilmAnimatorBg"+this.instance_num;
		
		this.insert_into_dom_tree();
		this.constructor.num_instances++;
		this.constructor.instances[this.instance_num] = this;

		if (!this.no_scale)
			PureFW.WidgetManager.manager_all.register_widget(this);
	}
};
/********************************************************************
 * FilmAnimator extends Container
 ********************************************************************/
PureFW.FilmAnimator.extend(PureFW.Container);

/********************************************************************
 * Statische Deklarationen
 ********************************************************************/
PureFW.FilmAnimator.num_instances = 0;
PureFW.FilmAnimator.instances = new Array();

/********************************************************************
 * Prototyp-Deklarationen
 ********************************************************************/
PureFW.FilmAnimator.prototype.init = function(parent_node, x, y, w, h, no_scale) 
{
	PureFW.FilmAnimator.parent.init.call(this, parent_node, x, y, w, h, 
		no_scale);

	this.number_of_frames = null;
	this.current_frame = 0;
	this.interval = null;
	this.running_film = false;
	this.run_on_show = false;
	this.inner_frame = null;
	this.frame_range_min = new Array();
	this.frame_range_max = new Array();
	this.reverse = false;
	this.yoyo = false;
	this.strip = 0;
	this.strips = 1;
	this.on_loop_functions = new Array();
	this.frame_delay = 100;				//Default Interval Delay of 100ms
};

PureFW.FilmAnimator.prototype.insert_into_dom_tree = function() 
{
	PureFW.FilmAnimator.parent.insert_into_dom_tree.call(this);

	this.inner_frame = this.add_widget(
		PureFW.Image,
		0, 0,
		this.width, this.height, 
		'', 
		this.no_scale
	);
	
	this.inner_frame.repaint = (function(_animator) {
		return function(ev) {
			PureFW.Image.parent.repaint.call(this);
			/**
			 * Höhe muss durch die Frameanzahl teilbar sein.
			 */
			var frames = _animator.number_of_frames + 1;
			var tmp = Math.floor(this.get_height() / frames);
			this.get_node().style.height = (tmp * frames) + 'px';
			
			/**
			 * Die Höhe des Animators muss genau so groß sein, wie ein 
			 * Einzelbild
			 */
			_animator.get_node().style.height = tmp + 'px';
			
			/**
			 * Dasselbe gilt auch für die Breite und die Filmstreifen
			 *
			var strips = _animator.strips;
			var tmp = Math.floor(this.get_width() / strips);
			this.get_node().style.width = (tmp * strips) + 'px';
			_animator.get_node().style.width = tmp + 'px';*/
		}
	})(this);

	this.parent_node.appendChild(this.node);
};

/**
 * Fügt einen neuen Filmstreifensatz in den Animator ein.
 * 
 * Alle Einstellungen werden zurückgesetzt, falls seit dem Einlegen des
 * letzten Streifens welche getroffen wurden.
 * 
 * Die Animation startet nicht direkt, es muss start_animation() aufgerufen
 * werden.
 * 
 * @param String path 		URL to image, relative to ../pix/
 * @param uint num			The number of frames in the longest film strip
 * @param uint strips		The number of strips in the file
 */
PureFW.FilmAnimator.prototype.set_image = function(path, num, strips)
{
	this.strips = strips || 1;
	this.strip = 0;
	this.number_of_frames = num - 1;
	this.frame_range_min = new Array();
	this.frame_range_max = new Array();
	for (var i = 0; i < this.strips; i++) {
		this.frame_range_min[i] = 0;
		this.frame_range_max[i] = this.number_of_frames;
	}
	
	this.inner_frame.set_pic_url(path);
	this.inner_frame.set_height(this.height*num);
	this.inner_frame.set_width(this.width*strips);
	if (this.is_running()) {
		this.stop_animation();
		this.start_animation();
	}
};

/**
 * Gibt zurück, ob der Film läuft, also die Animation abgespielt wird.
 * 
 * @return bool
 */
PureFW.FilmAnimator.prototype.is_running = function() {
	return this.running_film;
}

/**
 * Bestimmt den Filmstreifen, der abgespielt werden soll.
 * 
 * Die Streifen liegen vertikal nebeneinander in einer Datei und sind von
 * 0 an aufsteigend nummeriert.
 * 
 * Beim wechseln des Streifens wird die Animation wieder von Frame 0, bzw.
 * dem angegebenen Minimum für den Streifen, begonnen.
 * 
 * @param uint strip
 */
PureFW.FilmAnimator.prototype.set_strip = function(strip) {
	this.stop_animation();
	
	if (strip >= this.strips)
		this.strip = this.strips - 1;
	else if (strip < 0)
		this.strip = 0;
	else
		this.strip = strip;
	
	if (this.reverse)
		this.current_frame = this.frame_range_max[this.strip];
	else
		this.current_frame = this.frame_range_min[this.strip];
	
	this.inner_frame.set_x((-this.strip)*this.width);
	this.inner_frame.set_y(0);
	
	this.start_animation();
}

/**
 * Setzt die Grenzen zwischen welchen Frames die Animation im angegbenen 
 * Streifen abgespielt werden soll.
 * 
 * Beispiel: 
 * <code>
 * // Spielt nur die ersten 4 Frames in Schleife ab für den zweiten Streifen
 * set_frame_range(0, 3, 1);
 * // Spielt Frames 3-6 für den 1. Streifen ab
 * set_frame_range(3, 6);
 * </code>
 * 
 * @param uint min
 * @param uint max
 * @param uint strip=0
 * @returns
 */
PureFW.FilmAnimator.prototype.set_frame_range = function(min, max, strip) {
	strip = strip || 0;
	this.frame_range_min[strip] = (min < 0) ? 0 : min;
	this.frame_range_max[strip] = (max > this.number_of_frames) 
		? this.number_of_frames	: max;
	this.current_frame = min;
}

/**
 * Setzt die Verzögerung in Millisekunden zwischen zwei Frames.
 * 
 * @param uint amount
 */
PureFW.FilmAnimator.prototype.set_frame_delay = function(amount)
{
	this.stop_animation();
	this.frame_delay = amount;
	this.start_animation();
}

/**
 * Springt zum nächsten Frame in der Reihe des aktuellen Streifens.
 */
PureFW.FilmAnimator.prototype.animate = function()
{
	if (this.current_frame > this.frame_range_max[this.strip]) {
		if (this.yoyo) {
			this.reverse = !this.reverse;
			this.current_frame = this.frame_range_max[this.strip];
		}
		else
			this.current_frame = this.frame_range_min[this.strip];
		this.on_loop();
	}
	else if (this.current_frame < this.frame_range_min[this.strip]) {
		if (this.yoyo) {
			this.reverse = !this.reverse;
			this.current_frame = this.frame_range_min[this.strip];
		}
		else
			this.current_frame = this.frame_range_max[this.strip];
		this.on_loop();
	}
	if (!this.inner_frame.get_node())
		this.stop_animation();
	
	this.inner_frame.get_node().style.top =
		((-this.current_frame) * Math.floor(this.inner_frame.get_height() / 
				(this.number_of_frames+1))) + 'px';
	
	if (this.reverse)
		this.current_frame--;
	else
		this.current_frame++;
	
};

PureFW.FilmAnimator.prototype.toggle_animation = function()
{
	if(this.is_running())
		this.stop_animation();
	else
		this.start_animation();
}

/**
 * Startet die Animation.
 */
PureFW.FilmAnimator.prototype.start_animation = function() {
	if (this.is_running())
		return;
	
	if ((this.frame_range_max[this.strip]-this.frame_range_min[this.strip]) < 1)
	{	// Keine Animation zu spielen.
		return;
	}
	
	this.running_film = true;
	this.interval = 
		PureFW.Timeout.set_interval(
			(function(_instance) {
				return function() {
					try {
						_instance.animate();
					}
					catch(e) {
						_instance.stop_animation();					
					}
				}
			})(this), 
			this.frame_delay
		);
};

PureFW.FilmAnimator.prototype.stop_animation = function() {
	if (!this.is_running())
		return;
	
	this.running_film = false;
	PureFW.Timeout.clear_interval(this.interval);
	this.interval = null;
};

PureFW.FilmAnimator.prototype.hide = function() {
	PureFW.FilmAnimator.parent.hide.call(this);
	if (this.is_running()) {
		this.run_on_show = true;
		this.stop_animation();
	}
};

PureFW.FilmAnimator.prototype.show = function() {
	PureFW.FilmAnimator.parent.show.call(this);
	if (this.run_on_show)
		this.start_animation();
	this.run_on_show = false;
};

PureFW.FilmAnimator.prototype.destroy = function() {
	this.stop_animation();
	PureFW.FilmAnimator.parent.destroy.call(this);
}

/**
 * Fügt einen EventHandler hinzu. Funktion überschrieben, da zusätzlich
 * der Typ "loop" unterstützt wird, welcher normalerweise aufgerufen wird,
 * wenn das Ende einer Animation erreicht ist und wieder zum Anfang gesprungen
 * wird.
 * 
 * @param string type
 * @param Function fn
 * @see Widget.add_event_handler
 */
PureFW.FilmAnimator.prototype.add_event_handler = function(type, fn) {
	if (type === "loop")
		this.on_loop_functions.push(fn);
	else
		PureFW.FilmAnimator.parent.add_event_handler.call(this, type, fn);
};

/**
 * Entfernt einen EventHandler. Funktion überschrieben, da zusätzlich
 * der Typ "loop" unterstützt wird, welcher normalerweise aufgerufen wird,
 * wenn das Ende einer Animation erreicht ist und wieder zum Anfang gesprungen
 * wird.
 * 
 * @param string type
 * @param Function fn
 * @see FilmAnimator.prototype.remove_event_handler
 */
PureFW.FilmAnimator.prototype.remove_event_handler = function(type, fn) {
	if (type === "loop")
		this.on_loop_functions.remove(fn);
	else
		PureFW.FilmAnimator.parent.remove_event_handler.call(this, type, fn);
};

/**
 * Ruft das "on-loop"-Event exlizit auf. Wird normalerweise aufgerufen,
 * wenn das Ende einer Animation erreicht ist und wieder zum Anfang gesprungen
 * wird.
 * 
 * @param Event ev [optional]
 */
PureFW.FilmAnimator.prototype.on_loop = function(ev) {
	if (!ev)
		ev = this.create_event("loop");
	
	var n = this.on_loop_functions.length;
	for (var i = 0; i < n; i++) {
		this.on_loop_functions[i].call(this, ev);
	}
};


PureFW.FilmAnimator.prototype.set_width = function(w, ms, easing, on_finish) {
	PureFW.FilmAnimator.parent.set_width.call(this, w, ms, easing, on_finish);
	this.inner_frame.set_width(w, ms, easing, on_finish);
};

PureFW.FilmAnimator.prototype.set_height = function(h, ms, easing, on_finish) {
	PureFW.FilmAnimator.parent.set_height.call(this, h, ms, easing, on_finish);
	this.inner_frame.set_height(h*this.number_of_frames+1, 
		ms, easing, on_finish);
};

PureFW.FilmAnimator.prototype.repaint = function() {
	PureFW.FilmAnimator.parent.repaint.call(this);
	/**
	 * Die Höhe muss genau so groß sein, wie ein Einzelbild
	 */
	this.get_node().style.height =
		(Math.floor(this.inner_frame.get_height() / (this.number_of_frames+1)))
			+ 'px';
	/**
	 * Dasselbe für die Breite
	 *
	this.get_node().style.width =
		(Math.floor(this.inner_frame.get_width() / (this.strips))) + 'px';
		*/
}