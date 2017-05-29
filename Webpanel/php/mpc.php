<?php 
$host = "localhost"; //default = localhost, ip of mpd server
$port = "6601"; //default = 6600, port of mpd server
$password = "null"; //default = null, password for mpd server
$request = $_GET["action"];
if ($request=="update_db") {
	shell_exec("mpc --port=6601 clear");
	shell_exec("mpc --port=6601 update");
	shell_exec("mpc --port=6601 ls | mpc --port=6601 add");
	shell_exec("mpc --port=6601 play");
} else {
	
	$command = escapeshellcmd(sprintf('mpc --host=%s --port=%s %s', $host, $port, $request));
	$output = shell_exec($command);
}
echo $output;
?>
