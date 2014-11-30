/**
 * Hier werden Maus-Features wie Drag&Drop geboten.
 * 
 * @author Phil
 */

PureFW.MouseFeatures = new Object();

/**
 * Gibt die aktuelle Cursor-Position zurück
 */
PureFW.MouseFeatures.get_cursor_pos = function() {
	return PureFW.MouseFeatures.__cursor_pos;
}

/**
 * @access private
 * @var PureFW.Point 	aktuelle Cursor-Position
 */
PureFW.MouseFeatures.__cursor_pos = new PureFW.Point(0,0);

/**
 * Setzt die aktuelle Cursor-Position anhand des übergebenen Maus-Events.
 * 
 * @param MouseEvent ev
 * @access private
 */
PureFW.MouseFeatures.__update_cursor_pos_by_event = function(ev) {
	try {
		ev = PureFW.EventUtil.formatEvent(ev);
	}
	catch(e) {}
	try {
		if (ev.clientX && ev.clientY) {
			PureFW.MouseFeatures.__cursor_pos.x = ev.clientX 
				+ document.getScrollX();
			PureFW.MouseFeatures.__cursor_pos.y = ev.clientY 
				+ document.getScrollY();
		}
		else if (ev.pageX && ev.pageY) {
			PureFW.MouseFeatures.__cursor_pos.x = ev.pageX;
			PureFW.MouseFeatures.__cursor_pos.y = ev.pageY;
		}
		else {
			PureFW.MouseFeatures.__cursor_pos.x = 0;
			PureFW.MouseFeatures.__cursor_pos.y = 0;
		}
	}
	catch (e) {}	
}

/**
 * (Fast) alles was mit Drag&Drop zu tun hat, wird hierüber gesteuert.
 */
PureFW.MouseFeatures.Dragging = new Object();

PureFW.MouseFeatures.Dragging.RESTRICT_MOVING_NONE = 0;
PureFW.MouseFeatures.Dragging.RESTRICT_MOVING_X = 1;
PureFW.MouseFeatures.Dragging.RESTRICT_MOVING_Y = 2;

/**
 * @access private
 * @var HTMLElement		Der Knoten, der bewegt werden soll
 */
PureFW.MouseFeatures.Dragging.__cur_node = null;
/**
 * @access private
 * @var ENUM			Ob die Bewegung beim Draggen auf eine Achse beschränkt 
 * 						ist, und wenn ja, auf welche.
 */
PureFW.MouseFeatures.Dragging.__restricted_direction = 
	PureFW.MouseFeatures.Dragging.RESTRICT_MOVING_NONE;
/**
 * @access private
 * @var	PureFW.Point	An welcher Stelle sich der Maus-Cursor befand, als das 
 * 						Dragging gestartet wurde.
 */
PureFW.MouseFeatures.Dragging.__start_cursor_pos = new PureFW.Point(0,0);
/**
 * @access private
 * @var	PureFW.Point	An welcher Stelle sich das HTML-Element befand, als das 
 * 						Dragging gestartet wurde.
 */
PureFW.MouseFeatures.Dragging.__start_obj_pos = new PureFW.Point(0,0);
/**
 * @access private
 * @var	PureFW.Point	Hierüber kann das Dragging räumlich eingeschränkt werden
 * 						durch Angabe eine maximalen x- und y-Position.
 * 						FALSE heißt, dass es keine Einschränkung gibt.
 */
PureFW.MouseFeatures.Dragging.__max_pos = new PureFW.Point(false,false);
/**
 * @access private
 * @var	PureFW.Point	Hierüber kann das Dragging räumlich eingeschränkt werden
 * 						durch Angabe eine minimalen x- und y-Position.
 * 						FALSE heißt, dass es keine Einschränkung gibt.
 */
PureFW.MouseFeatures.Dragging.__min_pos = new PureFW.Point(false,false);
/**
 * @access private
 * @var Function[]		Welche Funktionen beim Draggen aufgerufen werden sollen.
 */
