<?
/**
 * ACHTUNG: Wegen automatischem Tagschließen von Browsern nach innerHTML darf
 * kein JS innerhalb von Tags stehen! Am besten JS am Anfang und am Ende der Datei
 * reinschreiben. Wenn innerhalb von Tabellen entstehen meist Tabellenfehler!
 */
$this_id = rand(0,1000000);	// eindeutige ID // NOT SURE IF I NEED THIS!!!
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

<script type="text/javascript">

UnitWeaponScreen = null;	// später
UnitSideInfo = null; 		// später
UnitIndividualScreen = null;// später

UnitProduction = new Object();
UnitProduction.loaded = false;

UnitProduction.this_window = PureFW.ContainerAJAX.instances[<?=$this_win_id?>];
UnitProduction.this_window.set_css_class('Window_dark');

UnitProduction.unit_selected = null;
UnitProduction.refresh_interval = null;

<? /* Alle Funktionen, die hier reingepusht werden, werden aufgerufen, wenn
	  ein Mammun komplett konfiguriert wurde, und sein Training jetzt beginnt */
?>
UnitProduction.training_started_functions = new Array();
<? /** Wie viele Waffen/Rassen er erforscht hat */ ?>
UnitProduction.researched_races_amt = <?= count($researched_races) ?>;
<?/**
    * Units-Array enthält alle Einheiten, die als Trainingsplan erstellt
    * werden können. Dabei enthält die 1.Dimension des Unit-Arrays die Rassen.
    * Darunter sind dann alle Fähigkeiten, die diese Rasse bekommen darf,
    * sortiert nach Ability-Typ.
    */ ?>
