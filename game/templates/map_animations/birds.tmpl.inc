<script type="text/javascript">
if (typeof(MapAnimations) == 'undefined')
	MapAnimations = new Object();

MapAnimations.Birds = new Object();
MapAnimations.Birds.start = function() {
	PureFW.Timeout.set_timeout(
		function() {
			MapAnimations.Birds.bird = new PureFW.FilmAnimator(
				document.getElemById("map_body"),
				1800, 100,
				120, 120,
				true
			);
			MapAnimations.Birds.bird.instance_name = "MammunUI.get_top_frame()."
				 + MapAnimations.Birds.bird.instance_name;

			MapAnimations.Birds.bird.set_image(
				'ui/animations/bird/steven_2.png', 16
			);
			MapAnimations.Birds.bird.start_animation();

			MapAnimations.Birds.bird.set_x(
				-200, 15000
			),
			MapAnimations.Birds.bird.set_y(
				1200, 15000, null, function(ev) {
					MapAnimations.Birds.bird.destroy();
					MapAnimations.Birds.bird = null;
				}
			);
			MapAnimations.Birds.bird.set_z_index(40);
			MapAnimations.Birds.start();

			MapAnimations.Birds.bird.add_event_handler("click",
				function(ev) {
					var _top = MammunUI.get_top_frame();
					var bird = _top.MapAnimations.Birds.bird;
					var n = bird.active_tweens.length;
					for (var i = 0; i < n; i++) {
						this.active_tweens[i].stopEnterFrame();
					}
					this.active_tweens = new Array();

					bird.set_x(	bird.position.x - bird.width, 1000 );
					bird.set_y(
						bird.position.y +
							(bird.height >> 1), 1000
					);
					bird.set_height(0, 1000);
					bird.set_width(0, 1000, null,
						function() {
							var _top = MammunUI.get_top_frame();
							_top.MapAnimations.Birds.bird.destroy();
							_top.MapAnimations.Birds.bird = null;
						}
					);
				}
			);
		},
		40000 + Math.random()*300000
	);
}

</script>