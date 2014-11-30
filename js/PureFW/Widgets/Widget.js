/**
 * Repräsentiert die Abstrakte Oberklasse aller Widgets.
 * 
 * @author Phil
 */


/** Wir definieren hier den Prototyp für alle Widgets */
PureFW.Widget = new Object();
PureFW.Widget.id = null;
PureFW.Widget.position = new PureFW.Point(0,0);
PureFW.Widget.width = 0;
PureFW.Widget.height = 0;
PureFW.Widget.parent_node = null;
PureFW.Widget.z_index = 1;
PureFW.Widget.pic_path = '';
PureFW.Widget.positioning = 'absolute';
PureFW.Widget.floating = 'none';
PureFW.Widget.node = null;

/**
 * Initialisiert wichtige Dinge
 */
PureFW.Widget.init = function(parent_node, x, y, w, h, no_scale) {
	this.no_scale = no_scale || false;
	this.width = parseInt(w) || 0;
	this.height = parseInt(h) || 0;
	this.position = new PureFW.Point(parseInt(x) || 0, parseInt(y) || 0);
	if (parent_node) {
		if (PureFW.instance_of(parent_node, PureFW.Container)) {
			parent_node = parent_node.get_content_node();
		}
		else if (PureFW.instance_of(parent_node, PureFW.Widget)) {
			parent_node = parent_node.get_node();
		}
		if (typeof(parent_node.appendChild) == 'undefined') {
			throw Error("Wrong parent_node given!");
		}
	}
	this.parent_node = parent_node || document.body;
	
	this.activated = true;
	this.opacity = 1;
	this._active_opacity = this.opacity = 1;	// Small hack for (de)activation
	this.current_fading_interval = 0;
	
	this.active_tweens = new Object();

	this.on_destroy_functions = new Array();
	this.on_click_functions = new Array();
	this.on_hide_functions = new Array();
	this.on_show_functions = new Array();
	this.on_mouseover_functions = new Array();
	this.on_mouseout_functions = new Array();
	this.on_mousedown_functions = new Array();
	this.on_mouseup_functions = new Array();
	this.on_fade_out_begin_functions = new Array();
	this.on_fade_out_end_functions = new Array();
	this.on_fade_in_begin_functions = new Array();
	this.on_fade_in_end_functions = new Array();
	this.quality = 'high';
	if (this.no_scale)
		this.scale_factor = 1;
	else
		this.scale_factor = PureFW.WidgetManager.manager_all.get_scale_factor();
	if (this.scale_factor == 1)
		this.pic_path = PureFW.PicPath.reference_path;
	else
		this.pic_path = PureFW.PicPath.reference_path + 'scales/' 
				+ this.scale_factor + '/';
};

/**
 * Gibt die aktuelle Position zurück.
 * 
 * @return Point
 */
PureFW.Widget.get_position = function() {
	return new PureFW.Point(this.get_x(), this.get_y());
};

/**
 * Gibt die x-Koordinate der aktuellen Position zurück.
 * 
 * @return int
 */
PureFW.Widget.get_x = function() {
	return Math.round(this.position.x*this.scale_factor);
};

/**
 * Gibt die y-Koordinate der aktuellen Position zurück.
 * 
 * @return int
 */
PureFW.Widget.get_y = function() {
	return Math.round(this.position.y*this.scale_factor);
};
/**
 * Setzt die x-Koordinate der Position des Widgets.
 * 
 * @param int x
 * @param uint ms			Positionssetzung soll animiert werden mit
 * 							Motiontweening und so viele Millisekunden lang
 * 							dauern.
 * @param Function easing	Funktion zum glätten der Animation
 * @param Function on_finish Funktion, die am Ende der Animation aufgerufen
 * 							 werden soll
 */
PureFW.Widget.set_x = function(x, ms, easing, on_finish) {
	x = parseInt(x);
	elem = this.get_node();
	var tween = null;
	if (elem) {
		if (!ms) {	// Direktes Setzen
			elem.style.left = x*this.scale_factor+'px';
		}
		else {		// Motion-Tweening
			if (!this.active_tweens)
				this.active_tweens = new Object();
			if (this.active_tweens.set_x)
				this.active_tweens.set_x.stop();

			tween = new Tween(
				elem.style, 'left', easing, 
				this.get_x(), x*this.scale_factor, ms/1000, 'px'
			);
			this.active_tweens.set_x = tween;
			tween.onMotionFinished = (function(_instance, _x, _cb) {
				return function() {
					_instance.active_tweens.set_x = null;
					if (_cb)
						_cb.call(this);
				}
			})(this, x, on_finish);
			tween.onMotionStopped = (function(_instance) {
				return function(evt) {
					_instance.position.x 
						= evt.target._pos / _instance.scale_factor;
					_instance.active_tweens.set_x = null;
				}
			})(this);
			tween.start();
		}
	}
	this.position.x = x;
	
	return tween;
};

/**
 * Setzt die y-Koordinate der Position des Widgets.
 * 
 * @param int y
 * @param uint ms			Positionssetzung soll animiert werden mit
 * 							Motiontweening und so viele Millisekunden lang
 * 							dauern.
 * @param Function easing	Funktion zum glätten der Animation
 * @param Function on_finish Funktion, die am Ende der Animation aufgerufen
 * 							 werden soll
 */
