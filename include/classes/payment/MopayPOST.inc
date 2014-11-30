<?php
require_once('IPOSTPayment.inc');


class Mopay_Response_Parser {
	public static function get_instance() {
		if (!isset(self::$INSTANCE))
			self::$INSTANCE = new self();
		return self::$INSTANCE;
	}
	private static $INSTANCE;

	private function __construct() {}

	private static $result;
	private static $cur_key;

	/**
	 * Parset eine Mopay-XML-Antwort. Das Ergebnis ist ein assotiatives
	 * Array, dass prizipiell eindimensional ist. Im Falle von Doppelung
	 * von Tags wird diese Eindimensionalität zwangsläufig aufgebohrt,
	 * so dass für diesen Key ein Array entsteht mit allen gefundenen
	 * Vorkommen.
	 *
	 * @param string $xml_data
	 * @return string[]
	 */
	public function parse($xml_data) {
		$xml_parser = xml_parser_create("ISO-8859-1");
		xml_set_element_handler(
			$xml_parser,
			array($this, "start_elem"),
			array($this, "end_elem")
		);
		xml_set_character_data_handler(
			$xml_parser,
			array($this, "c_data")
		);
		xml_parse($xml_parser, trim($xml_data), true);
		xml_parser_free($xml_parser);
		return $this->result;
	}

	public function start_elem($parser, $tag, $attributes) {
		$this->cur_key = str_replace('-', '_', $tag);
		foreach ($attributes as $key => $value) {
		$key = str_replace('-', '_', $key);
			$k = $this->cur_key.".".$key;
			if (!isset($this->result[$k]))
				$this->result[$k] = $value;
			else {
				if (!is_array($this->result[$k])) {
					$tmp = $this->result[$k];
					$this->result[$k] = array();
					$this->result[$k][] = $tmp;
				}
				$this->result[$k][] = $value;
			}
		}
	}

	public function end_elem($parser, $tag) {
	}

	public function c_data($parser, $data) {
		if (trim($data) === '')
			return;
		if ($this->cur_key) {
			$this->result[$this->cur_key] = trim($data);
			$this->cur_key = null;
		}
    }
}


class MopayPOST implements IPOSTPayment {
	private $uid;
	private $button_id;
	private $product_name;
	private $return_url;
	private $transaction_id;
	private $payment_amount;
	private $myid;
	private $country_code;

	private static $button_ids = array(
		"de"	=>	array(
			"0.99"	=>	"5F811A0F82D1EC4EF800FCFA88E8CEDB",
			"2.99"	=>	"7C7F0710ED6ED8A2ABC7E21997A39BA7",
			"4.99"	=>	"2476F470A3C59DCA4D675BC71F75A59C",
			"9.99"	=>	"47E1EEBC6B41710EE00A854B28D78C2C"
		),
		"at"	=>	array(
			"1"		=>	"5F811A0F82D1EC4EF800FCFA88E8CEDB",
			"3"		=>	"7C7F0710ED6ED8A2ABC7E21997A39BA7",
			"5"		=>	"2476F470A3C59DCA4D675BC71F75A59C",
			"10"	=>	"47E1EEBC6B41710EE00A854B28D78C2C"
		),
		"ch"	=>	array(	// in CHF
			"1.5"	=>	"5F811A0F82D1EC4EF800FCFA88E8CEDB",
			"5"		=>	"7C7F0710ED6ED8A2ABC7E21997A39BA7"
		)
	);

	private static $INSTANCE;

	const SERVER_URL = "https://securepay.mopay.com/mopay20/button/start.action";
	const SECRET =  "XZX!7aee7G";	//"uZuaUczuP6";
	const SERVICE_NAME = "Mammun";

	/**
	 * @param uint $uid
	 * @return MopayPOST
	 */
	public static function get_instance($uid) {
		if (!isset(self::$INSTANCE[$uid]))
			self::$INSTANCE[$uid] = new self($uid);
		return self::$INSTANCE[$uid];
	}

	/**
	 * Gibt die ID des Buttons zurück, der in dem angegebenen Land den
	 * angegebenen Preis (in Landeswährung) enthält.
	 *
	 * @param string $country_code
	 * @param float $price
	 */
	public static function get_button_id($country_code, $price) {
		$price = (string)$price;
		$country_code = strtolower($country_code);
		return self::$button_ids[$country_code][$price];
	}


	private function __construct($uid) {
		$this->uid = $uid;
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


	public function set_payment_information($payment_amount, $payment_usage,
		$identification_transactionid, $payment_currency='EUR')
	{
		$this->payment_amount = $payment_amount;
		$this->transaction_id = $identification_transactionid;
		$this->product_name = $payment_usage;
		$this->myid = $this->transaction_id.'-'.$this->payment_amount;
	}

	public function set_button_id($button_id) {
		$this->button_id = $button_id;
	}

	public function set_country($country_code) {
		$this->country_code = strtoupper($country_code);
	}

	private function calc_token() {
		return strtoupper(md5(
			self::SECRET.$this->button_id.$this->myid.$this->return_url.
			$this->product_name.self::SERVICE_NAME.$this->uid
		));
	}

	public static function verify_request($params) {
		return ($params['token'] == strtoupper(md5(
			self::SECRET.$params['guid'].$params['myid'].$params['status']
			.$params['msisdn'])));
	}

	/**
	 * Bei mopay brauchen wir keine Server-Kommunikation hierzu, sondern können
	 * das lokal ausrechnen. Wir tun aber so, als hätten wir's gemacht, um das
	 * ganze Kompatibel zu halten!
	 *
	 * @see include/classes/payment/IPOSTPayment#commit_post_payment()
	 */
	public function commit_post_payment() {
		return array(
			'buttonid'		=> $this->button_id,
			'productname'	=> $this->product_name,
			'myid'			=> $this->myid,
			'backurl'		=> $this->return_url,
			'servicename'	=> self::SERVICE_NAME,
			'token'			=> $this->calc_token(),
			'backurltarget'	=> '_self',
			'externaluid'	=> $this->uid,
			'country'		=> $this->country_code
		);
	}
}
?>