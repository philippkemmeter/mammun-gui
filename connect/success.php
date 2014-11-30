<?
require_once('../include/_config.inc');
require_once('../include/shared/classes/server_com/ClientServer.inc');

if ($_REQUEST['code']) {
	$url = "https://graph.facebook.com/oauth/access_token?client_id="
		.CONF_FACEBOOK_APP_ID.
		"&redirect_uri=http://www.mammun.com/connect/success.php&client_secret="
		.CONF_FACEBOOK_API_SECRET."&code=".$_REQUEST['code'];
	$ch = curl_init();
	$opts[CURLOPT_URL] = $url;
	$opts[CURLOPT_RETURNTRANSFER] = true;
	curl_setopt_array($ch, $opts);
	$result = curl_exec($ch);
	curl_close($ch);
	$access_token = explode('&', $result);
	$access_token = explode('=', $access_token[0]);
	$access_token = $access_token[1];

	$url
		= "https://graph.facebook.com/me/?access_token=$access_token&fields=id";
	$ch = curl_init();
	$opts[CURLOPT_URL] = $url;
	$opts[CURLOPT_RETURNTRANSFER] = true;
	curl_setopt_array($ch, $opts);
	$result = curl_exec($ch);
	curl_close($ch);
	$uid = json_decode($result, true);
	$uid = $uid['id'];

	$params = array(
		"access_token"	=> $access_token,
		"uid"			=> $uid
	);

	// work with sorted data
	ksort($params);

	// generate the base string
	$base_string = '';
	foreach($params as $key => $value) {
		$base_string .= $key . '=' . $value;
	}
	$base_string .= CONF_FACEBOOK_API_SECRET;
	$sig = md5($base_string);

	/**
	 * Wir versuchen jetzt direkt den FB-Account mit dem Mammun-Account zu
	 * verbinden, falls das noch nicht geschehen ist und der User das wünscht.
	 * Dazu muss eine gültige Session aktiv sein, sonst wissen wir nicht,
	 * welchen User wir im Hintergrund connecten sollen.
	 */
	if ($_REQUEST['try_connect']) {
		require_once ('../include/std_include.inc');
		session_start();
		try {
			$user = User::get_current_user();
		}
		catch (Exception $e) {
			/**
			 * Kein gültiger aktiver User (wahrscheinlich keine Session)
			 * Da kann man nix machen => abbruch.
			 */
		}
		if ($user) {
			try {
				Server::get_instance(CONF_ENGINE_SERVER_URL)
					->cmd_connect_account_with_fb($uid, $user->get_nick(),
						$user->get_pass());
			}
			catch (Exception $e) {
				/**
				 * Ignore: User ist schon verbunden mit einem Account,
				 * wahrscheinlich demjenigen, zudem er sich verbinden wollte,
				 * also machen wir nichts.
				 *
				 * Wenn es ein anderer Account sein sollte, können wir die
				 * Situation auch nicht retten ^^
				 */
			}
		}
	}
}
?>
<html>
<head>
<script type="text/javascript">
<!--

var r = new Object();

r.session = '{"access_token": "<?= urlencode($access_token) ?>", "uid": "<?= $uid?>", "sig": "<?= $sig ?>"}';
window.opener.login_fb_c_callback(r);
window.close();
//-->
</script>
</head>
<body>
</body>
</html>

