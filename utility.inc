<?php
/**
 * @file utility.inc: uitility form, conversion and rendering functions for
 * image processing.
 */

/**
 * Validates that the element is a positive number.
 *
 * This is a Form API #element_validate callback.
 *
 * @param array $element
 * param array $form_status
 */
function image_effects_validate_number_positive(&$element/*, &$form_status*/) {
  $value = $element['#value'];
  if ($value != '') {
    if (!is_numeric($value) || (float) $value <= 0.0) {
      form_error($element, t('%name must be a positive number.', array('%name' => $element['#title'])));
    }
  }
}

/**
 * Validates that the element is a non negative number.
 *
 * This is a Form API #element_validate callback.
 *
 * @param array $element
 * param array $form_status
 */
function image_effects_validate_number_non_negative(&$element/*, &$form_status*/) {
  $value = $element['#value'];
  if ($value != '') {
    if (!is_numeric($value) || (float) $value < 0.0) {
      form_error($element, t('%name must be a non negative number.', array('%name' => $element['#title'])));
    }
  }
}

/*
 * File field handling.
 */
/**
 * @return string
 */
function image_effects_file_field_description() {
  return t('File is either a file with one of the valid schemes, an absolute path, or a relative path (relative to the current directory, probably the Backdrop site root). Valid schemes are a.o. private://, public://, and if the !module module has been installed, also module:// and theme://.',
   array('!module' => l('System Stream Wrapper', 'https://github.com/backdrop-contrib/system_stream_wrapper', array('external' => TRUE))));
}

/**
 * Validates that the file as specified in the element exists and is readable.
 *
 * This is a Form API #element_validate callback.
 *
 * @param array $element
 * param array $form_status
 */
function image_effects_validate_file(&$element/*, &$form_status*/) {
  if (!image_effects_find_file($element['#value'])) {
    form_error($element, t("Unable to find the file '%file'. Please check the path.", array('%file' => $element['#value'])));
  }
}

/**
 * Looks up and returns the full path of the given file.
 *
 * @param string $file
 *   A file name. We accept:
 *   - stream wrapper notation like: private://, public://, temporary:// and
 *     module:// (the latter provided by the system stream wrapper module).
 *   - relative: relative to the current directory (probably BACKDROP_ROOT).
 *   - absolute: as is.
 *
 * @return string|false
 *   The full file path of the file, so the image toolkit knows exactly where it
 *   is. False if the file cannot be found or is not readable.
 */
function image_effects_find_file($file) {
  $result = FALSE;
  if (is_readable($file)) {
    $result = backdrop_realpath($file);
  }
  return $result;
}

/**
 * Loads the given file as an image object.
 *
 * @param string $file
 *   A file name, with a scheme, relative, or absolute.
 * @param bool $toolkit
 *
 * @return object|FALSE
 *   The image object.
 *
 * @see image_effects_find_file()
 * @see image_load()
 */
function image_effects_image_load($file, $toolkit = FALSE) {
  $full_path = image_effects_find_file($file);
  if (!$full_path) {
    trigger_error("Failed to find file $file.", E_USER_ERROR);
    return FALSE;
  }

  $image = image_load($full_path, $toolkit);
  if (!$image) {
    trigger_error("Failed to open file '$file' as an image resource.", E_USER_ERROR);
  }

  return $image;
}

/**
 * Returns the label for the given style name.
 *
 * As of D7.23 image styles can also have a human readable label besides their
 * machine readable name. This function returns that label if available, or the
 * name if the label is absent.
 *
 * @param string|null $style_name
 *   The name of the image style.
 *
 * @return string
 *   The label for the image style.
 */
function image_effects_get_style_label($style_name) {
  if (!empty($style_name)) {
    $style = image_style_load($style_name);
    $label = isset($style['label']) ? $style['label'] : $style['name'];
    if (!empty($style)) {
      return $label;
    }
    else {
      return $label . ' ' . t('<span class="error">Invalid reference. The referenced image style may have been deleted!</span>');
    }
  }
  else {
    return t('none');
  }
}

/**
 * Returns an array with context information about the image.
 *
 * This information is called by effects that need contextual information or
 * that allow custom PHP:
 * - Custom action.
 * - Text from image alt or title.
 * - Text with tokens.
 * - Text from PHP code.
 *
 * @param object $image
 *   The image object.
 * @param array $data
 *   An associative array with the effect options.
 *
 * @return array
 *   An associative array with context information about the image. It contains
 *   the following keys:
 *   - effect_data: array with the effect data.
 *   - managed_file: object|null, a managed file object for the current image.
 *     This may be (an extended) media/file_entity file object.
 *   - referring_entities: array, list of (loaded) entities that have an image
 *       field referring to the managed file.
 *   - entity: object|null, the 1st (and often only) entity that has an image
 *       field referring to the managed file.
 *   - image_field: array|null, the (1st and often only) image field item of
 *       entity that refers to the managed file (single field item, not the
 *       whole field value).
 */
