#include "DSP2833x_Device.h"     // DSP28 Headerfile Include File
#include "DSP2833x_Examples.h"   // DSP28 Examples Include File
#include "uart_fpga.h"
#include "MotorCanCtrl.h"



static flying_parameter_s  flying_parameter;
static waypoint_info_s waypoint_info;
static global_flag_s global_flag;
static uart_fpga_rx_frame_info frame_info={0,0};


flying_parameter_s *get_flying_parameter()
{
	return &flying_parameter;
}

waypoint_info_s *get_waypoint_info()
{
	return &waypoint_info;
}
global_flag_s *get_global_flag()
{
	return &global_flag;
}


Uint16 crc_check(const void *pFrame, Uint16 bufferSize)
{
	const unsigned char *pBuffer = (const unsigned char*)pFrame;
	Uint16 poly = 0xA001;
	Uint16 crc = 0xffff;
	unsigned char carry;
	unsigned char i_bits;
	Uint16 j;

	for (j=0 ; j < bufferSize ; j++)
	{
		crc = crc ^ pBuffer[j] ;
		for (i_bits=0 ; i_bits < 8 ; i_bits++)
		{
			carry = crc & 1 ;
			crc = crc / 2 ;
			if (carry)
			{
				crc = crc & 0x7FFF;
				crc = crc ^ poly;
			}
			else
			    crc = crc & 0x7FFF;
		}
	}

	return crc;
}

unsigned char frame_check(unsigned char *msg,unsigned char length)
{  
	unsigned char parity;
	
	parity = crc_check(msg,length);
	if(parity!= msg[length-1])
	   //parity check err
	   return FAIL;
	if((msg[CTRL_FRAME_TYPE_MASK-1]!=msg[CTRL_FRAME_TYPE_MASK+1])||(msg[CTRL_FRAME_TYPE_MASK]!=msg[CTRL_FRAME_TYPE_MASK+2]))
	   //command type err
	   return FAIL;
	return SUCCESS;   	
}

memmove(unsigned char *dest,unsigned char *source,unsigned char length)
{
	int i;
	unsigned char buf[128];
	for(i=0;i<length;i++)
	{
	  	buf[i] = source[i];
	}
	for(i=0;i<length;i++)
	{
	  	dest[i] = buf[i];
	}
	
}	

void scib_fifo_init()
{
    ScibRegs.SCIFFTX.all=0xE040;
    ScibRegs.SCIFFRX.all=0x204f;
    ScibRegs.SCIFFCT.all=0x0;

}


// Transmit a character from the SCI
void scib_xmit(int a)
{
    while (ScibRegs.SCIFFTX.bit.TXFFST != 0) {}
    ScibRegs.SCITXBUF=a;

}


//  8-bit word, baud rate 0x000F, default, 1 STOP bit, no parity
void scib_echoback_init()
{
    // Note: Clocks were turned on to the Scib peripheral
    // in the InitSysCtrl() function

 	ScibRegs.SCICCR.all =0x0007;   // 1 stop bit,  No loopback
                                   // No parity,8 char bits,
                                   // async mode, idle-line protocol
	ScibRegs.SCICTL1.all =0x0003;  // enable TX, RX, internal SCICLK,
                                   // Disable RX ERR, SLEEP, TXWAKE
	ScibRegs.SCICTL2.all =0x0003;
	ScibRegs.SCICTL2.bit.TXINTENA =1;
	ScibRegs.SCICTL2.bit.RXBKINTENA =1;
	#if (CPU_FRQ_150MHZ)
	      /*
	      ScibRegs.SCIHBAUD    =0x0001;  // 9600 baud @LSPCLK = 37.5MHz.
	      ScibRegs.SCILBAUD    =0x00E7;
	      */
	      //ScibRegs.SCIHBAUD    =0x0000;  // 19200 baud @LSPCLK = 37.5MHz.
	      //ScibRegs.SCILBAUD    =0x00F3;

	      ScibRegs.SCIHBAUD    =0x0000;  // 38400 baud @LSPCLK = 37.5MHz.
	      ScibRegs.SCILBAUD    =0x0079;	      
	#endif
	#if (CPU_FRQ_100MHZ)
      ScibRegs.SCIHBAUD    =0x0001;  // 9600 baud @LSPCLK = 20MHz.
      ScibRegs.SCILBAUD    =0x0044;
	#endif
	ScibRegs.SCICTL1.all =0x0023;  // Relinquish SCI from Reset
}

