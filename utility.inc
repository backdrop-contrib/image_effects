<?php
/**
 * Utility form, conversion and rendering functions for image processes
 */



/**
 * Prepare a subform for displaying RGB fields
 *
 * Helper function to render a common element.
 */
function imagecache_rgb_form($action) {
  if($action['HEX'] && $deduced = hex_to_rgb($action['HEX'])) {
    $action = array_merge($action, $deduced);
  }
  $form = array('#theme' => 'canvasactions_rgb_form');
  $form['red'] = array( '#type' => 'textfield', '#title' => t('Red'), '#default_value' => $action['red'], '#size' => 3);
  $form['green'] = array( '#type' => 'textfield', '#title' => t('Green'), '#default_value' => $action['green'], '#size' => 3);
  $form['blue'] = array( '#type' => 'textfield', '#title' => t('Blue'), '#default_value' => $action['blue'], '#size' => 3);
  $form['HEX'] = array( '#type' => 'textfield', '#title' => t('HEX'), '#default_value' => $action['HEX'], '#size' => 7);

  return $form;
}

/**
 * Prepare a subform for displaying positioning fields
 *
 * Helper function to render a common element.
 */
function canvasactions_pos_form($action) {
  $form = array(
    #'#theme' => 'canvasactions_pos_form',
    'xpos' => array(
      '#type' => 'textfield',
      '#title' => t('X offset'),
      '#default_value' => isset($action['xpos']) ? $action['xpos'] : 'center',
      '#size' => 6,
      '#description' => t('Enter an offset in pixels or use a keyword: <em>left</em>, <em>center</em>, or <em>right</em>.'),
    ),
    'ypos' => array(
      '#type' => 'textfield',
      '#title' => t('Y offset'),
      '#default_value' => isset($action['ypos']) ? $action['ypos'] : 'center',
      '#size' => 6,
      '#description' => t('Enter an offset in pixels or use a keyword: <em>top</em>, <em>center</em>, or <em>bottom</em>.'),
    ),
  );
  return $form;
}

/**
 * Render the subform in a table
 */
function theme_canvasactions_rgb_form(&$form) {
  $header = array();
  $table = array();
  foreach (element_children($form) as $key) {
    $header[$key] = $form[$key]['#title'];
    unset($form[$key]['#title']);
    $table['field'][$key] = drupal_render($form[$key]);
  }
  $output .= theme('table', $header, $table);
  $output .= t('Enter colors in decimal, 0-255, or in HEX. If HEX is set, it will take priority.');
  $output .= drupal_render($form);
  return $output;
}

function theme_canvasactions_rgb($rgb) {
  if($rgb['HEX'] && $deduced = hex_to_rgb($rgb['HEX'])) {
    $rgb = array_merge($rgb, $deduced);
  }
  if ($rgb['red'] || $rgb['green'] || $rgb['blue']) {
    $output .= ' RGB:['. $rgb['red'] .', '. $rgb['green'] .', '. $rgb['blue'] .'] #'. $rgb['HEX'];
  }
  else {
    $output .= ' ' .t('Transparent');
  }
  return $output;
}



 /**
 * Decode an HTML hex-code into an array of R, G, and B values.
 * accepts these formats: (case insensitive) #ffffff, ffffff, #fff, fff
 */
function hex_to_rgb($hex) {
  $hex = trim($hex);
  // remove '#'
  if (substr($hex, 0, 1) == '#')
    $hex = substr($hex, 1) ;

  // expand short form ('fff') color
  if (strlen($hex) == 3) {
    $hex = substr($hex, 0, 1) . substr($hex, 0, 1) .
           substr($hex, 1, 1) . substr($hex, 1, 1) .
           substr($hex, 2, 1) . substr($hex, 2, 1) ;
  }

  if (strlen($hex) != 6)
    trigger_error('Error: Invalid color "'. $hex .'"') ;

  // convert
  $rgb['red'] = hexdec(substr($hex, 0, 2)) ;
  $rgb['green'] = hexdec(substr($hex, 2, 2)) ;
  $rgb['blue'] = hexdec(substr($hex, 4, 2)) ;

  return $rgb ;
}

