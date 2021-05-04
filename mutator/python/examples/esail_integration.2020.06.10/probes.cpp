//#include "FAQAS_dataDrivenMutator.h"


/* Copyright (C) OHB-System AG
 *
 * All Rights Reserved. No part of this software may
 * be published, distributed, translated or otherwise
 * reproduced by any means or for any purpose without
 * the prior written consent of OHB-System AG.
 * Proprietary and confidential.
 */
// -----------------------------------------------------------------------------
// --------------------------------- Includes ----------------------------------
// -----------------------------------------------------------------------------

#include "AdcsIf.h"
#include "Generic/PoweredUnit.h"
#include "Mdk/Aggregate.h"
#include "Mdk/Composite.h"
#include "Mdk/EntryPoint.h"
#include "Mdk/Management/EntryPointPublisher.h"
#include "Mdk/Management/ManagedContainer.h"
#include "Mdk/Management/ManagedReference.h"

#include "Adcs/AdcsIfGyroModule.h"
#include "Adcs/AdcsIfHk.h"
#include "Adcs/AdcsIfMgtmModule.h"
#include "Adcs/AdcsIfMgtqModule.h"
#include "Adcs/AdcsIfRwModule.h"
#include "Adcs/AdcsIfScHk.h"
#include "Adcs/AdcsIfSsModule.h"
#include "Adcs/AdcsIfStatus.h"
#include "Adcs/AdcsNamespace.h"
#include "Adcs/IObcAiDll.h"
#include "Generic/AsynchRequestManager.h"
#include "Generic/ByVeAsynchRequest.h"
#include "Generic/CommandsBuffer.h"
#include "Generic/IByteNode.h"
#include "Mdk/SimpleTypes.h"
#include "Smp/Publication/IClassType.h"
#include "Smp/Publication/IEnumerationType.h"
#include "Smp/Publication/IPublishOperation.h"
#include "Smp/Publication/IStructureType.h"
#include "Smp/Publication/IType.h"
#include "Smp/SimpleTypes.h"
#include "SystemIF/IPulseLine.h"
#include "SystemIF/IUARTNode.h"
#include "vector"

// --OPENING ELEMENT--::Adcs::AdcsIf/AdcsIf::Extra_Includes--
// MARKER: FILE INCLUSION: START
#include "Generic/ByVeAsynchRequest.h"
// MARKER: FILE INCLUSION: END

// MARKER: FORWARD DECLARATIONS: START
// MARKER: FORWARD DECLARATIONS: END

//int mutate( std::vector<Smp::UInt8> *v, FaultModel *fm ){
//    return _FAQAS_mutate(v->data(),fm);
//}

// -----------------------------------------------------------------------------
// ------------------------ Custom field definitions ---------------------------
// -----------------------------------------------------------------------------
// MARKER: EXTRA FIELD DEFINITIONS: START
// MARKER: EXTRA FIELD DEFINITIONS: END
// --CLOSING ELEMENT--::Adcs::AdcsIf/AdcsIf::Extra_Includes--

