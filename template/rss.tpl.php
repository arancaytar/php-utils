<rss version="2.0">
   <channel>
      <title><?=$feed['title']?></title>
      <link><?=$feed['link']?></link>
      <description><?=$feed['description']?></description>
      <language><?=$feed['language']?></language>
      <pubDate><?=$feed['pubdate']?></pubDate>
      <lastBuildDate><?=$feed['pubdate']?></lastBuildDate>
      <docs>http://blogs.law.harvard.edu/tech/rss</docs>
      <generator>Ermarian Network</generator>
      <managingEditor><?=$feed['managingeditor']?></managingEditor>
      <webMaster><?=$feed['managingeditor']?></webMaster>

<?php foreach ($feed['items'] as $item) { ?>

      <item>
         <title><?=$item['title']?></title>
         <link><?=$item['link']?></link>
         <description><?=$item['description']?></description>
         <pubDate><?=$item['pubdate']?></pubDate>
         <guid><?=$item['guid']?></guid>
      </item>

<?php } ?>

   </channel>
</rss>
