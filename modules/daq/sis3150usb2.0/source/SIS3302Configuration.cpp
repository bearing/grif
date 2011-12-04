#include <SIS3150CCI2.h>
#include "cmath"

/*
  Author:Cameron Bates cameron.r.bates@gmail.com
  Source code based on Struck Innovative Systeme 3302 configuration routines from the gamma_system code
  This files contains methods relevant to configuring the SIS3302 and 3820 modules.
  It was seperated from the DAQ to improve readability.

  */

int SIS3150CCI2::ReadConfiguration (char *path_file)
{
    int imod,iadc ;
    int i;
    int comp_err ;
    FILE *used_file ;

    char temp_char[MAX_FILENAME_LEN]  ;


    used_file=fopen(path_file, "r");

    //temp = errno ;
    if (used_file==NULL)  return FILE_ACCESS_NOT_OK;
    fgets (temp_char, MAX_FILENAME_LEN, used_file);  /* get header */
    fgets (temp_char, MAX_FILENAME_LEN, used_file);  /* get get data and time */

    // get   PC Readout Mode
    fscanf(used_file,"%s\n",temp_char);	      // get KEY-STRING
    comp_err = strcmp (temp_char, STRING_RUN_READOUT_MODE); // check KEY-STRING
    if(comp_err != 0)  {
        fclose(used_file);
        return FILE_ACCESS_NOT_OK;
    }
    fscanf(used_file,"%8x \n", &gl_uint_RunReadoutMode);


    // get Clock Module Parameter
    fscanf(used_file,"%s\n",temp_char);	      // get KEY-STRING
    comp_err = strcmp (temp_char, STRING_CLOCK_MODULE); // check KEY-STRING
    if(comp_err != 0)  {
        fclose(used_file);
        return FILE_ACCESS_NOT_OK;
    }
    fscanf(used_file,"%8x %8x \n",&gl_uint_SIS3820EnableConf, &gl_uint_SIS3820AddrConf);   // read  Module Enable Flag / Address
    //cout << gl_uint_SIS3820AddrConf << endl;

    // get Clock Modes
    fscanf(used_file,"%s\n",temp_char);	      // get KEY-STRING
    comp_err = strcmp (temp_char, STRING_CLOCK_MODE); // check KEY-STRING
    if(comp_err != 0)  {
        fclose(used_file);
        return FILE_ACCESS_NOT_OK;
    }

    fscanf(used_file,"%8x \n",&gl_uint_SIS3820ClockModeConf);   // read  Clock Mode
    fscanf(used_file,"%8x \n",&gl_uint_SIS3820ClockCtrl4FlagConf);   //
    fscanf(used_file,"%8x \n",&gl_uint_SIS3820ClockCtrl3FlagConf);   //
    fscanf(used_file,"%8x \n",&gl_uint_SIS3820ClockCtrl2FlagConf);   //
    fscanf(used_file,"%8x \n",&gl_uint_SIS3820ClockTriggerMaskConf);   //

    fscanf(used_file,"%8x \n",&gl_uint_SIS3302ClockModeConf);   // read  Clock Mode



    // get STRING_SIS3302_MODES
    fscanf(used_file,"%s\n",temp_char);	      // get KEY-STRING
    comp_err = strcmp (temp_char, STRING_SIS3302_MODES); // check KEY-STRING
    if(comp_err != 0)  {
        fclose(used_file);
        return FILE_ACCESS_NOT_OK;
    }

    fscanf(used_file,"%8x %8x %8x %8x \n",&gl_uint_SIS3302_Trigger_Mode_Source_ModeConf, &gl_uint_SIS3302External_Lemo_InOutput_Conf,
           &gl_uint_SIS3302_Reserved1_ModeConf, &gl_uint_SIS3302_Reserved2_ModeConf);   // read


    // get Modules
    fscanf(used_file,"%s\n",temp_char);	      // get KEY-STRING
    comp_err = strcmp (temp_char, STRING_ADC_MODULES); // check KEY-STRING
    if(comp_err != 0)  {
        fclose(used_file);
        return FILE_ACCESS_NOT_OK;
    }
    for (i=0;i<MAX_NO_OF_MODULES;i++) {
        fscanf(used_file,"%8x %8x \n",&gl_uint_ModEnableConf[i], &gl_uint_ModAddrConf[i]);   // read  Modules
    }


    // get SIS3150CCI2 TS Readout Loader File // BAD
    fscanf(used_file,"%s\n",temp_char);	      // get KEY-STRING
    //comp_err = strcmp (temp_char, STRING_sis3150usb_TS_LOADER_FILE); // check KEY-STRING
    if(comp_err != 0)  {
        fclose(used_file);
        return FILE_ACCESS_NOT_OK;
    }
    //cout << used_file << endl;
    fscanf(used_file,"%s\n",gl_char_FileNameLoadersis3150usb_TS);	      // get


    // get Sampling and Energy Parameters
    fscanf(used_file,"%s\n",temp_char);	      // get KEY-STRING
    comp_err = strcmp (temp_char, STRING_SAMPLING_ENERGY); // check KEY-STRING
    if(comp_err != 0)  {
        fclose(used_file);
        return FILE_ACCESS_NOT_OK;
    }
unsigned int temp_uint;
    fscanf(used_file,"%s\n",gl_char_FileName_sis3150_9300_TSLoaderFile);	      // g
    fscanf(used_file,"%8x \n",&temp_uint);   //gl_uint_FirEnergyLength
    fscanf(used_file,"%8x \n",&temp_uint);   //gl_uint_FirEnergyGap
    fscanf(used_file,"%8x \n",&temp_uint);   //gl_uint_FirDecimationMode
    fscanf(used_file,"%8x \n",&gl_uint_FirMwShapingLength);   //
    fscanf(used_file,"%8x \n",&gl_uint_RawSampleStart_Index);   //
    fscanf(used_file,"%8x \n",&gl_uint_RawSampleLength);   //
    fscanf(used_file,"%8x \n",&gl_uint_EnergySampleMode);   //
    fscanf(used_file,"%8x \n",&gl_uint_EnergySampleStart_Index1);   //
    fscanf(used_file,"%8x \n",&gl_uint_EnergySampleStart_Index2);   //
    fscanf(used_file,"%8x \n",&gl_uint_EnergySampleStart_Index3);   //
    fscanf(used_file,"%8x \n",&gl_uint_EnergySampleLength);   //
    fscanf(used_file,"%8x \n",&gl_uint_TriggerGateLength);   //
    fscanf(used_file,"%8x \n",&gl_uint_PreTriggerDelay);   //


    if (gl_uint_EnergySampleMode > 1) {gl_uint_EnergySampleMode = 0;}



    // get DAC Offset values
    fscanf(used_file,"%s\n",temp_char);	      // get KEY-STRING
    comp_err = strcmp (temp_char, STRING_DAC_OFFSET_VALUES); // check KEY-STRING
    if(comp_err != 0)  {
        fclose(used_file);
        return FILE_ACCESS_NOT_OK;
    }

    for (imod=0;imod<MAX_NO_OF_MODULES;imod++) {
        fscanf(used_file,"%8x %8x %8x %8x %8x %8x %8x %8x \n",	&gl_uint_DacOffset_Conf[imod][INDEX_ADC_CHANNEL1],
               &gl_uint_DacOffset_Conf[imod][INDEX_ADC_CHANNEL2],
               &gl_uint_DacOffset_Conf[imod][INDEX_ADC_CHANNEL3],
               &gl_uint_DacOffset_Conf[imod][INDEX_ADC_CHANNEL4],
               &gl_uint_DacOffset_Conf[imod][INDEX_ADC_CHANNEL5],
               &gl_uint_DacOffset_Conf[imod][INDEX_ADC_CHANNEL6],
               &gl_uint_DacOffset_Conf[imod][INDEX_ADC_CHANNEL7],
               &gl_uint_DacOffset_Conf[imod][INDEX_ADC_CHANNEL8] );
    }





    // get FIR Trigger values
    fscanf(used_file,"%s\n",temp_char);	      // get KEY-STRING
    comp_err = strcmp (temp_char, STRING_FIR_TRIGGER_VALUES); // check KEY-STRING
    if(comp_err != 0)  {
        fclose(used_file);
        return FILE_ACCESS_NOT_OK;
    }

    for (imod=0;imod<MAX_NO_OF_MODULES;imod++) {
        for (iadc=0;iadc<NO_OF_ADC_CHANNELS;iadc++) {
            fscanf(used_file,"%8x %8x %8x %8x %8x \n", &gl_uint_FirTriggerP_Conf[imod][iadc],
                   &gl_uint_FirTriggerG_Conf[imod][iadc],
                   &gl_uint_FirTriggerThreshold_GtLtFlag_Conf[imod][iadc],
                   &gl_int_FirTriggerThresholdValue_Conf[imod][iadc],
                   &gl_uint_FirTriggerPulseLength_Conf[imod][iadc] );
        }
    }




    // get ADC_SIGNAL_INVERT_FLAGS
    fscanf(used_file,"%s\n",temp_char);	      // get KEY-STRING
    comp_err = strcmp (temp_char, STRING_ADC_SIGNAL_INVERT_FLAGS); // check KEY-STRING
    if(comp_err != 0)  {
        fclose(used_file);
        return FILE_ACCESS_NOT_OK;
    }

    for (imod=0;imod<MAX_NO_OF_MODULES;imod++) {
        fscanf(used_file,"%8x %8x %8x %8x %8x %8x %8x %8x \n",	&gl_uint_ADC_SignalInvertConf[imod][INDEX_ADC_CHANNEL1],
               &gl_uint_ADC_SignalInvertConf[imod][INDEX_ADC_CHANNEL2],
               &gl_uint_ADC_SignalInvertConf[imod][INDEX_ADC_CHANNEL3],
               &gl_uint_ADC_SignalInvertConf[imod][INDEX_ADC_CHANNEL4],
               &gl_uint_ADC_SignalInvertConf[imod][INDEX_ADC_CHANNEL5],
               &gl_uint_ADC_SignalInvertConf[imod][INDEX_ADC_CHANNEL6],
               &gl_uint_ADC_SignalInvertConf[imod][INDEX_ADC_CHANNEL7],
               &gl_uint_ADC_SignalInvertConf[imod][INDEX_ADC_CHANNEL8] );
    }


    // get ADC_GAIN_FACTOR_FLAGS
    fscanf(used_file,"%s\n",temp_char);	      // get KEY-STRING
    comp_err = strcmp (temp_char, STRING_ADC_GAIN_FACTOR_FLAGS) && strcmp (temp_char, STRING_ADC_NEXT_NEIGHBOR_ENABLE_FLAGS); // check KEY-STRING
    if(comp_err != 0)  {
        fclose(used_file);
        return FILE_ACCESS_NOT_OK;
    }

    for (imod=0;imod<MAX_NO_OF_MODULES;imod++) {
        fscanf(used_file,"%8x %8x %8x %8x %8x %8x %8x %8x \n",	&gl_uint_ADC_Next_NeighborConf[imod][INDEX_ADC_CHANNEL1],
               &gl_uint_ADC_Next_NeighborConf[imod][INDEX_ADC_CHANNEL2],
               &gl_uint_ADC_Next_NeighborConf[imod][INDEX_ADC_CHANNEL3],
               &gl_uint_ADC_Next_NeighborConf[imod][INDEX_ADC_CHANNEL4],
               &gl_uint_ADC_Next_NeighborConf[imod][INDEX_ADC_CHANNEL5],
               &gl_uint_ADC_Next_NeighborConf[imod][INDEX_ADC_CHANNEL6],
               &gl_uint_ADC_Next_NeighborConf[imod][INDEX_ADC_CHANNEL7],
               &gl_uint_ADC_Next_NeighborConf[imod][INDEX_ADC_CHANNEL8] );
    }






    // get Energy Factor Values
    fscanf(used_file,"%s\n",temp_char);	      // get KEY-STRING
    comp_err = strcmp (temp_char, STRING_ENERGY_FACTOR_VALUES); // check KEY-STRING
    if(comp_err != 0)  {
        fclose(used_file);
        return FILE_ACCESS_NOT_OK;
    }
    for (imod=0;imod<MAX_NO_OF_MODULES;imod++) {
        for (iadc=0;iadc<NO_OF_ADC_CHANNELS;iadc++) {
            fscanf(used_file,"%f  \n",&gl_float_EnergyFactorConf[imod][iadc] );   //
        }
    }







    // get  Decay Tau Factor Values
    fscanf(used_file,"%s\n",temp_char);	      // get KEY-STRING
    comp_err = strcmp (temp_char, STRING_DECAY_TAU_VALUES); // check KEY-STRING
    if(comp_err != 0)  {
        fclose(used_file);
        return FILE_ACCESS_NOT_OK;
    }

    for (imod=0;imod<MAX_NO_OF_MODULES;imod++) {
        fscanf(used_file,"%4x %4x %4x %4x %4x %4x %4x %4x \n",	&gl_uint_DecayTauConf[imod][INDEX_ADC_CHANNEL1],
               &gl_uint_DecayTauConf[imod][INDEX_ADC_CHANNEL2],
               &gl_uint_DecayTauConf[imod][INDEX_ADC_CHANNEL3],
               &gl_uint_DecayTauConf[imod][INDEX_ADC_CHANNEL4],
               &gl_uint_DecayTauConf[imod][INDEX_ADC_CHANNEL5],
               &gl_uint_DecayTauConf[imod][INDEX_ADC_CHANNEL6],
               &gl_uint_DecayTauConf[imod][INDEX_ADC_CHANNEL7],
               &gl_uint_DecayTauConf[imod][INDEX_ADC_CHANNEL8] );
    }

    fclose(used_file);

    return FILE_ACCESS_OK;
}