UnitProduction.units = new Array();
<? foreach ($researched_races as $i => $race) : ?>
	<? $pic_core = substr(ScienceConstants::$PICS[$race], 0,
		strrpos(ScienceConstants::$PICS[$race],'.')); ?>

	var l = UnitProduction.units.length;
	UnitProduction.units[l] = new Object();
	UnitProduction.units[l].pic =
		"ui/icons/labels/units/54/<?=ScienceConstants::$PICS[$race]?>";
	UnitProduction.units[l].pic_big =	<? /* Inidivi-Big */ ?>
		"ui/units/indivi/<?=$pic_core?>/<?=$pic_core?>_big_0_0_0_0_0_0_0_0.png";
	UnitProduction.units[l].choose_pic_big =
		"ui/units/choose/<?=$pic_core?>/<?=$pic_core?>_big2_0_0_0_0_0_0_0_0.png";
	UnitProduction.units[l].overlay_pic_path =
		 "ui/units/indivi/<?=$pic_core?>/item_overlay/";
	UnitProduction.units[l].desc =
		"<?= ScienceConstants::$SC_DESC[$race] ?>";
	UnitProduction.units[l].name =
		"<?= ScienceConstants::$SC_NAME[$race] ?>";
	UnitProduction.units[l].race =
		"<?= $cryptObj->do_encryption(array("chosen_race" => $race)) ?>";
	UnitProduction.units[l].num = l;
	<? $tribe = new Tribe((array)($techtree->get_science($race))); ?>
	UnitProduction.units[l].att = <?= $tribe->get_att() ?>;
	UnitProduction.units[l].def = <?= $tribe->get_def() ?>;
	UnitProduction.units[l].hp = <?= $tribe->get_hp() ?>;
	UnitProduction.units[l].speed = <?= $tribe->get_speed() ?>;
	UnitProduction.units[l].cost = <?= $tribe->get_cost()*1000 ?>;
	<? if (isset($morphing_tribes_by_race[$race])) : ?>
		<? $tribe = $morphing_tribes_by_race[$race]; ?>
		UnitProduction.units[l].time_needed = <?= $tribe->get_time_needed()*
			$tribe->get_add_value()?>;
		UnitProduction.units[l].is_morphing = true;
		UnitProduction.units[l].add_stamp = <?= $tribe->get_add_stamp() ?>;
		UnitProduction.units[l].training_pic_big =
			'ui/<?= $tribe->get_pic_url() ?>'.replace(".png", '_0.png')
				.replace('<?=$pic_core?>/<?=$pic_core?>_',
					'training/<?=$pic_core?>/<?=$pic_core?>_small_');
		UnitProduction.units[l].unit_name = '<?= $tribe->get_name() ?>';
	<? else : ?>
		UnitProduction.units[l].time_needed = <?= $tribe->get_time_needed()*1000 ?>;
		UnitProduction.units[l].is_morphing = false;
		UnitProduction.units[l].add_stamp = 0;
	<? endif; ?>
	UnitProduction.units[l].allowed_abils = new Array();
	<? $ai=0;$k = 0;
	foreach($allowed_ability_types[$i] as $abil_type => $foo) : ?>
		UnitProduction.units[l].allowed_abils[<?=$k?>] = new Array();
		UnitProduction.units[l].allowed_abils[<?=$k?>].name = "";
		UnitProduction.units[l].allowed_abils[<?=$k?>].desc = "";
		UnitProduction.units[l].allowed_abils[<?=$k?>].id =
			<?= $abil_type ?>;
		<? for($j = 0; $j < count($researched_abilities[$abil_type]); $j++,$ai++) : ?>
			UnitProduction.units[l].allowed_abils[<?=$k?>][<?=$j?>] = new Object();
			UnitProduction.units[l].allowed_abils[<?=$k?>][<?=$j?>].name =
				"<?= ScienceConstants::$SC_NAME[$researched_abilities[$abil_type][$j]] ?>";
			UnitProduction.units[l].allowed_abils[<?=$k?>][<?=$j?>].desc =
				"<?= ScienceConstants::$SC_DESC[$researched_abilities[$abil_type][$j]] ?>";
			UnitProduction.units[l].allowed_abils[<?=$k?>][<?=$j?>].id =
				"<?= $cryptObj->do_encryption(array("ability$ai"
						=> $researched_abilities[$abil_type][$j])) ?>";
			UnitProduction.units[l].allowed_abils[<?=$k?>][<?=$j?>].pic =
				"ui/icons/labels/ability/42/<?=
					ScienceConstants::$PICS[$researched_abilities[$abil_type][$j]] ?>";
			UnitProduction.units[l].allowed_abils[<?=$k?>][<?=$j?>].num = <?=$ai?>;
			UnitProduction.units[l].allowed_abils[<?=$k?>][<?=$j?>].abil_num = <?=$j?>;
			UnitProduction.units[l].allowed_abils[<?=$k?>][<?=$j?>].type = <?=$k?>;
			<? $abilityvalues = $techtree->get_science(
				$researched_abilities[$abil_type][$j]); ?>
			UnitProduction.units[l].allowed_abils[<?=$k?>][<?=$j?>].att =
				<?= $abilityvalues->attack ?>;
			UnitProduction.units[l].allowed_abils[<?=$k?>][<?=$j?>].def =
				<?= $abilityvalues->defend ?>;
			UnitProduction.units[l].allowed_abils[<?=$k?>][<?=$j?>].hp =
				<?= $abilityvalues->hitpoints ?>;
			UnitProduction.units[l].allowed_abils[<?=$k?>][<?=$j?>].speed =
				<?= $abilityvalues->speed ?>;
			UnitProduction.units[l].allowed_abils[<?=$k?>][<?=$j?>].cost =
				<?= $abilityvalues->cost * 1000?>;
		<? endfor; ?>

		<? if ($change_clothes) : ?>
		<? if ($change_clothes_race == $race) : ?>
		UnitProduction.unit_selected = UnitProduction.units[l];
		<? endif; ?>
		<? endif; ?>
	<? $k++;
	endforeach; ?>
<? endforeach; ?>
<? /**
	* UnitProduction.items[type][i] = new Object();
	* UnitProduction.items[type][i].cost = KOSTEN
	* UnitProduction.items[type][i].id = ID (nicht verschlüsselt)
	* UnitProduction.items[type][i].type = TYP (dasselbe wie type)
	* UnitProduction.items[type][i].shop_id = WO KANN MAN DAS KAUFEN
	* UnitProduction.items[type][i].name = NAME
	* UnitProduction.items[type][i].desc = BESCHREIBUNG
	*/ ?>
