<?php

#$url_to_fetch = "http://picasaweb.google.com/data/feed/base/user/tjfulopp/albumid/5288246472868572145?alt=rss&kind=photo&hl=en_US";
if ($url_to_fetch == '') return;

// Create a special directory in temp dir for this Picasa album, then save the images there.
#$mgalleryurl = md5($url_to_fetch);
#$mkdirek = '/tmp/bg_picasa_'. $mgalleryurl;
//exec('mkdir '.$mkdirek);
#watchdog('Brilliant Gal', mt_rand(1000,9999) . ' ' . arg(1).' pinc '.microtime());
/**
    ob_start();
        debug_print_backtrace();
    $btrace = ob_get_contents();
    ob_end_clean();
*/
#watchdog('Brilliant Gal', mt_rand(1000,9999) . ' ' . arg(1).' pinc '.$btrace);

@mkdir($mkdirek); // See http://drupal.org/node/368583#comment-1243641

// Get the Picasa gallery feed XML file.
$cacheid_url_to_fetch = 'bg_gallery_picasa_xml_'.md5($url_to_fetch);
if ($cache = cache_get($cacheid_url_to_fetch) and !empty($cache->data)) {
  $html = $cache->data;
  /*
  global $user; $userId = $user->uid;
  if ($userId == 1){
    drupal_set_message('got it from cache '.$url_to_fetch);
  }
  */
}
else {
  // Do your expensive calculations here...
  $timenow = time();
  $htmlobj = drupal_http_request($url_to_fetch);
  $html = $htmlobj->data; // Get just the data from the returned object.
  #cache_set($cacheid_url_to_fetch, $html, 'cache', $timenow + $bgcachexpire);
  cache_set($cacheid_url_to_fetch, $html, 'cache', $timenow + $bgcachexpire);
  watchdog('Brilliant Gal', 'Fetched XML from '.$url_to_fetch.'. Elapsed time: '.(time()-$timenow).' seconds.');
  /*
  global $user; $userId = $user->uid;
  if ($userId == 1){
    drupal_set_message('again fetched '.$url_to_fetch);
  }
  */
}

# if ($http_result->code == 200) {
#$headers    = $hc->getHeaders();
#$header        = $hc->getHeader();
#$inf        = $hc->getInfo();
// Links to full images are in this format: <enclosure type='image/jpeg' url='http://lh4.ggpht.com/_HkgjhlSfbZA/SWOgKhb6LyI/AAAAAAAABZ4/5AEu8AF01sw/DSC03173.JPG' length='0'/>
$imgchunks = explode("' url='",$html);
$fetchingnow = 0;
$timenow = time();
foreach($imgchunks as $imgchunk) {

  // Make sure the script execution does not expire in case many images need to be fetched from remote server.
  set_time_limit(360);

  // Get the file name
  $chunk2 = explode("' length='",$imgchunk);
  $imgurl = trim($chunk2[0]);
  if (substr($imgurl,0,4) <> 'http') {
    continue;
  }
  $imagename = substr($imgurl,strrpos($imgurl,'/')+1);
  #echo $imagename . '<br>';
  $foqen = $mkdirek . '/' . $imagename;

  $lastchanged = @filemtime($foqen); // Last file modification time, or FALSE on error.
  /*
  global $user; $userId = $user->uid;
  if ($userId == 1){
    drupal_set_message('pic '.($timenow - $lastchanged).' > '.$bgcachexpire.' ... '.$foqen);
  }
  */
  #if ($_GET['fetchnow']==1){ $lastchanged = FALSE; } // Debugging!
  if ( $lastchanged === FALSE OR ($timenow - $lastchanged > $bgcachexpire) ){
    // If the image is expired, we need to actively delete it, for the case that it was removed / hidden by the owner.
    @unlink($foqen);
    /*
    global $user; $userId = $user->uid;
    if ($userId == 1){
      drupal_set_message('in!');
    }
    */
    ///*
    $imager = drupal_http_request($imgurl);
    $image = $imager->data;
    //*/
    #watchdog('bg', $imager->code.' ... '.$imgurl.' =imgurl');
    // It happens that the file size is 0 (image not fetched). In such case, don't write it.
    if (strlen($image)>0){
      $fp = fopen($foqen, 'w');
      #drupal_set_message($foqen.'<br>');
      #watchdog('bg', $foqen.' ...ok<br>');
      fwrite($fp, $image);
      fclose($fp);
      $fetchingnow ++;
    }
  }

}

if ($fetchingnow <> 0){
  watchdog('Brilliant Gal', 'Fetched '.$fetchingnow.' images from '.$url_to_fetch.'. Elapsed time: '.(time()-$timenow).' seconds.');
}
