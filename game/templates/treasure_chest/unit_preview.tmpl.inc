<script>

if (typeof(TreasureChest) == 'undefined')
	TreasureChest = new Object();
else if (!TreasureChest)
	TreasureChest = new Object();

TreasureChest.create_unit_preview = function(o, parent, item, chest_item) {
	o.details_chooser = new PureFW.Container(
		parent,
		40, 40,
		238, 92
	);
	o.details_chooser.set_bg_img(
		'ui/backgrounds/shop/unit_choose.png'
	);

	o.units = new Array();

<? foreach ($researched_races as $i => $race) : ?>
	<? $pic_core = substr(ScienceConstants::$PICS[$race], 0,
		strrpos(ScienceConstants::$PICS[$race],'.')); ?>

	var l = o.units.length;

	o.units[l] = new Object();

	o.units[l].pic =
		"ui/icons/labels/shop_unit_choose/<?=ScienceConstants::$PICS[$race]?>";
	o.units[l].pic_big =	<? /* Inidivi-Big */ ?>
		"ui/units/indivi/<?=$pic_core?>/<?=$pic_core?>_big_0_0_0_0_0_0_0_0.png";
	o.units[l].choose_pic_big =
		"ui/units/choose/<?=$pic_core?>/<?=$pic_core?>_big2_0_0_0_0_0_0_0_0.png";
	o.units[l].overlay_pic_path =
		 "ui/units/indivi/<?=$pic_core?>/item_overlay/";
	o.units[l].name = "<?= ScienceConstants::$SC_NAME[$race] ?>";
	o.units[l].type = '<?=$pic_core?>';
 <? endforeach; ?>

	o.race_icons = new Array();
	var l = o.units.length;
	var icon_counter = 0;

	for (var unit_counter = 0; unit_counter < l; unit_counter++)
	{
		if( (o.units[unit_counter].type != "club") 	&&
			(o.units[unit_counter].type != "blade") 	&&
			(o.units[unit_counter].type != "bow") 	&&
			(o.units[unit_counter].type != "javelin"))
		{
			continue;
		}

		var temp_icon =
			new PureFW.Image(
				o.details_chooser.get_content_node(),
				3, 10,
				57, 75,
				o.units[unit_counter].pic
			);
		temp_icon.set_tooltip(o.units[unit_counter].name);
		temp_icon.set_positioning('relative');
		temp_icon.set_float('left');

		temp_icon.add_event_handler(
			"click",
			(function(_unit, _icon, _item) {
				return function(ev) {
					TreasureChest.select_race(_unit, _icon, _item);
				}
			})(unit_counter, icon_counter, item)
		);

		o.race_icons.push(temp_icon);
		icon_counter++;
	}

	o.details_pic = new PureFW.Container(
		parent,
		o.details_chooser.position.x+3,
		o.details_chooser.position.y
			+ o.details_chooser.height,
		232,289
	);
	o.details_pic.set_bg_img(
			'ui/backgrounds/shop/unit_preview.png'
		);

	o.details_pic2 = new PureFW.Image(
		o.details_pic,
		(o.details_pic.width - 165) >> 1,
		0,
		165, 289
	);

	o.details_pic2.set_pic_url('/pattern/spacer.gif');
}

TreasureChest.select_race = function(which_unit, which_icon, item)
{
	TreasureChest.item_details.unit_selected =
		TreasureChest.item_details.units[which_unit];
	chosen_unit = TreasureChest.item_details.unit_selected;

	var n = TreasureChest.item_details.race_icons.length;
	for (var i = 0; i < n; i++)
	{
		TreasureChest.item_details.race_icons[i].set_width(57);
		TreasureChest.item_details.race_icons[i].set_height(75);
		TreasureChest.item_details.race_icons[i].set_y(10);
	}
	TreasureChest.item_details.race_icons[which_icon].set_width(67);
	TreasureChest.item_details.race_icons[which_icon].set_height(89);
	TreasureChest.item_details.race_icons[which_icon].set_y(3);

	var img_pic_path = 'ui/units/training/'+TreasureChest.item_details.units[which_unit].type+
								'/'+TreasureChest.item_details.units[which_unit].type+'_small_0_';
	for (var i = 0; i < item.type-1; i++) {
		img_pic_path += '0_';
	}
	img_pic_path += item.id;
	for (var i = item.type; i < 6; i++) {
		img_pic_path += '_0';
	}
	img_pic_path += '_0.png';
	TreasureChest.item_details.details_pic2.set_pic_url(img_pic_path);
};
</script>