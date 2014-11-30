<script type="text/javascript">
if (PureFW.Officer.get_current_active_officer())
	PureFW.Officer.get_current_active_officer().hide();

Profile = new Object();

Profile.this_window = PureFW.ContainerAJAX.instances[<?=$this_win_id?>];
Profile.this_window.set_css_class("Window_bright");
<? /* Enthält alle Items, die man kaufen kann. */ ?>
Profile.most_expensive_units = <?= json_encode($most_expensive_units) ?>;
Profile.avatars = <?= json_encode($avatar_data) ?>;

Profile.all_medals = <?= json_encode($all_medals) ?>;
Profile.my_medals = <?= json_encode($my_medals) ?>;
Profile.best_medals = <?= json_encode($best_3_medals) ?>;
Profile.medal_cat_names = <?= json_encode($medal_cat_names) ?>;

Profile.player_stats = new Array();
<? for ($i = 0; $i < count($map_ranking); $i++) : ?>
<?	if ($map_ranking[$i]->get_id() == $profile_user->get_id())
		continue;
	$user = $map_ranking[$i];
	$color = MapPlayers::get_instance($profile_world_id, $profile_map_id)
				->get_color($user->get_id());

	$nick = $user->get_nick();
	$personal_stuff = $user->get_personal_data();
	$wcf_uid = $personal_stuff->id_wcf; ?>
	Profile.tmp = new Object();
	Profile.tmp.nick = "<?= $nick ?>";
	Profile.tmp.color = "<?= $color ?>";
	Profile.tmp.wcf_uid = "<?= $wcf_uid ?>";
	Profile.player_stats.push(Profile.tmp);
<? endfor ?>
Profile.buddies = <?= json_encode($buddies) ?>;

<? /**
	* Hilfsfunktion, um die Freunde zu erstellen
	*
	* @param object[] array		Array, das die Freunde enthält
	* @param uint x				x-Position, wo das Icon erscheinen soll
	* @param unit y				y-Position, wo das Icon erscheinen soll
	**/ ?>
Profile.create_friends_helper = function(array, x, y) {
	var n = array.length;
	for(var i = 0; i < n; i++)
	{
		var temp = Profile.friends_container.add_widget(
			PureFW.Avatar.Avatar,
			x, y,
			48, 48,
			Profile.avatars[array[i].nick].replace('SIZE',44),
			array[i].nick
		);

		if (x >= 58) {
			y += 58;
			x -= 58;
		}
		else {
			x += 58;
		}
	}
	o = new Object();
	o.x = x; o.y = y;
	return o;
}

<?  /**
	 * Erstellt die Icons der Freunde
	 **/ ?>
Profile.create_friends = function()
{
	var x = 10;
	var y = 10;
	var temp;
	if (typeof(Profile.buddies.onlineFriends) != 'undefined') {
		temp = Profile.create_friends_helper(Profile.buddies.onlineFriends, x, y);
		x = temp.x; y = temp.y;
	}
	if (typeof(Profile.buddies.offlineFriends) != 'undefined') {
		temp = Profile.create_friends_helper(Profile.buddies.offlineFriends, x, y);
		x = temp.x; y = temp.y;
	}
	if (x >= 58)
		Profile.friends_container.set_inner_height(y+58);
	else
		Profile.friends_container.set_inner_height(y);
}

<? /**
	* Erzeugt die Mitspielericons
	**/ ?>
Profile.create_playmates = function()
{
	var n = Profile.player_stats.length;
	var x = 10;
	var y = 10;
	for(var i = 0; i < n; i++)
	{
		var tmp = Profile.playmates_container.add_widget(
			PureFW.Avatar.Avatar,
			x, y,
			48, 48,
			Profile.avatars[Profile.player_stats[i].nick].replace('SIZE',44),
			Profile.player_stats[i].nick,
			Profile.player_stats[i].color
		);
		if ((i | 1) == i) {
			y += 58;
			x -= 58;
		}
		else {
			x += 58;
		}
	}
	if ((i | 1) == i)
		Profile.playmates_container.set_inner_height(y+58);
	else
		Profile.playmates_container.set_inner_height(y);
}

<? /**
	* Erzeugt die Icons für die bestbekleidetsten Einheiten
	**/ ?>
