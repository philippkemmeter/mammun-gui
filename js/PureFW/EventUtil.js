/**
 * Klasse, die Wrapperfunktionen bietet, um zwischen IE und DOM-Standard zu
 * jonglieren.
 * Die Datei ist angelehnt an das Code-Beispiel  von Kapitel 9 des Buches
 * "Professional JavaScript for Web Developers" von "Nicholas C. Zakas".
 * Geiles Buch, unbedingt kaufen!
 * 
 * @author Phil
 */
 
// rein statisch
PureFW.EventUtil = new Object();
 
/**
 * DOM-Kompatible Funktion, außer, dass das geile Feature, Events zur 
 * Capture-Phase zu registrieren nicht funktioniert, da im IE nicht möglich...
 * Da wir uns nach dem kleinsten gemeinsamen Nenner richten müssen, müssen
 * wir uns - wie sooft - nach dem IE richten und uns einschränken.
 */
PureFW.EventUtil.add_event_handler = function(target, type, fn) {
	if (target.addEventListener)
		target.addEventListener(type, fn, false);
	else if (target.attachEvent)
		target.attachEvent("on"+type, fn);
};

/**
 * Entfernt einen EventHandler nach DOM-Standard angelehnt und nach IE gebeugt.
 */
PureFW.EventUtil.remove_event_handler = function(target, type, fn) {
	if (target.removeEventListener)
		target.removeEventListener(type, fn, false);
	else if (target.detachEvent)
		target.detachEvent("on"+type, fn);
};

/**
 * Diese Funktion formatiert ein IE-Event zu einem DOM-Standard-Event um.
 */
PureFW.EventUtil.formatEvent = function (ev) {
	if (!ev)
		ev = window.event;
    if (!ev.stopPropagation) {
        ev.charCode = (ev.type == "keypress") ? ev.keyCode : 0;
        ev.eventPhase = 2;
        ev.isChar = (ev.charCode > 0);
        ev.pageX = ev.clientX + document.body.scrollLeft;
        ev.pageY = ev.clientY + document.body.scrollTop;
        ev.preventDefault = function () {
            this.returnValue = false;
        };

        if (ev.type == "mouseout") {
            ev.relatedTarget = ev.toElement;
        } else if (ev.type == "mouseover") {
            ev.relatedTarget = ev.fromElement;
        }

        ev.stopPropagation = function () {
            this.cancelBubble = true;
        };

        ev.target = ev.srcElement;
        ev.time = (new Date).getTime();
    }
    return ev;
};

/**
 * Diese Funktion erzeugt ein Ereignis.
 * 
 * @param String type
 * @param mixed detail [optional]
 * @return Event
 */
PureFW.EventUtil.create_event = function(type, detail) {
	var ev = new Object();
	ev.bubbles = false;
	ev.cancelable = false;
	ev.target = null;
	ev.timeStamp = (new Date()).getTime();
	ev.type = type;
	ev.detail = detail || 0;
	ev.preventDefault = ev.stopPropagation = function(){};
	return ev;
}