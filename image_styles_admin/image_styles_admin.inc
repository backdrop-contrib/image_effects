<?php

/**
 * @file Include file for image_styles_admin routines that do not need to be
 * loaded on each request.
 */

/**
 * Menu callback: Duplicates an image style and redirects to the image styles
 * overview page.
 *
 * @param array $style
 *   An image style array.
 *
 * @see image_style_name_validate()
 */
function image_styles_admin_duplicate_page_callback($style) {
  $duplicate_style = image_styles_admin_duplicate($style);
  backdrop_set_message(t('Style %name has been duplicated to %new_name.', array(
    '%name' => isset($style['label']) ? $style['label'] : $style['name'],
    '%new_name' =>  isset($duplicate_style['label']) ? $duplicate_style['label'] : $duplicate_style['name'])));
  backdrop_goto('admin/config/media/image-styles');
}

/**
 * Duplicates an image style and saves it.
 *
 * @param array $style
 *   An image style array.
 * @param string|null $new_style_name
 *   The preferred name for the new style. If left empty, the new name will be
 *   based on the name of the style to duplicate. In both cases and when
 *   necessary, the new name will be made unique by adding some suffix to it.
 * @param string|null $new_style_label
 *   The preferred label for the new style. If left empty, the new label will be
 *   based on the label of the style to duplicate. If that one is also empty,
 *   no label will be defined for the new style, so Drupal (>=7.23) will create
 *   one.
 *
 * @return array
 *   An image style array with the newly created copy of the given style.
 *
 * @see image_style_name_validate()
 */
function image_styles_admin_duplicate($style, $new_style_name = NULL, $new_style_label = NULL) {
  // Find a unique name for the copy.
  // Step 1: Find the base: name without things like '-copy' or '-copy-1'
  $style_name_base = empty($new_style_name) ? $style['name'] : $new_style_name;
  if (preg_match('/-copy(-\d+)?$/', $style_name_base)) {
    $style_name_base = substr($style_name_base, 0, strpos($style_name_base, '-copy'));
  }

  // Step 2: Add -copy to it (if the name comes from the current style).
  if (empty($new_style_name)) {
    $style_name_base .= '-copy';
  }

  // Step 3: Ensure the new name will be unique.
  $i = 0;
  $style_name = $style_name_base;
  $styles = image_styles();
  while (isset($styles[$style_name])) {
    $i++;
    $style_name = $style_name_base . '-' . $i;
  }
  $style['name'] = $style_name;

  // Step 4: Find a new label for the copy.
  if (isset($new_style_label) || isset($style['label'])) {
    $style_label = empty($new_style_label) ? $style['label'] : $new_style_label;
    $copy = t('copy');
    if (preg_match("/ $copy( \d+)?$/", $style_label)) {
      $style_label = substr($style_label, 0, strpos($style_label, " $copy"));
    }

    // Step 4a: Add " copy" to it (if the name comes from the current style).
    if (empty($new_style_label)) {
      $style_label .= " $copy";
    }
    $style['label'] = $style_label;
    
    // Step 4b: Make "unique" (based on the number added to the name)
    if ($i > 0) {
      $style['label'] .= " $i";
    }
  }
  
  // Unset isid to save it as a new style.
  unset($style['isid']);
  $style['module'] = '';
  $style = image_style_save($style);

  return $style;
}