int SIS3150CCI2::Configuration_SIS3302ADC (unsigned int module_addr, unsigned int module_conf_index )
{
    unsigned int error;
    unsigned int data, addr;
    unsigned int adc12_data, adc34_data, adc56_data, adc78_data;



    //Reset

    /*  Reset SIS330x  */
    addr = module_addr + SIS3302_KEY_RESET   ;
    if ((error = sub_vme_A32D32_write(addr,0x0 )) != 0) {
        sisVME_ErrorHandling (error, addr, "sub_vme_A32D32_write");
        return -1;
    }

    // Broadcast setup
    gl_uint_SIS3302_BroadcastAddrConf =   ADC_MODULE_CONF_DEFAULT_BROADCAST_ADDRESS ;
    data = (gl_uint_SIS3302_BroadcastAddrConf & 0xff000000)  ;
    data = data + SIS3302_BROADCAST_ENABLE  ;

    if (module_conf_index == 0) {
        data = data + SIS3302_BROADCAST_MASTER_ENABLE  ;
    }

    addr = module_addr + SIS3302_CBLT_BROADCAST_SETUP ;
    if ((error = sub_vme_A32D32_write(addr,data )) != 0) {
        sisVME_ErrorHandling (error, addr, "sub_vme_A32D32_write")   ;
        gl_uint_system_status = SYSTEM_STATUS_MODULES_NOT_READY ;
        return -1 ;
    }

    //set Clock source
    switch (gl_uint_SIS3302ClockModeConf)
    {
    case  0: //
        data = SIS3302_ACQ_SET_CLOCK_TO_100MHZ ;	   //
        break;
    case  1: //
        data = SIS3302_ACQ_SET_CLOCK_TO_50MHZ ;	   //
        break;
    case  2: //
        data = SIS3302_ACQ_SET_CLOCK_TO_25MHZ ;	   //

        break;
    case  3: //
        data = SIS3302_ACQ_SET_CLOCK_TO_10MHZ ;	   //
        break;
    case  4: //
        data = SIS3302_ACQ_SET_CLOCK_TO_1MHZ ;	   //
        break;
    case  5: //
        data = SIS3302_ACQ_SET_CLOCK_TO_LEMO_RANDOM_CLOCK_IN ;	   //
        break;
    case  6: // intern 25 MHz
        data = SIS3302_ACQ_SET_CLOCK_TO_LEMO_CLOCK_IN ;	   //
        break;
    case  7: //
        data = SIS3302_ACQ_SET_CLOCK_TO_SECOND_100MHZ;	   //
        break;
    } // end switch (uintClockMode)


    data = data + (gl_uint_SIS3302External_Lemo_InOutput_Conf & SIS3302_ACQ_SET_LEMO_IN_MODE_BIT_MASK) ;	   //
    data = data + (gl_uint_SIS3302External_Lemo_InOutput_Conf & SIS3302_ACQ_SET_LEMO_OUT_MODE_BIT_MASK) ;	   //
    data = data + (gl_uint_SIS3302External_Lemo_InOutput_Conf & SIS3302_ACQ_SET_FEEDBACK_INTERNAL_TRIGGER) ;	   //


    // Enable LEMO IN3 ,
    if ((gl_uint_SIS3302External_Lemo_InOutput_Conf & SIS3302_ACQ_ENABLE_EXTERNAL_LEMO_IN3) == SIS3302_ACQ_ENABLE_EXTERNAL_LEMO_IN3)  {
        data = data + SIS3302_ACQ_ENABLE_EXTERNAL_LEMO_IN3 ;	   //
    }

    // Enable LEMO IN2
    if ((gl_uint_SIS3302External_Lemo_InOutput_Conf & SIS3302_ACQ_ENABLE_EXTERNAL_LEMO_IN2) == SIS3302_ACQ_ENABLE_EXTERNAL_LEMO_IN2)  {
        data = data + SIS3302_ACQ_ENABLE_EXTERNAL_LEMO_IN2 ;	   //
    }
    // Enable LEMO IN1 ,
    if ((gl_uint_SIS3302External_Lemo_InOutput_Conf & SIS3302_ACQ_ENABLE_EXTERNAL_LEMO_IN1) == SIS3302_ACQ_ENABLE_EXTERNAL_LEMO_IN1)  {
        data = data + SIS3302_ACQ_ENABLE_EXTERNAL_LEMO_IN1 ;	   //
    }



    // write data to acquisition control register
    addr = module_addr + SIS3302_ACQUISITION_CONTROL ;
    if ((error = sub_vme_A32D32_write(addr,data )) != 0) {
        sisVME_ErrorHandling (error, addr, "sub_vme_A32D32_write")   ;
        gl_uint_system_status = SYSTEM_STATUS_MODULES_NOT_READY ;
        return -1 ;
    }

    // Event Config (all adc-FPGAs)
    //data = 0 ;	   //

    if ((gl_uint_SIS3302_Trigger_Mode_Source_ModeConf & 0x10) == 0x10) { // Coincidence mode
            data =    EVENT_CONF_ADC2_INTERN_GATE_ENABLE_BIT
                    + EVENT_CONF_ADC1_NP1_NB_GATE_ENABLE_BIT 	   //
                    + EVENT_CONF_ADC1_INTERN_TRIGGER_ENABLE_BIT;	   //
    }
    else { // no  Coincidence mode
            data = 0 ;	   //
            if ((gl_uint_SIS3302_Trigger_Mode_Source_ModeConf & 0x8) == 0x8) { //  Bit 3   = Enable ADCx External Gate
                    data = data + EVENT_CONF_ADC1_EXTERN_GATE_ENABLE_BIT + EVENT_CONF_ADC2_EXTERN_GATE_ENABLE_BIT ;	   //
            }

            if ((gl_uint_SIS3302_Trigger_Mode_Source_ModeConf & 0x4) == 0x4) { //  Bit 2   = Enable ADCx Internal Gate
                    data = data + EVENT_CONF_ADC1_INTERN_GATE_ENABLE_BIT + EVENT_CONF_ADC2_INTERN_GATE_ENABLE_BIT ;	   //
            }

            if ((gl_uint_SIS3302_Trigger_Mode_Source_ModeConf & 0x2) == 0x2) { //  Bit 1   = Enable ADCx External Trigger
                    data = data + EVENT_CONF_ADC1_EXTERN_TRIGGER_ENABLE_BIT + EVENT_CONF_ADC2_EXTERN_TRIGGER_ENABLE_BIT ;	   //
            }

            if ((gl_uint_SIS3302_Trigger_Mode_Source_ModeConf & 0x1) == 0x1) { //  Bit 0   = Enable ADCx Internal Trigger
                    data = data + EVENT_CONF_ADC1_INTERN_TRIGGER_ENABLE_BIT + EVENT_CONF_ADC2_INTERN_TRIGGER_ENABLE_BIT ;	   //
            }
    }




    // channel ID = Module Address 31:27
    data = data  + (module_addr & 0xf8000000) ;	   //

    adc12_data = data ;	   //
    adc34_data = data ;	   //
    adc56_data = data ;	   //
    adc78_data = data ;	   //




    //ADC1,2
    adc12_data =   adc12_data
                   + ((gl_uint_ADC_Next_NeighborConf[module_conf_index][INDEX_ADC_CHANNEL1] & 0x3) << 6)
                   + ((gl_uint_ADC_Next_NeighborConf[module_conf_index][INDEX_ADC_CHANNEL2] & 0x3) << 14) ; //

    adc34_data =   adc34_data
                   + ((gl_uint_ADC_Next_NeighborConf[module_conf_index][INDEX_ADC_CHANNEL3] & 0x3) << 6)
                   + ((gl_uint_ADC_Next_NeighborConf[module_conf_index][INDEX_ADC_CHANNEL4] & 0x3) << 14) ; //

    adc56_data =   adc56_data
                   + ((gl_uint_ADC_Next_NeighborConf[module_conf_index][INDEX_ADC_CHANNEL5] & 0x3) << 6)
                   + ((gl_uint_ADC_Next_NeighborConf[module_conf_index][INDEX_ADC_CHANNEL6] & 0x3) << 14) ; //

    adc78_data =   adc78_data
                   + ((gl_uint_ADC_Next_NeighborConf[module_conf_index][INDEX_ADC_CHANNEL7] & 0x3) << 6)
                   + ((gl_uint_ADC_Next_NeighborConf[module_conf_index][INDEX_ADC_CHANNEL8] & 0x3) << 14) ; //


    //ADC1,2
    if (gl_uint_ADC_SignalInvertConf[module_conf_index][INDEX_ADC_CHANNEL1] != 0) {
        adc12_data = adc12_data + EVENT_CONF_ADC1_INPUT_INVERT_BIT ;	   //
    }
    if (gl_uint_ADC_SignalInvertConf[module_conf_index][INDEX_ADC_CHANNEL2] != 0) {
        adc12_data = adc12_data + EVENT_CONF_ADC2_INPUT_INVERT_BIT ;	   //
    }

    //ADC3,4
    if (gl_uint_ADC_SignalInvertConf[module_conf_index][INDEX_ADC_CHANNEL3] != 0) {
        adc34_data = adc34_data + EVENT_CONF_ADC1_INPUT_INVERT_BIT ;	   //
    }
    if (gl_uint_ADC_SignalInvertConf[module_conf_index][INDEX_ADC_CHANNEL4] != 0) {
        adc34_data = adc34_data + EVENT_CONF_ADC2_INPUT_INVERT_BIT ;	   //
    }

    //ADC5,6
    if (gl_uint_ADC_SignalInvertConf[module_conf_index][INDEX_ADC_CHANNEL5] != 0) {
        adc56_data = adc56_data + EVENT_CONF_ADC1_INPUT_INVERT_BIT ;	   //
    }
    if (gl_uint_ADC_SignalInvertConf[module_conf_index][INDEX_ADC_CHANNEL6] != 0) {
        adc56_data = adc56_data + EVENT_CONF_ADC2_INPUT_INVERT_BIT ;	   //
    }

    //ADC7,8
    if (gl_uint_ADC_SignalInvertConf[module_conf_index][INDEX_ADC_CHANNEL7] != 0) {
        adc78_data = adc78_data + EVENT_CONF_ADC1_INPUT_INVERT_BIT ;	   //
    }
    if (gl_uint_ADC_SignalInvertConf[module_conf_index][INDEX_ADC_CHANNEL8] != 0) {
        adc78_data = adc78_data + EVENT_CONF_ADC2_INPUT_INVERT_BIT ;	   //
    }



    addr = module_addr + SIS3302_EVENT_CONFIG_ADC12 ;
    if ((error = sub_vme_A32D32_write(addr,adc12_data )) != 0) {
        sisVME_ErrorHandling (error, addr, "sub_vme_A32D32_write")   ;
        gl_uint_system_status = SYSTEM_STATUS_MODULES_NOT_READY ;
    }
    addr = module_addr + SIS3302_EVENT_CONFIG_ADC34 ;
    if ((error = sub_vme_A32D32_write(addr,adc34_data )) != 0) {
        sisVME_ErrorHandling (error, addr, "sub_vme_A32D32_write")   ;
        gl_uint_system_status = SYSTEM_STATUS_MODULES_NOT_READY ;
    }

    if(gl_uint_SIS3302_FourChannelFlag == 0x0) { //

        addr = module_addr + SIS3302_EVENT_CONFIG_ADC56 ;
        if ((error = sub_vme_A32D32_write(addr,adc56_data )) != 0) {
            sisVME_ErrorHandling (error, addr, "sub_vme_A32D32_write")   ;
            gl_uint_system_status = SYSTEM_STATUS_MODULES_NOT_READY ;
        }
        addr = module_addr + SIS3302_EVENT_CONFIG_ADC78 ;
        if ((error = sub_vme_A32D32_write(addr,adc78_data )) != 0) {
            sisVME_ErrorHandling (error, addr, "sub_vme_A32D32_write")   ;
            gl_uint_system_status = SYSTEM_STATUS_MODULES_NOT_READY ;
        }

    }



    // event extended configuration register

    adc12_data = 0 ;	   //
    adc34_data = 0 ;	   //
    adc56_data = 0 ;	   //
    adc78_data = 0 ;	   //

    if ((gl_uint_SIS3302_Trigger_Mode_Source_ModeConf & 0x10) == 0x10) { // Coincidence mode
            adc12_data =    0x4000 ;	    // ADC2 ADC N-1 next neigbor trigger enable
            adc34_data =    0x4000 ;	    // ADC2 ADC N-1 next neigbor trigger enable
            adc56_data =    0x4000 ;	    // ADC2 ADC N-1 next neigbor trigger enable
            adc78_data =    0x4000 ;	    // ADC2 ADC N-1 next neigbor trigger enable
    }
    else { // no  Coincidence mode
            //ADC1,2
            adc12_data =   adc12_data
                         + ((gl_uint_ADC_Next_NeighborConf[module_conf_index][INDEX_ADC_CHANNEL1] & 0xC) << 4)
                         + ((gl_uint_ADC_Next_NeighborConf[module_conf_index][INDEX_ADC_CHANNEL2] & 0xC) << 12) ; //

            adc34_data =   adc34_data
                         + ((gl_uint_ADC_Next_NeighborConf[module_conf_index][INDEX_ADC_CHANNEL3] & 0xC) << 4)
                     + ((gl_uint_ADC_Next_NeighborConf[module_conf_index][INDEX_ADC_CHANNEL4] & 0xC) << 12) ; //

            adc56_data =   adc56_data
                         + ((gl_uint_ADC_Next_NeighborConf[module_conf_index][INDEX_ADC_CHANNEL5] & 0xC) << 4)
                     + ((gl_uint_ADC_Next_NeighborConf[module_conf_index][INDEX_ADC_CHANNEL6] & 0xC) << 12) ; //

            adc78_data =   adc78_data
                         + ((gl_uint_ADC_Next_NeighborConf[module_conf_index][INDEX_ADC_CHANNEL7] & 0xC) << 4)
                         + ((gl_uint_ADC_Next_NeighborConf[module_conf_index][INDEX_ADC_CHANNEL8] & 0xC) << 12) ; //
    }



    addr = module_addr + SIS3302_EVENT_EXTENDED_CONFIG_ADC12 ;
    if ((error = sub_vme_A32D32_write(addr,adc12_data )) != 0) {
        sisVME_ErrorHandling (error, addr, "sub_vme_A32D32_write")   ;
        gl_uint_system_status = SYSTEM_STATUS_MODULES_NOT_READY ;
    }
    addr = module_addr + SIS3302_EVENT_EXTENDED_CONFIG_ADC34 ;
    if ((error = sub_vme_A32D32_write(addr,adc34_data )) != 0) {
        sisVME_ErrorHandling (error, addr, "sub_vme_A32D32_write")   ;
        gl_uint_system_status = SYSTEM_STATUS_MODULES_NOT_READY ;
    }

    if(gl_uint_SIS3302_FourChannelFlag == 0x0) { //

        addr = module_addr + SIS3302_EVENT_EXTENDED_CONFIG_ADC56 ;
        if ((error = sub_vme_A32D32_write(addr,adc56_data )) != 0) {
            sisVME_ErrorHandling (error, addr, "sub_vme_A32D32_write")   ;
            gl_uint_system_status = SYSTEM_STATUS_MODULES_NOT_READY ;
        }
        addr = module_addr + SIS3302_EVENT_EXTENDED_CONFIG_ADC78 ;
        if ((error = sub_vme_A32D32_write(addr,adc78_data )) != 0) {
            sisVME_ErrorHandling (error, addr, "sub_vme_A32D32_write")   ;
            gl_uint_system_status = SYSTEM_STATUS_MODULES_NOT_READY ;
        }

    }




    // it is here only a first configuration, it will be reconfigured in run-program
    // address threshold flag
    data = 0x1c0 ;		// write 0x1c0 x 2 samples
    addr = module_addr + SIS3302_END_ADDRESS_THRESHOLD_ALL_ADC ;
    if ((error = sub_vme_A32D32_write(addr,data )) != 0) {
        sisVME_ErrorHandling (error, addr, "sub_vme_A32D32_write")   ;
        gl_uint_system_status = SYSTEM_STATUS_MODULES_NOT_READY ;
        return -1 ;
    }


    // Fast FIR Trigger
    sub_sis3302_fastTriggerOut_setup(module_addr, module_conf_index) ;



    // DACs
    //sub_sis3302_Configuration_DACs_setup(module_addr, module_conf_index)  ;
    // This method was short and only used once so it has been included inline below
        unsigned int i_adc;
        unsigned int dac_offset[8];


        for (i_adc=0;i_adc<8;i_adc++) {
                        dac_offset[i_adc]	= gl_uint_DacOffset_Conf[module_conf_index][i_adc] ;
        }
        error=sis3302_write_dac_offset(module_addr, dac_offset)  ;
        //error=sis3320_write_dac_offset(module_addr, dac_offset)  ;
        if (error != 0) {
                sisVME_ErrorHandling (error, module_addr, "sis3302_write_dac_offset")   ;
                gl_uint_system_status = SYSTEM_STATUS_MODULES_NOT_READY ;
                return -1 ;
        }

        //end of included method


    // SIS3302_PRETRIGGER_Delay and TriggerGATE_ALL_ADC
    data = (gl_uint_PreTriggerDelay << 16) + gl_uint_TriggerGateLength      ; //
    addr = module_addr + SIS3302_PRETRIGGER_DELAY_TRIGGERGATE_LENGTH_ALL_ADC ;
    if ((error = sub_vme_A32D32_write(addr,data )) != 0) {
        sisVME_ErrorHandling (error, addr, "sub_vme_A32D32_write")   ;
        gl_uint_system_status = SYSTEM_STATUS_MODULES_NOT_READY ;
        return -1 ;
    }



    // Energy Window
    data =    gl_uint_EnergyGateLength ;	   //

    addr = module_addr + SIS3302_ENERGY_GATE_LENGTH_ALL_ADC ;
    if ((error = sub_vme_A32D32_write(addr,data )) != 0) {
        sisVME_ErrorHandling (error, addr, "sub_vme_A32D32_write")   ;
        gl_uint_system_status = SYSTEM_STATUS_MODULES_NOT_READY ;
        return -1 ;
    }


    // Energy Setup GP
    // decimation mode
    data = 0 ;
    switch (gl_uint_FirDecimationMode)
    {
    case  0: // no decimation
        data = DECIMATION_DISABLE ;	   //
        break;
    case  1: //
        data = DECIMATION_2 ;	   //
        break;
    case  2: //
        data = DECIMATION_4 ;	   //
        break;
    case  3: //
        data = DECIMATION_8;	   //
        break;
    } // end switch (gl_uint_FirDecimationMode)

    // GP and shaping
    data =     data
               //  +  ((gl_uint_FirMwShapingLength << 16) & 0x00ff0000)  	   //
               +  ((gl_uint_FirEnergyGap << 8)  & 0x0ff00)  	   //
               +  (gl_uint_FirEnergyLength & 0x0ff) 	   //
               +  ((gl_uint_FirEnergyLength & 0x300) << 8);	   //  new v_1406
    addr = module_addr + SIS3302_ENERGY_SETUP_GP_ALL_ADC ;
    if ((error = sub_vme_A32D32_write(addr,data )) != 0) {
        sisVME_ErrorHandling (error, addr, "sub_vme_A32D32_write")   ;
        gl_uint_system_status = SYSTEM_STATUS_MODULES_NOT_READY ;
        return -1 ;
    }


    // Tau Factor
    data =     ((gl_uint_DecayTauConf[module_conf_index][INDEX_ADC_CHANNEL1] & 0x7f) )    ;

    addr = module_addr + SIS3302_ENERGY_TAU_FACTOR_ADC1 ;
    if ((error = sub_vme_A32D32_write(addr,data )) != 0) {
        sisVME_ErrorHandling (error, addr, "sub_vme_A32D32_write")   ;
        gl_uint_system_status = SYSTEM_STATUS_MODULES_NOT_READY ;
    }

    data =     ((gl_uint_DecayTauConf[module_conf_index][INDEX_ADC_CHANNEL2] & 0x7f) )    ;
    addr = module_addr + SIS3302_ENERGY_TAU_FACTOR_ADC2 ;
    if ((error = sub_vme_A32D32_write(addr,data )) != 0) {
        sisVME_ErrorHandling (error, addr, "sub_vme_A32D32_write")   ;
        gl_uint_system_status = SYSTEM_STATUS_MODULES_NOT_READY ;
    }

    data =     ((gl_uint_DecayTauConf[module_conf_index][INDEX_ADC_CHANNEL3] & 0x7f) )    ;
    addr = module_addr + SIS3302_ENERGY_TAU_FACTOR_ADC3 ;
    if ((error = sub_vme_A32D32_write(addr,data )) != 0) {
        sisVME_ErrorHandling (error, addr, "sub_vme_A32D32_write")   ;
        gl_uint_system_status = SYSTEM_STATUS_MODULES_NOT_READY ;
    }

    data =     ((gl_uint_DecayTauConf[module_conf_index][INDEX_ADC_CHANNEL4] & 0x7f) )    ;
    addr = module_addr + SIS3302_ENERGY_TAU_FACTOR_ADC4 ;
    if ((error = sub_vme_A32D32_write(addr,data )) != 0) {
        sisVME_ErrorHandling (error, addr, "sub_vme_A32D32_write")   ;
        gl_uint_system_status = SYSTEM_STATUS_MODULES_NOT_READY ;
    }


    if(gl_uint_SIS3302_FourChannelFlag == 0x0) { //

        data =     ((gl_uint_DecayTauConf[module_conf_index][INDEX_ADC_CHANNEL5] & 0x7f) )    ;
        addr = module_addr + SIS3302_ENERGY_TAU_FACTOR_ADC5 ;
        if ((error = sub_vme_A32D32_write(addr,data )) != 0) {
            sisVME_ErrorHandling (error, addr, "sub_vme_A32D32_write")   ;
            gl_uint_system_status = SYSTEM_STATUS_MODULES_NOT_READY ;
        }

        data =     ((gl_uint_DecayTauConf[module_conf_index][INDEX_ADC_CHANNEL6] & 0x7f) )    ;
        addr = module_addr + SIS3302_ENERGY_TAU_FACTOR_ADC6 ;
        if ((error = sub_vme_A32D32_write(addr,data )) != 0) {
            sisVME_ErrorHandling (error, addr, "sub_vme_A32D32_write")   ;
            gl_uint_system_status = SYSTEM_STATUS_MODULES_NOT_READY ;
        }

        data =     ((gl_uint_DecayTauConf[module_conf_index][INDEX_ADC_CHANNEL7] & 0x7f) )    ;
        addr = module_addr + SIS3302_ENERGY_TAU_FACTOR_ADC7 ;
        if ((error = sub_vme_A32D32_write(addr,data )) != 0) {
            sisVME_ErrorHandling (error, addr, "sub_vme_A32D32_write")   ;
            gl_uint_system_status = SYSTEM_STATUS_MODULES_NOT_READY ;
        }

        data =     ((gl_uint_DecayTauConf[module_conf_index][INDEX_ADC_CHANNEL8] & 0x7f) )    ;
        addr = module_addr + SIS3302_ENERGY_TAU_FACTOR_ADC8 ;
        if ((error = sub_vme_A32D32_write(addr,data )) != 0) {
            sisVME_ErrorHandling (error, addr, "sub_vme_A32D32_write")   ;
            gl_uint_system_status = SYSTEM_STATUS_MODULES_NOT_READY ;
        }

    }


    // raw data buffer configuration
    data =     (gl_uint_RawSampleStart_Index & 0xfffe)     // ist jetzt start index
               + ((gl_uint_RawSampleLength & 0xfffc) << 16) ;
    addr = module_addr + SIS3302_RAW_DATA_BUFFER_CONFIG_ALL_ADC ;
    if ((error = sub_vme_A32D32_write(addr,data )) != 0) {
        sisVME_ErrorHandling (error, addr, "sub_vme_A32D32_write")   ;
        gl_uint_system_status = SYSTEM_STATUS_MODULES_NOT_READY ;
        return -1 ;
    }


    // energy sample length
    data = 0 ;	   //
    switch (gl_uint_EnergySampleMode)
    {
    case  0: // //  Energy Trapezoidal 500 values
        data = 510   ;	   //
        break;
    case  1: // / no Energy Trapezoidal values
        data = 0 ;	   //
        break;
    case  2: // / 3 parts programmable length  not supported yet
        data = gl_uint_EnergySampleLength ;	   //
        break;
    case  3: // / programmable length  not supported yet
        data = gl_uint_EnergySampleLength ;	   //
        break;
    } // end switch (gl_uint_EnergySampleMode)

    addr = module_addr + SIS3302_ENERGY_SAMPLE_LENGTH_ALL_ADC ;
    if ((error = sub_vme_A32D32_write(addr,data )) != 0) {
        sisVME_ErrorHandling (error, addr, "sub_vme_A32D32_write")   ;
        gl_uint_system_status = SYSTEM_STATUS_MODULES_NOT_READY ;
        return -1 ;
    }

    data = gl_uint_EnergySampleStart_Index1 ;
    addr = module_addr + SIS3302_ENERGY_SAMPLE_START_INDEX1_ALL_ADC ;
    if ((error = sub_vme_A32D32_write(addr,data )) != 0) {
        sisVME_ErrorHandling (error, addr, "sub_vme_A32D32_write")   ;
        gl_uint_system_status = SYSTEM_STATUS_MODULES_NOT_READY ;
        return -1 ;
    }

    data = gl_uint_EnergySampleStart_Index2 ;
    addr = module_addr + SIS3302_ENERGY_SAMPLE_START_INDEX2_ALL_ADC ;
    if ((error = sub_vme_A32D32_write(addr,data )) != 0) {
        sisVME_ErrorHandling (error, addr, "sub_vme_A32D32_write")   ;
        gl_uint_system_status = SYSTEM_STATUS_MODULES_NOT_READY ;
        return -1 ;
    }

    data = gl_uint_EnergySampleStart_Index3 ; // zero is disable
    addr = module_addr + SIS3302_ENERGY_SAMPLE_START_INDEX3_ALL_ADC ;
    if ((error = sub_vme_A32D32_write(addr,data )) != 0) {
        sisVME_ErrorHandling (error, addr, "sub_vme_A32D32_write")   ;
        gl_uint_system_status = SYSTEM_STATUS_MODULES_NOT_READY ;
        return -1 ;
    }


    // VME IRQ generation
    data = 0x800 + 0x100 ;		// IRQ 6 enable
    addr = module_addr + SIS3302_IRQ_CONFIG ;
    if ((error = sub_vme_A32D32_write(addr,data )) != 0) {
        sisVME_ErrorHandling (error, addr, "sub_vme_A32D32_write")   ;
        gl_uint_system_status = SYSTEM_STATUS_MODULES_NOT_READY ;
        return -1 ;
    }

    data = 0x2 ;		// Enable IRQ Address Threshold Flag Level sensitive
    addr = module_addr + SIS3302_IRQ_CONTROL ;
    if ((error = sub_vme_A32D32_write(addr,data )) != 0) {
        sisVME_ErrorHandling (error, addr, "sub_vme_A32D32_write")   ;
        gl_uint_system_status = SYSTEM_STATUS_MODULES_NOT_READY ;
        return -1 ;
    }

#ifdef raus
                addr = module_addr + SIS3302_KEY_SAMPLE_LOGIC_RESET ;
                if ((error = sub_vme_A32D32_write(addr,0 )) != 0) {
                        sisVME_ErrorHandling (error, addr, "sub_vme_A32D32_write")   ;
                        gl_uint_system_status = SYSTEM_STATUS_MODULES_NOT_READY ;
                        return -1 ;
                }

 #endif

    addr = module_addr + SIS3302_KEY_0x404_SAMPLE_LOGIC_RESET ;
    if ((error = sub_vme_A32D32_write(addr,0 )) != 0) {
        sisVME_ErrorHandling (error, addr, "sub_vme_A32D32_write")   ;
        gl_uint_system_status = SYSTEM_STATUS_MODULES_NOT_READY ;
        return -1 ;
    }

    return 0x0 ;


} // end function  ()



