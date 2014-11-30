<script type="text/javascript">
InsertPersonalData = new Object();

InsertPersonalData.form_submit = function(ev) {
	if (!document.forms.InsertPersonalDataFrom.salutation) {
		
	}
}
</script>

<form action="javascript: alert('fehler');" name="InsertPersonalDataFrom"
onsubmit="javascript: InsertPersonalData.form_submit(event)">
<table border="0" cellpadding="0" cellspacing="2" width="80%">
<tr>
	<td width="50%" valign="top"><?= LNG_SALUTATION ?>, <?= LNG_TITLE ?></td>
	<td width="50%" valign="top">
		<select name="salutation" style="width: 40%; font-size: 1em">
			<option value="0">--<?=LNG_CHOOSE_SALUTATION?>--</option>
			<option value="<?=LNG_MISTER?>"><?=LNG_MISTER?></option>
	<? if ($language != 'de') : ?>
			<option value="<?=LNG_MISS?>"><?=LNG_MISS?></option>
	<? endif; ?>
			<option value="<?=LNG_MISSES?>"><?=LNG_MISSES?></option>
		</select>
		<input type="text" name="title" value="<?=$personal_data->title?>" 
			 style="width: 60%; font-size: 1em"/>
	</td>
</tr>
<tr>
	<td valign="top"><?= LNG_FIRSTNAMES?>, <?= LNG_SURNAMES ?></td>
	<td valign="top">
		<input type="text" name="firstnames" 
			value="<?=$personal_data->firstnames?>" 
			style="width: 50%;font-size:1em"/>
		<input type="text" name="surnames"
			value="<?=$personal_data->surnames?>" 
			style="width: 50%;font-size:1em"/>
	</td>
</tr>
<tr>
	<td valign="top"><?= LNG_STREET ?>, <?= LNG_STREET_NUMBER ?></td>
	<td valign="top">
		<input type="text" name="street" value="<?=$personal_data->street ?>" 
			style="width: 80%;font-size:1em"/>
		<input type="text" name="street" 
			value="<?=$personal_data->street_number ?>" 
			style="width: 20%;font-size:1em"/>
	</td>
</tr>
<tr>
	<td valign="top"><?= LNG_ZIPCODE ?>, <?= LNG_CITY ?></td>
	<td valign="top">
		<input type="text" name="zipcode" value="<?=$personal_data->zipcode ?>"
			style="width: 30%;font-size:1em"/>
		<input type="text" name="city" 
			value="<?=$personal_data->city ?>" 
			style="width: 70%;font-size:1em"/>
	</td>
</tr>
<tr>
	<td valign="top"><?= LNG_COUNTRY ?></td>
	<td valign="top">
		<input type="text" name="country" value="<?=$personal_data->country ?>"
			style="width: 100%;font-size:1em"/>
	</td>
</tr>
<tr>
	<td valign="top"><?= LNG_STATE ?></td>
	<td valign="top">
		<input type="text" name="state" value="<?=$personal_data->state ?>"
			style="width: 100%;font-size:1em"/>
	</td>
</tr>
<tr>
	<td valign="top"><?= LNG_BIRTHDAY ?></td>
	<td valign="top">
		<input type="text" name="birthday" 
			value="<?=$personal_data->birthday ?>"
			style="width: 100%;font-size:1em"/>
	</td>
</tr>
</table>
<input type="submit" value="OK" name="submitok" />
</form>

<script type="text/javascript">
</script>