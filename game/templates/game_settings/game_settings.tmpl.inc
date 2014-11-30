<script type="text/javascript">
GameSettings = new Object();
GameSettings.this_window = PureFW.ContainerAJAX.instances[<?=$this_win_id?>];

GameSettings.destroy = function()
{
	GameSettings.this_window.remove_event_handler("destroy", GameSettings.destroy);
	GameSettings.this_window.remove_event_handler("change", GameSettings.destroy);
	for (var x in GameSettings) {
		<?/* Nicht das aktuelle Fenster zerstören. Das zerstört sich entweder
		   * selbst, oder es möchte nicht zerstört werden - ganz einfach :)
		   * Wir haben hier nicht die Berechtigung dort einzugreifen!*/?>
		if (x == 'this_window')
			continue;
		try {
			GameSettings[x].destroy();
		}
		catch (e) {
			delete GameSettings[x];
		}
	}
	delete GameSettings;
	GameSettings = null;
};

GameSettings.all_countries = <?=json_encode($my_country_array);?>;

//alert(GameSettings.all_countries[GameSettings.all_prefixes[21]]);
//<?
//$i=0;
//foreach($my_country_array as $country): ?>
//	GameSettings.all_countries[<?=$i?>] = '<?=$country?>';
//<?$i++;
//endforeach?>

GameSettings.old_password = null;
GameSettings.user_password = null;
GameSettings.player_gender = <?=$pd->gender?>;
GameSettings.player_country = '<?=$pd->country?>';
GameSettings.birth_day = '<?=$birth_day?>';
GameSettings.birth_month = '<?=$birth_month?>';
GameSettings.birth_year = '<?=$birth_year?>';

GameSettings.insert_countries = function(drop_menu)
{
	drop_menu.add_item('');
	for(var key in GameSettings.all_countries) {
		if (typeof(GameSettings.all_countries[key]) == 'string') {
			drop_menu.add_item(GameSettings.all_countries[key],
				function(_key, _drop_menu)
				{
					return function(ev)
					{
						GameSettings.player_country = _key;
					}
				}(key, drop_menu)
			);
		}
	}
};

GameSettings.this_window.get_node().style.overflow = 'visible';

GameSettings.insert_months = function(drop_menu)
{
	drop_menu.add_item("<?=LNG_JANUARY?>",
			function() { GameSettings.birth_month = 1;}
	);
	drop_menu.add_item("<?=LNG_FEBRUARY?>",
			function() { GameSettings.birth_month = 2;}
	);
	drop_menu.add_item("<?=LNG_MARCH?>",
			function() { GameSettings.birth_month = 3;}
	);
	drop_menu.add_item("<?=LNG_APRIL?>",
			function() { GameSettings.birth_month = 4;}
	);
	drop_menu.add_item("<?=LNG_MAY?>",
			function() { GameSettings.birth_month = 5;}
	);
	drop_menu.add_item("<?=LNG_JUNE?>",
			function() { GameSettings.birth_month = 6;}
	);
	drop_menu.add_item("<?=LNG_JULY?>",
			function() { GameSettings.birth_month = 7;}
	);
	drop_menu.add_item("<?=LNG_AUGUST?>",
			function() { GameSettings.birth_month = 8;}
	);
	drop_menu.add_item("<?=LNG_SEPTEMBER?>",
			function() { GameSettings.birth_month = 9;}
	);
	drop_menu.add_item("<?=LNG_OCTOBER?>",
			function() { GameSettings.birth_month = 10;}
	);
	drop_menu.add_item("<?=LNG_NOVEMBER?>",
			function() { GameSettings.birth_month = 11;}
	);
	drop_menu.add_item("<?=LNG_DECEMBER?>",
			function() { GameSettings.birth_month = 12;}
	);
};

GameSettings.insert_numbers_ascending = function(drop_menu, from_num, to_num)
{
	for(var i = from_num; i <= to_num; i++)
	{
		//WARNING!!! It's just a coincidince that this works. Normally using
		//using this function to attach values to the birth day wouldn't work.
		drop_menu.add_item(i,
			function(_i)
			{
				return function(ev)
				{
					GameSettings.birth_day = _i;
				}
			}(i));
	}
};

