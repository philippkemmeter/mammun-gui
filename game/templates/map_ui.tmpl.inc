<?
Language::get_instance()->load_language_file('twitter_publish');
$header_add = '
<style type="text/css">@import "../css/Widgets.css?'.filemtime(dirname(__FILE__).'/../../css/Widgets.css').'";</style>
<script type="text/javascript" src="../js/classes/MammunUI.js?'.filemtime('../js/classes/MammunUI.js').'"></script>
<script type="text/javascript">
MammunUI.facebook_app_url = "'.CONF_FACEBOOK_APP_URL.'?map_key='.$auth_key_ext.'&visit='.@$_SESSION['visit'].'";

MammunUI.user_in_facebook = '.((isset($_SESSION['fb']['fb_sig'])||isset($_SESSION['fb']['session'])) ? "true" : "false").'
MammunUI.LNG = "'.$LNG.'";
MammunUI.LNG_WARNING_NOT_IN_FACEBOOK = "'.str_replace(
				array('[A]', '[/A]'),
				array('<a href=\"javascript: return false;\" onclick=\"window.open('.
					'\'https://graph.facebook.com/oauth/authorize?client_id='
						.CONF_FACEBOOK_APP_ID.
						'&redirect_uri=http%3A%2F%2Fwww.mammun.com%2Fconnect'.
							'%2Fsuccess.php%3F'.SID.'%26try_connect=true'.
						'&scope=email&display=popup\', '.
						'\'_blank\', \'top=300,width=640,height=400,'.
						'resizable=yes\', true);\">',
					'</a>'),
				LNG_UI_WARNING_NOT_IN_FACEBOOK) .'";

</script>
<script type="text/javascript" src="../js/classes/SocialNetworks.js?'.filemtime('../js/classes/SocialNetworks.js').'"></script>
<script type="text/javascript">
SocialNetworks.in_fb = '.((isset($_SESSION['fb']['fb_sig'])||isset($_SESSION['fb']['session'])) ? "true" : "false").'
SocialNetworks.Facebook.LNG_WARNING_NOT_IN_FACEBOOK = "'.str_replace(
				array('[A]', '[/A]'),
				array('<a href=\"'.CONF_FACEBOOK_APP_URL.'\">','</a>'),
				LNG_UI_WARNING_NOT_IN_FACEBOOK) .'";
SocialNetworks.Twitter.LNG_SHARE_GENERAL = "'.LNG_TWITTER_SHARE_GENERAL.'";
SocialNetworks.Twitter.LNG_SHARE_INVITE_TO_MAP =
	"'.LNG_TWITTER_SHARE_INVITE_TO_MAP.'";
