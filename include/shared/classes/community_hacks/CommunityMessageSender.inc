<?
require_once(dirname(__FILE__).'/../exceptions/IllegalArgumentException.inc');
require_once(dirname(__FILE__).'/../server_com/ClientServer.inc');

/**
 * Diese Klasse erlaubt Nachrichten über die Community an Spieler zu senden.
 * So können z.B. Systemnachrichten verschickt werden oder Nachrichten von NSCs.
 *
 * @author Philipp Kemmeter
 */
class CommunityMessageSender {
	private function __construct(){}

	const SENDER_BERNARD = "Bernard";
	const SENDER_SYSTEM = "Admin";
	const SENDER_OMA = "Oma";
	const SENDER_OPA = "Opa";

	/**
	 * Versendet eine Nachricht über die Community.
	 *
	 * @param string $recipient
	 * @param string $subject
	 * @param string $text
	 * @param string $sender	[nur SENDER_-Konstanten erlaubt]
	 * @throws Exception
	 */
	public static function send_message($recipient, $subject, $text, $sender) {
		if (($sender != self::SENDER_BERNARD) &&
			($sender != self::SENDER_SYSTEM) &&
			($sender != self::SENDER_OMA) &&
			($sender != self::SENDER_OPA))
		{
			throw new IllegalArgumentException("sender",
				"[CommunityMessageSender::SENDER_...]", $sender);
		}
		$text = str_replace(
			array("<br>", "<br/>"), array("\n", "\n") , $text
		);

		$url = CONF_WCF_SERVER_URL.'/gausus_wcf_hacks/pm_interface.php';
		$post_data = 	"recipients[0]=$recipient".
						"&subject=".urlencode($subject).
						"&text=".urlencode($text).
						"&sender=$sender";
		$x = trim(ClientServer::send_request($url.'?'.$post_data, true));
		if (!empty($x)) {
			throw new Exception($x);
		}
	}
}
?>