<?php
/**
 * @file
 * grievance_roles_and_permissions.features.user_permission.inc
 */

/**
 * Implements hook_user_default_permissions().
 */
function grievance_roles_and_permissions_user_default_permissions() {
  $permissions = array();

  // Exported permission: 'access administration menu'.
  $permissions['access administration menu'] = array(
    'name' => 'access administration menu',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'admin_menu',
  );

  // Exported permission: 'access administration pages'.
  $permissions['access administration pages'] = array(
    'name' => 'access administration pages',
    'roles' => array(
      'administrator' => 'administrator',
      'grievance admin' => 'grievance admin',
      'grievance user' => 'grievance user',
    ),
    'module' => 'system',
  );

  // Exported permission: 'access all views'.
  $permissions['access all views'] = array(
    'name' => 'access all views',
    'roles' => array(
      'administrator' => 'administrator',
      'grievance admin' => 'grievance admin',
    ),
    'module' => 'views',
  );

  // Exported permission: 'access content'.
  $permissions['access content'] = array(
    'name' => 'access content',
    'roles' => array(
      'administrator' => 'administrator',
      'grievance admin' => 'grievance admin',
      'grievance user' => 'grievance user',
    ),
    'module' => 'node',
  );

  // Exported permission: 'access content overview'.
  $permissions['access content overview'] = array(
    'name' => 'access content overview',
    'roles' => array(
      'administrator' => 'administrator',
      'grievance admin' => 'grievance admin',
      'grievance user' => 'grievance user',
    ),
    'module' => 'node',
  );

  // Exported permission: 'access contextual links'.
  $permissions['access contextual links'] = array(
    'name' => 'access contextual links',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'contextual',
  );

  // Exported permission: 'access dashboard'.
  $permissions['access dashboard'] = array(
    'name' => 'access dashboard',
    'roles' => array(
      'administrator' => 'administrator',
      'grievance admin' => 'grievance admin',
      'grievance user' => 'grievance user',
    ),
    'module' => 'dashboard',
  );

  // Exported permission: 'access devel information'.
  $permissions['access devel information'] = array(
    'name' => 'access devel information',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'devel',
  );

  // Exported permission: 'access flood unblock'.
  $permissions['access flood unblock'] = array(
    'name' => 'access flood unblock',
    'roles' => array(
      'administrator' => 'administrator',
      'grievance admin' => 'grievance admin',
      'grievance staff' => 'grievance staff',
    ),
    'module' => 'flood_unblock',
  );

  // Exported permission: 'access private fields'.
  $permissions['access private fields'] = array(
    'name' => 'access private fields',
    'roles' => array(
      'administrator' => 'administrator',
      'grievance admin' => 'grievance admin',
    ),
    'module' => 'field_permissions',
  );

  // Exported permission: 'access site in maintenance mode'.
  $permissions['access site in maintenance mode'] = array(
    'name' => 'access site in maintenance mode',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'system',
  );

  // Exported permission: 'access site reports'.
  $permissions['access site reports'] = array(
    'name' => 'access site reports',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'system',
  );

  // Exported permission: 'access user profiles'.
  $permissions['access user profiles'] = array(
    'name' => 'access user profiles',
    'roles' => array(
      'administrator' => 'administrator',
      'grievance admin' => 'grievance admin',
    ),
    'module' => 'user',
  );

  // Exported permission: 'admin_classes'.
  $permissions['admin_classes'] = array(
    'name' => 'admin_classes',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'ds_ui',
  );

  // Exported permission: 'admin_display_suite'.
  $permissions['admin_display_suite'] = array(
    'name' => 'admin_display_suite',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'ds',
  );

  // Exported permission: 'admin_fields'.
  $permissions['admin_fields'] = array(
    'name' => 'admin_fields',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'ds_ui',
  );

  // Exported permission: 'admin_view_modes'.
  $permissions['admin_view_modes'] = array(
    'name' => 'admin_view_modes',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'ds_ui',
  );

  // Exported permission: 'administer @font-your-face'.
  $permissions['administer @font-your-face'] = array(
    'name' => 'administer @font-your-face',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'fontyourface',
  );

  // Exported permission: 'administer actions'.
  $permissions['administer actions'] = array(
    'name' => 'administer actions',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'system',
  );

  // Exported permission: 'administer blocks'.
  $permissions['administer blocks'] = array(
    'name' => 'administer blocks',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'block',
  );

  // Exported permission: 'administer ckeditor'.
  $permissions['administer ckeditor'] = array(
    'name' => 'administer ckeditor',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'ckeditor',
  );

  // Exported permission: 'administer content translations'.
  $permissions['administer content translations'] = array(
    'name' => 'administer content translations',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'i18n_node',
  );

  // Exported permission: 'administer content types'.
  $permissions['administer content types'] = array(
    'name' => 'administer content types',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'node',
  );

  // Exported permission: 'administer custom_help_text'.
  $permissions['administer custom_help_text'] = array(
    'name' => 'administer custom_help_text',
    'roles' => array(
      'administrator' => 'administrator',
      'grievance admin' => 'grievance admin',
    ),
    'module' => 'custom_help_text',
  );

  // Exported permission: 'administer elysia_cron'.
  $permissions['administer elysia_cron'] = array(
    'name' => 'administer elysia_cron',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'elysia_cron',
  );

  // Exported permission: 'administer features'.
  $permissions['administer features'] = array(
    'name' => 'administer features',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'features',
  );

  // Exported permission: 'administer feeds'.
  $permissions['administer feeds'] = array(
    'name' => 'administer feeds',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'feeds',
  );

  // Exported permission: 'administer field permissions'.
  $permissions['administer field permissions'] = array(
    'name' => 'administer field permissions',
    'roles' => array(
      'administrator' => 'administrator',
      'grievance admin' => 'grievance admin',
    ),
    'module' => 'field_permissions',
  );

  // Exported permission: 'administer fieldgroups'.
  $permissions['administer fieldgroups'] = array(
    'name' => 'administer fieldgroups',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'field_group',
  );

  // Exported permission: 'administer fields'.
  $permissions['administer fields'] = array(
    'name' => 'administer fields',
    'roles' => array(
      'administrator' => 'administrator',
      'grievance admin' => 'grievance admin',
    ),
    'module' => 'field',
  );

  // Exported permission: 'administer filters'.
  $permissions['administer filters'] = array(
    'name' => 'administer filters',
    'roles' => array(
      'administrator' => 'administrator',
      'grievance admin' => 'grievance admin',
    ),
    'module' => 'filter',
  );

  // Exported permission: 'administer front page'.
  $permissions['administer front page'] = array(
    'name' => 'administer front page',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'front_page',
  );

  // Exported permission: 'administer image styles'.
  $permissions['administer image styles'] = array(
    'name' => 'administer image styles',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'image',
  );

  // Exported permission: 'administer languages'.
  $permissions['administer languages'] = array(
    'name' => 'administer languages',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'locale',
  );

  // Exported permission: 'administer login destination settings'.
  $permissions['administer login destination settings'] = array(
    'name' => 'administer login destination settings',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'login_destination',
  );

  // Exported permission: 'administer mailsystem'.
  $permissions['administer mailsystem'] = array(
    'name' => 'administer mailsystem',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'mailsystem',
  );

  // Exported permission: 'administer masquerade'.
  $permissions['administer masquerade'] = array(
    'name' => 'administer masquerade',
    'roles' => array(
      'administrator' => 'administrator',
      'grievance admin' => 'grievance admin',
    ),
    'module' => 'masquerade',
  );

  // Exported permission: 'administer menu'.
  $permissions['administer menu'] = array(
    'name' => 'administer menu',
    'roles' => array(
      'administrator' => 'administrator',
      'grievance admin' => 'grievance admin',
    ),
    'module' => 'menu',
  );

  // Exported permission: 'administer modules'.
  $permissions['administer modules'] = array(
    'name' => 'administer modules',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'system',
  );

  // Exported permission: 'administer nodes'.
  $permissions['administer nodes'] = array(
    'name' => 'administer nodes',
    'roles' => array(
      'administrator' => 'administrator',
      'grievance admin' => 'grievance admin',
    ),
    'module' => 'node',
  );

  // Exported permission: 'administer permissions'.
  $permissions['administer permissions'] = array(
    'name' => 'administer permissions',
    'roles' => array(
      'administrator' => 'administrator',
      'grievance admin' => 'grievance admin',
    ),
    'module' => 'user',
  );

  // Exported permission: 'administer popup elements'.
  $permissions['administer popup elements'] = array(
    'name' => 'administer popup elements',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'popup',
  );

  // Exported permission: 'administer responsive menus'.
  $permissions['administer responsive menus'] = array(
    'name' => 'administer responsive menus',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'responsive_menus',
  );

  // Exported permission: 'administer search'.
  $permissions['administer search'] = array(
    'name' => 'administer search',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'search',
  );

  // Exported permission: 'administer shortcuts'.
  $permissions['administer shortcuts'] = array(
    'name' => 'administer shortcuts',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'shortcut',
  );

  // Exported permission: 'administer simplify'.
  $permissions['administer simplify'] = array(
    'name' => 'administer simplify',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'simplify',
  );

  // Exported permission: 'administer site configuration'.
  $permissions['administer site configuration'] = array(
    'name' => 'administer site configuration',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'system',
  );

  // Exported permission: 'administer software updates'.
  $permissions['administer software updates'] = array(
    'name' => 'administer software updates',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'system',
  );

  // Exported permission: 'administer superfish'.
  $permissions['administer superfish'] = array(
    'name' => 'administer superfish',
    'roles' => array(
      'administrator' => 'administrator',
      'grievance admin' => 'grievance admin',
    ),
    'module' => 'superfish',
  );

  // Exported permission: 'administer tabtamer'.
  $permissions['administer tabtamer'] = array(
    'name' => 'administer tabtamer',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'tabtamer',
  );

  // Exported permission: 'administer taxonomy'.
  $permissions['administer taxonomy'] = array(
    'name' => 'administer taxonomy',
    'roles' => array(
      'administrator' => 'administrator',
      'grievance admin' => 'grievance admin',
    ),
    'module' => 'taxonomy',
  );

  // Exported permission: 'administer themes'.
  $permissions['administer themes'] = array(
    'name' => 'administer themes',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'system',
  );

  // Exported permission: 'administer url aliases'.
  $permissions['administer url aliases'] = array(
    'name' => 'administer url aliases',
    'roles' => array(
      'administrator' => 'administrator',
      'grievance admin' => 'grievance admin',
    ),
    'module' => 'path',
  );

  // Exported permission: 'administer users'.
  $permissions['administer users'] = array(
    'name' => 'administer users',
    'roles' => array(
      'administrator' => 'administrator',
      'grievance admin' => 'grievance admin',
    ),
    'module' => 'user',
  );

  // Exported permission: 'administer uuid'.
  $permissions['administer uuid'] = array(
    'name' => 'administer uuid',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'uuid',
  );

  // Exported permission: 'administer views'.
  $permissions['administer views'] = array(
    'name' => 'administer views',
    'roles' => array(
      'administrator' => 'administrator',
      'grievance admin' => 'grievance admin',
    ),
    'module' => 'views',
  );

  // Exported permission: 'administer views calc'.
  $permissions['administer views calc'] = array(
    'name' => 'administer views calc',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'views_calc',
  );

  // Exported permission: 'block IP addresses'.
  $permissions['block IP addresses'] = array(
    'name' => 'block IP addresses',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'system',
  );

  // Exported permission: 'bypass node access'.
  $permissions['bypass node access'] = array(
    'name' => 'bypass node access',
    'roles' => array(
      'administrator' => 'administrator',
      'grievance admin' => 'grievance admin',
    ),
    'module' => 'node',
  );

  // Exported permission: 'cancel account'.
  $permissions['cancel account'] = array(
    'name' => 'cancel account',
    'roles' => array(
      'administrator' => 'administrator',
      'grievance admin' => 'grievance admin',
    ),
    'module' => 'user',
  );

  // Exported permission: 'change own username'.
  $permissions['change own username'] = array(
    'name' => 'change own username',
    'roles' => array(
      'administrator' => 'administrator',
      'grievance admin' => 'grievance admin',
    ),
    'module' => 'user',
  );

  // Exported permission: 'clear grievance_shops_import feeds'.
  $permissions['clear grievance_shops_import feeds'] = array(
    'name' => 'clear grievance_shops_import feeds',
    'roles' => array(),
    'module' => 'feeds',
  );

  // Exported permission: 'create field_grievance_rep_lead'.
  $permissions['create field_grievance_rep_lead'] = array(
    'name' => 'create field_grievance_rep_lead',
    'roles' => array(
      'administrator' => 'administrator',
      'grievance admin' => 'grievance admin',
    ),
    'module' => 'field_permissions',
  );

  // Exported permission: 'create field_grievance_rep_organizer'.
  $permissions['create field_grievance_rep_organizer'] = array(
    'name' => 'create field_grievance_rep_organizer',
    'roles' => array(
      'administrator' => 'administrator',
      'grievance admin' => 'grievance admin',
    ),
    'module' => 'field_permissions',
  );

  // Exported permission: 'create field_grievance_shops'.
  $permissions['create field_grievance_shops'] = array(
    'name' => 'create field_grievance_shops',
    'roles' => array(
      'administrator' => 'administrator',
      'grievance admin' => 'grievance admin',
    ),
    'module' => 'field_permissions',
  );

  // Exported permission: 'create grievance content'.
  $permissions['create grievance content'] = array(
    'name' => 'create grievance content',
    'roles' => array(
      'grievance staff' => 'grievance staff',
      'grievance user' => 'grievance user',
    ),
    'module' => 'node',
  );

  // Exported permission: 'create grievance_attachment content'.
  $permissions['create grievance_attachment content'] = array(
    'name' => 'create grievance_attachment content',
    'roles' => array(
      'grievance staff' => 'grievance staff',
      'grievance user' => 'grievance user',
    ),
    'module' => 'node',
  );

  // Exported permission: 'create grievance_basic_page content'.
  $permissions['create grievance_basic_page content'] = array(
    'name' => 'create grievance_basic_page content',
    'roles' => array(
      'administrator' => 'administrator',
      'grievance admin' => 'grievance admin',
    ),
    'module' => 'node',
  );

  // Exported permission: 'create grievance_company content'.
  $permissions['create grievance_company content'] = array(
    'name' => 'create grievance_company content',
    'roles' => array(),
    'module' => 'node',
  );

  // Exported permission: 'create grievance_contract content'.
  $permissions['create grievance_contract content'] = array(
    'name' => 'create grievance_contract content',
    'roles' => array(),
    'module' => 'node',
  );

  // Exported permission: 'create grievance_contract_clause content'.
  $permissions['create grievance_contract_clause content'] = array(
    'name' => 'create grievance_contract_clause content',
    'roles' => array(),
    'module' => 'node',
  );

  // Exported permission: 'create grievance_contract_rule content'.
  $permissions['create grievance_contract_rule content'] = array(
    'name' => 'create grievance_contract_rule content',
    'roles' => array(),
    'module' => 'node',
  );

  // Exported permission: 'create grievance_contract_section content'.
  $permissions['create grievance_contract_section content'] = array(
    'name' => 'create grievance_contract_section content',
    'roles' => array(),
    'module' => 'node',
  );

  // Exported permission: 'create grievance_contract_template content'.
  $permissions['create grievance_contract_template content'] = array(
    'name' => 'create grievance_contract_template content',
    'roles' => array(),
    'module' => 'node',
  );

  // Exported permission: 'create grievance_field_overrides content'.
  $permissions['create grievance_field_overrides content'] = array(
    'name' => 'create grievance_field_overrides content',
    'roles' => array(),
    'module' => 'node',
  );

  // Exported permission: 'create grievance_holiday content'.
  $permissions['create grievance_holiday content'] = array(
    'name' => 'create grievance_holiday content',
    'roles' => array(),
    'module' => 'node',
  );

  // Exported permission: 'create grievance_log content'.
  $permissions['create grievance_log content'] = array(
    'name' => 'create grievance_log content',
    'roles' => array(
      'grievance staff' => 'grievance staff',
      'grievance user' => 'grievance user',
    ),
    'module' => 'node',
  );

  // Exported permission: 'create grievance_news content'.
  $permissions['create grievance_news content'] = array(
    'name' => 'create grievance_news content',
    'roles' => array(),
    'module' => 'node',
  );

  // Exported permission: 'create grievance_settlement content'.
  $permissions['create grievance_settlement content'] = array(
    'name' => 'create grievance_settlement content',
    'roles' => array(
      'grievance staff' => 'grievance staff',
    ),
    'module' => 'node',
  );

  // Exported permission: 'create grievance_shop content'.
  $permissions['create grievance_shop content'] = array(
    'name' => 'create grievance_shop content',
    'roles' => array(
      'administrator' => 'administrator',
      'grievance admin' => 'grievance admin',
    ),
    'module' => 'node',
  );

  // Exported permission: 'create grievance_shop_contact content'.
  $permissions['create grievance_shop_contact content'] = array(
    'name' => 'create grievance_shop_contact content',
    'roles' => array(
      'grievance staff' => 'grievance staff',
    ),
    'module' => 'node',
  );

  // Exported permission: 'create page content'.
  $permissions['create page content'] = array(
    'name' => 'create page content',
    'roles' => array(
      'administrator' => 'administrator',
      'grievance admin' => 'grievance admin',
    ),
    'module' => 'node',
  );

  // Exported permission: 'create url aliases'.
  $permissions['create url aliases'] = array(
    'name' => 'create url aliases',
    'roles' => array(
      'administrator' => 'administrator',
      'grievance admin' => 'grievance admin',
    ),
    'module' => 'path',
  );

  // Exported permission: 'create views calc'.
  $permissions['create views calc'] = array(
    'name' => 'create views calc',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'views_calc',
  );

  // Exported permission: 'customize ckeditor'.
  $permissions['customize ckeditor'] = array(
    'name' => 'customize ckeditor',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'ckeditor',
  );

  // Exported permission: 'customize shortcut links'.
  $permissions['customize shortcut links'] = array(
    'name' => 'customize shortcut links',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'shortcut',
  );

  // Exported permission: 'delete any grievance content'.
  $permissions['delete any grievance content'] = array(
    'name' => 'delete any grievance content',
    'roles' => array(
      'grievance staff' => 'grievance staff',
    ),
    'module' => 'node',
  );

  // Exported permission: 'delete any grievance_attachment content'.
  $permissions['delete any grievance_attachment content'] = array(
    'name' => 'delete any grievance_attachment content',
    'roles' => array(
      'grievance staff' => 'grievance staff',
    ),
    'module' => 'node',
  );

  // Exported permission: 'delete any grievance_basic_page content'.
  $permissions['delete any grievance_basic_page content'] = array(
    'name' => 'delete any grievance_basic_page content',
    'roles' => array(
      'administrator' => 'administrator',
      'grievance admin' => 'grievance admin',
    ),
    'module' => 'node',
  );

  // Exported permission: 'delete any grievance_company content'.
  $permissions['delete any grievance_company content'] = array(
    'name' => 'delete any grievance_company content',
    'roles' => array(),
    'module' => 'node',
  );

  // Exported permission: 'delete any grievance_contract content'.
  $permissions['delete any grievance_contract content'] = array(
    'name' => 'delete any grievance_contract content',
    'roles' => array(),
    'module' => 'node',
  );

  // Exported permission: 'delete any grievance_contract_clause content'.
  $permissions['delete any grievance_contract_clause content'] = array(
    'name' => 'delete any grievance_contract_clause content',
    'roles' => array(),
    'module' => 'node',
  );

  // Exported permission: 'delete any grievance_contract_rule content'.
  $permissions['delete any grievance_contract_rule content'] = array(
    'name' => 'delete any grievance_contract_rule content',
    'roles' => array(),
    'module' => 'node',
  );

  // Exported permission: 'delete any grievance_contract_section content'.
  $permissions['delete any grievance_contract_section content'] = array(
    'name' => 'delete any grievance_contract_section content',
    'roles' => array(),
    'module' => 'node',
  );

  // Exported permission: 'delete any grievance_contract_template content'.
  $permissions['delete any grievance_contract_template content'] = array(
    'name' => 'delete any grievance_contract_template content',
    'roles' => array(),
    'module' => 'node',
  );

  // Exported permission: 'delete any grievance_field_overrides content'.
  $permissions['delete any grievance_field_overrides content'] = array(
    'name' => 'delete any grievance_field_overrides content',
    'roles' => array(),
    'module' => 'node',
  );

  // Exported permission: 'delete any grievance_holiday content'.
  $permissions['delete any grievance_holiday content'] = array(
    'name' => 'delete any grievance_holiday content',
    'roles' => array(),
    'module' => 'node',
  );

  // Exported permission: 'delete any grievance_log content'.
  $permissions['delete any grievance_log content'] = array(
    'name' => 'delete any grievance_log content',
    'roles' => array(
      'grievance staff' => 'grievance staff',
    ),
    'module' => 'node',
  );

  // Exported permission: 'delete any grievance_news content'.
  $permissions['delete any grievance_news content'] = array(
    'name' => 'delete any grievance_news content',
    'roles' => array(),
    'module' => 'node',
  );

  // Exported permission: 'delete any grievance_settlement content'.
  $permissions['delete any grievance_settlement content'] = array(
    'name' => 'delete any grievance_settlement content',
    'roles' => array(
      'grievance staff' => 'grievance staff',
    ),
    'module' => 'node',
  );

  // Exported permission: 'delete any grievance_shop content'.
  $permissions['delete any grievance_shop content'] = array(
    'name' => 'delete any grievance_shop content',
    'roles' => array(
      'administrator' => 'administrator',
      'grievance admin' => 'grievance admin',
    ),
    'module' => 'node',
  );

  // Exported permission: 'delete any grievance_shop_contact content'.
  $permissions['delete any grievance_shop_contact content'] = array(
    'name' => 'delete any grievance_shop_contact content',
    'roles' => array(
      'grievance staff' => 'grievance staff',
    ),
    'module' => 'node',
  );

  // Exported permission: 'delete any page content'.
  $permissions['delete any page content'] = array(
    'name' => 'delete any page content',
    'roles' => array(
      'administrator' => 'administrator',
      'grievance admin' => 'grievance admin',
    ),
    'module' => 'node',
  );

  // Exported permission: 'delete own grievance content'.
  $permissions['delete own grievance content'] = array(
    'name' => 'delete own grievance content',
    'roles' => array(
      'grievance staff' => 'grievance staff',
    ),
    'module' => 'node',
  );

  // Exported permission: 'delete own grievance_attachment content'.
  $permissions['delete own grievance_attachment content'] = array(
    'name' => 'delete own grievance_attachment content',
    'roles' => array(
      'grievance staff' => 'grievance staff',
    ),
    'module' => 'node',
  );

  // Exported permission: 'delete own grievance_basic_page content'.
  $permissions['delete own grievance_basic_page content'] = array(
    'name' => 'delete own grievance_basic_page content',
    'roles' => array(
      'administrator' => 'administrator',
      'grievance admin' => 'grievance admin',
    ),
    'module' => 'node',
  );

  // Exported permission: 'delete own grievance_company content'.
  $permissions['delete own grievance_company content'] = array(
    'name' => 'delete own grievance_company content',
    'roles' => array(),
    'module' => 'node',
  );

  // Exported permission: 'delete own grievance_contract content'.
  $permissions['delete own grievance_contract content'] = array(
    'name' => 'delete own grievance_contract content',
    'roles' => array(),
    'module' => 'node',
  );

  // Exported permission: 'delete own grievance_contract_clause content'.
  $permissions['delete own grievance_contract_clause content'] = array(
    'name' => 'delete own grievance_contract_clause content',
    'roles' => array(),
    'module' => 'node',
  );

  // Exported permission: 'delete own grievance_contract_rule content'.
  $permissions['delete own grievance_contract_rule content'] = array(
    'name' => 'delete own grievance_contract_rule content',
    'roles' => array(),
    'module' => 'node',
  );

  // Exported permission: 'delete own grievance_contract_section content'.
  $permissions['delete own grievance_contract_section content'] = array(
    'name' => 'delete own grievance_contract_section content',
    'roles' => array(),
    'module' => 'node',
  );

  // Exported permission: 'delete own grievance_contract_template content'.
  $permissions['delete own grievance_contract_template content'] = array(
    'name' => 'delete own grievance_contract_template content',
    'roles' => array(),
    'module' => 'node',
  );

  // Exported permission: 'delete own grievance_field_overrides content'.
  $permissions['delete own grievance_field_overrides content'] = array(
    'name' => 'delete own grievance_field_overrides content',
    'roles' => array(),
    'module' => 'node',
  );

  // Exported permission: 'delete own grievance_holiday content'.
  $permissions['delete own grievance_holiday content'] = array(
    'name' => 'delete own grievance_holiday content',
    'roles' => array(),
    'module' => 'node',
  );

  // Exported permission: 'delete own grievance_log content'.
  $permissions['delete own grievance_log content'] = array(
    'name' => 'delete own grievance_log content',
    'roles' => array(
      'grievance staff' => 'grievance staff',
    ),
    'module' => 'node',
  );

  // Exported permission: 'delete own grievance_news content'.
  $permissions['delete own grievance_news content'] = array(
    'name' => 'delete own grievance_news content',
    'roles' => array(),
    'module' => 'node',
  );

  // Exported permission: 'delete own grievance_settlement content'.
  $permissions['delete own grievance_settlement content'] = array(
    'name' => 'delete own grievance_settlement content',
    'roles' => array(
      'grievance staff' => 'grievance staff',
    ),
    'module' => 'node',
  );

  // Exported permission: 'delete own grievance_shop content'.
  $permissions['delete own grievance_shop content'] = array(
    'name' => 'delete own grievance_shop content',
    'roles' => array(
      'administrator' => 'administrator',
      'grievance admin' => 'grievance admin',
    ),
    'module' => 'node',
  );

  // Exported permission: 'delete own grievance_shop_contact content'.
  $permissions['delete own grievance_shop_contact content'] = array(
    'name' => 'delete own grievance_shop_contact content',
    'roles' => array(
      'grievance staff' => 'grievance staff',
    ),
    'module' => 'node',
  );

  // Exported permission: 'delete own page content'.
  $permissions['delete own page content'] = array(
    'name' => 'delete own page content',
    'roles' => array(
      'administrator' => 'administrator',
      'grievance admin' => 'grievance admin',
    ),
    'module' => 'node',
  );

  // Exported permission: 'delete revisions'.
  $permissions['delete revisions'] = array(
    'name' => 'delete revisions',
    'roles' => array(
      'administrator' => 'administrator',
      'grievance admin' => 'grievance admin',
    ),
    'module' => 'node',
  );

  // Exported permission: 'delete terms in grievance_alert_types'.
  $permissions['delete terms in grievance_alert_types'] = array(
    'name' => 'delete terms in grievance_alert_types',
    'roles' => array(),
    'module' => 'taxonomy',
  );

  // Exported permission: 'delete terms in grievance_document_types'.
  $permissions['delete terms in grievance_document_types'] = array(
    'name' => 'delete terms in grievance_document_types',
    'roles' => array(),
    'module' => 'taxonomy',
  );

  // Exported permission: 'delete terms in grievance_log_types'.
  $permissions['delete terms in grievance_log_types'] = array(
    'name' => 'delete terms in grievance_log_types',
    'roles' => array(),
    'module' => 'taxonomy',
  );

  // Exported permission: 'delete terms in grievance_remedies'.
  $permissions['delete terms in grievance_remedies'] = array(
    'name' => 'delete terms in grievance_remedies',
    'roles' => array(),
    'module' => 'taxonomy',
  );

  // Exported permission: 'delete terms in grievance_status'.
  $permissions['delete terms in grievance_status'] = array(
    'name' => 'delete terms in grievance_status',
    'roles' => array(),
    'module' => 'taxonomy',
  );

  // Exported permission: 'delete terms in grievance_tags'.
  $permissions['delete terms in grievance_tags'] = array(
    'name' => 'delete terms in grievance_tags',
    'roles' => array(),
    'module' => 'taxonomy',
  );

  // Exported permission: 'delete terms in grievance_types'.
  $permissions['delete terms in grievance_types'] = array(
    'name' => 'delete terms in grievance_types',
    'roles' => array(),
    'module' => 'taxonomy',
  );

  // Exported permission: 'delete terms in tags'.
  $permissions['delete terms in tags'] = array(
    'name' => 'delete terms in tags',
    'roles' => array(
      'administrator' => 'administrator',
      'grievance admin' => 'grievance admin',
    ),
    'module' => 'taxonomy',
  );

  // Exported permission: 'display drupal links'.
  $permissions['display drupal links'] = array(
    'name' => 'display drupal links',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'admin_menu',
  );

  // Exported permission: 'ds_switch grievance'.
  $permissions['ds_switch grievance'] = array(
    'name' => 'ds_switch grievance',
    'roles' => array(),
    'module' => 'ds_extras',
  );

  // Exported permission: 'ds_switch grievance_attachment'.
  $permissions['ds_switch grievance_attachment'] = array(
    'name' => 'ds_switch grievance_attachment',
    'roles' => array(),
    'module' => 'ds_extras',
  );

  // Exported permission: 'ds_switch grievance_basic_page'.
  $permissions['ds_switch grievance_basic_page'] = array(
    'name' => 'ds_switch grievance_basic_page',
    'roles' => array(),
    'module' => 'ds_extras',
  );

  // Exported permission: 'ds_switch grievance_company'.
  $permissions['ds_switch grievance_company'] = array(
    'name' => 'ds_switch grievance_company',
    'roles' => array(),
    'module' => 'ds_extras',
  );

  // Exported permission: 'ds_switch grievance_contract'.
  $permissions['ds_switch grievance_contract'] = array(
    'name' => 'ds_switch grievance_contract',
    'roles' => array(),
    'module' => 'ds_extras',
  );

  // Exported permission: 'ds_switch grievance_contract_clause'.
  $permissions['ds_switch grievance_contract_clause'] = array(
    'name' => 'ds_switch grievance_contract_clause',
    'roles' => array(),
    'module' => 'ds_extras',
  );

  // Exported permission: 'ds_switch grievance_contract_rule'.
  $permissions['ds_switch grievance_contract_rule'] = array(
    'name' => 'ds_switch grievance_contract_rule',
    'roles' => array(),
    'module' => 'ds_extras',
  );

  // Exported permission: 'ds_switch grievance_contract_section'.
  $permissions['ds_switch grievance_contract_section'] = array(
    'name' => 'ds_switch grievance_contract_section',
    'roles' => array(),
    'module' => 'ds_extras',
  );

  // Exported permission: 'ds_switch grievance_contract_template'.
  $permissions['ds_switch grievance_contract_template'] = array(
    'name' => 'ds_switch grievance_contract_template',
    'roles' => array(),
    'module' => 'ds_extras',
  );

  // Exported permission: 'ds_switch grievance_field_overrides'.
  $permissions['ds_switch grievance_field_overrides'] = array(
    'name' => 'ds_switch grievance_field_overrides',
    'roles' => array(),
    'module' => 'ds_extras',
  );

  // Exported permission: 'ds_switch grievance_holiday'.
  $permissions['ds_switch grievance_holiday'] = array(
    'name' => 'ds_switch grievance_holiday',
    'roles' => array(),
    'module' => 'ds_extras',
  );

  // Exported permission: 'ds_switch grievance_log'.
  $permissions['ds_switch grievance_log'] = array(
    'name' => 'ds_switch grievance_log',
    'roles' => array(),
    'module' => 'ds_extras',
  );

  // Exported permission: 'ds_switch grievance_news'.
  $permissions['ds_switch grievance_news'] = array(
    'name' => 'ds_switch grievance_news',
    'roles' => array(),
    'module' => 'ds_extras',
  );

  // Exported permission: 'ds_switch grievance_settlement'.
  $permissions['ds_switch grievance_settlement'] = array(
    'name' => 'ds_switch grievance_settlement',
    'roles' => array(),
    'module' => 'ds_extras',
  );

  // Exported permission: 'ds_switch grievance_shop'.
  $permissions['ds_switch grievance_shop'] = array(
    'name' => 'ds_switch grievance_shop',
    'roles' => array(),
    'module' => 'ds_extras',
  );

  // Exported permission: 'ds_switch grievance_shop_contact'.
  $permissions['ds_switch grievance_shop_contact'] = array(
    'name' => 'ds_switch grievance_shop_contact',
    'roles' => array(),
    'module' => 'ds_extras',
  );

  // Exported permission: 'ds_switch page'.
  $permissions['ds_switch page'] = array(
    'name' => 'ds_switch page',
    'roles' => array(),
    'module' => 'ds_extras',
  );

  // Exported permission: 'edit any grievance content'.
  $permissions['edit any grievance content'] = array(
    'name' => 'edit any grievance content',
    'roles' => array(
      'grievance staff' => 'grievance staff',
      'grievance user' => 'grievance user',
    ),
    'module' => 'node',
  );

  // Exported permission: 'edit any grievance_attachment content'.
  $permissions['edit any grievance_attachment content'] = array(
    'name' => 'edit any grievance_attachment content',
    'roles' => array(
      'grievance staff' => 'grievance staff',
      'grievance user' => 'grievance user',
    ),
    'module' => 'node',
  );

  // Exported permission: 'edit any grievance_basic_page content'.
  $permissions['edit any grievance_basic_page content'] = array(
    'name' => 'edit any grievance_basic_page content',
    'roles' => array(
      'administrator' => 'administrator',
      'grievance admin' => 'grievance admin',
    ),
    'module' => 'node',
  );

  // Exported permission: 'edit any grievance_company content'.
  $permissions['edit any grievance_company content'] = array(
    'name' => 'edit any grievance_company content',
    'roles' => array(),
    'module' => 'node',
  );

  // Exported permission: 'edit any grievance_contract content'.
  $permissions['edit any grievance_contract content'] = array(
    'name' => 'edit any grievance_contract content',
    'roles' => array(),
    'module' => 'node',
  );

  // Exported permission: 'edit any grievance_contract_clause content'.
  $permissions['edit any grievance_contract_clause content'] = array(
    'name' => 'edit any grievance_contract_clause content',
    'roles' => array(),
    'module' => 'node',
  );

  // Exported permission: 'edit any grievance_contract_rule content'.
  $permissions['edit any grievance_contract_rule content'] = array(
    'name' => 'edit any grievance_contract_rule content',
    'roles' => array(),
    'module' => 'node',
  );

  // Exported permission: 'edit any grievance_contract_section content'.
  $permissions['edit any grievance_contract_section content'] = array(
    'name' => 'edit any grievance_contract_section content',
    'roles' => array(),
    'module' => 'node',
  );

  // Exported permission: 'edit any grievance_contract_template content'.
  $permissions['edit any grievance_contract_template content'] = array(
    'name' => 'edit any grievance_contract_template content',
    'roles' => array(),
    'module' => 'node',
  );

  // Exported permission: 'edit any grievance_field_overrides content'.
  $permissions['edit any grievance_field_overrides content'] = array(
    'name' => 'edit any grievance_field_overrides content',
    'roles' => array(),
    'module' => 'node',
  );

  // Exported permission: 'edit any grievance_holiday content'.
  $permissions['edit any grievance_holiday content'] = array(
    'name' => 'edit any grievance_holiday content',
    'roles' => array(),
    'module' => 'node',
  );

  // Exported permission: 'edit any grievance_log content'.
  $permissions['edit any grievance_log content'] = array(
    'name' => 'edit any grievance_log content',
    'roles' => array(
      'grievance staff' => 'grievance staff',
      'grievance user' => 'grievance user',
    ),
    'module' => 'node',
  );

  // Exported permission: 'edit any grievance_news content'.
  $permissions['edit any grievance_news content'] = array(
    'name' => 'edit any grievance_news content',
    'roles' => array(),
    'module' => 'node',
  );

  // Exported permission: 'edit any grievance_settlement content'.
  $permissions['edit any grievance_settlement content'] = array(
    'name' => 'edit any grievance_settlement content',
    'roles' => array(
      'grievance staff' => 'grievance staff',
    ),
    'module' => 'node',
  );

  // Exported permission: 'edit any grievance_shop content'.
  $permissions['edit any grievance_shop content'] = array(
    'name' => 'edit any grievance_shop content',
    'roles' => array(
      'administrator' => 'administrator',
      'grievance admin' => 'grievance admin',
    ),
    'module' => 'node',
  );

  // Exported permission: 'edit any grievance_shop_contact content'.
  $permissions['edit any grievance_shop_contact content'] = array(
    'name' => 'edit any grievance_shop_contact content',
    'roles' => array(
      'grievance staff' => 'grievance staff',
    ),
    'module' => 'node',
  );

  // Exported permission: 'edit any page content'.
  $permissions['edit any page content'] = array(
    'name' => 'edit any page content',
    'roles' => array(
      'administrator' => 'administrator',
      'grievance admin' => 'grievance admin',
    ),
    'module' => 'node',
  );

  // Exported permission: 'edit field_grievance_rep_lead'.
  $permissions['edit field_grievance_rep_lead'] = array(
    'name' => 'edit field_grievance_rep_lead',
    'roles' => array(
      'administrator' => 'administrator',
      'grievance admin' => 'grievance admin',
    ),
    'module' => 'field_permissions',
  );

  // Exported permission: 'edit field_grievance_rep_organizer'.
  $permissions['edit field_grievance_rep_organizer'] = array(
    'name' => 'edit field_grievance_rep_organizer',
    'roles' => array(
      'administrator' => 'administrator',
      'grievance admin' => 'grievance admin',
    ),
    'module' => 'field_permissions',
  );

  // Exported permission: 'edit field_grievance_shops'.
  $permissions['edit field_grievance_shops'] = array(
    'name' => 'edit field_grievance_shops',
    'roles' => array(
      'administrator' => 'administrator',
      'grievance admin' => 'grievance admin',
    ),
    'module' => 'field_permissions',
  );

  // Exported permission: 'edit mimemail user settings'.
  $permissions['edit mimemail user settings'] = array(
    'name' => 'edit mimemail user settings',
    'roles' => array(
      'administrator' => 'administrator',
      'authenticated user' => 'authenticated user',
    ),
    'module' => 'mimemail',
  );

  // Exported permission: 'edit own field_grievance_rep_lead'.
  $permissions['edit own field_grievance_rep_lead'] = array(
    'name' => 'edit own field_grievance_rep_lead',
    'roles' => array(
      'administrator' => 'administrator',
      'grievance admin' => 'grievance admin',
    ),
    'module' => 'field_permissions',
  );

  // Exported permission: 'edit own field_grievance_rep_organizer'.
  $permissions['edit own field_grievance_rep_organizer'] = array(
    'name' => 'edit own field_grievance_rep_organizer',
    'roles' => array(
      'administrator' => 'administrator',
      'grievance admin' => 'grievance admin',
    ),
    'module' => 'field_permissions',
  );

  // Exported permission: 'edit own field_grievance_shops'.
  $permissions['edit own field_grievance_shops'] = array(
    'name' => 'edit own field_grievance_shops',
    'roles' => array(
      'administrator' => 'administrator',
      'grievance admin' => 'grievance admin',
    ),
    'module' => 'field_permissions',
  );

  // Exported permission: 'edit own grievance content'.
  $permissions['edit own grievance content'] = array(
    'name' => 'edit own grievance content',
    'roles' => array(
      'grievance staff' => 'grievance staff',
      'grievance user' => 'grievance user',
    ),
    'module' => 'node',
  );

  // Exported permission: 'edit own grievance_attachment content'.
  $permissions['edit own grievance_attachment content'] = array(
    'name' => 'edit own grievance_attachment content',
    'roles' => array(
      'grievance staff' => 'grievance staff',
      'grievance user' => 'grievance user',
    ),
    'module' => 'node',
  );

  // Exported permission: 'edit own grievance_basic_page content'.
  $permissions['edit own grievance_basic_page content'] = array(
    'name' => 'edit own grievance_basic_page content',
    'roles' => array(
      'administrator' => 'administrator',
      'grievance admin' => 'grievance admin',
    ),
    'module' => 'node',
  );

  // Exported permission: 'edit own grievance_company content'.
  $permissions['edit own grievance_company content'] = array(
    'name' => 'edit own grievance_company content',
    'roles' => array(),
    'module' => 'node',
  );

  // Exported permission: 'edit own grievance_contract content'.
  $permissions['edit own grievance_contract content'] = array(
    'name' => 'edit own grievance_contract content',
    'roles' => array(),
    'module' => 'node',
  );

  // Exported permission: 'edit own grievance_contract_clause content'.
  $permissions['edit own grievance_contract_clause content'] = array(
    'name' => 'edit own grievance_contract_clause content',
    'roles' => array(),
    'module' => 'node',
  );

  // Exported permission: 'edit own grievance_contract_rule content'.
  $permissions['edit own grievance_contract_rule content'] = array(
    'name' => 'edit own grievance_contract_rule content',
    'roles' => array(),
    'module' => 'node',
  );

  // Exported permission: 'edit own grievance_contract_section content'.
  $permissions['edit own grievance_contract_section content'] = array(
    'name' => 'edit own grievance_contract_section content',
    'roles' => array(),
    'module' => 'node',
  );

  // Exported permission: 'edit own grievance_contract_template content'.
  $permissions['edit own grievance_contract_template content'] = array(
    'name' => 'edit own grievance_contract_template content',
    'roles' => array(),
    'module' => 'node',
  );

  // Exported permission: 'edit own grievance_field_overrides content'.
  $permissions['edit own grievance_field_overrides content'] = array(
    'name' => 'edit own grievance_field_overrides content',
    'roles' => array(),
    'module' => 'node',
  );

  // Exported permission: 'edit own grievance_holiday content'.
  $permissions['edit own grievance_holiday content'] = array(
    'name' => 'edit own grievance_holiday content',
    'roles' => array(),
    'module' => 'node',
  );

  // Exported permission: 'edit own grievance_log content'.
  $permissions['edit own grievance_log content'] = array(
    'name' => 'edit own grievance_log content',
    'roles' => array(
      'grievance staff' => 'grievance staff',
      'grievance user' => 'grievance user',
    ),
    'module' => 'node',
  );

  // Exported permission: 'edit own grievance_news content'.
  $permissions['edit own grievance_news content'] = array(
    'name' => 'edit own grievance_news content',
    'roles' => array(),
    'module' => 'node',
  );

  // Exported permission: 'edit own grievance_settlement content'.
  $permissions['edit own grievance_settlement content'] = array(
    'name' => 'edit own grievance_settlement content',
    'roles' => array(
      'grievance staff' => 'grievance staff',
    ),
    'module' => 'node',
  );

  // Exported permission: 'edit own grievance_shop content'.
  $permissions['edit own grievance_shop content'] = array(
    'name' => 'edit own grievance_shop content',
    'roles' => array(
      'administrator' => 'administrator',
      'grievance admin' => 'grievance admin',
    ),
    'module' => 'node',
  );

  // Exported permission: 'edit own grievance_shop_contact content'.
  $permissions['edit own grievance_shop_contact content'] = array(
    'name' => 'edit own grievance_shop_contact content',
    'roles' => array(
      'grievance staff' => 'grievance staff',
    ),
    'module' => 'node',
  );

  // Exported permission: 'edit own page content'.
  $permissions['edit own page content'] = array(
    'name' => 'edit own page content',
    'roles' => array(
      'administrator' => 'administrator',
      'grievance admin' => 'grievance admin',
    ),
    'module' => 'node',
  );

  // Exported permission: 'edit terms in grievance_alert_types'.
  $permissions['edit terms in grievance_alert_types'] = array(
    'name' => 'edit terms in grievance_alert_types',
    'roles' => array(),
    'module' => 'taxonomy',
  );

  // Exported permission: 'edit terms in grievance_document_types'.
  $permissions['edit terms in grievance_document_types'] = array(
    'name' => 'edit terms in grievance_document_types',
    'roles' => array(),
    'module' => 'taxonomy',
  );

  // Exported permission: 'edit terms in grievance_log_types'.
  $permissions['edit terms in grievance_log_types'] = array(
    'name' => 'edit terms in grievance_log_types',
    'roles' => array(),
    'module' => 'taxonomy',
  );

  // Exported permission: 'edit terms in grievance_remedies'.
  $permissions['edit terms in grievance_remedies'] = array(
    'name' => 'edit terms in grievance_remedies',
    'roles' => array(),
    'module' => 'taxonomy',
  );

  // Exported permission: 'edit terms in grievance_status'.
  $permissions['edit terms in grievance_status'] = array(
    'name' => 'edit terms in grievance_status',
    'roles' => array(),
    'module' => 'taxonomy',
  );

  // Exported permission: 'edit terms in grievance_tags'.
  $permissions['edit terms in grievance_tags'] = array(
    'name' => 'edit terms in grievance_tags',
    'roles' => array(),
    'module' => 'taxonomy',
  );

  // Exported permission: 'edit terms in grievance_types'.
  $permissions['edit terms in grievance_types'] = array(
    'name' => 'edit terms in grievance_types',
    'roles' => array(),
    'module' => 'taxonomy',
  );

  // Exported permission: 'edit terms in tags'.
  $permissions['edit terms in tags'] = array(
    'name' => 'edit terms in tags',
    'roles' => array(
      'administrator' => 'administrator',
      'grievance admin' => 'grievance admin',
    ),
    'module' => 'taxonomy',
  );

  // Exported permission: 'execute elysia_cron'.
  $permissions['execute elysia_cron'] = array(
    'name' => 'execute elysia_cron',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'elysia_cron',
  );

  // Exported permission: 'execute php code'.
  $permissions['execute php code'] = array(
    'name' => 'execute php code',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'devel',
  );

  // Exported permission: 'flush caches'.
  $permissions['flush caches'] = array(
    'name' => 'flush caches',
    'roles' => array(
      'administrator' => 'administrator',
      'grievance admin' => 'grievance admin',
    ),
    'module' => 'admin_menu',
  );

  // Exported permission: 'generate features'.
  $permissions['generate features'] = array(
    'name' => 'generate features',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'features',
  );

  // Exported permission: 'grievance admin'.
  $permissions['grievance admin'] = array(
    'name' => 'grievance admin',
    'roles' => array(
      'administrator' => 'administrator',
      'grievance admin' => 'grievance admin',
    ),
    'module' => 'grievance',
  );

  // Exported permission: 'grievance arbitrationform'.
  $permissions['grievance arbitrationform'] = array(
    'name' => 'grievance arbitrationform',
    'roles' => array(
      'administrator' => 'administrator',
      'grievance admin' => 'grievance admin',
      'grievance staff' => 'grievance staff',
    ),
    'module' => 'grievance',
  );

  // Exported permission: 'grievance attachment'.
  $permissions['grievance attachment'] = array(
    'name' => 'grievance attachment',
    'roles' => array(
      'administrator' => 'administrator',
      'grievance admin' => 'grievance admin',
      'grievance staff' => 'grievance staff',
      'grievance user' => 'grievance user',
    ),
    'module' => 'grievance',
  );

  // Exported permission: 'grievance contact'.
  $permissions['grievance contact'] = array(
    'name' => 'grievance contact',
    'roles' => array(
      'administrator' => 'administrator',
      'grievance admin' => 'grievance admin',
      'grievance staff' => 'grievance staff',
    ),
    'module' => 'grievance',
  );

  // Exported permission: 'grievance contract'.
  $permissions['grievance contract'] = array(
    'name' => 'grievance contract',
    'roles' => array(
      'administrator' => 'administrator',
      'grievance admin' => 'grievance admin',
    ),
    'module' => 'grievance',
  );

  // Exported permission: 'grievance forward'.
  $permissions['grievance forward'] = array(
    'name' => 'grievance forward',
    'roles' => array(
      'administrator' => 'administrator',
      'grievance admin' => 'grievance admin',
      'grievance staff' => 'grievance staff',
    ),
    'module' => 'grievance',
  );

  // Exported permission: 'grievance notification'.
  $permissions['grievance notification'] = array(
    'name' => 'grievance notification',
    'roles' => array(
      'administrator' => 'administrator',
      'grievance admin' => 'grievance admin',
      'grievance staff' => 'grievance staff',
      'grievance user' => 'grievance user',
    ),
    'module' => 'grievance',
  );

  // Exported permission: 'grievance quicksearch'.
  $permissions['grievance quicksearch'] = array(
    'name' => 'grievance quicksearch',
    'roles' => array(
      'administrator' => 'administrator',
      'grievance admin' => 'grievance admin',
      'grievance staff' => 'grievance staff',
    ),
    'module' => 'grievance',
  );

  // Exported permission: 'grievance report'.
  $permissions['grievance report'] = array(
    'name' => 'grievance report',
    'roles' => array(
      'administrator' => 'administrator',
      'grievance admin' => 'grievance admin',
      'grievance staff' => 'grievance staff',
    ),
    'module' => 'grievance',
  );

  // Exported permission: 'grievance settlement'.
  $permissions['grievance settlement'] = array(
    'name' => 'grievance settlement',
    'roles' => array(
      'administrator' => 'administrator',
      'grievance admin' => 'grievance admin',
      'grievance staff' => 'grievance staff',
    ),
    'module' => 'grievance',
  );

  // Exported permission: 'grievance statuslog'.
  $permissions['grievance statuslog'] = array(
    'name' => 'grievance statuslog',
    'roles' => array(
      'administrator' => 'administrator',
      'grievance admin' => 'grievance admin',
      'grievance staff' => 'grievance staff',
      'grievance user' => 'grievance user',
    ),
    'module' => 'grievance',
  );

  // Exported permission: 'grievance user'.
  $permissions['grievance user'] = array(
    'name' => 'grievance user',
    'roles' => array(
      'administrator' => 'administrator',
      'grievance admin' => 'grievance admin',
      'grievance user' => 'grievance user',
    ),
    'module' => 'grievance',
  );

  // Exported permission: 'grievance view'.
  $permissions['grievance view'] = array(
    'name' => 'grievance view',
    'roles' => array(
      'administrator' => 'administrator',
      'grievance admin' => 'grievance admin',
      'grievance view' => 'grievance view',
    ),
    'module' => 'grievance',
  );

  // Exported permission: 'import grievance_shops_import feeds'.
  $permissions['import grievance_shops_import feeds'] = array(
    'name' => 'import grievance_shops_import feeds',
    'roles' => array(),
    'module' => 'feeds',
  );

  // Exported permission: 'manage features'.
  $permissions['manage features'] = array(
    'name' => 'manage features',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'features',
  );

  // Exported permission: 'masquerade as admin'.
  $permissions['masquerade as admin'] = array(
    'name' => 'masquerade as admin',
    'roles' => array(
      'administrator' => 'administrator',
      'grievance admin' => 'grievance admin',
    ),
    'module' => 'masquerade',
  );

  // Exported permission: 'masquerade as any user'.
  $permissions['masquerade as any user'] = array(
    'name' => 'masquerade as any user',
    'roles' => array(
      'administrator' => 'administrator',
      'grievance admin' => 'grievance admin',
    ),
    'module' => 'masquerade',
  );

  // Exported permission: 'masquerade as user'.
  $permissions['masquerade as user'] = array(
    'name' => 'masquerade as user',
    'roles' => array(
      'administrator' => 'administrator',
      'grievance admin' => 'grievance admin',
    ),
    'module' => 'masquerade',
  );

  // Exported permission: 'rename features'.
  $permissions['rename features'] = array(
    'name' => 'rename features',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'features',
  );

  // Exported permission: 'revert revisions'.
  $permissions['revert revisions'] = array(
    'name' => 'revert revisions',
    'roles' => array(
      'administrator' => 'administrator',
      'grievance admin' => 'grievance admin',
    ),
    'module' => 'node',
  );

  // Exported permission: 'search content'.
  $permissions['search content'] = array(
    'name' => 'search content',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'search',
  );

  // Exported permission: 'select account cancellation method'.
  $permissions['select account cancellation method'] = array(
    'name' => 'select account cancellation method',
    'roles' => array(
      'administrator' => 'administrator',
      'grievance admin' => 'grievance admin',
    ),
    'module' => 'user',
  );

  // Exported permission: 'send arbitrary files'.
  $permissions['send arbitrary files'] = array(
    'name' => 'send arbitrary files',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'mimemail',
  );

  // Exported permission: 'switch shortcut sets'.
  $permissions['switch shortcut sets'] = array(
    'name' => 'switch shortcut sets',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'shortcut',
  );

  // Exported permission: 'switch users'.
  $permissions['switch users'] = array(
    'name' => 'switch users',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'devel',
  );

  // Exported permission: 'translate admin strings'.
  $permissions['translate admin strings'] = array(
    'name' => 'translate admin strings',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'i18n_string',
  );

  // Exported permission: 'translate content'.
  $permissions['translate content'] = array(
    'name' => 'translate content',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'translation',
  );

  // Exported permission: 'translate interface'.
  $permissions['translate interface'] = array(
    'name' => 'translate interface',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'locale',
  );

  // Exported permission: 'translate user-defined strings'.
  $permissions['translate user-defined strings'] = array(
    'name' => 'translate user-defined strings',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'i18n_string',
  );

  // Exported permission: 'unlock grievance_shops_import feeds'.
  $permissions['unlock grievance_shops_import feeds'] = array(
    'name' => 'unlock grievance_shops_import feeds',
    'roles' => array(),
    'module' => 'feeds',
  );

  // Exported permission: 'use PHP for settings'.
  $permissions['use PHP for settings'] = array(
    'name' => 'use PHP for settings',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'php',
  );

  // Exported permission: 'use PHP for title patterns'.
  $permissions['use PHP for title patterns'] = array(
    'name' => 'use PHP for title patterns',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'auto_nodetitle',
  );

  // Exported permission: 'use advanced search'.
  $permissions['use advanced search'] = array(
    'name' => 'use advanced search',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'search',
  );

  // Exported permission: 'use all savedsearches'.
  $permissions['use all savedsearches'] = array(
    'name' => 'use all savedsearches',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'views_savedsearches',
  );

  // Exported permission: 'use ctools import'.
  $permissions['use ctools import'] = array(
    'name' => 'use ctools import',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'ctools',
  );

  // Exported permission: 'use text format filtered_html'.
  $permissions['use text format filtered_html'] = array(
    'name' => 'use text format filtered_html',
    'roles' => array(
      'administrator' => 'administrator',
      'grievance admin' => 'grievance admin',
      'grievance user' => 'grievance user',
    ),
    'module' => 'filter',
  );

  // Exported permission: 'use text format full_html'.
  $permissions['use text format full_html'] = array(
    'name' => 'use text format full_html',
    'roles' => array(
      'administrator' => 'administrator',
      'grievance admin' => 'grievance admin',
      'grievance user' => 'grievance user',
    ),
    'module' => 'filter',
  );

  // Exported permission: 'use text format php_code'.
  $permissions['use text format php_code'] = array(
    'name' => 'use text format php_code',
    'roles' => array(),
    'module' => 'filter',
  );

  // Exported permission: 'use text format token_text'.
  $permissions['use text format token_text'] = array(
    'name' => 'use text format token_text',
    'roles' => array(
      'administrator' => 'administrator',
      'grievance admin' => 'grievance admin',
    ),
    'module' => 'filter',
  );

  // Exported permission: 'use views savedsearch'.
  $permissions['use views savedsearch'] = array(
    'name' => 'use views savedsearch',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'views_savedsearches',
  );

  // Exported permission: 'view custom_help_text'.
  $permissions['view custom_help_text'] = array(
    'name' => 'view custom_help_text',
    'roles' => array(
      'administrator' => 'administrator',
      'grievance admin' => 'grievance admin',
      'grievance user' => 'grievance user',
    ),
    'module' => 'custom_help_text',
  );

  // Exported permission: 'view elysia_cron'.
  $permissions['view elysia_cron'] = array(
    'name' => 'view elysia_cron',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'elysia_cron',
  );

  // Exported permission: 'view field_grievance_rep_lead'.
  $permissions['view field_grievance_rep_lead'] = array(
    'name' => 'view field_grievance_rep_lead',
    'roles' => array(
      'administrator' => 'administrator',
      'grievance admin' => 'grievance admin',
    ),
    'module' => 'field_permissions',
  );

  // Exported permission: 'view field_grievance_rep_organizer'.
  $permissions['view field_grievance_rep_organizer'] = array(
    'name' => 'view field_grievance_rep_organizer',
    'roles' => array(
      'administrator' => 'administrator',
      'grievance admin' => 'grievance admin',
    ),
    'module' => 'field_permissions',
  );

  // Exported permission: 'view field_grievance_shops'.
  $permissions['view field_grievance_shops'] = array(
    'name' => 'view field_grievance_shops',
    'roles' => array(
      'administrator' => 'administrator',
      'grievance admin' => 'grievance admin',
    ),
    'module' => 'field_permissions',
  );

  // Exported permission: 'view hidden fields'.
  $permissions['view hidden fields'] = array(
    'name' => 'view hidden fields',
    'roles' => array(),
    'module' => 'simplify',
  );

  // Exported permission: 'view own field_grievance_rep_lead'.
  $permissions['view own field_grievance_rep_lead'] = array(
    'name' => 'view own field_grievance_rep_lead',
    'roles' => array(
      'administrator' => 'administrator',
      'grievance admin' => 'grievance admin',
    ),
    'module' => 'field_permissions',
  );

  // Exported permission: 'view own field_grievance_rep_organizer'.
  $permissions['view own field_grievance_rep_organizer'] = array(
    'name' => 'view own field_grievance_rep_organizer',
    'roles' => array(
      'administrator' => 'administrator',
      'grievance admin' => 'grievance admin',
    ),
    'module' => 'field_permissions',
  );

  // Exported permission: 'view own field_grievance_shops'.
  $permissions['view own field_grievance_shops'] = array(
    'name' => 'view own field_grievance_shops',
    'roles' => array(
      'administrator' => 'administrator',
      'grievance admin' => 'grievance admin',
    ),
    'module' => 'field_permissions',
  );

  // Exported permission: 'view own unpublished content'.
  $permissions['view own unpublished content'] = array(
    'name' => 'view own unpublished content',
    'roles' => array(
      'administrator' => 'administrator',
      'grievance admin' => 'grievance admin',
      'grievance user' => 'grievance user',
    ),
    'module' => 'node',
  );

  // Exported permission: 'view revisions'.
  $permissions['view revisions'] = array(
    'name' => 'view revisions',
    'roles' => array(
      'administrator' => 'administrator',
      'grievance admin' => 'grievance admin',
    ),
    'module' => 'node',
  );

  // Exported permission: 'view the administration theme'.
  $permissions['view the administration theme'] = array(
    'name' => 'view the administration theme',
    'roles' => array(
      'administrator' => 'administrator',
      'grievance admin' => 'grievance admin',
      'grievance user' => 'grievance user',
    ),
    'module' => 'system',
  );

  return $permissions;
}
