<?php

require_once(dirname(__FILE__).'/IPOSTPayment.inc');

/**
 * Diese Klasse erlaubt Zahlung per Heidelpay-POST-Verfahren. Es wird ein Paket
 * per POST an Heidelpay gesandt, worauf eine URL als Antwort kommt, welche
 * vom Nutzer angesurft werden muss, um den Zahlungsvorgang abzuschließen.
 *
 * @author Philipp Kemmeter
 */
class HeidelpayPOST implements IPOSTPayment {
	private static $INSTANCES;
	private $uid;
	private $params;	// Alle POST-Parameter, welche gesandt werden sollen
	private $curl_error;
	private $curl_info;

	const SERVER = "ctpe.net";
	const PATH = "/frontend/payment.prc";
	const SENDER = "ff80818124c867260124d84986f21f0a";
	const USER_ID = "ff80818124c867260124d84987041f0f";
	const PASSWORD = "N2FAyCjz";
	const CHANNEL = "ff80818124c867260124d84ade301f22";
	const TOKEN = '';
	const TRANSACTION_MODE = "LIVE";

	/*
	const SERVER = "test.ctpe.net";
	const SENDER = "d225a9fefe3fbaf400fe43281064000a";
	const CHANNEL = "d225a9fefe3fbaf400fe43294aca000d";
	const USER_ID = "8a90818a230893b201230f089dab03c1";
	const PASSWORD = "demo";
	const TOKEN = "test";
	const TRANSACTION_MODE = "INTEGRATOR_TEST";*/
	const TRANSACTION_RESPONSE = "SYNC";
	const USER_AGENT = "php ctpepost";

	const REQUEST_VERSION = "1.0";


	const FRONTEND_MODE_DEFAULT = 0;

	const PAYMENT_CODE_CREDITCARD = 0;
	const PAYMENT_CODE_DIRECTDEBIT = 1;


	/**
	 * Gibt die entsprechende Instanz zurück (Sigleton)
	 * @param uint $uid
	 * @return HeidelpayPOST
	 */
	public static function get_instance($uid) {
		if (!isset(self::$INSTANCES[$uid]))
			self::$INSTANCES[$uid] = new self($uid);
		return self::$INSTANCES[$uid];
	}

	/**
	 * Erzeugt die entsprechende Instanz für den User mit der ID $uid.
	 * @param uint $uid
	 * @return HeidelpayPOST
	 */
	private function __construct($uid) {
		$this->uid = $uid;

		$user = User::get_instance_by_id($uid);
		$personal_data = $user->get_personal_data();

		$this->params["request.version"] = self::REQUEST_VERSION;
		$this->params["security.token"] = self::TOKEN;
		$this->params["server"] = self::SERVER;
		$this->params["path"] = self::PATH;
		$this->params["security.sender"] = self::SENDER;
		$this->params["transaction.channel"] = self::CHANNEL;
		$this->params["user.login"] = self::USER_ID;
		$this->params["user.pwd"] = self::PASSWORD;
		$this->params["transaction.mode"] = self::TRANSACTION_MODE;
		$this->params["transaction.response"] = self::TRANSACTION_RESPONSE;

		$this->params["contact.email"] = $user->get_email();
		$this->params["contact.ip"] = $_SERVER['REMOTE_ADDR'];
		$this->params["contact.mobile"] = '';
		$this->params["contact.phone"] = '';

		$this->params["address.street"] = $personal_data->street . " " .
											$personal_data->street_number;
		$this->params["address.zip"] = $personal_data->zipcode;
		$this->params["address.city"] = $personal_data->city;
		$this->params["address.state"] = $personal_data->state;
		$this->params["address.country"] = strtoupper($personal_data->country);

		$this->params["name.salutation"] = $personal_data->salutation;
		$this->params["name.title"] = $personal_data->title;
		$this->params["name.given"] = $personal_data->firstnames;
		$this->params["name.family"] = $personal_data->surnames;
		$this->params["name.company"] = '';

		$this->params["IDENTIFICATION.UNIQUEID"] =
			Server::get_instance(CONF_PAYMENT_SERVER_URL,
				CONF_MAMMUN_AUTH_SECRET)
					->get_heidelpay_unique_id($this->uid);

		$this->params["FRONTEND.ENABLED"] = 'true';
		$this->params["FRONTEND.POPUP"] = 'false';
		$this->params["FRONTEND.MODE"] = "DEFAULT";
		$this->params["FRONTEND.LANGUAGE"] =
			$this->params["address.country"] ? $this->params["address.country"]
				: "DE";
		$this->params["FRONTEND.RESPONSE_URL"] = $_SERVER['PHP_SELF'];
		/*$this->params["FRONTEND.CSS_PATH"] =
			CONF_MY_SERVER_URL.'/css/heidelpay.css';*/

		$this->params["FRONTEND.PM.DEFAULT_DISABLE_ALL"] = "TRUE";
		$this->params["FRONTEND.PM.1.METHOD"] = "CC";
		$this->params["FRONTEND.PM.1.ENABLED"] = "TRUE";


		$this->params["CRITERION.UserID"] = $this->uid;
		$this->params["CRITERION.known"] = "yes";
	}