/* --------------------------------------------------------------------------
   SIS3302 Fast Trigger Out Setup
   module_addr
-------------------------------------------------------------------------- */
int SIS3150CCI2::sub_sis3302_fastTriggerOut_setup(unsigned int module_addr, unsigned int module_conf_index){
        unsigned int error;
        unsigned int data, extended_data, addr, base_addr;
        unsigned int g_value, p_value, puls_length_value, trigger_delay_value, gate_length_value;
        int threshold_value ;
        unsigned int threshold_flag;



        //Ch 1
                // Trigger G and P
                g_value           = gl_uint_FirTriggerG_Conf[module_conf_index] [INDEX_ADC_CHANNEL1] ;
                p_value           = gl_uint_FirTriggerP_Conf[module_conf_index] [INDEX_ADC_CHANNEL1] ;
                puls_length_value = gl_uint_FirTriggerPulseLength_Conf[module_conf_index] [INDEX_ADC_CHANNEL1] & 0xffff ;
                trigger_delay_value = (gl_uint_FirTriggerPulseLength_Conf[module_conf_index] [INDEX_ADC_CHANNEL1] & 0xff000000) >> 24 ;
                gate_length_value   = (gl_uint_FirTriggerPulseLength_Conf[module_conf_index] [INDEX_ADC_CHANNEL1] & 0x00ff0000) >> 16 ;

// begin mod 9.8.2010
                extended_data = 	 ((trigger_delay_value & 0xff) << 24 ) + ((p_value & 0x0f000000) >> 8) + (g_value & 0xf00) + ((p_value & 0xf00) >> 8) ;
// end mod 9.8.2010

                data = ((gate_length_value & 0xff) << 24) + ((puls_length_value & 0xff) << 16) + ((g_value & 0xff) << 8) + (p_value & 0xff)   ; //
                addr = module_addr + SIS3302_TRIGGER_SETUP_ADC1 ;
                if ((error = sub_vme_A32D32_write(addr,data )) != 0) {
                        sisVME_ErrorHandling (error, addr, "sub_vme_A32D32_write");
                        gl_uint_system_status = SYSTEM_STATUS_MODULES_NOT_READY ;
                }

                addr = module_addr + SIS3302_TRIGGER_EXTENDED_SETUP_ADC1 ;
                if ((error = sub_vme_A32D32_write(addr,extended_data )) != 0) {
                        sisVME_ErrorHandling (error, addr, "sub_vme_A32D32_write");
                        gl_uint_system_status = SYSTEM_STATUS_MODULES_NOT_READY ;
                }



        //Ch 2
                g_value           = gl_uint_FirTriggerG_Conf[module_conf_index] [INDEX_ADC_CHANNEL2] ;
                p_value           = gl_uint_FirTriggerP_Conf[module_conf_index] [INDEX_ADC_CHANNEL2] ;
                puls_length_value = gl_uint_FirTriggerPulseLength_Conf[module_conf_index] [INDEX_ADC_CHANNEL2] & 0xffff ;
                trigger_delay_value = (gl_uint_FirTriggerPulseLength_Conf[module_conf_index] [INDEX_ADC_CHANNEL2] & 0xff000000) >> 24 ;
                gate_length_value   = (gl_uint_FirTriggerPulseLength_Conf[module_conf_index] [INDEX_ADC_CHANNEL2] & 0x00ff0000) >> 16 ;

// begin mod 9.8.2010
                extended_data = 	 ((trigger_delay_value & 0xff) << 24 ) + ((p_value & 0x0f000000) >> 8) + (g_value & 0xf00) + ((p_value & 0xf00) >> 8) ;
// end mod 9.8.2010
                data = ((gate_length_value & 0xff) << 24) + ((puls_length_value & 0xff) << 16) + ((g_value & 0xff) << 8) + (p_value & 0xff)   ; //
                addr = module_addr + SIS3302_TRIGGER_SETUP_ADC2 ;
                if ((error = sub_vme_A32D32_write(addr,data )) != 0) {
                        sisVME_ErrorHandling (error, addr, "sub_vme_A32D32_write");
                        gl_uint_system_status = SYSTEM_STATUS_MODULES_NOT_READY ;
                }

                addr = module_addr + SIS3302_TRIGGER_EXTENDED_SETUP_ADC2 ;
                if ((error = sub_vme_A32D32_write(addr,extended_data )) != 0) {
                        sisVME_ErrorHandling (error, addr, "sub_vme_A32D32_write");
                        gl_uint_system_status = SYSTEM_STATUS_MODULES_NOT_READY ;
                }



        //Ch 3
                g_value           = gl_uint_FirTriggerG_Conf[module_conf_index] [INDEX_ADC_CHANNEL3] ;
                p_value           = gl_uint_FirTriggerP_Conf[module_conf_index] [INDEX_ADC_CHANNEL3] ;
                puls_length_value = gl_uint_FirTriggerPulseLength_Conf[module_conf_index] [INDEX_ADC_CHANNEL3] & 0xffff ;
                trigger_delay_value = (gl_uint_FirTriggerPulseLength_Conf[module_conf_index] [INDEX_ADC_CHANNEL3] & 0xff000000) >> 24 ;
                gate_length_value   = (gl_uint_FirTriggerPulseLength_Conf[module_conf_index] [INDEX_ADC_CHANNEL3] & 0x00ff0000) >> 16 ;

// begin mod 9.8.2010
                extended_data = 	 ((trigger_delay_value & 0xff) << 24 ) + ((p_value & 0x0f000000) >> 8) + (g_value & 0xf00) + ((p_value & 0xf00) >> 8) ;
// end mod 9.8.2010
                data = ((gate_length_value & 0xff) << 24) + ((puls_length_value & 0xff) << 16) + ((g_value & 0xff) << 8) + (p_value & 0xff)   ; //
                addr = module_addr + SIS3302_TRIGGER_SETUP_ADC3 ;
                if ((error = sub_vme_A32D32_write(addr,data )) != 0) {
                        sisVME_ErrorHandling (error, addr, "sub_vme_A32D32_write");
                        gl_uint_system_status = SYSTEM_STATUS_MODULES_NOT_READY ;
                }

                addr = module_addr + SIS3302_TRIGGER_EXTENDED_SETUP_ADC3 ;
                if ((error = sub_vme_A32D32_write(addr,extended_data )) != 0) {
                        sisVME_ErrorHandling (error, addr, "sub_vme_A32D32_write");
                        gl_uint_system_status = SYSTEM_STATUS_MODULES_NOT_READY ;
                }



        //Ch 4
                g_value           = gl_uint_FirTriggerG_Conf[module_conf_index] [INDEX_ADC_CHANNEL4] ;
                p_value           = gl_uint_FirTriggerP_Conf[module_conf_index] [INDEX_ADC_CHANNEL4] ;
                puls_length_value = gl_uint_FirTriggerPulseLength_Conf[module_conf_index] [INDEX_ADC_CHANNEL4] & 0xffff ;
                trigger_delay_value = (gl_uint_FirTriggerPulseLength_Conf[module_conf_index] [INDEX_ADC_CHANNEL4] & 0xff000000) >> 24 ;
                gate_length_value   = (gl_uint_FirTriggerPulseLength_Conf[module_conf_index] [INDEX_ADC_CHANNEL4] & 0x00ff0000) >> 16 ;

// begin mod 9.8.2010
                extended_data = 	 ((trigger_delay_value & 0xff) << 24 ) + ((p_value & 0x0f000000) >> 8) + (g_value & 0xf00) + ((p_value & 0xf00) >> 8) ;
// end mod 9.8.2010
                data = ((gate_length_value & 0xff) << 24) + ((puls_length_value & 0xff) << 16) + ((g_value & 0xff) << 8) + (p_value & 0xff)   ; //
                addr = module_addr + SIS3302_TRIGGER_SETUP_ADC4 ;
                if ((error = sub_vme_A32D32_write(addr,data )) != 0) {
                        sisVME_ErrorHandling (error, addr, "sub_vme_A32D32_write");
                        gl_uint_system_status = SYSTEM_STATUS_MODULES_NOT_READY ;
                }

                addr = module_addr + SIS3302_TRIGGER_EXTENDED_SETUP_ADC4 ;
                if ((error = sub_vme_A32D32_write(addr,extended_data )) != 0) {
                        sisVME_ErrorHandling (error, addr, "sub_vme_A32D32_write");
                        gl_uint_system_status = SYSTEM_STATUS_MODULES_NOT_READY ;
                }




        if(gl_uint_SIS3302_FourChannelFlag == 0x0) { //

        //Ch 5
                g_value           = gl_uint_FirTriggerG_Conf[module_conf_index] [INDEX_ADC_CHANNEL5] ;
                p_value           = gl_uint_FirTriggerP_Conf[module_conf_index] [INDEX_ADC_CHANNEL5] ;
                puls_length_value = gl_uint_FirTriggerPulseLength_Conf[module_conf_index] [INDEX_ADC_CHANNEL5] & 0xffff ;
                trigger_delay_value = (gl_uint_FirTriggerPulseLength_Conf[module_conf_index] [INDEX_ADC_CHANNEL5] & 0xff000000) >> 24 ;
                gate_length_value   = (gl_uint_FirTriggerPulseLength_Conf[module_conf_index] [INDEX_ADC_CHANNEL5] & 0x00ff0000) >> 16 ;

// begin mod 9.8.2010
                extended_data = 	 ((trigger_delay_value & 0xff) << 24 ) + ((p_value & 0x0f000000) >> 8) + (g_value & 0xf00) + ((p_value & 0xf00) >> 8) ;
// end mod 9.8.2010
                data = ((gate_length_value & 0xff) << 24) + ((puls_length_value & 0xff) << 16) + ((g_value & 0xff) << 8) + (p_value & 0xff)   ; //
                addr = module_addr + SIS3302_TRIGGER_SETUP_ADC5 ;
                if ((error = sub_vme_A32D32_write(addr,data )) != 0) {
                        sisVME_ErrorHandling (error, addr, "sub_vme_A32D32_write");
                        gl_uint_system_status = SYSTEM_STATUS_MODULES_NOT_READY ;
                }

                addr = module_addr + SIS3302_TRIGGER_EXTENDED_SETUP_ADC5 ;
                if ((error = sub_vme_A32D32_write(addr,extended_data )) != 0) {
                        sisVME_ErrorHandling (error, addr, "sub_vme_A32D32_write");
                        gl_uint_system_status = SYSTEM_STATUS_MODULES_NOT_READY ;
                }

        //Ch 6
                // Trigger G and P
                g_value           = gl_uint_FirTriggerG_Conf[module_conf_index] [INDEX_ADC_CHANNEL6] ;
                p_value           = gl_uint_FirTriggerP_Conf[module_conf_index] [INDEX_ADC_CHANNEL6] ;
                puls_length_value = gl_uint_FirTriggerPulseLength_Conf[module_conf_index] [INDEX_ADC_CHANNEL6] & 0xffff ;
                trigger_delay_value = (gl_uint_FirTriggerPulseLength_Conf[module_conf_index] [INDEX_ADC_CHANNEL6] & 0xff000000) >> 24 ;
                gate_length_value   = (gl_uint_FirTriggerPulseLength_Conf[module_conf_index] [INDEX_ADC_CHANNEL6] & 0x00ff0000) >> 16 ;

// begin mod 9.8.2010
                extended_data = 	 ((trigger_delay_value & 0xff) << 24 ) + ((p_value & 0x0f000000) >> 8) + (g_value & 0xf00) + ((p_value & 0xf00) >> 8) ;
// end mod 9.8.2010
                data = ((gate_length_value & 0xff) << 24) + ((puls_length_value & 0xff) << 16) + ((g_value & 0xff) << 8) + (p_value & 0xff)   ; //
                addr = module_addr + SIS3302_TRIGGER_SETUP_ADC6 ;
                if ((error = sub_vme_A32D32_write(addr,data )) != 0) {
                        sisVME_ErrorHandling (error, addr, "sub_vme_A32D32_write");
                        gl_uint_system_status = SYSTEM_STATUS_MODULES_NOT_READY ;
                }

                addr = module_addr + SIS3302_TRIGGER_EXTENDED_SETUP_ADC6 ;
                if ((error = sub_vme_A32D32_write(addr,extended_data )) != 0) {
                        sisVME_ErrorHandling (error, addr, "sub_vme_A32D32_write");
                        gl_uint_system_status = SYSTEM_STATUS_MODULES_NOT_READY ;
                }

        //Ch 7
                // Trigger G and P
                g_value           = gl_uint_FirTriggerG_Conf[module_conf_index] [INDEX_ADC_CHANNEL7] ;
                p_value           = gl_uint_FirTriggerP_Conf[module_conf_index] [INDEX_ADC_CHANNEL7] ;
                puls_length_value = gl_uint_FirTriggerPulseLength_Conf[module_conf_index] [INDEX_ADC_CHANNEL7] & 0xffff ;
                trigger_delay_value = (gl_uint_FirTriggerPulseLength_Conf[module_conf_index] [INDEX_ADC_CHANNEL7] & 0xff000000) >> 24 ;
                gate_length_value   = (gl_uint_FirTriggerPulseLength_Conf[module_conf_index] [INDEX_ADC_CHANNEL7] & 0x00ff0000) >> 16 ;

// begin mod 9.8.2010
                extended_data = 	 ((trigger_delay_value & 0xff) << 24 ) + ((p_value & 0x0f000000) >> 8) + (g_value & 0xf00) + ((p_value & 0xf00) >> 8) ;
// end mod 9.8.2010
                data = ((gate_length_value & 0xff) << 24) + ((puls_length_value & 0xff) << 16) + ((g_value & 0xff) << 8) + (p_value & 0xff)   ; //
                addr = module_addr + SIS3302_TRIGGER_SETUP_ADC7 ;
                if ((error = sub_vme_A32D32_write(addr,data )) != 0) {
                        sisVME_ErrorHandling (error, addr, "sub_vme_A32D32_write");
                        gl_uint_system_status = SYSTEM_STATUS_MODULES_NOT_READY ;
                }

                addr = module_addr + SIS3302_TRIGGER_EXTENDED_SETUP_ADC7 ;
                if ((error = sub_vme_A32D32_write(addr,extended_data )) != 0) {
                        sisVME_ErrorHandling (error, addr, "sub_vme_A32D32_write");
                        gl_uint_system_status = SYSTEM_STATUS_MODULES_NOT_READY ;
                }

        //Ch 8
                // Trigger G and P
                p_value           = gl_uint_FirTriggerP_Conf[module_conf_index] [INDEX_ADC_CHANNEL8] ;
                puls_length_value = gl_uint_FirTriggerPulseLength_Conf[module_conf_index] [INDEX_ADC_CHANNEL8] & 0xffff ;
                trigger_delay_value = (gl_uint_FirTriggerPulseLength_Conf[module_conf_index] [INDEX_ADC_CHANNEL8] & 0xff000000) >> 24 ;
                gate_length_value   = (gl_uint_FirTriggerPulseLength_Conf[module_conf_index] [INDEX_ADC_CHANNEL8] & 0x00ff0000) >> 16 ;

// begin mod 9.8.2010
                extended_data = 	 ((trigger_delay_value & 0xff) << 24 ) + ((p_value & 0x0f000000) >> 8) + (g_value & 0xf00) + ((p_value & 0xf00) >> 8) ;
// end mod 9.8.2010
                data = ((gate_length_value & 0xff) << 24) + ((puls_length_value & 0xff) << 16) + ((g_value & 0xff) << 8) + (p_value & 0xff)   ; //
                addr = module_addr + SIS3302_TRIGGER_SETUP_ADC8 ;
                if ((error = sub_vme_A32D32_write(addr,data )) != 0) {
                        sisVME_ErrorHandling (error, addr, "sub_vme_A32D32_write");
                        gl_uint_system_status = SYSTEM_STATUS_MODULES_NOT_READY ;
                }

                addr = module_addr + SIS3302_TRIGGER_EXTENDED_SETUP_ADC8 ;
                if ((error = sub_vme_A32D32_write(addr,extended_data )) != 0) {
                        sisVME_ErrorHandling (error, addr, "sub_vme_A32D32_write");
                        gl_uint_system_status = SYSTEM_STATUS_MODULES_NOT_READY ;
                }
        }	//



        // Trigger Threshold
        // Ch 1
                threshold_value =  0x10000 + (gl_int_FirTriggerThresholdValue_Conf[module_conf_index] [INDEX_ADC_CHANNEL1] & 0xffff);
                data = (unsigned int) threshold_value ;
                threshold_flag  =  gl_uint_FirTriggerThreshold_GtLtFlag_Conf[module_conf_index] [INDEX_ADC_CHANNEL1] ;
                switch (threshold_flag & 0xf)	{  // mod 08.11.2010
                          case  FIR_TRIGGER_THRESHOLD_CONF_GT_OUT_ENABLE: //
                                data = data | 0x2000000 ;   // Bit 25
                                break;
                          case  FIR_TRIGGER_THRESHOLD_CONF_GT_OUT_DISABLE:
                                data = data | 0x6000000 ; // Bit 26 and bit 25
                                break;
                          case  FIR_TRIGGER_THRESHOLD_CONF_GT_EXTEND_OUT_ENABLE: //
                                data = data | 0x2800000 ;   // Bit 25, Bit 23
                                break;
                          case  FIR_TRIGGER_THRESHOLD_CONF_GT_EXTEND_OUT_DISABLE:
                                data = data | 0x6800000 ; // Bit 26 and bit 25   , Bit 23
                                break;
                } // end switch ( )


// begin mod 08.11.2010
                switch ((threshold_flag >> 16) & 0xf)	{
                          case  FIR_TRIGGER_CFD_CONF_75_ENABLE: //
                                data = data | 0x200000 ;   // Bit 21
                                break;
                          case  FIR_TRIGGER_CFD_CONF_50_ENABLE:
                                data = data | 0x300000 ;   // Bit 21 + Bit 20
                                break;
                } // end switch ( )
// end mod 08.11.2010

                addr = module_addr + SIS3302_TRIGGER_THRESHOLD_ADC1 ;
                if ((error = sub_vme_A32D32_write(addr,data )) != 0) {
                        sisVME_ErrorHandling (error, addr, "sub_vme_A32D32_write")   ;
                        gl_uint_system_status = SYSTEM_STATUS_MODULES_NOT_READY ;
                        return -1 ;
                }

                // extended Trigger Threshold value
                data =  0x2000000 + (gl_int_FirTriggerThresholdValue_Conf[module_conf_index] [INDEX_ADC_CHANNEL1]  & 0x1ffffff);
                addr = module_addr + SIS3302_TRIGGER_EXTENDED_THRESHOLD_ADC1 ;
                if ((error = sub_vme_A32D32_write(addr,data )) != 0) {
                        sisVME_ErrorHandling (error, addr, "sub_vme_A32D32_write")   ;
                        gl_uint_system_status = SYSTEM_STATUS_MODULES_NOT_READY ;
                        return -1 ;
                }


        // Trigger Threshold
        // Ch 2
                threshold_value =  0x10000 + (gl_int_FirTriggerThresholdValue_Conf[module_conf_index] [INDEX_ADC_CHANNEL2] & 0xffff);
                data = (unsigned int) threshold_value ;
                threshold_flag  =  gl_uint_FirTriggerThreshold_GtLtFlag_Conf[module_conf_index] [INDEX_ADC_CHANNEL2] ;
                switch (threshold_flag & 0xf)	{  // mod 08.11.2010
                          case  FIR_TRIGGER_THRESHOLD_CONF_GT_OUT_ENABLE: //
                                data = data | 0x2000000 ;   // Bit 25
                                break;
                          case  FIR_TRIGGER_THRESHOLD_CONF_GT_OUT_DISABLE:
                                data = data | 0x6000000 ; // Bit 26 and bit 25
                                break;
                          case  FIR_TRIGGER_THRESHOLD_CONF_GT_EXTEND_OUT_ENABLE: //
                                data = data | 0x2800000 ;   // Bit 25, Bit 23
                                break;
                          case  FIR_TRIGGER_THRESHOLD_CONF_GT_EXTEND_OUT_DISABLE:
                                data = data | 0x6800000 ; // Bit 26 and bit 25   , Bit 23
                                break;
                } // end switch ( )
// begin mod 08.11.2010
                switch ((threshold_flag >> 16) & 0xf)	{
                          case  FIR_TRIGGER_CFD_CONF_75_ENABLE: //
                                data = data | 0x200000 ;   // Bit 21
                                break;
                          case  FIR_TRIGGER_CFD_CONF_50_ENABLE:
                                data = data | 0x300000 ;   // Bit 21 + Bit 20
                                break;
                } // end switch ( )
// end mod 08.11.2010
                addr = module_addr + SIS3302_TRIGGER_THRESHOLD_ADC2 ;
                if ((error = sub_vme_A32D32_write(addr,data )) != 0) {
                        sisVME_ErrorHandling (error, addr, "sub_vme_A32D32_write")   ;
                        gl_uint_system_status = SYSTEM_STATUS_MODULES_NOT_READY ;
                        return -1 ;
                }

                // extended Trigger Threshold value
                data =  0x2000000 + (gl_int_FirTriggerThresholdValue_Conf[module_conf_index] [INDEX_ADC_CHANNEL2]  & 0x1ffffff);
                addr = module_addr + SIS3302_TRIGGER_EXTENDED_THRESHOLD_ADC2 ;
                if ((error = sub_vme_A32D32_write(addr,data )) != 0) {
                        sisVME_ErrorHandling (error, addr, "sub_vme_A32D32_write")   ;
                        gl_uint_system_status = SYSTEM_STATUS_MODULES_NOT_READY ;
                        return -1 ;
                }


        // Trigger Threshold
        // Ch 3
                threshold_value =  0x10000 + (gl_int_FirTriggerThresholdValue_Conf[module_conf_index] [INDEX_ADC_CHANNEL3]  & 0xffff);
                data = (unsigned int) threshold_value ;
                threshold_flag  =  gl_uint_FirTriggerThreshold_GtLtFlag_Conf[module_conf_index] [INDEX_ADC_CHANNEL3] ;
                switch (threshold_flag & 0xf)	{  // mod 08.11.2010
                          case  FIR_TRIGGER_THRESHOLD_CONF_GT_OUT_ENABLE: //
                                data = data | 0x2000000 ;   // Bit 25
                                break;
                          case  FIR_TRIGGER_THRESHOLD_CONF_GT_OUT_DISABLE:
                                data = data | 0x6000000 ; // Bit 26 and bit 25
                                break;
                          case  FIR_TRIGGER_THRESHOLD_CONF_GT_EXTEND_OUT_ENABLE: //
                                data = data | 0x2800000 ;   // Bit 25, Bit 23
                                break;
                          case  FIR_TRIGGER_THRESHOLD_CONF_GT_EXTEND_OUT_DISABLE:
                                data = data | 0x6800000 ; // Bit 26 and bit 25   , Bit 23
                                break;
                } // end switch ( )
// begin mod 08.11.2010
                switch ((threshold_flag >> 16) & 0xf)	{
                          case  FIR_TRIGGER_CFD_CONF_75_ENABLE: //
                                data = data | 0x200000 ;   // Bit 21
                                break;
                          case  FIR_TRIGGER_CFD_CONF_50_ENABLE:
                                data = data | 0x300000 ;   // Bit 21 + Bit 20
                                break;
                } // end switch ( )
// end mod 08.11.2010
                addr = module_addr + SIS3302_TRIGGER_THRESHOLD_ADC3 ;
                if ((error = sub_vme_A32D32_write(addr,data )) != 0) {
                        sisVME_ErrorHandling (error, addr, "sub_vme_A32D32_write")   ;
                        gl_uint_system_status = SYSTEM_STATUS_MODULES_NOT_READY ;
                        return -1 ;
                }
                // extended Trigger Threshold value
                data =  0x2000000 + (gl_int_FirTriggerThresholdValue_Conf[module_conf_index] [INDEX_ADC_CHANNEL3]  & 0x1ffffff);
                addr = module_addr + SIS3302_TRIGGER_EXTENDED_THRESHOLD_ADC3 ;
                if ((error = sub_vme_A32D32_write(addr,data )) != 0) {
                        sisVME_ErrorHandling (error, addr, "sub_vme_A32D32_write")   ;
                        gl_uint_system_status = SYSTEM_STATUS_MODULES_NOT_READY ;
                        return -1 ;
                }


        // Trigger Threshold
        // Ch 4
                threshold_value =  0x10000 + (gl_int_FirTriggerThresholdValue_Conf[module_conf_index] [INDEX_ADC_CHANNEL4]  & 0xffff);
                data = (unsigned int) threshold_value ;
                threshold_flag  =  gl_uint_FirTriggerThreshold_GtLtFlag_Conf[module_conf_index] [INDEX_ADC_CHANNEL4] ;
                switch (threshold_flag & 0xf)	{  // mod 08.11.2010
                          case  FIR_TRIGGER_THRESHOLD_CONF_GT_OUT_ENABLE: //
                                data = data | 0x2000000 ;   // Bit 25
                                break;
                          case  FIR_TRIGGER_THRESHOLD_CONF_GT_OUT_DISABLE:
                                data = data | 0x6000000 ; // Bit 26 and bit 25
                                break;
                          case  FIR_TRIGGER_THRESHOLD_CONF_GT_EXTEND_OUT_ENABLE: //
                                data = data | 0x2800000 ;   // Bit 25, Bit 23
                                break;
                          case  FIR_TRIGGER_THRESHOLD_CONF_GT_EXTEND_OUT_DISABLE:
                                data = data | 0x6800000 ; // Bit 26 and bit 25   , Bit 23
                                break;
                } // end switch ( )
// begin mod 08.11.2010
                switch ((threshold_flag >> 16) & 0xf)	{
                          case  FIR_TRIGGER_CFD_CONF_75_ENABLE: //
                                data = data | 0x200000 ;   // Bit 21
                                break;
                          case  FIR_TRIGGER_CFD_CONF_50_ENABLE:
                                data = data | 0x300000 ;   // Bit 21 + Bit 20
                                break;
                } // end switch ( )
// end mod 08.11.2010
                addr = module_addr + SIS3302_TRIGGER_THRESHOLD_ADC4 ;
                if ((error = sub_vme_A32D32_write(addr,data )) != 0) {
                        sisVME_ErrorHandling (error, addr, "sub_vme_A32D32_write")   ;
                        gl_uint_system_status = SYSTEM_STATUS_MODULES_NOT_READY ;
                        return -1 ;
                }

                // extended Trigger Threshold value
                data =  0x2000000 + (gl_int_FirTriggerThresholdValue_Conf[module_conf_index] [INDEX_ADC_CHANNEL4]  & 0x1ffffff);
                addr = module_addr + SIS3302_TRIGGER_EXTENDED_THRESHOLD_ADC4 ;
                if ((error = sub_vme_A32D32_write(addr,data )) != 0) {
                        sisVME_ErrorHandling (error, addr, "sub_vme_A32D32_write")   ;
                        gl_uint_system_status = SYSTEM_STATUS_MODULES_NOT_READY ;
                        return -1 ;
                }


        if(gl_uint_SIS3302_FourChannelFlag == 0x0) { //

        // Trigger Threshold
        // Ch 5
                threshold_value =  0x10000 + (gl_int_FirTriggerThresholdValue_Conf[module_conf_index] [INDEX_ADC_CHANNEL5]   & 0xffff);
                data = (unsigned int) threshold_value ;
                threshold_flag  =  gl_uint_FirTriggerThreshold_GtLtFlag_Conf[module_conf_index] [INDEX_ADC_CHANNEL5] ;
                switch (threshold_flag & 0xf)	{  // mod 08.11.2010
                          case  FIR_TRIGGER_THRESHOLD_CONF_GT_OUT_ENABLE: //
                                data = data | 0x2000000 ;   // Bit 25
                                break;
                          case  FIR_TRIGGER_THRESHOLD_CONF_GT_OUT_DISABLE:
                                data = data | 0x6000000 ; // Bit 26 and bit 25
                                break;
                          case  FIR_TRIGGER_THRESHOLD_CONF_GT_EXTEND_OUT_ENABLE: //
                                data = data | 0x2800000 ;   // Bit 25, Bit 23
                                break;
                          case  FIR_TRIGGER_THRESHOLD_CONF_GT_EXTEND_OUT_DISABLE:
                                data = data | 0x6800000 ; // Bit 26 and bit 25   , Bit 23
                                break;
                } // end switch ( )
// begin mod 08.11.2010
                switch ((threshold_flag >> 16) & 0xf)	{
                          case  FIR_TRIGGER_CFD_CONF_75_ENABLE: //
                                data = data | 0x200000 ;   // Bit 21
                                break;
                          case  FIR_TRIGGER_CFD_CONF_50_ENABLE:
                                data = data | 0x300000 ;   // Bit 21 + Bit 20
                                break;
                } // end switch ( )
// end mod 08.11.2010
                addr = module_addr + SIS3302_TRIGGER_THRESHOLD_ADC5 ;
                if ((error = sub_vme_A32D32_write(addr,data )) != 0) {
                        sisVME_ErrorHandling (error, addr, "sub_vme_A32D32_write")   ;
                        gl_uint_system_status = SYSTEM_STATUS_MODULES_NOT_READY ;
                        return -1 ;
                }
                // extended Trigger Threshold value
                data =  0x2000000 + (gl_int_FirTriggerThresholdValue_Conf[module_conf_index] [INDEX_ADC_CHANNEL5]  & 0x1ffffff);
                addr = module_addr + SIS3302_TRIGGER_EXTENDED_THRESHOLD_ADC5 ;
                if ((error = sub_vme_A32D32_write(addr,data )) != 0) {
                        sisVME_ErrorHandling (error, addr, "sub_vme_A32D32_write")   ;
                        gl_uint_system_status = SYSTEM_STATUS_MODULES_NOT_READY ;
                        return -1 ;
                }


        // Trigger Threshold
        // Ch 6
                threshold_value =  0x10000 + (gl_int_FirTriggerThresholdValue_Conf[module_conf_index] [INDEX_ADC_CHANNEL6]   & 0xffff);
                data = (unsigned int) threshold_value ;
                threshold_flag  =  gl_uint_FirTriggerThreshold_GtLtFlag_Conf[module_conf_index] [INDEX_ADC_CHANNEL6] ;
                switch (threshold_flag & 0xf)	{  // mod 08.11.2010
                          case  FIR_TRIGGER_THRESHOLD_CONF_GT_OUT_ENABLE: //
                                data = data | 0x2000000 ;   // Bit 25
                                break;
                          case  FIR_TRIGGER_THRESHOLD_CONF_GT_OUT_DISABLE:
                                data = data | 0x6000000 ; // Bit 26 and bit 25
                                break;
                          case  FIR_TRIGGER_THRESHOLD_CONF_GT_EXTEND_OUT_ENABLE: //
                                data = data | 0x2800000 ;   // Bit 25, Bit 23
                                break;
                          case  FIR_TRIGGER_THRESHOLD_CONF_GT_EXTEND_OUT_DISABLE:
                                data = data | 0x6800000 ; // Bit 26 and bit 25   , Bit 23
                                break;
                } // end switch ( )
// begin mod 08.11.2010
                switch ((threshold_flag >> 16) & 0xf)	{
                          case  FIR_TRIGGER_CFD_CONF_75_ENABLE: //
                                data = data | 0x200000 ;   // Bit 21
                                break;
                          case  FIR_TRIGGER_CFD_CONF_50_ENABLE:
                                data = data | 0x300000 ;   // Bit 21 + Bit 20
                                break;
                } // end switch ( )
// end mod 08.11.2010
                addr = module_addr + SIS3302_TRIGGER_THRESHOLD_ADC6 ;
                if ((error = sub_vme_A32D32_write(addr,data )) != 0) {
                        sisVME_ErrorHandling (error, addr, "sub_vme_A32D32_write")   ;
                        gl_uint_system_status = SYSTEM_STATUS_MODULES_NOT_READY ;
                        return -1 ;
                }
                // extended Trigger Threshold value
                data =  0x2000000 + (gl_int_FirTriggerThresholdValue_Conf[module_conf_index] [INDEX_ADC_CHANNEL6]  & 0x1ffffff);
                addr = module_addr + SIS3302_TRIGGER_EXTENDED_THRESHOLD_ADC6 ;
                if ((error = sub_vme_A32D32_write(addr,data )) != 0) {
                        sisVME_ErrorHandling (error, addr, "sub_vme_A32D32_write")   ;
                        gl_uint_system_status = SYSTEM_STATUS_MODULES_NOT_READY ;
                        return -1 ;
                }


        // Trigger Threshold
        // Ch 7
                threshold_value =  0x10000 + (gl_int_FirTriggerThresholdValue_Conf[module_conf_index] [INDEX_ADC_CHANNEL7]   & 0xffff);
                data = (unsigned int) threshold_value ;
                threshold_flag  =  gl_uint_FirTriggerThreshold_GtLtFlag_Conf[module_conf_index] [INDEX_ADC_CHANNEL7] ;
                switch (threshold_flag & 0xf)	{  // mod 08.11.2010
                          case  FIR_TRIGGER_THRESHOLD_CONF_GT_OUT_ENABLE: //
                                data = data | 0x2000000 ;   // Bit 25
                                break;
                          case  FIR_TRIGGER_THRESHOLD_CONF_GT_OUT_DISABLE:
                                data = data | 0x6000000 ; // Bit 26 and bit 25
                                break;
                          case  FIR_TRIGGER_THRESHOLD_CONF_GT_EXTEND_OUT_ENABLE: //
                                data = data | 0x2800000 ;   // Bit 25, Bit 23
                                break;
                          case  FIR_TRIGGER_THRESHOLD_CONF_GT_EXTEND_OUT_DISABLE:
                                data = data | 0x6800000 ; // Bit 26 and bit 25   , Bit 23
                                break;
                } // end switch ( )
// begin mod 08.11.2010
                switch ((threshold_flag >> 16) & 0xf)	{
                          case  FIR_TRIGGER_CFD_CONF_75_ENABLE: //
                                data = data | 0x200000 ;   // Bit 21
                                break;
                          case  FIR_TRIGGER_CFD_CONF_50_ENABLE:
                                data = data | 0x300000 ;   // Bit 21 + Bit 20
                                break;
                } // end switch ( )
// end mod 08.11.2010
                addr = module_addr + SIS3302_TRIGGER_THRESHOLD_ADC7 ;
                if ((error = sub_vme_A32D32_write(addr,data )) != 0) {
                        sisVME_ErrorHandling (error, addr, "sub_vme_A32D32_write")   ;
                        gl_uint_system_status = SYSTEM_STATUS_MODULES_NOT_READY ;
                        return -1 ;
                }

                // extended Trigger Threshold value
                data =  0x2000000 + (gl_int_FirTriggerThresholdValue_Conf[module_conf_index] [INDEX_ADC_CHANNEL7]  & 0x1ffffff);
                addr = module_addr + SIS3302_TRIGGER_EXTENDED_THRESHOLD_ADC7 ;
                if ((error = sub_vme_A32D32_write(addr,data )) != 0) {
                        sisVME_ErrorHandling (error, addr, "sub_vme_A32D32_write")   ;
                        gl_uint_system_status = SYSTEM_STATUS_MODULES_NOT_READY ;
                        return -1 ;
                }

        // Trigger Threshold
        // Ch 8
                threshold_value =  0x10000 + (gl_int_FirTriggerThresholdValue_Conf[module_conf_index] [INDEX_ADC_CHANNEL8]  & 0xffff) ;
                data = (unsigned int) threshold_value ;
                threshold_flag  =  gl_uint_FirTriggerThreshold_GtLtFlag_Conf[module_conf_index] [INDEX_ADC_CHANNEL8] ;
                switch (threshold_flag & 0xf)	{  // mod 08.11.2010
                          case  FIR_TRIGGER_THRESHOLD_CONF_GT_OUT_ENABLE: //
                                data = data | 0x2000000 ;   // Bit 25
                                break;
                          case  FIR_TRIGGER_THRESHOLD_CONF_GT_OUT_DISABLE:
                                data = data | 0x6000000 ; // Bit 26 and bit 25
                                break;
                          case  FIR_TRIGGER_THRESHOLD_CONF_GT_EXTEND_OUT_ENABLE: //
                                data = data | 0x2800000 ;   // Bit 25, Bit 23
                                break;
                          case  FIR_TRIGGER_THRESHOLD_CONF_GT_EXTEND_OUT_DISABLE:
                                data = data | 0x6800000 ; // Bit 26 and bit 25   , Bit 23
                                break;
                } // end switch ( )
// begin mod 08.11.2010
                switch ((threshold_flag >> 16) & 0xf)	{
                          case  FIR_TRIGGER_CFD_CONF_75_ENABLE: //
                                data = data | 0x200000 ;   // Bit 21
                                break;
                          case  FIR_TRIGGER_CFD_CONF_50_ENABLE:
                                data = data | 0x300000 ;   // Bit 21 + Bit 20
                                break;
                } // end switch ( )
// end mod 08.11.2010
                addr = module_addr + SIS3302_TRIGGER_THRESHOLD_ADC8 ;
                if ((error = sub_vme_A32D32_write(addr,data )) != 0) {
                        sisVME_ErrorHandling (error, addr, "sub_vme_A32D32_write")   ;
                        gl_uint_system_status = SYSTEM_STATUS_MODULES_NOT_READY ;
                        return -1 ;
                }
                // extended Trigger Threshold value
                data =  0x2000000 + (gl_int_FirTriggerThresholdValue_Conf[module_conf_index] [INDEX_ADC_CHANNEL8]  & 0x1ffffff) ;
                addr = module_addr + SIS3302_TRIGGER_EXTENDED_THRESHOLD_ADC8 ;
                if ((error = sub_vme_A32D32_write(addr,data )) != 0) {
                        sisVME_ErrorHandling (error, addr, "sub_vme_A32D32_write")   ;
                        gl_uint_system_status = SYSTEM_STATUS_MODULES_NOT_READY ;
                        return -1 ;
                }

        }	//



        return 0x0 ;

} // end function  ()


