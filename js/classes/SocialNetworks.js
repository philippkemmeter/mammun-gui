/**
 * Diese Klasse beinhaltet alle Funktionen, zusätzlich in sozialen Netzwerken
 * verfügbar sind.
 */

SocialNetworks = new Object();

SocialNetworks.in_fb = false;

/**
 * Facebook-spezifische Funktionen
 */
SocialNetworks.Facebook = new Object();

SocialNetworks.Facebook.LNG_WARNING_NOT_IN_FACEBOOK = '';

/**
 * Initiiert den Share-Dialog, um Leute auf die eigene Insel einzuladen.
 */
SocialNetworks.Facebook.share_invite_island = function() {
	window.open(
		"http://www.facebook.com/sharer.php?u="
			+"http%3A%2F%2Fwww.mammun.com%2Ffb_index_invite.php%3Fmap_key="
												+MapDataCtrl.auth_key
			+"%26lang="+MammunUI.LNG,
		'_blank', 'width=640,height=400,resizable=yes', true
	);
}

/**
 * Initiiert den Share-Dialog, um Leute generell zu Mammun einzuladen
 */
SocialNetworks.Facebook.share_general = function() {
	window.open(
		"http://www.facebook.com/sharer.php?u="
			+"http%3A%2F%2Fapps.facebook.com%2FMammunIslands%2F%3Fmap_key="
												+MapDataCtrl.auth_key
			+"%26lang="+MammunUI.LNG,
		'_blank', 'width=640,height=400,resizable=yes', true
	);
}

/**
 * Twitter-spezifische Funktionen
 */

SocialNetworks.Twitter = new Object();
SocialNetworks.Twitter.LNG_SHARE_GENERAL = '';
SocialNetworks.Twitter.LNG_SHARE_INVITE_TO_MAP = '';

SocialNetworks.Twitter.share_general = function() {
	window.open(
		"http://twitter.com/share?url=http%3A%2F%2Fwww.mammun.com%2F%3Fmap_key="
														+MapDataCtrl.auth_key
		+"&text="+SocialNetworks.Twitter.LNG_SHARE_GENERAL,
		'_blank', 'width=640,height=400,resizable=yes', true
	);
}

SocialNetworks.Twitter.share_invite_island = function() {
	window.open(
		"http://twitter.com/share?url=http%3A%2F%2Fwww.mammun.com%2F%3Fmap_key="
														+MapDataCtrl.auth_key
		+"&text="+SocialNetworks.Twitter.LNG_SHARE_INVITE_TO_MAP,
		'_blank', 'width=640,height=400,resizable=yes', true
	);
}