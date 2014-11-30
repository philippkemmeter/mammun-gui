if (typeof(MapObjects) == 'undefined')
	MapObjects = new Object();

MapObjects.Mammun = function(parent_node, x, y, w, h, mammun, clothes, color, 
	no_scale) 
{
	if (typeof(parent_node) !== 'undefined') {
	 	this.init(parent_node, x, y, w, h, mammun, clothes, color, no_scale);
	 	this.instance_num = this.constructor.num_instances;
		this.instance_name = "MapObjects.Mammun.instances["
			+this.instance_num+"]";

		this.id = 'MapObjects.Mammun'+this.instance_num;
		this.bg_img_id = 'MapObjects.MammunBg'+this.instance_num;
		this.content_id = 'MapObjects.MammunCont'+this.instance_num;
		
		this.constructor.instances[this.instance_num] = this;
		this.constructor.num_instances++;
		this.insert_into_dom_tree();
		
		PureFW.WidgetManager.manager_all.register_widget(this);
	}
};
/********************************************************************
 * MapObjects.Mammun EXTENDS Animal
 ********************************************************************/
MapObjects.Mammun.extend(MapObjects.Animal);

/********************************************************************
 * Statische Deklarationen
 ********************************************************************/

MapObjects.Mammun.mammuns = {
	'club_m': {
		'pic_base':	'map/units/club/club_0',
		'poses': 4,
		'width': 80,
		'height': 80,
		'strips': 7,
		'frames': 26,
		'walk_speed': 0.01,
		'frame_ranges': [
			[0, 9],
			[0, 5],
			[0, 18],
			[0, 5],
			[0, 19],
			[0, 25],
			[0, 15]
		],
		'animations': {
			'stand': [
				{
					'strip': 3,
					'frame_range': [0, 5],
					'frame_delay': 360,
					'yoyo': true
				}
			],
			'walk': [
				{
					'strip': 0,
					'frame_range': [0, 9],
					'frame_delay': 180
				}
			],
			'run': [
				{
					'strip': 1,
					'frame_range': [0, 5],
					'frame_delay': 120
				}
			],
			'fight': [
				{
					'strip': 2,
					'frame_range': [0, 18],
					'frame_delay': 120
				}
			],
			'harvest': [
				{
					'strip': 4,
					'frame_range': [0, 19],
					'frame_delay': 120
				}
			],
			'dance': [
				{
					'strip': 5,
					'frame_range': [0, 25],
					'frame_delay': 120
				}
			],
			'sit_down': [
				{
					'strip': 6,
					'frame_range': [1, 11],
					'frame_delay': 120
				}
			],
			'sit': [
				{
					'strip': 6,
					'frame_range': [12, 15],
					'frame_delay': 480,
					'yoyo': true
				}
			],
			'stand_up': [
				{
					'strip': 6,
					'frame_range': [2, 11],
					'frame_delay': 120,
					'reverse': true
				}
			]
		}
	},
	'javelin_m': {
		'pic_base':	'map/units/javelin/javelin_0',
		'poses': 4,
		'width': 80,
		'height': 80,
		'strips': 7,
		'frames': 26,
		'walk_speed': 0.01,
		'frame_ranges': [
			[0, 9],
			[0, 5],
			[0, 13],
			[0, 5],
			[0, 19],
			[0, 25],
			[0, 15]
		],
		'animations': {
			'stand': [
				{
					'strip': 3,
					'frame_range': [0, 5],
					'frame_delay': 360,
					'yoyo': true
				}
			],
			'walk': [
				{
					'strip': 0,
					'frame_range': [0, 9],
					'frame_delay': 180
				}
			],
			'run': [
				{
					'strip': 1,
					'frame_range': [0, 5],
					'frame_delay': 120
				}
			],
			'fight': [
				{
					'strip': 2,
					'frame_range': [0, 13],
					'frame_delay': 120
				}
			],
			'harvest': [
				{
					'strip': 4,
					'frame_range': [0, 19],
					'frame_delay': 120
				}
			],
			'dance': [
				{
					'strip': 5,
					'frame_range': [0, 25],
					'frame_delay': 120
				}
			],
			'sit_down': [
				{
					'strip': 6,
					'frame_range': [1, 11],
					'frame_delay': 120
				}
			],
			'sit': [
				{
					'strip': 6,
					'frame_range': [12, 15],
					'frame_delay': 480,
					'yoyo': true
				}
			],
			'stand_up': [
				{
					'strip': 6,
					'frame_range': [2, 11],
					'frame_delay': 120,
					'reverse': true
				}
			]
		}
	},
	'bow_m': {
		'pic_base':	'map/units/bow/bow_0',
		'poses': 4,
		'width': 80,
		'height': 80,
		'strips': 7,
		'frames': 26,
		'walk_speed': 0.01,
		'frame_ranges': [
			[0, 9],
			[0, 5],
			[0, 19],
			[0, 5],
			[0, 19],
			[0, 25],
			[0, 15]
		],
		'animations': {
			'stand': [
				{
					'strip': 3,
					'frame_range': [0, 5],
					'frame_delay': 360,
					'yoyo': true
				}
			],
			'walk': [
				{
					'strip': 0,
					'frame_range': [0, 9],
					'frame_delay': 180
				}
			],
			'run': [
				{
					'strip': 1,
					'frame_range': [0, 5],
					'frame_delay': 120
				}
			],
			'fight': [
				{
					'strip': 2,
					'frame_range': [0, 19],
					'frame_delay': 120
				}
			],
			'harvest': [
				{
					'strip': 4,
					'frame_range': [0, 19],
					'frame_delay': 120
				}
			],
			'dance': [
				{
					'strip': 5,
					'frame_range': [0, 25],
					'frame_delay': 120
				}
			],
			'sit_down': [
				{
					'strip': 6,
					'frame_range': [1, 11],
					'frame_delay': 120
				}
			],
			'sit': [
				{
					'strip': 6,
					'frame_range': [12, 15],
					'frame_delay': 480,
					'yoyo': true
				}
			],
			'stand_up': [
				{
					'strip': 6,
					'frame_range': [2, 11],
					'frame_delay': 120,
					'reverse': true
				}
			]
		}
	},

	'blade_m': {
		'pic_base':	'map/units/blade/blade_0',
		'poses': 4,
		'width': 80,
		'height': 80,
		'strips': 7,
		'frames': 26,
		'walk_speed': 0.01,
		'frame_ranges': [
			[0, 9],
			[0, 5],
			[0, 17],
			[0, 5],
			[0, 19],
			[0, 25],
			[0, 15]
		],
		'animations': {
			'stand': [
				{
					'strip': 3,
					'frame_range': [0, 5],
					'frame_delay': 360,
					'yoyo': true
				}
			],
			'walk': [
				{
					'strip': 0,
					'frame_range': [0, 9],
					'frame_delay': 180
				}
			],
			'run': [
				{
					'strip': 1,
					'frame_range': [0, 5],
					'frame_delay': 120
				}
			],
			'fight': [
				{
					'strip': 2,
					'frame_range': [0, 17],
					'frame_delay': 120
				}
			],
			'harvest': [
				{
					'strip': 4,
					'frame_range': [0, 19],
					'frame_delay': 120
				}
			],
			'dance': [
				{
					'strip': 5,
					'frame_range': [0, 25],
					'frame_delay': 120
				}
			],
			'sit_down': [
				{
					'strip': 6,
					'frame_range': [1, 11],
					'frame_delay': 120
				}
			],
			'sit': [
				{
					'strip': 6,
					'frame_range': [12, 15],
					'frame_delay': 480,
					'yoyo': true
				}
			],
			'stand_up': [
				{
					'strip': 6,
					'frame_range': [2, 11],
					'frame_delay': 120,
					'reverse': true
				}
			]
		}
	}
}
		
