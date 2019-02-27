<?php

use Com\Tecnick\Barcode\Barcode as BarcodeGenerator;

function sirius_qr_url($url = '') {
	if (!$url) { $url = current_path(); }
	if (!preg_match('/^http/', $url)) {
		global $base_url;
		if (preg_match('/^\//', $url)) {
			$url = $base_url . $url;
		} else {
			$url = $base_url . '/' . $url;
		}
	}

	return sirius_qr_string($url);
}

function sirius_qr_string($string) {
	$generator = new BarcodeGenerator();
  $barcode = $generator->getBarcodeObj(
    variable_get('barcodes_type', 'QRCODE'),
    $string,
    variable_get('barcodes_width', 100),
    variable_get('barcodes_height', 100),
    variable_get('barcodes_color', '#000000'),
    array(
      variable_get('barcodes_padding_top', 0),
      variable_get('barcodes_padding_right', 0),
      variable_get('barcodes_padding_bottom', 0),
      variable_get('barcodes_padding_left', 0),
    )
  );

  return '<code>' . $barcode->getSvgCode() . '</code>';
}