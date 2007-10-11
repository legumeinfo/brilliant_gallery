<?php
/* $Id$ */

/*
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
*/

drupalize();
function drupalize() {
 while (!@stat('./includes/bootstrap.inc')) {
   chdir ('..');
 }
 require_once './includes/bootstrap.inc';
 drupal_bootstrap(DRUPAL_BOOTSTRAP_FULL);
#drupal_cron_run();
}

#chdir ('../../../../');
#require_once './includes/bootstrap.inc';
#require_once '../../../../../includes/bootstrap.inc';
#drupal_bootstrap(DRUPAL_BOOTSTRAP_FULL);

$my_data = unserialize(base64_decode( resizeimage_wrapper() ));
header($my_data[0]);
echo $my_data[1];

function resizeimage_wrapper($reset = FALSE) {
		global $user;
			$userId = $user->uid;
		$bgcacheid = 'bg_' . $userId . '_' . md5($_GET['imgp'] . $_GET['imgw'] . $_GET['imgh']);
		#echo $bgcacheid;
		static $my_data;
		#echo '0.... ';
		if (!isset($my_data) || $reset) {
					if (!$reset && ($cache = cache_get($bgcacheid)) && !empty($cache->data)) {
						$my_data = $cache->data;
							#echo '1.... ' . $my_data;
				}
				else {
						// Do your expensive calculations here, and populate $my_data
						// with the correct stuff..
						$my_data = resizeimage ( $_GET['imgp'], $_GET['imgw'], $_GET['imgh'] );
							#echo ' 2.... ' . $my_data;
						cache_set($bgcacheid, 'cache', $my_data, time() + 3600*24*7);
				}
		}
  return $my_data;
}

function resizeimage( $imgp, $imgw, $imgh ) {
         $imagepath = base64_decode( $imgp );
         #echo '.... ' . base64_decode( $imgp );
         #flush();die(' stop!');
         $suffix = strtolower(substr($imagepath, -4)); # Thanks to Michał Albrecht!
         if ( $suffix == ".gif" ) {
														#Header("Content-type: image/gif");
              $head = "Content-type: image/gif";
              $img = imagecreatefromgif( $imagepath );
            } else if ( $suffix == ".jpg" or $suffix == "jpeg" ) { # Thanks to Michał Albrecht!
														#Header("Content-type: image/jpeg");
              $head = "Content-type: image/jpeg";
              $img = imagecreatefromjpeg( $imagepath );
            } else if ( $suffix == ".png" ) {
														#Header("Content-type: image/png");
              $head = "Content-type: image/png";
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
         ob_start();
           if ( $suffix == ".gif" ) {
                Imagegif($img);
              } else if ( $suffix == ".jpg" or $suffix == ".jpeg" ) {
                Imagejpeg($img,'',80);
              } else if ( $suffix == ".png" ) {
                Imagepng($img);
              }
         $result = ob_get_clean();
         #ImageDestroy($img);
         $result = mysql_escape_string(base64_encode(serialize(array($head, $result))));
         return $result;
       }

?>
