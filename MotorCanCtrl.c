#include "DSP2833x_Device.h"     // DSP28 Headerfile Include File
#include "DSP2833x_Examples.h"   // DSP28 Examples Include File
#include "MotorCanCtrl.h"


struct drive_status drive_status[2];
struct position_mode_parameter pos_mode_parameter1;
struct position_mode_parameter pos_mode_parameter2;
struct position_mode_parameter *get_motor_pos_mode_para(Uint8 id)
{
	if(id==1)
	  return &pos_mode_parameter1;
	else if(id==2)
	  return &pos_mode_parameter2;
	return 0;
}

struct drive_status *get_drive_status(Uint8 id)
{
	
	  return &drive_status[id-1];	
}

/* enable one mailbox for tx ,and one mailbox for rx */
void can_rw_with_response_init()
{
			/* Configure Mailbox  as a Transmit mailbox */
		   struct ECAN_REGS ECanaShadow;
		   ECanaShadow.CANMD.all = ECanaRegs.CANMD.all;
		   ECanaShadow.CANMD.bit.MD25 = 0;
		   ECanaRegs.CANMD.all = ECanaShadow.CANMD.all;
		/* Configure Mailbox  as a Receive mailbox */
		
		   ECanaShadow.CANMD.all = ECanaRegs.CANMD.all;
		   ECanaShadow.CANMD.bit.MD16 = 1;
		   ECanaRegs.CANMD.all = ECanaShadow.CANMD.all;
		
		/* Enable Mailbox */
		
		   ECanaShadow.CANME.all = ECanaRegs.CANME.all;
		   ECanaShadow.CANME.bit.ME25 = 1;
		   ECanaRegs.CANME.all = ECanaShadow.CANME.all;
		
		   ECanaShadow.CANME.all = ECanaRegs.CANME.all;
		   ECanaShadow.CANME.bit.ME16 = 1;
		   ECanaRegs.CANME.all = ECanaShadow.CANME.all;
}


