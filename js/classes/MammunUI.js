/**
 * Hier werden UI-Konfigurationen definiert und Funktionen abgelegt, die
 * sowohl bei MapUI als auch bei NonMapUI benötigt werden.
 */

MammunUI = new Object();
MammunUI.reference_width = 1000;
MammunUI.reference_height = 660;
MammunUI.facebook_app_url = '';		// verzögert
MammunUI.user_in_facebook = false;

MammunUI.unit_details = null;
MammunUI.warning_cont = null;

MammunUI.LNG = 'de';
MammunUI.LNG_WARNING_NOT_IN_FACEBOOK = '';


/**
 * Zeigt die Einheit anhand der angegebenen Daten in groß an.
 * 
 * @param Object info
 */
MammunUI.show_unit_details = function(info) {
	if (MapDataCtrl.my_nick == info.user_nick) {
		var my_mammun = true;
	}
	else {
		var my_mammun = false;
	}
	
	if (MammunUI.unit_details) {
		MammunUI.unit_details.destroy();
	}
	MammunUI.unit_details = new Object();
	MammunUI.unit_details.destroy = function() {
		destroy_object(MammunUI.unit_details);
		MammunUI.unit_details = null;
	};

	var pic_core = info.pic_url.substring(0, info.pic_url.indexOf('/'));
	var t = info.pic_url.replace(eval('/'+pic_core+'_/'), pic_core+'_small_');
	t = t.replace(/[0-9]\.png/g,"0.png");
	var pic_url = 'ui/units/training/'+t;

	if (my_mammun)
		var ultimate_url = 'http://www.mammun.com/pix/ui/units/training/'+t;
	var unit_health = info.amount/1000*info.hp;
	var rounded_health = Math.ceil(unit_health/10);
	var health_url = 'whatever_url/subdirectory/' + rounded_health + '0';

	MammunUI.unit_details.box = new PureFW.ConfirmationBox(
		document.body,
		(MammunUI.reference_width-240)>>1,
		(MammunUI.reference_height-400)>>1,
		240, 400,
		PureFW.ConfirmationBox.NO, true
	);
	MammunUI.unit_details.box.set_bg_img(
		'ui/backgrounds/sector_details/unit_details_background.png');
	MammunUI.unit_details.box.set_z_index(500);
	MammunUI.unit_details.box.set_font_color('#fff');

	MammunUI.unit_details.name_container = new PureFW.Container(
		MammunUI.unit_details.box,
		20, 20,
		MammunUI.unit_details.box.width - 40, 25);
	MammunUI.unit_details.name_container.set_font_size(1.2);
	MammunUI.unit_details.name_container.set_font_weight('bold');
	MammunUI.unit_details.name_container.set_text_align('center');
	MammunUI.unit_details.name_container.set_content(info.name);

	MammunUI.unit_details.aura = new PureFW.Image(
		MammunUI.unit_details.box.get_content_node(),
		35, 310,
		100, 100,
		'pattern/spacer.gif'
	);
	if (my_mammun) {
		if(MammunUI.get_top_frame().MammunUI.user_in_facebook) {
			MammunUI.unit_details.publish_button 
				= MammunUI.unit_details.box.add_widget
			(
				PureFW.Container,
				20, 340,
				80, 24
			);
			MammunUI.unit_details.publish_button.set_font_size(1.1);
			MammunUI.unit_details.publish_button.set_z_index(11);
			MammunUI.unit_details.publish_button.set_font_weight(700);
			MammunUI.unit_details.publish_button.set_bg_img(
					'ui/elements/logos/facebook24x24.png', false);
			MammunUI.unit_details.publish_button.set_padding_left(28);
			MammunUI.unit_details.publish_button.set_padding_top(3);
			MammunUI.unit_details.publish_button.set_content("Publish");
			MammunUI.unit_details.publish_button.add_event_handler('click',
				function ()
				{
					var attachment = {
				      'name' : info.name,
				      'href' : MammunUI.facebook_app_url,
	//			      'caption' : info.name,
				      'media' : [{'type' : 'image',
				                   'src' : ultimate_url,
				                  'href' : MammunUI.facebook_app_url}]};
					FB.Connect.streamPublish('', attachment);
				});
		}
		if (info.id) {
			MammunUI.unit_details.recloth_button 
				= MammunUI.unit_details.box.add_widget
			(
				PureFW.Image,
				200, 330,
				22, 41,
				'ui/icons/labels/unit_orders/change_clothes.png'
			);
			MammunUI.unit_details.recloth_button.add_event_handler("click",
				(function(_unit_info) {
					return function(ev) {
						MammunUI.unit_details.box.destroy();
						UIButtons.toggle_unit_wizard(
							'id0=' + _unit_info.id + '&race_change='
								+ _unit_info.race + '&action=change_clothes'
						);
					}
				})(info)
			);
		}
	}
			

	MammunUI.unit_details.image = new PureFW.Image(
		MammunUI.unit_details.box.get_content_node(),
		35, 60,
		165, 289,
		pic_url);
}


