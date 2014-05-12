<?php

require dirname(__FILE__) . '/../bootstrap.php';

use \ServerStatus\Repository\DirectoryRepository;

$key = filter_input(INPUT_GET, 'key');
$host = filter_input(INPUT_POST, 'hostname');

// TODO: actually filter this...
$serverRecord = $_POST;

$errors = array();

if ($key !== $config['api_key']) {
	$errors[] = "invalid key";
}

if (!preg_match('/^[a-z]+$/', $host)) {
	$errors[] = "invalid host name";
}

if ($errors) {
	echo "errors: " . implode('/', $errors);
} else {
	$dr = new DirectoryRepository(REPO_PATH);
	$dr->storeServerRecord($host, $serverRecord);
	echo 'ok';
}