PureFW.MouseFeatures.Dragging.__on_dragging_functions = new Array();
/**
 * @access private
 * @var Function[]		Welche Funktionen beim Drop aufgerufen werden sollen.
 */
PureFW.MouseFeatures.Dragging.__on_drop_functions = new Array();
/**
 * Fügt einen Event-Handler des angegebenen Typs dem Drag-Objekt hinzu.
 * 
 * Mögliche Typen sind "dragging" - Aufruf bei einer jeden Mausbewegung
 * während des Draggens - und "drop" - Aufruf beim Absetzen des zu draggenden
 * Objekts.
 * 
 * @param String type
 * @param Function fn
 */
PureFW.MouseFeatures.Dragging.add_event_handler = function(type, fn) {
	if (typeof (type) !== 'string')
		throw new Error("Widget::remove_event_handler: First argument 'type' " +
				"has to be of type 'string'");
	if (typeof (fn) !== 'function')
		throw new Error("Widget::remove_event_handler: Second argument 'fn' " +
				"has to be of type 'function'");
	if (type == "dragging")
		PureFW.MouseFeatures.Dragging.__on_dragging_functions.push(fn);
	else if (type == "drop") {
		PureFW.MouseFeatures.Dragging.__on_drop_functions.push(fn);
	}
}
/**
 * Entfernt den angegebenen Event-Handler des angegebenen Typs dem Drag-Objekt.
 * 
 * Wird kein Event-Handler angegeben, so werden alle Handler des Typs entfernt.
 * 
 * Mögliche Typen sind "dragging" - Aufruf bei einer jeden Mausbewegung
 * während des Draggens - und "drop" - Aufruf beim Absetzen des zu draggenden
 * Objekts.
 * 
 * @param String type
 * @param Function fn
 */
PureFW.MouseFeatures.Dragging.remove_event_handler = function(type, fn) {
	if (typeof (type) !== 'string')
		throw new Error("Widget::remove_event_handler: First argument 'type' " +
				"has to be of type 'string'");
	if (typeof(fn) == 'undefined') {
		if (type == "dragging") {
			PureFW.MouseFeatures.Dragging.__on_dragging_functions.destroy();
			PureFW.MouseFeatures.Dragging.__on_dragging_functions = null;
			PureFW.MouseFeatures.Dragging.__on_dragging_functions = new Array();
		}
		else {
			PureFW.MouseFeatures.Dragging.__on_drop_functions.destroy();
			PureFW.MouseFeatures.Dragging.__on_drop_functions = null;
			PureFW.MouseFeatures.Dragging.__on_drop_functions = new Array();
		}
	}
	else {
		if (typeof (fn) !== 'function')
			throw new Error("Widget::remove_event_handler: Second argument 'fn' " +
					"has to be of type 'function'");
		if (type == "dragging")
			PureFW.MouseFeatures.Dragging.__on_dragging_functions.remove(fn);
		else if (type == "drop")
			PureFW.MouseFeatures.Dragging.__on_drop_functions.remove(fn);
	}
}

/**
 * Ruft das on-dragging-Event explizit auf. Wird normalerweise während einer 
 * Dragbewegung aufgerufen.
 * 
 * @param Event ev [optional]
 */
PureFW.MouseFeatures.Dragging.on_dragging = function(ev) {
	if (!ev)
		ev = PureFW.EventUtil.create_event("dragging");
	var n = PureFW.MouseFeatures.Dragging.__on_dragging_functions.length;
	for (var i = 0; i < n; i++) {
		PureFW.MouseFeatures.Dragging.__on_dragging_functions[i].call(
			PureFW.MouseFeatures.Dragging.__cur_node, ev);
	}
}
/**
 * Ruft das on-drop-Event explizit auf. Wird normalerweise aufgerufen,
 * wenn das Drag-Objekt abgesetzt wird, also am Ende einer Dragbewegung.
 * 
 * @param Event ev [optional]
 */
