
#ifndef SISDEFINES_H
#define SISDEFINES_H

/*
  SISDefines.h

  This header file contains defines for constants used in SIS3150USB.h and SIS3150USB.cpp. It also contains typedefs for
  the SIS library functions that we use to connect and access the sis3150usb board.

  */

/*
Data structure for Raw Event Data
*/

struct SISdata {
    unsigned long long timestamp;
    unsigned short rawdata[256];
    unsigned int adcid;
    unsigned int energy;
    double tfifty;
    double dt;
    bool neighborplus;
    bool neighborminus;
    bool pileup;
};

#define GL_UINT_RUNCHECKSTOPEVENTSFLAG_ON   1
#define GL_UINT_RUNCHECKSTOPEVENTSFLAG_OFF  0

#define version_1205



// --- Constants ------------------------------------------

#define OFF					 0
#define ON					 1

#define SET_BIT16			 0x10000
#define SET_BIT15			 0x8000
#define SET_BIT14			 0x4000
#define SET_BIT13			 0x2000
#define SET_BIT12			 0x1000
#define SET_BIT11			 0x800
#define SET_BIT10			 0x400
#define SET_BIT9			 0x200
#define SET_BIT8			 0x100
#define SET_BIT7			 0x80
#define SET_BIT6			 0x40
#define SET_BIT5			 0x20
#define SET_BIT4			 0x10
#define SET_BIT3			 0x8
#define SET_BIT2			 0x4
#define SET_BIT1			 0x2
#define SET_BIT0			 0x1


// Panel Open Positions

#define SYSTEM_STATUS_NULL        		 	0
#define SYSTEM_STATUS_MODULES_NOT_READY 	1		 // red
#define SYSTEM_STATUS_MODULES_READY			2		 // yellow
#define SYSTEM_STATUS_MODULES_CONFIGURED	3		 // green
#define SYSTEM_STATUS_MODULES_RUNNING		4		 // blue



//#define MAX_NO_OF_MODULES    	16
#define MAX_NO_OF_MODULES    	20
#define MAX_NO_OF_CHANNELS    	(MAX_NO_OF_MODULES * 8)


#define INDEX_MODULE1 			0
#define INDEX_MODULE2 			1
#define INDEX_MODULE3 			2
#define INDEX_MODULE4 			3
#define INDEX_MODULE5 			4
#define INDEX_MODULE6 			5
#define INDEX_MODULE7 			6
#define INDEX_MODULE8 			7
#define INDEX_MODULE9 			8
#define INDEX_MODULE10 			9
#define INDEX_MODULE11 			10
#define INDEX_MODULE12 			11
#define INDEX_MODULE13 			12
#define INDEX_MODULE14 			13
#define INDEX_MODULE15 			14
#define INDEX_MODULE16 			15
#define INDEX_MODULE17 			16
#define INDEX_MODULE18 			17
#define INDEX_MODULE19 			18
#define INDEX_MODULE20 			19

#define NO_OF_ADC_CHANNELS    	8

#define INDEX_ADC_CHANNEL1 		0
#define INDEX_ADC_CHANNEL2 		1
#define INDEX_ADC_CHANNEL3 		2
#define INDEX_ADC_CHANNEL4 		3
#define INDEX_ADC_CHANNEL5 		4
#define INDEX_ADC_CHANNEL6 		5
#define INDEX_ADC_CHANNEL7 		6
#define INDEX_ADC_CHANNEL8 		7







#define ENERGY_SPECTRUM_DISPLAY_FLOAT_FACTOR  	4.0
#define ENERGY_SPECTRUM_DISPLAY_FACTOR  		4
#define MAX_NOF_ENERGY_SPECTRUM  				(0x10000)   // 16 bit

#define ENERGY_PEAK_ZOOM_VALUE	(50*ENERGY_SPECTRUM_DISPLAY_FACTOR)



#define MAX_PATHNAME_LENGTH 	1024
#define MAX_FILENAME_LENGTH 	1024  //including Pathname


#define NORMAL_SIS3150USB_TS_READOUT_LOADER_FILE  	"vme_readout.ldr"
#define NORMAL_TS_LOADER_FILE  						"gamma_adc_prog.ldr"
#define NORMAL_ADC_TS_LOADER_FILE  					"gamma_adc_prog.ldr"

#define NORMAL_CONFIGURATION_FILE  					"sis3302_gamma_gui_config.ini"



#define STRING_CONFIGURATION_FILE					"SIS3302 Gamma CONFIGURATION File"
#define STRING_DATE  								"Date: "
#define STRING_TIME  								"   Time: "

#define STRING_ADC_MODULES  						"ADC_MODULES"
#define STRING_CLOCK_MODULE  						"CLOCK_MODULE"

#define STRING_CLOCK_MODE  							"CLOCK_MODE"
#define STRING_SIS3302_MODES  						"SIS3302_MODES"


#define STRING_SIS3150USB_TS_LOADER_FILE  			"SIS3150USB_TS_Readout_Loader_File"
#define STRING_SAMPLING_ENERGY  					"SAMPLING_ENERGY"


#define STRING_ADC_SIGNAL_INVERT_FLAGS 				"ADC_SIGNAL_INVERT_FLAGS"
#define STRING_ADC_GAIN_FACTOR_FLAGS 				"ADC_GAIN_FACTOR_FLAGS"
#define STRING_ADC_NEXT_NEIGHBOR_ENABLE_FLAGS		"ADC_NEXT_NEIGHBOR_ENABLE_FLAGS"
#define STRING_DAC_OFFSET_VALUES  					"DAC_OFFSET_VALUES"
#define STRING_FIR_TRIGGER_VALUES  					"FIR_TRIGGER_VALUES"
#define STRING_ENERGY_FACTOR_VALUES 				"ENERGY_FACTOR_VALUES"
#define STRING_DECAY_TAU_VALUES						"DECAY_TAU_VALUES"
#define STRING_ENERGY_PULSE_INVERT_FLAGS			"ENERGY_PULSE_INVERT_FLAGS"
#define STRING_SIS3150USB_TRIGGER_READOUT_MATRIX	"SIS3150USB_TRIGGER_READOUT_MATRIX"
#define STRING_RUN_READOUT_MODE						"RUN_READOUT_MODE"


#define STRING_BLANK				" "
#define STRING_END_OF_FILE			"END_OF_FILE"

#define FILE_ACCESS_OK  			0
#define FILE_ACCESS_NOT_OK  		-1

#define STATUS_YES  	  			0
#define STATUS_NO			  		-1

#define PRINT_DATE_YES 	  			0
#define PRINT_DATE_NO 	  			-1


// default configuration values
#define CLOCK_MODULE_CONF_DEFAULT_ADDRESS   		0x38000000

#define ADC_MODULE_CONF_DEFAULT_STARTADDRESS   		0x40000000
#define ADC_MODULE_CONF_DEFAULT_ADDRESSINC   		0x10000000
#define ADC_MODULE_CONF_DEFAULT_BROADCAST_ADDRESS  	0x30000000


// Common Clock
#define CLOCK_SIS3820_CONF_DEFAULT_CLOCK_MODE   		0x000000
#define CLOCK_SIS3820_CONF_DEFAULT_CTRL4_ENABLE_FLAG   	0
#define CLOCK_SIS3820_CONF_DEFAULT_CTRL3_ENABLE_FLAG   	0
#define CLOCK_SIS3820_CONF_DEFAULT_CTRL2_ENABLE_FLAG   	0
#define CLOCK_SIS3820_CONF_DEFAULT_TRIGGERMASK   		0x7F