/************************************************************************************************/




/* --------------------------------------------------------------------------
   SIS3302 Offset
   module_addr			    vme module base address
   offset Value_array		DAC offset value (16 bit)
-------------------------------------------------------------------------- */
int SIS3150CCI2::sis3302_write_dac_offset(unsigned int module_addr, unsigned int *offset_value_array)
{
    unsigned int i, nof_channel, error;

    unsigned int data, addr;
    unsigned int max_timeout, timeout_cnt;

    if(gl_uint_SIS3302_FourChannelFlag == 0x0) { //
        nof_channel = 8 ;
    }
    else {
        nof_channel = 4 ;
    }


    for (i=0;i<nof_channel;i++) {

        data =  offset_value_array[i] ;
        addr = module_addr + SIS3302_DAC_DATA  ;
        if ((error = sub_vme_A32D32_write(addr,data )) != 0) {
            sisVME_ErrorHandling (error, addr, "sub_vme_A32D32_write");
            return -1;
        }

        data =  1 + (i << 4); // write to DAC Register
        addr = module_addr + SIS3302_DAC_CONTROL_STATUS  ;
        if ((error = sub_vme_A32D32_write(addr,data )) != 0) {
            sisVME_ErrorHandling (error, addr, "sub_vme_A32D32_write");
            return -1;
        }

        max_timeout = 5000;
        timeout_cnt = 0 ;
        addr = module_addr + SIS3302_DAC_CONTROL_STATUS  ;
        do {
            if ((error = sub_vme_A32D32_read(addr,&data )) != 0) {
                sisVME_ErrorHandling (error, addr, "sub_vme_A32D32_read");
                return -1;
            }
            timeout_cnt++;
        } while ( ((data & 0x8000) == 0x8000) && (timeout_cnt <  max_timeout) )    ;

        if (timeout_cnt >=  5000) {
            return -2 ;
        }



        data =  2 + (i << 4); // Load DACs
        addr = module_addr + SIS3302_DAC_CONTROL_STATUS  ;
        if ((error = sub_vme_A32D32_write(addr,data )) != 0) {
            sisVME_ErrorHandling (error, addr, "sub_vme_A32D32_write");
            return -1;
        }
        timeout_cnt = 0 ;
        addr = module_addr + SIS3302_DAC_CONTROL_STATUS  ;
        do {
            if ((error = sub_vme_A32D32_read(addr,&data )) != 0) {
                sisVME_ErrorHandling (error, addr, "sub_vme_A32D32_read");
                return -1;
            }
            timeout_cnt++;
        } while ( ((data & 0x8000) == 0x8000) && (timeout_cnt <  max_timeout) )    ;

        if (timeout_cnt >=  5000) {
            return -3 ;
        }


    }




    return 0x0 ;

} // end function  ()


