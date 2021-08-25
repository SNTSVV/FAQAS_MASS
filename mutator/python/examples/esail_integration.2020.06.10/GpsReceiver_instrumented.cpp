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

#include "GpsReceiver.h"
#include "Generic/PoweredUnit.h"
#include "Mdk/Aggregate.h"
#include "Mdk/Composite.h"
#include "Mdk/EntryPoint.h"
#include "Mdk/Management/EntryPointPublisher.h"
#include "Mdk/Management/ManagedContainer.h"
#include "Mdk/Management/ManagedReference.h"

#include "Generic/ByteVectorPort.h"
#include "Generic/CommandsBuffer.h"
#include "Generic/IProtocolLayer.h"
#include "Generic/WinMonProtocol.h"
#include "Gps/GpsReceiver.h"
#include "Mdk/SimpleTypes.h"
#include "Smp/Publication/IClassType.h"
#include "Smp/Publication/IEnumerationType.h"
#include "Smp/Publication/IPublishOperation.h"
#include "Smp/Publication/IStructureType.h"
#include "Smp/Publication/IType.h"
#include "Smp/Services/ServicesNamespace.h"
#include "Smp/SimpleTypes.h"
#include "SystemIF/IPulseLine.h"
#include "functional"
#include "vector"

//FAQAS INCLUSION
#include "FAQAS_dataDrivenMutator.h"

// --OPENING ELEMENT--::Gps::GpsReceiver/GpsReceiver::Extra_Includes--
// MARKER: FILE INCLUSION: START
#include "Common/CommonNamespace.h"
#include "Generic/Utils.h"
#include "OhbCommon/ByteUtils.h"
// MARKER: FILE INCLUSION: END

// MARKER: FORWARD DECLARATIONS: START
// MARKER: FORWARD DECLARATIONS: END

// -----------------------------------------------------------------------------
// ------------------------ Custom field definitions ---------------------------
// -----------------------------------------------------------------------------
// MARKER: EXTRA FIELD DEFINITIONS: START
// MARKER: EXTRA FIELD DEFINITIONS: END
// --CLOSING ELEMENT--::Gps::GpsReceiver/GpsReceiver::Extra_Includes--

