if (typeof(MapObjects) == 'undefined')
	MapObjects = new Object();

MapObjects.Animal = function(parent_node, x, y, w, h, animal, clothes, 
	no_scale) 
{
	if (typeof(parent_node) !== 'undefined') {
	 	this.init(parent_node, x, y, w, h, animal, clothes, no_scale);
	 	this.instance_num = this.constructor.num_instances;
		this.instance_name = "MapObjects.Animal.instances["
			+this.instance_num+"]";

		this.id = 'MapObjects.Animal'+this.instance_num;
		this.bg_img_id = 'MapObjects.AnimalBg'+this.instance_num;
		this.content_id = 'MapObjects.AnimalCont'+this.instance_num;
		
		this.constructor.instances[this.instance_num] = this;
		this.constructor.num_instances++;
		this.insert_into_dom_tree();
		
		PureFW.WidgetManager.manager_all.register_widget(this);
	}
};
/********************************************************************
 * MapObjects.Animal EXTENDS FilmAnimator
 ********************************************************************/
MapObjects.Animal.extend(PureFW.FilmAnimator);

/********************************************************************
 * Statische Deklarationen
 ********************************************************************/
MapObjects.Animal.animals = {
	'megaloceros': {
		'pic_base':	'map/animals/megaloceros/megaloceros_0',
		'poses': 4,
		'width': 90,
		'height': 90,
		'strips': 5,
		'frames': 17,
		'walk_speed': 0.02,	// px pro ms
		'on_click': {
			'animation': {
				'strip': 2,
				'frame_range': [0, 14],
				'frame_delay': 100
			}
		},
		'frame_ranges': [
			[0, 16],
			[0, 11],
			[0, 14],
			[0, 14],
			[0, 14]
		],
		'animations': {
			'stand': [
				{
					'strip': 0,
					'frame_range': [0, 4],
					'frame_delay': 180,
					'yoyo': true
				},
				{
					'strip': 0,
					'frame_range': [0, 6],
					'frame_delay': 180,
					'yoyo': true
				},
				{
					'strip': 0,
					'frame_range': [8, 16],
					'frame_delay': 180
				},
			 	{
					'strip': 0,
					'frame_range': [0, 16],
					'frame_delay': 180
				},
				{
					'strip': 0,
					'frame_range': [0, 16],
					'frame_delay': 180
				},
				{
					'strip': 0,
					'frame_range': [0, 16],
					'frame_delay': 180,
					'yoyo': true
				},
				/*{
					'strip': 2,
					'frame_range': [0, 14],
					'frame_delay': 100
				},*/
				{
					'strip': 2,
					'frame_range': [0, 2],
					'frame_delay': 240,
					'yoyo': true
				}
			],
			'walk': [
				{
					'strip': 1,
					'frame_range': [0, 11],
					'frame_delay': 180
				}
			],
			'fight': [ 
				{
					'strip': 3,
					'frame_range': [0, 14],
					'frame_delay': 100
				}
			],
			'fight_ultimate': [ 
				{
					'strip': 4,
					'frame_range': [0, 14],
					'frame_delay': 100
				}
			]
		}
	}
}

MapObjects.Animal.WALK_DIRECTIONS = [
	{x: 1, y: 1}, 
	{x:-1, y:-1}, 
	{x:-1, y: 1}, 
	{x: 1, y:-1}
];

MapObjects.Animal.PERPSECTIVE = {x: 58, y: 45};

MapObjects.Animal.STATUS_STAND = 0;
MapObjects.Animal.STATUS_WALK = 1;
MapObjects.Animal.STATUS_FIGHT = 2;

MapObjects.Animal.num_instances = 0;			// Instanzüberwachung (Anzahl)
MapObjects.Animal.instances = new Array();

