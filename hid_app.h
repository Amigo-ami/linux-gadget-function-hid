 #ifndef _hid_app_H_
    #define _hid_app_H_ 
       
   #define VID 0x2207
   #define PID 0x0019
      
   #define Report_Size   0x03
   #define REPORT_ID     0X01
      
   #define VOLUME_UP     0x01  //0xE9    // 音量加
   #define VOLUME_DOWN   0x02  //0xEA    // 音量减
   #define PLAY_PAUSE    0x04  //0xCD    // 播放/暂停
   #define NEXT_TRACK    0x08  //0xB5    // 下一曲
   #define PREV_TRACK    0x10  //0xB6    // 上一曲
   #define STOPS         0x20  //0xB7    // 停止 
   #define PHONE_MUTE    0x40  //0xE2    // 静音   
   //#define FAST_FORWARD  0x80  //0xB3    // 快进
  // #define REWIND        0x80  //0xB4    // 回退
   #define  MIC_MUTE     0x80  
   #define  LOOSEN       0x00    // 松开
       
   #define Volume_Increment 0x73
   #define Volume_Decrement 0x72
   #define Play             0xCF
   #define Mute             0xF8
   #define Stop             0x75
     
struct KEY_STATE
{
	char *key_name;
	int  key_value;
	int  key_press_flag;
};

extern int send_report(unsigned char *report_buf, int report_length);

extern int open_hiddevice(void);
extern void close_hiddevice(void);

#endif


