<!-- Welcome text  /  Login -->
   
<table border=0 cellspacing=0 cellpadding=0 width=800 style="margin-bottom: 10px;">
<tr valign=top>
 <td class="headCurveLeft"><img class="headCurveLeft" src="<?= pic_path('pattern/spacer.gif') ?>" /></td> 
 <td class="head"><?= LNG_WELCOME_TO_PV ?></td>
 <td class="headCurveRight"><img class="headCurveRight" src="<?= pic_path('pattern/spacer.gif') ?>" /></td>
</tr>
<tr valign=top>
 <td class="bodyLeft"><img class="bodyLeft" src="<?= pic_path('pattern/spacer.gif') ?>" /></td>
 <td class="bodyContent">
  <table border=0 cellspacing=0 cellpadding=0 width=100%>
  <tr>
   <td valign=middle align=right>
     <form 'action="sync_login.php" target="_self"' method="post">
     <table border="0" cellspacing="5" cellpadding="0" width=300>
      <tr>
       <td style="line-height: 17px"><?= LNG_USER_NAME ?>:</td>
       <td colspan="3" valign="bottom" width=250><input type="text" size="20" style="width: 95%;" class="login" maxlength="50" name="<?= (defined('NO_COMMUNITY')) ? 'nick' : 'username' ?>" value="" tabindex="1"></td>
      </tr>
      <tr>
       <td style="line-height: 17px"><?= LNG_PASSWORD ?>:<br>(<a href="password_forgotten.php?lang=<?= $lang ?>" target="help" tabindex="5"><?= LNG_FORGOTTEN ?>?</a>)</td>
       <td colspan="3" valign="bottom" width=250><input type="password" class="login" size="20" style="width: 95%;" maxlength="50" name="<?= (defined('NO_COMMUNITY')) ? 'pass' : 'password' ?>" value="" tabindex="2"></td>
      </tr>
      <tr>
       <td style="line-height: 17px"><?= Tooltip::parse("ABN", "ABN") ?>:</td>
       <td valign="bottom" width=50><input type"text" maxlength="4" size="5" style="width: 95%;" name="input_pin" tabindex="3"></td>
       <td align=center><nobr>&nbsp;<img src="pin.php?dataY=<?= $cryptObj->do_encryption(array('zahl'=>$zahl),0) ?>"></nobr></td>   
       <td valign="bottom"><input type="submit" size="20" value="Login" class="button" tabindex="4"></td>
      </tr>
     </table>
     <input type=hidden name="dataY" value="<?= $cryptObj->do_encryption(array('pin_number'=>$zahl,'login'=>1,'pin_time'=>$t_now),0) ?>">
     <input type=hidden name="lang" value="<?= $lang ?>">
     </form>
   </td>
   
   <td align=right valign=bottom>
   <img id="officer_pic_big" style="position: relative; left: 9px; top: 12px" width=310 height=405 src="<?= pic_path('chief_warfare_bigger.png') ?>">
   
   <script language="javascript">
   <!--
   var browser=navigator.appName;
   if (browser.indexOf('Explorer') != -1 && !window.XMLHttpRequest) {	// IE < 7 doesn't handle alpha transparence at all
   	 getElem('officer_pic_big').src = '<?= pic_path('chief_warfare_bigger.jpg') ?>';
   	 getElem('officer_pic_big').style.top = '15px';
   }
   -->
   </script>
   </td>
  </tr>
  </table>
 </td>
 <td class="bodyRight"><img class="bodyRight" src="<?= pic_path('pattern/spacer.gif') ?>" /></td>
</tr>
<tr valign=top>
 <td class="footCornLeft"><img class="footCornLeft" src="<?= pic_path('pattern/spacer.gif') ?>" /></td> 
 <td class="foot"><img class="foot" src="<?= pic_path('pattern/spacer.gif') ?>" /></td>
 <td class="footCornRight"><img class="footCornRight" src="<?= pic_path('pattern/spacer.gif') ?>" /></td>
</tr>
<?php include_once('templates/footer.tmpl.inc')?>