<?php $header_add = <<<HAT
<script type="text/javascript" src="../js/classes/MammunUI.js?'.filemtime('../js/classes/MammunUI.js').'"></script>
<script type="text/javascript" src="../js/PureFW/pure_fw_all.js"></script>
<script type="text/javascript" src="../js/gen_funcs.js"></script>
<script type="text/javascript">
<!--
function signup(ev) {
	try {
		ev = PureFW.EventUtil.formatEvent(ev);
	}
	catch (e) {}
	ev.preventDefault();
	var form_elems = document.forms.signup_form.elements;
	var str = '';
	var n = form_elems.length;
	for (i = 0; i < n; i++) {
		str += form_elems[i].name;
		str += '=';
		if (form_elems[i].type == 'checkbox')
			str += (form_elems[i].checked) ? 1 : 0;
		else
			str += form_elems[i].value.replace(/[#&?=]/g, '+');
		str+='&';
	}
	str += 'lang={$_REQUEST['lang']}';
	PureFW.AJAXClientServer.send_request('{$_SERVER['PHP_SELF']}?'+str, signup_callback);
	return false;
}

function signup_callback(response_arr) {
	if (response_arr[0] == '0') {
		document.setElemInnerHTML('signup_error', response_arr[1]);
	}
	else if (response_arr[0] == '1') {
		alert('Alles klar');
	}
	else {
		alert('Error: ' +response_arr);
	}
}
-->
</script>
HAT;
include (dirname(__FILE__).'/../header.tmpl.inc')?>
<div class="error" id="signup_error"></div>
<br>
<table border=0 cellspacing=0 cellpadding=0 width=800>
<tr valign=top>
 <td class="headCurveLeft"><img class="headCurveLeft" src="<?= pic_path('pattern/spacer.gif') ?>" /></td>
 <td class="head" colspan=5><?= LNG_SIGNUP_FOR_PV ?></td>
 <td class="headCurveRight"><img class="headCurveRight" src="<?= pic_path('pattern/spacer.gif') ?>" /></td>
</tr>
<tr valign="top">
 <td class="footCornLeft"><img class="footCornLeft" src="<?= pic_path('pattern/spacer.gif') ?>" /></td>
 <td class="foot" colspan=5><img class="foot" src="<?= pic_path('pattern/spacer.gif') ?>" /></td>
 <td class="footCornRight"><img class="footCornRight" src="<?= pic_path('pattern/spacer.gif') ?>" /></td>
</tr>

<tr valign="top">
 <td colspan="7"><img src="<?= pic_path('pattern/spacer.gif') ?>" width="1" height="10" /></td>
</tr>

<tr valign="top">
 <td colspan="3" class="borderHorzBot"><img class="borderHorzBot" src="<?= pic_path('pattern/spacer.gif') ?>" /></td>

 <td width=10></td>

 <td colspan="3" class="borderHorzBot"><img class="borderHorzBot" src="<?= pic_path('pattern/spacer.gif') ?>" /></td>
</tr>
<tr valign=top>
 <td class="bodyLeft"><img class="bodyLeft" src="<?= pic_path('pattern/spacer.gif') ?>" /></td>
 <td class="bodyContent">
	I'd like to share a revelation that I've had during my time here.
	It came to me when I tried to classify your species. I realized
	that you're not actually mammals. Every mammal on this planet
	instinctively develops a natural equilibrium with the surrounding
	environment, but you humans do not. You move to an area and you
	multiply and multiply until every natural resource is consumed.
	The only way you can survive is to spread to another area. There
	is another organism on this planet that follows the same pattern.
	Do you know what it is? A virus.<br>
	<br>
	<div align=right><font color=red>Agent Smith, The Matrix.</font></div>
 <td class="bodyRight"><img class="bodyRight" src="<?= pic_path('pattern/spacer.gif') ?>" /></td>

 <td width=10></td>

 <td class="bodyLeft"><img class="bodyLeft" src="<?= pic_path('pattern/spacer.gif') ?>" /></td>
 <td class="bodyContent" style="text-align: left">
 <? if (!$all_done) { ?>
	<?= LNG_SIGNUP_TEXT ?><br>
	<br>
	<form action="js_required.html" onsubmit="javascript: return signup(event);"
		accept-charset="utf-8" name="signup_form">
	<table border=0 cellpadding=4 cellspacing=0 width=370>
	<tr>
	 <td align=right>
	  <?= LNG_NICKNAME ?>:
	 </td>
	 <td>
	  <input name=nick type=text maxlength=30 size=30 value="<?= $nick ?>">
	 </td>
	</tr>
	<tr>
	 <td align=right>
	  <?= LNG_FULLNAME ?>:
	 </td>
	 <td>
	  <input name=firstname type=text maxlength=40 size=30>
	  <input name=surname type=text maxlength=40 size=30>
	 </td>
	</tr>
	<tr>
	 <td align=right>
	  <?= LNG_EMAIL_ADDR ?>:
	 </td>
	 <td>
	  <input name=email type=text size=30 value="<?= $email ?>">
	 </td>
	</tr>
	<tr>
	 <td align=right>
	  <?= LNG_EMAIL_AGAIN ?>:
	 </td>
	 <td>
	  <input name=email2 type=text size=30>
	 </td>
	</tr>
	<tr>
	 <td align=right>
	  <?= LNG_RECRUITED_BY_USER ?>:
	 </td>
	 <td>
	  <? if ($recruit) { ?>
	  <input name=recruit2 disabled type=text size=30 value="<?= $recruit ?>">
	  <input type=hidden name=recruit value="<?= $recruit ?>">
	  <? } else { ?>
	  <input name=recruit2 type=text size=30 value="<?= $recruit2 ?>">
	  <? } ?>
	 </td>
	</tr>
	<tr>
	 <td align=right colspan=2>
	  <input type=checkbox name=agb_accepted>&nbsp;<?= LNG_AGB_TEXT ?><br>
	  <br>
	  <input type=hidden name=vip value=>
	  <input type=hidden name=lang value=<?= $language ?>>
	  <input type="text" name="map_key" value="<?= @$_SESSION['map_key'] ?>" />
	  <input type="text" name="visit" value="<?= @$_SESSION['visit'] ?>" />
	  <input class=button type=submit name=submitok value='   OK   '>
	 </td>
	</tr>
	</table>
	</form>
 <? } else { ?>
 	<table border=0 cellpadding=4 cellspacing=0 width=370>
	<tr>
	 <td align=center>
	  <br>
	  <br>
	  <br>
	  <br>

	  <br>
	  <br>
	  <br>
	  <br>
	 </td>
 	</tr>
 	</table>
 <? } ?>

 <td class="bodyRight"><img class="bodyRight" src="<?= pic_path('pattern/spacer.gif') ?>" /></td>
</tr>
<tr valign="top">
 <td class="footCurveLeft"><img class="footCurveLeft" src="<?= pic_path('pattern/spacer.gif') ?>" /></td>
 <td class="foot"><img class="foot" src="<?= pic_path('pattern/spacer.gif') ?>" /></td>
 <td class="footCornRight"><img class="footCornRight" src="<?= pic_path('pattern/spacer.gif') ?>" /></td>

 <td width=10></td>

 <td class="footCornLeft"><img class="footCornLeft" src="<?= pic_path('pattern/spacer.gif') ?>" /></td>
 <td class="foot"><img class="foot" src="<?= pic_path('pattern/spacer.gif') ?>" /></td>
 <td class="footCurveRight"><img class="footCurveRight" src="<?= pic_path('pattern/spacer.gif') ?>" /></td>
</tr>
</table>
<?php include (dirname(__FILE__).'/../footer.tmpl.inc')?>