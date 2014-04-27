<?php

set_include_path(get_include_path() . PATH_SEPARATOR . realpath(dirname(__FILE__) . "/library"));

spl_autoload_register(function ($class) {
	$path = str_replace(array("\\", "_"), "/", $class) . '.php';
	require $path;
	return true;
});

define('REPO_PATH', realpath(__DIR__ . '/servers'));
