<?
# This script ranslates XML document, containing screen definition to PHP code,
# and executes it.

foreach (array('xsl','newt') as $ext) {
	if(!extension_loaded($ext)) {
		die ("This script requires PHP extension '$ext'!\n");
	}
}

if(!file_exists("php-newt.xslt")) {
	die ("Cannot find XSLT document: php-newt.xslt!\n");
}

if($_SERVER["argc"] != 2) {
	die ("USAGE: ".$_SERVER["argv"][0]." <XML document>\n");
}
$xml_filename = $_SERVER["argv"][1];

$xslt_proc = new xsltprocessor();
$xml_doc = new DomDocument;
$xslt_doc = new DomDocument;

$xml_doc->loadXML(file_get_contents($xml_filename));
$xslt_doc->loadXML(file_get_contents('php-newt.xslt'));
$xslt_proc->importStylesheet($xslt_doc);
$xml_result = $xslt_proc->transformToDoc($xml_doc);

eval(preg_replace('@<\?xml.*\?>@U', '', $xml_result->savexml()));

?>