// transmit one message and wait until response message arrived 
// if no reponse message arrived in timeout time,then return error
unsigned int can_tx_with_response(struct can_frame *tx_frame,struct can_frame *rx_frame)
{
	   struct ECAN_REGS ECanaShadow;
	   unsigned long int time_out_counter=0;
	   volatile struct MBOX *Mailbox;
	   
      /* Disnable Mailbox ,required for writing MSGID */
       ECanaShadow.CANME.all = ECanaRegs.CANME.all;
       ECanaShadow.CANME.bit.ME25 = 0;
       ECanaRegs.CANME.all = ECanaShadow.CANME.all;

       ECanaShadow.CANME.all = ECanaRegs.CANME.all;
       ECanaShadow.CANME.bit.ME16 = 0;
       ECanaRegs.CANME.all = ECanaShadow.CANME.all;
       
          
	   ECanaMboxes.MBOX25.MSGID.all=tx_frame->id;  

       ECanaMboxes.MBOX16.MSGID.all=rx_frame->id;  
       
       /* Enable Mailbox */

       ECanaShadow.CANME.all = ECanaRegs.CANME.all;
       ECanaShadow.CANME.bit.ME25 = 1;
       ECanaRegs.CANME.all = ECanaShadow.CANME.all;

       ECanaShadow.CANME.all = ECanaRegs.CANME.all;
       ECanaShadow.CANME.bit.ME16 = 1;
       ECanaRegs.CANME.all = ECanaShadow.CANME.all;
       /* Write to DLC field in Master Control reg of tx mailbox*/

       ECanaMboxes.MBOX25.MSGCTRL.bit.DLC = tx_frame->len;

       /* Write to the mailbox RAM field */

       ECanaMboxes.MBOX25.MDL.all = tx_frame->buf[3]|(tx_frame->buf[2]<<8)|((Uint32)tx_frame->buf[1]<<16)|((Uint32)tx_frame->buf[0]<<24);
       ECanaMboxes.MBOX25.MDH.all = tx_frame->buf[7]|(tx_frame->buf[6]<<8)|((Uint32)tx_frame->buf[5]<<16)|((Uint32)tx_frame->buf[4]<<24);;
      
       while(ECanaRegs.CANRMP.bit.RMP16==1) //clear receive message pending bit before transmit
       {
          //clear receive message pending bit 
          ECanaShadow.CANRMP.all = ECanaRegs.CANRMP.all;
          ECanaShadow.CANRMP.bit.RMP16 = 1;
          ECanaRegs.CANRMP.all = ECanaShadow.CANRMP.all; 
                  time_out_counter++;
          if(time_out_counter==1000000)
             return FAIL;  //no response message    
       }
 
       /* Begin transmitting */

       ECanaShadow.CANTRS.all = 0;
       ECanaShadow.CANTRS.bit.TRS25 = 1;             // Set TRS for mailbox under test
       ECanaRegs.CANTRS.all = ECanaShadow.CANTRS.all;

       do
    	{
      	ECanaShadow.CANTA.all = ECanaRegs.CANTA.all;
      	time_out_counter++;
          if(time_out_counter==1000000)
             return FAIL;  //no response message
    	} while(ECanaShadow.CANTA.bit.TA25 == 0 );   // Wait for TA5 bit to be set..

       ECanaShadow.CANTA.all = 0;
       ECanaShadow.CANTA.bit.TA25 = 1;     	         // Clear TA25
       ECanaRegs.CANTA.all = ECanaShadow.CANTA.all;

       while(ECanaRegs.CANRMP.bit.RMP16==0) //check if response message is arrived
       {
          time_out_counter++;
          if(time_out_counter==1000000)
             return FAIL;  //no response message

       }
       
       while(ECanaRegs.CANRMP.bit.RMP16==1) //if response message is arrived,clear RMP
       {
          //clear receive message pending bit 
          ECanaShadow.CANRMP.all = ECanaRegs.CANRMP.all;
          ECanaShadow.CANRMP.bit.RMP16 = 1;
          ECanaRegs.CANRMP.all = ECanaShadow.CANRMP.all; 
          time_out_counter++;
          if(time_out_counter==1000000)
             return FAIL;  //no response message    
       }
       
       Mailbox = &ECanaMboxes.MBOX0 + 16;  
       rx_frame->buf[0] = Mailbox->MDL.byte.BYTE0; 
       rx_frame->buf[1] = Mailbox->MDL.byte.BYTE1;  
       rx_frame->buf[2] = Mailbox->MDL.byte.BYTE2;
       rx_frame->buf[3] = Mailbox->MDL.byte.BYTE3;
       rx_frame->buf[4] = Mailbox->MDH.byte.BYTE4; 
       rx_frame->buf[5] = Mailbox->MDH.byte.BYTE5;  
       rx_frame->buf[6] = Mailbox->MDH.byte.BYTE6;
       rx_frame->buf[7] = Mailbox->MDH.byte.BYTE7;
       //DELAY_US(10000);
       return SUCCESS;
}