/**
 * Verknüpft die zwei Tiere miteinander, dass sie zusammen interagieren sollen.
 * Das hat z.B. Auswirkungen auf Kampf.
 * 
 * Dabei gibt <code>edge</code> an, an welcher Seite des ersten Tieres das
 * zweite Tier ist. 0 Bedeutet somit, dass das Tier 2 sich an Seite 0 von Tier 1
 * aktuell befindet. Diese Angabe ist nötig, da sich die Tiere zueinander 
 * drehen.
 * 
 * @param [0,1,2,3] edge
 * @param MapObjects.Animal animal1
 * @param MapObjects.Animal animal2
 */
MapObjects.Animal.link_interaction_partners = function(edge, animal1, animal2) {
	animal1._add_interaction_partner(edge, animal2);
	animal2._add_interaction_partner(edge^1, animal1);
}

/********************************************************************
 * Prototyp-Deklarationen
 ********************************************************************/
MapObjects.Animal.prototype.init = function(parent_node, x, y, w, h, animal, 
	clothes, no_scale)
{
	MapObjects.Animal.parent.init.call(this, parent_node, x, y, w, h, no_scale);
	this.animal = animal;
	this.clothes = (!clothes || !isArray(clothes) || clothes.length != 6)
		? [0,0,0,0,0,0] : clothes;
	this.pose = 0;
	this.status = -1;
	this.next_animation = null;
	this.interaction_partners = [[], [], [], []];
	
	this.click_div_offset = new MathExt.LinAlg2D.Vector(0,0);
	
	this.cur_animation = null;
	this.idle_timeout = null;	// Timeout, wie lange das Tier eingefroren ist 
	/**
	 * Bereich in dem er sich frei Bewegen darf. Bei NULL bewegt er sich
	 * niemals selbstständig.
	 */
	this.walk_region = null;
	
	this.start_animation();	// Tiere leben immer :)
	
	this.on_loop_functions.push(
		(function(_instance) {
			/**
			 * Setzt man eine neue Animation, dann wird sie nicht sofort 
			 * abgespielt, sondern erst am Ende der letzten Animation, damit es
			 * schöne Übergänge gibt
			 */
			return function(ev) {
				if (this.yoyo) {
					this.yoyo = false;
					return;
				}
				
				if (_instance.next_animation) {
					_instance._trigger_animation_now(
						_instance.next_animation.name,
						_instance.next_animation.num,
						true
					);
				}
				_instance._set_status(-1);//ggf. Animationswechsel
			}
		})(this)
	);
};

MapObjects.Animal.prototype.insert_into_dom_tree = function() {
	MapObjects.Animal.parent.insert_into_dom_tree.call(this);
	this._set_pose(this.pose);
	
	this.click_div = new PureFW.Image(
		this.parent_node,
		this.position.x + this.click_div_offset.x, 
		this.position.y + this.click_div_offset.y,
		this.width, this.height,
		'pattern/spacer.gif'
	);
	this.click_div.set_z_index(10);
	
	this.click_div.motion_change_helper = {
		'x' : (function(_instance) {
			return function(evt) {
				var v = evt.target._pos;
				_instance.click_div.set_x(
					v / _instance.scale_factor +
					_instance.click_div_offset.x
				);
			};
		})(this),
		'y' : (function(_instance) {
			return function(evt) {
				var v = evt.target._pos;
				_instance.click_div.set_y(
					v / _instance.scale_factor +
					_instance.click_div_offset.y
				);
			};
		})(this)
	}
	
	if (this.animal.on_click && this.animal.on_click.animation) {
		this.click_div.add_event_handler("click",
			(function(_instance, _ani) {
				return function (ev) {
					_instance.stop_animation();
					_instance._trigger_animation_now(_ani);
					_instance.start_animation();
				}
			})(this, this.animal.on_click.animation)
		);
	}
	
	var pic = this.animal.pic_base;
	var n = this.clothes.length;
	for (var i = 0; i < n; i++) {
		pic += '_';
		pic += this.clothes[i];
	}
	pic += '.png';
	
	this.set_image(
		pic,
		this.animal.frames,
		this.animal.strips * this.animal.poses
	);
	var n = this.animal.frame_ranges.length;
	for (var i = 0; i < n; i++) {
		for (var pose = 0; pose < this.animal.poses; pose++) {
			this.set_frame_range(
				this.animal.frame_ranges[i][0],
				this.animal.frame_ranges[i][1],
				this.animal.strips*pose + i
			);
		}
	}
	
	this._set_status(MapObjects.Animal.STATUS_STAND);
	this._trigger_animation_now('stand');
};

