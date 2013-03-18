<?php

/**
 * Implements hook_field_formatter_info().
 *
 */
function brilliant_gallery_field_formatter_info() {
  return array(
    'brilliant_gallery_render' => array(
      'label' => t('Brilliant Gallery (local path, Picasa RSS link or Google+ album URL)'),
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

  }

  //dpm($element);
  return $element;
}


