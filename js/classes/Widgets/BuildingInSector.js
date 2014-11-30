/**
 * Repräsentiert ein Gebäude im Sektor. Mit ihm kann entsprechend interagiert
 * werden.
 * 
 * @author Phil
 */

BuildingInSector = function(parent_node, x, y, w, h, pic) {
	if (typeof(parent_node) !== 'undefined') {
		this.init(parent_node, x, y, w, h, pic);

		BuildingInSector.clean_up();
			
		this.instance_num = this.constructor.num_instances;
		this.instance_name = "BuildingInSector.instances["
			+ this.instance_num + "]";
		
		this.id = 'BuildingInSector'+this.instance_num;
		this.bg_img_id = 'BuildingInSectorBg'+this.instance_num;
		this.content_id = 'BuildingInSectorCont'+this.instance_num;
		this.progress_id = 'BuildingInSectorProgress'+this.instance_num;
		
		this.insert_into_dom_tree();
		this.constructor.num_instances++;
		this.constructor.instances[this.instance_num] = this;
		
		PureFW.WidgetManager.manager_all.register_widget(this);
	}
};
/********************************************************************
 * BuildingInSector extends Container
 ********************************************************************/
BuildingInSector.extend(PureFW.Container);

/********************************************************************
 * Statische Deklarationen
 ********************************************************************/
BuildingInSector.num_instances = 0;
BuildingInSector.instances = new Array();

BuildingInSector.clean_up = function() {
	var i = 0;
	while (i < BuildingInSector.num_instances) {
		var b = BuildingInSector.instances[i];
		if (b && !document.getElemById(b.id)) 
		{	// HTML-Objektrepräsentation bereits gelöscht	
			// => Objekt kann gelöscht werden (nur Garbage)
			delete b;
			for (var j = i+1; j < BuildingInSector.num_instances; j++) {
				BuildingInSector.instances[j-1] = BuildingInSector.instances[j];
			}
			BuildingInSector.num_instances--;
		}
		else {
			 i++;
		}
	}
}
/********************************************************************
 * Prototyp-Deklarationen
 ********************************************************************/
/**
 * @see PureFW.Container.prototype#init
 */
BuildingInSector.prototype.init = function(parent_node, x, y, w, h, pic) {
	BuildingInSector.parent.init.call(this, parent_node, x, y, w, h);
	
	this.pic = pic || null;
	this.z_index = 1;
	this.upper_left_corner = new PureFW.Point(0, 0);
	this.neutral = false;
	
	this.progress = this.t_stamp = this.grow = 0;
};

/**
 * @see PureFW.Container.prototype#insert_into_dom_tree
 */
BuildingInSector.prototype.insert_into_dom_tree = function() {
	BuildingInSector.parent.insert_into_dom_tree.call(this);
	
	this.set_bg_img(
		this.pic
	);
	this.set_content(
		'<div id="'+this.progress_id+'" '+
		' style="position: absolute; bottom: 10px; right: 10px; z-index:1">'+
		'</div>'
	);
};

/**
 * Setzt die Daten, die notwendig sind, um den aktuellen Konstruktionsablauf
 * des Gebäudes (es wird also gerade errichtet) zu beschreiben.
 * 
 * @param ufloat progress	Aktueller Baufortschritt
 * @param uint grow			Baugeschwindigkeit
 * @param uint t_stamp		Zeitpunkt des Baubeginns
 */
BuildingInSector.prototype.set_constructing_data = function(progress, grow, 
	t_stamp) 
{
	this.progress = (progress) ? parseFloat(progress) : 0;
	this.grow = (grow) ? parseInt(grow) : 1;
	this.t_stamp = (t_stamp) ? parseInt(t_stamp) : 0;
	this.refresh_progress_display();
};

/**
 * Setzt die Grid-Koordinaten des Gebäudes.
 * 
 * Das Setzen hat keinen Einfluss auf die Position, wo das Gebäude tatsächlich
 * auf dem Bildschirm erscheint, sondern dient nur als zusätzlich beschreibendes
 * Attribut.
 * 
 * @param {0,1,..7} x
 * @param {0,1,..7} y
 */
BuildingInSector.prototype.set_upper_left_corner = function(x, y) {
	this.upper_left_corner.x = x || 0;
	this.upper_left_corner.y = y || 0;
};

/**
 * Setzt, ob das Gebäude ein neutrales ist, oder nicht.
 * 
 * Neutrale Gebäude sind solche, die niemals einem Spieler zugeordnet werden
 * können und somit insbesondere nicht abgerissen werden können. Dazu zählen
 * vorallem natürliche Hindernisse wie Bäume.
 * 
 * @param bool neutral
 */
BuildingInSector.prototype.set_neutral = function(neutral) {
	this.neutral = neutral;
};

/**
 * Gibt den aktuellen Baufortschritt zurück.
 * 
 * @return ufloat
 * @see BuildingInSector.prototype#set_construction_data
 */
BuildingInSector.prototype.get_progress = function() {
	var t_now = PureFW.Time.get_current_server_time();
	var cur_progress = this.progress + ((t_now - this.t_stamp) / this.grow);
	if (cur_progress >= 100)
		this.progress = this.grow = this.t_stamp = cur_progress = 0;
	return (cur_progress < 0) ? 0 : cur_progress;
};

/**
 * Gibt die aktuell gesetzten Grid-Koordinaten zurück.
 * 
 * @return PureFW.Point
 * @see BuildingInSector.prototype#set_upper_left_corner
 */
BuildingInSector.prototype.get_upper_left_corner = function() {
	return this.upper_left_corner;
};

/**
 * Gibt zurück, ob das Gebäude neutral ist.
 * 
 * @return bool
 * @see BuildingInSector.prototype#set_neutral
 */
BuildingInSector.prototype.is_neutral = function() {
	return this.neutral;
};

/**
 * Aktuallisiert die Fortschrittsanzeige des Gebäudes.
 * 
 * Wird diese Funktion niemals aufgerufen, so wird der Fortschritt auch niemals
 * aktuallisiert! Es ist somit ratsam einen Timer einzurichten, der diese
 * Funktion immer wieder aufruft.
 * Das Widget selbst verfügt über keinen solchen Timer.
 */
BuildingInSector.prototype.refresh_progress_display = function() {
	if (!document.getElemById(this.progress_id))
		return;
	
	if (!this.t_stamp)
		document.setElemInnerHTML(this.progress_id, '');
	else if (!isNaN(this.get_progress()))
		document.setElemInnerHTML(this.progress_id, 
			(Math.floor(this.get_progress()*10)/10)+'%');
	else
		document.setElemInnerHTML(this.progress_id, '');
};