UnitProduction.items = <?=$all_items_json?>;
<? /**
    * UnitProduction.my_items[type][i] = new Object();
    * UnitProduction.my_items[type][i].amount = ANZAHL VERFÜGBAR
    * UnitProduction.my_items[type][i].mp_amount = ANZAHL SCHAUFENSTER
    * UnitProduction.my_items[type][i].id = ID
    * UnitProduction.my_items[type][i].type = TYPE
    * UnitProduction.my_items[type][i].name = NAME
    * UnitProduction.my_items[type][i].desc = BESCHREIBUNG
    */ ?>
UnitProduction.my_items = <?= $my_items_json ?>;

UnitProduction.slots = new Array();
UnitProduction.abils_selected = null;
UnitProduction.items_selected = <?= json_encode($items_selected) ?>;
UnitProduction.chosen_name = "";

UnitProduction.create_unit_slots = function()
{
	<?//TODO: Need failsafe to make sure no more than 3 units are morphing simultaneously?>
	var slot_num = 0;
	var n = UnitProduction.units.length;
	var slot_size = new PureFW.Point(165, 289);
	var first_slot_pos = new PureFW.Point(220, 95);
	var slot_h_space = 14;
	var slot_x_pos = first_slot_pos.x;
	for(var i = 0; i < n; i++)
	{
		if(UnitProduction.units[i].is_morphing)
		{
			UnitProduction.slots[slot_num] = new PureFW.UnitSlot(
				UnitProduction.this_window,
				slot_x_pos,
				first_slot_pos.y,
				slot_size.x,
				slot_size.y
			);
			UnitProduction.slots[slot_num].current_unit = UnitProduction.units[i];
			UnitProduction.slots[slot_num].set_unit_pic_url(
				UnitProduction.units[i].training_pic_big
			);
			UnitProduction.slots[slot_num].set_unit_name(
				UnitProduction.units[i].unit_name);
			/*UnitProduction.slots[slot_num].set_unit_description(
				'('+UnitProduction.units[i].desc+')');*/
			UnitProduction.slots[slot_num].set_unit_time(
				UnitProduction.units[i].time_needed);
			UnitProduction.slots[slot_num].show_content();

			slot_num += 1;
			slot_x_pos += slot_size.x+slot_h_space;
		}
	}
	while (slot_num < 3)
	{
		UnitProduction.slots[slot_num] = new PureFW.UnitSlot(
			UnitProduction.this_window,
			slot_x_pos,
			first_slot_pos.y,
			slot_size.x,
			slot_size.y
		);
		if (slot_num < UnitProduction.researched_races_amt) {
			UnitProduction.slots[slot_num].display_empty(
				(function(_i)
				{
					return function() {
						UnitProduction.choose_race_dialog(_i);
					}
				})(slot_num));
			UnitProduction.slots[slot_num].set_overlay_text(
				'<br/><br/><br/><b><?= LNG_B_CLICK_HERE_TO_ADD ?></b>');
		}
		else {
			UnitProduction.slots[slot_num].display_empty(function(){});
			//UnitProduction.slots[slot_num].deactivate();
			UnitProduction.slots[slot_num].hide();
		}
		slot_num += 1;
		slot_x_pos += slot_size.x+slot_h_space;
	}
	UnitProduction.rt_morphing();

};

UnitProduction.destroy_unit_slots = function()
{
	UnitProduction.slots.destroy();
	UnitProduction.slots = new Array();
};

UnitProduction.show_first_page = function(officer_text)
{
	if (!officer_text)
		UnitProduction.officer.set_text('<?=$officer_text_intro?>');
	else
		UnitProduction.officer.set_text(officer_text);

	if (UnitWeaponScreen) {
		UnitWeaponScreen.destroy();
	}
	if (UnitSideInfo) {
		UnitSideInfo.destroy();
	}
	if (UnitIndividualScreen) {
		UnitIndividualScreen.destroy();
	}
	UnitProduction.create_unit_slots();
};


