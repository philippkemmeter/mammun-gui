/**
 * Dieses Framework ist im Gegensatz zu den den meisten anderen frei verfügbaren
 * Frameworks auf Geschwindigkeit optimiert, derart, dass es komplett in purem
 * Javascript geschrieben ist und keine neue Highlevel-Pseudo-Sprache definiert.
 * Pure Framework ist leicht erweiterbar und wartbar.
 * 
 * @author Phil
 */
PureFW = new Object();

/**
 * Kleine Hilfsdatenstruktur
 */
PureFW.Point = function(x,y) {
	this.x = x;
	this.y = y;
}

/**
 * Checks if a is instance of b
 */
PureFW.instance_of = function(a, b) {
	if (!a.constructor) {
		return false;
	}
	if ((a.constructor === b)
		|| ((b.constructor && (b.constructor == a.constructor))))
	{
		return true;
	}
	if (!a.constructor.parent) {
		return false;
	}
	return PureFW.instance_of(a.constructor.parent, b);
};

/**
 * Prüft, ob zwei Objekte gleich sind.
 */
PureFW.equals = function(obj1, obj2, exact_match) {
	if ((typeof(obj1) != 'object') || (typeof(obj2) != 'object'))
		return false;
	
	var n = 0;
	for (var i in obj1) {
		if (typeof(obj1[i]) != typeof(obj2[i]))
			return false;
		if (typeof(obj1[i]) == 'object') {
			if (!PureFW.equals(obj1[i], obj2[i], exact_match))
				return false;
		}
		if ((exact_match && (obj1[i] !== obj2[i]))
				|| (!exact_match && (obj1[i] != obj2[i])))
			return false;
		n++;
	}
	var m = 0;
	for (var i in obj2)
		m++;
	if (n != m)
		return false;
	
	return true;
}

/**
 * Kopiert alle Style-Attribute von from_elem nach to_elem
 * 
 * @param HTMLElement from_elem
 * @param HTMLElement to_elem
 */
PureFW.copy_style = function(from_elem, to_elem) {
    for (s in from_elem.style) {
        try {
        	to_elem.style[s] = from_elem.style[s];
        }
        catch(e) {}
    }
};

/**
 * Gibt die Parameter, welche über die URL mitgeliefert wurden, als ein Objekt
 * zurück mit OBJ.key = value.
 * 
 * @param string params			Die URL-Paramter (?x=u&z0=hase&...)
 * @return Object
 */
PureFW.get_url_params = function(params) {
	var param_arr = params.substr(1).split('&');
	var n = param_arr.length;
	var pair;
	var result = new Object();
	for (var i = 0; i < n; i++) {
		pair = param_arr[i].split('=');
		result[pair[0]] = pair[1];
	}
	return result;
};

/**
 * Trimmt einen String (wie PHP-Trim). Wird chars angegeben, werden diese
 * Buchstaben herausgefiltert am Anfang und Ende des Strings, sonst wie in PHP
 * \0
 * \t
 * \r
 * \n
 * " "
 * \x0B (vertical Tab)
 * 
 * Basiert auf der Idee und Umsetzung von http://www.webtoolkit.info/.
 * 
 * @param String s
 * @param String characters 
 * @see http://www.webtoolkit.info/
 */
PureFW.trim = function(s, characters) {
	if (s)
		return PureFW.ltrim(PureFW.rtrim(s, characters), characters);
	else
		return '';
};

/**
 * Trimmt den String nur von links.
 * @see PureFW.trim
 * @see http://www.webtoolkit.info/
 */
PureFW.ltrim = function(s, characters) 
{
	if (s)
		return s.replace(new RegExp("^[" + (characters||"\\s") + "]+", "g"), "");
	else
		return '';
};

/**
 * Trimmt den String nur von links.
 * @see PureFW.trim
 * @see http://www.webtoolkit.info/
 */
PureFW.rtrim = function(s, characters)
{
	if(s)
		return s.replace(new RegExp("[" + (characters||"\\s") + "]+$", "g"), "");
	else
		return '';
};


/**
 * Konvertiert alle iFrames im Dokument in Divs. Das kann nötig sein, da es
 * gerade auf mobilen Endgeräten Browser gibt, die keine iFrames unterstützen.
 */
PureFW.iframes_to_div = function() {
    var iframes = document.getElementsByTagName('iframe');
    var n = iframes.length;
    var i, j, ifr, children, m, body;
    for (i = 0; i < n; i++) {
        ifr = iframes[i];
        if (typeof(ifr) == 'undefined')
        	continue;
        PureFW.iframe_to_div(iframes[i]);
    }
};