</script>
<script type="text/javascript" src="../js/PureFW/pure_fw_all.js?'.filemtime('../js/PureFW/pure_fw_all.js').'"></script>
<script type="text/javascript" src="../js/MathExt/math_ext_all.js?'.filemtime('../js/MathExt/math_ext_all.js').'"></script>
<script type="text/javascript">
PureFW.AJAXClientServer.session_name = "'.session_name().'";
PureFW.AJAXClientServer.session_id= "'.session_id().'";
</script>
<script type="text/javascript" src="../js/PureFW/jstween/jstween.js?'.filemtime('../js/PureFW/jstween/jstween.js').'"></script>
<script type="text/javascript" src="../js/extern/formatdate.js?'.filemtime(dirname(__FILE__).'/../../js/extern/formatdate.js').'"></script>
<script type="text/javascript">
PureFW.Time.set_init_server_time('.Time::get_t_now().');
PureFW.Time.LNG_DAY = "'.LNG_TIME_DAY.'";
PureFW.Time.LNG_DAYS = "'.LNG_TIME_DAYS.'";
</script>
<script type="text/javascript" src="../js/gen_funcs.js?'.filemtime(dirname(__FILE__).'/../../js/gen_funcs.js').'"></script>
<script type="text/javascript" src="../js/classes/Widgets/TextBullet.js?'.filemtime('../js/classes/Widgets/TextBullet.js').'"></script>
<script type="text/javascript" src="../js/classes/Widgets/RadioLabel.js?'.filemtime('../js/classes/Widgets/RadioLabel.js').'"></script>
<script type="text/javascript" src="../js/classes/Widgets/FullWindowAJAX.js?'.filemtime(dirname(__FILE__).'/../../js/classes/Widgets/FullWindowAJAX.js').'"></script>
<script type="text/javascript" src="../js/classes/PopupMenu.js?'.filemtime(dirname(__FILE__).'/../../js/classes/PopupMenu.js').'"></script>
<style type="text/css">@import "../css/PopupMenu.css?'.filemtime(dirname(__FILE__).'/../../css/PopupMenu.css').'";</style>
<script type="text/javascript" src="../js/classes/datastructures/WGraph.js?'.filemtime('../js/classes/datastructures/WGraph.js').'"></script>
<script type="text/javascript" src="../js/classes/datastructures/MinHeap.js?'.filemtime('../js/classes/datastructures/MinHeap.js').'"></script>
<script type="text/javascript" src="../js/classes/Pathfinder.js?'.filemtime('../js/classes/Pathfinder.js').'"></script>
<script type="text/javascript" src="../js/classes/ParticleSys.js?'.filemtime('../js/classes/ParticleSys.js').'"></script>
<script type="text/javascript" src="../js/classes/Widgets/Grid.js?'.filemtime(dirname(__FILE__).'/../../js/classes/Widgets/Grid.js').'"></script>
<script type="text/javascript" src="../js/classes/Widgets/UnitSlot.js?'.filemtime(dirname(__FILE__).'/../../js/classes/Widgets/UnitSlot.js').'"></script>
<script type="text/javascript" src="../js/classes/Widgets/HealthBar.js?'.filemtime(dirname(__FILE__).'/../../js/classes/Widgets/HealthBar.js').'"></script>
<script type="text/javascript" src="../js/classes/Widgets/Person.js?'.filemtime(dirname(__FILE__).'/../../js/classes/Widgets/Person.js').'"></script>
<script type="text/javascript" src="../js/classes/Widgets/ConfirmationBox.js?'.filemtime(dirname(__FILE__).'/../../js/classes/Widgets/ConfirmationBox.js').'"></script>
<script type="text/javascript" src="../js/classes/Widgets/PublishConfirmBox.js?'.filemtime(dirname(__FILE__).'/../../js/classes/Widgets/PublishConfirmBox.js').'"></script>
<script type="text/javascript" src="../js/classes/Widgets/Officer.js?'.filemtime(dirname(__FILE__).'/../../js/classes/Widgets/Officer.js').'"></script>
<script type="text/javascript" src="../js/classes/Widgets/OfficerBox.js?'.filemtime(dirname(__FILE__).'/../../js/classes/Widgets/OfficerBox.js').'"></script>
<script type="text/javascript" src="../js/classes/Widgets/OfficerBalloon.js?'.filemtime(dirname(__FILE__).'/../../js/classes/Widgets/OfficerBalloon.js').'"></script>
<script type="text/javascript" src="../js/classes/Widgets/BuildingInSector.js?'.filemtime(dirname(__FILE__).'/../../js/classes/Widgets/BuildingInSector.js').'"></script>
<script type="text/javascript" src="../js/classes/Widgets/AJAXInfoBalloon.js?'.filemtime('../js/classes/Widgets/AJAXInfoBalloon.js').'"></script>
<script type="text/javascript" src="../js/classes/Widgets/SlideTable.js?'.filemtime('../js/classes/Widgets/SlideTable.js').'"></script>
<script type="text/javascript" src="../js/classes/Widgets/ShopItem.js?'.filemtime('../js/classes/Widgets/ShopItem.js').'"></script>
<script type="text/javascript" src="../js/classes/Widgets/ChestItem.js?'.filemtime('../js/classes/Widgets/ChestItem.js').'"></script>
<script type="text/javascript" src="../js/classes/Widgets/BubbleBar.js?'.filemtime('../js/classes/Widgets/BubbleBar.js').'"></script>
<script type="text/javascript" src="../js/classes/Widgets/BubbleScroller.js?'.filemtime('../js/classes/Widgets/BubbleScroller.js').'"></script>
<script type="text/javascript" src="../js/classes/Widgets/map_objects/Animal.js?'.filemtime('../js/classes/Widgets/map_objects/Animal.js').'"></script>
<script type="text/javascript" src="../js/classes/Widgets/map_objects/Mammun.js?'.filemtime('../js/classes/Widgets/map_objects/Mammun.js').'"></script>
<script type="text/javascript" src="../js/classes/Widgets/avatar/Avatar.js?'.filemtime('../js/classes/Widgets/avatar/Avatar.js').'"></script>
<script type="text/javascript" src="../js/classes/Widgets/avatar/AvatarCheckbox.js?'.filemtime('../js/classes/Widgets/avatar/AvatarCheckbox.js').'"></script>
<script type="text/javascript" src="../js/classes/Widgets/avatar/AvatarXP.js?'.filemtime('../js/classes/Widgets/avatar/AvatarXP.js').'"></script>
<script type="text/javascript" src="../js/classes/SectorPrint.js?'.filemtime(dirname(__FILE__).'/../../js/classes/SectorPrint.js').'"></script>
<script type="text/javascript" src="../js/classes/Gainer.js?'.filemtime(dirname(__FILE__).'/../../js/classes/Gainer.js').'"></script>
<script type="text/javascript" src="../js/classes/DocumentLoadListener.js?'.filemtime(dirname(__FILE__).'/../../js/classes/DocumentLoadListener.js').'"></script>
<script type="text/javascript" src="../js/classes/TutorialCtrl.js?'.filemtime(dirname(__FILE__).'/../../js/classes/TutorialCtrl.js').'"></script>
<style type="text/css">@import "../css/MammunMap.css?'.filemtime(dirname(__FILE__).'/../../css/MammunMap.css').'";</style>
<script type="text/javascript" src="../js/classes/Widgets/game_map/SectorData.js?'.filemtime('../js/classes/Widgets/game_map/SectorData.js').'"></script>
<script type="text/javascript" src="../js/classes/Widgets/game_map/BuildingImage.js?'.filemtime('../js/classes/Widgets/game_map/BuildingImage.js').'"></script>
<script type="text/javascript" src="../js/classes/Widgets/game_map/ClickRectMap.js?'.filemtime('../js/classes/Widgets/game_map/ClickRectMap.js').'"></script>
<script type="text/javascript" src="../js/classes/Widgets/game_map/MammunMap.js?'.filemtime('../js/classes/Widgets/game_map/MammunMap.js').'"></script>
<script type="text/javascript" src="../js/classes/Widgets/game_map/PositionChooseMap.js?'.filemtime('../js/classes/Widgets/game_map/PositionChooseMap.js').'"></script>
<script type="text/javascript" src="../js/classes/Widgets/game_map/BuildingsField.js?'.filemtime('../js/classes/Widgets/game_map/BuildingsField.js').'"></script>
<script type="text/javascript" src="../js/classes/Widgets/game_map/PathFindField.js?'.filemtime('../js/classes/Widgets/game_map/PathFindField.js').'"></script>
<script type="text/javascript" src="../js/classes/Widgets/game_map/MammunSectorZoom.js?'.filemtime('../js/classes/Widgets/game_map/MammunSectorZoom.js').'"></script>
<script type="text/javascript" src="../js/classes/Widgets/SpecialShopOffer.js?'.filemtime('../js/classes/Widgets/SpecialShopOffer.js').'"></script>
<script type="text/javascript" src="../js/classes/Widgets/PolaroidContainer.js?'.filemtime('../js/classes/Widgets/PolaroidContainer.js').'"></script>
<script type="text/javascript" src="../js/classes/Widgets/TradeItem.js?'.filemtime('../js/classes/Widgets/TradeItem.js').'"></script>
<script type="text/javascript" src="../js/classes/Widgets/PolaroidContainerOverlay.js?'.filemtime('../js/classes/Widgets/PolaroidContainerOverlay.js').'"></script>
<script type="text/javascript" src="../js/classes/Widgets/PolaroidMultiPic.js?'.filemtime('../js/classes/Widgets/PolaroidMultiPic.js').'"></script>
<script type="text/javascript" src="../js/classes/Widgets/choose_map/MapTemplatePreview.js?'.filemtime('../js/classes/Widgets/choose_map/MapTemplatePreview.js').'"></script>
<script type="text/javascript" src="../js/classes/Widgets/choose_map/IslandPreview.js?'.filemtime('../js/classes/Widgets/choose_map/IslandPreview.js').'"></script>
<script type="text/javascript" src="../js/classes/Widgets/choose_map/NewIslandPreview.js?'.filemtime('../js/classes/Widgets/choose_map/NewIslandPreview.js').'"></script>
<script type="text/javascript" src="../js/classes/Widgets/map_ui/XPBar.js?'.filemtime('../js/classes/Widgets/map_ui/XPBar.js').'"></script>
<script type="text/javascript" src="../js/classes/Widgets/map_ui/MapVisitNaviIcon.js?'.filemtime('../js/classes/Widgets/map_ui/MapVisitNaviIcon.js').'"></script>
<script type="text/javascript" src="../js/classes/Widgets/items/RarenessMeter.js?'.filemtime('../js/classes/Widgets/items/RarenessMeter.js').'"></script>
<script type="text/javascript" src="../js/classes/Widgets/items/ClothPreview.js?'.filemtime('../js/classes/Widgets/items/ClothPreview.js').'"></script>
<script type="text/javascript" src="../js/classes/Widgets/items/UnitTypeChooser.js?'.filemtime('../js/classes/Widgets/items/UnitTypeChooser.js').'"></script>
<script type="text/javascript" src="../js/classes/Widgets/items/ShopCatalog.js?'.filemtime('../js/classes/Widgets/items/ShopCatalog.js').'"></script>
<script type="text/javascript" src="../js/classes/Widgets/sector_details/MammunSpeechBubble.js?'.filemtime('../js/classes/Widgets/sector_details/MammunSpeechBubble.js').'"></script>
<script type="text/javascript" src="../js/classes/Widgets/QuantityDisplay.js?'.filemtime('../js/classes/Widgets/QuantityDisplay.js').'"></script>
<script type="text/javascript" src="../js/classes/Widgets/fireworks/Fireworks.js?'.filemtime('../js/classes/Widgets/fireworks/Fireworks.js').'"></script>
<script type="text/javascript" src="../js/classes/MapDataCtrl.js?'.filemtime('../js/classes/MapDataCtrl.js').'"></script>
<script type="text/javascript">MapDataCtrl.LNG.WARNING_MOVE_TO_DARK_SECTOR = "'.LNG_WARNING_MOVE_TO_DARK_SECTOR.'";</script>
<script type="text/javascript" src="../js/classes/EventMessage.js?'.filemtime('../js/classes/EventMessage.js').'"></script>
<script type="text/javascript">
EventMessage.TYPE_SC_FINISHED = '.IEventLog::TYPE_SC_FINISHED.';
EventMessage.TYPE_TRAINING_FINISHED_SCIENTIST = '.IEventLog::TYPE_TRAINING_FINISHED_SCIENTIST.';
EventMessage.TYPE_SPECIAL_MIN = '.IEventLog::TYPE_SPECIAL_MIN.';
EventMessage.TYPE_OPEN_INVITATION_DIALOG = '.IEventLog::TYPE_OPEN_INVITATION_DIALOG.';
EventMessage.TYPE_OPEN_COL_CHOOSE_DIALOG = '.IEventLog::TYPE_OPEN_COL_CHOOSE_DIALOG.';
EventMessage.TYPE_OPEN_COL_CHOOSE_DIALOG_INVITE = '.IEventLog::TYPE_OPEN_COL_CHOOSE_DIALOG_INVITE.';
EventMessage.TYPE_SPECIAL_MAX = '.IEventLog::TYPE_SPECIAL_MAX.';
</script>
<script type="text/javascript" src="../js/classes/MammunSpeech.js?'.filemtime('../js/classes/MammunSpeech.js').'"></script>
<script type="text/javascript" src="../js/classes/NameSuggestions.js?'.filemtime('../js/classes/NameSuggestions.js').'"></script>

