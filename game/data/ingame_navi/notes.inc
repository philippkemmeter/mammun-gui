<?php
require_once('../include/classes/Notes.inc');
$notes = Notes::get_instance(0)->get_note();

include('templates/ingame_navi/notes.tmpl.inc');
?>