PureFW.MouseFeatures.Dragging.on_drop = function(ev) {
	if (!ev)
		ev = PureFW.EventUtil.create_event("drop");
	var n = PureFW.MouseFeatures.Dragging.__on_drop_functions.length;
	for (var i = 0; i < n; i++) {
		PureFW.MouseFeatures.Dragging.__on_drop_functions[i].call(
			PureFW.MouseFeatures.Dragging.__cur_node, ev);
	}
}

/**
 * Startet das Draggen des angegebenen Objekts.
 * 
 * Als <code>object</code> kann entweder ein HTML-Knoten oder ein Widget 
 * angegeben werden.
 * 
 * @param HTMLElement/PureFW.Widget object
 */
PureFW.MouseFeatures.Dragging.start_dragging = function(object) {
	PureFW.MouseFeatures.Dragging.stop_dragging();
	
	if (PureFW.instance_of(object, PureFW.Widget)) {
		object = object.get_node();
	}
	
	PureFW.MouseFeatures.Dragging.__cur_node = object;
	PureFW.MouseFeatures.Dragging.__start_cursor_pos.x 
		= PureFW.MouseFeatures.__cursor_pos.x;
	PureFW.MouseFeatures.Dragging.__start_cursor_pos.y 
		= PureFW.MouseFeatures.__cursor_pos.y;
	
	PureFW.MouseFeatures.Dragging.__start_obj_pos.x
		= parseInt(object.style.left);
	PureFW.MouseFeatures.Dragging.__start_obj_pos.y
		= parseInt(object.style.top);
}
/**
 * Beendet das Draggen.
 */
PureFW.MouseFeatures.Dragging.stop_dragging = function() {
	/**
	 * Alles auf 0 zurücksetzen.
	 */
	PureFW.MouseFeatures.Dragging.__max_pos.x
		= PureFW.MouseFeatures.Dragging.__max_pos.y
		= PureFW.MouseFeatures.Dragging.__min_pos.x
		= PureFW.MouseFeatures.Dragging.__min_pos.y = false;
	
	PureFW.MouseFeatures.Dragging.__restricted_direction = 
		PureFW.MouseFeatures.Dragging.RESTRICT_MOVING_NONE;


	if (PureFW.MouseFeatures.Dragging.__cur_node) {
		PureFW.MouseFeatures.Dragging.on_drop(
			PureFW.EventUtil.create_event("drop", 
				new PureFW.Point(
					parseInt(
						PureFW.MouseFeatures.Dragging.__cur_node.style.left),
					parseInt(
						PureFW.MouseFeatures.Dragging.__cur_node.style.top)
				)
			)
		);
		
		PureFW.MouseFeatures.Dragging.__cur_node = null
	}
	
	PureFW.MouseFeatures.Dragging.remove_event_handler("dragging");
	PureFW.MouseFeatures.Dragging.remove_event_handler("drop");
}
/**
 * Beschränkt die Drag-Bewegung auf die angegebene Achse.
 * 
 * @param enum dir		Gültige Werte sind:
 * 						- PureFW.MouseFeatures.Dragging.RESTRICT_MOVING_X
 * 						- PureFW.MouseFeatures.Dragging.RESTRICT_MOVING_Y
 * 						- PureFW.MouseFeatures.Dragging.RESTRICT_MOVING_NONE
 */
PureFW.MouseFeatures.Dragging.restrict_direction = function(dir) {
	if ((dir < 0) || (dir > 2))
		throw new Error("dir has to be in {0, 1, 2}, '" + dir + "' given.");
	PureFW.MouseFeatures.Dragging.__restricted_direction = dir;
}
/**
 * Setzt die Bewegungsgrenze, in dem sich das Objekt bewegen darf. 
 * 
 * Achtung: Die Grenzen beziehen sich immer auf die obere linke Ecke des 
 * Objekts, Breite und Höhe müssen also vom aufrufer selbst miteingerechnet
 * werden, falls eine solche Einschränkung gewünscht ist.
 * 
 * Jede Koordinate kann sowohl beim Minimum, als auch beim Maximum auf FALSE
 * gesetzt werden, um zu signalisieren, dass es keine Einschränkung geben soll.
 * 
 * @param uint/false min_x
 * @param uint/false min_y
 * @param uint/false max_x
 * @param uint/false max_y
 */
