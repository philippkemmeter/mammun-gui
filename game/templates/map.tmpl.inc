<?
$map_img_width = 2000;
$map_img_height = 2000;
$map_x_offset = 1000;
$map_y_offset = 630;
$sector_width = 138;
$sector_height = 106;
$header_add = '
<script type="text/javascript" src="../js/classes/MammunUI.js?'.filemtime('../js/classes/MammunUI.js').'"></script>
<script type="text/javascript" src="../js/classes/ParticleSys.js?'.filemtime('../js/classes/ParticleSys.js').'"></script>
<script type="text/javascript" src="../js/PureFW/pure_fw_all.js?'.filemtime('../js/PureFW/pure_fw_all.js').'"></script>
<script type="text/javascript" src="../js/MathExt/math_ext_all.js?'.filemtime('../js/MathExt/math_ext_all.js').'"></script>
<script type="text/javascript" src="../js/PureFW/jstween/jstween.js?'.filemtime('../js/PureFW/jstween/jstween.js').'"></script>
<script type="text/javascript" src="../js/extern/formatdate.js?'.filemtime('../js/extern/formatdate.js').'"></script>
<script type="text/javascript">PureFW.Time.set_init_server_time('.Time::get_t_now().');</script>
<script type="text/javascript" src="../js/gen_funcs.js?'.filemtime('../js/gen_funcs.js').'"></script>
<script type="text/javascript" src="../js/classes/Widgets/TextBullet.js?'.filemtime('../js/classes/Widgets/TextBullet.js').'"></script>
<script type="text/javascript" src="../js/classes/Widgets/UnitSlot.js?'.filemtime('../js/classes/Widgets/UnitSlot.js').'"></script>
<script type="text/javascript" src="../js/classes/Widgets/map_objects/Animal.js?'.filemtime('../js/classes/Widgets/map_objects/Animal.js').'"></script>
<script type="text/javascript" src="../js/classes/Widgets/map_objects/Mammun.js?'.filemtime('../js/classes/Widgets/map_objects/Mammun.js').'"></script>
<script type="text/javascript" src="../js/classes/PopupMenu.js?'.filemtime('../js/classes/PopupMenu.js').'"></script>
<style type="text/css">@import "../css/PopupMenu.css";</style>
<script type="text/javascript" src="../js/PureFW/MouseFeatures.js?'.filemtime('../js/PureFW/MouseFeatures.js').'"></script>
<script type="text/javascript" src="../js/classes/Widgets/AJAXInfoBalloon.js?'.filemtime('../js/classes/Widgets/AJAXInfoBalloon.js').'"></script>
<style type="text/css">@import "../css/AJAXInfoBalloon.css";</style>
<style type="text/css">@import "../css/MammunMap.css?'.filemtime(dirname(__FILE__).'/../../css/MammunMap.css').'";</style>
<script type="text/javascript">
<!--
/***** KEY-CONTROL *******/

var scroll_intervals = new Array();

var template_id = '.$template_id.';

/** Dragging der Karte */
PureFW.EventUtil.add_event_handler(document, "mousemove",
	MammunUI.get_top_frame().while_drag_map);
PureFW.EventUtil.add_event_handler(document, "mouseup",
	MammunUI.get_top_frame().stop_pos_map);

//-->
</script>
';/*<style type="text/css">
body {
	width:2000px;
	height: 2000px;
}
</style>';*/

$body_add =
	'onkeydown="MammunUI.get_top_frame().MapUI.handle_key_down(event);" '.
	'onkeyup="MammunUI.get_top_frame().MapUI.handle_key_up();" '.
	'onclick="MammunUI.get_top_frame().focus();" ';
?>
<? include('header.tmpl.inc') ?>
<script type="text/javascript">
function startscroll(x, y) {
	scroll_intervals.push(window.setInterval("window.scrollBy("+x+","+y+")", 20));
}
function stopscroll() {
	var n = scroll_intervals.length;
	for(var i = 0; i < n; i++) {
		window.clearInterval(scroll_intervals[i]);
		scroll_intervals.splice(i, 1);
	}
}