PureFW.Widget.set_y = function(y, ms, easing, on_finish) {
	y = parseInt(y);
	elem = this.get_node();
	var tween = null;
	if (elem) {
		if (!ms) {	// Direktes Setzen
			elem.style.top = y*this.scale_factor+'px';
		}
		else {		// Motion-Tweening
			if (!this.active_tweens)
				this.active_tweens = new Object();
			if (this.active_tweens.set_y)
				this.active_tweens.set_y.stop();
			
			tween = new Tween(
				elem.style, 'top', easing, 
				this.get_y(), y*this.scale_factor, ms/1000, 'px'
			);
			this.active_tweens.set_y = tween;
			tween.onMotionFinished = (function(_instance, _y, _cb) {
				return function() {
					_instance.active_tweens.set_y = null;
					if (_cb)
						_cb.call(this);
				}
			})(this, y, on_finish);
			tween.onMotionStopped = (function(_instance) {
				return function(evt) {
					_instance.position.y = 
						evt.target._pos / _instance.scale_factor;
					_instance.active_tweens.set_y = null;
				}
			})(this);
			tween.start();
		}
	}
	this.position.y = y;

	return tween;
};

/**
 * Setzt die Position des Widgets durch übergabe der x- und y-Koordinate.
 * 
 * @param int/PureFW.Point 	x
 * @param int 				y [optional, wenn 1. Parameter vom Typ Point]
 * @param uint ms			Positionssetzung soll animiert werden mit
 * 							Motiontweening und so viele Millisekunden lang
 * 							dauern.
 * @param Function easing	Funktion zum glätten der Animation
 * @param Function on_finish Funktion, die am Ende der Animation aufgerufen
 * 							 werden soll
 */
PureFW.Widget.set_position = function(x,y, ms, easing, on_finish) {
	delete this.position;
	x = parseInt(x); y = parseInt(y);
	if ((typeof(x) == 'object') && (x.constructor == PureFW.Point)) {
		x = x.x;
		y = x.y;
		ms = y;
		easing = ms;
	}
	else if (typeof(x) == 'object')
			throw new Error ("PureFW.Widget::set_position: " +
					"First argument is an object, but no PureFW.Point");
	
	if (x == this.get_x())
		x = this.position.x;
	if (y == this.get_y())
		y = this.position.y;
	
	var elem = this.get_node();
	if (elem) {
		if (!ms) {	// Direktes setzen der Position
			elem.style.left = x*this.scale_factor+'px';
			elem.style.top = y*this.scale_factor+'px';
		}
		else {		// Tweening
			this.set_x(x, ms, easing, on_finish);
			this.set_y(y, ms, easing);
		}
	}
	this.position = new PureFW.Point(x,y);
};

/**
 * Gibt die aktuelle Breite des Widgets zurück.
 * 
 * @result uint
 */
PureFW.Widget.get_width = function() {
	return Math.round(this.width*this.scale_factor);
};

/**
 * Setzt die Breite des Widgets auf die angegebene Größe.
 * 
 * @param uint w
 * @param uint ms			Positionssetzung soll animiert werden mit
 * 							Motiontweening und so viele Millisekunden lang
 * 							dauern.
 * @param Function easing	Funktion zum glätten der Animation
 * @param Function on_finish Funktion, die am Ende der Animation aufgerufen
 * 							 werden soll
 */
PureFW.Widget.set_width = function(w, ms, easing, on_finish) {
	w = parseInt(w);
	var elem = this.get_node();
	var tween = null;
	if (!elem) {
		this.width = w;
	}
	else {
		if (!ms) {
			this.width = w;
			this.repaint();
		}
		else {
			if (!this.active_tweens)
				this.active_tweens = new Object();
			if (this.active_tweens.set_width)
				this.active_tweens.set_width.stop();
			
			tween = new Tween(
				elem.style, 'width', easing, 
				this.get_width(), w*this.scale_factor, ms/1000, 'px'
			);
			this.active_tweens.set_width = tween;
			tween.onMotionFinished = (function(_instance, _w, _cb) {
				return function() {
					_instance.active_tweens.set_width = null;
					_instance.width = _w;
					_instance.repaint();
					if (_cb)
						_cb.call(this);
				}
			})(this, w, on_finish);
			tween.onMotionStopped = (function(_instance) {
				return function(evt) {
					_instance.width 
						= evt.target._pos / _instance.scale_factor;
					_instance.active_tweens.set_width = null;
				}
			})(this);
			this.width = w;
			tween.start();
		}
	}
	return tween;
};

/**
 * Gibt die aktuelle Höhe des Widgets zurück.
 * 
 * @result uint
 */
PureFW.Widget.get_height = function() {
	return Math.round(this.height*this.scale_factor);
};

/**
 * Setzt die Höhe des Widgets auf die angegebene Größe.
 * 
 * @param uint h
 * @param uint ms			Positionssetzung soll animiert werden mit
 * 							Motiontweening und so viele Millisekunden lang
 * 							dauern.
 * @param Function easing	Funktion zum glätten der Animation
 * @param Function on_finish Funktion, die am Ende der Animation aufgerufen
 * 							 werden soll
 */
PureFW.Widget.set_height = function(h, ms, easing, on_finish) {
	h = parseInt(h);
	var tween = null;
	var elem = this.get_node();
	if (!elem) {
		this.height = h;
	}
	else {
		if (!ms) {
			this.height = h;
			this.repaint();
		}
		else {
			if (!this.active_tweens)
				this.active_tweens = new Object();
			if (this.active_tweens.set_height)
				this.active_tweens.set_height.stop();
			
			tween = new Tween(
				elem.style, 'height', easing, 
				this.get_height(), h*this.scale_factor, ms/1000, 'px'
			);
			this.active_tweens.set_height = tween;
			tween.onMotionFinished = (function(_instance, _h, _cb) {
				return function() {
					_instance.active_tweens.set_height = null;
					_instance.height = _h;
					_instance.repaint();
					if (_cb)
						_cb.call(this);
				}
			})(this, h, on_finish);
			tween.onMotionStopped = (function(_instance) {
				return function(evt) {
					_instance.height 
						= evt.target._pos / _instance.scale_factor;;
					_instance.active_tweens.set_height = null;
				}
			})(this);
			tween.start();
			this.height = h;
		}
	}
	return tween;
};

/**
 * Setzt den z-Index des Widgets. Widgets mit höherem z-index überlappen
 * solche, mit niedrigerem.
 * 
 * @param uint i
 */
