<?php

/***
 * Implements hook_permission
 */

function grievance_worker_permission() {
  return array(
    'grievance worker lookup' => array(
      'title' => t('Grievance Worker Lookup'), 
      'description' => t('Permission to look up workers from the worker database'),
    ),
    'grievance worker edit' => array(
      'title' => t('Grievance Worker Edit'), 
      'description' => t('Permission to update existing workers via the grievance form'),
    ),
    'grievance worker create' => array(
      'title' => t('Grievance Worker Create'), 
      'description' => t('Permission to create new workers via the grievance form'),
    ),
  );
}
