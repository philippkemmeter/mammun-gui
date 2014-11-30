/**
 * Ein PopupMenü (z.B. Rechte-Maustasten-Menü)
 * 
 * @author Phil
 */
 
/** Statische Variable */
/*PRIVATE*/ var PopupMenu__num_instances = 0;			// Instanzüberwachung (anzahl)
/*PRIVATE*/ var PopupMenu__instances = new Array();		// Instanzüberwachung (Instanzen)


/** Konstruktor */
PopupMenu = function(w, title, parent_node) {
	this.width = w;
	this.title = title ? title : '';
	this.position = new PureFW.Point(0,0);
	this.id = PopupMenu__num_instances;
	this.parent_node = parent_node ? parent_node : document.body;
	
	PopupMenu__num_instances++;
	this.popup_id = "popup_menu_"+PopupMenu__num_instances;
	this.title_id = "popup_menu_title"+PopupMenu__num_instances;
	this.create_menu();
	
	PopupMenu__instances[this.id] = this;
};

/** nicht-statische Funktionen */
PopupMenu.prototype={
	show:function() {
		PopupMenu__hide_all();
		document.getElemById(this.popup_id).style.visibility = 'visible';
	},
	hide:function() {
		document.getElemById(this.popup_id).style.visibility = 'hidden';
	},
	set_title:function(title) {
		this.title = title;
		document.setElemInnerHTML(this.title_id, title);
	},
	set_tooltip:function(title) {
		this.set_title(title);
	},
	set_position:function(x,y) {
		// automatischer Test, dass das Popup nicht horizontal aus dem Body
		// rausragen kann - dann wird es zur anderen Seite hin geöffnet
		var bsz = getBodySize();
		if (x+2 > (bsz[0] - parseInt(getElemStyle(this.popup_id).width)-2))
			this.position.x = (x - parseInt(getElemStyle(this.popup_id).width)-2);
		else
			this.position.x = x+2;
		this.position.y = y;
		
		document.getElemById(this.popup_id).style.left = this.position.x + 'px';
		document.getElemById(this.popup_id).style.top = this.position.y + 'px';
	},
	add_entry:function(name, href) {
		var entry = document.createElement('div');
		document.getElemById(this.popup_id).appendChild(entry);
		entry.id=this.popup_id + '_' +
			document.getElemById(this.popup_id).childNodes.length;
		document.setElemInnerHTML(entry.id, name);
		if (href)
			this.activate_entry(getElem(this.popup_id).childNodes.length-1, href);
		else
			this.deactivate_entry(getElem(this.popup_id).childNodes.length-1);
	},
	change_entry:function(number, name, href) {
		var p = getElem(this.popup_id);
		var entry = p.childNodes[number];
		document.setElemInnerHTML(entry.id, name);
		if (href)
			this.activate_entry(number, href);
		else
			this.deactivate_entry(number, href);
	},
	deactivate_entry:function(number) {
		var p = getElem(this.popup_id);
		var entry = p.childNodes[number];
		entry.className = "disabled";
		entry.setAttribute('onmouseover', '');
		entry.setAttribute('onmouseout', '');
		entry.setAttribute('onclick', ''); 
	},
	activate_entry:function(number, href) {
		var p = getElem(this.popup_id);
		var entry = p.childNodes[number];
		entry.className="";
		if (href.toLowerCase().indexOf('javascript') != -1)
			entry.setAttribute('onclick', href);
		else
			entry.setAttribute('onclick', 'javascript: '+href);
		entry.setAttribute('onmouseover', 
			'javascript: this.className = "hover";'); 
		entry.setAttribute('onmouseout', 
			'javascript: this.className = "";');
	},
	hide_entry:function(number) {
		var p = getElem(this.popup_id);
		var entry = p.childNodes[number];
		entry.style.display = 'none';
	},
	show_entry:function(number) {
		var p = getElem(this.popup_id);
		var entry = p.childNodes[number];
		entry.style.display = 'block';
	},
/*** AB HIER ALLES PRIVATE ***/
	create_menu:function() {
		var new_inner_html = 
	 		'<span id="'+this.title_id+'" style="color: #000000; '+
 			'font-style: italic; border-bottom: 1px dashed #000000;">'+this.title+'</span>';
		var new_popup = document.createElement('div');
		new_popup.id = this.popup_id;
		new_popup.className="PopupMenu";
		if (this.width > 0)
			new_popup.style.width = this.width+'px';
		new_popup.style.position = 'absolute';
		new_popup.style.left = this.position.x+'px';
		new_popup.style.top = this.position.y+'px';
		new_popup.style.visibility = 'hidden';
		new_popup.style.zIndex = 10000;
		new_popup.onclick = 
			(function(_instance) {
				return function(ev) {
					_instance.hide();
				}
			}(this));
		new_popup.innerHTML = new_inner_html;
		
		this.parent_node.appendChild(new_popup);
	}
}

/** Statische Funktionen */
function PopupMenu__hide_all() {
	for (var i = 0; i < PopupMenu__instances.length; i++)
		PopupMenu__instances[i].hide();
}