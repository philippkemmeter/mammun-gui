/**
 * Diese Klasse verwaltet die onLoad-Events des Bodys. Da man normalerweise
 * <body onload=".."> schreiben muss, was man aber nicht kann, wenn man später
 * im Skript generiert wird, und das onload im Body schon gesetzt sein muss,
 * man aber auch nicht hässlich hinter dem <body>-Tag etwas an Code einfügen
 * will, kann man sich HIER registrieren. Einmal 
 * <body onload="DocumentLoadListener.call_listeners();"> aufrufen und später mit
 * DocumentLoadListener.add() Sachen hinzufügen, die ausgeführt werden sollen.
 * Have Fun! 
 *
 * @author Phil
 */

// Rein statisch
DocumentLoadListener = new Object();

DocumentLoadListener.listeners = new Array();

DocumentLoadListener.add = function(fn) {
	DocumentLoadListener.listeners.push(fn);
};

DocumentLoadListener.call_listeners = function(ev) {
	var n = DocumentLoadListener.listeners.length;
	for (var i = 0; i < n; i++)
		DocumentLoadListener.listeners[i](ev);
};

if (typeof(PureFW) !== 'undefined')
	DocumentLoadListener.add(PureFW.body_loaded);

// Rein statisch
DocumentUnloadListener = new Object();

DocumentUnloadListener.listeners = new Array();

DocumentUnloadListener.add = function(fn) {
	DocumentUnloadListener.listeners.push(fn);
};

DocumentUnloadListener.call_listeners = function(ev) {
	var n = DocumentUnloadListener.listeners.length;
	for (var i = 0; i < n; i++)
		DocumentUnloadListener.listeners[i](ev);
};