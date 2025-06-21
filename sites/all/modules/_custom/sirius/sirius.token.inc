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

  if ($type == 'user') {
    foreach ($tokens as $name => $original) {
      list ($sirius, $type) = explode(':', $name);
      if ($sirius == 'sirius' && $type == 'signature') {
        $replacements[$original] = sirius_signature_render($data['user']);
      }
    }
  }

  if ($type == 'node') {
    foreach ($tokens as $name => $original) {
      list ($a, $b, $c, $d) = explode(':', $name);
      if ($a == 'sirius' && $b == 'esignature') {
        $which = $c;
        $format = $d;
        $json = sirius_jsonfield_util_node_json_get($data['node']);
        $esignature = sirius_jsonfield_util_esignature_get($json, $which);
        $replacements[$original] = sirius_esignature_render($esignature, $format, $data['node']->nid, $which);
      }

      if ($a == 'sirius' && $b == 'customfield') {
        $machine_name = $c;
        $path = preg_replace("/^$a:$b:$c/", '', $name);
        $replacements[$original] = sirius_customfield_value($data['node'], $machine_name, $path);
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

/**
 * Support for token failures. 
 * 
 * When a token "fails", the wrapper should also fail. 
 * 
 * This is a fairly vauge feature, and I'm not sure that I like it. The token replacement tool does not have a way to 
 * implement the logic: "If a token is missing, then don't send the message at all." That should be handled
 * by the wrapper, but we don't know, in advance, which tokens will appear in the message. So this is sort of a
 * shoddy ad-hoc exception management tool, implemented for a specific purpose.
 * 
 * - For example, if you are sending a bulk message, and the phone number is missing, that's not a failure, just an 
 * empty value. 
 * 
 * - But if you ask for a COBRA rate, and the rate fails to generate, then that's a failure, and you shouldn't 
 * send the messaage.
 */

function sirius_token_failures($action = 'fetch', $name = NULL, $value = NULL) {
  static $failures;
  if (!isset($failures)) { $failures = array(); }

  if ($action == 'fetch') { return $failures; }
  if ($action == 'clear') { $failures = array(); }
  if ($action == 'set') { $failures[$name] = $value; }

  return $failures;
}

function sirius_token_failures_clear() { return sirius_token_failures('clear'); }

function sirius_token_failures_fetch() { return sirius_token_failures('fetch'); }

function sirius_token_failures_set($name, $value) { return sirius_token_failures('set', $name, $value); }