PureFW.Widget.set_z_index = function(i) {
	this.z_index = i;
	var elem = this.get_node();
	if (elem)
		elem.style.zIndex = i;
};

/**
 * Gibt den z-Index des Widgets zurück.
 * 
 * @result uint
 */
PureFW.Widget.get_z_index = function() {
	return this.z_index;
};

/**
 * Setzt die Positionierungsart ('absolute' oder 'relative').
 * 
 * @param enum('absolute', 'relative')	type
 */
PureFW.Widget.set_positioning = function(str) {
	this.positioning = (str == 'absolute') ? 'absolute' : 'relative';
	var elem = this.get_node();
	if (elem)
		elem.style.position = this.positioning;
};

/**
 * Gibt die Positionierungsart ('absolute' oder 'relative') zurück
 * 
 * @return string
 */
PureFW.Widget.get_positioning = function(str) {
	return this.positioning;
};

/**
 * Setzt das Fließverhalten des Widgets. Normalerweise initial ist es 
 * 'none'. Gültige Werte sind 'left', 'right', 'none'.
 * 
 * @param enum('left', 'right', 'none') type
 */
PureFW.Widget.set_float = function(type) {
	this.floating = (type == 'left' || type == 'right') ? type : 'none';
	var elem = this.get_node();
	if (elem) {
		elem.style.cssFloat = this.floating;
		elem.style.styleFloat = this.floating;
	}
};

/**
 * Setzt die angegebene CSS-Klasse. 
 * 
 * Dadurch können zentraler Stelle (z.B. in einer CSS-Datei) definierte Styles
 * mit dem Objekt verknüpft werden.
 * 
 * @param String class_name
 */
PureFW.Widget.set_css_class = function(class_name) {
	var elem = this.get_node();
	if (elem)
		elem.className = class_name;
}

/**
 * Setzt den Tooltip, der beim drübermausen und kurz warten erscheint.
 * 
 * @param String str
 */
PureFW.Widget.set_tooltip = function(str) {
	this.get_node().title = str;
}

/**
 * Gibt den aktuellen Tooltip zurück.
 * 
 * @return String
 */
PureFW.Widget.get_tooltip = function() {
	return this.get_node().title;
}

/**
 * Zeigt das Widget an. Ist die Umkehrfunktion zu Widget.hide().
 * 
 * @see Widget.hide()
 */
PureFW.Widget.show = function() {
	var elem = this.get_node();
	if (elem)
		elem.style.display = 'block';
	this.on_show();
};

/**
 * Versteckt das Widget (es wird nicht mehr angezeigt, ist aber noch im
 * Speicher und kann per Widget show() wieder angezeigt werden).
 * 
 * @see Widget.show()
 */
PureFW.Widget.hide = function() {
	var elem = this.get_node();
	if (elem)
		elem.style.display = 'none';
	this.on_hide();
};

/**
 * Gibt zurück, ob das Widget versteckt ist
 * 
 * @return bool
 */
PureFW.Widget.is_hidden = function() {
	var elem = this.get_node();
	if (elem)
		return (elem.style.display == 'none');
	
	return true;	// Element hat keinen Repräsentanten: hidden ^^
};

/**
 * Blendet das Widget animiert aus.
 * 
 * Der Parameter <code>ms</code> gibt an, wie schnell, das faden passieren soll.
 * <code>ms</code> gibt dabei an, wie viele Millisekunden vergehen sollen,
 * bis das Objekt den gewünschten, durch <code>min_opacity</code> bestimmten
 * Transparenz erreicht oder unterschritten hat.
 * Eine Echtzeitleistung kann aufgrund der Sprachumgebung nicht gewährt werden, 
 * es ist also als Näherung zu verstehen.
 * 
 * @param uint ms			gewünschte Animationsdauer (default 1000)
 * @param unit min_opacity	geschünschte minimale Transparenz (default=0)
 * @param Function easing	Funktion zum glätten der Animation
 */
PureFW.Widget.fade_out = function(ms, min_opacity, easing) {
	this.on_fade_out_begin();
	if (typeof(ms) == 'undefined')
		ms = 500;
	else
		ms = parseInt(ms);
	easing = easing || null;
	min_opacity = min_opacity || 0;
	
	if (!this.active_tweens)
		this.active_tweens = new Object();
	if (this.active_tweens.fade)
		this.active_tweens.fade.stop();
	
	var tween = new OpacityTween(
		this.get_node(), 
		easing, 
		this.get_opacity()*100, 
		min_opacity*100,
		ms/1000
	);
	this.active_tweens.fade = tween;
	tween.onMotionFinished = (function(_instance, _min_op) {
			return function() {
				_instance.active_tweens.fade = null;
				_instance.set_opacity(_min_op);
				_instance.on_fade_out_end();
			}
		})(this, min_opacity);
	tween.start();
	/*
	frame_ms = 30;
	if (ms <= frame_ms) {
		this.set_opacity(min_opacity);
	}
	else {
		min_opacity = min_opacity | 0;
		var steps = Math.ceil(ms / frame_ms);
		var opacity_loss = (this.get_opacity()-min_opacity) / steps;
		if (opacity_loss <= 0) {
			this.on_fade_out_end();
			return;
		}
		
		this.current_fading_interval = PureFW.Timeout.set_interval(
			(function(_instance, _opacity_loss, _min_opacity) {
				return function() {
					if (_instance.get_opacity() <= _min_opacity) {
						_instance.set_opacity(_min_opacity);
						PureFW.Timeout.clear_interval(_instance.current_fading_interval);
						_instance.on_fade_out_end();
					}
					else {
						_instance.set_opacity(
							_instance.get_opacity() - _opacity_loss
						);
					}
				}
			})(this, opacity_loss, min_opacity),
			20
		);
	}*/
};

