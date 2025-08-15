<?php

function sirius_banner_config() {
  $form = array();

  $form['sirius_banner_name'] = array(
    '#type' => 'textarea',
    '#title' => t('Site Name'),
    '#default_value' => variable_get('sirius_banner_name', ''),
    '#description' => t('May contain HTML'),
  );

  $form['sirius_banner_text'] = array(
    '#type' => 'textarea',
    '#title' => t('Banner Text'),
    '#default_value' => variable_get('sirius_banner_text', ''),
    '#description' => t('Usually contact information. May contain HTML.'),
  );

	$form['sirius_banner_css'] = array(
		'#title' => t('CSS'),
		'#type' => 'textarea',
		'#default_value' => variable_get('sirius_banner_css', ''),
		'#description' => t('Enter any additional CSS directives desired for styling the site.'),
	);

	$form['sirius_banner_logo'] = array(
		'#title' => t('Logo'),
		'#type' => 'managed_file',
		'#default_value' => variable_get('sirius_banner_logo', ''),
		'#upload_location' => 'public://sirius/banner/logo',
	);

	$form['sirius_banner_homelink'] = array(
		'#title' => t('Home Link?'),
		'#type' => 'select',
		'#default_value' => variable_get('sirius_banner_homelink', 'No'),
		'#options' => array(
			'No' => t('No'),
			'Yes' => t('Yes'),
		),
		'#description' => t('Show a link to "Home" in the banner? (The site title also always links to home.)'),
	);

	$form = system_settings_form($form);
	$form['#submit'][] = 'sirius_banner_config_submit';	
  return $form;
}


function sirius_banner_config_submit($form, &$form_state) {
	if (isset($form_state['values']['sirius_banner_logo'])) {
		$file = file_load($form_state['values']['sirius_banner_logo']);
		if ($file) {
			$file->status = FILE_STATUS_PERMANENT;
			file_save($file);
			variable_set('sirius_banner_logo', $file->fid);
			file_usage_add($file, 'sirius_logo', 'sirius_logo', 1);
		}
	}
	else {
		$file = file_load(variable_get('sirius_banner_logo', ''));
		if ($file->fid) {
			file_delete($file, TRUE);
		}
	}
}

function sirius_login_config() {
  $form = array();

  $form['sirius_login_intro'] = array(
    '#type' => 'text_format',
    '#title' => t('Intro Text'),
    '#default_value' => sirius_richtext_handle_input(variable_get('sirius_login_intro', '')),
    '#description' => t('Enter the introductory text that should appear before the site\'s login form. May contain HTML.'),
  );

  $form['sirius_login_outro'] = array(
    '#type' => 'text_format',
    '#title' => t('Outro Text'),
    '#default_value' => sirius_richtext_handle_input(variable_get('sirius_login_outro', '')),
    '#description' => t('Enter the text that should appear BELOW the site\'s login form. May contain HTML.'),
  );

  $form['sirius_login_path'] = array(
    '#type' => 'textfield',
    '#title' => t('Login Path'),
    '#default_value' => variable_get('sirius_login_path', ''),
    '#description' => t('Enter the Drupal path to login. This is probably only relevant if you want to encourage users to login as saml/login.'),
  );

  $form['sirius_login_account_url'] = array(
    '#type' => 'textfield',
    '#title' => t('Account URL'),
    '#default_value' => variable_get('sirius_login_account_url', ''),
    '#description' => t('Enter the account URL. This is only appropriate if you want to send people to an offsite link, such as https://mysite.okta.com/enduser/settings'),
  );

	$form = system_settings_form($form);
  return $form;
}

function sirius_form_user_login_alter(&$form, &$form_state, $form_id) {
	$intro = sirius_richtext_handle_input(variable_get('sirius_login_intro', ''));
	$outro = sirius_richtext_handle_input(variable_get('sirius_login_outro', ''));

	if ($intro) {
		$form['intro'] = array('#markup' => '<div class="sirius_login_intro">' . $intro . '</div>', '#weight' => -1000);
	}

	if ($outro) {
		$form['outro'] = array('#markup' => '<div class="sirius_login_intro">' . $outro . '</div>', '#weight' => 1000);
	}
}

function sirius_banner_render() {
	global $user;
  $domain = sirius_domain();
	$html = '<div class="user-links">';

		if ($user->uid) 
		{
		  $user_full = user_load($user->uid);
		  $html .= '<div class="user-links-content">';

			  $html .= 'Welcome, ';
			  $html .= $user->name;
				$html .= "&nbsp;&nbsp;&nbsp;&nbsp;";

				if (variable_get('sirius_banner_homelink', 'No') == 'Yes') {
					$html .= l(t('home'), '<front>');
					$html .= ' / ';
				}

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
			    if (drupal_valid_path('node/' . $shop_node->nid)) {
			    	$html .= l($shop_node->title, 'node/' . $shop_node->nid);
			    } else {
			    	$html .= $shop_node->title;
			    }
			    $html .= ' / ';
			    $contract_uri = $shop_node->field_grievance_contract['und'][0]['uri'];
			    if ($contract_uri) {
			      $contract_url = file_create_url($contract_uri);
			      $html .= l(t('contract (pdf)'), $contract_url, array('attributes' => array('target'=>'_blank')));
			      $html .= ' / ';
			    }
			  }

			  if (variable_get('sirius_login_account_url')) {
			  	$html .= '<a href="' . variable_get('sirius_login_account_url') . '">' . t('account') . '</a> / ';
			  } else if (user_access('sirius edit own user')) {
			    $html .= l(t('account'),'user/'.$user->uid.'/edit');
			    $html .= ' / ';
			  }


			  $html .= l(t("logout"),"user/logout");
		  $html .= '</div>'; // user-links-content
	  }
	  else 
	  {

	    $html .= '<div class="user-links-content anon-user-links-content">';
				if (variable_get('sirius_banner_homelink', 'No') == 'Yes') {
					$html .= l(t('home'), '<front>');
					$html .= ' / ';
				}
				$login_path = variable_get('sirius_login_path', '');
				if (!$login_path) { $login_path = 'user/login'; }
	    	$html .= l("login", $login_path);
	    $html .= '</div>'; // user-links-content
	  }

		$html .= '<div class="user-links-text">';
			$html .= variable_get('sirius_banner_text', '');
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
				$html .= variable_get('sirius_banner_name', ''); 
			  if ($domain) {
			    $html .= ' / ';
			    $html .= $domain->title;
			  }
				$html .= '</span>';
			$html .= '</a>';
		$html .= '</div>'; // inner
	$html .= '</div>'; // banner-wrapper

	drupal_alter('sirius_banner_render_html', $html);

	return $html;
}