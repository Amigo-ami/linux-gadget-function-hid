#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <linux/input.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <signal.h>
#include <assert.h>

#include "include/hid_app.h"

#define KEY_INPUT "/dev/input/event1"
#define HID_DEV   "/dev/hidg0"
#define KEY_TIMEOUT_DOWN 30
#define KEY_TIMEOUT_UP 5

struct KEY_STATE key_state[6] = { 
		
	{ "Up_key", 115, 0},
	{ "Down_key", 114, 0 },
	{ "StopMic_key", 248, 0 },
	{ "Play_key", 164, 0 },
	{ "Change_key", 116, 0},
  { NULL, 0, 0 } 
  
};

static int hid_minor;

int send_report(unsigned char *report_buf, int report_length)
{ 	 
	 int ret;	 	 	 	
	 ret = write(hid_minor, report_buf, report_length);
	 if(ret <0 )
	 	{		 			
	 		printf(" Can't write to HID device \n");
	 	  close(hid_minor);
	 	}
	 		
	 return ret;
	    
}

int open_hiddevice( void )
{
	 hid_minor = open(HID_DEV, O_RDWR | O_NOCTTY); // O_WRONLY
 	 if(hid_minor < 0)
		{
			 printf(" Can't open HID device \n");
			 return -1 ;	 
		}	
			
	return hid_minor ;
	
}


void close_hiddevice(void)
{
	  if(hid_minor != NULL)
	  	{
	  		 close(hid_minor);
	  	}
	 return ;
}



int key_event_read(int fd, struct input_event *buf)
{
    int ret = 0;
    ret = read(fd, buf, sizeof(*buf));
    if (ret < 0) {
        if ((errno != EINTR) && (errno != EAGAIN))
            return 0;
            
       return -1;
    } 
    
    if(buf->type)
        return 1;
        
    return 0;
}

/*
static input_exit(int sign_no)
{
	
	 printf(" function : %s finish \n", __func__ );
	 exit(0);

}  
*/

static unsigned char get_keycode(int fd, fd_set *rdfds, int *times)
{
	int ret;
	struct input_event key_event ;
	struct timeval timeout ;
	unsigned char keycode = 0x00;
	
	FD_ZERO( rdfds );
	timeout.tv_sec = *times ;
	timeout.tv_usec = 0 ;
	FD_SET(fd,rdfds);
	ret = select(fd+1, rdfds, NULL, NULL, &timeout);
	if(ret > 0)
		{
			 if(FD_ISSET(fd, rdfds))
			 	{
			 		 ret = key_event_read(fd, &key_event);
			 		 if(ret > 0)
			 		 	{
			 		 		 if(key_event.value)
			 		 		 	{
			 		 		 		 *times = KEY_TIMEOUT_UP  ;
			 		 		 	}
			 		 		 	else
			 		 		 	{
			 		 		 		 *times = KEY_TIMEOUT_DOWN;
			 		 		 		 
			 		 		 		 keycode = (unsigned char )key_event.code;
			 		 		 		 
			 		 		 		 /*
			 		 		 		 		 		 		 		 
			 		 		 		 switch( key_event.code )
			 		 		 		  {
			 		 		 		  	 case 114 :
			 		 		 		  	 	 {
			 		 		 		  	 	 	 keycode = 0x01; 
			 		 		 		  	 	 }
			 		 		 		  	 	break ;
			 		 		 		  	 case 115:
			 		 		 		  	 	 {
			 		 		 		  	 	 	 keycode = 0x02 ;
			 		 		 		  	 	 }
			 		 		 		  	 	break;
			 		 		 		  	 case 116:
			 		 		 		  	   {
			 		 		 		  	   	 keycode = 0x03 ;
			 		 		 		  	   }
			 		 		 		  	  break ;
			 		 		 		  	 case 164 :
			 		 		 		  	 	 {
			 		 		 		  	 	 	  keycode = 0x04;
			 		 		 		  	 	 }
			 		 		 		  	 	break;
			 		 		 		  	 case 248:
			 		 		 		  	 	 { 
			 		 		 		  	 	 	  keycode = 0x05;
			 		 		 		  	 	 }
			 		 		 		  	 	break ;
			 		 		 		  	 default 
			 		 		 		  	  {
			 		 		 		  	  	
			 		 		 		  	  }		
			 		 		 		  	  break ;	 		 		 		  	  
			 		 		 		  }
			 		 		 		  
			 		 		 		 */
			 		 		 	}
			 		 	}
			 	}
		}
	 else if( !ret )
		{
		   keycode = 0;					
		}
					
	 return keycode;  
}