#define CLOCK_SIS3302_CONF_DEFAULT_CLOCK_MODE   					4 //

// SIS3320 Modes
#define SIS3302_CONF_DEFAULT_TRIGGER_MODE_SOURCE   					0 //
#define SIS3302_CONF_DEFAULT_DUMMY_MODE		   						0 //
#define SIS3302_CONF_DEFAULT_LEMO_TRIGGER_ENABLE_FLAG   			0
#define SIS3302_CONF_DEFAULT_LEMO_TIMSTAMPCLR_ENABLE_FLAG   		0


// Sampling and Energy Parameters

#define FIR_ENERGY_PEAKING_DEFAULT  	 	150
#define FIR_ENERGY_GAP_DEFAULT    			50
#define FIR_ENERGY_DECIMATION_MODE_DEFAULT	1
#define FIR_ENERGY_MW_SHAPING_TIME_DEFAULT	255

#define RAW_PRE_SAMPLE_LENGTH_DEFAULT  	 	16
#define RAW_SAMPLE_LENGTH_DEFAULT  	 		48
#define ENERGY_SAMPLE_MODE_DEFAULT			1
#define ENERGY_SAMPLE_ADR1_DEFAULT			1
#define ENERGY_SAMPLE_ADR2_DEFAULT			0
#define ENERGY_SAMPLE_ADR3_DEFAULT			0
#define ENERGY_SAMPLE_LENGTH_DEFAULT		510
#define TRIGGER_WINDOW_ACTIVE_DEFAULT  	 	100
#define TRIGGER_GATE_LENGTH_DEFAULT  	 	256
#define PRETIGGER_DELAY_DEFAULT  	 	    128

// DAC Offset
#define DAC_OFFSET_DEFAULT  			 	38144



// Trigger
#define FIR_TRIGGER_PEAKING_DEFAULT     	10
#define FIR_TRIGGER_GAP_DEFAULT    			30
#define FIR_TRIGGER_PULSE_LENGTH_DEFAULT  	20

#define FIR_TRIGGER_THRESHOLD_CONF_DEFAULT_VALUE   	-1000

#define FIR_TRIGGER_THRESHOLD_CONF_DISABLE      	0x0
#define FIR_TRIGGER_THRESHOLD_CONF_GT_OUT_ENABLE	0x1
#define FIR_TRIGGER_THRESHOLD_CONF_GT_OUT_DISABLE	0x2
#define FIR_TRIGGER_THRESHOLD_CONF_GT_EXTEND_OUT_ENABLE	0x3
#define FIR_TRIGGER_THRESHOLD_CONF_GT_EXTEND_OUT_DISABLE	0x4


// begin mod 08.11.2010
#define FIR_TRIGGER_CFD_CONF_DISABLE      	0x0
#define FIR_TRIGGER_CFD_CONF_75_ENABLE		0x1
#define FIR_TRIGGER_CFD_CONF_50_ENABLE		0x2
// end mod 08.11.2010


// Energy Factor (default)
#define ENERGY_FACTOR_DEFAULT  				1.52

// Energy Factor (default)
//#define DECAY_FACTOR_DEFAULT  				5.00
#define DECAY_TAU_FACTOR_DEFAULT  				0
#define PULSE_INVERT_FLAG_DEFAULT  				0

#define RUN_READOUT_MODE_DEFAULT  				1



#define ADC_SIGNAL_NOT_INVERT      		0x0
#define FIR_ENERGY_PULSE_NOT_INVERT      	0x0



#define AQC_STOP_STATUS		  		0
#define AQC_RUN_STATUS		  		1



// Running Modes

#define RUNNING_MODE_PC_SINGLE_EVENT		  					0
#define RUNNING_MODE_PC_MULTI_EVENT		  						1
#define RUNNING_MODE_PC_SIS3150USB_DSP_MULTI_EVENT				2
#define RUNNING_MODE_PC_MULTI_EVENT_ASYN_MODE					3
#define RUNNING_MODE_PC_SIS3150USB_DSP_MULTI_EVENT_ASYN_MODE	4


// data file
#define FILE_FORMAT_EVENT_HEADER        	0xABBAABBA
#define FILE_FORMAT_EOF_TRAILER        		0x0E0F0E0F




#define NO_OF_PANEL			 		60	  // Anzahl der Fenster im UIR-File

#define	MAIN				 		0	  // Haupt-Fenster


#define MODULE_MESS_SYSTEM	 		1
#define MODULE_MESS_ERROR	 		2

#define MODULE_PARA_CONF	 		3
#define DAC_PARA_CONF	 			4
#define CLOCK_PARA_CONF		 		5


#define TRIG_PARA_CONF	 			6
#define SAMPLE_PARA_CONF	 		7
#define ENERGY_FACTOR_CONF	 		8
#define DECAY_FACTOR_CONF	 		9
#define TRIGGER_READOUT_MATRIX_CONF	10

#define RUN_CTRL_MENUE       		11	  //
#define RUN_MCA_MENUE       		12	  //
#define OFFLINE_CONTROL       		13	  //



#define SIGNAL_INVERT_CONF	 		14
#define GAIN_FACTOR_CONF	 		15



// sis3150usb.h
#define SIS3150USB_VME_MASTER_CONTROL_STATUS	0x10


/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
///////////////SIS Library Methods///////////////////////////////////////////////

/*
  SIS Library Methods
  Our code dynamically uses methods given by SIS. For Windows, this means that we use sis3150usb_vme_win.dll. For linux,
  we use libsis3150.so in a /usr/opt/sisusb/lib/ directory. These differences are documented in the code.

  For Qt, we create method pointers for the methods that we want to extract from the dynamic library (.dll in Windows and
  .so in Linux). These method pointers are declared below for the corresponding methods in the dynamic library. These
  methods are "gotten" in InitLibraries, where we resolve the methods. See InitLibraries() for more information.
  */


// typedefs needed in function prototypes (were originally in <windows.h>)
//typedef unsigned long ULONG;
//typedef ULONG * PULONG;
#include "GRIDefines.h"

#if (OPERATING_SYSTEM == WINDOWS)
#include "Windows.h"
#else
typedef uint32_g ULONG;
typedef ULONG * PULONG;
typedef char CHARS;
typedef CHARS *PCHAR;
typedef void * HANDLE;
#endif

typedef unsigned long uint32;

// Library function prototypes (sis3150usb_vme_win.dll)
typedef int (*sis3150Usb_Register_Single_Read_PROT)(HANDLE, ULONG, ULONG*);
typedef int (*FindAll_SIS3150USB_Devices_PROT)(struct SIS3150USB_Device_Struct* sis3150usb_Device,
                                               unsigned int *nof_usbdevices, unsigned int max_usb_device_Number);
typedef int (*Sis3150usb_OpenDriver_And_Download_FX2_Setup_PROT)(PCHAR usbDeviceName, HANDLE *usbDeviceHandle );
typedef int (*sis3150Usb_Register_Single_Write_PROT)(HANDLE usbDevice, ULONG addr, ULONG data);
typedef int (*sis3150Usb_Vme_Dma_Read_PROT)(HANDLE usbDevice, // used for sub_vme_A32MBLT64_read
                                            ULONG addr,
                                            ULONG am,
                                            ULONG size,
                                            ULONG fifo_mode,
                                            ULONG* dmabufs,
                                            ULONG req_nof_data,
                                            ULONG* got_nof_data);
