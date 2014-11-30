<?php
include_once('include/shared/classes/Language.inc');

Language::get_instance()->load_language_file('fb_index');
$mammun_lng = Language::get_instance()->get_language();
$language = ($mammun_lng == 'de') ? 'de-DE' : 'en-GB';
$direction = 'ltr';

$request = array_merge($_POST, $_GET);

foreach ($request as $key => $value)
	$str .= "&$key=$value";

?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="<?php echo $language; ?>" lang="<?php echo $language; ?>" dir="<?php echo $direction; ?>" >
<head>
	<title><?= LNG_FB_SHARE_TITLE_GENERAL ?></title>
	<link rel="shortcut icon" href="joomla_cms/templates/mammun/fav.ico" />
	<link rel="icon" href="joomla_cms/templates/mammun/fav.png" type="image/png" />
	<meta http-equiv="refresh" content="0; URL=http://www.mammun.com/<?=$mammun_lng?>/?template=mammun_fb<?=$str?>" />
	<meta name="description" content="<?= LNG_FB_SHARE_DESCRIPTION_GENERAL ?>" />
	<link rel="image_src" href="http://www.mammun.com/pix/mammun_logo_icon.gif" />
</head>
</html>
<?
?>
