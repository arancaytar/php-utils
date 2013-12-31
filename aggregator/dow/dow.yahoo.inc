<?php

require_once 'net/http.inc';

define('DOW_YAHOO_URL_ALL', 'http://ichart.finance.yahoo.com/table.csv?s=^DJI&c=%Y');
define('DOW_YAHOO_URL_TODAY', 'http://download.finance.yahoo.com/d/quotes.csv?s=^DJI&f=o');

class DowYahoo extends DowFeed {
  function query($time) {
    // First, determine whether the Dow is even available on this day.
    if (date('N', $time + 3 * 3600) > 5) {
      return array(); // 4. Thou shalt not trade stocks during the weekend.
    }

    if (date('Y-m-d', $time + 3 *3600) == date('Y-m-d', time() + 3*3600)) {
      // Prices are being requested for today.
      if (date('H', time() + 3*3600) < 10) {
        return array(); // Give me some time. The Dow opens at 0930.
      }

      $url = DOW_YAHOO_URL_TODAY;
    }
    elseif ($time > time()) {
      return array(); // Do I LOOK like fucking Nostradamus?
    }
    else {
      $url = str_replace('%Y', date('Y', $time), DOW_YAHOO_URL_ALL);
    }

    $query = http($url);
    if ($query->code != 200) return NULL;

    if ($url != DOW_YAHOO_URL_TODAY) {
      $data = csv_parse($query->data, CSV_MODE_HEADER_BOTH);
      foreach ($data as &$set) $set['open'] = $set['Open'];
    }
    else {
      $data[date('Y-m-d')]['open'] = trim($query->data);
    }
    return $data;
  }
}
