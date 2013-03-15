<?='<?xml version="1.0" encoding="utf-8"?>'?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN"
        "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="en">
  <!-- 
    Template for Ermarian Network (http://ermarian.net/)
    Copyright Arancaytar <arancaytar@ermarian.net> 2006-2011.
  -->
  
  <head>
    <title><?=$raw_title ? $raw_title : "$title | $site_name"?></title>

    <!-- meta -->
    <meta name="author" lang="en" content="<?=$meta->author?>" />
    <meta name="description" lang="en" content="<?=$meta->description?>" />
    <meta name="keywords" lang="en" content="<?=$meta->keywords?>" />
    <meta http-equiv="Content-Type" content="application/xhtml+xml; charset=utf-8" />
    <?php if ($meta->extra) { print $meta->extra; } ?>

    <!-- /meta -->
    
    <base href="http://ermarian.net" />
    
    <!-- related documents -->
    <link rel="prev"  href="<?=$links->prev?>" />
    <link rel="next"  href="<?=$links->next?>" />
    <link rel="contents" href="<?=$links->index?>" />
    <link rel="start" href="<?=$links->main?>" />
    <link rel="help" href="<?=$links->about?>" />
    <!-- /related documents -->
    
    <?php if ($links->rss) { print $links->rss; } ?>

    <!-- styles and scripts -->
    <link rel="stylesheet" 
          type="text/css" 
          media="all" 
          href="style/default.css" />
    <link rel="stylesheet" 
          type="text/css" 
          media="screen" 
          href="style/screen.css" />
    <link rel="stylesheet" 
          type="text/css" 
          media="print" 
          href="style/print.css" />

    <?=$styles?>
    <?=$scripts?>

    <!-- /styles and scripts -->
  </head>
  
  <body>
    <div id="content">
      <!-- header -->
      <div class="filled-box head-bar" id="title">
        <h1><?=$site_name?></h1>
      </div>
      <!-- /header -->
      
      <!-- sidebar -->
      <div class="filled-box side-bar" id="navside">
        <h2 class="navside">Navigation</h2>
        <div id="menu"><?=$navigation?></div>
        <div class="filler">
        </div>
      </div>
      <!-- /sidebar -->
      
      <!-- main -->
      <div id="main">
        <div id="main2">
          <!-- ads -->
          <?=TEMPLATE_AD?>

          <!-- /ads -->
          
          <h2><?=$title?></h2>
          
          <!-- text -->
          <div id="page-inner">
            <?=$content?>
            
          </div>
          <!-- /text -->
          
          <!-- site validation -->
          <div class="filled-box" id="footer">
            <a href="http://validator.w3.org/check?uri=referer">
              <img src="images/validation/valid-xhtml10.png" alt="Valid XHTML 1.0 Strict" height="31" width="88" style="float:left" />
            </a>
            <a href="http://jigsaw.w3.org/css-validator/validator">
              <img style="border:0;width:88px;height:31px;float:left" src="images/validation/valid-css.png" alt="Valid CSS!" />
            </a>
            This page can be viewed in any standards-compliant browser.<br/>
            Recommended: 
            <a href="http://www.spreadfirefox.com/?q=affiliates&amp;id=96065&amp;t=54">Firefox</a>,  
            <a href="http://www.opera.com">Opera</a>, <a href="http://www.chromium.org//">Chromium</a>
          </div>
          <!-- /validation -->

          <!-- copyright -->
          <div class="filled-box" id="copyright">
            All content on this page, unless stated otherwise, is owned by 
            <a class="mail" title="arancaytar.ilyaran@gmail.com" href="mailto:&quot;Arancaytar Ilyaran&quot; &lt;arancaytar.ilyaran@gmail.com&gt;?subject=Enquiry about Ermarian Network site&amp;body=%0D--%0DSent by ermarian.net contact link">Arancaytar</a>, 
            &copy; 2006-2013, all rights reserved.
            No responsibility is taken for the content of external links, which are marked by 
            <img src="images/external.png" alt="a blue arrow icon" />.
          </div>
          <!-- /copyright -->

        </div>
      </div>
      <!-- /main -->
      
    </div>
    <!-- /content -->
  </body>
</html>
