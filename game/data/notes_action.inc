<?php
/**
 * File created by Philipp Kemmeter on 11.02.2009 at 11:23:20
 */

include_once ('../include/std_include.inc');
require_once ('../include/classes/Notes.inc');

$expected_data = array (
	'note'		=>	'string'
);
$GP = $cryptObj->load_GET_POST($expected_data);
$r = new ClientServerResponse();
if (isset($GP['note'])) {
	Notes::get_instance(0)->set_note($GP['note']);
	$r->add_value(1);
	die($r);
}
$r->add_value(0);
die($r);
?>