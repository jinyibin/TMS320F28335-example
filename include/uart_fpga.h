#ifndef UART_FPGA_H_
#define UART_FPGA_H_

#endif /*UART_FPGA_H_*/
//-----frame head FPGA to DSP----
#define CTRL_FRAME_HEAD1  0xAA
#define CTRL_FRAME_HEAD2  0x55
#define CTRL_FRAME_HEAD3  0xEB
#define CTRL_FRAME_HEAD4  0x90
#define CTRL_FRAME_HEAD5  0x1C
#define CTRL_FRAME_HEAD6  0x1A
//-----frame head DSP to FPGA----
#define CTRL_FRAME_HEAD1_TO_FPGA  0xBB
#define CTRL_FRAME_HEAD2_TO_FPGA  0x66
#define CTRL_FRAME_HEAD3_TO_FPGA  0xEB
#define CTRL_FRAME_HEAD4_TO_FPGA  0x90
#define CTRL_FRAME_HEAD5_TO_FPGA  0xCC
#define CTRL_FRAME_HEAD6_TO_FPGA  0xBB

#define CTRL_FRAME_HEAD_LENGTH 6

#define CTRL_FRAME_TYPE_MASK   7
//------frame DSP to FPGA-------------
#define CTRL_FRAME_WP_NO_MASK_TX       9
#define CTRL_FRAME_MOTOR_SP1_MASK_TX   10
#define CTRL_FRAME_MOTOR_SP2_MASK_TX   12
#define CTRL_FRAME_MOTOR_SP3_MASK_TX   14
#define CTRL_FRAME_MOTOR_SP4_MASK_TX   16
#define CTRL_FRAME_MOTOR_ANGLE1_MASK_TX 18
#define CTRL_FRAME_MOTOR_ANGLE2_MASK_TX 20
#define CTRL_FRAME_ERR_MASK_TX          22
#define CTRL_FRAME_THROTTLE_MASK_TX    24
#define CTRL_FRAME_RSV_MASK_TX         25

#define CTRL_FRAME_CHECK_LENGTH_TX     24
#define CTRL_FRAME_LENGTH_TX           32
#define CTRL_FRAME_MINIMUM_LEN_TX      32
//---------------------------------------------
#define CTRL_FRAME_FLYING_MODE_MASK_RX   9

//-----frame FPGA to DSP for  manual mode----------
#define CTRL_FRAME_MOTOR_POS1_MASK_RX_MANUAL     10
#define CTRL_FRAME_MOTOR_POS2_MASK_RX_MANUAL     12
//-----frame FPGA to DSP for upwind mode-----------
#define CTRL_FRAME_PID1_MASK_RX_UPWIND          10
#define CTRL_FRAME_PID2_MASK_RX_UPWIND          13
#define CTRL_FRAME_PID3_MASK_RX_UPWIND          16
#define CTRL_FRAME_PID_PERIOD_MASK_RX_UPWIND    17
#define CTRL_FRAME_LONG_MASK_RX_UPWIND          18
#define CTRL_FRAME_LAT_MASK_RX_UPWIND           22
#define CTRL_FRAME_HEADING_MASK_RX_UPWIND       26
#define CTRL_FRAME_WIND_DIR_MASK_RX_UPWIND      28
#define CTRL_FRAME_THROTTLE_MASK_RX_UPWIND      30
//-----frame FPGA to DSP for hovering-------------
#define CTRL_FRAME_PID1_MASK_RX_HOVER          10
#define CTRL_FRAME_PID2_MASK_RX_HOVER          13
#define CTRL_FRAME_PID3_MASK_RX_HOVER          16
#define CTRL_FRAME_PID_PERIOD_MASK_RX_HOVER    17
#define CTRL_FRAME_LONG_MASK_RX_HOVER          18
#define CTRL_FRAME_LAT_MASK_RX_HOVER           22
#define CTRL_FRAME_HEADING_MASK_RX_HOVER       26
#define CTRL_FRAME_HOV_LONG_MASK_RX_HOVER      28
#define CTRL_FRAME_HOV_LAT_MASK_RX_HOVER       32
#define CTRL_FRAME_THROTTLE_MASK_RX_HOVER      36
//-----frame FPGA to DSP for airline mode---------
#define CTRL_FRAME_PID1_MASK_RX_AIRLINE          10
#define CTRL_FRAME_PID2_MASK_RX_AIRLINE          13
#define CTRL_FRAME_PID3_MASK_RX_AIRLINE          16
#define CTRL_FRAME_PID_PERIOD_MASK_RX_AIRLINE    17
#define CTRL_FRAME_LONG_MASK_RX_AIRLINE          18
#define CTRL_FRAME_LAT_MASK_RX_AIRLINE           22
#define CTRL_FRAME_HEADING_MASK_RX_AIRLINE       26
#define CTRL_FRAME_WP_NO_MASK_RX_AIRLINE         28
#define CTRL_FRAME_WP_NUM_MASK_RX_AIRLINE        29
#define CTRL_FRAME_WP_LONG_MASK_RX_AIRLINE       30
#define CTRL_FRAME_WP_LAT_MASK_RX_AIRLINE        34
#define CTRL_FRAME_WP_TIME_MASK_RX_AIRLINE       38
#define CTRL_FRAME_THROTTLE_MASK_RX_AIRLINE      41


