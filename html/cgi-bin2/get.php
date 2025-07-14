#!/usr/bin/env php-cgi
<?php
header("Content-Type: text/html");

echo "<html><body>";
echo "<h1>CGI GET PHP</h1>";

$query = $_SERVER["QUERY_STRING"];
echo "<p><strong>Query:</strong> " . htmlspecialchars($query) . "</p>";

parse_str($query, $params);
echo "<ul>";
foreach ($params as $key => $value) {
    echo "<li>" . htmlspecialchars($key) . ": " . htmlspecialchars($value) . "</li>";
}
echo "</ul>";

echo "</body></html>";
?>

