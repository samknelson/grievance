<?php

function sirius_service_page_config() {
	$html .= '<p>' . t('Use these tools to configure this site as either a client or a server.');

	$html .= '<h2>' . t('Server Settings') . '</h2>';
	$html .= sirius_fakerender_field('Component Available', sirius_domain_component_enabled('service:server') ? 'Yes' : 'No');
	$html .= sirius_fakerender_field('Active', sirius_service_server_active() ? 'Yes' : 'No');
	$html .= l('Server Settings', 'sirius/service/config/server', array('attributes' => array('class' => array('tool-button'))));

	$html .= '<h2>' . t('Client Settings') . '</h2>';
	$html .= sirius_fakerender_field('Component Available', sirius_domain_component_enabled('service:client') ? 'Yes' : 'No');
	$html .= sirius_fakerender_field('Active', sirius_service_client_active() ? 'Yes' : 'No');
	$html .= sirius_fakerender_field('Proxy', sirius_proxy_active() ? 'Yes' : 'No');
	$html .= l('Client Settings', 'sirius/service/config/client', array('attributes' => array('class' => array('tool-button'))));

	return $html;
}