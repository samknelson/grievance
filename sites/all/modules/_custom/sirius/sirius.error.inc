<?php 

// Suppress "Deprecated" and "Notice" errors
ini_set('error_reporting', ini_get('error_reporting') & ~E_NOTICE & ~E_DEPRECATED);

/**
 * PHP8 converted "undefined array index" from a NOTICE to a WARNING. 
 * 
 * The "right" thing to do would probably be to check every array entry for existence. But
 * that would 
 * 
 * A) require remediating a tremendous amount of code, and
 * B) go against my personal aesthetic
 * 
 * So instead, with eyes wide open and full awareness that this is improper behaviore, we set our own error handler
 * that looks at the error text and decides if it should be a WARNING or a NOTICE.
 */

// This function doesn't exist in php7.
function sirius_error_str_starts_with($haystack, $needle) {
  return (string)$needle !== '' && strncmp($haystack, $needle, strlen($needle)) === 0;
}

function sirius_error_handler($error_level, $message, $filename, $line) {
  // If this doesn't look like an important WARNING, then throw it away.
  if ($message && (
      sirius_error_str_starts_with($message, 'Trying to access array offset') ||
      sirius_error_str_starts_with($message, 'Undefined array') || 
      sirius_error_str_starts_with($message, 'Attempt to read property "') ||
      sirius_error_str_starts_with($message, 'Undefined variable') ||
      sirius_error_str_starts_with($message, 'Undefined property')
    ) 
  ) {
    // trigger_error($message, E_USER_NOTICE);
    return TRUE;
  } 

  // Fail through to Drupal's standard error handling.
  // 
  // Ideally we'd simply call _drupal_error_handler. But that function calls debug_backtrace(), 
  // which would always report sirius_error_handler() as its caller. So instead we copy and paste
  // the code here.
  // 
  // See errors.inc: _drupal_error_handler().

  if ($error_level & error_reporting()) {
    require_once DRUPAL_ROOT . '/includes/errors.inc';
    $types = drupal_error_levels();
    list($severity_msg, $severity_level) = $types[$error_level];
    $caller = _drupal_get_last_caller(debug_backtrace());

    if (!function_exists('filter_xss_admin')) {
      require_once DRUPAL_ROOT . '/includes/common.inc';
    }

    // We treat recoverable errors as fatal, and also allow fatal errors to be
    // explicitly triggered by drupal_trigger_fatal_error().
    _drupal_log_error(array(
      '%type' => isset($types[$error_level]) ? $severity_msg : 'Unknown error',
      // The standard PHP error handler considers that the error messages
      // are HTML. We mimic this behavior here.
      '!message' => filter_xss_admin($message),
      '%function' => $caller['function'],
      '%file' => $caller['file'],
      '%line' => $caller['line'],
      'severity_level' => $severity_level,
    ), $error_level == E_RECOVERABLE_ERROR || drupal_static('drupal_trigger_fatal_error'));
  }
}

// Globally set our new error handler.
set_error_handler('sirius_error_handler', E_WARNING);

