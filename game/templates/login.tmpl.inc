<?php
/**
 * Generiert uns eine schicke Loginseite.
 *
 * @author Robert Kemmeter
 *
 * TODO Header auslagern.
 */
?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8"/>
<meta http-equiv="expires" content="0" />
<style type="text/css">@import "../css/main2.css";</style>
<style type="text/css">@import "../css/ui.css";</style>
<style type="text/css">@import "../css/login.css?2";</style>
<script type="text/javascript" src="../js/classes/MammunUI.js?'.filemtime('../js/classes/MammunUI.js').'"></script>
<script type="text/javascript" src="../js/PureFW/pure_fw_all.js?<?=filemtime(dirname(__FILE__).'/../../js/PureFW/pure_fw_all.js')?>"></script>
<script type="text/javascript" src="../js/gen_funcs.js?<?=filemtime(dirname(__FILE__).'/../../js/gen_funcs.js')?>"></script>
<script type="text/javascript" src="../js/classes/DocumentLoadListener.js?<?=filemtime(dirname(__FILE__).'/../../js/classes/DocumentLoadListener.js')?>"></script>
<link rel="shortcut icon" href="../pix/fav_icon.ico" />
<link rel="icon" href="../pix/fav_icon.png" />
<title>Mammun</title>
</head>
<body onload="DocumentLoadListener.call_listeners()">
<?php
include('login_box.tmpl.inc');
include('footer.tmpl.inc');
?>