/**
 * Konvertiert das angegebene iFrame in ein Div
 */
PureFW.iframe_to_div = function(ifr, _parent) {
	_parent = _parent || top;
	ifr_body = _parent[ifr.name].document.getElementsByTagName('body')[0];

    new_div = document.createElement('div');
    PureFW.copy_style(ifr, new_div);
    new_div.style.width = (ifr.width.indexOf('%') == -1)
    	? ifr.width + 'px'
    	: ifr.width;
    new_div.style.height = (ifr.height.indexOf('%') == -1)
    	? ifr.height + 'px'
    	: ifr.height;
    if (ifr.getAttribute("frameborder") !== '0')
        new_div.style.border = '2px black solid';
    if (ifr.getAttribute("scrolling") !== 'no')
        new_div.style.overflow = 'auto';
    else
        new_div.style.overflow = 'hidden';
    
    new_div.id = ifr.id;
    new_div.name = ifr.name;
    new_div.innerHTML = ifr_body.innerHTML;
    ifr.parentNode.replaceChild(new_div, ifr);
}

PureFW.body_loaded = function(ev) {
	/**
	 * Ergänzt, wenn nötig, das document-Objekt um den body-Knoten
	 */
	if (!document.body)
		document.body = document.getElementsByTagName("body")[0];
	 
	/**
	 * Im iPhone können iFrames zwar angezeigt werden, aber mit vielen Fehlern.
	 */
	if (false /*navigator.userAgent.toLowerCase().indexOf('iphone') > -1*/) {
		PureFW.iframes_to_div();
		if (PureFW.WidgetManager) {
			PureFW.WidgetManager.manager_all.set_scale_value(
					PureFW.WidgetManager.scale_reference_value << 1
			);
			PureFW.WidgetManager.manager_all.set_quality('low');
		}
		var viewport_meta = document.createElement('meta');
		viewport_meta.setAttribute("id", "viewport");
		viewport_meta.setAttribute("name", "viewport");
		viewport_meta.setAttribute("content", 
				"maximum-scale = 10");
		document.getElementsByTagName("head")[0].appendChild(viewport_meta);

		var div = document.createElement("div");                       
        div.style.position = "absolute";                               
        div.style.left = "0";                                          
        div.style.top = "0";                                           
        div.style.zIndex = 1000;                                       
        div.style.width = "2000px";                                    
        div.style.height= "2000px";                                    
        div.style.backgroundColor = "black";                           
        div.style.display = "none";                                    
        div.id = "iphone_flipme";                                      
                                                                       
        var img = document.createElement("img");                       
        img.src = "../pix/ui/special_situations/iphone/flipme.png";    
        img.style.position = "absolute";                               
        img.style.left = "0";                                          
        img.style.top = "0";                                           
        div.appendChild(img);                                          
        document.body.appendChild(div);
		
		document.body.onorientationchange = foo = function() {
			/**
			 * Scrollt nach 100ms einen Pixel runter. Das führt dazu, dass das iPhone
			 * die Adressleiste verschwinden lässt, weil das direkt bei einem
			 * Scroll-Event geschieht. Da das ggf. auch bei anderen Browsern der Fall 
			 * ist, und es auf keinen Fall schadet, ist es allgemein drin.
			 */
			PureFW.on_orientation_change();
			window.setTimeout("window.scrollTo(0,1);",100);
		}
		foo();
	}
	else {
		// Alle anderen sollen nach Browsergröße skaliert werden:
		if (PureFW.WidgetManager) {
			var w = window.innerWidth || document.body.clientWidth;
            var h = window.innerHeight || document.body.clientHeight;
            if (PureFW.WidgetManager.page_aspect_ratio && 
            		(h < (w / PureFW.WidgetManager.page_aspect_ratio)) )
            	w = h * PureFW.WidgetManager.page_aspect_ratio;
            PureFW.WidgetManager.manager_all.set_scale_value(w);
        
            PureFW.WidgetManager.manager_all.set_quality('high');
		}
	}
};

PureFW.handle_resize = function(ev) {
	if ((PureFW.WidgetManager) &&
			navigator.userAgent.toLowerCase().indexOf('iphone') < 0)
	{
		var w = window.innerWidth || document.body.clientWidth;
		var h = window.innerHeight || document.body.clientHeight;
		if (PureFW.WidgetManager.page_aspect_ratio && 
				(h < (w / PureFW.WidgetManager.page_aspect_ratio)) )
			w = h * PureFW.WidgetManager.page_aspect_ratio;
		PureFW.WidgetManager.manager_all.set_scale_value(w);
	}
	
	try {
		ev = PureFW.EventUtil.formatEvent(ev);
	}
	catch (e){
	}
	ev.preventDefault();
};

