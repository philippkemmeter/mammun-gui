/**
 * Eine Confirmation-Box, also eine Box mit einer zu bestätigenden Nachricht.
 * Das Bestätigen führt dazu, dass etwas ausgeführt wird, das abbrechen schließt
 * einfach nur die Box. Confirmation-Box halt :)
 * 
 * @author Phil
 */

/**
 * @param HTMLElement parent_node
 * @param int x
 * @param int y
 * @param uint w
 * @param uint h
 * @param uint buttons [= YES|NO]
 * @param bool grey 	if yes, then grey background will be created
 * @see PureFW.ConfirmationBox
 */
PureFW.ConfirmationBox = function(parent_node, x, y, w, h, buttons, grey, 
	no_scale) 
{
	if (typeof(parent_node) !== 'undefined') {
		this.init(parent_node, x, y, w, h, buttons, grey, no_scale);
		
		this.instance_num = this.constructor.num_instances;
		this.instance_name = 
			"PureFW.ConfirmationBox.instances["+this.instance_num+"]";
		this.id = "ConfirmationBox"+this.instance_num;
		this.bg_img_id = "ConfirmationBox_bg_img"+this.instance_num;
		this.content_id = "ConfirmationBox_cont"+this.instance_num;
		this.grey_layer_node_id ="ConfirmationBox_grey_layer"+this.instance_num;

		this.insert_into_dom_tree();
		this.constructor.num_instances++;
		this.constructor.instances[this.instance_num] = this;
		
		if (!this.no_scale)
			PureFW.WidgetManager.manager_all.register_widget(this);
	}
};

/********************************************************************
* ConfirmationBox extends Container
********************************************************************/
PureFW.ConfirmationBox.extend(PureFW.Container);

/********************************************************************
 * Statische Deklarationen
 ********************************************************************/
PureFW.ConfirmationBox.num_instances = 0;
PureFW.ConfirmationBox.instances = new Array();

PureFW.ConfirmationBox.YES = 1;
PureFW.ConfirmationBox.NO = 2;
PureFW.ConfirmationBox.NO_BUTTONS = 4;

/********************************************************************
 * Prototype-Deklarationen
 ********************************************************************/
PureFW.ConfirmationBox.prototype.init = function(parent_node, x, y, w, h, 
		buttons, grey, no_scale)
{
	PureFW.ConfirmationBox.parent.init.call(this, parent_node, x, y, w, h,
		no_scale);
	this.org_parent_node = this.parent_node;
	this.on_confirm_functions = new Array();
	this.on_cancel_functions = new Array();
	this.button_flags = buttons || 
		(PureFW.ConfirmationBox.YES | PureFW.ConfirmationBox.NO);
	
	this.yes_container = null;
	this.no_container = null;
	this.grey_layer_node = null;
	this.z_index = 0;
	this.create_grey_bg = grey || false;
	
	
	this.confirm_button_width = 54;
	this.confirm_button_height = 54;
	this.confirm_button_offset = new PureFW.Point(0,-12);
	
};

/**
 * Fügt das Widget in den DOM-Tree ein.
 * @see PureFW.Container#insert_into_dom_tree
 */