/**
 * Bewegt das Tier zum angegebenen Punkt.
 * 
 * @param uint x
 * @param uint y
 * @param Function on_finish
 */
MapObjects.Animal.prototype.walk_to = function(x, y, on_finish) {
	if (this.status == MapObjects.Animal.STATUS_WALK)
		return;
	
	/**          p_goal1				Es soll nach p_goal gelaufen werden.
	 *           /\						Tier kann aber nur diskrete Richtungen
	 *          /  \					einschlagen / bzw. \, deswegen muss
	 *         /    \ 					über p_goal1 gelaufen werden.
	 * 		  /      x p_goal			l_horizontal und l_help treffen sich
	 * 		 /    .   \					im Punkt intersec, der für die 
	 * 		/  .      l_help			Berechnung von p_goal1 nötig sind.
	 *     /            \				
	 *  this.position -- \-----------------------l_horizontal------------
	 *                  intersec
	 */
	var p_goal = new MathExt.LinAlg2D.Vector(x, y);
	var l_horizontal = new MathExt.LinAlg2D.Line(0, this.position.y);
	
	if (p_goal.x == this.position.x)
		p_goal.x = this.position.x+1;	// sonst funktionieren die Berechnungen nicht
	
	if (p_goal.y == this.position.y)
		p_goal.y = this.position.y+1;	// sonst funktionieren die Berechnungen nicht
	
	/**
	 * Herausfinden, in welche Richtung die Hilfsgerade l_help verlaufen
	 * muss.
	 */
	var pose;
	var v_goal_direction = new MathExt.LinAlg2D.Vector();
	if (p_goal.x > this.position.x) {
		v_goal_direction.x = MapObjects.Animal.PERPSECTIVE.x;
		if (p_goal.y > this.position.y) {
			v_goal_direction.y = -MapObjects.Animal.PERPSECTIVE.y;
			pose = 0;
		}
		else {
			v_goal_direction.y = MapObjects.Animal.PERPSECTIVE.y;
			pose = 3;
		}
	}
	else {
		v_goal_direction.x = -MapObjects.Animal.PERPSECTIVE.x;
		if (p_goal.y > this.position.y) {
			v_goal_direction.y = -MapObjects.Animal.PERPSECTIVE.y;
			pose = 2;
		}
		else {
			v_goal_direction.y = MapObjects.Animal.PERPSECTIVE.y;
			pose = 1;
		}
	}
	
	var l_help = MathExt.LinAlg2D.calc_line_mb_by_points(
		p_goal,
		p_goal.add(v_goal_direction)
	);
	
	
	var intersec = MathExt.LinAlg2D.calc_line_intersection(
		l_horizontal, l_help
	);
	var intersec0 = new MathExt.LinAlg2D.Vector(
		intersec.x-this.position.x,
		intersec.y
	);
	delete l_help;
	delete l_horizontal;
	
	var p_goal1 = new MathExt.LinAlg2D.Vector(
		this.position.x + (intersec0.x >> 1),
		this.position.y - v_goal_direction.y
			*((intersec0.x/v_goal_direction.x) >> 1)
	);
	
	
	this._set_pose(pose);
	//Sofort loslaufen, nicht den nächsten Loop abwarten
	this._trigger_animation_now('walk');
	this._set_status(MapObjects.Animal.STATUS_WALK);
	
	var tmp = p_goal.substract(p_goal1);
	var length2 = tmp.get_vector_length();
	delete tmp;

	/**
	 * Wenn er nur eine kurze Korrektur braucht, dann kann er ruhig etwas
	 * schief laufen, das sieht besser aus, als ein Knick für ein paar wenige
	 * Pixel
	 */
	if (length2 < 3) {
		delete p_goal1;
		var length = p_goal.substract(this.position).get_vector_length();
		var ms = Math.floor(length / this.animal.walk_speed);
		var t = this.set_x(
			p_goal.x, ms, null,
			(function(_instance, _on_finish) {
				return function() {
					//Sofort zum Stehen kommen, nicht den nächsten Loop abwarten
					_instance._trigger_animation_now('stand');
					_instance._set_status(MapObjects.Animal.STATUS_STAND);
					if (_on_finish)
						_on_finish();
				}
			})(this, on_finish)
		);
		t.onMotionChanged = this.click_div.motion_change_helper.x;
		t = this.set_y( p_goal.y, ms)
		t.onMotionChanged = this.click_div.motion_change_helper.y;
	}
	else {
		var length1 = p_goal1.substract(this.position).get_vector_length();
		var ms = Math.floor(length1 / this.animal.walk_speed);
		var t = this.set_x(
			p_goal1.x, ms, null,
			(function(_instance, _p_goal, _p_goal1, _length, _on_finish) {
				return function() {
					var pose;
					if (_p_goal.x > _p_goal1.x) {
						if (_p_goal.y > _p_goal1.y)
							pose = 0;
						else
							pose = 3;
					}
					else {
						if (_p_goal.y > _p_goal1.y)
							pose = 2;
						else
							pose = 1;
					}
					
					var ms = Math.floor(_length / _instance.animal.walk_speed);
					_instance._set_pose(pose)
					//Sofort loslaufen, nicht den nächsten Loop abwarten
					_instance._trigger_animation_now('walk');
					_instance._set_status(MapObjects.Animal.STATUS_WALK);
					var t = _instance.set_x(
						_p_goal.x, ms, null,
						function () {
							// Sofort zum Stehen kommen
							_instance._trigger_animation_now('stand');
							_instance._set_status(
								MapObjects.Animal.STATUS_STAND
							);
							if (_on_finish)
								_on_finish();
						}
					);
					t.onMotionChanged = 
						_instance.click_div.motion_change_helper.x;
					t = _instance.set_y( _p_goal.y, ms );
					t.onMotionChanged = 
						_instance.click_div.motion_change_helper.y;
					/*_instance.click_div.set_x(
						_p_goal.x + _instance.click_div_offset.x, 
						ms
					);
					_instance.click_div.set_y(
						_p_goal.y + _instance.click_div_offset.y, 
						ms
					);*/
				}
			})(this, p_goal, p_goal1, length2, on_finish)
		);
		t.onMotionChanged = this.click_div.motion_change_helper.x; 
		var t = this.set_y( p_goal1.y, ms );
		t.onMotionChanged = this.click_div.motion_change_helper.y;
	}
	delete intersec;
	delete intersec0;	
};