PureFW.on_orientation_change = function() {
	
};

/********************************************************************
 * DOM-Elementmanipulationen und -Erhalten
 ********************************************************************/

/**
 * Hilfsfunktion für getElemById zur Durchsuchung aller Frames nach einem
 * Objekt
 * 
 * @param string id
 * @return HTMLElement
 */
PureFW._get_elem_by_id_frames = function(id) {
	var elem,i;
    for (i = 0; i < frames.length; i++) {
        try {
            elem = frames[i].document.getElementById(id);
            if ((elem) && (typeof(elem) !== 'undefined'))
                return elem;
        }
        catch (e){}
    }
    return null;
}

/**
 * Gibt das Element mit der ID zurück, auch wenn es sich in einer Frame
 * unterhalb des Dokuments befindet. Somit eine Erweiterung von 
 * document.getElementById
 * 
 * @param string id
 * @return HTMLElement
 */
document.getElemById = function(id) {
	var elem;
    try {
        elem = document.getElementById(id);
        if ((elem) && (typeof(elem) !== 'undefined'))
            return elem;
        else {
        	if (elem = PureFW._get_elem_by_id_frames(id))
        		return elem;
        	else
        		return null;
        }
    }
    catch (e) {
    	if (elem = PureFW._get_elem_by_id_frames(id))
    		return elem;
    	else
    		throw new Error("getElem("+id+") has no properties.");
    }
};

/**
 * Setzt das InnerHTML des angegebenen Elements. Im Gegensatz zu 
 * "HTMLElement.innerHTML = XY" ist liegt der Nutzen dieser Funktion in der
 * zusätzlichen Möglichkeit, dass enthaltenes Javascript interpretiert wird,
 * was gerade für AJAX-Anwendungen interessant ist.
 * 
 * @param HTMLElement elem
 * @param string text
 * @param bool interprete_js	[default=false]
 */
document.setElemInnerHTML = function(elem, text, interprete_js) {
	if (typeof(elem) === 'string')
		elem = document.getElemById(elem);
	if (interprete_js) {
		elem.innerHTML = '';
		var arr2, arr1 = text.split('<script');
	    var i,j,k,m,n = arr1.length;
	    for (i = 0, k = false; i < n; i++) {
	        arr2 = arr1[i].split('</script>');
	        m = arr2.length;
	        for (j = 0; j < m; j++, k=!k) {
	            if (k) {
	        		eval(arr2[j].substr(arr2[j].indexOf('>')+1));
	            }
	            else {
	                elem.innerHTML = elem.innerHTML + arr2[j];
	            }
	        }
	    }
	}
	else
		elem.innerHTML = text;
};

/********************************************************************
 * Erweiterungen der built-in sowie Host-Objekte
 ********************************************************************/

/**
 * Erweiterung des Function-Objekts um eine einfache Vererbungsmethodik.
 * Die Vererbung bleibt - Javascript getreu - prototypbasiert, es wird
 * zusätzlich ein Link zum Vater erstellt: this.parent.
 * 
 * @param Object parent_o
 */
Function.prototype.extend = function(parent_o) {
	/**
	 * Wenn es sich bei der Vaterklasse um eine nicht-statische und 
	 * nicht-abstrake Klasse handelt, dann den Prototyp entsprechend erweitern
	 * (denn <code>this</code> ist somit notwendigerweise auch nicht-statisch
	 * und nicht-abstrakt)
	 */
	if (parent_o.constructor == Function) {
		this.prototype = new parent_o;
		this.prototype.constructor = this;
		this.parent = parent_o.prototype;
	}
	else {
		/**
		 * Andernfalls wird der Prototyp entsprechend erweitert und der
		 * Konstruktor gesetzt.
		 */
		for (x in parent_o)
			this.prototype[x] = parent_o[x];
		this.prototype.constructor = this;
		this.parent = parent_o;
	}
};

/**
 * Ergänzt das Array-Objekt um die Funktion contains, welche angibt, ob
 * das Array den übergebenen Wert enthält.
 * 
 * @param mixed obj
 * @return boolean
 */
