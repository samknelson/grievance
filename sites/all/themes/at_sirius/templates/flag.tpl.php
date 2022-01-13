<?php if ($needs_wrapping_element): ?>
  <div class="flag-outer flag-outer-<?php print $flag_name_css; ?>">
<?php endif; ?>
<span class="<?php print $flag_wrapper_classes; ?>">
  <?php if ($link_href): ?>
    <a href="<?php print $link_href; ?>" title="<?php print $link_title; ?>" class="<?php print $flag_classes ?> sirius_quickaction_link" rel="nofollow"><span class="sirius_quickaction_link_inner"><?php print $link_text; ?></a></span><span class="flag-throbber">&nbsp;</span>
  <?php else: ?>
    <span class="<?php print $flag_classes ?>"><span class="sirius_quickaction_link_inner"><?php print $link_text; ?></span></span>
  <?php endif; ?>
  <?php if ($after_flagging): ?>
    <span class="<?php print $message_classes; ?>">
      <?php print $message_text; ?>
    </span>
  <?php endif; ?>
</span>
<?php if ($needs_wrapping_element): ?>
  </div>
<?php endif; ?>
