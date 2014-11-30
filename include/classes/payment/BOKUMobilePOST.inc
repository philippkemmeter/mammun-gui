<?php
require_once(dirname(__FILE__).'/IPOSTPayment.inc');
require_once(dirname(__FILE__).'/../../shared/classes/Time.inc');
require_once(dirname(__FILE__).'/../../shared/classes/ISO3166.inc');
require_once(dirname(__FILE__).'/../../shared/classes/payment/BOKUMobile.inc');


class BOKU_Response_Parser {
        private function __construct() {}

        private static $result;
        private static $cur_key;

        /**
         * Parset eine BOKU-XML-Antwort. Das Ergebnis ist ein assotiatives
         * Array, dass prizipiell eindimensional ist. Im Falle von Doppelung
         * von Tags wird diese Eindimensionalität zwangsläufig aufgebohrt,
         * so dass für diesen Key ein Array entsteht mit allen gefundenen
         * Vorkommen.
         *
         * @param string $xml_data
         * @return string[]
         */
        public static function parse($xml_data) {
                $xml_parser = xml_parser_create("UTF-8");
                xml_set_element_handler(
                        $xml_parser,
                        "BOKU_Response_Parser::start_elem",
                        "BOKU_Response_Parser::end_elem"
                );
                xml_set_character_data_handler(
                        $xml_parser,
                        "BOKU_Response_Parser::c_data"
                );
                xml_parse($xml_parser, trim($xml_data), true);
                xml_parser_free($xml_parser);
                return self::$result;
        }

        public static function start_elem($parser, $tag, $attributes) {
                self::$cur_key = str_replace('-', '_', $tag);
                foreach ($attributes as $key => $value) {
                        $k = self::$cur_key.".".$key;
                        if (!isset(self::$result[$k]))
                                self::$result[$k] = $value;
                        else {
                                if (!is_array(self::$result[$k])) {
                                        $tmp = self::$result[$k];
                                        self::$result[$k] = array();
                                        self::$result[$k][] = $tmp;
                                }
                                self::$result[$k][] = $value;
                        }
                }
        }

        public static function end_elem($parser, $tag) {
        }

        public static function c_data($parser, $data) {
                if (trim($data) === '')
                        return;
                if (self::$cur_key) {
                        self::$result[self::$cur_key] = trim($data);
                        self::$cur_key = null;
                }
        }
}


/**
 * Diese Klasse erlaubt Zahlung per Heidelpay-POST-Verfahren. Es wird ein Paket
 * per POST an Heidelpay gesandt, worauf eine URL als Antwort kommt, welche
 * vom Nutzer angesurft werden muss, um den Zahlungsvorgang abzuschließen.
 *
 * @author Philipp Kemmeter
 */
class BOKUMobilePOST extends BOKUMobile implements IPOSTPayment {
	private static $INSTANCES;
	private $uid;
	private $return_url;
	private $language;
	private $country;
	private $mobile_number;
	private $payment_amount;
	private $payment_currency;
	private $transaction_id;
	private $payment_usage;
	private $key;

	const SERVER_URL = "https://api2.boku.com/billing/request";
	const SERVICE_ID = "135389155cf0c115861fc3c8";
	const MERCHANT_ID = "Gausus";
	const PASSWORD = "shad0wm4ster";
	const SECURITY_KEY = "mMPrga05BkSmxpY0ML1hfYZySgg6K8es08uq0R7j8fCVMfRcAynAn64UZUHvfmAPvqR0oJi259yME7LbbpuOAj9EADUb2ct3aRrr";


	/**
	 * Gibt die entsprechende Instanz zurück (Sigleton)
	 * @param uint $uid
	 * @return BOKUMobilePOST
	 */
	public static function get_instance($uid) {
		if (!isset(self::$INSTANCES[$uid]))
			self::$INSTANCES[$uid] = new self($uid);
		return self::$INSTANCES[$uid];
	}