MapObjects.Animal.prototype.set_click_div_offset = function(x, y) {
	this.click_div_offset.x = x + ((this.width - this.click_div.width) >> 1);
	this.click_div_offset.y = y + ((this.height - this.click_div.height) >> 1);
	
	this.click_div.set_position(
		this.position.x + this.click_div_offset.x,
		this.position.y + this.click_div_offset.y 
	);
	
};

MapObjects.Animal.prototype.get_click_div_offset_x = function() {
	return this.click_div_offset.x * this.scale_factor;
};

MapObjects.Animal.prototype.get_click_div_offset_y = function() {
	return this.click_div_offset.y * this.scale_factor;
}

/**
 * Bestimmt einen Interaktionspartner.
 * Dem Tier können beliebig viele Interaktionspartner zugewiesen werden. Jede
 * Zuweisung geschieht dabei immer automatisch doppelseitig, also wenn dem Tier
 * A der Interaktionspartner B zugewiesen wird, dann wird automatisch auch dem
 * Tier B der Interaktionspartner A zugewiesen.
 * 
 * So können z.B. Kämpfe miteinander dargestellt werden und nicht nur mit der
 * Luft.
 * 
 * @param {0,1,2,3} edge	An welcher Seite der Partner steht. In diese 
 * 							Richtung dreht sich	das Tier dann bei der 
 * 							Interaktion
 * @param MapObjects.Animal animal		Das Animal-Objekt des Partners
 */
