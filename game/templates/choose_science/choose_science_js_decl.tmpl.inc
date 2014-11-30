
var selected_sciences<?=$this_id?> = null;
var selectable_sciences<?=$this_id?> = null;

create_selectable_sciences<?=$this_id?> = function(json_str) {
	selectable_sciences<?=$this_id?> = eval('('+json_str+')');
	document.setElemInnerHTML('science_choose_board_general', '');
	document.setElemInnerHTML('science_choose_board_races', '');
	document.setElemInnerHTML('science_choose_board_abilities', '');
	document.setElemInnerHTML('science_choose_board_buildings', '');
	document.setElemInnerHTML('science_choose_board_artifacts', '');
	var no_selectable_sciences = true;
	if (selectable_sciences<?=$this_id?>.general) {
		for (var i = 0; i < selectable_sciences<?=$this_id?>.general.length; i++) {
			new Icon(
				'',
				48, 48,
				'select_science<?=$this_id?>("'+selectable_sciences<?=$this_id?>.general[i].id+'");',
				'',
				selectable_sciences<?=$this_id?>.general[i].name,
				getElem('science_choose_board_general')
			);
			no_selectable_sciences = false;
		}
	}
	if (selectable_sciences<?=$this_id?>.races) {
		for (var i = 0; i < selectable_sciences<?=$this_id?>.races.length; i++) {
			new Icon(
				'',
				48, 48,
				'select_science<?=$this_id?>("'+selectable_sciences<?=$this_id?>.races[i].id+'");',
				'',
				selectable_sciences<?=$this_id?>.races[i].name,
				getElem('science_choose_board_races')
			);
			no_selectable_sciences = false;
		}
	}
	if (selectable_sciences<?=$this_id?>.abilities) {
		for (var i = 0; i < selectable_sciences<?=$this_id?>.abilities.length; i++) {
			new Icon(
				'',
				48, 48,
				'select_science<?=$this_id?>("'+selectable_sciences<?=$this_id?>.abilities[i].id+'");',
				'',
				selectable_sciences<?=$this_id?>.abilities[i].name,
				getElem('science_choose_board_abilities')
			);
			no_selectable_sciences = false;
		}
	}
	if (selectable_sciences<?=$this_id?>.buildings) {
		for (var i = 0; i < selectable_sciences<?=$this_id?>.buildings.length; i++) {
			new Icon(
				'',
				48, 48,
				'select_science<?=$this_id?>("'+selectable_sciences<?=$this_id?>.buildings[i].id+'");',
				'',
				selectable_sciences<?=$this_id?>.buildings[i].name,
				getElem('science_choose_board_buildings')
			);
			no_selectable_sciences = false;
		}
	}
	if (selectable_sciences<?=$this_id?>.artifacts) {
		for (var i = 0; i < selectable_sciences<?=$this_id?>.artifacts.length; i++) {
			new Icon(
				'',
				48, 48,
				'select_science<?=$this_id?>("'+selectable_sciences<?=$this_id?>.artifacts[i].id+'");',
				'',
				selectable_sciences<?=$this_id?>.artifacts[i].name,
				getElem('science_choose_board_artifacts')
			);
			no_selectable_sciences = false;
		}
	}
	if (no_selectable_sciences)
		getElemStyle('science_choose_okay').visibility = 'visible';
}
create_selected_sciences<?=$this_id?> = function(json_str) {
	selected_sciences<?=$this_id?> = eval('('+json_str+')');
	document.setElemInnerHTML('sciences_chosen', '');
	for (var i = 0; i < selected_sciences<?=$this_id?>.length; i++) {
		new Icon(
			'',
			48, 48,
			'javascript:;',
			'',
			selected_sciences<?=$this_id?>[i].name,
			getElem('sciences_chosen')
		);
	}
}


select_science<?=$this_id?> = function(sc_id) {
	PureFW.AJAXClientServer.send_request('choose_science.php?sc_id='+sc_id,
		select_science_fetch_result<?=$this_id?>);
};

select_science_fetch_result<?=$this_id?> = function(response_arr) {
	if (response_arr[0] == "1") {	// success
		document.setElemInnerHTML("scs_left",response_arr[1]);
		create_selected_sciences<?=$this_id?>(response_arr[2]);
		create_selectable_sciences<?=$this_id?>(response_arr[3]);
		var scs_left = parseInt(response_arr[1]);
		if (response_arr[4])
			officer.set_text(response_arr[4]);

		if (!scs_left) {
			getElemStyle('science_choose_okay').visibility = 'visible';
		}
	}
	else if (response_arr[0] == "!") {	// error
		officer.set_text(response_arr[1]);
	}
	else {	// wahrscheinlich(!) eine HTML-Seite - z.B. Formular zum neueinloggen
		document.write(response_text);
	}
};

finish<?=$this_id?> = function() {
	PureFW.AJAXClientServer.send_request('choose_science.php?dataX=<?= $cryptObj->do_encryption(array('finish' => true)) ?>',
		finish_fetch_result<?=$this_id?>);
};

finish_fetch_result<?=$this_id?> = function(response_arr) {
	if (response_arr[0] == "1") {	// success
		alert("1");
		nonmap_ui_next();
	}
	else { // wahrscheinlich(!) eine HTML-Seite - z.B. Formular zum neueinloggen
		document.write(response_text);
	}
};