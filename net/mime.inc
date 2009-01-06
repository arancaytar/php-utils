<?php

function mime($type = 'xhtml') {
  static $mimes = array(
    'xhtml' => 'application/xhtml+xml',
    'rss'   => 'application/rss+xml',
    'html'  => 'text/html',
    'js'    => 'text/javascript',
    'xml'   => 'application/xml',
    'json'  => 'text/javascript',
    'png'   => 'image/png',
    'gif'   => 'image/gif',
    'jpg'   => 'image/jpeg',
    'jpeg'  => 'image/jpeg',
    'txt'   => 'text/plain',
  );
  
  if ($type == 'xhtml' && !preg_match('/application\/xhtml\+xml/', $_SERVER['HTTP_ACCEPT'])) {
    $type = 'html';
  }
  header('Content-type: ' . $mimes[$type] . ';charset=utf-8');
  
  if ($type[0] == 'x') {
    print '<?xml version="1.0" encoding="utf-8"?>';
  }
}