Profile.create_units = function() {
	if (!Profile.most_expensive_units)
		return;
	var n = Profile.most_expensive_units.length;
	for (var i = 0; i < n; i++) {
		var pose = ((i | 1) == i) ? 0 : 2;
		var pic_url = Profile.most_expensive_units[i].pic_url;
		pic_url = pic_url.replace(/\.png/g, "_"+pose+".png");
		var tmp = Profile.best_units_container.add_widget(
			PureFW.Image,
			i*50, 0,
			50, 50,
			'<?= pic_path('') ?>map/units/'+pic_url
		);

		var unit = Profile.most_expensive_units[i];
		unit.pic_url = pic_url;
		tmp.add_event_handler("click",
			(function (_unit) {
				return function (ev) {
					MammunUI.show_unit_details(_unit);
				}
			})(unit)
		);
	}
}

<? /**
	* Erzeugt die besten Medallien-Icons
	**/ ?>
Profile.create_best_medals = function()
{
	if (!Profile.best_medals)
		return;

	var n = Profile.best_medals.length;
	for (var i = 0; i < n; i++) {
		var medal = Profile.best_medals[i];

		var tmp = Profile.medals_container.add_widget(
			PureFW.Image,
			i*50, 0,
			50,54,
			'../pix/ui/icons/labels/medals/50x54/'+medal.id+'.png'
		);
		tmp.set_tooltip(medal.name);
		tmp.add_event_handler("click",
			(function (_medal) {
				return function(ev) {
					MammunUI.show_medal_details(_medal);
				}
			})(medal)
		);
	}
}

<? /**
	* Zeigt alle Orden in einer Liste an
	**/ ?>
Profile.show_all_medals = function() {
	var confirm_box = MammunUI.create_conf_box(
		810, 540, PureFW.ConfirmationBox.YES
	);

	var content = confirm_box.add_widget(
		PureFW.ScrollContainer,
		30, 20,
		confirm_box.width - 60, 0
	);
	var x = 0, y = -70;
	for (type in Profile.all_medals) {
		if (isNaN(type))
			continue;
		if (type == 8)
			continue;

		if (type != 1) {
			x = 0;
			y += 70;
			var tmp = content.add_widget(
				PureFW.Container,
				x, y,
				0,0
			);
			tmp.set_content(
				Profile.medal_cat_names[type] + ':'
			);
			tmp.set_font_size('headline_mid_simple');
			y += 30;
		}

		for (subtype in Profile.all_medals[type]) {
			if (isNaN(subtype))
				continue;
			for (amount in Profile.all_medals[type][subtype]) {
				if (isNaN(amount))
					continue;
				var medal_id = Profile.all_medals[type][subtype][amount];
				if (Profile.my_medals[type] && Profile.my_medals[type][subtype]
					&& Profile.my_medals[type][subtype][amount])
				{
					var tmp = content.add_widget(
						PureFW.Image,
						x, y,
						50, 54,
						'../pix/ui/icons/labels/medals/50x54/'+medal_id+'.png'
					);
					tmp.set_tooltip(
						Profile.my_medals[type][subtype][amount].name
					);
					tmp.add_event_handler("click",
						(function (_medal) {
							return function(ev) {
								MammunUI.show_medal_details(_medal);
							}
						})(Profile.my_medals[type][subtype][amount])
					);
				}
				else {
					var tmp = content.add_widget(
						PureFW.Image,
						x, y,
						50, 54,
						'../pix/ui/icons/labels/medals/50x54/empty/'+type+'.png'
					);
				}
				x += 50;
				if (x > content.width) {
					y += 50;
					x = 0;
				}
			}
		}
	}
	content.set_height(confirm_box.height - 60);
	content.set_inner_height(y+54);
}

<?	/**
	 * Zerstört das Fenster usw.
	 **/ ?>
