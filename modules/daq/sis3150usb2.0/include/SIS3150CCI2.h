#ifndef SIS3150USBCCI2_H
#define SIS3150USBCCI2_H

#include "fstream"
#include <QtCore/QCoreApplication>
#include <QLibrary>
#include <QDir>
#include <QtDebug>
#include <iostream>
#include <string>
#include <QDateTime>
#include <QFileInfo>
#include <math.h>
//#include <stdint.h>
#include <assert.h>

#include "Rtypes.h"


#include "GCG_daq_aux.h"

#include "GRIDefines.h"
#include "GRIDAQThread.h"
#include "GRIDataDefines.h"
#include "GRIDAQAccumulator.h"
#include "GRIDAQAccumNode.h"

#include "SISDefines.h"

/**********************************************/
/**********************************************/

/*
  SIS3150USB concrete class
  This class is used for interfacing with the SIS3150USB firmware board manufactured by Struck Innovative Systeme (SIS).
  The code for this class was largely taken from code written by SIS in LabWindows CVI.
  */







class SIS3150CCI2 : public GRIDAQThread
{

public:

    SIS3150CCI2();
    /*
      loadConfiguration()
      This method loads the configuration file. It just calls FirstReadConfiguration()
      */
    int loadConfiguration();

    /*
      connectToDaq()
      This is an implemented method of the virtual GRIDAQ class. This method intializes the SIS3150USB libraries given by
      Struck Innovative Systeme (those libraries are explained in InitLibraries()). connectToDaq() also gets the
      sis3150usb board information by connecting to the board and downloading the board's information. It then performs
      some other commands important for connecting to the sis3150usb board.
      */
    int connectToDAQ();

    /*
      initialize()
      This method performs some extra initialization of class variables and writing to the sis3150usb board.
      */
    int initialize(); //{cout << "initialize inline" << endl; return 0;}

    /*
      startDataAcquisition()
      This method begins the process of getting ready for data acquisition. Our program does not begin acquiring data
      immediately after startDataAcquisition() is called. Instead, the SIS3150USB needs to set some class variables and
      on-board configurations before beginning acquisition.

      TODO: In the future, for when we time-synchronize the packet system, it would be good to know when the timestamps
      on the SIS3150USB actually begin at zero (meaning, at what point in the code the system clock for the SIS3150USB is
      reset.

      */
    int startDataAcquisition();

    /*
      stopDataAcquisition()
      This method sets a flag to keep the multievent acquisition "while" loop from continuing. This essentially stops the
      data acquisition once we have finished reading out a bank of data. For more information on how the sis3150usb acquires
      data on the hardware level look to further documentation.
      */
    int stopDataAcquisition();

    /*
      acquireData()
      This is the main acquisition function it is called repeatadly by the thread while the run flag is true.
      */

    int acquireData(int n);

    GRIDAQAccumNode* RegisterDataOutput(QString outName);
    /*
      setfiltervalues()
      function to set trapezoidal filter peaking and gap times. To be replaced by macros and online configuration routines.

    */
    void setfiltervalues(unsigned int peaking, unsigned int gap, unsigned int decimation);
    void setfiltervalues(unsigned int peaking, unsigned int gap, unsigned int decimation, void* usbptr);

    void* getusbptr();

private:
    /*
      InitLibraries()
      This method resolves SIS library methods used for accessing the sis3150usb board. There is also a DEBUG_LIB section
      that can be defined or not defined depending on whether you want the code to check if the library functions were
      successfully loaded.
      */
    void InitLibraries(void);

    /*
      ReadConfiguration()
      This method reads the data from the config file listed as a character string in path_file. In the future for the GRI framework
      we will be writing a standard method for parsing XML files. That standard method and the connected methods will eventually
      replace the functionality of ReadConfiguration()
      */
    int  ReadConfiguration (char *path_file);


    /************Methods in startDataAcquisition()*************************/

    /*
      Configuration_SIS3820Clock()
      This method configures hardware aspects of the SIS3820 clock which keeps the SIS3302 modules' timestamps synchronized.
      */
    int  Configuration_SIS3820Clock (void);