/*  CAN read data from server which is motor driver for this project*/
//  return 1 if read success
//  return 0 if no response message arrived
//  return errorcode if read aborted by server
unsigned long int sdo_upload_normal(Uint8 id,Uint32 ob_infor,Uint32 *data)
{
		struct can_frame sdo_upload_init_response;
		struct can_frame sdo_upload_init;
		
		struct can_frame sdo_upload_segment_response;
		struct can_frame sdo_upload_segment;
		unsigned char sdo_ccs,sdo_e,sdo_s,sdo_n;//sdo message control field
		Uint32 error_code;
		
		int ret=0;
		if(id==1)
		  sdo_upload_init.id=SDO_ID1_CLIENT_TO_SERVER;
		else if(id==2)
		  sdo_upload_init.id=SDO_ID2_CLIENT_TO_SERVER;
		sdo_upload_init.buf[0]=0x40;
		sdo_upload_init.buf[1]=(ob_infor)&0xFF;
		sdo_upload_init.buf[2]=(ob_infor>>8)&0xFF;
		sdo_upload_init.buf[3]=(ob_infor>>16)&0xFF;	
	    sdo_upload_init.buf[4]=0x0;
	    sdo_upload_init.buf[5]=0x0;
	    sdo_upload_init.buf[6]=0x0;
	    sdo_upload_init.buf[7]=0x0;        
		sdo_upload_init.len = 0x8;
			
	    if(id==1)
	       sdo_upload_init_response.id=SDO_ID1_SERVER_TO_CLIENT;	
	    else if(id==2)
	       sdo_upload_init_response.id=SDO_ID2_SERVER_TO_CLIENT;	    
		
		
		/*send sdo upload initial request */
		ret=can_tx_with_response(&sdo_upload_init,&sdo_upload_init_response);
		
		if(ret==FAIL)
		{
			//printf("sdo upload init failed:no response\n");
			return FAIL;
		}
		/* sdo upload initial reponse check */
		/* check sdo control field  */
		sdo_ccs=(sdo_upload_init_response.buf[0]>>5)&0x7;
		sdo_e=(sdo_upload_init_response.buf[0]>>1)&0x1;
		sdo_s=sdo_upload_init_response.buf[0]&0x1;
		sdo_n=(sdo_upload_init_response.buf[0]>>2)&0x3;
		if((sdo_e==1)&&(sdo_s==1))
		{
	    //expedited transfer,the data is already in the data field of response message	
	    //data length=4-sdo_n
	       *data=((Uint32)sdo_upload_init_response.buf[7]<<24)|((Uint32)sdo_upload_init_response.buf[6]<<16)|((Uint32)sdo_upload_init_response.buf[5]<<8)|(sdo_upload_init_response.buf[4]);
	       return SUCCESS; 	
		}
	    if(sdo_ccs==4)//sdo abort transfer
	    {
	       error_code=((Uint32)sdo_upload_init_response.buf[7]<<24)|((Uint32)sdo_upload_init_response.buf[6]<<16)|((Uint32)sdo_upload_init_response.buf[5]<<8)|(sdo_upload_init_response.buf[4]);
	       return error_code;
	    }
	   
	   /* sdo upload segment */
	    if(id==1)
	 	   sdo_upload_segment.id=SDO_ID1_CLIENT_TO_SERVER;
	 	else if(id==2)
	 	   sdo_upload_segment.id=SDO_ID2_CLIENT_TO_SERVER;
		sdo_upload_segment.buf[0]=0x60;
		//*(unsigned long int *) sdo_upload_segment.buf=0;
	    //*(unsigned long int *)(sdo_upload_segment.buf+4)=0;
	    sdo_upload_segment.buf[1]=0;
	    sdo_upload_segment.buf[2]=0;
	    sdo_upload_segment.buf[3]=0;
	    sdo_upload_segment.buf[4]=0;
	    sdo_upload_segment.buf[5]=0;
	    sdo_upload_segment.buf[6]=0;
	    sdo_upload_segment.buf[7]=0;
	    sdo_upload_segment.len = 0x8;
	    if(id==1)
	      sdo_upload_segment_response.id=SDO_ID1_SERVER_TO_CLIENT;
	    else if(id==2)
	      sdo_upload_segment_response.id=SDO_ID2_SERVER_TO_CLIENT;
	    /* send sdo upload segment */
	    ret=can_tx_with_response(&sdo_upload_segment,&sdo_upload_segment_response);
		
		if(ret==FAIL)
		{
			//printf("sdo upload segment failed:no response\n");
			return FAIL;
		} 		
		
	return FAIL;			
}

