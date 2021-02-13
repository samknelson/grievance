<?php

/**
 * Uiltity function to handle input that might be either HTML markup from a plain-text field or from a "text_format" field.
 *
 * @param $value Either a text field containing HTML markup, or an associate array containing the two fields "value" and "format"
 * @return string A string containing HTML markup, suitable for storing in a database or rendering on a page.
 */

function sirius_richtext_handle_input($value) {
	if (is_array($value)) { 
		return check_markup($value['value'], $value['format']);
	} else { 
		return $value;
	}
}
