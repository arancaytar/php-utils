<!DOCTYPE html>
  <!--
    Template for Ermarian Network (http://ermarian.net/)
    Copyright Arancaytar <arancaytar@ermarian.net> 2006-2013.
  -->
<html>
  <head>
    <meta http-equiv="Content-Type" content="text/html; charset=utf-8" />

    <title><?=$raw_title ? $raw_title : "$title | $site_name"?></title>

    <!-- meta -->
    <meta name="author" lang="en" content="<?=$meta->author?>" />
    <meta name="description" lang="en" content="<?=$meta->description?>" />
    <meta name="keywords" lang="en" content="<?=$meta->keywords?>" />
    <?php if ($meta->extra) { print $meta->extra; } ?>

    <!-- /meta -->

    <base href="http://<?=$base?>" />

    <!-- related documents -->
    <?php if ($links->prev) : ?><link rel="prev"  href="<?=$links->prev?>" /><?php endif; ?>
    <?php if ($links->next) : ?><link rel="next"  href="<?=$links->next?>" /><?php endif; ?>
    <?php if ($links->index) : ?><link rel="contents" href="<?=$links->index?>" /><?php endif; ?>
    <?php if ($links->main) : ?><link rel="start" href="<?=$links->main?>" /><?php endif; ?>
    <?php if ($links->about) : ?><link rel="help" href="<?=$links->about?>" /><?php endif; ?>
    <!-- /related documents -->

    <?php if ($links->rss) { print $links->rss; } ?>

    <!-- styles and scripts -->
    <link rel="icon" href="images/icon" />
    <link rel="stylesheet"
          type="text/css"
          media="all"
          href="css/default.css" />
    <link rel="stylesheet"
          title="Screen"
          type="text/css"
          media="screen"
          href="css/screen.css" />
    <link rel="stylesheet"
          type="text/css"
          media="print"
          href="css/print.css" />
    <link rel="alternate stylesheet"
          type="text/css"
          title="Print"
          href="css/print.css" />

    <?=$styles?>
    <?=$scripts?>

    <!-- /styles and scripts -->
  </head>

  <body>
    <div id="content">
      <!-- header -->
      <div class="filled-box head-bar" id="title">
        <h1><a href="/"><?=$site_name?></a></h1>
      </div>
      <!-- /header -->

      <!-- sidebar -->
      <div class="filled-box side-bar" id="navside">
        <h2 class="navside">Navigation</h2>
        <div id="menu"><?=$navigation?></div>
      </div>
      <!-- /sidebar -->

      <!-- page-footer -->
      <div id="page-footer">
        <!-- ads -->
        <div id="advertisement">
        <?=TEMPLATE_AD?>
        </div>

        <!-- /ads -->

        <h2><?=$title?></h2>

        <!-- page-inner -->
        <div id="page-inner">
          <?=$content?>

        </div>
        <!-- /page-inner -->

        <!-- site validation -->
        <div class="filled-box" id="footer">
          <a href="http://validator.w3.org/check?uri=referer">
            <img src="images/validation/valid-html5.png" alt="Valid HTML 5 Strict" height="31" width="88" style="float:left" />
          </a>
          <a href="http://jigsaw.w3.org/css-validator/validator">
            <img style="border:0;width:88px;height:31px;float:left" src="images/validation/valid-css.png" alt="Valid CSS" />
          </a>
          This page can be viewed in any standards-compliant browser.<br/>
          Recommended:
          <a class="external" href="http://www.spreadfirefox.com/?q=affiliates&amp;id=96065&amp;t=54">Firefox</a>,
          <a class="external" href="http://www.opera.com">Opera</a>,
          <a class="external" href="http://www.chromium.org//">Chromium</a>
        </div>
        <!-- /validation -->

        <!-- copyright -->
        <div class="filled-box" id="copyright">
          All content on this page, unless stated otherwise, is owned by
          <a class="mail" title="arancaytar.ilyaran@gmail.com" href="<?=urlencode(htmlentities('mailto:"Arancaytar Ilyaran" <arancaytar.ilyaran@gmail.com>?subject=[ermarian.net] (enter subject)&body=
--
Sent by ermarian.net contact link'))?>">Arancaytar</a>,
          &copy; 2006-2014, all rights reserved.
          No responsibility is taken for the content of external links, which are marked by
          <img src="images/external.png" alt="a blue arrow icon" />.
        </div>
        <!-- /copyright -->

      </div>
      <!-- /page-footer -->

    </div>
    <!-- /content -->
  </body>
</html>
