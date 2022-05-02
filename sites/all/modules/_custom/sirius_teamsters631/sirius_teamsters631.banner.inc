<?php

/**
 * Implements hook_sirius_banner_render_html_alter
 */

function sirius_teamsters631_sirius_banner_render_html_alter(&$html) {
	$which = sirius_teamsters631_banner_which();
	if (!$which) { return; }

	$html = sirius_teamsters631_render_banner($which);
}

function sirius_teamsters631_banner_which() {
	if (preg_match('/631train.com/', $_SERVER['SERVER_NAME'])) { return 'tc'; }
	if (preg_match('/teamsters631.com/', $_SERVER['SERVER_NAME'])) { return 'tc'; }

	$domain = sirius_domain();
	if ($domain->title == 'Teamsters 631 Training Center') { return 'tc'; }
	if ($domain->title == 'Teamsters 631 Union Hall') { return 'uh'; }

	return NULL;
}

function sirius_teamsters631_banner_switch_url() {
	// global $base_url;
	// drupal_set_message($base_url);
	$which = sirius_teamsters631_banner_which();
	$url = '/teamsters631/switch/leave';
	$url .= "?ts=" . time();
	$url .= '&which=' . $which;
	return $url;
}

function sirius_teamsters631_render_banner($which) {
	if ($which != 'tc' && $which != 'uh') {
		return 'Showing banner: Unknown site';
	}


	$html = '';

	if (variable_get('sirius_teamsters631_tophat', '') == 'Yes') {
		grievance_classes_array("teamsters631-$which");
		drupal_add_css(drupal_get_path('module', 'sirius_teamsters631') . '/css/banner.css');
		$html .= '<div id="page-tophat">';
		$switch_url = sirius_teamsters631_banner_switch_url();
		if ($which == 'tc') {
			$html .= "<a href=\"$switch_url\">Viewing: <strong>Training Center</strong>&nbsp;&nbsp;&nbsp;&nbsp;Switch to: Union Hall</a>";
		} else {
			$html .= "<a href=\"$switch_url\">Viewing: <strong>Union Hall</strong>&nbsp;&nbsp;&nbsp;&nbsp;Switch to: Training Center</a>";
		}
		$html .= '</div>';
	}

	global $user;
	$html .= '<div class="user-links">';

		if ($user->uid) 
		{
		  $user_full = user_load($user->uid);
		  $html .= '<div class="user-links-content">';

			  $html .= 'Welcome, ';
			  $html .= $user->name;
				$html .= "&nbsp;&nbsp;&nbsp;&nbsp;";

			  $domain = sirius_domain();
			  if (user_access('sirius domain switch')) {
			    $html .= l('domain','sirius/domain/switch');
			    $html .= ' / ';
			  }

			  if (user_access('sirius bookmark')) {
			    $html .= l('bookmarks','sirius/bookmarked');
			    $html .= ' / ';
			  }

			  if ($user_full->field_grievance_shops['und'][0]['target_id'] && !$user_full->field_grievance_shops['und'][1]['target_id']) {
			    $shop_nid = $user_full->field_grievance_shops['und'][0]['target_id'];
			    $shop_node = node_load($shop_nid);
			    // print l($shop_node->title, 'node/' . $shop_node->nid);
			    $html .= $shop_node->title;
			    $html .= ' / ';
			    $contract_uri = $shop_node->field_grievance_contract['und'][0]['uri'];
			    if ($contract_uri) {
			      $contract_url = file_create_url($contract_uri);
			      $html .= l(t('contract (pdf)'), $contract_url, array('attributes' => array('target'=>'_blank')));
			      $html .= ' / ';
			    }
			  }

			  if (user_access('sirius edit own user')) {
			    $html .= l(t('account'),'user/'.$user->uid.'/edit');
			    $html .= ' / ';
			  }
			  $html .= l(t("logout"),"user/logout");
		  $html .= '</div>'; // user-links-content
	  }
	  else 
	  {
	    $html .= '<div class="user-links-content anon-user-links-content">';
	    	$html .= l("login","user/login");
	    $html .= '</div>'; // user-links-content
	  }

		$html .= '<div class="user-links-text">';
			if ($which == 'uh') {
				$html .= 'Questions? Contact the Union Hall at <a href="phone:+17024305048">(702) 430-5048</a>';
			} else {
				$html .= 'Questions? Contact the Training Center at <a href="phone:+17026510344">(702) 651-0344</a>';
			}
		$html .= '</div>'; // user-links-text
	$html .= '</div>'; // user-links

	$html .= '<div class="banner-wrapper">';
		$html .= '<div class="inner">';
			$html .= '<a href="/">';
			  $logo_file = file_load(variable_get('sirius_banner_logo', ''));
			  if ($logo_file) {
			    $logo_url = file_create_url($logo_file->uri);
			    global $base_url;
			    $logo_url = str_replace($base_url, '', $logo_url);
			    $html .= '<span class="banner-logo-wrapper">';
			    	$html .= "<img class=\"banner-logo\" src=\"$logo_url\">";
			    $html .= '</span>';
			  }
			  $html .= '<span class="banner-name-wrapper">';
			  if ($which == 'tc') {
			  	$html .= 'Teamsters 631 Training Center'; 
			  } else if ($which == 'uh') {
			  	$html .= 'Teamsters 631 Union Hall'; 
			  } else {
			  	$html .= 'Teamsters 631';
			  }
				$html .= '</span>';
			$html .= '</a>';
		$html .= '</div>'; // inner
	$html .= '</div>'; // banner-wrapper

	return $html;
}