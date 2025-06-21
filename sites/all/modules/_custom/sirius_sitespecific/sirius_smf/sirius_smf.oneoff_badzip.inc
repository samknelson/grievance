<?php

// while true ; do drush --user=1 php-eval "sirius_require('sirius_smf', 'sirius_smf.oneoff_badzip.inc'); sirius_smf_oneoff_badzip();" ; sleep 10; done

// drush --user=1 php-eval "sirius_require('sirius_smf', 'sirius_smf.oneoff_badzip.inc'); sirius_smf_oneoff_badzip();"

function sirius_smf_oneoff_badzip() {
  $last = variable_get('sirius_smf_oneoff_badzip', 0);

  $sql = "select count(*) as c from field_data_field_sirius_address ";
  $sql .= "where field_sirius_address_postal_code like '%000000%' ";
  $sql .= "and field_sirius_address_thoroughfare is null ";
  $sql .= "and entity_id > $last ";
  $stmt = db_query($sql, array());
  $hr = $stmt->fetchAssoc();
  $total = $hr['c'];
  print "Total: $total\n";

  $limit = 1000;
  $sql = "select * from field_data_field_sirius_address ";
  $sql .= "where field_sirius_address_postal_code like '%000000%' ";
  $sql .= "and field_sirius_address_thoroughfare is null ";
  $sql .= "and entity_id > $last ";
  $sql .= "order by entity_id limit $limit ";

  $stmt = db_query($sql, array());

  $nids = array();

  $count = 0;
  while ($hr = $stmt->fetchAssoc()) {
    $nids[] = $hr['entity_id'];
  }

  foreach ($nids as $nid) {
    ++$count;
    $node = node_load($nid);
    print "Checking $count / $limit of $total: $nid\n";
    $node = node_load($nid);
    unset($node->field_sirius_address['und'][0]);
    node_save($node);

    variable_set('sirius_smf_oneoff_end_of_month', $nid);
  }
}