typedef int (*sis3150Usb_Vme_Single_Read_PROT)(HANDLE usbDevice, ULONG addr, ULONG am, ULONG size, ULONG* data) ;
typedef int (*sis3150Usb_Vme_Single_Write_PROT)(HANDLE usbDevice, ULONG addr, ULONG am, ULONG size, ULONG data);


///////////////SIS Library Methods - END ////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////


/**************Default Inputs******************/


#define DEFAULT_MAX_EVENTS              1
#define DEFAULT_MAX_EVENTS_ENABLED_FLAG 1

#define DEFAULT_CHECK_ON_MCA_MODE       0       // temp.cpp
#define DEFAULT_UINT_MCA_HISTO_SIZE     1000

#define DEFAULT_RUN_START_ACQ_EVENT     1       // was EVENT_COMMIT
#define UNUSED_VAR                      0

#define WRITE_DATA_TO_FILE              0       // 1 == true, 0 == false
#define DEFAULT_UINT_BASEMAX            0

// Histogramming - the below values PROBABLY DO NOT WORK
#define DEFAULT_ENERGY_MODE             0
#define DEFAULT_FIND_FROM_INDEX1        0
#define DEFAULT_FIND_FROM_INDEX2        0
#define DEFAULT_FIND_AVERAGE            1


/**************Default Inputs End**************/


/*****************CVI Globals******************/
#define SIS3150USB_VME_INTERRUPT_STATUS			0x12


/*****************CVI Globals END**************/

#define DATE_LENGTH 11
#define TIME_LENGTH 9


/**********************************************************************/
// File path length

// for this system:
#define _NI_mswin32_    // will be different depending on what system you use

// if-else code
#if defined(_NI_mswin16_)
#define MAX_PATHNAME_LEN   80    /* includes nul byte */
#define MAX_DRIVENAME_LEN   3    /* includes nul byte */
#define MAX_DIRNAME_LEN    66    /* includes nul byte */
#define MAX_FILENAME_LEN   13    /* includes nul byte */

#elif defined(_NI_mswin32_) || defined(_NI_mswin64_)
#define MAX_PATHNAME_LEN  260    /* includes nul byte */
#define MAX_DRIVENAME_LEN   3    /* includes nul byte */
#define MAX_DIRNAME_LEN   256    /* includes nul byte */
#define MAX_FILENAME_LEN  256    /* includes nul byte */

#elif defined(_NI_linux_)
#define MAX_PATHNAME_LEN 1024    /* recommended buffer size for paths */
#define MAX_DRIVENAME_LEN   1    /* includes nul byte */
#define MAX_DIRNAME_LEN   256    /* includes nul byte */
#define MAX_FILENAME_LEN  256    /* recommended buffer size for file names */

#elif defined(_NI_unix_)
#define MAX_PATHNAME_LEN  256    /* recommended buffer size for paths */
#define MAX_DRIVENAME_LEN   1    /* includes nul byte */
#define MAX_DIRNAME_LEN   256    /* includes nul byte */
#define MAX_FILENAME_LEN  256    /* recommended buffer size for file names */

#else
#error Undefined Platform. You need to add one of the
#error following to your compiler defines:
#error     Platform                      Preprocessor directive
#error Microsoft Windows 3.1           #define _NI_mswin16_
#error Windows 95/NT                   #define _NI_mswin32_
#error Solaris 1                       #define _NI_sparc_       1
#error Solaris 2                       #define _NI_sparc_       2
#error HP-UX                           #define _NI_hpux_
#error Linux x86                       #define _NI_linux_
#error
#error _NI_i386_ has been replaced with _NI_mswin16_.
#error See Programmers Reference Manual for more information.


#endif

/**********************************************************************/



typedef void * HANDLE;

// PC interface
#define USB_VME_INTERFACE		// sis3150USB USB2.0 interface

extern struct SIS1100_Device_Struct  gl_sis1100_device ;

#ifdef USB_VME_INTERFACE

#define _DEBUG
#define PLX_9054
#define PCI_CODE
#define LITTLE_ENDIAN

#define MAX_FILE_SIZE (1024*64)     //max size of download file
#define MAX_USB_DEV_NUMBER 4
#define MAX_DRIVER_NAME 64

// added by RTGIF
typedef unsigned char UCHAR;
typedef unsigned short USHORT;


struct SIS3150USB_Device_Struct {	   //Aucott 6/4 as per Zheng
    UCHAR	cDName[128];
    USHORT	idVendor;
    USHORT	idProduct;
    USHORT	idSerNo;
    USHORT	idFirmwareVersion;
} ;


// DLL support
#ifndef EXPORT
#define EXPORT __declspec(dllexport)
#endif

/* Replaced by dynamic loading with initlibraries
#ifdef __cplusplus
extern "C" {
#endif


// Functions in sis3150usb_vme_win.dll
int EXPORT FindAll_SIS3150USB_Devices(struct SIS3150USB_Device_Struct* sis3150usb_Device, unsigned int *nof_usbdevices, unsigned int max_usb_device_Number) ;

int EXPORT Sis3150usb_OpenDriver (PCHAR usbDeviceName, HANDLE *usbDeviceHandle );
int EXPORT Sis3150usb_OpenDriver_And_Download_FX2_Setup (PCHAR usbDeviceName, HANDLE *usbDeviceHandle ) ;
int EXPORT Sis3150usb_CloseDriver(HANDLE usbDevice);
int EXPORT Sis3150usb_version(ULONG *data);
int EXPORT sis3150Usb_load_dsp(HANDLE usbDevice,char* dsppath);




int EXPORT sis3150Usb_Register_Single_Read(HANDLE usbDevice, ULONG addr, ULONG* data) ;
int EXPORT sis3150Usb_Register_Dma_Read(HANDLE usbDevice, ULONG addr, ULONG* dmabufs,ULONG req_nof_data, ULONG* got_nof_data) ;

int EXPORT sis3150Usb_Register_Single_Write(HANDLE usbDevice, ULONG addr, ULONG data) ;
int EXPORT sis3150Usb_Register_Dma_Write(HANDLE usbDevice, ULONG addr,ULONG* dmabufs, ULONG req_nof_data, ULONG* put_nof_data) ;


int EXPORT sis3150Usb_TsBus_Single_Read(HANDLE usbDevice, ULONG addr, ULONG* data) ;
int EXPORT sis3150Usb_TsBus_Dma_Read(HANDLE usbDevice, ULONG addr, ULONG* dmabufs,ULONG req_nof_data, ULONG* got_nof_data) ;

int EXPORT sis3150Usb_TsBus_Single_Write(HANDLE usbDevice, ULONG addr, ULONG data) ;
int EXPORT sis3150Usb_TsBus_Dma_Write(HANDLE usbDevice, ULONG addr,ULONG* dmabufs, ULONG req_nof_data, ULONG* put_nof_data) ;


int EXPORT sis3150Usb_Vme_Single_Read(HANDLE usbDevice, ULONG addr, ULONG am, ULONG size, ULONG* data) ;
int EXPORT sis3150Usb_Vme_Dma_Read(HANDLE usbDevice,
                                ULONG addr,
                                ULONG am,
                                ULONG size,
                                ULONG fifo_mode,
                                ULONG* dmabufs,
                                ULONG req_nof_data,
                                ULONG* got_nof_data) ;


int EXPORT sis3150Usb_Vme_Single_Write(HANDLE usbDevice, ULONG addr, ULONG am, ULONG size, ULONG data);

int EXPORT sis3150Usb_Vme_Dma_Write(HANDLE usbDevice, ULONG addr, ULONG am, ULONG size, ULONG fifo_mode,
                            ULONG* dmabufs, ULONG req_nof_data, ULONG* put_nof_data);


int EXPORT sis3150Usb_VmeSysreset(HANDLE usbDevice) ;



#ifdef __cplusplus
}
#endif
*/

