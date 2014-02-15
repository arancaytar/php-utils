<?php

define('BASE_DIGITS', '0123456789abcdefghijklmnopqrstuvwxyz');
define('BASE_DEFAULT_PRECISION', 100);

function math_base_convert($number, $from, $to, $precision = BASE_DEFAULT_PRECISION) {
  $number = explode(".", "$number");
  $integer = $number[0];
  $decimal = !empty($number[1]) ? $number[1] : '';
  $from   = _base_digits($from);
  $to     = _base_digits($to);

  if (!_base_validate_digits($from) || !_base_validate_digits($to)) return;
  $from   = _base_values($from);
  $per = count($from);
  $converted = array();

  for($i = 0; $i < strlen($integer); $i++) {
    if (!isset($from[$integer[$i]])) return;

    $digit = $from[$integer[$i]];
    $power = strlen($integer) - $i -1;
    // Value of the digit is $digit * ($per ^ $power)

    // TODO: Find a method that does not involve storing this big number.
    $big = $digit * pow($per, $power);
    $target = array();
    while ($big) {
      $store = $big % strlen($to);
      $big -= $store;
      $big /= strlen($to);
      $target[] = $store;
    }
    foreach ($target as $i => $a) {
      if (empty($converted[count($target) - $i - 1])) {
        $converted[count($target) - $i - 1] = 0;
      }
      $converted[count($target) - $i - 1] += $a;
    }
  }

  $small = array();
  if ($decimal) {
    for ($i = 0; $i < strlen($decimal); $i++) {
      if (!isset($from[$decimal[$i]])) return;

      $digit = $from[$decimal[$i]];
      $fraction = $digit / pow($per, $i + 1);
      for ($j = 0; $fraction && $j < $precision; $j++) {
        $value = floor($fraction * pow(strlen($to), $j+1)) % strlen($to);
        if (!isset($small[$j])) $small[$j] = 0;
        $small[$j] += $value;
        $fraction -= $value / pow(strlen($to), $j+1);
      }
    }

    $shifting = true;
    while ($shifting) {
      $shifting = false;
      foreach ($small as $i => $a) {
        if ($a >= strlen($to)) {
          $shifting = true;
          $small[$i-1]++;
          $small[$i]-=strlen($to);
        }
      }
    }
  }

  ksort($converted);
  if (!$converted) $converted = array(0);
  ksort($small);
  $out = '';
  foreach ($converted as $a) $out .= $to[$a];
  if ($decimal) {
    $out .= '.';
    foreach ($small as $a) $out .= $to[$a];
  }
  return $out;
}

function _base_digits($base) {
  if (is_array($base)) {
    $digits = implode('', $base);
  }
  else {
    if ($base > 36) return;
    $digits = substr(BASE_DIGITS, 0, $base);
  }
  return $digits;
}

function _base_values($digits) {
  $values = array();
  for ($i = 0; $i < strlen($digits); $i++) {
    $values[$digits[$i]] = $i;
  }
  return $values;
}

function _base_validate_digits($digits) {
  if (!$digits) return FALSE;
  $a = array();
  for ($i = 0; $i < strlen($digits); $i++) {
    if (isset($a[$digits[$i]])) return FALSE;
    $a[$digits[$i]] = TRUE;
  }
  return TRUE;
}