<?php

$systempath = '/home/14656/domains/vacilando.org/html/quidne/';

# Require some PEAR fruit :-)
  require_once('Cache/Lite/Function.php');
  $cachloptions = array(
                        'cacheDir' => '/home/14656/domains/vacilando.org/cachelite_tmp/',
                        'automaticCleaningFactor' => 100,
                        'hashedDirectoryLevel' => 9 # THIS CANNOT BE CHANGED WHILE CACHE IS BEING ACCUMULATED! I therefore cleaned the cache and set it on 9 on 2006081.
                      );
  # For flexibility, 'caching' => true and 'lifeTime' => 3600 will be set for each class instance.

if ( $_GET['imgp'] <> '' ) { # Means there's a request to draw one image.
     $cache = new Cache_Lite_Function( array_merge( $GLOBALS['cachloptions'], array( 'caching' => false, 'lifeTime' => (60*60*24 * 30) ) ) ); # Photocollections can be cached for a long time as they do not change frequently. Besides, here it is not crucial if all pictures show randomly immediately or not.
      $cache->call( 'resizeimage', $_GET['imgp'], $_GET['imgw'], $_GET['imgh'] );
     exit();
   }

function resizeimage( $imgp, $imgw, $imgh ) {
         global $systempath;
         $imagepath = $systempath . base64_decode( $imgp );
         $suffix = strtolower( substr( $imagepath, strlen( $imagepath ) - 4 ) );
         if ( $suffix == ".gif" ) {
              Header("Content-type: image/gif");
              $img = imagecreatefromgif( $imagepath );
            } else if ( $suffix == ".jpg" or $suffix == ".jpeg" ) {
              Header("Content-type: image/jpeg");
              $img = imagecreatefromjpeg( $imagepath );
            } else if ( $suffix == ".png" ) {
              Header("Content-type: image/png");
              $img = imagecreatefrompng( $imagepath );
            }
         # Resize the image
           $src_h = ImageSY( $img );
           $src_w = ImageSX( $img );
           $dst_img = imagecreatetruecolor( $imgw, $imgh );
           imagecopyresampled( $dst_img, $img, 0, 0, 0, 0, $imgw, $imgh, $src_w, $src_h );
           $img = $dst_img;
         imageinterlace ( $img, 1 );
         imagecolortransparent ( $img );
         if ( $suffix == ".gif" ) {
              Imagegif($img);
            } else if ( $suffix == ".jpg" or $suffix == ".jpeg" ) {
              Imagejpeg($img,'',80);
            } else if ( $suffix == ".png" ) {
              Imagepng($img);
            }
         ImageDestroy($img);
         return;
       }

?>