//extern HANDLE  gl_USBDevice ;


#endif



/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
///////////////////SIS3820 CLOCK/////////////////////////////////


/* addresses  */

#define SIS3820CLOCK_CONTROL_STATUS			0x0		/* read/write; D32 */
#define SIS3820CLOCK_MODID				0x4		/* read only; D32 */
#define SIS3820CLOCK_CLOCK_SOURCE			0x10		/* read/write; D32 */
#define SIS3820CLOCK_TRIGGERMASK			0x14		/* read/write  D32 */
#define SIS3820_LNE_PRESCALE				0x18		/* read/write; D32 */

#define SIS3820CLOCK_KEY_RESET				0x60
#define SIS3820CLOCK_KEY_TRIGGER			0x64
#define SIS3820CLOCK_KEY_CLR_TIMESTAMP			0x68





/* bit definitions  */
#define SIS3820CLOCK_GENERAL_DISABLE			0x80000000
#define SIS3820CLOCK_GENERAL_ENABLE			0x8000

#define SIS3820CLOCK_CLR_TRIGGER_VETO			0x40000000
#define SIS3820CLOCK_SET_TRIGGER_VETO			0x4000


#define SIS3820CLOCK_EXT_CLR_TIMESTAMP_DISABLE          0x10000000
#define SIS3820CLOCK_EXT_CLR_TIMESTAMP_ENABLE           0x1000

#define SIS3820CLOCK_EXT_TRIGGER_IN_DISABLE		0x8000000
#define SIS3820CLOCK_EXT_TRIGGER_IN_ENABLE		0x800

#define SIS3820CLOCK_EXT_VETO_IN_DISABLE		0x4000000
#define SIS3820CLOCK_EXT_VETO_IN_ENABLE			0x400

#define SIS3820CLOCK_FP_CLOCK_OUT_DISABLE		0x800000
#define SIS3820CLOCK_FP_CLOCK_OUT_ENABLE		0x80

#define SIS3820CLOCK_P2_OUT_DISABLE			0x400000
#define SIS3820CLOCK_P2_OUT_ENABLE			0x40


#define SIS3820CLOCK_CLOCK_SOURCE_100MHZ		0x0
#define SIS3820CLOCK_CLOCK_SOURCE_80MHZ			0x1
#define SIS3820CLOCK_CLOCK_SOURCE_EXT_CTRL		0x2

#define SIS3820CLOCK_CLOCK_DIVIDE_1			0x0
#define SIS3820CLOCK_CLOCK_DIVIDE_2			0x10
#define SIS3820CLOCK_CLOCK_DIVIDE_4			0x20







#define SIS3820_IRQ_SOURCE0_ENABLE				0x1
#define SIS3820_IRQ_SOURCE1_ENABLE				0x2
#define SIS3820_IRQ_SOURCE2_ENABLE				0x4
#define SIS3820_IRQ_SOURCE3_ENABLE				0x8
#define SIS3820_IRQ_SOURCE4_ENABLE				0x10
#define SIS3820_IRQ_SOURCE5_ENABLE				0x20
#define SIS3820_IRQ_SOURCE6_ENABLE				0x40
#define SIS3820_IRQ_SOURCE7_ENABLE				0x80

#define SIS3820_IRQ_SOURCE0_DISABLE				0x100
#define SIS3820_IRQ_SOURCE1_DISABLE				0x200
#define SIS3820_IRQ_SOURCE2_DISABLE				0x400
#define SIS3820_IRQ_SOURCE3_DISABLE				0x800
#define SIS3820_IRQ_SOURCE4_DISABLE				0x1000
#define SIS3820_IRQ_SOURCE5_DISABLE				0x2000
#define SIS3820_IRQ_SOURCE6_DISABLE				0x4000
#define SIS3820_IRQ_SOURCE7_DISABLE				0x8000

#define SIS3820_IRQ_SOURCE0_CLEAR				0x10000
#define SIS3820_IRQ_SOURCE1_CLEAR				0x20000
#define SIS3820_IRQ_SOURCE2_CLEAR				0x40000
#define SIS3820_IRQ_SOURCE3_CLEAR				0x80000
#define SIS3820_IRQ_SOURCE4_CLEAR				0x100000
#define SIS3820_IRQ_SOURCE5_CLEAR				0x200000
#define SIS3820_IRQ_SOURCE6_CLEAR				0x400000
#define SIS3820_IRQ_SOURCE7_CLEAR				0x800000

#define SIS3820_IRQ_SOURCE0_FLAG				0x1000000
#define SIS3820_IRQ_SOURCE1_FLAG				0x2000000
#define SIS3820_IRQ_SOURCE2_FLAG				0x4000000
#define SIS3820_IRQ_SOURCE3_FLAG				0x8000000
#define SIS3820_IRQ_SOURCE4_FLAG				0x10000000
#define SIS3820_IRQ_SOURCE5_FLAG				0x20000000
#define SIS3820_IRQ_SOURCE6_FLAG				0x40000000
#define SIS3820_IRQ_SOURCE7_FLAG				0x80000000


/* Control register bit defintions */

#define CTRL_USER_LED_OFF				0x10000	   /* default after Reset */
#define CTRL_USER_LED_ON				0x1


#define CTRL_COUNTER_TEST_MODE_DISABLE			0x200000
#define CTRL_COUNTER_TEST_MODE_ENABLE			0x20

#define CTRL_REFERENCE_CH1_DISABLE			0x400000
#define CTRL_REFERENCE_CH1_ENABLE			0x40


/* Status register bit defintions */

#define STAT_OPERATION_SCALER_ENABLED			0x10000
#define STAT_OPERATION_MCS_ENABLED			0x40000
#define STAT_OPERATION_VME_WRITE_ENABLED		0x800000



/* Acqusition / Mode register bit defintions */
#define SIS3820_CLEARING_MODE				0x0
#define SIS3820_NON_CLEARING_MODE			0x1

#define SIS3820_MCS_DATA_FORMAT_32BIT			0x0
#define SIS3820_MCS_DATA_FORMAT_24BIT			0x4
#define SIS3820_MCS_DATA_FORMAT_16BIT			0x8
#define SIS3820_MCS_DATA_FORMAT_8BIT			0xC

#define SIS3820_SCALER_DATA_FORMAT_32BIT		0x0
#define SIS3820_SCALER_DATA_FORMAT_24BIT		0x4

#define SIS3820_LNE_SOURCE_VME				0x0
#define SIS3820_LNE_SOURCE_CONTROL_SIGNAL		0x10
#define SIS3820_LNE_SOURCE_INTERNAL_10MHZ		0x20
#define SIS3820_LNE_SOURCE_CHANNEL_N			0x30
#define SIS3820_LNE_SOURCE_PRESET			0x40

#define SIS3820_ARM_ENABLE_CONTROL_SIGNAL		0x000
#define SIS3820_ARM_ENABLE_CHANNEL_N			0x100

#define SIS3820_FIFO_MODE				0x0000
#define SIS3820_SDRAM_MODE				0x1000

#define SIS3820_CONTROL_INPUT_MODE0			0x00000
#define SIS3820_CONTROL_INPUT_MODE1			0x10000
#define SIS3820_CONTROL_INPUT_MODE2			0x20000
#define SIS3820_CONTROL_INPUT_MODE3			0x30000
#define SIS3820_CONTROL_INPUT_MODE4			0x40000

