<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">

	<xsl:output method="text" omit-xml-declaration="yes" standalone="no" indent="no"/>

	<!-- Don't print unrecognized data //-->
	<xsl:template match="text()">
		<xsl:apply-templates/>
	</xsl:template>

	<!-- Init newt toolkit, first //-->
	<xsl:template match="/">
		<!-- Initialize newt and clear screen: //-->
		<xsl:text disable-output-escaping="yes">
			if(!defined('V_NOEMPTY')) define('V_NOEMPTY', 1&lt;&lt;1);
			if(!defined('V_FILE_EXISTS')) define('V_FILE_EXISTS', 1&lt;&lt;2);
			if(!defined('V_DIR_EXISTS')) define('V_DIR_EXISTS', 1&lt;&lt;3);
			if(!defined('V_URL')) define('V_URL', 1&lt;&lt;4);
		</xsl:text>

		newt_init();
		<!-- Build all components and show them: //-->
		<xsl:apply-templates/>
		<!-- Uninitialize newt: //-->

		newt_finished();
	</xsl:template>

	<xsl:template match="window">
		<!-- Set window title and backtitle: //-->
		<xsl:choose>
			<xsl:when test="@title != ''">
				$TITLE = "<xsl:value-of select="@title"/>";
			</xsl:when>
			<xsl:otherwise>
				$TITLE = "";
			</xsl:otherwise>
		</xsl:choose>

		<!-- Open new window: //-->
		<xsl:text disable-output-escaping="yes">newt_get_screen_size (&amp;$COLS, &amp;$ROWS);</xsl:text>
		<!-- Indent from the edges on 4 symbols by default: //-->
		$W_LEFT = 4;
		$W_TOP = 4;
		$W_WIDTH = $COLS - 8;
		$W_HEIGHT = $ROWS - 8;

		<xsl:if test="@left != ''">
			$W_LEFT = <xsl:value-of select="@left"/>;
		</xsl:if>
		<xsl:if test="@top != ''">
			$W_TOP = <xsl:value-of select="@top"/>;
		</xsl:if>
		<xsl:if test="@width != ''">
			$W_WIDTH = <xsl:value-of select="@width"/>;
		</xsl:if>
		<xsl:if test="@height != ''">
			$W_HEIGHT = <xsl:value-of select="@height"/>;
		</xsl:if>
		<xsl:choose>
			<xsl:when test="@left != '' or @top != ''">
				newt_open_window ($W_LEFT, $W_TOP, $W_WIDTH, $W_HEIGHT, $TITLE);
			</xsl:when>
			<xsl:otherwise>
				newt_centered_window ($W_WIDTH, $W_HEIGHT, $TITLE);
			</xsl:otherwise>
		</xsl:choose>
		<!-- Display backtitle: //-->
		<xsl:if test="@backtitle != ''">
			newt_draw_root_text (0, 0, "<xsl:value-of select="@backtitle"/>");
		</xsl:if>
		<!-- Continue processing all sub-forms of window: //-->
		<xsl:apply-templates select=".//form"/>
	</xsl:template>

	<!-- Process main form: //-->
	<xsl:template match="form">
		$<xsl:value-of select="@id"/> = newt_form();
		<xsl:if test="@width != ''">
			newt_form_set_width ($<xsl:value-of select="@id"/>,<xsl:value-of select="@width"/>);
		</xsl:if>
		<xsl:if test="@height != ''">
			newt_form_set_height ($<xsl:value-of select="@id"/>,<xsl:value-of select="@height"/>);
		</xsl:if>

		<!-- Continue processing all sub-components of form: //-->
		<xsl:apply-templates select=".//*"/>

		<!-- Run main form: //-->
		newt_refresh();

		<!-- Run form and validate all subcomponents: //-->
		do {
			$last_comp = newt_run_form($<xsl:value-of select="@id"/>);
			if ($last_comp == $cancel) {
				break;
			}
			$form_is_validated = true;

			foreach ($comps as $comp) {

				if (!isset($comp['validate'])) {
					continue;
				}

				if ($comp['type'] == "entry") {
					$res = trim(newt_entry_get_current($comp['comp']));

					<xsl:text disable-output-escaping="yes">if($comp['validate'] &amp; V_NOEMPTY) {</xsl:text>
						if(empty ($res)) {
							newt_win_message ("Error", "OK", "Field '%s' is empty!", isset($comp['name']) ? $comp['name'] : $comp['id']);
							$form_is_validated = false;
							break;
						}
					}
					<xsl:text disable-output-escaping="yes">if($comp['validate'] &amp; V_FILE_EXISTS) {</xsl:text>
						if(!@file_exists ($res)) {
							newt_win_message ("Error", "OK", "File: '%s' doesn't exist!", $res);
							$form_is_validated = false;
							break;
						}
					}
					<xsl:text disable-output-escaping="yes">if($comp['validate'] &amp; V_DIR_EXISTS) {</xsl:text>
						if (!@file_exists ($res) || !@is_dir($res)) {
							newt_win_message ("Error", "OK", "Directory: '%s' doesn't exist!", $res);
							$form_is_validated = false;
							break;
						}
					}
				}
			}
		
		} while (!$form_is_validated);

		<!-- Retreive submitted values //-->
		foreach ($comps as $comp) {
			switch ($comp['type'])
			{
				case "entry":
					$ret_vals[$comp['id']] = newt_entry_get_value ($comp['comp']);
					break;
					
				case "checkbox":
					$ret_vals[$comp['id']] = newt_checkbox_get_value ($comp['comp']);
					break;
					
				case "radiobox":
					$rb = newt_radio_get_current ($comp['comp']);
					foreach ($comps as $rb_comp) {
						if ($comps['type'] != "radiobutton") {
							continue;
						}
						if ($rb_comp['comp'] == $rb) {
							$ret_vals[$comp['id']] = $rb_comp['id'];
							break;
						}
					}
					break;
					
				case "listbox":
					$ret_vals[$comp['id']] = newt_listbox_get_value ($comp['comp']);
					break;

				case "button":
					if ($comp['comp'] == $last_comp) {
						$last_comp_id = $comp['id'];
					}
					break;
			}
		}

		newt_form_destroy ($<xsl:value-of select="@id"/>);
	</xsl:template>

	<!-- Process buttons //-->
	<xsl:template match="button">
		$<xsl:value-of select="@id"/> = newt_button(
			<xsl:value-of select="@left"/>,
			<xsl:value-of select="@top"/>,
			"<xsl:value-of select="text()"/>"
			);
		newt_form_add_component ($<xsl:value-of select="ancestor::form/@id"/>, $<xsl:value-of select="@id"/>);

		$comps[] = array (
			<xsl:text disable-output-escaping="yes">'type' =&gt; "button",</xsl:text>
			<xsl:text disable-output-escaping="yes">'comp' =&gt;</xsl:text> $<xsl:value-of select="@id"/>,
			<xsl:text disable-output-escaping="yes">'id' =&gt;</xsl:text> "<xsl:value-of select="@id"/>"
		);
	</xsl:template>

	<!-- Process labels //-->
	<xsl:template match="label">
		$<xsl:value-of select="@id"/> = newt_label(
			<xsl:value-of select="@left"/>,
			<xsl:value-of select="@top"/>,
			"<xsl:value-of select="text()"/>"
			);
		newt_form_add_component ($<xsl:value-of select="ancestor::form/@id"/>, $<xsl:value-of select="@id"/>);
	</xsl:template>

	<!-- Process editboxes //-->
	<xsl:template match="entry">
		$<xsl:value-of select="@id"/> = newt_entry(
			<xsl:value-of select="@left"/>,
			<xsl:value-of select="@top"/>,
			<xsl:value-of select="@width"/>
			<xsl:if test="text() != ''">, "<xsl:value-of select="text()"/>"</xsl:if>
			);
		newt_form_add_component ($<xsl:value-of select="ancestor::form/@id"/>, $<xsl:value-of select="@id"/>);

		$comps[] = array(
			<xsl:text disable-output-escaping="yes">'type' =&gt; "entry",</xsl:text>
			<xsl:text disable-output-escaping="yes">'comp' =&gt;</xsl:text> $<xsl:value-of select="@id"/>,
			<xsl:if test="@validate != ''">
				<xsl:text disable-output-escaping="yes">'validate' =&gt;</xsl:text> <xsl:value-of select="@validate"/>,
			</xsl:if>
			<xsl:if test="@name != ''">
				<xsl:text disable-output-escaping="yes">'name' =&gt;</xsl:text> "<xsl:value-of select="@name"/>",
			</xsl:if>
			<xsl:text disable-output-escaping="yes">'id' =&gt;</xsl:text> "<xsl:value-of select="@id"/>"
		);
	</xsl:template>

	<!-- Process passwordboxes (it's really an editbox, different is the flag) //-->
	<xsl:template match="password">
		$<xsl:value-of select="@id"/> = newt_entry(
			<xsl:value-of select="@left"/>,
			<xsl:value-of select="@top"/>,
			<xsl:value-of select="@width"/>,
			<xsl:choose>
				<xsl:when test="text() != ''">"<xsl:value-of select="text()"/>"</xsl:when>
				<xsl:otherwise>NULL</xsl:otherwise>
			</xsl:choose>
			,NEWT_FLAG_PASSWORD
			);
		newt_form_add_component ($<xsl:value-of select="ancestor::form/@id"/>, $<xsl:value-of select="@id"/>);

		$comps[] = array(
			<xsl:text disable-output-escaping="yes">'type' =&gt; "entry",</xsl:text>
			<xsl:text disable-output-escaping="yes">'comp' =&gt;</xsl:text> $<xsl:value-of select="@id"/>,
			<xsl:if test="@validate != ''">
				<xsl:text disable-output-escaping="yes">'validate' =&gt;</xsl:text> <xsl:value-of select="@validate"/>,
			</xsl:if>
			<xsl:if test="@name != ''">
				<xsl:text disable-output-escaping="yes">'name' =&gt;</xsl:text> "<xsl:value-of select="@name"/>",
			</xsl:if>
			<xsl:text disable-output-escaping="yes">'id' =&gt;</xsl:text> "<xsl:value-of select="@id"/>"
		);
	</xsl:template>

	<!-- Process checkboxes //-->
	<xsl:template match="checkbox">
		$<xsl:value-of select="@id"/> = newt_checkbox(
			<xsl:value-of select="@left"/>,
			<xsl:value-of select="@top"/>,
			"<xsl:value-of select="text()"/>",
			'<xsl:value-of select="@value"/>'
			);
		newt_form_add_component ($<xsl:value-of select="ancestor::form/@id"/>, $<xsl:value-of select="@id"/>);

		$comps[] = array(
			<xsl:text disable-output-escaping="yes">'type' =&gt; "checkbox",</xsl:text>
			<xsl:text disable-output-escaping="yes">'comp' =&gt;</xsl:text> $<xsl:value-of select="@id"/>,
			<xsl:text disable-output-escaping="yes">'id' =&gt;</xsl:text> "<xsl:value-of select="@id"/>"
		);
	</xsl:template>

	<!-- Process radioboxes //-->
	<xsl:template match="radiobox">
		<xsl:for-each select=".//radiobutton">
			$<xsl:value-of select="@id"/> = newt_radiobutton(
				<xsl:value-of select="@left"/>,
				<xsl:value-of select="@top"/>,
				"<xsl:value-of select="text()"/>",
				<xsl:choose>
					<xsl:when test="ancestor::radiobox/@value = @id">1</xsl:when>
					<xsl:otherwise>0</xsl:otherwise>
				</xsl:choose>
				<xsl:if test="preceding-sibling::radiobutton/@id != ''">
					,$<xsl:value-of select="preceding-sibling::radiobutton/@id"/>
				</xsl:if>
				);
			newt_form_add_component ($<xsl:value-of select="ancestor::form/@id"/>, $<xsl:value-of select="@id"/>);
			$comps[] = array(
				<xsl:text disable-output-escaping="yes">'type' =&gt; "radiobutton",</xsl:text>
				<xsl:text disable-output-escaping="yes">'comp' =&gt;</xsl:text> $<xsl:value-of select="@id"/>,
				<xsl:text disable-output-escaping="yes">'id' =&gt;</xsl:text> "<xsl:value-of select="@id"/>"
			);
		</xsl:for-each>

		$comps[] = array(
			<xsl:text disable-output-escaping="yes">'type' =&gt; "radiobox",</xsl:text>
			<xsl:text disable-output-escaping="yes">'comp' =&gt;</xsl:text> $<xsl:value-of select="@id"/>,
			<xsl:text disable-output-escaping="yes">'id' =&gt;</xsl:text> "<xsl:value-of select="@id"/>"
		);
	</xsl:template>

	<!-- Process textboxes //-->
	<xsl:template match="textbox">
		$<xsl:value-of select="@id"/> = newt_textbox(
			<xsl:value-of select="@left"/>,
			<xsl:value-of select="@top"/>,
			<xsl:value-of select="@width"/>,
			<xsl:value-of select="@height"/>,
			NEWT_FLAG_WRAP|NEWT_FLAG_SCROLL
			);
		newt_textbox_set_text ($<xsl:value-of select="@id"/>, @file_get_contents('<xsl:value-of select="@file"/>'));
		newt_form_add_component ($<xsl:value-of select="ancestor::form/@id"/>, $<xsl:value-of select="@id"/>);
	</xsl:template>

	<!-- Process textareas (it's really a textbox, with different flag) //-->
	<xsl:template match="textarea">
		$<xsl:value-of select="@id"/> = newt_textbox(
			<xsl:value-of select="@left"/>,
			<xsl:value-of select="@top"/>,
			<xsl:value-of select="@width"/>,
			<xsl:value-of select="@height"/>,
			NEWT_FLAG_WRAP|NEWT_FLAG_SCROLL
			);
		newt_textbox_set_text ($<xsl:value-of select="@id"/>, "<xsl:value-of select="text()"/>");
		newt_form_add_component ($<xsl:value-of select="ancestor::form/@id"/>, $<xsl:value-of select="@id"/>);
	</xsl:template>

	<!-- Process listboxes //-->
	<xsl:template match="listbox">
		$<xsl:value-of select="@id"/> = newt_listbox(
			<xsl:value-of select="@left"/>,
			<xsl:value-of select="@top"/>,
			<xsl:value-of select="@height"/>,
			NEWT_FLAG_SCROLL|NEWT_FLAG_BORDER
			);
		<xsl:if test="@width != ''">
			newt_listbox_set_width ($<xsl:value-of select="@id"/>, <xsl:value-of select="@width"/>);
		</xsl:if>
		<!-- Process items of listbox //-->
		<xsl:for-each select=".//item">
			$<xsl:value-of select="@id"/> = newt_listbox_add_entry(
				$<xsl:value-of select="ancestor::listbox/@id"/>,
				"<xsl:value-of select="text()"/>",
				"<xsl:value-of select="@id"/>"
				);
		</xsl:for-each>
		newt_listbox_set_current_by_key ($<xsl:value-of select="@id"/>, "<xsl:value-of select="@value"/>");
		newt_form_add_component ($<xsl:value-of select="ancestor::form/@id"/>, $<xsl:value-of select="@id"/>);

		$comps[] = array(
			<xsl:text disable-output-escaping="yes">'type' =&gt; "listbox",</xsl:text>
			<xsl:text disable-output-escaping="yes">'comp' =&gt;</xsl:text> $<xsl:value-of select="@id"/>,
			<xsl:text disable-output-escaping="yes">'id' =&gt;</xsl:text> "<xsl:value-of select="@id"/>"
		);
	</xsl:template>

	<!-- Process menuboxes (it's really a listbox) //-->
	<xsl:template match="menubox">
		$<xsl:value-of select="@id"/> = newt_listbox(
			<xsl:value-of select="@left"/>,
			<xsl:value-of select="@top"/>,
			<xsl:value-of select="@height"/>,
			NEWT_FLAG_SCROLL
			);
		newt_listbox_set_width ($<xsl:value-of select="@id"/>, <xsl:value-of select="@width"/>);
		<!-- Process items of menuboxes //-->
		<xsl:for-each select=".//item">
			$<xsl:value-of select="@id"/> = newt_listbox_add_entry(
				$<xsl:value-of select="ancestor::menubox/@id"/>,
				"<xsl:value-of select="text()"/>",
				"<xsl:value-of select="@id"/>"
				);
		</xsl:for-each>
		newt_listbox_set_current_by_key ($<xsl:value-of select="@id"/>, "<xsl:value-of select="@value"/>");
		newt_form_add_component ($<xsl:value-of select="ancestor::form/@id"/>, $<xsl:value-of select="@id"/>);

		$comps[] = array(
			<xsl:text disable-output-escaping="yes">'type' =&gt; "listbox",</xsl:text>
			<xsl:text disable-output-escaping="yes">'comp' =&gt;</xsl:text> $<xsl:value-of select="@id"/>,
			<xsl:text disable-output-escaping="yes">'id' =&gt;</xsl:text> "<xsl:value-of select="@id"/>"
		);
	</xsl:template>

	<!-- Process scales //-->
	<xsl:template match="scale">
		$<xsl:value-of select="@id"/> = newt_scale(
			<xsl:value-of select="@left"/>,
			<xsl:value-of select="@top"/>,
			<xsl:value-of select="@width"/>,
			<xsl:value-of select="@size"/>
			);
		newt_form_add_component ($<xsl:value-of select="ancestor::form/@id"/>, $<xsl:value-of select="@id"/>);
	</xsl:template>

	<!-- Process scrollbars //-->
	<xsl:template match="scrollbar">
		$<xsl:value-of select="@id"/> = newt_vertical_scrollbar(
			<xsl:value-of select="@left"/>,
			<xsl:value-of select="@top"/>,
			<xsl:value-of select="@height"/>
			);
		newt_form_add_component ($<xsl:value-of select="ancestor::form/@id"/>, $<xsl:value-of select="@id"/>);
	</xsl:template>

</xsl:stylesheet>