	/**
	 * Erzeugt die entsprechende Instanz für den User mit der ID $uid.
	 * @param uint $uid
	 * @return BOKUMobilePOST
	 */
	private function __construct($uid) {
		$this->uid = $uid;

		$user = User::get_instance_by_id($uid);
		$personal_data = $user->get_personal_data();
		$this->language = $personal_data->country
							? strtoupper($personal_data->country)
							: "DE";
		$this->mobile_number = $personal_data->mobile;
		$this->country = strtolower($personal_data->country);
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
	 * Setzt das Land, aus dem der Anruf getätigt werden wird. Überschreibt
	 * somit ggf. im Profil des Users hinterlegte Daten.
	 *
	 * @param string $country	(ISO 3166)
	 */
	public function set_country($country) {
		if (!in_array($country, ISO_3166::$all_codes))
			throw new IllegalArgumentException("country", "{ISO 3166}", $country);
		$this->country = $country;
	}

	/**
	 * Diese Funktion verifiziert die Transaktion, also ob diese
	 * Transaktions-ID auch korrekt ist.
	 *
	 * @param $trx_id
	 * @return unknown_type
	 */
	public static function verify_trx($trx_id) {
		$post_array = array (
			'action'				=>	'verify-trx-id',
			'merchant-id'			=>	self::MERCHANT_ID,
			'timestamp'				=>	Time::get_t_now(),
			'trx-id'				=>	$trx_id
		);
		$post_array['sig'] = self::create_signatur($post_array);
		$post_string = self::create_post_string($post_array);

		$result= self::send_post_package_to_server($post_string);
		return $result;
	}

	/**
	 * Bei mobilen Anwendungen ist es nicht möglich, dass man den payment-amount
	 * frei wählt (aufgrund u.a. gesetzlicher Normen). Der gewünschte Payment-
	 * Amount, der hier angegeben wird, wird für das angegebene Land verglichen.
	 * Ist der Wert nicht verfügbar, wird der nächst höhere Wert zurückgegeben,
	 * welcher verfügbar ist. Ist kein höherer Wert verfügbar, wird ein leeres
	 * Array zurückgegeben. Zu beachten gilt hier, dass alles in € abgewickelt
	 * wird. Der Rückgabewert ist daher ein Array mit drei Werten:
	 * [0]	=>	korrigierter Wert in EUR
	 * [1]	=>	Währung des übergebenen Landes
	 * [2]	=>	korrigierter Wert in der Währung aus [1]
	 *
	 * @param ufloat $payment_amount
	 * @param string $country	(ISO_3166)
	 * @return (float, string, float)
	 * @throws ExceptionWithErrorlog
	 */
	public static function correct_payment_amount($payment_amount, $country)
	{
		if (!in_array($country, ISO_3166::$all_codes))
			throw new IllegalArgumentException("country", "{ISO 3166}", $country);

		$boku_amount_table = Server::get_instance(CONF_PAYMENT_SERVER_URL,
			CONF_MAMMUN_AUTH_SECRET)->
						get_boku_pricing_table($country);
		if (empty($boku_amount_table))
			return array();

		$boku_amount_table = Converter::stdClassToArray($boku_amount_table);

		/**
		 * Das erhaltene Array wird nun durchsucht, um den Preis zu finden,
		 * der der nächst höhere Preis ist, falls der angeforderte nicht
		 * verfügbar ist.
		 */
		$amount_key = $payment_amount*100;	// BOKU gibt Preise in Cent an

		/**
		 * Nun aufsteigend durchsuchen. Erster Hit der >= ist, ist der
		 * nächste und somit gesuchte.
		 */
		asort($boku_amount_table);
		foreach ($boku_amount_table as $k => $boku_amount) {
			list($amount_with_currency,$currency) = explode("_",$k);
			if ($boku_amount >= $amount_key) {
				return array(
					$boku_amount/100,
					$currency,
					$amount_with_currency/100
				);
			}
		}
		/**
		 * Wurde keiner gefunden
		 */
		return array();
	}

	/**
	 * ACHTUNG: Bei mobilen Anwendungen ist es NICHT möglich, dass man den
	 * payment-amount frei wählt! Der angegebene Wert wird entsprechend
	 * derart korrigiert, dass der nächst höhere Amount gewählt wird. Dieser
	 * wird dann zurückgeliefert, damit der Aufrufende der Funktion die
	 * Korrektur nachvollziehen kann.
	 * Um diese Korrektur vorzunehmen, muss zuvor das Land bekannt sein, aus
	 * dem der User anrufen möchte! Ansonsten gibt's ne Exception.
	 *
	 * @return $ufloat
	 * @see include/classes/payment/IPOSTPayment#set_payment_information()
	 * @throws Exception
	 */
	public function set_payment_information($payment_amount, $payment_usage,
		$identification_transactionid, $payment_currency='EUR')
	{
		if (!$this->country) {
			throw new ExceptionWithErrorlog(
				"BOKUMobilePOST::set_payment_information: NO COUNTRY!");
		}

		/**
		 * Zunächst den $payment_amount korrigieren.
		 */
		list($new_amount,$foo,$bar) = self::correct_payment_amount(
			$payment_amount, $this->country, $payment_currency
		);

		$this->key = $payment_amount*100;
		$this->payment_amount = $new_amount*100;
		$this->payment_currency = $payment_currency;
		$this->transaction_id = $identification_transactionid;
		$this->payment_usage = $payment_usage;
	}

	/**
	 * Diese Funktion generiert die Signatur für eine BOKU-Anfrage gemäß der
	 * Security-Spezifikation von BOKU (BOKU Security Implementation). Dazu
	 * muss ein Asso-Array von POST-Key-Value-Paren übergeben werden, welche
	 * an den BOKU-Server gesendet werden sollen. Auf der Basis der Variablen
	 * sowie des Security-Keys (self::SECURITY_KEY) wird dann ein md5-Hash
	 * erstellt, welcher als Parameter sig zusätzlich an den Server geschickt
	 * werden muss.
	 * Achtung: timestamp muss im übergebenen POST-Array enthalten sein.
	 *
	 * @param string[] $post_array
	 * @return string
	 */
	private static function create_signatur($post_array) {
		if (!$post_array['timestamp'])
			throw new IllegalArgumentException("Kein timestamp in \$postarray: ".
													print_r($post_array,true));
		unset($post_array['password']);
		ksort($post_array);
		$result_string = '';
		foreach ($post_array as $key => $value) {
			if (empty($value))
				continue;
			$result_string .= "$key$value";
		}
		$result_string .= self::SECURITY_KEY;
		return md5($result_string);

	}

	/**
	 * Konvertiert ein Key-Value-Pair-Array in einen POST-String (&..=..&..=)
	 *
	 * @param string[] $post_array
	 * @return string
	 */
	private static function create_post_string($post_array) {
		$result = '';
		foreach ($post_array as $key => $value)
			$result .= "$key=$value&";
		return substr($result, 0, -1);
	}


	private static function send_post_package_to_server($post_string) {
		$ch = curl_init();
		curl_setopt($ch, CURLOPT_URL, self::SERVER_URL);
		curl_setopt($ch, CURLOPT_VERBOSE, 1);

		// turning off the server and peer verification(TrustManager Concept).
		curl_setopt($ch, CURLOPT_SSL_VERIFYPEER, FALSE);
		curl_setopt($ch, CURLOPT_SSL_VERIFYHOST, FALSE);

		curl_setopt($ch, CURLOPT_RETURNTRANSFER,1);
		curl_setopt($ch, CURLOPT_POST, 1);

		// setting the $post_string as POST FIELD to curl
		curl_setopt($ch, CURLOPT_POSTFIELDS, $post_string);
		//getting response from server
		$response = curl_exec($ch);
		curl_close($ch);
		return BOKU_Response_Parser::parse($response);
	}

	/**
	 * @see include/classes/payment/IPOSTPayment#commit_post_payment()
	 */
	public function commit_post_payment() {
		$kkr_amount = PricingTable::get_variable_mobile_kkr(
			$this->payment_amount/100);
		$post_array = array (
			'action'				=>	'prepare',
			'merchant-id'			=>	self::MERCHANT_ID,
			'service-id'			=>	self::SERVICE_ID,

			'country'				=>	$this->country,
			'fwdurl'				=>	$this->return_url,
			'desc'					=>	"$kkr_amount Klunker",
			'key'					=>	$this->key,
			'param'					=>	$this->transaction_id.'-'.$this->key.
											'-'.$kkr_amount,
			'timestamp'				=>	Time::get_t_now()
		);

		$post_array['sig'] = self::create_signatur($post_array);
		$post_string = self::create_post_string($post_array);

		$result= self::send_post_package_to_server($post_string);
		return $result;
	}
}
?>