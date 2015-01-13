<?php

/**
 * @file
 * Class that will handle the Intuit Payment.
 */
require_once 'Veritrans/ApiRequestor.php';
require_once 'Veritrans/VtWeb.php';

// Sanitization.
require_once 'Veritrans/Sanitizer.php';

class UbercartVeritrans_Config {

  public static $serverKey;
  public static $apiVersion = 2;
  public static $isProduction = FALSE;
  public static $is3ds = FALSE;
  public static $isSanitized = FALSE;

  const SANDBOX_BASE_URL = 'https://api.sandbox.veritrans.co.id/v2';
  const PRODUCTION_BASE_URL = 'https://api.veritrans.co.id/v2';

  public static function getBaseUrl() {
    return Veritrans_Config::$isProduction ?
        Veritrans_Config::PRODUCTION_BASE_URL : Veritrans_Config::SANDBOX_BASE_URL;
  }

}

class UbercartVeritrans_Notification {

  private $response;

  public function __construct() {
    $this->response = json_decode(file_get_contents('php://input'), TRUE);
  }

  public function __get($name) {
    if (array_key_exists($name, $this->response)) {
      return $this->response[$name];
    }
  }

}

class UbercartVeritrans_Transaction {

  public static function status($id) {
    return CommerceVeritrans_ApiRequestor::get(
            Veritrans_Config::getBaseUrl() . '/' . $id . '/status', Veritrans_Config::$serverKey, FALSE);
  }

  public static function approve($id) {
    return CommerceVeritrans_ApiRequestor::post(
            Veritrans_Config::getBaseUrl() . '/' . $id . '/approve', Veritrans_Config::$serverKey, FALSE)->status_code;
  }

  public static function cancel($id) {
    return CommerceVeritrans_ApiRequestor::post(
            Veritrans_Config::getBaseUrl() . '/' . $id . '/cancel', Veritrans_Config::$serverKey, FALSE)->status_code;
  }

}