GameSettings.insert_numbers_descending = function(drop_menu, from_num, to_num)
{
	for(var i = from_num; i >= to_num; i--)
	{
		//WARNING!!! It's just a coincidince that this works. Normally using
		//using this function to attach values to the birth day wouldn't work.
		drop_menu.add_item(i,
				function(_i)
				{
					return function(ev)
					{
						GameSettings.birth_year = _i;
					}
				}(i));
	}
};


GameSettings.upload_avatar_dialog = function()
{
	<? /* Wenn wider erwarten der details-container noch existiert, wird er
	    * hiermit zerstört. Wahrscheinlicher ist jedoch, dass details_container
	    * nicht "null" ist, aber das widget bereits zerstört wurde, was zu einem
	    * Fehler führte, weswegen hier ein silent-try-catch eingebaut ist.*/ ?>
	try {
		destroy_object(GameSettings.upload_avatar_dialog);
	}
	catch(e){}

	GameSettings.upload_avatar_dialog = new Object();
	GameSettings.upload_avatar_dialog.over_box = new PureFW.ConfirmationBox(
		GameSettings.this_window,
		20, 110,
		410, 190);
	GameSettings.upload_avatar_dialog.over_box.set_bg_img(
		'ui/backgrounds/confirm/info/info_410x190.png');

	GameSettings.upload_avatar_dialog.old_pass_field = new PureFW.Container(
		GameSettings.upload_avatar_dialog.over_box,
		25, 30,
		400, 130);
	GameSettings.upload_avatar_dialog.old_pass_field.set_bg_img(
			'ui/elements/form/input/input_text-230x30.png');
	GameSettings.upload_avatar_dialog.old_pass_field.set_content(
            '<form name="GameSettings_upload_avatar_form" \
                    action="javascript:alert(\'muh\'); GameSettings.form_keyupdown(event);">\
            <input type="file" onchange="javascript: document.forms.GameSettings_upload_avatar_form.submit();" size="100" id="GameSettingsUserOldPass" style="width:100%; \
                    height: 100%; padding:4px; color: #366A9B; \
                    opacity:0;\
             background:url(../pix/pattern/spacer.gif) 0 0 no-repeat transparent;\
             border: none; position:absolute; top:0;left:0;" \
             onkeydown="javascript: GameSettings.form_keyupdown(event)"\
             onkeyup="javascript: GameSettings.form_keyupdown(event)"/></form>'
    );

    GameSettings.upload_avatar_dialog.over_box.add_event_handler('confirm',
    	    function()
    	    {
	    		return function(ev)
	    		{

	    		}
    	    }()
    );

	var destructor = function(ev)
	{
		GameSettings.upload_avatar_dialog.over_box
			.remove_event_handler("destroy",destructor);
		GameSettings.upload_avatar_dialog.over_box
			.remove_event_handler("change",destructor);
		destroy_object(GameSettings.upload_avatar_dialog);
	}
	GameSettings.upload_avatar_dialog.over_box.add_event_handler("destroy",destructor);
	GameSettings.upload_avatar_dialog.over_box.add_event_handler("change",destructor);
};

