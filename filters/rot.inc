<?php

function rotate($text,$offset) {
  $lowcase = strtolower($text);
  for ($i=0;$i<strlen($text);$i++) {
    if ($text[$i]!=$lowcase[$i]) $shift[$i]=true;
    else $shift[$i]=false;
  }
  $letters = array('a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r',
           's','t','u','v','w','x','y','z');
  for ($i=0;$i<strlen($text);$i++) {
    $position = array_keys($letters,$lowcase[$i]);
    if ($position) {
      $position=($position[0]+$offset)%26;
      $rotated[$i]=$letters[$position];
    } else $rotated[$i]=$lowcase[$i];
    if ($shift[$i]) $rotated[$i]=strtoupper($rotated[$i]);
  }
  return implode("",$rotated);
}

