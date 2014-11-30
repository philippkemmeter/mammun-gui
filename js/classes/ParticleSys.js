/**
 * Ein Partikel im Partikelsystem
 * 
 * @param Widget w
 * @param Function dy
 * @param Function dy
 * @param Function ttl
 */
Particle = function(w, x, y, dx, dy, ttl) {
	this.widget = w;
	this.x = x ? x() : 0.0;
	this.y = x ? y() : 0.0;
	this.dx = dx ? dx() : Particle._dxy_simple();
	this.dy = dy ? dy() : Particle._dxy_simple();
	this.ttl = ttl ? ttl() : Particle._ttl_simple();
	this.fading_out = false;
}

Particle._dxy_simple = function() {
	return Math.random() + 0.5;
}

Particle._ttl_simple = function() {
	return Math.floor(Math.random()*50) + 200;
}

/**
 * Partikelsystem
 * 
 * @param Function x_func
 * @param Function y_func
 * @param Function dx_func
 * @param Function dy_func
 * @param Function ttl_func
 */
ParticleSys = function(x_func, y_func, dx_func, dy_func, ttl_func) {
	this.container = null;
	this.particles = new Array();
	this.instance_num = this.constructor.num_instances;
	this.instance_name = "ParticleSys.instances["+this.instance_num+"]";
	
	this.constructor.instances[this.instance_num] = this;
	this.constructor.num_instances++;
	
	this.dx_func = dx_func || Particle._dxy_simple;
	this.dy_func = dy_func || Particle._dxy_simple;
	this.ttl_func = ttl_func ||Particle._ttl_simple;
	this.x_func = x_func || function() {
		return 0;
	}
	this.y_func = y_func || function() {
		return 0;
	}
}

/********************************************************************
 * Statische Deklarationen
 ********************************************************************/
ParticleSys.num_instances = 0;
ParticleSys.instances = new Array();

ParticleSys.update = function() {
	var n = ParticleSys.instances.length;
	for (var i = 0; i < n; i++) {
		ParticleSys.instances[i].update();
	}
}
ParticleSys.start = function() {
	PureFW.Timeout.set_interval(ParticleSys.update, 100);
}

/*********************************************************************
 * Prototyp-Deklarationen
 *********************************************************************/

/**
 * Fügt ein Partikel dem System hinzu
 * 
 * @param Widget w
 * @param float dy
 * @param float dy
 * @param uint ttl
 */
ParticleSys.prototype.add_particle = function(w) {
	var p = new Particle(w, this.x_func, this.y_func,
		this.dx_func, this.dy_func, this.ttl_func);
	this.particles.push(p);
	return p;
}

ParticleSys.prototype.update = function() {
	var m = this.particles.length;
	for (var j = 0; j < m; j++) {
		var particle = this.particles[j];
		particle.ttl = particle.ttl - 1;
		if ((particle.ttl > 0) || (particle.fading_out)){
			particle.x = particle.x + particle.dx;
			particle.y = particle.y + particle.dy;
			particle.widget.set_position(
				Math.floor(particle.x),
				Math.floor(particle.y)
			);
		} else {
			var w = particle.widget;
			w.fade_out(3000);
			w.add_event_handler("fade_out_end", (function(_instance,_i) {
				return function() {
					var w = _instance.particles[_i].widget;
					delete _instance.particles[_i];
					_instance.particles[_i] = null;
					_instance.particles[_i] = new Particle(
						w,
						_instance.x_func, _instance.y_func,
						_instance.dx_func, _instance.dy_func, 
						_instance.ttl_func
					);
					w.fade_in(3000);
				}
			})(this,j));
			this.particles[j].fading_out = true; 
		}
	}	
}