Profile.destroy = function()
{
	if (PureFW.Officer.get_current_active_officer())
		PureFW.Officer.get_current_active_officer().show();
	Profile.this_window.remove_event_handler("destroy", Profile.destroy);
	Profile.this_window.remove_event_handler("change", Profile.destroy);
	destroy_object(Profile);
	Profile = null;
	UINewProfile.window = null;
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


<?
/**
 * 1. Spalte: Portrait und Freunde
 */ ?>

Profile.portrait_container = new PureFW.Container(
	Profile.this_window,
	20, 5,
	154, 154
);
Profile.portrait_container.set_bg_img(
	'ui/backgrounds/profile/bg_shadow.png'
);
Profile.portrait_image = new PureFW.Image(
	Profile.portrait_container,
	2, 1,
	150, 150,
	Profile.avatars['<?= $profile_user->get_nick()?>'].replace('SIZE',150)
);

Profile.friends_over_container = new PureFW.Container(
	Profile.this_window,
	Profile.portrait_container.position.x + 3,
	Profile.portrait_container.position.y +
		Profile.portrait_container.height + 25,
	150, 205
);
Profile.friends_over_container.set_bg_img('ui/backgrounds/profile/bg_grouping.png');
Profile.friends_container = new PureFW.ScrollContainer(
	Profile.friends_over_container,
	13, 11,
	Profile.friends_over_container.width - 26,
	Profile.friends_over_container.height - 22);


Profile.friends_text = new PureFW.Image(
	Profile.this_window,
	Profile.friends_over_container.position.x - 10,
	Profile.friends_over_container.position.y - 20,
	111, 40,
	'ui/elements/texts/<?=$LNG?>/profile/label_friends.png'
);
Profile.friends_text.set_z_index(2);


<?
/**
 * 2. Spalte: Benutzername und Aktionen zum Benutzer, sowie Steckbrief
 */ ?>

Profile.nick_container = new PureFW.Container(
	Profile.this_window,
	Profile.portrait_container.position.x
		+ Profile.portrait_container.width + 30,
	Profile.portrait_container.position.y,
	170, 30
);
Profile.nick_container.set_content(
	MammunUI.beautify_nick('<?= $profile_user->get_nick() ?>')
);
<? if ((($current_user->get_nick() == "Robert") || ($current_user->get_nick()
	== "Philipp")) && $profile_user_personal->id_fb) : ?>
Profile.nick_container.add_content(
	'<a href="http://www.facebook.com/profile.php?id=<?=
		$profile_user_personal->id_fb ?>" target="_blank">(FB)</a>');
<? endif; ?>

Profile.nick_container.set_font_size(1.4);
Profile.nick_container.set_font_color('#333333');
Profile.nick_container.set_font_weight('bold');



Profile.characteristics_container = new PureFW.Container(
	Profile.this_window,
	Profile.nick_container.position.x - 8,
	Profile.friends_over_container.position.y +
		Profile.friends_over_container.height - 329 + 16,
	559, 329
);
Profile.characteristics_container.set_bg_img(
	'ui/backgrounds/profile/bg_characteristics.png'
);
Profile.characteristics_container.set_font_color('#333333');

Profile.characteristics_text = new PureFW.Image(
	Profile.this_window,
	Profile.characteristics_container.position.x,
	Profile.characteristics_container.position.y - 20,
	115, 40,
	'ui/elements/texts/<?=$LNG?>/profile/label_characteristics.png'
);
Profile.characteristics_text.set_z_index(2);

// von rechts ausgerichtet am rechten Steckbriefrand
<? /** FREUNDSCHAFTSBUTTON MIT AKTIONEN */ ?>
Profile.add_friend_btn = new PureFW.Container(
	Profile.this_window,
	Profile.characteristics_container.position.x
		+ Profile.characteristics_container.width
		- 152,
	Profile.portrait_container.position.y,
	152, 46
);
Profile.add_friend_btn.set_font_color('#333333');
<? if (($friend_status == 0)
		&& ($profile_user->get_id() != $current_user->get_id())): ?>
Profile.add_friend_btn.set_bg_img(
	'ui/elements/form/buttons/<?=$LNG?>/profile/add_as_friend.png'
);
Profile.add_friend_function = function(ev) {
	PureFW.AJAXClientServer.send_request(
		'../community/wbb/index.php?form=WhiteListEdit'
			+ '&add=<?= $profile_user_personal->id_wcf ?>'
			+ '&u=<?= $current_user_personal->id_wcf ?>'
			+ '&s=<?= $_SESSION['WCF_SID']?>&t=<?= $_SESSION['WCF_ST'] ?>',
		function(response) {
			if ((typeof(PauseMenu) != 'undefined') && (PauseMenu)) {
				PauseMenu.refresh_buddies();
			}
			Profile.add_friend_btn.set_bg_img('pattern/spacer.gif');
			Profile.add_friend_btn.set_content(
				'<?= LNG_USER_FRIEND_REQUEST_PENDING ?>'
			);
			Profile.add_friend_btn.remove_event_handler("click",
				Profile.add_friend_function
			);
		},null,true
	);
}
Profile.add_friend_btn.add_event_handler("click",
	Profile.add_friend_function
);
<? else: ?>
<? if ($profile_user->get_id() == $current_user->get_id()) : ?>
Profile.add_friend_btn.set_content(
	'<?= LNG_USER_IS_YOURSELF ?>'
);
<? elseif ($friend_status == 1) : ?>
Profile.add_friend_btn.set_content(
	'<?= LNG_USER_FRIEND_REQUEST_PENDING ?>'
);
<? elseif ($friend_status == 2) : ?>
Profile.add_friend_btn.set_content(
	'<?= str_replace('[NICK]', $profile_user->get_nick_main(),
		LNG_USER_IS_YOUR_FRIEND) ?>'
);
<? endif; ?>
<? endif; ?>
<? /** ENDE MIT DEM FREUNDSCHAFTSBUTTON */ ?>
Profile.chest_icon = new PureFW.Image(
	Profile.this_window,
	Profile.add_friend_btn.position.x - 48 - 20,
	Profile.add_friend_btn.position.y - 1,
	42, 48,
	'ui/icons/labels/communication/42x48/chest.png'
);
Profile.chest_icon.add_event_handler("click",
	function(ev) {
		UIButtons.toggle_treasure_chest("nick=<?=$profile_user->get_nick() ?>",
			true);
	}
);
Profile.chest_icon.set_tooltip(
	'<?= LNG_TITLE_CHEST_BUTTON ?>'
		.replace('[NICK]', '<?=$profile_user->get_nick() ?>')
		.replace('&#039;', '\'')
);
Profile.pm_icon = new PureFW.Image(
	Profile.this_window,
	Profile.chest_icon.position.x - 48 - 5,
	Profile.chest_icon.y,
	42, 48,
	'ui/icons/labels/communication/42x48/pm.png'
);
Profile.pm_icon.add_event_handler("click",
	function(ev) {
		UIButtons.toggle_new_message(
			"recipients=<?=$profile_user->get_nick() ?>");
	}
);
Profile.pm_icon.set_tooltip(
	'<?= LNG_TITLE_PM_BUTTON ?>'
		.replace('[NICK]', '<?=$profile_user->get_nick() ?>')
);

<?
/**
 * 3. Spalte: Kartenbild und -Aktionen sowie Mitspieler
 */ ?>

Profile.map_container = new PureFW.Container(
	Profile.this_window,
	Profile.this_window.width - 20 - 154,
	Profile.portrait_container.position.y,
	154, 154
);
Profile.map_container.set_bg_img(
	'ui/backgrounds/profile/bg_shadow.png'
);
Profile.map_img = new PureFW.Image(
	Profile.map_container,
	2, 1,
	150, 150,
	'<?= pic_path('') ?>map/templates/<?= $template_id ?>/_0_/template'+
		'<?= $template_id ?>_profile.jpg'
);
<? if ($profile_world_id) : ?>
Profile.map_img.add_event_handler("click",
	function (ev) {
		MammunUI.get_top_frame().MapUI.change_map(
			<?= $profile_world_id ?>, <?= $profile_map_id ?>
		);
	}
);
Profile.map_img_text = new PureFW.Container(
	Profile.map_container,
	0, Profile.map_container.height - 60,
	Profile.map_container.width, 0
);
Profile.map_img_text.set_text_align('center');
Profile.map_img_text.set_css_class('Window_dark');
Profile.map_img_text.set_font_size(0.9);
Profile.map_img_text.set_content(
	'<?= str_replace('[NICK]', $profile_user->get_nick_main(),
			LNG_VISIT_USER_ON_MAP) ?>'
);
Profile.map_img_text.add_event_handler("click",
	function (ev) {
		MammunUI.get_top_frame().MapUI.change_map(
			<?= $profile_world_id ?>, <?= $profile_map_id ?>
		);
	}
);
<? endif; ?>

