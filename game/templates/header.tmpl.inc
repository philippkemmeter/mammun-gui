<?
require_once(dirname(__FILE__) .'/../../include/shared/main_funcs.inc');
require_once(dirname(__FILE__) .'/../../include/_config.inc');
?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8"/>
<meta http-equiv="expires" content="0" />
<style type="text/css">@import "../css/main2.css?<?=filemtime(dirname(__FILE__).'/../../css/main2.css')?>";</style>
<style type="text/css">@import "../joomla_cms/templates/mammun/css/shared.css?<?=filemtime(dirname(__FILE__).'/../../joomla_cms/templates/mammun/css/shared.css')?>";</style>
<style type="text/css">@import "../css/ui.css?<?=filemtime(dirname(__FILE__).'/../../css/ui.css')?>";</style>
<link rel="shortcut icon" href="../pix/fav_icon.ico" />
<link rel="icon" href="../pix/fav_icon.png" />
<title>Mammun</title>
<script type="text/javascript">
function pic_path(pic) {
	return "<?= pic_path('') ?>"+pic;
}
</script>
<?
if (isset($header_add))
  echo $header_add;
?>
</head>
<?
/* wenn es schon ein "onload" statement gibt, das per body_add übergeben wurde,
 * muss body_loaded() noch hinzugefügt werden - in das onload statement. Dazu
 * muss dieses ersteinmal gefunden werden, was leider auf viele weise möglich ist...
 */
if (isset($body_add) && (stripos($body_add, 'onload=') !== false)) {
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
//	$body_add = ' onload="javascript: body_loaded();"';
	$body_add = '';
} ?>
<body <?= $body_add?> id="theBody">

<?
/**
 * When the game is first loaded, it is checked whether the user is logged in
 * through facebook, (all Facebook data is stored in the Session under
 * $_SESSION['fb']) and if he is, the facebook API and the Facebook Javascript
 * library are loaded. In case the user isn't logged in to facebook after all,
 * (can happen when he connects through facebook, then logs in normally, and
 * disconnects from facebook) he is logged out and sent to the first page.
 */

if(isset($_SESSION['fb']['fb_sig']) || isset($_SESSION['fb']['fb_sig_post'])
	|| isset($_SESSION['fb']['session']))
{
	echo '<script type="text/javascript"
	src="http://static.ak.connect.facebook.com/js/api_lib/v0.4/FeatureLoader.js.php">
	</script>
	<script type="text/javascript">
	clear_full_session = function()
	{
		window.location = "logout.php?";
	}
	FB_RequireFeatures(["CanvasUtil"], function()
	{
		FB.init("'.CONF_FACEBOOK_API_KEY.'","../include/xd_reciever.html",
			{"ifUserNotConnected":clear_full_session});
		FB.CanvasClient.setCanvasHeight(\'502px\'); // not sure if we need this
	});
    </script>';
}

?>
<!-- <div id="loading" class="loading"><?/* echo LNG_LOADING */?>...&nbsp;&nbsp;&nbsp;</div> -->
