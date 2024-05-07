<?php

function report_cache_page_report_list($report_type, $info) {
	$generator = $info['generator'];

	$report_ids = $generator->search();
	
	if (!$report_ids) {
		drupal_set_message(t('No reports of type @report_type have been generated yet.', array('@report_type' => $report_type)));
	}

	$rows = array();
	foreach ($report_ids as $report_id => $parameters) {
		
		$tools = '';
		if ($info['render_callbacks']) {
			foreach ($info['render_callbacks'] as $name => $callback) {
				$tools .= l(
					$name, 
					"sirius/reports/$report_type", 
					array(
						'attributes' => array('class' => array('tool-button')), 
						'query' => array(
							'report_id' => $report_id, 
							'action' => 'render',
							'callback-name' => $name,
						)
					)
				);
			}
		}

		if ($info['operations']) {
			$tools .= l(t('Operations'), "sirius/reports/$report_type", array('attributes' => array('class' => array('tool-button')), 'query' => array('report_id' => $report_id, 'action' => 'operations')));
		}
		$tools .= l(t('View Source Data'), "sirius/reports/$report_type", array('attributes' => array('class' => array('tool-button')), 'query' => array('report_id' => $report_id, 'action' => 'preview')));
		$tools .= l(t('Download Source Data'), "sirius/reports/$report_type", array('attributes' => array('class' => array('tool-button')), 'query' => array('report_id' => $report_id, 'action' => 'download')));
		$tools .= l(t('Purge'), "sirius/reports/$report_type", array('attributes' => array('class' => array('tool-button')), 'query' => array('report_id' => $report_id, 'action' => 'purge')));

		$parameters_render = $info['generator']->render_parameters($parameters);

		$row = array();
		$row[] = $report_id;
		if ($info['title_callback']) {
			$has_title_column = TRUE;
			$row[] = $info['title_callback']($report_type, $report_id);
		}
		if ($parameters_render) { 
			$has_parameters_column = TRUE;
			$row[] = $parameters_render;
		}

		$row[] = $tools;
		$rows[] = $row;
	}

	// $html .= '<h2>' . $info['name'] . '</h2>';
	$html .= $info['description'];

	$header = array();
	$header[] = t('Report ID');
	if ($has_title_column) { $header[] = t('Title'); }
	if ($has_parameters_column) { $header[] = t('Parameters'); }
	$header[] = t('Tools');


	$html .= sirius_table_render($header, $rows);

	if ($preview) {
		$html .= '<hr>';
		$html .= $preview;
	}

	$path = $info['path'];
	if (!$path) { 
		$path = 'sirius/reports/' . $report_type . '/form';
	}
	$html .= l(t('Generate a new report'), $path, array('attributes' => array('class' => array('tool-button'))));

	return $html;
}
