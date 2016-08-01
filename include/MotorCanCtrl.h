#ifndef MOTORCANCTRL_H_
#define MOTORCANCTRL_H_

#define CCS_INIT_UPLOAD  2
#define SCS_INIT_UPLOAD 2
#define CCS_INIT_DOWNLOAD 1
#define SCS_INIT_DOWNLOAD 3
#define SDO_SCS_ABORT 4

#define SDO_ID1_SERVER_TO_CLIENT ((unsigned long int)0x581)<<18
#define SDO_ID1_CLIENT_TO_SERVER ((unsigned long int)0x601)<<18
#define NMT_ID1_HEART_BEAT  ((unsigned long int)0x701)<<18

#define SDO_ID2_SERVER_TO_CLIENT ((unsigned long int)0x582)<<18
#define SDO_ID2_CLIENT_TO_SERVER ((unsigned long int)0x602)<<18
#define NMT_ID2_HEART_BEAT  ((unsigned long int)0x702)<<18

#define FAIL   0
#define SUCCESS 1
#define RW_PROTECTION_TIMES 3

#define OBJECT_INFORMATION(index,sub_index,data_length)   ((Uint32)data_length<<24)|((Uint32)sub_index<<16)|index
/************************ can open object index *******************************/

/*---------------------motor configure object-----------------------------*/
#define OBJECT_MOTOR_TYPE         OBJECT_INFORMATION(0x2383,1,2)
#define OBJECT_MOTOR_POLE_PAIR    OBJECT_INFORMATION(0x2383,2,2)
#define OBJECT_MOTOR_MAX_VELOCITY OBJECT_INFORMATION(0x2383,11,2)
#define OBJECT_MOTOR_PEAK_TORQUE  OBJECT_INFORMATION(0x2383,13,2)
/*---------------------sdo configure object-------------------------------*/
#define OBJECT_SDO_RX_COBID  OBJECT_INFORMATION(0x1200,1,2)
#define OBJECT_SDO_TX_COBID  OBJECT_INFORMATION(0x1200,2,2)

/*---------------------pdo configure object------------------------------*/
#define OBJECT_PDO_COBID1  OBJECT_INFORMATION(0x1400,0,2)
#define OBJECT_PDO_COBID2  OBJECT_INFORMATION(0x1401,0,2)

/*---------------------amplifier configure object--------------------------*/
#define OBJECT_DEVICE_TYPE                   OBJECT_INFORMATION(0x1000,0,2)
#define OBJECT_DEVICE_NAME                   OBJECT_INFORMATION(0x1008,0,2)
#define OBJECT_HARDWARE_VERSION              OBJECT_INFORMATION(0x1009,0,2)
#define OBJECT_AMPLIFIER_CONTINUOUS_CURRENT  OBJECT_INFORMATION(0x2384,4,2)
#define OBJECT_AMPLIFIER_PEAK_CURRENT        OBJECT_INFORMATION(0x2384,3,2)

/*--------------------network configure object-----------------------------*/
#define OBJECT_NETWORK_OPTION  OBJECT_INFORMATION(0x21B,0,2)

/*--------------------device control and status object--------------------*/
#define OBJECT_CONROL_WORD            OBJECT_INFORMATION(0x6040,0,2)
#define OBJECT_STATUS_WORD            OBJECT_INFORMATION(0x6041,0,2)
#define OBJECT_MANUFACTURER_STATUS    OBJECT_INFORMATION(0x1002,0,4)
#define OBJECT_NETWORK_STATUS         OBJECT_INFORMATION(0x21B4,0,2)
#define OBJECT_MODE_OF_OPERATION      OBJECT_INFORMATION(0x6060,0,1)
/*--------------------position mode operation-----------------------------*/
#define OBJECT_TRAJECTORY_JERK_LIMIT  OBJECT_INFORMATION(0x2121,0,4)  //units:100 counts/s
#define OBJECT_TRAJECTORY_GEN_STATUS  OBJECT_INFORMATION(0x2252,0,2)  //
#define OBJECT_TARGET_POSITION        OBJECT_INFORMATION(0x607A,0,4)  //units:counts
#define OBJECT_PROFILE_VELOCITY       OBJECT_INFORMATION(0x6081,0,4)  //units:0.1counts/s
#define OBJECT_PROFILE_ACCELERATION   OBJECT_INFORMATION(0x6083,0,4)  //units:10counts/s*s
#define OBJECT_PROFILE_DECELERATION   OBJECT_INFORMATION(0x6084,0,4)  //units:10counts/s*s
#define OBJECT_QUICK_STOP_DECELERAT   OBJECT_INFORMATION(0x6085,0,4)  //units:10counts/s*s
#define OBJECT_PROFILE_TYPE           OBJECT_INFORMATION(0x6086,0,2)  //