/*********************************************************/
/**************************************************************************************************/
/**************************************************************************************************/


/*---------------------------------------------------------------------------
Read configuration parameter from file
---------------------------------------------------------------------------*/



/********************************************************************************************************/




int SIS3150CCI2::CopyModuleAddressesToTable (void)
{

    int i;
    int no_of_enabled_modules ;
    int error ;
    unsigned int data_rd;

    no_of_enabled_modules = 0 ;
    gl_uint_system_status =  SYSTEM_STATUS_MODULES_READY ;

    // Clock Modules

    // check writetreeif disabled/enabled
    if  (gl_uint_SIS3820EnableConf != 0) {   // enabled
        //cout << gl_uint_SIS3820AddrConf << endl;
        error = sub_vme_A32D32_read(gl_uint_SIS3820AddrConf + SIS3820CLOCK_MODID, &data_rd) ;

        if (error != 0x0) { // vme error
            gl_uint_system_status =  SYSTEM_STATUS_MODULES_NOT_READY ;
        }
        else { // VME OK
            if((data_rd  & 0xfffff000) != 0x3820E000) { // check Mod ID and Major Version
                gl_uint_system_status =  SYSTEM_STATUS_MODULES_NOT_READY ;
            }
        }
    }

    gl_uint_SIS3302_FourChannelFlag = 0 ;

    for (i=0;i<MAX_NO_OF_MODULES;i++) {

        // check if disabled/enabled
        if  (gl_uint_ModEnableConf[i] != 0){   // enabled
            error = sub_vme_A32D32_read(gl_uint_ModAddrConf[i] + SIS3302_MODID , &data_rd) ;

            if (error != 0x0) { // vme error
                gl_uint_system_status =  SYSTEM_STATUS_MODULES_NOT_READY ;
            }
            else  {  // vme OK

                if((data_rd  & 0xffffffff) < 0x33021407) { // check Mod ID and Major Version
                    gl_uint_system_status =  SYSTEM_STATUS_MODULES_NOT_READY ;
                    cout << "module #: " << QString::number(i).toStdString() << " with old firmware has been disabled" << endl;
                    gl_uint_ModEnableConf[i] = 0;
                }else{
                    no_of_enabled_modules++;
                }
                if((data_rd  & 0x00000040) != 0x0) { //
                    gl_uint_SIS3302_FourChannelFlag = 1 ;
                }
            }  // VME OK
        } // disabled/enabled
    } /* for loop */

    /* if no module is disabled then */
    cout << "Number of enabled modules: " << no_of_enabled_modules << endl;
    if (no_of_enabled_modules == 0)   gl_uint_system_status =  SYSTEM_STATUS_MODULES_NOT_READY ;

    return 0;
} // end function  ()


