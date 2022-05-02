<?php

function unite_page_home() {
	drupal_add_css(drupal_get_path('module', 'unite') . '/css/unite.css', 'file');

	$html .= '<div class="unite_home_col">';

	$html .= '<div class="unite_home_block unite_home_col_left">';
	$html .= '<h2>' . t('Bookmarks') . '</h2>';
	$html .= views_embed_view('bookmarked', 'block');
	$html .= '</div>';

	$html .= '<div class="unite_home_block unite_home_col_left">';
	$html .= '<h2>' . t('Saved Searches') . '</h2>';
	$html .= mightysearch_saved_render_home();
	$html .= '</div>';

	$html .= '</div>';

	$html .= '<div class="unite_home_col">';

	$html .= '<div class="unite_home_block unite_home_col_right">';
	$html .= '<h2>' . t('Recently Viewed') . '</h2>';
	$html .= views_embed_view('recently_viewed', 'block');
	$html .= '</div>';

	$html .= '<div class="unite_home_block">';
	$html .= '<h2>' . t('Recently Updated') . '</h2>';
	$html .= views_embed_view('recently_updated', 'block_1');
	$html .= '</div>';

	$html .= '</div>';

	return $html;
}