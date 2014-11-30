/**
 * Repräsentiert die Leiste, die den aktuellen XP-Stand anzeigt
 */
if (typeof(PureFW.MapUI) != 'object')
	PureFW.MapUI = new Object();

/**
 * @param HTMLElement parent_node
 * @param int x
 * @param int y
 * @param uint w
 * @param uint h
 * @param uint bar_w		Breite der Bar (ohne den Stern)
 */
PureFW.MapUI.XPBar = function(parent_node, x, y, w, h, bar_w, no_scale) {
	if (typeof(parent_node) !== 'undefined') {
		this.init(parent_node, x, y, w, h, bar_w, no_scale);
	
		this.instance_num = this.constructor.num_instances;
		this.instance_name = 
			"PureFW.MapUI.XPBar.instances["+this.instance_num+"]";
		
		this.id = "XPBar"+this.instance_num;
		this.bg_img_id = "XPBarBg"+this.instance_num;
		this.content_id = "XPBarCont"+this.instance_num;
		
		this.insert_into_dom_tree();
		this.constructor.num_instances++;
		this.constructor.instances[this.instance_num] = this;
		
		if (!this.no_scale)
			PureFW.WidgetManager.manager_all.register_widget(this);
	}
};
/********************************************************************
 * Avatar extends Container
 ********************************************************************/
PureFW.MapUI.XPBar.extend(PureFW.Container);

/********************************************************************
 * Statische Deklarationen
 ********************************************************************/
PureFW.MapUI.XPBar.num_instances = 0;
PureFW.MapUI.XPBar.instances = new Array();

/********************************************************************
 * Prototyp-Deklarationen
 ********************************************************************/
PureFW.MapUI.XPBar.prototype.init = function(parent_node, x, y, w, h, bar_w,
	no_scale) 
{
	PureFW.MapUI.XPBar.parent.init.call(this, parent_node, x, y, w, h, no_scale);

	this.bar_width = (bar_w > w) ? w : bar_w;
	
	this.xp_score = 0;
	this.xp_level = 0;
	this.xp_score_next_level = 0;
	this.xp_score_after_next_level = 0;
	this.xp_score_last_level = 0;
	
	this.bar_frame = null;
	this.bar_pic = null;
	this.overlay_cont = null;
	this.xp_level_cont = null;
	this.xp_score_cont = null;
	this.xp_level_up_ani = null;
	this.lvl_up_present_pic = null;
	this.lvl_up_present = new Object();
};

PureFW.MapUI.XPBar.prototype.insert_into_dom_tree = function() {
	PureFW.MapUI.XPBar.parent.insert_into_dom_tree.call(this);
	this.set_bg_img(
		'ui/backgrounds/XPBar/bottom_shadow.png'
	);
	this.bar_frame = this.add_widget(
		PureFW.Container,
		this.width - this.bar_width, 0,
		this.bar_width, this.height
	);
	this.bar_pic = this.bar_frame.add_widget(
		PureFW.Image,
		0, 0,
		this.bar_width, this.height,
		'ui/backgrounds/XPBar/middle_green_bar.png'
	);
	this.overlay_cont = this.add_widget(
		PureFW.Container,
		0, 0,
		this.width, this.height
	);
	this.overlay_cont.set_bg_img(
		'ui/backgrounds/XPBar/top_star.png'
	);
	this.overlay_cont.set_z_index(1);
	this.xp_score_cont = this.overlay_cont.add_widget(
		PureFW.Container,
		50, 20,
		170, 20
	);
	this.xp_score_cont.set_css_class('XPBar_score');
	this.xp_level_cont = this.overlay_cont.add_widget(
		PureFW.Container,
		14, 20,
		32, 20
	);
	this.xp_level_cont.set_css_class('XPBar_level');
	
	
	this.xp_level_up_ani = new PureFW.FilmAnimator(
		this.parent_node,
		this.position.x - 184,
		this.position.y - 12,
		424, 150
	);
	this.xp_level_up_ani.set_image(
		'ui/backgrounds/XPBar/level_up_ani.png', 8
	);
	this.xp_level_up_ani.set_z_index(this.get_z_index()-1);
	this.xp_level_up_ani.hide();
	this.xp_level_up_ani.add_event_handler("loop",
		(function(_this) {
			return function(ev) {
				/**
				 * Wenn die Animation einmal abgespielt wurde, dann das
				 * Widget gleich wieder verbergen ....
				 */
				_this.xp_level_up_ani.hide();
				/**
				 * ... und erst jetzt die Bar aktuallisieren!
				 */
				_this.xp_score_last_level 
					= _this.xp_score_next_level;
				_this.set_xp_level(_this.xp_level+1);
				_this.xp_score_next_level 
					= _this.xp_score_after_next_level;
				
				_this.xp_score_after_next_level = 0;
				
				_this._recalc_bar_length();
				_this.xp_score_cont.set_content(
					_this.xp_score + '/' + 
						_this.xp_score_next_level
				);
				PureFW.AJAXClientServer.send_request(
					"ingame_update.php?xp_bar_data=true",
					function(response_arr) {
						if (response_arr[0] != '1') {
							throw new Error(
								"Got wronge response in XPBar.set_xp_score: "
										+ response_arr);
						}
						var data_obj = eval('('+response_arr[1]+')');
						_this.set_level_up_present(
							data_obj.lvl_up_present.type,
							data_obj.lvl_up_present.id,
							parseInt(data_obj.lvl_up_present.blueprint)
						);
					}
				);
			}
		})(this)
	);
	
	this._recalc_bar_length();
	
	this.lvl_up_present_pic = new PureFW.Image(
		this.parent_node,
		this.position.x + this.width - 10,
		this.position.y + (this.height - 73),
		73, 73,
		'pattern/spacer.gif'
	);
};

