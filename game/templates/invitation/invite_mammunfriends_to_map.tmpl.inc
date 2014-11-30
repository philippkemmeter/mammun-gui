<script>
FriendInviteMF = new Object();

FriendInviteMF.destroy = function()
{
	FriendInviteMF.this_window.remove_event_handler("destroy", FriendInviteMF.destroy);
	FriendInviteMF.this_window.remove_event_handler("change", FriendInviteMF.destroy);
	try
	{
		destroy_object(FriendInviteMF);
		FriendInviteMF = null;
	}
	catch (e) {}
};
FriendInviteMF.this_window = PureFW.ContainerAJAX.instances[<?=$this_win_id?>];
FriendInviteMF.this_window.set_font_size(1.3);
FriendInviteMF.this_window.set_font_color('#333');
FriendInviteMF.this_window.set_text_shadow('#fff', 1, 0, 0);


FriendInviteMF.minimum_level = <?= (int)$this_world->minimum_level ?>;
FriendInviteMF.invited_friends = <?= json_encode($invited_friends) ?>;

FriendInviteMF.avatar_cb_instances = new Array();

FriendInviteMF.warning_cont = null;
<?
/**
 * Es kann sein, dass diese Seite aufpoppt, bevor das Pausemenü geladen ist.
 * Wir brauchen aber Daten vom Pausemenü und wir sind uns auch sicher, dass
 * das Pausemenü noch geladen wird im Hintergrund. Deswegen können wir warten.
 */
?>
FriendInviteMF.all_friends = null;
FriendInviteMF.init_from_pause_menu = function() {
	FriendInviteMF.buddies = PauseMenu.buddies;
	FriendInviteMF.avatars = PauseMenu.avatars;
	FriendInviteMF.xps = PauseMenu.xps;
	FriendInviteMF.players_on_map = <?= json_encode($nicks_on_map) ?>;

	<? /**
	    * Das sind alle Mammun-Freunde. Sie liegen als einfaches Array der Nicks
	    * der Freund vor.
	    *
	    * Außerdem werden nur die Aufgenommen, die auch genug Level haben,
	    * um eingeladen werden zu können.
	    */ ?>
	FriendInviteMF.all_friends = new Array();
	for (var x in FriendInviteMF.buddies.offlineFriends) {
		if (isNaN(x))
			continue;
		if  (FriendInviteMF.xps[FriendInviteMF.buddies.offlineFriends[x].nick]
												< FriendInviteMF.minimum_level)
			continue;
		var n = FriendInviteMF.players_on_map.length;
		var skip = false;
		for (var i = 0; i < n; i++) {
			if (FriendInviteMF.players_on_map[i]
								== FriendInviteMF.buddies.offlineFriends[x].nick)
			{
				skip = true;
				break;
			}
		}
		if (skip)
			continue;

		FriendInviteMF.all_friends.push(
			FriendInviteMF.buddies.offlineFriends[x].nick
		);
	}
	for (var x in FriendInviteMF.buddies.onlineFriends) {
		if (isNaN(x))
			continue;
		if  (FriendInviteMF.xps[FriendInviteMF.buddies.onlineFriends[x].nick]
												< FriendInviteMF.minimum_level)
			continue;
		var n = FriendInviteMF.players_on_map.length;
		var skip = false;
		for (var i = 0; i < n; i++) {
			if (FriendInviteMF.players_on_map[i]
								== FriendInviteMF.buddies.offlineFriends[x].nick)
			{
				skip = true;
				break;
			}
		}
		if (skip)
			continue;

		FriendInviteMF.all_friends.push(
			FriendInviteMF.buddies.onlineFriends[x].nick
		);
	}
}

if (!((typeof(PauseMenu) != 'undefined') && PauseMenu && PauseMenu.loaded)) {
	FriendInviteMF.waiting_cycle = function () {
		if ((typeof(PauseMenu) != 'undefined') && PauseMenu
			&& PauseMenu.loaded)
		{
			PureFW.Timeout.clear_interval(FriendInviteMF.waiting_interval);
			FriendInviteMF.init_from_pause_menu();
		}
	}
	FriendInviteMF.waiting_interval = PureFW.Timeout.set_interval(
		FriendInviteMF.waiting_cycle, 500
	);
}
else {
	FriendInviteMF.init_from_pause_menu();
}

<?
/**
 * Füllt den Container, wo die Freunde angezeigt werden mit entsprechend diesen.
 * Avatar, Name und Checkbox werden generiert.
 */