Profile.playmates_over_container = new PureFW.Container(
	Profile.this_window,
	Profile.map_container.position.x + 3,
	Profile.friends_over_container.position.y,
	150, 205
);
Profile.playmates_over_container.set_bg_img(
	'ui/backgrounds/profile/bg_grouping.png'
);
Profile.playmates_container = new PureFW.ScrollContainer(
	Profile.playmates_over_container,
	13, 11,
	Profile.playmates_over_container.width - 26,
	Profile.playmates_over_container.height - 22);


Profile.playmates_text = new PureFW.Image(
	Profile.this_window,
	Profile.playmates_over_container.position.x - 10,
	Profile.playmates_over_container.position.y - 20,
	112, 40,
	'ui/elements/texts/<?=$LNG?>/profile/label_playmates.png'
);
Profile.playmates_text.set_z_index(2);


<?/*
<? if ($profile_world_id) { ?>
Profile.portrait_container.set_content(
	'<a href="javascript: MammunUI.get_top_frame().MapUI.change_map(
		<?= $profile_world_id ?>, <?= $profile_map_id ?>);'
		+ '">Aktuelle Insel</a><br/><br/>'
);
<? } ?>
<? foreach ($won_maps as $map_id ???? WORLD_ID???) { ?>
Profile.portrait_container.add_content(
	'<a href="javascript: MammunUI.get_top_frame().MapUI.change_map(<?= $map_id ?>);'
	+ '">Insel <?= $map_id ?></a><br/>'
);
<? } ?>
Profile.portrait_container.set_font_size(1.2);
*/?>

