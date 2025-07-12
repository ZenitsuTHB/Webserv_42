<?php
$uploadPath = getenv("UPLOAD_PATH");

if (!$uploadPath) {
    $uploadPath = "/tmp";
}

$filename = $uploadPath . "/datos_guardados.txt";

$data = file_get_contents("php://input");

file_put_contents($filename, $data . "\n", FILE_APPEND);

header("Content-Type: text/plain");
echo "Guardado en: $filename\n";
?>

