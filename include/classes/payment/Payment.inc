<?php
require_once(dirname(__FILE__).'/../../shared/classes/exceptions/'
	.'IllegalArgumentException.inc');
require_once(dirname(__FILE__).'/../../shared/classes/payment/IPayment.inc');
require_once(dirname(__FILE__).'/../../shared/classes/payment/PricingTable.inc');
require_once(dirname(__FILE__).'/../../shared/classes/server_com/ClientServer.inc');
require_once(dirname(__FILE__).'/HeidelpayPOST.inc');
require_once(dirname(__FILE__).'/PaypalPOST.inc');
require_once(dirname(__FILE__).'/BOKUMobilePOST.inc');
require_once(dirname(__FILE__).'/MopayPOST.inc');
require_once(dirname(__FILE__).
			'/../../shared/classes/exceptions/ExceptionWithErrorlog.inc');

/**
 * Diese Klasse ist für die Zahlungsabwicklung zuständig. Sie erstellt
 * Transaktionspakete für verschiedene Zahlungsmethoden. Bei erfolgreicher
 * Abwicklung einer Zahlungsmethode, wird entsprechend der gekaufte Artikel
 * zugewiesen usw.
 *
 * @author Philipp Kemmeter
 */
class Payment implements IPayment {
	private static $INSTANCES;
	private $uid;
	private $klunker_amount = false;

	/**
	 * Gibt die entsprechende Instanz zurück (Sigleton)
	 * @param uint $uid
	 * @return Payment
	 */
	public static function get_instance($uid) {
		if (!isset(self::$INSTANCES[$uid]))
			self::$INSTANCES[$uid] = new self($uid);
		return self::$INSTANCES[$uid];
	}

	/**
	 * Erzeugt die entsprechende Instanz für den User mit der ID $uid.
	 * @param uint $uid
	 * @return Payment
	 */
	private function __construct($uid) {
		$this->uid = $uid;
	}


	private function buy_per_post(IPOSTPayment $post_payment_obj, $product_id,
		$response_url)
	{
		$post_payment_obj->set_response_url($response_url);
		switch ($product_id) {
			case self::PRODUCT_100_KLUNKER:
			case self::PRODUCT_320_KLUNKER:
			case self::PRODUCT_550_KLUNKER:
			case self::PRODUCT_1250_KLUNKER:
			case self::PRODUCT_2900_KLUNKER:
			case self::PRODUCT_8000_KLUNKER:
				/**
				 * Wir starten nun den asynchronen Zahlungsprozess. Zunächst
				 * muss der Engine-Server darüber informiert werden. Er sendet
				 * dann eine für den User eindeutige Transaktions-ID zurück.
				 */

				$transaction_id = Server::get_instance(CONF_PAYMENT_SERVER_URL,
					CONF_MAMMUN_AUTH_SECRET)
						->cmd_start_buy_klunker_transaction($this->uid);


				/**
				 * Nun werden die Zahlungsinformationen gesetzt. Aus der
				 * Produkt-ID lässt sich ablesen, wie viele Klunker der User
				 * kaufen möchte. Die PricingTable gibt aufschluss, wieviel
				 * EUR diese Anzahl dann kostet. In die Transaktions-ID wird
				 * die User-ID noch miteincodiert, da die vom Engine-Server
				 * geschickte Transaktions-ID nur pro User eindeutig ist. Somit
				 * wird die ID komplett eindeutig.
				 */
				list($junk, $kkr_amount) = explode("_", $product_id);

				$eur_amount = PricingTable
					::$pricing_table_kkr_eur["default"][$kkr_amount];

				$post_payment_obj->set_payment_information(
					$eur_amount, $product_id, $this->uid."-".$transaction_id,
					"EUR"
				);
				$response_array = $post_payment_obj->commit_post_payment();
				break;
			default:
				throw new IllegalArgumentException("product",
					"{Payment::PRODUCT_...}", "$product_id");
		}
		return $response_array;
	}

