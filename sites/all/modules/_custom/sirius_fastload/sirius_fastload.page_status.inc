<?php

function sirius_fastload_page_status() {
	$content_types = sirius_fastload_content_types();

	$buffer_size = variable_get('sirius_fastload_buffer_size', 0);

	$rows = array();
	foreach ($content_types as $content_type) {
		$available = sirius_fastload_buffer_count($content_type, 'available');
		$used = sirius_fastload_buffer_count($content_type, 'used');
		$pct = 0;
		if ($buffer_size) {
			$pct = round(100 * $available / $buffer_size);
		}

		$row = array();
		$row[] = $content_type;
		$row[] = theme_progress_bar(array('percent' => $pct, 'message' => "$available / $buffer_size"));
		$row[] = number_format($buffer_size);
		$row[] = number_format($available);
		$row[] = number_format($used);

		$rows[] = $row;
	}

	$header = array(
		'Content Type',
		'% of Buffer Generated',
		'Buffer Size',
		'Available Nodes',
		'Used Nodes',
	);

	return theme_table(array('rows' => $rows, 'header' => $header, 'attributes' => array()));
}