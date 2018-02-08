<?php

require_once 'customization.civix.php';

/**
 * Implementation of hook_civicrm_config
 *
 * @link http://wiki.civicrm.org/confluence/display/CRMDOC/hook_civicrm_config
 */
function customization_civicrm_config(&$config) {
  _customization_civix_civicrm_config($config);
}

/**
 * Implementation of hook_civicrm_xmlMenu
 *
 * @param $files array(string)
 *
 * @link http://wiki.civicrm.org/confluence/display/CRMDOC/hook_civicrm_xmlMenu
 */
function customization_civicrm_xmlMenu(&$files) {
  _customization_civix_civicrm_xmlMenu($files);
}

/**
 * Implementation of hook_civicrm_install
 *
 * @link http://wiki.civicrm.org/confluence/display/CRMDOC/hook_civicrm_install
 */
function customization_civicrm_install() {
  _customization_civix_civicrm_install();
}

/**
 * Implementation of hook_civicrm_uninstall
 *
 * @link http://wiki.civicrm.org/confluence/display/CRMDOC/hook_civicrm_uninstall
 */
function customization_civicrm_uninstall() {
  _customization_civix_civicrm_uninstall();
}

/**
 * Implementation of hook_civicrm_enable
 *
 * @link http://wiki.civicrm.org/confluence/display/CRMDOC/hook_civicrm_enable
 */
function customization_civicrm_enable() {
  _customization_civix_civicrm_enable();
}

/**
 * Implementation of hook_civicrm_disable
 *
 * @link http://wiki.civicrm.org/confluence/display/CRMDOC/hook_civicrm_disable
 */
function customization_civicrm_disable() {
  _customization_civix_civicrm_disable();
}

/**
 * Implementation of hook_civicrm_upgrade
 *
 * @param $op string, the type of operation being performed; 'check' or 'enqueue'
 * @param $queue CRM_Queue_Queue, (for 'enqueue') the modifiable list of pending up upgrade tasks
 *
 * @return mixed  based on op. for 'check', returns array(boolean) (TRUE if upgrades are pending)
 *                for 'enqueue', returns void
 *
 * @link http://wiki.civicrm.org/confluence/display/CRMDOC/hook_civicrm_upgrade
 */
function customization_civicrm_upgrade($op, CRM_Queue_Queue $queue = NULL) {
  return _customization_civix_civicrm_upgrade($op, $queue);
}

/**
 * Implementation of hook_civicrm_managed
 *
 * Generate a list of entities to create/deactivate/delete when this module
 * is installed, disabled, uninstalled.
 *
 * @link http://wiki.civicrm.org/confluence/display/CRMDOC/hook_civicrm_managed
 */
function customization_civicrm_managed(&$entities) {
  _customization_civix_civicrm_managed($entities);
}

/**
 * Implementation of hook_civicrm_caseTypes
 *
 * Generate a list of case-types
 *
 * Note: This hook only runs in CiviCRM 4.4+.
 *
 * @link http://wiki.civicrm.org/confluence/display/CRMDOC/hook_civicrm_caseTypes
 */
function customization_civicrm_caseTypes(&$caseTypes) {
  _customization_civix_civicrm_caseTypes($caseTypes);
}

/**
 * Implementation of hook_civicrm_alterSettingsFolders
 *
 * @link http://wiki.civicrm.org/confluence/display/CRMDOC/hook_civicrm_alterSettingsFolders
 */
function customization_civicrm_alterSettingsFolders(&$metaDataFolders = NULL) {
  _customization_civix_civicrm_alterSettingsFolders($metaDataFolders);
}

/**
* implementation of CiviCRM hook
*/
function customization_civicrm_tokens(&$tokens) {
  $tokenFunctions = customization_initialize();
  $customizations = array();
  foreach ($tokenFunctions as $token) {
    $fn = $token . '_customization_declare';
    $tokens[$token] = array_merge($customizations, $fn($token));
  }
  $tokens['customizations']= $customizations;
}

/**
 * implementation of CiviCRM hook
 *
 * @param array $values
 * @param $contactIDs
 * @param null $job
 * @param array $tokens
 * @param null $context
 */
function customization_civicrm_tokenValues(&$values, $contactIDs, $job = null, $tokens = array(), $context = null) {
  $tokenFunctions = customization_initialize();
  // @todo figure out full conditions for returning here.
  if (empty($tokens) || array_keys($tokens) == array('contact')) {
    return;
  }

  foreach ($tokenFunctions as $token) {
    if (in_array($token, array_keys($tokens))) {
      $fn = $token . '_customization_get';
      foreach ($contactIDs as $contactID) {
        $value =& $values[$contactID];
        $fn($contactID, $value, $context);
      }
    }
  }
}
/**
* Gather functions from tokens in tokens folder
*/
function customization_initialize() {
  static $customization_init = null;
  static $tokens = array();
  if ($customization_init){
    return $tokens;
  }
  static $tokenFiles = null;
  $config = CRM_Core_Config::singleton();
  if(!is_array($tokenFiles)){
    $directories = array( __DIR__  . '/tokens');
    if (!empty($config->customPHPPathDir)) {
      if (file_exists($config->customPHPPathDir . '/tokens')) {
        $directories[] = $config->customPHPPathDir . '/tokens';
      }
    }
    foreach ($directories as $directory) {
      $tokenFiles = _customization_civix_find_files($directory, '*.inc');
      foreach ($tokenFiles as $file) {
        require_once $file;
        $re = "/.*\\/([a-z]*).inc/";
        preg_match($re, $file, $matches);
        $tokens[] = $matches[1];
      }
    }
  }
  $customization_init = 1;
  return $tokens;
}