</script>
<!-- Kartenbild -->
<? $map_img_height = 1400; $map_y_offset -= 300; ?>
<div id="map_body" style="position:absolute; left: 0px; top: 0px; width: 2000px; height: 1400px"
onmousedown="MammunUI.get_top_frame().start_pos_map(event)">
<img src="<?= pic_path("map/templates/$template_id/_0_/template{$template_id}_0.jpg") ?>"
	id="map_tmpl_pic0"
	style="position: absolute; left: 0px; top: 0px;"
	width="<?= $map_img_width >> 1 ?>" height="<?= $map_img_height >> 1 ?>"
	onclick="javascript: PopupMenu__hide_all(); MammunUI.get_top_frame().PopupMenu__hide_all();" alt="map" />
<img src="<?= pic_path("map/templates/$template_id/_0_/template{$template_id}_1.jpg") ?>"
	id="map_tmpl_pic1"
	style="position: absolute; left: 1000px; top: 0px;"
	width="<?= $map_img_width >> 1 ?>" height="<?= $map_img_height >> 1 ?>"
	onclick="javascript: PopupMenu__hide_all(); MammunUI.get_top_frame().PopupMenu__hide_all();" alt="map" />
<img src="<?= pic_path("map/templates/$template_id/_0_/template{$template_id}_2.jpg") ?>"
	id="map_tmpl_pic2"
	style="position: absolute; left: 0px; top: 700px;"
	width="<?= $map_img_width >> 1 ?>" height="<?= $map_img_height >> 1 ?>"
	onclick="javascript: PopupMenu__hide_all(); MammunUI.get_top_frame().PopupMenu__hide_all();" alt="map" />
<img src="<?= pic_path("map/templates/$template_id/_0_/template{$template_id}_3.jpg") ?>"
	id="map_tmpl_pic3"
	style="position: absolute; left: 1000px; top: 700px;"
	width="<?= $map_img_width >> 1 ?>" height="<?= $map_img_height >> 1 ?>"
	onclick="javascript: PopupMenu__hide_all(); MammunUI.get_top_frame().PopupMenu__hide_all();" alt="map" />
</div>
<!-- Karte -->
<script type="text/javascript">
_top = MammunUI.get_top_frame();
_top.MammunUI.user_in_facebook = <?	if(isset($_SESSION['fb']['fb_sig'])
										|| isset($_SESSION['fb']['fb_sig_post'])
										|| isset($_SESSION['fb']['session']))
									echo "true";
								else
									echo "false"; ?>;

_top.MapDataCtrl.dominators = <?= json_encode($map_data->dominator) ?>;
_top.MapDataCtrl.buildings = <?= json_encode(Buildings::convert_server_buildings($map_data->building)) ?>;
_top.MapDataCtrl.player_colors = <?= json_encode($user_colors) ?>;
_top.MapDataCtrl.movements = <?= json_encode($map_data->moving_units) ?>;
_top.MapDataCtrl.units = <?= json_encode($map_data->units) ?>;
_top.MapDataCtrl.fights = <?= json_encode($map_data->fight) ?>;
_top.MapDataCtrl.resources = <?= json_encode($map_data->bio) ?>;
_top.MapDataCtrl.is_mentor_map = <?= ($map_data->mentor_map) ? 'true':'false'?>;
_top.MapDataCtrl.map_is_closed = <?= ($map_data->closed) ? 'true':'false'?>;
_top.MapDataCtrl.max_players = <?= $max_players ?>;
_top.MapDataCtrl.players_on_map_amount = 0;
for (var xyz_key in _top.MapDataCtrl.player_colors)
	_top.MapDataCtrl.players_on_map_amount++;

<? if (($current_user->get_world_id() == $world_id)
	&& ($current_user->get_map_id() == $map_id)) : ?>
_top.MapDataCtrl.my_is_mentor_map = <?= ($map_data->mentor_map) ? 'true':'false'?>;
_top.MapDataCtrl.my_map_is_closed = <?= ($map_data->closed) ? 'true':'false'?>;
_top.MapDataCtrl.my_max_players = <?= $max_players ?>;
_top.MapDataCtrl.my_players_on_map_amount = 0;
for (var xyz_key in _top.MapDataCtrl.player_colors)
	_top.MapDataCtrl.my_players_on_map_amount++;

