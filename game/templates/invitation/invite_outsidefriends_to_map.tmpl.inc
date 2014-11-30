<script>
FriendInvite = new Object();
FriendInvite.this_window = PureFW.ContainerAJAX.instances[<?=$this_win_id?>];

FriendInvite.destroy = function() {
	<? if (!$invite_general) : ?>
	if ((!FriendInvite.init_err) /** Nach 30min verfällt der einladungszwang*/
		&& ((PureFW.Time.get_current_server_time()
					- MapDataCtrl.my_map_start_time) < 1800) )
	{
		if (!FriendInvite.has_invited_somebody) {
			var box = MammunUI.show_warning(
				404, 325, '<?= LNG_YOU_HAVE_TO_INVITE ?>'.replace(
					'[MIN_AMOUNT]', 3),
				PureFW.ConfirmationBox.YES | PureFW.ConfirmationBox.NO, true
			);
			box.add_event_handler("confirm",
				UIButtons.open_friend_invitation
			);
			box.add_event_handler("cancel",
				function(ev) { MapUI.has_invited_somebody=true; }
			);
		}
		else {
			var box = MammunUI.show_warning(
				400, 300,
				'<?= LNG_INVITATION_SUCCESSFUL ?>'
					.replace('[DAYS]', <?=$timeout_days?>),
				PureFW.ConfirmationBox.YES, false
			);
			box.add_event_handler("destroy",
				function(ev) { MapUI.has_invited_somebody=true; }
			);
		}
	}
	<? endif; ?>

	FriendInvite.this_window.remove_event_handler("destroy", FriendInvite.destroy);
	FriendInvite.this_window.remove_event_handler("change", FriendInvite.destroy);

	try {
		destroy_object(FriendInvite);
	}
	catch (e) {}
	FriendInvite = null;

};
FriendInvite.this_window.set_css_class('ConfirmationBox');

FriendInvite.has_invited_somebody = false; //MapUI.has_invited_somebody;

FriendInvite.fb_friends = <?= json_encode($fb_friends) ?>;
FriendInvite.email_invite_conf = null;
FriendInvite.email_submit_button = null;

<?
/**
 * Öffnet den Dialog, um Freunde aus FB einzuladen.
 */
?>
FriendInvite.facebook_invite_dialog = function() {
	if(!MammunUI.user_in_facebook) {
		MammunUI.show_warning(
			350, 200, '<?= str_replace(
				array('[A]', '[/A]'),
				array('<a href="http://apps.facebook.com/mammunIslands">','</a>'),
				LNG_WARNING_NOT_IN_FACEBOOK) ?>',
			PureFW.ConfirmationBox.NO
		);
	}
	else {
		FriendInvite.has_invited_somebody = true;
		<?
		$friend_arr = array();
		if ($fb_invite_excludes) {
			foreach($fb_invite_excludes as $friend) {
				$friend_arr[] = $friend->fb_id;
			}
		}
		$friend_str = implode(',', $friend_arr);
		?>
		<? if ($invite_general) : ?>
		MammunUI.get_top_frame().parent.open_facebook_dialog(
			'<?=$friend_str?>', MapDataCtrl.auth_key,
			'<?= LNG_FB_INVITE_MAMMUN_GENERAL_INSTRUCTIONS ?>',
			'<?= LNG_FB_INVITE_MAMMUN_GENERAL_CAPTION ?>',
			'<?= CONF_MY_SERVER_URL ?>/game/invitation_fb_feedback.php');
		<? else : ?>
		MammunUI.get_top_frame().parent.open_facebook_dialog(
			'<?=$friend_str?>', MapDataCtrl.auth_key,
			'<?= LNG_FB_INVITE_MAMMUN_MAP_INSTRUCTIONS ?>',
			'<?= LNG_FB_INVITE_MAMMUN_MAP_CAPTION ?>',
			'<?= CONF_MY_SERVER_URL ?>/game/invitation_fb_feedback.php');
		<? endif; ?>
	}
}

