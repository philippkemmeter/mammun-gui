/**
 * Grid. Assists in the building process, controlling where buildings already
 * exist and providing a visual grid for reference.
 * 
 * @author Alex, Phil
 * 
 * @param Node parent_node	An welchen Knoten das Grind angehängt werden soll
 * @param int x_position	Wo sich das Grid befinden soll in x-Richtung in Pixeln
 * @param int y_position	Wo sich das Grid befinden soll in y-Richtung in Pixeln
 * @param int dimensions_x	Wie viele Felder das Grid in x-Richtung haben soll
 * @param int dimensions_y	Wie viele Felder das Grid in y-Richtung haben soll
 * @param int field_width	Wie breit ein Feld des Grids sein soll in Pixeln
 * @param int field_height	Wie hoch ein Feld des Grids sein soll in Pixeln
 */
Grid = function(parent_node, x_position, y_position, dimensions_x, 
	dimensions_y, field_width, field_height)
{
	if (typeof(parent_node) !== 'undefined') {
		Grid.Field.prototype.width = (field_width || (136 >> 1));
		Grid.Field.prototype.height = (field_height || (106 >> 1));
	
		this.init(parent_node, x_position, y_position, dimensions_x,
			dimensions_y, field_width, field_height);
		
		this.instance_num = this.constructor.num_instances;
		this.instance_name = "Grid.instances["+this.instance_num+"]";
		
		this.id = "Grid"+this.instance_num;
		this.map_id = 'Grid'+this.instance_num+'_map';
		this.area_id_prefix = "GridArea"+this.instance_num;
		
		this.insert_into_dom_tree();
		this.constructor.num_instances++;
		this.constructor.instances[this.instance_num] = this;
		
		PureFW.WidgetManager.manager_all.register_widget(this);
	}
};
/********************************************************************
 * Grid extends Widget
 ********************************************************************/
Grid.extend(PureFW.Widget);

/********************************************************************
 * Statische Deklarationen
 ********************************************************************/
Grid.num_instances = 0;			// Instanzüberwachung (anzahl)
Grid.instances = new Array();	// Instanzüberwachung (Instanzen)

/********************************************************************
 * Prototyp-Deklarationen
 ********************************************************************/
Grid.prototype.init = function(parent_node, x, y, dim_x, dim_y, field_w, 
	field_h)
{
	this.dimensions_x = (dim_x || 8);
	this.dimensions_y = (dim_y || 8);
	this.field_width = (field_w || (136 >> 1));
	this.field_height = (field_h || (106 >> 1));

	Grid.parent.init.call(this, parent_node, x, y, 
		this.field_width * this.dimensions_x, 
		this.field_height * this.dimensions_y);
	
	this.field_click_functions = new Array();
	this.field_over_functions = new Array();
	
	this.visual_gap = 10; // Number of z-indeces between the grid and its image
	
	// Basically initialize all...
	this.fields = new Array();
	for (x = 0; x < this.dimensions_x; x++) {
		this.fields[x] = new Array();
	}
};

Grid.prototype.insert_into_dom_tree = function() {
	Grid.parent.insert_into_dom_tree.call(this);
	
	var sector_inner_html = '';
	
	sector_inner_html += 
		'<img id="'+this.id+'" border="0"'+
		' src="../pix/pattern/spacer.gif" usemap="#'+this.map_id+'"'+
		' style="position: absolute; top: '+this.position.y+'px;'+
		' left: '+(this.position.x)+'px;'+
		' width:'+(this.width)+'px; height:'+(this.height)+'px; '+
		'z-index: '+(this.z_index+1)+';"/>';
	this.parent_node.innerHTML = this.parent_node.innerHTML + sector_inner_html;
	
	sector_inner_html = '';
	
	sector_inner_html += '<map id="'+this.map_id+'" name="'+this.map_id+'">'
		+ this._generate_map_inner_html() + '</map>';
	
	this.parent_node.innerHTML = this.parent_node.innerHTML + sector_inner_html;
	
	this.grid_background = new PureFW.Image(
		this.parent_node,
		this.position.x, this.position.y,
		this.width, this.height,
		'ui/backgrounds/sector_details/grid.png'
	);
	
	this.grid_background.hide();
};

/**
 * Diese Hilfsfunktion generiert den Inner-HTML-String des Map-Tags neu.
 * 
 * Dieser Inner-HTML-Shit ist notwendig, weil vor allem der Internet-Explorer
 * lauter Bugs erzeugt bei dynamisch generierten Area-Tags als Knoten und z.B.
 * die Area dann einfach gar nicht erzeugt...
 * 
 * @return String
 * @access private
 * @see Grid.prototype#insert_into_dom_tree
 * @see Grid.prototype#repaint
 * 
 */
