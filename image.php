<?php
/* $Id$ */

#if (strpos(base64_decode($_GET['imgp']), "://") !== false) {
/* Check for bad URL inputs */
$urlpath = base64_decode($_GET['imgp']);
if (strpos($urlpath, "://") !== false ||
    strpos($urlpath, "..") !== false ||
    preg_match('/\D/', ($_GET['imgw'] . $_GET['imgh'])) > 0 ||
    ($_GET['imgw'] + $_GET['imgh']) < 10 ||
    ($_GET['imgw'] + $_GET['imgh']) > 20000 ) {
  header("HTTP/1.0 404 Not Found");
  exit();
}

drupalize();
function drupalize() {
  while (!@stat('./includes/bootstrap.inc')) {
    chdir('..');
  }
  #module_load_include('/includes/bootstrap.inc', 'image', 'includes/bootstrap');
  require_once './includes/bootstrap.inc';
  require_once './includes/file.inc';
  drupal_bootstrap(DRUPAL_BOOTSTRAP_FULL); // See http://drupal.org/node/211378#comment-924059
  #drupal_bootstrap(DRUPAL_BOOTSTRAP_DATABASE);
  #drupal_cron_run();
}

$imagepath = realpath(file_directory_path() . $urlpath);
#watchdog('Brilliant Gal','imgp: '.$imagepath);

// Crucial - to suppress Devel (if installed and enabled) output appearing in the generated XML!
$GLOBALS['devel_shutdown'] = FALSE;

#chdir ('../../../../');
#module_load_include('/includes/bootstrap.inc', 'image', 'includes/bootstrap');
#module_load_include('./../../../../includes/bootstrap.inc', 'image', '');
#drupal_bootstrap(DRUPAL_BOOTSTRAP_FULL);

#if ( $_SERVER['SERVER_ADDR'] == '64.13.192.90' ) {
/*
if (variable_get('brilliant_gallery_cache', 'd') == 'f') {
  #echo '.....................' . $_SERVER['SERVER_ADDR'];
  #drupal_set_message('cachetype1 '.variable_get('brilliant_gallery_cache', 'd'));
  #watchdog('Brilliant Gal', '1 '.variable_get('brilliant_gallery_cache'));
  $my_data = resizeimage_wrapper_filecache();
}
else {
  #drupal_set_message('cachetype2 '.variable_get('brilliant_gallery_cache', 'd'));
  #watchdog('Brilliant Gal', '2 '.variable_get('brilliant_gallery_cache'));
  $my_data = resizeimage_wrapper_dbcache();
}
*/
$my_data = resizeimage_wrapper_dbcache();

#echo '....'. sess_read('vacilando');
header($my_data[0]);
echo base64_decode($my_data[1]);
// IMPORTANT to exit() - otherwise some process after BG adds strings and breaks the image!
exit();
/*
function resizeimage_wrapper_filecache() {
  $bgcacheid = 'bg_'. md5($_GET['imgp'] . $_GET['imgw'] . $_GET['imgh']);
  #echo '. 0.... ';
  $bgcachexpire = variable_get('brilliant_gallery_cache_duration',90) * 24 * 3600; // Cache expiration time in days.
  // Tested that both relative (eg sites/all/files/cache) and absolute (eg /home/data/tmp) tmp path settings work OK here.
  $cachetemp = variable_get('brilliant_gallery_pcache', file_directory_temp());
  $cachedfile = $cachetemp .'/'. $bgcacheid;
  $fileexists = false;
  $fileexists = file_exists($cachedfile);
  $timenow = time();
  $lastchanged = $timenow;
  $lastchanged = @filemtime($cachedfile);
  $fileexpired = false;
  if ($timenow - $lastchanged > $bgcachexpire){
    $fileexpired = true;
    // If the image is expired, we need to actively delete it, for the case that it was removed / hidden by the owner.
    @unlink($cachedfile);
  }
  if (!$fileexists or $fileexpired) {
    #echo '. 1.... ';
    // Cache file does not exist or is too old.
    #$my_data = resizeimage($_GET['imgp'], $_GET['imgw'], $_GET['imgh']);
    $my_data = resizeimage($_GET['imgp'], $_GET['imgw'], $_GET['imgh'], $_GET['imgcrop']);
    // Now put $my_data to cache!
    $fh = fopen($cachedfile, "w+");
    fwrite($fh, $my_data);
    fclose($fh);
    $my_data = unserialize($my_data);
  }
  else {
    #echo '. 2.... ';
    // Cache file exists.
    $my_data = unserialize(file_get_contents($cachedfile));
  }
  return $my_data;
}
*/