<?
/**
 * Öffnet den Dialog, um E-Mail-Einladungen zu versenden
 */
?>
FriendInvite.email_invite_dialog = function() {
	if (FriendInvite.email_invite_conf) {
		try {
			FriendInvite.email_invite_conf.destroy();
		}
		catch(e){}
		FriendInvite.email_invite_conf = null;
	}
	FriendInvite.email_invite_conf = MammunUI.create_conf_box(480, 410,
		PureFW.ConfirmationBox.NO_BUTTONS);

	var conf = FriendInvite.email_invite_conf;

	var headline = conf.add_widget(
		PureFW.Image,
		(conf.width - 418) >> 1,
		30,
		418, 28,
		'ui/elements/texts/<?=
			$LNG ?>/invitation/headline_invite_friends_by_mail.png'
	);

	var von = conf.add_widget(
		PureFW.Container,
		10,
		headline.position.y + headline.height + 25,
		125,
		20
	);
	von.set_content('<b><?= LNG_EMAIL_FIELD_SENT_BY ?>:</b>');
	von.set_text_align('right');
	var my_mailadd = conf.add_widget(
		PureFW.Container,
		140,
		von.position.y,
		conf.width - 140 - 5,
		40
	);
	my_mailadd.set_content(
		'<?= $current_user->get_nick()
			?> &lt;<?= $current_user->get_email() ?>&gt;'
	);

	var to = conf.add_widget(
		PureFW.Container,
		10,
		von.position.y + von.height + 15,
		125,
		20
	);
	to.set_content('<b><?= LNG_EMAIL_FIELD_SENT_TO ?>:</b>');
	to.set_text_align('right');

	var seperate_by_comma = conf.add_widget(
		PureFW.Container,
		10,
		to.position.y + to.height + 4,
		125,
		100
	);
	seperate_by_comma.set_font_size(0.9);
	seperate_by_comma.set_text_align('right');
	seperate_by_comma.set_content('<?= LNG_SEPERATE_MAILS_BY_COMMA ?>');
	seperate_by_comma.set_font_color('#999');


	var address_container = conf.add_widget(
		PureFW.Container,
		my_mailadd.position.x,
		to.position.y,
		287, 165
	);
	address_container.set_bg_img('ui/elements/form/input/textarea_287x165.png');

	FriendInvite.address_textarea = address_container.add_widget(
		PureFW.TextArea,
		3, 3,
		address_container.width, address_container.height
	);
	FriendInvite.address_textarea.set_font_color("#333333");
	FriendInvite.address_textarea.set_font_size(0.9);

	var mail_agb = conf.add_widget(
		PureFW.Container,
		address_container.position.x,
		address_container.position.y + address_container.height + 10,
		address_container.width,
		40
	);
	mail_agb.set_font_color('#999');
	mail_agb.set_font_size(0.9);
	mail_agb.set_content('<?= LNG_EMAIL_TERMS ?>');

	FriendInvite.email_submit_button = conf.add_widget(
		PureFW.Image,
		address_container.position.x,
		mail_agb.position.y + mail_agb.height,
		288, 44,
		'ui/elements/form/buttons/de/invitation/send_invitations.png'
	);

	FriendInvite.email_submit_button.add_event_handler("click",
		function(ev) {
			FriendInvite.submit_email();
		}
	);
}

<?
/**
 * verschickt emails
 */
