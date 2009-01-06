<?php
require_once 'database/db.inc';
require_once 'parsers/csv.inc';
require_once 'net/http.inc';

define('DOW_YAHOO_URL_ALL', 'http://ichart.finance.yahoo.com/table.csv?s=^DJI&c=%Y');
define('DOW_YAHOO_URL_TODAY', 'http://download.finance.yahoo.com/d/quotes.csv?s=^DJI&f=o');

define('DOW_MODE_TODAY', 0);
define('DOW_MODE_ALL', 1);

define('DOW_DEFAULT_START_YEAR', 1928);

class Dow {
  var $db;
  var $updated;

  function Dow() {
    $this->db = new Database();
    $this->updated = FALSE;
    $this->setUp();
  }

  function getOpening($time = NULL) {
    if (empty($time)) $time = time();
    $date = date('Y-m-d', $time);
    $open = $this->_getOpening($date);
    if ($open) {
      return $open;
    }
    else {
      if ($this->_update()) {
        return $this->_getOpening($date);
      }
    }
  }

  function _getOpening($date) {
    return $this->db->query("SELECT open FROM {dow} WHERE `date` = '%s'", $date)->cell();
  }

  function _update() {
    $n = date('N');
    if ($n > 5) {
      $date = date('Y-m-d', time() - ($n - 5) * 86400);
    }
    else {
      $date = date('Y-m-d');
    }
    var_dump($date, $this->_getOpening($date), $this->updated);
    
    if ($this->updated || $this->_getOpening($date)) {
      $this->updated = TRUE;
      return; // already up-to-date.
    }
    
    $date = date('Y-m-d', time() - 7*86400);
    $yesterday = $this->_getOpening($date);
    if ($yesterday) {
      $data = $this->_query_yahoo(DOW_MODE_TODAY);
    }
    else {
      $data = $this->_query_yahoo(DOW_MODE_ALL, date('Y'));
    }
    $args = array();
    $vals = array();
    foreach ($data as $date => $row) {
      $args[] = "('%s', %f)";
      array_push($vals, $date, $row['Open']);
    }
    
    $this->db->query('INSERT INTO {dow} (`date`, `open`) VALUES ' . implode(', ', $args), $vals);
    return count($vals);
  }


  function _query_yahoo($mode = DOW_MODE_TODAY, $start_year = DOW_DEFAULT_START_YEAR) {
    if ($mode == DOW_MODE_TODAY) {
      $url = DOW_YAHOO_URL_TODAY;
    }
    else {
      $url = str_replace('%Y', $start_year, DOW_YAHOO_URL_ALL);
    }
 
    $query = http($url);
    if ($query->code != 200) return NULL;
  
    if ($mode == DOW_MODE_ALL) {
      $data = csv_parse($query->data, CSV_MODE_HEADER_BOTH);
    }
    else {
      $data[date('Y-m-d')]['Open'] = trim($query->data);
    }
    return $data;
  }

  function setUp() {
    if ($this->db->table_exists('dow')) return;

    $this->db->query('CREATE TABLE {dow} (`date` DATE, `open` DECIMAL(8, 2))');
    $this->_update();
  }
}