/**
 * Blendet das Widget animiert ein.
 * 
 * Der Parameter <code>ms</code> gibt an, wie schnell, das faden passieren soll.
 * <code>ms</code> gibt dabei an, wie viele Millisekunden vergehen sollen,
 * bis das Objekt den gewünschten, durch <code>max_opacity</code> bestimmten
 * Transparenz erreicht oder überschritten hat.
 * Eine Echtzeitleistung kann aufgrund der Sprachumgebung nicht gewährt werden, 
 * es ist also als Näherung zu verstehen.
 * 
 * @param uint ms			gewünschte Animationsdauer (default 1000)
 * @param unit max_opacity	geschünschte maximale Transparenz (default=1)
 * @param Function easing	Funktion zum glätten der Animation
 */
PureFW.Widget.fade_in = function(ms, max_opacity, easing) {
	this.on_fade_in_begin();
	if (typeof(ms) == 'undefined')
		ms = 500;
	else
		ms = parseInt(ms);
	easing = easing || null;
	max_opacity = max_opacity || 1;
	
	if (!this.active_tweens)
		this.active_tweens = new Object();
	if (this.active_tweens.fade)
		this.active_tweens.fade.stop();
	
	var tween = new OpacityTween(
		this.get_node(), 
		easing, 
		this.get_opacity()*100, 
		max_opacity*100, 
		ms/1000
	);
	this.active_tweens.fade = tween;
	
	tween.onMotionFinished = (function(_instance, _max_op) {
			return function() {
				_instance.active_tweens.fade = null;
				_instance.set_opacity(_max_op);
				_instance.on_fade_in_end();
			}
		})(this, max_opacity);
	tween.start();
	
	/*
	this.on_fade_in_begin();
	if (typeof(ms) == 'undefined')
		ms = 500;
	else
		ms = parseInt(ms);
	frame_ms = frame_ms | 30;
	if (ms <= frame_ms) {
		this.set_opacity(max_opacity);
	}
	else {
		max_opacity = max_opacity | 1;
		var steps = Math.ceil(ms / frame_ms);
		var opacity_gain = (max_opacity - this.get_opacity()) / steps;
		if (opacity_gain <= 0) {
			this.on_fade_in_end();
			return;
		}
		
		this.current_fading_interval = PureFW.Timeout.set_interval(
			(function(_instance, _opacity_gain, _max_opacity) {
				return function() {
					if (_instance.get_opacity() >= max_opacity) {
						_instance.set_opacity(_max_opacity);
						PureFW.Timeout.clear_interval(_instance.current_fading_interval);
						_instance.on_fade_in_end();
					}
					else {
						_instance.set_opacity(
							_instance.get_opacity() + _opacity_gain
						);
					}
				}
			})(this, opacity_gain, max_opacity),
			20
		);
	}*/
};

/**
 * Deaktiviert das Widget
 */
PureFW.Widget.deactivate = function() {
	this.activated = false;
	this.set_opacity(this._active_opacity-0.3);
};

/**
 * Aktiviert das Widget
 */
PureFW.Widget.activate = function() {
	this.activated = true;
	this.set_opacity(this._active_opacity);
};

/**
 * Gibt zurück, ob das Widget aktiviert ist.
 * 
 * @return bool
 */
PureFW.Widget.is_activated = function() {
	return this.activated;
}

PureFW.Widget.is_active = PureFW.Widget.is_activated;


/**
 * Hilfsfunktion, die bei add_event_handler benutzt wird
 */
PureFW.Widget._register_event = function(type) {
	if (!document.all) {
		this.get_node().setAttribute("on"+type, "javascript: "+
			this.instance_name+".on_"+type+"(event);");
	}
	/**
	 * Der IE-Workaround! Ein Riesenspaß für die ganze 
	 * Familie... 
	 */
	else {
		if (!this.ie_ev_workaround)
			this.ie_ev_workaround = new Object();
		this.ie_ev_workaround[type] = new Object();
		this.ie_ev_workaround[type].trys = 10;
		this.ie_ev_workaround[type].fn = (function(_this, _type) {
			return function() {
				try {
					_this.get_node()['on'+_type] = function(ev) {
						_this['on_'+_type](ev);
					}
				}
				catch (e) {
					throw e;
				}
				_this.ie_ev_workaround[_type].trys--;
				if (_this.ie_ev_workaround[_type].trys > 0)
					PureFW.Timeout.set_timeout(
						_this.ie_ev_workaround[_type].fn, 200);
			}
		})(this, type);
		PureFW.Timeout.set_timeout(this.ie_ev_workaround[type].fn, 200);
	}
}

/**
 * Fügt dem Widget einen Event-Handler hinzu. type ist dabei z.B. "click",
 * "mouseover", "mouseout", "keydown" usw., fn ein Pointer auf eine Funktion,
 * welche als ersten Parameter das Ereignis übergeben bekommt.
 * 
 * @see EventUtil.add_event_handler
 * @param string type
 * @param Function fn
 */
