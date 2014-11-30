/**
 * Diese Klasse dient dazu, alle setTimeout / setInterval aufrufe zu optimieren,
 * indem nur ein einziger Timeout permanent angestoßen wird und dann die
 * registrierten Funktionen anstößt. Das bringt einen gigantischen Performance-
 * Schub, gerade bei vielen Intervallen in einem Projekt.
 */

PureFW.Timeout = new Object();
/**
 * Globaler Tackt. Funktionen werden im Takt ausgeführt. Je kleiner der 
 * Takt-Wert, desto genauer können Delays eingestellt werden und desto glatter
 * laufen Animationen, aber auch desto mehr Performance kostet die Taktung.
 * 
 * @var uint
 */
PureFW.Timeout.global_clock = 60;
PureFW.Timeout.clock_num = 0;
PureFW.Timeout.is_running = false;

/**
 * Funktionen, die einmal ausgeführt werden sollen.
 * Abgelegt werden sie wie folgt:
 * <code>
 * PureFW.Timeout.registered_functions_once[C] = [F1, F2, F3, ... ];
 * </code>
 * mit C clock_num, zu der die Funktionen Fn ausgeführt werden sollen.
 */
PureFW.Timeout.registered_functions_once = new Object();

/**
 * Funktionen, die regelmäßig ausgeführt werden sollen.
 * Abgelegt werden sie wie folgt:
 * <code>
 * PureFW.Timeout.registered_functions_interval[C] = [F1, F2, F3, ... ];
 * </code>
 * mit C alle wie viele Takte die Funktionen Fn ausgeführt werden sollen.
 */
PureFW.Timeout.registered_functions_interval = new Object();

/**
 * Führt alle Funktionen aus, die dem aktuellen Takt zugeordnet sind und
 * erhöht die Taktzahl um eins.
 * 
 * @access private
 */
PureFW.Timeout._tick = function() {
	var t = PureFW.Timeout.clock_num;
	PureFW.Timeout.clock_num++;	// jetzt schon erhöhen, man weiß nie, was kommt
	window.setTimeout(PureFW.Timeout._tick, PureFW.Timeout.global_clock);
	
	if (PureFW.Timeout.registered_functions_once[t]) {
		var n = PureFW.Timeout.registered_functions_once[t].length;
		try {
			for (var i = 0; i < n; i++) {
				if (PureFW.Timeout.registered_functions_once[t][i])
					PureFW.Timeout.registered_functions_once[t][i]();
				PureFW.Timeout.registered_functions_once[t][i] = null;
			}	
		}
		catch (e) {
			throw PureFW.Timeout.registered_functions_once[t][i] + e;
		}
		finally {
			PureFW.Timeout.registered_functions_once[t] = null;
		}
	}
	for (var T in PureFW.Timeout.registered_functions_interval) {
		if ((t % T) == 0) {
			var n = PureFW.Timeout.registered_functions_interval[T].length;
			for (var i = 0; i < n; i++) {
				if (PureFW.Timeout.registered_functions_interval[T][i])
					PureFW.Timeout.registered_functions_interval[T][i]();
			}
		}
	}
}

/**
 * Startet den PureFW-Timeout-Takt.
 * 
 * Diese Funktion muss explizit aufgerufen werden, weil nicht jedes Projekt
 * dieses Feature benötigt.
 */
PureFW.Timeout.start = function() {
	if (!PureFW.Timeout.is_running)
		window.setTimeout(PureFW.Timeout._tick, PureFW.Timeout.global_clock);
	PureFW.Timeout.is_running = true;
}

/**
 * Setzt die Zeit in ms, die zwischen zwei Ticks liegen soll.
 * 
 * @param uint d
 */
PureFW.Timeout.set_global_clock = function(d) {
	d = parseInt(d);
	if (delay <= 0)
		throw new Error("Timeout::set_gobal_clock: First argument 'd' " +
			"has to be greater than 0");
	
	PureFW.Timeout.global_clock = d;
};

/**
 * Gibt die Zeit in ms zurück, die zwischen zwei Ticks liegt.
 * 
 * @return uint
 */
PureFW.Timeout.get_global_clock = function() {
	return PureFW.Timeout.global_clock;
};

/**
 * Führt nach ungefähr <code>delay</code> ms (abhängig vom globalen Takt) die 
 * angebene Funktion aus.
 * 
 * @param Function fn
 * @param uint delay
 */
