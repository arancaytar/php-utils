<?php
require_once 'database/db.inc';
require_once 'parsers/csv.inc';
require_once 'net/http.inc';

class Dow {
  var $db;
  var $updated;
  var $feed;

  function Dow($feed) {
    $this->db = new Database();
    $this->updated = FALSE;
    $this->feed = $feed;
    $this->setUp();
  }

  function getOpening($time = NULL) {
    //var_dump(date('Y-m-d N(...)', $time + 3*3600));
    if (empty($time)) $time = time();
    $date = date('Y-m-d', $time + 3*3600);
    $open = $this->_getOpening($date);
    //var_dump($open);
    if ($open) {
      return $open;
    }
    else {
      //var_dump("UPDATING");
      if ($this->_update($time)) {
        return $this->_getOpening($date);
      }
    }
  }

  function _getOpening($date) {
    return $this->db->query("SELECT open FROM {dow} WHERE `date` = '%s'", $date)->cell();
  }

  function openedToday($time = NULL) {
    if (!$time) $time = time();
    if (date('H') * 60 + date('i') < 400) return FALSE;
  }

  function _update($time) {
    if ($this->updated || $this->_getOpening(date('Y-m-d', $time + 3*3600))) {
      $this->updated = TRUE;
      return; // already up-to-date.
    }

    $data = $this->feed->query($time);

    $args = array();
    $vals = array();
    foreach ($data as $date => $row) {
      $args[] = "('%s', %f)";
      array_push($vals, $date, $row['open']);
    }

    $this->db->query('INSERT INTO {dow} (`date`, `open`) VALUES ' . implode(', ', $args), $vals);
    return count($vals);
  }


  function setUp() {
    if ($this->db->table_exists('dow')) return;

    $this->db->query('CREATE TABLE {dow} (`date` DATE, `open` DECIMAL(8, 2))');
    $this->_update();
  }
}

function last_business_day($time = NULL) {
  if (!$time) $time = time();

  while (date('N', $time) > 5) $time -= 86400;
  return $time;
}

abstract class DowFeed {
  // Source classes that implement this interface are expected to accept a UNIX Timestamp.
  // The data returned from this function MUST BE a structured array indexed by date.
  // Each entry must be another array with the field "open".
  // The source can return any number of entries for multiple dates (depending on how much is provided
  // with a single query), but these must include the current date.
  // The source is expected to return an empty array if no data is available for the current day.
  abstract function query($time);
}
