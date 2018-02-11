<?php

define('NAV_EXT', 'menu');
define('WEB_ROOT', rtrim($_SERVER['DOCUMENT_ROOT']));

class NavigationMenu {
  function __construct($index, $tree) {
    $this->index = $index;
    $this->tree = $tree;
  }

  function save_to_file($filename) {
    file_put_contents($filename, $this->save_data());
  }

  function save_data() {
    return serialize([$this->index, $this->tree]);
  }

  function save_cache() {
    $this->save_to_file(WEB_ROOT . '/.navigation.cache');
  }

  static function load() {
    if (empty($_REQUEST['_recrawl']) && $menu = self::load_from_file(WEB_ROOT . '/.navigation.cache')) {
      return $menu;
    }
    else {
      $menu = self::load_crawl(WEB_ROOT);
      $menu->save_cache();
    }
    return $menu;
  }

  static function load_from_file($filename) {
    if (is_file($filename)) {
      return self::load_from_data(file_get_contents($filename));
    }
  }

  static function load_from_data($data) {
    list($index, $tree) = unserialize($data);
    return new static($index, $tree);
  }

  function load_crawl($webroot, $extension = NAV_EXT) {
    $tree = (object)['sub' => []];
    $index = [];
    $uncrawled  = [''];
    $visited = []; // Prevent infinite recursion.

    while ($uncrawled) {
      $dir = array_pop($uncrawled);
      $real = realpath("$webroot/$dir");
      if (empty($visited[$real])) $visited[$real] = TRUE;
      else continue;

      if ($handle = opendir("$webroot/$dir")) {
        while (FALSE !== ($item = readdir($handle))) {
          if ($item == '.' || $item == '..') {
            continue;
          }
          $path  = ltrim("$dir/$item", '/');
          if (is_dir("$webroot/$path")) {
            array_push($uncrawled, $path);
          }
          elseif (FALSE !== ($i = strrpos($item, '.'))) {
            $path = ltrim("$dir/" . substr($item, 0, $i), '/');
          }
          if (empty($index[$path]) and file_exists("$webroot/$path.$extension")) {
            $parent = self::_find_parent($tree, $path);
            $index[$path] = $parent->sub[$path] = self::_parse_file($webroot, $path, $extension);
          }
        }
        closedir($handle);
      }
    }
    return new self($index, $tree);
  }

  function html($request) {
    return self::_html($this->tree->sub, $request);
  }

  static function _html($tree, $request = NULL) {
    if (empty($tree)) return '';
    $tree = array_filter($tree, function($entry) { return !empty($entry->title); });
    usort($tree, function($a, $b) { return strcmp($a->title, $b->title); });
    $list = [];
    foreach ($tree as $entry) {
      $list[] = self::_link($entry, $request) . self::_html($entry->sub, $request);
    }
    return '<ul class="menu"><li>' . implode("</li>\n<li>", $list) . '</li></ul>';
  }

  static function _link($entry, $request = NULL) {
    if ($entry->link) {
      if ($entry->resource == $request) {
        return '<strong title="' . $entry->description . '">'. $entry->title . '</strong>';
      }
      else {
        return '<a title="' . $entry->description . '" href="' . $entry->resource . '">' . $entry->title . '</a>';
      }
    }
    else {
      return '<span title="' . $entry->description . '">' . $entry->title . '</span>';
    }
  }

  static function _find_parent($tree, $path) {
    $tokens = explode('/', $path);
    for ($i = 1; $i < count($tokens); $i++) {
      $prefix = implode('/', array_slice($tokens, 0, $i));
      if (isset($tree->sub[$prefix])) {
        return self::_find_parent($tree->sub[$prefix], $path);
      }
    }
    return $tree;
  }

  static function _parse_file($webroot, $path, $extension) {
    $data = explode("\n", file_get_contents("$webroot/$path.$extension"));
    $item = ['title' => '', 'description' => '', 'resource' => $path, 'link' => TRUE];
    foreach ($data as $line) {
      if (preg_match('/^(?<key>[a-z]+)\s*=\s*(?<value>.*?)$/', trim($line), $match)) {
        $item[$match['key']] = $match['value'];
      }
    }
    $item['link'] = ($item['link'] == 'True' || $item['link'] === TRUE);
    $item['sub'] = [];
    return (object) $item;
  }
}
