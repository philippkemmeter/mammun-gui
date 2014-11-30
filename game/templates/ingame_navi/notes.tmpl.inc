<script type="text/javascript">
DocumentLoadListener.add(function(){PureFW.EventUtil.add_event_handler(document.ui_notes_form,"submit",ui_form_submit);});
function ui_form_submit(ev) {
	//try {
		evtemp = PureFW.EventUtil.formatEvent(ev);
	//}
	//catch(e){
	//}
	evtemp.preventDefault();
	ui_notes_save();
}
function ui_notes_keyup(ev) {
	//try {
		evtemp = PureFW.EventUtil.formatEvent(ev);
	//}
	//catch(e){
	//}
	// Tastendruck nicht weitergeben an z.B. die Karte, damit das Tippen
	// keine Auswirkungen auf die Steuerung hat
	evtemp.stopPropagation();

	// Größe anpassen
	var notes_elem = document.getElemById("uiNotesTextArea");
	if (parseInt(notes_elem.scrollHeight) > 196)
		notes_elem.style.height = parseInt(notes_elem.scrollHeight)+"px";
	else
		notes_elem.style.height = "196px";
	UINotes.notes_content.set_inner_height(parseInt(notes_elem.style.height));
}
function ui_notes_keydown(ev) {
	//try {
		evtemp = PureFW.EventUtil.formatEvent(ev);
	//}
	//catch(e){
	//}
	// Tastendruck nicht weitergeben an z.B. die Karte, damit das Tippen
	// keine Auswirkungen auf die Steuerung hat
	evtemp.stopPropagation();

	// Größe anpassen
	var notes_elem = document.getElemById("uiNotesTextArea");
	if (parseInt(notes_elem.scrollHeight) > 196)
		notes_elem.style.height = parseInt(notes_elem.scrollHeight)+"px";
	else
		notes_elem.style.height = "196px";
	UINotes.notes_content.set_inner_height(parseInt(notes_elem.style.height));
}

function ui_notes_save() {
	PureFW.AJAXClientServer.send_request(
			"notes_action.php",
			ui_notes_save_callback,
			form_elems_to_param_string(new Array(UINotes.notes_content.get_text_element()))
	);
}
function ui_notes_save_callback(response_arr) {
	if (response_arr[0] != '1')
		alert('Error. Could not save your Notes.'+response_arr);
}
// Notizen beim Verlassen der Seite in jedem Fall speichern
DocumentUnloadListener.add(ui_notes_save);

UINotes = new Object();
UINotes.notes_cont = new PureFW.Container(
	document.body,
	MammunUI.reference_width-271, 100,
	271,327
);
UINotes.notes_cont.set_bg_img(
	'ui/backgrounds/notes/background.png'
);
UINotes.notes_cont.hide();
UINotes.notes_cont.set_z_index(300);

UINotes.close_button = new PureFW.Image(
	UINotes.notes_cont.get_node(),
	UINotes.notes_cont.width - 30, 5,
	28, 28,
	'ui/elements/form/buttons/btn_no.png');
UINotes.close_button.add_event_handler('click',	UIButtons.toggle_notes);
UINotes.close_button.set_z_index(10);

UINotes.notes_cont.set_content('\
		<form id="SpecialNotesForm" action="" accept-charset="utf-8" name="ui_notes_form"></form>');

UINotes.notes_content = new PureFW.TextArea(
	document.getElemById("SpecialNotesForm"),
	40, 65,
	196, 212
);
UINotes.notes_content.set_content(
	'<?= str_replace(array("\n", "\r"), array('\n', ''), $notes) ?>'
);
UINotes.notes_content.set_name("note");
</script>
