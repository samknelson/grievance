<?php 
////////////////////////////////////////////////////////////////////////////////////////////////////
// POPUP VERSION
////////////////////////////////////////////////////////////////////////////////////////////////////
if (at_sirius_is_popup()) {
?>

<div id="page">
 <div id="columns" class="columns clearfix">
    <div class="container">
      <div id="content-column" class="content-column" role="main">
        <div class="content-inner">

          <?php print render($page['highlighted']); ?>

          <div id="messages-wrapper">
            <?php print $messages; ?>
          </div>
          <?php print render($page['help']); ?>

          <?php if ($action_links): ?>
            <nav class="actions-wrapper menu-wrapper clearfix">
              <ul class="action-links clearfix">
                <?php print render($action_links); ?>
              </ul>
            </nav>
          <?php endif; ?>

          <<?php print $tag; ?> id="main-content">

            <!-- region: Main Content -->
            <?php if ($content = render($page['content'])): ?>
              <div id="content">
                <?php print $content; ?>
              </div>
            <?php endif; ?>

            <!-- Feed icons (RSS, Atom icons etc -->
            <?php print $feed_icons; ?>
            
          </<?php print $tag; ?>><!-- /end #main-content -->

        </div><!-- /end .content-inner -->
      </div><!-- /end #content-column -->

    </div><!-- /end #columns -->
  </div>
</div>
<?php 
////////////////////////////////////////////////////////////////////////////////////////////////////
// NORMAL VERSION
////////////////////////////////////////////////////////////////////////////////////////////////////
} else {
?>
<div id="page">

    <div id="page-banner">
      <div class="container">
      	<div class="user-links">
          
					<?php
          global $user;
          
          if ($user->uid) 
          {
            print t('<div class="user-links-content">');
            print t('Welcome, ');
            print $user->name;
						print "&nbsp;&nbsp;&nbsp;&nbsp;";

            $user_full = user_load($user->uid);
            if ($user_full->field_grievance_shops['und'][0]['target_id']) {
              $shop_nid = $user_full->field_grievance_shops['und'][0]['target_id'];
              $shop_node = node_load($shop_nid);
              // print l($shop_node->title, 'node/' . $shop_node->nid);
              print $shop_node->title;
              print ' / ';
              $contract_uri = $shop_node->field_grievance_contract['und'][0]['uri'];
              if ($contract_uri) {
                $contract_url = file_create_url($contract_uri);
                print l(t('contract (pdf)'), $contract_url, array('attributes' => array('target'=>'_blank')));
                print ' / ';
              }
            }

            if (user_access('sirius edit own user')) {
              print l(t('account'),'user/'.$user->uid.'/edit');
              print " / ";
            }
            print l(t("logout"),"user/logout");
            print t('</div>');
          ?>
          <!--
            / <a href="#" onClick="javascript:sirius_popup('/sirius/dispatch/operator', 'sirius_operator', 1025, 600); return false;">operator window</a>
          -->
          <?php 
          }
          else 
          {
            print t('<div class="user-links-content anon-user-links-content">');
            print l("login","user/login");
            print t('</div>');
          }
          ?>
          <div class="user-links-text">
          <?php print variable_get('sirius_banner_text', ''); ?>
        </div>

        </div>

        <div class="banner-wrapper">
          <div class="inner">
            <a href="/">
            
              <?php 
              $logo_file = file_load(variable_get('sirius_banner_logo', ''));
              if ($logo_file) {
                $logo_url = file_create_url($logo_file->uri);
                global $base_url;
                $logo_url = str_replace($base_url, '', $logo_url);
                echo('<span class="banner-logo-wrapper">');
                echo("<img class=\"banner-logo\" src=\"$logo_url\">");
                echo('</span>');
              }
              ?>
              <span class="banner-name-wrapper">
                <?php echo(variable_get('sirius_banner_name', '')); ?>
              </span>
            </a>
          </div>
        </div>

        <?php print render($page['banner']); ?>
        <!--
        <a href="/"><img src="/sites/all/themes/at_tf_admin/images/banner.png" alt="UNITE HERE Local 100 Grievances Database" border="0" style="height: auto; max-width: 960px; width: 100%;" /></a>
        -->
      </div>
    </div>


      <!-- region: Banner Menu -->
      <div class="banner-menu-outer-wrapper">
        <div class="container">
          <?php $banner_menu = render($page['banner_menu']); print $banner_menu; ?>
        </div>
      </div>

  <div class="topbar-wrapper">
    <div class="container">
      <div id="topbar" class="clearfix">
        <?php if ($datetime_rfc): ?>
          <time datetime="<?php print $datetime_iso; ?>"><?php print $datetime_rfc; ?></time>
        <?php endif; ?>
      </div>
    </div>
  </div>

  <div id="header" class="content-header">
    <div class="container">
      <header<?php print $content_header_attributes; ?>>
        <?php print $breadcrumb; ?>
        <?php print render($title_prefix); ?>
        <?php if ($title): ?>
          <h1 id="page-title"><?php print $title; ?></h1>
        <?php endif; ?>
        <?php print render($title_suffix); ?>

        <!-- region: Sub-Title -->
        <?php if ($page['subtitle']): ?>
          <footer id="page-subtitle">
            <div class="container">
              <?php print render($page['subtitle']); ?>
            </div>
          </footer>
        <?php endif; ?>

      </header>
    </div>
  </div>


  <?php if ($primary_local_tasks): ?>
    <div class="primary-local-tasks-wrapper">
      <div class="container">
          <nav id="primary-tasks" class="clearfix<?php print $secondary_local_tasks ? ' with-secondary' : '' ?>" role="navigation">
            <ul class="tabs primary"><?php print render($primary_local_tasks); ?></ul>
          </nav>
      </div>
    </div>
  <?php endif; ?>

  <?php if ($secondary_local_tasks): ?>
    <div class="secondary-tasks-wrapper">
      <div class="container">
        <nav id="secondary-tasks" class="clearfix menu-wrapper" role="navigation">
          <ul class="tabs secondary clearfix"><?php print render($secondary_local_tasks); ?></ul>
        </nav>
      </div>
    </div>
  <?php endif; ?>

  <div id="columns" class="columns clearfix">
    <div class="container">
      <div id="content-column" class="content-column" role="main">
        <div class="content-inner">

          <?php print render($page['highlighted']); ?>
          <div id="messages-wrapper">
            <?php print $messages; ?>
          </div>
          <?php print render($page['help']); ?>

          <?php if ($action_links): ?>
            <nav class="actions-wrapper menu-wrapper clearfix">
              <ul class="action-links clearfix">
                <?php print render($action_links); ?>
              </ul>
            </nav>
          <?php endif; ?>

          <<?php print $tag; ?> id="main-content">

            <!-- region: Main Content -->
            <?php if ($content = render($page['content'])): ?>
              <div id="content">
                <?php print $content; ?>
              </div>
            <?php endif; ?>

            <!-- Feed icons (RSS, Atom icons etc -->
            <?php print $feed_icons; ?>
            
          </<?php print $tag; ?>><!-- /end #main-content -->

        </div><!-- /end .content-inner -->
      </div><!-- /end #content-column -->

      <!-- regions: Sidebar first and Sidebar second -->
      <?php $sidebar_first = render($page['sidebar_first']); print $sidebar_first; ?>
      <?php $sidebar_second = render($page['sidebar_second']); print $sidebar_second; ?>

    </div><!-- /end #columns -->

  </div>

  <?php if ($page['footer']): ?>
    <footer id="page-footer">
      <div class="container">
        <?php print render($page['footer']); ?>
      </div>
    </footer>
  <?php endif; ?>

</div>
<?php } ?>