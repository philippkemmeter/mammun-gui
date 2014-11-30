/**
 * Behandelt Ereignismeldungen
 */

EventMessage = new Object();	// statisch

/**
 * Hier werden die Hashs der offenen Popups gespeichert, dass nicht 2x dieselbe
 * Nachricht aufpoppt
 */
EventMessage.open_popup_hashs = new Object();


EventMessage.create_msg_hash = function(message_obj) {
	return message_obj.type+"-"+message_obj.extra+"-"+message_obj.extra_str
		+"-"+message_obj.num;
}

/**
 * Gibt zurück, ob gerade ein Popup angezeigt wird.
 * 
 * @return bool
 */
EventMessage.is_showing_popup = function() {
	for (i in EventMessage.open_popup_hashs) {
		return true;
	}
	return false;
}

/**
 * Holt jetzt außer der Reihe die Ereignismeldungen vom Server.
 */
EventMessage.get_new_events_now = function() {
	var request_str = "ingame_update.php?"
		+"my_event_log=true";
	PureFW.AJAXClientServer.send_request(request_str, 
		function(response_arr) {
			if (response_arr[0] != '1') {
				throw new Error(
					"Got wronge response in EventMessage.get_new_events_now: "
							+ response_arr);
			}
			if (response_arr[1])
				EventMessage
					.handle_event_messages(eval('('+response_arr[1]+')'));
		}
	);
}

/**
 * TYPE-Declarations has to be done in PHP-Part.
 * 
 * EventMessage.TYPE_SPECIAL_MIN = ...
 * ...
 * ...
 */

/**
 * Diese Funktion behandelt ein Array von Nachrichtenobjekten.
 * 
 * Jedes Objekt hat folgende Struktur:
 * <code>
 * o.type			Typ des Events (s. IEventLog-TYPE-Konstanten)
 * o.t_stamp		Zeitpunkt des Events (UNIX-Timestamp)			UNUSED
 * o.num			Spezifische ID des Nachricht
 * o.str			Stringrepräsentation des Events zur Nutzerausgabe
 * o.pic			Bild, das zum Text angezeigt werden soll
 * o.on_yes			Funktion, die ausgeführt werden soll, wenn YES angeklickt 
 * 					wird
 * o.on_no			Funktion, die ausgeführt werden soll, wenn NO angeklickt
 * 					wird
 * </code>
 * 
 * Abhängig vom type passieren andere Dinge, bzw. die Nachricht wird anders
 * dargestellt.
 * 
 * @param Object[] message_arr
 * @see PureFW.Label
 */
EventMessage.handle_event_messages = function(message_arr) {
	if (!isArray(message_arr))
		 return;
	var n = message_arr.length;
	for (var i = 0; i < n; i++) {
		if (!message_arr[i] || typeof(message_arr[i]) != 'object')
			continue;
			
		if ((message_arr[i].type >= EventMessage.TYPE_SPECIAL_MIN) 
			&& (message_arr[i].type <= EventMessage.TYPE_SPECIAL_MAX))
		{
			switch (message_arr[i].type) {
				case EventMessage.TYPE_OPEN_INVITATION_DIALOG:
					if ((typeof(InvitationWindow) == 'undefined')
						|| !InvitationWindow)
					{
						UIButtons.open_friend_invitation();
					}
					break;
				case EventMessage.TYPE_OPEN_COL_CHOOSE_DIALOG:
					if ((typeof(ColPosChoose) == 'undefined')
						|| !ColPosChoose)
					{
						UIButtons.open_col_choose_dialog();
					}
					break;
				case EventMessage.TYPE_OPEN_COL_CHOOSE_DIALOG_INVITE:
					if ((typeof(ColPosChoose) == 'undefined')
						|| !ColPosChoose)
					{
						UIButtons.open_col_choose_dialog("invite=true");
					}
					break;
			}
			PureFW.Timeout.set_timeout(
				(function(_num) {
					return function() {
						PureFW.AJAXClientServer.send_request(
							"map.php?delete_message="+_num, 
							null, null, true
						);
					}
				})(message_arr[i].num)
			, 2000);
		}
		else {
			var skip = false;
			switch (message_arr[i].type) {
				case EventMessage.TYPE_TRAINING_FINISHED_SCIENTIST:
				case EventMessage.TYPE_SC_FINISHED:
					UIButtons.bb3_zzz.start_animation();
					UIButtons.bb3_zzz.show();
					break;
				case EventMessage.TYPE_USE_FB_SHARE:
					if (!MammunUI.user_in_facebook)
						skip = true;
					break;
			}
			if (!skip)
				EventMessage.show_event_message(message_arr[i]);
		}
	}
}