/**
 * Berechnet die Größe der Bar neu anhand der gegebenen score und score-max
 * Werte.
 */
PureFW.MapUI.XPBar.prototype._recalc_bar_length = function() {
	if ((this.xp_score_next_level - this.xp_score_last_level <= 0)
		|| ((this.xp_score - this.xp_score_last_level) <= 0) )
	{
		this.bar_frame.set_width(0);
	}
	else {
		this.factor = (this.xp_score - this.xp_score_last_level) /
				(this.xp_score_next_level - this.xp_score_last_level);
		
		if (this.factor > 1)
			this.factor = 1;
		
		this.bar_frame.set_width(this.factor * (this.bar_width));
	}		
}

/**
 * Setzt die Punktzahl, die der Spieler aktuell hat.
 * 
 * @param uint score
 */
PureFW.MapUI.XPBar.prototype.set_xp_score = function(score) {
	this.xp_score = parseInt(score);
	/**
	 * LEVEL UP :)
	 */
	if (this.xp_score > this.xp_score_next_level) {
		/**
		 * Animation abspielen:
		 */
		
		this.xp_level_up_ani.show();
		this.xp_level_up_ani.start_animation();
		PureFW.Timeout.set_timeout(
			function() {
				EventMessage.get_new_events_now();
			}, 1000);
	}
	/**
	 * Sonst, wenn kein Level-Up, einfach die Werte anpassen.
	 */
	else {
		this._recalc_bar_length();
		this.xp_score_cont.set_content(
			this.xp_score + '/' + this.xp_score_next_level
		);
	}
}

/**
 * Gibt die Punktzahl zurück.
 * 
 * @return uint
 */
PureFW.MapUI.XPBar.prototype.get_xp_score = function() {
	return this.xp_score;
}

/**
 * Fügt die angegebene Punktzahl der aktuellen hinzu.
 * 
 * @param uint score
 */
PureFW.MapUI.XPBar.prototype.add_xp_score = function(score) {
	this.set_xp_score(this.xp_score + parseInt(score));
}

/**
 * Setzt die Punktzahl, die der Spieler braucht, um das nächste XP-Level
 * zu erreichen.
 * 
 * Das ist also die maximale Punktzahl, wenn sie erreicht ist, ist der Balken
 * voll.
 * @param uint score
 */
PureFW.MapUI.XPBar.prototype.set_xp_score_next_level = function(score) {
	this.xp_score_next_level = parseInt(score);
	this._recalc_bar_length();
	this.xp_score_cont.set_content(
		this.xp_score + '/' + this.xp_score_next_level
	);
}

/**
 * Setzt die Punktzahl, die der Spieler braucht, um das übernächste XP-Level
 * zu erreichen.
 * 
 * Diese Punktzahl wird im Falle eines Level-Ups benötigt, damit next_level
 * korrekt initialisiert werden kann.
 * @param uint score
 */
PureFW.MapUI.XPBar.prototype.set_xp_score_after_next_level = function(score) {
	this.xp_score_after_next_level = parseInt(score);
}
/**
 * Setzt die Punktzahl, die der Spieler brauchte, um das aktuelle XP-Level
 * zu erreichen.
 * 
 * Bis zu dieser Punktzahl bleibt der Balken leer.
 * @param uint score
 */
PureFW.MapUI.XPBar.prototype.set_xp_score_last_level = function(score) {
	this.xp_score_last_level = parseInt(score);
	this._recalc_bar_length();
}

/**
 * Setzt das anzuzeigende XP-Level.
 * 
 * @param uint lvl
 */
PureFW.MapUI.XPBar.prototype.set_xp_level = function(lvl) {
	this.xp_level = parseInt(lvl);
	this.xp_level_cont.set_content(this.xp_level);
}

/**
 * Setzt das Item, das der Spieler als Geschenk beim Level-Up erhalten soll.
 * 
 * @param uint type
 * @param uint id
 * @param bool bb		Ob es sich nur um ein Blueprint handelt (Teaser)
 */
PureFW.MapUI.XPBar.prototype.set_level_up_present = function(type, id, bb) {
	this.lvl_up_present.type = type;
	this.lvl_up_present.id = id;
	this.lvl_up_present.bb = bb;
	
	if (bb) {
		this.lvl_up_present_pic.set_pic_url(
			'ui/icons/labels/XPBar/presents/'+type+'/'+id+'_b.png'
		);
	}
	else {
		this.lvl_up_present_pic.set_pic_url(
			'ui/icons/labels/XPBar/presents/'+type+'/'+id+'.png'
		);
	}
}

/**
 * Setzt den z-Index.
 * @param z_index
 * @see PureFW.Widget#set_z_index
 */
PureFW.MapUI.XPBar.prototype.set_z_index = function(z_index) {
	PureFW.MapUI.XPBar.parent.set_z_index.call(this, z_index);
	this.xp_level_up_ani.set_z_index(z_index-1);
	this.lvl_up_present_pic.set_z_index(z_index-1);
}

PureFW.MapUI.XPBar.prototype.hide = function() {
	PureFW.MapUI.XPBar.parent.hide.call(this);
	this.lvl_up_present_pic.hide();
}

PureFW.MapUI.XPBar.prototype.show = function() {
	PureFW.MapUI.XPBar.parent.show.call(this);
	this.lvl_up_present_pic.show();
}

/**
 * Zerstört das Widget
 * @see PureFW.Widget#set_z_index
 */
PureFW.MapUI.XPBar.prototype.destroy = function() {
	this.xp_level_up_ani.destroy();
	this.lvl_up_present_pic.destroy();
	PureFW.MapUI.XPBar.parent.destroy.call(this);
}