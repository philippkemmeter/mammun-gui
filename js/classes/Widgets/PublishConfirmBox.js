/**
 * Eine Confirmation-Box, also eine Box mit einer zu bestätigenden Nachricht,
 * die als wichtigste Option das Veröffentlichen der Meldung über soziale 
 * Netzwerke etc ermöglicht.
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
 * @see PureFW.PublishConfirmBox
 */
PureFW.PublishConfirmBox = function(parent_node, x, y, w, h, buttons, grey) {
	if (typeof(parent_node) !== 'undefined') {
		this.init(parent_node, x, y, w, h, buttons, grey);
		
		this.instance_num = this.constructor.num_instances;
		this.instance_name = 
			"PureFW.PublishConfirmBox.instances["+this.instance_num+"]";
		this.id = "PublishConfirmBox"+this.instance_num;
		this.bg_img_id = "PublishConfirmBox_bg_img"+this.instance_num;
		this.content_id = "PublishConfirmBox_cont"+this.instance_num;
		this.grey_layer_node_id ="PublishConfirmBox_grey_layer"
			+this.instance_num;

		this.insert_into_dom_tree();
		this.constructor.num_instances++;
		this.constructor.instances[this.instance_num] = this;
		PureFW.WidgetManager.manager_all.register_widget(this);
	}
};

/********************************************************************
* PublishConfirmBox extends ConfirmationBox
********************************************************************/
PureFW.PublishConfirmBox.extend(PureFW.ConfirmationBox);

/********************************************************************
 * Statische Deklarationen
 ********************************************************************/
PureFW.PublishConfirmBox.num_instances = 0;
PureFW.PublishConfirmBox.instances = new Array();

PureFW.PublishConfirmBox.YES = 1;
PureFW.PublishConfirmBox.NO = 2;
PureFW.PublishConfirmBox.NO_BUTTONS = 4;

/********************************************************************
 * Prototype-Deklarationen
 ********************************************************************/
PureFW.PublishConfirmBox.prototype.init = function(parent_node, x, y, w, h, 
		buttons, grey) 
{
	PureFW.PublishConfirmBox.parent.init.call(this, parent_node, x, y, w, h,
		buttons, grey);
	
	this.publish_container = null;
	this.on_publish_functions = new Array();
	
};

/**
 * Fügt das Widget in den DOM-Tree ein.
 * @see PureFW.Container#insert_into_dom_tree
 */
