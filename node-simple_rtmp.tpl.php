<div id="node-<?php print $node->nid; ?>" class="node<?php if ($sticky) { print ' sticky'; } ?><?php if (!$status) { print ' node-unpublished'; } ?> clear-block">
  <?php if ($page == 0): ?>
  <h2><?php print $title; ?></h2>
  <?php endif; ?>
  <div class="content">
    <!-- Node Body -->
    <div class="clear-block">
      <?php print $content ?>
    </div>
    <!-- video image container -->
    <div class="simple_rtmp_video" id="video_<?php print $nid; ?>">
      <a href="#TB_inline?width=380&amp;height=300&amp;inlineId=container_<?php print $nid; ?>" class="thickbox">
      <?php print $video_icon; ?>    
      </a>
    </div>
    <!-- Hidden video div w/ js below -->
    <div id="container_<?php print $nid; ?>" style="display:block">
      <a href="http://www.macromedia.com/go/getflashplayer">Get the Flash Player</a> to see this player
      <br style="clear:all">
    </div>
  </div>
</div>