?>
FriendInvite.submit_email = function() {
	FriendInvite.email_submit_button.deactivate();
	var emails = FriendInvite.address_textarea.get_text();
	if (emails.length < 10) {
		var box = MammunUI.show_warning(
			350, 200,
			'<?= LNG_EMAILS_ABORT_CONFIRM ?>'
		);
		box.add_event_handler("confirm",
			function(ev) {
				try {
					FriendInvite.email_invite_conf.destroy();
				}
				catch(e){}
				FriendInvite.email_invite_conf = null;
			}
		);
		FriendInvite.email_submit_button.activate();
		return;
	}

	MammunUI.show_warning(
		350, 200,
		'<?= LNG_TEXT_PLEASE_WAIT_WHILE_SENDING ?>',
		PureFW.ConfirmationBox.NO_BUTTONS
	);

	var post_string = "send=true&email="+emails;
	<? if ($invite_general) : ?>
	post_string += "&general_invitation=true";
	<? endif; ?>
	PureFW.AJAXClientServer.send_request(
		"invitation2.php",
		FriendInvite.submit_email_response,
		post_string
	);

}
FriendInvite.submit_email_response = function(response_arr) {
	if (response_arr[0] == 1) {
		MammunUI.show_warning(
			350, 200,
			'<?= LNG_EMAILS_SENT_SUCCESSFULLY ?>',
			PureFW.ConfirmationBox.YES
		);
		try {
			FriendInvite.email_invite_conf.destroy();
		}
		catch(e){}
		FriendInvite.email_invite_conf = null;
		FriendInvite.has_invited_somebody = true;
		return;
	}
	else if (response_arr[0] == 0) {
		MammunUI.show_warning(
			350, 200,
			'<?= LNG_ERR_EMAIL_INVALID ?>',
			PureFW.ConfirmationBox.NO,
			true
		);
	}
	else {
		throw new Error(response_arr);
	}
	FriendInvite.email_submit_button.activate();
}


<?
/**
 * Öffnet den Dialog, um Freunde aus Mammun einzuladen.
 */
?>
FriendInvite.mammun_invite_dialog = function() {
		FriendInvite.has_invited_somebody = true;
		UIButtons.open_friend_invitation_friend_popup();
}

<?
/******************************************************************************
 *                  /\     .---.    -----    -     .---.   .  .               *
 *                 /__\    |          |      |     |   |   |`.|               *
 *                /    \   `---'      -      -     `---'   '  '               *
 *                                                                            *
 *                             A  C  T  I  O  N                            PK *
 ******************************************************************************/
?>

FriendInvite.friend_caption = FriendInvite.this_window.add_widget(
	PureFW.Image,
	<? if ($invite_general) : ?>
	(FriendInvite.this_window.width - 388) >> 1, 10,
	388, 28,
	'ui/elements/texts/<?=
		$LNG ?>/invitation/headline_invite_outside_friends_to_mammun.png'
<? else : ?>
		(FriendInvite.this_window.position.x +20), 10,
		388, 28,
	'ui/elements/texts/<?=
		$LNG ?>/invitation/headline_invite_outside_friends_to_island.png'
<? endif; ?>
);

FriendInvite.link_to_island = FriendInvite.this_window.add_widget(
	PureFW.Container,
	FriendInvite.friend_caption.position.x,
	FriendInvite.friend_caption.position.y
		+ FriendInvite.friend_caption.height + 8,
	388, 60
);
FriendInvite.link_to_island.set_bg_img(
	'ui/backgrounds/invitation/link_to_island_bg.png'
);
FriendInvite.link_to_island_headline = FriendInvite.link_to_island.add_widget(
	PureFW.Image,
	10, 10,
	160, 17,
	'ui/elements/texts/<?= $LNG ?>/invitation/link_to_your_island.png'
);
FriendInvite.link_to_island_link = FriendInvite.link_to_island.add_widget(
	PureFW.Container,
	FriendInvite.link_to_island_headline.position.x,
	FriendInvite.link_to_island_headline.position.y
		+ FriendInvite.link_to_island_headline.height + 5,
	0,0
);
FriendInvite.link_to_island_link.set_content(
	'<b>http://isle.mn/<?= User::get_current_user()->get_nick() ?></b>'
);
FriendInvite.link_to_island_help = FriendInvite.link_to_island.add_widget(
	PureFW.Container,
	FriendInvite.link_to_island.width - 100,
	FriendInvite.link_to_island_link.position.y,
	100, 0
);
FriendInvite.link_to_island_help.set_font_size(0.9);
FriendInvite.link_to_island_help.set_content(
	'<a href="javascript: UIButtons.show_link_to_island_help();"><?=
		LNG_LINK_TO_ISLAND_WHAT ?></a>'
);
<?
/**
 * Einladungsboxen. 1. FB (links)
 */
