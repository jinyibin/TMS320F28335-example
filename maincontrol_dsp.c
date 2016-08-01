// TI File $Revision: /main/8 $
// Checkin $Date: August 10, 2007   09:04:22 $
//###########################################################################
// Filename: Example_28xEcan_A_to_B_Xmit.c
//
// Description: eCAN-A To eCAN-B TXLOOP - Transmit loop
//
// ASSUMPTIONS:
//
//    This program requires the DSP2833x header files.
//
//    Both CAN ports of the 2833x DSP need to be connected
//    to each other (via CAN transceivers)
//
//       eCANA is on GPIO19(CANTXA)  and
//                   GPIO18 (CANRXA)
//
//       eCANB is on GPIO20  (CANTXB)  and
//                   GPIO21 (CANRXB)
//
//    As supplied, this project is configured for "boot to SARAM"
//    operation.  The 2833x Boot Mode table is shown below.
//    For information on configuring the boot mode of an eZdsp,
//    please refer to the documentation included with the eZdsp,
//
//       $Boot_Table:
//
//         GPIO87   GPIO86     GPIO85   GPIO84
//          XA15     XA14       XA13     XA12
//           PU       PU         PU       PU
//        ==========================================
//            1        1          1        1    Jump to Flash
//            1        1          1        0    SCI-A boot
//            1        1          0        1    SPI-A boot
//            1        1          0        0    I2C-A boot
//            1        0          1        1    eCAN-A boot
//            1        0          1        0    McBSP-A boot
//            1        0          0        1    Jump to XINTF x16
//            1        0          0        0    Jump to XINTF x32
//            0        1          1        1    Jump to OTP
//            0        1          1        0    Parallel GPIO I/O boot
//            0        1          0        1    Parallel XINTF boot
//            0        1          0        0    Jump to SARAM	    <- "boot to SARAM"
//            0        0          1        1    Branch to check boot mode
//            0        0          1        0    Boot to flash, bypass ADC cal
//            0        0          0        1    Boot to SARAM, bypass ADC cal
//            0        0          0        0    Boot to SCI-A, bypass ADC cal
//                                              Boot_Table_End$
//
// DESCRIPTION:
//
//    This example TRANSMITS data to another CAN module using MAILBOX5
//    This program could either loop forever or transmit "n" # of times,
//    where "n" is the TXCOUNT value.
//
//    This example can be used to check CAN-A and CAN-B. Since CAN-B is
//    initialized in DSP2833x_ECan.c, it will acknowledge all frames
//    transmitted by the node on which this code runs. Both CAN ports of
//    the 2833x DSP need to be connected to each other (via CAN transceivers)
//
//###########################################################################
// Original Author: HJ
//
// $TI Release: DSP2833x Header Files V1.10 $
// $Release Date: February 15, 2008 $
//###########################################################################

#include "DSP2833x_Device.h"     // DSP2833x Headerfile Include File
#include "DSP2833x_Examples.h"   // DSP2833x Examples Include File
#include "MotorCanCtrl.h"
#include "uart_fpga.h"
#include "algrithom.h"

interrupt void cpu_timer0_isr(void);
interrupt void cpu_timer1_isr(void);
interrupt void cpu_timer2_isr(void);
void memset(Uint8 *buf,Uint8 value,Uint32 length);

