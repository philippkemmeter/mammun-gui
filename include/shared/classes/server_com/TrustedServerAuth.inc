<?php
/**
 * Diese Klasse prüft, ob ein Request von einem vertauten Server kam.
 *
 * Dazu müssen bestimmte Parameter per POST oder GET zusätzlich zur eigenen
 * Anfrage geschickt werden, die nur der vertraute Server kennen kann und die
 * nicht nachahmbar sind.
 *
 * @author Philipp Kemmeter
 */
class TrustedServerAuth {
	/**
	 * Gibt zurück, ob der Request wirklich von dem Server gekommen ist.
	 *
	 * Erwartet als erstes Argument ein Geheimnis, dass nur der Trusted-Server
	 * und der prüfende Server kennen. Mithilfe des Timeouts wird
	 * sichergestellt, dass keine Requests einfach abgefangen, kopiert und
	 * immer wieder verwendet werden können. Je kürzer der Timeout, desto
	 * sicherer, je länger, desto stabiler.
	 *
	 * @param assoc $params		Alle externen Parameter sollen hier enthalten
	 * 							sein. $_GET, $_POST, $_REQUEST oder $_COOKIE
	 * 							sind mögliche sinnvolle Alternativen.
	 * @param String $secret	Das Geheimnis, das nur der Server kennen kann.
	 * @param uint $timeout		Timeout in Sekunden.
	 * @return bool
	 */
	public static function verify(array $params, $secret,
		$timeout=300)
	{
		$prefix = self::get_reserved_params_prefix();
		/**
		 * Prüfen, ob die Zeit bereits überschritten wurde
		 */
		if ($timeout && (!isset($params[$prefix . 'time'])
			|| time() - $params[$prefix . 'time'] > $timeout))
		{
			return false;
		}

		if (!isset($params[$prefix . 'sig']))
			return false;

		$sig = $params[$prefix . 'sig'];
		unset($params[$prefix . 'sig']);

		if ($sig !== self::generate_sig($params, $secret))
			return false;

		return true;
	}

	/**
	 * Diese Funktion generiert aus einem normalen Request-Array einen
	 * vertrauenswürdigen, indem ein Zeitstempel und eine Signatur hinzugefügt
	 * werden.
	 *
	 * Dies basiert auf dem übergebenen Geheimnis, dass der Empfänger kennen
	 * muss, um prüfen zu können, ob die Anfrage auch tatsächlich vom korrekten
	 * Absender kommt.
	 *
	 * DIE ANFRAGE DARF DANACH NICHT MEHR VERÄNDERT WERDEN, SONST PASST DIE
	 * SIGNATUR NICHT MEHR!
	 *
	 * @param assoc/string $params		Alle externen Parameter sollen hier
	 * 									enthalten sein. $_GET, $_POST, $_REQUEST
	 * 									oder $_COOKIE sind mögliche sinnvolle
	 * 									Alternativen.
	 *
	 * @param string $secret	Das Geheimnis, das nur der Server kennen kann.
	 * @return assoc/string		Rückgabe in entsprechend gleicher Form wie
	 * 							$params
	 */
	public static function create_trusted_request_array(array $params, $secret) {
		$prefix = self::get_reserved_params_prefix();
		$params[$prefix . 'time'] = time();
		$params[$prefix . 'sig'] = self::generate_sig($params, $secret);

		return $params;
	}

	/**
	 * Gibt die Signatur der Anfrage zurück basierend auf dem Geheimnis.
	 *
	 * Beim Versenden muss diese Signatur immer unter dem Schlüssel "sig"
	 * mitgeschickt werden.
	 *
	 * @param assoc $params		Alle externen Parameter sollen hier enthalten
	 * 							sein. $_GET, $_POST, $_REQUEST oder $_COOKIE
	 * 							sind mögliche sinnvolle Alternativen.
	 * @param String $secret	Das Geheimnis, das nur der Server kennen kann.
	 */
	private static function generate_sig(array $params, $secret) {
		$str = '';

		ksort($params);
		foreach ($params as $k=>$v) {
			$v = stripslashes($v);
			$str .= "$k=$v";
		}
		$str .= $secret;

	    return md5($str);
	}

	/**
	 * Gibt den Präfix aller reservierten Keys zurück.
	 *
	 * Die Keys, die in Anfragen via post, get o.ä. nicht explizit genutzt
	 * werden sollen, da sie für die Abwicklung der Authentifizierung
	 * benötigt werden, haben alle einen gemeinsamen Präfix, an denen man
	 * sie identifizieren kann.
	 *
	 * Mit anderen Worten, alle keys, die mit diesem String anfangen, sind nicht
	 * zu ändern, und man soll keine eigenen Parameter benutzten, die hiermit
	 * anfangen.
	 *
	 * @return string[]
	 */
	public static function get_reserved_params_prefix() {
		return 'trusted_server_';
	}

}
?>