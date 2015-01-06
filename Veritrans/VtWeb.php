<?php

/**
 * @file
 * Contains processing of VtWeb redirection Url.
 */

/**
 * Class for VtWeb Url processing.
 */
class Ubercart_Veritrans_VtWeb {

  /**
   * Function for VtWeb Url processing.
   */
  public static function getRedirectionUrl($params) {
    $payloads = array(
        'payment_type' => 'vtweb',
        'vtweb' => array(
           'enabled_payments' => array('credit_card'),
          'credit_card_3d_secure' => Veritrans_Config::$is3ds,
        ),
      );

    if (array_key_exists('item_details', $params)) {
      $gross_amount = 0;
      foreach ($params['item_details'] as $item) {
        $gross_amount += $item['quantity'] * $item['price'];
      }
      $payloads['transaction_details']['gross_amount'] = $gross_amount;
    }

    $payloads = array_replace_recursive($payloads, $params);

    if (Veritrans_Config::$isSanitized) {
      CommerceVeritrans_Sanitizer::jsonRequest($payloads);
    }

    $result = CommerceVeritrans_ApiRequestor::post(
        Veritrans_Config::getBaseUrl() . '/charge',
        Veritrans_Config::$serverKey,
        $payloads);

    return $result->redirect_url;
  }
}
