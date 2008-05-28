<?php
// $ID: $
/**
 * Helper functions for the text2canvas action for imagecache
 * 
 */

/**
 * Place text on top of the current canvas
 *
 * Implementation of imagecache_hook_form()
 *
 * @param $action array of settings for this action
 * @return a form definition
 */
function canvasactions_text2canvas_form($action) {
  $defaults = array(
    'size' => 12,
    'angle' => 0,
    'xpos' => left,
    'ypos' => bottom,
    'RGB' => array(
      'R' => 33,
      'G' => 33,
      'B' => 33,
    ),
    'fontfile' => 'MgOpenModernaBold.ttf',
    'text' => 'Hello World!',
  );
  $action = array_merge($defaults, (array)$action);
  $form = array(
    'size' => array(
      '#type' => 'textfield',
      '#title' => t('size'),
      '#default_value' => $action['size'],
      '#description' => t('Size: The font size. Depending on your version of GD, this should be specified as the pixel size (GD1) or point size (GD2).'),
      '#size' => 3,
    ),
    'angle' => array(
      '#type' => 'textfield',
      '#title' => t('angle'),
      '#default_value' => $action['angle'],
      '#description' => t('Angle: The angle in degrees, with 0 degrees being left-to-right reading text. Higher values represent a counter-clockwise rotation. For example, a value of 90 would result in bottom-to-top reading text.'),
      '#size' => 3,
    ),
    'xpos' => array(
      '#type' => 'textfield',
      '#title' => t('X offset'),
      '#default_value' => $action['xpos'],
      '#description' => t('Enter an offset in pixels or use a keyword: <em>left</em>, <em>center</em>, or <em>right</em>.'),
      '#size' => 6,
    ),
    'ypos' => array(
      '#type' => 'textfield',
      '#title' => t('Y offset'),
      '#default_value' => $action['ypos'],
      '#description' => t('Enter an offset in pixels or use a keyword: <em>top</em>, <em>center</em>, or <em>bottom</em>.'),
      '#size' => 6,
    ),
    'RGB' => imagecache_rgb_form($action['RGB']),
    'fontfile' => array(
      '#type' => 'textfield',
      '#title' => t('font file name'),
      '#default_value' => $action['fontfile'],
      '#description' => t('Font file name (font file is in the module or "%filedir" folder). Example: "arial.ttf".', array('%filedir' => file_directory_path())),
    ),
    'text' => array(
      '#type' => 'textarea',
      '#rows' => 7,
      '#title' => t('text'),
      '#default_value' => $action['text'],
      '#description' => t('The text string.'),
    )
  );
  $form['#validate']['canvasactions_text2canvas_validate'] = array() ;
  return $form;
}

function canvasactions_text2canvas_validate($form_values) {
  if (! $fontfile = canvasactions_find_font($form_values['fontfile']['#value'], TRUE)) {
    // Just warn, don't prevent
    drupal_set_message(t("Unable to confirm that the font %fontfile is available on your system", array('%fontfile' => $form_values['fontfile']['#value'])) );
  }
  else {
    drupal_set_message(t("Font was found at %fontfile", array('%fontfile' => $fontfile)));
  }
}

/**
 * Implementation of theme_hook() for imagecache_ui.module
 */
function theme_canvasactions_text2canvas($element) {
  $data = $element['#value'];
  return "<em><strong>". $data['text'] ."</strong></em>" ;
}

/**
 * Place the source image on the current background
 *
 * Implementation of hook_image()
 *
 * @param $image
 * @param $action
 */
function canvasactions_text2canvas_image(&$image, $action = array()) {
  $fontpath = canvasactions_find_font($action['fontfile']);

  // should calc position here

  // calc bounding box;
  $bb = imagettfbbox($action['size'], $action['angle'], $fontpath, $action['text'] );
  $width = max($bb[0], $bb[2], $bb[4], $bb[6]) - min($bb[0], $bb[2], $bb[4], $bb[6]);
  $height = max($bb[1], $bb[3], $bb[5], $bb[7]) - min($bb[1], $bb[3], $bb[5], $bb[7]);
  // use imagecache tool to parse keywords
  $x_ins = _imagecache_keyword_filter($action['xpos'], $image->info['width'], $width);
  $y_ins = _imagecache_keyword_filter($action['ypos'], $image->info['height'], $height);

  // text bounding boxes are painful. Approximate a reasonable trailer space.
  if ($action['ypos'] == 'bottom') {
    $y_ins +=  $height - $action['size'] * 0.8;
  }
  if ($action['ypos'] == 'top') {
    $y_ins = $height;
  }
  // when rotated, the bb still contains wierd offsets. Like negative numbers!
  if ($action['xpos'] == 'left') {
    $x_ins = 0 - min($bb[0], $bb[2], $bb[4], $bb[6]);
  }

  // convert color from hex (as it is stored in the UI)
  if($action['RGB']['HEX'] && $deduced = hex_to_rgb($action['RGB']['HEX'])) {
    $action['RGB'] = array_merge($action['RGB'], $deduced);
  }

  return imageapi_image_overlaytext($image, $action['text'], $action['size'], $x_ins, $y_ins, $action['RGB'], $fontpath, $action['angle']);
}

/**
 * Given a font name or path, return the full real path.
 * Because the toolkit doesn't scan too well, we need to look ahead to avoid
 * problems and validate.
 *
 * Look for the named font file relative to Drupal, the module, and the files
 * dir.
 *
 * @param $fontpath a font file name, eg 'arial.ttf'
 * @param $strict bool. if set, the func will return nothing on failure. Normal
 * behaviour is to return the input fontfile name, trusting the system to be
 * able to find it for you. Strict is used to trigger a validation alert.
 *
 * @return the FULL filepath of the fonr so the image toolkit knows exactly
 * where it is.
 */
function canvasactions_find_font($fontpath, $strict = FALSE) {
  // Track down the font.
  if (is_file($fontpath)) {
    return realpath($fontpath);
  }

  $fontpath = ltrim($fontpath, '/');
  // Try local
  $tryfontpath = drupal_get_path('module', 'imagecache_canvasactions') .'/'. $fontpath;
  if (is_file($tryfontpath)) {
    return realpath($tryfontpath);
  }

  // Try files
  $tryfontpath =  file_create_path($fontpath);
  if (is_file($tryfontpath)) {
    return realpath($tryfontpath);
  }

  if ($strict) return FALSE;
  // otherwise, just return what we had and hope it's in a system library
  return $fontpath;
}