	/**
	 * Setzt den Modus des Frontends entsprechend der Angabe für $mode. Dabei
	 * muss $mode einer der HeidelpayPOST::FRONTEND_MODE-Konstanten entsprechen.
	 *
	 * @param self::FRONTEND_MODE-Konstante $mode
	 */
	public function set_frontend_mode($mode) {
		if (($mode < 0) || ($mode > self::FRONTEND_MODE_DEFAULT)) {
			throw new IllegalArgumentException("mode",
				"{HeidelpayPOST::FRONTEND_MODE_...}", $mode);
		}
		$this->params["FRONTEND.MODE"] = $mode;
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
		$this->params["FRONTEND.RESPONSE_URL"] = $url;
	}

	/**
	 * Setzt den Code der vorgesehenen Zahlung auf den angegebenen Wert. Dabei
	 * muss dieser einer der HeidelpayPOST::PAYMENT_CODE-Konstanten entprechen.
	 *
	 * @param self::PAYMENT_CODE-Konstante $mode
	 */
	public function set_payment_code($code, $subtype=false) {
		if ($payment_code == self::PAYMENT_CODE_CREDITCARD) {
			$this->params["payment.code"] = 'CC.DB';
			if ($subtype) {
				$this->params["FRONTEND.PM.1.SUBTYPES"] = strtoupper($subtype);
			}
		}
		else if ($payment_code == self::PAYMENT_CODE_DIRECTDEBIT)
			$this->params["payment.code"] = 'DD.CB';
		else throw new IllegalArgumentException(
				"payment_code", "[0,1]", $payment_code
		);
	}

	/**
	 * Setzt die Zahlungsinformationen.
	 *
	 * @param ufloat $payment_amount		Wie viel Geld
	 * @param string $payment_usage			Was gekauft wurde
	 * @param string $identification_transactionid
	 * @param string $payment_currency		Währung (DEFAULT: EUR)
	 */
	public function set_payment_information($payment_amount, $payment_usage,
		$identification_transactionid, $payment_currency='EUR')
	{
		$this->params["presentation.amount"] = (float)$payment_amount;
		$this->params["presentation.usage"] = $payment_usage;
		$this->params["identification.transactionID"] =
											$identification_transactionid;
		$this->params["presentation.currency"] = $payment_currency;
	}

	/**
	 * Schickt die Anfrage ab, welche zuvor durch die verschiedenen Setter
	 * dieser Klasse definiert wurde.
	 *
	 * Gibt u.a. URL zurück, welche vom User angesurft werden soll, um den
	 * Zahlungsvorgang abzuschließen, zudem andere Daten, die Heidelpay
	 * zurückliefert. Alles in einem Asso-Array verpackt.
	 *
	 * @see include/classes/payment/IPOSTPayment#commit_post_payment()
	 * @return string[]
	 */
	public function commit_post_payment() {
		$result = '';
		foreach (array_keys($this->params) AS $key)
		{
			$$key .= $this->params[$key];
			$$key = urlencode($$key);
			$$key .= "&";
			$var = strtoupper($key);
			$value = $$key;
			$result .= "$var=$value";
		}
		$postdata = stripslashes($result);

		$cpt = curl_init();

		curl_setopt($cpt, CURLOPT_URL, "https://".self::SERVER.self::PATH);
		curl_setopt($cpt, CURLOPT_SSL_VERIFYHOST, 1);
		curl_setopt($cpt, CURLOPT_USERAGENT, self::USER_AGENT);
		curl_setopt($cpt, CURLOPT_RETURNTRANSFER, 1);
		curl_setopt($cpt, CURLOPT_SSL_VERIFYPEER, 1);
		//curl_setopt($cpt, CURLOPT_SSL_VERIFYPEER, FALSE);

		curl_setopt($cpt, CURLOPT_POST, 1);
		curl_setopt($cpt, CURLOPT_POSTFIELDS, $postdata);

		$resultURL = curl_exec($cpt);
		$this->curl_error = curl_error($cpt);
		$this->curl_info = curl_getinfo($cpt);

		curl_close($cpt);
		return $this->parse_response($resultURL);
	}

	/**
	 * Parset den übergebenen Antwortstring von Heidelpay und wandelt ihn in
	 * ein assoziatives Array um.
	 *
	 * @param string $response
	 * @return assoc-array
	 */
	private function parse_response($response) {
		$r_arr=explode("&",$response);

		foreach($r_arr AS $buf)
		{
			$temp=explode("=",urldecode($buf),2);
			$returnvalue[$temp[0]] = $temp[1];
		}
		return $returnvalue;
	}
}
?>