Grid.prototype._generate_map_inner_html = function() {
	var x_offset = this.get_x() + (this.get_width() >> 1);
	var y_offset = this.get_y();
	/**
	 * Wir speichern hier Hilfswerte um Multiplikationen und Shift-Operationen
	 * zu sparen.
	 */
	var field_width = this.get_field_width();  // Implizite Scale-Multiplikation
	var field_height = this.get_field_height();// Implizite Scale-Multiplikation
	var field_half_height = field_height >> 1;
	var field_half_width = field_width >> 1;

	var x, y;
	var area_string = '';
	for (var j = 0; j < this.dimensions_y; j++)	{
		for (var i = 0; i < this.dimensions_x; i++)	{
			var x = x_offset + (((i-j-1)*field_width) >> 1) - this.get_x();
			var y = y_offset + (((i+j)*field_height) >> 1) - this.get_y();
			
			this.fields[i][j] = new Grid.Field(
				this, i, j, x, y, true
			);
			
			area_string += '<area id="'+this.area_id_prefix+'_'+i+'_'+j+'"';
			
			if (this.get_field(i, j).enabled) {
				area_string += ' href="javascript: ;"';
				area_string += ' onclick="javascript: '+this.instance_name+
					'.on_field_click(event, '+this.instance_name+
												'.get_field('+i+','+j+'));"';
				area_string += ' onmouseover="javascript: '+this.instance_name+
					'.on_field_mouseover(event, '+this.instance_name+
												'.get_field('+i+','+j+')); "';
			}
			else {
				area_string += ' href="javascript:return false;"';
			}
			area_string += ' shape="poly" coords="'+
				(x)+','+(y+field_half_height)+', '+
				(x+field_half_width)+','+(y)+', '+
				(x+field_width)+','+(y+field_half_height)+', '+
				(x+field_half_width)+','+(y+field_height)+'"/>';
		}
	}
	return area_string;
}

/**
 * @see PureFW.Widget#repaint
 */
Grid.prototype.repaint = function() {
	Grid.parent.repaint.call(this);
	
	document.setElemInnerHTML(this.map_id, this._generate_map_inner_html());
}

/**
 * Fügt das angegebene Ereignis hinzu.
 * 
 * Erweitert die Ereignisbehandlung um die Ereignisse "field_click" und
 * "field_mouseover", welche aufgerufen werden, wenn ein Feld des Grids 
 * angeklickt bzw. wenn über ein Feld mit der Maus gefahren wird. 
 * 
 * Bei den so registrierten Field-Events wird auch nicht das Grid als 
 * this-Pointer mitgeliefert, sondern die entsprechende Instanz von Grid.Field. 
 * 
 * @param string type
 * @param Function fn
 * @see PureFW.Widget#add_event_handler
 */
Grid.prototype.add_event_handler = function(type, fn) {
	if (typeof (type) !== 'string')
		throw new Error("Widget::add_event_handler: First argument 'type' " +
				"has to be of type 'string'");
	if (typeof (fn) !== 'function')
		throw new Error("Widget::add_event_handler: Second argument 'fn' " +
				"has to be of type 'function'");
	
	if (type == "field_click") {
		this.field_click_functions.push(fn);
	}
	else if (type == "field_mouseover") {
		this.field_over_functions.push(fn);
	}
	else
		Grid.parent.add_event_handler.call(this, type, fn);
}

/**
 * Entfernt das angegebene Ereignis.
 * 
 * Erweitert die Ereignisbehandlung um die Ereignisse "field_click" und
 * "field_mouseover", welche aufgerufen werden, wenn ein Feld des Grids 
 * angeklickt bzw. wenn über ein Feld mit der Maus gefahren wird. 
 * 
 * Bei den so registrierten Field-Events wird auch nicht das Grid als 
 * this-Pointer mitgeliefert, sondern die entsprechende Instanz von Grid.Field. 
 * 
 * @param string type
 * @param Function fn
 * @see PureFW.Widget#remove_event_handler
 */
Grid.prototype.remove_event_handler = function(type, fn) {
	if (typeof (type) !== 'string')
		throw new Error("Widget::add_event_handler: First argument 'type' " +
				"has to be of type 'string'");
	if (typeof (fn) !== 'function')
		throw new Error("Widget::add_event_handler: Second argument 'fn' " +
				"has to be of type 'function'");
	
	if (type == "field_click") {
		this.field_click_functions.remove(fn);
	}
	else if (type == "field_mouseover") {
		this.field_over_functions.remove(fn);
	}
	else
		Grid.parent.remove_event_handler.call(this, type, fn);
}

