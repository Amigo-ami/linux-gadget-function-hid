#include <dlfcn.h>
#include <pthread.h>
#include <math.h>
#include <string.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <assert.h>

#include "include/keyuart.h"
#include "include/uart_drv.h"
#include "include/visca.h"
#include "include/hid_app.h"
#include "include/Common.h"
#include "include/led.h"

static int signal_Exit(int sign_no)
{
	printf("=================function :%s finished =====================\n", __func__);
	exit(0);
}

void pthread_uart1( int *undata )
{
	
	  //printf(" start pthread  \n");
	  do{ 
	  		
	  	 read_comport();  	 
	  	 usleep(1000 * 100);
	  	 
	  } while(1);
	   
  usleep(1000 * 10 );
	pthread_exit(NULL);
	return ;  

}

void pthread_hidkey( int *undata )
{
	int i, ret, fd; 
	unsigned char buf[6] ;
	static unsigned char hidbuff[3] = { 0x01, 0x00, 0x00};
	unsigned char len ;
	static unsigned char mic_mutestatus = 0, mode = 0;
	struct uac_status *hid_status ; 
  hid_status = get_uac_status();
  unsigned char last_hidstatus ;
  fd_set fds ;
     
	do{
		
		  usleep(1000 * 50);
		  if(hid_status->connected)
		  {
		    if(last_hidstatus != hid_status->connected )
		 			{
		 				 fd = open_hiddevice();
		 				 if(fd<0){
		 				 	  goto EXIT;
		 				 }
		 				 
		 				 last_hidstatus = hid_status->connected;
		 			}	
		  }
		 else
		 	{
		 		if(last_hidstatus != hid_status->connected )
		 			{
		 				 close_hiddevice();
		 				 last_hidstatus = hid_status->connected;
		 			}	
		 	} 	
			if(uart_drv_chk_rcv()){
							
		     ret = uart_drv_read((void *)buf, 6, &len );		 
		     if(ret)
		       goto EXIT ;         
		  }    
		 else 
		 	{		
		 				 		 		 		
		 	  continue;
		 	  	  		 		
		 	}
		 	 		 	 	
		 if(last_hidstatus)
		 	{							
		 		if( len == 6 )
		 			{
		 		 		if(buf[0] == 0x71 && buf[len-1] == 0xFF )
		 		  		{ 
		 		  			            	 		  		 		 		  	 	
		 		  	 		if( buf[3] == MIC_MUTE)
		 		     			{
		 		     	   		mic_mutestatus = ~mic_mutestatus ;	
		 		     	   		set_micledstatus(mic_mutestatus);
		 		     	   		continue ;
		 		     			}				     				 		     			 		     					 		     			 		     		 		     		 		     		 		     			 		     	     	
		 		  	 		hidbuff[1] = buf[3];
		 		  	 		printf( " keyuart runing hi !!! \n");
		 		  	 			 		  	 			 		  		  	 				 		  	 				 		  		
		 		  		  FD_ZERO(&fds);
	 	            FD_SET(fd, &fds);
	 	            ret = select(fd+1, NULL, &fds, NULL, NULL);
	 	            if(ret > 0)
	 	    	        {
	 	    		   			if(FD_ISSET(fd, &fds))
	 	    		 		 	  	{ 		  			 		  	 				 		  	 	 		  	   	 	 		  	 		  	 	   	 		  	 	   		 		  	 	  	 	  	  	 	  		 		  	 	  
		 		    	  				 ret = send_report(hidbuff, 3);
		 		     						 if(ret < 0)
		 		     							{
		 		     	 							goto EXIT;
		 		     				  		}
		 		     				  }	 		     				  
		 		     		 	}
		 		     		 else 
		 		     		 	{
		 		     		 		printf( "select device failed :%s \n", __func__ );
	 	    		        goto EXIT ; 
		 		     		 	}
		 		     		 			     		 		    		 		        		          	   		 		     	
		 		  		} 		  
		 			}
		 					
		 			printf("********* last_hidstatus:%d array[3]:%d \n", hid_status->connected, buf[3]);
		 	}
		 		 		   
	} while(1);
	
EXIT:
	
  printf( " pthread key_uart exit \n");
  usleep(1000 * 10);	
  pthread_exit(NULL); 
  return ;
}