#define SIS3820_CONTROL_INPUTS_INVERT			0x80000

#define SIS3820_CONTROL_OUTPUT_MODE0			0x000000
#define SIS3820_CONTROL_OUTPUT_MODE1			0x100000

#define SIS3820_CONTROL_OUTPUTS_INVERT			0x800000


#define SIS3820_OP_MODE_SCALER				0x00000000
#define SIS3820_OP_MODE_MULTI_CHANNEL_SCALER            0x20000000
#define SIS3820_OP_MODE_VME_FIFO_WRITE			0x70000000


/* preset enable/hit register */
#define SIS3820_PRESET_STATUS_ENABLE_GROUP1		0x1
#define SIS3820_PRESET_REACHED_GROUP1			0x2
#define SIS3820_PRESET_LNELATCHED_REACHED_GROUP1	0x4
#define SIS3820_PRESET_STATUS_ENABLE_GROUP2		0x10000
#define SIS3820_PRESET_REACHED_GROUP2			0x20000
#define SIS3820_PRESET_LNELATCHED_REACHED_GROUP2	0x40000

/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////

/***************************************************************/

/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
/////////////////////SIS3302_V1406///////////////////////////////


// SIS3302 Standard
#define SIS3302_CONTROL_STATUS                       0x0	  /* read/write; D32 */
#define SIS3302_MODID                                0x4	  /* read only; D32 */
#define SIS3302_IRQ_CONFIG                           0x8      /* read/write; D32 */
#define SIS3302_IRQ_CONTROL                          0xC      /* read/write; D32 */
#define SIS3302_ACQUISITION_CONTROL                  0x10      /* read/write; D32 */


#define SIS3302_CBLT_BROADCAST_SETUP                0x30      /* read/write; D32 */
#define SIS3302_ADC_MEMORY_PAGE_REGISTER            0x34      /* read/write; D32 */

#define SIS3302_DAC_CONTROL_STATUS                  0x50      /* read/write; D32 */
#define SIS3302_DAC_DATA                            0x54      /* read/write; D32 */




// Key Addresses
#define SIS3302_KEY_RESET                           			0x400	  /* write only; D32 */

#define SIS3302_KEY_0x404_SAMPLE_LOGIC_RESET           			0x404	  /* write only; D32 */

#define SIS3302_KEY_SAMPLE_LOGIC_RESET              			0x410	  /* write only; D32 */
#define SIS3302_KEY_DISARM                         			0x414	  /* write only; D32 */
#define SIS3302_KEY_TRIGGER                         			0x418	  /* write only; D32 */
#define SIS3302_KEY_TIMESTAMP_CLEAR                 			0x41C	  /* write only; D32 */
#define SIS3302_KEY_DISARM_AND_ARM_BANK1           			0x420	  /* write only; D32 */
#define SIS3302_KEY_DISARM_AND_ARM_BANK2           			0x424	  /* write only; D32 */

#define SIS3302_KEY_RESET_DDR2_LOGIC                			0x428	  /* write only; D32 */


// all ADC FPGA groups
#define SIS3302_EVENT_CONFIG_ALL_ADC                			0x01000000
#define SIS3302_END_ADDRESS_THRESHOLD_ALL_ADC       			0x01000004	    /* Gamma */
#define SIS3302_PRETRIGGER_DELAY_TRIGGERGATE_LENGTH_ALL_ADC             0x01000008	    /* Gamma */
#define SIS3302_RAW_DATA_BUFFER_CONFIG_ALL_ADC         			0x0100000C	    /* Gamma */

#define SIS3302_ENERGY_SETUP_GP_ALL_ADC   				0x01000040      /* Gamma */
#define SIS3302_ENERGY_GATE_LENGTH_ALL_ADC				0x01000044      /* Gamma */
#define SIS3302_ENERGY_SAMPLE_LENGTH_ALL_ADC                            0x01000048      /* Gamma */
#define SIS3302_ENERGY_SAMPLE_START_INDEX1_ALL_ADC			0x0100004C      /* Gamma */
#define SIS3302_ENERGY_SAMPLE_START_INDEX2_ALL_ADC			0x01000050      /* Gamma */
#define SIS3302_ENERGY_SAMPLE_START_INDEX3_ALL_ADC			0x01000054      /* Gamma */
#define SIS3302_ENERGY_TAU_FACTOR_ADC1357				0x01000058      /* Gamma */
#define SIS3302_ENERGY_TAU_FACTOR_ADC2468				0x0100005C      /* Gamma */

#define SIS3302_EVENT_EXTENDED_CONFIG_ALL_ADC          			0x01000070



// ADC12 FPGA group
#define SIS3302_EVENT_CONFIG_ADC12                				0x02000000 		/* Gamma */
#define SIS3302_END_ADDRESS_THRESHOLD_ADC12      	 			0x02000004	    /* Gamma */
#define SIS3302_PRETRIGGER_DELAY_TRIGGERGATE_LENGTH_ADC12  		0x02000008	    /* Gamma */
#define SIS3302_RAW_DATA_BUFFER_CONFIG_ADC12       				0x0200000C	    /* Gamma */

#define SIS3302_ACTUAL_SAMPLE_ADDRESS_ADC1          			0x02000010
#define SIS3302_ACTUAL_SAMPLE_ADDRESS_ADC2          			0x02000014
#define SIS3302_PREVIOUS_BANK_SAMPLE_ADDRESS_ADC1   			0x02000018
#define SIS3302_PREVIOUS_BANK_SAMPLE_ADDRESS_ADC2   			0x0200001C

#define SIS3302_ACTUAL_SAMPLE_VALUE_ADC12           			0x02000020
#define SIS3302_DDR2_TEST_REGISTER_ADC12						0x02000028

#define SIS3302_TRIGGER_SETUP_ADC1                  			0x02000030
#define SIS3302_TRIGGER_THRESHOLD_ADC1              			0x02000034
#define SIS3302_TRIGGER_SETUP_ADC2                  			0x02000038
#define SIS3302_TRIGGER_THRESHOLD_ADC2              			0x0200003C

#define SIS3302_ENERGY_SETUP_GP_ADC12   						0x02000040      /* Gamma */
#define SIS3302_ENERGY_GATE_LENGTH_ADC12						0x02000044      /* Gamma */
#define SIS3302_ENERGY_SAMPLE_LENGTH_ADC12						0x02000048      /* Gamma */
#define SIS3302_ENERGY_SAMPLE_START_INDEX1_ADC12				0x0200004C      /* Gamma */

#define SIS3302_ENERGY_SAMPLE_START_INDEX2_ADC12				0x02000050      /* Gamma */
#define SIS3302_ENERGY_SAMPLE_START_INDEX3_ADC12				0x02000054      /* Gamma */
#define SIS3302_ENERGY_TAU_FACTOR_ADC1							0x02000058      /* Gamma */
#define SIS3302_ENERGY_TAU_FACTOR_ADC2							0x0200005C      /* Gamma */

#define SIS3302_EVENT_EXTENDED_CONFIG_ADC12       				0x02000070
#define SIS3302_TRIGGER_EXTENDED_SETUP_ADC1             		0x02000078
#define SIS3302_TRIGGER_EXTENDED_SETUP_ADC2             		0x0200007C


#define SIS3302_TRIGGER_EXTENDED_THRESHOLD_ADC1            		0x020000A0
#define SIS3302_TRIGGER_EXTENDED_THRESHOLD_ADC2            		0x020000A4



 // ADC34 FPGA group
