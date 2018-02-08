<?php
/**
 * Created by PhpStorm.
 * User: eileen
 * Last update: Detlev
 * Date: 03.04.2017
 * Time: 13:58
 */

/**
 * @param $token
 * @return array
 */
function date_customization_declare($token){
  return array(
    $token. '.today_format_full' => 'Today\'s Date',
    $token. '.today_format_raw' => 'Today\'s Date (raw)',
  );
}



/**
 * @param $cid
 * @param $value
 */
function date_customization_get($cid, &$value){
  $value['date.today_format_full'] = CRM_Utils_Date::customFormat(date('Ymd'));
  $value['date.today_format_raw'] = date('Ymd');
}
