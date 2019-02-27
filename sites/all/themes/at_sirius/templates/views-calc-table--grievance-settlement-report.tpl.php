<?php
/**
 * Overridding this theme just because views_calc_table doesn't give us an option to skip zebra, and zebra conflicts with the table_trash
 * dammit

 */
if (empty($rows) && empty($totals)) {
  return;
}
?>
<table class="<?php print $class; ?>">
  <?php if (!empty($title)) : ?>
    <caption><?php print $title; ?></caption>
  <?php endif; ?>
  <thead>
    <tr>
      <?php foreach ($header as $field => $label): ?>
        <th class="views-field views-field-<?php print $fields[$field]; ?> <?php print $options['info'][$field]['align'] ?>">
          <?php print $label; ?>
        </th>
      <?php endforeach; ?>
    </tr>
  </thead>
  <tbody>
    <?php foreach ($rows as $count => $row): ?>
      <tr >
        <?php foreach ($row as $field => $content): ?>
          <td class="views-field views-field-<?php print $fields[$field]; ?>  <?php print $options['info'][$field]['align'] ?>">
            <?php print $content; ?>
          </td>
        <?php endforeach; ?>
      </tr>
    <?php endforeach; ?>
  </tbody>
  <tfoot>
    <?php foreach ($sub_totals as $type => $row): ?>
      <tr class="view-subfooter-number">
        <?php foreach ($row as $field => $content): ?>
          <td class="view-subfooter views-field views-field-<?php print $fields[$field]; ?>  <?php print $options['info'][$field]['align'] ?>">
            <?php print $content; ?>
          </td>
        <?php endforeach; ?>
      </tr>
    <?php endforeach; ?>
    <?php foreach ($totals as $type => $row): ?>
      <tr class="view-footer-number">
        <?php foreach ($row as $field => $content): ?>
          <td class="view-footer views-field views-field-<?php print $fields[$field]; ?>  <?php print $options['info'][$field]['align'] ?>">
            <?php print $content; ?>
          </td>
        <?php endforeach; ?>
      </tr>
    <?php endforeach; ?>
  </tfoot>
  
</table>