void uart_fpga_init()
{
	scib_fifo_init();
	scib_echoback_init();
	
}

void uart_fpga_tx_msg(char * msg,unsigned char length)
{
    int i;
    i = 0;
    for(i=0;i<length;i++)
    {
        scib_xmit(msg[i]);
        
    }
}

unsigned char uart_fpga_frame_receive(  unsigned char * msg)
{
	  unsigned int i=0;
	  unsigned int frame_crc;
	  unsigned int frame_head_found;
    
   if(ScibRegs.SCIFFRX.bit.RXFFST >0){ // new byte arrived
       // Get character
        msg[frame_info.bytes_received] = ScibRegs.SCIRXBUF.all;  

 	    frame_info.bytes_received ++;

	 	while(frame_info.bytes_received > 0){
	 	   // start searching frame head if at least CTRL_FRAME_HEAD_LENGTH has been received
	 	   if(frame_info.bytes_received >= CTRL_FRAME_HEAD_LENGTH){
	 	        for(i=0;i<frame_info.bytes_received-1;i++){
	 	             if((msg[i]==CTRL_FRAME_HEAD1)&&(msg[i+1]==CTRL_FRAME_HEAD2)&&
	 	                (msg[i+2]==CTRL_FRAME_HEAD3)&&(msg[i+3]==CTRL_FRAME_HEAD4)&&
	 	                (msg[i+4]==CTRL_FRAME_HEAD5)&&(msg[i+5]==CTRL_FRAME_HEAD6)){
	 	                //found the frame head ,remove useless received bytes before the beginning of the frame
	 	                  if(i>0){
	 	                       memmove(msg,msg+i,frame_info.bytes_received-i);
	 	                       frame_info.bytes_received=frame_info.bytes_received-i;
	 	                  }
	 	                       frame_head_found = 1;
	 	                       break;
	 	              }
	 	         }
	 	    }else{
	 	    	// not enough data
	 	    	return 0;
	 	    }
	
	 	  if(frame_head_found){
	            // frame head has been found ,start extract frame if we have received minimum bytes of a frame
	 	        if(frame_info.bytes_received < CTRL_FRAME_MINIMUM_LEN_RX){
	 	        	//do not have enough data for a valid frame
	 	        	//print_debug("ctrl :do not have enough data for a valid frame\n");
	 	        	return 0;
	
	 	        }

	            //frame_info->frame_size = (buf[CTRL_FRAME_MASK_FRAME_SIZE+1]<<8) | buf[CTRL_FRAME_MASK_FRAME_SIZE];
	              //for fixed length frame only
	              frame_info.frame_size = CTRL_FRAME_LENGTH_RX;
	              
	            //check if frame size is valid
	            //if((frame_info->frame_size >= CTRL_FRAME_MINIMUM_LEN)&&(frame_info->frame_size <= CTRL_FRAME_MAX_LEN)){
	            if(1){//for fixed length frame only
	                 if(frame_info.frame_size > frame_info.bytes_received){
	            	    // do not have received whole frame
	                	 //print_debug("ctrl :do not have received whole frame\n");
	            	     return 0;
	                 }
	
	                 // we have received the whole frame ,so check the frame tail
	                 //if(msg[frame_info->frame_size-1]==CTRL_FRAME_END){
	
	            	     //return frame_info.frame_size;
	            	     
	            	     //frame_crc = msg[frame_info->frame_size-1] ;
	            	        frame_crc=(msg[CTRL_FRAME_LENGTH_RX-2]<<8)| msg[CTRL_FRAME_LENGTH_RX-1];    
                            if(frame_crc==crc_check(msg,CTRL_FRAME_LENGTH_RX-2)){
    	                       
                            
	            	     //if(frame_crc==check(msg,CTRL_FRAME_CHECK_LENGTH)){
	            	     //if(1){
	            		    // we have a valid CRC
	
	            		     return  frame_info.frame_size;
	            		 
	            	     
	            	     }else{
	                        // invalid CRC ,remove the whole frame from the buffer
	
	            		    memmove(msg,msg+frame_info.frame_size,frame_info.bytes_received-frame_info.frame_size);
	            	    	frame_info.bytes_received=frame_info.bytes_received-frame_info.frame_size;
	            	    	frame_info.frame_size = 0;
	            		    frame_head_found = 0;
	            		    return 1;

	            		    //print_debug("ctrl :invalid crc\n");

	            	     }
	                 /*
	                 }else{
	            	     //frame tail not found ,so the frame is invalid,
	            	     //we should have incorrectly detected a start of frame
	                     //remove the  frame head bytes and start searching frame head again
	                	// print_debug("ctrl :frame tail not found ,so the frame is invalid\n");
	            	     memmove(msg,msg+CTRL_FRAME_HEAD_LENGTH,frame_info->bytes_received-CTRL_FRAME_HEAD_LENGTH);
	            	     frame_info->bytes_received=frame_info->bytes_received-CTRL_FRAME_HEAD_LENGTH;
	            	     frame_info->frame_size = 0;
	            	     frame_head_found = 0;
	            	     continue;
	                 }
	                 */
	                 
	            }else{
	                // invalid frame_size ,which means wrong frame head is detected
	                 // we need to remove the 2 wrong frame head bytes
	            	 //print_debug("ctrl :invalid frame_size \n");
	                 memmove(msg,msg+CTRL_FRAME_HEAD_LENGTH,frame_info.bytes_received-CTRL_FRAME_HEAD_LENGTH);
	                 frame_info.bytes_received=frame_info.bytes_received-CTRL_FRAME_HEAD_LENGTH;
	                 frame_info.frame_size = 0;
	                 frame_head_found = 0;
	            }
	       }else{
	            //unable to find a valid start of frame
	            //so check the last byte is FRAME_HEAD1 in order to keep it for next time
	    	    //print_debug("ctrl :invalid start of frame\n");
	            if(msg[frame_info.bytes_received-1]==CTRL_FRAME_HEAD1){
	                msg[0] = CTRL_FRAME_HEAD1;
	                frame_info.bytes_received = 1;
	            }else if((msg[frame_info.bytes_received-2]==CTRL_FRAME_HEAD1)&&(msg[frame_info.bytes_received-1]==CTRL_FRAME_HEAD2)){
	                msg[0] = CTRL_FRAME_HEAD1;
	                msg[1] = CTRL_FRAME_HEAD2;
	                frame_info.bytes_received = 2;
	            }else if((msg[frame_info.bytes_received-3]==CTRL_FRAME_HEAD1)&&(msg[frame_info.bytes_received-2]==CTRL_FRAME_HEAD2)
	                      &&(msg[frame_info.bytes_received-1]==CTRL_FRAME_HEAD3)){
	                msg[0] = CTRL_FRAME_HEAD1;
	                msg[1] = CTRL_FRAME_HEAD2;
	                msg[2] = CTRL_FRAME_HEAD3;
	                frame_info.bytes_received = 3;
	            }else if((msg[frame_info.bytes_received-4]==CTRL_FRAME_HEAD1)&&(msg[frame_info.bytes_received-3]==CTRL_FRAME_HEAD2)
	                      &&(msg[frame_info.bytes_received-2]==CTRL_FRAME_HEAD3)&&(msg[frame_info.bytes_received-1]==CTRL_FRAME_HEAD4)){
	                msg[0] = CTRL_FRAME_HEAD1;
	                msg[1] = CTRL_FRAME_HEAD2;
	                msg[2] = CTRL_FRAME_HEAD3;
	                msg[3] = CTRL_FRAME_HEAD4;
	                frame_info.bytes_received = 4;
	            }else if((msg[frame_info.bytes_received-5]==CTRL_FRAME_HEAD1)&&(msg[frame_info.bytes_received-4]==CTRL_FRAME_HEAD2)
	                      &&(msg[frame_info.bytes_received-3]==CTRL_FRAME_HEAD3)&&(msg[frame_info.bytes_received-2]==CTRL_FRAME_HEAD4)
	                      &&(msg[frame_info.bytes_received-1]==CTRL_FRAME_HEAD5)){
	                msg[0] = CTRL_FRAME_HEAD1;
	                msg[1] = CTRL_FRAME_HEAD2;
	                msg[2] = CTRL_FRAME_HEAD3;
	                msg[3] = CTRL_FRAME_HEAD4;
	                msg[4] = CTRL_FRAME_HEAD5;
	                frame_info.bytes_received = 5;    
	            }else{
	                //discard the whole buffer
	                frame_info.bytes_received = 0;
	            }
	            return 0;
	        }
	
	
	 	}
	 	return 0;
   }
   return 0;
}



