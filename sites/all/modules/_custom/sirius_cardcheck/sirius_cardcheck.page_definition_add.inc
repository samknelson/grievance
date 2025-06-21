<?php

function sirius_cardcheck_page_definition_add() {
	sirius_backlink_add('sirius/cardcheck/list', array('title' => 'Card Check list'));
	return drupal_get_form('sirius_cardcheck_definition_form');
}

