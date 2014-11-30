<script type="text/javascript">
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
 *    eval("var l<?=$this_id?> = 2"); // danach ist l NICHT überschrieben (was besser ist :))
 * }
 */
?>

Buildings = new Object();
GlobalConstructionSite = null;

Buildings.painting_costs = <?= IBuildings::KKR_PAINT ?>;

Buildings.change_mode = <?= $change_mode ? 'true' : 'false' ?>;

Buildings.buildable_buildings = new Array();
<? $i=0;foreach ($buildable_buildings as $building) : ?>
	Buildings.buildable_buildings[<?=$i?>] = new Object();
	Buildings.buildable_buildings[<?=$i?>].size =
		new Array(<?= $building->get_size() ?>, <?= $building->get_height() ?>);
	Buildings.buildable_buildings[<?=$i?>].type = <?= $building->get_type() ?>;
	Buildings.buildable_buildings[<?=$i?>].pic =
		'<?=pic_path('map/buildings/'.$building->get_pic())?>';
	Buildings.buildable_buildings[<?=$i?>].pic_big =
		'<?=pic_path('ui/elements/building_preview_boxes/'.$building->get_pic())?>';
	Buildings.buildable_buildings[<?=$i?>].icon_path =
		'ui/icons/labels/buildings/65/<?=$building->get_pic()?>';
	Buildings.buildable_buildings[<?=$i?>].name = '<?= $building->get_name() ?>';
	Buildings.buildable_buildings[<?=$i?>].custom = '<?= $building->is_custom_building() ?>';
	Buildings.buildable_buildings[<?=$i?>].cost = <?= $building->get_cost() ?>;
	Buildings.buildable_buildings[<?=$i?>].grow =
		<?= $building->get_grow() ?> * 100;
	Buildings.buildable_buildings[<?=$i?>].desc = '<?= $building->get_desc()?>';
	Buildings.buildable_buildings[<?=$i?>].unit_limit_increase = <?
		switch ($building->get_type()) {
			case Buildings::TENT:
				echo 1;
				break;
			case Buildings::DWELLING:
				echo 2;
				break;
			default:
				echo 0;
		} ?>;
<? $i++;
endforeach ?>

Buildings.treasure_buildings = <?= $all_buildings ?>;
Buildings.treasure_buildings = Buildings.treasure_buildings[<?= IIndividualization::TYPE_BUILDING ?>]
Buildings.this_window = PureFW.ContainerAJAX.instances[<?=$this_win_id?>];

Buildings.selected_building = null;
Buildings.selected_color = 0;

Buildings.init_all_buildings = function()
{
	<? include('building_selection_screen.tmpl.inc'); ?>

	var select_i = -1;
	Buildings.all_buildings = new Array();
	found = false;
	trys = 0;
	while (!found) {
		if (trys > 0) {

		}
		var n = Buildings.buildable_buildings.length;
		for(var i = 0; i < n; i++)
		{
			<? if ($type) : /* Gebäude TYPE gleich anwählen */ ?>
				if (Buildings.buildable_buildings[i].type == <?= $type ?>) {
					select_i = i;
				}
				<? /**
					* Im Change-Mode wird genau ein Gebäude verändert. Man kann
					* deswegen hier keine anderen auswählen
					*/ ?>
				else if (Buildings.change_mode) {
					continue;
				}
			<? endif; ?>

			found = true;
			Buildings.all_buildings[i] = new PureFW.Icon(
				BuildingSelection.buildings_choose_scroll.get_content_node(),
				0, 8, 71, 69,
				Buildings.buildable_buildings[i].icon_path
			);
			Buildings.all_buildings[i].set_tooltip(
				Buildings.buildable_buildings[i].name
			);
			Buildings.all_buildings[i].set_color('darkblue');
			Buildings.all_buildings[i].set_positioning('relative');
			Buildings.all_buildings[i].set_float('left');
			document.getElemById(Buildings.all_buildings[i].img_id)
				.style.left = '12.5%';
			document.getElemById(Buildings.all_buildings[i].img_id)
				.style.top = '12.5%';
			document.getElemById(Buildings.all_buildings[i].img_id)
				.style.width = '75%';
			document.getElemById(Buildings.all_buildings[i].img_id)
				.style.height = '75%';



			Buildings.all_buildings[i].add_event_handler('click',
				(function(_i){
					return function() {
						Buildings.select_building(_i);
					}
				})(i)
			);
		}
	}
	if (select_i > -1) <? /*Gebäude TYPE gleich anwählen */ ?>
		Buildings.select_building(select_i);

	if (Buildings.change_mode) {
		n = 1;	// Gibt nur ein Gebäude in der Liste
		<? if ($color) : ?>
		Buildings.select_color(<?= $color ?>);
		<? endif; ?>
	}

	BuildingSelection.buildings_choose_scroll.set_inner_width((n-1)*71+90);
};