    /*
      Configuration_SIS3302ADC()
      This method configures the SIS3302ADC modules accordingly.
      */
    int  Configuration_SIS3302ADC (unsigned int module_addr, unsigned int module_conf_index );

    /*
      sis3302_write_dac_offset()
      This method configures some aspects of the DAQ.
      */
    int  sis3302_write_dac_offset(unsigned int module_addr, unsigned int *offset_value_array);

    /*
      sub_sis3302_fastTriggerOut_setup
      */
    int sub_sis3302_fastTriggerOut_setup(unsigned int module_addr, unsigned int module_conf_index);

    /*
       DefineRunEnergyFactor()
       Sets the "run energy factor". We don't know what this means, but we choose to leave it in so that the program
       works. In the future we may clean up the code, taking away unecessary functions, incrementally testing to make sure
       we haven't broken the system.
    */
    int  DefineRunEnergyFactor (void);


    /*
      WriteParseBuffer()
      This method takes in the memory_data_array as raw events and parses out the energy, timestamp, and channel id of
      each event. These are not the only pieces of data that could be parsed out for the SIS3150USB. We just used these
      for simplicity's sake and for testing. If we need to, we could parse out other elements of the events.

      After parsing the raw events, this method passes the information along to the SISAccumulator, which handles the
      accumulation of events and eventual packaging based on timestamps of the events.
      */
    int WriteParseBuffer (ULONG* memory_data_array, unsigned int nof_write_length_lwords);



    /*
      CopyModuleAddressesToTable()
      This method is VERY important. It copies the modules addresses to a table used for interfacing with teh 3302 modueles
      through the sis3150usb.
      */
    int  CopyModuleAddressesToTable (void);

    /*
      copy_sis3302_sample_energy_CommonParameters_to_inbox()
      We don't know if this important or not, but we've left it in.
      */
    int  copy_sis3302_sample_energy_CommonParameters_to_inbox (void);
    /*
      calculate_energy_trigger_windows_and_parameters()
      This method affects important class variables.
      */
    int  calculate_energy_trigger_windows_and_parameters (void);



    /*
      Get_ADC_sample_clock_for_DecayExpCalculation()
      This method affects important class variables.
      */
    int  Get_ADC_sample_clock_for_DecayExpCalculation(unsigned int* sample_clock);

    /*
      TauFactor_to_DecayExpCalculation()
      This method affects important class variables.
      */
    int  TauFactor_to_DecayExpCalculation(void);


    /************Methods in startDataAcquisition() - END*************************/

    /*
      Error handling and messaging.
      These three methods are used for handling error messages. Essentially: they print out the messages you send to them.
      If we want to, we can add more functionality in the future. For now, they just "cout" the messages.
      */
    int sisVME_ErrorHandling (unsigned int prot_error, unsigned int vme_addr, const char *err_messages);
    int write_system_messages (char *system_messages, int write_datetime);
    int sis_AnyErrorHandling (char *err_messages);





    /*
      vme and sub_vme methods
      These methods are used to read from or write to the SIS3150USB board throughout the SIS3150USB code. Usually, the
      paremeters specify an address and the data to be read from or written to that address.
      */
    typedef unsigned char           u_int8_t;
    typedef unsigned short          u_int16_t;
    typedef unsigned long           u_int32_t;

    int vme_A32D32_read(HANDLE  hXDev, u_int32_t vme_adr, u_int32_t* vme_data ) ;
    int vme_A32D32_write(HANDLE  hXDev, u_int32_t vme_adr, u_int32_t vme_data );
    int sub_vme_A32D32_read (unsigned int vme_adr, unsigned int* vme_data) ;
    int sub_vme_A32D32_write (unsigned int vme_adr, unsigned int vme_data) ;
    int sub_vme_A32MBLT64_read (unsigned int vme_adr, ULONG* dma_buffer,
                                unsigned int request_nof_words, ULONG* got_nof_words)  ; // J_type_change


    // Instance variables
    // DAC Offset
      unsigned int  gl_uint_DacOffset_Conf[MAX_NO_OF_MODULES] [NO_OF_ADC_CHANNELS]     ;

