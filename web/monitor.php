<?php

require dirname(__FILE__) . '/bootstrap.php';

use \ServerStatus\Repository\DirectoryRepository;

$dr = new DirectoryRepository(REPO_PATH);

$down = array();

$timeout = 210; //More than 2.5 minutes is considered down
$noticeTimeout = 60*60; //Only send emails hourly

foreach ($dr->getServerRecords() as $server => $record) {
	if (time() - $record['timestamp'] > $timeout) {
		$down[] = $server;
	}
}

if (count($down)) {
	$downLog = array();
	if (file_exists(dirname(__FILE__) . '/downlog')) {
		$downLog = unserialize(file_get_contents(dirname(__FILE__) . '/downlog'));
	}
	$needSend = false;
	foreach ($down as $serv) {
		if (!isset($downLog[$serv]) || time() - $downLog[$serv] > $noticeTimeout) {
			$needSend = true;
			break;
		}
	}
	if ($needSend) {
		$log = array();

		foreach ($down as $d) {
			$log[$d] = time();
		}

		$transport = new Zend_Mail_Transport_Smtp('smtp.gmail.com', array(
			'username' => 'something@gmail.com',
			'password' => 'password',
			'auth' => 'login',
			'ssl' => 'tls',
			'port' => 587
		));

		$mail = new Zend_Mail();

		$mail->addTo('recipient@blah.tld', 'Recipient Name');
		$mail->setSubject('Servers Down: ' . implode(', ', $down));
		$mail->setBodyText('The servers were reported as down.');
		$mail->send($transport);

		file_put_contents(dirname(__FILE__) . '/downlog', serialize($log));
	}
}
