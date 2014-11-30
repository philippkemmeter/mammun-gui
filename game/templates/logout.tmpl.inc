<?
$header_add = '<script type="text/javascript" src="../js/classes/MammunUI.js?'.filemtime('../js/classes/MammunUI.js').'"></script>';
include_once('header.tmpl.inc');
include_once('../include/_config.inc') ?>
<script language="javascript">
<!--
function logout() {
	var _top = MammunUI.get_top_frame();
	if (top != _top) {
		<?/** We guess being in FB. */ ?>
		top.location.href
			= '<?= CONF_FACEBOOK_APP_URL ?>?map_key=<?=@$_SESSION['map_key']
				?>&visit=<?= @$_SESSION['visit'] ?>';

	}
	else {
		_top.location.href = "<?= $redirect_page ?>";
	}
}

window.setTimeout(logout , 200);
-->
</script>
<?php include_once 'footer.tmpl.inc';?>