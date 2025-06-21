<?php

use Com\Tecnick\Barcode\Barcode as BarcodeGenerator;

function sirius_qr_url($url = '', $format = 'svg', $options = array()) {
	if (!$url) { $url = current_path(); }
	if (!preg_match('/^http/', $url)) {
		global $base_url;
		if (preg_match('/^\//', $url)) {
			$url = $base_url . $url;
		} else {
			$url = $base_url . '/' . $url;
		}
	}

	return sirius_qr_string($url, $format, $options);
}

function sirius_qr_string($string, $format = 'svg', $options = array()) {
	$generator = new BarcodeGenerator();
  $barcode = $generator->getBarcodeObj(
    variable_get('barcodes_type', 'QRCODE'),
    $string,
    variable_get('barcodes_width', $options['barcodes_width'] ? $options['barcodes_width'] : 100),
    variable_get('barcodes_height', $options['barcodes_height'] ? $options['barcodes_height'] : 100),
    variable_get('barcodes_color', $options['barcodes_color'] ? $options['barcodes_color'] : '#000000'),
    array(
      variable_get('barcodes_padding_top', 0),
      variable_get('barcodes_padding_right', 0),
      variable_get('barcodes_padding_bottom', 0),
      variable_get('barcodes_padding_left', 0),
    )
  );

  if ($format == 'svg') {
    return '<code>' . $barcode->getSvgCode() . '</code>';
  }

  if ($format == 'png') {
    return $barcode->getPngData();
  }
}