<?php

define('DB_FILE_AUTH', '/home/cburschka/files/auth/.database');
define('DB_DEFAULT_SERVER', 'mysql.ermarian.net');
define('DB_DEFAULT_USER', 'admin');
define('DB_DEFAULT_DATABASE', 'misc');
define('DB_DEFAULT_PREFIX', '');

define('DB_QUERY_REGEXP', '/(%d|%s|%%|%f|%b)/');

class Database {
  var $config;

  function Database($config = array()) {
    $this->config = $config;
    if (!is_array($this->config)) $this->config = array('database' => $config);
    $this->config += array(
      'server'   => DB_DEFAULT_SERVER,
      'user'     => DB_DEFAULT_USER,
      'database' => DB_DEFAULT_DATABASE,
      'prefix'   => DB_DEFAULT_PREFIX,
    );
    $link = $this->_connect();
    $db = (object) array('link' => $link, 'config' => $config);
    return $link;
  }
  
  function close() {
    mysql_close($this->link);
  }

  function _connect() {
    $passwords = explode("\n", trim(file_get_contents(DB_FILE_AUTH)));
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
    if (isset($args[0]) and is_array($args[0])) { // 'All arguments in one array' syntax
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

  switch ($match[1]) {
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
