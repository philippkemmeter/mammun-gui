/**
 * Repräsentiert ein Icon. Das Icon besteht aus einem Bild, ggf. einer 
 * Unterschrift, ggf. einem Tooltip-Text u.a.
 * Icon wird NICHT absolut positioniert, sondern "direkt" an den parent_node
 * angehängt. Absolute Positionierung möglich, indem parent_node derart 
 * positioniert ist.
 *
 * @author Phil
 */


PureFW.Icon = function(parent_node, x, y, w, h, bild, color) {
	if (typeof(parent_node) !== 'undefined') {
		this.init();
		
		/****************************************************************
		 * Public Eigenschaften (priveligiert), geerbet von Widget
		 ****************************************************************/
		this.parent_node = parent_node || document.body;
		this.position = new PureFW.Point(x||0, y||0);
		this.width = w || 48;
		this.height = h || 48;
		this.color = color || 'white';
		this.pic_url = bild || '';
		
		this.instance_num = this.constructor.num_instances;
		this.instance_name = "PureFW.Icon.instances["+this.instance_num+"]";
		
		this.id = 'Icon'+this.constructor.num_instances;
		this.node = null;
		
		this.subtitle = '';
		this.tooltip = '';
		this.overicons = '';
		this.bg_img = '';
		
		this.subtitle_id = 'Icon_subtitle'+this.instance_num;
		this.subtitle_node = null;
		this.overicons_id = 'Icon_overlays'+this.instance_num;
		this.overicons_node = null;
		this.img_id = 'Icon_img'+this.instance_num;
		this.img_node = null;
		this.bg_img_node = null;
		this.bg_img_id = 'Icon_bg_img'+this.instance_num;
	
	
	
		
		/****************************************************************
		 * Initial Statements
		 ****************************************************************/
		this.set_color(this.color);
		
		this.constructor.instances[this.instance_num] = this;
		this.constructor.num_instances++;
	
		this.insert_into_dom_tree();
		PureFW.WidgetManager.manager_all.register_widget(this);
	}
};

/********************************************************************
 * Icon EXTENDS Widget
 ********************************************************************/
PureFW.Icon.extend(PureFW.Widget);

/********************************************************************
 * Statische Deklarationen
 ********************************************************************/
PureFW.Icon.num_instances = 0;			// Instanzüberwachung (Anzahl)
PureFW.Icon.instances = new Array();

/********************************************************************
 * Prototyp-Funktionen (public)
 ********************************************************************/

/**
 * Fügt das Icon in den DOM-Baum ein
 */
PureFW.Icon.prototype.insert_into_dom_tree = function() {
	PureFW.Icon.parent.insert_into_dom_tree.call(this);
	
	this.node = document.createElement('div');
	this.node.style.width =  this.get_width()+'px';
	this.node.style.height = this.get_height()+'px';
	this.node.style.position = 'absolute';
	this.node.style.left = this.get_x()+'px';
	this.node.style.top = this.get_y()+'px';
	this.node.style.cursor = 'pointer';
	this.node.style.color = this.color;
	this.node.style.overflow = 'hidden';
	this.node.id=this.id;
	
	this.bg_img_node = document.createElement('img');
	this.bg_img_node.style.width = "100%";
	this.bg_img_node.style.height = "100%";
	this.bg_img_node.style.display = 'block';
	this.bg_img_node.style.zIndex = 0;
	this.bg_img_node.style.position = 'absolute';
	this.bg_img_node.style.left = 0;
	this.bg_img_node.style.top = 0;
	if (this.bg_img) {
		this.bg_img_node.src = this.add_pic_path(this.bg_img);
	}
	this.bg_img_node.id = this.bg_img_id;
	
	this.img_node = document.createElement('img');
	this.img_node.src = this.add_pic_path(this.pic_url);
	this.img_node.style.width = "100%";
	this.img_node.style.height = "100%";
	this.img_node.style.position = 'absolute';
	this.img_node.style.left = 0;
	this.img_node.style.top = 0;
	this.img_node.style.display = 'block';
	this.img_node.style.zIndex = 1;
	this.img_node.style.cursor = 'pointer';
	this.img_node.id = this.img_id;
	
	this.subtitle_node = document.createElement('div');
	this.subtitle_node.style.textAlign = 'center';
	this.subtitle_node.style.cursor = 'pointer';
	this.subtitle_node.style.width = "100%";
	this.subtitle_node.style.height = 16 * this.scale_factor + 'px';
	this.subtitle_node.style.position = 'absolute';
	this.subtitle_node.style.left = 0;
	this.subtitle_node.style.top = (this.get_height()-16*this.scale_factor)+'px';
	this.subtitle_node.style.zIndex = 2;
	this.subtitle_node.cursor = 'pointer';
	this.subtitle_node.innerHTML = this.subtitle;
	this.subtitle_node.id = this.subtitle_id;
	
	this.overicons_node = document.createElement('div');
	this.overicons_node.display = 'none';
	/*this.overicons_node.style.width = 32*this.scale_factor + 'px';
	this.overicons_node.style.height = (this.get_height()-16) + 'px';
	this.overicons_node.style.cursor = 'pointer';
	this.overicons_node.style.display = 'block';
	this.overicons_node.id = this.overicons_id;*/

	if (this.tooltip)
		this.node.title = this.tooltip;
	this.parent_node.appendChild(this.node);
	this.node.appendChild(this.bg_img_node);
	this.node.appendChild(this.img_node);
	this.node.appendChild(this.overicons_node);
	this.node.appendChild(this.subtitle_node);
};

