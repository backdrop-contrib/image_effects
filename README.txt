README for the Imagecache Actions Drupal module
-----------------------------------------------

Project page: http://drupal.org/project/imagecache_actions

Current and past maintainers for Imagecache Actions:
- dman (http://drupal.org/user/33240)
- sidneyshan (http://drupal.org/user/652426)
- fietserwin (http://drupal.org/user/750928)


Introduction
------------
The Imagecache Actions module provides a suite of additional image effects that
can be added to image styles. Image styles let you create derivations of images
by applying (a series of) effect(s) to it. Think of resizing, desaturating,
masking, etc.

The additional effects that Imagecache Actions provides include:
- Watermark: place a image with transparency anywhere over a source picture.
- Overlay: add photo-corners etc to the image
- Text overlay: add e.g. a copyright notice to your image.
- Color-shifting: colorize images.
- Brighten/Darken.
- Alpha blending: use a grayscale image to define the transparency layer of an
  image.
- Canvas manipulation: resize the canvas and add a backgroundcolor or image.
- File Format switcher: if you need tranparency in JPGs, make them PNG. If your
  PNG thumbnails are 30K each, save them as JPGs.
- Rounded corners.
- TODO: complete list, check short descrptions

These effects are grouped in submodules. Just enable the ones you want to use.
TODO: list submodules and their sets of effects.

Imagecache Actions supports both the GD toolkit from Drupal core as well as the
Imagemagick toolkit, though please note that Imagemagick support is still not
complete.


A note about the name of this module
------------------------------------
Image styles are part of Drupal 7 core and are the successor of the Drupal 6
imagecache module. In Drupal 6 the separate effects that made up a style were
called imagecache actions. In porting to D7, that name has not been changed
(yet).


Dependencies
------------
- Drupal 7.x
- Image module from Drupal core

At least one of:
- GD toolkit from Drupal core
- Imagemagick toolkit: http://drupal.org/project/imagemagick


Installing
----------
As usual. After enabling the module:
- Assure that the Image module from core is enabled.
- Configure your toolkit and its settings at admin/config/media/image-toolkit.
- Define image styles at admin/config/media/image-styles.
- Use the image styles via e.g. the formatters of image fields.


Support
-------
Via the issue queue of this project at Drupal.org.