PureFW.Timeout.set_timeout = function(fn, delay) {
	if (!PureFW.Timeout.is_running)
		PureFW.Timeout.start();
	if (typeof (fn) !== 'function') {
		throw new Error("Timeout::set_timeout: First argument 'fn' " +
				"has to be of type Function");
	}
	if (isNaN(delay))
		throw new Error("Timeout::set_timeout: Second argument 'delay' " +
				"has to be of type Number");
	
	if (delay <= 0)
		throw new Error("Timeout::set_timeout: Second argument 'delay' " +
			"has to be greater than 0");
	var t = PureFW.Timeout.clock_num 
		+ Math.round(delay / PureFW.Timeout.global_clock);
	if (t == PureFW.Timeout.clock_num) {
		fn();
	}
	else {
		if ((typeof(PureFW.Timeout.registered_functions_once[t]) == 'undefined')
			|| !PureFW.Timeout.registered_functions_once[t])
		{
			PureFW.Timeout.registered_functions_once[t] = new Array();
		}
		PureFW.Timeout.registered_functions_once[t].push(fn);
		
		return [ t, PureFW.Timeout.registered_functions_once[t].length-1 ];
	}
};

PureFW.Timeout.clear_timeout = function(o) {
	if ((typeof(o) == 'undefined') || !o || (typeof(o) != 'object') 
		|| (o.length != 2)) 
	{
		throw new Error(
			"PureFW.Timeout.clear_timeout: First argument 'o' has to be "
				+ "an Array of lenght 2"
		);
	}
	
	if (!PureFW.Timeout.registered_functions_once[o[0]] 
		|| !PureFW.Timeout.registered_functions_once[o[0]][o[1]])
	{
	/*	throw new Error(
			"PureFW.Timeout.registered_functions_once[" + o[0] 
				+ "][" + o[1] + "] does not exist"
		);*/
		return;
	}
	
	PureFW.Timeout.registered_functions_once[o[0]][o[1]] = null;
}

/**
 * Führt ungefähr alle <code>delay</code> ms (abhängig vom globalen Takt) die
 * angegebene Funktion aus.
 * 
 * @param Function fn
 * @param uint delay
 */
PureFW.Timeout.set_interval = function(fn, delay) {
	if (!PureFW.Timeout.is_running)
		PureFW.Timeout.start();
	
	if (typeof (fn) !== 'function')
		throw new Error("Timeout::set_interval: First argument 'fn' " +
				"has to be of type Function");
	if (isNaN(delay))
		throw new Error("Timeout::set_interval: Second argument 'delay' " +
				"has to be of type Number");
	
	if (delay <= 0)
		throw new Error("Timeout::set_interval: Second argument 'delay' " +
			"has to be greater than 0");
	var t = Math.round(delay / PureFW.Timeout.global_clock);
	if (t < 1)
		t = 1;

	if ((typeof(PureFW.Timeout.registered_functions_interval[t]) == 'undefined')
		|| !PureFW.Timeout.registered_functions_interval[t])
	{
		PureFW.Timeout.registered_functions_interval[t] = new Array();
	}
	
	PureFW.Timeout.registered_functions_interval[t].push(fn);
	
	return [ t, PureFW.Timeout.registered_functions_interval[t].length-1 ];
};

/**
 * Entfernt eine zuvorgesetzte Funktion im entsprechenden Interval, die der
 * ID zugewiesen wurde.
 * 
 * @param uint id
 */
PureFW.Timeout.clear_interval = function(o) {
	if ((typeof(o) == 'undefined') || !o || (typeof(o) != 'object') 
		|| (o.length != 2)) 
	{
		throw new Error(
			"PureFW.Timeout.clear_interval: First argument 'o' has to be "
				+ "an Array of lenght 2"
		);
	}
	
	if (!PureFW.Timeout.registered_functions_interval[o[0]] 
		|| !PureFW.Timeout.registered_functions_interval[o[0]][o[1]])
	{
	/*	throw new Error(
			"PureFW.Timeout.registered_functions_interval[" + o[0] 
				+ "][" + o[1] + "] does not exist"
		);*/
		return;
	}
	
	PureFW.Timeout.registered_functions_interval[o[0]][o[1]] = null;
}