<script type="text/javascript">
SID_ARG_2ND="&s='.$_SESSION['WCF_SID'].'";
</script>
<script type="text/javascript" src="../js/extern/WCF/default.js?'.filemtime('../js/extern/WCF/default.js').'"></script>
<script type="text/javascript" src="../js/extern/WCF/Wysiwyg.class.js?'.filemtime('../js/extern/WCF/Wysiwyg.class.js').'"></script>
<script type="text/javascript" src="../js/extern/WCF/AjaxRequest.class.js?'.filemtime('../js/extern/WCF/AjaxRequest.class.js').'"></script>
<script type="text/javascript" src="../js/extern/WCF/InlineListEdit.class.js?'.filemtime('../js/extern/WCF/InlineListEdit.class.js').'"></script>
<script type="text/javascript" src="../js/extern/WCF/PMListEdit.class.js?'.filemtime('../js/extern/WCF/PMListEdit.class.js').'"></script>
<script type="text/javascript" src="../js/extern/WCF/StringUtil.class.js?'.filemtime('../js/extern/WCF/StringUtil.class.js').'"></script>
<script type="text/javascript" src="../js/extern/WCF/Suggestion.class.js?'.filemtime('../js/extern/WCF/Suggestion.class.js').'"></script>



<script type="text/javascript">

/**
 * FB-Connect-Hack (sorry)
 */
login_fb_c_callback = function(response) {
	if (response.session) {
		document.href = \'http://apps.facebook.com/MammunIslands/\';
	}
}
MapUI = new Object();


