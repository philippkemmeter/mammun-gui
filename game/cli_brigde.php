<?
unset($argv[0]);
$target = $argv[1];
unset($argv[1]);
foreach ($argv as $value) {
	list($key, $value) = explode('=', $value);
	$_GET[$key] = $value;
}
print_r($_GET);

include($target);
?>
