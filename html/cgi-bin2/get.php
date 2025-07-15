#!/usr/bin/env php-cgi
<?php
header("Content-Type: text/html");

$query = $_SERVER["QUERY_STRING"];
echo "Query: " . htmlspecialchars($query);

parse_str($query, $params);
foreach ($params as $key => $value) {
    echo "\n" . htmlspecialchars($key) . ": " . htmlspecialchars($value);
}
?>