function image_effects_get_image_context($image, $data) {
  // Store context about the image.
  $image_context = array(
    'effect_data' => $data,
    'managed_file' => NULL,
    'referring_entities' => array(),
    'entity' => NULL,
    'image_field' => NULL,
  );

  // Find the managed file object (at most 1 object as 'uri' is a unique index).
  $managed_file = file_load_multiple(array(), array('uri' => $image->source));
  $managed_file = reset($managed_file);
  if ($managed_file !== FALSE) {
    $image_context['managed_file'] = $managed_file;
    // And find the entities referring to this managed file, image fields first,
    // then file fields (very specific use cases).
    $references = file_get_file_references($managed_file, NULL, FIELD_LOAD_CURRENT, 'image')
      + file_get_file_references($managed_file, NULL, FIELD_LOAD_CURRENT, 'file');

    if ($references) {
      // Load referring entities., keep track of 1st reference separately.
      $entity_type_first = '';
      $field_name_first = '';
      foreach ($references as $field_name => $field_references) {
        foreach ($field_references as $entity_type => $entity_stubs) {
          $entities = entity_load($entity_type, array_keys($entity_stubs));
          if (!empty($entities)) {
            $image_context['referring_entities'][$field_name][$entity_type] = $entities;
            // Make it easy to access the '1st' entity and its referring image
            // field, part 1: entity.
            if (empty($entity_type_first)) {
              $entity_type_first = $entity_type;
              $field_name_first = $field_name;
              $image_context['entity'] = reset($entities);
            }
          }
        }
      }

      // Make it easy to access the '1st' entity and its referring image field,
      // part 2: image field.
      /** @var array|false $image_field */
      $image_field = field_get_items($entity_type_first, $image_context['entity'], $field_name_first);
      if ($image_field) {
        // Get referring item.
        foreach ($image_field as $image_field_value) {
          if ($image_field_value['fid'] === $managed_file->fid) {
            $image_context['image_field'] = $image_field_value;
            break;
          }
        }
      }
    }
  }

  return $image_context;
}


/**
 * Returns information about the context, image style and image effect id, of
 * the current image effect.
 *
 * Note that this information is not alterable, that is, it will not have any
 * effect on the rest of the derivative image generation process including its
 * subsequent effects.
 *
 * This information is called by effects that may need contextual information or
 * that allow custom PHP:
 * - Custom action.
 * - Text from image alt or title.
 * - Text with tokens.
 * - Text from PHP code.
 *
 * This information is fetched by traversing the backtrace, looking for known
 * functions that have the image style or effect as argument..
 *
 * @return array
 *   Array with keys 'image_style' and 'image_effect_id' pointing to the current
 *   image style (array) resp. image effect id (int).
 */
function image_effects_get_image_effect_context() {
  $result = array();
  $backtrace = debug_backtrace();

  foreach ($backtrace as $function_call) {
    if ($function_call['function'] && $function_call['function'] === 'image_effect_apply') {
      $result['image_effect_id'] = isset($function_call['args'][1]['ieid']) ? $function_call['args'][1]['ieid'] : NULL;
    }
    else if ($function_call['function'] && $function_call['function'] === 'image_style_create_derivative') {
      $result['image_style'] = isset($function_call['args'][0]) ? $function_call['args'][0] : NULL;
    }

    if (count($result) === 2) {
      break;
    }
  }

  $result += array(
    'image_effect_id' => NULL,
    'image_style' => NULL,
  );
  return $result;
}

/**
 * Given two imageapi objects with dimensions, and some positioning values,
 * calculate a new x,y for the layer to be placed at.
 *
 * This is a different approach to image_effects_keyword_filter() - more
 * like css.
 *
 * The $style is an array, and expected to have 'top, bottom, left, right'
 * attributes set. These values may be positive, negative, or in %.
 *
 * % is calculated relative to the base image dimensions.
 * Using % requires that the layer is positioned CENTERED on that location, so
 * some offsets are added to it. 'right-25%' is not lined up with a margin 25%
 * in, it's centered at a point 25% in - which is therefore identical with
 * left+75%
 *
 * @param $base
 * @param $layer
 * @param $style
 *
 * @return array
 *   A keyed array of absolute x,y co-ordinates to place the layer at.
 */
