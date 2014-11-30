<? /** Wird in einem iFrame geladen, daher volle HTML-Seite */ ?>
<html>
<head>
<script type="text/javascript" src="../js/classes/MammunUI.js?'.filemtime('../js/classes/MammunUI.js').'"></script>
<script type="text/javascript">
MammunUI.get_top_frame().upload_done<?=$this_id?>(<?=$success?>,
	'<?=$return_string?>', '<?=$param2?>');
</script>
</head>
<body>
</html>