PureFW.ConfirmationBox.prototype.insert_into_dom_tree = function() {
	if (this.create_grey_bg) {
		this.grey_layer_node = document.createElement('div');
		this.grey_layer_node.style.width = '100%';
		this.grey_layer_node.style.height = '100%';
		this.grey_layer_node.style.backgroundImage = 
			'url(../pix/ui/backgrounds/confirm/greyout_bg.png)';
		this.grey_layer_node.style.backgroundRepeat = 'repeat';
		this.grey_layer_node.style.position = 'absolute';
		this.grey_layer_node.style.left = 0;
		this.grey_layer_node.style.top = 0;
		this.grey_layer_node.style.zIndex = this.z_index;
		this.grey_layer_node.id = this.grey_layer_node_id;
		this.parent_node.appendChild(this.grey_layer_node);
		this.parent_node = this.grey_layer_node;
	}
	
	PureFW.ConfirmationBox.parent.insert_into_dom_tree.call(this);
	
	this.get_node().className = 'ConfirmationBox';
	
	var confirm_buttons_position = new PureFW.Point(
		this.position.x + (this.width >> 1) 
		+ this.confirm_button_offset.x,
		this.position.y + this.height - (this.confirm_button_height >> 1) + 10
		+ this.confirm_button_offset.y
	);
	var yes = 
		(this.button_flags == (this.button_flags | PureFW.ConfirmationBox.YES));
	var no =
		(this.button_flags == (this.button_flags | PureFW.ConfirmationBox.NO));
	if (yes && no) {
		var yes_pos_offset = -this.confirm_button_width;
		var no_pos_offset = 0;
	}
	else if (yes) {
		var yes_pos_offset = -(this.confirm_button_width>>1);
	}
	else if (no) {
		var no_pos_offset = -(this.confirm_button_width>>1);
	}
	
	if (yes) {
		this.yes_container = new PureFW.Image(
			this.parent_node,
			confirm_buttons_position.x + yes_pos_offset,
			confirm_buttons_position.y,
			this.confirm_button_width,
			this.confirm_button_height,
			'ui/elements/form/buttons/confirm/confirm_yes.png'
		);
		this.yes_container.set_z_index(this.get_z_index() + 1);
		this.yes_container.add_event_handler(
			"click",
			(function(_instance) {
				return function(ev) {
					_instance.on_confirm();
				};
			})(this)
		);
	}
	
	if (no) {
		this.no_container = new PureFW.Image(
			this.parent_node,
			confirm_buttons_position.x + no_pos_offset,
			confirm_buttons_position.y,
			this.confirm_button_width,
			this.confirm_button_height,
			'ui/elements/form/buttons/confirm/confirm_no.png'
		);
		this.no_container.set_z_index(this.get_z_index() + 1);
		this.no_container.add_event_handler(
			"click",
			(function(_instance) {
				return function(ev) {
					_instance.on_cancel();
				};
			})(this)
		);
	}
	
	this.set_z_index(300);
};

/**
 * Setzt die x-Position des Widgets. Überschrieben, dass die YES-, NO-Buttons
 * ebenfalls korrekt rearrangiert werden.
 * 
 * @param uint x
 * @param uint ms			Positionssetzung soll animiert werden mit
 * 							Motiontweening und so viele Millisekunden lang
 * 							dauern.
 * @param Function easing	Funktion zum glätten der Animation
 * @param Function on_finish Funktion, die am Ende der Animation aufgerufen
 * 							 werden soll
 */
PureFW.ConfirmationBox.prototype.set_x = function(x, ms, easing, on_finish) {
	if (ms) {
		if (this.yes_container)
			this.yes_container.hide();
		if (this.no_container)
			this.no_container.hide();
		
		on_finish = (function(_instance, _fn) {
			return function() {
				if (_instance.yes_container)
					_instance.yes_container.show();
				if (_instance.no_container)
					_instance.no_container.show();
				if (_fn)
					_fn.call(this);
			}
		})(this, on_finish)
	}
	var x_b = x + (this.width >> 1) 
				+ this.confirm_button_offset.x;
	PureFW.ConfirmationBox.parent.set_x.call(this, x, ms, easing, on_finish);
	var yes = 
		(this.button_flags == (this.button_flags | PureFW.ConfirmationBox.YES));
	var no =
		(this.button_flags == (this.button_flags | PureFW.ConfirmationBox.NO));
	if (yes && no) {
		var yes_pos_offset = -this.confirm_button_width;
		var no_pos_offset = 0;
	}
	else if (yes) {
		var yes_pos_offset = -(this.confirm_button_width>>1);
	}
	else if (no) {
		var no_pos_offset = -(this.confirm_button_width>>1);
	}
	if (yes)
		this.yes_container.set_x(x_b + yes_pos_offset);
	if (no)
		this.no_container.set_x(x_b + no_pos_offset);
};

