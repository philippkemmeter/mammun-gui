<?php
$header_add = '<style type="text/css">
	body {
		background-color: #fff;
		color: #333;
		padding: 10px;
	}
</style>
<script language="JavaScript" type="text/javascript">
	function toggleDisplay( id ) {
		var e = document.getElementById( id );
		if (e.style.display == \'none\')
			e.style.display = \'\';
		else
			e.style.display = \'none\';
	}

	function new_window(bug_id) {
		javascript: var x=window.open("bug.php?id=" + bug_id, "Bug" + bug_id, "width=1000, height=660, scrollbars=yes, resizable=yes, toolbar=no, location=no, directories=no, status=no, menubar=no, copyhistory=no");
	}

</script>';
?>
<?php include('templates/header.tmpl.inc')?>
<?php
	if ( isset( $_GET[ 'onlyUserId' ] ) ) {
		$tmpArray = array();
		foreach ($bugreports as $report) {
			if ( $report->user_ID == $_GET[ 'onlyUserId' ] )
				$tmpArray[] = $report;
		}
		$bugreports = $tmpArray;
	}
?>
<h2>Alle Fehlerberichte <?php echo ( isset( $_GET[ 'onlyUserId' ] ) ) ? '(von ' . $_GET[ 'onlyUserId' ] .')' : ''?></h2>
<?php if ( isset( $_GET[ 'onlyUserId' ] ) ) :?>
<a href="<?=$_SERVER['PHP_SELF']?>">alle Anzeigen</a>
<?php endif ?>

<p><em>Details zu einzelnen Bugs können per Klick auf die Tabellenreihe des Bugs angezeigt werden lassen</em></p>
<table border="0" cellspacing="1" cellpadding="1">
	<tr>
		<td>
			<table border="0" cellspacing="1" cellpadding="1">
				<tr style="cursor: pointer;"
					onclick="document.location='<?=$_SERVER['PHP_SELF']?>';">
					<td colspan="2"><em>Alle (<?=array_sum($bugamounts)?>)</em></td>
				</tr>
				<tr style="cursor: pointer;"
					onclick="document.location='<?=$_SERVER['PHP_SELF']?>?filter_status=1';">
					<td style="border: 1px solid #ccc">&nbsp;</td><td><em>Unberührt (<?=$bugamounts[1]?>)</em></td>
				</tr>
				<tr style="cursor: pointer;"
					onclick="document.location='<?=$_SERVER['PHP_SELF']?>?filter_status=2';">
					<td style="background: #ff6; border: 1px solid #ccc" width="15">&nbsp;</td><td><em>In Bearbeitung (<?=$bugamounts[2]?>)</em></td>
				</tr>
				<tr style="cursor: pointer;"
					onclick="document.location='<?=$_SERVER['PHP_SELF']?>?filter_status=3';">
					<td style="background: #9f0; border: 1px solid #ccc">&nbsp;</td><td><em>gefixt (<?=$bugamounts[3]?>)</em></td>
				</tr>
				<tr style="cursor: pointer;"
					onclick="document.location='<?=$_SERVER['PHP_SELF']?>?filter_status=4';">
					<td style="background: #09f; border: 1px solid #ccc">&nbsp;</td><td><em>Müll (<?=$bugamounts[4]?>)</em></td>
				</tr>
			</table>
		</td>
		<td>
			<form method="POST" action="">
			<select name="filter_cat_id" id="filter_cat_id">
			<option value="<?= Bugreport::CAT_UNKNOWN?>"
				>Alle Kategorien anzeigen</option>
			<option value="<?= Bugreport::CAT_CORE?>"<?
				if($GP['filter_cat_id']==Bugreport::CAT_CORE)
					echo ' selected="selected"'
				?>>Core</option>
			<option value="<?= Bugreport::CAT_GUI_JS?>"<?
				if($GP['filter_cat_id']==Bugreport::CAT_GUI_JS)
					echo ' selected="selected"'
				?>>GUI Javascript</option>
			<option value="<?= Bugreport::CAT_COMMUNITY?>"<?
				if($GP['filter_cat_id']==Bugreport::CAT_COMMUNITY)
					echo ' selected="selected"'
				?>>Community</option>
			<option value="<?= Bugreport::CAT_DESIGN?><?
				if($GP['filter_cat_id']==Bugreport::CAT_DESIGN)
					echo ' selected="selected"'
				?>">Design</option>
			<option value="<?= Bugreport::CAT_LANGUAGE?><?
				if($GP['filter_cat_id']==Bugreport::CAT_LANGUAGE)
					echo ' selected="selected"'
				?>">Language</option>
			<option value="<?= Bugreport::CAT_WISHES?><?
				if($GP['filter_cat_id']==Bugreport::CAT_WISHES)
					echo ' selected="selected"'
				?>">Wishes</option>
			</select>
			<select name="filter_priority" id="filter_priority">
			<option value="<?= Bugreport::PRIORITY_UNKNOWN?>"
				>Alle Prioritäten anzeigen</option>
			<option value="<?= Bugreport::PRIORITY_HIGH?>"<?
				if ($GP['filter_priority']==Bugreport::PRIORITY_HIGH)
					echo ' selected="selected"'
				?>>! Hoch</option>
			<option value="<?= Bugreport::PRIORITY_NORMAL?>"<?
				if ($GP['filter_priority']==Bugreport::PRIORITY_NORMAL)
					echo ' selected="selected"'
				?>>Normal</option>
			<option value="<?= Bugreport::PRIORITY_LOW?>"<?
				if ($GP['filter_priority']==Bugreport::PRIORITY_LOW)
					echo ' selected="selected"'
				?>>niedrig</option>
			</select>
			<input type="submit" value="Filter">
			</form>
		</td>
	</tr>
</table>
<br/>

<table border="0" cellspacing="1" cellpadding="1" width="100%" style="background-color: #ccc;">
	<tr>
		<th align="center">Bug_ID</th>
		<th align="left" width="1%">User_ID</th>
		<th align="center">Datum</th>
		<th align="left">Fehler</th>
		<th align="center">Bearbeitungszeitpunkt</th>
	</tr>
<?php $oddEven = false ?>
<?php foreach ($bugreports as $bug) :?>

<?php
	/**
	 * Setzt den Hintergrund für verschiedene Status
	 */
	switch ( $bug->status ) {
		case 2 :
			$bg = '#ff6';
			break;
		case 3 :
			$bg = '#9f0';
			break;
		case 4 :
			$bg = '#09f';
			break;
		default :
			if ( $oddEven ) {
				$bg = '#ddd';
			}
			else {
				$bg = '#fff';
			}
	}
?>
	<?php
	/**
	 * Reports werden in einzelne worte zerlegt und diese auf auf Überlänge geprüft.
	 * Ist ein String zu lang, wird dieser verkürzt.
	 */

	$string = explode(' ', $bug->comment);
	for ($si = 0; $si < count($string); $si++ ) {
		$string[$i] = strip_tags($string[$si]);
		if (strlen($string[$si]) > 70) {
			$string[$si] = substr($string[$si], 0, 70)
				. ' [<span title="' . $string[$si]
				. '" onclick="alert(\'' . $string[$si]
				. '\'); return false">...</span>]';
		}
	}
	$bug->comment = implode(' ', $string);
	$bug->comment = ($bug->comment != '') ? nl2br( $bug->comment ) : nl2br( $bug->repro_desc );
	//print_r($string);

	?>
	<tr style="background-color: <?php echo $bg ?>" onclick="toggleDisplay('bugdetails_<?php echo $bug->id ?>')" valign="top">
		<td align="center">
			<a href="javascript:new_window(<?php echo $bug->id?>)" title="nur Berichte von diesem User"><?= $bug->id ?></a>
			<?php if ( count( $bug->joint_bugs ) ) : ?>
			<br/>(+<?php echo count( $bug->joint_bugs ) ?>)
			<?php endif ?>
		</td>
		<td align="center"><a href="<?= $_SERVER[ 'PHP_SELF' ] ?>?onlyUserId=<?= $bug->user_ID ?>"><?= $bug->user_ID ?></a></td>
		<td align="center"><?= $bug->datum ?></td>
		<td align="left"><?= $bug->comment ?></td>
		<td align="center"><?= $bug->fixdate ?></td>
	</tr>
	<tr id="bugdetails_<?php echo $bug->id ?>" style="display: none;">
		<td colspan="6">
			<table cellspacing="0" cellpadding="1" style="background-color: #ff9" width="100%">
				<tr valign="top">
					<th align="left">Fehlerquelle</th>
					<td align="left" valign="top"><?= $bug->bug_source ?></td>
				</tr>
				<tr valign="top">
					<th align="left">Name</th>
					<td align="left" ><?= $bug->nick ?></td>
				</tr>
				<tr valign="top">
					<th align="left">E-Mail-Adresse</th>
					<td align="left" ><?= $bug->email ?></td>
				</tr>
				<tr valign="top">
					<th align="left">Reproduktionsweg</th>
					<td align="left" valign="top"><?= nl2br($bug->repro_desc) ?></td>
				</tr>
				<tr valign="top">
					<th align="left">Betriebssystem</th>
					<td align="left" valign="top"><?= $bug->os ?></td>
				</tr>
				<tr valign="top">
					<th align="left">Browser</th>
					<td align="left" valign="top"><?= $bug->browser ?></td>
				</tr>
				<tr valign="top">
					<th align="left">Auflösung</th>
					<td align="left" valign="top"><?= $bug->resolution ?></td>
				</tr>
				<tr valign="top">
					<th align="left">Fenstergröße</th>
					<td align="left" valign="top"><?= $bug->window_size ?></td>
				</tr>
				<tr valign="top">
					<th align="left">Bearbeitungskommentar</th>
					<td align="left" valign="top"><?= nl2br( strip_tags( $bug->fixcomment ) ) ?></td>
				</tr>
			</table>
		</td>
	</tr>
	<?php $oddEven = !$oddEven ?>
<?php endforeach;?>
</table>



<?php include('templates/footer.tmpl.inc')?>