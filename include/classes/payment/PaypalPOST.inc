<?php
require_once(dirname(__FILE__).'/IPOSTPayment.inc');


/**
 * Diese Klasse initiiert Zahlungen zu Paypal.
 *
 * @author Philipp Kemmeter
 */
class PaypalPOST implements IPOSTPayment {
	private $uid;
	private $local_code;
	private $email;
	private $payment_usage;
	private $payment_amount;
	private $payment_currency;
	private $return_url;
	private $transaction_id;
	private $language;

	const SERVER_URL = 'https://api-3t.paypal.com/nvp';
	const USERNAME = 'customer_api1.gausus.com';
	const PASSWORD = 'ZXQN9VCW4UDEGLF5';
	const SIGNATURE = 'AX9TfNC.Jgz1XG2EiWsE46ZSSl7tAqBaFPy1Y4XPqGu.my40apoOlXYC';
	const VERSION = '2.3';

	const PAYPAL_URL =
		'https://www.paypal.com/webscr&cmd=_express-checkout&token=';

/*	const SERVER_URL = 'https://api-3t.sandbox.paypal.com/nvp';
	const USERNAME = 'sdk-three_api1.sdk.com';
	const PASSWORD = 'QFZCWN5HZM8VBG7Q';
	const SIGNATURE = 'A.d9eRKfd1yVkRrtmMfCFLTqa6M9AyodL0SJkhYztxUi8W9pCXF6.4NI';
	const PAYPAL_URL =
		'https://www.sandbox.paypal.com/webscr&cmd=_express-checkout&token=';*/


	private static $INSTANCES;

	/**
	 * Gibt die entsprechende Instanz zurück (Sigleton)
	 * @param uint $uid
	 * @return PaypalPOST
	 */
	public static function get_instance($uid) {
		if (!isset(self::$INSTANCES[$uid]))
			self::$INSTANCES[$uid] = new self($uid);
		return self::$INSTANCES[$uid];
	}

	/**
	 * Erzeugt die entsprechende Instanz für den User mit der ID $uid.
	 * @param uint $uid
	 * @return PaypalPOST
	 */
	private function __construct($uid) {
		$this->uid = $uid;
		$user = User::get_instance_by_id($uid);
		$this->email = $user->get_email();
		$personal_data = $user->get_personal_data();
		$this->language = $personal_data->country
							? strtoupper($personal_data->country)
							: "DE";
	}

	/**
	 * Setzt die URL, die der User nach erfolgreicher Bezahlung angesurft
	 * bekommt.
	 *
	 * @param url $url
	 * @throws IllegalArgumentException
	 * @see include/classes/payment/IPOSTPayment#set_response_url()
	 */
	public function set_response_url($url) {
		if (strpos($url, "http://") !== 0) {
			throw new IllegalArgumentException(
				"\$url should start with http://, $url given.");
		}
		$this->return_url = $url;
	}

	/**
	 * @see include/classes/payment/IPOSTPayment#set_payment_information()
	 */
	public function set_payment_information($payment_amount, $payment_usage,
		$identification_transactionid, $payment_currency='EUR') {
		$this->payment_amount = $payment_amount;
		$this->payment_currency = $payment_currency;
		$this->transaction_id = $identification_transactionid;
		$this->payment_usage = $payment_usage;
	}

