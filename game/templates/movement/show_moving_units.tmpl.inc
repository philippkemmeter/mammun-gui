<?
/**
 * ACHTUNG: Wegen automatischem Tagschließen von Browsern nach innerHTML darf
 * kein JS innerhalb von Tags stehen! Am besten JS am Anfang und am Ende der Datei
 * reinschreiben. Wenn innerhalb von Tabellen entstehen meist Tabellenfehler!
 */
$this_id = rand(0,1000000);	// eindeutige ID
/**
 * ACHTUNG: Allen Variablen das Suffix $this_id anhängen, da es sonst passieren
 * kann, dass Werte von Variablen in der Funktion, die eval aufruft, geändert
 * werden. Beispiel:
 * function x() {
 *    var l = 1;
 *    eval("var l = 2");  // danach ist l überschrieben
 *    eval("var l<?=$this_id?> = 2"); // danach ist l natürlich NICHT überschrieben
 * }
 */
?>
<script type="text/javascript">
ShowMovingUnits = new Object();
ShowMovingUnits.this_window = PureFW.ContainerAJAX.instances[<?= $this_win_id?>];

ShowMovingUnits.interval = 0;	<? /* Später */ ?>

ShowMovingUnits.all_units = new Array();
ShowMovingUnits.icons = new Array();
ShowMovingUnits.name_conts = new Array();
ShowMovingUnits.countdown_conts = new Array();
ShowMovingUnits.stop_conts = new Array();
ShowMovingUnits.unit_count = 0;
ShowMovingUnits.dummy = null;
<?	/**
	 * Initialisiert alles, was zum Anzeigen der Seite nötig ist.
	 */ ?>
ShowMovingUnits.init = function()
{
	var _top = MammunUI.get_top_frame();

	for (var u in _top.MapDataCtrl.movements[<?=$start_sector_id?>]
															[<?= $direction ?>])
	{
		if (isNaN(u))
			continue;

		var unit = _top.MapDataCtrl.movements[<?= $start_sector_id ?>]
														[<?= $direction ?>][u];

		var l = ShowMovingUnits.all_units.length;
		ShowMovingUnits.all_units[l] = new Object();
		ShowMovingUnits.all_units[l].name = unit.name;
		ShowMovingUnits.all_units[l].id = unit.id;
		ShowMovingUnits.all_units[l].race = unit.race;
		ShowMovingUnits.all_units[l].amount = unit.amount;
		ShowMovingUnits.all_units[l].clothes = unit.clothes;
		ShowMovingUnits.all_units[l].color =
			_top.MapDataCtrl.player_colors[unit.user_nick];
		ShowMovingUnits.all_units[l].count = parseInt(unit.amount);
		ShowMovingUnits.all_units[l].start_time =
			parseInt(unit.start_time);
		ShowMovingUnits.all_units[l].arrival_time =
			parseInt(unit.arrival_time);
		ShowMovingUnits.all_units[l].path = unit.path;
		ShowMovingUnits.all_units[l].user_nick = unit.user_nick;

		ShowMovingUnits.all_units[l].pic_url = unit.pic_url
												.replace(/\.png/,"_0.png");


		ShowMovingUnits.all_units[l].time_per_sector = unit.time_per_sector;
		ShowMovingUnits.unit_count++;
	}
}

<?	/**
	 * Zeigt alle Einheiten auf der Seite an
	 */ ?>