GameSettings.change_password = function()
{

	GameSettings.password_section = new Object();
	var conf_box = MammunUI.create_conf_box(
		410, 190, PureFW.ConfirmationBox.YES | PureFW.ConfirmationBox.NO
	);

	var tmp = conf_box.add_widget(
		PureFW.Image,
		20, 40,
		131, 17,
		'ui/elements/texts/<?=$LNG?>/game_settings/txt_pwchange_old.png'
	);

	tmp = conf_box.add_widget(
		PureFW.Image,
		tmp.position.x, tmp.position.y + tmp.height + 23,
		131, 17,
		'ui/elements/texts/<?=$LNG?>/game_settings/txt_pwchange_new.png'
	);

	tmp = conf_box.add_widget(
		PureFW.Image,
		tmp.position.x, tmp.position.y + tmp.height + 23,
		131, 17,
		'ui/elements/texts/<?=$LNG?>/game_settings/txt_pwchange_repeat.png'
	);

	tmp = conf_box.add_widget(
		PureFW.Container,
		155, 30,
		230, 30
	);
	tmp.set_bg_img('ui/elements/form/input/input_text-230x30.png');
	tmp.set_content(
            '<form name="MammunShop_chest_item_list_form" \
                    action="javascript:GameSettings.form_keyupdown(event);">\
            <input type="password" id="GameSettingsUserOldPass" style="width:100%; \
                    height: 100%; padding:4px; color: #366A9B; \
             background:url(../pix/pattern/spacer.gif) 0 0 no-repeat transparent;\
             border: none; position:absolute; top:0;left:0;" \
             onkeydown="javascript: GameSettings.form_keyupdown(event)"\
             onkeyup="javascript: GameSettings.form_keyupdown(event)"/></form>'
    );

	tmp = conf_box.add_widget(
		PureFW.Container,
		tmp.position.x, tmp.position.y + tmp.height + 15,
		230, 30
	);
	tmp.set_bg_img('ui/elements/form/input/input_text-230x30.png');
	tmp.set_content(
            '<form name="MammunShop_chest_item_list_form" \
                    action="javascript:GameSettings.form_keyupdown(event);">\
            <input type="password" id="GameSettingsUserPassword" style="width:100%; \
                    height: 100%; padding:4px; color: #366A9B; \
             background:url(../pix/pattern/spacer.gif) 0 0 no-repeat transparent;\
             border: none; position:absolute; top:0;left:0;" \
             onkeydown="javascript: GameSettings.form_keyupdown(event)"\
             onkeyup="javascript: GameSettings.form_keyupdown(event)"/></form>'
    );

	tmp = conf_box.add_widget(
		PureFW.Container,
		tmp.position.x, tmp.position.y + tmp.height + 15,
		230, 30
	);

	tmp.set_bg_img('ui/elements/form/input/input_text-230x30.png');
	tmp.set_content(
    	'<form name="MammunShop_chest_item_list_form" \
        	action="javascript:GameSettings.form_keyupdown(event);">\
    		<input type="password" id="GameSettingsPassRepeat" style="width:100%; \
            height: 100%; padding:4px; color: #366A9B; \
            background:url(../pix/pattern/spacer.gif) 0 0 no-repeat transparent;\
            border: none; position:absolute; top:0;left:0;" \
            onkeydown="javascript: GameSettings.form_keyupdown(event)"\
            onkeyup="javascript: GameSettings.form_keyupdown(event)"/></form>'
    );

    conf_box.add_event_handler('confirm',
    	function(ev)
    	{
    		var old_pass = document.getElemById("GameSettingsUserOldPass").value;
    		var pass = document.getElemById("GameSettingsUserPassword").value;
    		var repeat = document.getElemById("GameSettingsPassRepeat").value;
    		GameSettings.check_password(old_pass, pass, repeat);
    	}
    );
};

GameSettings.check_password = function(old_pass, pass, repeat)
{
	if(pass == repeat)
	{
		var email = document.getElemById("GameSettingsEmailField").value;
		var post_string = 	"email="+email+
						 	"&old_pass="+old_pass+
						 	"&password="+pass;

		PureFW.AJAXClientServer.send_request(
				"../game/game_settings.php",
				GameSettings.password_change_response,
				post_string
		);
	}
	else
	{
		var tmp = MammunUI.show_warning(
			350, 200, '<?= LNG_PASSWORDS_DO_NOT_MATCH ?>',
			PureFW.ConfirmationBox.YES, true
		);
		tmp.add_event_handler("confirm",
			function(ev) {
				GameSettings.change_password();
			}
		);
		GameSettings.user_password = null;
	}
};

GameSettings.password_change_response = function(response_arr)
{
	if (response_arr[0] == 0)
	{
		var tmp = MammunUI.show_warning(
			350, 200, response_arr[1],
			PureFW.ConfirmationBox.NO, true
		);
		tmp.add_event_handler("confirm",
			function(ev) {
				GameSettings.change_password();
			}
		);
	}
	else if (response_arr[0] == 1)
	{
		var tmp = MammunUI.show_warning(
			350, 200, response_arr[1],
			PureFW.ConfirmationBox.YES, false
		);
	}
	else
	{
		var tmp = MammunUI.show_warning(
			350, 200, response_arr[0],
			PureFW.ConfirmationBox.NO, true
		);
		tmp.add_event_handler("confirm",
			function(ev) {
				GameSettings.change_password();
			}
		);
		throw Error(response_arr[0]);
	}
}