#define  CPU_FREQUENCY  150000000
#define  WAITING_TIME_OUT  CPU_FREQUENCY*0.5 //500ms


                                                                                                         
void main()
{
 
   Uint32 start_time,stop_time,diff_time;
  // Uint32 a_start_time,a_stop_time;
   Uint32 tx_cnt=0;
   Uint32 rx_cnt=0;
   Uint8  frame_buf[128];
   Uint32 i;
   Uint16 frame_ready=0;
   Uint16 msg_process_ok=0;
   Uint8  time_out_cnt=0;
    flying_parameter_s  *f;
    waypoint_info_s *w;
    global_flag_s *g;
    struct position_mode_parameter *motor_pos_para;  

    f=get_flying_parameter();
    w=get_waypoint_info();
    g=get_global_flag();
    motor_pos_para=get_motor_pos_mode_para(1);
    
    //initial variant stuct
    motor_pos_para->ptp_move_methods=1;// 0:discrete profile  1:continuous profile
	motor_pos_para->profile_type=0;// 0:trapezoid    1:s-curve
	motor_pos_para->jerk_limit=1;// 100counts/s^3
	motor_pos_para->velocity=1;// 0.1counts/s
	motor_pos_para->acceleration=1;// acceleration
	motor_pos_para->deceleration=1;// deceleration
	motor_pos_para->quick_stop_deceleration=1;// quick stop deceleration   

    for(i=0;i<sizeof(flying_parameter_s);i++)
    {
    	*((Uint8*)f+i) = 0;
    }
    for(i=0;i<sizeof(waypoint_info_s);i++)
    {
    	*((Uint8*)w+i) = 0;
    }
    g->running_algrithom = 0;
    g->timer_up =0;
    g->waypoint_ready = 0;
    //g->work_mode = FLYING_MODE_MANUAL; 
    g->work_mode = WORKING_MODE_READING_WP; 
    f->pid_period = 10;//pid sample time,1 lsb = 0.1S  

/* Create a shadow register structure for the CAN control registers. This is
 needed, since, only 32-bit access is allowed to these registers. 16-bit access
 to these registers could potentially corrupt the register contents. This is
 especially true while writing to a bit (or group of bits) among bits 16 - 31 */
 

// Step 1. Initialize System Control:
// PLL, WatchDog, enable Peripheral Clocks
// This example function is found in the DSP2833x_SysCtrl.c file.
   InitSysCtrl();

// Step 2. Initalize GPIO:
// This example function is found in the DSP2833x_Gpio.c file and
// illustrates how to set the GPIO to it's default state.
// InitGpio();  // Skipped for this example

   // Just initalize eCAN pins for this example
   // This function is in DSP2833x_ECan.c
   InitECanGpio();
   InitSciGpio();

// Step 3. Clear all interrupts and initialize PIE vector table:
// Disable CPU interrupts
   DINT;

// Initialize the PIE control registers to their default state.
// The default state is all PIE interrupts disabled and flags
// are cleared.
// This function is found in the DSP2833x_PieCtrl.c file.
   InitPieCtrl();
   

   
   
// Disable CPU interrupts and clear all CPU interrupt flags:
   IER = 0x0000;
   IFR = 0x0000;

// Initialize the PIE vector table with pointers to the shell Interrupt
// Service Routines (ISR).
// This will populate the entire table, even if the interrupt
// is not used in this example.  This is useful for debug purposes.
// The shell ISR routines are found in DSP2833x_DefaultIsr.c.
// This function is found in DSP2833x_PieVect.c.
   InitPieVectTable();
   
   // Interrupts that are used in this example are re-mapped to
// ISR functions found within this file.
   EALLOW;  // This is needed to write to EALLOW protected registers
   PieVectTable.TINT0 = &cpu_timer0_isr;
   PieVectTable.XINT13 = &cpu_timer1_isr;
   PieVectTable.TINT2 = &cpu_timer2_isr;
   EDIS;    // This is needed to disable write to EALLOW protected registers
   
   // Step 4. Initialize the Device Peripheral. This function can be
//         found in DSP2833x_CpuTimers.c
   InitCpuTimers();   // For this example, only initialize the Cpu Timers

// Configure CPU-Timer 0, 1, and 2 to interrupt every second:
// 150MHz CPU Freq, 1 second Period (in uSeconds)

   ConfigCpuTimer(&CpuTimer0, 150, f->pid_period*100000 );
   ConfigCpuTimer(&CpuTimer1, 150, f->pid_period*100000);
   ConfigCpuTimer(&CpuTimer2, 150, f->pid_period*100000);
   
// To ensure precise timing, use write-only instructions to write to the entire register. Therefore, if any
// of the configuration bits are changed in ConfigCpuTimer and InitCpuTimers (in DSP2833x_CpuTimers.h), the
// below settings must also be updated.

   CpuTimer0Regs.TCR.all = 0x4001; // Use write-only instruction to set TSS bit = 0
   CpuTimer1Regs.TCR.all = 0x4001; // Use write-only instruction to set TSS bit = 0
   CpuTimer2Regs.TCR.all = 0x4001; // Use write-only instruction to set TSS bit = 0
   // Enable CPU int1 which is connected to CPU-Timer 0, CPU int13
// which is connected to CPU-Timer 1, and CPU int 14, which is connected
// to CPU-Timer 2:
   IER |= M_INT1;
   IER |= M_INT13;
   IER |= M_INT14;

// Enable TINT0 in the PIE: Group 1 interrupt 7
   PieCtrlRegs.PIEIER1.bit.INTx7 = 1;
  
   EALLOW;   
   GpioCtrlRegs.GPBMUX2.bit.GPIO61 = 0;
   GpioCtrlRegs.GPBDIR.bit.GPIO61 = 1;
   EDIS;

// Enable global Interrupts and higher priority real-time debug events:
   EINT;   // Enable Global interrupt INTM
   ERTM;   // Enable Global realtime interrupt DBGM

// Interrupts that are used in this example are re-mapped to
// ISR functions found within this file.

// No interrupts used in this example.

// Step 4. Initialize all the Device Peripherals:
// This function is found in DSP2833x_InitPeripherals.c
// InitPeripherals(); // Not required for this example

   // In this case just initalize eCAN-A and eCAN-B
   // This function is in DSP2833x_ECan.c
   InitECan();
   
   // set CAN transmit and receive channel
   //can_rw_with_response_init(); 
   // set motor drive for position mode 
   //drive_position_mode_init(&position_mode_parameter);
   
   uart_fpga_init();
   //send_frame(CTRL_FRAME_TYPE_QUERY);//query flying mode

  for(;;)
  {
  	
	   	   if(g->work_mode==WORKING_MODE_READING_WP){//wayppoint is ready ,reading waypoint
	   	      time_out_cnt=0;//clear time out counter
              memset(frame_buf,0,sizeof(frame_buf));//clear frame buffer
           	  send_frame(CTRL_FRAME_TYPE_WP);//send waypoint request frame
           	  
           	  //waiting for waypoint 
           	  start_time = ReadCpuTimer0Counter();
           	  while(frame_ready==0){//no frame received,waiting until time out          	  	 
           	     frame_ready=uart_fpga_frame_receive(frame_buf);
           	     stop_time = ReadCpuTimer0Counter();
           	     if(stop_time>start_time)//cross timer boundery
           	     	diff_time = start_time + f->pid_period*100000-stop_time;
           	     else 
           	        diff_time = start_time - stop_time;
           	     if(diff_time>WAITING_TIME_OUT){//no waypoint return in WAITING_TIME_OUT time
           	         time_out_cnt++; // recording time out times
           	         break;
           	     }
           	  }
           	  if(frame_ready==1){//crc error
            	   //g->timer_up = 0;  resend request frame immediately
            	   frame_ready = 0;
           	       continue;          	  	
           	  }else if(frame_ready>1){
           	    //a complete frame is ready,extract information and process it
           	    frame_ready = 0;
           	    msg_process_ok=frame_parse(frame_buf);
           	    if(msg_process_ok==0){//message err
           	       g->timer_up = 0;
           	       continue;
           	    }
           	  }
           	   
	       }
 

	   if(g->timer_up==1){//timer up active every 1 second
	   	   //check_drive_status(1);
			    
		   //read_motor_prosition(1,&f->heading_motor_actual_position1);
		   
	   	   if(g->work_mode==FLYING_MODE_MANUAL)
	       { 
              memset(frame_buf,0,sizeof(frame_buf));//clear frame buffer
           	  send_frame(CTRL_FRAME_TYPE_MANUAL);//send data frame
           	  tx_cnt++;
           	  
           	  //waiting for return data 
           	  start_time = ReadCpuTimer0Counter();
           	  while(frame_ready==0){//no frame received,waiting until time out          	  	 
           	     frame_ready=uart_fpga_frame_receive(frame_buf);           	     
           	     stop_time = ReadCpuTimer0Counter();
           	     if(stop_time>start_time)//cross timer boundery
           	     	diff_time = start_time + f->pid_period*100000-stop_time;
           	     else 
           	        diff_time = start_time - stop_time;
           	     if(diff_time>WAITING_TIME_OUT){//no waypoint return in WAITING_TIME_OUT time          	         
           	         break;
           	     }
           	     
           	  }           	  
           	  if(frame_ready==1){//crc error
            	   g->timer_up = 0;  
            	   frame_ready = 0;
            	   rx_cnt++;
           	       continue;          	  	
           	  }else if(frame_ready>1){
           	    //a complete frame is ready,extract information and process it
           	    frame_ready = 0;
           	    rx_cnt++;
           	    msg_process_ok=frame_parse(frame_buf);
           	    if(msg_process_ok==0){//message err
           	       g->timer_up = 0;
           	       continue;
           	    }
           	  }	       
	       }
	       if(g->work_mode==FLYING_MODE_AUTO)
	       {
			     //check_drive_status(1);
			    
		         //read_motor_prosition(1,&f->heading_motor_actual_position1);
			      time_out_cnt=0;//clear time out counter
			      
read_para_auto:	      //a_start_time = ReadCpuTimer0Counter();    
	              memset(frame_buf,0,sizeof(frame_buf));//clear frame buffer
	           	  send_frame(CTRL_FRAME_TYPE_AUTO);//send data frame
	           	  
	           	  //waiting for return data 
	           	  start_time = ReadCpuTimer0Counter();
	           	  while(frame_ready==0){//no frame received,waiting until time out          	  	 
	           	     frame_ready=uart_fpga_frame_receive(frame_buf);           	     
	           	     stop_time = ReadCpuTimer0Counter();
           	     if(stop_time>start_time)//cross timer boundery
           	     	diff_time = start_time + f->pid_period*100000-stop_time;
           	     else 
           	        diff_time = start_time - stop_time;
           	     if(diff_time>WAITING_TIME_OUT){//no waypoint return in WAITING_TIME_OUT time
	           	     	 time_out_cnt++;//clear time out counter  
	           	     	 if(time_out_cnt==3){//FPGA no response,switch to manual mode
	           	     	    g->work_mode=FLYING_MODE_MANUAL; 
	           	     	    break;
	           	     	 }        	         
	           	         goto read_para_auto;
	           	     }
	           	     
	           	  }           	  
		          if(frame_ready==1){//crc error
		            	   //g->timer_up = 0;  resend request frame immediately
		            	   frame_ready = 0;
		           	       continue;          	  	
		          }else if(frame_ready>1){
	           	    //a complete frame is ready,extract information and process it
	           	    frame_ready = 0;
	           	    msg_process_ok=frame_parse(frame_buf);
           	        if(msg_process_ok==0){//message err
           	           g->timer_up = 0;
           	           continue;
           	        }
	           	  }	       
			    
			      //  main algrithm 
			    
			      write_target_position(1,f->heading_motor_target_position1);
			    
			      //a_stop_time = ReadCpuTimer0Counter(); 			    
        	
	       	
	       }

	       if(g->work_mode==FLYING_MODE_UPWIND)
	       {
			     //check_drive_status(1);
			    
		         //read_motor_prosition(1,&f->heading_motor_actual_position1);
			      time_out_cnt=0;//clear time out counter
			      
read_para_upwind:	      //a_start_time = ReadCpuTimer0Counter();    
	              memset(frame_buf,0,sizeof(frame_buf));//clear frame buffer
	           	  send_frame(CTRL_FRAME_TYPE_UPWIND);//send data frame
	           	  
	           	  //waiting for return data 
	           	  start_time = ReadCpuTimer0Counter();
	           	  while(frame_ready==0){//no frame received,waiting until time out          	  	 
	           	     frame_ready=uart_fpga_frame_receive(frame_buf);           	     
	           	     stop_time = ReadCpuTimer0Counter();
           	     if(stop_time>start_time)//cross timer boundery
           	     	diff_time = start_time + f->pid_period*100000-stop_time;
           	     else 
           	        diff_time = start_time - stop_time;
           	     if(diff_time>WAITING_TIME_OUT){//no waypoint return in WAITING_TIME_OUT time
	           	     	 time_out_cnt++;//clear time out counter  
	           	     	 if(time_out_cnt==3){//FPGA no response,switch to manual mode
	           	     	    g->work_mode=FLYING_MODE_MANUAL; 
	           	     	    break;
	           	     	 }        	         
	           	         goto read_para_upwind;
	           	     }
	           	     
	           	  }           	  
		          if(frame_ready==1){//crc error
		            	   //g->timer_up = 0;  resend request frame immediately
		            	   frame_ready = 0;
		           	       continue;          	  	
		          }else if(frame_ready>1){
	           	    //a complete frame is ready,extract information and process it
	           	    frame_ready = 0;
	           	    msg_process_ok=frame_parse(frame_buf);
           	        if(msg_process_ok==0){//message err
           	           g->timer_up = 0;
           	           continue;
           	        }
	           	  }	       
			    
			      //  main algrithm 
			    
			      write_target_position(1,f->heading_motor_target_position1);
			    
			     // a_stop_time = ReadCpuTimer0Counter(); 			    
        	
	       	
	       }
	   
	       if(g->work_mode==FLYING_MODE_HOVER)
	       {
			     //check_drive_status(1);
			    
		         //read_motor_prosition(1,&f->heading_motor_actual_position1);
			      time_out_cnt=0;//clear time out counter
			      
read_para_hover:	      //a_start_time = ReadCpuTimer0Counter();    
	              memset(frame_buf,0,sizeof(frame_buf));//clear frame buffer
	           	  send_frame(CTRL_FRAME_TYPE_HOVER);//send data frame
	           	  
	           	  //waiting for return data 
	           	  start_time = ReadCpuTimer0Counter();
	           	  while(frame_ready==0){//no frame received,waiting until time out          	  	 
	           	     frame_ready=uart_fpga_frame_receive(frame_buf);           	     
	           	     stop_time = ReadCpuTimer0Counter();
           	     if(stop_time>start_time)//cross timer boundery
           	     	diff_time = start_time + f->pid_period*100000-stop_time;
           	     else 
           	        diff_time = start_time - stop_time;
           	     if(diff_time>WAITING_TIME_OUT){//no waypoint return in WAITING_TIME_OUT time
	           	     	 time_out_cnt++;//clear time out counter  
	           	     	 if(time_out_cnt==3){//FPGA no response,switch to manual mode
	           	     	    g->work_mode=FLYING_MODE_MANUAL; 
	           	     	    break;
	           	     	 }        	         
	           	         goto read_para_hover;
	           	     }
	           	     
	           	  }           	  
		          if(frame_ready==1){//crc error
		            	   //g->timer_up = 0;  resend request frame immediately
		            	   frame_ready = 0;
		           	       continue;          	  	
		          }else if(frame_ready>1){
	           	    //a complete frame is ready,extract information and process it
	           	    frame_ready = 0;
	           	    msg_process_ok=frame_parse(frame_buf);
           	        if(msg_process_ok==0){//message err
           	           g->timer_up = 0;
           	           continue;
           	        }
	           	  }	       
			    
			      //  main algrithm 
			    
			      write_target_position(1,f->heading_motor_target_position1);
			    
			      //a_stop_time = ReadCpuTimer0Counter(); 			    
        	
	       	
	       }
            //DELAY_US(1000);
             
           g->timer_up = 0;
           
	   }	   
  	
  }

}

interrupt void cpu_timer0_isr(void)
{
   global_flag_s *g;
   g= get_global_flag();
   
    //light the LED
    CpuTimer0.InterruptCount++;
    if ((CpuTimer0.InterruptCount%2)==0)
       GpioDataRegs.GPBTOGGLE.bit.GPIO61 = 1; 
       
    g->timer_up = 1;


    // Acknowledge this interrupt to receive more interrupts from group 1
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;
}

interrupt void cpu_timer1_isr(void)
{
   CpuTimer1.InterruptCount++;
   // The CPU acknowledges the interrupt.
   EDIS;
}

interrupt void cpu_timer2_isr(void)
{  EALLOW;
   CpuTimer2.InterruptCount++;
   //if ((CpuTimer2.InterruptCount%2)==0)
   //GpioDataRegs.GPBTOGGLE.bit.GPIO61 = 1; // Toggle GPIO61 once
   // The CPU acknowledges the interrupt.
   EDIS;
}
void memset(Uint8 *buf,Uint8 value,Uint32 length)
{
   Uint32 i=0;
   for(i=0;i<length;i++){
   	  buf[i] = value;
   }	
	
}