	/**
	 * @see include/classes/payment/IPOSTPayment#commit_post_payment()
	 */
	public function commit_post_payment() {
		$ch = curl_init();
		curl_setopt($ch, CURLOPT_URL, self::SERVER_URL);
		curl_setopt($ch, CURLOPT_VERBOSE, 1);

		// turning off the server and peer verification(TrustManager Concept).
		curl_setopt($ch, CURLOPT_SSL_VERIFYPEER, FALSE);
		curl_setopt($ch, CURLOPT_SSL_VERIFYHOST, FALSE);

		curl_setopt($ch, CURLOPT_RETURNTRANSFER,1);
		curl_setopt($ch, CURLOPT_POST, 1);
		$result_url = $this->return_url."?INVNUM={$this->transaction_id}".
											"&CUSTOM={$this->payment_usage}";
		$cancel_url = $this->return_url;
		$nvpreq="METHOD=SetExpressCheckout".
			"&VERSION=".urlencode(self::VERSION).
			"&PWD=".urlencode(self::PASSWORD).
			"&USER=".urlencode(self::USERNAME).
			"&SIGNATURE=".urlencode(self::SIGNATURE).
			"&EMAIL=".urlencode($this->email).
			"&AMT=".urlencode($this->payment_amount).
			"&CURRENCYCODE=".urlencode($this->payment_currency).
			"&RETURNURL=".urlencode($result_url).
			"&CANCELURL=".urlencode($cancel_url).
			"&GIROPAYSUCCESSURL=".urlencode($this->return_url).
			"&GIROPAYCANCELURL=".urlencode($this->return_url).
			"&PAYMENTACTION=Sale".
			"&INVNUM=".urlencode($this->transaction_id).
			"&CUSTOM=".urlencode($this->payment_usage).
			"&NOSHIPPING=1".
			"&DESC=".urlencode($this->payment_usage).
			"&LOCALCODE=".urlencode($this->language);

		// setting the nvpreq as POST FIELD to curl
		curl_setopt($ch, CURLOPT_POSTFIELDS, $nvpreq);

		//getting response from server
		$response = curl_exec($ch);
		curl_close($ch);
		return self::parse_response($response);
	}

	/**
	 * Parset den übergebenen Antwortstring von Paypal und wandelt ihn in
	 * ein assoziatives Array um.
	 *
	 * @param string $response
	 * @return assoc-array
	 */
	private static function parse_response($response) {
		$r_arr=explode("&",$response);

		foreach($r_arr AS $buf)
		{
			$temp=split("=",urldecode($buf),2);
			$returnvalue[$temp[0]] = $temp[1];
		}
		$returnvalue["REDIRECT_URL"] = self::PAYPAL_URL.$returnvalue["TOKEN"];
		return $returnvalue;
	}
	/**
	 * Schließt die POST-Sache bei Paypal derart ab, dass das zweite von Paypal
	 * erwartete POST-Paket geschickt wird. Dazu muss der User das Formular der
	 * Seite, die unter der URL, welche commit_post_payment zurückgibt, zu
	 * finden ist, ausgefüllt haben.
	 * Die zu übergebenen Parameter gibt Paypal dann der response_url mit.
	 *
	 * @param string $token
	 * @param string $payerid
	 * @param string $payment_amout
	 * @param string $payment_currency
	 * @param string $payment_usage
	 * @return string[]
	 */
	public static function finish_transaction($token, $payerid, $payment_amout,
		$payment_currency, $payment_usage)
	{
		$ch = curl_init();
		curl_setopt($ch, CURLOPT_URL, self::SERVER_URL);
		curl_setopt($ch, CURLOPT_VERBOSE, 1);

		// turning off the server and peer verification(TrustManager Concept).
		curl_setopt($ch, CURLOPT_SSL_VERIFYPEER, FALSE);
		curl_setopt($ch, CURLOPT_SSL_VERIFYHOST, FALSE);

		curl_setopt($ch, CURLOPT_RETURNTRANSFER,1);
		curl_setopt($ch, CURLOPT_POST, 1);
		$nvpreq="METHOD=DoExpressCheckoutPayment".
			"&VERSION=".urlencode(self::VERSION).
			"&PWD=".urlencode(self::PASSWORD).
			"&USER=".urlencode(self::USERNAME).
			"&SIGNATURE=".urlencode(self::SIGNATURE).
			"&TOKEN=".urlencode($token).
			"&PAYERID=".urlencode($payerid).
			"&AMT=".urlencode($payment_amout).
			"&CURRENCYCODE=".urlencode($payment_currency).
			"&CUSTOM=".urlencode($payment_usage).
			"&NOSHIPPING=1".
			"&PAYMENTACTION=Sale".
			"&DESC=".urlencode($payment_usage);

		// setting the nvpreq as POST FIELD to curl
		curl_setopt($ch, CURLOPT_POSTFIELDS, $nvpreq);


		//getting response from server
		$response = curl_exec($ch);
		curl_close($ch);
		return self::parse_response($response);
	}
}
?>