function image_effects_calculate_relative_position($base, $layer, $style) {
  // Both images should now have size info available.
  if (isset($style['bottom'])) {
    $ypos = image_effects_calculate_offset('bottom', $style['bottom'], $base->info['height'], $layer->info['height']);
  }
  if (isset($style['top'])) {
    $ypos = image_effects_calculate_offset('top', $style['top'], $base->info['height'], $layer->info['height']);
  }
  if (isset($style['right'])) {
    $xpos = image_effects_calculate_offset('right', $style['right'], $base->info['width'], $layer->info['width']);
  }
  if (isset($style['left'])) {
    $xpos = image_effects_calculate_offset('left', $style['left'], $base->info['width'], $layer->info['width']);
  }
  if (!isset($ypos)) {
    // Assume center.
    $ypos = ($base->info['height'] / 2) - ($layer->info['height'] / 2);
  }
  if (!isset($xpos)) {
    // Assume center.
    $xpos = ($base->info['width'] / 2) - ($layer->info['width'] / 2);
  }
  // dpm(__FUNCTION__ . " Calculated offsets");
  // dpm(get_defined_vars());
  return array('x' => $xpos, 'y' => $ypos);
}

/**
 * Calculates an offset from an edge.
 *
 * Positive numbers are IN from the edge, negative offsets are OUT.
 *
 * Examples:
 * - left, 20, 200, 100 = 20
 * - right, 20, 200, 100 = 80 (object 100 wide placed 20px from the right)
 * - top, 50%, 200, 100 = 50 (Object is centered when using %)
 * - top, 20%, 200, 100 = -10
 * - bottom, -20, 200, 100 = 220
 * - right, -25%, 200, 100 = 200 (this ends up just off screen)
 *
 * Also, the value can be a string, eg "bottom-100", or "center+25%"

 * @param string $keyword
 *   The edge to calculate the offset from. Can be one of: left, right, top,
 *   bottom, middle, center.
 * @param string $value
 *   The value to offset with: can be:
 *   - a numeric value: offset in pixels
 *   - a percentage value: offset with a percentage of the $base_size.
 *   - a keyword indicating a pxiel size: left, right, top, bottom, middle,
 *     center.
 *   - an expression of the format: keyword +|- value[%], e.g. center+25%.
 * @param int $base_size
 *   The size of the dimension. Used to calculate percentages of.
 * @param int $layer_size
 *   The size of the canvas in the current dimension. The value to take for
 *   $keyword will be based on this value.
 *
 * @return int
 */
function image_effects_calculate_offset($keyword, $value, $base_size, $layer_size) {
  $offset = 0;
  // Used to account for dimensions of the placed object.
  $direction = 1;
  $base = 0;
  if ($keyword == 'right' || $keyword == 'bottom') {
    $direction = -1;
    $offset = -1 * $layer_size;
    $base = $base_size;
  }
  if ($keyword == 'middle' || $keyword == 'center') {
    $base = $base_size / 2;
    $offset = -1 * ($layer_size / 2);
  }

  // Keywords may be used to stand in for numeric values.
  switch ($value) {
    case 'left':
    case 'top':
      $value = 0;
      break;

    case 'middle':
    case 'center':
      $value = $base_size / 2;
      break;

    case 'bottom':
    case 'right':
      $value = $base_size;
  }

  // Handle keyword-number cases like top+50% or bottom-100px,
  // @see image_effects_keyword_filter().
  if (preg_match('/^([a-z]+) *([+-]) *(\d+)((px|%)?)$/', $value, $results)) {
    list(, $value_key, $value_mod, $mod_value, $mod_unit) = $results;
    if ($mod_unit == '%') {
      $mod_value = $mod_value / 100 * $base_size;
    }
    $mod_direction = ($value_mod == '-') ? -1 : +1;
    switch ($value_key) {
      case 'left':
      case 'top':
      default:
        $mod_base = 0;
        break;

      case 'middle':
      case 'center':
        $mod_base = $base_size / 2;
        break;

      case 'bottom':
      case 'right':
        $mod_base = $base_size;
        break;
    }
    $modified_value = $mod_base + ($mod_direction * $mod_value);
    return $modified_value;
  }

  // Handle % values.
  if (substr($value, -1) === '%') {
    // Explicitly convert $value to prevent warnings in PHP 7.1.
    $value = intval((int) $value / 100 * $base_size);
    $offset = -1 * ($layer_size / 2);
  }
  // $value may contain 'px' at the end: explicitly convert to prevent warnings
  // in PHP 7.1.
  $value = $base + ($direction * (int) $value);

  // Add any extra offset to position the item.
  return $value + $offset;
}