/**
 * Setzt die y-Position des Widgets. Überschrieben, dass die YES-, NO-Buttons
 * ebenfalls korrekt rearrangiert werden.
 * 
 * @param uint y
 * @param uint ms			Positionssetzung soll animiert werden mit
 * 							Motiontweening und so viele Millisekunden lang
 * 							dauern.
 * @param Function easing	Funktion zum glätten der Animation
 * @param Function on_finish Funktion, die am Ende der Animation aufgerufen
 * 							 werden soll
 */
PureFW.ConfirmationBox.prototype.set_y = function(y, ms, easing, on_finish) {
	if (ms) {
		if (this.yes_container)
			this.yes_container.hide();
		if (this.no_container)
			this.no_container.hide();
		on_finish = (function(_instance, _fn) {
			return function() {
				if (_instance.yes_container)
					_instance.yes_container.show();
				if (_instance.no_container)
					_instance.no_container.show();
				if (_fn)
					_fn.call(this);
			}
		})(this, on_finish)
	}
	var y_b = y + this.height - (this.confirm_button_height >> 1)
		+ this.confirm_button_offset.y;
	PureFW.ConfirmationBox.parent.set_y.call(this, y, ms, easing, on_finish);
	
	if (this.yes_container)
		this.yes_container.set_y(y_b);
	if (this.no_container)
		this.no_container.set_y(y_b);
};


/**
 * Setzt den z-index.
 * @see PureFW.Widget#set_z_index
 */
PureFW.ConfirmationBox.prototype.set_z_index = function(z) {
	if (this.create_grey_bg) {
		this.z_index = z;
		this.grey_layer_node.style.zIndex = z;
	}
	else {
		PureFW.ConfirmationBox.parent.set_z_index.call(this,z);
		if (this.yes_container)
			this.yes_container.set_z_index(z+1);
		if (this.no_container)
			this.no_container.set_z_index(z+1);
	}
};

PureFW.ConfirmationBox.prototype.destroy = function() {
	if (this.create_grey_bg) {
		try {
			this.org_parent_node.removeChild(this.grey_layer_node);
		}
		catch(e) {}
	}
	PureFW.ConfirmationBox.parent.destroy.call(this);
	if (this.yes_container)
		this.yes_container.destroy();
	if (this.no_container)
		this.no_container.destroy();
};

PureFW.ConfirmationBox.prototype.hide = function() {
	PureFW.ConfirmationBox.parent.hide.call(this);
	if (this.yes_container)
		this.yes_container.hide();
	if (this.no_container)
		this.no_container.hide();
};

PureFW.ConfirmationBox.prototype.show = function() {
	PureFW.ConfirmationBox.parent.show.call(this);
	if (this.yes_container)
		this.yes_container.show();
	if (this.no_container)
		this.no_container.show();
};

PureFW.ConfirmationBox.prototype.fade_in = function(ms, max_opacity, frame_ms) {
	PureFW.ConfirmationBox.parent.fade_in.call(this);
	if (this.yes_container)
		this.yes_container.fade_in(ms, max_opacity, frame_ms);
	if (this.no_container)
		this.no_container.fade_in(ms, max_opacity, frame_ms);
};

PureFW.ConfirmationBox.prototype.fade_out = function(ms, max_opacity, frame_ms){
	PureFW.ConfirmationBox.parent.fade_out.call(this);
	if (this.yes_container)
		this.yes_container.fade_out(ms, max_opacity, frame_ms);
	if (this.no_container)
		this.no_container.fade_out(ms, max_opacity, frame_ms);
};


/**
 * Fügt einen EventHandler hinzu. Funktion überschrieben, da zusätzlich
 * der Typ "confirm" unterstützt wird, welcher beim Bestätigen des Fensters
 * ausgelöst wird (also beim Klick auf "YES"), sowie "cancel" bei Klick auf 
 * "NO".
 * 
 * @param string type
 * @param Function fn
 * @see PureFW.Widget#add_event_handler
 */
