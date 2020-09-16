#!/bin/bash

declare sources=( "./HighLevelDriverLayer/CAN_Handler/Source/can_outputHandlerTask.c" "./HighLevelDriverLayer/TMTC_SYS_Handler/Source/tmtchdl_TmTcSysHandlerTask.c" "./HighLevelDriverLayer/CAN_Handler/Source/can_inputHandlerTask.c" "./ProtocolLayer/CANDispatcher/Source/process_PDO.c" "./ProtocolLayer/CANDispatcher/Source/CANDispatcher.c" "./ProtocolLayer/TCFrameReader/Source/TCFrameReader.c" "./ProtocolLayer/TMFrameBuilder/Source/TMFrameBuilder.c" "./ProtocolLayer/CANIO/Source/CANIO_PDO.c" "./ServiceLayer/PUS_1/Source/pus1_GenerateReport.c" "./ServiceLayer/PUS_3/Source/pus3_GenerateHkGroupReport.c" "./ServiceLayer/PUS_3/Source/pus3_ExecuteTc_3_129.c" "./ServiceLayer/PUS_128/Source/pus128_ExecuteTc_128_1.c" "./ServiceLayer/PUS_130/Source/pus130_ExecuteTc_130_1.c" "./ApplicationLayer/Operational_Sequences/Source/SequencesExecutionTask.c" "./ApplicationLayer/Operational_Sequences/Source/SpacecraftConfigurationVector.c" "./ApplicationLayer/Operational_Sequences/Source/Transition_To_OPM.c" "./ApplicationLayer/Operational_Sequences/Source/Transition_To_SCM.c" "./ApplicationLayer/SYS_FDIR/Source/RecoveryManager_ProcessingTask.c" )

while read line; do
    location=$(echo $line | awk -F'|' '{print $2}')
    src=$(echo $line | awk -F'|' '{print $1}' | awk -F'.' '{print $1}')
    value=${location}/${src}.c
    
    if [[ " ${sources[@]} " =~ " ${value} " ]]; then
    # whatever you want to do when array contains value
        echo $line >> esail_yago
    fi

done < all
