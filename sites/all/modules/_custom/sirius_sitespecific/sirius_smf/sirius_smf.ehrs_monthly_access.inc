<?php

class Sirius_Smf_Ehrs_Monthly_Access extends Sirius_Employer_Monthly_Upload_Access {
  public function is_employer() { return user_access('sirius trust employer'); }
  public function is_admin() { return user_access('sirius_admin') || user_access('sirius trust admin'); }
  public function is_staff() { return user_access('sirius trust admin'); }
}
