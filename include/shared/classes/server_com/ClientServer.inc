<?php
/**
 * ClientServer spiegelt eine Klassensammlung wider, die es ermöglicht
 * zwischen zwei PHP-Programmen Daten zu versenden. Der eine nimmt dabei
 * die Rolle eines (Web-)Clients ein, der andere bleibt in der gewohnten
 * Server-Script-Rolle.
 * Der Client sendet einen HTTP-Request und übergibt dabei GET-Variablen,
 * der Server kann durch einfaches echo-Statement antworten. Diese Klassen-
 * sammlung ist insofern interessant, dass sie einen Standard definiert, wie
 * die Pakete durch echo ausgeben werden sollen.
 * Die Serialisierung der Pakete geschieht durch JSON, was den Vorteil hat,
 * dass sowohl PHP, als auch JS die Deserialisierung von Haus aus durchführen
 * können. Die entsprechende JS-Klasse, welche denselben Paketstandard für JS
 * definiert, ist AJAXClientServer.js.
 * @see AJAXClientServer.js
 */

require_once (dirname(__FILE__).'/../helper/Converter.inc');
require_once (dirname(__FILE__).'/TrustedServerAuth.inc');

/**
 * Ein Antwort-Paket, das zwischen Client und Server auf eine Anfrage
 * hin geschickt wird.
 *
 * @author Philipp Kemmeter
 */
class ClientServerResponse {
	const CMD_RELOGIN = 1;
	const CMD_SERVER_DOWN = 2;
	const CMD_MAINTENANCE = 3;
	const VALUE = 100;
	const VALUE_SIMPLE = 101;
	const VALUE_ENCRYPTED = 102;
	const SESSION = 103;
	const EXCEPTION = 104;


	private $response = '';
	private $session_added;

	public function __construct($response_string='') {
		$this->response = $response_string;
		$this->session_added = false;
	}

	/**
	 * Fügt zur ClientServer-Antwort einen normalen Wert hinzu.
	 *
	 * @param mixed $value
	 */
	public function add_value($value) {
		if (is_array($value) || is_object($value))
			$this->response .= self::VALUE.'||'.Converter::xToJson($value).'||';
		else
			$this->response .= self::VALUE_SIMPLE.'||'.$value.'||';
	}

	/**
	 * Fügt einen normalen Wert verschlüsselt hinzu.
	 *
	 * TODO: Aktuell wird der Wert NICHT verschlüsselt!
	 *
	 * @param mixed $value
	 */
	public function add_value_encrypted($value) {
		/*$cryptObj = new PV_encryption();
		$dataX = $cryptObj->do_encryption($value, false);*/
		$dataX = $value;
		$this->response .= self::VALUE_ENCRYPTED.'||'.$dataX.'||';
	}

	/**
	 * Fügt den Befehl zum Neu-Einloggen hinzu.
	 *
	 * Ist dieser Befehl in der Client/Sever-Antwort enthalten, wird der die
	 * Antwort erhaltende Client/Server seine Sitzung sofort beenden und zum
	 * Relogin auffordern.
	 *
	 * @param string $reason		Warum der Logout passierte
	 */
	public function add_relogin_command($reason) {
		$this->response .= self::CMD_RELOGIN.'||'.$reason.'||';
	}

	/**
	 * Fügt eine Exception hinzu.
	 *
	 * Wird eine Exception gesendet, wird der Empfänger diese sofort werfen.
	 *
	 * @param Exception $e
	 */
	public function add_exception(Exception $e) {
		$this->response .= self::EXCEPTION.'||'.get_class($e).'||'.
			$e->getMessage().'||'.$e->getCode().'||';
	}

	/**
	 * Fügt Session-Informationen hinzu.
	 *
	 * Der Empfänger weiß dann, dass er diese Sitzungsdaten verwenden soll bei
	 * jeder weiteren Kommunikation, die sich auf die aktuelle Sitzung bezieht.
	 *
	 * @param String $session_name
	 * @param String $session_id
	 */
	public function add_session($session_name='', $session_id='') {
		if (!$this->session_added) {
			if (!$session_name || !$session_id) {
				$session_name = session_name();
				$session_id = session_id();
			}
			$this->response .=
				self::SESSION.'||'.$session_name.'||'.$session_id.'||';
			$this->session_added = true;
		}
	}

	/**
	 * @return string
	 */
	public function to_string() {
		// Die letzten beiden Zeichen sind || - nicht mitÃ¼bertragen
		if (substr($this->response, -2) === '||')
			return substr($this->response, 0, -2);
		else
			return $this->response;
	}