UnitProduction.choose_race_dialog = function(cur_slot)
{
	if (Overview.get_unit_produced() >= Overview.get_unit_limit()) {
		UnitProduction.officer.set_text('<font color="red">'
			+'<?=$officer_text_unit_limit_reached?></font>');
		return;
	}
	else if (ui_food_gainer.get_cur_value() < 3000) {
		UnitProduction.officer.set_text(
			'<?=$officer_text_not_enough_food_for_anything?>');
		return;
	}
	UnitProduction.officer.set_text('<?=$officer_text_select_unit_options?>');
	if (UnitIndividualScreen) {
		UnitIndividualScreen.destroy();
	}
	UnitProduction.destroy_unit_slots();
	if (!UnitSideInfo)
	{
		<? include('unit_production_attrib_etc.tmpl.inc'); ?>
	}

	<? include('unit_production_training_display.tmpl.inc'); ?>

	document.forms.UnitProduction_form.chosen_name.value =
		(UnitProduction.chosen_name.length > 0)
			? UnitProduction.chosen_name
			: NameSuggestions.get_name();
	UnitProduction.race_icons = new Array();
	var l = UnitProduction.units.length;

	for (var i = 0; i < l; i++)
	{
		UnitProduction.race_icons[i] =
			new PureFW.Image(
				UnitWeaponScreen.weapon_choose_scroll.get_content_node(),
				0, 10,
				57, 75,
				UnitProduction.units[i].pic
			);
		UnitProduction.race_icons[i].set_tooltip(UnitProduction.units[i].name);
		UnitProduction.race_icons[i].set_positioning('relative');
		UnitProduction.race_icons[i].set_float('left');

		if(UnitProduction.units[i].is_morphing)
		{
			UnitProduction.race_icons[i].deactivate();
		}
		else
		{
			UnitProduction.race_icons[i].add_event_handler(
				"click",
				(function(_i) {
					return function(ev) {
						UnitProduction.select_race(_i);
					}
				})(i)
			);
		}
	}
	UnitWeaponScreen.weapon_choose_scroll.set_inner_width(
		(l-1)*57+75
	);

	var abils_selected = UnitProduction.abils_selected;

	<? /* Wenn bereits eine gewählt wurde (z.B. wenn man zurücknavigiert) */?>
	if (UnitProduction.unit_selected) {
		UnitProduction.select_race(UnitProduction.unit_selected.num);
	}
	if (abils_selected && (abils_selected.length > 0))
	{
		for (var i = 0; i < abils_selected.length; i++) {
			UnitProduction.select_abil(
				abils_selected[i].type,
				abils_selected[i].abil_num
			);
		}
	}
};

UnitProduction.choose_inidiv_dialog = function(cur_slot) {
	<? if (!$change_clothes) : ?>
	if (!UnitProduction.unit_selected) {
		UnitProduction.officer.set_text('<?= LNG_ERR_NO_WEAPON ?>');
	}
	else if (!UnitProduction.chosen_name) {
		UnitProduction.officer.set_text('<?= LNG_ERR_NO_NAME ?>');
	}
	else {
	<? endif; ?>
		if (UnitWeaponScreen)
			UnitWeaponScreen.destroy();
		if (!UnitSideInfo)
		{
			<? include('unit_production_attrib_etc.tmpl.inc'); ?>
		}
		UnitProduction.officer.set_text('<?= $officer_text_select_indivi ?>');
		var items_selected = UnitProduction.items_selected;
		<? include('unit_production_indivi.tmpl.inc'); ?>
		if (items_selected && (items_selected.length > 0)) {
			for (i in items_selected) {
				if (typeof(items_selected[i]) != 'function')
					UnitIndividualScreen.select_item_shop(items_selected[i]);
			}
		}
	<? if (!$change_clothes) : ?>
	}
	<? endif; ?>
}

UnitProduction.select_race = function(which)
{
	if (UnitProduction.units[which].is_morphing)
	{
		alert('Unit is morphing');
		return;
	}

	UnitProduction.unit_selected = UnitProduction.units[which];
	chosen_unit = UnitProduction.unit_selected;

	var n = UnitProduction.race_icons.length;
	for (var i = 0; i < n; i++) {
		UnitProduction.race_icons[i].set_width(57);
		UnitProduction.race_icons[i].set_height(75);
		UnitProduction.race_icons[i].set_y(10);
	}
	UnitProduction.race_icons[which].set_width(67);
	UnitProduction.race_icons[which].set_height(89);
	UnitProduction.race_icons[which].set_y(3);


	UnitWeaponScreen.unit_container.set_bg_img(chosen_unit.choose_pic_big);

	UnitSideInfo.cost_label.set_text(
		number_format(-chosen_unit.cost, 0, ',', '.')+'g'
	);
	UnitSideInfo.t_n_label.set_text(
		PureFW.Time.sec_in_time(chosen_unit.time_needed)
	);

	<? /** Jetzt die Attribut-Label den entsprechenden HTML-Elementen zuordnen */?>
	UnitSideInfo.att_label.set_text(chosen_unit.att);
	UnitSideInfo.def_label.set_text(chosen_unit.def);
	UnitSideInfo.hp_label.set_text(chosen_unit.hp);
	UnitSideInfo.speed_label.set_text(chosen_unit.speed);
	UnitSideInfo.kkr_label.set_text(0);

	UnitProduction.choose_abilities_dialog();
};

