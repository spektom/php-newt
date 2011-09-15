<?
	newt_init ();
	newt_cls ();

	$options = array("eth0", "eth1", "lo");
	$option = 1;
	
	$rc = newt_win_menu("Network configuration tool", "Which interface to configure?", 30, 0, 20, 6, $options, &$option, "Ok", "Back");

	newt_finished ();

	if($rc == 1) {
		echo "You have choosed to configure interface: ".$options[$option]."\n";
	} else if($rc == 2) {
		echo "You have pressed Back, but there's no screen to display :(\n";
	}
?>