#define SIS3302_EVENT_CONFIG_ADC34                				0x02800000 		/* Gamma */
#define SIS3302_END_ADDRESS_THRESHOLD_ADC34      	 			0x02800004	    /* Gamma */
#define SIS3302_PRETRIGGER_DELAY_TRIGGERGATE_LENGTH_ADC34  		0x02800008	    /* Gamma */
#define SIS3302_RAW_DATA_BUFFER_CONFIG_ADC34       				0x0280000C	    /* Gamma */

#define SIS3302_ACTUAL_SAMPLE_ADDRESS_ADC3          			0x02800010
#define SIS3302_ACTUAL_SAMPLE_ADDRESS_ADC4          			0x02800014
#define SIS3302_PREVIOUS_BANK_SAMPLE_ADDRESS_ADC3   			0x02800018
#define SIS3302_PREVIOUS_BANK_SAMPLE_ADDRESS_ADC4   			0x0280001C

#define SIS3302_ACTUAL_SAMPLE_VALUE_ADC34           			0x02800020
#define SIS3302_DDR2_TEST_REGISTER_ADC34						0x02800028

#define SIS3302_TRIGGER_SETUP_ADC3                  			0x02800030
#define SIS3302_TRIGGER_THRESHOLD_ADC3              			0x02800034
#define SIS3302_TRIGGER_SETUP_ADC4                  			0x02800038
#define SIS3302_TRIGGER_THRESHOLD_ADC4              			0x0280003C

#define SIS3302_ENERGY_SETUP_GP_ADC34   						0x02800040      /* Gamma */
#define SIS3302_ENERGY_GATE_LENGTH_ADC34						0x02800044      /* Gamma */
#define SIS3302_ENERGY_SAMPLE_LENGTH_ADC34						0x02800048      /* Gamma */
#define SIS3302_ENERGY_SAMPLE_START_INDEX1_ADC34				0x0280004C      /* Gamma */

#define SIS3302_ENERGY_SAMPLE_START_INDEX2_ADC34				0x02800050      /* Gamma */
#define SIS3302_ENERGY_SAMPLE_START_INDEX3_ADC34				0x02800054      /* Gamma */
#define SIS3302_ENERGY_TAU_FACTOR_ADC3							0x02800058      /* Gamma */
#define SIS3302_ENERGY_TAU_FACTOR_ADC4							0x0280005C      /* Gamma */

#define SIS3302_EVENT_EXTENDED_CONFIG_ADC34       				0x02800070
#define SIS3302_TRIGGER_EXTENDED_SETUP_ADC3             		0x02800078
#define SIS3302_TRIGGER_EXTENDED_SETUP_ADC4             		0x0280007C

#define SIS3302_TRIGGER_EXTENDED_THRESHOLD_ADC3            		0x028000A0
#define SIS3302_TRIGGER_EXTENDED_THRESHOLD_ADC4            		0x028000A4



// ADC56 FPGA group
#define SIS3302_EVENT_CONFIG_ADC56                				0x03000000 		/* Gamma */
#define SIS3302_END_ADDRESS_THRESHOLD_ADC56      	 			0x03000004	    /* Gamma */
#define SIS3302_PRETRIGGER_DELAY_TRIGGERGATE_LENGTH_ADC56  		0x03000008	    /* Gamma */
#define SIS3302_RAW_DATA_BUFFER_CONFIG_ADC56       				0x0300000C	    /* Gamma */

#define SIS3302_ACTUAL_SAMPLE_ADDRESS_ADC5          			0x03000010
#define SIS3302_ACTUAL_SAMPLE_ADDRESS_ADC6          			0x03000014
#define SIS3302_PREVIOUS_BANK_SAMPLE_ADDRESS_ADC5   			0x03000018
#define SIS3302_PREVIOUS_BANK_SAMPLE_ADDRESS_ADC6   			0x0300001C

#define SIS3302_ACTUAL_SAMPLE_VALUE_ADC56           			0x03000020
#define SIS3302_DDR2_TEST_REGISTER_ADC56						0x03000028

#define SIS3302_TRIGGER_SETUP_ADC5                  			0x03000030
#define SIS3302_TRIGGER_THRESHOLD_ADC5              			0x03000034
#define SIS3302_TRIGGER_SETUP_ADC6                  			0x03000038
#define SIS3302_TRIGGER_THRESHOLD_ADC6              			0x0300003C

#define SIS3302_ENERGY_SETUP_GP_ADC56   						0x03000040      /* Gamma */
#define SIS3302_ENERGY_GATE_LENGTH_ADC56						0x03000044      /* Gamma */
#define SIS3302_ENERGY_SAMPLE_LENGTH_ADC56						0x03000048      /* Gamma */
#define SIS3302_ENERGY_SAMPLE_START_INDEX1_ADC56				0x0300004C      /* Gamma */

#define SIS3302_ENERGY_SAMPLE_START_INDEX2_ADC56				0x03000050      /* Gamma */
#define SIS3302_ENERGY_SAMPLE_START_INDEX3_ADC56				0x03000054      /* Gamma */
#define SIS3302_ENERGY_TAU_FACTOR_ADC5							0x03000058      /* Gamma */
#define SIS3302_ENERGY_TAU_FACTOR_ADC6							0x0300005C      /* Gamma */

#define SIS3302_EVENT_EXTENDED_CONFIG_ADC56        				0x03000070
#define SIS3302_TRIGGER_EXTENDED_SETUP_ADC5             		0x03000078
#define SIS3302_TRIGGER_EXTENDED_SETUP_ADC6             		0x0300007C

#define SIS3302_TRIGGER_EXTENDED_THRESHOLD_ADC5            		0x030000A0
#define SIS3302_TRIGGER_EXTENDED_THRESHOLD_ADC6            		0x030000A4



 // ADC78 FPGA group
#define SIS3302_EVENT_CONFIG_ADC78                				0x03800000 		/* Gamma */
#define SIS3302_END_ADDRESS_THRESHOLD_ADC78      	 			0x03800004	    /* Gamma */
#define SIS3302_PRETRIGGER_DELAY_TRIGGERGATE_LENGTH_ADC78  		0x03800008	    /* Gamma */
#define SIS3302_RAW_DATA_BUFFER_CONFIG_ADC78       				0x0380000C	    /* Gamma */

#define SIS3302_ACTUAL_SAMPLE_ADDRESS_ADC7          			0x03800010
#define SIS3302_ACTUAL_SAMPLE_ADDRESS_ADC8          			0x03800014
#define SIS3302_PREVIOUS_BANK_SAMPLE_ADDRESS_ADC7   			0x03800018
#define SIS3302_PREVIOUS_BANK_SAMPLE_ADDRESS_ADC8   			0x0380001C

#define SIS3302_ACTUAL_SAMPLE_VALUE_ADC78           			0x03800020
#define SIS3302_DDR2_TEST_REGISTER_ADC78						0x03800028

#define SIS3302_TRIGGER_SETUP_ADC7                  			0x03800030
#define SIS3302_TRIGGER_THRESHOLD_ADC7              			0x03800034
#define SIS3302_TRIGGER_SETUP_ADC8                  			0x03800038
#define SIS3302_TRIGGER_THRESHOLD_ADC8              			0x0380003C

#define SIS3302_ENERGY_SETUP_GP_ADC78   						0x03800040      /* Gamma */
#define SIS3302_ENERGY_GATE_LENGTH_ADC78						0x03800044      /* Gamma */
#define SIS3302_ENERGY_SAMPLE_LENGTH_ADC78						0x03800048      /* Gamma */
#define SIS3302_ENERGY_SAMPLE_START_INDEX1_ADC78				0x0380004C      /* Gamma */

