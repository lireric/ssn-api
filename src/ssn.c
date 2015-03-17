/*
 ============================================================================
 Name        : ssn.c
 Author      : Eric V.
 Version     :
 Copyright   : Copyright Eric V.
 Description : Interface to MC application SSN2
 ============================================================================
 */

#include <stdlib.h>
#include <stdio.h>   /* Стандартные объявления ввода/вывода */
#include <string.h>  /* Объявления строковых функций */
#include <unistd.h>  /* Объявления стандартных функций UNIX */
#include <fcntl.h>   /* Объявления управления файлами */
#include <errno.h>   /* Объявления кодов ошибок */
#include <termios.h> /* Объявления управления POSIX-терминалом */
#include <sys/types.h>
#include <sys/stat.h>
#include "cJSON.h"
#include "crc.h"


#define MAX_BUF_SIZE 40000
#define JSON_BUF_SIZE 1024
#define JSON_SETTINGS_BUF_SIZE 1024
#define JSON_SETTINGS_FILE_NAME "settings.json"
#define MESSAGE_TYPE_JSON 2


//#define JSON_SETTINGS_FILE_NAME "/media/data1/eclipse/workspace/ssn-api/Debug/settings.json"
//#define JSON_FILE_NAME "/media/data1/eclipse/workspace/ssn-api/Debug/getdevvals.json"
#define JSON_END_PATTERN "}]}}}"

int main(int argc, char **argv) {

	int fd; /* file descriptor for COM port */
	int fd_json_file; /* file descriptor for json file */
	int fd_json_settings_file; /* file descriptor for json settings file */
	char buf[MAX_BUF_SIZE];
	char buf_json2[JSON_BUF_SIZE];
	char buf_json[JSON_BUF_SIZE];
//	char buf_settings_json[JSON_SETTINGS_BUF_SIZE];
	const char json_end_pattern[] = {JSON_END_PATTERN};
	int iOut;
	int iIn;
	int retry_count = 500;
	int json_pattern_cnt;
	char* json_file_name;
	char* com_port_name;
	unsigned int com_port_speed;
	unsigned int  baud_rate = B57600;
	int obj_dst;
	int obj_src;
	int crc = 0;

	  if (argc < 1) {
		  printf("%u", 0);
		  exit(1);
	  }

	const char* buf_settings_json = argv[1];

// load settings
/*
	fd_json_settings_file = open(JSON_SETTINGS_FILE_NAME, O_RDONLY);
    if (fd_json_settings_file == -1)
      {
         printf("error open JSON file: %s\n", JSON_SETTINGS_FILE_NAME);
         perror("open_port: Unable to open JSON settings FILE");
     	return EXIT_FAILURE;
      }
	  iIn=read(fd_json_settings_file, &buf_settings_json, JSON_SETTINGS_BUF_SIZE);
*/

		cJSON *json_root, *json_ssn;
		json_root=cJSON_Parse(buf_settings_json);
		if (!json_root) {
			printf("\n\rError before: [%s]\n\r",cJSON_GetErrorPtr());
		}
		else
		{
			json_ssn=cJSON_GetObjectItem(json_root,"ssn");
			if (!json_ssn) {
				printf( "\n\rJSON error before: [%s]\n\r",cJSON_GetErrorPtr());
			} else {
// now processing settings:
				json_file_name = (char*)cJSON_GetObjectItem(json_ssn,"json_file_name")->valuestring;
				com_port_name = (char*)cJSON_GetObjectItem(json_ssn,"com_port_name")->valuestring;
				com_port_speed = cJSON_GetObjectItem(json_ssn,"com_port_speed")->valueint;
				obj_dst  = cJSON_GetObjectItem(json_ssn,"obj_dst")->valueint;
				obj_src  = cJSON_GetObjectItem(json_ssn,"obj_src")->valueint;

				switch (com_port_speed) {
				case 1200:
					baud_rate = B1200;
					break;
				case 2400:
					baud_rate = B2400;
					break;
				case 4800:
					baud_rate = B4800;
					break;
				case 9600:
					baud_rate = B9600;
					break;
				case 19200:
					baud_rate = B19200;
					break;
				case 38400:
					baud_rate = B38400;
					break;
				case 57600:
					baud_rate = B57600;
					break;
				case 115200:
					baud_rate = B115200;
					break;
				case 230400:
					baud_rate = B230400;
					break;
				case 460800:
					baud_rate = B460800;
					break;
				}
			}
		}

// load JSON command file
	fd_json_file = open(json_file_name, O_RDONLY);
    if (fd_json_file == -1)
      {
         printf("error open JSON file: %s\n", json_file_name);
         perror("open_port: Unable to open JSON FILE");
     	return EXIT_FAILURE;
      }


//    buf_json[0]='j'; // "j" - command MC to receive JSON FILE
	  iIn=read(fd_json_file, &buf_json, JSON_BUF_SIZE); /* read file with command to MC*/
	    buf_json[iIn+1]=0; // EOF

	    crc = crc16((uint8_t*) buf_json, strlen(buf_json));
	    sprintf (buf_json2, "===ssn1%04x%04x%02x%04x%s%04x", obj_dst, obj_src, MESSAGE_TYPE_JSON, strlen(buf_json), buf_json, crc);

//  printf("JSON: %s", buf_json);

	    fd = open(com_port_name, O_RDWR | O_NOCTTY | O_NDELAY | O_NONBLOCK); /*'open_port()' - Открывает последовательный порт */
	      if (fd == -1)
	        {
	           printf("error port\n");
	           perror("open_port: Unable to open /dev/ttyUSBn - ");
	        }
	     else
	        {
	         struct termios options;
	         tcgetattr(fd, &options); /* read current port settings*/

	         cfsetispeed(&options, baud_rate); /* set input speed */
	         cfsetospeed(&options, baud_rate); /* set output speed */

	         options.c_cflag &= ~PARENB; /* parity off */
	         options.c_cflag &= ~CSTOPB; /* set 1 stopbit */
	         options.c_cflag &= ~CSIZE; /* bitmask off */
	         options.c_cflag |= CS8; /* set 8 bit */
	         tcsetattr(fd, TCSANOW, &options); /*  save new port settings */

	        }
	      if (iIn > 0) {
	    	  iOut = write(fd, &buf_json2, strlen(buf_json2));
	    	  if (iOut <= 0) {
		           printf("Error send json command\n");
		        	return EXIT_FAILURE;
	    	  }
//	    	  printf("wrote bytes:%d", iOut);
	      }

	  unsigned int counter = 0;
	  json_pattern_cnt = 0;
	  unsigned int step = 0;

	  do {
	  iIn = read(fd, &buf[counter], 1);
//	  if (buf[counter] == 0) { break; }
      if (iIn >= 0) {
    	  if (buf[counter] == '{' && (counter == 0)) { step = 1; }
// check end of json response
    	  if (buf[counter] == json_end_pattern[json_pattern_cnt]) {
    		  if ((sizeof(json_end_pattern)-2) == json_pattern_cnt)  {
    			  break;
    		  }
    		  json_pattern_cnt++;
    	  } else json_pattern_cnt = 0;

    	  counter += step;
      }
      else {
          usleep (3000);
          retry_count--;
      }

	  } while ((iIn<0 || iIn>0) && (counter < MAX_BUF_SIZE) && (retry_count > 0));
	  buf[counter+1] = 0;

	  printf("%s\n", buf);

	return EXIT_SUCCESS;
}
