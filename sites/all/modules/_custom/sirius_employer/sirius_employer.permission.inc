<?php

/***
 * Implements hook_permission
 */

function sirius_employer_permission() {
  return array(
    'sirius employer admin' => array(
      'title' => t('Sirius Employer Admin'), 
      'description' => t('Administer employers.'),
    ),
    'sirius employer user manager' => array(
      'title' => t('Sirius Employer User Manager'), 
      'description' => t('Create or disable other users in your own shop.'),
    ),
    'sirius employer contact manager' => array(
      'title' => t('Sirius Employer Contact Manager'), 
      'description' => t('Administer shop contacts.'),
    ),
    'sirius employer labor projection manager' => array(
      'title' => t('Sirius Employer Labor Projection Manager'), 
      'description' => t('View and edit labor projections in your own shop.'),
    ),
  );
}
