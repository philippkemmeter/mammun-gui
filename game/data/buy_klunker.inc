<?php
/**
 * File created by Philipp Kemmeter on 17.09.2009 at 19:20:46
 */

include_once ('../include/std_include.inc');
include('../include/shared/classes/ISO3166.inc');
if (isset($_GET['ajaxid'])) {// Ein ping-und-echo-Parameter für AJAX-Request-Zuweisungen
	$AJAX_OUTPUT = true;
	$r = new ClientServerResponse();
	$r->add_value($_GET['ajaxid']);
	$this_win_id = $_GET['ajaxid'];
}
ob_start();

require_once('../include/classes/payment/Payment.inc');

Language::get_instance()->load_language_file('buy_klunker');
Language::get_instance()->load_language_file('countries_iso3166');

$current_user = User::get_current_user();
$language = Language::get_instance()->get_language();

$expected_data = array (
	"visa"				=>		"bool",
	"master"			=>		"bool",
	"pp"				=>		"bool",
	"mopay"				=>		"bool",
	"product"			=>		"estring",
	"buttonid"			=>		"estring",
	"country"			=>		"estring"
);
$GP = $cryptObj->load_GET_POST($expected_data);

//$personal_data = $current_user->get_personal_data();


/**
 * Checken, ob alle generell nötigen Daten zur Zahlungsabwicklung vorliegen.
 * Wenn nicht, dann diese entsprechend per Formular anfordern.
 */
if ((!$personal_data->firstnames || !$personal_data->surnames ||
	!$personal_data->birthday || !$personal_data->street ||
	!$personal_data->street_number || !$personal_data->zipcode ||
	!$personal_data->city || !$personal_data->country ||
	!$personal_data->salutation) && false)
{
	include_once('templates/buy_klunker/insert_personal_data.tmpl.inc');
}

/**
 * Zahlung per Kreditkarte erwünscht
 */
else if ($GP['product']) {
	$uid = $current_user->get_id();
	if ($GP['visa']) {
		$url = Payment::get_instance($uid)->buy_per_credit_card(
			$GP['product'], CONF_MY_SERVER_URL."/game/buy_klunker_response.php",
			"VISA"
		);
		$w = 400; $h = 600;
		$text = LNG_BUY_KLUNKER_SAFE_POPUP_HEIDELPAY;
	}
	else if ($GP['master']) {
		$url = Payment::get_instance($uid)->buy_per_credit_card(
			$GP['product'], CONF_MY_SERVER_URL."/game/buy_klunker_response.php",
			"MASTERCARD"
		);
		$w = 400; $h = 600;
		$text = LNG_BUY_KLUNKER_SAFE_POPUP_HEIDELPAY;
	}
	else if ($GP['pp']) {
		$url = Payment::get_instance($uid)->buy_per_paypal(
			$GP['product'], CONF_MY_SERVER_URL."/game/buy_klunker_response.php"
		);
		$w = 800; $h = 600;
		$text = LNG_BUY_KLUNKER_SAFE_POPUP_PAYPAL;
	}
	else if ($GP['mopay']) {
		$url = Payment::get_instance($uid)->buy_per_mopay(
			$GP['product'], $GP['buttonid'],
			CONF_MY_SERVER_URL."/game/buy_klunker_response.php",
			$GP['country']
		);
		$w = 800; $h = 500;
		$text = LNG_BUY_KLUNKER_SAFE_POPUP_MOPAY;
	}
	else {
		$product = $GP['product'];
		$country = $GP['country'];
		list($jnk, $kkr) = explode("_", $product);
		/**
		 * Methode muss noch gewählt werden:
		 */
		include_once('templates/buy_klunker/choose_method.tmpl.inc');
	}
	if ($url) {
		$text .=
			'<br/><br/>'.
			'<a href="[HREF]">'.
				LNG_BUY_KLUNKER_CLICK_FOR_POPUP.
			'</a>';

		$r = new ClientServerResponse();
		$r->add_value(1);
		$r->add_value($text);
		$r->add_value($url);
		$r->add_value($w);
		$r->add_value($h);
		die($r);
	}
}

/**
 * Noch kein Produkt und keine Zahlungsmethode ausgewählt
 */
else {
	/**
	 * Handy-Tarife ermitteln für das angegebene Land des Users.
	 */
	if ($GP['country'])
		$country = strtoupper($GP['country']);
	else if ($personal_data->country)
		$country = strtoupper($personal_data->country);
	else $country = "DE";


	/**
	 * Nun kommt der tricky Part. SMS-Kosten sind nicht beliebig bestimmbar,
	 * sondern haben eine ganz strikte Staffelung, die sich jederzeit ändern
	 * kann und vom Land abhängt. Daher werden nun alle von uns gewünschten
	 * Preise durchgegangen und von BOKUMobilePOST korrigiert. Dabei kann es
	 * passieren, dass für manche gewünschte Preise beim besten Willen kein
	 * Pendant existiert (z.B. weil es in dem Land insgesamt weniger
	 * Staffelungen gibt), sowie, dass es für mehrere Wunschpreise einen
	 * selben Kandidaten gibt (z.B. wenn man 1.60 und 1.70 wünscht, aber nur
	 * 2.00 verfügbar ist. Dann wird sowohl für beide Wünsche derselbe Kandidat
	 * 2.00 gewählt). Diese Doppelungen und Auslassungen müssen gehandlet
	 * werden.
	 * Hinzu kommt, dass jeder in seiner Währung bezahlen muss, weswegen auch
	 * die Umrechnung in die Landeswährung vorgenommen wird (jedoch nicht hier,
	 * sondern von den BOKU- und Payment-Klassen hier und auf dem
	 * Payment-Server).
	 */
	$mobile_price_tables = null;		// Resultierende Preistabelle
	$country_array = null;
	foreach (PricingTable::$pricing_table_eur_kkr["mobile"] as $c => $d) {
		if (strtoupper($c) == "REFERENCE")
			continue;
		foreach ($d as $price => $kkr) {
			$mobile_price_tables[strtoupper($c)][] = array(
				"product_id"	=>	"Klunker_$kkr",	// Referenz-ID
				"kkr"			=>	$kkr,			// Klunker
				"price"			=>	$price,			// Preis in Landeswährung
				"currency"		=>	PricingTable	// Landeswährungskürzel
										::$currencies["mobile"][strtolower($c)],
				"buttonid"		=>	MopayPOST::get_button_id($c, $price)
			);
		}
		$c = strtoupper($c);
		$country_array[$c] = constant("LNG_ISO3166_$c");
	}

	include_once('templates/buy_klunker/buy_klunker.tmpl.inc');
}

if ($AJAX_OUTPUT) {
	$r->add_value(ob_get_clean());
	die($r);
}
else
	ob_flush();
?>