/*  CAN write data to server which is motor driver for this project*/
//  return 1 if write success
//  return 0 if no response message arrived
//  return errorcode if write aborted by server
unsigned long int sdo_download_normal(Uint8 id,Uint32 ob_infor,Uint32  data)
{
		struct can_frame sdo_download_init_response;
		struct can_frame sdo_download_init;
		
		//struct can_frame sdo_download_segment_response;
		//struct can_frame sdo_download_segment;
		Uint32 sdo_e,sdo_s,sdo_n,sdo_scs;//sdo message control field
		Uint32 error_code;
		
		int ret=0;
		sdo_e=1;//expedit transfer
		sdo_n=4-(ob_infor>>24)&0xFF;//caculate data length indicator
		sdo_s=1;//data size is indicated
		if(id==1)
		   sdo_download_init.id=SDO_ID1_CLIENT_TO_SERVER;
		else if(id==2)
		   sdo_download_init.id=SDO_ID2_CLIENT_TO_SERVER;		
		sdo_download_init.buf[0]=((Uint32)CCS_INIT_DOWNLOAD<<5)|(sdo_n<<2)|(sdo_e<<1)|sdo_s;
		sdo_download_init.buf[1]=(ob_infor)&0xFF;
		sdo_download_init.buf[2]=(ob_infor>>8)&0xFF;
		sdo_download_init.buf[3]=(ob_infor>>16)&0xFF;	
	    sdo_download_init.buf[4]=(data)&0xFF;
	    sdo_download_init.buf[5]=(data>>8)&0xFF;
	    sdo_download_init.buf[6]=(data>>16)&0xFF;
	    sdo_download_init.buf[7]=(data>>24)&0xFF;        
		sdo_download_init.len = 0x8;
			
	    	
		if(id==1)
		   sdo_download_init_response.id=SDO_ID1_SERVER_TO_CLIENT;		   
		else if(id==2)		
		   sdo_download_init_response.id=SDO_ID2_SERVER_TO_CLIENT;
		/*send sdo upload initial request */
		ret=can_tx_with_response(&sdo_download_init,&sdo_download_init_response);
		
		if(ret==FAIL)
		{
			//printf("sdo upload init failed:no response\n");
			return FAIL;
		}
		/* sdo upload initial reponse check */
		/* check sdo control field  */
		sdo_scs=(sdo_download_init_response.buf[0]>>5)&0x7;
		
	    if(sdo_scs==SDO_SCS_ABORT)//sdo abort transfer
	    {
	       error_code=((Uint32)sdo_download_init_response.buf[7]<<24)|((Uint32)sdo_download_init_response.buf[6]<<16)|((Uint32)sdo_download_init_response.buf[5]<<8)|(sdo_download_init_response.buf[4]);
	       return error_code;
	    }
	   
	   return SUCCESS;
		
		
}

void canopen_heart_beat_monitor_init()
{
		struct ECAN_REGS ECanaShadow;
	/* Configure Mailbox  as a Receive mailbox */
	
	   ECanaShadow.CANMD.all = ECanaRegs.CANMD.all;
	   ECanaShadow.CANMD.bit.MD19 = 1;
	   ECanaRegs.CANMD.all = ECanaShadow.CANMD.all;	
}

Uint16 canopen_heart_beat_monitor()
{
	   struct ECAN_REGS ECanaShadow;
	   unsigned int time_out_counter=0;
	   volatile struct MBOX *Mailbox;
	   unsigned char message_length;

	   unsigned char ret;
	   
      /* Disnable Mailbox ,required for writing MSGID */
       ECanaShadow.CANME.all = ECanaRegs.CANME.all;
       ECanaShadow.CANME.bit.ME19 = 0;
       ECanaRegs.CANME.all = ECanaShadow.CANME.all;      
          
	   //ECanaMboxes.MBOX19.MSGID.all=NMT_ID_HEART_BEAT;   
       
       /* Enable Mailbox */

       ECanaShadow.CANME.all = ECanaRegs.CANME.all;
       ECanaShadow.CANME.bit.ME19 = 1;
       ECanaRegs.CANME.all = ECanaShadow.CANME.all;
       
       while(ECanaRegs.CANRMP.bit.RMP19==0) //check if heart beat message is arrived
       {
          time_out_counter++;
          if(time_out_counter==10000)
             return FAIL;  //no heart beat message

       }
       Mailbox = &ECanaMboxes.MBOX0 + 19; 
       message_length=Mailbox->MSGCTRL.bit.DLC;

       ret = Mailbox->MDL.all&0xFF; 


       return ret;
}


 
Uint32 canopen_set_object(Uint8 id,Uint32 object,Uint32 data)
{
	Uint16 ret;
	Uint16 times=0;
	for(times=0;times<RW_PROTECTION_TIMES;)
	{
		ret=sdo_download_normal(id,object,data);		
	    if(ret==SUCCESS)
	         break;
	     else 
	     {
	     	if(times==2)
	     	{
	     	      drive_status[id-1].can_failure = 1;
	     	   
	     	   return FAIL;
	     	}
	     	else
	     	   times++;
	     }
	}	
	return FAIL;	
}

