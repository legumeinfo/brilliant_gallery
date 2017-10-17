<?php

function render_brilliant_gallery($paramarray = array()) {

  $thisfolder = '';
  if (isset($paramarray[1])) {
    $thisfolder = $paramarray[1];
  }

  $colcountoverride = @$paramarray[2];

  $widthoverride = @$paramarray[3];

  $sortoverride = @$paramarray[4];

  $maximumnumbertoshow = @$paramarray[5];

  $colouroverride = @$paramarray[6];

  $beginfromoverride = @$paramarray[7];
  if ($beginfromoverride == '') {
    $beginfromoverride = 1;
  }

  $captionyesnotext = @$paramarray[8];

  // Test whether 'thumbsquared' is set.
  @$paramarray[10] = strtolower(@$paramarray[10]);
  if ($paramarray[10] == 'yes' or $paramarray[10] == 'no') {
    $imagecrop = $paramarray[10];
  }
  else {
    $imagecrop = variable_get('brilliant_gallery_crop', 'no');
  }

  if (@$paramarray[12] == '') {
    $paramarray[12] = 'no';
  }
  $bgcachexpire = brilliant_gallery_get_days_in_seconds(variable_get('brilliant_gallery_cache_duration', 90));

  $thumbalignincell = strtolower(@$paramarray[15]);
  if ($thumbalignincell <> 'center' and $thumbalignincell <> 'left' and $thumbalignincell <> 'right') {
    $thumbalignincell = 'center';
  }

  // Is this a Google+ Photos album page?
  // E.g. https://plus.google.com/photos/104031880678382567653/albums/5591422169328270129
  // E.g. https://plus.google.com/photos/104031880678382567653/albums/5414337846751631057?authkey=CLa-o8OXyqC-kAE
  // E.g. https://plus.google.com/u/0/photos/104031880678382567653/albums/5641519066949458241?authkey=CMiq-vWZvIvaaA
  $picasawebsignature = 'http://plus.google.com/';
  $picasawebsignaturesecure = 'https://plus.google.com/';
  if (substr($thisfolder, 0, strlen($picasawebsignature)) == $picasawebsignature OR substr($thisfolder, 0, strlen($picasawebsignaturesecure)) == $picasawebsignaturesecure) {
    // Get the authkey, if any, from the URL query
    $urlquery = parse_url($thisfolder,PHP_URL_QUERY);
    parse_str($urlquery, $parsedquery);
    $authkey = '';
    if (@$parsedquery['authkey'] <> ''){
      $authkey = 'Gv1sRg'.$parsedquery['authkey'];
    }
    // Get the IDs from URL path
    $urlpath = parse_url($thisfolder,PHP_URL_PATH);
    $urlpath = explode('/',$urlpath);
      $picasauserid = '';
      $picasaalbumid = '';
      foreach($urlpath as $key=>$val){
        if (is_numeric($val) AND strlen($val) > 10 AND $picasauserid == ''){
          $picasauserid = $val;
        }
        else if (is_numeric($val) AND strlen($val) > 10 AND $picasauserid <> ''){
          $picasaalbumid = $val;
        }
      }
      // OK we've got all needed to construct a picasaweb feed URL
      $thisfolder = 'https://picasaweb.google.com/data/feed/base/user/'.$picasauserid.'/albumid/'.$picasaalbumid.'?alt=rss&kind=photo';
      if ($authkey <> ''){
        $thisfolder .= '&authkey='.$authkey;
      }
  }
  // Is this a Picasa gallery?
  $picasafolder = false;
  $picasawebsignature = 'http://picasaweb.google.';
  $picasawebsignaturesecure = 'https://picasaweb.google.';
  if (substr($thisfolder, 0, strlen($picasawebsignature)) == $picasawebsignature OR substr($thisfolder, 0, strlen($picasawebsignaturesecure)) == $picasawebsignaturesecure) {
    // Must work for all variants - http://picasaweb.google.com, http://picasaweb.google.co.uk, etc.
    // Replace '&amp;' (happens if tags are edited in rich text editors) by '&'.
    $thisfolder = str_replace("&amp;", "&", $thisfolder);
    $picasafolder = true;
  }

  // See if one or more particular images were asked to be shown.
  $thumbshowbyname = array();
  $tmp = explode(',', @$paramarray[13]);
  if (is_array($tmp)) {
    foreach ($tmp as $val) {
      $val = trim($val);
      if ($val == '') {
        continue;
      }
      $thumbshowbyname[] = $val;
      // Had an idea to update the column count to match the number of accepted images - but that does not work in case some images don't have a match anyway. So did not do it and users have to set the column override to whatever number they want. Too much intelligence in the gadget is bad anyways :-)
    }
  }
  #watchdog('Brilliant Gal', 's1: '.implode(' ... ',$thumbshowbyname));
  $result = '';
  $galleryfolder = variable_get('brilliant_gallery_folder', '');
  if ($thisfolder <> '') {
    if ($galleryfolder <> '') {
      #$galleryfolder .= '/' . $thisfolder;
      $galleryfolder .= ( substr($thisfolder, 0, 1) == '/' ? '' : '/') . $thisfolder; // See http://drupal.org/node/176939#comment-1494648
    }
    else {
      $galleryfolder = $thisfolder;
    }
  }
  if ($colcountoverride == '') {
    $columns = variable_get('brilliant_gallery_maxcol', 3);
  }
  else {
    $columns = $colcountoverride;
  }
  if ($widthoverride == '') {
    $imagewidth = variable_get('brilliant_gallery_maximagewidth', 150);
  }
  else {
    $imagewidth = $widthoverride;
  }
  if ($sortoverride == '' or strtolower($sortoverride) == 'sort') {
    $brilliant_gallery_sort = variable_get('brilliant_gallery_sort', '1');
  }
  else {
    $brilliant_gallery_sort = $sortoverride;
  }

  if ($colouroverride == '') {
    $bgcolour = check_plain(variable_get('brilliant_gallery_bcgcolour_textfield', '#000000'));
  }
  else {
    $bgcolour = $colouroverride;
  }
  if (($captionyesnotext == 'yes' or $captionyesnotext == '') and variable_get('brilliant_gallery_caption', '') <> '') {
    $showcaption = 'filename';
  }
  else if ($captionyesnotext == 'no' or (variable_get('brilliant_gallery_caption', '') == '' and ($captionyesnotext == 'yes' or $captionyesnotext == 'no'))) {
    $showcaption = '';
  }
  else {
    $showcaption = $captionyesnotext;
  }
  $padding = check_plain(variable_get('brilliant_gallery_padding', 3));
  $overbrowser = variable_get('brilliant_gallery_overbrowser', 'colorbox');
  // Totally full resolution display would be impractical, so this is the maximum width of "full" resolution.
  $fullresolutionmaxwidth = variable_get('brilliant_gallery_maxwidth', '1000');
  $path = url(FILE_DIRECTORY_PATH . '/' . $galleryfolder, array('absolute' => TRUE));
  if (BG_LANGCODE <> '') {
    $path = str_replace('/' . BG_LANGCODE . '/', '/', $path);
  }

  // Non-clean URLs need removing ?q=
  $path = str_replace("?q=", "", $path);

  if ($paramarray[12] == 'yes' and $picasafolder) {
    // Display a Picasa slide show - but only if this is Picasa data and not a local gallery.
    $slideshowurl = str_replace('/base/', '/api/', $thisfolder); // The only difference between RSS and slideshow feeds, apparently.
    $result .= '<embed type="application/x-shockwave-flash" src="https://picasaweb.google.com/s/c/bin/slideshow.swf" width="' . $widthoverride . '" height="' . ($widthoverride * 0.75) . '" flashvars="host=picasaweb.google.com&hl=en_US&feat=flashalbum&interval=20&RGB=0x000000&feed=' . urlencode($slideshowurl) . '" pluginspage="http://www.macromedia.com/go/getflashplayer"></embed>';
  }
  else { // If this is a gallery and not a slideshow.
    // Get absolute path
    if ($picasafolder) {
      $url_to_fetch = $thisfolder;
      $mgalleryurl = md5($url_to_fetch);
      $pcachetemp = variable_get('brilliant_gallery_pcache', BRILLIANT_GALLERY_DEFAULT_CACHE_DIR);
      if ($pcachetemp == '' or $pcachetemp == file_directory_temp()) {
        // If there is no cache directory in the files folder, then we need to use the default temp dir
        $pcachetemp = file_directory_temp();
        $beg_realpcachetemp = file_directory_temp();
        $slashpcachetemp = '';
      }
      else {
        $slashpcachetemp = '/' . $pcachetemp;
        $beg_realpcachetemp = realpath(FILE_DIRECTORY_PATH) . $slashpcachetemp;
      }
      $mkdirek = $beg_realpcachetemp . '/bg_picasa_orig_' . $mgalleryurl;
      // We go and look at the images every time we render BG, because it sometimes happens that Picasa does not provide them all at the time of the initial fetching. So we will fetch any missing ones, and also replace the expired ones.
      // Vacilando 20091016: It is ABSOLUTELY CRUCIAL that this uses include_ONCE, otherwise picasa.inc was called twice in some cases.
      if ($url_to_fetch == '') {
        return;
      }

      // Create a special directory in temp dir for this Picasa album, then save the images there.

      @mkdir($mkdirek); // See http://drupal.org/node/368583#comment-1243641
      // Get it over http, not https
      $url_to_fetch = str_replace('https://', 'http://', $url_to_fetch);

      // Get the Picasa gallery feed XML file.
      $cacheid_url_to_fetch = 'bg_gallery_picasa_xml_' . md5($url_to_fetch);
      if ($cache = cache_get($cacheid_url_to_fetch) and !empty($cache->data)) {
        $html = $cache->data;
      }
      else {
        // Do your expensive calculations here...
        $timenow = REQUEST_TIME;
        $htmlobj = drupal_http_request($url_to_fetch);
        $html = $htmlobj->data; // Get just the data from the returned object.
        cache_set($cacheid_url_to_fetch, $html, 'cache', brilliant_gallery_get_time_this_many_seconds_from_now($bgcachexpire));
        #drupal_set_message('...2'.$html);
        watchdog('Brilliant Gal', 'Fetched XML from ' . $url_to_fetch . ' (' . strlen($html) . ' bytes). Elapsed time: ' . (REQUEST_TIME - $timenow) . ' seconds.');
      }

      // Links to full images are in this format: <enclosure type='image/jpeg' url='http://lh4.ggpht.com/_HkgjhlSfbZA/SWOgKhb6LyI/AAAAAAAABZ4/5AEu8AF01sw/DSC03173.JPG' length='0'/>
      $imgchunks = explode("' url='", $html);
      $fetchingnow = 0;
      $timenow = REQUEST_TIME;
      foreach ($imgchunks as $imgchunk) {

        // Make sure the script execution does not expire in case many images need to be fetched from remote server.
        drupal_set_time_limit(360);

        // Get the file name
        $chunk2 = explode("' length='", $imgchunk);
        $imgurl = trim($chunk2[0]);
        if (substr($imgurl, 0, 4) <> 'http') {
          continue;
        }
        $imagename = substr($imgurl, strrpos($imgurl, '/') + 1);
        #echo $imagename . '<br>';
        $foqen = $mkdirek . '/' . $imagename;

        $lastchanged = @filemtime($foqen); // Last file modification time, or FALSE on error.
        if ($lastchanged === FALSE OR ($timenow - $lastchanged > $bgcachexpire)) {
          // If the image is expired, we need to actively delete it, for the case that it was removed / hidden by the owner.
          @unlink($foqen);
          $imager = drupal_http_request($imgurl);
          $image = $imager->data;
          // It happens that the file size is 0 (image not fetched). In such case, don't write it.
          if (strlen($image) > 0) {
            $fp = fopen($foqen, 'w');
            watchdog('bg', $foqen.' ...ok<br>');
            fwrite($fp, $image);
            fclose($fp);
            $fetchingnow++;
          }
        }
      }

      if ($fetchingnow <> 0) {
        watchdog('Brilliant Gal', 'Fetched ' . $fetchingnow . ' images from ' . $url_to_fetch . '. Elapsed time: ' . (REQUEST_TIME - $timenow) . ' seconds.');
      }

      $absolpath = $slashpcachetemp . '/bg_picasa_orig_' . $mgalleryurl;
    }
    else {
      $absolpath = '/' . $galleryfolder;
    }
    # Make an array with images in this folder, and their properties.
    $temp = load_dir_to_array($absolpath, $imagewidth, $fullresolutionmaxwidth, $brilliant_gallery_sort, $imagecrop, $maximumnumbertoshow);
    $retval_dimensions = $temp[0];
    $imagemaxh = $temp[1];
    $maxpoct = count($retval_dimensions);

    if (!$picasafolder) {
      # Get a list of images that are forbidden from display.
      $excludedimgs = array();
      // any checkbox id that starts with user- we remember the current user's settings
      // any other id is global and we use user=0
      $dbresult = db_query("select qid from {brilliant_gallery_checklist} where qid like 'user-%' and state='1' ");
      foreach ($dbresult as $data) {
        $excludedimgs[] = $data->qid;
      }
    }

    // If we need to show just 1 image, then it is useless to calculate the UL width. Use width of the image.
    if ($maximumnumbertoshow == 1) {
      $columns = 0;
    }

    // If $columns is 0, columns will fill up the available space.
    if ($columns <> 0) {
      // Total 'width' controls the number of thumbnails in each row. The number, in pixels, should be calculated thusly: [maximum width desired for thumbnails] + [padding - if any, x 2] + [margin - if any, x 2] + [border - if any, x 2] x [number of thumbnails desired in each row]
      $style_ul = "font-size: 0px; margin: 0px; padding: 0px; width: " . (($imagewidth + 2 * $padding) * $columns) . "px;";
    }
    else {
      $style_ul = "font-size: 0; margin: 0px; padding: 0px;";
    }
    $column_count = 0;
    $setname = md5($absolpath);
    $result .= '<ul style="' . $style_ul . '">';
    $maxpoct_show = '';
    if ($maximumnumbertoshow <> '' and is_numeric($maximumnumbertoshow)) {
      $maxpoct_show = $maximumnumbertoshow;
    }
    $retval = array();
    $cnt = 0;
    for ($poct = 1; $poct <= $maxpoct; $poct++) {

      if (!$picasafolder) {
        # Skip any images that were excluded from display.
        # md5() must be made of the main gallery path etc. ending with the file name (unique identifier). E.g. albums/2008/20080321-25_belgicko_zasypane_snehom/dsc02784_w1000.jpg
        if (array_search('user-' . md5($galleryfolder . '/' . $retval_dimensions[$poct - 1]['file']), $excludedimgs) !== false) {
          continue;
        }
      }

      // If we are to display only images specified in $thumbshowbyname, skip the others here:
      if (!empty($thumbshowbyname)) {
        if (array_search($retval_dimensions[$poct - 1]['file'], $thumbshowbyname) === FALSE) {
          continue;
        }
      }

      // Begin only from $beginfromoverride image.
      if ($poct < $beginfromoverride) {
        continue;
      }

      $cnt += 1;
      if ($cnt > $maxpoct_show and $maxpoct_show <> '') {
        // Do not show more than $maxpoct_show (if defined).
        continue;
      }
      $retval[$poct - 1] = $retval_dimensions[$poct - 1]['file'];
      $fullimgpath = $path . '/' . $retval[$poct - 1];
      if (brilliant_gallery_testext($retval[$poct - 1])) {
        $caption = str_replace(array('.', '_'), ' ', basename($retval[$poct - 1], strrchr($retval[$poct - 1], '.')));
        $imgw = $retval_dimensions[$poct - 1]['imgw'];
        $imgh = $retval_dimensions[$poct - 1]['imgh'];
        $imgwbig = $retval_dimensions[$poct - 1]['imgwbig'];
        $imghbig = $retval_dimensions[$poct - 1]['imghbig'];
        $imgcrop = $retval_dimensions[$poct - 1]['imgcrop'];
        // If we need to show just 1 image, then it is useless to use the maximum image height for the LI. Use height of the image. Analogous for the width.
        // Cropping has no influence on single images.
        $imagemaxh_here = $imagemaxh;
        $imagewidth_here = $imagewidth;
        if ($maximumnumbertoshow == 1) {
          $imagemaxh_here = $imgh;
          $imagewidth_here = $imgw;
        }

        $style_li = "font-size: 0px; float: left; width: " . $imagewidth_here . "px; list-style: none; background: " . $bgcolour . "; height: " . $imagemaxh_here . "px; padding: " . $padding . "px; text-align: " . $thumbalignincell . "; margin: 0px; border: none;";
        $result .= ( '<li style="' . $style_li . '">' . "\n");
        # Get this module's path:
        $modulepath = drupal_get_path('module', 'brilliant_gallery');
        if (BG_LANGCODE <> '') {
          $modulepath = str_replace('/' . BG_LANGCODE . '/', '/', $modulepath);
        }
        # Non-clean URLs need removing ?q=
        $modulepath = str_replace("?q=", "", $modulepath);
        # Important to begin with the "/" otherwise thumbnails in non-root folders fail. See http://drupal.org/node/175292
        $bgimgproperties_array = array(
          // This array contains everything that makes the image unique.
          'imgp' => $absolpath . '/' . $retval[$poct - 1],
          //'imgp' => $retval[$poct - 1],
          'imgw' => $imgwbig,
          'imgh' => $imghbig,
          'imgcrop' => 'no', # Not $imgcrop because zoomed images do never need to be cropped (only thumbnails)!
        );
        $bgimgproperties_array_token = brilliant_gallery_image_properties_set($bgimgproperties_array);
        $pcache = variable_get('brilliant_gallery_pcache', BRILLIANT_GALLERY_DEFAULT_CACHE_DIR);
        $imgsrc = file_create_url(FILE_DIRECTORY_PATH . '/' . $pcache . '/bg_cached_resized_' . $bgimgproperties_array_token . '.' . brilliant_gallery_get_extension($retval[$poct - 1]));
        $result .= '<a href="' . $imgsrc . '"';

        switch ($overbrowser) {

          case 'colorbox':
            $result .= ' class="colorbox" rel="bg-' . $setname . '"';
            break;

          case 'lightbox':
            $result .= ' rel="lightbox[' . $setname . ']"';
            break;

          default:
            break;

        }

        if ($showcaption <> '') {
          if ($showcaption <> 'filename') {
            $caption = $showcaption;
          }
          $result .= ' title="' . $caption . '"';
        }
        $result .= '>';
        # Important to begin with the "/" otherwise thumbnails in non-root folders fail. See http://drupal.org/node/175292
        $bgimgproperties_array = array(
          // This array contains everything that makes the image unique.
          'imgp' => $absolpath . '/' . $retval[$poct - 1],
          //'imgp' => $retval[$poct - 1],
          'imgw' => $imgw,
          'imgh' => $imgh,
          'imgcrop' => $imgcrop,
        );
        $bgimgproperties_array_token = brilliant_gallery_image_properties_set($bgimgproperties_array);
        $pcache = variable_get('brilliant_gallery_pcache', BRILLIANT_GALLERY_DEFAULT_CACHE_DIR);
        $imgsrc = file_create_url(FILE_DIRECTORY_PATH . '/' . $pcache . '/bg_cached_resized_' . $bgimgproperties_array_token . '.' . brilliant_gallery_get_extension($retval[$poct - 1]));
        $secur = empty($_SERVER["HTTPS"]) ? '' : ($_SERVER["HTTPS"] == "on") ? "s" : "";
        if ($secur == 's') {
          $imgsrc = str_replace('http://', 'https://', $imgsrc);
        }
        $result .= '<img style="border: 0px; margin: 0px; padding: 0px;" alt="" src="' . $imgsrc . '" />';
        $result .= '</a>' . "\n";
      }
      else {
        $fosiz = ceil($imagewidth / 13);
        $style_li = "font-size:" . $fosiz . "px; float: left; width: " . $imagewidth . "px; list-style: none; background: " . $bgcolour . "; height: " . $imagemaxh . "px; padding: " . $padding . "px; text-align: " . $thumbalignincell . "; margin: 0px; border: none;";
        $result .= ( '<li style="' . $style_li . '">' . "\n");
        $result .= '<a href="' . $fullimgpath . '">';
        $result .= $retval[$poct - 1];
        $result .= '</a>' . "\n";
      }
      $result .= '</li>' . "\n";

      $column_count += 1;
    }
    $result .= "</ul>\n";
    $result .= "<div style=\"clear: both;\"></div>\n"; // Add a new line (needed because of the float). See http://stackoverflow.com/questions/2580772/how-do-i-get-a-new-line-after-using-floatleft
  }

  return $result;
}