    // FIR Trigger Parameters
      unsigned int  gl_uint_FirTriggerP_Conf[MAX_NO_OF_MODULES] [NO_OF_ADC_CHANNELS]     ;
      unsigned int  gl_uint_FirTriggerG_Conf[MAX_NO_OF_MODULES] [NO_OF_ADC_CHANNELS]     ;
      int  gl_int_FirTriggerThresholdValue_Conf[MAX_NO_OF_MODULES] [NO_OF_ADC_CHANNELS]     ;
      unsigned int  gl_uint_FirTriggerThreshold_GtLtFlag_Conf[MAX_NO_OF_MODULES] [NO_OF_ADC_CHANNELS]     ;
      unsigned int  gl_uint_FirTriggerPulseLength_Conf[MAX_NO_OF_MODULES] [NO_OF_ADC_CHANNELS]     ;

    // ADC Signal Invert Flag
      unsigned int  gl_uint_ADC_SignalInvertConf[MAX_NO_OF_MODULES][NO_OF_ADC_CHANNELS]     ;

    // ADC Gain Factor
      unsigned int  gl_uint_ADC_Next_NeighborConf[MAX_NO_OF_MODULES][NO_OF_ADC_CHANNELS]     ;

    // Energy Factor
      float  gl_float_EnergyFactorConf[MAX_NO_OF_MODULES][ NO_OF_ADC_CHANNELS]    ;
      float  gl_float_EnergyFactorRun[MAX_NO_OF_MODULES][ NO_OF_ADC_CHANNELS]    ;

    // Decay Factor
      unsigned int  gl_uint_DecayTauConf[MAX_NO_OF_MODULES][NO_OF_ADC_CHANNELS]     ;


    // Common Sample and Energy Parameters
      char gl_char_FileName_sis3150_9300_TSLoaderFile[MAX_FILENAME_LENGTH];
      unsigned int  gl_uint_FirEnergyGap     ;
      unsigned int  gl_uint_FirEnergyLength     ;
      unsigned int  gl_uint_FirDecimationMode     ;
      unsigned int  gl_uint_FirMwShapingLength     ;
      unsigned int  gl_uint_EnergyGateLength     ;
      unsigned int  gl_uint_RawSampleStart_Index     ;
      unsigned int  gl_uint_RawSampleLength     ;
      unsigned int  gl_uint_EnergySampleMode     ;
      unsigned int  gl_uint_EnergySampleStart_Index1     ;
      unsigned int  gl_uint_EnergySampleStart_Index2     ;
      unsigned int  gl_uint_EnergySampleStart_Index3     ;
      unsigned int  gl_uint_EnergySampleLength     ;
      unsigned int  gl_uint_PreTriggerDelay     ;
      unsigned int  gl_uint_TriggerGateLength     ;

    // Library functions (sis3150usb_vme_win.dll - windows) or (libsis3150.so - linux)
      sis3150Usb_Register_Single_Read_PROT                sis3150usb_Register_Single_Read;
      FindAll_SIS3150USB_Devices_PROT                     FindAll_sis3150usb_Devices;
      Sis3150usb_OpenDriver_And_Download_FX2_Setup_PROT   sis3150usb_OpenDriver_And_Download_FX2_Setup;
      sis3150Usb_Register_Single_Write_PROT               sis3150usb_Register_Single_Write;
      sis3150Usb_Vme_Dma_Read_PROT                        sis3150usb_Vme_Dma_Read;   // used for sub_vme_A32MBLT64_read
      sis3150Usb_Vme_Single_Read_PROT                     sis3150usb_Vme_Single_Read;
      sis3150Usb_Vme_Single_Write_PROT                    sis3150usb_Vme_Single_Write;



    ////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////Unchangeable//////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////////
      HANDLE  gl_USBDevice;                // used for keeping track of the device - important
      SIS3150USB_Device_Struct	gl_sis3150_device_information_struct[MAX_USB_DEV_NUMBER];       // used throughout program

      char gl_char_FileNameData[MAX_FILENAME_LENGTH];
      char gl_char_FileNameLoadersis3150usb_TS[MAX_FILENAME_LENGTH];


    ////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////Unchangeable - END////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////////

    // configuration globals
      unsigned int gl_uint_system_status ;
      unsigned int gl_uint_ModEnableConf[MAX_NO_OF_MODULES]   ;
      unsigned int gl_uint_ModAddrConf[MAX_NO_OF_MODULES]		;
      unsigned int gl_uint_SIS3302_FourChannelFlag   ;

