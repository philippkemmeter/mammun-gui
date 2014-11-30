<script type="text/javascript">
if (typeof(MapAnimations) == 'undefined')
	MapAnimations = new Object();

MapAnimations.Fireworks = new Object();
MapAnimations.Fireworks.start = function(sector_x, sector_y) {
	var _top = MammunUI.get_top_frame();
	var mp = _top.MapDataCtrl.mammun_map;
	var p = mp.get_field_pixel_position(sector_x, sector_y);
	p.x += mp.position.x;
	p.y += mp.position.y;
	p.x += _top.MapDataCtrl.mammun_map.get_field_width()>>1;
	p.y += _top.MapDataCtrl.mammun_map.get_field_height()>>1;


	MapAnimations.Fireworks.fw = new Fireworks.Fireworks(
		document.getElemById("map_body"),
		p.x,
		p.y,
		200, 200,
		20,
		15,
		true
	);
	MapAnimations.Fireworks.fw.set_z_index(40);
}

MapAnimations.Fireworks.stop = function() {
	MapAnimations.Fireworks.fw.destroy();
}
</script>