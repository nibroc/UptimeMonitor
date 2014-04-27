<?php

namespace ServerStatus {

	interface Repository
	{
		public function getServerList();
		public function getServerRecord($serverName);
		public function getServerRecords();
		public function storeServerRecord($serverName, $record);
	}
	
}