/**
 * Diese Funktion zeigt eine normale Ereignismeldung als Popup an.
 * 
 * Es kann zum Obligaten Ereignistext auf eine Bild-URL angegeben werden. Das
 * Bild wird dann auf die angegebene Größe skaliert.
 * Mit <code>pic_pos</code> kann die Position des Bildes zum Text angegeben
 * werden. Hier werden PureFW.Label-Konstanten erwartet.
 * 
 * Übergeben werden die Daten in Form eines Message-Objektes folgender Struktur:
 * 
 * <code>
 * o.type				Typ des Events (s. IEventLog-TYPE-Konstanten)	UNUSED
 * o.t_stamp			Zeitpunkt des Events (UNIX-Timestamp)			UNUSED
 * o.num				Spezifische ID des Nachricht
 * o.str				Stringrepräsentation des Events zur Nutzerausgabe
 * o.add_str			Zusätzlicher String, der je nach Template angezeigt wird
 * o.headline			Überschrift, die über ,str' angezeigt werden soll
 * o.pic				Bild, das zum Text angezeigt werden soll
 * o.on_yes				Funktion, die ausgeführt werden soll, wenn YES angeklickt 
 * 						wird
 * o.on_no				Funktion, die ausgeführt werden soll, wenn NO angeklickt
 * 						wird
 * o.template			Welches Template benutzt werden soll, um das Ereignis
 * 						anzuzeigen
 * o.medal_id			Welcher Orden verliehen wurde für das Ereignis
 * o.score				Punktezahl, die dieses Ereignis gebracht hat
 * o.score_str			Strinrepräsentation der Punktzahl
 * o.additional_pics	Zusätzliche Bilder, die je nach Template angezeigt werden
 * o.publish_str		Text der Facebook Veröffentlichung
 * o.height				Höhe des Fensters	(je nach Template nicht veränderbar)
 * o.width				Breite des Fensters	(je nach Template nicht veränderbar)
 * </code>
 * 
 * @param Object message_obj
 * @see PureFW.Label
 */
EventMessage.show_event_message = function(message_obj) {
	TutorialCtrl.hide();
	var msg_hash = EventMessage.create_msg_hash(message_obj);
	if (EventMessage.open_popup_hashs[msg_hash])
		return;
	
	EventMessage.open_popup_hashs[msg_hash] = true;
	var medal_id = parseInt(message_obj.medal_id) || 0;
	var pic = message_obj.pic || {};
	pic.pic_url = pic.pic_url || ((medal_id > 0) 
			? '../pix/ui/icons/labels/medals/250x270/'+medal_id+'.png' 
			: null);
	pic.pic_url_small = pic.pic_url_small || 
		((medal_id > 0) 
			? '../pix/ui/icons/labels/medals/50x54/'+medal_id+'.png' 
			: null);
	pic.width = pic.width || 100;
	pic.height = pic.height || 100;
	pic.pos = (typeof(pic.pos) == 'undefined') ? PureFW.Label.OVER : pic.pos;
	pic.tooltip = pic.tooltip || '';
	var text = message_obj.str || '';
	var headline = message_obj.headline || '';
	var num = message_obj.num || 0;
	var on_yes = message_obj.on_yes || null;
	var on_no = message_obj.on_no || null;
	var type = message_obj.type || 0;
	var template = message_obj.template || 
		((medal_id > 0)	? 'medal' : (pic.pic_url ? 'pic_text' : 'default'));
	var score = message_obj.score || 0;
	var score_str = message_obj.score_str || '';
	var add_text = message_obj.add_str || '';
	var add_pictures = message_obj.additional_pics || [];
	var publish_title = message_obj.publish_title || '';
	var publish_str = message_obj.publish_str || '';
	var publish_action = message_obj.publish_action || '';
	
	var width = message_obj.width || 0;
	var height = message_obj.height || 0;
	
	
	if (typeof(on_yes) == 'string')
		on_yes = eval('('+on_yes+')');
	if (typeof(on_no) == 'string')
		on_no = eval('('+on_no+')');
	
	var buttons = 0;
	if (on_yes)
		buttons |= PureFW.ConfirmationBox.YES;
	if (on_no)
		buttons |= PureFW.ConfirmationBox.NO;
	
	if (!buttons)
		buttons = PureFW.ConfirmationBox.YES;

	var event_popup = EventMessage['create_'+template+'_confirm'](buttons,
		width, height, (publish_str != ''));
		
	
	if (on_yes)
		event_popup.con_box.add_event_handler("confirm", on_yes);
	if (on_no)
		event_popup.con_box.add_event_handler("cancel", on_no);
	
	event_popup.event_handler = (function(_score, _msg_hash) {
		return function(ev) {
			PureFW.AJAXClientServer.send_request(
				"map.php?delete_message="+num, null, null, true
			);
			Overview.xp_bar.add_xp_score(_score);
			try {
				PauseMenu.statistics.ui_my_score_gainer.add_current_amount(
					_score);
			}
			catch(e) {}
			delete EventMessage.open_popup_hashs[_msg_hash];
		}
	})(score, msg_hash);
	event_popup.con_box.add_event_handler(
		"confirm",
		event_popup.event_handler
	);
	event_popup.con_box.add_event_handler(
		"cancel",
		event_popup.event_handler
	);
	event_popup.con_box.add_event_handler(
		"destroy",
		function() {
			TutorialCtrl.show();
		}
	);
	if (pic.pic_url && event_popup.content.instance_of(PureFW.Label)) {
		event_popup.content.set_pic(
			pic.pic_url, pic.width, pic.height, pic.pos
		);
		event_popup.content.set_font_color('#333333');
	}
	else if (template == 'medal') {
		event_popup.medal_pic.set_pic_url(pic.pic_url);
		event_popup.medal_pic_small.set_pic_url(pic.pic_url_small);
	}

	if (publish_str != '') {
		event_popup.con_box.add_event_handler("publish",
			(function (_title, _text, _pic, _action) {
				return function(ev) {
					MammunUI.facebook_publish_prompt(_title, _text, _pic, _action);
				}
			})(publish_title, publish_str, pic.pic_url, publish_action)
		);
	}
	event_popup.content.set_content(text);
	if (event_popup.headline)
		event_popup.headline.set_content(headline);
	if (event_popup.score)
		event_popup.score.set_content(score_str);
	if (event_popup.additional_content)
		event_popup.additional_content.set_content(add_text);
	if (event_popup.additional_pics)
		event_popup.additional_pics.set_pics(add_pictures);
	if (event_popup.publish_str)
		event_popup.publish_str.set_content(publish_str);

}