PureFW.Widget.add_event_handler = function(type, fn) {
	if (typeof (type) !== 'string')
		throw new Error("Widget::add_event_handler: First argument 'type' " +
				"has to be of type 'string'");
	if (typeof (fn) !== 'function')
		throw new Error("Widget::add_event_handler: Second argument 'fn' " +
				"has to be of type 'function'");
	if (type == "destroy") {
		this.on_destroy_functions.push(fn);
	}
	else if (type == "click"){
		this.get_node().style.cursor = "pointer";
		this.on_click_functions.push(fn);
		
		this._register_event("click");
		this._register_event("dblclick");
		this._register_event("mouseover");
		this._register_event("mouseout");
	}
	else if (type == "show") {
		this.on_show_functions.push(fn);
	}
	else if (type == "hide") {
		this.on_hide_functions.push(fn);
	}
	else if (type == "mouseover") {
		this.on_mouseover_functions.push(fn);
		this._register_event("mouseover");
	}
	else if (type == "mouseout") {
		this.on_mouseout_functions.push(fn);
		this._register_event("mouseout");
	}
	else if (type == "mousedown") {
		this.get_node().style.cursor = "pointer";
		this.on_mousedown_functions.push(fn);
		this._register_event("mousedown");
	}
	else if (type == "mouseup") {
		this.get_node().style.cursor = "pointer";
		this.on_mouseup_functions.push(fn);
		this._register_event("mouseup");
	}
	else if (type == "fade_out_begin") {
		this.on_fade_out_begin_functions.push(fn);
	}
	else if (type == "fade_out_end") {
		this.on_fade_out_end_functions.push(fn);
	}
	else if (type == "fade_in_begin") {
		this.on_fade_in_begin_functions.push(fn);
	}
	else if (type == "fade_in_end") {
		this.on_fade_in_end_functions.push(fn);
	}
	else {
		try {
			PureFW.EventUtil.add_event_handler(this.get_node(), type, fn);
		}
		catch (e) {
			throw new Error("Widget.add_event_handler("+type+" ,"+fn+"): " +
					"getElem("+this.id+") has no properties.");
		}
	}
};

/**
 * Entfernt einen bereits registrierten Event-Handler, falls vorhanden. 
 * Dabei müssen die Parameter exakt diegleichen sein, wie beim vorherigen
 * Aufruf von add_event_handler.
 *
 * @see Widget.add_event_handler
 * @param string type
 * @param Function fn 
 */
PureFW.Widget.remove_event_handler = function(type, fn) {
	if (typeof (type) !== 'string')
		throw new Error("Widget::remove_event_handler: First argument 'type' " +
				"has to be of type 'string'");
	if (typeof (fn) !== 'function')
		throw new Error("Widget::remove_event_handler: Second argument 'fn' " +
				"has to be of type 'function'");
	if (type == "destroy") {
		this.on_destroy_functions.remove(fn);
	}
	else if (type == "click") {
		this.on_click_functions.remove(fn);
		if (this.on_click_functions.length <= 0)
			this.get_node().style.cursor = '';
	}
	else if (type == "show") {
		this.on_show_functions.remove(fn);
	}
	else if (type == "hide") {
		this.on_hide_functions.remove(fn);
	}
	else if (type == "mouseover") {
		this.on_mouseover_functions.remove(fn);
	}
	else if (type == "mouseout") {
		this.on_mouseout_functions.remove(fn);
	}
	else if (type == "fade_out_begin") {
		this.on_fade_out_begin_functions.remove(fn);
	}
	else if (type == "fade_out_end") {
		this.on_fade_out_end_functions.remove(fn);
	}
	else if (type == "fade_in_begin") {
		this.on_fade_in_begin_functions.remove(fn);
	}
	else if (type == "fade_in_end") {
		this.on_fade_in_end_functions.remove(fn);
	}
	else {
		PureFW.EventUtil.remove_event_handler(document.getElemById(this.id), type, fn);
	}
};

/**
 * Entfernt alle bereits registrierten Event-Handler, die für die angegebene
 * Ereignisart registriert wurden.
 * 
 * @param string type
 */
PureFW.Widget.clear_event_handler = function(type) {
	if (typeof (type) !== 'string')
		throw new Error("Widget::clear_event_handler: First argument 'type' " +
				"has to be of type 'string'");
	
	if (type == "destroy") {
		this.on_destroy_functions.destroy();
		this.on_destroy_functions = new Array();
	}
	else if (type == "click") {
		this.on_click_functions.destroy();
		this.on_click_functions = new Array();
		this.get_node().style.cursor = '';
	}
	else if (type == "show") {
		this.on_show_functions.destroy();
		this.on_show_functions = new Array();
	}
	else if (type == "hide") {
		this.on_hide_functions.destroy();
		this.on_hide_functions = new Array();
	}
	else if (type == "mouseover") {
		this.on_mouseover_functions.destroy();
		this.on_mouseover_functions = new Array();
	}
	else if (type == "mouseout") {
		this.on_mouseout_functions.destroy();
		this.on_mouseout_functions = new Array();
	}
}

/**
 * Ruft das "on-destroy"-Event explizit auf. Wird normalerweise aufgerufen,
 * wenn das Widget per Widget.destroy zerstört wird.
 * 
 * @param Event ev [optional]
 */
PureFW.Widget.on_destroy = function(ev) {
	if (!ev)
		ev = this.create_event("destroy");
	
	var n = this.on_destroy_functions.length;
	for (var i = 0; i < n; i++) {
		try { /** Wenn ein Destruktor zickt, dennoch weitermachen! */
			this.on_destroy_functions[i].call(this, ev);
		}
		catch (e) {}
	}
};

/**
 * Ruft das on-click-Event explizit auf. Wird normalerweise aufgerufen,
 * wenn das Widget angeklickt wird.
 * 
 * @param Event ev [optional]
 */
PureFW.Widget.on_click = function(ev) {
	if (!this.activated)
		return;
	if (!ev)
		ev = this.create_event("click");
	var n = this.on_click_functions.length;
	for (var i = 0; i < n; i++) {
		this.on_click_functions[i].call(this, ev);
	}
};

PureFW.Widget.on_dblclick = PureFW.Widget.on_click;

/**
 * Ruft das on-fade-in-begin-Event explizit auf. Wird normalerweise aufgerufen,
 * wenn die Fade-In-Animation startet.
 * 
 * @param Event ev [optional]
 */
PureFW.Widget.on_fade_in_begin = function(ev) {
	if (!ev)
		ev = this.create_event("fade_in_begin");
	var n = this.on_fade_in_begin_functions.length;
	for (var i = 0; i < n; i++) {
		this.on_fade_in_begin_functions[i].call(this, ev);
	}
};

