<?php
require_once('../include/classes/payment/Payment.inc');
require_once('../include/_config.inc');

$data = Mopay_Response_Parser::get_instance()->parse($HTTP_RAW_POST_DATA);

//error_log("\nMOPAY_CALLBACK:\n".print_r($data, true));
if ($data['STATUS'] == 'SUCCESS') {
	list($uid, $trans_id, $k) = explode("-", $data['MYID']);
	Payment::get_instance($uid)->finish_external_mopay_purchase(
		$trans_id,
		$data['PAYMENT_SESSION.AMOUNT'],
		$data['MSISDN.COUNTRY']
	);
}
?>