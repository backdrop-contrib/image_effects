<?php

/**
 * Implementation of hook_image_effects_default_presets().
 */
function image_effects_testsuite_image_effects_default_presets() {
  $filepath = backdrop_get_path('module', 'image_effects_testsuite');
  // Each of these presets is stored in its own file.
  // Scan each of these files and return the list.
  return image_effects_testsuite_get_tests();
}
