# ![Image Effects](https://github.com/backdrop-contrib/image_effects/blob/1.x-1.x/images/image_effects.png "Image Effects for BackdropCMS")

The Image Effects module provides a suite of additional image effects that
can be added to image styles. Image styles let you create derivations of images
by applying (a series of) effect(s) to it. Think of resizing, desaturating,
masking, etc.

The additional effects that Image Effects provides include:

- **Change file format:** e.g. Optimize image size by saving as `webp`. If you need
  transparency in `jpg`'s, make them `png` or `webp`.
- **Watermark:** e.g. Place a image with transparency anywhere over a source
  picture.
- **Overlay:** e.g. Add photo-corners or a frame to the image.
- **Text overlay:** e.g. Add a copyright notice to your image.
- **Color-shifting:** e.g. Colorize images.
- **Brighten/Darken**.
- **Alpha blending:** Use a gray scale image to define the transparency layer of
  an image.
- **Canvas manipulation:** Resize the canvas and add a background color or
  image.
- **Rounded corners**.

These effects are grouped in sub-modules. Just enable the ones you want to use.

The `Image styles admin` submodule adds the ability to duplicate and flush
existing image styles quickly and easily.

## Requirements

- The core `image` module.
- If you use the `module://` notation anywhere in an image effect, you must now
  install the [System Stream Wrapper](https://github.com/backdrop-contrib/system_stream_wrapper)
  module.

## Installation

- Install this module using the official
  [Backdrop CMS instructions](https://backdropcms.org/guide/modules)

## Usage / Documentation

- Additional documentation is located in the
  [Wiki](https://github.com/backdrop-contrib/image_effects/wiki/)

## Issues

- Bugs and Feature requests should be reported in the
  [Issue Queue](https://github.com/backdrop-contrib/image_effects/issues)

## Current Maintainers

- [Laryn Kragt Bakker](https://github.com/laryn) - [CEDC.org](https://cedc.org)
- Seeking additional maintainers

## Credits

- Ported to Backdrop CMS by [Laryn Kragt Bakker](https://github.com/laryn) -
  [CEDC.org](https://cedc.org).
- Current and past maintainers for the Drupal "Imagecache Actions" module that
  this module is ported from: [dman](https://drupal.org/user/33240),
  [sidneyshan](https://drupal.org/user/652426) and
  [fietserwin](https://drupal.org/user/750928)
- Functionality from the Drupal "Image style flush" module was merged into the
  "Image styles admin" submodule. That module was originally written by [Stepan Kuzmin](https://www.drupal.org/u/tostepankuzmingmailcom)
  and maintained by [Hargobind Khalsa](https://www.drupal.org/u/hargobind).

## License

This project is GPL v2 software. See the
[LICENSE.txt](https://github.com/backdrop-contrib/image_effects/blob/1.x-1.x/LICENSE.txt)
file in this directory for complete text.
