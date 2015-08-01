/*
 Some random cgi routines. Used in the LED example and the page that returns the entire
 flash as a binary. Also handles the hit counter on the main page.
 */

/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * Jeroen Domburg <jeroen@spritesmods.com> wrote this file. As long as you retain 
 * this notice you can do whatever you want with this stuff. If we meet some day, 
 * and you think this stuff is worth it, you can buy me a beer in return. 
 * ----------------------------------------------------------------------------
 */

#include <string.h>
#include <stdio.h>
#include <osapi.h>
#include "user_interface.h"
#include "mem.h"
#include "httpd.h"
#include "cgi.h"
#include "io.h"
#include <ip_addr.h>
#include "espmissingincludes.h"
#include "../include/httpdconfig.h"

//cause I can't be bothered to write an ioGetLed()
static char currLedState = 0;
const char ip[] = "10.0.0.40";
const int inputPorts[] = { 0 };
const int outputPorts[] = { 2 };

//step2
/*struct Station {
	char* ip;
	int32_t floor;
	Lamp *lampList[4];
	int32_t numberOfLamp;
};

struct Lamp {
	int32_t internalId;
	ESPInput *input;
	ESPOutput *output;
};

struct ESPInput {
	int32_t internalPort;
	int32_t status;
};

struct ESPOutput {
	int32_t internalPort;
	int32_t status;
};

Station station;*/


//Cgi that turns the LED on or off according to the 'led' param in the POST data
int ICACHE_FLASH_ATTR cgiLed(HttpdConnData *connData) {
	int len;
	char buff[1024];

	if (connData->conn == NULL) {
		//Connection aborted. Clean up.
		return HTTPD_CGI_DONE;
	}

	len = httpdFindArg(connData->post->buff, "led", buff, sizeof(buff));
	if (len != 0) {
		currLedState = atoi(buff);
		ioLed(currLedState);
	}

	httpdRedirect(connData, "led.tpl");
	return HTTPD_CGI_DONE;
}

void prepareToSetLed(HttpdConnData* connData) {
	//POST
	int led;
	int status;
	led = httpFindValueFromArg(connData->post->buff, "internalPort");
	status = httpFindValueFromArg(connData->post->buff, "status");
	os_printf("LED: %d\n", led);
	os_printf("Is set to status: %d\n", status);
	if (led != -1 && status != -1) {
		setLed(status, led);
	}
	httpdSend(connData, "", 2);
}

//step2
/*
int ICACHE_FLASH_ATTR setConfig(HttpdConnData *connData) {
	if (connData->conn == NULL) {
		//Connection aborted. Clean up.
		return HTTPD_CGI_DONE;
	}

	if (connData->requestType == 2) { //POST
		//os_printf("Config: %s\n", connData->post->buff);
		//set floor
		station.floor = httpFindValueFromArg(connData->post->buff, "floor");
		char * jsonString = strstr(connData->post->buff, "floor");
		os_printf("JsonString begin: %s\n", jsonString);
		os_printf("floor: %d\n", station.floor);
		station.ip = "";
		//set IP
		station.ip = httpFindStringFromArg(jsonString, "ip");
		//cut string befor IP
		jsonString = strstr(jsonString, "ip");
		os_printf("IP: %s\n", station.ip);
		Lamp lampList[4] = { };
		bool check = true;
		for (int i = 0; strstr(jsonString, "internalId") && check; i++) {
			ESPInput input;
			ESPOutput output;
			lampList[i].input = &input;
			lampList[i].output = &output;
			station.lampList[i] = &lampList[i];
			//looking for internal port (input)
			//start set Input
			input.internalPort = httpFindValueFromArg(jsonString,
					"internalPort");
			jsonString = strstr(jsonString, "internalPort");
			os_printf("status of internalPort: %d\n",
					station.lampList[i]->input->internalPort);
			//set status (input)
			input.status = httpFindValueFromArg(jsonString, "status");
			jsonString = strstr(jsonString, "status");
			os_printf("Status input: %d\n", station.lampList[i]->input->status);
			//set InternalID
			lampList[i].internalId = httpFindValueFromArg(jsonString,
					"internalId");
			jsonString = strstr(jsonString, "internalId");
			os_printf("internalId : %d\n", lampList[i].internalId);
			//looking for internal port (output)
			//start set Output
			output.internalPort = httpFindValueFromArg(jsonString,
					"internalPort");
			jsonString = strstr(jsonString, "internalPort");
			os_printf("status of internalPort: %d\n",
					station.lampList[i]->output->internalPort);
			//set status (output)
			output.status = httpFindValueFromArg(jsonString, "status");
			jsonString = strstr(jsonString, "status");
			os_printf("Status output: %d\n",
					station.lampList[i]->output->status);

			os_printf("JsonString middle: %s\n", jsonString);
			station.numberOfLamp=i;
		}

	}
	return HTTPD_CGI_DONE;
}*/

//Cgi that turns the LED on or off according to the 'led' param in the POST data
int ICACHE_FLASH_ATTR postLed(HttpdConnData *connData) {
	if (connData->conn == NULL) {
		//Connection aborted. Clean up.
		return HTTPD_CGI_DONE;
	}

	if (connData->requestType == 2) { //POST
		prepareToSetLed(connData);
	}
	return HTTPD_CGI_DONE;
}

