<?php

function unite_permission() {
  return array(
    'unite hotels user' => array(
      'title' => t('UNITE Hotels User'), 
      'description' => t('Use the hotels database'),
    ),
    'unite hotels admin' => array(
      'title' => t('UNITE Hotels Admin'), 
      'description' => t('Administer the hotels database'),
    ),
    'unite hotels acquire' => array(
      'title' => t('UNITE Hotels Acquire'), 
      'description' => t('Cause one actor to acquire another'),
    ),
  );	
}
