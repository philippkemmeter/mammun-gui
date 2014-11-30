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
//TODO Bei Aufruf dieser Datei sollte MammunUI.get_top_frame().MapDataCtrl.movements
// mit den Serverdaten synchronisiert werden
?>
<script type="text/javscript">
top<?= $this_id?> = MammunUI.get_top_frame();
if (!top<?= $this_id?>.MapDataCtrl.movements) {
	top<?= $this_id?>.MapDataCtrl.movements = new Array();
}
if (!top<?= $this_id?>.MapDataCtrl.movements[<?= $start_sector->get_id() ?>])
	top<?= $this_id?>.MapDataCtrl.movements[<?= $start_sector->get_id() ?>]
			= new Array();
<? $i=0; foreach ($moving_units_new as $unit) { ?>
	m<?=$this_id+$i?> = new Array();
	m<?=$this_id+$i?>[0] = '<?= $unit['name'] ?>';
	m<?=$this_id+$i?>[1] = '<?= $unit['nick'] ?>';
	m<?=$this_id+$i?>[2] = <?= $unit['count'] ?>;
	m<?=$this_id+$i?>[3] = <?= $unit['heading'] ?>;
	m<?=$this_id+$i?>[4] = <?= $unit['start_time'] ?>;
	m<?=$this_id+$i?>[5] = <?= $unit['arrival_time'] ?>;
	m<?=$this_id+$i?>[6] = '<?= $unit['path'] ?>';
	m<?=$this_id+$i?>[7] = '<?= $unit['pic_url'] ?>';
top<?= $this_id?>.MapDataCtrl.movements[<?= $start_sector->get_id() ?>]
	[top<?= $this_id?>.MapDataCtrl.movements[<?= $start_sector->get_id() ?>]
		.length] = m<?=$this_id+$i?>;
//	alert(top<?= $this_id?>.MapDataCtrl.movements[<?= $start_sector->get_id() ?>].length);
<? $i++; } ?>
/*for (var i<?=$this_id?> = 0; i<?=$this_id?> < top<?= $this_id?>.MapDataCtrl.movements[<?= $start_sector->get_id() ?>].length; i<?=$this_id?>++) {
	alert(top<?= $this_id?>.MapDataCtrl.movements[<?= $start_sector->get_id() ?>][i<?=$this_id?>]);
}*/
MapDataCtrl.refresh_mammun_map();
</script>
OK