//not needed
/*
int ICACHE_FLASH_ATTR getLed(HttpdConnData *connData) {
	char buff[1024];

	if (connData->conn == NULL) {
		//Connection aborted. Clean up.
		return HTTPD_CGI_DONE;
	}

	if (connData->requestType == 1) { //POST
		int led;
		os_printf("getled\n");
		led = httpFindValueFromArg(connData->url, "led");
		uint32 ledStatus = getLedStatus(led);
		char buffer[50];

		int size = os_sprintf(buffer, "{\"id\": %lu, \"status\": %d}", led,
				(unsigned long) ledStatus);
		os_printf("{\"id\": %lu, \"status\": %d}", led,
				(unsigned long) ledStatus);
		os_printf(buffer);

		httpdStartResponse(connData, 200);
		httpdHeader(connData, "Content-Type",
				"application/json; charset=ISO-8859-1\r\n");
		httpdEndHeaders(connData);
		httpdSend(connData, buffer, size);

	}

	return HTTPD_CGI_DONE;
}*/

int ICACHE_FLASH_ATTR getStationInputs(HttpdConnData *connData) {

	if (connData->conn == NULL || connData->requestType != 1) {
		return HTTPD_CGI_DONE;
	}

	char buffer[1024];
	os_sprintf(buffer, "{\"ip\": \"%s\"", ip);
	os_sprintf(buffer, "%s ,\"floor\": %d", buffer, 2);

	if ((sizeof(inputPorts) / sizeof(int)) > 0) {
		os_sprintf(buffer, "%s ,\"lampList\": [", buffer);
	}

	for (int i = 0; i < (sizeof(inputPorts) / sizeof(int)); i++) {
		if (i != 0) {
			os_sprintf(buffer, "%s ,", buffer);
		}
		os_sprintf(buffer, "%s {\"internalId\": %d ", buffer, i);
		os_sprintf(buffer,
				"%s ,\"input\": {\"internalPort\": %d , \"status\": %d}",
				buffer, inputPorts[i], getLedStatus(i));
		os_sprintf(buffer, "%s } ", buffer, i);
	}

	if ((sizeof(inputPorts) / sizeof(int)) > 0) {
		os_sprintf(buffer, "%s ]", buffer);
	}
	int size = os_sprintf(buffer, "%s}", buffer);

	httpdStartResponse(connData, 200);
	httpdHeader(connData, "Content-Type",
			"application/json; charset=ISO-8859-1\r\n");
	httpdEndHeaders(connData);
	httpdSend(connData, buffer, size);
	return HTTPD_CGI_DONE;
}

//Template code for the led page.
int ICACHE_FLASH_ATTR tplLed(HttpdConnData *connData, char *token, void **arg) {
	char buff[128];
	if (token == NULL)
		return HTTPD_CGI_DONE;

	os_strcpy(buff, "Unknown");
	if (os_strcmp(token, "ledstate") == 0) {
		if (currLedState) {
			os_strcpy(buff, "on");
		} else {
			os_strcpy(buff, "off");
		}
	}
	httpdSend(connData, buff, -1);
	return HTTPD_CGI_DONE;
}

static long hitCounter = 0;

//Template code for the counter on the index page.
int ICACHE_FLASH_ATTR tplCounter(HttpdConnData *connData, char *token,
		void **arg) {
	char buff[128];
	if (token == NULL)
		return HTTPD_CGI_DONE;

	if (os_strcmp(token, "counter") == 0) {
		hitCounter++;
		os_sprintf(buff, "%ld", hitCounter);
	}
	httpdSend(connData, buff, -1);
	return HTTPD_CGI_DONE;
}

//Cgi that reads the SPI flash. Assumes 512KByte flash.
int ICACHE_FLASH_ATTR cgiReadFlash(HttpdConnData *connData) {
	int *pos = (int *) &connData->cgiData;
	if (connData->conn == NULL) {
		//Connection aborted. Clean up.
		return HTTPD_CGI_DONE;
	}

	if (*pos == 0) {
		os_printf("Start flash download.\n");
		httpdStartResponse(connData, 200);
		httpdHeader(connData, "Content-Type", "application/bin");
		httpdEndHeaders(connData);
		*pos = 0x40200000;
		return HTTPD_CGI_MORE;
	}
//Send 1K of flash per call. We will get called again if we haven't sent 512K yet.
	espconn_sent(connData->conn, (uint8 *) (*pos), 1024);
	*pos += 1024;
	if (*pos >= 0x40200000 + (512 * 1024))
		return HTTPD_CGI_DONE;
	else
		return HTTPD_CGI_MORE;
}

int ICACHE_FLASH_ATTR cgiUploadEspfs(HttpdConnData *connData) {
	if (connData->conn == NULL) {
		//Connection aborted. Clean up.
		return HTTPD_CGI_DONE;
	}
	SpiFlashOpResult ret;
	int x;
	int flashOff = ESPFS_POS;
	int flashSize = ESPFS_SIZE;

//If this is the first time, erase the flash sector
	if (connData->post->received == 0) {
		os_printf("Erasing flash at 0x%x...\n", flashOff);
		// Which segment are we flashing?	
		for (x = 0; x < flashSize; x += 4096) {
			spi_flash_erase_sector((flashOff + x) / 0x1000);
		}
		os_printf("Done erasing.\n");
	}

// The source should be 4byte aligned, so go ahead an flash whatever we have
	ret = spi_flash_write((flashOff + connData->post->received),
			(uint32 *) connData->post->buff, connData->post->buffLen);
	os_printf("Flash return %d\n", ret);

// Count bytes for data
	connData->post->received += connData->post->buffSize; //connData->postBuff);
	os_printf("Wrote %d bytes (%dB of %d)\n", connData->post->buffSize,
			connData->post->received, connData->post->len); //&connData->postBuff));

	if (connData->post->received == connData->post->len) {
		httpdSend(connData, "Finished uploading", -1);
		return HTTPD_CGI_DONE;
	} else {
		return HTTPD_CGI_MORE;
	}
}
