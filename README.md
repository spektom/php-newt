PHP newt
==========
PHP language extension for RedHat Newt library, a terminal-based window and widget library for writing applications with user friendly interface. Once this extension is enabled in PHP it will provide the use of Newt widgets, such as windows, buttons, checkboxes, radiobuttons, labels, editboxes, scrolls, textareas, scales, etc. Use of this extension if very similar to the original Newt API of C programming language.

Example
--------

![Redhat configuration utility dialog](http://php-newt.sourceforge.net/screenshots/01.png)

Source code:

```php
<?
    newt_init ();
    newt_cls ();
    
    newt_draw_root_text (0, 0, "Test Mode Setup Utility 1.12");
    newt_push_help_line (null);
    newt_draw_root_text (-30, 0, "(c) 1999-2002 RedHat, Inc");

    newt_get_screen_size (&$rows, &$cols);
    
    newt_open_window ($rows/2-17, $cols/2-10, 34, 17, "Choose a Tool");

    $form = newt_form ();

    $list = newt_listbox (3, 2, 10);
    
    foreach (array (
        "Authentication configuration",
        "Firewall configuration",
        "Mouse configuration",
        "Network configuration",
        "Printer configuration",
        "System services") as $l_item)
    {
        newt_listbox_add_entry ($list, $l_item, $l_item);
    }

    $b1 = newt_button (5, 12, "Run Tool");
    $b2 = newt_button (21, 12, "Quit");
    
    newt_form_add_component ($form, $list);
    newt_form_add_components ($form, array($b1, $b2));

    newt_refresh ();
    newt_run_form ($form);

    newt_form_destroy ($f);
    newt_pop_window ();
    newt_finished ();
?>
```


Documentation
--------------
For documentation please refer to [PHP.net](http://php.net/newt)