ShowMovingUnits.create_moving_ones = function()
{
	if (ShowMovingUnits.dummy)
		ShowMovingUnits.dummy.destroy();

	var n = ShowMovingUnits.name_conts.length;
	for (var i = 0; i < n; i++)
	{
		ShowMovingUnits.name_conts[i].destroy();
		ShowMovingUnits.countdown_conts[i].destroy();
		ShowMovingUnits.icons[i].destroy();
		ShowMovingUnits.stop_conts.destroy();
	}

	var n = ShowMovingUnits.all_units.length;
	ShowMovingUnits.dummy = new PureFW.Container(
		ShowMovingUnits.this_window,
		0, 0,
		130, n*80,
		true
	);
	ShowMovingUnits.dummy.set_positioning('relative');
	for (var i = 0; i < n; i++)
	{
		var unit = ShowMovingUnits.all_units[i];

		ShowMovingUnits.name_conts[i] = new PureFW.Container(
			ShowMovingUnits.this_window,
			0, i*80,
			130, 20,
			true
		);
		ShowMovingUnits.name_conts[i].set_content('<b>'+unit.name+'</b>');

		ShowMovingUnits.icons[i] = new PureFW.Container(
			ShowMovingUnits.this_window,
			0, i*80+20,
			54, 60,
			true
		);
		/*ShowMovingUnits.icons[i].set_bg_img(
			'ui/icons/backgrounds/user_colors/'+unit.color+'.png'
		);*/

		var mammun = MapObjects.Mammun.get_mammun_by_type_and_sex( unit.race );
		var m = ShowMovingUnits.icons[i].add_widget(
			MapObjects.Mammun,
			((54 - mammun.width) >> 1), ((60 - mammun.height) >> 1),
			mammun.width, mammun.height,
			mammun,
			unit.clothes,
			MammunUI.get_top_frame().MapDataCtrl.player_colors[unit.user_nick],
			true
		);
		m._set_pose(<?= $direction ?>);
		m._trigger_animation_now('walk');
		m._set_status(MapObjects.Mammun.STATUS_WALK);
		m.set_hp(unit.amount);
		<?/*
		var pic = ShowMovingUnits.all_units[i].pic_url;
		var pic_core = pic.substring(0, pic.indexOf('/'));
		var t = pic.replace(eval('/'+pic_core+'_/'), pic_core+'_small_');
		t = t.replace(/[0-9]\.png/g,"0.png");
		pic = '<?= pic_path('ui/units/training/') ?>'+t;
		ShowMovingUnits.icons[i].set_content(
			'<img style="margin-left: 20%" '
				+ 'src="'+pic+'"'
				+ ' height="100%" width="57%" />'
		);*/?>
		ShowMovingUnits.icons[i].set_z_index(10);
		ShowMovingUnits.icons[i].add_event_handler(
			"mouseover",
			(function(nr){
				return function(ev){
					ShowMovingUnits.unit_icon_over(nr)
				};
			})(i)
		);
		ShowMovingUnits.icons[i].add_event_handler(
			"mouseout",
			(function(nr){
				return function(ev){
					ShowMovingUnits.unit_icon_out(nr)
				}
			})(i)
		);
		ShowMovingUnits.icons[i].add_event_handler(
			"click",
			(function(nr){
				return function(ev){
					ShowMovingUnits.show_unit_details(
							ShowMovingUnits.all_units[nr])
				}
			})(i)
			);

		ShowMovingUnits.countdown_conts[i] = new PureFW.Container(
			ShowMovingUnits.this_window,
			70, i*80+20,
			50, 0
		);
		ShowMovingUnits.countdown_conts[i].set_content(
			PureFW.Time.sec_in_time(unit.arrival_time
				- PureFW.Time.get_current_server_time())
		);

		ShowMovingUnits.stop_conts[i] = new PureFW.Image(
			ShowMovingUnits.this_window,
			70, i*80+40,
			32, 32,
			'ui/icons/labels/unit_orders/stop.png', true
		);
		if (unit.user_nick != '<?= User::get_current_user()->get_nick()?>') {
			ShowMovingUnits.stop_conts[i].hide();
		}
		else {
			ShowMovingUnits.stop_conts[i].set_tooltip(
				'<?= LNG_B_STOP ?>'
			);
			ShowMovingUnits.stop_conts[i].add_event_handler("click",
				(function(_i) {
					return function(ev) {
						ShowMovingUnits.stop_unit(_i);
					}
				})(i)
			);
		}
	}
}

<? /** Stoppt die Einheit */ ?>
ShowMovingUnits.stop_unit = function(i) {
	<? /**
	    * Während das Tutorial läuft, ist STOP nicht aktiv, was dem Spieler
	    * per Popup erklärt wird.
	    */ ?>
    var _top = MammunUI.get_top_frame();
    if (_top.TutorialCtrl.running) {
    	_top.MammunUI.show_warning(
        	360, 255, '<?= LNG_NO_STOP_WHILE_TUTORIAL ?>',
        	_top.PureFW.ConfirmationBox.YES
        );
        return;
    }
	var unit = ShowMovingUnits.all_units[i];
	PureFW.AJAXClientServer.send_request('movement.php?stop=true&id0='+unit.id,
		(function(_i) {
			return function(response_arr) {
				if (response_arr[0] == '1') {
					var sector = parseInt(response_arr[1]);
					var _top = MammunUI.get_top_frame();
					if (response_arr[2])
						_top.MapDataCtrl.movements[sector]
							= eval('('+response_arr[2]+')');
					else
						_top.MapDataCtrl.movements[sector] = null;
					_top.MapDataCtrl.dominators[sector] = response_arr[3];
					_top.MapDataCtrl.fights[sector] = response_arr[4];
					_top.MapDataCtrl.units[sector]
						= eval('('+response_arr[5]+')');

					_top.MapDataCtrl.refresh_mammun_map();
					ShowMovingUnits.all_units.splice(_i,1);
					if (ShowMovingUnits.all_units.length > 0)
						ShowMovingUnits.create_moving_ones();
					else
						_top.MapDataCtrl.movement_array_info_balloon.hide();
				}
				else {
					var tmp = _top.MammunUI.show_warning(
						350, 200, response_arr[1],
						_top.PureFW.ConfirmationBox.YES
					);
				}
			}
		})(i)
	);
}