MapObjects.Animal.prototype._add_interaction_partner = function(edge, animal) {
	if ((edge < 0) ||(edge > 3)) {
		throw new Error("MapObjects.Animal.prototype.add_interaction_partner: "
			+ "First parameter 'edge' has to be of in {0,1,2,3}.");
	}
	if (!PureFW.instance_of(animal, MapObjects.Animal)) {
		throw new Error("MapObjects.Animal.prototype.add_interaction_partner: "
			+ "Second parameter 'animal' has to be of type MapObjects.Animal.");
	}

	this.interaction_partners[edge].push(animal);
}

/**
 * Gibt die Interaktionspartner zurück
 * 
 * @return MapObjects.Animal[]
 */
MapObjects.Animal.prototype.get_interaction_partners = function(edge) {
	return this.interaction_partners[edge];
}

/**
 * Tier wechselt in den Kampfzustand.
 */
MapObjects.Animal.prototype.fight = function() {
	if (this.status == MapObjects.Animal.STATUS_FIGHT)
		return;
	
	this._trigger_animation_now('fight');
	this._set_status(MapObjects.Animal.STATUS_FIGHT);
}

MapObjects.Animal.prototype.trigger_animation = function(ani_name, num) {
	this.next_animation = { 'name': ani_name, 'num': num || -1 };
};

MapObjects.Animal.prototype.set_walk_region = function(x, y, w, h) {
	this.walk_region = [
		new MathExt.LinAlg2D.Vector(x, y),
		new MathExt.LinAlg2D.Vector(x+w, y+h)
	];
}

MapObjects.Animal.prototype._set_pose = function(pose) {
	if (pose == this.pose)
		return
		
	this.pose = pose;
	
	this.set_strip(this.animal.strips*this.pose + this.cur_animation.strip);
	this.set_frame_delay(this.cur_animation.frame_delay);
	this.set_frame_range(
		this.cur_animation.frame_range[0], 
		this.cur_animation.frame_range[1], 
		this.animal.strips*this.pose + this.cur_animation.strip
	);
	
/*
	this.set_image(
		this.animal.pic_base + '_0_0_0_0_0_0_0_' + this.pose + '.png',
		this.animal.frames,
		this.animal.strips
	);
	for (var ani in this.animal.animations) {
		var n = this.animal.animations[ani].length;
		for (var i = 0; i < n; i++) {
			var animation = this.animal.animations[ani][i];
			this.set_frame_range(
				animation.frame_range[0],
				animation.frame_range[1], 
				animation.strip
			);
		}
	}*/
};