MapUI.has_invited_somebody = true;
MapUI.active_key = null;
MapUI.scroll_intervals = new Array();
MapUI.handle_key_down = function(ev) {
	if (MapUI.active_key)	// bereits eine Taste runtergedrück - zwei glz werden
		return;		// nicht unterstützt
	try {
		var evtemp = PureFW.EventUtil.formatEvent(ev);
	}
	catch (e) {
	}
	if (evtemp.keyCode != 116)	// F5 zulassen
		evtemp.preventDefault();	// alle Tasten deaktivieren
	if(MammunUI.user_in_facebook)
		return;
	MapUI.active_key = evtemp.keyCode;
	switch (evtemp.keyCode) {
		case 13:	// Return
			//scroll_to_map_center();
			break;
		case 27:
			MammunUI.get_top_frame().GameMenu.open_game_menu();
			break;
		case 32:	// Space
			MapUI.scroll_to_hs();
			break;
		case 37:	// Links
			MapUI.start_map_scroll(-10, 0);
	  		break;
	 	case 38:	// Hoch
	 		MapUI.start_map_scroll(0, -10);
			break;
		case 39:	// Rechts
			MapUI.start_map_scroll(10, 0);
			break;
		case 40:	// Runter
			MapUI.start_map_scroll(0, 10);
			break;
	}
	MammunUI.get_top_frame().focus();	// Top fokussieren
}

MapUI.handle_key_up = function(ev) {
	try {
		 var evtemp = PureFW.EventUtil.formatEvent(ev);
	}
	catch (e) {
	}
	evtemp.preventDefault();	// alle Tasten deaktivieren
	MapUI.stop_map_scroll();
	MapUI.active_key = null;
	MammunUI.get_top_frame().focus();
}

MapUI.scroll_intervals = new Array();

MapUI.scroll_map_div_by = function(x,y) {
	var new_x = parseInt(document.getElemById("mapx").style.left) - x;
	var new_y = parseInt(document.getElemById("mapx").style.top) - y;
	MapUI.scroll_map_div_to(-new_x, -new_y);
}

MapUI.scroll_map_div_to = function(x,y) {
	var new_x = -x;
	var new_y = -y;
	if (new_x > 0)
		new_x = 0;
	else {
		var min_x = -(2000-map_ui_map_container.get_width());
		if (new_x < min_x)
			new_x = min_x;
	}
	if (new_y > 0)
		new_y = 0;
	else {
		var min_y = -(1400-map_ui_map_container.get_height());
		if (new_y < min_y)
			new_y = min_y;
	}
	document.getElemById("mapx").style.left = new_x+"px";
	document.getElemById("mapx").style.top = new_y+"px";
}

MapUI.start_map_scroll = function(x,y) {
	try{
		MammunUI.get_top_frame().mapx.startscroll(x, y);
	} catch(e) {
		MapUI.scroll_intervals.push(window.setInterval(
			"MapUI.scroll_map_div_by("+x+","+y+");", 20
		));
	}
}

MapUI.stop_map_scroll = function() {
	try {
		MammunUI.get_top_frame().mapx.stopscroll();
	}
	catch(e) {
	}
	var n = MapUI.scroll_intervals;
	for(var i = 0; i < n; i++) {
		window.clearInterval(MapUI.scroll_intervals[i]);
		MapUI.scroll_intervals.splice(i, 1);
	}
}

MapUI.scroll_to_hs = function() {
	MapUI.scroll_to_sector('.($current_user->get_homesector() % 100).');
}


MapUI.scroll_to_sector = function(sector) {
	var mp = MapDataCtrl.mammun_map;
	var x = sector % 10;
	var y = Math.floor(sector/10);
	var p = mp.get_field_pixel_position(x, y);
	p.x += mp.position.x;
	p.y += mp.position.y;

	var frame_size = getFrameSize();
	var w = mp.get_field_width() >> 1;
	var h = mp.get_field_height() >> 1;

	if (MapUI.no_iframes) {
		MapUI.scroll_map_div_to(p.x-(frame_size[0]>>1)+w,
			 p.y-(frame_size[1]>>1)+h);
	}
	else {
		MammunUI.get_top_frame().mapx.scrollTo(p.x-(frame_size[0]>>1)+w,
			p.y-(frame_size[1]>>1)+h);
	}
}

MapUI.no_iframes = (navigator.userAgent.toLowerCase().indexOf("firefox") == -1);


MapUI.ingame_show_loading = function() {
	try {
		MapUI.map_loading_cont.destroy();
		MapUI.map_loading_cont = null;
		MapUI.map_loading_pic.destroy();
		MapUI.map_loading_pic = null;
	}
	catch(e) {}
	MapUI.map_loading_cont = new PureFW.Container(
		document.body,
		0,0,
		MammunUI.reference_width, MammunUI.reference_height
	);
	MapUI.map_loading_cont.set_z_index(90);
	MapUI.map_loading_cont.set_bg_color("#EFF7FF");
	MapUI.map_loading_pic = new PureFW.Image(
		MapUI.map_loading_cont,
		(MapUI.map_loading_cont.width-291) >> 1,
		(MapUI.map_loading_cont.height-89) >> 1,
		291, 89,
		"ui/elements/texts/'.$LNG.'/loading_animations/loading_change_map_'.$LNG.'.gif"
	)
}

MapUI.ingame_hide_loading = function() {
	try {
		MapUI.map_loading_cont.destroy();
		MapUI.map_loading_cont = null;
		MapUI.map_loading_pic.destroy();
		MapUI.map_loading_pic = null;
	}
	catch(e){}
}

