<?php

/**
 * @file morse.inc
 * Converts and parses morse code.
 */

define('MORSE_CODE_BOOK', dirname(__FILE__) . '/morse.txt');

define('MORSE_DIT', 0);
define('MORSE_DAH', 1);
define('MORSE_GAP_SIGNAL', 2);
define('MORSE_GAP_LETTER', 3);
define('MORSE_GAP_WORD', 4);

function morse_decode($input, $override = array()) {
  $map = morse_map($override);
	$codes = explode($map['visual'][MORSE_GAP_LETTER], $input);
  $output = '';

	foreach ($codes as $i => $code) {
		if (isset($map['decode'][$code])) {
      $output .= $map['decode'][$code];
    }
    else {
      return "Invalid code at #$i: $code";
		}
	}
	return strtolower($output);
}

function morse_encode($input, $override = array()) {
  $map = morse_map($override);
	$input = strtoupper($input);
  $output = array();

	foreach (str_split($input) as $i => $char) {
		if (isset($map['encode'][$char])) {
      $output[] = $map['encode'][$char];
    }
    else {
      return "Invalid character at #$i: $char";
    }
	}
	return implode($map['visual'][MORSE_GAP_LETTER], $output);
}

function morse_map($override = array()) {
  static $map;

  if (!isset($map)) {
    $map = array();
    $map['visual'] = $override + array(
      MORSE_DIT => '.',
      MORSE_DAH => '-',
      MORSE_GAP_SIGNAL => '',
      MORSE_GAP_LETTER => ' ',
      MORSE_GAP_WORD => '/',
    );

    $alphabet = explode("\n", trim(file_get_contents(MORSE_CODE_BOOK)));

    foreach ($alphabet as $line) {
      $line = explode('|', rtrim($line));
      $line[1] = _morse_process($line[1], $map['visual']);
      $map['encode'][$line[0]] = $line[1];
      $map['decode'][$line[1]] = $line[0];
    }
  }

  return $map;
}

function _morse_process($code, &$visual = array()) {
  $out = '';
  foreach (str_split($code) as $signal) {
    $out .= $visual[$signal];
  }
  return $out;
}
