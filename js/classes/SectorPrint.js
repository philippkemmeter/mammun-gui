/**
 * @author Phil
 */
/** Statische Variable */
/*READONLY*/ var SectorPrint__instance = null;


/**
 *
 * @param int x_offset	Wo sich der Sektor befinden soll in x-Richtung in Pixeln
 * @param int y_offset	Wo sich der Sektor befinden soll in y-Richtung in Pixeln
 * @param int img_w		Die Breite des Sektorbildes
 * @param int img_h		Die Höhe des Sektorbildes
 * @param float img2b_div_factor	Faktor, wie groß die Gebäude gezeichnet
 *									werden sollen im Verhältnis zum Sektor. Bei
 *									einem Wert von 1 wird der ganze Sektorraum
 *									zur Darstellung der Gebäude genutzt, bei
 *									z.B. 0.5 nur die Hälfte.
 */
var SectorPrint = function(x_offset, y_offset, img_w, img_h, img2b_div_factor,
	parent_node) 
{
	if (SectorPrint__instance)
		delete SectorPrint__instance;
		
	this.grid_nx = 8;
	this.grid_ny = 8;
		
	this.img_w = img_w ? img_w : 136 << 2;
	this.img_h = img_h ? img_h : 106 << 2;
	this.img_w_2 = this.img_w >> 1;
	this.img_h_2 = this.img_h >> 1;
	this.b_div_w = img2b_div_factor ? img_w*img2b_div_factor : img_w;//117,3;
	this.b_div_h = img2b_div_factor ? img_h*img2b_div_factor : img_h;//90,1;
	this.b_div_w_2 = this.b_div_w >> 1;
	this.b_div_h_2 = this.b_div_h >> 1;
	this.b_field_w = this.b_div_w_2 >> 2;
	this.b_field_h = this.b_div_h_2 >> 2;
	this.parent_node = parent_node ? parent_node : document.body;

	this.x_offset = this.img_w_2 - this.b_div_w_2;	//Zentrierung
	this.y_offset = this.img_h_2 - this.b_div_h_2;
	if (x_offset)
		this.x_offset += x_offset;
	if (y_offset)
		this.y_offset += y_offset;
		
	this.z_index = 11;
	
	this.buildings_array = null;
	this.building_objects = new Array();
	this.on_building_click = '';
	this.on_building_hover = '';
	this.last_hit_building_index = 0;
	
	this.grid = null;
	
//	this.grid = new Grid(
//			this.x_offset,
//			this.y_offset,
//			this.b_field_w,
//			this.b_field_h,
//			this.grid_nx,
//			this.grid_ny,
//			this.parent_node
//	);
//	
//	this.grid_vis = new Grid(
//			this.x_offset,
//			this.y_offset,
//			this.b_field_w,
//			this.b_field_h,
//			this.grid_nx,
//			this.grid_ny,
//			this.parent_node
//	);
	
//	this.grid_vis = new Grid(
//			this.parent_node,
//			this.x_offset,
//			this.y_offset,
//			this.grid_nx,
//			this.grid_ny,
//			this.b_field_w,
//			this.b_field_h
//	);
	SectorPrint__instance = this;
};