    // Clock Module
      unsigned int gl_uint_SIS3820EnableConf   ;
      unsigned int gl_uint_SIS3820AddrConf	;

    // Common Sample and Energy Parameters
      unsigned int gl_uint_SIS3820ClockModeConf	;
      unsigned int gl_uint_SIS3820ClockCtrl4FlagConf	;
      unsigned int gl_uint_SIS3820ClockCtrl3FlagConf	;
      unsigned int gl_uint_SIS3820ClockCtrl2FlagConf	;
      unsigned int gl_uint_SIS3820ClockTriggerMaskConf	;
      unsigned int  gl_uint_SIS3302ClockModeConf     ;

    // Common SIS3150
      unsigned int  gl_uint_SIS3302_Trigger_Mode_Source_ModeConf     ;
      unsigned int  gl_uint_SIS3302External_Lemo_InOutput_Conf     ;
      unsigned int  gl_uint_SIS3302_Reserved1_ModeConf     ;
      unsigned int  gl_uint_SIS3302_Reserved2_ModeConf     ;

    // Instance variables:

    /***********************************************************/
    /***********************************************************/
    /********************Instance Variables*********************/

    // Added by Jeremy to reconcile the separation of the acquisition function into separate parts
    unsigned int event_length_lwords;
    unsigned int max_events;
    unsigned int  adc_buffer_reg_offset[8] ;
    unsigned int  energy_max_index ;
    unsigned int  previous_bank_sample_address_reg_offset[8] ;
    unsigned int bank1_armed_flag;
    unsigned int buffer_switch_counter;
    unsigned int nof_raw_data_lwords;
    unsigned int raw_data_index;
    //////////////////////////////////////////////////////////////////////////




    //#ifdef COMPILE_WITH_FRAMEWORK
    //  SISAccumulator *acum;    // The accumulator for our data acquisition system.
    //#endif

      FILE *gl_stdDebugFilePointer;    // file for writing debug methods (only written to in STD_DEBUG code sections
    //  FILE *gl_filePointer;  //added by jeremy; after we parse the data we write it to this file for debugging purposes









    //run
      unsigned int  gl_uint_RunStopStatusFlag     ;
      unsigned int  gl_uint_RunReadoutMode     ;


      unsigned int  gl_uint_RunCheckWrDataToFileFlag     ;
      unsigned int  gl_uint_RunCheckStopEventsFlag     ;       // flag for whether we will stop after a certain # of events
      unsigned int  gl_uint_RunEventCounter        ;
      unsigned int  gl_uint_RunEventOldCounter     ;
      unsigned int  gl_unit_RunOneSecondCounter    ;


    // run globals
      unsigned int gl_uint_ModConfIndexRun[MAX_NO_OF_MODULES]  ;
      unsigned int gl_uint_ModAddrRun[MAX_NO_OF_MODULES]  ;
      unsigned int gl_uint_NoOfModulesRun ;
      unsigned int gl_uint_SIS3302_BroadcastAddrConf ;

    // Display Diagnostic
      unsigned int  gl_uint_CountOfPileupsTriggerCountBasedArray[16]  ;

    // data event file
      FILE *gl_FILE_DataEvenFilePointer           ;
      unsigned int  gl_uint_DataEvent_OpenFlag	;
      unsigned int  gl_uint_DataEvent_FileCounter ;
      unsigned int  gl_uint_DataEvent_RunFile_NOF_Energy ;
      unsigned int  gl_uint_DataEvent_LWordCounter ;

    // MCA Mode upgrading
    // offline


    // offline

    // Macro for IS_64_BIT is found in GRIDefines.h
    #if IS_64_BIT
      ULONG gl_dma_rd_buffer[0x800000] ;
    #else
      ULONG gl_dma_rd_buffer[0x1000000] ;
    #endif

      unsigned int  gl_uint_end_sample_address[NO_OF_ADC_CHANNELS]  ;

    /*******************Instance Variables END******************/
    /***********************************************************/
    /***********************************************************/

   // Below Added by CRB  09/2011
   int firstrun;

};

#endif // sis3150usb_H
