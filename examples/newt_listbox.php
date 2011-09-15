<?php

newt_init ();
newt_cls ();
newt_get_screen_size (&$rows, &$cols);
newt_open_window ($rows/2-20, $cols/2-10, 54, 20, "Choose");

$form = newt_form ();
newt_form_add_hot_key ($form, NEWT_KEY_ESCAPE);

$list = newt_listbox (2, 1, 13, NEWT_LISTBOX_RETURNEXIT);
foreach (array (
       "Authentication configuration",
       "Firewall configuration",
       "Mouse configuration",
       "Network configuration",
       "Printer configuration",
       "System services") as $l_item)
   newt_listbox_add_entry ($list, $l_item, $l_item);

newt_form_add_component ($form, $list);
newt_refresh ();
newt_form_run ($form, &$res);

$chosen = newt_listbox_get_current($list);

newt_pop_window ();
newt_finished ();
newt_form_destroy ($form);

print "You've chosen: {$chosen}\n";

?>
