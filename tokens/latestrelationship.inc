<?php
/**
 * Created by Detlev Sieber
 * Last update: Detlev
 * Date: 19.6.2017
 * Time: 17:27
 */

/**
 * First, the available tokens are defined:
 * @param $token
 * @return array
 */
function latestrelationship_customization_declare($token){
  return array(
    $token. '.related_organization' => 'FW-Dienst bei Einsatzstelle',
    $token. '.related_streetaddress' => 'FW-Dienst/Adresse Einsatzstelle',
    $token. '.related_postalcode' => 'FW-Dienst/PLZ Einsatzstelle',
    $token. '.related_city' => 'FW-Dienst/Ort Einsatzstelle',
    $token. '.related_country' => 'FW-Dienst/Land Einsatzstelle',
    $token. '.related_nr' => 'FW-Dienst/Einsatzstelle Nr.',
    $token. '.related_taetigkeit_kurz_dt' => 'FW-Dienst/Tätigkeit (kurz, Deutsch)',
    $token. '.related_taetigkeit_kurz_einsatzspr' => 'FW-Dienst/Tätigkeit (kurz, Einsatzsprache)',
    $token. '.related_taetigkeit_w' => 'FW-Dienst/Tätigkeit (ausführlich/weiblich)',
    $token. '.related_taetigkeit_m' => 'FW-Dienst/Tätigkeit (ausführlich/männlich)',
    $token. '.start_date' => 'Beziehung Beginn',
    $token. '.end_date' => 'Beziehung Ende',
    $token. '.custom_21' => 'Programm',
    $token. '.custom_22' => 'Tätigkeit',
    $token. '.custom_23' => 'Dauer',
    $token. '.related_po_organization' => 'FW-Dienst - Partnerorganisation',
    $token. '.related_po_streetaddress' => 'FW-Dienst/Adresse Partnerorganisation',
    $token. '.related_po_postalcode' => 'FW-Dienst/PLZ Partnerorganisation',
    $token. '.related_po_city' => 'FW-Dienst/Ort Partnerorganisation',
    $token. '.related_po_country' => 'FW-Dienst/Land Partnerorganisation',
    $token. '.related_po_custom_3' => 'Arbeitszeit',
    $token. '.related_po_custom_2' => 'Taschengeld',
    $token. '.related_po_custom_4' => 'Urlaub',
    $token. '.gemeindereferenz' => 'Gemeindereferenz',

  );
}



/**
 * Now, the tokens are filled with their values:
 * @param $cid
 * @param $value
 */
function latestrelationship_customization_get($cid, &$value){

/* Get fields for the relationship
/* Sort descending, because latest relationship should be the first one
/* Look up only relationship_type_id "Freiwillige/r" (id=6)
*/
    $result = civicrm_api3('Relationship', 'get', array(
      'sequential' => 1,
      'return' => array("contact_id_b", "start_date", "end_date", 
              "custom_21", "custom_22", "custom_23"),
      'contact_id_a' => $cid,
      'relationship_type_id' => 6,
      'options' => array('sort' => "start_date DESC"),
    ));

    $contact_id_b = $result['values'][0]['contact_id_b'];

/* Look up the corresponding partner organisation
 */
    $result_po = civicrm_api3('Relationship', 'get', array(
      'sequential' => 1,
      'return' => array("contact_id_b"),
      'contact_id_a' => $contact_id_b,
      'relationship_type_id' => 11,
      'options' => array('sort' => "start_date DESC"),
    ));

    $contact_id_po = $result_po['values'][0]['contact_id_b'];


/* Get fields for the related contact 
*/
    $related_result = civicrm_api3('Contact', 'get', array(
      'sequential' => 1,
      'return' => array("legal_name", "street_address", "postal_code", "city", "country", 
                    "custom_9", "custom_32","custom_33","custom_34","custom_35"),
      'id' => $contact_id_b,
    )); 

    $related_po_result = civicrm_api3('Contact', 'get', array(
      'sequential' => 1,
      'return' => array("legal_name", "street_address", "postal_code", "city", "country", 
                        "custom_3", "custom_2", "custom_4"),
      'id' => $contact_id_po,
    )); 

/* Get für Gemeindereferenz die aktuellsten Freiwilligen 
/* Sort descending, because latest relationship should be the first one
/* Look up only relationship_type_id "Gemeindereferenz" (id=16)
*/
    $result_gemeindereferenz = civicrm_api3('Relationship', 'get', array(
      'sequential' => 1,
      'return' => array("contact_id_b"),
      'contact_id_a' => $cid,
      'relationship_type_id' => 16,
      'options' => array('sort' => "id DESC"),
    ));

    $contact_id_fw = $result_gemeindereferenz['values'][0]['contact_id_b'];

    $related_fw_referenz = civicrm_api3('Contact', 'get', array(
      'sequential' => 1,
      'return' => array("display_name", "sort_name"),
      'id' => $contact_id_fw,
    )); 




/* Populate tokens 
*/

    $value['latestrelationship.contact_id_b'] = $contact_id_b;
    $value['latestrelationship.start_date'] = CRM_Utils_Date::customFormat($result['values'][0]['start_date']); 
    $value['latestrelationship.end_date'] = CRM_Utils_Date::customFormat($result['values'][0]['end_date']); 
    $value['latestrelationship.custom_21'] = $result['values'][0]['custom_21']; 
    $value['latestrelationship.custom_22'] = $result['values'][0]['custom_22']; 
    $value['latestrelationship.custom_23'] = $result['values'][0]['custom_23']; 

    $value['latestrelationship.related_organization'] = $related_result['values'][0]['legal_name']; 
    $value['latestrelationship.related_streetaddress'] = ts($related_result['values'][0]['street_address']);
    $value['latestrelationship.related_postalcode'] = ts($related_result['values'][0]['postal_code']);
    $value['latestrelationship.related_city'] = ts($related_result['values'][0]['city']);
    $value['latestrelationship.related_country'] = ts($related_result['values'][0]['country']);
    $value['latestrelationship.related_nr'] = ts($related_result['values'][0]['custom_9']);
    $value['latestrelationship.related_taetigkeit_kurz_dt'] = ts($related_result['values'][0]['custom_32']);
    $value['latestrelationship.related_taetigkeit_kurz_einsatzspr'] = ts($related_result['values'][0]['custom_33']);
    $value['latestrelationship.related_taetigkeit_w'] = ts($related_result['values'][0]['custom_34']);
    $value['latestrelationship.related_taetigkeit_m'] = ts($related_result['values'][0]['custom_35']);

    $value['latestrelationship.related_po_organization'] = $related_po_result['values'][0]['legal_name']; 
    $value['latestrelationship.related_po_streetaddress'] = ts($related_po_result['values'][0]['street_address']);
    $value['latestrelationship.related_po_postalcode'] = ts($related_po_result['values'][0]['postal_code']);
    $value['latestrelationship.related_po_city'] = ts($related_po_result['values'][0]['city']);
    $value['latestrelationship.related_po_country'] = ts($related_po_result['values'][0]['country']);
    $value['latestrelationship.related_po_custom_3'] = $related_po_result['values'][0]['custom_3']; 
    $value['latestrelationship.related_po_custom_2'] = $related_po_result['values'][0]['custom_2']; 
    $value['latestrelationship.related_po_custom_4'] = $related_po_result['values'][0]['custom_4']; 

    $value['latestrelationship.gemeindereferenz'] = $related_fw_referenz['values'][0]['display_name']; 

}
