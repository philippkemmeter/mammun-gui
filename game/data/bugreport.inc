<?php
/**
 * Diese Datei zeigt Bugreportseite und Dialoge an. Per AJAX geladen.
 */
include ('../include/std_include.inc');
$r = new ClientServerResponse();
if (isset($_GET['ajaxid'])) {// Ein ping-und-echo-Parameter für AJAX-Request-Zuweisungen
	$AJAX_OUTPUT = true;
	$r->add_value($_GET['ajaxid']);
	$this_win_id = $_GET['ajaxid'];
}
else
	$AJAX_OUTPUT = false;
ob_start();

include_once ('../include/classes/Bugreport.inc');

Language::get_instance()->load_language_file("bugreport");

$expected_data = array 
(
	'user_id'		=>	'seint',
	'ajaxwinid' 	=> 	'int',
	'browser'		=>	'string',	
	'os'			=>	'string',
	'resolution'	=>	'string',
	'window_size'	=>	'string',
	'source'		=>	'int',
	'repro'			=>	'string',
	'bug'			=>	'string',
	'this_id'		=>	'int',
	'del_attach'	=>	'string'
);
$GP = $cryptObj->load_GET_POST($expected_data);

if (isset($_FILES['upload']['name'])) {
	//MUTEX: Gleichzeitiger Bildupload führt zu konflikten, da nur eine 
	// Temp-Datei gleichzeitig existieren kann
	$sem_id = sem_get(MUTEX_BUG_PIC_UPLOAD, 1, 0600, -1);
	sem_acquire($sem_id);
	
	$dest = CONF_UPLD_TMP_DIR . '/tmp';
	move_uploaded_file($_FILES['upload']['tmp_name'], $dest);

	if (exif_imagetype($dest) !== IMAGETYPE_JPEG)
	{
		$return_string = LNG_NO_JPG_FILE;
		$success = 0;
		$this_id = $GP['this_id'];
		include('templates/bugreport/bugreport_file_upload_result.tmpl.inc');
	}
	else {
		$instr = fopen($dest,"rb");
		$image = addslashes(fread($instr,filesize($dest)));
		fclose($instr);

		if (strlen($image) > 150000) {
			$return_string = LNG_IMG_TOO_BIG;
			$success = 0;
			$this_id = $GP['this_id'];
			include('templates/bugreport/bugreport_file_upload_result.tmpl.inc');
		}
		else {
			if (!Bugreport::add_attachment(0, $image, $_FILES['upload']['name'])) {
				$return_string = LNG_SAME_NAME;
				$success = 0;
				$this_id = $GP['this_id'];
				include('templates/bugreport/bugreport_file_upload_result.tmpl.inc');
			}
			else {
				$return_string = $_FILES['upload']['name'];
				$param2 = strlen($image);
				$success = 1;
				$this_id = $GP['this_id'];
				include('templates/bugreport/bugreport_file_upload_result.tmpl.inc');
			}
		}
	}
	// MUTEX
	sem_release($sem_id);
	die();
}

if (isset($GP['del_attach'])) {
	$r = new ClientServerResponse();
	$r->add_value((int)Bugreport::remove_current_attachment($GP['del_attach']));
	$r->add_value($GP['del_attach']);
	die($r);
}

if (isset($GP['bug'])) {
	$result = Bugreport::insert_new_report($GP['user_id'],
		$GP['browser'], $GP['os'],
		$GP['resolution'], $GP['window_size'], $GP['source'], $GP['repro'],
		$GP['bug']);
	$r = new ClientServerResponse();
	$r->add_value((int)$result);
	$r->add_value(LNG_BUG_REPORT_ADDED);
	die($r);
}

include('templates/bugreport/bugreport.tmpl.inc');
if ($AJAX_OUTPUT) {
	$r->add_value(ob_get_clean());
	echo $r;
}
else
	ob_flush();
?>
