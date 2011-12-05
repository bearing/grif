#include <SIS3150CCI2.h>

/*
  Author: Cameron Bates cameron.r.bates@gmail.com
  Source code based on Struck Innovative Systeme vme routines from gamma_system code
  This is a companion file to SIS3150CCI2. It contains all of the 3150 vme calls that are used.

  */
/************************************************************************************************/
/************************************************************************************************/
// ERROR HANDLING AND SYSTEM MESSAGES

int SIS3150CCI2::sisVME_ErrorHandling (unsigned int prot_error, unsigned int vme_addr, const char *err_messages)
{
    std::cout << "ERROR: prot_error=" << prot_error << ", vme_addr=0x" << hex << vme_addr << dec << endl;
    std::cout << "\t\tMessage: " << err_messages << endl;

    return 0;
} // end function sisVME_ErrorHandling ()


int SIS3150CCI2::write_system_messages (char *system_messages, int write_datetime)
{
    std::cout << system_messages << endl;
    return 0;
} // end function sisVME_ErrorHandling ()


/* --------------------------------------------------------------------------
   VME Error Handling
-------------------------------------------------------------------------- */


int SIS3150CCI2::sis_AnyErrorHandling (char *err_messages)
{
    std::cout << err_messages << endl;
    return 0;
} // end function sisVME_ErrorHandling ()


/************************************************************************************************/
/************************************************************************************************/
// LOADING LIBRARY FUNCTIONS (sis3150usb_vme_win.dll)

// Loads library functions into memory
void SIS3150CCI2::InitLibraries(void){


#if (OPERATING_SYSTEM == WINDOWS)
    // gets sis3150usb_vme_win.dll from target directory
    /* NOTE: This assumes that when the project is compiled the target directory is "debug" and that the "lib" directory
       containing "sis3150usb_vme_win.dll" is a sibling of "debug", so that we only have to go up a directory and then go
       into the lib directory. In the future, it will probably be better to have a set path upon installation that can be
       used to specify where the "lib" folder is with the appropriate files.
     */
    QDir dllDir(QDir::current());
    dllDir.cdUp();
    dllDir.cd(QString("lib"));
    QString libraryPath = QString("C:/WINDOWS/system32/sis3150usb_vme_win.dll"); //(dllDir.absolutePath()).append("/sis3150usb_vme_win.dll");

#elif (OPERATING_SYSTEM == LINUX)
    // gets libsis3150.so from standard installation directory
    /*
      We assume that when installing the drivers for the SIS3150CCI2, the user puts the library file in the directory
      specified below. Keep in mind: because it is in "/usr/opt/...", you must have permissions in order to access this
      directory (I think).
      */
    QString libraryPath = QString(SISUSB_PATH) + QString("sisusb/lib/libsis3150.so");
    std::cout << "SIS3150 Library: " << libraryPath.toStdString().c_str() << endl;

#elif (OPERATING_SYSTEM == DARWIN)
#error Sorry, mac is not supported yet.
#else

#error You do not have a supported OS.

#endif

    QLibrary sis3150lib(libraryPath);

    sis3150usb_Register_Single_Read
            = (sis3150Usb_Register_Single_Read_PROT) sis3150lib.resolve("sis3150Usb_Register_Single_Read");

    FindAll_sis3150usb_Devices
            = (FindAll_SIS3150USB_Devices_PROT) sis3150lib.resolve("FindAll_SIS3150USB_Devices");

    sis3150usb_OpenDriver_And_Download_FX2_Setup
            = (Sis3150usb_OpenDriver_And_Download_FX2_Setup_PROT) sis3150lib.resolve("Sis3150usb_OpenDriver_And_Download_FX2_Setup");

    sis3150usb_Register_Single_Write
            = (sis3150Usb_Register_Single_Write_PROT) sis3150lib.resolve("sis3150Usb_Register_Single_Write");

    // sub_vme_A32MBLT64_read
    sis3150usb_Vme_Dma_Read
            = (sis3150Usb_Vme_Dma_Read_PROT) sis3150lib.resolve("sis3150Usb_Vme_Dma_Read");

    sis3150usb_Vme_Single_Read
            = (sis3150Usb_Vme_Single_Read_PROT) sis3150lib.resolve("sis3150Usb_Vme_Single_Read");

    sis3150usb_Vme_Single_Write
            = (sis3150Usb_Vme_Single_Write_PROT) sis3150lib.resolve("sis3150Usb_Vme_Single_Write");

//#ifdef DEBUG_LIB

            ///////////////////////////////////////////
            ///////////Loading Libraries///////////////
            ///////////////////////////////////////////

    std::cout << "Loading " << libraryPath.toStdString().c_str() << ":\n" << (sis3150lib.load() ? "\tsuccess" : "\tfailure") << "\n\n";


} // end InitLibraries()

/********************************************************************************************************/
/***********PC_TO_VME_ROUTINES***************************************************************************/


int SIS3150CCI2::sub_vme_A32MBLT64_read (unsigned int vme_adr, ULONG* dma_buffer, unsigned int request_nof_words, ULONG* got_nof_words)
{
    return sis3150usb_Vme_Dma_Read(gl_USBDevice, vme_adr, 0x8, 8, 0, dma_buffer,
                                    request_nof_words & 0xfffffffe, got_nof_words);
} // end function  ()


int SIS3150CCI2::sub_vme_A32D32_read (unsigned int vme_adr, unsigned int* vme_data)
{
    return vme_A32D32_read(gl_USBDevice, vme_adr, (u_int32_t*)vme_data)  ;
} // end function  ()


int SIS3150CCI2::sub_vme_A32D32_write (unsigned int vme_adr, unsigned int vme_data)
{
    return vme_A32D32_write(gl_USBDevice, vme_adr, vme_data)  ;
} // end function  ()


int SIS3150CCI2::vme_A32D32_read(HANDLE  hXDev, u_int32_t vme_adr, u_int32_t* vme_data )
{
  ULONG readdata ;      // J_type_change
  int return_code ;
  return_code = sis3150usb_Vme_Single_Read(hXDev, vme_adr, 0x9,4, &readdata)  ;
  if (return_code < 0)  return return_code ;
  *vme_data = readdata;
  return return_code ;
}


int SIS3150CCI2::vme_A32D32_write(HANDLE  hXDev, u_int32_t vme_adr, u_int32_t vme_data )
{
  return sis3150usb_Vme_Single_Write(hXDev, vme_adr, 0x9, 4,  vme_data) ;
}

/*************PC TO VME ROUTINES END*********************************************************************/
/********************************************************************************************************/
