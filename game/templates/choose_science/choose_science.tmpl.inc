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

<script type="text/javscript">
<? include('choose_science_js_decl.tmpl.inc') ?>
</script>

<? /* Hier werden die wählbaren Forschungen eingehängt */ ?>

<div id="science_choose_board" style="position: absolute; top:0; left:0; height: 300px; width:99%; border: 1px #ddd solid;">
Choose! <span id="scs_left"><?=$scs_left?></span> left.
<div id="science_choose_board_general" style="height: 50px; clear: both;"></div>
<div id="science_choose_board_races" style="height: 50px; clear: both;"></div>
<div id="science_choose_board_abilities" style="height: 50px; clear: both;"></div>
<div id="science_choose_board_buildings" style="height: 50px; clear: both;"></div>
<div id="science_choose_board_artifacts" style="height: 50px; clear: both;"></div>
</div>
<div id="sciences_chosen" style="position: absolute; top:325px; left:0; width:99%; height:100px;border: 1px #ddd solid;">
</div>

<button type="button" onclick="javascript: finish<?=$this_id?>();" id="science_choose_okay" 
 style="position: absolute; top: 150px; left: 46%; width: 8%; visibility: hidden">OK</button>

<script type="text/javscript">
<? include ('choose_science_js_actions.tmpl.inc'); ?>
</script>