<?
/**
 * Jetzt alles vorbereitete füllen. Füllen füllen füllen :D
 */
?>

<?
/**
 * Steckbrieffüllung
 *
 * Linke Seite
 */
?>
Profile.y_skip_offset = 0;
Profile.login_date_text = new PureFW.Image(
	Profile.characteristics_container,
	30, 40,
	138, 18,
	'ui/elements/texts/<?=$LNG?>/profile/txt_login_date2.png'
);
Profile.login_date_value = new PureFW.Container(
	Profile.characteristics_container,
	Profile.login_date_text.position.x + Profile.login_date_text.width
		+ 2,
	Profile.login_date_text.position.y-2,
	200, 19
);
Profile.login_date_value.set_content(
	'<?= date('d.m.Y', $profile_user->get_signup()) ?>'
);

Profile.maps_played_text = new PureFW.Image(
	Profile.characteristics_container,
	Profile.login_date_text.position.x,
	Profile.login_date_text.position.y + Profile.login_date_text.height
		+ 2,
	138, 18,
	'ui/elements/texts/<?=$LNG?>/profile/txt_maps_played2.png'
);
Profile.maps_played_value = new PureFW.Container(
	Profile.characteristics_container,
	Profile.maps_played_text.position.x + Profile.maps_played_text.width
		+ 2,
	Profile.maps_played_text.position.y-2,
	200, 19
);
Profile.maps_played_value.set_content(
	'<?= $profile_user->get_maps_won() + $profile_user->get_maps_lost() ?>'
);

Profile.maps_won_text = new PureFW.Image(
	Profile.characteristics_container,
	Profile.maps_played_text.position.x,
	Profile.maps_played_text.position.y + Profile.maps_played_text.height
		+ 20,
	138, 18,
	'ui/elements/texts/<?=$LNG?>/profile/txt_maps_won2.png'
);
Profile.maps_won_value = new PureFW.Container(
	Profile.characteristics_container,
	Profile.maps_won_text.position.x + Profile.maps_won_text.width
		+ 2,
	Profile.maps_won_text.position.y-2,
	200, 19
);
Profile.maps_won_value.set_content(
	'<?= $profile_user->get_maps_won() ?>'
);

Profile.chest_item_amount_text = new PureFW.Image(
	Profile.characteristics_container,
	Profile.maps_won_text.position.x,
	Profile.maps_won_text.position.y + Profile.maps_won_text.height
		+ 2,
	138, 18,
	'ui/elements/texts/<?=$LNG?>/profile/txt_chest_item_amount2.png'
);
Profile.chest_item_amount_value = new PureFW.Container(
	Profile.characteristics_container,
	Profile.chest_item_amount_text.position.x + Profile.chest_item_amount_text.width
		+ 2,
	Profile.chest_item_amount_text.position.y-2,
	200, 19
);
Profile.chest_item_amount_value.set_content(
	'<?= $profile_user->get_maps_won() + $profile_user->get_maps_lost() ?>'
);

