<?php
$data = $_GET["data"];
error_reporting(E_ALL);


/* Получаем порт сервиса WWW. */
$service_port = 9090; 
$address = gethostbyname('localhost');

/* Создаём  TCP/IP сокет. */
$socket = socket_create(AF_INET, SOCK_STREAM, SOL_TCP);



$result = socket_connect($socket, $address, $service_port);

$out = '';



socket_write($socket, $data, strlen($data));

while ($out = socket_read($socket, 2048)) {
    echo $out;
    
}

socket_close($socket);
?>