namespace Gps
{
// --OPENING ELEMENT--GpsReceiver::SetRecvBlockCb--
/// The callback function that is called when a block of data is received.
/// @param recvBlockCb The receive block callback function.

//FAQAS MUTATE function
int mutate( std::vector<double> *v, FaultModel *fm ){
    return _FAQAS_mutate(v->data(),fm);
}
//end of Mutate function


void GpsReceiver::SetRecvBlockCb(::std::function<void(const ::std::vector<Smp::UInt8>&)> recvBlockCb)
{
    // MARKER: OPERATION BODY: START
    Trace(5, "GpsReceiver::SetRecvBlockCb");

    recvWinMonDataCb = recvBlockCb;
    // MARKER: OPERATION BODY: END
}
// --CLOSING ELEMENT--GpsReceiver::SetRecvBlockCb--

// --OPENING ELEMENT--GpsReceiver::SendBlock--
/// Sends a block of data.
/// @param block The data block to be sent.
void GpsReceiver::SendBlock(const std::vector<Smp::UInt8>& block)
{
    // MARKER: OPERATION BODY: START
    Trace(5, "GpsReceiver::SendBlock");

    if(block.size() < 2)
    {
        Log(Smp::Services::LMK_Warning, "Unexpected size %u < 2", block.size());

        return;
    }

    switch(block[0])
    {
    case 'A':
    {
        switch(block[1])
        {
        case 'M':
        {
            SelectAidingMode(block.begin() + 2, block.end());
        }
        break;
        default:
        {
            Log(Smp::Services::LMK_Warning, "Unknown command %c%c", (char)block[0], (char)block[1]);
        }
        }
    }
    break;
    case 'C':
    {
        switch(block[1])
        {
        case 'S':
        {
            ColdStart(block.begin() + 2, block.end());
        }
        break;
        default:
        {
            Log(Smp::Services::LMK_Warning, "Unknown command %c%c", (char)block[0], (char)block[1]);
        }
        }
    }
    break;
    case 'D':
    {
        switch(block[1])
        {
        case 'R':
        {
            DataRate(block.begin() + 2, block.end());
        }
        break;
        default:
        {
            Log(Smp::Services::LMK_Warning, "Unknown command %c%c", (char)block[0], (char)block[1]);
        }
        }
    }
    break;
    case 'E':
    {
        switch(block[1])
        {
        case 'A':
        {
            EndAlmanac(block.begin() + 2, block.end());
        }
        break;
        default:
        {
            Log(Smp::Services::LMK_Warning, "Unknown command %c%c", (char)block[0], (char)block[1]);
        }
        }
    }
    break;
    case 'F':
    {
        if(block.size() < 3)
        {
            Log(Smp::Services::LMK_Warning, "Unexpected size %u < 3", block.size());

            return;
        }

        switch(block[1])
        {
        case '1':
        {
            switch(block[2])
            {
            case '3':
            {
                LoadF13(block.begin() + 3, block.end());
            }
            break;
            case '4':
            {
                LoadF14(block.begin() + 3, block.end());
            }
            break;
            case '5':
            {
                LoadF15(block.begin() + 3, block.end());
            }
            break;
            default:
            {
                Log(Smp::Services::LMK_Warning, "Unknown command %c%c%c", (char)block[0], (char)block[1], (char)block[2]);
            }
            }
        }
        break;
        default:
        {
            Log(Smp::Services::LMK_Warning, "Unknown command %c%c%c", (char)block[0], (char)block[1], (char)block[2]);
        }
        }
    }
    break;
    case 'L':
    {
        switch(block[1])
        {
        case 'A':
        {
            LoadAlmanac(block.begin() + 2, block.end());
        }
        break;
        case 'O':
        {
            LoadOrbit(block.begin() + 2, block.end());
        }
        break;
        default:
        {
            Log(Smp::Services::LMK_Warning, "Unknown command %c%c", (char)block[0], (char)block[1]);
        }
        }
    }
    break;
    case 'S':
    {
        switch(block[1])
        {
        case 'A':
        {
            SaveAlmanac(block.begin() + 2, block.end());
        }
        break;
        default:
        {
            Log(Smp::Services::LMK_Warning, "Unknown command %c%c", (char)block[0], (char)block[1]);
        }
        }
    }
    break;
    case 'T':
    {
        switch(block[1])
        {
        case 'A':
        {
            TransmitAlmanac(block.begin() + 2, block.end());
        }
        break;
        case 'E':
        {
            TransmitEphemeris(block.begin() + 2, block.end());
        }
        break;
        case 'O':
        {
            TransmitOrbit(block.begin() + 2, block.end());
        }
        break;
        default:
        {
            Log(Smp::Services::LMK_Warning, "Unknown command %c%c", (char)block[0], (char)block[1]);
        }
        }
    }
    break;
    default:
    {
        Log(Smp::Services::LMK_Warning, "Unknown command %c%c", (char)block[0], (char)block[1]);
    }
    }
    // MARKER: OPERATION BODY: END
}
// --CLOSING ELEMENT--GpsReceiver::SendBlock--

// --OPENING ELEMENT--GpsReceiver::AfterPowerOff--
void GpsReceiver::AfterPowerOff()
{
    // MARKER: OPERATION BODY: START
    cartNavData.navigationStatus = 0;

    if(ppsEventId != 0)
    {
        simulator->GetScheduler()->RemoveEvent(ppsEventId);
        ppsEventId = 0;
    }

    if(sendMessageEventId != 0)
    {
        simulator->GetScheduler()->RemoveEvent(sendMessageEventId);
        sendMessageEventId = 0;
    }
    // MARKER: OPERATION BODY: END
}
// --CLOSING ELEMENT--GpsReceiver::AfterPowerOff--

// --OPENING ELEMENT--GpsReceiver::AfterPowerOn--
void GpsReceiver::AfterPowerOn()
{
    // MARKER: OPERATION BODY: START
    if(twoHzVersion)
    {
        if(realtime)
        {
            ppsEventId = simulator->GetScheduler()->AddZuluTimeEvent(PpsEntry, 500000000ULL, 500000000ULL, -1);
        }
        else
        {
            ppsEventId = simulator->GetScheduler()->AddSimulationTimeEvent(PpsEntry, 500000000ULL, 500000000ULL, -1);
        }
    }
    else
    {
        if(realtime)
        {
            ppsEventId = simulator->GetScheduler()->AddZuluTimeEvent(PpsEntry, 1000000000ULL, 1000000000ULL, -1);
        }
        else
        {
            ppsEventId = simulator->GetScheduler()->AddSimulationTimeEvent(PpsEntry, 1000000000ULL, 1000000000ULL, -1);
        }
    }

    if(achieveLockEventId != 0)
    {
        simulator->GetScheduler()->RemoveEvent(achieveLockEventId);
    }

    achieveLockEventId = simulator->GetScheduler()->AddSimulationTimeEvent(AchieveLock, lockDelay * 1000000000ULL);
    // MARKER: OPERATION BODY: END
}
// --CLOSING ELEMENT--GpsReceiver::AfterPowerOn--

// --OPENING ELEMENT--GpsReceiver::SyncTime--
/// Synchronize the time reported in the F40 message with the current epoch time. Only relevant when field realtime is
/// set to true. If never called, the zulu time reported is relative to 0.
void GpsReceiver::SyncTime()
{
    // MARKER: OPERATION BODY: START
    zuluTimeOffset = simulator->GetTimeKeeper()->GetEpochTime() - simulator->GetTimeKeeper()->GetZuluTime();
    // MARKER: OPERATION BODY: END
}
// --CLOSING ELEMENT--GpsReceiver::SyncTime--

// --OPENING ELEMENT--GpsReceiver::_PpsEntry--
// Handler for Entry Point: PpsEntry
void GpsReceiver::_PpsEntry()
{
    // MARKER: OPERATION BODY: START
    Trace(5, "%s", __PRETTY_FUNCTION__);

    if(PpsOut == nullptr)
    {
        Log(Smp::Services::LMK_Error, "PpsOut not set");
    }
    else
    {
        if(!twoHzVersion || (ppsCounter % 2 == 0))
        {
            PpsOut->Pulse(0);
        }
    }

    if(f40Interval > 0)
    {
        if(ppsCounter % f40Interval == 0)
        {
            SendF40();
        }
    }

    ++ppsCounter;
    // MARKER: OPERATION BODY: END
}
// --CLOSING ELEMENT--GpsReceiver::_PpsEntry--
// --OPENING ELEMENT--GpsReceiver::_AchieveLock--
// Handler for Entry Point: AchieveLock
void GpsReceiver::_AchieveLock()
{
    // MARKER: OPERATION BODY: START
    cartNavData.navigationStatus = 2;
    achieveLockEventId = 0;
    // MARKER: OPERATION BODY: END
}
// --CLOSING ELEMENT--GpsReceiver::_AchieveLock--
// --OPENING ELEMENT--GpsReceiver::_SendBufferMessage--
// Handler for Entry Point: SendBufferMessage
void GpsReceiver::_SendBufferMessage()
{
    // MARKER: OPERATION BODY: START
    if(sendBuffer.size() > 0)
    {
        // Retrieve message info
        auto entry = sendBuffer.front();
        auto id = entry.first;
        auto data = entry.second;

        // Register message
        CommandsBuffer.AddCommand(simulator->GetTimeKeeper()->GetSimulationTime(), id, 0, data, 0);

        // Send message
        recvWinMonDataCb(data);

        // Remove message from buffer
        sendBuffer.pop_front();

        if(sendBuffer.size() > 0)
        {
            // Schedule sending of next message
            sendMessageEventId = simulator->GetScheduler()->AddSimulationTimeEvent(SendBufferMessage, getMessageDelay(id));
        }
        else
        {
            // End
            sendMessageEventId = 0;
        }
    }
    // MARKER: OPERATION BODY: END
}
// --CLOSING ELEMENT--GpsReceiver::_SendBufferMessage--

// - - - - - - - - - - - - - - Publish - - - - - - - - - - - - - - - -
// The Publish method is implemented in the GpsReceiverSmp.cpp file.

// --OPENING ELEMENT--GpsReceiver::Configure--
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

void GpsReceiver::Configure(::Smp::Services::ILogger* logger)
{
    // Call base implementation first
    ::Generic::PoweredUnit::Configure(logger);

    // MARKER: CONFIGURE BODY: START
    // MARKER: CONFIGURE BODY: END
}
// --CLOSING ELEMENT--GpsReceiver::Configure--

// --OPENING ELEMENT--GpsReceiver::Connect--
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

void GpsReceiver::Connect(::Smp::ISimulator* simulator)
{
    // Call Mdk implementation first
    ::Generic::PoweredUnit::Connect(simulator);
    SetupContainersAndReferences(simulator);

    // MARKER: CONNECT BODY: START
    using namespace std::placeholders;

    if(SerialPort)
        SerialPort->SetRecvDataCb((std::bind(&GpsReceiver::SerialDataRecvCb, this, _1)));

    if(WinMonProtocol)
        WinMonProtocol->SetRecvBlockCb((std::bind(&GpsReceiver::EncWinMonDataCb, this, _1)));
    // MARKER: CONNECT BODY: END
}
// --CLOSING ELEMENT--GpsReceiver::Connect--

// -----------------------------------------------------------------------------
// ------------------------ Custom method definitions --------------------------
// -----------------------------------------------------------------------------
// --OPENING ELEMENT--::Gps::GpsReceiver/GpsReceiver::Extra_Methods--
// MARKER: EXTRA METHOD DEFINITIONS: START
void GpsReceiver::SerialDataRecvCb(const ::std::vector<::Smp::UInt8>& block)
{
    WinMonProtocol->SendBlock(block);
}

void GpsReceiver::EncWinMonDataCb(const ::std::vector<::Smp::UInt8>& data)
{
    SerialPort->SendData(data);
}

void GpsReceiver::SelectAidingMode(::std::vector<Smp::UInt8>::const_iterator begin, ::std::vector<Smp::UInt8>::const_iterator end)
{
    Trace(5, "%s", __PRETTY_FUNCTION__);

    if((end - begin) != 1)
    {
        Log(Smp::Services::LMK_Warning, "Unexpected size %u != 1", (end - begin));
        return;
    }

    ::Smp::UInt8 aidingMode = *begin - '0';

    switch(aidingMode)
    {
    case 0:
        SendF98("I", "AM", "Disabled aiding");
        break;
    case 1:
        SendF98("I", "AM", "Enabled aiding for ballistic trajectories");
        break;
    case 2:
        SendF98("I", "AM", "Enabled aiding for low Earth orbits");
        break;
    default:
    {
        Log(Smp::Services::LMK_Warning, "Unexpected aiding mode %c", (char)*begin);
        return;
    }
    }
}

void GpsReceiver::ColdStart(::std::vector<Smp::UInt8>::const_iterator begin, ::std::vector<Smp::UInt8>::const_iterator end)
{
    Trace(5, "%s", __PRETTY_FUNCTION__);

    if((end - begin) != 0)
    {
        Log(Smp::Services::LMK_Warning, "Unexpected size %u != 0", (end - begin));
        return;
    }

    Reset();

    SendF98("I", "CS", "Performed cold start");
}

void GpsReceiver::DataRate(::std::vector<Smp::UInt8>::const_iterator begin, ::std::vector<Smp::UInt8>::const_iterator end)
{
    Trace(5, "%s", __PRETTY_FUNCTION__);

    size_t idx;

    ::std::string messageNumberString(begin, begin + 2);
    ::Smp::Int8 messageNumber = std::stoi(messageNumberString, &idx);
    if(idx != 2)
    {
        Log(Smp::Services::LMK_Warning, "Unexpected message number %s", messageNumberString);
        return;
    }

    ::std::string outputIntervalString(begin + 2, end);
    ::Smp::Int16 outputInterval = std::stoi(outputIntervalString, &idx);
    if(idx != 1 && idx != 2 && idx != 3)
    {
        Log(Smp::Services::LMK_Warning, "Unexpected output interval %s", outputIntervalString);
        return;
    }

    if((outputInterval < -1) || (outputInterval > 127))
    {
        Log(Smp::Services::LMK_Warning, "Unexpected output interval [-1, 127] %i", outputInterval);
        return;
    }

    ::Smp::Float32 outputIntervalFloat = outputInterval;
    if(twoHzVersion)
    {
        outputIntervalFloat = outputIntervalFloat / 2;
    }

    std::string operationStr1;
    std::string operationStr2;
    if(outputInterval > 0)
    {
        operationStr1 = "Changed";
        std::stringstream ss;
        ss << std::fixed << std::setprecision(1) << outputIntervalFloat;

        operationStr2 = "output interval to " + ss.str() + "s";
    }
    else if(outputInterval == 0)
    {
        operationStr1 = "Requested";
        operationStr2 = "one-time output";
    }
    else
    {
        operationStr1 = "Disabled";
        operationStr2 = "output";
    }

    std::string messageId;
    if(messageNumber >= 0)
    {
        messageId = "F" + messageNumberString;
    }

    SendF98("I", "DR", operationStr1 + " " + messageId + " " + operationStr2);

    if(messageNumber != 40)
    {
        Log(Smp::Services::LMK_Error, "Data Rate telecommand for message ID %s not implemented", messageId);
        return;
    }

    if(outputInterval == 0)
    {
        SendF40();
    }
    else if(outputInterval > 0)
    {
        f40Interval = outputInterval;
    }
    else
    {
        f40Interval = 0;
    }
}
void GpsReceiver::LoadAlmanac(::std::vector<Smp::UInt8>::const_iterator begin, ::std::vector<Smp::UInt8>::const_iterator end)
{
    Trace(5, "%s", __PRETTY_FUNCTION__);

    if((end - begin) != 0)
    {
        Log(Smp::Services::LMK_Warning, "Unexpected size %u != 0", (end - begin));
        return;
    }

    loadAlmanac = true;

    almanacsReceived = 0;
    ephemeridesReceived = 0;
    ionoUtcModelDataReceived = false;

    SendF98("I", "LA", "Initiated loading of alm, ephem, iono/utc");
}

void GpsReceiver::EndAlmanac(::std::vector<Smp::UInt8>::const_iterator begin, ::std::vector<Smp::UInt8>::const_iterator end)
{
    Trace(5, "%s", __PRETTY_FUNCTION__);

    if((end - begin) != 0)
    {
        Log(Smp::Services::LMK_Warning, "Unexpected size %u != 0", (end - begin));
        return;
    }

    if(loadAlmanac)
    {
        loadAlmanac = false;
        SendF98("I", "EA", "32 almanac messages loaded");
    }
    else
    {
        SendF98("E", "EA", "Ignored unsupported command");
    }
}

void GpsReceiver::SaveAlmanac(::std::vector<Smp::UInt8>::const_iterator begin, ::std::vector<Smp::UInt8>::const_iterator end)
{
    Trace(5, "%s", __PRETTY_FUNCTION__);

    if((end - begin) != 0)
    {
        Log(Smp::Services::LMK_Warning, "Unexpected size %u != 0", (end - begin));
        return;
    }

    saveAlmanac = true;

    SendF98("I", "SA", " Requested alm, eph, ion/utc output");

    for(unsigned n = 0; n < 32; ++n)
    {
        SendF13(n + 1);
    }

    for(unsigned n = 0; n < 32; ++n)
    {
        SendF14(n + 1);
    }

    SendF15();

    saveAlmanac = false;
}

void GpsReceiver::TransmitAlmanac(::std::vector<Smp::UInt8>::const_iterator begin, ::std::vector<Smp::UInt8>::const_iterator end)
{
    Trace(5, "%s", __PRETTY_FUNCTION__);

    if((end - begin) != 0)
    {
        Log(Smp::Services::LMK_Warning, "Unexpected size %u != 0", (end - begin));
        return;
    }

    transmitAlmanac = true;

    SendF98("I", "TA", "Requested almanac output");

    for(unsigned n = 0; n < 32; ++n)
    {
        SendF13(n + 1);
    }

    SendF15();

    transmitAlmanac = false;
}

void GpsReceiver::LoadOrbit(::std::vector<Smp::UInt8>::const_iterator begin, ::std::vector<Smp::UInt8>::const_iterator end)
{
    Trace(5, "%s", __PRETTY_FUNCTION__);

    if((end - begin) != 69)
    {
        Log(Smp::Services::LMK_Warning, "Unexpected size %u != 69", (end - begin));

        SendF98("E", "LO", " Inconsistent twoline elements");
        loadOrbit = false;
        return;
    }

    if(!loadOrbit)
    {
        loadOrbitTle1 = std::string(begin, end).c_str();
        SendF98("I", "LO", "Loaded twoline elements (line 1)");
        loadOrbit = true;
    }
    else
    {
        loadOrbitTle2 = std::string(begin, end).c_str();
        SendF98("I", "LO", " Loaded twoline elements (line 2)");
        loadOrbit = false;
    }
}

void GpsReceiver::TransmitOrbit(::std::vector<Smp::UInt8>::const_iterator begin, ::std::vector<Smp::UInt8>::const_iterator end)
{
    Trace(5, "%s", __PRETTY_FUNCTION__);

    if((end - begin) != 0)
    {
        Log(Smp::Services::LMK_Warning, "Unexpected size %u != 0", (end - begin));
        return;
    }

    transmitOrbit = true;

    SendF98("I", "TO", "Requested orbital elements output");

    SendF52();
}

void GpsReceiver::TransmitEphemeris(::std::vector<Smp::UInt8>::const_iterator begin, ::std::vector<Smp::UInt8>::const_iterator end)
{
    Trace(5, "%s", __PRETTY_FUNCTION__);

    if((end - begin) != 2)
    {
        Log(Smp::Services::LMK_Warning, "Unexpected size %u != 2", (end - begin));
        return;
    }

    size_t idx;

    ::std::string prnString(begin, begin + 2);
    ::Smp::Int8 prn = std::stoi(prnString, &idx);
    if(idx != 2)
    {
        Log(Smp::Services::LMK_Warning, "Unexpected PRN %s", prnString);
        return;
    }

    if((prn < 0) || (prn > 32))
    {
        Log(Smp::Services::LMK_Warning, "Unexpected PRN [0, 32] %i", prn);
        return;
    }

    transmitEphemeris = true;
    tePrn = prn;

    std::string prnStr;
    if(prn > 0)
    {
        prnStr = "PRN" + prnString;
    }
    else
    {
        prnStr = "all PRNs";
    }

    SendF98("I", "TE", "Requested GPS ephemeris output for " + prnStr);

    if(prn == 0)
    {
        for(unsigned n = 0; n < 32; ++n)
        {
            SendF14(n + 1);
        }
    }
    else
    {
        SendF14(prn);
    }

    transmitEphemeris = false;
}

void GpsReceiver::LoadF13(::std::vector<Smp::UInt8>::const_iterator begin, ::std::vector<Smp::UInt8>::const_iterator end)
{
    Trace(5, "%s", __PRETTY_FUNCTION__);

    if(!loadAlmanac)
    {
        Log(Smp::Services::LMK_Warning, "The load almanac command has not been received");
        return;
    }

    if((end - begin) != 113)
    {
        Log(Smp::Services::LMK_Warning, "Unexpected size %u != 113", (end - begin));
        return;
    }

    AlmanacData ad;
    size_t idx;
    ::std::string auxString;
    ::Smp::UInt16 paramSize;

    paramSize = 2;
    auxString = std::string(begin, begin + paramSize);
    Common::trim(auxString);
    ad.prn = std::stoi(auxString, &idx);
    if(idx != auxString.size())
    {
        Log(Smp::Services::LMK_Warning, "Unexpected PRN %s: can't convert to int", auxString);
        return;
    }
    if((ad.prn < 1) || (ad.prn > 32))
    {
        Log(Smp::Services::LMK_Warning, "Unexpected PRN %i: out of range [0, 32]", ad.prn);
        return;
    }
    begin += paramSize;

    paramSize = 1;
    auxString = std::string(begin, begin + paramSize);
    Common::trim(auxString);
    ad.vFlg = std::stoi(auxString, &idx);
    if(idx != auxString.size())
    {
        Log(Smp::Services::LMK_Warning, "Unexpected vFlg %s", auxString);
        return;
    }
    if(ad.vFlg > 2)
    {
        Log(Smp::Services::LMK_Warning, "Unexpected vFlg [0, 2] %i", ad.vFlg);
        return;
    }
    begin += paramSize;

    paramSize = 3;
    auxString = std::string(begin, begin + paramSize);
    Common::trim(auxString);
    ad.almHlth = std::stoi(auxString, &idx);
    if(idx != auxString.size())
    {
        Log(Smp::Services::LMK_Warning, "Unexpected almHlth %s", auxString);
        return;
    }
    begin += paramSize;

    paramSize = 4;
    auxString = std::string(begin, begin + paramSize);
    Common::trim(auxString);
    ad.refWeek = std::stoi(auxString, &idx);
    if(idx != auxString.size())
    {
        Log(Smp::Services::LMK_Warning, "Unexpected refWeek %s", auxString);
        return;
    }
    begin += paramSize;

    paramSize = 6;
    auxString = std::string(begin, begin + paramSize);
    Common::trim(auxString);
    ad.timeOfApplicability = std::stoi(auxString, &idx);
    if(idx != auxString.size())
    {
        Log(Smp::Services::LMK_Warning, "Unexpected timeOfApplicability %s", auxString);
        return;
    }
    begin += paramSize;

    paramSize = 9;
    auxString = std::string(begin, begin + paramSize);
    Common::trim(auxString);
    ad.eccentricity = std::stold(auxString, &idx);
    if(idx != auxString.size())
    {
        Log(Smp::Services::LMK_Warning, "Unexpected eccentricity %s", auxString);
        return;
    }
    begin += paramSize;

    paramSize = 9;
    auxString = std::string(begin, begin + paramSize);
    Common::trim(auxString);
    ad.inclination = std::stold(auxString, &idx);
    if(idx != auxString.size())
    {
        Log(Smp::Services::LMK_Warning, "Unexpected inclination %s", auxString);
        return;
    }
    begin += paramSize;

    paramSize = 15;
    auxString = std::string(begin, begin + paramSize);
    Common::trim(auxString);
    ad.rightAscensionRate = std::stold(auxString, &idx);
    if(idx != auxString.size())
    {
        Log(Smp::Services::LMK_Warning, "Unexpected rightAscensionRate %s", auxString);
        return;
    }
    begin += paramSize;

    paramSize = 9;
    auxString = std::string(begin, begin + paramSize);
    Common::trim(auxString);
    ad.semiMajorAxisSqrt = std::stold(auxString, &idx);
    if(idx != auxString.size())
    {
        Log(Smp::Services::LMK_Warning, "Unexpected semiMajorAxisSqrt %s", auxString);
        return;
    }
    begin += paramSize;

    paramSize = 10;
    auxString = std::string(begin, begin + paramSize);
    Common::trim(auxString);
    ad.rightAscension = std::stold(auxString, &idx);
    if(idx != auxString.size())
    {
        Log(Smp::Services::LMK_Warning, "Unexpected rightAscension %s", auxString);
        return;
    }
    begin += paramSize;

    paramSize = 10;
    auxString = std::string(begin, begin + paramSize);
    Common::trim(auxString);
    ad.argumentOfPerigee = std::stold(auxString, &idx);
    if(idx != auxString.size())
    {
        Log(Smp::Services::LMK_Warning, "Unexpected argumentOfPerigee %s", auxString);
        return;
    }
    begin += paramSize;

    paramSize = 10;
    auxString = std::string(begin, begin + paramSize);
    Common::trim(auxString);
    ad.meanAnomaly = std::stold(auxString, &idx);
    if(idx != auxString.size())
    {
        Log(Smp::Services::LMK_Warning, "Unexpected meanAnomaly %s", auxString);
        return;
    }
    begin += paramSize;

    paramSize = 10;
    auxString = std::string(begin, begin + paramSize);
    Common::trim(auxString);
    ad.af0 = std::stold(auxString, &idx);
    if(idx != auxString.size())
    {
        Log(Smp::Services::LMK_Warning, "Unexpected af0 %s", auxString);
        return;
    }
    begin += paramSize;

    paramSize = 15;
    auxString = std::string(begin, begin + paramSize);
    Common::trim(auxString);
    ad.af1 = std::stold(auxString, &idx);
    if(idx != auxString.size())
    {
        Log(Smp::Services::LMK_Warning, "Unexpected af1 %s", auxString);
        return;
    }
    begin += paramSize;

    almanacsReceived = OhbCommon::ByteUtils::SetBits(almanacsReceived, ad.prn - 1, 1, 1);
    almanacs[ad.prn - 1] = ad;
}

void GpsReceiver::LoadF14(::std::vector<Smp::UInt8>::const_iterator begin, ::std::vector<Smp::UInt8>::const_iterator end)
{
    Trace(5, "%s", __PRETTY_FUNCTION__);

    if(!loadAlmanac)
    {
        Log(Smp::Services::LMK_Warning, "The load almanac command has not been received");
        return;
    }

    if((end - begin) != 297)
    {
        Log(Smp::Services::LMK_Warning, "Unexpected size %u != 297", (end - begin));
        return;
    }

    EphemerisData ed;
    size_t idx;
    ::std::string auxString;
    ::Smp::UInt16 paramSize;

    paramSize = 2;
    auxString = std::string(begin, begin + paramSize);
    Common::trim(auxString);
    ed.prn = std::stoi(auxString, &idx);
    if(idx != auxString.size())
    {
        Log(Smp::Services::LMK_Warning, "Unexpected PRN %s", auxString);
        return;
    }
    if((ed.prn < 1) || (ed.prn > 32))
    {
        Log(Smp::Services::LMK_Warning, "Unexpected PRN [0, 32] %i", ed.prn);
        return;
    }
    begin += paramSize;

    paramSize = 1;
    auxString = std::string(begin, begin + paramSize);
    Common::trim(auxString);
    ed.vFlg = std::stoi(auxString, &idx);
    if(idx != auxString.size())
    {
        Log(Smp::Services::LMK_Warning, "Unexpected vFlg %s", auxString);
        return;
    }
    if(ed.vFlg > 2)
    {
        Log(Smp::Services::LMK_Warning, "Unexpected vFlg [0, 2] %i", ed.vFlg);
        return;
    }
    begin += paramSize;

    paramSize = 6;
    auxString = std::string(begin, begin + paramSize);
    Common::trim(auxString);
    ed.timeOfTransmission = std::stoi(auxString, &idx);
    if(idx != auxString.size())
    {
        Log(Smp::Services::LMK_Warning, "Unexpected timeOfTransmission %s", auxString);
        return;
    }
    begin += paramSize;

    paramSize = 3;
    auxString = std::string(begin, begin + paramSize);
    Common::trim(auxString);
    ed.s1Health = std::stoi(auxString, &idx);
    if(idx != auxString.size())
    {
        Log(Smp::Services::LMK_Warning, "Unexpected s1Health %s", auxString);
        return;
    }
    begin += paramSize;

    paramSize = 1;
    auxString = std::string(begin, begin + paramSize);
    Common::trim(auxString);
    ed.codeL2 = std::stoi(auxString, &idx);
    if(idx != auxString.size())
    {
        Log(Smp::Services::LMK_Warning, "Unexpected codeL2 %s", auxString);
        return;
    }
    begin += paramSize;

    paramSize = 4;
    auxString = std::string(begin, begin + paramSize);
    Common::trim(auxString);
    ed.weekNumber = std::stoi(auxString, &idx);
    if(idx != auxString.size())
    {
        Log(Smp::Services::LMK_Warning, "Unexpected weekNumber %s", auxString);
        return;
    }
    begin += paramSize;

    paramSize = 1;
    auxString = std::string(begin, begin + paramSize);
    Common::trim(auxString);
    ed.l2pData = std::stoi(auxString, &idx);
    if(idx != auxString.size())
    {
        Log(Smp::Services::LMK_Warning, "Unexpected l2pData %s", auxString);
        return;
    }
    begin += paramSize;

    paramSize = 2;
    auxString = std::string(begin, begin + paramSize);
    Common::trim(auxString);
    ed.ura = std::stoi(auxString, &idx);
    if(idx != auxString.size())
    {
        Log(Smp::Services::LMK_Warning, "Unexpected ura %s", auxString);
        return;
    }
    begin += paramSize;

    paramSize = 4;
    auxString = std::string(begin, begin + paramSize);
    Common::trim(auxString);
    ed.iodc = std::stoi(auxString, &idx);
    if(idx != auxString.size())
    {
        Log(Smp::Services::LMK_Warning, "Unexpected iodc %s", auxString);
        return;
    }
    begin += paramSize;

    paramSize = 13;
    auxString = std::string(begin, begin + paramSize);
    Common::trim(auxString);
    ed.tgd = std::stold(auxString, &idx);
    if(idx != auxString.size())
    {
        Log(Smp::Services::LMK_Warning, "Unexpected tgd %s", auxString);
        return;
    }
    begin += paramSize;

    paramSize = 4;
    auxString = std::string(begin, begin + paramSize);
    Common::trim(auxString);
    ed.tocWeekNumber = std::stoi(auxString, &idx);
    if(idx != auxString.size())
    {
        Log(Smp::Services::LMK_Warning, "Unexpected tocWeekNumber %s", auxString);
        return;
    }
    begin += paramSize;

    paramSize = 6;
    auxString = std::string(begin, begin + paramSize);
    Common::trim(auxString);
    ed.tocSecondsOfWeek = std::stoi(auxString, &idx);
    if(idx != auxString.size())
    {
        Log(Smp::Services::LMK_Warning, "Unexpected tocSecondsOfWeek %s", auxString);
        return;
    }
    begin += paramSize;

    paramSize = 13;
    auxString = std::string(begin, begin + paramSize);
    Common::trim(auxString);
    ed.af0 = std::stold(auxString, &idx);
    if(idx != auxString.size())
    {
        Log(Smp::Services::LMK_Warning, "Unexpected af0 %s", auxString);
        return;
    }
    begin += paramSize;

    paramSize = 16;
    auxString = std::string(begin, begin + paramSize);
    Common::trim(auxString);
    ed.af1 = std::stold(auxString, &idx);
    if(idx != auxString.size())
    {
        Log(Smp::Services::LMK_Warning, "Unexpected af1 %s", auxString);
        return;
    }
    begin += paramSize;

    paramSize = 20;
    auxString = std::string(begin, begin + paramSize);
    Common::trim(auxString);
    ed.af2 = std::stold(auxString, &idx);
    if(idx != auxString.size())
    {
        Log(Smp::Services::LMK_Warning, "Unexpected af2 %s", auxString);
        return;
    }
    begin += paramSize;

    paramSize = 3;
    auxString = std::string(begin, begin + paramSize);
    Common::trim(auxString);
    ed.iode = std::stoi(auxString, &idx);
    if(idx != auxString.size())
    {
        Log(Smp::Services::LMK_Warning, "Unexpected iode %s", auxString);
        return;
    }
    begin += paramSize;

    paramSize = 8;
    auxString = std::string(begin, begin + paramSize);
    Common::trim(auxString);
    ed.orbRadSineCorrAmp = std::stold(auxString, &idx);
    if(idx != auxString.size())
    {
        Log(Smp::Services::LMK_Warning, "Unexpected orbRadSineCorrAmp %s", auxString);
        return;
    }
    begin += paramSize;

    paramSize = 16;
    auxString = std::string(begin, begin + paramSize);
    Common::trim(auxString);
    ed.meanMotionDiff = std::stold(auxString, &idx);
    if(idx != auxString.size())
    {
        Log(Smp::Services::LMK_Warning, "Unexpected meanMotionDiff %s", auxString);
        return;
    }
    begin += paramSize;

    paramSize = 13;
    auxString = std::string(begin, begin + paramSize);
    Common::trim(auxString);
    ed.meanMotionAtRefTime = std::stold(auxString, &idx);
    if(idx != auxString.size())
    {
        Log(Smp::Services::LMK_Warning, "Unexpected meanMotionAtRefTime %s", auxString);
        return;
    }
    begin += paramSize;

    paramSize = 12;
    auxString = std::string(begin, begin + paramSize);
    Common::trim(auxString);
    ed.latCosCorrAmp = std::stold(auxString, &idx);
    if(idx != auxString.size())
    {
        Log(Smp::Services::LMK_Warning, "Unexpected latCosCorrAmp %s", auxString);
        return;
    }
    begin += paramSize;

    paramSize = 12;
    auxString = std::string(begin, begin + paramSize);
    Common::trim(auxString);
    ed.eccentricity = std::stold(auxString, &idx);
    if(idx != auxString.size())
    {
        Log(Smp::Services::LMK_Warning, "Unexpected eccentricity %s", auxString);
        return;
    }
    begin += paramSize;

    paramSize = 12;
    auxString = std::string(begin, begin + paramSize);
    Common::trim(auxString);
    ed.latSineCorrAmp = std::stold(auxString, &idx);
    if(idx != auxString.size())
    {
        Log(Smp::Services::LMK_Warning, "Unexpected latSineCorrAmp %s", auxString);
        return;
    }
    begin += paramSize;

    paramSize = 11;
    auxString = std::string(begin, begin + paramSize);
    Common::trim(auxString);
    ed.semiMajorAxisSqrt = std::stold(auxString, &idx);
    if(idx != auxString.size())
    {
        Log(Smp::Services::LMK_Warning, "Unexpected semiMajorAxisSqrt %s", auxString);
        return;
    }
    begin += paramSize;

    paramSize = 4;
    auxString = std::string(begin, begin + paramSize);
    Common::trim(auxString);
    ed.toeWeekNumber = std::stoi(auxString, &idx);
    if(idx != auxString.size())
    {
        Log(Smp::Services::LMK_Warning, "Unexpected toeWeekNumber %s", auxString);
        return;
    }
    begin += paramSize;

    paramSize = 6;
    auxString = std::string(begin, begin + paramSize);
    Common::trim(auxString);
    ed.toeSecondsOfWeek = std::stoi(auxString, &idx);
    if(idx != auxString.size())
    {
        Log(Smp::Services::LMK_Warning, "Unexpected toeSecondsOfWeek %s", auxString);
        return;
    }
    begin += paramSize;

    paramSize = 1;
    auxString = std::string(begin, begin + paramSize);
    Common::trim(auxString);
    ed.fti = std::stoi(auxString, &idx);
    if(idx != auxString.size())
    {
        Log(Smp::Services::LMK_Warning, "Unexpected fti %s", auxString);
        return;
    }
    begin += paramSize;

    paramSize = 12;
    auxString = std::string(begin, begin + paramSize);
    Common::trim(auxString);
    ed.incCosCorrAmp = std::stold(auxString, &idx);
    if(idx != auxString.size())
    {
        Log(Smp::Services::LMK_Warning, "Unexpected incCosCorrAmp %s", auxString);
        return;
    }
    begin += paramSize;

    paramSize = 13;
    auxString = std::string(begin, begin + paramSize);
    Common::trim(auxString);
    ed.ascNodeLong = std::stold(auxString, &idx);
    if(idx != auxString.size())
    {
        Log(Smp::Services::LMK_Warning, "Unexpected ascNodeLong %s", auxString);
        return;
    }
    begin += paramSize;

    paramSize = 12;
    auxString = std::string(begin, begin + paramSize);
    Common::trim(auxString);
    ed.incSineCorrAmp = std::stold(auxString, &idx);
    if(idx != auxString.size())
    {
        Log(Smp::Services::LMK_Warning, "Unexpected incSineCorrAmp %s", auxString);
        return;
    }
    begin += paramSize;

    paramSize = 13;
    auxString = std::string(begin, begin + paramSize);
    Common::trim(auxString);
    ed.inclination = std::stold(auxString, &idx);
    if(idx != auxString.size())
    {
        Log(Smp::Services::LMK_Warning, "Unexpected inclination %s", auxString);
        return;
    }
    begin += paramSize;

    paramSize = 8;
    auxString = std::string(begin, begin + paramSize);
    Common::trim(auxString);
    ed.orbRadCosCorrAmp = std::stold(auxString, &idx);
    if(idx != auxString.size())
    {
        Log(Smp::Services::LMK_Warning, "Unexpected orbRadCosCorrAmp %s", auxString);
        return;
    }
    begin += paramSize;

    paramSize = 13;
    auxString = std::string(begin, begin + paramSize);
    Common::trim(auxString);
    ed.olcArgOfPerigee = std::stold(auxString, &idx);
    if(idx != auxString.size())
    {
        Log(Smp::Services::LMK_Warning, "Unexpected olcArgOfPerigee %s", auxString);
        return;
    }
    begin += paramSize;

    paramSize = 16;
    auxString = std::string(begin, begin + paramSize);
    Common::trim(auxString);
    ed.rightAscensionRate = std::stold(auxString, &idx);
    if(idx != auxString.size())
    {
        Log(Smp::Services::LMK_Warning, "Unexpected rightAscensionRate %s", auxString);
        return;
    }
    begin += paramSize;

    paramSize = 16;
    auxString = std::string(begin, begin + paramSize);
    Common::trim(auxString);
    ed.inclinationRate = std::stold(auxString, &idx);
    if(idx != auxString.size())
    {
        Log(Smp::Services::LMK_Warning, "Unexpected inclinationRate %s", auxString);
        return;
    }
    begin += paramSize;

    ephemeridesReceived = OhbCommon::ByteUtils::SetBits(ephemeridesReceived, ed.prn - 1, 1, 1);
    ephemerides[ed.prn - 1] = ed;
}

void GpsReceiver::LoadF15(::std::vector<Smp::UInt8>::const_iterator begin, ::std::vector<Smp::UInt8>::const_iterator end)
{
    Trace(5, "%s", __PRETTY_FUNCTION__);

    if(!loadAlmanac)
    {
        Log(Smp::Services::LMK_Warning, "The load almanac command has not been received");
        return;
    }

    if((end - begin) != 137)
    {
        Log(Smp::Services::LMK_Warning, "Unexpected size %u != 137", (end - begin));
        return;
    }

    IonoUtcData iud;
    size_t idx;
    ::std::string auxString;
    ::Smp::UInt16 paramSize;

    paramSize = 1;
    auxString = std::string(begin, begin + paramSize);
    Common::trim(auxString);
    iud.vFlg = std::stoi(auxString, &idx);
    if(idx != auxString.size())
    {
        Log(Smp::Services::LMK_Warning, "Unexpected vFlg %s", auxString);
        return;
    }
    if(iud.vFlg > 1)
    {
        Log(Smp::Services::LMK_Warning, "Unexpected vFlg [0, 1] %i", iud.vFlg);
        return;
    }
    begin += paramSize;

    paramSize = 13;
    auxString = std::string(begin, begin + paramSize);
    Common::trim(auxString);
    iud.a0 = std::stold(auxString, &idx);
    if(idx != auxString.size())
    {
        Log(Smp::Services::LMK_Warning, "Unexpected a0 %s", auxString);
        return;
    }
    begin += paramSize;

    paramSize = 12;
    auxString = std::string(begin, begin + paramSize);
    Common::trim(auxString);
    iud.a1 = std::stold(auxString, &idx);
    if(idx != auxString.size())
    {
        Log(Smp::Services::LMK_Warning, "Unexpected a1 %s", auxString);
        return;
    }
    begin += paramSize;

    paramSize = 11;
    auxString = std::string(begin, begin + paramSize);
    Common::trim(auxString);
    iud.a2 = std::stold(auxString, &idx);
    if(idx != auxString.size())
    {
        Log(Smp::Services::LMK_Warning, "Unexpected a2 %s", auxString);
        return;
    }
    begin += paramSize;

    paramSize = 11;
    auxString = std::string(begin, begin + paramSize);
    Common::trim(auxString);
    iud.a3 = std::stold(auxString, &idx);
    if(idx != auxString.size())
    {
        Log(Smp::Services::LMK_Warning, "Unexpected a3 %s", auxString);
        return;
    }
    begin += paramSize;

    paramSize = 7;
    auxString = std::string(begin, begin + paramSize);
    Common::trim(auxString);
    iud.b0 = std::stoi(auxString, &idx);
    if(idx != auxString.size())
    {
        Log(Smp::Services::LMK_Warning, "Unexpected b0 %s", auxString);
        return;
    }
    begin += paramSize;

    paramSize = 8;
    auxString = std::string(begin, begin + paramSize);
    Common::trim(auxString);
    iud.b1 = std::stoi(auxString, &idx);
    if(idx != auxString.size())
    {
        Log(Smp::Services::LMK_Warning, "Unexpected b1 %s", auxString);
        return;
    }
    begin += paramSize;

    paramSize = 9;
    auxString = std::string(begin, begin + paramSize);
    Common::trim(auxString);
    iud.b2 = std::stoi(auxString, &idx);
    if(idx != auxString.size())
    {
        Log(Smp::Services::LMK_Warning, "Unexpected b2 %s", auxString);
        return;
    }
    begin += paramSize;

    paramSize = 9;
    auxString = std::string(begin, begin + paramSize);
    Common::trim(auxString);
    iud.b3 = std::stoi(auxString, &idx);
    if(idx != auxString.size())
    {
        Log(Smp::Services::LMK_Warning, "Unexpected b3 %s", auxString);
        return;
    }
    begin += paramSize;

    paramSize = 13;
    auxString = std::string(begin, begin + paramSize);
    Common::trim(auxString);
    iud.A0 = std::stold(auxString, &idx);
    if(idx != auxString.size())
    {
        Log(Smp::Services::LMK_Warning, "Unexpected A0 %s", auxString);
        return;
    }
    begin += paramSize;

    paramSize = 19;
    auxString = std::string(begin, begin + paramSize);
    Common::trim(auxString);
    iud.A1 = std::stold(auxString, &idx);
    if(idx != auxString.size())
    {
        Log(Smp::Services::LMK_Warning, "Unexpected A1 %s", auxString);
        return;
    }
    begin += paramSize;

    paramSize = 7;
    auxString = std::string(begin, begin + paramSize);
    Common::trim(auxString);
    iud.t_ot = std::stoi(auxString, &idx);
    if(idx != auxString.size())
    {
        Log(Smp::Services::LMK_Warning, "Unexpected t_ot %s", auxString);
        return;
    }
    begin += paramSize;

    paramSize = 4;
    auxString = std::string(begin, begin + paramSize);
    Common::trim(auxString);
    iud.t_lsDelta = std::stoi(auxString, &idx);
    if(idx != auxString.size())
    {
        Log(Smp::Services::LMK_Warning, "Unexpected t_lsDelta %s", auxString);
        return;
    }
    begin += paramSize;

    paramSize = 3;
    auxString = std::string(begin, begin + paramSize);
    Common::trim(auxString);
    iud.WNt = std::stoi(auxString, &idx);
    if(idx != auxString.size())
    {
        Log(Smp::Services::LMK_Warning, "Unexpected WNt %s", auxString);
        return;
    }
    begin += paramSize;

    paramSize = 3;
    auxString = std::string(begin, begin + paramSize);
    Common::trim(auxString);
    iud.WNlsf = std::stoi(auxString, &idx);
    if(idx != auxString.size())
    {
        Log(Smp::Services::LMK_Warning, "Unexpected WNlsf %s", auxString);
        return;
    }
    begin += paramSize;

    paramSize = 3;
    auxString = std::string(begin, begin + paramSize);
    Common::trim(auxString);
    iud.DN = std::stoi(auxString, &idx);
    if(idx != auxString.size())
    {
        Log(Smp::Services::LMK_Warning, "Unexpected DN %s", auxString);
        return;
    }
    begin += paramSize;

    paramSize = 4;
    auxString = std::string(begin, begin + paramSize);
    Common::trim(auxString);
    iud.t_lsfDelta = std::stoi(auxString, &idx);
    if(idx != auxString.size())
    {
        Log(Smp::Services::LMK_Warning, "Unexpected t_lsfDelta %s", auxString);
        return;
    }
    begin += paramSize;

    ionoUtcModelDataReceived = true;
    ionoUtc = iud;

    if((almanacsReceived == 0xFFFFFFFF) && (ephemeridesReceived == 0xFFFFFFFF))
    {
        loadAlmanac = false;
    }
}

void GpsReceiver::SendF13(::Smp::UInt8 prn)
{
    std::stringstream ss;
    const AlmanacData& ad = almanacs[prn - 1];

    ss << "F13" << std::left << std::setw(2) << (unsigned)prn << std::setw(1) << (unsigned)ad.vFlg << std::setw(3)
       << ad.almHlth << std::setw(4) << ad.refWeek << std::setw(6) << ad.timeOfApplicability << std::fixed
       << std::setw(9) << std::setprecision(7) << ad.eccentricity << std::showpos << std::setw(9)
       << std::setprecision(6) << ad.inclination << std::setw(15) << std::setprecision(12) << ad.rightAscensionRate
       << std::noshowpos << std::setw(9) << std::setprecision(4) << ad.semiMajorAxisSqrt << std::showpos
       << std::setw(10) << std::setprecision(7) << ad.rightAscension << std::setw(10) << std::setprecision(7)
       << ad.argumentOfPerigee << std::setw(10) << std::setprecision(7) << ad.meanAnomaly << std::setw(10)
       << std::setprecision(7) << ad.af0 << std::setw(15) << std::setprecision(12) << ad.af1;

    Trace(1, "F13: %s", ss.str());

    std::string response = ss.str();
    ::std::vector<::Smp::UInt8> responseData(response.begin(), response.end());
    SendMessage(13, responseData);
}

void GpsReceiver::SendF14(::Smp::UInt8 prn)
{
    std::stringstream ss;
    const EphemerisData& ed = ephemerides[prn - 1];

    ss << "F14" << std::left << std::setw(2) << (unsigned)prn << std::setw(1) << (unsigned)ed.vFlg << std::setw(6)
       << ed.timeOfTransmission << std::setw(3) << ed.s1Health << std::setw(1) << (unsigned)ed.codeL2 << std::setw(4)
       << ed.weekNumber << std::setw(1) << (unsigned)ed.l2pData << std::setw(2) << (unsigned)ed.ura << std::setw(4)
       << ed.iodc << std::fixed << std::showpos << std::setw(13) << std::setprecision(10) << ed.tgd << std::noshowpos
       << std::setw(4) << ed.tocWeekNumber << std::setw(6) << ed.tocSecondsOfWeek << std::showpos << std::setw(13)
       << std::setprecision(10) << ed.af0 << std::setw(16) << std::setprecision(13) << ed.af1 << std::setw(20)
       << std::setprecision(17) << ed.af2 << std::noshowpos << std::setw(3) << ed.iode << std::showpos << std::setw(8)
       << std::setprecision(2) << ed.orbRadSineCorrAmp << std::setw(16) << std::setprecision(13) << ed.meanMotionDiff
       << std::setw(13) << std::setprecision(10) << ed.meanMotionAtRefTime << std::setw(12) << std::setprecision(9)
       << ed.latCosCorrAmp << std::noshowpos << std::setw(12) << std::setprecision(10) << ed.eccentricity
       << std::showpos << std::setw(12) << std::setprecision(9) << ed.latSineCorrAmp << std::noshowpos << std::setw(11)
       << std::setprecision(6) << ed.semiMajorAxisSqrt << std::setw(4) << ed.toeWeekNumber << std::setw(6) << ed.toeSecondsOfWeek
       << std::setw(1) << (unsigned)ed.fti << std::showpos << std::setw(12) << std::setprecision(9) << ed.incCosCorrAmp
       << std::setw(13) << std::setprecision(10) << ed.ascNodeLong << std::setw(12) << std::setprecision(9) << ed.incSineCorrAmp
       << std::setw(13) << std::setprecision(10) << ed.inclination << std::setw(8) << std::setprecision(2)
       << ed.orbRadCosCorrAmp << std::setw(13) << std::setprecision(10) << ed.olcArgOfPerigee << std::setw(16)
       << std::setprecision(13) << ed.rightAscensionRate << std::setw(16) << std::setprecision(13) << ed.inclinationRate;

    Trace(1, "F14: %s", ss.str());

    std::string response = ss.str();
    ::std::vector<::Smp::UInt8> responseData(response.begin(), response.end());
    SendMessage(14, responseData);
}

void GpsReceiver::SendF15()
{
    std::stringstream ss;
    ss << "F15" << std::setw(1) << (unsigned)ionoUtc.vFlg << std::fixed << std::showpos << std::setw(13)
       << std::setprecision(10) << ionoUtc.a0 << std::setw(12) << std::setprecision(9) << ionoUtc.a1 << std::setw(11)
       << std::setprecision(8) << ionoUtc.a2 << std::setw(11) << std::setprecision(8) << ionoUtc.a3 << std::setw(7)
       << std::setprecision(0) << ionoUtc.b0 << std::setw(8) << std::setprecision(0) << ionoUtc.b1 << std::setw(9)
       << std::setprecision(0) << ionoUtc.b2 << std::setw(9) << std::setprecision(0) << ionoUtc.b3 << std::setw(13)
       << std::setprecision(10) << ionoUtc.A0 << std::setw(19) << std::setprecision(16) << ionoUtc.A1 << std::noshowpos
       << std::setw(7) << ionoUtc.t_ot << std::showpos << std::setw(4) << ionoUtc.t_lsDelta << std::noshowpos
       << std::setw(3) << ionoUtc.WNt << std::setw(3) << ionoUtc.WNlsf << std::setw(3) << ionoUtc.DN << std::showpos
       << std::setw(4) << ionoUtc.t_lsfDelta;

    Trace(1, "F15: %s", ss.str());

    std::string response = ss.str();
    ::std::vector<::Smp::UInt8> responseData(response.begin(), response.end());
    SendMessage(15, responseData);
}

void GpsReceiver::SendF40()
{
    ::Smp::UInt16 gpsWeek = 0;
    ::Smp::Float64 gpsSecondsOfWeek = 0.0;

    Generic::Utils::GetGpsTime(getCurrentDateTime(), gpsWeek, gpsSecondsOfWeek);

    //MANUALLY INSERTED PROBE
    std::vector<double> gps_buffer;
    gps_buffer.push_back(gpsWeek);
    gps_buffer.push_back(gpsSecondsOfWeek);
    gps_buffer.push_back(cartNavData.x);
    gps_buffer.push_back(cartNavData.y);
    gps_buffer.push_back(cartNavData.z);
    gps_buffer.push_back(cartNavData.vx);
    gps_buffer.push_back(cartNavData.vy);
    gps_buffer.push_back(cartNavData.vz);
    gps_buffer.push_back(cartNavData.navigationStatus);
    gps_buffer.push_back(cartNavData.nbOfTrackSats);
    gps_buffer.push_back(cartNavData.pdop);

    FaultModel *fm =_FAQAS_F40_FM();
    mutate( &gps_buffer, fm );
  	_FAQAS_delete_FM(fm);

    gpsWeek = gps_buffer[0];
    gpsSecondsOfWeek = gps_buffer[1];
    cartNavData.x = gps_buffer[2];
    cartNavData.y = gps_buffer[3];
    cartNavData.z = gps_buffer[4];
    cartNavData.vx = gps_buffer[5];
    cartNavData.vy = gps_buffer[6];
    cartNavData.vz = gps_buffer[7];
    cartNavData.navigationStatus = gps_buffer[8];
    cartNavData.nbOfTrackSats = gps_buffer[9];
    cartNavData.pdop = gps_buffer[10];
    //PROBE END

    std::stringstream ss;
    ss << "F40" << std::setw(4) << gpsWeek << std::fixed << std::setw(12) << std::setprecision(5) << gpsSecondsOfWeek
       << std::setw(2) << (unsigned)cartNavData.gpsUtc << std::showpos << std::setw(12) << std::setprecision(2)
       << cartNavData.x << std::setw(12) << std::setprecision(2) << cartNavData.y << std::setw(12)
       << std::setprecision(2) << cartNavData.z << std::setw(12) << std::setprecision(5) << cartNavData.vx
       << std::setw(12) << std::setprecision(5) << cartNavData.vy << std::setw(12) << std::setprecision(5)
       << cartNavData.vz << std::noshowpos << std::setw(1) << (unsigned)cartNavData.navigationStatus << std::setw(2)
       << (unsigned)cartNavData.nbOfTrackSats << std::setw(4) << std::setprecision(1) << cartNavData.pdop;

    Trace(1, "F40: %s", ss.str());

    std::string response = ss.str();
    ::std::vector<::Smp::UInt8> responseData(response.begin(), response.end());
    SendMessage(40, responseData);
}

void GpsReceiver::SendF52()
{
    ::Smp::UInt16 gpsWeek = 0;
    ::Smp::Float64 gpsSecondsOfWeek = 0.0;

    Generic::Utils::GetGpsTime(getCurrentDateTime(), gpsWeek, gpsSecondsOfWeek);

    std::stringstream ss;
    ss << "F52" << std::setw(4) << gpsWeek << std::fixed << std::setw(11) << std::setprecision(3) << gpsSecondsOfWeek
       << std::setw(11) << std::setprecision(4) << sgp4.semiMajorAxis << std::setw(11) << std::setprecision(8)
       << sgp4.eccentricity << std::setw(9) << std::setprecision(4) << sgp4.inclination << std::setw(10)
       << std::setprecision(4) << sgp4.rightAscension << std::setw(10) << sgp4.argumentOfPerigee << std::setw(10)
       << sgp4.meanAnomaly << std::setw(12) << std::setprecision(8) << sgp4.ballisticCoefficient;

    Trace(1, "F52: %s", ss.str());

    std::string response = ss.str();
    ::std::vector<::Smp::UInt8> responseData(response.begin(), response.end());
    SendMessage(52, responseData);
}

void GpsReceiver::SendF98(std::string messageType, std::string commandID, std::string text)
{
    ::Smp::UInt16 gpsWeek = 0;
    ::Smp::Float64 gpsSecondsOfWeek = 0.0;

    Generic::Utils::GetGpsTime(getCurrentDateTime(), gpsWeek, gpsSecondsOfWeek);

    std::stringstream ss;
    ss << "F98" << std::setw(4) << gpsWeek << std::fixed << std::setw(8) << std::setprecision(1) << gpsSecondsOfWeek
       << "-" << messageType << "-" << commandID << "-" << text << "-";

    std::string response = ss.str();
    ::std::vector<::Smp::UInt8> responseData(response.begin(), response.end());

    // Checksum
    ::Smp::UInt8 checksum = 0;
    for(auto byte : responseData)
    {
        checksum ^= byte;
    }
    responseData.push_back(checksum);

    ss.setf(std::ios_base::uppercase);
    ss << std::hex << std::setw(2) << std::setfill('0') << (unsigned)checksum;
    Trace(1, "F98: %s", ss.str());

    SendMessage(98, responseData);
}

Smp::DateTime GpsReceiver::getCurrentDateTime()
{
    if(realtime)
    {
        return zuluTimeOffset + simulator->GetTimeKeeper()->GetZuluTime();
    }
    else
    {
        return simulator->GetTimeKeeper()->GetEpochTime();
    }
}

void GpsReceiver::ReceiveMessage(Smp::UInt8 id, const std::vector<::Smp::UInt8>& data)
{
    CommandsBuffer.AddCommand(simulator->GetTimeKeeper()->GetSimulationTime(), id, 0, data, 1);
}

void GpsReceiver::SendMessage(Smp::UInt8 id, const std::vector<::Smp::UInt8>& data)
{
    sendBuffer.push_back({id, data});
    if(sendMessageEventId == 0)
    {
        sendMessageEventId = simulator->GetScheduler()->AddSimulationTimeEvent(SendBufferMessage, getMessageDelay(id));
    }
}

Smp::Duration GpsReceiver::getMessageDelay(Smp::UInt8 id)
{
    switch(id)
    {
    case 14:
        return 50000000;
    default:
        return 25000000;
    }
}

// MARKER: EXTRA METHOD DEFINITIONS: END
// --CLOSING ELEMENT--::Gps::GpsReceiver/GpsReceiver::Extra_Methods--

} // end namespace Gps