void send_frame(Uint8 frame_type )
{
     Uint8 msg[CTRL_FRAME_LENGTH_TX];
     struct drive_status *drive1,*drive2;
     Uint8 i;
     Uint16 crc;
     
     drive1 = get_drive_status(1);
     drive2 = get_drive_status(2);
     msg[0]  = CTRL_FRAME_HEAD1; 
     msg[1]  = CTRL_FRAME_HEAD2; 
     msg[2]  = CTRL_FRAME_HEAD3; 
     msg[3]  = CTRL_FRAME_HEAD4; 
     msg[4]  = CTRL_FRAME_HEAD5; 
     msg[5]  = CTRL_FRAME_HEAD6; 
     msg[6]  = 0;
     msg[7]  = frame_type;	

     if((frame_type==CTRL_FRAME_TYPE_MANUAL)||(frame_type==CTRL_FRAME_TYPE_IDLE)||(frame_type==CTRL_FRAME_TYPE_QUERY)){
     	msg[CTRL_FRAME_MOTOR_ANGLE1_MASK_TX-1] = (flying_parameter.heading_motor_actual_position1>>8)&0xFF;
     	msg[CTRL_FRAME_MOTOR_ANGLE1_MASK_TX]   = flying_parameter.heading_motor_actual_position1&0xFF;
     	msg[CTRL_FRAME_MOTOR_ANGLE2_MASK_TX-1] = (flying_parameter.heading_motor_actual_position2>>8)&0xFF;
     	msg[CTRL_FRAME_MOTOR_ANGLE2_MASK_TX]   = flying_parameter.heading_motor_actual_position2&0xFF;
     	for(i=CTRL_FRAME_WP_NO_MASK_TX-1;i<CTRL_FRAME_LENGTH_TX-2;i++){
     	   msg[i] = 0;	
     	}
     	msg[CTRL_FRAME_ERR_MASK_TX-1] = drive1->can_failure&0xFF;
     	msg[CTRL_FRAME_ERR_MASK_TX]   = drive2->can_failure&0xFF;
     }else if((frame_type==CTRL_FRAME_TYPE_AUTO)||(frame_type==CTRL_FRAME_TYPE_HOVER)||(frame_type==CTRL_FRAME_TYPE_UPWIND)){
     	msg[CTRL_FRAME_MOTOR_ANGLE1_MASK_TX-1] = (flying_parameter.heading_motor_actual_position1>>8)&0xFF;
     	msg[CTRL_FRAME_MOTOR_ANGLE1_MASK_TX]   = flying_parameter.heading_motor_actual_position1&0xFF;
     	msg[CTRL_FRAME_MOTOR_ANGLE2_MASK_TX-1] = (flying_parameter.heading_motor_actual_position2>>8)&0xFF;
     	msg[CTRL_FRAME_MOTOR_ANGLE2_MASK_TX]   = flying_parameter.heading_motor_actual_position2&0xFF;
     	msg[CTRL_FRAME_WP_NO_MASK_TX-1]       = 0;    	
       	msg[CTRL_FRAME_MOTOR_SP1_MASK_TX-1] = (flying_parameter.propell_motor_target_speed1>>8)&0xFF;
     	msg[CTRL_FRAME_MOTOR_SP1_MASK_TX]   = flying_parameter.propell_motor_target_speed1&0xFF;  
     	msg[CTRL_FRAME_MOTOR_SP2_MASK_TX-1] = (flying_parameter.propell_motor_target_speed2>>8)&0xFF;
     	msg[CTRL_FRAME_MOTOR_SP2_MASK_TX]   = flying_parameter.propell_motor_target_speed2&0xFF;
     	msg[CTRL_FRAME_MOTOR_SP3_MASK_TX-1] = (flying_parameter.propell_motor_target_speed3>>8)&0xFF;
     	msg[CTRL_FRAME_MOTOR_SP3_MASK_TX]   = flying_parameter.propell_motor_target_speed3&0xFF;
     	msg[CTRL_FRAME_MOTOR_SP4_MASK_TX-1] = (flying_parameter.propell_motor_target_speed4>>8)&0xFF;
     	msg[CTRL_FRAME_MOTOR_SP4_MASK_TX]   =  flying_parameter.propell_motor_target_speed4&0xFF; 
     	msg[CTRL_FRAME_ERR_MASK_TX-1] = drive1->can_failure&0xFF;
     	msg[CTRL_FRAME_ERR_MASK_TX]   = drive2->can_failure&0xFF;	
     }else if(frame_type==CTRL_FRAME_TYPE_WP){
     	msg[CTRL_FRAME_MOTOR_ANGLE1_MASK_TX-1] = (flying_parameter.heading_motor_actual_position1>>8)&0xFF;
     	msg[CTRL_FRAME_MOTOR_ANGLE1_MASK_TX]   = flying_parameter.heading_motor_actual_position1&0xFF;
     	msg[CTRL_FRAME_MOTOR_ANGLE2_MASK_TX-1] = (flying_parameter.heading_motor_actual_position2>>8)&0xFF;
     	msg[CTRL_FRAME_MOTOR_ANGLE2_MASK_TX]   = flying_parameter.heading_motor_actual_position2&0xFF;
     	msg[CTRL_FRAME_WP_NO_MASK_TX-1]       = waypoint_info.wp_no_received+1; 
     	for(i=CTRL_FRAME_MOTOR_SP1_MASK_TX-1;i<CTRL_FRAME_LENGTH_TX-2;i++){
     	   msg[i] = 0;	
     	}     	
     }

     //crc = crc_check(msg+CTRL_FRAME_HEAD_LENGTH,CTRL_FRAME_CHECK_LENGTH_TX);
     crc = crc_check(msg,CTRL_FRAME_LENGTH_TX-2);
     msg[CTRL_FRAME_LENGTH_TX-2] =(crc>>8) &0xff;
     msg[CTRL_FRAME_LENGTH_TX-1] =  crc &0xff;
 
     uart_fpga_tx_msg((char *)msg,CTRL_FRAME_LENGTH_TX);	
}
Uint16 frame_parse(Uint8 *msg)
{
    Uint16 crc;
    Uint16 frame_type;
    Uint16 i;
    Uint8  flying_mode; 
    Uint8  wp_no; 
      
    frame_info.bytes_received = frame_info.bytes_received - CTRL_FRAME_LENGTH_RX;
    /*
    crc=(msg[CTRL_FRAME_LENGTH_RX-2]<<8)| msg[CTRL_FRAME_LENGTH_RX-1];    
    if(crc!=crc_check(msg+CTRL_FRAME_HEAD_LENGTH,CTRL_FRAME_CHECK_LENGTH_RX)){
    	return FAIL;
    }*/
    
    frame_type = (msg[CTRL_FRAME_TYPE_MASK-1]<<8)| msg[CTRL_FRAME_TYPE_MASK];
    flying_mode = msg[CTRL_FRAME_FLYING_MODE_MASK_RX-1];
   // wp_no = msg[CTRL_FRAME_WP_NO_MASK_RX-1];
    
    if(frame_type==CTRL_FRAME_TYPE_ERR){//FPGA received wrong frame
    	global_flag.work_mode= flying_mode;
       return FAIL;
    }
    //FPGA send correct frame,check flying_mode field,and extract data    
    switch(flying_mode){
      case FLYING_MODE_AUTO:
           if(global_flag.work_mode== WORKING_MODE_READING_WP){
               wp_no = msg[CTRL_FRAME_WP_NO_MASK_RX_AIRLINE-1];
		       if(wp_no==waypoint_info.wp_no_received+1){//received waypoint no is what we request
			       waypoint_info.waypoint[wp_no-1].longitude = msg[CTRL_FRAME_WP_LONG_MASK_RX_AIRLINE+2]| 
			                                                ((Uint32)msg[CTRL_FRAME_WP_LONG_MASK_RX_AIRLINE+1]<<8)|
			                                                ((Uint32)msg[CTRL_FRAME_WP_LONG_MASK_RX_AIRLINE]<<16)|
			                                                ((Uint32)msg[CTRL_FRAME_WP_LONG_MASK_RX_AIRLINE-1]<<24)	;
			       waypoint_info.waypoint[wp_no-1].latitude = msg[CTRL_FRAME_WP_LAT_MASK_RX_AIRLINE+2]| 
			                                                ((Uint32)msg[CTRL_FRAME_WP_LAT_MASK_RX_AIRLINE+1]<<8)|
			                                                ((Uint32)msg[CTRL_FRAME_WP_LAT_MASK_RX_AIRLINE]<<16)|
			                                                ((Uint32)msg[CTRL_FRAME_WP_LAT_MASK_RX_AIRLINE-1]<<24)	;   
			       waypoint_info.waypoint[wp_no-1].hover_time = msg[CTRL_FRAME_WP_TIME_MASK_RX_AIRLINE+1]| 
			                                                ((Uint32)msg[CTRL_FRAME_WP_TIME_MASK_RX_AIRLINE]<<8)|
			                                                ((Uint32)msg[CTRL_FRAME_WP_TIME_MASK_RX_AIRLINE-1]<<16);
			
			       waypoint_info.wp_no_received ++; 
			       if( waypoint_info.wp_no_received == waypoint_info.wp_number)//all the waypoint received,switch to auto mode
			           global_flag.work_mode = FLYING_MODE_AUTO   ; 
		       }
           }else if(global_flag.work_mode== FLYING_MODE_AUTO){                                  
		       flying_parameter.current_longitude =  msg[CTRL_FRAME_LONG_MASK_RX_AIRLINE+2]| 
		                                                ((Uint32)msg[CTRL_FRAME_LONG_MASK_RX_AIRLINE+1]<<8)|
		                                                ((Uint32)msg[CTRL_FRAME_LONG_MASK_RX_AIRLINE]<<16)|
		                                                ((Uint32)msg[CTRL_FRAME_LONG_MASK_RX_AIRLINE-1]<<24)	; 
		       flying_parameter.current_latitude  =  msg[CTRL_FRAME_LAT_MASK_RX_AIRLINE+2]| 
		                                                ((Uint32)msg[CTRL_FRAME_LAT_MASK_RX_AIRLINE+1]<<8)|
		                                                ((Uint32)msg[CTRL_FRAME_LAT_MASK_RX_AIRLINE]<<16)|
		                                                ((Uint32)msg[CTRL_FRAME_LAT_MASK_RX_AIRLINE-1]<<24)	;
		       flying_parameter.heading           =  msg[CTRL_FRAME_HEADING_MASK_RX_AIRLINE+2]| 
		                                                ((Uint32)msg[CTRL_FRAME_HEADING_MASK_RX_AIRLINE+1]<<8)|
		                                                ((Uint32)msg[CTRL_FRAME_HEADING_MASK_RX_AIRLINE]<<16)|
		                                                ((Uint32)msg[CTRL_FRAME_HEADING_MASK_RX_AIRLINE-1]<<24)	;  
		      //flying_parameter.propell_motor_actual_speed1 =   (msg[CTRL_FRAME_MOTOR_SP1_MASK_RX_AIRLINE-1]<<8)| msg[CTRL_FRAME_MOTOR_SP1_MASK_RX];
		     // flying_parameter.propell_motor_actual_speed2 =   (msg[CTRL_FRAME_MOTOR_SP2_MASK_RX_AIRLINE-1]<<8)| msg[CTRL_FRAME_MOTOR_SP2_MASK_RX];
		      //flying_parameter.propell_motor_actual_speed3 =   (msg[CTRL_FRAME_MOTOR_SP3_MASK_RX_AIRLINE-1]<<8)| msg[CTRL_FRAME_MOTOR_SP3_MASK_RX];
		      //flying_parameter.propell_motor_actual_speed4 =   (msg[CTRL_FRAME_MOTOR_SP4_MASK_RX_AIRLINE-1]<<8)| msg[CTRL_FRAME_MOTOR_SP4_MASK_RX]; 
		      flying_parameter.pid1    =  msg[CTRL_FRAME_PID1_MASK_RX_AIRLINE+1]| 
		                                  ((Uint32)msg[CTRL_FRAME_PID1_MASK_RX_AIRLINE]<<8)|
		                                  ((Uint32)msg[CTRL_FRAME_PID1_MASK_RX_AIRLINE-1]<<16);
		      flying_parameter.pid2    =  msg[CTRL_FRAME_PID2_MASK_RX_AIRLINE+1]| 
		                                  ((Uint32)msg[CTRL_FRAME_PID2_MASK_RX_AIRLINE]<<8)|
		                                  ((Uint32)msg[CTRL_FRAME_PID2_MASK_RX_AIRLINE-1]<<16);
		      flying_parameter.pid3    =  msg[CTRL_FRAME_PID3_MASK_RX_AIRLINE+1]| 
		                                  ((Uint32)msg[CTRL_FRAME_PID3_MASK_RX_AIRLINE]<<8)|
		                                  ((Uint32)msg[CTRL_FRAME_PID3_MASK_RX_AIRLINE-1]<<16);
		      flying_parameter.pid_period    =  msg[CTRL_FRAME_PID_PERIOD_MASK_RX_AIRLINE-1];

		      flying_parameter.throttle_from_ground = msg[CTRL_FRAME_THROTTLE_MASK_RX_AIRLINE-1];		                                  		                                  		                                  
		                                   
		      //if(flying_mode == WORKING_MODE_MANUAL)
		      //   global_flag.work_mode = WORKING_MODE_MANUAL   ;
		     // else 
		     if(global_flag.airline_over)
		          global_flag.work_mode = FLYING_MODE_MANUAL   ;
		          //global_flag.work_mode = WORKING_MODE_IDLE   ;  
           } else {//waypoint ready
               global_flag.work_mode = WORKING_MODE_READING_WP   ;  
               for(i=0;i<sizeof(waypoint_info_s);i++)//clear waypoint struct
			        {
			    	  //*((Uint8*)waypoint_info+i) = 0;
			        }
			        waypoint_info.wp_no_received = 0;
			        waypoint_info.wp_number = msg[CTRL_FRAME_WP_NUM_MASK_RX_AIRLINE-1];
           }                                                                                                           
		       break;
      case FLYING_MODE_MANUAL:
               global_flag.work_mode = FLYING_MODE_MANUAL   ; 
		       flying_parameter.heading_motor_target_position1 =  (msg[CTRL_FRAME_MOTOR_POS1_MASK_RX_MANUAL-1]<<8)| msg[CTRL_FRAME_MOTOR_POS1_MASK_RX_MANUAL];
		       flying_parameter.heading_motor_target_position2 =  (msg[CTRL_FRAME_MOTOR_POS2_MASK_RX_MANUAL-1]<<8)| msg[CTRL_FRAME_MOTOR_POS2_MASK_RX_MANUAL];
		      /*
		      flying_parameter.pid1    =  msg[CTRL_FRAME_PID1_MASK_RX+1]| 
		                                  ((Uint32)msg[CTRL_FRAME_PID1_MASK_RX]<<8)|
		                                  ((Uint32)msg[CTRL_FRAME_PID1_MASK_RX-1]<<16);
		      flying_parameter.pid2    =  msg[CTRL_FRAME_PID1_MASK_RX+1]| 
		                                  ((Uint32)msg[CTRL_FRAME_PID1_MASK_RX]<<8)|
		                                  ((Uint32)msg[CTRL_FRAME_PID1_MASK_RX-1]<<16);
		      flying_parameter.pid3    =  msg[CTRL_FRAME_PID1_MASK_RX+1]| 
		                                  ((Uint32)msg[CTRL_FRAME_PID1_MASK_RX]<<8)|
		                                  ((Uint32)msg[CTRL_FRAME_PID1_MASK_RX-1]<<16);
		      flying_parameter.pid4    =  msg[CTRL_FRAME_PID1_MASK_RX+1]| 
		                                  ((Uint32)msg[CTRL_FRAME_PID1_MASK_RX]<<8)|
		                                  ((Uint32)msg[CTRL_FRAME_PID1_MASK_RX-1]<<16);	
		                                  */	       

			    /*
		       if(flying_mode == WORKING_MODE_IDLE)
		          global_flag.work_mode = WORKING_MODE_IDLE   ;
		          */
		       break;  
	  case FLYING_MODE_UPWIND:
	          global_flag.work_mode = FLYING_MODE_UPWIND   ;
	  		  flying_parameter.pid1    =  msg[CTRL_FRAME_PID1_MASK_RX_UPWIND+1]| 
		                                  ((Uint32)msg[CTRL_FRAME_PID1_MASK_RX_UPWIND]<<8)|
		                                  ((Uint32)msg[CTRL_FRAME_PID1_MASK_RX_UPWIND-1]<<16);
		      flying_parameter.pid2    =  msg[CTRL_FRAME_PID2_MASK_RX_UPWIND+1]| 
		                                  ((Uint32)msg[CTRL_FRAME_PID2_MASK_RX_UPWIND]<<8)|
		                                  ((Uint32)msg[CTRL_FRAME_PID2_MASK_RX_UPWIND-1]<<16);
		      flying_parameter.pid3    =  msg[CTRL_FRAME_PID3_MASK_RX_UPWIND+1]| 
		                                  ((Uint32)msg[CTRL_FRAME_PID3_MASK_RX_UPWIND]<<8)|
		                                  ((Uint32)msg[CTRL_FRAME_PID3_MASK_RX_UPWIND-1]<<16);
		      flying_parameter.pid_period    =  msg[CTRL_FRAME_PID_PERIOD_MASK_RX_UPWIND-1];
		      flying_parameter.wind_direction_from_ground = ((Uint32)msg[CTRL_FRAME_WIND_DIR_MASK_RX_UPWIND])|
		                                                    ((Uint32)msg[CTRL_FRAME_WIND_DIR_MASK_RX_UPWIND-1]<<8);                            
		      flying_parameter.throttle_from_ground = msg[CTRL_FRAME_THROTTLE_MASK_RX_UPWIND-1];
	  
		       break;
      case FLYING_MODE_HOVER:
	          global_flag.work_mode = FLYING_MODE_HOVER  ;
	  		  flying_parameter.pid1    =  msg[CTRL_FRAME_PID1_MASK_RX_HOVER+1]| 
		                                  ((Uint32)msg[CTRL_FRAME_PID1_MASK_RX_HOVER]<<8)|
		                                  ((Uint32)msg[CTRL_FRAME_PID1_MASK_RX_HOVER-1]<<16);
		      flying_parameter.pid2    =  msg[CTRL_FRAME_PID2_MASK_RX_HOVER+1]| 
		                                  ((Uint32)msg[CTRL_FRAME_PID2_MASK_RX_HOVER]<<8)|
		                                  ((Uint32)msg[CTRL_FRAME_PID2_MASK_RX_HOVER-1]<<16);
		      flying_parameter.pid3    =  msg[CTRL_FRAME_PID3_MASK_RX_HOVER+1]| 
		                                  ((Uint32)msg[CTRL_FRAME_PID3_MASK_RX_HOVER]<<8)|
		                                  ((Uint32)msg[CTRL_FRAME_PID3_MASK_RX_HOVER-1]<<16);
		      flying_parameter.pid_period    =  msg[CTRL_FRAME_PID_PERIOD_MASK_RX_HOVER-1];

		      flying_parameter.hover_longitude = msg[CTRL_FRAME_HOV_LONG_MASK_RX_HOVER+2]| 
		                                         ((Uint32)msg[CTRL_FRAME_HOV_LONG_MASK_RX_HOVER+1]<<8)|
		                                         ((Uint32)msg[CTRL_FRAME_HOV_LONG_MASK_RX_HOVER]<<16)|
		                                         ((Uint32)msg[CTRL_FRAME_HOV_LONG_MASK_RX_HOVER-1]<<24)	;    
		      flying_parameter.hover_latitude =  msg[CTRL_FRAME_HOV_LAT_MASK_RX_HOVER+2]| 
		                                         ((Uint32)msg[CTRL_FRAME_HOV_LAT_MASK_RX_HOVER+1]<<8)|
		                                         ((Uint32)msg[CTRL_FRAME_HOV_LAT_MASK_RX_HOVER]<<16)|
		                                         ((Uint32)msg[CTRL_FRAME_HOV_LAT_MASK_RX_HOVER-1]<<24)	;                                                                    
		      flying_parameter.throttle_from_ground = msg[CTRL_FRAME_THROTTLE_MASK_RX_HOVER-1];	  
		       break;
	/*     	                  
      case WORKING_MODE_IDLE:  //not use now 
                
			    if(frame_type==CTRL_FRAME_TYPE_WP){//waypoint ready
			        global_flag.work_mode = WORKING_MODE_READING_WP; 
			        for(i=0;i<sizeof(waypoint_info_s);i++)//clear waypoint struct
			        {
			    	  *((Uint8*)waypoint_info+i) = 0;
			        }
			        waypoint_info.wp_number = msg[CTRL_FRAME_WP_NUM_MASK_RX-1];
			       
			    } 
			    
			    if(flying_mode == FLYING_MODE_MANUAL)
		           global_flag.work_mode = FLYING_MODE_MANUAL   ;
			    break;
			    */
	  default:	global_flag.work_mode = FLYING_MODE_MANUAL;	
	            break;  
    }
	return SUCCESS  ;
    
}
