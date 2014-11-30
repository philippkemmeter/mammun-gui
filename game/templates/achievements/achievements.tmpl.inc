<script type="text/javascript">
Achievements = new Object();
Achievements.this_window = PureFW.ContainerAJAX.instances[<?=$this_win_id?>];
Achievements.researched_sciences = <?= $researched_scs_json ?>;
Achievements.type_containers = new Object();

Achievements.create_researched_scs_list = function() {
	var i = 0;
	var j;
	for (var type in Achievements.researched_sciences) {
		Achievements.type_containers[type] = new PureFW.ScrollContainer(
			Achievements.this_window,
			i*200+20, 0,
			180, 0
		);
		Achievements.type_containers[type].set_font_color('#000');
		j = 0;
		for (var sc_id in Achievements.researched_sciences[type])
		{
			Achievements.type_containers[type].add_content(
				'<a href="javascript: Achievements.this_window.set_url(\''
					+ '<?= $_SERVER['PHP_SELF']?>?dataX='
					+ Achievements.researched_sciences[type][sc_id].dataX +
					'\');">'+
				Achievements.researched_sciences[type][sc_id].name + '</a><br/>'
			);
			j++;
		}
		i++;
		Achievements.type_containers[type].set_height(400);
		Achievements.type_containers[type].set_inner_height(j*20);
	}
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

Achievements.create_researched_scs_list();
</script>