MapUI.map_navi_friend_mode = false;
MapUI.change_map = function(world_id, map_id, show_loading, friend_mode) {
	if (typeof(show_loading) == \'undefined\')
		show_loading = true;

	MapUI.map_navi_friend_mode = friend_mode || false;

	MapUI.ingame_show_loading();

	try {
		SectorDetails.this_window.destroy();
	}
	catch(e){}

	GameMenu.close_game_menu();
	try {
		UIButtons.bottom_container.hide();
	}
	catch(e){}

	try {
		MapVisitNavi.map_browser_icon_label.hide();
		MapVisitNavi.map_browser_icon.hide();
	}
	catch(e) {}
	try {
		TutorialCtrl.hide(TutorialCtrl.first_steps);
		TutorialCtrl.hide(TutorialCtrl.build_and_expand);
	}
	catch(e){}
	UIButtons.destroy_all_windows();
	MapDataCtrl.change_map(world_id, map_id, MapUI.change_map_callback);
}
MapUI.change_map_callback = function() {
	for (var i = 0; i < 4; i++) {
		try {
			document.getElemById(\'map_tmpl_pic\'+i).src =
				"'.pic_path('').'map/templates/"+MapDataCtrl.template_id+"/_0_/"
				+ "template"+MapDataCtrl.template_id+"_"+i+".jpg";
		}
		catch(e) {}
	}
	if ((MapDataCtrl.world_id == 0) ||
		((MapDataCtrl.world_id != '.($current_user->get_world_id()).') ||
		(MapDataCtrl.map_id != '.($current_user->get_map_id()).')))
	{
		try {
			UIButtons.bottom_container.hide();
		}
		catch(e){}
		try {
			Overview.xp_bar.hide();
		}
		catch(e){}
		try {
			MapVisitNavi.map_browser_icon.hide();
			MapVisitNavi.map_browser_icon_label.hide();
		}
		catch(e) {}
		try {
			TutorialCtrl.hide(TutorialCtrl.first_steps);
			TutorialCtrl.hide(TutorialCtrl.build_and_expand);
		}
		catch(e){}

		MapVisitNavi.create_navigation(MapUI.map_navi_friend_mode);
	}
	else {
		try {
			UIButtons.bottom_container.show();
		}
		catch(e){}
		try {
			Overview.xp_bar.show();
		}
		catch(e){}
		try {
			MapVisitNavi.map_browser_icon.show();
			MapVisitNavi.map_browser_icon_label.show();
		}
		catch(e) {}
		try {
			TutorialCtrl.show(TutorialCtrl.first_steps);
			TutorialCtrl.show(TutorialCtrl.build_and_expand);
		}
		catch(e){}
		MapVisitNavi.destroy_navigation();
	}
	MapUI.ingame_hide_loading();
}
<!--
/**
 *@deprecated
 */
function load_moving_final_dialog(elms, ajax_win_id) {
	var param_str = "?"+elms[0].name+"="+elms[0].value;
	for (var i = 1; i < elms.length; i++) {
		if (elms[i].name && elms[i].name != "") {
			param_str += "&";
			param_str += elms[i].name;
			param_str += "=";
			param_str += elms[i].value;
		}
	}

	if (ajax_win_id !== "undefined")
		PureFW.WindowAJAX.instances[ajax_win_id].set_url("movement.php"+param_str);
}

ui_auto_logout = false;
MapDataCtrl.template_id = '.$template_id.';
MapDataCtrl.world_id = '.$world_id.';
MapDataCtrl.map_id = '.$map_id.';
MapDataCtrl.my_world_id = '.(User::get_current_user()->get_world_id()).';
MapDataCtrl.my_map_id = '.(User::get_current_user()->get_map_id()).';
MapDataCtrl.my_homesector = '.(User::get_current_user()->get_homesector()%100).'
MapDataCtrl.my_homesector_field_pos = new PureFW.Point(
	MapDataCtrl.my_homesector % 10,
	Math.floor(MapDataCtrl.my_homesector / 10)
);
MapDataCtrl.is_public_map = '.(empty($auth_key) ? 'true' : 'false').';
MapDataCtrl.auth_key = "'.$auth_key_ext /*in GUI IMMER Inklusive UID*/.'";
MapDataCtrl.my_map_start_time = '.$map_start_time.';
MapDataCtrl.my_color = "'.User::get_current_user()->get_color().'";

	/*if (navigator.userAgent.toLowerCase().indexOf("explorer") > -1) {
		game_connection_timeout();
		return;	// not supported
	}*/
function game_start_up(ev) {
	DocumentLoadListener.call_listeners(ev);

	if ('.((User::get_current_user()->get_state() == IUser::STATE_NEW_PLAYER)
					? 'true' : 'false').') {

		UIButtons.open_leave_map_dialog();
	}
	else if ('.(!isset($map_key_map_data) ? 'true' : 'false').') {
		if ('.((User::get_current_user()->get_state() == IUser::STATE_PLAYING)
						? 'true' : 'false').') {
			if ('.(((User::get_current_user()->get_world_id() == $world_id)
					&& ((User::get_current_user()->get_map_id()) == $map_id))
						? 'true' : 'false').') {
				GameMenu.open_game_menu();
			}
		}
		else {
			UIButtons.open_leave_map_dialog();
		}
	}
	if (MapUI.no_iframes)
	{
		PureFW.iframe_to_div(document.getElemById("mapx"),
			MammunUI.get_top_frame());
		document.getElemById("mapx").style.position = "absolute";
		document.getElemById("mapx").style.left = "-500px";
		document.getElemById("mapx").style.top = "-400px";
		document.getElemById("mapx").style.width = "2000px";
		document.getElemById("mapx").style.height = "1400px";

		try{
			document.getElemById("map_body").setAttribute("onmousedown",
				"MammunUI.get_top_frame().start_pos_map(event)");
			document.getElemById("map_body").setAttribute("onmouseup",
				"MammunUI.get_top_frame().stop_pos_map(event)");
			document.getElemById("map_body").setAttribute("onmousemove",
				"MammunUI.get_top_frame().while_drag_map(event)");
		}
		catch(e)
		{
		}
	}
	MapDataCtrl.my_nick = "'.User::get_current_user()->get_nick().'";
	MapDataCtrl.set_mammun_map(new PureFW.GameMap.MammunMap(
		document.getElemById("map_body"),
		1000-(1380>>1), 335,
		1380, 1060,
		10, 10,
		true
	));

	MapDataCtrl.refresh_mammun_map();

	// check if frames and do stuff :)
	if (document.getElementsByTagName("iframe").length == 0) {
		getElemStyle("l_scroll").display = "none";
		getElemStyle("r_scroll").display = "none";
		getElemStyle("o_scroll").display = "none";
		getElemStyle("u_scroll").display = "none";
		getElemStyle("ol_scroll1").display = "none";
		getElemStyle("ol_scroll2").display = "none";
		getElemStyle("or_scroll1").display = "none";
		getElemStyle("or_scroll2").display = "none";
		getElemStyle("ul_scroll1").display = "none";
		getElemStyle("ul_scroll2").display = "none";
		getElemStyle("ur_scroll1").display = "none";
		getElemStyle("ur_scroll2").display = "none";
	}

	document.getElemById("preload_div").style.display = "none";
	PureFW.Timeout.set_timeout(function() { ui_auto_logout = true;}, 5000);	// workaround
	ui_connection_timeout = false;

	if (MapUI.no_iframes) {'.	/** See map.tmpl.inc! */ '
		MapDataCtrl.movement_array_info_balloon = new AJAXInfoBalloon(
			document.getElemById("map_body"), 800,800,""
		);
		//MapDataCtrl.movement_array_info_balloon.add_event_handler(
		//	"click", MapDataCtrl.hide_movement_array
		//);
	}
	/* WIR ÜBER EVENT MESSAGES GELÖST
	if ('.(isset($map_key_map_data) ? 'true' : 'false').') {
		if ('.((@$invitation_failed) ? 'true' : 'false').') {
			MapUI.settle_here_conf = new PureFW.ConfirmationBox(
				document.body,
				(MammunUI.reference_width - 450) >> 1,
				100,
				450, 130,
				PureFW.ConfirmationBox.NO
			);
			MapUI.settle_here_conf.set_bg_img(
				\'ui/backgrounds/confirm/info/info_450x130.png\'
			);
		}
		else {
			if ('.(($current_user->get_state() == User::STATE_PLAYING)
				? 'true' : 'false').')
			{
				MapUI.settle_here_conf = new PureFW.ConfirmationBox(
					document.body,
					(MammunUI.reference_width - 489) >> 1,
					100,
					489, 187,
					PureFW.ConfirmationBox.YES | PureFW.ConfirmationBox.NO
				);
				MapUI.settle_here_conf.set_bg_img(
					\'ui/backgrounds/confirm/info/info_489x187.png\'
				);
				MapUI.settle_here_conf.add_event_handler(
					"confirm",
					function (ev) {
						PureFW.AJAXClientServer.send_request(
							"ingame.php?dataX='.$cryptObj->do_encryption(
								array("leave_map"=>true)).'",
								PauseMenu.leave_map_fetch_result
						);
					}
				);
			}
			else {
				MapUI.settle_here_conf = new PureFW.ConfirmationBox(
					document.body,
					(MammunUI.reference_width - 450) >> 1,
					100,
					450, 130,
					PureFW.ConfirmationBox.YES | PureFW.ConfirmationBox.NO
				);
				MapUI.settle_here_conf.set_bg_img(
					\'ui/backgrounds/confirm/info/info_450x130.png\'
				);

				MapUI.settle_here_conf.add_event_handler(
					"confirm",
					function (ev) {
						PureFW.AJAXClientServer.send_request(
							\'choose_map.php?choose_by_auth_key='
								.@$_SESSION['map_key'].'&world_id='.($world_id).'\',
							function (response_arr) {
								var _top = MammunUI.get_top_frame();
								_top.ui_auto_logout = false;
								_top.location.href = \'ingame.php?start=true\';
							}
						);
					}
				);
				MapUI.settle_here_conf.add_event_handler(
					"cancel",
					function (ev) {
						UIButtons.open_leave_map_dialog();
						MapVisitNavi.destroy_navigation();
					}
				);
			}
		}
		MapUI.settle_here_conf.set_content(
			\'<div style="position: absolute; top:10%; left: 6%; width: 88%; text-align: center">\'
				+\''.$auth_key_ext.'\'
			+\'</div>\'
		);
	}*/
	if ('.$current_user->get_world_id().' != 0) {
		var is_chrome = navigator.userAgent.toLowerCase().indexOf("chrome") > -1;
		if(is_chrome || ((MapDataCtrl.world_id != MapDataCtrl.my_world_id)
			&& (MapDataCtrl.map_id != MapDataCtrl.my_map_id)))
		{
			MapDataCtrl.change_map(
				MapDataCtrl.world_id,
				MapDataCtrl.map_id,
				MapUI.change_map_callback
			);
		}
	}

	'. (($tutorials) ? '
		var tutorials = '.json_encode($tutorials).';
		for (t in tutorials) {
			for (s in tutorials[t].tutorial_texts)
				TutorialCtrl[t].steps[s].text
					= tutorials[t].tutorial_texts[s];
			for (s in tutorials[t].tutorial_flags)
				TutorialCtrl[t].steps[s].flag_to_set
					= tutorials[t].tutorial_flags[s];

			if (typeof(tutorials[t].tutorial_start_step) != \'undefined\') {
				var tu_sta_ste = TutorialCtrl[t].order.search(
					tutorials[t].tutorial_start_step
				);
				TutorialCtrl.start(TutorialCtrl[t], tu_sta_ste);
			}
		}

	' : '').'

	MapUI.scroll_to_hs();

	/**
	 * FB-Freunde initialisieren.
	 */
	PureFW.AJAXClientServer.send_request(
		"invitation.php?get_facebook_friends=true",
		function(response_arr) {
			var friends = eval("("+response_arr[0]+")");
			for (var i in friends) {
				if (isNaN(i))
					return;
				MapVisitNavi.map_browser_icon.add_avatar(
					friends[i].avatar.replace("SIZE", 44),
					0,
					0,
					true
				)
			}
		}
	);

	/**
	 * Header-Bar initialisieren
	 */
	if (MammunUI.get_header_bar_frame()) {
		MammunUI.get_header_bar_frame().HeaderBar.show();
	}

	/**
	 * Kartenanimationen
	 */
	MapAnimations.Clouds.start();
	MapAnimations.Birds.start();
	/*MapAnimations.Fireworks.start(
		MapDataCtrl.my_homesector_field_pos.x,
		MapDataCtrl.my_homesector_field_pos.y
	);*/
}