Buildings.select_building = function(which)
{
	var n = Buildings.all_buildings.length;
	for (var i = 0; i < n; i++) {
		try {
			Buildings.all_buildings[i].set_width(71);
			Buildings.all_buildings[i].set_height(69);
			Buildings.all_buildings[i].set_y(8);
		}
		catch(e){}
	}
	Buildings.all_buildings[which].set_width(87);
	Buildings.all_buildings[which].set_height(85);
	Buildings.all_buildings[which].set_y(0);

	Buildings.selected_building = Buildings.buildable_buildings[which];
	BuildingSelection.cost_label.set_text(
		-Buildings.selected_building.cost
	);
	BuildingSelection.grow_label.set_text(
		PureFW.Time.sec_in_time(Buildings.selected_building.grow)
	);
	BuildingSelection.unit_limit_label.set_text(
		"+"+Buildings.selected_building.unit_limit_increase
	);
	BuildingSelection.kkr_label.set_text(0);

	<? if (!$change_mode) : /** Im Changemode gibt es nur ein Gebäude */ ?>
	Buildings.officer.set_text(Buildings.selected_building.desc);
	<? endif; ?>
	BuildingSelection.preview_container.set_bg_img(
		Buildings.selected_building.pic_big
	);

	for(var i in Buildings.treasure_buildings)
	{
	        if(Buildings.selected_building.type == Buildings.treasure_buildings[i].id)
	        {
	                Buildings.amount = Buildings.treasure_buildings[i].amount_mp;
	                break;

	        }
	}

	BuildingSelection.next_button_container.show();

	if (!(!Buildings.buildable_buildings[which].custom
		&& (Buildings.buildable_buildings[which].type != 7)))
	{
		BuildingSelection.indivi_container.hide();
		BuildingSelection.amount_available.set_quantity(Buildings.amount);
	}
	else {
		BuildingSelection.amount_available.hide();
		Buildings.select_color(Buildings.selected_color);
		BuildingSelection.indivi_container.show();
	}

};