MammunUI.show_medal_details = function(medal) {
	var confirm_box = MammunUI.create_conf_box(
		530, 352, PureFW.ConfirmationBox.YES
	);
	confirm_box.add_widget(
		PureFW.Image,
		20, 10,
		250, 270,
		'../pix/ui/icons/labels/medals/250x270/'+medal.id+'.png'
	);
	
	var tmp = confirm_box.add_widget(
		PureFW.Container,
		270, 20,
		240, 0
	);
	tmp.set_content(medal.name);
	tmp.set_css_class('headline_mid_simple');
	
	var tmp = confirm_box.add_widget(
		PureFW.Container,
		tmp.position.x, 150,
		tmp.width, 0
	);
	tmp.set_content(medal.desc);
}

MammunUI.clear_session_first_map = function()
{
	PureFW.AJAXClientServer.send_request(
		"map.php?clear_session_first_map=true", MammunUI.clear_session_callback);
};
MammunUI.clear_session_callback = function(response_arr)
{
	if(response_arr[0] === 0)
	{
		throw "Cleared";
	}
	else
	{
		throw response_arr[0];
	}
}

/**
 * TODO: Put into new File SocialNetworks.js and refactor
 */
MammunUI.facebook_publish_prompt = function(title, text, img, action, callback)
{
	if(!MammunUI.user_in_facebook) {
		/**
		 * FB-Connect-Link!
		 */
		MammunUI.show_warning(
			350, 200, MammunUI.LNG_WARNING_NOT_IN_FACEBOOK,
			PureFW.ConfirmationBox.NO
		);
		return;
	}
	else {
		var attachment = {
	      'name' : title,
	      'href' : MammunUI.facebook_app_url
		}
		text = text || '';
		img = img || '';
		action = action || null;
		
		if (text.length > 0) {
			attachment['caption'] = text;
		}
		if (img.length > 0) {
			attachment['media'] = 
				[{'type' : 'image',
                'src' : 'http://www.mammun.com/pix/scales/1/'+img,
                'href' : MammunUI.facebook_app_url}];
		}
		
		if (action != null)
		{
			var action_link = {
				      'name' : action,
				      'href' : MammunUI.facebook_app_url
			}
		}
		
		callback = callback || null;
		FB.Connect.streamPublish('', attachment, action_link, null, "", callback);
	}
};

/**
 * Diese Funktion muss bei jedem Tastendruck von jedem Formulareingabefeld
 * verlinkt werden. Hier werden gewisse Sachen begünstigt, andere verhindert.
 * 
 * @param Event ev
 */
MammunUI.form_keyupdown = function(ev) {
	try {
		ev = PureFW.EventUtil.formatEvent(ev);
	}
	catch(e){}
	// Tastendruck nicht weitergeben an z.B. die Karte, damit das Tippen
	// keine Auswirkungen auf die Steuerung hat
	ev.stopPropagation();
};


/**
 * Verschönert den angegebenen Nick. Erwartet einen Nick mit Suffix.
 */
MammunUI.beautify_nick = function(nick_w_suffix) {
	var x = nick_w_suffix.split('.');
	var result = '<span class="username">'+x[0].ucfirst();
	if (x[1]) {
		result += '<span class="usersuffix">.'+x[1]+'</span>';
		result += '<span class="usersuffix star">&nbsp;</span>';
	}
	else {
		result += '<span class="usersuffix star">✮</span>';
	}
	return result + '</span>';
}

MammunUI.get_nick_main = function(nick_w_suffix) {
	var x = nick_w_suffix.split('.');
	return x[0].ucfirst();
}

/**
 * Gibt den top-Frame des Spiels zurück. Wenn das Spiel selbst im Top läuft,
 * ist es natürlich "top", ansonsten muss der Frame gefunden werden, in dem
 * das Spiel sitzt - nicht immer ganz einfach :)
 */