#define SIS3302_ENERGY_SAMPLE_START_INDEX2_ADC78				0x03800050      /* Gamma */
#define SIS3302_ENERGY_SAMPLE_START_INDEX3_ADC78				0x03800054      /* Gamma */
#define SIS3302_ENERGY_TAU_FACTOR_ADC7							0x03800058      /* Gamma */
#define SIS3302_ENERGY_TAU_FACTOR_ADC8							0x0380005C      /* Gamma */


#define SIS3302_EVENT_EXTENDED_CONFIG_ADC78               		0x03800070
#define SIS3302_TRIGGER_EXTENDED_SETUP_ADC7             		0x03800078
#define SIS3302_TRIGGER_EXTENDED_SETUP_ADC8             		0x0380007C

#define SIS3302_TRIGGER_EXTENDED_THRESHOLD_ADC7            		0x038000A0
#define SIS3302_TRIGGER_EXTENDED_THRESHOLD_ADC8            		0x038000A4


#define SIS3302_ADC1_OFFSET                         			0x04000000
#define SIS3302_ADC2_OFFSET                         			0x04800000
#define SIS3302_ADC3_OFFSET                         			0x05000000
#define SIS3302_ADC4_OFFSET                         			0x05800000
#define SIS3302_ADC5_OFFSET                         			0x06000000
#define SIS3302_ADC6_OFFSET                         			0x06800000
#define SIS3302_ADC7_OFFSET                         			0x07000000
#define SIS3302_ADC8_OFFSET                         			0x07800000

#define SIS3302_NEXT_ADC_OFFSET                     			0x00800000

/* define sample clock */
#define SIS3302_ACQ_SET_CLOCK_TO_100MHZ                 0x70000000  /* default after Reset  */
#define SIS3302_ACQ_SET_CLOCK_TO_50MHZ                  0x60001000
#define SIS3302_ACQ_SET_CLOCK_TO_25MHZ                  0x50002000
#define SIS3302_ACQ_SET_CLOCK_TO_10MHZ                  0x40003000
#define SIS3302_ACQ_SET_CLOCK_TO_1MHZ                   0x30004000
#define SIS3302_ACQ_SET_CLOCK_TO_LEMO_RANDOM_CLOCK_IN   0x20005000
#define SIS3302_ACQ_SET_CLOCK_TO_LEMO_CLOCK_IN          0x10006000
//#define SIS3302_ACQ_SET_CLOCK_TO_P2_CLOCK_IN            0x00007000
#define SIS3302_ACQ_SET_CLOCK_TO_SECOND_100MHZ           0x00007000



#define SIS3302_ACQ_DISABLE_LEMO_TRIGGER            0x01000000 /* GAMMA, 091207 */
#define SIS3302_ACQ_ENABLE_LEMO_TRIGGER             0x00000100 /* GAMMA, 091207 */
#define SIS3302_ACQ_DISABLE_LEMO_TIMESTAMPCLR       0x02000000 /* GAMMA, 091207 */
#define SIS3302_ACQ_ENABLE_LEMO_TIMESTAMPCLR        0x00000200 /* GAMMA, 091207 */

// new 16.3.2009
#define SIS3302_ACQ_DISABLE_EXTERNAL_LEMO_IN3       0x01000000 /* GAMMA, up V_1205 */
#define SIS3302_ACQ_ENABLE_EXTERNAL_LEMO_IN3        0x00000100 /* GAMMA, up V_1205 */
#define SIS3302_ACQ_DISABLE_EXTERNAL_LEMO_IN2       0x02000000 /* GAMMA, up V_1205 */
#define SIS3302_ACQ_ENABLE_EXTERNAL_LEMO_IN2        0x00000200 /* GAMMA, up V_1205 */
#define SIS3302_ACQ_DISABLE_EXTERNAL_LEMO_IN1       0x04000000 /* GAMMA, up V_1205 */
#define SIS3302_ACQ_ENABLE_EXTERNAL_LEMO_IN1        0x00000400 /* GAMMA, up V_1205 */

#define SIS3302_ACQ_SET_LEMO_IN_MODE0              	0x00070000  /* GAMMA, up V_1205   */
#define SIS3302_ACQ_SET_LEMO_IN_MODE1         		0x00060001  /* GAMMA, up V_1205   */
#define SIS3302_ACQ_SET_LEMO_IN_MODE2           	0x00050002  /* GAMMA, up V_1205   */
#define SIS3302_ACQ_SET_LEMO_IN_MODE3      			0x00040003  /* GAMMA, up V_1205   */
#define SIS3302_ACQ_SET_LEMO_IN_MODE4          		0x00030004  /* GAMMA, up V_1205   */
#define SIS3302_ACQ_SET_LEMO_IN_MODE5   			0x00020005  /* GAMMA, up V_1205   */
#define SIS3302_ACQ_SET_LEMO_IN_MODE6          		0x00010006  /* GAMMA, up V_1205   */
#define SIS3302_ACQ_SET_LEMO_IN_MODE7           	0x00000007  /* GAMMA, up V_1205   */
#define SIS3302_ACQ_SET_LEMO_IN_MODE_BIT_MASK      	0x00000007  /* GAMMA, up V_1205   */

#define SIS3302_ACQ_SET_LEMO_OUT_MODE0             	0x00300000  /* GAMMA, up V_1205   */
#define SIS3302_ACQ_SET_LEMO_OUT_MODE1         		0x00200010  /* GAMMA, up V_1205   */
#define SIS3302_ACQ_SET_LEMO_OUT_MODE2           	0x00100020  /* GAMMA, up V_1205   */
#define SIS3302_ACQ_SET_LEMO_OUT_MODE3      		0x00000030  /* GAMMA, up V_1205   */
#define SIS3302_ACQ_SET_LEMO_OUT_MODE_BIT_MASK 		0x00000030  /* GAMMA, up V_1205   */

#define SIS3302_ACQ_SET_FEEDBACK_INTERNAL_TRIGGER 	0x00000040  /* GAMMA, up V_1205   */
#define SIS3302_ACQ_CLR_FEEDBACK_INTERNAL_TRIGGER 	0x00400000  /* GAMMA, up V_1205   */




#define SIS3302_BROADCAST_MASTER_ENABLE        		0x20
#define SIS3302_BROADCAST_ENABLE               		0x10


/* gamma  */
#define EVENT_CONF_ADC2_NP1_NB_GATE_ENABLE_BIT		0x8000	  /*     */
#define EVENT_CONF_ADC2_NM1_NB_GATE_ENABLE_BIT		0x4000	  /*     */
#define EVENT_CONF_ADC2_EXTERN_GATE_ENABLE_BIT		0x2000	  /* GAMMA, up V_1205   */
#define EVENT_CONF_ADC2_INTERN_GATE_ENABLE_BIT		0x1000	  /* GAMMA, up V_1205   */
#define EVENT_CONF_ADC2_EXTERN_TRIGGER_ENABLE_BIT	0x800
#define EVENT_CONF_ADC2_INTERN_TRIGGER_ENABLE_BIT	0x400
#define EVENT_CONF_ADC2_INPUT_INVERT_BIT			0x100

#define EVENT_CONF_ADC1_NP1_NB_GATE_ENABLE_BIT		0x80	  /*     */
#define EVENT_CONF_ADC1_NM1_NB_GATE_ENABLE_BIT		0x40	  /*     */
#define EVENT_CONF_ADC1_EXTERN_GATE_ENABLE_BIT		0x20	  /* GAMMA, up V_1205   */
#define EVENT_CONF_ADC1_INTERN_GATE_ENABLE_BIT		0x10	  /* GAMMA, up V_1205   */
#define EVENT_CONF_ADC1_EXTERN_TRIGGER_ENABLE_BIT	0x8
#define EVENT_CONF_ADC1_INTERN_TRIGGER_ENABLE_BIT	0x4
#define EVENT_CONF_ADC1_INPUT_INVERT_BIT			0x1



