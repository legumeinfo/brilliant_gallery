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

• Anywhere in a node, using a special tag in the format [bg|pathtoyourgalleryfolderwithoutslashes] (only uses settings set forth in /admin/settings/brilliant_gallery).

• And you can add another table if you want. And you can also override the default number of columns and the thumbnail width (in pixels), respectively, using: [bg|2007_Tomas|15|50]

• As a default block (only settings set forth in /admin/settings/brilliant_gallery apply).

• As a custom block. Create a block with a Brilliant Gallery Tag in it - e.g. [bg|2007_Tomas|4|45]

Author: Tomas Fulopp (Vacilando) - info AT vacilando.org

If you use this module, find it useful, and want to send the author a thank you note, then use the following URL:
http://www.vacilando.org/index.php?x=1707
If you want to show me how your gallery looks like, don't forget to include a link, too.

The author can also be contacted for paid customizations of this and other modules.

Development of this module is sponsored by Vacilando.org ( http://www.vacilando.org/ )
<b>Highly customizable table gallery of quality-scaled images from multiple defined folders.</b>

&bull; Configurable image size, number of columns, background colour.
&bull; High quality image re-sampling.
&bull; Automatic re-sizing of both landscape and portrait images.
&bull; Automatically recognizes and displays <i>.png</i>, <i>.jpg</i> and <i>.jpeg</i>, and <i>.gif</i>. (Other media in the specified folder (e.g. <i>.mpg</i>) are also linked.)
&bull; Browsing images via slick <a href="http://drupal.org/project/lightbox2" target="_blank">Lightbox V2</a> or <a href="http://drupal.org/project/thickbox" target="_blank">Thickbox</a>.
&bull; Has both a default page ( <a href="http://quidne.vacilando.org/brilliant_gallery" target="_blank">/brilliant_gallery</a> ) and a block, for straightforward use, but any gallery can be entered into any page or custom block using a special tag.

<b>Planned features</b>:
&bull; Adding captions to display in the Lightbox or Thickbox overlays.
&bull; Caching of the thumbnails to further increase loading speed.

<b>Installation</b>:
<ol>
    <li>Download the latest version of the Brilliant Gallery module from its Drupal home <a href="http://drupal.org/project/brilliant_gallery" target="_blank">here</a>.
    </li>
    <li>Enable the module in your Drupal ( /admin/build/modules ).
    </li>
    <li>Go to Input Formats in your Drupal ( /admin/settings/filters ) and via the &quot;configure&quot; link enable one of your input formats to recognize the Brilliant Gallery Tag.
    </li>
    <li>Using FTP, create a main folder in your 'files' directory that will hold all your future gallery folders. E.g. <i>albums</i>. You do not have to do this if you prefer your gallery folders to be located directly in your 'files' folder.
    </li>
    <li>Change the default settings for Brilliant Gallery in the admin section of your Drupal ( /admin/settings/brilliant_gallery ).
    </li>
</ol>
<blockquote>
    <ul>
        <li>Path to the main gallery folder = Path to the main folder in which your individual gallery folders will be placed. This folder must exist under your 'files' folder. Exclude trailing trailing slashes. Example: <i>albums</i>.  Specify nothing if you prefer your gallery folders to be located directly in your 'files' folder.</li>
        <li>Maximum number of table columns = The maximum number of columns displayed in the table.
        </li>
        <li>Maximum width of table images = The maximum width of thumbnails in the table (height calculated automatically).
        </li>
        <li>Table background colour = Colour on the background of the table that holds the images.
        </li>
        <li>Table cell padding = Cell padding (around each image) in pixels.
        </li>
        <li>Overlay browser = Select the overlay image browser (must be installed, of course). Currently <a href="http://drupal.org/project/lightbox2" target="_blank">Lightbox V2</a> and <a href="http://drupal.org/project/thickbox" target="_blank">Thickbox</a> are supported.
        </li>
    </ul>
</blockquote>
<p>
<b>Use</b>:
</p>
<p>
There are several ways in which you can display your picture gallery.
</p>
<p>
• Anywhere in a node, using a special tag in the format <code>[<b></b>bg|pathtoyourgalleryfolderwithoutslashes]</code>. The following table is generated using: <code>[<b></b>bg|2007_Tomas]</code> (only uses settings set forth in /admin/settings/brilliant_gallery).
[bg|2007_Tomas]
</p>
<p>
• And you can add another table if you want. And you can also override the default number of columns and the thumbnail width (in pixels), respectively, using: <code>[<b></b>bg|2007_Tomas|15|50]</code>
[bg|2007_Tomas|15|50]
</p>
<p>
• As a default block (only settings set forth in /admin/settings/brilliant_gallery apply).
</p>
<p>
• As a custom block. Create a block with a Brilliant Gallery Tag in it - e.g. the one in the right column here was created simply using the following tag: <code>[<b></b>bg|2007_Tomas|4|45]</code>
</p>

<p><b>Author</b>: <a href="http://www.vacilando.org/index.php?x=4508" target="_blank">Tomas Fulopp (Vacilando)</a>

<p>If you use this module, find it useful, send me a message via <a href="http://www.vacilando.org/index.php?x=1707" target="_blank">my contact page</a> (and don't forget to include a link to your gallery!)

<p>The author can also be contacted for paid customizations of this and other modules.

<p>Development of this module is sponsored by Vacilando.org ( <a href="http://www.vacilando.org/" target="_blank">http://www.vacilando.org/</a> )

Installation and configuration instructions, demonstration and other information about this module are at http://quidne.vacilando.org/bg
