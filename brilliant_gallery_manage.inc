<?php

function render_brilliant_gallery_manage() {

  // Only on this management page - include the brilliant_gallery_checklist .js file.
  drupal_add_js(drupal_get_path('module', 'brilliant_gallery') . '/bgchecklist.js');

  $result = '';
  # Beginning of brilliant_gallery_checklist form.
  # Removed action "/node/$nid"
  # Hacked $nid ... 9999999 - cannot be empty!!
  $token = drupal_get_token('render_brilliant_gallery_manage');
  $ac_header = <<<HEADER
      <form action="" method="post" id="bgchecklist" class="$token">
      <div>
      <div id="nodeid">
      <input name="nid" id="edit-nid" value="9999999" type="hidden">
      </div>
HEADER;
  $result .= $ac_header;
  $overbrowser = variable_get('brilliant_gallery_overbrowser', 'colorbox');
  $galleryfolder = variable_get('brilliant_gallery_folder', '');
  $path = url(FILE_DIRECTORY_PATH . '/' . $galleryfolder, array('absolute' => TRUE));

  # url() ads i18n codes to the URL ... we need to remove them here...
  if (BG_LANGCODE <> '') {
    $path = str_replace('/' . BG_LANGCODE . '/', '/', $path);
  }

  # Non-clean URLs need removing ?q=
  $path = str_replace("?q=", "", $path);
  $rp = FILE_DIRECTORY_PATH;
  if ($galleryfolder <> '') {
    $rp .= '/' . $galleryfolder;
  }

  $absolpath = $rp;
  $result .= '<p>';
  $result .= '<p>This page allows you to set or unset visibility of each image (data item) in all Brilliant galleries there are on this website.';
  ob_start();
  #$recurs = bg_iterative_recurse_dir($absolpath);
  $recurs = bg_iterative_recurse_dir(realpath($absolpath));
  #echo '<pre>'; print_r( $recurs ); echo '</pre>';
  #echo '<pre>'; print_r( $retval_dimensions ); echo '</pre>';
  $result .= ob_get_contents();
  $dirshavefiles = array();
  foreach ($recurs as $key => $val) {
    $temp = explode('/', $val);
    $tempf = $temp[sizeof($temp) - 1];
    unset($temp[sizeof($temp) - 1]);
    $tempd = implode('/', $temp);
    $dirshavefiles[$tempd][] = $tempf;
  }
  ksort($dirshavefiles);
  $result .= '<p><b>Select an image folder:</b><ol>';
  foreach ($dirshavefiles as $key => $val) {
    # Get just the folder name in the main gallery folder.
    if ($galleryfolder == '' AND @$path_middle == '') { // ??? needed? @$path_middle
      $rootfolder = '/';
    }
    else {
      $rootfolder = '';
    }
    $path_middle = str_replace(realpath($absolpath), '', $key);
    $gallerypath = '/' . $galleryfolder . $path_middle;
    $setname = md5($gallerypath);
    $result .= '<li><a href="?fld=' . $galleryfolder . $path_middle . '">' . $galleryfolder . $path_middle . $rootfolder . '</a></li>';
    if ($galleryfolder . $path_middle == @$_GET['fld']) {
      # User has asked to manage images in this folder.
      $tablerows = array();
      $temp = load_dir_to_array($gallerypath, variable_get('brilliant_gallery_maximagewidth', 150), variable_get('brilliant_gallery_maxwidth', '1000'), 1, variable_get('brilliant_gallery_crop', 'no'));
      $retval_dimensions = $temp[0];
      $maxpoct = count($retval_dimensions);
      $retval = array();
      $cnt = 0;
      for ($poct = 1; $poct <= $maxpoct; $poct++) {
        $cnt += 1;
        $retval[$poct - 1] = $retval_dimensions[$poct - 1]['file'];
        $fullimgpath = $path . $path_middle . '/' . $retval[$poct - 1];
        if (brilliant_gallery_testext($retval[$poct - 1])) {
          $caption = str_replace(array('.', '_'), ' ', basename($retval[$poct - 1], strrchr($retval[$poct - 1], '.')));
          $imgw = $retval_dimensions[$poct - 1]['imgw'];
          $imgh = $retval_dimensions[$poct - 1]['imgh'];
          $imgwbig = $retval_dimensions[$poct - 1]['imgwbig'];
          $imghbig = $retval_dimensions[$poct - 1]['imghbig'];
          $modulepath = drupal_get_path('module', 'brilliant_gallery');
          if (BG_LANGCODE <> '') {
            $modulepath = str_replace('/' . BG_LANGCODE . '/', '/', $modulepath);
          }
          # Non-clean URLs need removing ?q=
          $modulepath = str_replace("?q=", "", $modulepath);
          $displayimage = '';
          # Important to begin with the "/" otherwise thumbnails in non-root folders fail. See http://drupal.org/node/175292
          $bgimgproperties_array = array(
            // This array contains everything that makes the image unique.
            'imgp' => $gallerypath . '/' . $retval[$poct - 1],
            //'imgp' => $retval[$poct - 1],
            'imgw' => $imgwbig,
            'imgh' => $imghbig,
          );
          $bgimgproperties_array_token = brilliant_gallery_image_properties_set($bgimgproperties_array);
          $pcache = variable_get('brilliant_gallery_pcache', BRILLIANT_GALLERY_DEFAULT_CACHE_DIR);
          $imgsrc = file_create_url(FILE_DIRECTORY_PATH . '/' . $pcache . '/bg_cached_resized_' . $bgimgproperties_array_token . '.' . brilliant_gallery_get_extension($retval[$poct - 1]));
          $displayimage .= '<a href="' . $imgsrc . '"';

          switch ($overbrowser) {

            case 'colorbox':
              $displayimage .= ' class="colorbox" rel="bg-' . $setname . '"';
              break;

            case 'lightbox':
              $displayimage .= ' rel="lightbox[' . $setname . ']"';
              break;

            default:
              break;
          }

          if (@$showcaption <> '') {
            if (@$showcaption <> 'filename') {
              $caption = $showcaption;
            }
            $displayimage .= ' title="' . $caption . '"';
          }
          $displayimage .= '>';
          $bgimgproperties_array = array(
            // This array contains everything that makes the image unique.
            'imgp' => $gallerypath . '/' . $retval[$poct - 1],
            'imgw' => $imgw,
            'imgh' => $imgh,
          );
          $bgimgproperties_array_token = brilliant_gallery_image_properties_set($bgimgproperties_array);
          $pcache = variable_get('brilliant_gallery_pcache', BRILLIANT_GALLERY_DEFAULT_CACHE_DIR);
          $imgsrc = file_create_url(FILE_DIRECTORY_PATH . '/' . $pcache . '/bg_cached_resized_' . $bgimgproperties_array_token . '.' . brilliant_gallery_get_extension($retval[$poct - 1]));
          $secur = empty($_SERVER["HTTPS"]) ? '' : ($_SERVER["HTTPS"] == "on") ? "s" : "";
          if ($secur == 's') {
            $imgsrc = str_replace('http://', 'https://', $imgsrc);
          }
          $displayimage .= '<img style="border: 0; margin:0px; padding:0px;" alt="" src="' . $imgsrc . '" />';
          $displayimage .= '</a>';
        }
        else {
          $displayimage .= '<a href="' . $fullimgpath . '">';
          $displayimage .= $retval[$poct - 1];
          $displayimage .= '</a>';
        }
        $tablerows[$cnt][0] = $displayimage;
        $tablerows[$cnt][1] = '<font size="-1">' . $retval[$poct - 1] . '</font>';
        # We need some code for brilliant_gallery_checklist
        $tmp = '';
        # E.g. albums/2008/20080321-25_belgicko_zasypane_snehom/dsc02784_w1000.jpg
        $tmp .= '<div class="bgchecklist"> <div class="form-item"> <input name="' . 'uid-' . md5($_GET['fld'] . '/' . $retval[$poct - 1]) . '" id="' . 'uid-' . md5($_GET['fld'] . '/' . $retval[$poct - 1]) . '" value="1" class="form-brilliant_gallery_checklist-checkbox" type="checkbox"> <label class="option"> <font color=green>visible</font> </label> </div></div>';
        $tablerows[$cnt][2] = $tmp;
      }
      $header = array('File name', 'Thumbnail', 'Gallery display');
      $data = array();
      foreach ($tablerows as $x => $val) {
        $data[] = array('data' => array($tablerows[$x][0], $tablerows[$x][1], $tablerows[$x][2]));
      }
      $result .= theme('table', array('header' => $header, 'rows' => $data));
      $result .= '<p>';
    }
  }
  $result .= '</ol>';
  # End of brilliant_gallery_checklist form.
  $result .= "</div></form>";
  return $result;
}