// common sis3150_9300 sample and energy paramters
int SIS3150CCI2::copy_sis3302_sample_energy_CommonParameters_to_inbox (void)
{

    switch (gl_uint_EnergySampleMode) {

    case 0: //  full trapez
        //gl_uint_displayEnergyValue_plot_size  = 512 ;
        break;

    case 1: //  only min and max
        break;


    case 2: //  3 parts of Energy Trapezoidal (3x170=510 values) + min/max
        gl_uint_EnergySampleLength = 170 ;
        gl_uint_EnergySampleStart_Index1 = 1 ;
        gl_uint_EnergySampleStart_Index2 = gl_uint_EnergySampleStart_Index1+171 ;
        gl_uint_EnergySampleStart_Index3 = gl_uint_EnergySampleStart_Index2+171 ;

        break;
    default:

        break;

    } // end switch (gl_uint_EnergySampleMode)

    calculate_energy_trigger_windows_and_parameters() ;

    return 0;
} // end function  ()




int SIS3150CCI2::calculate_energy_trigger_windows_and_parameters (void)
{

    int delay ;
    unsigned int triggerGate_length1 ;
    unsigned int triggerGate_length2 ;
    unsigned int decimation_factor ;

    if (gl_uint_FirDecimationMode == 0) {
        delay = (gl_uint_PreTriggerDelay)/1 ;
        decimation_factor = 1 ;
    }
    if (gl_uint_FirDecimationMode == 1) {
        delay = (gl_uint_PreTriggerDelay)/2 ;
        decimation_factor = 2 ;
    }
    if (gl_uint_FirDecimationMode == 2) {
        delay = (gl_uint_PreTriggerDelay)/4 ;
        decimation_factor = 4 ;
    }
    if (gl_uint_FirDecimationMode == 3) {
        delay = (gl_uint_PreTriggerDelay)/8 ;
        decimation_factor = 8 ;
    }

    // estimate from Energy value
    switch (gl_uint_EnergySampleMode) {
    case 0: //  full trapez
        gl_uint_EnergyGateLength =  delay + 600 ;   // must be greater 512
        break;

    case 1: //  only min and max
        gl_uint_EnergyGateLength =  delay + (2*gl_uint_FirEnergyLength) + (gl_uint_FirEnergyGap) + 20 ;
        break;

    case 2: //  3 parts of Energy Trapezoidal (3x170=510 values) + min/max
        if (((2*gl_uint_FirEnergyLength) + (gl_uint_FirEnergyGap) + 20) > 600) {
            gl_uint_EnergyGateLength =  delay + (2*gl_uint_FirEnergyLength) + (gl_uint_FirEnergyGap) + 120 ;
        }
        else {
            gl_uint_EnergyGateLength =  delay + 700 ;   // must be greater 512
        }
        break;


     case 3: //  programmable number of values
         gl_uint_EnergyGateLength =  delay + 600 ;   // must be greater 512
         break;

     } // end switch (gl_uint_EnergySampleMode)


#ifdef raus
    if (gl_uint_EnergySampleMode  == 1) { // no extra energy values
        gl_uint_EnergyGateLength =  delay + (2*gl_uint_FirEnergyLength) + (gl_uint_FirEnergyGap) + 20 ;
    }
    else {
        if (((2*gl_uint_FirEnergyLength) + (gl_uint_FirEnergyGap) + 20) > 600) {
            gl_uint_EnergyGateLength =  delay + (2*gl_uint_FirEnergyLength) + (gl_uint_FirEnergyGap) + 20 ;
        }
        else {
            gl_uint_EnergyGateLength =  delay + 600 ;   // must be greater 512
        }
    }
#endif

    // estimate from Energy value
    triggerGate_length1 =  gl_uint_PreTriggerDelay + (2*gl_uint_FirEnergyLength) + (gl_uint_FirEnergyGap)  ;

    // estimate from raw sample parameters
    triggerGate_length2 =  gl_uint_RawSampleLength +  gl_uint_RawSampleStart_Index ;

    if (triggerGate_length1 > triggerGate_length2)  {
        gl_uint_TriggerGateLength =  triggerGate_length1 + 16 ;
    }
    else {
        gl_uint_TriggerGateLength =  triggerGate_length2 + 16 ;
    }

    return 0;
}



