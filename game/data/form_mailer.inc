<?php
	require_once('../include/std_include.inc');
	
	$expected_data = array
	(
		'user_id'		=> 	'int',
		'browser'		=>	'string',	
		'os'			=>	'string',
		'resolution'	=>	'string',
		'window_size'	=>	'string',
		'source'		=>	'int',
		'repro'			=>	'string',
		'bug'			=>	'string',
		'delimiter'		=>	'string',
		'mailto'		=>	'string',
		'return'		=>	'string',
		'subject'		=>	'string',
	);
		       			
	$GP=$cryptObj->load_GET_POST($expected_data);

	$delimiter = $GP["delimiter"];
	$mailto = $GP["mailto"];
	$returnhtml = $GP["return"];
	$subject = $GP["subject"];
	if(isset($GP['bug'])) 
	{
    	$sql =	"INSERT INTO bugreports
				(user_ID, browser, os, resolution, window_size, bug_source, repro_desc, comment, datum)
            	VALUES
                ('".$GP['user_id']."',
                '".$GP['browser']."',
				'".$GP['os']."',
				'".$GP['resolution']."',
				'".$GP['window_size']."',
				'".$GP['source']."',
				'".$GP['repro']."',
                '".$GP['bug']."',
                NOW())";

    	query($sql) OR die(mysql_error());
    	$stringposition = strpos ( $_SERVER['REQUEST_URI'], 'form_mailer.php' );
		$path = substr($_SERVER['REQUEST_URI'],0,$stringposition) ;
		$mailtext = "http://".$_SERVER['HTTP_HOST'].$path."bug.php?bug_id=".mysql_insert_id();
		pv_mail($mailto,$subject,$mailtext);

		header("Location: $returnhtml");
	}
?>