Uint32 canopen_get_object(Uint8 id,Uint32 object,Uint32 *data)
{
	Uint16 ret;
	Uint16 times=0;
	
	for(times=0;times<RW_PROTECTION_TIMES;)
	{
		 ret=sdo_upload_normal(id,object,data);
	     if(ret==SUCCESS)
	         break;
	     else 
	     {
	     	if(times==2)
	     	{

	     	      drive_status[id-1].can_failure = 1;
	     	   return FAIL;
	     	}
	     	else
	     	   times++;
	     }            
	}
	return FAIL;	
}

Uint32 drive_position_mode_init(Uint8 id,struct position_mode_parameter *parameter)
{
			  Uint32 old_data;
			  Uint16 old_bit;

			  if(!drive_status[id-1].can_failure)
			  {
			  	/* set as position mode  */
			  	if(canopen_set_object(id,OBJECT_MODE_OF_OPERATION,MODE_OF_OPERATION_POSITION)==FAIL)
			        goto UART_ACCESS; 
			  	
			  	/* set continuous profile,motor is moving continuously,parameters can be changed while in progress */
			  	if(canopen_get_object(id,OBJECT_CONROL_WORD,&old_data)==FAIL)
			         goto UART_ACCESS;
			    old_bit=(old_data>>CTRL_WORD_CHANGE_SET_IMMEDIALY)&0x1; //extract bit 5  
			    if(old_bit!=1)//bit 5 is 0,discrete profile
			       canopen_set_object(id,OBJECT_CONROL_WORD,old_data|(0x1<< CTRL_WORD_CHANGE_SET_IMMEDIALY));//set bit 5 as 1
			     
			   	/* set acceleration */
			   	if(canopen_set_object(id,OBJECT_PROFILE_ACCELERATION,parameter->acceleration)==FAIL)
			         goto UART_ACCESS;         
			  	
			  	/* set deceleration */
			   	if(canopen_set_object(id,OBJECT_PROFILE_DECELERATION,parameter->deceleration)==FAIL)
			         goto UART_ACCESS;
			         
			    /* set jerk limit */
			   	if(canopen_set_object(id,OBJECT_TRAJECTORY_JERK_LIMIT,parameter->jerk_limit)==FAIL)
			         goto UART_ACCESS;     
			  	/* set point to point move methods:trajectory or s curve */
			   	if(canopen_set_object(id,OBJECT_PROFILE_TYPE,parameter->ptp_move_methods)==FAIL)
			         goto UART_ACCESS;
			    /* set quick stop deceleration */
			   	if(canopen_set_object(id,OBJECT_QUICK_STOP_DECELERAT,parameter->quick_stop_deceleration)==FAIL)
			         goto UART_ACCESS; 
			    /* set velocity */
			   	if(canopen_set_object(id,OBJECT_PROFILE_VELOCITY,parameter->velocity)==FAIL)
			         goto UART_ACCESS; 
			    return SUCCESS;   
			  }
UART_ACCESS:
			  if(drive_status[id-1].can_failure)
			  {
			  	//add UART access here
			  	return FAIL;
			  } 	
			  return FAIL;
}

