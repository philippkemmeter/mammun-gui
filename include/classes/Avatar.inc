<?php
require_once(dirname(__FILE__).'/../shared/classes/server_com/ClientServer.inc');
require_once(dirname(__FILE__).'/../_config.inc');

/**
 * Diese Klasse ist für das Verwalten von Avataren zuständig.
 *
 * @author Philipp Kemmeter
 */
class Avatar {

	/**
	 * Dieser Funktion gibt Avatare pro übergebenen Nick zurück.
	 *
	 * $all_avatar_nicks soll dabei ein Array von Nick-Names sein, die im
	 * Spiel registriert sind. Zurückgegeben wird dann eine stdClass bei
	 * dem jedem Nickname eine URI zugewiesen wird, die auf den Avatar zeigt.
	 * Die URI ist relativ zum mammun_gui_server-Projekt.
	 *
	 * @param string[] $all_avatar_nicks
	 * @return stdClass
	 */
	public static function get_avatars(array $all_avatar_nicks) {
		$n = count($all_avatar_nicks);
		$username_string = '';
		for ($i = 0; $i < $n; $i++) {
			$username_string .= "username[$i]={$all_avatar_nicks[$i]}&";
		}

		$avatar_data = ClientServer::send_request(CONF_WCF_SERVER_URL
			.'/gausus_wcf_hacks/avatar_interface.php?get_avatar=true'
			."&$username_string"
			.'&s='.$_SESSION['WCF_SID']);

		$avatar_data = $avatar_data[0];
		if (is_array($avatar_data) || is_object($avatar_data)) {
			foreach ($avatar_data as &$value) {
				if ($value[1]->w && $value[1]->h)
					$value = '../community/wcf/images/'.$value[0];
				else
					$value = pic_path('ui/icons/labels/avatars/SIZE/default.png');
			}
		}
		return $avatar_data;
	}

	public static function get_default_avatar($size) {
		return str_replace('SIZE', $size,
			'ui/icons/labels/avatars/SIZE/default.png');
	}
}
?>