PureFW.MouseFeatures.Dragging.set_pos_limits = function(min_x, min_y, 
	max_x, max_y) 
{
	if (typeof(max_x) == 'undefined')
		max_x = false;
	if (typeof(max_y) == 'undefined')
		max_y = false;
	if (typeof(min_x) == 'undefined')
		min_x = false;
	if (typeof(min_y) == 'undefined')
		min_y = false;
	PureFW.MouseFeatures.Dragging.__max_pos.x = max_x;
	PureFW.MouseFeatures.Dragging.__max_pos.y = max_y;
	PureFW.MouseFeatures.Dragging.__min_pos.x = min_x;
	PureFW.MouseFeatures.Dragging.__min_pos.y = min_y;
}

/**
 * Diese Funktion wird bei jeder Mausbewegung aufgerufen.
 * 
 * Dieser Event-Listener sorgt dafür, dass die Cursor-Position immer aktuell
 * verfügbar ist, sowie, dass Dragging korrekt ausgeführt wird.
 * 
 * @param MouseEvent ev
 * @access private
 */
PureFW.MouseFeatures.on_mouse_move = function(ev) {
	try {
		ev = PureFW.EventUtil.formatEvent(ev);
	}
	catch(e) {}
	
	PureFW.MouseFeatures.__update_cursor_pos_by_event(ev);
	
	if (PureFW.MouseFeatures.Dragging.__cur_node) {
		var new_pos = new PureFW.Point();
		if (PureFW.MouseFeatures.Dragging.__restricted_direction
			!= PureFW.MouseFeatures.Dragging.RESTRICT_MOVING_Y)
		{
			new_pos.x = PureFW.MouseFeatures.Dragging.__start_obj_pos.x
				+ PureFW.MouseFeatures.__cursor_pos.x
				- PureFW.MouseFeatures.Dragging.__start_cursor_pos.x;
			if ((PureFW.MouseFeatures.Dragging.__max_pos.x !== false)
				&& PureFW.MouseFeatures.Dragging.__max_pos.x < new_pos.x)
			{
				new_pos.x = PureFW.MouseFeatures.Dragging.__max_pos.x;
			}
			else if ((PureFW.MouseFeatures.Dragging.__min_pos.x !== false)
				&& PureFW.MouseFeatures.Dragging.__min_pos.x > new_pos.x)
			{
				new_pos.x = PureFW.MouseFeatures.Dragging.__min_pos.x;
			}
			PureFW.MouseFeatures.Dragging.__cur_node.style.left = new_pos.x+'px';
		}
		else
			new_pos.x = PureFW.MouseFeatures.Dragging.__start_obj_pos.x;
		if (PureFW.MouseFeatures.Dragging.__restricted_direction
			!= PureFW.MouseFeatures.Dragging.RESTRICT_MOVING_X)
		{
			new_pos.y = PureFW.MouseFeatures.Dragging.__start_obj_pos.y
				+ PureFW.MouseFeatures.__cursor_pos.y
				- PureFW.MouseFeatures.Dragging.__start_cursor_pos.y;
			if ((PureFW.MouseFeatures.Dragging.__max_pos.y !== false)
				&& PureFW.MouseFeatures.Dragging.__max_pos.y < new_pos.y)
			{
				new_pos.y = PureFW.MouseFeatures.Dragging.__max_pos.y;
			}
			else if ((PureFW.MouseFeatures.Dragging.__min_pos.y !== false)
				&& PureFW.MouseFeatures.Dragging.__min_pos.y > new_pos.y)
			{
				new_pos.y = PureFW.MouseFeatures.Dragging.__min_pos.y;
			}
			PureFW.MouseFeatures.Dragging.__cur_node.style.top = new_pos.y+'px';
		}
		else
			new_pos.y = PureFW.MouseFeatures.Dragging.__start_obj_pos.y;
		
		PureFW.MouseFeatures.Dragging.on_dragging(
			PureFW.EventUtil.create_event("dragging", new_pos)
		);
	}
}
PureFW.EventUtil.add_event_handler(document, "mousemove", 
	PureFW.MouseFeatures.on_mouse_move);

