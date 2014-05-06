<?php

require dirname(__FILE__) . '/../bootstrap.php';

use \ServerStatus\Repository\DirectoryRepository;

if (!empty($_REQUEST['key']) && $_REQUEST['key'] === 'hai') {
	$host = $_POST['hostname'];
	if(preg_match('/^[a-z]+$/', $host)) {
		$dr = new DirectoryRepository(REPO_PATH);
		$dr->storeServerRecord($host, $_POST);
		echo 'ok';
	}
} else {
	echo 'Invalid key';
}