?>
FriendInviteMF.create_friends_list = function() {
	if (!FriendInviteMF.all_friends) {
		FriendInviteMF.waiting_cycle2 = function () {
			if (FriendInviteMF.all_friends) {
				PureFW.Timeout.clear_interval(FriendInviteMF.waiting_interval2);
				FriendInviteMF.create_friends_list();
			}
		}
		FriendInviteMF.waiting_interval2 = PureFW.Timeout.set_interval(
			FriendInviteMF.waiting_cycle2, 500
		);
		return;
	}
	try {
		FriendInviteMF.avatar_cb_instances.destroy();
	}
	catch(e) {}
	FriendInviteMF.avatar_cb_instances = new Array();

	var n = FriendInviteMF.all_friends.length;
	for (var i = 0; i < n; i+=3) {
		var avatar_args = new Array();
		for (var row = 0, y = 0; row < 3; row++, y+=50) {
			var b = FriendInviteMF.all_friends[i+row];
			if (!b)
				break;
			avatar_args.push({
				x: 0, y: y,
				w: 150, h: 48,
				pic: FriendInviteMF.avatars[b],
				nick: b
			});
		}
		FriendInviteMF.all_friends_container.add_item(
			PureFW.Container,
			[ 150, 150 ],
			(function(_init) {
				return function() {
					var n = _init.length;
					for (var i = 0; i < n; i++) {
						var args = _init[i];
						var tmp = this.add_widget(
							PureFW.Avatar.AvatarXP, args.x, args.y,
								args.w, args.h, args.pic, args.nick, null,
								true
						);
						tmp.set_xp_level(FriendInviteMF.xps[args.nick]);

						<? /* Deaktivieren, wenn bereits eingeladen */ ?>
						if (FriendInviteMF.invited_friends.ingame.contains(
																	args.nick))
						{
							tmp.deactivate();
						}
						else
							tmp.check();
						FriendInviteMF.avatar_cb_instances.push(tmp);
					}
				}
			})(avatar_args)
		);
	}
}

<? /**
    * Gibt die Nicks der ausgewählten Mammun-Freunde zurück.
    *
    * @return string[]
    */ ?>
FriendInviteMF.get_chosen_friends = function()
{
	var result_nicks = new Array();
	var n = FriendInviteMF.avatar_cb_instances.length;
	for(var i = 0; i < n; i++)
	{
		if(FriendInviteMF.avatar_cb_instances[i].is_checked())
		{
			result_nicks.push(
				FriendInviteMF.avatar_cb_instances[i].get_nick()
			);
		}
	}
	return result_nicks;
}

<? /**
    * Deaktiviert die ausgewählten Freunde
    *
    * Dabei ist es egal, ob Mammun-Freund oder FB-Freund, weil einfach alles,
    * was angehakt ist, deaktiviert wird ^^
    */ ?>
FriendInviteMF.deactivate_chosen_friends = function() {
	var n = FriendInviteMF.avatar_cb_instances.length;
	for(var i = 0; i < n; i++)
	{
		if(FriendInviteMF.avatar_cb_instances[i].is_checked())
		{
			FriendInviteMF.avatar_cb_instances[i].uncheck();
			FriendInviteMF.avatar_cb_instances[i].deactivate();
		}
	}
}

<? /**
    * Sendet das ausgefüllte Formular an den Server
    *
    * @param Event ev
    * @see FriendInviteMF.submit_response
    */ ?>
FriendInviteMF.submit_form = function(ev, force)
{
	FriendInviteMF.finish_button.deactivate();
	try
	{
		ev = PureFW.EventUtil.formatEvent(ev);
	}
	catch(e) {}
	ev.preventDefault();

	var chosen_friends = FriendInviteMF.get_chosen_friends();
	var chosen_friend_string = '';

	for(var i = 0; i < chosen_friends.length; i++)
	{
		chosen_friend_string += chosen_friends[i];
		chosen_friend_string += ',';
	}
	chosen_friend_string = chosen_friend_string.substr(0,
		chosen_friend_string.length-1);

	if ((chosen_friends.length < 3)
		&& (FriendInviteMF.all_friends.length > 3)
		&&!force)
	{
		var cont = MammunUI.show_warning(
			350, 200,
			'<?= LNG_YOU_SHOULD_INVITE_MORE ?>'.replace('[MIN_AMOUNT]',
				3).replace('[AMOUNT]', chosen_friends.length),
			PureFW.ConfirmationBox.YES | PureFW.ConfirmationBox.NO,
			true
		);
		cont.add_event_handler("confirm",
			(function(_ev) {
				return function(ev) {
					FriendInviteMF.submit_form(_ev, true);
				}
			})(ev)
		);
		FriendInviteMF.finish_button.activate();
		return;
	}

	if (chosen_friends.length == 0) {
		InvitationWindow.destroy();
	}
	MammunUI.show_warning(
		350, 200,
		'<?= LNG_TEXT_PLEASE_WAIT_WHILE_SENDING ?>',
		PureFW.ConfirmationBox.NO_BUTTONS
	);

	var post_string = 	"send=true"+
						"&facebook_send=false"+
						"&chosen_friends="+chosen_friend_string;

	PureFW.AJAXClientServer.send_request(
			"../game/invitation2.php",
			FriendInviteMF.submit_response,
			post_string
	);

}