function resizeimage_wrapper_dbcache($reset = FALSE) {
  #global $user;
  #$userId = $user->uid;
  $bgcachexpire = variable_get('brilliant_gallery_cache_duration',90) * 24 * 3600; // Cache expiration time in days.
  $bgcacheid = 'bg_'. md5($_GET['imgp'] . $_GET['imgw'] . $_GET['imgh']);
  #echo $bgcacheid;
  static $my_data;
  #echo '0.... ';
  if (!isset($my_data) || $reset) {
    if (!$reset and ($cache = cache_get($bgcacheid)) and !empty($cache->data)) {
      #$my_data = $cache->data; echo '-1.... ' . $my_data;
      // Crucial to unserialize for the immediate use!
        $my_data = unserialize($cache->data);
      #echo $my_data;
    }
    else {
      // Do your expensive calculations here, and populate $my_data
      // with the correct stuff..
      #$my_data = resizeimage($_GET['imgp'], $_GET['imgw'], $_GET['imgh']);
      $my_data = resizeimage($_GET['imgp'], $_GET['imgw'], $_GET['imgh'], $_GET['imgcrop']);
      #echo ' -2.... ' . $bgcachexpire . ' // ' . $my_data;
      # For some reason I could not use: mysql_escape_string($my_data)
      #cache_set($bgcacheid, 'cache', time() + $bgcachexpire, $my_data);
      cache_set($bgcacheid, $my_data, 'cache', time() + $bgcachexpire);
      # FOR DRUPAL6 MUST USE:
      #cache_set($bgcacheid,  $my_data, time() + $bgcachexpire); # For some reason I could not use: mysql_escape_string($my_data)
      // Crucial to unserialize for the immediate use!
        $my_data = unserialize($my_data);
    }
  }
  return $my_data;
}

#function resizeimage($imgp, $imgw, $imgh) {
function resizeimage($imgp, $imgw, $imgh, $imgcrop) {
  #$imagepath = base64_decode($imgp);
  #echo '.... ' . base64_decode( $imgp );
  #flush();die(' stop!');
  global $imagepath;
  
  $suffix = strtolower(substr($imagepath, -4));
  $imgsize = @getimagesize($imagepath);
  # http://be.php.net/getimagesize
  $head = "Content-type: {$imgsize['mime']}";
  if ($suffix == ".gif") {
    #$head = "Content-type: image/gif";
    $img = @imagecreatefromgif($imagepath);
    if (!$img) {
      brokenimage("Error loading GIF");
    }
  }
  else if ($suffix == ".jpg" or $suffix == "jpeg") {
    #$head = "Content-type: image/jpeg";
    $img = @imagecreatefromjpeg($imagepath);
    if (!$img) {
      brokenimage("Error loading JPG");
    }
  }
  else if ($suffix == ".png") {
    #$head = "Content-type: image/png";
    $img = @imagecreatefrompng($imagepath);
    if (!$img) {
      brokenimage("Error loading PNG");
    }
  }
  # Resize the image
  $src_h   = ImageSY($img);
  $src_w   = ImageSX($img);
  #$dst_img = imagecreatetruecolor($imgw, $imgh);
  #imagecopyresampled($dst_img, $img, 0, 0, 0, 0, $imgw, $imgh, $src_w, $src_h);
  $dst_img = 0;
  if ($imgcrop) {
    if ($src_h>$src_w) {
      // portrait
      $dst_img = imagecreatetruecolor($imgh, $imgh);
      imagecopyresampled($dst_img, $img, 0, 0, 0, ($src_h-$src_w)/2 , $imgh, $imgh, $src_w, $src_w);
    }
    else {
      // landscape
      $dst_img = imagecreatetruecolor($imgw, $imgw);
      imagecopyresampled($dst_img, $img, 0, 0, ($src_w-$src_h)/2, 0 , $imgw, $imgw, $src_h, $src_h);
    }
  }
  else {
    $dst_img = imagecreatetruecolor($imgw, $imgh);
    imagecopyresampled($dst_img, $img, 0, 0, 0, 0, $imgw, $imgh, $src_w, $src_h);
  }
  $img = $dst_img;
  imageinterlace($img, 1);
  imagecolortransparent($img);
  ob_start();
  if ($suffix == ".gif") {
    Imagegif($img);
  }
  else if ($suffix == ".jpg" or $suffix == ".jpeg") {
    Imagejpeg($img, '', 80);
  }
  else if ($suffix == ".png") {
    Imagepng($img);
  }
  $result = ob_get_clean();
  #ImageDestroy($img);
  $result = serialize(array($head, base64_encode($result)));
  return $result;
}

function brokenimage($msg) {
  $im  = imagecreatetruecolor(150, 30);
  $bgc = imagecolorallocate($im, 0, 0, 0);
  $tc  = imagecolorallocate($im, 255, 255, 255);
  imagefilledrectangle($im, 0, 0, 150, 30, $bgc);
  imagestring($im, 1, 5, 5, $msg, $tc);
  imagejpeg($im);
  exit();
}