<?
$b = explode('-', $profile_user_personal->birthday);
if ( !(int)$b[0] && !(int)$b[1] && !(int)$b[2] ) : ?>
<?
else :
$b = $b[2] . '.' . $b[1] . '.' . $b[0];
?>
Profile.birthday_text = new PureFW.Image(
	Profile.characteristics_container,
	Profile.chest_item_amount_text.position.x,
	Profile.chest_item_amount_text.position.y
		+ Profile.chest_item_amount_text.height
		+ 20,
	138, 18,
	'ui/elements/texts/<?=$LNG?>/profile/txt_birthday2.png'
);
Profile.birthday_value = new PureFW.Container(
	Profile.characteristics_container,
	Profile.birthday_text.position.x + Profile.birthday_text.width
		+ 2,
	Profile.birthday_text.position.y-2,
	200, 19
);
Profile.birthday_value.set_content(
	'<?= $b ?>'
);
Profile.y_skip_offset += 39;
<? endif; ?>
<? if (empty($profile_user_personal->city)) : ?>
<? else : ?>
Profile.city_text = new PureFW.Image(
	Profile.characteristics_container,
	Profile.chest_item_amount_text.position.x,
	Profile.chest_item_amount_text.position.y
		+ Profile.chest_item_amount_text.height
		+ 2 + Profile.y_skip_offset,
	138, 18,
	'ui/elements/texts/<?=$LNG?>/profile/txt_city2.png'
);
Profile.city_value = new PureFW.Container(
	Profile.characteristics_container,
	Profile.city_text.position.x + Profile.city_text.width
		+ 2,
	Profile.city_text.position.y-2,
	200, 19
);
Profile.city_value.set_content(
	'<?= ucfirst($profile_user_personal->city) ?>'
);
Profile.y_skip_offset += 21;
<? endif; ?>
<? if (empty($profile_user_personal->country)): ?>
<? else : ?>
Profile.country_text = new PureFW.Image(
	Profile.characteristics_container,
	Profile.chest_item_amount_text.position.x,
	Profile.chest_item_amount_text.position.y
		+ Profile.chest_item_amount_text.height
		+ 2 + Profile.y_skip_offset,
	138, 18,
	'ui/elements/texts/<?=$LNG?>/profile/txt_country2.png'
);
Profile.country_value = new PureFW.Container(
	Profile.characteristics_container,
	Profile.country_text.position.x + Profile.country_text.width
		+ 2,
	Profile.country_text.position.y-2,
	200, 19
);
Profile.country_value.set_content(
	'<?= @constant("LNG_ISO3166_{$profile_user_personal->country}") ?>'
);
Profile.y_skip_offset += 21;
<? endif; ?>
<?
/**
 * Steckbrieffüllung
 *
 * Rechte Seite
 */
?>
<? /** Beste Items */ ?>
Profile.best_items_text = Profile.characteristics_container.add_widget(
	PureFW.Image,
	Profile.characteristics_container.width - 180,
	Profile.login_date_text.position.y,
	166, 20,
	'ui/elements/texts/<?=$LNG?>/profile/txt_best_items2.png'
);
Profile.best_units_container = Profile.characteristics_container.add_widget(
	PureFW.Container,
	Profile.best_items_text.position.x +
		((Profile.best_items_text.width - 150) >> 1),
	Profile.best_items_text.position.y + Profile.best_items_text.height + 10,
	150,
	50
);
Profile.go_to_chest_cont = Profile.characteristics_container.add_widget(
	PureFW.Container,
	Profile.best_units_container.position.x-5,
	Profile.best_units_container.position.y
		+ Profile.best_units_container.height + 10,
	Profile.best_units_container.width+10,
	20
);
Profile.go_to_chest_cont.set_text_align('center');
Profile.go_to_chest_cont.set_content(
	'<a href="javascript: UIButtons.toggle_treasure_chest(\'nick='
		+ '<?=$profile_user->get_nick() ?>\')"><?= LNG_BTN_GO_TO_CHEST ?></a>'
);

<? /** Orden */ ?>
Profile.medals_text = Profile.characteristics_container.add_widget(
	PureFW.Image,
	Profile.best_items_text.position.x,
	Profile.go_to_chest_cont.position.y + Profile.go_to_chest_cont.height
		+ 20,
	62, 18,
	'ui/elements/texts/<?=$LNG?>/profile/txt_medals.png'
);
Profile.medals_container = Profile.characteristics_container.add_widget(
	PureFW.Container,
	Profile.best_units_container.position.x,
	Profile.medals_text.position.y + Profile.medals_text.height + 10,
	150, 54
);
Profile.show_medals_cont = Profile.characteristics_container.add_widget(
	PureFW.Container,
	Profile.medals_container.position.x-5,
	Profile.medals_container.position.y
		+ Profile.medals_container.height + 10,
	Profile.medals_container.width+10,
	20
);
Profile.show_medals_cont.set_text_align('center');
Profile.show_medals_cont.set_content(
	'<a href="javascript: Profile.show_all_medals();"><?=
		LNG_BTN_SHOW_MEDALS ?></a>'
);

Profile.create_units();
Profile.create_friends();
Profile.create_playmates();
Profile.create_best_medals();

Profile.this_window.add_event_handler("destroy", Profile.destroy);
Profile.this_window.add_event_handler("change", Profile.destroy);
</script>