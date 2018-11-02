# ![Image Effects](https://github.com/backdrop-contrib/image_effects/blob/1.x-1.x/images/image_effects.png "Image Effects for BackdropCMS")

The Image Effects module provides a suite of additional image effects that
can be added to image styles. Image styles let you create derivations of images
by applying (a series of) effect(s) to it. Think of resizing, desaturating,
masking, etc.

The additional effects that Image Effects provides include:

- **Watermark:** place a image with transparency anywhere over a source picture.
- **Overlay:** add photo-corners etc to the image
- **Text overlay:** add e.g. a copyright notice to your image.
- **Color-shifting:** colorize images.
- **Brighten/Darken**.
- **Alpha blending:** use a gray scale image to define the transparency layer of an
  image.
- **Canvas manipulation:** resize the canvas and add a background color or image.
- **File Format switcher:** if you need transparency in JPGs, make them PNG. If your
  PNG thumbnails are 30K each, save them as JPGs.
- **Rounded corners**.

These effects are grouped in sub-modules. Just enable the ones you want to use.

## Requirements

- The core `image` module.
- If you use the `module://` notation anywhere in an image effect, you must now
  install the [System Stream Wrapper](https://github.com/backdrop-contrib/system_stream_wrapper) 
  module.

## Installation

- Install this module using the official 
  [Backdrop CMS instructions](https://backdropcms.org/guide/modules)

## Usage

After enabling the module:

- Configure your toolkit and its settings at admin/config/media/image-toolkit.
- Define image styles at admin/config/media/image-styles and add one or more
  effects as defined by this module
- Use the image styles via e.g. the formatters of image fields.

### File form fields

A number of effects have a file form field where the editor can define a file
name to use. This can be e.g. for overlays, masks or fonts. The file name should
be defined using either:

- One of the (enabled) default schemes: `public://`, `private://` *(Preferred for site specific masks, overlays, etc, that do not
    need to be shared publicly.)*, `temporary://` *(Unlikely to be useful, but supported anyway as all schemes are
    supported.)*

- One of the schemes provided by the
  [System Stream Wrapper](https://github.com/backdrop-contrib/system_stream_wrapper)
  module: `module://`, `theme://`, `profile://`, or `library://` 

- A relative (to the current directory, probably Backdrop root) or absolute path.

## Issues

Bugs and Feature requests should be reported in the 
[Issue Queue](https://github.com/backdrop-contrib/image_effects/issues)

## Notes
- If you use effects that use files (mask, overlays, underlays, text fonts),
  check the way they are specified. You have to specify the
  location using one of the schemes private://, public://, module:// or
  temporary://. If no scheme is specified, the file is searched for as is, thus
  relative to the current directory or as an absolute path.
- Effects that use the transparency layer (e.g. mask, rounded corners) do not
  automatically convert to PNG anymore. Use the "Change file format" for that.
- Ongoing development in the area of e.g. making the effects more consistent,
  adding and/or removing parameters or redefining their meaning, might cause
  backward incompatibilities between future versions and the current version.
  Thus, we cannot and do not guarantee backwards compatibility or automatic
  upgrade paths for future versions.
- This module is ported from a Drupal 7 module called "Imagecache Actions." 
  The name has been changed because the previous name was based on 
  Drupal 6 terminology and "Image Effects" is much clearer going forward.

## Current Maintainers

- [Laryn Kragt Bakker](https://github.com/laryn) - [CEDC.org](https://cedc.org)
- Seeking additional maintainers

## Credits

- Ported to Backdrop CMS by [Laryn Kragt Bakker](https://github.com/laryn) - [CEDC.org](https://cedc.org).
- Current and past maintainers for the Drupal "Imagecache Actions" module: [dman](https://drupal.org/user/33240), [sidneyshan](https://drupal.org/user/652426) and [fietserwin](https://drupal.org/user/750928)

## License

This project is GPL v2 software. See the [LICENSE.txt](https://github.com/backdrop-contrib/image_effects/blob/1.x-1.x/LICENSE.txt) 
file in this directory for complete text.