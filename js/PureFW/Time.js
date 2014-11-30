/**
 * Diese Klasse verwaltet Zeiten. Gespeichert sind Server- und Client-Zeit, 
 * sowie Ausgabefunktionen zum Formatieren von Unix-Zeitstempeln.
 *
 * @author Phil
 */
PureFW.Time = new Object();	// Statisch
PureFW.Time.LNG_DAY = 'day';
PureFW.Time.LNG_DAYS = 'days';

var jetzt = new Date();
PureFW.Time.init_client_time = Math.floor(jetzt.getTime()/1000);

/**
 * Setzt die Server-Zeit, welche beim Script-Start "jetzt" war.
 * 
 * @param UNIX_TIMESTAMP time
 */
PureFW.Time.set_init_server_time = function(time) {
	PureFW.Time.init_server_time = time;
}

/**
 * Setzt die Client-Zeit, welche beim Script-Start "jetzt" war.
 * 
 * @param UNIX_TIMESTAMP time
 */
PureFW.Time.set_init_client_time = function(time) {
	PureFW.Time.init_client_time = time;
}

/**
 * Gibt die aktuelle Client-Zeit zurück.
 * 
 * @return UNIX_TIMESTAMP
 */
PureFW.Time.get_current_client_time = function() {
	var jetzt = new Date();
	return Math.floor(jetzt.getTime()/1000);
}


/**
 * Gibt zurück, welche Zeit jetzt auf dem Server sein müsste, wenn die initialen
 * Zeiten korrekt gesetzt waren.
 * 
 * @return UNIX_TIMESTAMP
 */
PureFW.Time.get_current_server_time = function() {
	return PureFW.Time.get_current_client_time() 
		+ PureFW.Time.get_client_server_diff();
	
}

/**
 * Gibt die Differenz von Server- und Client-Zeit zurück. Ist eine der Zeiten
 * nicht gesetzt, wird 0 zurückgegeben.
 * 
 * @return uint
 */
PureFW.Time.get_client_server_diff = function() {
	return PureFW.Time.init_server_time - PureFW.Time.init_client_time;
}

/**
 * Formatiert den Unix-Timestamp in Sekunden in eine lesbare Form.
 * 
 * @param UNIX_TIMESTAMP time_left
 * @param bool show_seconds				[default: true]
 * @param bool leading_zero_hour		[default: true]
 */
PureFW.Time.sec_in_time = function(time_left, show_seconds, leading_zero_hour) 
{
 	var days_left = 0;
 	var hours_left = time_left;

	if (typeof(show_seconds) == 'undefined')
		show_seconds = true;
		
	if (typeof(leading_zero_hour) == 'undefined')
		leading_zero_hour = true;
	
	while(hours_left>=86400)
	{
		hours_left = hours_left - 86400;
		days_left = days_left+1; 
	}
	
	curdate = new Date(0);
	var tzo = curdate.getTimezoneOffset();
	var t = (hours_left+tzo*60)*1000;
	date = new Date(t);

	if (show_seconds) {
		if (leading_zero_hour) {
			ret= date.formatDate('H:i:s');
		}
		else {
			ret = date.formatDate('G:i:s');
		}
	}
	else {
		if (leading_zero_hour) {
			ret = date.formatDate("H:i");
		}
		else {
			ret = date.formatDate("G:i");
		}
	}
	
	if (days_left != 0) {
		if (days_left == 1)
			return (days_left+" "+PureFW.Time.LNG_DAY+", "+ret);
		else
			return (days_left+" "+PureFW.Time.LNG_DAYS+", "+ret);
	}
	else {
		return ret;
	}
}