MammunUI.get_top_frame = function() {
	if (window.location.href.indexOf("ingame.php") > -1)
		return window;
	
	try {   // ggf. ist Top ein anderer Server, dann hier Exception
		if (top.location.href.indexOf("ingame.php") > -1)
			return top;
	}
	catch(e) {}

	try {   // ggf. ist parent ein anderer Server, dann hier Exception
		if (parent.location.href.indexOf("ingame.php") > -1)
			return parent;
	}
	catch(e) {}
	
	return window;
}

/**
 * Gibt die Frame zurück, in der die Header-Bar sitzt, welche man aus FB
 * kennt. Sie liegt oberhalb vom Spiel-Frame aka top_frame, nur wo genau,
 * das wird sich dann zeigen, und ob sie überhaupt geladen ist. 
 */
MammunUI.get_header_bar_frame = function() {
	var _top = MammunUI.get_top_frame();
	try {
		if ((_top.parent.location.href.indexOf('mammun'))
			&& (_top.parent.HeaderBar))
		{
			return _top.parent;
		}
	}
	catch(e) {
		throw e;
	}
	try {
		if ((_top.parent.parent.location.href.indexOf('mammun'))
			&& (_top.parent.parent.HeaderBar))
		{
			return _top.parent.parent;
		}
	}
	catch(e) {}
	return null;
}

/**
 * Zeigt eine Warnung an, die bestätigt werden muss, bevor es weitergeht.
 *
 * Es ist also ein Shortcut für die Erzeugung einer neuen ConfirmationBox.
 * Alle anderen hierüber erzeugten Boxen werden direkt zerstört, so dass immer
 * nur eine solche Box gleichzeitig existieren kann.
 *
 * @param unit w
 * @param uint h
 * @param String text
 * @param uint buttons
 * @param bool alert
 * @return PureFW.ConfirmationBox
 */
MammunUI.show_warning = function(w, h, text, buttons, alert) {
	MammunUI.destroy_warning();

	MammunUI.warning_cont = new PureFW.ConfirmationBox(
		document.body,
		(MammunUI.reference_width - w) >> 1,
		(MammunUI.reference_height - h) >> 1,
		w, h,
		buttons,
		true
	);
	if (alert)
		MammunUI.warning_cont.set_bg_img(
			'ui/backgrounds/confirm/alert/alert_'+w+'x'+h+'.png'
		);
	else
		MammunUI.warning_cont.set_bg_img(
			'ui/backgrounds/confirm/info/info_'+w+'x'+h+'.png'
		);
	if (text) {
		var tmp = MammunUI.warning_cont.add_widget(
			PureFW.Container,
			25, 15,
			MammunUI.warning_cont.width - 50,
			MammunUI.warning_cont.height - 30
		);
		tmp.set_content(text);
	}

	return MammunUI.warning_cont;
}
MammunUI.destroy_warning = function() {
	if (MammunUI.warning_cont) {
		try {
			MammunUI.warning_cont.destroy();
		}
		catch(e){}
		MammunUI.warning_cont = null;
	}
}

/**
 * Erzeugt eine Confirmationbox und gibt sie zurück.
 * 
 * Im Gegensatz zu show_warning wird hier nicht nur explizit eine angezeigt,
 * und es wird sich auch nicht um das Zerstören gekümmert.
 * 
 * @param uint w
 * @param uint h
 * @param buttons
 * @return PureFW.ConfirmationBox
 */
MammunUI.create_conf_box = function(w, h, buttons, widget_constructor) {
	widget_constructor = widget_constructor || PureFW.ConfirmationBox;
	var conf = new widget_constructor(
		document.body,
		(MammunUI.reference_width - w) >> 1,
		(MammunUI.reference_height - h) >> 1,
		w, h,
		buttons,
		true
	);
	conf.set_bg_img(
		'ui/backgrounds/confirm/info/info_'+w+'x'+h+'.png'
	);
	
	return conf;
}


MammunUI.add_to_debug_console = function(text) {
	if ((typeof(MammunUI.debug_console) == 'undefined') 
		|| !MammunUI.debug_console)
	{
		MammunUI.debug_console = new PureFW.ScrollContainer(
			document.body,
			MammunUI.reference_width - 220,
			MammunUI.reference_height - 410,
			200, 0
		);
		MammunUI.debug_console.set_bg_color('#eeeeee');
		MammunUI.debug_console.set_font_color('#000');
		MammunUI.debug_console.set_z_index(400);
	}
	MammunUI.debug_console.set_height(0);
	MammunUI.debug_console.update_inner_height();
	MammunUI.debug_console.set_content(text+'<br/>'
		+ MammunUI.debug_console.get_content()
	);
	MammunUI.debug_console.set_height(400);
}