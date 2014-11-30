/**
 * Ein Feuerwerk bestehend aus mehreren verschiedenen Feuerblumen am Himmel.
 */

if (typeof(Fireworks) == 'undefined')
	Fireworks = new Object();

/**
 * 
 * @param HTMLElement parent_node
 * @param int x		x-Position des Zentrums des Feuerwerks
 * @param int y		y-Position des Zentrums des Feuerwerks
 * @param uint w	Breite einer Blume. Das Feuerwerk wird breiter (s. range)
 * @param uint h	Höhe einer Blume. Das Feuerwerk wird höher (s. range)
 * @param uint range	Maximale Abweichung des Feuerwerks in Höhe und Breite
 * @param uint max_flowers	Wie viele Blumen das Feuerwerk hat. Widget zerstört
 * 							sich dann selbst. Bei 0 ist es endlos aktiv.
 * @param bool no_scale
 */
Fireworks.Fireworks = function(parent_node, x, y, w, h, range, max_flowers,
	no_scale) 
{
	if (typeof(parent_node) !== 'undefined') {
	 	this.init(parent_node, x, y, w, h, range, max_flowers, no_scale);
	 	this.instance_num = this.constructor.num_instances;
		this.instance_name = "Fireworks.Fireworks.instances["
			+this.instance_num+"]";

		this.id = 'Fireworks.Fireworks'+this.instance_num;
		this.bg_img_id = 'Fireworks.FireworksBg'+this.instance_num;
		this.content_id = 'Fireworks.FireworksCont'+this.instance_num;
		
		this.constructor.instances[this.instance_num] = this;
		this.constructor.num_instances++;
		this.insert_into_dom_tree();
		
		PureFW.WidgetManager.manager_all.register_widget(this);
	}
};
/********************************************************************
 * Fireworks.Fireworks EXTENDS Widget
 ********************************************************************/
Fireworks.Fireworks.extend(PureFW.Widget);

/********************************************************************
 * Statische Deklarationen
 ********************************************************************/

Fireworks.Fireworks.flowers = [
	{
		'pic_url':	'ui/animations/fireworks/fw1.png',
		'frames':	14
	},
	{
		'pic_url':	'ui/animations/fireworks/fw2.png',
		'frames':	14
	},
	{
		'pic_url':	'ui/animations/fireworks/fw3.png',
		'frames':	12
	},
	{
		'pic_url':	'ui/animations/fireworks/fw4.png',
		'frames':	12
	}
];

Fireworks.Fireworks.num_instances = 0;			// Instanzüberwachung (Anzahl)
Fireworks.Fireworks.instances = new Array();

/********************************************************************
 * Prototyp-Deklarationen
 ********************************************************************/
Fireworks.Fireworks.prototype.init = function(parent_node, x, y, w, h, 
	range, max_flowers, no_scale)
{
	Fireworks.Fireworks.parent.init.call(this, parent_node, x, y, w, h, 
		no_scale);
	this.max_flowers = (max_flowers -1)|| Infinity;
	this.range = range;
	
	this.film_animators = [];
};

Fireworks.Fireworks.prototype.insert_into_dom_tree = function() {
	Fireworks.Fireworks.parent.insert_into_dom_tree.call(this);
	
	this.flower_counter = 0;
	this.timeout_fn = (function(_instance) {
		return function(ev) {
			_instance._shoot_random_flower();
			_instance.flower_counter++;
			if (_instance.flower_counter < _instance.max_flowers) {
				PureFW.Timeout.set_timeout(
					_instance.timeout_fn, Math.floor(Math.random()*2000)
				);
			}
			else if(_instance.flower_counter == _instance.max_flowers) {
				PureFW.Timeout.set_timeout(
					_instance.timeout_fn, Math.floor(2500)
				);
			}
			else {
				_instance.destroy();
			}
		}
	})(this)

	this.timeout_fn();
}

/**
 * Schießt jetzt eine zufällige Blume in den Himmel. Wird vom Widget gesteuert,
 * gezieltes Aufrufen macht wenig Sinn...
 * @access private
 */
Fireworks.Fireworks.prototype._shoot_random_flower = function() {
	var fw_film = new PureFW.FilmAnimator(
		this.parent_node,
		this.position.x - (this.width >> 1) - this.range
			+ (Math.floor(Math.random()*this.range) << 1),
		this.position.y - (this.height >> 1) - this.range
			+ (Math.floor(Math.random()*this.range) << 1),
		this.width,
		this.height
	);
	
	var fw_obj = Fireworks.Fireworks.flowers.random();
	
	fw_film.set_image(fw_obj.pic_url, fw_obj.frames);
	fw_film.set_z_index(this.get_z_index());
	fw_film.start_animation();
	
	this.film_animators.push(fw_film);
	
	fw_film.add_event_handler("loop", (function(_instance) {
		return function(ev) {
			_instance.film_animators.remove(this);
			this.destroy();
		}
	})(this))
}

Fireworks.Fireworks.prototype.set_z_index = function(z) {
	Fireworks.Fireworks.parent.set_z_index.call(this, z);
	var n = this.film_animators.length;
	for (var i = 0; i < n; i++) {
		this.film_animators[i].set_z_index(z);
	}
}
/*
Fireworks.Fireworks.prototype.destroy = function() {
	this.film_animators.destroy();
	
	Fireworks.Fireworks.parent.destroy.call(this);
};*/