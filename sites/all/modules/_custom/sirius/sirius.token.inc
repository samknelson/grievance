<?php

/**
 * A simple wrapper to render text with tokens
 */

function sirius_text_render($text, $tokens, $sanitize = TRUE) {
  $aliases = array();

  foreach ($tokens as $key => $value) {
    if (preg_match('/_node$/', $key)) {
      $aliases[$key] = 'node';
    } 
  }

  foreach ($tokens as $key => $value) {
    if (preg_match('/_user$/', $key)) {
      $aliases[$key] = 'user';
    } 
  }

  foreach ($tokens as $key => $value) {
    if (preg_match('/_contact$/', $key)) {
      $aliases[$key] = 'contact';
    } 
  }

  $text_rendered = token_replace(
    $text, 
    $tokens,
    array(
      'clear' => TRUE,
      'aliases' => $aliases, 
      'sanitize' => $sanitize,
    )
  );

  return $text_rendered;
}

/**
 * Implements hook_tokens
 */

function sirius_tokens($type, $tokens, array $data = array(), array $options = array()) {
  $replacements = array();

  if ($type == 'sirius') {
    foreach ($tokens as $name => $original) {
      list ($array_name, $field_name, $subfield_name, $subsubfield_name) = explode(':', $name);
      if ($subsubfield_name) {
        $replacements[$original] = $data[$type][$array_name][$field_name][$subfield_name][$subsubfield_name];
      } else if ($subfield_name) {
        $replacements[$original] = $data[$type][$array_name][$field_name][$subfield_name];
      } else {
        $replacements[$original] = $data[$type][$array_name][$field_name];
      }
    }
  }

  return $replacements;
}

/**
 * Implements hook_tokens_alter
 */

function sirius_tokens_alter(array &$replacements, array $context) {
  if (isset($context['options']['aliases']) && in_array($context['type'], array_keys($context['options']['aliases']))) {
    $new_data = array($context['options']['aliases'][$context['type']] => $context['data'][$context['type']]);
    $replacements += token_generate($context['options']['aliases'][$context['type']], $context['tokens'], $new_data, $context['options']);
  }
}
