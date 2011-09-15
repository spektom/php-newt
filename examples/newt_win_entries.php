<?
	newt_init ();
	newt_cls ();

	$entries[] = array('text' => "First name:", 'value' => &$f_name);
	$entries[] = array('text' => "Last name:", 'value' => &$l_name);

	$rc = newt_win_entries("User information", "Please enter your credentials:", 50, 7, 7, 30, $entries, "Ok", "Back");
	newt_finished ();

	if ($rc != 2) {
		print ("Your name is: $f_name $l_name\n");
		#print ("Your name is: {$entries[0]['value']} {$entries[1]['value']}\n");
	}
?>
