<?php
/**
 * Created by Detlev Sieber
 * Last update: Detlev
 * Date: 11.12.2017
 * Time: 13:46
 */

/**
 * First, the available tokens are defined:
 * @param $token
 * @return array
 */
function contributionsum_customization_declare($token){
  return array(
    $token. '.fw_name' => 'Name Freiwillige/r aus UK-Spende',
    $token. '.fw_sum' => 'Summe UK-Spende für akt. Freiwillige/n',
    $token. '.cd_sum_current_yr' => 'Summe Spenden für CD (aktuelles Jahr)',
    $token. '.cd_sum_last_yr' => 'Summe Spenden für CD (letztes Jahr)',
    $token. '.uk_lumpsums' => 'Summe UK-Abschläge',
    $token. '.uk_sum_softcredit' => 'Summe UK-Abschläge und UK-Spenden für Freiw.',
  );
}

/**
 * Now, the tokens are filled with their values:
 * @param $cid
 * @param $value
 */
function contributionsum_customization_get($cid, &$value){


/* fw_sum:
 * Get all contributions of type "UK-Spende" within the current year, the last year
 * and the year before. Then, all values in $uk_spende are summed up.
 * The values of $uk_spende will be used later to determine the token value for fw_name
*/
    $uk_spende = civicrm_api3('Contribution', 'get', array(
      'sequential' => 1,
      'return' => array("id", "soft_credit_to", "total_amount"),
      'contact_id' => $cid,
      'financial_type_id' => "UK-Spende",
      'receive_date' => array('BETWEEN' => 
         array(date(Y,strtotime("-24 months"))."01012359",
               date(Y)."12312359")
               ),
      'options' => array('sort' => "id DESC"),
    ));
    $value['contributionsum.fw_sum'] = array_sum(array_column($uk_spende['values'], 'total_amount'));


/* fw_name:
 * We look only for the latest entry of $uk_spende: If the same donator would support
 * more than one volunteer, only the name of the volunteer mentioned in the latest
 * contribution will be put in the token value.
 *
 * There are two cases: 
 * - People might donate via a "UK Ansprechperson"
 * - or people donate directly to CD
 * In any of these cases, at first we look up the relationship "UK-Ansprechperson":
*/

    $fw_id = civicrm_api3('Relationship', 'get', array(
      'sequential' => 1,
      'return' => array('id', 'contact_id_b'),
      'contact_id_a' => $uk_spende['values'][0]['soft_credit_to'],
      'relationship_type_id' => 17,
      'options' => array('sort' => "id DESC"),
    ));

/* Step one, we are checking, if the soft_credit goes directly to a volunteer:
 * This happens, when there is no relationship of type 17, where the soft credited person
 * has contact_id_a. In this case, fw_name is the name belonging to the soft credit:
*/
if (empty($fw_id['values'][0])){
        $fw_name = civicrm_api3('Contact', 'get', array(
          'sequential' => 1,
          'return' => array("display_name"),
          'id' => $uk_spende['values'][0]['soft_credit_to'],
        ));
        $value['contributionsum.fw_name'] = $fw_name['values'][0]['display_name']; 
        }

/* ELSE, UK-Spenden are paid via a UK-Ansprechperson:
*/
      else {
// For some reason, we have to test if values are positive - otherwise 
// testing on 4.7.27 experienced that the field was filled with random/wrong values.
        if ($uk_spende['values'][0]['soft_credit_to'] > 0) {
          $fw_name = civicrm_api3('Contact', 'get', array(
            'sequential' => 1,
            'return' => array("display_name"),
            'id' => $fw_id['values'][0]['contact_id_b'],
          ));
        $value['contributionsum.fw_name'] = $fw_name['values'][0]['display_name']; 
        }
      }



/* cd_sum_current_yr
 * Sum of all contributions of financial type = Spende within current year
*/

    $Spende_sum = civicrm_api3('Contribution', 'get', array(
      'sequential' => 1,
      'return' => array("total_amount"),
      'contact_id' => $cid,
      'financial_type_id' => "Spende",
      'receive_date' => array('BETWEEN' => 
         array(date(Y)."01010000", 
               date(Y)."12312359")
               ),
    ));
    $value['contributionsum.cd_sum_current_yr'] = array_sum(array_column($Spende_sum['values'], 'total_amount'));

/* cd_sum_last_yr
 * Sum of all contributions of financial type = Spende within the year before the current year
*/

    $Spende_sum = civicrm_api3('Contribution', 'get', array(
      'sequential' => 1,
      'return' => array("total_amount"),
      'contact_id' => $cid,
      'financial_type_id' => "Spende",
      'receive_date' => array('BETWEEN' => 
         array(date(Y,strtotime("-12 months"))."01010000",
                 date(Y,strtotime("-12 months"))."12312359")
               ),
    ));
    $value['contributionsum.cd_sum_last_yr'] = array_sum(array_column($Spende_sum['values'], 'total_amount'));


/* uk_lumpsums
 * Sum of all contributions of financial type = UK-Abschlag 
 * Only contributions of positive amount!
 * receive date: last year and current year
*/

    $lumpsum = civicrm_api3('Contribution', 'get', array(
      'sequential' => 1,
      'return' => array("total_amount"),
      'contact_id' => $cid,
      'financial_type_id' => "UK-Abschlag",
      'total_amount' => array('>' => 0),
      'receive_date' => array('BETWEEN' => 
         array(date(Y,strtotime("-12 months"))."01010000",
               date(Y)."12312359")
               ),
    ));
    $value['contributionsum.uk_lumpsums'] = array_sum(array_column($lumpsum['values'], 'total_amount'));


/* uk_sum_softcredit
 * Sum of all soft credits of financial type UK-Abschlag or UK-Spende
 * Code is created with the API Explorer using the join option
*/ 

    $uk_sum = civicrm_api3('ContributionSoft', 'get', array(
      'sequential' => 1,
      'return' => array("amount"),
      'contact_id' => $cid,
      'contribution_id.financial_type_id' => array('IN' => array("UK-Abschlag", "UK-Spende")),
      'contribution_id.contribution_status_id' => "Completed",
      'receive_date' => array('BETWEEN' => 
         array(date(Y,strtotime("-24 months"))."01010000",
               date(Y)."12312359")
               ),
    ));
    $value['contributionsum.uk_sum_softcredit'] = array_sum(array_column($uk_sum['values'], 'amount'));


}
