<?php

require_once 'net/http.inc';

define('DOW_PEERON_URL', 'http://irc.peeron.com/xkcd/map/data/%Y/%m/%d');

class DowPeeron extends DowFeed {
  function query($time) {
    //var_dump("CALLED");
    // First, determine whether the Dow is even available on this day.
    if (date('N', $time) > 5) {
      return array(); // 4. Thou shalt not trade stocks during the weekend.
    }

    if (date('Y-m-d', $time + 3*3600) == date('Y-m-d', time() + 3*3600)) {
      // Prices are being requested for today.
      if (date('H', time() + 3*3600) < 10) {
        return array(); // Give me some time. The Dow opens at 0930.
      }
    }
    elseif ($time > time()) {
      return array(); // Do I LOOK like fucking Nostradamus?
    }

    $date = explode('-', date('Y-m-d', $time + 3600));
    $url = str_replace(array('%Y', '%m', '%d'), $date, DOW_PEERON_URL);
    var_dump($url);

    $query = http($url);
    if ($query->code != 200) return NULL;

    $data[date('Y-m-d')]['open'] = trim($query->data);
    return $data;
  }
}