	public function buy_per_boku_mobile($product_id, $response_url, $country) {
		$boku = BOKUMobilePOST::get_instance($this->uid);

		$boku->set_response_url($response_url);
		$transaction_id = Server::get_instance(CONF_PAYMENT_SERVER_URL,
			CONF_MAMMUN_AUTH_SECRET)
					->cmd_start_buy_klunker_transaction($this->uid);

		list($junk, $kkr_amount) = explode("_", $product_id);

		$boku->set_country($country);

		$eur_amount =
			PricingTable::$pricing_table_kkr_eur["mobile"][$kkr_amount];

		$boku->set_payment_information(
			$eur_amount, $product_id, $this->uid."-".$transaction_id,
			"EUR"
		);

		$response_array = $boku->commit_post_payment();

		if ($response_array["RESULT_CODE"] != 0) {
			throw new ExceptionWithErrorlog(
				"Could not connect to BOKU Server!",
				$response_array["RESULT_CODE"]
			);
		}
		return $response_array["BUY_URL"];
	}

	/**
	 * Kauft eines der verfügbaren Produkte (siehe Payment::PRODUCT-Konstanten)
	 * per Paypal.
	 *
	 * Gibt die URL zurück, welche vom User angesurft werden muss, um die
	 * gewünschte Zahlung abzuschließen.
	 *
	 * @param uint $product_id
	 * @param string $response_url
	 * @return string
	 * @throws IllegalArgumentException, ExceptionWithErrorlog
	 */
	public function buy_per_paypal($product_id, $response_url) {
		$paypal = PaypalPOST::get_instance($this->uid);

		$response_array = $this->buy_per_post($paypal, $product_id,
			$response_url);
		if (strtoupper($response_array["ACK"]) != "SUCCESS") {
			throw new ExceptionWithErrorlog(
				"Could not connect to Paypal-Server!",
				$response_array["ACK"]
			);
		}
		return $response_array["REDIRECT_URL"];
	}

	/**
	 * Initiiert den Kaufvorgang per mopay.
	 * Gibt ein assoc-array mit den post-keys und -Werten zurück.
	 *
	 * @param uint $product_id
	 * @param string $button_id
	 * @param string $response_url
	 * @param string $country_code
	 * @return Gibt die URL zurück, welche vom User angesurft werden muss, um
	 * die gewünschte Zahlung einzuleiten.
	 */
	public function buy_per_mopay($product_id, $button_id, $response_url,
		$country_code)
	{
		$mopay = MopayPOST::get_instance($this->uid);
		$mopay->set_button_id($button_id);
		$mopay->set_response_url($response_url);
		$mopay->set_country($country_code);

		$transaction_id = Server::get_instance(CONF_PAYMENT_SERVER_URL,
			CONF_MAMMUN_AUTH_SECRET)
					->cmd_start_buy_klunker_transaction($this->uid);

		list($junk, $kkr_amount) = explode("_", $product_id);

		$mopay->set_payment_information($kkr_amount,
			$product_id, $this->uid.'-'.$transaction_id
		);
		$eur_amount =
			PricingTable::$pricing_table_kkr_eur["mobile"][$kkr_amount];

		$array = $mopay->commit_post_payment();

		$result = MopayPOST::SERVER_URL.'?';
		foreach ($array as $key => $value) {
			$result .= $key.'='.$value.'&';
		}
		return substr($result, 0, -1);
	}

	/**
	 * Kauft eines der verfügbaren Produkte (siehe Payment::PRODUCT-Konstanten)
	 * per Kreditkarte. Eine Antwort-URL ($response_url) muss angegeben werden,
	 * da die Bezahlung auf einer externen Seite geschieht.
	 * Gibt die URL zurück, welche vom User angesurft werden muss, um die
	 * gewünschte Zahlung abzuschließen.
	 * $subtype gibt an, um welche Kreditkarte es sich handelt.
	 *
	 * @param uint $product_id
	 * @param string $response_url
	 * @param string $subtype
	 * @return string
	 * @throws IllegalArgumentException, ExceptionWithErrorlog
	 */
	public function buy_per_credit_card($product_id, $response_url, $subtype) {
		$heidelpay = HeidelpayPOST::get_instance($this->uid);
		$heidelpay->set_payment_code(
			HeidelpayPOST::PAYMENT_CODE_CREDITCARD, $subtype
		);

		$response_array = $this->buy_per_post($heidelpay, $product_id,
			$response_url);
		if ($response_array["POST.VALIDATION"] != "ACK") {
			throw new ExceptionWithErrorlog(
				"Could not connect to Heidelpay-Server!",
				$response_array["POST.VALIDATION"]
			);
		}

		return $response_array["FRONTEND.REDIRECT_URL"];
	}