?>
if(<?= ($direct_to_map) ? 'true' : 'false' ?> == true)
{
	FriendInvite.fb_invite_box = FriendInvite.this_window.add_widget(
		PureFW.Container,
		FriendInvite.friend_caption.position.x,
		FriendInvite.link_to_island.position.y
			+ FriendInvite.link_to_island.height
			+ 7,
		192, 212
	);
}
else
{
	FriendInvite.fb_invite_box = FriendInvite.this_window.add_widget(
			PureFW.Container,
			FriendInvite.friend_caption.position.x,
			FriendInvite.link_to_island.position.y
				+ FriendInvite.link_to_island.height
				+ 7,
			192, 212
		);
}
FriendInvite.fb_invite_box.set_bg_img(
	'ui/backgrounds/invitation/paper_bg_small.png'
);
FriendInvite.fb_total_inner_width = 154; // =48*3 + 5*2;
FriendInvite.fb_logo = FriendInvite.fb_invite_box.add_widget(
	PureFW.Image,
	(FriendInvite.fb_invite_box.width - FriendInvite.fb_total_inner_width) >> 1,
	15,
	46, 46,
	'ui/elements/logos/facebook46x46.png'
);
FriendInvite.fb_invite_cap = FriendInvite.fb_invite_box.add_widget(
	PureFW.Container,
	FriendInvite.fb_logo.position.x + FriendInvite.fb_logo.width + 4,
	18,
	FriendInvite.fb_total_inner_width - FriendInvite.fb_logo.width - 4,
	40
);
FriendInvite.fb_invite_cap.set_content(
	'<b><?= LNG_INVITATION_BY_FB ?></b>'
);
(function() {	<? /** local stack :) */ ?>
	for (var x = 0, i = 0; x < 3; x++) {
		for (var y = 0; y < 2; y++, i++) {
			FriendInvite.fb_invite_box.add_widget(
				PureFW.Avatar.Avatar,
				FriendInvite.fb_logo.position.x + x*53,
				FriendInvite.fb_logo.position.y + FriendInvite.fb_logo.height
					+ 5 + y*53,
				48, 48,
				FriendInvite.fb_friends[i].avatar
			);
		}
	}
})()

FriendInvite.fb_invite_link = FriendInvite.fb_invite_box.add_widget(
	PureFW.Container,
	0,
	FriendInvite.fb_invite_box.height - 40,
	FriendInvite.fb_invite_box.width,
	20
);
FriendInvite.fb_invite_link.set_text_align('center');
FriendInvite.fb_invite_link.set_content(
	'<b><a href="javascript: ;"><?= LNG_BTN_INVITE_FRIENDS ?></a></b>'
);

FriendInvite.fb_invite_box.add_event_handler("click",
	FriendInvite.facebook_invite_dialog
);
<?
/**
 * Einladungsboxen. 2. E-Mail (mitte)
 */
?>
FriendInvite.email_box = FriendInvite.this_window.add_widget(
	PureFW.Container,
	FriendInvite.fb_invite_box.position.x + FriendInvite.fb_invite_box.width+4,
	FriendInvite.fb_invite_box.position.y,
	192, 212
);
FriendInvite.email_box.set_bg_img(
	'ui/backgrounds/invitation/paper_bg_small.png'
);

FriendInvite.email_cap = FriendInvite.email_box.add_widget(
	PureFW.Container,
	0, 18,
	FriendInvite.email_box.width,
	40
);
FriendInvite.email_cap.set_text_align('center');
FriendInvite.email_cap.set_content(
	'<b><?= LNG_INVITATION_BY_MAIL ?></b>'
);

FriendInvite.email_img = FriendInvite.email_box.add_widget(
	PureFW.Image,
	(FriendInvite.email_box.width - 82) >> 1,
	FriendInvite.email_cap.position.y + FriendInvite.email_cap.height,
	82, 90,
	'ui/icons/labels/communication/82x90/email.png'
);