MapObjects.Mammun.num_instances = 0;			// Instanzüberwachung (Anzahl)
MapObjects.Mammun.instances = new Array();

MapObjects.Mammun.get_mammun_by_type_and_sex = function (type, sex) {
	var w_str = '';
	switch (parseInt(type)) {
		case 3009:
			w_str = 'blade';
			break;
		case 3011:
		case 3014:
		case 3015:
		case 3016:
			w_str = 'club';
			break;
		case 3001:
		case 3008:
		case 3022:
			w_str = 'javelin';
			break;
		case 3007:
		case 3002:
		case 3021:
			w_str = 'bow';
			break;
	}
	w_str += (!sex) ? '_m' : '_f';
	
	return (MapObjects.Mammun.mammuns[w_str]) 
		? MapObjects.Mammun.mammuns[w_str]
		: MapObjects.Mammun.mammuns.club_m;
}

MapObjects.Mammun.STATUS_STAND = MapObjects.Animal.STATUS_STAND;
MapObjects.Mammun.STATUS_WALK = MapObjects.Animal.STATUS_WALK;
MapObjects.Mammun.STATUS_FIGHT = MapObjects.Animal.STATUS_FIGHT;
MapObjects.Mammun.STATUS_SIT = 3;

/********************************************************************
 * Prototyp-Deklarationen
 ********************************************************************/
MapObjects.Mammun.prototype.init = function(parent_node, x, y, w, h, mammun,
	clothes, color, no_scale)
{
	MapObjects.Mammun.parent.init.call(this, parent_node, x, y, w, h, mammun,
		clothes, no_scale);
	this.pose = 0;
	this.color = color || 'neutral';
	
	this.color_circle_pic_url = '../pix/map/units/markers/normal/'
		+ this.color + '.png';
	this.color_circle_pic_url_hover = '../pix/map/units/markers/hover/'
		+ this.color + '.png';
	
	this.hp = 1;
	this.max_hp = 1000;
	
	this.hp_bar = null;
	
	this.hp_bar_width = 50;
};

