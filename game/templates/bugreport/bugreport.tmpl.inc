<?Language::get_instance()->load_language_file("bugreport.tmpl") ?>
<? $this_id = rand(0,1000000);	// eindeutige ID ?>

<script type="text/javscript">
<? include('bugreport_js_decl.tmpl.inc') ?>
</script>

<div id="UI_window_bugreport">
	<div id="bug_feedback<?= $this_id ?>"></div>
	<h3><?= LNG_BUG_DESCRIPTION ?>:</h3>
		<form method="POST"
			name="bugreport_upload<?=$this_id?>"
			action="bugreport.php"
			enctype="multipart/form-data" target="file_upload">

			Screenshots anhängen:<br/>
			<div id="uploaded_file_list<?=$this_id?>">
			</div>
			<input type="hidden" name="this_id" value="<?=$this_id?>">
			<input type="file" name="upload" /> <input type="submit" value="Hochladen"/>
		</form>
		<form name="bugreport<?=$this_id?>" accept-charset="utf-8"
			action="javascript: alert('Fehler...')"
			onsubmit="form_submit<?=$this_id?>(event)">
			<input type="hidden" name="resolution" value=""/>
			<input type="hidden" name="user_id" value=""/>
			<input type="hidden" name="window_size" value=""/>
			<input type="hidden" name="browser" value=""/>
			<input type="hidden" name="os" value=""/>
			<select name="source" >
				<option value="">- <?= LNG_SOURCE_OF_ERROR ?> -</option>
				<option value="1"><?= LNG_CREATE_TRAINING ?> </option>
				<option value="2"><?= LNG_OVERVIEW ?> </option>
				<option value="3"><?= LNG_SCIENCES ?> </option>
				<option value="4"><?= LNG_STATS ?> </option>
				<option value="5"><?= LNG_PAUSE ?> </option>
				<option value="6"><?= LNG_CHAT ?> </option>
				<option value="7"><?= LNG_MESSAGES ?> </option>
				<option value="8"><?= LNG_NOTES ?> </option>
				<option value="9"><?= LNG_HELP ?> </option>
				<option value="10"><?= LNG_TRAIN ?> </option>
				<option value="11"><?= LNG_BUILD ?> </option>
				<option value="12"><?= LNG_DIPLOMACY ?> </option>
				<option value="13"><?= LNG_LANDSCAPE ?> </option>
				<option value="14"><?= LNG_GENERAL ?> </option>
			</select><br/>
           	<textarea name="bug" class="Textfield" cols="5" rows="5"
           		onkeyup="javascript: stop_prop<?=$this_id?>(event)"
           		onkeydown="javascript: stop_prop<?=$this_id?>(event)"></textarea><br/>
           	<input onclick="javascript: document.bugreport<?=$this_id?>.repro.disabled=!this.checked;"
           		type="checkbox" name="reproable"
           		value="1"/><?= LNG_REPRODUCEABLE ?><br/>
			<textarea name="repro" class="Textfield" cols="5" rows="5"
				onkeyup="javascript: stop_prop<?=$this_id?>(event)"
           		onkeydown="javascript: stop_prop<?=$this_id?>(event)"
				disabled></textarea>
     		<p align="center">
     			<input type="reset"
     				class="UI_wizard_button UI_wizard_buttonLeft"
     				onclick="javascript: abort<?=$this_id?>();"
     				value="<?= LNG_CANCEL ?>"/><input type="submit"
     				class="UI_wizard_button UI_wizard_buttonRight"
     				value="<?= LNG_SEND?>"/>
       		</p>
 		</form>
</div>

<script type="text/javscript">
<? include('bugreport_js_action.tmpl.inc') ?>
</script>