	public function __toString() {
		return $this->to_string();
	}
}

/**
 * Diese Klasse ermöglicht es HTTP-Requests zu senden, sowie Antworten
 * zu senden und zu empfangen.
 *
 * @author Philipp Kemmeter
 */
class ClientServer {
	static private $session_name = null;
	static private $session_id = null;
	static public $debug = false;

	/**
	 * Setzt die Sitzungsdaten, die bei Server-Requests verwendet werden sollen,
	 * um eine bestimmte Sitzung auf dem anderen Server zu adressieren.
	 *
	 * Diese Daten werden dann an jeden Request mitgeschickt.
	 *
	 * TODO: Mehrere Sitzungen gleichzeitig verwalten
	 *
	 * @param string $session_name
	 * @param string $session_id
	 */
	static public function set_session($session_name, $session_id) {
		self::$session_name = $session_name;
		self::$session_id = $session_id;
	}

	/**
	 * Gibt die aktuell gesetzte Sitzungs-ID zurück.
	 *
	 * @return string
	 */
	static public function get_session_id() {
		return self::$session_id;
	}

	/**
	 * Gibt die aktuell gesetzten Sitzungsnamen zurück.
	 *
	 * @return string
	 */
	static public function get_session_name() {
		return self::$session_name;
	}

	/**
	 * Sendet eine Server-Anfrage.
	 *
	 * @param string $url		Komplette URL inklusive GET/POST-Parameter
	 * @param bool $no_parse	Wenn TRUE, dann wird die Antwort plain
	 * 							zurückgegeben, ansonsten wird ein Array mit den
	 * 							zurückgegebenen Werten zurückgegeben.
	 * @param string $trusted_secret	Wenn gesetzt, wird die Anfrage per
	 * 									Trusted Server Authentification
	 * 									geschickt. Dazu muss hier ein Geheimnis
	 * 									angegeben werden, dass der Empfänger
	 * 									kennt, um die Anfrage verifizieren zu
	 * 									können.
	 * @return mixed[]/string
	 */
	static public function send_request($url, $no_parse=false,
		$trusted_secret=false)
	{
		$url = str_replace('http://', '', $url);
		$first_slash_pos = strpos($url, '/');
		$host = substr($url, 0, $first_slash_pos);
		$host = explode(':', $host);
		$port = (isset($host[1]) ? $host[1] : 80);
		$host = gethostbyname($host[0]);
		$path = substr($url,$first_slash_pos);
		$path = explode('?', $path);
		$data = (isset($path[1]) ? $path[1] : '');
		$path = $path[0];

		return self::send_request2($host, $port, $path, $data, $no_parse,
			$trusted_secret);
	}

	/**
	 * Diese Funktion sendet eine Anfrage so wie send_request, jedoch unter
	 * der Angabe anderer Werte.
	 *
	 * Hier wird keine URL angegeben, sondern explizit Host, Port, Server-Pfad
	 * und GET/POST-Daten. Ansonsten alles gleich.
	 *
	 * @param string $host
	 * @param uint $port
	 * @param string $path
	 * @param assoc $data
	 * @param bool $no_parse
	 * @param string $trusted_secret
	 * @return mixed[]/string
	 * @see ClientServer#send_request
	 */
	static public function send_request2($host, $port, $path, $data,
		$no_parse=false, $trusted_secret=false)
	{
		$t0 = microtime(true);
		$port = (int)$port;
		$session = (session_id() != "");

		if ($session)	// Session beenden, wegen Deadlock-Gefahr
			session_write_close();

		if (self::$session_name && self::$session_id) {
			if (strlen($data)>0)
				$data .= '&'.self::$session_name.'='.self::$session_id;
			else
				$data = self::$session_name.'='.self::$session_id;
		}
		if ($trusted_secret) {
			$data = Converter::assocToQueryString(
				TrustedServerAuth::create_trusted_request_array(
					Converter::queryStringToAssoc($data),
					$trusted_secret
				)
			);
		}

		$contents = '';
		try {
			$contents = self::httpSocketConnection(
				$host, $port, "POST", $path, $data
			);
		}
		catch (Exception $e) {
			$contents = ClientServerResponse::CMD_SERVER_DOWN;
		}
		if ($session)	// Session neu Starten
			@session_start();
		$t1 = microtime(true);

		if ($no_parse) {
			return new ClientServerResponse($contents);
		}
		else {
			$expected_response_data = array();	// not supported, yet
			return self::parse_response(
				new ClientServerResponse($contents), $expected_response_data
			);
		}
	}

