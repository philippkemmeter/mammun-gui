<?php //TODO Dieses Template ist noch zu gestalten :)?>
<?php include('header.tmpl.inc') ?>
<a href="javascript: if(confirm('This will reset ALL ACCOUNTS!')) document.location.href='<?=$_SERVER['PHP_SELF']?>?reset=1';">RESET ALL ACCOUNTS</a><br/>
<a href="fight_simulator.php">Fight Simulator</a><br/>
<a href="<?=$_SERVER['PHP_SELF']?>?gen_science_data=1">Print generate_science_data</a>
<a href="<?=$_SERVER['PHP_SELF']?>?get_user_statistics=1">Print User Statistics</a>
<?php include('footer.tmpl.inc') ?>