/*
int main(int argc, char *argv[])
{
	static int fd, ret, hidf;
	int times = KEY_TIMEOUT_DOWN;
	fd_set rdfds ;
	unsigned char keycode = 0x00;
	hid_device *handle;
	unsigned char report_buf[3]; ;
	
	signal( SIGTERM, input_exit  );
	signal( SIGINT,  input_exit  );
	
	report_buf[0] = 0x01;
	report_buf[2] = 0x00;
	

	ret = hid_init();
	if( ret != 0 )
		{
			printf("Can't Initialize HID Library \n");
			return -1 ;
		}
		
	handle = hid_open(VID, PID, NULL);
	if(!handle)
		{
			printf(" Can't open HID device \n");
			goto EXIT ;
		}

	 
  hidf = open(HID_DEV, O_RDWR | O_NOCTTY); // O_WRONLY
 	if(hidf < 0)
		{
			 printf(" Can't open HID device \n");
			 goto EXIT ;	 
		}
			 
	fd = open(KEY_INPUT, O_RDONLY | O_NOCTTY );
	if(fd < 0)
		{  
			 printf(" Can't open INPUT_EVENT1 device \n");
			 goto EXIT1 ;	 
		}			
	while(1)
	{
 		 keycode = get_keycode(fd, &rdfds, &times );
 		 switch(keycode)
 		 {
 		 	  case Volume_Increment:
 		 	  	{
 		 	  		 printf("VOL + \n");
 		 	  		 report_buf[1] = VOLUME_UP;
 		 	  		 ret = send_report(hidf, report_buf, Report_Size);
 		 	  		 if( ret <0)	
 		 	  		 	{
 		 	  		 		 goto EXIT2;
 		 	  		 	}	 	  		 
 		 	  	}
 		 	   break ;
 		 	  case Volume_Decrement:
 		 	  	{
 		 	  		printf("VOL - \n");
 		 	  		report_buf[1] = VOLUME_DOWN;
 		 	  		ret = send_report(hidf, report_buf, Report_Size);
 		 	  		 if( ret <0)	
 		 	  		 	{
 		 	  		 		 goto EXIT2;
 		 	  		 	}	 		 	  		
 		 	  	}
 		 	   break ;
 		 	  case Play:
 		 	  	{
 		 	  		 printf("Play/Pause \n");
 		 	  		 report_buf[1] = PLAY_PAUSE;
 		 	  		 ret = send_report(hidf, report_buf, Report_Size);
 		 	  		 if( ret <0)	
 		 	  		 	{
 		 	  		 		 goto EXIT2;
 		 	  		 	}	 		 	  		 
 		 	  	}
 		 	   break ; 	
 		 	  case Mute:
 		 	  	{
 		 	  		printf(" Mute \n");
 		 	  		report_buf[1] = PHONE_MUTE;
 		 	  		ret = send_report(hidf, report_buf, Report_Size);
 		 	  		 if( ret <0)	
 		 	  		 	{
 		 	  		 		 goto EXIT2;
 		 	  		 	}	 		 	  		
 		 	  	}
 		 	   break ; 
 		 	  case Stop:
 		 	  	{
 		 	  		 printf(" Stop\n ");
 		 	  		 report_buf[1] = STOPS;
 		 	  		 ret = send_report(hidf, report_buf, Report_Size);
 		 	  		 if( ret <0)	
 		 	  		 	{
 		 	  		 		 goto EXIT2;
 		 	  		 	}	 		 	  		 
 		 	  	}
 		 	   break ; 	
 		 	  default:
 		 	  	{
 		 	  		 printf(" losing %x \n", keycode);
 		 	  		 report_buf[1] = LOOSEN;
 		 	  		 ret = send_report(hidf, report_buf, Report_Size);
 		 	  		 if( ret <0)	
 		 	  		 	{
 		 	  		 		 goto EXIT2;
 		 	  		 	}	 		 	  		 
 		 	  	}	 
 		 	  break ; 	  	   		 	   	 	    		 	   
 		 }
 		 
	}	
	
 EXIT2:	
   close(fd);
 EXIT1 :
   close(hidf);
 EXIT :
	 sleep( 1 );
	printf(" **************** finish %s *************\n", __func__ );
	 
} */

