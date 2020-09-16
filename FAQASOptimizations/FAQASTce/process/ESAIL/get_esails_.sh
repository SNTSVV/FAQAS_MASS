#!/bin/bash

declare sources=( "./HighLevelDriverLayer/TMTC_SYS_Handler/Source/tmtchdl_TmTcSysHandlerTask.c" "./ProtocolLayer/TCFrameReader/Source/TCFrameReader.c" "./ProtocolLayer/TMFrameBuilder/Source/TMFrameBuilder.c" "./ServiceLayer/PUS_1/Source/pus1_GenerateReport.c" "./ServiceLayer/PUS_3/Source/pus3_GenerateHkGroupReport.c" "./ServiceLayer/PUS_3/Source/pus3_ExecuteTc_3_129.c" "./ServiceLayer/PUS_128/Source/pus128_ExecuteTc_128_1.c" "./ServiceLayer/PUS_130/Source/pus130_ExecuteTc_130_1.c" "./ApplicationLayer/Operational_Sequences/Source/SpacecraftConfigurationVector.c" "./ApplicationLayer/Operational_Sequences/Source/Transition_To_OPM.c" ) 

while read line; do
    # Do what you want to $name
    location=$(echo $line | awk -F'|' '{print $2}')
    src=$(echo $line | awk -F'|' '{print $1}' | awk -F'.' '{print $1}')
    value=${location}/${src}.c
    
    if [[ " ${sources[@]} " =~ " ${value} " ]]; then
    # whatever you want to do when array contains value
        echo $line >> esail_s
    fi

done < all
