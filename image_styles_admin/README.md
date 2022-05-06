# Image styles admin

## Introduction

The `Image styles admin` module extends the administrative interface for image
styles by providing:

 - a "Duplicate" feature for image styles and making a
slight adjustment to the image style admin overview page (showing details
of the image effects for each style underneath the image style names).
 - a "Flush" operation for individual image styles and a "Flush all styles"
action to flush the cache for all styles at once.

NOTE: This module has been simplified from the Drupal 7 version, removing the
import/export functionality (since that is available in Backdrop core by
default for image styles).

Credits:

 - Current and past maintainers for the Drupal version of `Image styles admin`:
[fietserwin](https://drupal.org/user/750928)
- Functionality from the Drupal `Image style flush` module was merged into this
  module. That module was originally written by [Stepan Kuzmin](https://www.drupal.org/u/tostepankuzmingmailcom)
  and maintained by [Hargobind Khalsa](https://www.drupal.org/u/hargobind).