MapObjects.Mammun.prototype.insert_into_dom_tree = function() {
	MapObjects.Mammun.parent.insert_into_dom_tree.call(this);
	
	this.set_bg_img(
		this.color_circle_pic_url
	);
	
	/*this.hp_bar = new HealthBar(
		this, 
		(this.width - this.hp_bar_width) >> 1, 
		this.height - 10, 
		this.hp_bar_width, 
		3
	);
	
	this.hp_bar.set_value(this.hp);
	this.hp_bar.set_max_value(this.max_hp);
	this.hp_bar.set_z_index(this.get_z_index());*/
	
	this.inner_frame.set_z_index(2);
	
	this.hp_bar = this.add_widget(
		PureFW.Image,
		0, 0,
		this.width, this.height,
		'map/units/markers/health/0.png'
	);
	this.hp_bar.set_z_index(1);
	
	this.add_event_handler("mouseover", (function(_instance) {
		return function(ev) {
			_instance.set_bg_img(_instance.color_circle_pic_url_hover);
		}
	})(this));
	this.add_event_handler("mouseout", (function(_instance) {
		return function(ev) {
			_instance.set_bg_img(_instance.color_circle_pic_url);
		}
	})(this));
};

/**
 * Lässt den Mammun sich hinsetzen.
 */
MapObjects.Mammun.prototype.sit_down = function() {
	if (this.status == MapObjects.Mammun.STATUS_SIT)
		return;
	
	this._trigger_animation_now('sit_down');
	this._set_status(MapObjects.Mammun.STATUS_SIT);
};

/**
 * Lässt den Mammun aufstehen
 */
MapObjects.Mammun.prototype.stand_up = function() {
	if (this.status != MapObjects.Mammun.STATUS_SIT)
		return;
	
	this._trigger_animation_now('stand_up');
	this._set_status(MapObjects.Mammun.STATUS_STAND);
};

/**
 * Setzt die Lebenspunkte, die die Mammun aktuell hat.
 * 
 * @param uint h
 */
MapObjects.Mammun.prototype.set_hp = function(h) {
	if (h > this.max_hp)
		this.hp = this.max_hp;
	else if (h < 1)
		this.hp = 1;
	else
		this.hp = h;

	var p = Math.ceil(this.hp * 100 / this.max_hp);
	// Für 0, 1 und 2 gibt es Bilder
	if (p > 2) {
		// Für 4, 6, 8 und 10 gibt es Bilder
		if (p < 11) {
			p = p & ~1;
		}
		else {
			// Für 20, 30, 40, 50, 60, 70, 80, 90, 100 gibt es Bilder
			p = Math.ceil(p / 10) * 10;
		}
	}
	this.hp_bar.set_pic_url('map/units/markers/health/'+p+'.png');
};

MapObjects.Mammun.prototype._set_status = function(status) {
	var old_status = this.status;
	if (status == -1)
		status = this.status
	else
		this.status = status;
	switch (status) {
		case MapObjects.Mammun.STATUS_STAND:
			/**
			 * Mammun darf sich frei bewegen.
			 * Bei einer gewissen Chance läuft es dann einfach los.
			 * Aber nur, wenn er schon mindestens eine Stehanimation abgespielt
			 * hat.
			 */
			if ((this.walk_region) && (old_status == status) 
				&& (Math.floor(Math.random()*5) == 0)) 
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
			/**
			 * Mammun setzt sich ab und zu hin.
			 */
			else if ((old_status == status)
				&&(Math.floor(Math.random()*10) == 0)) 
			{
				this.sit_down();
			}
			else {
				/**
				 * Mammun steht schon, dann machen wir eine "Stehpause", also 
				 * wir halten die Animationen kurz an.
				 *
				this.stop_animation();
				this.idle_timeout = PureFW.Timeout.set_timeout(
					(function(_instance) {
						return function (ev) {
							_instance.start_animation();
							_instance.idle_timeout = null;
						}
					})(this),
					500+Math.random()*1000
				);*/
				this.trigger_animation('stand');
			}
			break;
		case MapObjects.Mammun.STATUS_SIT:
			if ((old_status == status) 
				&&(Math.floor(Math.random()*10) == 0)) 
			{
				this.stand_up();
			}
			else {
				/**
				 * Mammun sitzt schon, dann machen wir eine "Sitzpause", also 
				 * wir halten die Animationen kurz an.
				 *
				this.stop_animation();
				this.idle_timeout = PureFW.Timeout.set_timeout(
					(function(_instance) {
						return function (ev) {
							_instance.start_animation();
							_instance.idle_timeout = null;
						}
					})(this),
					500+Math.random()*1000
				);*/
				this.trigger_animation('sit');
			}
			break;
		default:
			return MapObjects.Mammun.parent._set_status.call(this, status);
	}
};