	/**
	 * @deprecated
	 *
	 * @param string $url
	 * @param misc[] $data_to_forward
	 * @return string
	 */
	static public function forward_request($url, $data_to_forward=array()) {
		$session = (session_id() != "");
		if ($session)	// Session beenden, wegen Deadlock-Gefahr
			session_write_close();
		if (self::$session_id && self::$session_name) {
			if (strpos($url, '?'))
				$url .= '&';
			else
			 	$url .= '?';
			$url .= self::$session_name.'='.self::$session_id;
		}
		foreach ($data_to_forward as $key => $value) {
			if (strpos($url, '?'))
				$url .= '&';
			else
			 	$url .= '?';
			 $url .= $key.'='.$value;
		}

		$handle = fopen($url, "r");
		$contents = '';
		if ($handle) {
			while (!feof($handle)) {
				$contents .= fread($handle, 8192);
			}
			fclose($handle);
		}
		if ($session)	// Session neu Starten
			@session_start();
		return $contents;
	}

	/**
	 * Gibt eine ClientServer-Antwort aus.
	 *
	 * @deprecated
	 * @param ClientServerResponse $response
	 */
	static public function echo_response(ClientServerResponse $response) {
		echo $response;
	}

	/**
	 * Parst die Antwort, die von einem Client/Server zurückgeschickt wurde.
	 *
	 * $expected_data wird aktuell nicht genuzt! Future use!
	 *
	 * @param ClientServerResponse $response
	 * @param assoc $expected_data
	 * @return mixed[]
	 *
	 * @throws Exception
	 * @throws HackattackException
	 * @throws IllegalArgumentException
	 */
	static public function parse_response(
		ClientServerResponse $response, $expected_data=array())
	{
		$r_arr = explode ("||", $response->to_string());
		$result = null;
		while (count($r_arr) > 0) {
			switch (array_shift($r_arr)) {
				case ClientServerResponse::CMD_RELOGIN:
					self::$session_id = null;
					self::$session_name = null;
					$reason = array_shift($r_arr);
					if (isset($_GET['ajaxid'])) {
						$r = new ClientServerResponse();
						$r->add_relogin_command($reason);
						die($r);
					}
					else {
						$user_in_facebook = (isset($_SESSION['fb']['fb_sig']));
						if ($user_in_facebook) {
							@($redirect_page = CONF_FACEBOOK_APP_URL
								. "?map_key={$_SESSION['map_key']}".
									"&visit={$_SESSION['visit']}");
						}
						else {
							@($redirect_page = CONF_LOGIN_PAGE_URI
								. "?map_key={$_SESSION['map_key']}".
									"&visit={$_SESSION['visit']}");
						}

						include(dirname(__FILE__).
							'/../../../../game/templates/logout.tmpl.inc');
					}
					if ($user_in_facebook) {
						$fb = $_SESSION['fb'];
						$visited = $_SESSION['visited'];
					}
					if (session_id() != "") {
						session_destroy();
						@session_start();
					}
					if ($user_in_facebook) {
						$_SESSION['fb'] = $fb;
						$_SESSION['visited'] = $visited;
					}
					die();
					break;
				case ClientServerResponse::CMD_SERVER_DOWN:
					echo "Could not connect to server. Please try again later.";
					die();
					break;
				case ClientServerResponse::CMD_MAINTENANCE:
					echo "Maintenance!";
					die();
					break;
				case ClientServerResponse::VALUE:
					$result[] = Converter::jsonToX(array_shift($r_arr), false);
					break;
				case ClientServerResponse::VALUE_SIMPLE:
					$result[] = array_shift($r_arr);
					break;
				case ClientServerResponse::EXCEPTION:
					$e_class_name = array_shift($r_arr);
					$e_message = array_shift($r_arr);
					$e_code = array_shift($r_arr);
					error_log("ClientServer::EXCEPTION: $e_class_name($e_message, $e_code)");
					if (class_exists($e_class_name, false))
						throw new $e_class_name($e_message, $e_code);
					else
						throw new Exception('('.$e_class_name.') '.$e_message, $e_code);
					break;
				case ClientServerResponse::VALUE_ENCRYPTED:
					$result[] = /*$cryptObj->do_decryption(*/
						array_shift($r_arr)/*,
						$expected_data
					)*/;
					break;
				case ClientServerResponse::SESSION:
					self::$session_name = array_shift($r_arr);
					self::$session_id = array_shift($r_arr);
					break;
				default:
					throw new Exception("!PARSING ERROR!<br>\n".$response);
			}
		}
		return $result;
	}

