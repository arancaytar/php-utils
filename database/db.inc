<?php

define('DB_QUERY_REGEXP', '/(%a|%d|%s|%%|%f|%b)/');

class Database {
  var $config;

  function Database($config = array()) {
    $this->config = $config;
    $this->link = $this->_connect();
  }

  function close() {
    mysql_close($this->link);
  }

  function _connect() {
    $passwords = explode("\n", trim(file_get_contents($this->config['authfile'])));
    foreach ($passwords as $line) {
      $line = explode(":", trim($line));
      if (!isset($line[1])) continue;
      $passwords[$line[0]] = $line[1];
    }
    $config = $this->config + array(
      'password' => $passwords[$this->config['user']],
    );

    $this->link = @mysql_connect($config['server'], $config['user'], $config['password']);
    @mysql_select_db($config['database'], $this->link);
  }

  function query($query) {
    $args = func_get_args();
    array_shift($args);
    $query = $this->_prefix_tables($query);
    if (isset($args[0]) and is_array($args[0]) and count($args) == 1) { // 'All arguments in one array' syntax
      $args = $args[0];
    }
    _db_escape_args($args, TRUE);
    $query = preg_replace_callback(DB_QUERY_REGEXP, '_db_escape_args', $query);
    if (isset($_GET['debug'])) var_dump($query);
    $res = mysql_query($query);

    return new DatabaseResult($res);
  }

  function _prefix_tables($sql) {
    $db_prefix = $this->config['prefix'];
    if (is_array($db_prefix)) {
      if (array_key_exists('default', $db_prefix)) {
        $tmp = $db_prefix;
        unset($tmp['default']);
        foreach ($tmp as $key => $val) {
          $sql = strtr($sql, array('{'. $key .'}' => $val . $key));
        }
        return strtr($sql, array('{' => $db_prefix['default'], '}' => ''));
      }
      else {
        foreach ($db_prefix as $key => $val) {
          $sql = strtr($sql, array('{'. $key .'}' => $val . $key));
        }
        return strtr($sql, array('{' => '', '}' => ''));
      }
    }
    else {
      return strtr($sql, array('{' => $db_prefix, '}' => ''));
    }
  }

  function table_exists($table) {
    $table = str_replace("'", "", $table);
    return $this->query("SHOW TABLES LIKE '{$table}'")->rows;
  }

  function showTables($pre = '') {
    return $this->query("SHOW TABLES LIKE '{%s}%%'", $pre)->col();
  }

}

function _db_escape_args($match, $init = FALSE) {
  static $args = NULL;
  if ($init) {
    $args = $match;
    return;
  }
  if (isset($_GET['debug'])) var_dump($match, $args, "<hr />");

  switch ($match[1]) {
    case '%a':
      $t = array();
      foreach (array_shift($args) as $arg) {
        $t[] = _db_arg($match[1], $arg);
      }
      return '(' . implode(', ', $t) . ')';
    case '%d': // We must use type casting to int to convert FALSE/NULL/(TRUE?)
      return (int) array_shift($args); // We don't need db_escape_string as numbers are db-safe
    case '%s':
      return mysql_escape_string(array_shift($args));
    case '%%':
      return '%';
    case '%f':
      return (float) array_shift($args);
    case '%b': // binary data
      return "'" . mysql_escape_string(array_shift($args)) . "'";
  }
}

function _db_arg($type, $arg) {
  switch ($type) {
    case '%d': // We must use type casting to int to convert FALSE/NULL/(TRUE?)
      return (int) $arg; // We don't need db_escape_string as numbers are db-safe
    case '%s':
      return mysql_escape_string($arg);
    case '%%':
      return '%';
    case '%f':
      return (float) $arg;
    case '%b': // binary data
      return "'" . mysql_escape_string($arg) . "'";
  }
}

class DatabaseResult {
  function DatabaseResult($res) {
    $this->res = $res;
    $this->rows = @mysql_num_rows($res);
    $this->cols = @mysql_num_fields($res);
  }

  function cell() {
    if ($this->rows == 1 && $this->cols == 1) {
      $row = mysql_fetch_array($this->res);
      return $row[0];
    }
  }

  function col() {
    if ($this->rows > 0 && $this->cols == 1) {
      $col = array();
      while ($row = mysql_fetch_array($this->res)) {
        $col[] = $row[0];
      }
      return $col;
    }
  }
  function row() {
    return $this->getArray();
  }

  function getArray() {
    return mysql_fetch_array($this->res);
  }

  function table() {
    $all = array();
    while ($row = $this->getArray()) {
      $all[] = $row;
    }
    return $all;
  }
}

class DatabaseSchema {
  var $schema;

  function DatabaseSchema($schema) {
    $this->schema = $schema;
  }

  function create() {
    $sql = array();
    foreach ($schema as $name => $table) {
      $sql = 'CREATE TABLE `{' . $name . '}`';

    }
  }
}
