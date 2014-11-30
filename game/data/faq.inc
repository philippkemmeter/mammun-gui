<?php
/**
 * Diese Datei zeigt Forschungsstatusse und Dialoge an. Per AJAX geladen.
 */

require_once ('../include/std_include.inc');
if (isset($_GET['ajaxid'])) {// Ein ping-und-echo-Parameter für AJAX-Request-Zuweisungen
	$AJAX_OUTPUT = true;
	$r = new ClientServerResponse();
	$r->add_value($_GET['ajaxid']);
	$this_win_id = $_GET['ajaxid'];
}
ob_start();
Language::get_instance()->load_language_file('faq');
$current_user = User::get_current_user();
$expected_data = array();

$GP = $cryptObj->load_GET_POST($expected_data);

include('templates/faq.tmpl.inc');
if ($AJAX_OUTPUT) {
	$r->add_value(ob_get_clean());
	echo $r;
}
else
	ob_flush();
?>