<?php
// $ID: $
/**
 * Helper functions for imagecache_textactions
 * 
 * Static text2canvas and dynamic caption functions
 * 
 * Ported by dman
 * from http://drupal.org/node/264862#comment-865490 by patrickharris
 * 
 * TODO Needs review and clean up, this is a re-merge of branched code, may
 * contain duplicate stuff that should be combined.
 * 
 * Currently - The way the two methods calculate the bounding box/offset are
 * totally different.
 * 
 * The Settings form is largely duplicated
 * 
 * The bottom-20 positioning that dynamic text uses is good, and should be
 * worked back into other dimension calculations.
 * 
 * Contains a stub for imageapi functions : imageapi_image_overlaytext_alpha
 * that may be ported over to there if that makes sense.
 */

/**
 * Place text on top of the current canvas
 *
 * Implementation of imagecache_hook_form()
 *
 * @param $action array of settings for this action
 * @return a form definition
 */
function textactions_text2canvas_form($action) {
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
  $form['#validate']['textactions_text2canvas_validate'] = array() ;
  return $form;
}

function textactions_text2canvas_validate($form_values) {
  if (! $fontfile = textactions_find_font($form_values['fontfile']['#value'], TRUE)) {
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
function theme_textactions_text2canvas($element) {
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
function textactions_text2canvas_image(&$image, $action = array()) {
  $fontpath = textactions_find_font($action['fontfile']);

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

  return imageapi_image_overlaytext_alpha($image, $action['text'], $action['size'], $x_ins, $y_ins, $action['RGB'], $fontpath, $action['angle']);
}

/**
 * DYNAMIC CAPTION
 */

/**
 * Place dynamic text on top of the current canvas
 *
 * Implementation of imagecache_hook_form()
 *
 * @param $action array of settings for this action
 * @return a form definition
 */
function textactions_caption_form($action) {
  $defaults = array(
    'size' => 12,
    'angle' => 0,
    'xpos' => left,
    'ypos' => top,
    'RGB' => array(
      'R' => 33,
      'G' => 33,
      'B' => 33,
    ),
    'fontfile' => 'MgOpenModernaBold.ttf',
    'text' => 'return \'Hello World!\';',
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
    'alpha' => array(
      '#type' => 'textfield',
      '#title' => t('opacity'),
      '#default_value' => $action['alpha'] ? $action['alpha'] : 100,
      '#size' => 3,
      '#description' => t('Opacity: 1-100.'),
    ),
    'xpos' => array(
      '#type' => 'textfield',
      '#title' => t('X offset'),
      '#default_value' => $action['xpos'],
      '#description' => t('Enter an offset in pixels or use a keyword: <em>left</em>, <em>center</em>, or <em>right</em>.<br />You may also add a positive or negative integer: i.e. top+42 or center-42.'),
      '#size' => 10,
    ),
    'ypos' => array(
      '#type' => 'textfield',
      '#title' => t('Y offset'),
      '#default_value' => $action['ypos'],
      '#description' => t('Enter an offset in pixels or use a keyword: <em>top</em>, <em>center</em>, or <em>bottom</em><br />You may also add a positive or negative integer: i.e. top+42 or center-42.'),
      '#size' => 10,
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
      '#title' => t('php code to return text'),
      '#default_value' => $action['text'],
      '#description' => t('Enter PHP code that will return your dynamic text. Do not use %php tags. 
      <br />EG <code>return format_date(time());</code>  
      <br />Note that executing incorrect PHP-code can break your Drupal site.
      <p>You can access the $caption array which contains:<br />
      <b>$caption[\'path\']</b> Name of file, e.g. image.jpg<br />
      If the image is a cck imagefield, you will also have access to:<br />
      <b>$caption[\'title\']</b> optional imagefield \'title\' text<br />
      <b>$caption[\'alt\']</b> optional imagefield \'alt\' text<br />
      <b>$caption[\'node\']</b> the complete node variable that the image is attached to.</p>', 
      array('%php' => '<?php ?>')),
    ),
  );
  $form['#validate']['textactions_caption_validate'] = array() ;
  return $form;
}

function textactions_caption_validate($form_values) {
  if (! $fontfile = textactions_find_font($form_values['fontfile']['#value'], TRUE)) {
    // Just warn, don't prevent
    drupal_set_message(t("Unable to confirm that the font %fontfile is available on your system.", array('%fontfile' => $form_values['fontfile']['#value'])) );
  }
  else {
    drupal_set_message(t("Font was found at %fontfile", array('%fontfile' => $fontfile)));
  }
  if (!is_numeric($form_values['alpha']['#value']) || $form_values['alpha']['#value']<1 || $form_values['alpha']['#value']>100) {
    form_set_error('alpha', t('Opacity must be a number between 1 and 100.'));
  }
}

/**
 * Implementation of theme_hook() for imagecache_ui.module
 */
function theme_textactions_caption($element) {
  $data = $element['#value'];
  return "<em><strong>". $data['text'] ."</strong></em>" ;
}


/**
 * Place dynamic text on image
 *
 * Implementation of hook_image()
 *
 * @param $image
 * @param $action
 */
function textactions_caption_image(&$image, $action = array()) {
  // store our variables here
  $caption=array();
  
  // find the image name
  $caption['path'] = substr($image->source, strrpos($image->source, '/') + 1);
  
  // If the image is a cck imagefield,
  // find the image's optional title and alt text, as well as its node
  if (module_exists('imagefield')) {
  
    // a lot of work to find if the content_field_image_cache actually exists yet
    $table_found = false;
    $r = mysql_query("SHOW TABLES");
    while($row = mysql_fetch_array($r)) {
      if($row[0] == 'content_field_image_cache') {
        $table_found = true;
        break;
      }
    }
    if($table_found) {
      // if content_field_image_cache exists,
      // see if we can find info on the current image
      $result = db_fetch_object(db_query("SELECT c.nid, c.field_image_cache_title, c.field_image_cache_alt FROM {content_field_image_cache} c INNER JOIN {files} f ON c.field_image_cache_fid=f.fid WHERE f.filepath = '%s'", $caption['path']));
    }
    if ($result) {
      $caption['title']=$result->field_image_cache_title;
      $caption['alt']=$result->field_image_cache_alt;
      $caption['node']=node_load($result->nid);
    }
  }
  
  // process the php using drupal_eval (rather than eval), but with GLOBAL variables, so they can be passed successfully
  $GLOBALS['caption'] = $caption;
  
  $text = drupal_eval('<'.'?php global $caption; '.$action['text'].' ?'.'>');
  $text = eval($action['text']);
  $text = check_plain($text);

  $fontpath = textactions_find_font($action['fontfile']);

  // find the true height and width of the text, regardless of angle
  $temp = textactions_create_font_image($action['size'], $action['angle'], $fontpath, $text );

  if(! $temp) {
    drupal_set_message(t('Failed to generate text image. Cannot calculate text dimensions. Not overlaying text.'), 'error');
    return;
  }

  // parse keywords
  $x_ins = textactions_keyword_filter($action['xpos'], $image->info['width'], $temp['width'], $temp['bx'], 'x');
  $y_ins = textactions_keyword_filter($action['ypos'], $image->info['height'], $temp['height'], $temp['by'], 'y');


  // convert color from hex (as it is stored in the UI)
  if($action['RGB']['HEX'] && $deduced = hex_to_rgb($action['RGB']['HEX'])) {
    $action['RGB'] = array_merge($action['RGB'], $deduced);
  }
  
  $action['alpha'] = ($action['alpha'] / 100); //Convert to decimal between 0 and 1
  $action['RGB']['alpha'] = ((1 - $action['alpha']) * 127); //convert opacity to proper alpha value (0 = opaque, 127 = transparent)

  return imageapi_image_overlaytext_alpha($image, $text, $action['size'], $x_ins, $y_ins, $action['RGB'], $fontpath, $action['angle']);
}

/**
 * Creates an image containing only the text - used to calculate the true
 * bounding box.
 */
function textactions_create_font_image( $size, $angle, $font, $char ) {
    $rect = imagettfbbox( $size, 0, $font, $char );
    if(!$rect) {
      return NULL;
    }
    if( 0 == $angle ) {
        $imh = $rect[1] - $rect[7];
        $imw = $rect[2] - $rect[0];
        $bx = -1 - $rect[0];
        $by = -1 - $rect[7];
    } else {
        $rad = deg2rad( $angle );
        $sin = sin( $rad );
        $cos = cos( $rad );
        if( $angle > 0 ) {
            $tmp = $rect[6] * $cos + $rect[7] * $sin;
            $bx = -1 - round( $tmp );
            $imw = round( $rect[2] * $cos + $rect[3] * $sin - $tmp );
            $tmp = $rect[5] * $cos - $rect[4] * $sin;
            $by = -1 - round( $tmp );
            $imh = round( $rect[1] * $cos - $rect[0] * $sin - $tmp );
        } else {
            $tmp = $rect[0] * $cos + $rect[1] * $sin;
            $bx = -1 - round( $tmp );
            $imw = round( $rect[4] * $cos + $rect[5] * $sin - $tmp );
            $tmp = $rect[7] * $cos - $rect[6] * $sin;
            $by = -1 - round( $tmp );
            $imh = round( $rect[3] * $cos - $rect[2] * $sin - $tmp );
        }
    }
  
  return array('width'=>$imw, 'height'=>$imh, 'bx'=>$bx, 'by'=>$by);
}

/**
 * Accept a keyword (center, top, left, etc) and return it as an offset in pixels.
 */
function textactions_keyword_filter($value, $current_pixels, $new_pixels, $adj, $xy) {
  // check if we have plus or minus values
  $v = explode('+', $value);
  $v2 = explode('-', $value);
  if ($v2[1]) {$v[1]=-intval($v2[1]); $v[0]=$v2[0];}

  switch ($v[0]) {
    case 'top':
    case 'left':
      $value = 0;
      break;
    case 'bottom':
    case 'right':
      $value = $current_pixels - $new_pixels;
      break;
    case 'center':
      $value = $current_pixels/2 - $new_pixels/2;
      break;
  }
  
  // perform the adjustment
  $value=$value+$adj;
  // add any extra negative or positive
  if ($v[1]) {$value=$value+$v[1];}
  #print $current_pixels.' . '.$new_pixels.' . '.$adj.' . '.$xy.'<br />';if ($xy=='y'){exit;}
  return $value;
}



/**
 * Place text on an image.
 *
 * @ingroup imageapi
 */
function imageapi_image_overlaytext_alpha(&$image, $text, $size = 12, $x = 0, $y = 0, $RGB = 0, $fontfile = 'MgOpenModernaBold', $angle = 0) {
  return call_user_func($image->toolkit .'_image_overlaytext_alpha', $image, $text, $size, $x, $y, $RGB, $fontfile, $angle);
}

/**
 * Place text on an image.
 *
 * @ingroup imageapi
 */
function imageapi_gd_image_overlaytext_alpha(&$image, $text, $size = 12, $x = 0, $y = 0, $RGB, $fontfile = 'MgOpenModernaBold', $angle = 0) {
  $color = imagecolorallocatealpha($image->res, $RGB['red'], $RGB['green'], $RGB['blue'], $RGB['alpha']);
  imagettftext($image->res, $size, $angle, $x, $y, $color, $fontfile, $text);
  return TRUE;
}


/** 
 * UTILITY
 */

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
function textactions_find_font($fontpath, $strict = FALSE) {
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
