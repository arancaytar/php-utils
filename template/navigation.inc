<?php

function navigation_list($web_root = NULL, $recrawl = FALSE, $extension = 'info') {
  static $list = array();
  if (!$web_root) $web_root = $_SERVER['DOCUMENT_ROOT'];

  if (isset($list[$web_root])) return $list[$web_root];

  if ($recrawl || !file_exists("$web_root/.navigation.cache")) {
    navigation_crawl($web_root, $extension);
  }

  $data = file("$web_root/.navigation.cache");

  $list[$web_root] = navigation_parse_cache($data);
  return $list[$web_root]; 
}

function navigation_tree($web_root = NULL, $recrawl = FALSE, $extension = 'info') {
  static $tree = array();
  if (!$web_root) $web_root = $_SERVER['DOCUMENT_ROOT'];

  if (isset($tree[$web_root])) return $tree[$web_root];
  
  $tree[$web_root] = navigation_build_tree(navigation_list($web_root));
  return $tree[$web_root];
}

function navigation_title() {
  static $list = array();
  if (!$list) {
    $list = navigation_list();
  }
  preg_match('/^\/([^?]+?)(\.(html|htm|php))?$/', $_SERVER['REQUEST_URI'], $match);
  $request = $match[1];
  return isset($list[$request]) ? $list[$request]['title'] : "";
}

function navigation_parse_cache($data) {  
  $parsed = array();
  foreach ($data as $line) {
    if (!preg_match('/([^ ]+)\s+(.+) +: +(.*?)(\/([0-9]*))?$/', trim($line), $match)) continue;
    $parsed[$match[1]] = (object)array(
      'depth' => isset($match[5]) ? $match[5] : count(explode('/', $match[1])) - 1,
      'resource' => $match[1],
      'title' => $match[2],
      'description' => $match[3],
      'sub' => array(),
      'parent' => array(),
    );
  }
  return $parsed;
}


/* I HAVE NO IDEA WHAT THIS IS INTENDED TO DO. */
function ______navigation_build_tree($data, $level = 0) {
  $tree = array();
  $current = -1;
  $data[-1] = array('sub' => array(), 'depth' => 0, 'resource' => '', 'title' => 'Home', 'description' => '');
  ksort($data);
  foreach ($data as $i => $entry) {
    if ($entry['depth'] == $level) {
      $data[$current]['sub'] = navigation_build_tree($data[$current]['sub'], $level + 1);
      $current = $i;
    }
    else {
      $data[$current]['sub'][] = $entry;
      unset($data[$i]);
    }
  }
  return $data;
}

function navigation_build_tree($data) {
  $parents = array();
  foreach ($data as $path => $item)
    foreach ($data as $path2 => $item2)
      if ((empty($parents[$path]) || strlen($path2) > strlen($parents[$path])) && substr($path, 0, strlen($path2) + 1) == "$path2/")
        $parents[$path] = $path2;
  $nonroots = [];
  foreach ($data as $path => $item) {
    if (!empty($parents[$path])) {
      $data[$parents[$path]]->sub[$path] = $item;
      $nonroots[] = $path;
    }
  }
  foreach ($nonroots as $path) {
    unset($data[$path]);
  }
  return $data;
}

function theme_navigation($tree) {
  $tree2 = array();
  foreach ($tree as $entry) {
    if (!isset($entry->title)) continue;
    $tree2[$entry->title] = $entry;
  }
  ksort($tree2);
  foreach ($tree as $i=>&$entry) {
    if (isset($entry->resource)) $entry = l($entry) . theme_navigation($entry->sub);
    else unset($tree[$i]);
  }
  return $tree ? '<ul class="menu"><li>' . implode("</li>\n<li>", $tree) . '</li></ul>' : '';
}

function l($entry) {
  if (trim($_SERVER['REQUEST_URI'], '/') == $entry->resource) {
    return '<strong title="' . $entry->description . '">'. $entry->title . '</strong>';
  }
  else {
    return '<a title="' . $entry->description . '" href="' . $entry->resource . '">' . $entry->title . '</a>';
  }
}

function navigation_crawl($webroot, $extension = 'info') {
  $files = array();
  $pages = array();
  $cmd = "find $webroot -name '*.$extension'|sort";
  exec($cmd, $files);
  foreach ($files as $file) {
    $data = file($file);
    $depth = -1;
    $resource = array(
      'title' => '', 
      'description' => '', 
      'resource' => substr($file, strlen($webroot) + 1, -1 * strlen($extension) - 1),
    );
    
    foreach ($data as $line) {
      if (preg_match('/([^ ]+?) *= *(.+)/', trim($line), $match)) {
        if ($match[1] == 'resource') $depth = count(explode('/', $resource['resource'])) - 1;
        $resource[$match[1]] = $match[2];
      }
    }
    if ($depth >= 0) $resource['description'] .= "/$depth";
    $pages[$resource['resource']] = $resource;
  }
  $out = '';
  foreach ($pages as $page) {
    $out .= $page['resource'] . ' ' . $page['title'] . ' : ' . $page['description'] . "\n";
  }
  //var_dump($out);
  file_put_contents("$webroot/.navigation.cache", $out);
}