	/**
	 * Beendet jegliche externe Kaufaktion derart, dass die Klunker zugewiesen
	 * werden, wenn die Transaktions-ID stimmt. Die Funktionalität reicht ggf.
	 * aber nicht aus, um die Kaufaktion komplett abzuschließen, da z.B. bei
	 * Paypal der eigentliche Transfer vom Geld jetzt erst noch gemacht werden
	 * muss, wogegen das bei CC schon passiert ist.
	 *
	 * @param string $transaction_id
	 * @param string $product_id
	 * @throws IllegalArgumentException
	 */
	private function finish_external_purchase_general($transaction_id,
		$product_id)
	{
		$tmp = explode('-',$transaction_id);
		if (count($tmp) == 2)
			$transaction_id = $tmp[1];

		$transaction_id = Server::get_instance(CONF_PAYMENT_SERVER_URL,
			CONF_MAMMUN_AUTH_SECRET)
				->cmd_finish_buy_klunker_transaction($this->uid,
					$transaction_id, $product_id);
	}

	/**
	 * Beendet den Kaufvorgang, der durch buy_per_credit_card() gestartet
	 * wurde. Dazu muss die erhaltene, eindeutige Transaktions-ID übergeben
	 * werden. Ob die UID noch miteincodiert ist, oder nicht, spielt dabei
	 * keine Rolle.
	 * Die Unique-ID ist eine ID anhand dessen der Kunde bei Heidelpay erneut
	 * identifiziert werden kann, so dass die Felder vorausgefüllt sind, ohne
	 * dass wird.
	 *
	 * Der User bekommt dann seine gekauften Klunker gutgeschrieben.
	 *
	 * @param string $transaction_id
	 * @param string $product_id
	 * @param string $unique_id
	 * @see include/classes/payment/Payment#buy_per_credit_card
	 * @throws IllegalArgumentException
	 */
	public function finish_external_cc_purchase($transaction_id, $product_id,
		$unique_id)
	{
		switch ($product_id) {
			case self::PRODUCT_100_KLUNKER:
			case self::PRODUCT_320_KLUNKER:
			case self::PRODUCT_550_KLUNKER:
			case self::PRODUCT_1250_KLUNKER:
			case self::PRODUCT_2900_KLUNKER:
			case self::PRODUCT_8000_KLUNKER:
				break;
			default:
				throw new IllegalArgumentException("product",
					"{Payment::PRODUCT_...}", "$product");
		}

		/**
		 * Die angegebene Unique-ID wird nun gesetzt, damit der User später
		 * erneut daran identifiziert werden kann.
		 */
		Server::get_instance(CONF_PAYMENT_SERVER_URL, CONF_MAMMUN_AUTH_SECRET)
			->set_heidelpay_unique_id($this->uid, $unique_id);

		/**
		 * Nun alles abschließen.
		 */
		$this->finish_external_purchase_general($transaction_id, $product_id);
	}

	/**
	 * Beendet den Kaufvorgang, der durch buy_per_paypal() gestartet wurde. Dazu
	 * muss die erhaltene, eindeutige Transaktions-ID übergeben werden (für die
	 * eigene Sicherheit), sowie $token und $payerid für die Paypal-Zuordnung
	 * (Da bei Paypal die Bezahlung noch bestätigt werden muss).
	 *
	 * @param string $transaction_id
	 * @param string $product_id
	 * @param string $token
	 * @param string $payerid
	 * @see include/classes/payment/Payment#buy_per_paypal
	 * @throws IllegalArgumentException
	 */
	public function finish_external_pp_purchase($transaction_id, $product_id,
		$token, $playerid)
	{
		switch ($product_id) {
			case self::PRODUCT_100_KLUNKER:
			case self::PRODUCT_320_KLUNKER:
			case self::PRODUCT_550_KLUNKER:
			case self::PRODUCT_1250_KLUNKER:
			case self::PRODUCT_2900_KLUNKER:
			case self::PRODUCT_8000_KLUNKER:
				break;
			default:
				throw new IllegalArgumentException("product",
					"{Payment::PRODUCT_...}", "$product");
		}

		/**
		 * Zunächst muss bei Paypal die eigentliche Bezahl JETZT durchgeführt
		 * werden, da im Gegensatz zu CC der externe Screen nur das einloggen
		 * usw darstellt, aus Sicherheitsgründen das Bezahlen direkt hier
		 * passiert.
		 */
		list($junk, $kkr_amount) = explode("_", $product_id);
		$eur_amount =
			PricingTable::$pricing_table_kkr_eur["default"][$kkr_amount];
		$result = PaypalPOST::finish_transaction($token, $playerid,
			$eur_amount, "EUR", $product_id);

		$this->finish_external_purchase_general($transaction_id, $product_id);
	}