FriendInvite.email_link = FriendInvite.email_box.add_widget(
	PureFW.Container,
	0,
	FriendInvite.email_box.height - 40,
	FriendInvite.email_box.width,
	20
);
FriendInvite.email_link.set_text_align('center');
FriendInvite.email_link.set_content(
	'<b><a href="javascript: ;"><?= LNG_BTN_INVITE_FRIENDS ?></a></b>'
);
FriendInvite.email_box.add_event_handler("click",
	FriendInvite.email_invite_dialog
);


<?
/*
*Einladungsboxen. 3. Mammun (rechts)
*/
?>
if(<?= ($direct_to_map) ? 'true' : 'false' ?> == true)
{

		FriendInvite.mammun_invite_box = FriendInvite.this_window.add_widget(
			PureFW.Container,
			FriendInvite.email_box.position.x + FriendInvite.email_box.width+4,
			FriendInvite.fb_invite_box.position.y,
			192, 212
		);
		FriendInvite.mammun_invite_box.set_bg_img(
			'ui/backgrounds/invitation/paper_bg_small.png'
		);
		FriendInvite.mammun_total_inner_width = 154; // =48*3 + 5*2;
		FriendInvite.mammun_logo = FriendInvite.mammun_invite_box.add_widget(
			PureFW.Image,
			(FriendInvite.mammun_invite_box.width - FriendInvite.mammun_total_inner_width) >> 1,
			20,
			44, 37,
			'ui/elements/logos/mammun44x37.png'
		);
		FriendInvite.mammun_invite_cap = FriendInvite.mammun_invite_box.add_widget(
			PureFW.Container,
			FriendInvite.mammun_logo.position.x + FriendInvite.mammun_logo.width + 4,
			18,
			FriendInvite.mammun_total_inner_width - FriendInvite.mammun_logo.width - 4,
			40
		);
		FriendInvite.mammun_invite_cap.set_content(
			'<b><?= LNG_INVITATION_BY_MAMMUN ?></b>'
		);

		FriendInvite.init_from_pause_menu = function() {
			FriendInvite.buddies = PauseMenu.buddies;
			FriendInvite.avatars = PauseMenu.avatars;

			<? /**
			    * Das sind alle Mammun-Freunde. Sie liegen als einfaches Array
			    * der Nicks der Freund vor.
			    */ ?>
			FriendInvite.all_friends = new Array();
			for (var x in FriendInvite.buddies.offlineFriends) {
				if (isNaN(x))
					continue;

				FriendInvite.all_friends.push(
					FriendInvite.buddies.offlineFriends[x].nick
				);
			}
			for (var x in FriendInvite.buddies.onlineFriends) {
				if (isNaN(x))
					continue;

				FriendInvite.all_friends.push(
					FriendInvite.buddies.onlineFriends[x].nick
				);
			}

			for (var x = 0, i = 0; x < 3; x++) {
				for (var y = 0; y < 2; y++, i++) {
					var avatar = (FriendInvite.all_friends[i])
						? FriendInvite.avatars[FriendInvite.all_friends[i]]
						: '';
					FriendInvite.mammun_invite_box.add_widget(
						PureFW.Avatar.Avatar,
						FriendInvite.mammun_logo.position.x + x*53,
						FriendInvite.mammun_logo.position.y
							+ FriendInvite.mammun_logo.height
							+ 9 + y*53,
						48, 48,
						avatar
					);
				}
			}
		}

		if (!((typeof(PauseMenu) != 'undefined') && PauseMenu && PauseMenu.loaded)) {
			FriendInvite.waiting_cycle = function () {
				if ((typeof(PauseMenu) != 'undefined') && PauseMenu
					&& PauseMenu.loaded)
				{
					PureFW.Timeout.clear_interval(FriendInvite.waiting_interval);
					FriendInvite.init_from_pause_menu();
				}
			}
			FriendInvite.waiting_interval = PureFW.Timeout.set_interval(
				FriendInvite.waiting_cycle, 500
			);
		}
		else {
			FriendInvite.init_from_pause_menu();
		}


		FriendInvite.mammun_invite_link = FriendInvite.mammun_invite_box.add_widget(
			PureFW.Container,
			0,
			FriendInvite.mammun_invite_box.height - 40,
			FriendInvite.mammun_invite_box.width,
			20
		);
		FriendInvite.mammun_invite_link.set_text_align('center');
		FriendInvite.mammun_invite_link.set_content(
			'<b><a href="javascript: ;"><?= LNG_BTN_INVITE_FRIENDS ?></a></b>'
		);

		FriendInvite.mammun_invite_box.add_event_handler("click",
			FriendInvite.mammun_invite_dialog
		);
}

