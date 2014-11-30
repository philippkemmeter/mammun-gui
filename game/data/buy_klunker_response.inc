<?php
if ($_REQUEST["all_done"]) {
	require_once('../include/shared/classes/Language.inc');
	Language::get_instance()->load_language_file('buy_klunker');
	include('templates/header.tmpl.inc');
	$klunker_add = $_REQUEST["k"];
	if ($klunker_add <= 0)
		include('templates/buy_klunker/buy_klunker_external_failed.tmpl.inc');
	else
		include('templates/buy_klunker/buy_klunker_external_finish.tmpl.inc');
	include('templates/footer.tmpl.inc');
	die();
}
if ($_REQUEST["failed"])
{
	require_once('../include/shared/classes/Language.inc');
	Language::get_instance()->load_language_file('buy_klunker');
	include('templates/header.tmpl.inc');
	include('templates/buy_klunker/buy_klunker_external_failed.tmpl.inc');
	include('templates/footer.tmpl.inc');
	die();
}

require_once('../include/classes/payment/Payment.inc');
require_once('../include/_config.inc');

/**
 * Heidelpay-Transaktion wurde durchgeführt - Klunker gutschreiben.
 */
if (isset($_REQUEST["IDENTIFICATION_TRANSACTIONID"])) {
	 if ($_REQUEST["PROCESSING_RESULT"] != 'NOK') {
		list($uid,$trans_id) = explode("-", $_REQUEST["IDENTIFICATION_TRANSACTIONID"]);

		Payment::get_instance($uid)->finish_external_cc_purchase(
			$trans_id, $_REQUEST["PRESENTATION_USAGE"],
			$_REQUEST['IDENTIFICATION_UNIQUEID']
		);
		list($foo, $k) = explode("_",$_REQUEST["PRESENTATION_USAGE"]);
	}
	else {
		$k = 0;
	}
	/**
	 * Heidelpay leitet dann zu folgender URL weiter:
	 */
	die(CONF_MY_SERVER_URL.'/game/buy_klunker_response.php?all_done=true&k='.$k);
}
/**
 * Paypal-Transaktion wurde durchgeführt - Klunker gutschreiben.
 */
else if (isset($_REQUEST["INVNUM"])) {
	list($uid,$trans_id) = explode("-", $_REQUEST["INVNUM"]);

	Payment::get_instance($uid)->finish_external_pp_purchase(
		$trans_id, $_REQUEST["CUSTOM"], $_REQUEST["token"], $_REQUEST["PayerID"]
	);
	list($foo, $k) = explode("_",$_REQUEST["CUSTOM"]);
	/**
	 * Bei Paypal leiten WIR zur nächsten Seite weiter:
	 */
	header("Location: ".CONF_MY_SERVER_URL.'/game'.
							'/buy_klunker_response.php?all_done=true&k='.$k);

}
/**
 * mopay-Transaktion wurde durchgeführt - Klunker gutschreiben.
 */
else if (isset($_REQUEST["token"])) {
	if (isset($_REQUEST['status'])
		&& (strtolower($_REQUEST['status']) == 'error'))
	{
		$k = 0;
	}
	else {
		list($uid, $trans_id, $k) = explode('-', $_REQUEST['myid']);

		if(!Payment::get_instance($uid)->verify_mopay_purchase(
													array_merge($_GET, $_POST)))
		{
			$k = 0;
		}
	}
	/**
	 * Bei mopay leiten WIR zur nächsten Seite weiter
	 */
	header("Location: ".CONF_MY_SERVER_URL.'/game'.
				'/buy_klunker_response.php?all_done=true&k='.$k);
}


?>