PureFW.ConfirmationBox.prototype.add_event_handler = function(type, fn) {
	if (type === "confirm")
		this.on_confirm_functions.push(fn);
	else if (type === "cancel")
		this.on_cancel_functions.push(fn);
	else
		PureFW.ConfirmationBox.parent.add_event_handler.call(this, type, fn);
};

/**
 * Entfernt einen EventHandler. Funktion überschrieben, da zusätzlich
 * der Typ "confirm" unterstützt wird, welcher beim Bestätigen des Fensters
 * ausgelöst wird (also beim Klick auf "YES"), sowie "cancel" bei Klick auf 
 * "NO".
 * 
 * @param string type
 * @param Function fn
 * @see PureFW.ConfirmationBox#remove_event_handler
 */
PureFW.ConfirmationBox.prototype.remove_event_handler = function(type, fn) {
	if (type === "confirm")
		this.on_confirm_functions.remove(fn);
	else if (type === "cancel")
		this.on_cancel_functions.remove(fn);
	else
		PureFW.ConfirmationBox.parent.remove_event_handler.call(this, type, fn);
};

/**
 * Ruft das "confirm"-Event exlizit auf. Wird normalerweise aufgerufen,
 * wenn die Box bestätigt wurde.
 * 
 * @param Event ev [optional]
 */
PureFW.ConfirmationBox.prototype.on_confirm = function(ev) {
	if (!ev) {
		this.create_event("confirm");
	}
	
	var n = this.on_confirm_functions.length;
	for (var i = 0; i < n; i++) {
		this.on_confirm_functions[i].call(this, ev);
	}
	
	if ((typeof(Tween) == 'undefined') || (typeof(OpacityTween) == 'undefined'))
	{
		this.destroy();
	}
	else {
		/**
		 * Coole Confirm-Animation
		 */
		this.set_y(this.height-100, 500, Tween.backEaseIn,
			(function(_instance) {
				return function() {
					_instance.hide();
					var tween = new OpacityTween(
						document.getElemById(_instance.grey_layer_node_id), 
						null,
						100, 
						0, 
						0.2
					);
					tween.start();
					tween.onMotionFinished = function() {
						_instance.destroy();
					}
				};
			})(this)
		);
		this.set_x(this.get_x() 
			-1500 + 3000*Math.random(), 500, Tween.backEaseIn
		);
	}
}

/**
 * Ruft das "cancel"-Event explizit auf. Wird normalerweise aufgerufen,
 * wenn die Box geschlossen wurde, ohne Bestätigung (Drücken von (X)).
 *
 * @param Event ev [optional]
 */
PureFW.ConfirmationBox.prototype.on_cancel = function(ev) {
	if (!ev) {
		this.create_event("cancel");
	}
	
	var n = this.on_cancel_functions.length;
	for (var i = 0; i < n; i++) {
		this.on_cancel_functions[i].call(this,ev);
	}

	if ((typeof(Tween) == 'undefined') || (typeof(OpacityTween) == 'undefined'))
	{
		this.destroy();
	}
	else {
		var max_y = (PureFW.WidgetManager.page_aspect_ratio) 
		? PureFW.WidgetManager.scale_reference_value / 
			PureFW.WidgetManager.page_aspect_ratio 
			: 0;
		
		if (max_y) {
			/**
			 * Coole Cancel-Animation
			 */
			this.set_y(
				max_y, 600, 
				Tween.bounceEaseOut,
				(function(_instance) {
					return function() {
						var tween = new OpacityTween(
							document.getElemById(_instance.grey_layer_node_id), 
							null,
							100, 
							0, 
							0.2
						);
						tween.start();
						tween.onMotionFinished = function() {
							_instance.destroy();
						}
					};
				})(this)
			);
		}
		else {
			this.add_event_handler("fade_out_end", (function(_this) {
				return function(ev) {
					_this.destroy();
				}
			})(this));
			this.fade_out(500);
		}
	}
	
}