if ((_top.MapDataCtrl.my_is_mentor_map && !_top.MapDataCtrl.my_map_is_closed
	&& (_top.MapDataCtrl.my_players_on_map_amount
				< _top.MapDataCtrl.my_max_players))
	/*&& _top.MammunUI.user_in_facebook*/)
{
	<? /*var use_share_message = new Object();
	use_share_message.str = '<?= LNG_EVM_USE_FB_SHARE ?>';
	use_share_message.type = <?= IEventMessages::TYPE_USE_FACEBOOK_SHARE ?>;
	use_share_message.pic = new Object();
	use_share_message.pic.pic_url = 'ui/icons/labels/facebook_share/symbol_109x107.png';
	use_share_message.pic.width = 109;
	use_share_message.pic.height = 107;
	use_share_message.pic.pos = 0;
	use_share_message.template = 'fb_share';
	_top.EventMessage.Random.insert_message(use_share_message);*/?>
	var boring_alone_msg = new Object();
	boring_alone_msg.type = <?= IEventMessages::TYPE_OPEN_COL_CHOOSE_DIALOG_INVITE ?>;
	_top.EventMessage.Random.insert_message(boring_alone_msg);
}
<? endif; ?>


var message_arr = <?= json_encode($event_log) ?>;
try {
	_top.EventMessage.handle_event_messages(message_arr);
}
catch(e){}

//Selection bei allen DIVs ausschalten
/*var alldivs=document.getElementsByTagName("div")
for (var i=0; i<alldivs.length; i++)
	disableSelection(alldivs[i])*/
<? /**
	* Wenn iFrames nicht erlaubt sind, weil der Browser des Nutzers sie nicht
	* (vollständig) unterstützt, wird das gesamte Template nach dem Laden in ein
	* DIV gepresst. Dadurch wird ist der DOM-Knoten des
	* movement_array_info_balloon-Widgets nicht mehr aufzufinden, weswegen das
	* hier nicht deklariert werden darf. In dem Fall wird das Ding erst nach
	* dem Laden der Seite (s. map_ui.tmpl.inc#game_start_up()) erstellt.
	* Das funktioniert aber nicht, wenn nun das iFrame richtig funktioniert
	* - der Bug hierbei war, dass das Info-Balloon-Dings keine Mouse-Events
	* mehr annahm, da die beim Konvertieren kaputt gehn - muss bei iFrame-
	* funktionstüchtigen Browsern das hier inline deklariert werden.
	*
	* Somit: Wenn iFrames gehn, dann inline, sonst verzögert.
	*/ ?>
_top  = MammunUI.get_top_frame();
if (!_top.MapUI.no_iframes) {
	_top.MapDataCtrl.movement_array_info_balloon = new AJAXInfoBalloon(
		document.getElemById("map_body"), 800,800,""
	);
//	_top.MapDataCtrl.movement_array_info_balloon.add_event_handler(
//		"click", _top.MapDataCtrl.hide_movement_array
//	);

}
var map_just_chosen = <?	if(isset($_SESSION['map_just_chosen']))
								echo "true";
							else
								echo "false"; ?>;

var user_playing = <? 		if (User::get_current_user()->get_state() ==
														User::STATE_PLAYING)
								echo "true";
							else
								echo "false"; ?>;

<?	/**
	 * Normally, MammunUI.facebook_publish_prompt(...) should be used here,
	 * but as of this point in the game, it hasn't yet been loaded.
	 * Therefore, the FB publish is done manually.
	 */
/* Scheint nicht zu funktionieren: TODO: CHECK AND REPAIR
?>
if(_top.MammunUI.user_in_facebook && map_just_chosen && user_playing)
{
	var attachment = {
      "name" : '<?=LNG_NEW_MAP_STARTED?>',
      "caption" : '<?=LNG_NEW_MAP_STARTED_DESC?>'
	}
	_top.FB.Connect.streamPublish("", attachment);
	PureFW.AJAXClientServer.send_request("map.php?clear_session_first_map="
			+"true");
}
<?*/?>
delete user_playing;
delete map_just_chosen;
</script>
<? include('footer.tmpl.inc') ?>