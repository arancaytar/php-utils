<?php
require_once 'template/navigation.inc';

define('TEMPLATE', dirname(__FILE__) . '/ermarian.tpl.php');

define('TEMPLATE_AD', <<<END
			<script type="text/javascript"><!--
                        google_ad_client = "pub-7276462266487251";
                        /* 728x90, created 5/2/08 */
                        google_ad_slot = "9786486556";
                        google_ad_width = 728;
                        google_ad_height = 90;
                        //-->
                        </script>
                        <script type="text/javascript"
                        src="http://pagead2.googlesyndication.com/pagead/show_ads.js">
                        </script>
END
);

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
    'raw_title' => '',
    'options' => array(
      'content.add_id' => TRUE,
    ),
    'base' => $_SERVER['HTTP_HOST'],
  );

  $variables['meta'] = (object)((array)$variables['meta'] + array(
    'author' => htmlentities('Arancaytar Ilyaran <arancaytar@ermarian.net>'),
    'description' => '',
    'keywords' => array(),
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

  if (!$variables['title']) $variables['title'] = navigation_field('title');
  if (!$variables['navigation']) $variables['navigation'] = theme_navigation(navigation_tree());
  if (!$variables['meta']->description) $variables['meta']->description = navigation_field('description');

  if ($variables['options']['content.add_id']) {
    $variables['content'] = theme_heading_id($variables['content']);
  }

  foreach ($variables as $name => $value) $$name = $value;

  if (is_array($scripts)) {
    foreach ($scripts as $i => $script) {
      $scripts[$i] = "<script type='text/javascript' src='$script'></script>";
    }
    $scripts = implode("\n", $scripts);
  }
  if (is_array($styles)) {
    foreach ($styles as $i => $style) {
      $styles[$i] = "<link rel='stylesheet' type='text/css' href='$style' />";
    }
    $styles = implode("\n", $styles);
  }
  $meta->keywords = implode(', ', $meta->keywords);

  
  ob_start();
  require_once TEMPLATE;
  return ob_get_clean();
}

function theme_heading_id($text) {
  return preg_replace_callback('%<h([0-6])( [^>]*)?>(.*?)</h\\1>%', '_theme_heading_id_', $text);
}

function _theme_heading_id_($matches) {
  $level = $matches[1];
  $args = $matches[2];
  $content = $matches[3];

  if (!preg_match('/["\'\s]id=/', $args)) {
    $id = urlencode(strip_tags($content));
    $args = ' id="' . $id . '"' . $args;
  }
  return "<h$level$args>$content</h$level>";
}