UnitProduction.choose_abilities_dialog = function()
{
	if (UnitProduction.unit_selected.is_morphing)
	{
		return;
	}

	UnitProduction.officer.set_text('<?= $officer_text_select_abils ?>');

	UnitProduction.abils_selected = new Array();

	if(UnitProduction.abil_icons)
	{
		var n = UnitProduction.abil_icons.length;
		for(var i = 0; i < n; i++)
		{
			var k = UnitProduction.abil_icons[i].length;
			for(var j = 0; j < k; j++)
			{
				UnitProduction.abil_icons[i][j].dumb_pointless_container.destroy();
				UnitProduction.abil_icons[i][j].toggle_btn.destroy();
				UnitProduction.abil_icons[i][j].abil_pic.destroy();
				UnitProduction.abil_icons[i][j].abil_name.destroy();
				UnitProduction.abil_icons[i][j].destroy();
			}
		}
	}

	var chosen_unit = UnitProduction.unit_selected;

	<? /* Abilities */ ?>

	<? /** Altes Array leeren (sauber raus ausm Speicher) */ ?>
	if (UnitProduction.abil_icons) {
		var n = UnitProduction.abil_icons.length;
		for (i = 0; i < n; i++) {
			var m = UnitProduction.abil_icons[i].length;
			for (j = 0; j < UnitProduction.abil_icons; j++) {
				UnitProduction.abil_icons[i][j].toggle_btn.destroy();
				UnitProduction.abil_icons[i][j].abil_pic.destroy();
				UnitProduction.abil_icons[i][j].abil_name.destroy();

				UnitProduction.abil_icons[i][j].dumb_pointless_container
					.destroy();
				UnitProduction.abil_icons[i][j].destroy();
			}
		}
	}
	UnitProduction.abil_icons = new Array();

	<? /** Neues Array aufbauen :) */ ?>
	var abil,m,n = chosen_unit.allowed_abils.length;
	var y = 0;
	for (i = 0, k = 0; i < n; i++)
	{
		m = chosen_unit.allowed_abils[i].length;
		UnitProduction.abil_icons[i] = new Array();
		for (j = 0; j < m; j++)
		{
			abil = chosen_unit.allowed_abils[i][j];
			UnitProduction.abil_icons[i][j] = new PureFW.Container(
				UnitWeaponScreen.abilities_container.get_content_node(),
				1, y, 257, 54
			);
			UnitProduction.abil_icons[i][j].set_bg_img(
				'ui/backgrounds/unitBgs/ability_row.png'
			);
			UnitProduction.abil_icons[i][j].set_font_size(0.8);

			UnitProduction.abil_icons[i][j].add_event_handler(
				"click",
				(function(n1,n2) {
					return function(ev) {
						UnitProduction.select_abil(n1,n2);
					}
				})(i,j)
			);
			<?//TODO: Really lazy workaround for changing style.top of the button... will fix later?>
			<? //TODO: Use Checkbox-Widget here! ?>
			UnitProduction.abil_icons[i][j].dumb_pointless_container
				= new PureFW.Container(
					UnitProduction.abil_icons[i][j],
					10, 8,
					36, 31
			);
			UnitProduction.abil_icons[i][j].dumb_pointless_container.get_node()
				.style.overflow = 'hidden';
			UnitProduction.abil_icons[i][j].toggle_btn = new PureFW.Container(
				UnitProduction.abil_icons[i][j].dumb_pointless_container
															.get_content_node(),
					0, 0,
					36, 62
			);
			UnitProduction.abil_icons[i][j].toggle_btn.set_bg_img(
				'ui/elements/form/input/check_toggle.png'
			);

			UnitProduction.abil_icons[i][j].abil_pic = new PureFW.Container(
				UnitProduction.abil_icons[i][j],
				50, 4, 42, 42
			);
			UnitProduction.abil_icons[i][j].abil_pic.set_bg_img(abil.pic);

			UnitProduction.abil_icons[i][j].abil_name = new PureFW.Container(
				UnitProduction.abil_icons[i][j],
				100, 18,
				UnitProduction.abil_icons[i][j] - 96, 16
			);
			UnitProduction.abil_icons[i][j].abil_name.set_content(abil.name);

			y += 54;
		}
	}
	UnitWeaponScreen.abilities_container.set_inner_height(y);
};

