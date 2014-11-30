<?php
include_once('../include/classes/payment/Payment.inc');
print_r(array_merge($_GET, $_POST));

$post_vars = Payment::get_instance(123)->buy_per_mopay(
	Payment::PRODUCT_100_KLUNKER,
	"A7B529F19BE34A9917D3DD85086F73AB",
	"http://www.mammun.com/mopay_test.php"
);
?>
<!DOCTYPE form PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd">
<html>
<head>
</head>
<body>
<form action="https://securepay.mopay.com/mopay20/button/start.action"
	accept-charset="UTF-8" method="post" name="mopaybuttonform" id="buttonForm"
	target="buttonWindow1">
<? foreach ($post_vars as $key => $value) : ?>
	<input type="hidden" name="<?=$key?>" value="<?=$value?>" />
<? endforeach; ?>
</form>

<a href="#" id="buttonPopupLink" onclick="window.open('','buttonWindow1', 'height=450,width=600,resizable=no,modal=yes,location=no,menubar=no,resizable=no,status=no');document.getElementById('buttonForm').submit();"
    style="border:0;padding:5px;margin:5px;background-color:transparent;vertical-align:middle;" >
    ASD
</a>
</body>
</html>