	/**
	 * Beendet den Kaufvorgang, der durch buy_per_boku_mobile() gestartet wurde.
	 * Dazu muss die erhaltene, eindeutige Transaktions-ID übergeben werden
	 * (für die eigene Sicherheit), sowie das Geld, das bezahlt wurde in
	 * EUROCENT.
	 *
	 * Gibt zurück, wie viele Klunker aufgeladen wurden, da das ggf. in diesem
	 * Schritt noch korrigiert wird, abhängig davon, wie viel der User
	 * tatsächlich bezahlt hat.
	 *
	 * @param string $transaction_id
	 * @param string $boku_trx_id
	 * @return uint
	 * @see include/classes/payment/Payment#buy_per_boku_mobile
	 */
	public function finish_external_boku_purchase($transaction_id, $eur_amount){
		$kkr = PricingTable::get_variable_mobile_kkr($eur_amount/100);
		$this->finish_external_purchase_general($transaction_id,
			"Klunker_$kkr");
		return $kkr;
	}

	public function finish_external_mopay_purchase($transaction_id,
		$currency_amount, $country_code='de')
	{
		$country_code = strtolower($country_code);
		$kkr = PricingTable::$pricing_table_eur_kkr['mobile']
										[$country_code][$currency_amount];

		if (!$kkr) {
			throw new ExceptionWithErrorlog("Error while finishing mopay",
				$this->uid, "PricingTable::\$pricing_table_eur_kkr['mobile']".
					"[$country_code][$currency_amount] is $kkr. ".
					"Transaction ID: $transaction_id");
		}

		$this->finish_external_purchase_general($transaction_id,
			"Klunker_$kkr");
		return $kkr;
	}

	/**
	 * Verifiziert einen BOKU-Kauf ohne den Kaufvorgang abzuschließen - es
	 * werden also keine Klunker gutgeschrieben.
	 *
	 * Gibt zurück, wie viele Klunker aufgeladen wurden, da das ggf. in diesem
	 * Schritt noch korrigiert wird, abhängig davon, wie viel der User
	 * tatsächlich bezahlt hat.
	 *
	 * @param string $boku_trx_id
	 * @return uint
	 */
	public function verify_boku_purchase($boku_trx_id) {
		$result = BOKUMobilePOST::verify_trx($boku_trx_id);
		if ($result["RESULT_CODE"] != 0) {
			throw new Exception(
				"Verification failed: {$result["RESULT_MSG"]}!",
					$result["RESULT_CODE"]
			);
		}
		else {
			return PricingTable::get_variable_mobile_kkr($result["PAID"]/100);
		}
	}

	/**
	 * Verifiziert einen mopay-Kauf ohne den Kaufvorgang abzuschließen - es
	 * werden also keine Klunker gutgeschrieben.
	 *
	 * Gibt zurück, ob die angegebenen Daten von mopay kamen, oder nicht.
	 *
	 * @param assoc $params		z.B. array_merge($_POST, $_GET)
	 * @return bool
	 */
	public function verify_mopay_purchase($params) {
		return MopayPOST::verify_request($params);
	}

	/**
	 * Gibt zurück, wieviele Klunker der User hat.
	 *
	 * @return uint
	 */
	public function get_klunker_amount($forced=false) {
		if (($this->klunker_amount === false) || $forced)
			$this->klunker_amount = Server::get_instance(
				CONF_PAYMENT_SERVER_URL,CONF_MAMMUN_AUTH_SECRET)
					->get_klunker_amount($this->uid);
		return $this->klunker_amount;
	}
}
?>