Buildings.select_color = function(col) {
	if (col && (col > 1)) {
		var kkr = ui_klunker_gainer.get_cur_value();
		if (kkr < Buildings.painting_costs) {
			MammunUI.show_warning(
				350, 200,
				'<?= LNG_ERR_NOT_ENOUGH_KLUNKER ?>'.replace('[LINK]',
					'<a href="javascript: UIButtons.toggle_buy_klunker();">')
				.replace('[/LINK]', '</a>'),
				PureFW.ConfirmationBox.NO,
				true
			);
			Buildings.select_color(0);
			return;
		}
		if (col == <?= $color ?>) {
			<? /** Keine Änderung, hatte schon die Farbe, kein Preis */ ?>
			BuildingSelection.kkr_label.set_content(0);
		}
		else {
			BuildingSelection.kkr_label.set_content(
				-Buildings.painting_costs
			);
		}
		BuildingSelection.preview_container.set_bg_img(
			Buildings.selected_building.pic_big.replace(
				'.png', col+'.png'
			)
		);
	}
	else {
		BuildingSelection.kkr_label.set_content(0);
		BuildingSelection.preview_container.set_bg_img(
			Buildings.selected_building.pic_big
		);
	}
	Buildings.selected_color = col;
};
<? if ($change_mode) : ?>
Buildings.change_color = function() {
	if (Buildings.selected_color == <?= $color ?>) {
		try {
			close_fixed_window();	/* keine Änderung */
		}
		catch(e) {}
	}

	PureFW.AJAXClientServer.send_request(
		"buildings_action.php?change_color=true&x=<?=$x?>&y=<?=$y?>" +
		"&sector_id=<?= $sector_id ?>&world_id=<?=$world_id?>"+
		"&map_id=<?=$map_id?>&type=<?=$type?>" +
		"&color="+Buildings.selected_color,
		function(response) {
			if(isArray(response)) {
				response_text_arr = response;
				if (response_text_arr.length == 1)
				{
					Buildings.officer.set_text('<br/>'+response_text_arr[0]);
				}
				else
				{
					// Interface updaten
					MapDataCtrl.buildings[<?= $sector_id ?>]
						= eval('('+response_text_arr[1]+')');
					MapDataCtrl.refresh_mammun_map();
					if (typeof(response_text_arr[2]) !== 'undefined') {
						ui_klunker_gainer.set_current_amount(
							parseInt(response_text_arr[2])
						);
					}
					ui_refresh();

					if ((typeof(SectorDetails) != 'undefined')
						&& SectorDetails)
					{
						SectorDetails.sector_zoom.clear_buildings();
						SectorDetails.add_all_buildings();
						SectorDetails.sector_zoom.refresh();
					}

					<?/**
					 * Ereignismeldung mit Publish erzeugen
					 */?>
					msg = new Object();
					msg.pic = {
						'pic_url':
							BuildingSelection.preview_container.get_bg_img(),
						'width': 120,
						'height': 137,
						'pos': 0
					}
					<?	$beautiful_nick = User::beautify_nick($nick);
						$b_name = Buildings::get_b_name($type); ?>
					<? if ($nick && ($nick != $current_user->get_nick())) : ?>
					msg.publish_str =
						'<?= str_replace(
							array("[B_NAME]", "[NICK]"),
							array($b_name, $nick),
							LNG_FB_SHARE_CHANGE_COLOR_NON_OWNER_SUCCESS) ?>';
					msg.str =
						'<?= str_replace(
							array("[B_NAME]", "[NICK]"),
							array($b_name, $beautiful_nick),
							LNG_CHANGE_COLOR_NON_OWNER_SUCCESS) ?>';
					<? else: ?>
					msg.publish_str =
						'<?= str_replace(
							array("[B_NAME]", "[NICK]"),
							array($b_name, $nick),
							LNG_FB_SHARE_CHANGE_COLOR_MY_OVERPAINT_SUCCESS) ?>';
					msg.str =
						'<?= str_replace(
							array("[B_NAME]", "[NICK]"),
							array($b_name, $beautiful_nick),
							LNG_CHANGE_COLOR_MY_OVERPAINT_SUCCESS) ?>';
					<? endif; ?>
					EventMessage.show_event_message(msg);

					try {
						close_fixed_window();
					}
					catch(e) {}
				}
			}
		}
	);
}
<? else : ?>
Buildings.start_building = function()
{
	if (!(!Buildings.selected_building.custom
		&& (Buildings.selected_building.type != 7)))
	{
		Buildings.selected_color = 0;
	}
	Buildings.selected_building.color = Buildings.selected_color;

	if (Buildings.selected_color > 0) {
		Buildings.selected_building.pic =
			Buildings.selected_building.pic.replace(
				'.png', Buildings.selected_color+'.png'
			);
	}
	if (!((typeof(SectorDetails) != 'undefined') && (SectorDetails)))
	{<?/* if SectorDetails is undefined or null */?>
		BuildingsConfirm = new PureFW.ConfirmationBox(
			document.body,
			(MammunUI.reference_width - 350) >> 1,
			(MammunUI.reference_height - 200) >> 1,
			350, 200,
			PureFW.ConfirmationBox.YES,
			true
		);
		BuildingsConfirm.set_content(
				'<div style="margin: 5%"><?= LNG_CONSTRUCT_CONFIRMATION ?></div>'
		);
		BuildingsConfirm.set_bg_img(
				'ui/backgrounds/confirm/info/info_350x200.png'
		);
		BuildingsConfirm.add_event_handler("confirm",
			function(ev) {
				GlobalConstructionSite = Buildings.selected_building;
				close_fixed_window();
			}
		);

	}
	else
	{
		try
		{
			SectorDetails.start_building(Buildings.selected_building);
			GlobalConstructionSite = null;
			close_fixed_window();
		}
		catch(e)
		{}
	}
};
<? endif; ?>

Buildings.destroy = function()
{
	Buildings.officer.destroy();
	Buildings.this_window.remove_event_handler("destroy", Buildings.destroy);
	Buildings.this_window.remove_event_handler("change", Buildings.destroy);

	var n = Buildings.all_buildings.length;
	for(var i = 0; i < n; i++)
	{
		Buildings.all_buildings[i].destroy();
	}

	BuildingSelection.destroy();

	Buildings = null;
};



Buildings.officer = new PureFW.Officer(document.body,
	PureFW.Officer.OFFICERS.cco);
Buildings.officer.set_position(50,50);
Buildings.officer.set_officer_position(0, 50);
Buildings.officer.set_balloon_position(-50, -10);
<? if ($change_mode) : ?>
<? if ($nick && ($nick != $current_user->get_nick())) : ?>
Buildings.officer.set_text("<?=LNG_OFFICER_CHANGE_INFO_NON_OWNER?>");
<? else : ?>
Buildings.officer.set_text("<?=LNG_OFFICER_CHANGE_INFO?>");
<? endif; ?>
<? else : ?>
Buildings.officer.set_text("<?=LNG_OFFICER_BUILDING_INFO?>");
<? endif; ?>

Buildings.init_all_buildings();

Buildings.this_window.add_event_handler("destroy", Buildings.destroy);
Buildings.this_window.add_event_handler("change", Buildings.destroy);
</script>