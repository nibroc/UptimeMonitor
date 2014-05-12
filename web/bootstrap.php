<?php

set_include_path(get_include_path() . PATH_SEPARATOR . realpath(dirname(__FILE__) . "/library"));

spl_autoload_register(function ($class) {
	$path = str_replace(array("\\", "_"), "/", $class) . '.php';
	require $path;
	return true;
});

define('BASE_PATH', realpath(__DIR__));
define('REPO_PATH', BASE_PATH . '/servers');

$config = parse_ini_file(BASE_PATH . '/config.ini');

if ($config === false) {
	die("You must setup config.ini");
}