/**
 * Entfernt alle Ereignisse des angegebenen Typs.
 * 
 * Erweitert die Ereignisbehandlung um die Ereignisse "field_click" und
 * "field_mouseover", welche aufgerufen werden, wenn ein Feld des Grids 
 * angeklickt bzw. wenn über ein Feld mit der Maus gefahren wird. 
 * 
 * Bei den so registrierten Field-Events wird auch nicht das Grid als 
 * this-Pointer mitgeliefert, sondern die entsprechende Instanz von Grid.Field. 
 * 
 * @param string type
 * @see PureFW.Widget#clear_event_handler
 */
Grid.prototype.clear_event_handler = function(type) {
	if (typeof (type) !== 'string')
		throw new Error("Widget::add_event_handler: First argument 'type' " +
				"has to be of type 'string'");
	
	if (type == "field_click") {
		this.field_click_functions.destroy();
		this.field_click_functions = new Array();
	}
	else if (type == "field_mouseover") {
		this.field_over_functions.destroy();
		this.field_click_functions = new Array();
	}
	else
		Grid.parent.clear_event_handler.call(this, type);
}

/**
 * Löst das Click-Event für das entsprechende Feld aus (wurde zuvor als
 * "field_click"-Event registriert)
 * 
 * @param Event ev
 * @param Grid.Field field
 */
Grid.prototype.on_field_click = function(ev, field) {
	if (!ev)
		ev = this.create_event("click");
	var n = this.field_click_functions.length;
	for (var i = 0; i < n; i++) {
		this.field_click_functions[i].call(field, ev);
	}
}

/**
 * Löst das MouseOver-Event für das entsprechende Feld aus (wurde zuvor als
 * "field_mouseover"-Event registriert)
 * 
 * @param Event ev
 * @param Grid.Field field
 */
Grid.prototype.on_field_mouseover = function(ev, field)
{
	if (!ev)
		ev = this.create_event("mouseover");
	var n = this.field_over_functions.length;
	for(var i = 0; i < n; i++)
	{
		this.field_over_functions[i].call(field, ev);
	}
};

/**
 * Zerstört das Grid und alle seine Felder.
 * @see PureFW.Widget#destroy
 */
Grid.prototype.destroy = function() 
{
	Grid.parent.destroy.call(this);
	this.fields.destroy();
};

/**
 * Setzt den z-Index des Hintergrundbildes, welches das sichtbare Grid 
 * darstellt.
 * 
 * @param uint z
 */
Grid.set_background_z_index = function(z)
{
	this.visual_gap = this.z_index - z;
	this.grid_background.set_z_index(this.z_index + this.visual_gap);
};

/**
 * Gibt das Feld an der angegebenen Koordinate zurück
 * 
 * @param uint x
 * @param unit y
 * @return Grid.Field
 */
Grid.prototype.get_field = function(x, y)
{
	return this.fields[x][y];
};

/**
 * Aktiviert das durch die Koordinaten bestimmte Feld des Grids.
 * 
 * @param uint x
 * @param uint y
 */
Grid.prototype.activate_field = function(x, y)
{
	if (x < 0 || x >= this.dimensions_x || y < 0 || y >= this.dimensions_y)
		return;
	this.get_field(x, y).enable();
//	var elem = getElem('Grid_area'+this.id+'_'+x+'_'+y);
//	if (elem) 
//	{
//		elem.onclick = 'javascript: Grid.instances['+this.id+'].call_mouse_click_functions('+
//			'Grid.instances['+this.id+'].get_field('+i+','+j+'));';
//		elem.onmouseover = 'javascript: Grid.instances['+this.id+'].call_mouse_over_functions('+
//			'Grid.instances['+this.id+'].get_field('+i+','+j+'));';
//	}
};

/**
 * Deaktiviert das durch die Koordinaten bestimmte Feld des Grids.
 * 
 * @param uint x
 * @param uint y
 */
Grid.prototype.deactivate_field = function(x,y) 
{
	if (x < 0 || x >= this.dimensions_x || y < 0 || y >= this.dimensions_y)
		return;
	this.get_field(x, y).disable();
//	var elem = getElem('Grid_area'+this.id+'_'+x+'_'+y);
//	if (elem) 
//	{
//		alert('disabling: Grid_area'+this.id+'_'+x+'_'+y);
//		elem.onclick = "";
//		elem.onmouseover = "";
//	}
};

/**
 * Gibt die Breite in Pixeln eines einzelnen Grid-Feldes aus (Pixelgröße, wie
 * aktuell dargestellt, also inklusive Scale-Faktor).
 * 
 * @return ufloat
 */
Grid.prototype.get_field_width = function() {
	return this.field_width * this.scale_factor;
}