#define DECIMATION_DISABLE							0x00000000
#define DECIMATION_2								0x10000000
#define DECIMATION_4								0x20000000
#define DECIMATION_8								0x30000000


/* gamma  MCA */
/*******************************************************************************************************/




#define SIS3302_MCA_SCAN_NOF_HISTOGRAMS_PRESET	    			0x80	  /* read/write; D32 */
#define SIS3302_MCA_SCAN_HISTOGRAM_COUNTER	    				0x84	  /* read only; D32  */
#define SIS3302_MCA_SCAN_SETUP_PRESCALE_FACTOR    				0x88	  /* read only; D32  */
#define SIS3302_MCA_SCAN_CONTROL			    				0x8C	  /* read/write; D32  */

#define SIS3302_MCA_MULTISCAN_NOF_SCANS_PRESET	    			0x90	  /* read/write; D32 */
#define SIS3302_MCA_MULTISCAN_SCAN_COUNTER	    				0x94	  /* read only; D32  */
#define SIS3302_MCA_MULTISCAN_LAST_SCAN_HISTOGRAM_COUNTER		0x98	  /* read only; D32  */


#define SIS3302_KEY_MCA_SCAN_LNE_PULSE           				0x410	  /* write only; D32 */
#define SIS3302_KEY_MCA_SCAN_ARM         						0x414	  /* write only; D32 */
#define SIS3302_KEY_MCA_SCAN_START           					0x418	  /* write only; D32 */
#define SIS3302_KEY_MCA_SCAN_DISABLE           					0x41C	  /* write only; D32 */

#define SIS3302_KEY_MCA_MULTISCAN_START_RESET_PULSE				0x420	  /* write only; D32 */
#define SIS3302_KEY_MCA_MULTISCAN_ARM_SCAN_ARM        			0x424	  /* write only; D32 */
#define SIS3302_KEY_MCA_MULTISCAN_ARM_SCAN_ENABLE				0x428	  /* write only; D32 */
#define SIS3302_KEY_MCA_MULTISCAN_DISABLE           			0x42C	  /* write only; D32 */



#define SIS3302_MCA_ENERGY2HISTOGRAM_PARAM_ADC1357    			0x01000060	  /* write only; D32 */
#define SIS3302_MCA_ENERGY2HISTOGRAM_PARAM_ADC2468    			0x01000064	  /* write only; D32 */
#define SIS3302_MCA_HISTOGRAM_PARAM_ALL_ADC    					0x01000068	  /* write only; D32 */

#define SIS3302_MCA_ENERGY2HISTOGRAM_PARAM_ADC1    				0x02000060	  /* read/write; D32 */
#define SIS3302_MCA_ENERGY2HISTOGRAM_PARAM_ADC2    				0x02000064	  /* read/write; D32 */
#define SIS3302_MCA_HISTOGRAM_PARAM_ADC12    					0x02000068	  /* read/write; D32 */





#define SIS3302_MCA_TRIGGER_START_COUNTER_ADC1 					0x02000080	  /* read only; D32 */
#define SIS3302_MCA_PILEUP_COUNTER_ADC1 						0x02000084	  /* read only; D32 */
#define SIS3302_MCA_ENERGY2HIGH_COUNTER_ADC1 					0x02000088	  /* read only; D32 */
#define SIS3302_MCA_ENERGY2LOW_COUNTER_ADC1 					0x0200008C	  /* read only; D32 */

#define SIS3302_MCA_TRIGGER_START_COUNTER_ADC2 					0x02000090	  /* read only; D32 */
#define SIS3302_MCA_PILEUP_COUNTER_ADC2 						0x02000094	  /* read only; D32 */
#define SIS3302_MCA_ENERGY2HIGH_COUNTER_ADC2 					0x02000098	  /* read only; D32 */
#define SIS3302_MCA_ENERGY2LOW_COUNTER_ADC2 					0x0200009C	  /* read only; D32 */


#define SIS3302_MCA_TRIGGER_START_COUNTER_ADC3 					0x02800080	  /* read only; D32 */
#define SIS3302_MCA_PILEUP_COUNTER_ADC3 						0x02800084	  /* read only; D32 */
#define SIS3302_MCA_ENERGY2HIGH_COUNTER_ADC3 					0x02800088	  /* read only; D32 */
#define SIS3302_MCA_ENERGY2LOW_COUNTER_ADC3 					0x0280008C	  /* read only; D32 */

#define SIS3302_MCA_TRIGGER_START_COUNTER_ADC4 					0x02800090	  /* read only; D32 */
#define SIS3302_MCA_PILEUP_COUNTER_ADC4 						0x02800094	  /* read only; D32 */
#define SIS3302_MCA_ENERGY2HIGH_COUNTER_ADC4 					0x02800098	  /* read only; D32 */
#define SIS3302_MCA_ENERGY2LOW_COUNTER_ADC4 					0x0280009C	  /* read only; D32 */

#define SIS3302_MCA_TRIGGER_START_COUNTER_ADC5 					0x03000080	  /* read only; D32 */
#define SIS3302_MCA_PILEUP_COUNTER_ADC5 						0x03000084	  /* read only; D32 */
#define SIS3302_MCA_ENERGY2HIGH_COUNTER_ADC5 					0x03000088	  /* read only; D32 */
#define SIS3302_MCA_ENERGY2LOW_COUNTER_ADC5 					0x0300008C	  /* read only; D32 */

#define SIS3302_MCA_TRIGGER_START_COUNTER_ADC6 					0x03000090	  /* read only; D32 */
#define SIS3302_MCA_PILEUP_COUNTER_ADC6 						0x03000094	  /* read only; D32 */
#define SIS3302_MCA_ENERGY2HIGH_COUNTER_ADC6 					0x03000098	  /* read only; D32 */
#define SIS3302_MCA_ENERGY2LOW_COUNTER_ADC6 					0x0300009C	  /* read only; D32 */

#define SIS3302_MCA_TRIGGER_START_COUNTER_ADC7 					0x03800080	  /* read only; D32 */
#define SIS3302_MCA_PILEUP_COUNTER_ADC7 						0x03800084	  /* read only; D32 */
#define SIS3302_MCA_ENERGY2HIGH_COUNTER_ADC7 					0x03800088	  /* read only; D32 */
#define SIS3302_MCA_ENERGY2LOW_COUNTER_ADC7 					0x0380008C	  /* read only; D32 */

#define SIS3302_MCA_TRIGGER_START_COUNTER_ADC8 					0x03800090	  /* read only; D32 */
#define SIS3302_MCA_PILEUP_COUNTER_ADC8 						0x03800094	  /* read only; D32 */
#define SIS3302_MCA_ENERGY2HIGH_COUNTER_ADC8 					0x03800098	  /* read only; D32 */
#define SIS3302_MCA_ENERGY2LOW_COUNTER_ADC8 					0x0380009C	  /* read only; D32 */

#define SIS3302_ACQ_SET_MCA_MODE 	0x00000008  /* GAMMA, up V_1205   */
#define SIS3302_ACQ_CLR_MCA_MODE 	0x00080000  /* GAMMA, up V_1205   */

/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////


#endif // SISDEFINES_H
