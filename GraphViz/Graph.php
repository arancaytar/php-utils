<?php

namespace GraphViz;

class Graph {
  public function __construct($string) {
    $this->handle = readstring($string);
  }

  public function render($engine, $format = 'svg') {
    if (!$engine) $engine = 'dot';
    layout($this->handle, $engine);

    // renderresult() totally doesn't do what it says it does.
    $data = NULL;
    ob_start();
    renderresult($this->handle, $format, $data);
    return ob_get_clean();
  }
}
