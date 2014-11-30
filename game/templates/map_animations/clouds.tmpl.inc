<script type="text/javascript">
if (typeof(MapAnimations) == 'undefined')
	MapAnimations = new Object();

MapAnimations.Clouds = new Object();
MapAnimations.Clouds.start = function() {
	MapAnimations.Clouds.clouds1 = new ParticleSys(
		function() {
			return -200;
		},
		function() {
			return Math.random() * 200;
		},
		function() {
			return (Math.random() + 0.5) / 2;
		},
		function() {
			return Math.random() / 8;
		},
		function() {
			return Math.floor(Math.random()*50) + 1000;
		}
	);
	MapAnimations.Clouds.clouds1.add_particle(
		new PureFW.Image(
			(MapUI.no_iframes) ? document.getElemById("mapx")
				: MammunUI.get_top_frame().mapx.document.body,
			0, 0,
			400, 335,
			'ui/elements/clouds/cloud01.png',
			true
		)
	).widget.set_z_index(50);

	MapAnimations.Clouds.clouds1.add_particle(
		new PureFW.Image(
			(MapUI.no_iframes) ? document.getElemById("mapx")
				: MammunUI.get_top_frame().mapx.document.body,
			0, 0,
			400, 254,
			'ui/elements/clouds/cloud02.png',
			true
		)
	).widget.set_z_index(50);

	PureFW.Timeout.set_timeout(function()
		{
			MapAnimations.Clouds.clouds1.add_particle(
				new PureFW.Image(
					(MapUI.no_iframes) ? document.getElemById("mapx")
						: MammunUI.get_top_frame().mapx.document.body,
					0, 0,
					300, 300,
					'ui/elements/clouds/cloud03.png',
					true
				)
			).widget.set_z_index(50);
		}, 3000
	);

/*
	MapAnimations.Clouds.clouds2 = new ParticleSys(
		null,
		function() {
			return Math.random() * 200 + 900;
		},
		function() {
			return (Math.random() + 0.5) / 2;
		},
		function() {
			return (Math.random()*-1 - 0.5) / 2;
		},
		function() {
			return Math.floor(Math.random()*50) + 80;
		}
	);
	MapAnimations.Clouds.clouds2.add_particle(
		new PureFW.Image(
			(MapUI.no_iframes) ? document.getElemById("mapx")
				: MammunUI.get_top_frame().mapx.document.body,
			0, 0,
			300, 300,
			'ui/elements/clouds/cloud03.png',
			true
		)
	).widget.set_z_index(50);
	PureFW.Timeout.set_timeout(function()
		{
			MapAnimations.Clouds.clouds2.add_particle(
				new PureFW.Image(
					(MapUI.no_iframes) ? document.getElemById("mapx")
						: MammunUI.get_top_frame().mapx.document.body,
					0, 0,
					400, 335,
					'ui/elements/clouds/cloud01.png',
					true
				)
			).widget.set_z_index(50);
			MapAnimations.Clouds.clouds2.add_particle(
				new PureFW.Image(
					(MapUI.no_iframes) ? document.getElemById("mapx")
						: MammunUI.get_top_frame().mapx.document.body,
					0, 0,
					400, 254,
					'ui/elements/clouds/cloud02.png',
					true
				)
			).widget.set_z_index(50);
		}, 2000
	);
*/

	MapAnimations.Clouds.clouds3 = new ParticleSys(
		function() {
			return Math.random() * 100 + 1200;
		},
		function() {
			return Math.random() * 200;
		},
		function() {
			return (Math.random() + 0.5) / 2;
		},
		function() {
			return (Math.random()*2 - 1) / 2;
		},
		function() {
			return Math.floor(Math.random()*50) + 100;
		}
	);
	MapAnimations.Clouds.clouds3.add_particle(
		new PureFW.Image(
			(MapUI.no_iframes) ? document.getElemById("mapx")
				: MammunUI.get_top_frame().mapx.document.body,
			0, 0,
			300, 300,
			'ui/elements/clouds/cloud03.png',
			true
		)
	).widget.set_z_index(50);
	PureFW.Timeout.set_timeout(function()
		{
	/*		MapAnimations.Clouds.clouds3.add_particle(
				new PureFW.Image(
					(MapUI.no_iframes) ? document.getElemById("mapx")
						: MammunUI.get_top_frame().mapx.document.body,
					0, 0,
					400, 335,
					'ui/elements/clouds/cloud01.png',
					true
				)
			).widget.set_z_index(50);*/
			MapAnimations.Clouds.clouds3.add_particle(
				new PureFW.Image(
					(MapUI.no_iframes) ? document.getElemById("mapx")
						: MammunUI.get_top_frame().mapx.document.body,
					0, 0,
					400, 254,
					'ui/elements/clouds/cloud02.png',
					true
				)
			).widget.set_z_index(50);
		}, 2000
	);


/*
	MapAnimations.Clouds.clouds4 = new ParticleSys(
		function() {
			return 1450;
		},
		function() {
			return 1000;
		},
		function() {
			return (Math.random() + 0.5) / 4;
		},
		function() {
			return (Math.random()*-1 - 0.5) / 4;
		},
		function() {
			return Math.floor(Math.random()*50) + 90;
		}
	);
	MapAnimations.Clouds.clouds4.add_particle(
		new PureFW.Image(
			(MapUI.no_iframes) ? document.getElemById("mapx")
				: MammunUI.get_top_frame().mapx.document.body,
			0, 0,
			300, 300,
			'ui/elements/clouds/cloud03.png',
			true
		)
	).widget.set_z_index(50);
	PureFW.Timeout.set_timeout(function()
		{
			MapAnimations.Clouds.clouds4.add_particle(
				new PureFW.Image(
					(MapUI.no_iframes) ? document.getElemById("mapx")
						: MammunUI.get_top_frame().mapx.document.body,
					0, 0,
					400, 335,
					'ui/elements/clouds/cloud01.png',
					true
				)
			).widget.set_z_index(50);
			MapAnimations.Clouds.clouds4.add_particle(
				new PureFW.Image(
					(MapUI.no_iframes) ? document.getElemById("mapx")
						: MammunUI.get_top_frame().mapx.document.body,
					0, 0,
					400, 254,
					'ui/elements/clouds/cloud02.png',
					true
				)
			).widget.set_z_index(50);
		}, 5000
	);
*/
	ParticleSys.start();
}
</script>