FriendInvite.share_your_map_cont = FriendInvite.this_window.add_widget(
	PureFW.Container,
	FriendInvite.fb_invite_box.position.x,
	FriendInvite.fb_invite_box.position.y
		+ FriendInvite.fb_invite_box.height
		+ 10,
	0, 0
);
FriendInvite.share_your_map_cont.set_content(
	'<?= LNG_SHARE_YOUR_MAP ?>'
);
FriendInvite.share_your_map_cont.set_font_weight('bold');

FriendInvite.share_fb_btn = FriendInvite.this_window.add_widget(
	PureFW.Image,
	FriendInvite.share_your_map_cont.position.x + 5,
	FriendInvite.share_your_map_cont.position.y + 30,
	122, 30,
	'ui/elements/form/buttons/btn_facebook_122x30.png'
);

FriendInvite.share_fb_btn.add_event_handler("click",
	function (ev) {
	<? if ($invite_general) : ?>
		SocialNetworks.Facebook.share_general();
	<? else : ?>
		SocialNetworks.Facebook.share_invite_island();
	<? endif; ?>
	}
);

FriendInvite.share_twitter_btn = FriendInvite.this_window.add_widget(
	PureFW.Image,
	FriendInvite.email_box.position.x,
	FriendInvite.share_fb_btn.position.y,
	122, 30,
	'ui/elements/form/buttons/btn_twitter_122x30.png'
);
FriendInvite.share_twitter_btn.add_event_handler("click",
	function (ev) {
	<? if ($invite_general) : ?>
		SocialNetworks.Twitter.share_general();
	<? else : ?>
		SocialNetworks.Twitter.share_invite_island();
	<? endif; ?>
	}
);

<? if (!$invite_general) : ?>
FriendInvite.init_err = null;
if (MapDataCtrl.my_players_on_map_amount >= MapDataCtrl.my_max_players) {
	FriendInvite.init_err = MammunUI.show_warning(
		350, 200,
		'<?= LNG_NO_INVITATIONS_POSSIBLE_FULL ?>',
		PureFW.ConfirmationBox.YES, // | PureFW.ConfirmationBox.NO,
		false
	);
}
else if (MapDataCtrl.my_map_is_closed) {
	FriendInvite.init_err = MammunUI.show_warning(
		350, 200,
		'<?= LNG_NO_INVITATIONS_POSSIBLE ?>'
			.replace('[DAYS]', <?= $timeout_days?>),
		PureFW.ConfirmationBox.YES, // | PureFW.ConfirmationBox.NO,
		false
	);
}
if (FriendInvite.init_err) {
	FriendInvite.init_err.add_event_handler("destroy",
/*		function(ev) { InvitationWindow.destroy(); } );
	FriendInvite.init_err.add_event_handler("confirm",*/
		function(ev) {
			InvitationWindow.set_url('invitation.php?general_invitation=true');
		}
	);
}
<? endif; ?>
FriendInvite.this_window.add_event_handler("destroy", FriendInvite.destroy);
FriendInvite.this_window.add_event_handler("change", FriendInvite.destroy);
FriendInvite.loaded = true;
</script>