ui_connection_timeout = true;

function game_connection_timeout() {
	if (!ui_connection_timeout)
		return;
/*	game_start_up = null;
	var connection_timeout_msg_box = MammunUI.show_warning(
		404, 325,
		\'Für das beste Spielgefühl unterstützt Mammun bereits folgende Browser:<ul>\'
		+\'<li>Chrome</li>\'
		+\'<li>Firefox</li>\'
		+\'<li>Opera</li>\'
		+\'<li>Safari</li>\'
		+\'</ul>\'
		+\'<br/>\'
		+\'<br/>\'
		+\'<br/>\'
		+\'Weitere Browser wie zum Beispiel der Internet Explorer werden ebenfalls bald unterstützt.\',
		PureFW.ConfirmationBox.NO_BUTTONS,
		true
	);
	connection_timeout_msg_box.set_z_index(1000);*/
	game_start_up();
	game_start_up = function(){};
}
PureFW.Timeout.set_timeout(
	game_connection_timeout, 30000
);

function game_end() {
	if (ui_auto_logout && MammunUI.user_in_facebook) {
		PureFW.AJAXClientServer.send_request("logout.php");
	}
}

var timeout_orient_change = function() {
	var viewport_meta = document.getElemById("viewport");
	switch(window.orientation) {
		case 0:
		case 180:
			document.getElemById("iphone_flipme").style.display = "";

			if (viewport_meta) {
				viewport_meta.setAttribute("content",
					"minimum-scale=1,maximum-scale=1,initial-scale=1"
				);
			}
			MammunUI.get_top_frame().window.scrollTo(0,0);
			window.onscroll=function(ev){
				ev.preventDefault();
				MammunUI.get_top_frame().window.scrollTo(0,0);
			};
			break;
		default:
			document.getElemById("iphone_flipme").style.display = "none";

			if (viewport_meta) {
				viewport_meta.setAttribute("content",
					"minimum-scale=0.24,maximum-scale=10,initial-scale=0.24"
				);
			}
			window.onscroll=function(){};
	}
};