	/**
	 * Diese Funktion führt den eigentlichen Request aus.
	 *
	 * Es wird eine HTTP-Socket-Verbindung (blocking) ausgeführt und das
	 * Ergebnis zurückgegeben.
	 *
	 * Dies könnte auch mit cURL erledigt werden, aber so läuft es auch ohne.
	 *
	 * TODO: cURL-Variante implementieren, da schneller auf Servern, wo cURL
	 * verfügbar.
	 *
	 * @param string $host
	 * @param uint $port
	 * @param {'GET', 'POST'} $method
	 * @param string $path
	 * @param assoc $data
	 * @return string
	 */
  	public static function httpSocketConnection($host, $port, $method, $path,
  		$data)
  	{
		$method = strtoupper($method);

		if ($method == "GET") {
			$path.= '?'.$data;
		}

		$filePointer = @fsockopen($host, $port, $errorNumber, $errorString);

		if (!$filePointer) {
			throw new Exception("$errorNumber $errorString");
		}

		$requestHeader = $method." ".$path."  HTTP/1.1\r\n";
		$requestHeader.= "Host: ".$host."\r\n";
		$requestHeader.= "User-Agent:      Mozilla/5.0 (Windows; U; Windows NT 5.1; en-US; rv:1.8.1) Gecko/20061010 Firefox/2.0\r\n";
		$requestHeader.= "Content-Type: application/x-www-form-urlencoded\r\n";

		if ($method == "POST") {
			$requestHeader.= "Content-Length: ".strlen($data)."\r\n";
		}

		$requestHeader.= "Connection: close\r\n\r\n";

		if ($method == "POST") {
			$requestHeader.= $data;
		}

		fwrite($filePointer, $requestHeader);

		$responseHeader = '';
		$responseContent = '';

		do {
			$responseHeader.= fread($filePointer, 1);
		}
		while (!preg_match('/\\r\\n\\r\\n$/', $responseHeader));

		if (!strstr($responseHeader, "Transfer-Encoding: chunked")) {
			while (!feof($filePointer)) {
				$responseContent.= fgets($filePointer, 128);
			}
		}
		else {
			while ($chunk_length = hexdec(fgets($filePointer))) {
				$responseContentChunk = '';

				$read_length = 0;

				while ($read_length < $chunk_length) {
					$responseContentChunk .= fread($filePointer, $chunk_length - $read_length);
					$read_length = strlen($responseContentChunk);
				}

				$responseContent.= $responseContentChunk;

				fgets($filePointer);
			}
		}
		return chop($responseContent);
	}

	private function __construct() {}
}

/**
 * Die Server-Klasse bietet die Möglichkeit, Server-Requests wie RPCs
 * aufzurufen. Im Endeffekt ist sie also eine Wrapper-Klasse.
 *
 * @author Philipp Kemmeter
 */
class Server {
	private $url;
	private $trusted_server_sig=false;

	static private $INSTANCES;

	/**
	 * @return Server
	 */
	public static function get_instance($url, $trusted_server_sig=false) {
		if (!isset(self::$INSTANCES[$url]))
			self::$INSTANCES[$url] = new Server($url, $trusted_server_sig);
		else
			self::$INSTANCES[$url]->trusted_server_sig = $trusted_server_sig;
		return self::$INSTANCES[$url];
	}

	private function __construct($url, $trusted_server_sig) {
		$this->url = $url;
		$this->trusted_server_sig = $trusted_server_sig;
	}

	public function __call($fn_name, array $params) {
		$params = $this->parse_params($params);
		$x = ClientServer::send_request($this->url."?$fn_name=$params",false,
			$this->trusted_server_sig);
		return $x[0];
	}

	private function parse_params($params) {
		if (!$params)
			$params = 0;
		else if (!is_array($params))
			$params = $params;
		else if (count($params) == 1)
			$params = $params[0];
		return serialize($params);
	}

	public function multi_call(array $fn_calls) {
		$str = '';
		foreach ($fn_calls as $fn_name => $params) {
			$params = $this->parse_params($params);
			$str .= "&$fn_name=$params";
		}
		$str{0} = '?';
		$x = ClientServer::send_request($this->url.$str);
		return $x;
	}
}
?>