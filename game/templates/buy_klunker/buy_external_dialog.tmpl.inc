BuyKlunker.ExternalDialog = new Object();

BuyKlunker.ExternalDialog.open_external_window = function(url, w, h) {
	BuyKlunker.ExternalDialog.external_window = window.open(
		url, "external_window",
		"width="+w+",height="+h+",copyhistory=no"
	);
}

BuyKlunker.ExternalDialog.open_external_window_wrapped = function(url, w, h) {
	BuyKlunker.ExternalDialog.external_window = window.open(
		"url_wrapper.php?url="+encodeURI(url), "external_window",
		"width="+w+",height="+h+",copyhistory=no"
	);
}


BuyKlunker.ExternalDialog.destroy = function() {
	for (var x in BuyKlunker.ExternalDialog) {
		<?/* Nicht das aktuelle Fenster zerstören. Das zerstört sich entweder
		   * selbst, oder es möchte nicht zerstört werden - ganz einfach :)
		   * Wir haben hier nicht die Berechtigung dort einzugreifen!*/?>
		if (x == 'this_window')
			continue;
		try {
			BuyKlunker.ExternalDialog[x].destroy();
		}
		catch (e) {
			delete BuyKlunker.ExternalDialog[x];
		}
	}
	BuyKlunker.ExternalDialog.grey_area.destroy();
	delete BuyKlunker.ExternalDialog;
}

BuyKlunker.ExternalDialog.confirm_box = MammunUI.create_conf_box(
	600, 250,
	PureFW.ConfirmationBox.NO
);
BuyKlunker.ExternalDialog.confirm_box.set_text_align('center');


BuyKlunker.ExternalDialog.feedback_container = new PureFW.Container(
	BuyKlunker.ExternalDialog.confirm_box,
	20, 50,
	BuyKlunker.ExternalDialog.confirm_box.width-40,
	0
);