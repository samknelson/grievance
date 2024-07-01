<?php

class Sirius_Ledger_EA extends Sirius_Singleton {
  public function search($search_params) {
    $domain_nid = sirius_domain_nid();

    $sql_args = array();
    $sql = "select distinct nid from node ";
    $sql .= "left join field_data_field_sirius_domain on field_data_field_sirius_domain.entity_type = 'node' and field_data_field_sirius_domain.entity_id = node.nid ";
    $sql .= "left join field_data_field_sirius_category on field_data_field_sirius_category.entity_type = 'node' and field_data_field_sirius_category.entity_id = node.nid ";
    $sql .= "left join field_data_field_sirius_notes on field_data_field_sirius_notes.entity_type = 'node' and field_data_field_sirius_notes.entity_id = node.nid ";
    $sql .= "left join field_data_field_sirius_json on field_data_field_sirius_json.entity_type = 'node' and field_data_field_sirius_json.entity_id = node.nid ";
    $sql .= "where node.type = 'sirius_log' ";
    $sql .= "and field_sirius_category_value = 'ledger:ea' ";
    if ($search_params['key']) {
      $sql .= "and field_sirius_notes_value = :key ";
      $sql_args[':key'] = $search_params['key'];
    }
    if ($domain_nid) { 
      $sql .= "and field_sirius_domain_target_id = :domain_nid ";
      $sql_args[':domain_nid'] = $domain_nid;
    } else {
      $sql .= "and field_sirius_domain_target_id is null ";
    }
    $limit = $search_params['limit'];
    if (!$limit) { $limit = 1000; }
    $sql .= "order by nid desc ";
    $sql .= "limit $limit";

    $stmt = sirius_sql_query($sql, $sql_args);
    $ea_nids = array();
    while ($hr = $stmt->fetchAssoc()) { $ea_nids[] = $hr['nid']; }
    if ($search_params['nids_only']) { return $ea_nids; }
    return node_load_multiple($ea_nids);
  }

  public function balance($ea_node) {
    $participant_node = $this->participant_node($ea_node);
    $account_node = $this->account_node($ea_node);
    $params = array();
    $params['ledger_account'] = $account_node->nid;
    $params['ledger_participant'] = $participant_node->nid;
    $params['balance'] = TRUE;
    return sirius_ledger_ar_fetch($params);
  }

  public function fetch($entity_nid, $account_nid, $create = FALSE) {
    if (!$entity_nid) { return NULL; }
    if (!$account_nid) { return NULL; }

    $search_params = array();
    $search_params['key'] = $this->key($entity_nid, $account_nid);
    $search_params['limit'] = 1;
    $ea_nodes = $this->search($search_params);
    $ea_node = reset($ea_nodes);
    if ($ea_node) { return $ea_node; }

    if (!$create) { return FALSE; }

    $json = array();
    $json['ledger_ea']['entity_nid'] = $entity_nid;
    $json['ledger_ea']['account_nid'] = $account_nid;
    $json['ledger_ea']['entity_name'] = sirius_node_title($entity_nid);
    $json['ledger_ea']['account_name'] = sirius_node_title($account_nid);

    $ea_node = sirius_log(
      'ledger:ea',
      'active',
      $this->title($entity_nid, $account_nid),
      '',
      $this->key($entity_nid, $account_nid),
      array($entity_nid, $account_nid),
      NULL,
      $json
    );

    $ea_node->title = $this->title($entity_nid, $account_nid);
    node_save($ea_node);

    return $ea_node;
  }

  public function title($entity_nid, $account_nid) {
    return  sirius_node_title($entity_nid) . ' :: ' . sirius_node_title($account_nid);
  }

  public function key($entity_nid, $account_nid) {
    return "$entity_nid/$account_nid";
  }

  public function account_node($ea_node) {
    return sirius_log_handler($ea_node, 'sirius_ledger_account');
  }

  public function worker_node($ea_node) {
    return sirius_log_handler($ea_node, 'sirius_worker');
  }

  public function employer_node($ea_node) {
    return sirius_log_handler($ea_node, 'grievance_shop');
  }

  public function entity_node($ea_node) {
    $entity_node = $this->worker_node($ea_node);
    if (!$entity_node) { $entity_node = $this->employer_node($ea_node); }
    return $entity_node;
  }

  /**
   * Get the ledger participant associated with the entity. 
   * This is usually the entity itself, but for workers, it's the contact.
   */

  public function participant_node($ea_node) {
    $entity_node = $this->entity_node($ea_node);
    if (!$entity_node) { return NULL; }
    if ($entity_node->type == 'sirius_worker') {
      return node_load($entity_node->field_sirius_contact['und'][0]['target_id']);
    }
    return $entity_node;
  }

  public function backlinks_from_node_alter(&$links, $node) {
    if ($node->type == 'sirius_log' && $node->field_sirius_category['und'][0]['value'] == 'ledger:ea') {
      $account_node = $this->account_node($node);
      $entity_node = $this->entity_node($node);
      unset($links['node/' . $account_node->nid . '/sirius_ledger_account_log']);
      unset($links['node/' . $entity_node->nid . '/' . $entity_node->type . '_log']);
      $links['node/' . $entity_node->nid . '/sirius_worker_ledger'] = array(
        'name' => sirius_content_type_name($entity_node->type),
        'title' => $entity_node->title,
      );
    }

    if ($node->type == 'sirius_payment') {
      $payer_nid = $node->field_sirius_payer['und'][0]['target_id'];
      if ($payer_nid) {
        $worker_nid = sirius_worker_get_by_contact_nid($payer_nid, TRUE);
        if ($worker_nid) {
          $links["node/$worker_nid/sirius_worker_ledger"] = array('name' => 'Worker', 'title' => sirius_backlink_title($worker_nid));
        } else {
          $links["node/$payer_nid"] = array('name' => 'Payer', 'title' => sirius_backlink_title($payer_nid));
        }
      }

      $ea_node = $this->ea_node_from_payment($node);
      if ($ea_node) {
        $links['node/' . $ea_node->nid . '/sirius_ea_payments'] = array('name' => 'Account', 'title' => sirius_backlink_title($ea_node->nid));
      }
    }
  }

  function ea_node_from_payment($payment_node) {
    $participant_node = node_load($payment_node->field_sirius_payer['und'][0]['target_id']);
    if ($participant_node->type == 'sirius_contact') {
      $entity_node =  sirius_worker_get_by_contact_nid($participant_node->nid);
    } else {
      $entity_node = $participant_node;
    }
    if (!$entity_node) { return NULL; }

    $account_nid = $payment_node->field_sirius_ledger_account['und'][0]['target_id'];
    if (!$account_nid) { return NULL; }

    return $this->fetch($entity_node->nid, $account_nid, TRUE);
  }
}