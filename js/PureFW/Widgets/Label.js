/**
 * Ein einfaches Label, das wahlweise ein kleines Bildchen neben dem Textzug 
 * angefügt bekommen kann.
 */


/**
 * Erstellt ein einfaches Label zum Beschriften von Dingen.
 * 
 * @param HTMLElement parent_node		Wo das Label eingehängt werden soll
 * @param uint x						x-Position des Labels
 * @param uint y						y-Position des Labels
 * @param uint min_w					Breite des Labels [0 für auto].
 * @param uint min_h					Höhe des Labels [0 für auto].
 * @param string text					Text, den das Label tragen soll
 * @author Phil
 */
PureFW.Label = function (parent_node, x, y, w, h, text) {
	if (typeof(parent_node) !== 'undefined') {
		this.init(parent_node, x, y, w, h);
		this.text = text || '';
		this.pic_url = '';
		this.pic_h = 0;
		this.pic_w = 0;
		
		this.instance_num = this.constructor.num_instances;
		this.instance_name = "PureFW.Label.instances["+this.instance_num+"]";
		this.id = "Label"+this.instance_num;
		this.bg_img_id = "Label_bg_img"+this.instance_num;
		this.content_id = "Label_cont"+this.instance_num;
		this.text_id = "Label_text"+this.instance_num;
		this.pic_id = "Label_pic"+this.instance_num;
		this.node = null;
		this.pic_node = null;
		this.pic_node_pic = null;
		this.text_node = null;
		
		this.constructor.num_instances++;
		this.constructor.instances[this.instance_num] = this;
		this.insert_into_dom_tree();
		PureFW.WidgetManager.manager_all.register_widget(this);
	}
};

PureFW.Label.extend(PureFW.Container);

/********************************************************************
 * Statische Deklarationen
 ********************************************************************/
PureFW.Label.num_instances = 0;
PureFW.Label.instances = new Array();

PureFW.Label.OVER = 2;
PureFW.Label.UNDER = 1;
PureFW.Label.LEFT = 0;
PureFW.Label.RIGHT = 3;

/********************************************************************
 * Prototype-Funktionen
 ********************************************************************/

/**
 * Fügt das Label in den DOM-Baum ein.
 * 
 * @throws Error
 */
PureFW.Label.prototype.insert_into_dom_tree = function() {
	PureFW.Label.parent.insert_into_dom_tree.call(this);

	
	this.text_node = document.createElement('div');
	this.text_node.innerHTML = this.text;
	
	this.node.appendChild(this.text_node);
};

/**
 * Setzt ein Bildchen neben die Schrift des Labels
 * 
 * @param string pic_url		URL zum Bild
 * @param uint w				Breite des Bildchens
 * @param uint h				Höhe des Bildchens
 * @param [0,1,2,3] pos			Position, des Bildes
 * 
 * PureFW.Label.OVER:	über den Text
 * PureFW.Label.UNDER:	unter den Text
 * PureFW.Label.LEFT:	links vom Text
 * PureFW.Label.RIGHT:	rechts vom Text
 */
PureFW.Label.prototype.set_pic = function(pic_url, w, h, pos) {
	if (this.pic_node)
		this.node.removeChild(this.pic_node);
	if (this.text_node)
		this.node.removeChild(this.text_node);
	
	this.pic_w = w || 20;
	this.pic_h = h || 20;
	this.pic_url = pic_url;
	this.pic_node_pic = document.createElement('img');
	this.pic_node_pic.style.width = this.get_pic_width() + "px";
	this.pic_node_pic.style.height = this.get_pic_height() + "px";
	this.pic_node_pic.src = this.add_pic_path(this.pic_url);
	this.pic_node_pic.id = this.pic_id;
	
	switch (pos) {
	case PureFW.Label.OVER:
		this.pic_node = document.createElement('center');
		this.node.appendChild(this.pic_node);
		
		this.text_node = document.createElement('center');
		this.text_node.id = this.text_id;
		this.node.appendChild(this.text_node);
		break;
	case PureFW.Label.UNDER:
		this.text_node = document.createElement('center');
		this.text_node.id = this.text_id;
		this.node.appendChild(this.text_node);		
		
		this.pic_node = document.createElement('center');
		this.node.appendChild(this.pic_node);
		break;
	case PureFW.Label.RIGHT:
		var table = document.createElement('table');
		table.setAttribute("border", 0);
		table.setAttribute("cellspacing", 0);
		table.setAttribute("cellpadding", 2);
		
		var tr = document.createElement('tr');
		table.appendChild(tr);
		
		this.text_node = document.createElement('td');
		this.text_node.setAttribute("valign", "middle");
		this.text_node.id = this.text_id;
		tr.appendChild(this.text_node);

		this.pic_node = document.createElement('td');
		this.pic_node.setAttribute("valign", "middle");
		tr.appendChild(this.pic_node);

		this.node.appendChild(table);
		break;
	default:
		var table = document.createElement('table');
		table.setAttribute("border", 0);
		table.setAttribute("cellspacing", 0);
		table.setAttribute("cellpadding", 2);
		
		var tr = document.createElement('tr');
		table.appendChild(tr);
		
		this.pic_node = document.createElement('td');
		this.pic_node.setAttribute("valign", "middle");
		tr.appendChild(this.pic_node);
		
		this.text_node = document.createElement('td');
		this.text_node.setAttribute("valign", "middle");
		this.text_node.id = this.text_id;
		tr.appendChild(this.text_node);

		this.node.appendChild(table);
	}
	this.text_node.innerHTML = this.text;
	this.pic_node.appendChild(this.pic_node_pic);
};

/**
 * Setzt den Text des Labels
 * 
 * @param string text
 */
PureFW.Label.prototype.set_text = function(text) {
	this.text = text;
	if (this.text_node)
		this.text_node = document.getElemById(this.text_id);
	document.setElemInnerHTML(this.text_node, text);
};

PureFW.Label.prototype.set_font_color = function(col) {
	if (this.text_node)
		this.text_node = document.getElemById(this.text_id);
	document.getElemById(this.text_id).style.color = col;
}
PureFW.Label.prototype.set_content = function(text) {
	this.set_text(text);
}
/**
 * Setzt den Titel (on hover zu sehen) des Labels
 * 
 * @param string title
 */
PureFW.Label.prototype.set_title = function(title) {
	document.getElemById(this.id).title = title;
};

/**
 * Gibt die Breite des Bildchens zurück, das neben dem Text erscheint
 * 
 * @return uint
 */
PureFW.Label.prototype.get_pic_width = function() {
	return this.pic_w * this.scale_factor;
};

/**
 * Gibt die Höhe des Bildchens zurück, das neben dem Text erscheint
 * 
 * @return uint
 */
PureFW.Label.prototype.get_pic_height = function() {
	return this.pic_h * this.scale_factor;
};


/**
 * Malt das Label neu
 */
PureFW.Label.prototype.repaint = function() {
	this.node = this.get_node();
	this.pic_node = document.getElemById(this.pic_node_id);
	if (this.node) {
		if (this.width)
			this.node.style.width = this.get_width()+"px";
		if (this.height)
			this.node.style.height = this.get_height()+"px";
		this.node.style.left = this.get_x()+"px";
		this.node.style.top = this.get_y()+"px";
		if (this.pic_node_pic) {
			this.pic_node_pic = document.getElementById(this.pic_id);
			this.pic_node_pic.style.width = this.get_pic_width()+"px";
			this.pic_node_pic.style.height = this.get_pic_height()+"px";
		}
	}
};