UnitProduction.select_abil = function(type, abil_num)
{
	var abil = UnitProduction.unit_selected.allowed_abils[type][abil_num];
	if (UnitProduction.abils_selected.contains(abil))
	{
		UnitProduction.abil_icons[type][abil_num].toggle_btn.set_y(0);
		UnitProduction.abils_selected.remove(abil);
	}
	else
	{
		UnitProduction.abil_icons[type][abil_num].toggle_btn.set_y(
			-UnitProduction.abil_icons[type][abil_num].toggle_btn.height/2);
		UnitProduction.abils_selected.push(abil);
	}
	UnitProduction.recalculate_attribs();
};

UnitProduction.recalculate_attribs = function()
{
	var att = UnitProduction.unit_selected.att;
	var def = UnitProduction.unit_selected.def;
	var hp = UnitProduction.unit_selected.hp;
	var speed = UnitProduction.unit_selected.speed;
	var cost = UnitProduction.unit_selected.cost;

	var n = UnitProduction.abils_selected.length;
	for (var i = 0; i < n; i++)
	{
		att += UnitProduction.abils_selected[i].att;
		def += UnitProduction.abils_selected[i].def;
		hp += UnitProduction.abils_selected[i].hp;
		speed += UnitProduction.abils_selected[i].speed;
		cost += UnitProduction.abils_selected[i].cost;
	}
	UnitSideInfo.att_label.set_text(att);
	UnitSideInfo.def_label.set_text(def);
	UnitSideInfo.hp_label.set_text(hp);
	UnitSideInfo.speed_label.set_text(speed);
	UnitSideInfo.cost_label.set_text(number_format(-cost,0,',','.')+'g');
	var t_n = Math.floor(5*Math.pow(cost/1000, 1.15))*1000;
	UnitSideInfo.t_n_label.set_text(PureFW.Time.sec_in_time(t_n));
};

<? /**
	* Wird Am Ende (nach den Individualisierungen) die Wahl bestätigt, wird
	* diese Funktion aufgerufen, die den Bestätigungsvorgang durchgeht. Es
	* wird eine Anfrage an den Server gestellt, der die Wahl als hoffentlich
	* korrekt annehmen wird, und das Morphen beginnen lassen sollte.
	*/ ?>
UnitProduction.submit_all_choices = function(<?/*ev*/?>)
{
	<? if ($change_clothes) : ?>
	var post_data = "&action=change_clothes&id0=<?= $_REQUEST['id0'] ?>"
		+ "&race_change=<?= $GP['race_change'] ?>";
	<? else : ?>
	var n = UnitProduction.abils_selected.length;
	var post_data = 'action=morph&amount=1';
	for (var i = 0; i < n; i++) {
		post_data += "&ability";
		post_data += UnitProduction.abils_selected[i].num;
		post_data += "=";
		post_data += UnitProduction.abils_selected[i].id;
	}
	post_data += "&chosen_race=";
	post_data += UnitProduction.unit_selected.race;
	post_data += "&chosen_name=";
	post_data += UnitProduction.chosen_name.replace('&', '');
	<? endif; ?>
	for (i in UnitProduction.items_selected) {
		if (typeof(UnitProduction.items_selected[i]) != 'function') {
			post_data += "&item";
			post_data += UnitProduction.items_selected[i].type;
			post_data += "=";
			post_data += UnitProduction.items_selected[i].id;
		}
	}

	PureFW.AJAXClientServer.send_request("<?= $_SERVER['PHP_SELF'] ?>",
		UnitProduction.submit_response, post_data);
};

