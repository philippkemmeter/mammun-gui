<?php
session_start();
require_once ('../include/shared/classes/server_com/ClientServer.inc');
require_once ('../include/_config.inc');

/**
 * Speichert die Session id um sie für send_request zu übergeben.
 * Wird bei Bildern, CSS etc leer überschrieben.
 * @author Robert Kemmeter
 * @since 2009-05-20
 *
 * TODO dirty!!
 *
 * @var string session id#
 */

$method = (empty($_POST)) ? "GET" :"POST";

$keys = array_merge($_POST,$_GET);
$page = $keys['page'];
unset($keys['page']);
$i = 0;

function foo($append='', $array = array(), $moreThan1D = false, $arrayAdress ='')
{
	$glue = '&';
	foreach ($array as $key=>$value)
  	{

      	if (is_array($value))
      	{
      		if($moreThan1D == false)
      		{
	      		$append = foo($append, $value, true, $glue . $key);
      		}
      		else
      		{
	      		$append = foo($append, $value, true, $arrayAdress.'['. $key .']');
      		}
      	}

		else
		{
			if($moreThan1D == false)
			{
				$append .= $glue . $key . '=' . $value;
			}
			else
			{
				$append .= $arrayAdress . '[' . $key . ']' .  '=' . $value;
			}
		}
  	}
  	return $append;
}



$punkt_pos = strrpos($page, '.');
$file_extension = substr($page, $punkt_pos + 1);

switch ($file_extension)
{
	case 'css':
		header('Content-type: text/css');
		$append = '';
		break;
	case 'jpg':
	case 'jpeg':
	case 'jpe':
		header ("Content-type: image/jpeg");
		$append = '';
		break;
	case 'gif':
		header("Content-type: image/gif");
		$append = '';
		break;
	case 'png':
		header("Content-type: image/png");
		$append = '';
		break;
	default:
		if($page)
		{
			$append = $page;
			if($keys)
			{
				$append = foo($append,$keys);
			}

		}
		$append = str_replace('-FRAGEZEICHEN-','?',$append);
		if (!isset($keys['t']) && isset($_SESSION['WCF_ST'])) {
			$append.= (strpos($append,'?') === false) ? '?' : '&';
			$append.= 't='.$_SESSION['WCF_ST'];
		}
		if (!isset($keys['s']) && isset($_SESSION['WCF_SID'])) {
			$append.= '&s='.$_SESSION['WCF_SID'];
		}
}
//die($append);
if($append!='')
{
	echo ClientServer::send_request(CONF_WCF_SERVER_URL.'/'.$append, true, false, $method);
}
else
{
	echo ClientServer::send_request(CONF_WCF_SERVER_URL.'/'.$page, true, false, $method );
}

?>