GameSettings.save_settings = function()
{
	var name = document.getElemById("GameSettingsNameField").value;//GameSettings.name_field.get_text();
	var email = document.getElemById("GameSettingsEmailField").value;//GameSettings.email_field.get_text();
	var birth_day = parseInt(GameSettings.birth_day);
	var birth_month = parseInt(GameSettings.birth_month);
	var birth_year = parseInt(GameSettings.birth_year);
	var gender = GameSettings.player_gender;
	var city = document.getElemById("GameSettingsCityField").value;//GameSettings.city_field.get_text();
	var country = GameSettings.player_country;

	var post_string="name="+name+
					"&email="+email+
					"&birth_day="+birth_day+
					"&birth_month="+birth_month+
					"&birth_year="+birth_year+
					"&gender="+gender+
					"&city="+city+
					"&country="+country+
					"&send=true";
	PureFW.AJAXClientServer.send_request(
			"../game/game_settings.php",
			GameSettings.submit_response,
			post_string
	);
};

GameSettings.submit_response = function(response_arr)
{
	if (response_arr[0] == 0)
	{// Error case... small error
		throw response_arr[1];
	}
	else if (response_arr[0] == 1)
	{
		GameSettings.destroy();
		UISettings.window.destroy();
	}
	else
	{
		GameSettings.destroy();
		UISettings.window.destroy();
		throw response_arr;
	}
};

GameSettings.account_delete_prompt = function()
{
	GameSettings.SettingsConfirm = new PureFW.ConfirmationBox(
		GameSettings.this_window,
		60, 120,
		350, 200
	);
	GameSettings.SettingsConfirm.set_content(
			'<div style="margin: 5%"><?= LNG_ACCOUNT_DELETE_CONFIRM ?></div>'
	);
	GameSettings.SettingsConfirm.set_bg_img(
			'ui/backgrounds/confirm/alert/alert_350x200.png'
	);
	GameSettings.SettingsConfirm.add_event_handler("confirm",
		function(ev) {
			PureFW.AJAXClientServer.send_request(
				"../game/game_settings.php?delete_account=true",
				function() {
					location.href='logout.php';
				}
			);

		}
	);
};

GameSettings.make_notifications = function()
{
	alert("Your notifications have been notified.");
};

GameSettings.enter_private_sphere = function()
{
	alert("This sphere is private. You ain't got no access.");
};

GameSettings.form_keyupdown = function(ev)
{
	try {
		ev = PureFW.EventUtil.formatEvent(ev);
	}
	catch(e){}
	// Tastendruck nicht weitergeben an z.B. die Karte, damit das Tippen
	// keine Auswirkungen auf die Steuerung hat
	ev.stopPropagation();
};

<?/*---(((-----/\------------------- ACTION --------------------------------*/?>
<?/*----------//\\------__----_____- ACTION -_____-----__-------------------*/?>
<?/*---------//  \\----/  \-----|--- ACTION ---|------/  \----|\  |---------*/?>
<?/*--------//====\\--(    -----|--- ACTION ---|-----(    )---| \ |---------*/?>
<?/*-------//      \\--\__/-----|--- ACTION -__|__----\__/----|  \|---------*/?>
<?/*------//--------\\-------------- ACTION ----------------------------AA--*/?>

GameSettings.options_section = new PureFW.Container(
	GameSettings.this_window,
	28, 22,
	397, 356);
GameSettings.options_section.set_bg_img(
		'ui/backgrounds/game_settings/background-397x356.png');
GameSettings.options_section.get_node().style.overflow = 'visible';

GameSettings.vertical_label_offset = 41;

GameSettings.options_headline = new PureFW.Image(
	GameSettings.this_window,
	24, 3,
	152, 44,
	'ui/elements/texts/<?=$LNG?>/game_settings/options_headline.png');

GameSettings.name_text = new PureFW.Image(
	GameSettings.options_section,
	15, 25,
	108, 30,
	'ui/elements/texts/<?=$LNG?>/game_settings/txt_nickname.png');

GameSettings.nickname_text = new PureFW.Image(
	GameSettings.options_section,
	15, GameSettings.name_text.position.y + GameSettings.vertical_label_offset,
	108, 30,
	'ui/elements/texts/<?=$LNG?>/game_settings/txt_name.png');

GameSettings.email_text = new PureFW.Image(
	GameSettings.options_section,
	15, GameSettings.nickname_text.position.y + GameSettings.vertical_label_offset,
	108, 30,
	'ui/elements/texts/<?=$LNG?>/game_settings/txt_e-mail_address.png');

GameSettings.password_text = new PureFW.Image(
	GameSettings.options_section,
	15, GameSettings.email_text.position.y + GameSettings.vertical_label_offset,
	108, 30,
	'ui/elements/texts/<?=$LNG?>/game_settings/txt_password.png');

