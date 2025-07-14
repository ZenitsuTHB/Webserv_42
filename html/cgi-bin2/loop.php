#!/usr/bin/env php-cgi
<?php

while (1)
{
}

$uploadPath = getenv("UPLOAD_PATH");

if (!$uploadPath) {
    $uploadPath = "/tmp";
}

if (!is_dir($uploadPath))
{
	exit(1);
}

$filename = $uploadPath . "/datos_guardados.txt";

$data = file_get_contents("php://input");

file_put_contents($filename, $data . "\n", FILE_APPEND);

header("Content-Type: text/plain");
echo "Guardado en: $filename\n";
?>

