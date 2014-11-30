function destroy_object(o) {
	for (var x in o) {
		/** 
		 * Nicht das aktuelle Fenster zerstören. Das zerstört sich entweder
		 * selbst, oder es möchte nicht zerstört werden - ganz einfach :)
		 * Wir haben hier nicht die Berechtigung dort einzugreifen!
		 */
		if (x == 'this_window')
			continue;
		try {
			o[x].destroy();
		}
		catch (e) {
		}
		delete o[x];
		o[x] = null;
	}
	delete o;
}

function mousemove(ev) {
}
function mouseup(ev) {
}
PureFW.EventUtil.add_event_handler(document, "mousemove", mousemove);
PureFW.EventUtil.add_event_handler(document, "mouseup", mouseup);

/** Wheel Scroll (Vertical) **/
var scrolling = new Object();
scrolling.cur_obj = null;
scrolling.max_vertical_scroll = 0;
scrolling.max_horizontal_scroll = 0;
scrolling.while_scrolling = null;
scrolling.on_scroll = null;



function scroll(event) 
{
	if(scrolling.cur_obj)
	{
		var delta = 0;
		try 
		{
			event = PureFW.EventUtil.formatEvent(event);
		}
		catch (e){}
		
        if (event.wheelDelta)  /* IE/Opera. */
        {
            delta = event.wheelDelta/120;
            /*if (window.opera) //DEPRECATED
                    delta = -delta;*/
        } 
        else if (event.detail) 
        {
            delta = -event.detail/3;
        }
        event.preventDefault();
        
        if(scrolling.while_scrolling)
        {
        	scrolling.while_scrolling(delta);
        }
	}
}

/** IE/Opera. */
scrolling.on_scroll = window.onmousewheel = document.onmousewheel = scroll;
if (window.addEventListener)
	window.addEventListener('DOMMouseScroll', scroll, false);
	

/**
 * Gibt die Breite des Bodys zurück (des gesamten, nicht nur des angezeigten
 * Bereichs. Das ist wichtig, wenn der Body größer ist, als das Fenster, wie
 * es bei der Karte der Fall ist)
 * @return int[2] (Breite,Höhe)
 */
function getBodySize() {
	var w,h;
	try { var test1 = document.body.scrollHeight; } catch(e) {}
	try { var test2 = document.body.offsetHeight; } catch(e) {}
	if (test1 > test2) // all but Explorer Mac
	{
		try { w = document.body.scrollWidth; } 
		catch(e) {
			try { w = document.body.offsetWidth; } catch(e) {}
		}
		try { h = document.body.scrollHeight; } 
		catch(e) {
			try { h = document.body.offsetHeight; } catch(e) {}
		}
	}
	else // Explorer Mac;
		 //would also work in Explorer 6 Strict, Mozilla and Safari
	{
		try { w = document.body.offsetWidth; } catch(e) {}
		try { h = document.body.offsetHeight; } catch(e) {}
	}
	return new Array(w,h);
}

/**
 * Gibt die Größe des Anzeigefensters bzw. des aktuellen Frames an.
 * @return int[2] (Breite,Höhe) 
 */
function getFrameSize() {
	var w,h;
	if (self.innerHeight) // all except Explorer
	{
		try { w = self.innerWidth; } catch(e) {}
		try { h = self.innerHeight; } catch(e) {}
	}
	else if (document.documentElement && document.documentElement.clientHeight)
		// Explorer 6 Strict Mode
	{
		try { w = document.documentElement.clientWidth; } catch(e) {}
		try { h = document.documentElement.clientHeight; } catch(e) {}
	}
	else if (document.body) // other Explorers
	{
		try { w = document.body.clientWidth; } catch(e) {}
		try { h = document.body.clientHeight; } catch(e) {}
	}
	return new Array(w,h);
}

function getElem(id) {
	return document.getElemById(id);
}
function getElemStyle(id) {
	try {
		return getElem(id).style;
	}
	catch (e) {
		throw new Error("getElemStyle("+id+"): Element has no properties.");
	}
}

function getElemInnerHTML(id) {
	try {
		return getElem(id).innerHTML;
	}
	catch (e) {
		throw new Error("getElemInnerHTML("+id+"): Element has no properties.");
	}
}

function abbr_number(value) {
  if (value >= 1000000)
    value = (parseInt(value / 100000)/10)+'M';
  else if (value >= 1000)
    value = (parseInt(value / 100)/10)+'k';
    
  return value;
}

/** @deprecated*/
picpath = '../pix/';
/** Entspricht der pic_path-Funktion in PHP*/
/** @deprecated */
function pic_path(pic) {
	if (pic.substr(0,7) == 'http://')
		return pic;
	else
		return picpath+pic;
}

function number_format( number, decimals, dec_point, thousands_sep ) { 
    var n = number, c = isNaN(decimals = Math.abs(decimals)) ? 2 : decimals;
    var d = dec_point == undefined ? "." : dec_point;
    var t = thousands_sep == undefined ? "," : thousands_sep, s = n < 0 ? "-" : "";
    var i = parseInt(n = Math.abs(+n || 0).toFixed(c)) + "", j = (j = i.length) > 3 ? j % 3 : 0;
    
    return s + (j ? i.substr(0, j) + t : "") + i.substr(j).replace(/(\d{3})(?=\d)/g, "$1" + t) + (c ? d + Math.abs(n - i).toFixed(c).slice(2) : "");
}

/**
 * Geht die Elemente eines Formulars durch und erstellt einen String in der Form
 * "name=value&name=value&...", den man dann wunderbar per GET oder POST 
 * übermitteln kann. Der String ist noch nicht url-encoded, aber &-Zeichen sind
 * escaped.
 * 
 * @param Formelement[] form_elems
 * @return string
 */
function form_elems_to_param_string(form_elems) {
	var n = form_elems.length;
	var str = '';
	for (i = 0; i < n; i++) {
		if (form_elems[i].name.length == 0)
			continue;
			
		str += form_elems[i].name + '=';
		if (form_elems[i].type == 'checkbox')
			str += (form_elems[i].checked) ? 1 : 0;
		else
			str += form_elems[i].value;
		str+='&';
	}
	return str;
}

var gen_funcs_loaded = true;