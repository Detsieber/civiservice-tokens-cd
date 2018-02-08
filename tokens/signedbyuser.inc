<?php
/**
 * Created by Detlev Sieber
 * Last update: Detlev
 * Date: 16.6.2017
 * Time: 15:40
 */

/**
 * First, the available tokens are defined:
 * @param $token
 * @return array
 */
function signedbyuser_customization_declare($token){
  return array(
    $token. '.name' => 'Name des Unterzeichnenden',
    $token. '.function' => 'Funktion des Unterzeichnenden',
    $token. '.profession' => 'Berufsbezeichnung des Unterzeichnenden',


  );
}


/**
 * Now, the tokens are filled with their values:
 * @param $cid
 * @param $value
 */
function signedbyuser_customization_get($cid, &$value){

/* Get contact_id of logged-in user 
 */
    $currentuser = CRM_Core_Session::singleton()->getLoggedInContactID();


/*  Get name and job_title of logged-in user via API
 */ 
    $result = civicrm_api3('Contact', 'get', array(
      'sequential' => 1,
      'return' => "display_name,job_title, nick_name",
      'id' => $currentuser,
    ));

    $display_name = $result['values'][0]['display_name'];
    $job_title = $result['values'][0]['job_title'];
    $nick_name = $result['values'][0]['nick_name'];


/*  Fill in values to tokens
 */
    $value['signedbyuser.name'] = $display_name;
    $value['signedbyuser.function'] = $job_title;
    $value['signedbyuser.profession'] = $nick_name;

}
