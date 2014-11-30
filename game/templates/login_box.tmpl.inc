<?
include_once('../include/shared/classes/Time.inc');
include_once('../include/shared/classes/Language.inc');
include_once('../include/shared/classes/PV_encryption.inc');
Language::get_instance()->load_language_file('login_box.tmpl');
$zahl = rand(1000,9999);
if (!isset($cryptObj))
	$cryptObj = new PV_encryption();
?>
<script type="text/javascript">
DocumentLoadListener.add(function(){
	PureFW.EventUtil.add_event_handler(
			document.loginform,"submit",login_send_request);
	});
function login_send_request(ev) {
	try {
		ev = PureFW.EventUtil.formatEvent(ev);
	}
	catch (e){
	}
	ev.preventDefault();

	PureFW.AJAXClientServer.send_request('sync_login.php?nick='
		+ document.loginform.nick.value
		+ '&pass=' + document.loginform.pass.value
<?/*		+ '&input_pin=' + document.loginform.input_pin.value */?>
		+ '&dataY=' + document.loginform.dataY.value
		+ '&lang='+document.loginform.lang.value,

		login_fetch_response
	);
	document.loginform.submitk.focus();
}

function login_fetch_response(response) {
	if (!response) {
		// Relogin
		alert('<?= LNG_ERR_WRONG_USER_DATA_OR_CAPTCHA ?>');
	}
	else if (response[0] == 1) {
		try {
			var sid = PureFW.AJAXClientServer.session_name
				+ "=" + PureFW.AJAXClientServer.session_id;
			document.location.href="ingame.php?<?=rand() ?>&"+sid
				+ '&lang='+document.loginform.lang.value;
		}
		catch (e) {
			alert(response);
		}
	}
	else {
		alert(response);
	}
}
</script>

<div id="login_error" style="position: absolute; left:0; top:0; width: 200px;background-color: #ff0000">
	<h1><? if (isset($reasonerror)) echo $reasonerror ?></h1>
	<? if (isset($reasontext)) echo $reasontext ?>
</div>
<div id="loginBox">
	<form action="" accept-charset="utf-8" name="loginform">
		<label for="nick"><?= LNG_NICK_NAME ?></label>
		<input name="nick" id="nick" type="text" maxlength="30"
			size="15" value="" /><br/>

		<label for="pass"><?= LNG_PASSWORD ?></label>
		<input name="pass" id="pass" type="password"
			size="15" maxlength="20" value="" /><br/>

<?/*		<label for="input_pin">Captcha</label>
		<img id="pin"
			src="pin_wrapper.php?dataY=<?= $cryptObj->do_encryption(array('zahl'=>$zahl),0) ?>" />
		<input name="input_pin" id="input_pin"
			type="text" maxlength="4" size="5" /><br/>*/?>

		<input type="hidden" name="dataY"
			value="<?= (isset($hidden_field)?$cryptObj->do_encryption(array('pin_number'=>$zahl,'login'=>1,'pin_time'=>Time::get_t_now(),$hidden_field['key']=>$hidden_field['value']),0):$cryptObj->do_encryption(array('pin_number'=>$zahl,'login'=>1,'pin_time'=>Time::get_t_now()),0)) ?>" />
		<input class="UI_button UI_button_single" type="submit"
			name="submitk"
			value="   <?= LNG_LOGIN ?>   ">
		<select name="lang">
			<option value="de">Deutsch</option>
			<option value="en">English</option>
		</select>
	</form>
	<div style="text-align: right">Version: <?= @VERSION ?> (Beta)</div>
</div>