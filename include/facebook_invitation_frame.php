<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" xmlns:fb="http://www.facebook.com/2008/fbml">
<body>
<div id="comments_post">
  <form method="post">
	<fb:serverfbml>
	<script type="text/fbml">
    <fb:fbml>
<?
/**
 * @PHIL: Okay, so the action needs to be changed to some kind of .php request,
 * This can be the actual befriending of the two users or simply a confimation
 * box. Or both. The fb:req-choice should point to the URL that the user clicks
 * to be sent DIRECTLY to his island. Here the fancy function you created today
 * can be used. I'm sure there's a way to send it over from invitation2.tmpl.inc
 */
?>
        <fb:request-form
                action="http://apps.facebook.com/atrexos/"
                method="POST"
                invite="true"
                type="My App"
                content="Try out my app!
                        <fb:req-choice url='http://apps.facebook.com/atrexos'
                        label='<?php echo htmlspecialchars("Accept button text",ENT_QUOTES); ?>'
                        />
                " >
                <fb:multi-friend-selector
                        showborder="false"
                        actiontext="Invite your friends to try My App."
                        rows="3"
                />
        </fb:request-form>
    </fb:fbml>
	</script>
	</fb:serverfbml>
  </form>
</div>
<script type="text/javascript">
function update_user_box() 
{
	FB.XFBML.Host.parseDomTree();
}
</script>
<script type="text/javascript"
src="http://static.ak.connect.facebook.com/js/api_lib/v0.4/FeatureLoader.js.php"></script>
<script type="text/javascript">
   FB.init("625457925d3db8f7b0066b5c99333fe3","../include/xd_reciever.html", {"ifUserConnected":update_user_box});
</script>
</body>
</html>
