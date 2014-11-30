<?php
require_once('../include/shared/main_funcs.inc');
?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8"/>
<meta http-equiv="expires" content="0" />
<style type="text/css">@import "../css/admin.css";</style>
<link rel="shortcut icon" href="../pix/fav_icon.ico" />
<title>Mammun - Admin Console</title>
<script type="text/javascript">
function pic_path(pic) {
	return "<?= pic_path('') ?>"+pic;
}
</script>
<script type="text/javascript">
//parent.frames.top_frame.menuButtonDeactivate();
</script>
<?php
if (isset($header_add))
  echo $header_add;
?>
</head>
<?php
/* wenn es schon ein "onload" statement gibt, das per body_add übergeben wurde,
 * muss body_loaded() noch hinzugefügt werden - in das onload statement. Dazu
 * muss dieses ersteinmal gefunden werden, was leider auf viele weise möglich ist...
 */
if ($body_add && (stripos($body_add, 'onload=') !== false)) {
	if (($pos = stripos($body_add, 'onload="javascript:') === false)) {
		if (($pos = stripos($body_add, 'onload=javascript:') === false)) {
			if (($pos = stripos($body_add, 'onload="') === false)) {
 				$pos = stripos($body_add, 'onload=') + 7;
			}
			else
				$pos+=8;
		}
		else
			$pos+=18;
	}
	else
		$pos+=19;
	$t1 = substr($body_add, 0, $pos);
	$t2 = substr($body_add, $pos);
	$body_add = $t1 . /*' body_loaded();' .*/ $t2;
}
else {	// ansonsten isses ganz leicht und man ergänzt das onload statement einfach
//	$body_add .= ' onload="javascript: body_loaded();"';
} ?>
<body <?= $body_add?> id="theBody">
<!-- <div id="loading" class="loading"><?= LNG_LOADING ?>...&nbsp;&nbsp;&nbsp;</div> -->