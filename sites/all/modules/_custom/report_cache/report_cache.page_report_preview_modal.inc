<?php

function report_cache_preview_modal($report_type, $report_id, $info) {
	$data = $_REQUEST['data'];
	$html = '<div class="report_cache_modal_content">';
	$html .= $info['generator']->render_modal($report_id, $info, $data);
	$html .= '</div>';
	return $html;
}