/**
 * Setzt den Untertitel des Icons.
 *
 * @param string subtitle	Der neue Untertitel
 */
PureFW.Icon.prototype.set_subtitle = function(subt) {
	this.subtitle = subt;
	this.subtitle_node = document.getElemById(this.subtitle_id);
	if (this.subtitle_node) {
		document.setElemInnerHTML(this.subtitle_node, this.subtitle);
		if (!this.subtitle || this.subtitle == '') {
			this.subtitle_node.style.display = 'none';
			this.img_node.style.height = 
					this.get_height()+'px';
		}
		else {
			this.subtitle_node.style.display = 'block';
			this.img_node.height = 
				(this.get_width()-parseInt(this.subtitle_node.style.height))+'px';
		}
	}
};

/**
 * Setzt das Hintergrundbild anhand der übergebenen Farbe. Gültige Farben
 * sind übliche Farbworte wie "white" oder "green".
 *
 * @param string col	Die Farbe
 */
PureFW.Icon.prototype.set_color = function(col) {
	this.color = col;
	if (this.width == this.height)
		this.set_bg_img('/ui/icons/backgrounds/bg_'+this.color+'_'+this.width+'.png');
	else
		this.set_bg_img('/ui/icons/backgrounds/bg_'+this.color+'_'+this.width+'x'+
			this.height+'.png');
};

/**
 * Gibt die aktuell gewählte Icon-Farbe zurück
 */
PureFW.Icon.prototype.get_color = function() {
	return this.color;
};
/**
 * Setzt den Tooltip, der erscheint, wenn man mit der Maus über das Icon
 * fährt.
 * 
 * @param string str
 */
PureFW.Icon.prototype.set_tooltip = function(str) {
	this.tooltip = str;
	if (elem = this.get_node())
		elem.setAttribute("title", this.tooltip);
};

/**
 * Setzt die Icon Overlay Images, also die Bildchen, welche über das Icon
 * drübergeblendet werden sollen andhand des übergebenen Arrays von Bild-URLs.
 *
 * @param string[] img_url_arr	Bild-URL-Array
 */
PureFW.Icon.prototype.set_icon_overlay_imgs = function(img_url_arr) {
	this.overicons = '';
	for (var i = 0; i < this.img_url_arr.length; i++) {
		this.overicons += '<img src="'+this.add_pic_path(
			this.img_url_arr[i])+'" width="16" height="16" />';
	}
	if (this.overicons_node) {
		document.setElemInnerHTML(this.overicons_node, this.overicons);
		if (!this.overicons || this.overicons == '') {
			this.overicons_node.style.display = 'none';
			this.img_node.style.width =	this.get_width()+'px';
		}
		else {
			this.overicons_node.style.display = 'block';
			this.img_node.style.width =	(this.get_width() 
					- parseInt(this.overicons_node.style.width))+'px';
		}
	}
};

/**
 * Fügt das Bild mit der übergebenen URL zu den Icon Overlay Images hinzu.
 *
 * @param string img_url	Bild-URL
 */
PureFW.Icon.prototype.add_icon_overlay_img = function(img_url) {
	this.overicons += '<img src="'+this.add_pic_path(
		this.img_url)+'" width="16" height="16" />';
	if (this.overicons_node) {
		document.setElemInnerHTML(this.overicons_node, 
				this.overicons_node.innerHTML +	this.overicons);
		this.overicons_node.style.display = 'block';
		this.img_node.style.width =	(this.get_width() - 
				parseInt(this.overicons_node.style.width))+'px';
	}
};

/**
 * Setzt das Hintergrundbild des Icons anhand der übergebenen Bild-URI.
 *
 * @param string img			Die Bild-URI
 */
PureFW.Icon.prototype.set_bg_img = function(img) {
	this.bg_img = img;
	if (this.bg_img_node = document.getElemById(this.bg_img_id)) {
		this.bg_img_node.src = this.add_pic_path(this.bg_img);
	}
};

/**
 * Malt das Icon neu.
 */
PureFW.Icon.prototype.repaint = function() {
	this.node = document.getElemById(this.id);
	this.subtitle_node = document.getElemById(this.subtitle_id);
	this.img_node = document.getElemById(this.img_id);
//	if (!this.node)
//		alert(this.id);
	if (this.node) {
		this.node.style.width = this.get_width() + 'px';
		this.node.style.height = this.get_height() + 'px';
		this.node.style.top = this.get_y() + 'px';
		this.node.style.left = this.get_x() + 'px';
		this.subtitle_node.style.top = 
			(this.get_height() - 16*this.scale_factor)+'px';
		this.img_node.src = this.add_pic_path(this.pic_url);
	}
	this.set_color(this.get_color());
};

/**
 * Gibt zurück, wieviel Platz in Pixeln ein Icon horizontal belegt. Dies
 * ist nicht gleich der Breite, da das Icon einen Abstand rechts und links
 * für sich beansprucht. Die Summe dieser Abstände und der Breite ist somit
 * das Ergebnis dieser Funktion.
 *
 * @return uint
 */
PureFW.Icon.prototype.get_horizontal_space = function() {
	return (this.width+10)*this.scale_factor;
}