/*--------------------position loop configure operation---------------------*/
#define POSITION_ACTUAL_VALUE         OBJECT_INFORMATION(0x6063,0,4)

/******************************************************************************/
#define MOTION_PROFILE_TYPE_TRAPEZOIDDAL  0
#define MOTION_PROFILE_TYPE_S_CURVE  3
#define MOTION_PROFILE_TYPE_VELOCITY  -1

#define MODE_OF_OPERATION_POSITION 1
#define MODE_OF_OPERATION_VELOCITY 3
#define MODE_OF_OPERATION_TORQUE   4
#define MODE_OF_OPERATION_HOMEING  6

/*************************control word bit map for position mode****************/
#define CTRL_WORD_SWITCH_ON             0
#define CTRL_WORD_ENABLE_VOLTAGE        1
#define CTRL_WORD_QUICK_STOP            2
#define CTRL_WORD_ENABLE_OPERATION      3
#define CTRL_WORD_NEW_SETPOINT          4
#define CTRL_WORD_CHANGE_SET_IMMEDIALY  5
#define CTRL_WORD_ABS_RELATIVE          6  //0:move is based on home position 1: move is based on current position
#define CTRL_WORD_RESET_FAULT           7
#define CTRL_WORD_HALT                  8
#define CTRL_WORD_ALL                   0xFF
/***********************status word bit map for position mode******************/
#define  STATUS_READY_TO_SWITCH_ON     0
#define  STATUS_SWITCHED_ON            1
#define  STATUS_OPERATION_ENABLED      2
#define  STATUS_FAULT                 3
#define  STATUS_VOLTAGE_ENABLED        4
#define  STATUS_QUICK_STOP             5
#define  STATUS_SWITCH_ON_DISABLED     6
#define  STATUS_WARNING                7
#define  STATUS_TRAJECTORY_ABORT       8
#define  STATUS_REMOTE                 9
#define  STATUS_TARGET_REACHED         10
#define  STATUS_INTERNAL_LIMIT_ACTIVE  11
#define  STATUS_SETPOINT_ACKNOWLEDGE   12
#define  STATUS_FOLLOWING_ERROR        13

/******************************************************************************/
struct can_frame
{
  unsigned long int id;
  unsigned char buf[8];
  unsigned char len;
};

struct drive_status
{
  Uint16  can_failure;
  Uint16  status;
  Uint32  manufac_status;
  Uint16  network_status;	
};

struct position_mode_parameter
{
	Uint16 ptp_move_methods;// 0:discrete profile  1:continuous profile
	Uint16 profile_type;    // 0:trapezoid    1:s-curve
	Uint32 jerk_limit;      // 100counts/s^3
	int32  velocity;        // 0.1counts/s
	int32  acceleration;
	int32  deceleration;
	int32  quick_stop_deceleration;
};	



void can_rw_with_response_init();
static unsigned int can_tx_with_response(struct can_frame *frame,struct can_frame *rx_frame);

static unsigned long int sdo_upload_normal(Uint8 id,Uint32 ob_infor,Uint32 *data);
static unsigned long int sdo_download_normal(Uint8 id,Uint32 ob_infor,Uint32  data);

/*functions read and write drive by CAN */
static Uint32 canopen_set_object(Uint8 id,Uint32 object,Uint32 data);
static Uint32 canopen_get_object(Uint8 id,Uint32 object,Uint32 *data);

/*functions read and write drive by UART */
static Uint32 uart_set_para(Uint32 object,Uint32 data);
static Uint32 uart_get_para(Uint32 object,Uint32 *data);

Uint16 canopen_heart_beat_monitor();
void canopen_heart_beat_monitor_init();
Uint16 drive_get_control_word(Uint16 parameter);
Uint16 drive_get_status_word(Uint16 parameter);

/* functions for  main to call to access the drive */
Uint32 drive_position_mode_init(Uint8 id,struct position_mode_parameter *paramter);
Uint16 write_target_position(Uint8 id,Uint16 position);
Uint16 read_motor_prosition(Uint8 id,Uint16 *position);
Uint32 check_drive_status(Uint8 id);

struct position_mode_parameter *get_motor_pos_mode_para(Uint8 id);
struct drive_status *get_drive_status(Uint8 id);


#endif /*MOTORCANCTRL_H_*/