MapObjects.Animal.prototype._set_status = function(status) {
	var old_status = this.status;
	if (status == -1)
		status = this.status
	else
		this.status = status;	
	switch (status) {
		case MapObjects.Animal.STATUS_STAND:
			/**
			 * Tier darf sich frei bewegen.
			 * Bei einer gewissen Chance läuft es dann einfach los.
			 * Aber nur, wenn er schon mindestens eine Stehanimation abgespielt
			 * hat.
			 */
			if ((this.walk_region) && (old_status == status) 
				&& (Math.floor(Math.random()*2) == 0)) 
			{
				this.walk_to(
					this.walk_region[0].x + Math.floor(
						Math.random()*(
							this.walk_region[1].x - this.walk_region[0].x
						)
					),
					this.walk_region[0].y + Math.floor(
						Math.random()*(
							this.walk_region[1].y - this.walk_region[0].y
						)
					)
				);
			}
			else {
				/**
				 * Tier steht schon, dann machen wir eine "Stehpause", also wir
				 * halten die Animationen kurz an.
				 */
				this.stop_animation();
				this.idle_timeout = PureFW.Timeout.set_timeout(
					(function(_instance) {
						return function (ev) {
							_instance.start_animation();
							_instance.idle_timeout = null;
						}
					})(this),
					500+Math.random()*1000
				);
				this.trigger_animation('stand');
			}
			break;
		case MapObjects.Animal.STATUS_WALK:
			this.trigger_animation('walk');
			break;
		case MapObjects.Animal.STATUS_FIGHT:
			this.stop_animation();
			this.idle_timeout = PureFW.Timeout.set_timeout(
				(function(_instance) {
					return function (ev) {
						_instance.wait_int = PureFW.Timeout.set_interval(
							function() {
								var n = _instance.interaction_partners.length;
								var edges = [];
								for (var edge = 0; edge < n; edge++) {
									var m = _instance.interaction_partners[edge]
										.length;
									for (var i = 0; i < m; i++) {
										if (_instance.interaction_partners[edge]
														[i].is_running())
										{
											return;
										}
									}
									if (m > 0) {
										edges.push(edge);
									}
								}
								PureFW.Timeout.clear_interval(
									_instance.wait_int
								);
								if (edges.length == 0)
									edges = [0,1,2,3];
								_instance._set_pose(edges.random()^1);
								_instance.start_animation();
								_instance.idle_timeout = null;								
							},
							60
						);
					}
				})(this),
				1000+Math.random()*2000
			);
			this.trigger_animation('fight');
			break;
		default:
			return;
	}
};

MapObjects.Animal.prototype._trigger_animation_now = function(ani, num, force) {
	if (typeof(ani) == 'string') {
		if ((typeof(num) == 'undefined') || (num < 0) || isNaN(num))
			var ani = this.animal.animations[ani].random();
		else
			var ani = this.animal.animations[ani][num];
	}
	
	if ((ani == this.cur_animation) && !force) {
		return;	// nothing to do
	}
	
	if (this.idle_timeout) {
		PureFW.Timeout.clear_timeout(this.idle_timeout);
		this.idle_timeout = null;
		this.start_animation();
	}
	this.cur_animation = ani;
	try {
		this.yoyo = (typeof(ani.yoyo) == 'undefined') ? false : ani.yoyo;
		this.reverse 
			= (typeof(ani.reverse) == 'undefined') ? false : ani.reverse;
		this.set_frame_range(
			ani.frame_range[0], 
			ani.frame_range[1], 
			this.animal.strips*this.pose + ani.strip
		);
		this.set_strip(this.animal.strips*this.pose + ani.strip);
		this.set_frame_delay(ani.frame_delay);
	}
	catch(e) {
		MammunUI.add_to_debug_console(e);
	}
};

MapObjects.Animal.prototype.destroy = function() {
	MapObjects.Animal.parent.destroy.call(this);
	if (this.idle_timeout) {
		PureFW.Timeout.clear_timeout(this.idle_timeout);
		this.idle_timeout = null;
	}
	this.click_div.destroy();
}

MapObjects.Animal.prototype.add_event_handler = function(type, ev) {
	if ((type == "click") || (type.indexOf("mouse") != -1))
		this.click_div.add_event_handler(type, ev);
	else
		MapObjects.Animal.parent.add_event_handler.call(this, type, ev);
};

MapObjects.Animal.prototype.remove_event_handler = function(type, ev) {
	if ((type == "click") || (type.indexOf("mouse") != -1))
		this.click_div.remove_event_handler(type, ev);
	else
		MapObjects.Animal.parent.remove_event_handler.call(this, type, ev);
};