/**
 * Gibt die Höhe in Pixeln eines einzelnen Grid-Feldes aus (Pixelgröße, wie
 * aktuell dargestellt, also inklusive Scale-Faktor).
 * 
 * @return ufloat
 */
Grid.prototype.get_field_height = function() {
	return this.field_height * this.scale_factor;
}

/**
 * Blendet das Hintergrundbild des Grids aus.
 */
Grid.prototype.hide_background = function() {
	if(this.grid_background)
	{
		this.grid_background.hide();
	}
};

/**
 * Zeigt das Hintergrundbild des Grids (wieder) an.
 */
Grid.prototype.show_background = function() {
	if(this.grid_background)
	{
		this.grid_background.show();
	}
};

/**
 * Zeigt das durch seine Koordinaten bestimmte Feld an.
 * 
 * @param uint x
 * @param uint y
 */
Grid.prototype.show_field = function(x, y)
{
	this.get_field(x, y).show();
};

/**
 * Blendet das durch seine Koordinaten bestimmte Feld aus.
 * 
 * @param uint x
 * @param uint y
 */
Grid.prototype.hide_field = function(x, y)
{
	this.get_field(x, y).hide();
};


/**
 * Grid.Field is the visual representation of one field of the grid, which
 * can get colored in order to visualize some status. 
 * 
 * @param x_coordinate	The x position of the Field in Coordinate terms 
 * 							(e.g. (1, 3))
 * @param y_coordinate	The y position of the Field in Coordinate terms
 * @param x_pixel 		The x pixel position of the Field, located absolutely 
 * 							(e.g. (142, 275))
 * @param y_pixel		The y pixel position of the Field, located absolutely
 * @param enabled		Boolean stating whether the Field is enabled
 */
Grid.Field = function(parent_grid, x_coordinate, y_coordinate, x_pixel, 
	y_pixel, enabled)
{
	if (typeof(parent_grid) !== 'undefined') {
		this.init(parent_grid, x_coordinate, y_coordinate, x_pixel, y_pixel,
			enabled);
	
		this.instance_num = this.constructor.num_instances;
		this.instance_name = "Grid.Field.instances["+this.instance_num+"]";
		
		this.id = "Grid_Field"+this.instance_num;
		
		this.insert_into_dom_tree();
	
		this.constructor.num_instances++;
		this.constructor.instances[this.instance_num] = this;
	}
};

/********************************************************************
 * Grid.Field extends Image
 ********************************************************************/
Grid.Field.extend(PureFW.Image);

/********************************************************************
 * Statische Deklarationen
 ********************************************************************/
Grid.Field.num_instances = 0;			// Instanzüberwachung (anzahl)
Grid.Field.instances = new Array();	// Instanzüberwachung (Instanzen)

/********************************************************************
 * Prototyp-Deklarationen
 ********************************************************************/
Grid.Field.prototype.init = function(parent_grid, x_coordinate, y_coordinate, 
	x_pixel, y_pixel, enabled)
{
	Grid.Field.parent.init.call(this,
		parent_grid.parent_node, 
		x_pixel / parent_grid.scale_factor + parent_grid.position.x, 
		y_pixel / parent_grid.scale_factor + parent_grid.position.y,
		parent_grid.field_width,
		parent_grid.field_height
	);
	this.parent_grid = parent_grid;
	this.pixel_origin = new PureFW.Point(x_pixel, y_pixel);
	this.coordinates = new PureFW.Point(x_coordinate, y_coordinate);
	if (enabled)
		this.enable();
	else
		this.disable();
}

Grid.Field.prototype.insert_into_dom_tree = function() {
	Grid.Field.parent.insert_into_dom_tree.call(this);
	
	this.hide();
}

/**
 * Disables the field, prohibiting construction and setting the background to red.
 */
Grid.Field.prototype.disable = function()
{
	this.enabled = false;
	this.set_pic_url( 
		'../pix/ui/backgrounds/sector_details/red.png'
	);
};

/**
 * Enables the field, allowing construction and setting the background to white.
 */
Grid.Field.prototype.enable = function()
{
	this.enabled = true;
	this.set_pic_url( 
		'../pix/ui/backgrounds/sector_details/white.png'
	);
};

/**
 * Returns the pixel position of the Field relative to the grid origin.
 * 
 * @return PureFW.Point
 */
Grid.Field.prototype.get_position = function()
{
	return this.pixel_origin;
};

/**
 * Returns the Grid coordinates of the Field.
 * 
 * @return PureFW.Point
 */
Grid.Field.prototype.get_coordinates= function()
{
	return this.coordinates;
};

/**
 * Returns whether the Field is active.
 */
Grid.Field.prototype.is_active = function()
{
	return this.enabled;
};