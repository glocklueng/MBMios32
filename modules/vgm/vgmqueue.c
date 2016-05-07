/*
 * VGM Data and Playback Driver: VGM Data Queue
 *
 * ==========================================================================
 *
 *  Copyright (C) 2016 Sauraen (sauraen@gmail.com)
 *  Licensed for personal non-commercial use only.
 *  All other rights reserved.
 * 
 * ==========================================================================
 */


#include "vgmqueue.h"
#include "vgmtuning.h"

VgmHeadQueue* VGM_HeadQueue_Create(VgmSource* source){
    VgmHeadQueue* vhq = malloc(sizeof(VgmHeadQueue));
    vhq->busy = 0;
    vhq->start = 0;
    vhq->depth = 0;
    return vhq;
}
void VGM_HeadQueue_Delete(void* headqueue){
    VgmHeadQueue* vhq = (VgmHeadQueue*)headqueue;
    free(vhq);
}
void VGM_HeadQueue_Restart(VgmHead* head){
    //do nothing, there's no concept of this
}
void VGM_HeadQueue_cmdNext(VgmHead* head, u32 vgm_time){
    VgmHeadQueue* vhq = (VgmHeadQueue*)head->data;
    if(vhq->busy){
        //We interrupted while a command was being written to the queue
        //Make it try again after the minimal time
        head->iswait = 1;
        head->iswrite = 0;
        head->ticks = vgm_time;
    }else if(vhq->depth == 0){
        //No commands, come back later
        head->iswait = 1;
        head->iswrite = 0;
        head->ticks = vgm_time + 10;
    }else{
        //Command ready
        head->iswait = 0;
        head->iswrite = 1;
        head->writecmd = vhq->queue[vhq->start];
        ++vhq->start;
        if(vhq->start == VGM_QUEUE_LENGTH) vhq->start = 0;
        --vhq->depth;
    }
}

void VGM_HeadQueue_Enqueue(VgmHead* head, VgmChipWriteCmd cmd, u8 fixfreq){
    VgmHeadQueue* vhq = (VgmHeadQueue*)head->data;
    vhq->busy = 1;
    if(vhq->depth == VGM_QUEUE_LENGTH){
        vhq->busy = 0;
        return; //drop the command
    }
    if((cmd.cmd & 0x0F) == 0){
        //PSG write
        if((cmd.data & 0x80) && !(cmd.data & 0x10) && (cmd.data < 0xE0)){
            //First byte write, not attenuation, and not noise channel
            //We will make a 2-write command
            if(vhq->depth == VGM_QUEUE_LENGTH-1){
                vhq->busy = 0;
                return; //drop the command
            }
            //Fix PSG frequency
            if(fixfreq) VGM_fixPSGFrequency(&cmd, head->psgmult, head->psgfreq0to1);
            //Create second write command
            VgmChipWriteCmd cmd2;
            cmd2.cmd = cmd.cmd;
            cmd2.data = cmd.data2;
            //Add both write commands
            vhq->queue[(vhq->start + vhq->depth) % VGM_QUEUE_LENGTH] = cmd;
            vhq->queue[(vhq->start + vhq->depth+1) % VGM_QUEUE_LENGTH] = cmd2;
            vhq->depth += 2;
            vhq->busy = 0;
            return;
        }
    }else if((cmd.cmd & 0x0E) == 2){
        //OPN2 write
        if((cmd.addr & 0xF4) == 0xA4){
            //Frequency write
            //We will make a 2-write command
            if(vhq->depth == VGM_QUEUE_LENGTH-1){
                vhq->busy = 0;
                return; //drop the command
            }
            //Fix OPN2 frequency
            if(fixfreq) VGM_fixOPN2Frequency(&cmd, head->opn2mult);
            //Create second write command
            VgmChipWriteCmd cmd2;
            cmd2.cmd = cmd.cmd;
            cmd2.addr = cmd.addr & 0xFB; //A4 -> A0
            cmd2.data = cmd.data2;
            //Add both write commands
            vhq->queue[(vhq->start + vhq->depth) % VGM_QUEUE_LENGTH] = cmd;
            vhq->queue[(vhq->start + vhq->depth+1) % VGM_QUEUE_LENGTH] = cmd2;
            vhq->depth += 2;
            vhq->busy = 0;
            return;
        }
    }
    //Otherwise, regular write
    vhq->queue[(vhq->start + vhq->depth) % VGM_QUEUE_LENGTH] = cmd;
    ++vhq->depth;
    vhq->busy = 0;
}

VgmSource* VGM_SourceQueue_Create(){
    VgmSource* source = malloc(sizeof(VgmSource));
    source->type = VGM_SOURCE_TYPE_QUEUE;
    source->opn2clock = 0;
    source->psgclock = 0;
    source->loopaddr = 0;
    source->loopsamples = 0xFFFFFFFF;
    source->data = NULL;
    return source;
}
void VGM_SourceQueue_Delete(void* sourcequeue){
    //do nothing, there's nothing allocated
}