/**
 * Diese Funktion wird aufgerufen, wenn eine Maustaste gehen gelassen wird.
 * 
 * Dieser Event-Listener sorgt dafür, dass ein Drop korrekt ausgeführt wird.
 * 
 * @param MouseEvent ev
 * @access private
 */
PureFW.MouseFeatures.on_mouse_up = function(ev) {
	PureFW.MouseFeatures.Dragging.stop_dragging();
}
PureFW.EventUtil.add_event_handler(document, "mouseup", 
	PureFW.MouseFeatures.on_mouse_up);

/**
 * WheelScrolling-Objekt für Mausrad-Scrollen
 */
PureFW.MouseFeatures.WheelScrolling = new Object();

/**
 * @var HTMLElement		Das aktuell gescrollte Objekt
 * @access private
 */
PureFW.MouseFeatures.WheelScrolling.__cur_node = null;
/**
 * @access private
 * @var	PureFW.Point	Hierüber kann das WheelScrolling räumlich eingeschränkt 
 * 						werden durch Angabe eine maximalen x- und y-Position.
 * 						FALSE heißt, dass es keine Einschränkung gibt.
 */
PureFW.MouseFeatures.WheelScrolling.__max_pos = new PureFW.Point(false,false);
/**
 * @access private
 * @var	PureFW.Point	Hierüber kann das WheelScrolling räumlich eingeschränkt 
 * 						werden durch Angabe eine minimalen x- und y-Position.
 * 						FALSE heißt, dass es keine Einschränkung gibt.
 */
PureFW.MouseFeatures.WheelScrolling.__min_pos = new PureFW.Point(false,false);
/**
 * @access private
 * @var bool	Ob vertikal (true) gescrollt werden soll oder horizontal (false)
 */
PureFW.MouseFeatures.WheelScrolling.__vertical_scrolling = true;
/**
 * @access private
 * @var Function[]		Welche Funktionen beim Draggen aufgerufen werden sollen.
 */
PureFW.MouseFeatures.WheelScrolling.__on_scroll_functions = new Array();

/**
 * Aktiviert das Scrollen für das angegebene Objekt.
 * 
 * Danach wird bei einer Mausradbewegung dieses Objekt verschoben.
 * Das Objekt kann ein Widget oder ein HTMLElement sein.
 * 
 * @param PureFW.Widget/HTMLElement object
 * @param bool vertical=true	(if false: horizontal scroll is done)
 */
PureFW.MouseFeatures.WheelScrolling.start_scrolling = function(object, 
	vertical) 
{
	PureFW.MouseFeatures.WheelScrolling.stop_scrolling();

	if (PureFW.instance_of(object, PureFW.Widget)) {
		object = object.get_node();
	}

	PureFW.MouseFeatures.WheelScrolling.__cur_node = object;
	
	if (typeof(vertical) == 'undefined')
		vertical = true;
	PureFW.MouseFeatures.WheelScrolling.__vertical_scrolling = vertical;
}
/**
 * Beendet das Scrollen.
 */
