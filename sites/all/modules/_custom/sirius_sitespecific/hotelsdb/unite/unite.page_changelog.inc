<?php

function unite_page_changelog() {
	$releases['hotelsdb/v1.1'] = array(
		'date' => '2025-06-21',
		'changes' => array(
			'Add Y/N fields for "Hotel?", "Casino?", "Food Service?"',
			'Restrict tabs for hotel amenities, FairHotel, food service, and casino based on new Y/N fields',
			'Rename "Amenities" to "Hotel Amenities"',
			'Move "rooms" and "niche" fields to the "Hotel Amenities" group',
			'Delete the "Airport" field',
			'Change format to the Actors list',
			'Relationships now have a field for "Terminal / Floor"',
			'Added two new relationship types, "Colocated" and "Colacation Host"',
			'Change dropdown options for "casino type" field',
		),
	);

	$html = '';
	foreach ($releases as $key => $record) {
		$html .= "<h2>$key ($record[date])</h2>";
		$html .= '<ul>';
		foreach ($record['changes'] as $change) {
			$html .= '<li>' . $change . '</li>';
		}
		$html .= '</ul>';
	}

	return $html;
}
