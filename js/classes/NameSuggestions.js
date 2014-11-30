/**
 * Diese Klasse schlägt Namen vor :)
 * 
 * @auther Phil
 */

NameSuggestions = new Object();	// rein statisch

NameSuggestions.names = {
	'm': [],
	'f': []
}


NameSuggestions.get_name = function(female) {
	var gender = (female) ? 'f' : 'm';
	return NameSuggestions.names[gender].random();
}