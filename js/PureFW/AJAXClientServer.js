/**
 * Verwaltet Server-(AJAX)-Requests. Erwartet vom Server Antworten in der Form
 * TYP||WERT||TYP||WERT.... wobei "||" tatsächlich Trenner sind, TYP eine der
 * unten angegebenen Konstanten (CMD_RELOGIN ff.) sein muss, und Wert zu Typ
 * passen sollte ;-)
 * 
 * @see ClientServer.inc (PHP-Variante)
 * @author Phil
 */

/** Alles statisch */
PureFW.AJAXClientServer = new Object();

/** Welche Funktion bei erfolgreichem Request aufgerufen werden soll */
PureFW.AJAXClientServer.callback_functions = new Array();
/** 
 * Der Name der Session, den der Server vergeben hat (muss mitgesandt werden
 * zur Identifikation der Verbindung
 */
PureFW.AJAXClientServer.session_name = '';
/** 
 * Die ID der Session, den der Server vergeben hat (muss mitgesandt werden
 * zur Identifikation der Verbindung 
 */
PureFW.AJAXClientServer.session_id = '';
/**
 * Array von XHR-Objekten. Immer, wenn gerade eines in Gebrauch
 * ist, wird ein neues erzeugt. Alte, ungenutzte, werden dann wieder freigegeben
 * und deren Slots können erneut benutzt werden.
 */
PureFW.AJAXClientServer.xml_http_requests = new Array();
/**
 * Array, das angibt, ob das XHR-Objekt in xml_http_requests mit demselben Index
 * gerade in gebrauch ist, oder nicht.
 */
PureFW.AJAXClientServer.xhr_available = new Array();
/**
 * Anzahl der Requests. Wird beim Abschicken hochgezählt, damit Cachen vermieden
 * wird.
 */
PureFW.AJAXClientServer.request_num = 0;


/**
 * Konstanten, die beim Parsen gebraucht werden
 */
PureFW.AJAXClientServer.CMD_RELOGIN = 1;	// Relogin nötig
PureFW.AJAXClientServer.VALUE = 100;		// Ein Objekt
PureFW.AJAXClientServer.VALUE_SIMPLE = 101;// Ein Primitivum
PureFW.AJAXClientServer.SESSION = 103;		// Die Session-Daten
PureFW.AJAXClientServer.EXCEPTION = 104;	// Eine Fehlermeldung


/**
 * Gibt ein neu erzeugtes XHR-Objekt zurück
 * 
 * @return XMLHttpRequest
 * @throws Error
 */
PureFW.AJAXClientServer.create_new_request = function() {
	if (typeof XMLHttpRequest != 'undefined')
		return new XMLHttpRequest();
	else if (window.ActiveXObject) {
		var avers = ["Microsoft.XmlHttp", "MSXML2.XmlHttp", 
		             "MSXML2.XmlHttp.3.0",  "MSXML2.XmlHttp.4.0", 
		             "MSXML2.XmlHttp.5.0"];
		for (var i = avers.length -1; i >= 0; i--) {
			try {
				httpObj = new ActiveXObject(avers[i]);
				return httpObj;
			} catch(e) {}
		}
	}
	throw new Error('XMLHttp (AJAX) not supported');
};

/**
 * PRIVATE:
 * Gibt den Index des nächsten freien XHR-Objekts zurück
 * 
 * @private
 * @return uint
 * @throws Error
 */
PureFW.AJAXClientServer.get_xhr_index = function() {
	var n = PureFW.AJAXClientServer.xhr_available.length;
	var result;
	for (var i = 0; i < n; i++) {
		if (PureFW.AJAXClientServer.xhr_available[i]) {
			result = i;
			break;
		}
	}

	if (i == n) {
		// Wenn kein neues gefunden werden konnte, dann ein neues erzeugen
		PureFW.AJAXClientServer.xml_http_requests[n] =
			PureFW.AJAXClientServer.create_new_request();
		result = n;
	}
	// und als "not available" markieren
	PureFW.AJAXClientServer.xhr_available[result] = false;

	return result;
};

/**
 * Sendet einen Request an den durch die URL bestimmten Server. Wird post_data
 * angegeben, werden sie als POST-Daten versandt, ansonsten wird die URL 
 * inklusive aller mitgelieferten Parameter als GET versandt.
 * Die angegebene callback_function wird dann bei Erfolg aufgerufen.
 * 
 * @param string url					Bestimmt das Ziel des Requests
 * @param Function callback_function	Welche Funktion bei Erfolg aufzurufen ist
 * @param string post_data[optional]	ggf. POST-Daten zum Versenden
 * @param bool no_parse[=false]			Ob die Serverantwort geparst werden soll
 * @throws Error						wenn Request nicht gesendet werden konnte
 */

