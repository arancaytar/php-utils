<?php

define('CSV_MODE_HEADER_BOTH', 6);
define('CSV_MODE_HEADER_COL', 3);
define('CSV_MODE_HEADER_ROW', 2);
define('CSV_MODE_HEADER_NONE', 1);

define('CSV_DEFAULT_DELIM', ',');

function csv_parse($data, $mode = CSV_MODE_HEADER_ROW, $d = CSV_DEFAULT_DELIM) {
  $rows = explode("\n", trim($data));
  if ($mode % CSV_MODE_HEADER_ROW == 0) {
    $header = explode($d, trim(array_shift($rows)));
  } else {
    for ($i = 0; $i < count($rows[0]); $i++) $header[$i] = $i;
  }

  foreach ($rows as $i => $row) {
    $row = explode($d, trim($row));
    if ($header) {
      $new = array();
      foreach ($row as $i => $cell) {
        $new[$header[$i]] = $cell;
      }
    }
    else $new = $row;
    if ($mode % CSV_MODE_HEADER_COL == 0) {
      $parsed[$row[0]] = $new;
    }
  }
  return $parsed;
}