/**
 * JETZT FOLGEN DIE TEMPLATE-ERZEUGER
 */

EventMessage.create_research_confirm = function(buttons) {
	var event_popup = new Object();
	event_popup.con_box = MammunUI.create_conf_box(530, 437, 
		PureFW.ConfirmationBox.NO, PureFW.PublishConfirmBox);

	event_popup.officer_pic = event_popup.con_box.add_widget(
		PureFW.Image,
		16, 49,
		93, 217,
		'ui/officers/cso/grandma_left_93x217.png'
	);
	event_popup.headerline = event_popup.con_box.add_widget(
		PureFW.Image,
		118, 19,
		313, 54,
		'ui/elements/texts/'+ MammunUI.LNG
			+'/EventMessages/research_finished_headline.png'
	);
	
	event_popup.headline = event_popup.con_box.add_widget(
		PureFW.Container,
		event_popup.headerline.position.x,
		event_popup.headerline.position.y
			+ event_popup.headerline.height
			+ 20,
			event_popup.con_box.width
				- event_popup.headerline.position.x
				- 30,
		27
	);
	event_popup.headline.set_font_size(1.5);
	event_popup.headline.set_font_weight('bold');
	event_popup.headline.set_text_align('center');
	
	event_popup.content = event_popup.con_box.add_widget(
		PureFW.Container,
		event_popup.headerline.position.x,
		event_popup.headline.position.y
			+ event_popup.headline.height
			+ 20,
		event_popup.headline.width,
		0
	);
	
	event_popup.score = event_popup.con_box.add_widget(
		PureFW.Container,
		0, 
		event_popup.con_box.height - 70,
		event_popup.con_box.width,0
	);
	event_popup.score.set_css_class('EvMessage_score');
	event_popup.score.set_text_align('center');
	
	return event_popup;
}