GameSettings.birthday_text = new PureFW.Image(
	GameSettings.options_section,
	15, GameSettings.password_text.position.y + GameSettings.vertical_label_offset,
	108, 30,
	'ui/elements/texts/<?=$LNG?>/game_settings/txt_birthday.png');

GameSettings.gender_text = new PureFW.Image(
	GameSettings.options_section,
	15, GameSettings.birthday_text.position.y + GameSettings.vertical_label_offset,
	108, 30,
	'ui/elements/texts/<?=$LNG?>/game_settings/txt_gender.png');

GameSettings.city_text = new PureFW.Image(
	GameSettings.options_section,
	15, GameSettings.gender_text.position.y + GameSettings.vertical_label_offset,
	108, 30,
	'ui/elements/texts/<?=$LNG?>/game_settings/txt_city.png');

GameSettings.country_text = new PureFW.Image(
	GameSettings.options_section,
	15, GameSettings.city_text.position.y + GameSettings.vertical_label_offset,
	108, 30,
	'ui/elements/texts/<?=$LNG?>/game_settings/txt_country.png');

GameSettings.nickname_field = new PureFW.Container(
	GameSettings.options_section,
	143, 25,
	230, 30);
GameSettings.nickname_field.set_content('<?=$current_user->get_nick()?>');
GameSettings.nickname_field.set_font_weight('bold');
GameSettings.nickname_field.set_font_size(1.3);

GameSettings.name_field = new PureFW.Container(
	GameSettings.options_section,
	143, GameSettings.nickname_field.position.y + GameSettings.vertical_label_offset,
	230, 30);
GameSettings.name_field.set_bg_img('ui/elements/form/input/input_text-230x30.png');
GameSettings.name_field.set_content(
        '<form name="MammunShop_chest_item_list_form" \
        action="javascript:GameSettings.form_keyupdown(event);">\
		<input type="text" id="GameSettingsNameField" \
		value="<?=$pd->firstnames?> <?=$pd->surnames?>" style="width:100%; \
		        height: 100%; padding:4px; color: #366A9B; \
		 background:url(../pix/pattern/spacer.gif) 0 0 no-repeat transparent;\
		 border: none; position:absolute; top:0;left:0;" \
		 onkeydown="javascript: GameSettings.form_keyupdown(event)"\
		 onkeyup="javascript: GameSettings.form_keyupdown(event)"/></form>'
);

GameSettings.nickname_field.set_y(GameSettings.nickname_field.position.y + 5); // cheap lazy workaround

GameSettings.email_field = new PureFW.Container(
	GameSettings.options_section,
	143, GameSettings.name_field.position.y + GameSettings.vertical_label_offset,
	230, 30);
GameSettings.email_field.set_bg_img('ui/elements/form/input/input_text-230x30.png');
GameSettings.email_field.set_content(
		'<form name="MammunShop_chest_item_list_form" \
		        action="javascript:GameSettings.form_keyupdown(event);">\
		<input type="text" value="<?=$current_user->get_email()?>" \
				id="GameSettingsEmailField" style="width:100%;\
		        height: 100%; padding:4px; color: #366A9B;\
		 background:url(../pix/pattern/spacer.gif) 0 0 no-repeat transparent;\
		 border: none; position:absolute; top:0;left:0;" \
		 onkeydown="javascript: GameSettings.form_keyupdown(event)"\
		 onkeyup="javascript: GameSettings.form_keyupdown(event)"/></form>'
);

GameSettings.password_field = new PureFW.Container(
	GameSettings.options_section,
	143, GameSettings.email_field.position.y + GameSettings.vertical_label_offset,
	230, 30);
GameSettings.password_field.set_bg_img('ui/elements/form/input/input_text-230x30.png');
GameSettings.password_field.set_content('<input type="password" onkeyup="return false;" onkeydown="return false;" value="asdasd" />');
GameSettings.password_field.add_event_handler('click', GameSettings.change_password);

GameSettings.birth_date_input = new PureFW.DropMenu(
	GameSettings.options_section,
	143, GameSettings.password_field.position.y + GameSettings.vertical_label_offset,
	55, 28);
GameSettings.insert_numbers_ascending(GameSettings.birth_date_input, 1, 31);
GameSettings.birth_date_input.select_item(<?=$birth_day?> - 1);

