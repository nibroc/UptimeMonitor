<?php

namespace ServerStatus\Repository {

	class DirectoryRepository implements \ServerStatus\Repository
	{

		private $_directory;
		
		private $_fileExtension;
		
		private $_servers = null;
		
		public function __construct($directory, $fileExtension = '')
		{
			if (!is_dir($directory)) {
				throw new \Exception("Invalid directory");
			}
			$this->_directory = $directory;
			$this->_fileExtension = $fileExtension;
		}
		
		public function getServerList()
		{
			if (null === $this->_servers)
			{
				$dh = opendir($this->_directory);
				if (!$dh) {
					throw new \Exception("Unable to open directory");
				}
				$this->_servers = array();
				while (false !== ($fn = readdir($dh))) {
					if ($fn == '.' || $fn == '..') {
						continue;
					}
					$path = $this->_directory . '/' . $fn;
					if (is_file($path)) {
						$this->_servers[] = $fn;
					}
				}
			}
			return $this->_servers;
		}
		
		public function getServerRecord($serverName)
		{
			if (!in_array($serverName, $this->getServerList())) {
				throw new \Exception("Invalid server specified");
			}
			
			$filepath = $this->getFileName($serverName);
			
			if (!file_exists($filepath) || !is_readable($filepath)) {
				throw new \Exception("Unable to read file");
			}
			
			$data = unserialize(file_get_contents($filepath));
			
			$timestamp = filemtime($filepath);
			
			$data['timestamp'] = $timestamp;
			
			return $data;
		}
		
		public function getServerRecords()
		{
			$records = array();
			foreach ($this->getServerList() as $name) {
				$records[$name] = $this->getServerRecord($name);
			}
			return $records;
		}
		
		public function storeServerRecord($serverName, $record)
		{
			
			if (!file_put_contents($this->getFileName($serverName), serialize($record))) {
				throw new \Exception("Error storing record");
			}
			
			return $this;
			
		}
		
		private function getFileName($serverName)
		{
			return sprintf("%s/%s%s", $this->_directory, $serverName, $this->_fileExtension);
		}

	}
	
}