/**
 * Ruft das on-fade-in-end-Event explizit auf. Wird normalerweise aufgerufen,
 * wenn die Fade-In-Animation endet.
 * 
 * @param Event ev [optional]
 */
PureFW.Widget.on_fade_in_end = function(ev) {
	if (!ev)
		ev = this.create_event("fade_in_end");
	var n = this.on_fade_in_end_functions.length;
	for (var i = 0; i < n; i++) {
		this.on_fade_in_end_functions[i].call(this, ev);
	}
};

/**
 * Ruft das on-fade-out-begin-Event explizit auf. Wird normalerweise aufgerufen,
 * wenn die Fade-Out-Animation startet.
 * 
 * @param Event ev [optional]
 */
PureFW.Widget.on_fade_out_begin = function(ev) {
	if (!ev)
		ev = this.create_event("fade_out_begin");
	var n = this.on_fade_out_begin_functions.length;
	for (var i = 0; i < n; i++) {
		this.on_fade_out_begt_functions[i].call(this, ev);
	}
};

/**
 * Ruft das on-fade-out-end-Event explizit auf. Wird normalerweise aufgerufen,
 * wenn die Fade-Out-Animation endet.
 * 
 * @param Event ev [optional]
 */
PureFW.Widget.on_fade_out_end = function(ev) {
	if (!ev)
		ev = this.create_event("fade_out_end");
	var n = this.on_fade_out_end_functions.length;
	for (var i = 0; i < n; i++) {
		this.on_fade_out_end_functions[i].call(this, ev);
	}
};


/**
 * Vergrößert das Widget durch zentralen Zoom
 * 
 * @param ufloat factor
 */
PureFW.Widget.zoom = function(factor) {
	if (factor <= 0)
		throw new Error("PureFW.Widget.zoom: factor has to be > 0, "+factor
			+" given");
	var new_w = this.width*factor;
	var new_h = this.height*factor;
	
	this.position.x += (this.width - new_w)>>1;
	this.position.y += (this.height - new_h)>>1;
	
	this.set_width(new_w);
	this.set_height(new_h);
}

/**
 * Ruft das on-mouseover-Event explizit auf. Wird normalerweise aufgerufen,
 * wenn man mit dem Cursor über das Widget fährt.
 * 
 * @param Event ev [optional]
 */
PureFW.Widget.on_mouseover = function(ev) {
	if (!this.activated)
		return;
	/*if (this.on_click_functions.length > 0)
		this.set_opacity(0.5);*/
	if (!ev)
		ev = this.create_event("mouseover");
	var n = this.on_mouseover_functions.length;
	for (var i = 0; i < n; i++) {
		this.on_mouseover_functions[i].call(this, ev);
	}
};

/**
 * Ruft das on-mouseout-Event explizit auf. Wird normalerweise aufgerufen,
 * wenn man mit dem Cursor vom Widget runterfährt.
 * 
 * @param Event ev [optional]
 */
PureFW.Widget.on_mouseout = function(ev) {
	if (!this.activated)
		return;
	/*if (this.on_click_functions.length > 0)
		this.set_opacity(1);*/
	if (!ev)
		ev = this.create_event("mouseout");
	var n = this.on_mouseout_functions.length;
	for (var i = 0; i < n; i++) {
		this.on_mouseout_functions[i].call(this, ev);
	}
};

/**
 * Ruft das on-mousedown-Event explizit auf. Wird normalerweise aufgerufen,
 * wenn man die Maustaste über dem Widget herunterdrückt.
 * 
 * @param Event ev [optional]
 */
PureFW.Widget.on_mousedown = function(ev) {
	if (!this.activated)
		return;
	if (!ev)
		ev = this.create_event("mousedown");
	var n = this.on_mousedown_functions.length;
	for (var i = 0; i < n; i++) {
		this.on_mousedown_functions[i].call(this, ev);
	}
};

/**
 * Ruft das on-mouseup-Event explizit auf. Wird normalerweise aufgerufen,
 * wenn man die Maustaste über dem Widget herunterdrückt.
 * 
 * @param Event ev [optional]
 */
PureFW.Widget.on_mouseup = function(ev) {
	if (!this.activated)
		return;
	if (!ev)
		ev = this.create_event("mouseup");
	var n = this.on_mouseup_functions.length;
	for (var i = 0; i < n; i++) {
		this.on_mouseup_functions[i].call(this, ev);
	}
};

/**
 * Ruft das on-show-Event explizit auf. Wird normalerweise aufgerufen, wenn
 * "show()" aufgerufen wird.
 * 
 * @param Event ev [optional]
 */
PureFW.Widget.on_show = function(ev) {
	if (!ev)
		ev = this.create_event("show");
	var n = this.on_show_functions.length;
	for (var i = 0; i < n; i++) {
		this.on_show_functions[i].call(this, ev);
	}
};

/**
 * Ruft das on-hide-Event explizit auf. Wird normalerweise aufgerufen, wenn
 * "hide()" aufgerufen wird.
 * 
 * @param Event ev [optional]
 */
PureFW.Widget.on_hide = function(ev) {
	if (!ev)
		ev = this.create_event("hide");
	var n = this.on_hide_functions.length;
	for (var i = 0; i < n; i++) {
		this.on_hide_functions[i].call(this, ev);
	}
};

/**
 * Gibt die ID des Widgets zurück. Diese wird i.d.R. eindeutig beim Erzeugen
 * gesetzt und der HTML-Repräsentation zugewiesen.
 * 
 * @return string
 */
PureFW.Widget.get_id = function() { 
	return this.id 
};

/**
 * Setzt die ID des Elements. Diese ID muss eindeutig sein für das gesamte
 * HTML-Dokument (inklusive Frames) und somit aktuell noch nicht vergeben
 * sein.
 * 
 * @param string id
 * @throws Error
 */
