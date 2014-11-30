confirm_amount<?=$this_id?> = function() {
	var elms = document.forms.choose_amount<?=$this_id?>.elements;
	var unit_array = new Array();
	var j = 0;
	var id_set = false; var count_set = false;
	unit_array[j] = new Object();
	for (var i = 0; i < elms.length; i++) {
		if (id_set && count_set) {
			j++;
			count_set = id_set = false;
		}
		if (elms[i].name.indexOf('count') == 0) {
			if (!unit_array[j])
				unit_array[j] = new Object();
			unit_array[j].count = elms[i].value;
			count_set = true;
		}
		else if (elms[i].name.indexOf('id') == 0) {
			if (!unit_array[j])
				unit_array[j] = new Object();
			unit_array[j].id = elms[i].value;
			id_set = true;
		}
	}
	MammunUI.get_top_frame().MapDataCtrl.Movement.units_move(
		<?= $sector_id ?>, unit_array
	);
}