int SIS3150CCI2::TauFactor_to_DecayExpCalculation(void)
{

    //   int CviErr;
    float float_decay_factor, float_decay_time_us, float_sampling_time_us ;
    int imod, iadc;
    unsigned int sample_clock ;

    Get_ADC_sample_clock_for_DecayExpCalculation(&sample_clock);

    switch (gl_uint_FirDecimationMode)	{
    case  0: // no decimation
        float_sampling_time_us = 1000.0/sample_clock  ; //
        break;
    case  1: //
        float_sampling_time_us = 2000.0/sample_clock  ; //
        break;
    case  2: //
        float_sampling_time_us = 4000.0/sample_clock  ; //
        break;
    case  3: //
        float_sampling_time_us = 8000.0/sample_clock  ; //
        break;
    } // end switch (gl_uint_FirDecimationMode)

    for (imod=0;imod < MAX_NO_OF_MODULES; imod++) {
        for (iadc=0; iadc < NO_OF_ADC_CHANNELS; iadc++) {
            float_decay_factor = (gl_uint_DecayTauConf[imod][iadc] / 32768.0 ) ; // 2 hoch 15
            float_decay_time_us =  -(float_sampling_time_us / (std::log(1.0-float_decay_factor)) ) ; // "log" does natural log ("ln") in math.h
        }
    }
    return 0;
} // end function  ()



