<?php

/**
 * Implements hook_block_info
 */
 
function unite_block_info() {
  $blocks['unite_quicksearch'] = array(
    'info' => t('UNITE QuickSearch'),
    'cache' => DRUPAL_CACHE_PER_ROLE,
  );

  return $blocks;
}

/**
 * Implements hook_block_view
 */

function unite_block_view($delta = '') {
	if ($delta == 'unite_quicksearch') { 
		$block['subject'] = t('QuickSearch');
		$block['content'] = drupal_get_form('unite_quicksearch_form');
	}

  return $block;
}

