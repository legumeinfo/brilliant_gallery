/* $Id$ */

Highly customizable table gallery of quality-scaled images from multiple defined folders.

• Configurable image size, number of columns, background colour.
• High quality image re-sampling.
• Automatic re-sizing of both landscape and portrait images.
• Automatically recognizes and displays .png, .jpg and .jpeg, and .gif. (Other media in the specified folder (e.g. .mpg) are also linked.)
• Browsing images via slick Lightbox V2 or Thickbox.
• Has both a default page ( /brilliant_gallery ) and a block, for straightforward use, but any gallery can be entered into any page or custom block using a special tag.

Planned features:
• Adding captions to display in the Lightbox or Thickbox overlays.
• Caching of the thumbnails to further increase loading speed.

Installation:

   1. Download the latest version of the Brilliant Gallery module from its Drupal home here.
   2. Enable the module in your Drupal ( /admin/build/modules ).
   3. Go to Input Formats in your Drupal ( /admin/settings/filters ) and via the "configure" link enable one of your input formats to recognize the Brilliant Gallery Tag.
   4. Using FTP, create a main folder in your 'files' directory that will hold all your future gallery folders. E.g. albums. You do not have to do this if you prefer your gallery folders to be located directly in your 'files' folder.
   5. Change the default settings for Brilliant Gallery in the admin section of your Drupal ( /admin/settings/brilliant_gallery ).

        * Path to the main gallery folder = Path to the main folder in which your individual gallery folders will be placed. This folder must exist under your 'files' folder. Exclude trailing trailing slashes. Example: albums. Specify nothing if you prefer your gallery folders to be located directly in your 'files' folder.
        * Maximum number of table columns = The maximum number of columns displayed in the table.
        * Maximum width of table images = The maximum width of thumbnails in the table (height calculated automatically).
        * Table background colour = Colour on the background of the table that holds the images.
        * Table cell padding = Cell padding (around each image) in pixels.
        * Overlay browser = Select the overlay image browser (must be installed, of course). Currently Lightbox V2 and Thickbox are supported.

Use:

There are several ways in which you can display your picture gallery.

• Anywhere in a node, using a special tag in the format [bg|pathtoyourgalleryfolderwithoutslashes]. The following table is generated using: [bg|2007_Tomas] (only uses settings set forth in /admin/settings/brilliant_gallery).

• And you can add another table if you want. And you can also override the default number of columns and the thumbnail width (in pixels), respectively, using: [bg|2007_Tomas|15|50]

• As a default block (only settings set forth in /admin/settings/brilliant_gallery apply).

• As a custom block. Create a block with a Brilliant Gallery Tag in it - e.g. the one in the right column here was created simply using the following tag: [bg|2007_Tomas|4|45]

Author: Tomas Fulopp (Vacilando)

If you use this module, find it useful, send me a message via http://www.vacilando.org/index.php?x=1707 (and don't forget to include a link to your gallery!)

The author can also be contacted for paid customizations of this and other modules.

Development of this module is sponsored by Vacilando.org ( http://www.vacilando.org/ )

Installation and configuration instructions, demonstration and other information about this module are at http://quidne.vacilando.org/bg.