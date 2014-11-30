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

<?/*<form action="javascript: MammunUI.get_top_frame().load_moving_final_dialog(document.forms.choose_amount<?= $this_win_id?>.elements, <?= $this_win_id ?>)"
	name="choose_amount<?= $this_win_id ?>">*/?>

<script type="text/javscript">
<? include ('choose_amount_js_decl.tmpl.inc'); ?>
</script>
<form action="" name="choose_amount<?=$this_id?>">
<table border="0" width="90%" cellpadding="0" cellspacing="0">
<? $i=0; foreach ($moving_tribes as $tribe) { ?>
<tr>
 <td><img src="<?= pic_path($tribe->get_pic_url()) ?>" width="20" height="20" /></td>
 <td><?= $tribe->get_name() ?></td>
 <td><input type="text" name="count<?=$i?>" style="width: 40px;" value="1"/>
     <input type="hidden" name="id<?=$i?>" value="<?= $cryptObj->do_encryption(array('id'.$i => $tribe->get_id())) ?>" />
 </td>
</tr>
<? $i++;} ?>
</table>
<input type="hidden" name="dataX" value="<?= $cryptObj->do_encryption(array('path' => $GP['path'])) ?>" />
<!--  <input type="submit" value="   OK   "/>-->
<div style="text-align: right; width: 90%">
	<button type="button" onclick="confirm_amount<?=$this_id?>();">  OK  </button>
</div>
</form>