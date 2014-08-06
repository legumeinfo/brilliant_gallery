<?php

/**
 * Implements hook_field_formatter_info().
 *
 */
function brilliant_gallery_field_formatter_info() {
  return array(
    'brilliant_gallery_render' => array(
      'label' => t('Brilliant Gallery: local path, BT Sync secret (default 33 bytes only), Picasa RSS link or Google+ album URL'),
      'field types' => array('text'),
    ),
    'brilliant_gallery_single' => array(
      'label' => t('A random image from Brilliant Gallery: local path, BT Sync secret (default 33 bytes only), Picasa RSS link or Google+ album URL'),
      'field types' => array('text'),
    ),
  );
}

/**
 * Implements hook_field_formatter_view().
 *
 */
function brilliant_gallery_field_formatter_view($entity_type, $entity, $field, $instance, $langcode, $items, $display) {
  $element = array();

  switch ($display['type']) {

    case 'brilliant_gallery_render':
      foreach ($items as $delta => $item) {
        //dpm($item);
        $pathurl = trim($item['value']);
        $pathurl_btsync_check = brilliant_gallery_pathurl_btsync_check($pathurl);
        // If this is a BT Sync folder (secret), change $pathurl
        if ($pathurl_btsync_check) $pathurl = $pathurl_btsync_check;
        //dpm('pathurl: '.$pathurl);
        if (function_exists('replace_brilliant_gallery_tags') AND $pathurl <> ''){
          $result = '';
          //$bgcode = '[bg|'.$pathurl.'|0|107|sort|100000|#ffffff]';
          $bgcode = '[bg|'.$pathurl.']';
          $gallery = replace_brilliant_gallery_tags($bgcode);
          //dpm(strip_tags($gallery));
          if (strip_tags($gallery) <> ''){ // Useful?
            $result .= $gallery; // <br clear=all><br clear=all>
          }
          $element[$delta] = array(
            '#markup' => $result,
          );
        }
      }
      break;

    case 'brilliant_gallery_single':
      foreach ($items as $delta => $item) {
        //dpm($item);
        $pathurl = trim($item['value']);
        $pathurl_btsync_check = brilliant_gallery_pathurl_btsync_check($pathurl);
        // If this is a BT Sync folder (secret), change $pathurl
        if ($pathurl_btsync_check) $pathurl = $pathurl_btsync_check;
        if (function_exists('replace_brilliant_gallery_tags') AND $pathurl <> ''){
          $result = '';
          // [bg|path/to/your/gallery/folder/without/wrapping/slashes |columncountoverride|widthoverride|sortorrandomoverride|maximumnumbertoshow|colouroverride|beginfromoverride|caption-yes-no-text]
          // maximumnumbertoshow = 1
          // random
          // columnoverride = 0 ... because it does not matter, we show just 1 image
          // widthoverride ... taken from settings
          $bgcode = '[bg|'.$pathurl.'|0|'.variable_get('brilliant_gallery_maximagewidth', 150).'|random|1]';
          $gallery = replace_brilliant_gallery_tags($bgcode);
          //dpm(strip_tags($gallery));
          if (strip_tags($gallery) <> ''){ // Useful?
            $result .= $gallery; // <br clear=all><br clear=all>
          }
          $element[$delta] = array(
            '#markup' => $result,
          );
        }
      }
      break;

  }

  //dpm($element);
  return $element;
}

function brilliant_gallery_pathurl_btsync_check($pathurl){
  // Only continue if it looks like a BT Sync secret (must be 33 chars)
  // See http://forum.bittorrent.com/topic/29304-rules-for-valid-secret/
  preg_match("/[A-Z2-7]{33}/", $pathurl, $output_array);
  if (!array_key_exists(0, $output_array)) {
    return FALSE;
  }
  // Only continue if the main gallery folder has been configured.
  if (variable_get('brilliant_gallery_folder', '') == '') break;
  $btsyncdir = 'public://'.variable_get('brilliant_gallery_folder', '').'/btsync';
  $dirtest = file_prepare_directory($btsyncdir, FILE_CREATE_DIRECTORY);
  // Only continue if the special btsync directory exists and is writable
  if (!$dirtest) {
    watchdog('Brilliant Gal', 'Special folder "btsync" cannot be created or is not writable in ' . $btsyncdir = 'public://'.variable_get('brilliant_gallery_folder', ''));
    return FALSE;
  }
  // OK now let's check or add the particular sync folder
  $btsyncgallerydir = $btsyncdir.'/'.$pathurl;
  $dirtest = file_prepare_directory($btsyncgallerydir, FILE_CREATE_DIRECTORY);
  // Only continue if the special btsync directory exists and is writable
  if (!$dirtest) {
    watchdog('Brilliant Gal', 'Sync folder "'.$pathurl.'" cannot be created or is not writable in ' . $btsyncdir);
    return FALSE;
  }
  if (function_exists('btsync_method_callback')){
    btsync_method_callback('add_folder', array(
      'dir' => drupal_realpath($btsyncgallerydir), // Did not work without drupal_realpath here.
      'secret' => $pathurl,
    ));
  }
  else {
    watchdog('Brilliant Gal', 'BT Sync gallery formatter selected but the BitTorrent Sync API module ("btsync") is not enabled.');
    return FALSE;
  }
  $pathurl = 'btsync/'.$pathurl;
  return $pathurl;
}
