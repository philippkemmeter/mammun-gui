<?
//TODO Kann man später wieder durch festen Wert ersetzen. So ist es übersichtlicher ;)
function newWindowParams () {
	$params = array ();
	$params [] = 'width=1000';
	$params [] = 'height=660';
	$params [] = 'scrollbars=no';
	$params [] = 'resizable=no';
	$params [] = 'toolbar=no';
	$params [] = 'location=no';
	$params [] = 'directories=no';
	$params [] = 'status=no';
	$params [] = 'menubar=no';
	$params [] = 'copyhistory=no';

	return implode (', ', $params);
}

$body_add = 'onload="javascript: start_game()"';
$header_add =
'<script type="text/javascript">
function start_game() {
	var x=window.open("ingame.php?start=true&'.session_name().
		'='.session_id().'", "Mammun", "'.newWindowParams().'");
}
</script>';
?>

<? include('header.tmpl.inc') ?>

<p>
	<a href="javascript: start_game()" style="font-size: 50px;
	color: #fff">START</a>
</p>
<p style="color: #fff">
	Ein Popup mit dem Spiel sollte sich öffnen.
	Wenn nicht, dann bitte auf START klicken oder Popup-Blocker deaktivieren.
</p>
<? include('footer.tmpl.inc') ?>
