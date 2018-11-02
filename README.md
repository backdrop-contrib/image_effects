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

## Usage / Documentation

- Additional documentation is located in the [Wiki](https://github.com/backdrop-contrib/image_effects/wiki/)

## Issues

- Bugs and Feature requests should be reported in the 
[Issue Queue](https://github.com/backdrop-contrib/image_effects/issues)

## Current Maintainers

- [Laryn Kragt Bakker](https://github.com/laryn) - [CEDC.org](https://cedc.org)
- Seeking additional maintainers

## Credits

- Ported to Backdrop CMS by [Laryn Kragt Bakker](https://github.com/laryn) - [CEDC.org](https://cedc.org).
- Current and past maintainers for the Drupal "Imagecache Actions" module: [dman](https://drupal.org/user/33240), [sidneyshan](https://drupal.org/user/652426) and [fietserwin](https://drupal.org/user/750928)

## License

This project is GPL v2 software. See the [LICENSE.txt](https://github.com/backdrop-contrib/image_effects/blob/1.x-1.x/LICENSE.txt) 
file in this directory for complete text.
