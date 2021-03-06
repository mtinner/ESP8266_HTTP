

/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * Jeroen Domburg <jeroen@spritesmods.com> wrote this file. As long as you retain 
 * this notice you can do whatever you want with this stuff. If we meet some day, 
 * and you think this stuff is worth it, you can buy me a beer in return. 
 * ----------------------------------------------------------------------------
 */


#include "espmissingincludes.h"
#include "ets_sys.h"
#include "osapi.h"
#include "httpd.h"
#include "io.h"
#include "httpdespfs.h"
#include "cgi.h"
#include "stdout.h"




/*
This is the main url->function dispatching data struct.
In short, it's a struct with various URLs plus their handlers. The handlers can
be 'standard' CGI functions you wrote, or 'special' CGIs requiring an argument.
They can also be auth-functions. An asterisk will match any url starting with
everything before the asterisks; "*" matches everything. The list will be
handled top-down, so make sure to put more specific rules above the more
general ones. Authorization things (like authBasic) act as a 'barrier' and
should be placed above the URLs they protect.
*/
HttpdBuiltInUrl builtInUrls[]={
	//{"/", cgiRedirect, "/index.tpl"},
	//{"/flash.bin", cgiReadFlash, NULL},
	//{"/led.tpl", cgiEspFsTemplate, tplLed},
	//{"/index.tpl", cgiEspFsTemplate, tplCounter},
	//{"/led.cgi", cgiLed, NULL},
	{"/output", postLed, ""},
	{"/inputs", getStationInputs, NULL},
	//step2
	//{"/config", setConfig, NULL},
	//not needed
	//{"/led/*", getLed, NULL},
	//{"/updateweb.cgi", cgiUploadEspfs, NULL},


	{"*", cgiEspFsHook, NULL}, //Catch-all cgi function for the filesystem
	{NULL, NULL, NULL}
};


//Main routine. Initialize stdout, the I/O and the webserver and we're done.
void user_init(void) {
	stdoutInit();
	ioInit();
	httpdInit(builtInUrls, 80);
	os_printf("\nReady\n");
}
