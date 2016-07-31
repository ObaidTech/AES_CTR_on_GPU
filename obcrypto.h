//=================================================================================================
// Name        : obcrypto.h
// Author      : Muhammad Obaidullah
// Version     : 0.1
// Copyright   : Electrical & Computer Engineering, Ryerson University ©2016
// Description : OBEncryptor singleton class which manges AES kernel execution on GPU and provides
//               all realted APIs to the developer.
//=================================================================================================

#ifndef OBCRYPTO_H
#define OBCRYPTO_H
#include <vector>
#include <iostream>
#include <sstream>
#include <QObject>
#include <thread>
#include <QDebug>
#include <CL/cl.h>
#include <encryptionthread.h>

using namespace std;

enum CryptionOptions
{
    CPU_MutliThreaded,
    CPU_SingleThreaded,
    GPU_ExpandKeyOnGPU,
    GPU_ExpandKeyonCPU
};

class OBCrpto : public QObject
{
    Q_OBJECT

public:
    // BEGIN - Functions ------------ //
    static OBCrpto* Instance();
    void encryptFile(cl_device_type deviceType, string inputFilePath, string outputFilePath, string key, CryptionOptions options);
    void decryptFile(cl_device_type deviceType, string inputFilePath, string outputFilePath, string key, CryptionOptions options);
    // END - Functions ------------ //

    // BEGIN - Properties ------------ //
    // END - Properties ------------ //

private:
    // BEGIN - Functions ------------ //
    OBCrpto();
    void CPU_EncrptionThread(int inputPath);
    // END - Functions ------------ //

    // BEGIN - Properties ------------ //
    static OBCrpto* m_pInstance;
    EncryptionThread* encryptionThread;
    // END - Properties ------------ //

signals:
    void signalEncryptionStarted();
    void signalEncyptionProgress(int progress);
    void signalEncryptionFinished();
    void signalDecryptionStarted();
    void signalDecryptionProgress(int progress);
    void signalDecryptionFinished();
    void signalLog(string str);
};

#endif // OBCRYPTO_H