GameSettings.birth_month_input = new PureFW.DropMenu(
	GameSettings.options_section,
	210, GameSettings.password_field.position.y + GameSettings.vertical_label_offset,
	90, 28);
GameSettings.insert_months(GameSettings.birth_month_input);
GameSettings.birth_month_input.select_item(<?=$birth_month?> - 1);

GameSettings.birth_year_input = new PureFW.DropMenu(
	GameSettings.options_section,
	310, GameSettings.password_field.position.y + GameSettings.vertical_label_offset,
	60, 28);
GameSettings.insert_numbers_descending(GameSettings.birth_year_input, 2010, 1900);
GameSettings.birth_year_input.select_item((2010 - 1900) - (<?=$birth_year?> - 1900));

GameSettings.gender_input = new PureFW.DropMenu(
	GameSettings.options_section,
	143, GameSettings.birth_year_input.position.y + GameSettings.vertical_label_offset,
	124, 28);
GameSettings.gender_input.set_z_index(3);
GameSettings.gender_input.add_item('<?=LNG_MALE?>',
	function ()
	{
		GameSettings.player_gender = 0;
	});

GameSettings.gender_input.add_item('<?=LNG_FEMALE?>',
	function ()
	{
		GameSettings.player_gender = 1;
	});
GameSettings.gender_input.select_by_name(
	'<?= ($pd->gender == 0) ? LNG_MALE : LNG_FEMALE ?>'
);

GameSettings.city_field = new PureFW.Container(
	GameSettings.options_section,
	143, GameSettings.gender_input.position.y + GameSettings.vertical_label_offset,
	230, 30);
GameSettings.city_field.set_bg_img('ui/elements/form/input/input_text-230x30.png');
GameSettings.city_field.set_content(
        '<form name="MammunShop_chest_item_list_form" \
        action="javascript:GameSettings.form_keyupdown(event);">\
		<input type="text" id="GameSettingsCityField" \
		value="<?=$pd->city?>" style="width:100%; \
		        height: 100%; padding:4px; color: #366A9B; \
		 background:url(../pix/pattern/spacer.gif) 0 0 no-repeat transparent;\
		 border: none; position:absolute; top:0;left:0;" \
		 onkeydown="javascript: GameSettings.form_keyupdown(event)"\
		 onkeyup="javascript: GameSettings.form_keyupdown(event)"/></form>'
);

GameSettings.country_input = new PureFW.DropMenu(
	GameSettings.options_section,
	143, GameSettings.city_field.position.y + GameSettings.vertical_label_offset,
	230, 28);
GameSettings.insert_countries(GameSettings.country_input);
GameSettings.country_input.select_by_name(GameSettings.all_countries['<?=$pd->country?>']);
GameSettings.country_input.set_text_align('left');
GameSettings.country_input.set_item_padding(5);

//GameSettings.country_field.set_bg_img('ui/elements/form/input/input_text-230x30.png');
//GameSettings.country_field.set_content(
//        '<form name="MammunShop_chest_item_list_form" \
//        action="javascript:GameSettings.form_keyupdown(event);">\
//		<input type="text" id="GameSettingsCountryField" \
//		value="<?=$pd->country?>" style="width:100%; \
//		        height: 100%; padding:4px; color: #366A9B; \
//		 background:url(../pix/pattern/spacer.gif) 0 0 no-repeat transparent;\
//		 border: none; position:absolute; top:0;left:0;" \
//		 onkeydown="javascript: GameSettings.form_keyupdown(event)"\
//		 onkeyup="javascript: GameSettings.form_keyupdown(event)"/></form>'
//);

GameSettings.avatar_section = new PureFW.Container(
	GameSettings.this_window,
	448, 25,
	218, 205);
GameSettings.avatar_section.set_bg_img(
		'ui/backgrounds/game_settings/square_background-218x205.png');

GameSettings.avatar_headline = new PureFW.Image(
	GameSettings.this_window,
	575, 1,
	90, 44,
	'ui/elements/game_settings/texts/<?=$LNG?>/avatar_headline.png');

GameSettings.avatar_group = new PureFW.Container(
	GameSettings.avatar_section,
	32, 23,
	150, 170
);

GameSettings.avatar_picture = new PureFW.Image(
	GameSettings.avatar_group,
	0, 0,
	150, 150,
	'<?= $avatar_url ?>'.replace("SIZE", 150)
);

GameSettings.avatar_picture.set_z_index(1);

