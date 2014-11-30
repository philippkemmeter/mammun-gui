<? 
/**
 * Die Navigationsleiste eines Wizards.
 */
// normalerweise wird dieses Tmpl von anderen Tmpls aufgerufen, 
// so dass $this_id bereits gesetzt
if (!$this_id)	// wenn aber nicht:	
	$this_id = rand(0,1000000);	// eindeutige ID zuweisen
?>
<? Language::get_instance()->load_language_file('std_buttons') ?>
<div style="clear: both; width: 100%; text-align: center">
<button type="button" id="back<?=$this_id?>" class="UI_wizard_button UI_wizard_buttonLeft"><?= LNG_B_BACK ?></button><button type="button" id="next<?=$this_id?>" class="UI_wizard_button UI_wizard_buttonMiddle"><?= LNG_B_NEXT ?></button><button type="button" id="cancel<?=$this_id?>" class="UI_wizard_button UI_wizard_buttonRight"><?= LNG_B_CANCEL ?></button>
</div>