<? /**
	* Hierüber wird die Rückantwort des Servers nach submit_all_choices
	* abgehandelt
	*/ ?>
UnitProduction.submit_response = function(response_arr)
{
	if (response_arr[0] == 1)
	{
	<? if ($change_clothes) : ?>
		if (ui_klunker_gainer)
			ui_klunker_gainer.set_current_amount(parseInt(response_arr[2]));
		ui_refresh();

		MapDataCtrl.update_map(
			(function(id) {
				return function(ev) {
					SectorDetails.open_unit_popup(
						id,
						'"<?= LNG_MAMMUN_W_NEW_CLOTHES_SAIS ?>"'
					);
					SectorDetails.create_units();
					SectorDetails.sector_zoom.refresh();
				}
			})(response_arr[1])
		);
		UnitProduction.destroy();
		map_ui__fixed_window.destroy();

	<? else : ?>
		UnitProduction.unit_selected.add_stamp = parseInt(response_arr[3]);
		UnitProduction.unit_selected.time_needed = parseInt(response_arr[5]);
		UnitProduction.unit_selected.is_morphing = true;
		UnitProduction.unit_selected.unit_name = response_arr[11];
		var pic_core = response_arr[10].substring('units/'.length,
			response_arr[10].lastIndexOf('/'));
		UnitProduction.unit_selected.training_pic_big = 'ui/'+response_arr[10]
			.replace(".png", '_0.png')
				.replace(pic_core+'/'+pic_core+'_',
					'training/'+pic_core+'/'+pic_core+'_small_');

		<? /* Essensmenge korrigieren (das morphen ist nicht umsonst) */ ?>
		if (ui_food_gainer)
			ui_food_gainer.set_current_amount(parseInt(response_arr[6]));
		<? /* Punkte anpassen */ ?>
		if (PauseMenu.statistics.ui_my_score_gainer) { // Map-Score
			PauseMenu.statistics.ui_my_score_gainer.set_current_amount(
				parseInt(response_arr[7])
			);
			PauseMenu.rt_refresh();
		}
		Overview.xp_bar.set_xp_score(parseInt(response_arr[8]));
		<? /* Klunker anpassen */ ?>
		if (ui_klunker_gainer)
			ui_klunker_gainer.set_current_amount(parseInt(response_arr[9]));
		<? /* Einheitenlimit anpassen */ ?>
		Overview.increase_unit_produced(1);

		ui_refresh();
		UnitProduction.unit_selected = null;
		UnitProduction.abils_selected = null;
		UnitProduction.items_selected = null;
		var text = '<?=LNG_TRAINING_STARTED_SUCCESSFULLY_TEXT?>'
				.replace(/\[NAME\]/g, response_arr[11]);

		UnitProduction.chosen_name = "";
		UnitProduction.show_first_page('');

		var n = UnitProduction.training_started_functions.length;
		for (var i = 0; i < n; i++)
			UnitProduction.training_started_functions[i].call(this,
				PureFW.Widget.create_event('training_started'));

		UnitProduction.officer.balloon_container.hide();
		MammunUI.show_warning(360, 255, text, PureFW.ConfirmationBox.YES);
	<? endif; ?>
	}
	else {
		MammunUI.show_warning(350, 200, response_arr[1],
			PureFW.ConfirmationBox.NO, true);
	}
};

UnitProduction.begin_rt_morphing = function()
{
	UnitProduction.rt_morphing();

	UnitProduction.refresh_interval =
		PureFW.Timeout.set_interval(UnitProduction.rt_morphing, 1000);
};

UnitProduction.rt_morphing = function() {
	var j = new Date();
	var t_now = PureFW.Time.get_current_server_time();
	var n = UnitProduction.slots.length;
	var cur_unit = null;
	for (var i = 0; i < n; i++) {
		if(UnitProduction.slots[i].current_unit)
		{
			cur_unit = UnitProduction.slots[i].current_unit;
			if (!cur_unit.is_morphing)
				continue;

			var secs_left = cur_unit.time_needed
				- (t_now - cur_unit.add_stamp);

			var secintime = PureFW.Time.sec_in_time(secs_left, true, false);
			if (UnitProduction.slots[i]) {
				UnitProduction.slots[i].set_unit_time(secintime);
			}
			if (secs_left <= 0) {
				<? /* TODO: Alle Daten aktuallisieren */ ?>
				cur_unit.is_morphing = false;
				UnitProduction.slots[i].display_empty(
					function() {
						UnitProduction.choose_race_dialog(i);
					});
			}
		}
	}
};