namespace Adcs
{
// --OPENING ELEMENT--AdcsIf::ObcRecvByteCb--
/// Operation called when a byte is received from the OBC.
/// @param recvByte The received byte.
void AdcsIf::ObcRecvByteCb(::Smp::UInt8 recvByte)
{
    // MARKER: OPERATION BODY: START
    Trace(3, "Uart Received: 0x%02X", recvByte);
    if(ObcLinkBbc)
        ObcLinkBbc->SendByte(recvByte);
    // MARKER: OPERATION BODY: END
}
// --CLOSING ELEMENT--AdcsIf::ObcRecvByteCb--

// --OPENING ELEMENT--AdcsIf::ObcSendCb--
/// Operation called when a byte is ready to be sent to the OBC.
/// @param sendByte The byte to be sent.
void AdcsIf::ObcSendCb(::Smp::UInt8 sendByte)
{
    // MARKER: OPERATION BODY: START
    sendBuffer.push_back(sendByte);
    sendMessageIfReady();
    // MARKER: OPERATION BODY: END
}
// --CLOSING ELEMENT--AdcsIf::ObcSendCb--


int mutate( std::vector<unsigned char> *v, FaultModel *fm ){
    return _FAQAS_mutate(v->data(),fm);
}

// --OPENING ELEMENT--AdcsIf::ObcRecvBlockCb--
/// Function that is called when a block of data is received from the data link layer.
/// @param block The received data block.
void AdcsIf::ObcRecvBlockCb(const std::vector<Smp::UInt8>& block)
{
    // MARKER: OPERATION BODY: START
    Trace(4, "Received command: 0x%s", OhbCommon::ByteUtils::BinToHex(block));

    if(!CheckRxEnabled())
    {
        return;
    }

    std::vector<Smp::UInt8> newBlock(block);

    Smp::UInt8 cmdId = block[0];
    Smp::UInt8 subcmdId = block[1];

    if(forcedResponse && (forcedResponseCmdId == cmdId || forcedResponseCmdId < 0)
       && (forcedResponseSubcmdId == subcmdId || forcedResponseSubcmdId < 0))
    {
        newBlock.resize(2);

        if(forcedResponseErrorCode >= 0)
        {
            // Generate forced error response
            Trace(2, "Generating forced response message with error code 0x%02X", forcedResponseErrorCode);
            newBlock.push_back(forcedResponseErrorCode);
            SendResponse(newBlock, true);
        }
        else
        {
            // Generate forced valid response
            Trace(2, "Generating forced response message with data 0x%s", forcedResponseData.c_str());
            for(unsigned int i = 0; i < forcedResponseData.length(); i += 2)
            {
                std::string byteString = forcedResponseData.substr(i, 2);
                newBlock.push_back(strtol(byteString.c_str(), NULL, 16));
            }
            SendResponse(newBlock, false);
        }
        return;
    }

    bool processed = true;
    CommandResult cr = CR_Failure;

    if(Status->ADRD || ((cmdId == 1) && (subcmdId < 3)))
    {
        switch(cmdId)
        {
        case 1:
        {
            switch(subcmdId)
            {
            case 0:
            {
                cr = GetIfStatus(newBlock);

                //MANUALLY INSERTED PROBES
                if(cr == CR_Failure){
                  probe IfStatusFailure
                  FaultModel *fm = _FAQAS_IfStatusFailure_FM();
      		        mutate( &newBlock, fm );
  		            _FAQAS_delete_FM(fm);
                  //end probe
                }
                else{
                  //probe IfStatus
                  FaultModel *fm = _FAQAS_IfStatus_FM();
      		        mutate( &newBlock, fm );
  		            _FAQAS_delete_FM(fm);
                  //end probe
                }
                //END PROBES

            }
            break;
            case 1:
            {
                cr = GetIfHk(newBlock);

                //MANUALLY INSERTED PROBES
                if(cr == CR_Failure){
                  //probe IfHKFailure
                  FaultModel *fm = _FAQAS_IfHKFailure_FM();
      		        mutate( &newBlock, fm );
  		            //_FAQAS_delete_FM(fm);
                  //end probe
                }
                else{
                  //probe IfHK
                  FaultModel *fm = _FAQAS_IfHK_FM();
      		        mutate( &newBlock, fm );
  		            //_FAQAS_delete_FM(fm);
                  //end probe
                }
                //END PROBES


            }
            break;
            case 2:
            {
                cr = SetIfPower(newBlock);
            }
            break;
            case 3:
            {
                cr = SetUnitStatus(newBlock);
            }
            break;
            case 4:
            {
                cr = SetConfiguration(newBlock);
            }
            break;
            case 5:
            {
                cr = LclRetrigger(newBlock);
            }
            break;
            default:
                Log(Smp::Services::LMK_Warning, "Sub-command %u not implemented", subcmdId);
                processed = false;
            }
        }
        break;
        case 4:
        {
            switch(subcmdId)
            {
            case 0:
            {
                cr = GetGyroTm(newBlock);

                //MANUALLY INSERTED PROBES
                if(cr == CR_Failure){

                  //probe GYTMFailure
                  FaultModel *fm = _FAQAS_GYTMFailure_FM();
      		        mutate( &newBlock, fm );
  		            //_FAQAS_delete_FM(fm);
                  //end probe

                }
                else{

                  //probe GYTM
                  FaultModel *fm = _FAQAS_GYTM_FM();
      		        mutate( &newBlock, fm );
  		            //_FAQAS_delete_FM(fm);
                  //end probe
                }
                //END PROBES
            }
            break;
            default:
                Log(Smp::Services::LMK_Warning, "Sub-command %u not implemented", subcmdId);
                processed = false;
            }
        }
        break;
        case 5:
        {
            switch(subcmdId)
            {
            case 0:
            {
                cr = GetMgtmTm(newBlock);

                //MANUALLY INSERTED PROBES
                if(cr == CR_Failure){
                  //probe MMTXfailure (wrong name on the document?)
                  FaultModel *fm = _FAQAS_MMTXFailure_FM();
                  mutate( &newBlock, fm );
                  //_FAQAS_delete_FM(fm);
                  //end probe
                }
                else{
                  //probe MMTX (wrong name on the document?)
                  FaultModel *fm = _FAQAS_MMTX_FM();
                  mutate( &newBlock, fm );
                  //_FAQAS_delete_FM(fm);
                  //end probe
                }
                //END PROBES


            }
            break;
            default:
                Log(Smp::Services::LMK_Warning, "Sub-command %u not implemented", subcmdId);
                processed = false;
            }
        }
        break;
        case 6:
        {
            switch(subcmdId)
            {
            case 0:
            {
                cr = GetSsTm(newBlock);
                //MANUALLY INSERTED PROBES
                if(cr == CR_Failure){
                  //probe SunSensorTMfailure
                  FaultModel *fm = _FAQAS_SunSensorTMFailure_FM();
      		        mutate( &newBlock, fm );
  		            //_FAQAS_delete_FM(fm);
                  //end probe
                }
                else{
                  //probe SunSensorTM
                  FaultModel *fm = _FAQAS_SunSensorTM_FM();
      		        mutate( &newBlock, fm );
  		            //_FAQAS_delete_FM(fm);
                  //end probe
                }
                //END PROBES

            }
            break;
            case 1:
            {
                cr = GetSsTemp(newBlock);
                //MANUALLY INSERTED PROBES
                if(cr == CR_Failure){
                  //probe SSTPFailure
                  FaultModel *fm = _FAQAS_SSTPFailure_FM();
                  mutate( &newBlock, fm );
                  //_FAQAS_delete_FM(fm);
                  //end probe
                }
                else{
                  //probe SSTP
                  FaultModel *fm = _FAQAS_SSTP_FM();
                  mutate( &newBlock, fm );
                  //_FAQAS_delete_FM(fm);
                  //end probe
                }
                //END PROBES


            }
            break;
            default:
                Log(Smp::Services::LMK_Warning, "Sub-command %u not implemented", subcmdId);
                processed = false;
            }
        }
        break;
        case 7:
        {
            switch(subcmdId)
            {
            case 0:
            {
                cr = GetRwTm(newBlock);
            }
            break;
            default:
                Log(Smp::Services::LMK_Warning, "Sub-command %u not implemented", subcmdId);
                processed = false;
            }
        }
        break;
        case 8:
        {
            switch(subcmdId)
            {
            case 0:
            {
                cr = SetMgtqPwm(newBlock);


//MANUALLY INSERTED PROBES
                if(cr == CR_Failure){
                  //probe MagnetorquerSetPWMRSPFailure
                  FaultModel *fm = _FAQAS_MagnetorquerSetPWMRSP_FM();
                  mutate( &newBlock, fm );
                  //_FAQAS_delete_FM(fm);
                  //end probe
                }
                else{
                  //probe MagnetorquerSetPWMRSP
                  FaultModel *fm = _FAQAS_MagnetorquerSetPWMRSPFailure_FM();
                  mutate( &newBlock, fm );
                  //_FAQAS_delete_FM(fm);
                  //end probe
                }
//END PROBES

                if(cr == CR_Success)
                {
                    if(newBlock[2] == 0x55)
                    {
                        // Bypass Magnetometer response
                        newBlock.resize(2);
                        cr = GetMgtqTm(newBlock);
                    }
                    else
                    {
                        cr = BuildMgtmDataRequestCmd(newBlock);
                        cr = GetMgtmTm(newBlock);
                    }
                }
            }
            break;
            default:
                Log(Smp::Services::LMK_Warning, "Sub-command %u not implemented", subcmdId);
                processed = false;
            }
        }
        break;
        case 9:
        {
            switch(subcmdId)
            {
            case 0:
            {
                cr = GetIfScHk(newBlock);

                //  MANUALLY INSERTED PROBE
                if(cr == CR_Failure){
                  //probe SpaceCraftHK
                  FaultModel *fm = _FAQAS_SpaceCraftHKFailure_FM();
                  mutate( &newBlock, fm );
                  //_FAQAS_delete_FM(fm);
                  //end probe
                }
                else{
                  //probe SpaceCraftHK
                  FaultModel *fm = _FAQAS_SpaceCraftHK_FM();
                  mutate( &newBlock, fm );
                  //_FAQAS_delete_FM(fm);
                  //end probe
                }
                //END PROBE

            }
            break;
            default:
                Log(Smp::Services::LMK_Warning, "Sub-command %u not implemented", subcmdId);
                processed = false;
            }
        }
        break;
        default:
            Log(Smp::Services::LMK_Warning, "Command %u not implemented", cmdId);
            processed = false;
        }
    }
    switch(cr)
    {
    case CR_Success:
    {
        SendResponse(newBlock, false);
    }
    break;
    case CR_InProgress:
    {
        Trace(5, "Operation in progress");
    }
    break;
    case CR_Failure:
    {
        if(!processed)
        {
            newBlock.push_back(0x56);
        }
        SendResponse(newBlock, true);
    }
    break;
    default:
        Log(Smp::Services::LMK_Error, "Command result %u not supported", cr);
    }
    // MARKER: OPERATION BODY: END
}
// --CLOSING ELEMENT--AdcsIf::ObcRecvBlockCb--

// --OPENING ELEMENT--AdcsIf::LclRetriggerCb--
/// Operation called when a pulse is received on the LCL retrigger input.
/// @param duration The pulse duration.
void AdcsIf::LclRetriggerCb(::Smp::Float32 duration)
{
    // MARKER: OPERATION BODY: START
    Trace(5, "Received LCL retrigger pulse with duration %f", duration);

    lclTripOff = false;
    // MARKER: OPERATION BODY: END
}
// --CLOSING ELEMENT--AdcsIf::LclRetriggerCb--

// --OPENING ELEMENT--AdcsIf::AfterPowerOff--
void AdcsIf::AfterPowerOff()
{
    // MARKER: OPERATION BODY: START
    Status->ADRD = false;
    Status->OCER = false;
    Status->UCER = false;
    Status->set_Uner(0);
    ++Status->ORCT;
    Status->set_Agye(GMS_Disabled);
    Status->set_Arwe(RWS_DisabledTransciever);
    Status->set_Amte(::Generic::EDS_Disabled);
    Status->set_Amme(GMS_Disabled);
    Status->set_Asse(0);

    set_AdcsPowerStatus(POWER_OFF);
    // MARKER: OPERATION BODY: END
}
// --CLOSING ELEMENT--AdcsIf::AfterPowerOff--

// --OPENING ELEMENT--AdcsIf::AfterPowerOn--
void AdcsIf::AfterPowerOn()
{
    // MARKER: OPERATION BODY: START
    Status->ADRD = true;
    // MARKER: OPERATION BODY: END
}
// --CLOSING ELEMENT--AdcsIf::AfterPowerOn--

// --OPENING ELEMENT--AdcsIf::SetForcedResponseData--
/// Set the forced response data that will be generated when forcedResponse is true.
/// @param data The data in hex to be sent.
void AdcsIf::SetForcedResponseData(::Smp::String8 data)
{
    // MARKER: OPERATION BODY: START
    forcedResponseData = data;
    // MARKER: OPERATION BODY: END
}
// --CLOSING ELEMENT--AdcsIf::SetForcedResponseData--

// --OPENING ELEMENT--AdcsIf::getForcedResponseData--
/// Get the forced response data that will be generated when forcedResponse is true.
::Smp::String8 AdcsIf::getForcedResponseData()
{
    // MARKER: OPERATION BODY: START
    return forcedResponseData.c_str();
    // MARKER: OPERATION BODY: END
}
// --CLOSING ELEMENT--AdcsIf::getForcedResponseData--

// --OPENING ELEMENT--AdcsIf::WatchdogTimeout--
/// Simulate watchdog timeout.
void AdcsIf::WatchdogTimeout()
{
    // MARKER: OPERATION BODY: START
    ++Status->WRCT;
    Reset();
    // MARKER: OPERATION BODY: END
}
// --CLOSING ELEMENT--AdcsIf::WatchdogTimeout--

// --OPENING ELEMENT--AdcsIf::get_AdcsPowerStatus--
/// Get AdcsPowerStatus.
/// The ADCS units power status.
::Adcs::ASPW AdcsIf::get_AdcsPowerStatus()
{
    // MARKER: OPERATION BODY: START
    return this->adcsPowerStatus;
    // MARKER: OPERATION BODY: END
}
// --CLOSING ELEMENT--AdcsIf::get_AdcsPowerStatus--

// --OPENING ELEMENT--AdcsIf::set_AdcsPowerStatus--
/// Set AdcsPowerStatus.
/// The ADCS units power status.
void AdcsIf::set_AdcsPowerStatus(::Adcs::ASPW value)
{
    // MARKER: OPERATION BODY: START
    this->adcsPowerStatus = value;

    using namespace ::Generic;

    struct AdcsAvailablility
    {
        EnabledDisabled gyroscope;
        EnabledDisabled magnetometer;
        EnabledDisabled sunSensor;
        EnabledDisabled reactionWheel;
        EnabledDisabled magnetorquer;
    };

    AdcsAvailablility test = {EDS_Enabled, EDS_Enabled, EDS_Enabled, EDS_Enabled, EDS_Enabled};

    int powerUnit = 0x01 & value;
    Trace(5, "Gyro status : 0x%02X", powerUnit);
    test.gyroscope = (EnabledDisabled)powerUnit;

    powerUnit = 0x01 & value >> 1;
    Trace(5, "MM status : 0x%02X", powerUnit);
    test.magnetometer = (EnabledDisabled)powerUnit;

    powerUnit = 0x01 & value >> 2;
    Trace(5, "RW status : 0x%02X", powerUnit);
    test.reactionWheel = (EnabledDisabled)powerUnit;

    powerUnit = 0x01 & value >> 3;
    Trace(5, "SunSensor status : 0x%02X", powerUnit);
    test.sunSensor = (EnabledDisabled)powerUnit;

    Gyroscope->set_Status(test.gyroscope);
    Magnetometer->set_Status(test.magnetometer);
    SunSensor->set_Status(test.sunSensor);
    ReactionWheel->set_Status(test.reactionWheel);
    // MARKER: OPERATION BODY: END
}
// --CLOSING ELEMENT--AdcsIf::set_AdcsPowerStatus--

// --OPENING ELEMENT--AdcsIf::_delayTransmission--
// Handler for Entry Point: delayTransmission
void AdcsIf::_delayTransmission()
{
    // MARKER: OPERATION BODY: START
    Trace(3, "Uart : 0x%02X", sendBuffer.front());
    ObcLink->SendByte(sendBuffer.front());
    sendBuffer.pop_front();
    pendingBytesToSend--;
    sendMessageIfReady();
    // MARKER: OPERATION BODY: END
}
// --CLOSING ELEMENT--AdcsIf::_delayTransmission--

// - - - - - - - - - - - - - - Publish - - - - - - - - - - - - - - - -
// The Publish method is implemented in the AdcsIfSmp.cpp file.

// --OPENING ELEMENT--AdcsIf::Configure--
/// - - - - - - - - - - - - - - Configure - - - - - - - - - - - - - - -
///
/// Request for configuration.  Request the model to perform any custom
/// configuration.  The  model can  create and configure  other  models
/// using the field values of its published fields.
///
/// @par     This  method  can only be called  once for each model, and
///          when the model is in <em>Publishing</em> state.  The model
///          can still publish further  features in this call,  and can
///          even create other models, but at the end of this call,  it
///          needs to enter the<em>Configured</em> state.
///
/// @param   logger Logger.
///
/// @remarks The simulation  environment typically calls this method in
///          the <em>Building</em> state.
/// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

void AdcsIf::Configure(::Smp::Services::ILogger* logger)
{
    // Call base implementation first
    ::Generic::PoweredUnit::Configure(logger);

    _FAQAS_IfStatusFailure_FM();
    _FAQAS_IfStatus_FM();
    _FAQAS_IfHKFailure_FM();
    _FAQAS_IfHK_FM();
    _FAQAS_GYTMFailure_FM();
    _FAQAS_GYTM_FM();
    _FAQAS_MMTXFailure_FM();
    _FAQAS_MMTX_FM();
    _FAQAS_SunSensorTMFailure_FM();
    _FAQAS_SunSensorTM_FM();
    _FAQAS_SSTP_FM();
    _FAQAS_SSTPFailure_FM();
    _FAQAS_ReactionWheelTXFailure_FM();
    _FAQAS_ReactionWheelTX_FM();
    _FAQAS_SpaceCraftHKFailure_FM();
    _FAQAS_SpaceCraftHK_FM();
    _FAQAS_MagnetorquerSetPWMRSPFailure_FM();
    _FAQAS_MagnetorquerSetPWMRSP_FM();

    // MARKER: CONFIGURE BODY: START
    // MARKER: CONFIGURE BODY: END
}
// --CLOSING ELEMENT--AdcsIf::Configure--

// --OPENING ELEMENT--AdcsIf::Connect--
/// - - - - - - - - - - - - - - - Connect - - - - - - - - - - - - - - -
///
/// Connect model to simulator. Allow the model to connect to the simu-
/// lator (ISimulator).
///
/// @par     This method  can only be called once  for each model,  and
///          only  when the model  is in the <em>Configured</em> state.
///          When  this  operation is  called,  the  model  immediately
///          enters  the <em>Connected</em> state,  before it  uses any
///          of the simulator methods and services.
///
/// @par     In this method, the model may query for and use any of the
///          available simulation services,  as they are all guaranteed
///          to be  fully  functional  at that  time.  It  may  as well
///          connect  to  other  models' functionality  (e.g.  to event
///          sources),  as it is  guaranteed that  all models have been
///          created and configured  before the Connect() method of any
///          model is called.
///
/// @param   simulator  Simulation  Environment  that  hosts the model.
///
/// @remarks The simulation  environment typically calls this method in
///          the <em>Connecting</em> state.
/// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

void AdcsIf::Connect(::Smp::ISimulator* simulator)
{
    // Call Mdk implementation first
    ::Generic::PoweredUnit::Connect(simulator);
    SetupContainersAndReferences(simulator);

    // MARKER: CONNECT BODY: START
    //    CheckConnections();

    using namespace std::placeholders;
    if(PowerInput)
        PowerInput->SetVoltageCallback(std::bind(&AdcsIf::SetVoltageCb, this, _1));

    if(ObcLink)
        ObcLink->SetCallback(std::bind(&AdcsIf::ObcRecvByteCb, this, _1));

    if(ObcLinkBbc)
        ObcLinkBbc->SetRecvByteCb(std::bind(&AdcsIf::ObcSendCb, this, _1));

    if(ObcLinkDll)
        ObcLinkDll->SetUserRecvBlockCb(std::bind(&AdcsIf::ObcRecvBlockCb, this, _1));

    if(LclRetriggerIn)
        LclRetriggerIn->SetCallback(std::bind(&AdcsIf::LclRetriggerCb, this, _1));

    if(AsynchRequestMgr)
        smpScheduler = simulator->GetScheduler();
    // MARKER: CONNECT BODY: END
}
// --CLOSING ELEMENT--AdcsIf::Connect--

// -----------------------------------------------------------------------------
// ------------------------ Custom method definitions --------------------------
// -----------------------------------------------------------------------------
// --OPENING ELEMENT--::Adcs::AdcsIf/AdcsIf::Extra_Methods--
// MARKER: EXTRA METHOD DEFINITIONS: START
void AdcsIf::RequestCompleted(::Smp::UInt8, const ::std::vector<Smp::UInt8>& command, const ::std::vector<Smp::UInt8>& response, ::Smp::UInt8 error)
{
    Trace(5, "RequestCompleted");

    if(error == 0)
    {
        std::vector<Smp::UInt8> block;
        block.reserve(command.size() + response.size());
        block.insert(block.end(), command.begin(), command.end());
        block.insert(block.end(), response.begin(), response.end());

        SendResponse(block, false);
    }
    else
    {
        std::vector<Smp::UInt8> block(command);
        if(error == 0xFF)
        {
            block.pop_back();
            block.push_back(0x52);
        }
        else
        {
            Log(Smp::Services::LMK_Warning, "Unknown error %u", error);
            block.push_back(error);
        }

        SendResponse(block, true);
    }
}

void AdcsIf::RslgRequestCompleted(::Smp::UInt8,
                                  const ::std::vector<Smp::UInt8>& command,
                                  const ::std::vector<Smp::UInt8>& response,
                                  ::Smp::UInt8 error)
{
    if(false == receivedUnitResponse)
    {
        Trace(5, "RslgRequestCompleted");

        /* SRP# 928 introduced dismiss faulty/ambigious callback behaviour
         * timeout callback cannot be deleted because its created after the DataReceived callback was consumed.
         * Send between the units its happening to fast, this is why there is a timeout cb even when the data was
         * received successfully */
        receivedUnitResponse = true;

        if(error == 0)
        {
            std::vector<Smp::UInt8> block;
            block.reserve(3 + response.size());
            block.push_back(command[0]);
            block.push_back(command[1]);
            block.push_back(command[3]);
            block.insert(block.end(), response.begin(), response.end());

            // MTSP
            CommandResult cr = CommandResult::CR_Success;
            if(command[0] == 0x08 && command[1] == 0x00)
            {
                cr = GetMgtqTm(block);
            }

            SendResponse(block, cr == CommandResult::CR_Failure);
        }
        else
        {
            std::vector<Smp::UInt8> block(command.begin(), command.begin() + 2);
            if(error == 0xFF)
            {
                block.push_back(0x52);
            }
            else if(error == 0x53)
            {
                block.push_back(0x53);
            }
            else
            {
                Log(Smp::Services::LMK_Warning, "Unknown error %u", error);
                block.push_back(error);
            }

            SendResponse(block, true);
        }
    }
}

void AdcsIf::MgtmRequestCompleted(::Smp::UInt8,
                                  const ::std::vector<Smp::UInt8>& command,
                                  const ::std::vector<Smp::UInt8>& response,
                                  ::Smp::UInt8 error)
{
    Trace(5, "MgtmRequestCompleted");

    if(error == 0)
    {
        if(response.size() != command[2])
        {
            Log(Smp::Services::LMK_Error, "Received data length %u, expected %u", response.size(), command[2]);

            error = 0x53;
        }
    }

    RslgRequestCompleted(0, command, response, error);
}

void AdcsIf::RwRequestCompleted(::Smp::UInt8,
                                const ::std::vector<Smp::UInt8>& command,
                                const ::std::vector<Smp::UInt8>& response,
                                ::Smp::UInt8 error)
{
    Trace(5, "RwRequestCompleted");

    if(error == 0)
    {
        if(response.size() < command[2])
        {
            Log(Smp::Services::LMK_Error, "Received data length %u, expected %u", response.size(), command[2]);

            error = 0x53;
        }
        else if(response.size() >= command[2])
        {
            if(response.back() != 0xD)
            {
                error = 0x53;
            }
        }
    }
    RslgRequestCompleted(0, command, response, error);
}

CommandResult AdcsIf::GetIfStatus(std::vector<Smp::UInt8>& block) const
{
    if(Status.Count() != 0)
    {
        Status->Serialize(block);
    }

    return CR_Success;
}

CommandResult AdcsIf::GetIfHk(std::vector<Smp::UInt8>& block) const
{
    const unsigned responseSize = 38;

    std::vector<Smp::UInt8> msgBlock;

    if(Hk.Count() != 0)
    {
        Hk->Serialize(msgBlock);
    }

    if(msgBlock.size() == responseSize)
    {
        block.insert(block.end(), msgBlock.begin(), msgBlock.end());
        return CR_Success;
    }

    return CR_Failure;
}

CommandResult AdcsIf::SetIfPower(std::vector<Smp::UInt8>& block)
{
    Smp::UInt8 adcsMode = block.back();
    block.pop_back();

    set_AdcsPowerStatus((ASPW)adcsMode);

    return CR_Success;
}

CommandResult AdcsIf::SetUnitStatus(std::vector<Smp::UInt8>& block)
{
    Smp::UInt8 byte0 = *(block.end() - 2);
    Smp::UInt8 byte1 = *(block.end() - 1);
    block.resize(2);

    Smp::UInt8 agye = byte0 & 0x03;
    Smp::UInt8 arwe = (byte0 >> 2) & 0x01;
    Smp::UInt8 amte = byte0 >> 5;
    Smp::UInt8 amme = byte1 & 0x03;
    Smp::UInt8 asse = byte1 >> 2;

    Trace(1, "SetUnitStatus: AGYE=%d, ARWE=%d, AMTE=%d, AMME=%d, ASSE=%d", agye, arwe, amte, amme, asse);

    Status->set_Agye((Adcs::GMStatus)agye);
    Status->set_Amme((Adcs::GMStatus)amme);
    Status->set_Amte(amte);
    Status->set_Arwe((Adcs::RwStatus)arwe);
    Status->set_Asse(asse);

    return CR_Success;
}

CommandResult AdcsIf::SetConfiguration(std::vector<Smp::UInt8>& block)
{
    std::vector<Smp::UInt8> bytes(block);

    block.resize(2);

    ::Smp::Bool clrc = bytes[2] & 0x01;
    ::Smp::UInt8 nbSp = bytes[3];

    if(clrc)
    {
        Status->ClearResetCounter();
    }

    SunSensor->nbOfSamples = nbSp;

    Gyroscope->timeOut = bytes[4];
    Magnetometer->timeOut = bytes[5];
    ReactionWheel->timeOut = bytes[6];

    return CR_Success;
}

CommandResult AdcsIf::LclRetrigger(std::vector<Smp::UInt8>&)
{
    if(LclRetriggerOut != nullptr)
    {
        LclRetriggerOut->Pulse(1.0);
    }
    else
    {
        Log(Smp::Services::LMK_Warning, "LCL retrigger output not connected");
    }
    return CR_Success;
}

CommandResult AdcsIf::GetGyroTm(std::vector<Smp::UInt8>& block)
{
    auto request = AsynchRequest[0];
    request->SetSender(Gyroscope[0]);
    request->SetScheduler(smpScheduler);
    request->SetManager(AsynchRequestMgr[0]);

    using namespace std::placeholders;

    request->SetCompletionCb(std::bind(&AdcsIf::RequestCompleted, this, _1, _2, _3, _4));

    try
    {
        request->Send(0, block, Gyroscope->timeOut * 1.02);
    }
    catch(::Smp::UInt8 error)
    {
        block.pop_back();
        block.push_back(error);

        return CR_Failure;
    }

    return CR_InProgress;
}

CommandResult AdcsIf::BuildMgtmDataRequestCmd(std::vector<Smp::UInt8>& block)
{
    try
    {
        Magnetometer->BuildMgtmDataRequestCmd(block);
    }
    catch(::Smp::UInt8 error)
    {
        block.resize(2);
        block.push_back(error);

        return CR_Failure;
    }

    return CR_Success;
}

CommandResult AdcsIf::GetMgtmTm(std::vector<Smp::UInt8>& block)
{
    auto request = AsynchRequest[0];
    request->SetSender(Magnetometer[0]);
    request->SetScheduler(smpScheduler);
    request->SetManager(AsynchRequestMgr[0]);

    using namespace std::placeholders;
    request->SetCompletionCb(std::bind(&AdcsIf::MgtmRequestCompleted, this, _1, _2, _3, _4));

    receivedUnitResponse = false;

    try
    {
        request->Send(0, block, Magnetometer->timeOut * 1.02);
    }
    catch(::Smp::UInt8 error)
    {
        block.resize(2);
        block.push_back(error);

        return CR_Failure;
    }

    return CR_InProgress;
}

CommandResult AdcsIf::GetSsTm(std::vector<Smp::UInt8>& block)
{
    try
    {
        SunSensor->GetTm(block);
    }
    catch(::Smp::UInt8 error)
    {
        block.push_back(error);

        return CR_Failure;
    }

    return CR_Success;
}

CommandResult AdcsIf::GetSsTemp(std::vector<Smp::UInt8>& block)
{
    try
    {
        SunSensor->GetTemp(block);
    }
    catch(::Smp::UInt8 error)
    {
        block.push_back(error);

        return CR_Failure;
    }

    return CR_Success;
}

CommandResult AdcsIf::GetRwTm(std::vector<Smp::UInt8>& block)
{
    auto request = AsynchRequest[0];
    request->SetSender(ReactionWheel[0]);
    request->SetScheduler(smpScheduler);
    request->SetManager(AsynchRequestMgr[0]);

    receivedUnitResponse = false;

    using namespace std::placeholders;
    request->SetCompletionCb(std::bind(&AdcsIf::RwRequestCompleted, this, _1, _2, _3, _4));

    try
    {
        request->Send(0, block, ReactionWheel->timeOut * 1.02);
    }
    catch(::Smp::UInt8 error)
    {
        block.resize(2);
        block.push_back(error);

        return CR_Failure;
    }

    return CR_InProgress;
}

CommandResult AdcsIf::SetMgtqPwm(std::vector<Smp::UInt8>& block)
{
    try
    {
        Magnetorquer->SetPwm(block);
    }
    catch(::Smp::UInt8 error)
    {
        block.resize(2);
        block.push_back(error);

        return CR_Failure;
    }

    return CR_Success;
}

CommandResult AdcsIf::GetMgtqTm(std::vector<Smp::UInt8>& block)
{
    try
    {
        Magnetorquer->GetTm(block);
    }
    catch(::Smp::UInt8 error)
    {
        block.resize(2);
        block.push_back(error);

        return CR_Failure;
    }

    return CR_Success;
}

CommandResult AdcsIf::GetIfScHk(std::vector<Smp::UInt8>& block) const
{
    const unsigned responseSize = 18;

    std::vector<Smp::UInt8> msgBlock;

    if(ScHk.Count() != 0)
    {
        ScHk->Serialize(msgBlock);
        block.insert(block.end(), msgBlock.begin(), msgBlock.end());
    }

    if(msgBlock.size() == responseSize)
    {
        return CR_Success;
    }

    return CR_Failure;
}

void AdcsIf::SendResponse(std::vector<Smp::UInt8> block, Smp::Bool isErrorResponse)
{
    if(isErrorResponse)
    {
        ObcLinkDll->SendUserErrorBlock(block);
    }
    else
    {
        ObcLinkDll->SendUserBlock(block);
    }
}

void AdcsIf::sendMessageIfReady()
{
    if(pendingBytesToSend == 0)
    {
        Smp::UInt16 messageLength = 0;
        if(sendBuffer.size() > 2u)
        {
            messageLength = sendBuffer[2] + 5u;
        }

        if(messageLength > 0 && sendBuffer.size() == messageLength)
        {
            if(CheckTxEnabled())
            {
                pendingBytesToSend = messageLength;
                Smp::Duration delay = 1000000000ULL / (500000 / 10); // ns
                smpScheduler->AddSimulationTimeEvent(delayTransmission, delay, delay, messageLength - 1);
            }
            else
            {
                sendBuffer.clear();
            }
        }
    }
}
// MARKER: EXTRA METHOD DEFINITIONS: END
// --CLOSING ELEMENT--::Adcs::AdcsIf/AdcsIf::Extra_Methods--

} // end namespace Adcs