PureFW.AJAXClientServer.send_request = function(url, callback_function, 
	post_data, no_parse) 
{
	var xhri = PureFW.AJAXClientServer.get_xhr_index();
	var xhr = PureFW.AJAXClientServer.xml_http_requests[xhri];
	
	PureFW.AJAXClientServer.callback_functions[xhri] = callback_function;
	var method = (typeof(post_data) == 'undefined') ? "GET" : "POST";

	url = (url.indexOf('?') == -1) ? url + '?' : url + '&';
	url += "anti_cache=" + PureFW.AJAXClientServer.request_num;
	if (url.indexOf('&s=') == -1) {
		url += "&"+PureFW.AJAXClientServer.session_name + "="
				+ PureFW.AJAXClientServer.session_id;
	}
	xhr.open(method, url, true);
	xhr.onreadystatechange = function() {
		var my_xhr = PureFW.AJAXClientServer.xml_http_requests[xhri];
		if ((my_xhr.readyState == 4) && (my_xhr.status == 200)) {
			PureFW.AJAXClientServer.xhr_available[xhri] = true;
			
			//try {
			
			if (PureFW.AJAXClientServer.callback_functions[xhri]) {
				if(no_parse)
				{
					PureFW.AJAXClientServer.callback_functions[xhri].call(this,
							my_xhr.responseText);
				}
				else
				{
					PureFW.AJAXClientServer.callback_functions[xhri].call(this,
						PureFW.AJAXClientServer.parse(my_xhr.responseText));
				}
			};
				
			/*}
			catch (e) {
				PureFW.AJAXClientServer.callback_functions[xhri].call(this, e + my_xhr.responseText);
			}*/
		}
	};
	if (method == "POST") {
		xhr.setRequestHeader("Content-type", 
			"application/x-www-form-urlencoded;charset=UTF-8");
		/* Unsafe: Wird von modernen Browser nicht erlaubt
		 * xhr.setRequestHeader("Content-length", post_data.length);
		 */
		xhr.send(encodeURI(post_data));
	}
	else {
		xhr.setRequestHeader("Content-type", 
			"text/plain;charset=UTF-8");
		/* Unsafe: Wird von modernen Browser nicht erlaubt
		 * xhr.setRequestHeader("Content-length", 0);
		 */
		xhr.send(null);
	}
	PureFW.AJAXClientServer.request_num++;
	return true;
};
PureFW.AJAXClientServer.show_reload_demand_box = null;
PureFW.AJAXClientServer.show_reload_demand = function() {
	if (PureFW.AJAXClientServer.show_reload_demand_box)
		return;
	
	var body_w = PureFW.WidgetManager.scale_reference_value;
	var body_h = PureFW.WidgetManager.scale_reference_value  / 
							PureFW.WidgetManager.page_aspect_ratio;
	var box = new PureFW.ConfirmationBox(
		document.body,
		(body_w - 350) >> 1,
		(body_h - 220) >> 1,
		350, 220,
		PureFW.ConfirmationBox.NO_BUTTONS,
		true
	);
	box.set_bg_color('#ecc');
	box.set_font_color('#000');
	box.set_content(
		'The session has timed out or a server error has occured. The page has '
		+'to be reloaded in order to continue.<br/>'
		+'<br/>'
		+'Unfortunately, the security settings of your browser do not allow '
		+'an automated page reload.<br/>'
		+'<br/>'
		+'<b>Please reload the page manually by pressing F5</b>.'
	);
	
	PureFW.AJAXClientServer.show_reload_demand_box = box;
}

PureFW.AJAXClientServer.parse = function(str) {
	var r_arr = str.split("||");
	var result = new Array();
	while (r_arr.length > 0) {
		var t = r_arr.shift();
		switch (parseInt(t)) {
			case PureFW.AJAXClientServer.CMD_RELOGIN:
				try {
					top.location.reload();
				}
				catch(e) {	/** Cross-Domain */
					PureFW.AJAXClientServer.show_reload_demand();
				}
				return;
			case PureFW.AJAXClientServer.VALUE:
			case PureFW.AJAXClientServer.VALUE_SIMPLE:
				result.push(r_arr.shift());
				break;
			case PureFW.AJAXClientServer.SESSION:
				PureFW.AJAXClientServer.session_name = r_arr.shift();
				PureFW.AJAXClientServer.session_id = r_arr.shift();
				break;
			case PureFW.AJAXClientServer.EXCEPTION:
				r_arr.shift();	// Class ignorieren
				throw new Error(r_arr.shift());
				// Code ignorieren
			default:
				if (str.indexOf('logout()') != -1) {
					try {
						top.location.reload();
					}
					catch(e) {	/** Cross-Domain */
						PureFW.AJAXClientServer.show_reload_demand();
					}
					return;
				}
				else
					throw new Error("PARSING_ERROR: "+str);
		}
	}
	return result;
};