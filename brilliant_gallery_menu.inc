<?php

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function brilliant_gallery_menu() {

  $items = array();

  $items['admin/config/media/brilliant_gallery'] = array(
    'title' => 'Brilliant gallery',
    'description' => 'Brilliant gallery module settings.',
    'page callback' => 'drupal_get_form',
    'page arguments' => array('brilliant_gallery_admin'),
    'access arguments' => array('administer brilliant_gallery'),
    'type' => MENU_NORMAL_ITEM,
  );
  // Same as 'admin/config/media/brilliant_gallery' -- just for the convenience/clarity in a dropdown menu.
  $items['admin/config/media/brilliant_gallery/settings'] = array(
    'title' => 'Brilliant gallery',
    'description' => 'Brilliant gallery module settings.',
    'page callback' => 'drupal_get_form',
    'page arguments' => array('brilliant_gallery_admin'),
    'access arguments' => array('administer brilliant_gallery'),
    'type' => MENU_DEFAULT_LOCAL_TASK,
    'weight' => 10,
  );

  # A shortcut to the permissions settings for this module.
  $items['admin/config/media/brilliant_gallery/permissions'] = array(
    'title' => 'Configure permissions',
    'description' => 'Configure access permissions for the Brilliant gallery module.',
    'page callback' => 'brilliant_gallery_perms',
    'access arguments' => array('administer brilliant_gallery'),
    'type' => MENU_LOCAL_TASK,
    'weight' => 12,
  );

  $items['admin/config/media/brilliant_gallery/manage'] = array(
    'title' => 'Manage galleries',
    //'description' => 'Manage galleries displayed using the Brilliant gallery module - e.g. visibility of individual images, etc.',
    'page callback' => 'render_brilliant_gallery_manage',
    'access arguments' => array('administer brilliant_gallery'),
    'type' => MENU_LOCAL_TASK,
    'weight' => 11,
  );

  $items['brilliant_gallery'] = array(
    'title' => 'Brilliant gallery',
    'page callback' => 'brilliant_gallery_all',
    'access arguments' => array('access brilliant_gallery'),
    'type' => MENU_CALLBACK,
  );

  $pcache = variable_get('brilliant_gallery_pcache', BRILLIANT_GALLERY_DEFAULT_CACHE_DIR);
  $items[FILE_DIRECTORY_PATH . '/' . $pcache] = array(
    'title' => 'render image',
    'page callback' => 'brilliant_gallery_deliver_image',
    'access arguments' => array('access brilliant_gallery'),
    'type' => MENU_CALLBACK,
  );

  # 2 menu callback functions to implement the ajax load and save routines
  $items['bgchecklist/loadall'] = array(
    'page callback' => 'brilliant_gallery_checklist_loadall',
    'type' => MENU_CALLBACK,
    'access arguments' => array('access brilliant_gallery'),
  );
  $items['bgchecklist/save'] = array(
    'page callback' => 'brilliant_gallery_checklist_save',
    'type' => MENU_CALLBACK,
    'access arguments' => array('administer brilliant_gallery'),
  );

  return $items;
}