GameSettings.change_avatar_text = new PureFW.Container(
	GameSettings.avatar_group,
	0,
	GameSettings.avatar_picture.position.y
		+ GameSettings.avatar_picture.height,
	GameSettings.avatar_group.width,
	20);
GameSettings.change_avatar_text.set_font_color('#366A9B');
GameSettings.change_avatar_text.set_text_align('center');
GameSettings.change_avatar_text.set_content('<a href="javascript:;"><?= LNG_CHANGE_AVATAR?></a>');
GameSettings.change_avatar_text.set_z_index(1);

GameSettings.change_avatar_upload_hack = new PureFW.Container(
	GameSettings.avatar_group,
	GameSettings.change_avatar_text.position.x,
	GameSettings.change_avatar_text.position.y,
	GameSettings.change_avatar_text.width,
	GameSettings.change_avatar_text.height
);
GameSettings.change_avatar_upload_hack.set_z_index(2);

GameSettings.change_avatar_upload_hack.set_content(
	'<form method="POST"\
			name="GameSettings_upload_avatar_form"\
			target="file_upload"\
			action="../community/wbb/index.php?form=AvatarEditPlain&s=<?
					echo $_SESSION['WCF_SID']?>&t=<?= $_SESSION['WCF_ST'] ?>" \
			enctype="multipart/form-data" >\
    <input type="file" \
    		onchange="javascript: document.forms.GameSettings_upload_avatar_form.submit();"\
    		size="1" id="avatarUpload" name="avatarUpload" style="width:100%; \
            height: 100%; padding:4px; color: #366A9B; cursor: pointer; \
            opacity:0; position:absolute; top:0;left:-22%;" \
            onmouseover="javascript: GameSettings.change_avatar_text.get_node().style.textDecoration=\'underline\'"\
            onmouseout="javascript: GameSettings.change_avatar_text.get_node().style.textDecoration=\'none\'"\
     		onkeydown="javascript: GameSettings.form_keyupdown(event)"\
     		onkeyup="javascript: GameSettings.form_keyupdown(event)"/>\
	<input type="hidden" name="avatarURL" value="http://" /></form>'
);

GameSettings.misc_section = new PureFW.Container(
	GameSettings.this_window,
	448, 264,
	213, 119);
GameSettings.misc_section.set_bg_img(
		'ui/backgrounds/game_settings/square_background-213x119.png');

//GameSettings.private_sphere_text = new PureFW.Container(
//	GameSettings.misc_section,
//	12, 25,
//	GameSettings.misc_section.width, 25);
//GameSettings.private_sphere_text.set_font_color('#366A9B');
//GameSettings.private_sphere_text.set_content('<?=LNG_PRIVATE_SPHERE?>');
//GameSettings.private_sphere_text.add_event_handler('click', GameSettings.enter_private_sphere);
//
//GameSettings.notifications_text = new PureFW.Container(
//	GameSettings.misc_section,
//	12, 50,
//	GameSettings.misc_section.width, 25);
//GameSettings.notifications_text.set_font_color('#366A9B');
//GameSettings.notifications_text.set_content('<?=LNG_NOTIFICATION_SETTINGS?>');
//GameSettings.notifications_text.add_event_handler('click', GameSettings.make_notifications);

GameSettings.account_delete_text = new PureFW.Container(
	GameSettings.misc_section,
	12, 75,
	GameSettings.misc_section.width, 25);
GameSettings.account_delete_text.set_font_color('#366A9B');
GameSettings.account_delete_text.set_content('<a href="javascript:;"><?=LNG_ACCOUNT_DELETE?></a>');
GameSettings.account_delete_text.add_event_handler('click', GameSettings.account_delete_prompt);

GameSettings.misc_headline = new PureFW.Image(
	GameSettings.this_window,
	440, 246,
	151, 36,
	'ui/elements/game_settings/texts/<?=$LNG?>more_headline.png');

GameSettings.finish_button = new PureFW.Image(
	GameSettings.this_window,
	GameSettings.misc_section.position.x + GameSettings.misc_section.width
		- 96, 390,
	93, 30,
	'ui/elements/form/buttons/<?=$LNG?>/wizard/finish.png');
GameSettings.finish_button.add_event_handler('click', GameSettings.save_settings);

GameSettings.this_window.add_event_handler("destroy", GameSettings.destroy);
GameSettings.this_window.add_event_handler("change", GameSettings.destroy);
</script>
