<?php
include (dirname(__FILE__).'/../header.tmpl.inc');
if ($error_text)
	echo $error_text;
else
	echo LNG_ACTIVATION_COMPLETED;
include (dirname(__FILE__).'/../footer.tmpl.inc');
?>