Array.prototype.contains = function(obj, excat_match) {
	if (typeof(exact_match) == 'undefined')
		exact_match = true;
	var listed = false;
	for (var i = 0; i < this.length; i++) {
		if ((exact_match && (this[i] === obj)) 
			|| (!exact_match && this[i] == obj)) 
		{
			listed = true;
			break;
		}
	}
	return listed;
};

/**
 * Ergänzt das Array-Objekt um die Funktion search, welche den Indexschlüssel
 * zurückgibt, unter welchem das angegebene Objekt abgelegt ist. Wird das
 * Element nicht gefunden, wird -1 zurückgegeben.
 * 
 * @param mixed obj
 * @return {-1, 0, 1, ..., n-1}
 */
Array.prototype.search = function(obj, exact_match) {
	if (typeof(exact_match) == 'undefined')
		exact_match = true;

	if (typeof(obj) == 'object') {
		for (var i = 0; i < this.length; i++) {
			if (PureFW.equals(this[i], obj, exact_match))
				return i;
		}
	}
	else {
		for (var i = 0; i < this.length; i++) {
			if ((exact_match&&(this[i] === obj))
					|| (!exact_match&&(this[i] == obj)))
				return i;
		}
	}

	return -1;
}

/**
 * Entfernt das angegebene Objekt aus dem Array. Wird der letzte Parameter
 * "max_removements" kann optional angegeben werden, um anzugeben, wie viele 
 * Vorkommen des Objekts maximal gelöscht werden dürfen.
 * 
 * @param mixt obj
 * @param uint max_removements
 * @return Array	Gibt sich selbst nach der Operation zurück
 */
Array.prototype.remove = function(obj, max_removements) {
    var i = 0;
    var n = this.length;
    var r = 0;
    while ((i < n) && (!max_removements || ((r < max_removements)))) {
        if (this[i] === obj){
            this.splice(i, 1);
            r++;
            n--;
        } else {
            i++;
        }
    }
    return this;
};

/**
 * Zerstört das Array und ruft destroy für alle Kinder auf.
 */
Array.prototype.destroy = function() {
	for (var i = 0; i < this.length; i++) {
		if (typeof(this[i]) !== 'function') {
			try {
				this[i].destroy();
			}
			catch(e) {
				try {
					delete this[i];
				}
				catch(e) {}
			}
		}
	}
	try {
		delete this;
	}
	catch(e) {}
}

/**
 * Mischt das Array (basiert auf Fisher-Yates-Algorithmus)
 */
Array.prototype.shuffle = function() {
	var i = this.length;
	if ( i == 0 )
		return this;
	while ( --i ) {
		var j = Math.floor( Math.random() * ( i + 1 ) );
		var tempi = this[i];
		var tempj = this[j];
		this[i] = tempj;
		this[j] = tempi;
	}
	return this;
}

/**
 * Gibt ein zufällig gezogenes Element zurück
 * 
 * @return mixed
 */
Array.prototype.random = function() {
	return this[Math.floor(Math.random()*this.length)];
}

/**
 * Array-Push-Methode, wenn noch nicht existent
 */
if (!Array.prototype.push) {
	Array.prototype.push = function(elem) {
		this[this.length] = elem;
	}
};

/**
 * Prüft, ob a ein Array ist.
 * 
 * @param mixed a
 * @return bool
 */
isArray = function (a) {
	return ((a && (typeof a == 'object')) || (typeof a == 'function')) 
		&& (typeof(a.length) != 'undefined');
}

/**
 * Math-Objekt wird um die sign-Funktion erweitert.
 */
if (!Math.sign) {
	Math.sign = function(n) {
		if (n > 0)
			return 1;
		if (n < 0)
			return -1;
		else
			return 0;
	}
}

/**
 * Rückgabe des Strings als Kopie mit erstem Buchstaben groß.
 * @return
 */
String.prototype.ucfirst = function() {
    return this.charAt(0).toUpperCase() + this.substr(1);
}

/**
 * Gibt standardisiert zurück, wie weit das Dokument in x-Richtung gescrollt 
 * wurde.
 * 
 * @return uint
 */
document.getScrollX = function() {
	return document.documentElement && document.documentElement.scrollLeft ?
			document.documentElement.scrollLeft : document.body.scrollLeft;
}

/**
 * Gibt standardisiert zurück, wie weit das Dokument in y-Richtung gescrollt 
 * wurde.
 * 
 * @return uint
 */
document.getScrollY = function() {
	return document.documentElement && document.documentElement.scrollTop ?
			document.documentElement.scrollTop : document.body.scrollTop
}