int SIS3150CCI2::Get_ADC_sample_clock_for_DecayExpCalculation(unsigned int* sample_clock)
{

    //   int CviErr;
    switch (gl_uint_SIS3302ClockModeConf) {
    case  0: // intern 100 MHz
        *sample_clock = 100000 ;	   //
        break;
    case  1: // intern 50 MHz
        *sample_clock = 50000 ;	   //
        break;
    case  2: // intern 25 MHz
        *sample_clock = 25000 ;	   //
        break;
    case  3: // intern 62.25 MHz
        *sample_clock = 62500 ;	   //
        break;
    case  4: // extern LEMO
    case  5: // extern P2


        switch (gl_uint_SIS3820ClockModeConf)	{
        case  0: // 100 MHz
            *sample_clock = 100000 ;	   //
            break;
        case  1: // 80 MHz
            *sample_clock = 80000 ;	   //
            break;
        case  2: // 50 MHz
            *sample_clock = 50000 ;	   //
            break;
        case  3: // 40 MHz
            *sample_clock = 40000 ;	   //
            break;
        case  4: // 25 MHz
            *sample_clock = 25000 ;	   //
            break;
        case  5: // 20 MHz
            *sample_clock = 20000 ;	   //
            break;
        case  6: // extern LEMO  CTRL 1
            *sample_clock = 100000 ;	   //
            break;
        } // end switch (gl_uint_SIS3820ClockModeConf)

        break;
    } // end switch (gl_uint_SIS9300ClockModeConf)

    return 0;
} // end function  ()





/* --------------------------------------------------------------------------
   SIS3820 Configuration
-------------------------------------------------------------------------- */
int SIS3150CCI2::Configuration_SIS3820Clock (void)
{
    unsigned int error;
    unsigned int data, addr;



    //if(gl_uint_SIS3820EnableConf != 0) { // wird Configuration
    // Key reset
    addr = gl_uint_SIS3820AddrConf + SIS3820CLOCK_KEY_RESET   ;
    if ((error = sub_vme_A32D32_write(addr,0x0 )) != 0) {
        sisVME_ErrorHandling (error, addr, "sub_vme_A32D32_write");
        return -1;
    }


    //set Clock
    switch (gl_uint_SIS3820ClockModeConf)	{
    case  0: // 100 MHz
        data = SIS3820CLOCK_CLOCK_SOURCE_100MHZ + SIS3820CLOCK_CLOCK_DIVIDE_1 ;	   //
        break;
    case  1: // 80 MHz
        data = SIS3820CLOCK_CLOCK_SOURCE_80MHZ  + SIS3820CLOCK_CLOCK_DIVIDE_1;	   //
        break;
    case  2: // 50 MHz
        data = SIS3820CLOCK_CLOCK_SOURCE_100MHZ + SIS3820CLOCK_CLOCK_DIVIDE_2;	   //
        break;
    case  3: // 40 MHz
        data = SIS3820CLOCK_CLOCK_SOURCE_80MHZ + SIS3820CLOCK_CLOCK_DIVIDE_2 ;	   //
        break;
    case  4: // 25 MHz
        data = SIS3820CLOCK_CLOCK_SOURCE_100MHZ + SIS3820CLOCK_CLOCK_DIVIDE_4 ;	   //
        break;
    case  5: // 20 MHz
        data = SIS3820CLOCK_CLOCK_SOURCE_80MHZ + SIS3820CLOCK_CLOCK_DIVIDE_4;	   //
        break;
    case  6: // extern LEMO  CTRL 1
        data = SIS3820CLOCK_CLOCK_SOURCE_EXT_CTRL;	   //
        break;
    } // end switch (uintClockMode)


    addr = gl_uint_SIS3820AddrConf + SIS3820CLOCK_CLOCK_SOURCE   ;
    if ((error = sub_vme_A32D32_write(addr,data )) != 0) {
        sisVME_ErrorHandling (error, addr, "sub_vme_A32D32_write");
        return -1;
    }


    data = gl_uint_SIS3820ClockTriggerMaskConf ;
    addr = gl_uint_SIS3820AddrConf + SIS3820CLOCK_TRIGGERMASK   ;
    if ((error = sub_vme_A32D32_write(addr,data )) != 0) {
        sisVME_ErrorHandling (error, addr, "sub_vme_A32D32_write");
        return -1;
    }




    // General Enable
    data = SIS3820CLOCK_GENERAL_ENABLE ;
    addr = gl_uint_SIS3820AddrConf + SIS3820CLOCK_CONTROL_STATUS   ;
    if ((error = sub_vme_A32D32_write(addr,data )) != 0) {
        sisVME_ErrorHandling (error, addr, "sub_vme_A32D32_write");
        return -1;
    }


    // Enable  P2 and Frontpanel Outputs
    data = 0 ;

    if (gl_uint_SIS3820ClockCtrl4FlagConf == 1) {
        data = data + SIS3820CLOCK_EXT_CLR_TIMESTAMP_ENABLE ;
    }
    else {
        data = data + SIS3820CLOCK_EXT_CLR_TIMESTAMP_DISABLE ;
    }

    if (gl_uint_SIS3820ClockCtrl3FlagConf == 1) {
        data = data + SIS3820CLOCK_EXT_VETO_IN_ENABLE ;
    }
    else {
        data = data + SIS3820CLOCK_EXT_VETO_IN_DISABLE ;
    }

    if (gl_uint_SIS3820ClockCtrl2FlagConf == 1) {
        data = data + SIS3820CLOCK_EXT_TRIGGER_IN_ENABLE ;
    }
    else {
        data = data + SIS3820CLOCK_EXT_TRIGGER_IN_DISABLE ;
    }

    addr = gl_uint_SIS3820AddrConf + SIS3820CLOCK_CONTROL_STATUS   ;
    if ((error = sub_vme_A32D32_write(addr,data )) != 0) {
        sisVME_ErrorHandling (error, addr, "sub_vme_A32D32_write");
        return -1;
    }


    // Enable  P2 and Frontpanel Outputs
    data = SIS3820CLOCK_FP_CLOCK_OUT_ENABLE + SIS3820CLOCK_P2_OUT_ENABLE ;
    addr = gl_uint_SIS3820AddrConf + SIS3820CLOCK_CONTROL_STATUS   ;
    if ((error = sub_vme_A32D32_write(addr,data )) != 0) {
        sisVME_ErrorHandling (error, addr, "sub_vme_A32D32_write");
        return -1;
    }


    return 0x0 ;


} // end function  ()
