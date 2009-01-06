<?php

function theme_rss($feed) {
  $feed += array(
    'title' => '',
    'link' => '',
    'description' => '',
    'language' => 'en-us',
    'image' => '',
    'webmaster' => '',
    'managingeditor' => '',
    'items' => array(),
  );

  // How often can I say field on one line? Let's see:
  foreach ($feed as &$field) if ("$field" == $field) $field = htmlentities($field);
  
  if (!is_array($feed['items'])) $feed['items'] = array();
  
  foreach ($feed['items'] as &$item) {
    $item += array(
      'title' => '',
      'link' => '',
      'description' => '',
      'pubdate' => '',
      'author' => '',
      'guid',
    );

    foreach ($item as &$field) $field = htmlentities($field);
  }
  
  require_once 'net/mime.inc';
  mime('rss');

  ob_start();
  require_once 'template/rss.tpl.php';
  return ob_get_clean();
}