#define CTRL_FRAME_CHECK_LENGTH_RX   56
#define CTRL_FRAME_LENGTH_RX         64
#define CTRL_FRAME_MINIMUM_LEN_RX    64



#define CTRL_FRAME_TYPE_ERR     0xF000
#define CTRL_FRAME_TYPE_QUERY   0x0000
#define CTRL_FRAME_TYPE_MANUAL  0x0001
#define CTRL_FRAME_TYPE_WP      0x0002
#define CTRL_FRAME_TYPE_AUTO    0x0003
#define CTRL_FRAME_TYPE_IDLE    0x0004
#define CTRL_FRAME_TYPE_UPWIND  0x0005
#define CTRL_FRAME_TYPE_HOVER   0x0006
#define FLYING_MODE_AUTO        0xA5
#define FLYING_MODE_MANUAL      0xB6
#define FLYING_MODE_HOVER       0xA2
#define FLYING_MODE_UPWIND      0xA1
#define WORKING_MODE_READING_WP  0xFF
#define WORKING_MODE_IDLE        0x04
#define FAIL   0
#define SUCCESS 1
typedef struct global_flag_s
{
  Uint8 running_algrithom; // 1 enable algrithom to calculate
  Uint8 waypoint_ready;
  Uint8 navigation_data_received;
  Uint8 timer_up; //1 means timer is up
  Uint8 work_mode; //B6:manual 00:reading waypoint A5:auto
  Uint8 airline_over;

} global_flag_s;

typedef struct waypoint{
   float longitude;
   float latitude;
   Uint32 hover_time;
} waypoint;

typedef struct waypoint_info_s{
   Uint8 wp_number; // total number of waypoint
   Uint8 wp_no_received;//waypoint number has been received
   waypoint waypoint[256];	
} waypoint_info_s;
	
typedef struct flying_parameter_s{
	Uint16 propell_motor_actual_speed1;
	Uint16 propell_motor_actual_speed2;
	Uint16 propell_motor_actual_speed3;
	Uint16 propell_motor_actual_speed4;
	Uint16 heading_motor_actual_position1;
	Uint16 heading_motor_actual_position2;
	float current_longitude;
	float current_latitude;
	float current_height;
	float hover_longitude;
	float hover_latitude;
	Uint32 heading;
	Uint32 pid1;
	Uint32 pid2;
	Uint32 pid3;
	Uint8  pid_period;
	Uint16 wind_direction_from_ground;
	
			
	Uint16 propell_motor_target_speed1;
	Uint16 propell_motor_target_speed2;
	Uint16 propell_motor_target_speed3;
	Uint16 propell_motor_target_speed4;
	Uint16 heading_motor_target_position1;	
	Uint16 heading_motor_target_position2;
	
	Uint8  throttle_from_ground;
	Uint8  throttle_caculated;
	
} flying_parameter_s;




typedef struct uart_fpga_rx_frame_info
{
	Uint8 bytes_received;
	Uint8 frame_size;

} uart_fpga_rx_frame_info;

flying_parameter_s *get_flying_parameter();
waypoint_info_s *get_waypoint_info();
global_flag_s *get_global_flag();

void uart_fpga_tx_msg(char * msg,Uint8 length);
void uart_fpga_init();
Uint8 uart_fpga_frame_receive( Uint8 * msg);
void send_frame(Uint8 frame_type );
Uint16 frame_parse(Uint8 *msg);

