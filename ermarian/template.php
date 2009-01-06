<?php
define('TEMPLATE', dirname(__FILE__) . '/ermarian.tpl.php');

function theme_page($variables) {
  $variables += array(
    'site_name' => "The Ermarian Network",
    'links' => array(),
    'styles' => '',
    'scripts' => '',
    'title' => '',
    'content' => '',
    'navigation' => '',
    'meta' => new stdClass(),
  );
  
  $variables['meta'] = (object)((array)$variables['meta'] + array(
    'author' => 'Arancaytar Ilyaran <arancaytar@ermarian.net>',
    'description' => 'This page demonstrates the use of a slider to change opacity of an image.',
    'keywords' => array('slider', 'jquery'),
    'extra' => '',
  ));
  
  $variables['links'] = (object)((array)$variables['links'] + array(
    'prev' => '',
    'next' => '',
    'main' => '',
    'index' => '',
    'about' => '',
    'rss' => '',
  ));
  
  foreach ($variables as $name => $value) $$name = $value;
  ob_start();
  require_once TEMPLATE;
  return ob_get_clean();
}