PureFW.on_orientation_change = function() {
	PureFW.Timeout.set_timeout(timeout_orient_change, 100);
};
PureFW.WidgetManager.page_aspect_ratio =
	MammunUI.reference_width/MammunUI.reference_height;
//-->

if (window.addEventListener)
	window.addEventListener("load", game_start_up, false);
else if (document.addEventListener)
	document.addEventListener("load",game_start_up,false);
else if (window.attachEvent)
	window.attachEvent("onload",game_start_up);
else {//Older browsers only
	if (typeof window.onload=="function") {
		var oldload=window.onload;
		window.onload=function(){
			oldload();
			game_start_up();
		};
	}
	else
		window.onload=game_start_up;
}
</script>
';

$body_add = 'onload=""
	onunload="DocumentUnloadListener.call_listeners(event); game_end();"
	onresize="PureFW.handle_resize(event);"
	onkeydown="MapUI.handle_key_down(event);"
	onkeyup="MapUI.handle_key_up(event);"';
?>
<? include('header.tmpl.inc') ?>

<script type="text/javascript">
var map_ui__fixed_window = null;
function open_dialog(url, title, w, h, full_fade, x, y) {
	var window;
	if (full_fade) {
		window = new FullWindowAJAX(document.body, w, h, url);
		if (MapVisitNavi.navigation && !MapVisitNavi.navigation.hidden) {
			MapVisitNavi.hide_navigation();
			window.add_event_handler("destroy",
				function(ev) {
					MapVisitNavi.show_navigation();
				}
			);
		}
	}
	else {
		window = new PureFW.WindowAJAX(document.body, x, y, w, h, url, '');
	}
	window.show();
}

function open_fixed_window(url, title, w, h, x, y) {
	if (map_ui__fixed_window) {
		map_ui__fixed_window.set_url(url);
		map_ui__fixed_window.set_title('');
		/*TODO map_ui__fixed_window.set_width(w);
		map_ui__fixed_window.set_width(h);*/
	}
	else {
		map_ui__fixed_window = new PureFW.WindowAJAX(document.body, x, y,
				w, h, url, title);

		map_ui__fixed_window.add_event_handler("destroy",
				function() {
					try{ delete map_ui__fixed_window; }catch(e){}
					map_ui__fixed_window = null;
					MapVisitNavi.map_browser_icon_label.show();
				}
		);


	}
	map_ui__fixed_window.set_bg_img(
		'ui/backgrounds/window/'+
			MapDataCtrl.my_color + '/window-'
			+w+'x'+h+'.png'
	);
	map_ui__fixed_window.show();
}
function get_cur_fixed_window_url() {
	return map_ui__fixed_window ? map_ui__fixed_window.get_url() : '';
}
function close_fixed_window() {
	if (map_ui__fixed_window) {
		map_ui__fixed_window.destroy();
		map_ui__fixed_window = null;
	}
}
function resume_fixed_window() {
	if (map_ui__fixed_window)
		map_ui__fixed_window.show();
}
function minimize_fixed_window() {
	if (map_ui__fixed_window)
		map_ui__fixed_window.hide();
}
function fixed_window_minimized() {
	if (map_ui__fixed_window)
		return map_ui__fixed_window.is_hidden();
	else
		return false;
}

function pause_game() {
	ui_refresh_active = false;
}

function resume_game() {
	ui_refresh_active = true;
}