EventMessage.create_medal_confirm = function(buttons) {
	var event_popup = new Object();
	event_popup.con_box = new PureFW.PublishConfirmBox(
		document.body,
		(MammunUI.reference_width - 675) >> 1,
		(MammunUI.reference_height - 550) >> 1,
		675, 550,
		PureFW.ConfirmationBox.NO,
		true
	);
	event_popup.con_box.set_bg_img(
		'ui/backgrounds/confirm/info/info_'+675+'x'+550+'.png'
	);
	event_popup.medal_pic = event_popup.con_box.add_widget(
		PureFW.Image,
		20, 9,
		250, 270
	);
	event_popup.medal_pic.set_z_index(2);
	
	event_popup.medal_desc_box = event_popup.con_box.add_widget(
		PureFW.Container,
		20, 248,
		238, 121
	);
	event_popup.medal_desc_box.set_bg_img(
		'ui/backgrounds/EventMessages/medal/medal_desc_box.png'
	);

	event_popup.medal_pic_small = event_popup.medal_desc_box.add_widget(
		PureFW.Image,
		8, 10,
		50, 54
	);
	event_popup.medal_pic_small.hide();
	event_popup.additional_content = event_popup.medal_desc_box.add_widget(
		PureFW.Container,
		15
		/*event_popup.medal_pic_small.position.x + 
			event_popup.medal_pic_small.width + 5*/,
		35,
		210, 0
	);
	event_popup.additional_content.set_text_align('center');
	
	event_popup.score = event_popup.con_box.add_widget(
		PureFW.Container,
		(event_popup.con_box.width - 300)>>1, 480,
		300 ,0
	);
	event_popup.score.set_css_class('EvMessage_score');
	event_popup.score.set_text_align('center');
	
	event_popup.headline = event_popup.con_box.add_widget(
		PureFW.Container,
		280, 20,
		330, 120
	);
	event_popup.headline.set_css_class('EvMessage_headline');
	
	event_popup.content = event_popup.con_box.add_widget(
		PureFW.Container,
		280, 135,
		370, 0
	);
	
	event_popup.additional_pics = event_popup.con_box.add_widget(
		PureFW.Container,
		18, 410,
		630, 110
	);
	event_popup.additional_pics.set_pics = 
		(function(_this) {
			return function(pics, w, h) {
				w = w || 50;
				h = h || 54;
				var s = 0;	// spanning
				var rows = Math.ceil(((pics.length * w) + (pics.length-1 * s)) 
																/ _this.width);
				for (var y = 0, i = 0, r = 0; 
						y < rows*h + (rows-1)*s; y+=h+s, r++) 
				{
					for (var x = 0; x < _this.width; x+=w+s, i++) {
						var tmp = _this.add_widget(
							PureFW.Image,
							x, y,
							w, h,
							pics[i].pic_url
						);
						if (pics[i].deactivated)
							tmp.deactivate();
						tmp.set_tooltip(pics[i].tooltip);
					}
				}
			}
		})(event_popup.additional_pics)
	return event_popup;
}

EventMessage.create_fb_share_confirm = function(buttons, w, h) {
	var event_popup = new Object();
	event_popup.con_box = MammunUI.create_conf_box(
		w || 489, h || 187, PureFW.ConfirmationBox.NO, PureFW.PublishConfirmBox
	);
	
	event_popup.content = event_popup.con_box.add_widget (
		PureFW.Label,
		15, 15,
		event_popup.con_box.width - 30,
		event_popup.con_box.height - 40
	);
	
	event_popup.con_box.add_event_handler("publish",
		function (ev) {
			SocialNetworks.Facebook.share_invite_island();
		}
		
	);
	return event_popup;
}

EventMessage.create_default_confirm = function(buttons, w, h) {
	var event_popup = new Object();
	event_popup.con_box = MammunUI.create_conf_box(w || 400, h || 300, buttons);
	event_popup.content = event_popup.con_box.add_widget(
		PureFW.Container,
		15, 15,
		event_popup.con_box.width - 30,
		event_popup.con_box.height - 30
	);
	return event_popup;
}

EventMessage.create_pic_text_confirm = function(buttons, w, h, share) {
	var event_popup = new Object();
	event_popup.con_box = MammunUI.create_conf_box(w || 400, h || 300, 
		share ? PureFW.ConfirmationBox.NO : buttons, 
		(share) ? PureFW.PublishConfirmBox : null
	);
	event_popup.content = event_popup.con_box.add_widget (
		PureFW.Label,
		15, 15,
		event_popup.con_box.width - 30,
		event_popup.con_box.height - 40
	);
	event_popup.score = event_popup.con_box.add_widget(
		PureFW.Container,
		event_popup.content.position.x, 
		event_popup.content.position.y + event_popup.content.height - 30,
		event_popup.content.width,0
	);
	event_popup.score.set_css_class('EvMessage_score');
	event_popup.score.set_text_align('center');
	return event_popup;
}


/**
 * Zufällig aufpoppende Ereignisse - Zeitgesteuert
 */

EventMessage.Random = new Object();

EventMessage.Random.messages = new Array();

EventMessage.Random.trigger_next_message = function() {
	if (TutorialCtrl.running)
		return;
	if (EventMessage.Random.messages.length == 0)
		return;
	
	var message = EventMessage.Random.messages.random();
	
	EventMessage.Random.messages.remove(message);
	EventMessage.handle_event_messages(new Array(message));	
}

EventMessage.Random.insert_message = function(message) {
	var n = EventMessage.Random.messages.length;
	for (var i = 0; i < n; i++) {
		if (EventMessage.create_msg_hash(message)
			== EventMessage.create_msg_hash(EventMessage.Random.messages[i]))
		{
			// Nachricht bereits eingefügt
			return;
		}
	}
	EventMessage.Random.messages.push(message);
}

EventMessage.Random.interval = PureFW.Timeout.set_interval(
	EventMessage.Random.trigger_next_message, 80000
);