Uint16 write_target_position(Uint8 id,Uint16 position)
{
	          Uint32 old_data;
	          Uint16 old_bit;
	          
	          if(!drive_status[id-1].can_failure)
	          {
	          	/* write targe position */
			   	if(canopen_set_object(id,OBJECT_TARGET_POSITION,(Uint32)position)==FAIL)
			         goto UART_ACCESS;
			  	
			  	/* check control word bit 4 ,0 to 1 transition causes the drive to copy parameters to the register*/
			  	if(canopen_get_object(id,OBJECT_CONROL_WORD,&old_data)==FAIL)
			         goto UART_ACCESS;
			    old_bit=(old_data>>CTRL_WORD_NEW_SETPOINT)&0x1; //extract bit 4  
			    if(old_bit!=0)//bit 4 is 1,it need to be 0
			       canopen_set_object(id,OBJECT_CONROL_WORD,old_data&(0x0<< CTRL_WORD_NEW_SETPOINT));//set bit 4 as 0
			       
			    /* set bit 4 to 1,to start move imediately  */
			    canopen_set_object(id,OBJECT_CONROL_WORD,old_data|(0x1<< CTRL_WORD_NEW_SETPOINT));
			    
			    
			    /* check if motor parameter is ackenowledged  */
			    do{
			         if(canopen_get_object(id,OBJECT_STATUS_WORD,&old_data)==FAIL)
			            goto UART_ACCESS;
			         old_bit=(old_data>>STATUS_SETPOINT_ACKNOWLEDGE)&0x1; //extract bit 12  
			    }while(old_bit==0);
			         
	            return SUCCESS;
	          }   
UART_ACCESS:
			  if(drive_status[id-1].can_failure)
			  {
			  	//add UART access here
			  	return FAIL;
			  } 
			  return FAIL;		
}
Uint16 read_motor_prosition(Uint8 id,Uint16 *position)
{
	          Uint32 *p;
	          if(!drive_status[id-1].can_failure)
	          {	  	
			  	
			  	if(canopen_get_object(id,POSITION_ACTUAL_VALUE,p)==FAIL)
			         goto UART_ACCESS;
			    
			    *position = (Uint16)(*p);
			    return SUCCESS;  
	          }   
UART_ACCESS:
			  if(drive_status[id-1].can_failure)
			  {
			  	//add UART access here
			  	return FAIL;
			  }	
			  return FAIL;	
	
}

Uint32 check_drive_status(Uint8 id)
{
	          Uint16 warning;
	          Uint16 fault;
	          Uint16 trajectory_aborted;
	          if(!drive_status[id-1].can_failure)
	          {	  	
			    if(canopen_get_object(id,OBJECT_STATUS_WORD,(Uint32*)(&drive_status[id-1].status))==FAIL)
			         goto UART_ACCESS;			  	
			  	if(canopen_get_object(id,OBJECT_MANUFACTURER_STATUS,&drive_status[id-1].manufac_status)==FAIL)
			         goto UART_ACCESS;
			    if(canopen_get_object(id,OBJECT_NETWORK_STATUS,(Uint32*)(&drive_status[id-1].network_status))==FAIL)
			         goto UART_ACCESS;
			    
			    warning=(drive_status[id-1].status>>STATUS_WARNING)&0x01;
			    fault=(drive_status[id-1].status>>STATUS_FAULT)&0x01;
			    trajectory_aborted  =(drive_status[id-1].status>>STATUS_TRAJECTORY_ABORT)&0x01; 
			    if(warning|fault|trajectory_aborted)
			        //send error message to ground  
			        asm ("      ESTOP0");
			    return SUCCESS;    
	          }   
UART_ACCESS:
			  if(drive_status[id-1].can_failure)
			  {
			  	//add UART access here
			  	return FAIL;
			  }		
			  return FAIL;	
}