function ui_hide_map_scroll_arrows() {
	getElemStyle("l_scroll").display = "none";
	getElemStyle("r_scroll").display = "none";
	getElemStyle("o_scroll").display = "none";
	getElemStyle("u_scroll").display = "none";
	getElemStyle("ol_scroll1").display = "none";
	getElemStyle("ol_scroll2").display = "none";
	getElemStyle("or_scroll1").display = "none";
	getElemStyle("or_scroll2").display = "none";
	getElemStyle("ul_scroll1").display = "none";
	getElemStyle("ul_scroll2").display = "none";
	getElemStyle("ur_scroll1").display = "none";
	getElemStyle("ur_scroll2").display = "none";
}
function ui_show_map_scroll_arrows() {
	getElemStyle("l_scroll").display = "block";
	getElemStyle("r_scroll").display = "block";
	getElemStyle("o_scroll").display = "block";
	getElemStyle("u_scroll").display = "block";
	getElemStyle("ol_scroll1").display = "block";
	getElemStyle("ol_scroll2").display = "block";
	getElemStyle("or_scroll1").display = "block";
	getElemStyle("or_scroll2").display = "block";
	getElemStyle("ul_scroll1").display = "block";
	getElemStyle("ul_scroll2").display = "block";
	getElemStyle("ur_scroll1").display = "block";
	getElemStyle("ur_scroll2").display = "block";
}

ui_wood_gainer = new Gainer();	// wird in overview gesetzt
ui_food_gainer = new Gainer();	// wird in overview gesetzt
ui_klunker_gainer = new Gainer();	// wird in overview gesetzt
ui_items_gainer = new Gainer();		// wird in overview gesetzt
ui_score_gainers = new Array();	// wird in der Diplo gesetzt (Array of Gainer)
ui_my_score_gainer = null;	// wird in der Diplo gesetzt (Verwaltet eigene Score)

ui_refresh_active = true;
function ui_refresh() {
	if (!ui_refresh_active)
		return;

	if (Overview && Overview.wood_label) {
		Overview.wood_label.set_text(
			number_format( ui_wood_gainer.get_cur_value(), 0, ',', '.' )
		);
	}
	if (Overview && Overview.food_label) {
		Overview.food_label.set_text(
			number_format( ui_food_gainer.get_cur_value(), 0, ',', '.' )
		);
	}
	if (Overview && Overview.klunker_label) {
		Overview.klunker_label.set_text(
			number_format( ui_klunker_gainer.get_cur_value(), 0, ',', '.' )
		);
	}
	if (Overview && Overview.items_label) {
		Overview.items_label.set_text(
			number_format( ui_items_gainer.get_cur_value(), 0, ',', '.' )
		);
	}
}

var MapDrag = new Object();
MapDrag.start = new PureFW.Point(-1,-1);
MapDrag.dragging = false;

function stop_pos_map() {
	MapDrag.start.x = MapDrag.start.y = -1;
}
function while_drag_map(ev) {
	try {
		if ((MapDrag.start.x >= 0) && (MapDrag.start.y >= 0)) {
			MapDrag.dragging = true;
			PureFW.MouseFeatures.on_mouse_move(ev);
			var p = PureFW.MouseFeatures.get_cursor_pos();
			var x_diff = p.x - MapDrag.start.x;
			var y_diff = p.y - MapDrag.start.y;

			MapDrag.start.x = p.x;
			MapDrag.start.y = p.y;
			scroll_map(
				-x_diff,
				-y_diff
			);
		}
	}
	catch(e) {}
}
function scroll_map(x,y) {
	if (MapUI.no_iframes) {
		MapUI.scroll_map_div_by(x,y);
	}
	else {
		MammunUI.get_top_frame().mapx.scrollBy(x,y);
	}
}
function start_pos_map(ev) {
	try {
		ev = PureFW.EventUtil.formatEvent(ev);
	}
	catch (e) {}
	ev.preventDefault();
	PureFW.MouseFeatures.on_mouse_move(ev);
	var p = PureFW.MouseFeatures.get_cursor_pos();
	MapDrag.start.x = p.x;
	MapDrag.start.y = p.y;
	MapDrag.dragging = false;
}
</script>

<div id="preload_div"
	style="width: 100%; height: 100%; position: absolute; left: 0px; top: 0px;
		z-index: 999; background-color: #f2f7ff; padding-top:30%; font-size: 3em;
		text-align: center">

	<img src="../pix/ui/elements/texts/<?=$LNG?>/loading_animations/connecting_<?=$LNG?>.gif"/>
</div>

<!-- Kartenframe -->
<script type="text/javascript">
map_ui_map_container = new PureFW.Container(
	document.body,
	0,0,
	MammunUI.reference_width, MammunUI.reference_height
);

map_ui_map_container.set_content(
	'<iframe name="mapx" id="mapx" src="map.php?world_id=<?=$world_id
			?>&map_id=<?=$map_id?>&<?=session_name()?>=<?=session_id()
			?>" frameborder="0"\
 		scrolling="no" width="100%" height="100%"\
 		style="width: 100%; height: 100%"></iframe>');

</script>

<!-- Navi -->
<? require('data/ingame_navi.inc') ?>
<? include('map_animations/clouds.tmpl.inc') ?>
<? include('map_animations/birds.tmpl.inc') ?>
<? include('map_animations/fireworks.tmpl.inc') ?>

<!-- MammunSpeech Init -->
<? require('templates/mammun_speech_init.tmpl.inc') ?>
<!-- NameSuggestsions Init -->
<? require('templates/name_suggestions_init.tmpl.inc') ?>

<script type="text/javascript">
<!--
// Selection bei allen DIVs ausschalten
/*var alldivs=document.getElementsByTagName("div")
for (var i=0; i<alldivs.length; i++)
	disableSelection(alldivs[i])*/


PureFW.Timeout.set_interval(ui_refresh, 10000);
PureFW.Timeout.set_interval(MapDataCtrl.update_map, 60000);


//-->
</script>

<?/* Unsichtbares iFrame für bestimmte Aktionen, die mit AJAX nicht gehn.
     Solche Aktionen sind vor allem Dateiupload, da man per AJAX keinen Zugriff
     auf den Inhalt einer lokalen Datei hat. */ ?>
<iframe name="file_upload" id="file_upload" frameborder="0" scrolling="no"
	src="file_upload.php"
	style="position: absolute; left: 0; top: 0; width: 0; height: 0; display: none">
</iframe>
<?php include_once 'templates/footer.tmpl.inc';?>