PureFW.MouseFeatures.WheelScrolling.stop_scrolling = function() {
	/**
	 * Alles auf 0 zurücksetzen
	 */
	PureFW.MouseFeatures.WheelScrolling.__vertical_scrolling = true;
	PureFW.MouseFeatures.WheelScrolling.__min_pos.x
		= PureFW.MouseFeatures.WheelScrolling.__min_pos.y
		= PureFW.MouseFeatures.WheelScrolling.__max_pos.x
		= PureFW.MouseFeatures.WheelScrolling.__max_pos.y = false;
		
	PureFW.MouseFeatures.WheelScrolling.__cur_node = null;
}
/**
 * Fügt einen Event-Handler des angegebenen Typs dem Drag-Objekt hinzu.
 * 
 * Möglicher Typ ist "scroll". Wird aufgerufen, wenn das Mausrad betätigt wird.
 * 
 * @param String type
 * @param Function fn
 */
PureFW.MouseFeatures.WheelScrolling.add_event_handler = function(type, fn) {
	if (typeof (type) !== 'string')
		throw new Error("Widget::remove_event_handler: First argument 'type' " +
				"has to be of type 'string'");
	if (typeof (fn) !== 'function')
		throw new Error("Widget::remove_event_handler: Second argument 'fn' " +
				"has to be of type 'function'");
	if (type == "scroll")
		PureFW.MouseFeatures.WheelScrolling.__on_scroll_functions.push(fn);
}
/**
 * Entfernt den angegebenen Event-Handler des angegebenen Typs dem Drag-Objekt.
 * 
 * Wird kein Event-Handler angegeben, so werden alle Handler des Typs entfernt.
 * 
 * @param String type
 * @param Function fn
 */
PureFW.MouseFeatures.WheelScrolling.remove_event_handler = function(type, fn) {
	if (typeof (type) !== 'string')
		throw new Error("Widget::remove_event_handler: First argument 'type' " +
				"has to be of type 'string'");
	if (typeof(fn) == 'undefined') {
		if (type == "scroll") {
			PureFW.MouseFeatures.WheelScrolling.__on_scroll_functions.destroy();
			PureFW.MouseFeatures.WheelScrolling.__on_scroll_functions = null;
			PureFW.MouseFeatures.WheelScrolling.__on_scroll_functions 
				= new Array();
		}
	}
	else {
		if (typeof (fn) !== 'function')
			throw new Error("Widget::remove_event_handler: Second argument 'fn' " +
					"has to be of type 'function'");
		if (type == "scroll")
			PureFW.MouseFeatures.WheelScrolling.__on_scroll_functions.remove(fn);
	}
}

/**
 * Ruft das on-scroll-Event explizit auf. Wird normalerweise während einer 
 * Mausradbewegung aufgerufen.
 * 
 * @param Event ev [optional]
 */
PureFW.MouseFeatures.WheelScrolling.on_scroll = function(ev) {
	if (!ev)
		ev = PureFW.EventUtil.create_event("scroll");
	var n = PureFW.MouseFeatures.WheelScrolling.__on_scroll_functions.length;
	for (var i = 0; i < n; i++) {
		PureFW.MouseFeatures.WheelScrolling.__on_scroll_functions[i].call(
			PureFW.MouseFeatures.WheelScrolling.__cur_node, ev);
	}
}

PureFW.MouseFeatures.WheelScrolling.__scroll = function(ev) {
	if (PureFW.MouseFeatures.WheelScrolling.__cur_node) {
		var delta = 0;
		try 
		{
			event = PureFW.EventUtil.formatEvent(event);
		}
		catch (e){}
		
	    if (event.wheelDelta)  /* IE/Opera. */
	    {
	        delta = event.wheelDelta/120;
	        if (window.opera)
	                delta = -delta;
	    } 
	    else if (event.detail) 
	    {
	        delta = -event.detail/3;
	    }
	    event.preventDefault();
	 
	    
	    PureFW.MouseFeatures.WheelScrolling.on_scroll(
	    	PureFW.EventUtil.create_event("scroll", delta)
    	)
	}
}