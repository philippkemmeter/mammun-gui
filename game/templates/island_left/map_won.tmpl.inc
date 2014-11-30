<? include ('island_left_general.tmpl.inc')
?>
<script type="text/javascript">

IslandLeft.facebook_brag = function()
{
	var attachment = {
      "name" : '<?=LNG_ISLAND_CONQUERED?>',
      "caption" : '<?=LNG_ISLAND_CONQUERED_DESC?>'
	}
	if (MammunUI.get_top_frame().FB)
		MammunUI.get_top_frame().FB.Connect.streamPublish("", attachment);
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
// should use the below function if it works. If the reader doesn't understand
// what it's for, delete the entire comment block.
//MammunUI.facebook_publish_prompt('Island Conquered', '{*actor*} just conquered '
//	+'an island in Mammun! Who wants to join in the next round?');

IslandLeft.facebook_brag();
</script>