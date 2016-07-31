//=================================================================================================
// Name        : obcrypto.cpp
// Author      : Muhammad Obaidullah
// Version     : 0.1
// Copyright   : Electrical & Computer Engineering, Ryerson University ©2016
// Description : OBEncryptor singleton class which manges AES kernel execution on GPU and provides
//               all realted APIs to the developer.
//=================================================================================================

#include "obcrypto.h"

// Global static pointer used to ensure a single instance of the class.
OBCrpto* OBCrpto::m_pInstance = NULL;

OBCrpto::OBCrpto()
{
    // Ititialize everything here
}

OBCrpto* OBCrpto::Instance()
{
    if (m_pInstance == NULL)   // Only allow one instance of class to be generated.
    {
        m_pInstance = new OBCrpto();
    }
    return m_pInstance;
}

// ---------------------------------------------------------------------------------------------------------------------------
//
//	Function:		encryptFile
// 	Purpose:		Do background encryption on the device chosen and inform GUI of the status
// 	Parameters: 	deviceType      - A device type on which to do encryption on
//                  inputFilePath   - A complete path to the file which is to be encrypted
//                  outputFilePath  - A complete path to the file to save/create
//                  options         - Settings to control how the encryption is done
//	Returns:		nothing
//
// ---------------------------------------------------------------------------------------------------------------------------
void OBCrpto::encryptFile(cl_device_type deviceType, string inputFilePath, string outputFilePath, string key, CryptionOptions options)
{
    if(encryptionThread != NULL)
    {
        qDebug() << "Can't encrypt as previous encryption thread hasn't finished running.";
        return;
    }
    switch(deviceType)
    {
    case CL_DEVICE_TYPE_CPU:
        {
            EncryptionThread encryptThread(inputFilePath,outputFilePath, key);
            encryptionThread = &encryptThread;
           break;
        }
    case CL_DEVICE_TYPE_GPU:
        {
           break;
        }
    default:
        {
            break;
        }
    }
}

// ---------------------------------------------------------------------------------------------------------------------------
//
//	Function:		CPU_EncrptionThread
// 	Purpose:		Do background encryption on the CPU and inform parent classof status
// 	Parameters: 	inputFilePath   - A complete path to the file which is to be encrypted
//                  outputFilePath  - A complete path to the file to save/create
//	Returns:		nothing
//
// ---------------------------------------------------------------------------------------------------------------------------
void OBCrpto::CPU_EncrptionThread(int inputPath)
{

}

// ---------------------------------------------------------------------------------------------------------------------------
//
//	Function:		decryptFile
// 	Purpose:		Do background encryption on the device chosen and inform GUI of the status
// 	Parameters: 	deviceType      - A device type on which to do decrytion on
//                  inputFilePath   - A complete path to the file which is to be decrypted
//                  outputFilePath  - A complete path to the file to save/create
//                  options         - Settings to control how the decryption is done
//	Returns:		nothing
//
// ---------------------------------------------------------------------------------------------------------------------------
void OBCrpto::decryptFile(cl_device_type deviceType, string inputFilePath, string outputFilePath, string key, CryptionOptions options)
{
}