PureFW.Widget.set_id = function(id) {
	if (id == this.id)
		return;
	if (document.getElemById(id)) {
		alert(PureFW.Widget.set_id.caller);
		throw new Error("Cannot set ID '"+id+"'. Element with this ID " +
				"already defined in HTML-Document.");
	}
	var elem = this.get_node();
	if (elem)
		elem.id = id;
	this.id = id;
};

/**
 * Skaliert das Widget um den angegebenen Faktor.
 * 
 * @param float factor
 */
PureFW.Widget.scale = function(factor) {
	this.scale_factor = factor;
	if (this.quality !== 'high') {
		this.pic_path = PureFW.PicPath.reference_path + 'scales/' 
			+ this.scale_factor + '/quality/' +  this.quality + '/';
	}
	else {
		this.pic_path = PureFW.PicPath.reference_path + 'scales/' 
			+ this.scale_factor + '/';
	}
	this.repaint();
};

/**
 * Setzt die Bildqualität des Widgets.
 * 
 * @param low|high		qual
 */
PureFW.Widget.set_quality = function(qual) {
	this.quality = qual;
	if (this.quality !== 'high') {
		this.pic_path = PureFW.PicPath.reference_path + 'scales/' 
			+ this.scale_factor + '/quality/' +  this.quality + '/';
	}
	else
		this.pic_path = PureFW.PicPath.reference_path + 'scales/' 
			+ this.scale_factor + '/';
	this.repaint();
};

/**
 * Setzt den Pfad zum Ordner aller Bildern, die das Widget anzeigen kann.
 * 
 * @param string path
 */
PureFW.Widget.set_pic_path = function(path) {
	this.pic_path = path;
	this.repaint();
};

/**
 * Gibt den aktuellen Pfad zu den Bildern zurück.
 * 
 * @return string
 */
PureFW.Widget.get_pic_path = function() {
	return this.pic_path; 
};

/**
 * Fügt der angegebenen URL den Pic-Path hinzu.
 * 
 * Immer diese Funktion benutzen, nicht von hand this.pic_path + URL machen,
 * weil das nicht immer stimmt (z.B. bei absoluten Pfaden oder externen URLs)
 */
PureFW.Widget.add_pic_path = function(pic_url) {
	if ((pic_url.substr(0,7) == 'http://')||(pic_url.substr(0,8) == 'https://'))
		return pic_url;
	if (pic_url.substr(0,3) == '../')
		return PureFW.PicPath.reference_path + pic_url;
	else
		return this.pic_path + pic_url;
}

/**
 * Malt das Widget neu, d.h. es werden alle Bilder neu zugewiesen, die
 * Größen neu gesetzt etc.
 */
PureFW.Widget.repaint = function() {
	var elem = this.get_node();
	if (elem) 
	{
		/** IE-8 schmiert hier ab und zu ab. Daher try-catch */
		try	{
			elem.style.width = this.get_width() + 'px';
		}
		catch(err) { }
		try	{
			elem.style.height = this.get_height() + 'px';
		}
		catch(err) { }
		try	{
			elem.style.left = this.get_x() + 'px';
		}
		catch(err) { }
		try	{
			elem.style.top = this.get_y() + 'px';
		}
		catch(err) { }
	}
};

/**
 * Setzt die Schriftgröße des Widgets
 * 
 * @param int size
 */
PureFW.Widget.set_font_size = function(size) {
	var elem = this.get_node();
	if (elem)
		elem.style.fontSize = (parseFloat(size) == size) ? size+'em' : size;
};

/**
 * Setzt den Schattenwurf des Textes.
 * 
 * Schattenwurf wird nur von mordernen Browsern unterstützt. Da es aber kein
 * kritisches Feature ist, ist es im Framework vorhanden. Man beachte nur, dass
 * es Browser geben kann, die dann den Schatten einfach nicht anzeigen.
 * 
 * Beispiel:
 * <code>
 * w.set_text_shadow("#cccccc", 3, 4, 3);
 * </code>
 * 
 * @param string color		Hex-Farbcode inklusive #
 * @param int x				X-Position des Schattens in Pixel
 * @param int y				Y-Position des Schattens in Pixel
 * @param int blur_radius	Blur-Radius des Schattens in Pixel
 */
PureFW.Widget.set_text_shadow = function(color, x, y, blur_radius) {
	this.get_node().style.textShadow = color + " " + x + "px " + y + "px "
		+ blur_radius + "px ";
}

/**
 * Setzt das Schriftgewicht (von dünn bis fett)
 * 
 * @param string/int weight
 */
PureFW.Widget.set_font_weight = function(weight) {
	var elem = this.get_node();
	if (elem)
		elem.style.fontWeight = weight;
};

/**
 * Setzt den Stil der Schrift.
 * 
 * Gültige Werte sind die CSS-3-kompatiblen "normal", "italic", "oblique" und
 * "inherit".
 * 
 * @param {'normal', 'italic', 'oblique', 'inherit'} style
 */
PureFW.Widget.set_font_style = function(style) {
	var elem = this.get_node();
	if (elem)
		elem.style.fontStyle = style;
}

/**
 * Setzt die Schriftfarbe (#XXX, #XXXXXX, 'black', 'red' ...)
 * 
 * @param string col
 */
PureFW.Widget.set_font_color = function(col) {
	var elem = this.get_node();
	if (elem)
		elem.style.color = col;
};

/**
 * Setzt die Deckkraft des Widgets. Werte zwischen von 0 bis 1 erlaubt, wobei 0
 * komplett durchsichtig und 1 opac ist.
 * 
 * @param [0; 1] value
 */
PureFW.Widget.set_opacity = function(value) {
	var elem = this.get_node();
	if (value < 0)
		value = 0;
	if (value > 1)
		value = 1;
	this.opacity = value;
	if (this.activated)
		this._active_opacity = value;
	if (elem) {
		if (document.all)
			return;
		/* Internet Explorer */
		//elem.style.filter = "Alpha(opacity="+Math.round(value*100)+")";
		
		elem.style.opacity = value; /* Safari, Opera, Firefox >= 3 */
		elem.style.MozOpacity = value; /* Firefox< 3, Mozilla*/
	}
};

