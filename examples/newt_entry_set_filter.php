<?php
class A
{
	function run()
	{
		newt_init ();
		newt_cls ();

		newt_centered_window (50, 10, "Test entry callbacks");

		$f = newt_form ();
		$l = newt_label (3, 1, "You cannot type whitespaces in this entry:");
		$e = newt_entry (5, 3, 40);
		newt_entry_set_filter ($e, array(&$this, "entry_callback"), null);
		$b = newt_button (21, 6, "OK");
		newt_form_add_components ($f, array ($l, $e, $b));
		newt_run_form ($f);

		newt_pop_window();
		newt_form_destroy($f);

		newt_finished ();
	}

	function entry_callback($e, $d, $ch, $cur)
	{
		if($ch == ' ') return 0;
		return $ch;
	}
}

$a =& new A();
$a->run();

?>