SectorPrint.prototype = {
	set_buildings:function(buildings) {
		this.buildings_array = buildings;
		this._recreate_buildings();
	},
	set_on_building_click:function(str) {
		this.on_building_click = str;
	},
	set_on_building_hover:function(str) {
		this.on_building_hover = str;
	},
	bring_grid_to_front:function() 
	{
		if (this.grid) {
			this.grid.set_z_index(this.z_index+10);
			this.grid.show_background();
		}
	},
	bring_grid_to_back:function() 
	{
		if (this.grid) {
			this.grid.hide_background();
			this.grid.set_z_index(this.z_index-10);
		}
	},
	get_grid_field_width:function() {
		return this.b_field_w;
	},
	get_grid_field_height:function() {
		return this.b_field_h;
	},
//	set_on_field_over:function(fn) {
//		this.grid.set_on_field_over(fn);
//	},
//	set_on_field_click:function(fn) {
//		this.grid.set_on_field_click(fn);
//	},
	show_grid_field:function(x,y)
	{
		this.grid.show_field(x,y);
	},
	hide_grid_field:function(x,y)
	{
		this.grid.hide_field(x,y);
	},
	set_on_field_over:function(fn) 
	{
		this.grid.add_event_handler("field_mouseover", fn);
	},
	set_on_field_click:function(fn) 
	{
		this.grid.add_event_handler("field_click", fn);
	},
	clear_on_field_over:function() 
	{
		this.grid.clear_event_handler("field_mouseover");
	},
	clear_on_field_click:function() 
	{
		this.grid.clear_event_handler("field_click");
	},
	get_grid_offset_x:function() {
		return this.x_offset;
	},
	get_grid_offset_y:function() {
		return this.y_offset;
	},
//	activate_grid_field:function(x,y) {
//		this.grid.activate_field(x,y);
//	},
//	deactivate_grid_field:function(x,y) {
//		this.grid.deactivate_field(x,y);
//	},
	activate_grid_field:function(x, y) 
	{
		this.grid.activate_field(x, y);
	},
	deactivate_grid_field:function(x,y) 
	{
		this.grid.deactivate_field(x, y);
	},
//	get_grid_field_active:function(x,y) {
//		return this.grid.is_field_active(x,y);
//	},
	get_grid_field:function(x,y)
	{
		return this.grid.get_field(x,y);
	},
	get_grid_field_active:function(x,y) 
	{
		return this.grid.get_field(x,y).enabled;
	},
	get_grid_width:function() {
		return this.grid_nx;
	},
	get_grid_height:function() {
		return this.grid_ny;
	},
	get_grid_field_position:function(x,y) 
	{
		return this.grid.get_field(x,y).get_position();
	},
	get_buildings:function() {
		return this.building_objects;
	},
	get_not_neutral_buildings:function() {
		var n = this.building_objects.length;
		var result = new Array();
		for (var i = 0; i < n; i++) {
			if (!this.building_objects[i].is_neutral())
				result.push(this.building_objects[i]);
		}
		return result;
	},
	get_neutral_buildings:function() {
		var n = this.building_objects.length;
		var result = new Array();
		for (var i = 0; i < n; i++)
			if (this.building_objects[i].is_neutral())
				result.push(this.building_objects[i]);
		return result;
	},
	get_last_hit_building:function() {
		return this.building_objects[this.last_hit_building_index];
	},
	refresh_building_progress:function() {
		for (var j = 0; j < this.building_objects.length; j++)
			if (this.building_objects[j])
				this.building_objects[j].refresh_progress_display();
	},
	destroy:function() {
		this.grid.destroy();
		try
		{
			destroy_object(SectorPrint__instance);
		}
		catch(e)
		{
			alert("Trouble @ destroying SectorPrint");
		}
		var nodes = this.parent_node.childNodes;
		var node = null;
		for (i = 0; i < nodes.length; i++) 
		{
			if (nodes[i].id == this.win_id) 
			{
				node = nodes[i];
				break;
			}
		}
		if (node)
		{
			this.parent_node.removeChild(node);
		}
		delete this;
	},
	print_2_6:function() 
	{
		var new_div = document.createElement('div');
		new_div.id = "SectorPrint_Buildings";
		new_div.style.zIndex = 3;
		this.parent_node.appendChild(new_div);
		
		this._recreate_buildings();
		
		
		/** Gebäude-Raster erstellen **/
		this.grid = new Grid(
			this.parent_node,
			this.x_offset,
			this.y_offset,
			this.grid_nx,
			this.grid_ny,
			this.b_field_w,
			this.b_field_h
		);
	},
	/**** AB HIER ALLES PRIVATE ***/
	_set_last_hit_building_index:function(index) {
		this.last_hit_building_index = index;
	},
	_recreate_buildings:function() {
		/** Gebäude erstellen **/
		if (this.buildings_array) {
			var building_count = this.buildings_array.length;
			for (var i = 0; i < building_count; i++) 
			{
				if (this.building_objects[i])
				{
					this.building_objects[i].destroy();
					delete this.building_objects[i];
				}
			}
			this.buildings_objects = new Array();
			document.setElemInnerHTML("SectorPrint_Buildings", '');
			var x_pos_0 = this.x_offset + this.b_div_w_2;
			var y_pos_0 = this.y_offset;
			var area_string = '';
			var min_y = Infinity;
			for (var j = 0; j < building_count; j++) {
				var name = this.buildings_array[j][0];
				var upper_left_corner = this.buildings_array[j][1];
				var pic = pic_path('map/buildings/'+this.buildings_array[j][2]);
				var size = this.buildings_array[j][3];
				var progress = this.buildings_array[j][4][0];
				var grow = this.buildings_array[j][4][1];
				var t_stamp = this.buildings_array[j][4][2];
				var neutral = Boolean(this.buildings_array[j][5]);
				var x = x_pos_0 + (((upper_left_corner[0]-upper_left_corner[1]-size[0])*this.b_field_w) >> 1);
				var y = y_pos_0 + (((upper_left_corner[0]+upper_left_corner[1])*this.b_field_h) >> 1) 
										- size[0]*(size[1]-1)*this.b_field_h;

				if (y < min_y)
					min_y = y;
				var bw = size[0]*this.b_field_w;	// 2D-projizierte Breite des Gebäudes in px
				var bh = size[0]*size[1]*this.b_field_h; // 2D-projizierte Höhe des Geb. in px
				var bbaseh = size[0]*this.b_field_h;	// 2D-projizierte Höhe der Grundfläche des Gebäudes in px
				
	
				this.building_objects[j] = new BuildingInSector(
					document.getElemById("SectorPrint_Buildings"),
					x, y, 
					bw,	bh,
					pic
				);
				this.building_objects[j].set_z_index(this.z_index+1);
				this.building_objects[j].set_upper_left_corner(
					upper_left_corner[0], upper_left_corner[1]);
				this.building_objects[j].set_tooltip(name);
				this.building_objects[j].set_constructing_data(progress, 
					grow, t_stamp);
				this.building_objects[j].set_neutral(neutral);
				var onclick = 'javascript: SectorPrint__instance._set_last_hit_building_index('+j+'); '+this.on_building_click;
				x-=this.x_offset;
				y-=min_y;	// ImgMap muss von min_y bis ganz nach unten alles umfassen
				/**
				 * Bei überlappenden Areas wird die ERSTE nach vorne geholt
				 * (also im Gegenteil zur Renderreihenfolge)
				 * Deswegen wird die neuste Area vor die andern geschrieben
				 */
				area_string = '<area id="area_'+this.building_objects[j].id+'"'+
					' href="javascript: ;"'+
					' onclick="'+onclick+'"'+
					' shape="poly" coords="'+
					x + ', ' + (y+(bbaseh >> 1)) + ', ' +
					(x+(bw >> 1)) + ', ' + y + ', ' +
					(x+bw) + ', ' + (y+(bbaseh >> 1)) + ', '+
					(x+bw) + ', ' + (y+(bh - (bbaseh >> 1))) + ', '+
					(x+(bw >> 1)) + ', ' + (y+bh) + ', '+
					x+ ', ' + (y+(bh - (bbaseh >> 1))) + '"/>' + area_string;
				
				//alert(upper_left_corner[0] + "-" + size[0]);
	
				for (var b_x = upper_left_corner[0]; b_x < size[0]+upper_left_corner[0]; b_x++) {
					for (var b_y = upper_left_corner[1]; b_y < size[0]+upper_left_corner[1]; b_y++) {
						this.grid.deactivate_field(b_x, b_y);
					}
				}
			}
			/** Gebäude-Clickmap erstellen */
			var inner_html = '<map id="sectorprintbuildingsmap" name="SectorPrintBuildingsMap">'+
				area_string + '</map>' +
				'<img src="../pix/pattern/spacer.gif" width="'+(this.b_field_w*this.grid_nx)+'"' +
				' height="'+(this.b_field_h*this.grid_ny+(y_pos_0-min_y))+'"'+
				' style="position:absolute; left: '+this.x_offset+'px;'+
				' top: '+(min_y)+'px; z-index:'+(this.z_index+1)+';"'+
				' usemap="#SectorPrintBuildingsMap" border="0"'+
				' id="sectorprintbuildingsimg"/>\n';
			
			document.setElemInnerHTML("SectorPrint_Buildings", 
				document.getElemById("SectorPrint_Buildings").innerHTML
				+ inner_html);
		}
	}
};