/**
 * Gibt die Deckkraft zurück.
 * 
 * @return [0; 1]
 */
PureFW.Widget.get_opacity = function() {
	return this.opacity;
}

/**
 * Fügt das Widget schließlich in den DOM-Baum ein. Diese Funktion sollte
 * nur genau einmal aufgerufen werden, um die Entsprechung des Widgets im
 * DOM-Baum zu erzeugen. Wann sie aufgerufen wird, bleibt dem Entwickler
 * überlassen (ein Verzögerter Aufruf ist oftmals sinnvoll, daher nicht
 * direkt im Konstruktor)
 */
PureFW.Widget.insert_into_dom_tree = function() {
//		if (this.id && document.getElemById(this.id));
//			throw new Error("Widget already inserte into DOM tree.");
//			alert(this.id);
};

/**
 * Löscht das Objekt nicht nur aus dem JS-Speicher, sondern dessen 
 * Repräsentation auch aus dem DOM-Baum.
 */
PureFW.Widget.destroy = function() {
	// Widget direkt ausblenden, da das eigentliche Löschen etwas dauern 
	// kann und der User direkt Feedback bekommen sollte
	var elem = this.get_node();
	if (elem)
		elem.style.display = 'none';
	
	this.on_destroy();
	while (this.on_destroy_functions.length)
		this.on_destroy_functions.pop();
	while (this.on_click_functions.length)
		this.on_click_functions.pop();
	while (this.on_mouseover_functions.length)
		this.on_mouseover_functions.pop();
	while (this.on_mouseout_functions.length)
		this.on_mouseout_functions.pop();
	while (this.on_mousedown_functions.length)
		this.on_mousedown_functions.pop();
	while (this.on_mouseup_functions.length)
		this.on_mouseup_functions.pop();
	while (this.on_hide_functions.length)
		this.on_hide_functions.pop();
	while (this.on_show_functions.length)
		this.on_show_functions.pop();
	while (this.on_fade_out_begin_functions.length)
		this.on_fade_out_begin_functions.pop();
	while (this.on_fade_out_end_functions.length)
		this.on_fade_out_end_functions.pop();
	while (this.on_fade_in_begin_functions.length)
		this.on_fade_in_begin_functions.pop();
	while (this.on_fade_in_end_functions.length)
		this.on_fade_in_end_functions.pop();
	if (this.active_tweens) {
		for (var i in this.active_tweens) {
			if (this.active_tweens[i]) {
				this.active_tweens[i].stopEnterFrame();
				delete this.active_tweens[i];
				this.active_tweens[i] = null;
			}
		}
	}
	delete this.active_tweens;
	this.active_tweens = null;
	
	var nodes = this.parent_node.childNodes;
	var node = null;
	for (i = 0; i < nodes.length; i++) {
		if (nodes[i].id == this.id) {
			node = nodes[i];
			break;
		}
	}
	if (node) {
		this.parent_node.removeChild(node);
	}
	PureFW.WidgetManager.manager_all.unregister_widget(this);
	try {
		delete this;
	}
	catch(e){}
};

/**
 * Setzt einen neuen Vaterknoten im DOM-Baum, an den das Widget gehängt 
 * wird.
 * 
 * @param HTMLElement p_node
 */
PureFW.Widget.set_parent_node = function(p_node) {
	if (this.parent_node && this.id && (this.parent_node !== p_node)) {
		var nodes = this.parent_node.childNodes;
		var node = null;
		for (i = 0; i < nodes.length; i++) {
			if (nodes[i].id == this.id) {
				node = nodes[i];
				break;
			}
		}
		if (node) {
			this.parent_node.removeChild(node);
			p_node.appendChild(node);
		}
	}
	this.parent_node = p_node;
};

/**
 * Gibt den aktuellen DOM-Vaterknoten zurück.
 * 
 * @return HTMLElement
 */
PureFW.Widget.get_parent_node = function() { 
	return this.parent_node; 
};

/**
 * Prüft ob das Widget durch den übergebenen Konstruktor erzeugt wurde,
 * oder ob der Konstruktor, der das Widget erzeugt hat, von dem übergebenen
 * Objekt geerbt hat. Somit ist dies eine Implementierung des instanceof-
 * Operators aus OO-Sprachen.
 * 
 * @param Object o
 */
PureFW.Widget.instance_of = function(o) {
	return PureFW.instance_of(this, o);
};

/**
 * Gibt den Knoten des DOM-Baumes zurück, mit dem das Widget verbunden ist.
 * 
 * @return HTMLElement
 */
PureFW.Widget.get_node = function() {
	if (!this.id && this.node)
		this.id = this.node.id;
	
	if (this.id) {
		var tmp = document.getElemById(this.id);
		if (tmp)
			this.node = tmp;
	}
	return this.node;
};

/**
 * Erzeugt ein nicht DOM-Standard-Event, welches nur für PureFW-Widgets
 * gilt.
 * 
 * @param string type
 * @param int detail [optional]
 * @return Event
 */
PureFW.Widget.create_event = function(type, detail) {
	return PureFW.EventUtil.create_event(type, detail);
};

/**
 * Verhindert, dass Text oder Bilder im Widget markiert werden können.
 */
PureFW.Widget.disable_selection = function(target) {
	if (typeof(target) == 'undefined')
		target = this.get_node();
	
	if (typeof target.onselectstart!="undefined") //IE route
		target.onselectstart=function(){return false};
	else if (typeof target.style.MozUserSelect!="undefined") //Firefox route
		target.style.MozUserSelect="none";
	else
		target.onmousedown=function(){return false};
	if (target.style.cursor != 'pointer')
		target.style.cursor = "default";
}