PureFW.PublishConfirmBox.prototype.insert_into_dom_tree = function() {
	PureFW.PublishConfirmBox.parent.insert_into_dom_tree.call(this);
	
	var confirm_buttons_position = new PureFW.Point(
		this.position.x + (this.width >> 1) 
			+ this.confirm_button_offset.x,
		this.position.y + this.height - (this.confirm_button_height >> 1) + 10
			+ this.confirm_button_offset.y
	);
	

	if (this.yes_container && this.no_container) {
		var yes_pos_offset = -this.confirm_button_width
			- (this.confirm_button_width>>1);
		var no_pos_offset = -(this.confirm_button_width>>1);
		var publish_pos_offset = (this.confirm_button_width>>1);
	}
	else {
		if (this.yes_container) {
			var yes_pos_offset = -this.confirm_button_width;
		}
		else if (this.no_container) {
			var no_pos_offset = -this.confirm_button_width;
		}
		var publish_pos_offset = 0;
	}

	if (this.yes_container) {
		this.yes_container.set_x(confirm_buttons_position.x + yes_pos_offset);
	}
	if (this.no_container) {
		this.no_container.set_x(confirm_buttons_position.x + no_pos_offset);
		this.no_container.set_pic_url(
			'ui/elements/form/buttons/confirm/confirm_no_grey.png'
		);
	}
	
	this.publish_container = new PureFW.Image(
		this.parent_node,
		confirm_buttons_position.x + publish_pos_offset,
		confirm_buttons_position.y + (this.confirm_button_height - 62) - 5,
		159, 62,
		'ui/elements/form/buttons/'+MammunUI.LNG+'/PublishConfirmBox'+
			'/btn_share.png'
	);
	this.publish_container.set_z_index(this.get_z_index() + 1);
	this.publish_container.add_event_handler(
		"click",
		(function(_instance) {
			return function(ev) {
				_instance.on_publish();
			};
		})(this)
	);
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
PureFW.PublishConfirmBox.prototype.set_x = function(x, ms, easing, on_finish) {
	PureFW.Container.prototype.set_x.call(this, x, ms, easing, on_finish);
	var x = x + (this.width >> 1) + this.confirm_button_offset.x;
	
	if (this.yes_container && this.no_container) {
		var yes_pos_offset = -this.confirm_button_width
			- (this.confirm_button_width>>1);
		var no_pos_offset = -(this.confirm_button_width>>1);
		var publish_pos_offset = (this.confirm_button_width>>1);
	}
	else {
		if (this.yes_container) {
			var yes_pos_offset = -this.confirm_button_width;
		}
		else if (this.no_container) {
			var no_pos_offset = -this.confirm_button_width;
		}
		var publish_pos_offset = 0;
	}

	if (this.yes_container) {
		this.yes_container.set_x(x + yes_pos_offset);
	}
	if (this.no_container) {
		this.no_container.set_x(x + no_pos_offset);
	}
	if (ms)
		this.publish_container.hide();
	this.publish_container.set_x(x + publish_pos_offset);
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
PureFW.PublishConfirmBox.prototype.set_y = function(y, ms, easing, on_finish) {
	PureFW.PublishConfirmBox.parent.set_y.call(this, y, ms, easing, on_finish);
	var y = y + this.height - (this.confirm_button_height >> 1)
				+ this.confirm_button_offset.y;
	if (this.yes_container)
		this.yes_container.set_y(y);
	if (this.no_container)
		this.no_container.set_y(y);
	if (ms)
		this.publish_container.hide();
	this.publish_container.set_y(y - (this.confirm_button_height - 62));
};


/**
 * Setzt den z-index.
 * @see PureFW.Widget#set_z_index
 */
PureFW.PublishConfirmBox.prototype.set_z_index = function(z) {
	PureFW.PublishConfirmBox.parent.set_z_index.call(this, z);
	if (!this.create_grey_bg) 
		this.publish_container.set_z_index(z+1);
};

PureFW.PublishConfirmBox.prototype.destroy = function() {
	PureFW.PublishConfirmBox.parent.destroy.call(this);
	this.publish_container.destroy();
};

PureFW.PublishConfirmBox.prototype.hide = function() {
	PureFW.PublishConfirmBox.parent.hide.call(this);
	this.publish_container.hide();
};

PureFW.PublishConfirmBox.prototype.show = function() {
	PureFW.PublishConfirmBox.parent.show.call(this);
	this.publish_container.hide();
};

PureFW.PublishConfirmBox.prototype.fade_in = function(ms, max_opacity, frame_ms) 
{
	PureFW.PublishConfirmBox.parent.fade_in.call(this);
	this.publish_container.fade_in(ms, max_opacity, frame_ms);
};

PureFW.PublishConfirmBox.prototype.fade_out = function(ms, max_opacity, frame_ms){
	PureFW.PublishConfirmBox.parent.fade_out.call(this);
	this.publish_container.fade_out(ms, max_opacity, frame_ms);
};


/**
 * Fügt einen EventHandler hinzu. Funktion überschrieben, da zusätzlich
 * der Typ "publish" unterstützt wird, welcher beim Anklicken des Publish-
 * Buttons ausgelöst wird.
 * 
 * @param string type
 * @param Function fn
 * @see PureFW.Widget#add_event_handler
 */
PureFW.PublishConfirmBox.prototype.add_event_handler = function(type, fn) {
	if (type === "publish")
		this.on_publish_functions.push(fn);
	else
		PureFW.PublishConfirmBox.parent.add_event_handler.call(this, type, fn);
};

/**
 * Entfernt einen EventHandler. Funktion überschrieben, da zusätzlich
 * der Typ "publish" unterstützt wird, welcher beim Anklicken des Publish-
 * Buttons ausgelöst wird.
 * 
 * @param string type
 * @param Function fn
 * @see PureFW.PublishConfirmBox#add_event_handler
 */
PureFW.PublishConfirmBox.prototype.remove_event_handler = function(type, fn) {
	if (type === "publish")
		this.on_publish_functions.remove(fn);
	else
		PureFW.PublishConfirmBox.parent.remove_event_handler.call(this, type, 
			fn);
};

/**
 * Ruft das "publish"-Event exlizit auf. Wird normalerweise aufgerufen,
 * wenn der Publish-Knopf gedrückt wurde.
 * 
 * @param Event ev [optional]
 */
PureFW.PublishConfirmBox.prototype.on_publish = function(ev) {
	if (!ev) {
		this.create_event("publish")
	}
	var n = this.on_publish_functions.length;
	for (var i = 0; i < n; i++) {
		this.on_publish_functions[i].call(this, ev);
	}
}

/**
 * Ruft das "cancel"-Event explizit auf. Wird normalerweise aufgerufen,
 * wenn die Box geschlossen wurde, ohne Bestätigung (Drücken von (X)).
 *
 * @param Event ev [optional]
 */
PureFW.PublishConfirmBox.prototype.on_cancel = function(ev) {
	if (!ev) {
		this.create_event("cancel")
	}
	/**
	 * Coole Cancel-Animation
	 */
	var tween = new OpacityTween(
		document.getElemById(this.grey_layer_node_id), 
		null,
		100, 
		0, 
		0.2
	);
	tween.start();
	tween.onMotionFinished = (function(_instance) {
		return function() {
			_instance.destroy();
		}
	})(this);
	
	var n = this.on_cancel_functions.length;
	for (var i = 0; i < n; i++) {
		this.on_cancel_functions[i].call(this,ev);
	}
}