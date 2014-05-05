<?php

require dirname(__FILE__) . '/../bootstrap.php';

$memories = array(
	'server1' => 256 * 1024,
	'server2' => 512 * 1024,
	'lmxe' => 2048 * 1024
);

use \ServerStatus\Repository\DirectoryRepository;

$dr = new DirectoryRepository(REPO_PATH);

function formatTime($time)
{
	$seconds = (float) $time;

	$days = (int) ($seconds / 86400);
	$seconds -= $days * 86400;

	$hours = (int) ($seconds / 3600);
	$seconds -= $hours * 3600;

	$minutes = (int) ($seconds / 60);
	$seconds -= $minutes * 60;
	
	return sprintf("%d days, %d hours, %d minutes, %d seconds", $days, $hours, $minutes, $seconds);
}

$records = $dr->getServerRecords();

usort($records, function ($rec1, $rec2) {
	return strcmp($rec1['hostname'], $rec2['hostname']);
});

?><!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01//EN" "http://www.w3.org/TR/html4/strict.dtd">
<html>
	<head>
		<title>Servers</title>
		<meta http-equiv="Content-Type" content="text/html; charset=utf-8">
		<link rel="stylesheet" href="style.css" type="text/css">
	</head>
	<body>
		<div class="heading">
			<h1>Server Status</h1>
		</div>
		<table id="status-table">
			<thead style="font-weight: bold; text-align: center;">
				<tr>
					<td style="width: 70px" rowspan="2">Hostname</td>
					<td style="width: 285px" rowspan="2">Uptime</td>
					<td style="width: 310px" colspan="4">Memory</td>
					<td style="width: 100px;" colspan="3">Load</td>
					<td style="width: 200px" colspan="2">Processes</td>
					<td style="width: 260px" rowspan="2">Last Check-in</td>
				</tr>
				<tr>
					<td>Used</td>
					<td>Free</td>
					<td>Limit</td>
					<td>Burst</td>
					<td>1</td>
					<td>5</td>
					<td>15</td>
					<td>Running</td>
					<td>Total</td>
				</tr>
			</thead>
			<tbody>
				<?php

				foreach($records as $server => $data) {

					$date = DateTime::createFromFormat('U', $data['timestamp']);

					$date->setTimezone(new DateTimeZone('America/Chicago'));

					$seconds = (time() - $date->format('U'));
					$up = ($seconds < 180);
					
					$m = $data['mem'];
					
					$mem = (isset($memories[$data['hostname']])) ? $memories[$data['hostname']] : $m['total'];

				?>

				<tr>

					<td>
						<span class="server <?php echo $up ? 'online' : 'offline'; ?>"><?php echo htmlentities($data['hostname']); ?></span>
					</td>

					<td><?php echo formatTime($data['uptime']['total']); ?></td>

					<td style="text-align: right">
						<?php printf("%dM", ($m['total'] - $m['free']) / 1024); ?>
					</td>

					<td style="text-align: right">
						<?php printf("%dM", ($mem - ($m['total'] - $m['free']))/ 1024); ?>
					</td>

					<td style="text-align: right">
						<?php printf("%dM", $mem / 1024); ?>
					</td>
					
					<td style="text-align: right">
						<?php printf("%dM", $m['total'] / 1024); ?>
					</td>


					<td style="text-align: center">
						<?php $l = $data['load']; printf("%.2f", $l['l1']); ?>
					</td>

					<td style="text-align: center">
						<?php printf("%.2f", $l['l5']); ?>
					</td>

					<td style="text-align: center">
						<?php printf("%.2f", $l['l15']); ?>
					</td>

					<td style="text-align: right">
						<?php echo $l['runningProcs']; ?>
					</td>

					<td style="text-align: right">
						<?php echo $l['totalProcs']; ?>
					</td>

					<td>
						<?php echo (time() - $date->format('U')) . ' seconds ago'; ?>
					</td>

				</tr>

				<?php

				}

				?>
			</tbody>
		</table>
		<div class="footing">uptimed v0.1 &copy; 2012</div>
	</body>
</html>