/**
 * Convert a hex string to its RGBA (Red, Green, Blue, Alpha) integer
 * components.
 *
 * Stolen from imageapi D6 2011-01
 *
 * @param string $hex
 *   A string specifing an RGB color in the formats:
 *   '#ABC','ABC','#ABCD','ABCD','#AABBCC','AABBCC','#AABBCCDD','AABBCCDD'
 *
 * @return array|false
 *   An array with four elements for red, green, blue, and alpha.
 */
function image_effects_hex2rgba($hex) {
  $hex = ltrim($hex, '#');
  if (preg_match('/^[0-9a-f]{3}$/i', $hex)) {
    // 'FA3' is the same as 'FFAA33' so r=FF, g=AA, b=33
    $r = str_repeat($hex[0], 2);
    $g = str_repeat($hex[1], 2);
    $b = str_repeat($hex[2], 2);
    $a = '0';
  }
  elseif (preg_match('/^[0-9a-f]{6}$/i', $hex)) {
    // #FFAA33 or r=FF, g=AA, b=33
    list($r, $g, $b) = str_split($hex, 2);
    $a = '0';
  }
  elseif (preg_match('/^[0-9a-f]{8}$/i', $hex)) {
    // #FFAA33 or r=FF, g=AA, b=33
    list($r, $g, $b, $a) = str_split($hex, 2);
  }
  elseif (preg_match('/^[0-9a-f]{4}$/i', $hex)) {
    // 'FA37' is the same as 'FFAA3377' so r=FF, g=AA, b=33, a=77
    $r = str_repeat($hex[0], 2);
    $g = str_repeat($hex[1], 2);
    $b = str_repeat($hex[2], 2);
    $a = str_repeat($hex[3], 2);
  }
  else {
    // error: invalid hex string, @todo: set form error.
    return FALSE;
  }

  $r = hexdec($r);
  $g = hexdec($g);
  $b = hexdec($b);
  $a = hexdec($a);
  // Alpha over 127 is illegal. assume they meant half that.
  if ($a > 127) {
    $a = (int) $a / 2;
  }
  return array('red' => $r, 'green' => $g, 'blue' => $b, 'alpha' => $a);
}

/**
 * Accept a keyword (center, top, left, etc) and return it as an offset in pixels.
 * Called on either the x or y values.
 *
 * May  be something like "20", "center", "left+20", "bottom+10". + values are
 * in from the sides, so bottom+10 is 10 UP from the bottom.
 *
 * "center+50" is also OK.
 *
 * "30%" will place the CENTER of the object at 30% across. to get a 30% margin,
 * use "left+30%"
 *
 * @param string|int $value
 *   string or int value.
 * @param int $base_size
 *   Size in pixels of the range this item is to be placed in.
 * @param int $layer_size
 *   Size in pixels of the object to be placed.
 *
 * @return int
 */
function image_effects_keyword_filter($value, $base_size, $layer_size) {
  // See above for the patterns this matches.
  if (!preg_match('/([a-z]*) *([+-]?) *(\d*)((px|%)?)/', $value, $results)) {
    trigger_error("image_effects had difficulty parsing the string '$value' when calculating position. Please check the syntax.", E_USER_WARNING);
  }
  list(, $keyword, $plusminus, $value, $unit) = $results;

  return image_effects_calculate_offset($keyword, $plusminus . $value . $unit, $base_size, $layer_size);
}

/**
 * Computes a length based on a length specification and an actual length.
 *
 * Examples:
 *  (50, 400) returns 50; (50px, 400) returns 50; (50%, 400) returns 200;
 *  (50, null) returns 50; (50%, null) returns null;
 *  (null, null) returns null; (null, 100) returns null.
 *
 * @param string|null $length_specification
 *   The length specification. An integer constant optionally followed by 'px'
 *   or '%'.
 * @param int|null $current_length
 *   The current length. May be null.
 *
 * @return int|null
 */
function image_effects_percent_filter($length_specification, $current_length) {
  if (strpos($length_specification, '%') !== FALSE) {
    $length_specification = $current_length !== NULL ? str_replace('%', '', $length_specification) * 0.01 * $current_length : NULL;
  }
  else {
    // Strips 'px' if available.
    $length_specification = (int) $length_specification;
  }
  return $length_specification;
}
