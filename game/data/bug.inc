<?
require_once ('../include/std_include.inc');

$current_user = User::get_current_user();

$allowed_nicks = array ('david', 'matthias', 'robert', 'philipp');

if (!in_array(strtolower($current_user->get_nick()), $allowed_nicks))
	die("Access denied");

require_once ('../include/classes/Bugreport.inc');
$expected_data = array 
(
	'id'			=>	'int',
	'status'	 	=> 	'int',
	'fixcomment'	=>	'string',
	'jointId'		=>	'int',
	'priority'		=>	'int',
	'bugCategory'	=>	'int',
	'filter_status'	=>	'int',
	'filter_priority'=>	'int',
	'filter_cat_id'	=>	'int',
	'attachment'	=>	'int'
);

$GP = $cryptObj->load_GET_POST($expected_data);

if (isset($GP['status']) && isset($GP['id'])) {
	Bugreport::set_state($GP['id'], $GP['status'], $GP['fixcomment']);
}
if (isset($GP['priority']) && isset($GP['id'])) {
	Bugreport::set_priority($GP['id'], $GP['priority']);
}
if (isset($GP['bugCategory']) && isset($GP['id'])) {
	Bugreport::set_cat_id($GP['id'], $GP['bugCategory']);
}
if (isset( $GP[ 'id' ] ) && isset( $GP[ 'jointId' ] ) ) {
	Bugreport::join_bugs( $GP[ 'jointId' ], $GP[ 'id' ] );
}

if (isset($GP['attachment']) && isset($GP['id'])) {
	$attachment = Bugreport::get_attachment($GP['id'], $GP['attachment']);
	if (substr($attachment, 6, 4) === "JFIF")
		header("Content-type: image/jpeg");	
	echo $attachment;
	die();
}
else if (isset($GP['id'])) {
	$bug = Bugreport::get_report($GP['id']);
	include ('templates/bugreport/bug.tmpl.inc');
	die();
}
else {
	$bugreports = Bugreport::get_reports(false, false);
	$bugamounts = array();
	foreach($bugreports as $key => $report) {
		$bugamounts[$report->status]++;
		if (isset($GP['filter_status'])) {
			if ($report->status != $GP['filter_status'])
				unset($bugreports->$key);
		}
		if ($GP['filter_cat_id']) {
			if ($report->cat_id != $GP['filter_cat_id'])
				unset($bugreports->$key);
		}
		if ($GP['filter_priority']) {
			if ($report->priority != $GP['filter_priority'])
				unset($bugreports->$key);
		}
	}
	if (!$bugreports)
		$bugreports = array();
	include ('templates/bugreport/bug_list.tmpl.inc');
	die();
}
?>