<? /* Show a detailed picture of the clicked unit. */ ?>

ShowMovingUnits.show_unit_details = function(info)
{
	MammunUI.get_top_frame().MammunUI.show_unit_details(info);
}

<?	/**
	 * Aktuallisiert die Seite. Diese Funktion wird jede Sekunde aufgerufen.
	 * rt steht für Real Time ^^
	 */ ?>
ShowMovingUnits.rtMoving = function() {
	var j = new Date();
	var t_now = PureFW.Time.get_current_server_time();//Math.floor(j.getTime()/1000);
	for (var i = 0; i < ShowMovingUnits.all_units.length; i++)
	{
		var t_left = ShowMovingUnits.all_units[i].arrival_time - t_now;
		if (t_left > 0) {
			ShowMovingUnits.countdown_conts[i].set_content(
				PureFW.Time.sec_in_time(t_left)
			);
		}
		else {
			MammunUI.get_top_frame().MapDataCtrl.update_map();
								<? /* komplett alles updaten, da man oft mehrere
			                   	    * Einheiten gleichzeitig verschickt und sonst
			                   	    * merken würde, dass diese Einheit, die man
			                   	    * beobachtet schon da ist, die andere(n) aber
			                   	    * noch unterwegs sind.
			                   	    */ ?>
			ShowMovingUnits.icons[i].destroy();
			ShowMovingUnits.countdown_conts[i].destroy();
			ShowMovingUnits.name_conts[i].destroy();

			ShowMovingUnits.unit_count--;
			if (ShowMovingUnits.unit_count == 0) {
				ShowMovingUnits.this_window.destroy();
			}
		}
	}
}

<? 	/**
	 * Diese Funktion wird aufgerufen, wenn jemand über ein Icon hovert.
	 *
	 * @param uint index
	 */ ?>
ShowMovingUnits.unit_icon_over = function(index) {
	var unit = ShowMovingUnits.all_units[index];
	if (unit.path) {
		MammunUI.get_top_frame().MapDataCtrl.mammun_map.show_path(
			unit.path, unit.time_per_sector,
			PureFW.Time.get_current_server_time() - unit.start_time
		);
	}
}
<? 	/**
	 * Diese Funktion wird aufgerufen bei Mouseout (Icon)
	 *
	 * @param uint index
	 */ ?>
ShowMovingUnits.unit_icon_out = function(index) {
	MammunUI.get_top_frame().MapDataCtrl.mammun_map.show_path(new Array());
}

<?	/**
	 * Destructor der Seite
	 */ ?>
ShowMovingUnits.destroy = function()
{
	if (ShowMovingUnits.unit_details)
	{
		ShowMovingUnits.unit_details.destroy();
	}

	MammunUI.get_top_frame().PureFW.Timeout.clear_interval(
		ShowMovingUnits.interval
	);

	ShowMovingUnits.this_window.remove_event_handler("destroy",
		ShowMovingUnits.destroy);
	ShowMovingUnits.this_window.remove_event_handler("change",
		ShowMovingUnits.destroy);

	destroy_object(ShowMovingUnits);
	ShowMovingUnits = null;
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

ShowMovingUnits.interval = MammunUI.get_top_frame().PureFW.Timeout.set_interval(
	ShowMovingUnits.rtMoving, 1000
);

ShowMovingUnits.init();
ShowMovingUnits.create_moving_ones();
ShowMovingUnits.this_window.add_event_handler("destroy",ShowMovingUnits.destroy);
ShowMovingUnits.this_window.add_event_handler("change", ShowMovingUnits.destroy);
</script>