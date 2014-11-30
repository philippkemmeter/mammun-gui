<?php
$header_add = '<style type="text/css">
	body {
		background-color: #fff;
		color: #333;
		padding: 10px;
	}
</style>
<script language="JavaScript" type="text/javascript">
	function toggleHeight( id ) {
		var e = document.getElementById( id );
		if (e.style.height == \'1.2em\')
			e.style.height = \'auto\';
		else
			e.style.height = \'1.2em\';
	}

	function closeWindow() {
		window.close();
	}
	function submit() {
		opener.location.reload(true);
		return true;
	}
	function new_window(bug_id) {
		var newWindow = window.open("bug.php?id=" + bug_id, "Bug" + bug_id, "width=1000, height=660, scrollbars=yes, resizable=yes, toolbar=no, location=no, directories=no, status=no, menubar=no, copyhistory=no");
		newWindow.focus();
		return;
	}
	function resize_h(id, height) {
		var e = document.getElementById(id);
		var currentHeight = e.style.height;
		var newHeight = height;
		if (currentHeight == height)
			newHeight = \'auto\';
		e.style.height = newHeight;
		return;
	}
	function enableElement( id ) {
		var e = document.getElementById( id );
		if (e.getAttribute(\'disabled\')) {
			e.removeAttribute(\'disabled\');
			e.value=null;
			e.focus();
		}
		else {
			e.setAttribute(\'disabled\', \'disabled\');
			if ( e.value == null )
				e.value = \'' . $bug->joint_id . '\';
		}
		return;
	}
</script>';
/**
 * FIXME opener sollte fixe ID haben, da er sonst auch den vater versuchen
 * könnte zu laden, statt der bugübersicht
 */
$body_add = 'onunload="opener.location.reload(true);"';
?>
<?php
	switch ( $bug->status ) {
		case 2 :
			$bg = '#ff6';
			break;
		case 3 :
			$bg = '#9f0';
			break;
		case 4:
			$bg = '#09f';
			break;
		default :
			$bg = '#fff';
	}
?>

<?php include('templates/header.tmpl.inc')?>
<div style="border: 1px solid #ccc; text-align: right;
	background: <?php echo $bg ?>; padding: 5px;">
	<h2 style="float: left;">Fehlerbericht</h2>
	<a href="javascript:closeWindow()">Fenster Schließen</a>
</div>

<form name="bugfixing" accept-charset="utf8"
	action="<?=$_SERVER['PHP_SELF']?>" method="post">

	<p>
		<label for="status">Status: </label>
		<select name="status" id="status" style="border: 1px solid #ccc;">
			<option value="">- Status -</option>
			<option value="<?=Bugreport::STATE_OPEN?>"<?
				if($bug->status==Bugreport::STATE_OPEN)
					echo ' selected="selected"'
				?>>unbearbeitet</option>
			<option value="<?=Bugreport::STATE_FIXING?>"<?
				if($bug->status==Bugreport::STATE_FIXING)
					echo ' selected="selected"'
				?>>in Bearbeitung</option>
			<option value="<?=Bugreport::STATE_FIXED?>"<?
				if($bug->status==Bugreport::STATE_FIXED)
					echo ' selected="selected"'
				?>>Gefixt</option>
			<option value="<?=Bugreport::STATE_TRASH?>"<?
				if($bug->status==Bugreport::STATE_TRASH)
					echo ' selected="selected"'
				?>>Müll</option>
		</select>

		<label for="bugCategory">Kategorie: </label>
		<select name="bugCategory" id="bugCategory" style="border: 1px solid #ccc">
			<option value="">- Kategorie -</option>
			<option value="<?=Bugreport::CAT_CORE?>"<?
				if($bug->cat_id==Bugreport::CAT_CORE)
					echo ' selected="selected"'
				?>>Core</option>
			<option value="<?=Bugreport::CAT_GUI_JS?>"<?
				if($bug->cat_id==Bugreport::CAT_GUI_JS)
					echo ' selected="selected"'
				?>>GUI Javascript</option>
			<option value="<?=Bugreport::CAT_COMMUNITY?>"<?
				if($bug->cat_id==Bugreport::CAT_COMMUNITY)
					echo ' selected="selected"'
				?>>Community</option>
			<option value="<?=Bugreport::CAT_DESIGN?>"<?
				if($bug->cat_id==Bugreport::CAT_DESIGN)
					echo ' selected="selected"'
				?>>Design</option>
			<option value="<?=Bugreport::CAT_LANGUAGE?>"<?
				if($bug->cat_id==Bugreport::CAT_LANGUAGE)
					echo ' selected="selected"'
				?>>Language</option>
			<option value="<?=Bugreport::CAT_WISHES?>"<?
				if($bug->cat_id==Bugreport::CAT_WISHES)
					echo ' selected="selected"'
				?>>Wishes</option>
		</select>

		<label for="priority">Priorität: </label>
		<select name="priority" id="priority">
			<option value="">- Priorität -</option>
			<option value="<?=Bugreport::PRIORITY_HIGH?>"<?
				if($bug->priority==Bugreport::PRIORITY_HIGH)
					echo ' selected="selected"'
				?>>! Hoch</option>
			<option value="<?=Bugreport::PRIORITY_NORMAL?>"<?
				if($bug->priority==Bugreport::PRIORITY_NORMAL)
					echo ' selected="selected"'
				?>>Normal</option>
			<option value="<?=Bugreport::PRIORITY_LOW?>"<?
				if($bug->priority==Bugreport::PRIORITY_LOW)
					echo ' selected="selected"'
				?>>niedrig</option>
		</select>

		<? for ($i = 1; $i <= $bug->attach_amount; $i++) : ?>
		<img src="bug.php?attachment=<?=$i?>&id=<?=$bug->id?>"
			height="60" width="80"
			style="cursor:pointer"
			onclick="window.open('bug.php?attachment=<?=$i?>&id=<?=$bug->id?>',
				'screenshot<?=$bug->id?>_<?=$i?>');"/>
		<? endfor; ?>
	</p>

	<div style="width: 60%; float: left">
		<h3>Fehler <a onclick="javascript: resize_h('bug_comment', '15em')" style="font-size: 0.8em; font-weight: normal;">[resize]</a></h3>
		<div id="bug_comment" style=" overflow: auto; height: 15em; border: 1px solid #ccc"><?php echo nl2br( strip_tags( $bug->comment ) )?></div>

		<?php /* spacer*/?>
		<div style="clear: both; height: 10px;"></div>

		<h3>Reproduktionsweg <a onclick="javascript: resize_h('bug_repro_desc', '5em')" style="font-size: 0.8em; font-weight: normal;">[resize]</a></h3>
		<div id="bug_repro_desc" style=" overflow: auto; height: 5em; border: 1px solid #ccc"><?php echo nl2br( $bug->repro_desc ) ?></div>
	</div>
	<div style="width: 35%; float: right" >
		<h3>Infos</h3>
		<table cellspacing="1" cellpadding="1" width="100%" style="background-color: #ccc;">
			<tr valign="top">
				<th align="left">Bug_ID</th>
				<td align="left"><?=$bug->id?></td>
			</tr>
			<tr valign="top">
				<th align="left">User_ID</th>
				<td align="left"><?=$bug->user_ID?></td>
			</tr>
			<tr valign="top">
				<th align="left">Name</th>
				<td align="left"><?=$bug->nick?></td>
			</tr>
			<tr valign="top">
				<th align="left">E-Mail</th>
				<td align="left"><a href="mailto:<?php echo $bug->email?>"><?php echo $bug->email?></a></td>
			</tr>
			<tr valign="top">
				<th align="left">Datum</th>
				<td align="left"><?=$bug->datum?></td>
			</tr>
			<tr valign="top">
				<th align="left">Fehlerquelle</th>
				<td align="left"><?=$bug->bug_source?></td>
			</tr>
			<tr valign="top">
				<th align="left">Betriebssystem</th>
				<td align="left"><?=$bug->os?></td>
			</tr>
			<tr valign="top">
				<th align="left">Auflösung</th>
				<td align="left"><?=$bug->resolution?></td>
			</tr>
			<tr valign="top">
				<th align="left">Fenstergröße</th>
				<td align="left"><?=$bug->window_size?></td>
			</tr>
			<tr valign="top">
				<th align="left">Status</th>
				<td align="left"><?=$bug->status?></td>
			</tr>
			<tr valign="top">
				<th align="left">Bearbeitungszeitpunkt</th>
				<td align="left"><?=$bug->fixdate?></td>
			</tr>
		</table>
		<p>Gehört zu Bug: #ID
		<input type="text" maxlength="5" size="5" align="right" name="jointId" id="jointId" onblur="enableElement(this.id)" <?php echo ($bug->joint_id) ? 'disabled="disabled"' : '' ?> value="<?php echo ($bug->joint_id) ? $bug->joint_id : '' ?>" /> <a onclick="enableElement('jointId')" style="font-size: 0.8em">[ändern]</a>
		</p>
		</div>

	<?php /* spacer*/?>
	<div style="clear: both; height: 10px;"></div>

	<div style="width: 60%; float: left">
	<h3>Beschreibung</h3>
		<textarea name="fixcomment" class="Textfield" cols="30" rows="15"
			style="width: 100%; height: 150px; border: 1px solid #ccc"><?php
		echo nl2br($bug->fixcomment)
		?></textarea>
   	<p>
   		<input type="submit" value="senden"/>
   	</p>
   	</div>
   	<div style="float: right; width: 35%">
   		<h3>weitere Meldungen</h3>
   		<table cellspacing="1" cellpadding="2" width="100%" style="background-color: #ccc">
   			<tr valign="top">
   				<th width="1%">ID</th><th>Beschreibung</th>
   			</tr>
   			<?php foreach ( $bug->joint_bugs as $jbug ) : ?>
   			<tr valign="top" style="background-color: #fff;" onclick="toggleHeight('jbug_<?php echo $jbug->id?>')">
   				<td><a href="javascript:new_window(<?php echo $jbug->id?>)"><?php echo $jbug->id ?></a></td>
   				<td><div style="height: 1.2em; overflow: hidden" id="jbug_<?php echo $jbug->id ?>"><?php echo $jbug->comment ?></div></td>
   			</tr>
   			<?php endforeach ?>
   		</table>
   	</div>

	<input type="hidden" name="id" value="<?=$GP['id']?>" />
</form>
<div style="border: 1px solid #ccc; clear: both; text-align: right; background: <?php echo $bg ?>; padding: 5px;"><a href="javascript:closeWindow()">Fenster schließen</a></div>

<?php include('templates/footer.tmpl.inc')?>