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

#include "PdhuPdoService.h"
#include "Generic/CanPdoService.h"

#include "Mdk/SimpleTypes.h"
#include "Pdhu/Pdhu.h"
#include "Smp/Publication/IClassType.h"
#include "Smp/Publication/IEnumerationType.h"
#include "Smp/Publication/IPublishOperation.h"
#include "Smp/Publication/IStructureType.h"
#include "Smp/Publication/IType.h"
#include "Smp/SimpleTypes.h"
#include "vector"

// --OPENING ELEMENT--::Pdhu::PdhuPdoService/PdhuPdoService::Extra_Includes--
// MARKER: FILE INCLUSION: START
// MARKER: FILE INCLUSION: END

// MARKER: FORWARD DECLARATIONS: START
#include "Generic/Utils.h"
#include "OhbCommon/ByteUtils.h"
// MARKER: FORWARD DECLARATIONS: END

// -----------------------------------------------------------------------------
// ------------------------ Custom field definitions ---------------------------
// -----------------------------------------------------------------------------
// MARKER: EXTRA FIELD DEFINITIONS: START
// MARKER: EXTRA FIELD DEFINITIONS: END
// --CLOSING ELEMENT--::Pdhu::PdhuPdoService/PdhuPdoService::Extra_Includes--

namespace Pdhu
{
// --OPENING ELEMENT--PdhuPdoService::IndicationReceived--
/// Indication received by the service.
/// @param functionCode The CAN open function code.
/// @param data The data.
::Smp::Bool PdhuPdoService::IndicationReceived(::Smp::UInt8 functionCode, const std::vector<Smp::UInt8>& data)
{
    // MARKER: OPERATION BODY: START
    Trace(5, "PdhuPdoService::IndicationReceived");

    if(!CanPdoService::IndicationReceived(functionCode, data))
    {
        return false;
    }

    if(data.size() != 8)
    {
        Log(Smp::Services::LMK_Warning, "Data size %u != 8", data.size());

        std::vector<Smp::UInt8> newData(8);

        if(data.size() > 0)
        {
            newData[0] = data[0];
        }

        newData[1] = 0x02;
        responseCb(functionCode - 1, newData);

        return true;
    }

    Smp::UInt8 commandId = data[0];
    std::vector<Smp::UInt8> newData(8);
    newData[0] = data[0];

    switch(commandId)
    {
    case 0x01:
    {
        ::Smp::UInt64 sequenceId;
        ::Smp::UInt16 nbOfPages;

        Generic::Utils::DeSerializeLe(sequenceId, data.begin() + 1, 5);
        Generic::Utils::DeSerializeLe(nbOfPages, data.begin() + 6);

        Trace(5, "data = 0x%s", OhbCommon::ByteUtils::BinToHex(data));

        auto statusCode = Pdhu->StorageReadBack->ReadFrames(sequenceId, nbOfPages);

        if(statusCode == RCSC_CmdOk)
        {
            Generic::Utils::SerializeLe((::Smp::UInt32)Pdhu->StorageReadBack->pendingRequests, newData.begin() + 2);
        }
        else
        {
            newData[1] = statusCode;
        }
    }
    break;
    case 0x02:
    {
        ::Smp::UInt64 sequenceId;

        Generic::Utils::DeSerializeLe(sequenceId, data.begin() + 1, 5);

        auto statusCode = Pdhu->StorageReadBack->AckFrames(sequenceId);

        if(statusCode == RCSC_CmdOk)
        {
            Generic::Utils::SerializeLe((::Smp::UInt32)Pdhu->StorageReadBack->pendingRequests, newData.begin() + 2);
        }
        else
        {
            newData[1] = statusCode;
        }
    }
    break;
    case 0x03:
    {
        auto statusCode = Pdhu->StorageReadBack->AckAllFrames(data[1]);

        newData[1] = statusCode;
    }
    break;
    case 0x04:
    {
        ::Smp::UInt64 sequenceId;
        auto statusCode = Pdhu->StorageReadBack->GetHeadPointer(data[1], sequenceId);

        if(statusCode == RCSC_CmdOk)
        {
            Generic::Utils::SerializeLe(sequenceId, newData.begin() + 2, 5);
        }
        else
        {
            newData[1] = statusCode;
        }
    }
    break;
    case 0x05:
    {
        ::Smp::UInt64 sequenceId;
        auto statusCode = Pdhu->StorageReadBack->GetTailPointer(data[1], sequenceId);

        if(statusCode == RCSC_CmdOk)
        {
            Generic::Utils::SerializeLe(sequenceId, newData.begin() + 2, 5);
        }
        else
        {
            newData[1] = statusCode;
        }
    }
    break;
    case 0x13:
    {
        ::Smp::UInt16 blockId;
        Generic::Utils::DeSerializeLe(blockId, data.begin() + 1);

        auto statusCode = Pdhu->QueueStorage->SetCb0Size(blockId);

        newData[1] = statusCode;
    }
    break;
    case 0x1C:
    {
        ::Smp::UInt8 writeResult = data[1];

        ::Smp::UInt16 ccittCrc;

        auto statusCode = Pdhu->ComputeCcittCrc(writeResult > 0, ccittCrc);

        newData[1] = statusCode;

        Generic::Utils::SerializeLe(ccittCrc, newData.begin() + 2);
    }
    break;
    default:
        Log(Smp::Services::LMK_Warning, "Command with id 0x%02X not implemented", commandId);
        newData[0] = 0xFF;
        newData[1] = commandId;
        for(unsigned n = 0; n < 6; ++n)
        {
            newData[2 + n] = 0;
        }
    }

    responseCb(functionCode - 1, newData);

    return true;
    // MARKER: OPERATION BODY: END
}
// --CLOSING ELEMENT--PdhuPdoService::IndicationReceived--

// - - - - - - - - - - - - - - Publish - - - - - - - - - - - - - - - -
// The Publish method is implemented in the PdhuPdoServiceSmp.cpp file.

// --OPENING ELEMENT--PdhuPdoService::Configure--
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

void PdhuPdoService::Configure(::Smp::Services::ILogger* logger)
{
    // Call base implementation first
    ::Generic::CanPdoService::Configure(logger);

    // MARKER: CONFIGURE BODY: START
    // MARKER: CONFIGURE BODY: END
}
// --CLOSING ELEMENT--PdhuPdoService::Configure--

// --OPENING ELEMENT--PdhuPdoService::Connect--
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

void PdhuPdoService::Connect(::Smp::ISimulator* simulator)
{
    // Call Mdk implementation first
    ::Generic::CanPdoService::Connect(simulator);

    // MARKER: CONNECT BODY: START
    // MARKER: CONNECT BODY: END
}
// --CLOSING ELEMENT--PdhuPdoService::Connect--

// -----------------------------------------------------------------------------
// ------------------------ Custom method definitions --------------------------
// -----------------------------------------------------------------------------
// --OPENING ELEMENT--::Pdhu::PdhuPdoService/PdhuPdoService::Extra_Methods--
// MARKER: EXTRA METHOD DEFINITIONS: START
void PdhuPdoService::GetErrorResponse(const std::vector<Smp::UInt8>& inputData, ::Smp::UInt8 errorCode, std::vector<Smp::UInt8>& outputData)
{
    ;
}
// MARKER: EXTRA METHOD DEFINITIONS: END
// --CLOSING ELEMENT--::Pdhu::PdhuPdoService/PdhuPdoService::Extra_Methods--

} // end namespace Pdhu
