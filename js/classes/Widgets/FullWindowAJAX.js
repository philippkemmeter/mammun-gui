/**
 * Vollbild, rahmenlos in der Mitte der Inhalt (per AJAX geladen).
 * Es kann immer nur ein solches Fenster gleichzeitig existieren.
 * 
 * Ein FullWindowAJAX besteht aus einem Container, der alles überdeckt (vollbild),
 * und einem ContainerAJAX, der den Inhalt enthält.
 *
 * @author Phil
 */

/**
 * Singleton
 * 
 * @param HTMLElement parent_node	Knoten, wo eingehängt werden soll
 * @param uint body_w				Breite des Bodys (inner container)
 * @param uint body_h				Höhe des Bodys (inner container)
 * @param string url				URL woraus die Daten geladen werden sollen
 */
FullWindowAJAX = function(parent_node, body_w, body_h, url) {
	var viewport_meta = document.getElemById("viewport");
	if (viewport_meta) {
		viewport_meta.setAttribute("content",
			"minimum-scale=0.48,maximum-scale=10,initial-scale=0.48"
		);
		PureFW.WidgetManager.manager_all.set_scale_value(1000);
		window.scrollTo(0,0);
//		document.getElemById("mapx").style.display = "none";
	}
	
	if (this.constructor.instance) {
		this.constructor.instance.set_url(url);
		this.constructor.instance.set_body_width(body_w || 400);
		this.constructor.instance.set_body_height(body_h || 300);
		this.constructor.instance.set_parent_node(parent_node || document.body);
		try {delete this; } catch (e) {}
		return this.constructor.instance;
	}
	if (typeof(PureFW.Container) == 'undefined')
		throw new Error('PureFW.Container needs to be '+
				'loaded before creating an FullWindowAJAX object.');
	if (typeof(PureFW.ContainerAJAX) == 'undefined')
		throw new Error('PureFW.ContainerAJAX needs to be '+
				'loaded before creating an FullWindowAJAX object.');
	
	this.init(parent_node);
	this.url = url;
	
	this.body_width = body_w || 400;
	this.body_height = body_h || 300;
	
	this.position = new PureFW.Point(0,0);
	
	x = getBodySize();
	this.width = 1000;
	this.height = 700;

	this.id = 'AJAXFFWin';
	this.bg_img_id = "AJAXFFWin_bg_img";
	this.content_id = "AJAXFFWin_cont";
	/*ContainerAJAX*/this.body = null;

	if (parent_node)
		this.insert_into_dom_tree();
	this.constructor.instance = this;
//	PureFW.WidgetManager.manager_all.register_widget(this);
};

/********************************************************************
* FullWindowAJAX extends PureFW.Container
********************************************************************/
FullWindowAJAX.extend(PureFW.Container);

/**
 * Das setzen von Position, Höhe oder Breite ist nicht möglich!
 */
FullWindowAJAX.prototype.set_position = null;
FullWindowAJAX.prototype.set_width = null;
FullWindowAJAX.prototype.set_height = null;

/********************************************************************
* Statische Deklarationen
********************************************************************/
/*PRIVATE*/ FullWindowAJAX.instance = null;

/********************************************************************
* Prototyp-Deklarationen
********************************************************************/
/**
 * Fügt dem Container Inhalt (HTML ohne Javascript) hinzu.
 * 
 * @param string text
 * @see PureFW.Container
 */
FullWindowAJAX.prototype.add_content = function(text) {
	this.body.add_content(text);
}

/**
 * Setzt den Inhalt (HTML) des Containers. Wird interprete_js angegeben, wird
 * ggf. mitgesandtes JS (innerhalb der Tags <script></script>) interpretiert!
 * 
 * @param string text
 * @param bool interprete_js [=false]
 * @see PureFW.Container                          
 */
FullWindowAJAX.prototype.set_content = function(text, interprete_js) {
	this.body.set_content(text, interprete_js);
}

/**
* Gibt den Inhalt (HTML) des Containers zurück.
* @see PureFW.Container
* @return string
*/
FullWindowAJAX.prototype.get_content = function() {
	return this.body.get_content();
}

/**
 * Gibt den Body zurück, also den inneren ContainerAJAX, welcher den gesamten
 * Inhalt enthält
 * 
 * @return ContainerAJAX
 */
FullWindowAJAX.prototype.get_body = function() {
	return this.body;
}

/**
 * Malt das Fenster neu
 */
FullWindowAJAX.prototype.repaint = function() {
	FullWindowAJAX.parent.repaint.call(this);
	if (this.body)
		this.body.repaint();
}
 
/**
 * Fensterinhalt durch Inhalt der angegebenen Ziel-URL neu füllen (url
 * ansurfen)
 * 
 * @param String str
 * @see PureFW.ContainerAJAX.set_url
 */
 FullWindowAJAX.prototype.set_url = function(url) {
	this.body.set_url(url);
};

/**
 * Gibt die zuletzt geladene URL zurück
 * 
 * @return String
 * @see PureFW.ContainerAJAX.get_url
 */
FullWindowAJAX.prototype.get_url = function() {
	return this.body.get_url();
}

/**
 * Setzt die Breite des Fenster-Bodys
 * 
 * @param uint w
 */
FullWindowAJAX.prototype.set_body_width = function(w) {
	this.body_width = w;
	this.body.set_width(w);
};

/**
 * Setzt die Höhe des Fenster-Bodys
 * 
 * @param uint h
 */
FullWindowAJAX.prototype.set_body_height = function(h) {
	this.body_height = h;
	this.body.set_height(h);
};
 
FullWindowAJAX.prototype.get_body_width = function() {
	return this.body_width;// * this.scale_factor;
};
FullWindowAJAX.prototype.get_body_height = function() {
	return this.body_height;// * this.scale_factor;
};

/**
 * Fügt das Fenster in den DOM-Baum ein.
 */
FullWindowAJAX.prototype.insert_into_dom_tree = function() {
	FullWindowAJAX.parent.insert_into_dom_tree.call(this);
	this.set_bg_img('pattern/white.png');
	this.set_bg_repeat('repeat');
	if (document.getElementsByTagName("iframe").length == 0) {
		this.get_node().style.width = '2000';
		this.get_node().style.height = '1400';
	}
	else {
		this.get_node().style.width = '100%';
		this.get_node().style.height = '100%';
	}
	this.set_z_index(50);
	this.body = new PureFW.ContainerAJAX(
			document.getElemById(this.id), 
			(this.get_width() >> 1) - (this.get_body_width() >> 1),
			(this.get_height() >> 1) - (this.get_body_height() >> 1),
			this.get_body_width(),
			this.get_body_height(),
			this.url
	);
	this.body.scale(1);
	this.body.add_event_handler("load", function(){
		//FullWindowAJAX.instance.get_node().style.backgroundColor = '#000';
	});
	PureFW.WidgetManager.manager_all.unregister_widget(this.body);
};

/**
 * Zerstört das Fenster
 * @see PureFW.Widget.destroy
 * @see PureFW.Container.destroy
 */
FullWindowAJAX.prototype.destroy = function() {
	FullWindowAJAX.parent.destroy.call(this);
	var viewport_meta = document.getElemById("viewport");
	if (viewport_meta) {
		viewport_meta.setAttribute("content",
			"minimum-scale=0.24,maximum-scale=10,initial-scale=0.24"
		);
		PureFW.WidgetManager.manager_all.set_scale_value(2000);
//		document.getElemById("mapx").style.display = "block";
	}
	this.constructor.instance = null;
}