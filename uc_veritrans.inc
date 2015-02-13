<?php

/**
 * @file
 * Defines the veritrans pages.
 */

/**
 * Function to complete the transaction and sending request to Veritrans.
 */
function uc_veritrans_vtweb_payment_complete($order) {

  // Return an error if the enabling action's settings haven't been configured.
  $server_key = variable_get('uc_veritrans_server_key', '');
  if (empty($server_key)) {
    drupal_set_message(t('Veritrans is not configured for use. Please check the Veritrans Server Key.'), 'error');
    return FALSE;
  }
  libraries_load('veritrans');
  Veritrans_Config::$serverKey = variable_get('uc_veritrans_server_key', '');
  $payment_mode = variable_get('uc_veritrans_payment_mode', 'TEST');
  if ($payment_mode == 'TEST') {
    Veritrans_Config::$isProduction = FALSE;
  }
  else {
    Veritrans_Config::$isProduction = TRUE;
  }

  $items_details = array();
  foreach ($order->products as $item_id => $line_item) {
    $item = array();
    $item['id'] = $item_id;
    $item['quantity'] = $line_item->qty;
    $item['price'] = $line_item->price;
    $item['name'] = $line_item->title;
    $items_details[] = $item;
  }
  $finish_redirect_url = variable_get('uc_veritrans_finish_redirect_url', 'vt_web/success');
  $unfinish_redirect_url = variable_get('uc_veritrans_unfinish_redirect_url', 'vt_web/unfinish');
  $error_redirect_url = variable_get('uc_veritrans_error_redirect_url', 'vt_web/error');
  $params = array(
    "vtweb" => array(
      'enabled_payments' => array('credit_card'),
      'finish_redirect_url' => url($finish_redirect_url, array('absolute' => TRUE)),
      'unfinish_redirect_url' => url($unfinish_redirect_url, array('absolute' => TRUE)),
      'error_redirect_url' => url($error_redirect_url, array('absolute' => TRUE)),
    ),
    'transaction_details' => array(
      'order_id' => $order->order_id,
      'gross_amount' => $order->order_total,
    ),
    'item_details' => $items_details,
    'customer_details' => array(
      'first_name' => $order->delivery_first_name,
      'last_name' => $order->delivery_last_name,
      'email' => $order->primary_email,
      'phone' => $order->delivery_phone,
      'billing_address' => array(
        'first_name' => $order->billing_first_name,
        'last_name' => $order->billing_last_name,
        'address' => $order->billing_street1 . ' ' . $order->billing_street2,
        'city' => $order->billing_city,
        'postal_code' => $order->billing_postal_code,
        'country_code' => $order->billing_country,
        'phone' => $order->billing_phone,
      ),
    ),
  );

  try {
    // Redirect to Veritrans VTWeb page.
    $path = Veritrans_VtWeb::getRedirectionUrl($params);
    drupal_goto($path);
  }
  catch (Exception $e) {
    drupal_set_message($e->getMessage(), 'error');
    return t('error');
  }
}

/**
 * Success callback.
 */
function uc_veritrans_vt_web_payment_success() {
  return t('This is success page.');
}

/**
 * Unfinish callback.
 */
function uc_veritrans_vt_web_payment_unfinish() {
  return t('This is unfinish page.');
}

/**
 * Error callback.
 */
function uc_veritrans_vt_web_payment_error() {
  return t('This is error page.');
}