UnitProduction.keyup = function(ev)
{
	try
	{
		ev = PureFW.EventUtil.formatEvent(ev);
	}
	catch(e){}
	ev.stopPropagation();
};

UnitProduction.destroy = function()
{

	PureFW.AJAXClientServer.send_request("<?= $_SERVER['PHP_SELF'] ?>",
		null, "dataX=<?
		echo $cryptObj->do_encryption(array("release_locks" => 1)); ?>", true);

	if (UnitProduction.refresh_interval)
		PureFW.Timeout.clear_interval(UnitProduction.refresh_interval);
	UnitProduction.this_window.set_css_class('');
	UnitProduction.this_window
		.remove_event_handler("destroy", UnitProduction.destroy);
	UnitProduction.this_window
		.remove_event_handler("change", UnitProduction.destroy);

	if(UnitWeaponScreen)
	{
		UnitWeaponScreen.destroy();
	}
	if(UnitSideInfo)
	{
		UnitSideInfo.destroy();
	}
	if (UnitIndividualScreen)
	{
		UnitIndividualScreen.destroy();
	}

	if(UnitProduction.race_icons)
	{
		var n = UnitProduction.race_icons.length;
		for (var i = 0; i < n; i++)
		{
			UnitProduction.race_icons[i].destroy();
		}
	}
	var k = UnitProduction.slots.length;
	for (var i = 0; i < k; i++)
	{
		if(UnitProduction.slots[i].big_huge_x)
			UnitProduction.slots[i].big_huge_x.destroy();
		UnitProduction.slots[i].destroy();
	}
	if(UnitProduction.abil_icons)
	{
		var n = UnitProduction.abil_icons.length;
		for(var i = 0; i < n; i++)
		{
			var k = UnitProduction.abil_icons[i].length;
			for(var j = 0; j < k; j++)
			{
				UnitProduction.abil_icons[i][j].dumb_pointless_container.destroy();
				UnitProduction.abil_icons[i][j].toggle_btn.destroy();
				UnitProduction.abil_icons[i][j].abil_pic.destroy();
				UnitProduction.abil_icons[i][j].abil_name.destroy();
				UnitProduction.abil_icons[i][j].destroy();
			}
		}
	}

	destroy_object(UnitProduction);
};


<?
/******************************************************************************
 *                  /\     .---.    -----    -     .---.   .  .               *
 *                 /__\    |          |      |     |   |   |`.|               *
 *                /    \   `---'      -      -     `---'   '  '               *
 *                                                                            *
 *                             A  C  T  I  O  N                            PK *
 ******************************************************************************/
?>

UnitProduction.officer = new PureFW.Officer(
	document.body, PureFW.Officer.OFFICERS.cwo
);
UnitProduction.officer.set_pic_width(181);
UnitProduction.officer.set_pic_height(324);
UnitProduction.officer.set_overall_position(46, 45);
UnitProduction.officer.set_officer_position(-10, 70);
UnitProduction.officer.set_balloon_position(-40, -10);

<? if ($change_clothes) : ?>
UnitProduction.choose_inidiv_dialog();
UnitSideInfo.t_n_label.hide();
UnitSideInfo.cost_label.hide();
UnitSideInfo.att_label.hide();
UnitSideInfo.def_label.hide();
UnitSideInfo.hp_label.hide();
UnitSideInfo.speed_label.hide();
<? else : ?>
UnitProduction.begin_rt_morphing();
UnitProduction.create_unit_slots();

if (Overview.get_unit_produced() >= Overview.get_unit_limit()) {
	UnitProduction.officer.set_text('<?= $officer_text_unit_limit_reached ?>');
}
else {
	UnitProduction.officer.set_text('<?= $officer_text_intro ?>');
}
<? endif; ?>
UnitProduction.this_window.add_event_handler("destroy", UnitProduction.destroy);
UnitProduction.this_window.add_event_handler("change", UnitProduction.destroy);
UnitProduction.loaded = true;
</script>