<? /**
    * Wird durch FriendInviteMF.submit_form indirekt aufgerufen. Wickelt die
    * Server-Antwort von FriendInviteMF.submit_form ab.
    *
    * @param string[] response_arr
    * @see FriendInviteMF.submit_form
    */ ?>
FriendInviteMF.submit_response = function(response_arr)
{
	if (response_arr[0] == 0)
	{
		var cont = MammunUI.show_warning(
			350, 200,
			response_arr[1],
			PureFW.ConfirmationBox.NO,
			true
		);
	}
	else if (response_arr[0] == 1)
	{
		var cont = MammunUI.show_warning(
			360, 255,
			response_arr[1],
			PureFW.ConfirmationBox.YES,
			false
		);
		cont.add_event_handler("confirm",
			function(ev) {
				InvitationWindow.destroy();
			}
		);
	}
	else
	{
		throw Error(response_arr[0]);
	}
	FriendInviteMF.finish_button.activate();
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

FriendInviteMF.friend_caption = FriendInviteMF.this_window.add_widget(
	PureFW.Image,
	40, 10,
	455, 28,
	'ui/elements/texts/<?= $LNG
		?>/invitation/headline_invite_friends_to_island.png'
);

FriendInviteMF.all_friends_background
	= FriendInviteMF.this_window.add_widget
(
	PureFW.Container,
	(FriendInviteMF.this_window.width - 496) >> 1,
	FriendInviteMF.friend_caption.position.y
		+ FriendInviteMF.friend_caption.height
		+ 20,
	496, 300
);
FriendInviteMF.all_friends_background.set_bg_img(
	'ui/backgrounds/choose_map/friends_list_new_map.png');

FriendInviteMF.all_friends_background.set_text_align('center');
FriendInviteMF.all_friends_background.add_content(
	'<br/><span style="color: #aaa"><?= LNG_HIGH_LEVEL_PLAYERS_ONLY ?></span>'
);
FriendInviteMF.all_friends_container
	= FriendInviteMF.all_friends_background.add_widget
(
	PureFW.BubbleScroller,
	(FriendInviteMF.all_friends_background.width - 480)>>1,
	30,
	480, 260
);

FriendInviteMF.create_friends_list();


FriendInviteMF.finish_button = FriendInviteMF.this_window.add_widget(
	PureFW.Image,
	(FriendInviteMF.this_window.width - 288) >> 1,
	FriendInviteMF.this_window.height - 44 - 30,
	288, 44,
	'ui/elements/form/buttons/<?=$LNG?>/invitation/send_invitations.png'
);

FriendInviteMF.finish_button.add_event_handler("click",
	function()
	{
		return function(ev)
		{
			FriendInviteMF.submit_form(ev);
		}
	}()
);

FriendInviteMF.init_err = null;
if (MapDataCtrl.players_on_map_amount >= MapDataCtrl.max_players) {
	FriendInviteMF.init_err = MammunUI.show_warning(
		350, 200,
		'<?= LNG_NO_INVITATIONS_POSSIBLE_FULL ?>',
		PureFW.ConfirmationBox.YES,
		false
	);
}
else if (MapDataCtrl.map_is_closed) {
	FriendInviteMF.init_err = MammunUI.show_warning(
		350, 200,
		'<?= LNG_NO_INVITATIONS_POSSIBLE ?>',
		PureFW.ConfirmationBox.YES,
		false
	);
}

if (FriendInviteMF.init_err) {
	FriendInviteMF.init_err.add_event_handler("destroy",
/*		function(ev) { InvitationWindow.destroy(); } );
	FriendInviteMF.init_err.add_event_handler("confirm",*/
		function(ev) {
			InvitationWindow.set_url('invitation.php?general_invitation=true');
		}
	);
}

FriendInviteMF.loaded = true;
FriendInviteMF.this_window.add_event_handler("destroy", FriendInviteMF.destroy);
FriendInviteMF.this_window.add_event_handler("change", FriendInviteMF.destroy);
</script>
