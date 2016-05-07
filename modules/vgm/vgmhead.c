/*
 * VGM Data and Playback Driver: VGM Playback Head
 *
 * ==========================================================================
 *
 *  Copyright (C) 2016 Sauraen (sauraen@gmail.com)
 *  Licensed for personal non-commercial use only.
 *  All other rights reserved.
 * 
 * ==========================================================================
 */


#include "vgmhead.h"
#include "vgmqueue.h"
#include "vgmram.h"
#include "vgmstream.h"

VgmHead* vgm_heads[VGM_HEAD_MAXNUM];
u32 vgm_numheads;

void VGM_Head_Init(){
    vgm_numheads = 0;
}

VgmHead* VGM_Head_Create(VgmSource* source){
    if(source == NULL) return NULL;
    VgmHead* head = malloc(sizeof(VgmHead));
    head->playing = 0;
    head->source = source;
    head->ticks = 0; //will get changed at restart
    u8 i;
    for(i=0; i<12; ++i){
        head->channel[i].mute = 0;
        head->channel[i].map_chip = 0;
        if(i>=1 && i<=6){
            head->channel[i].map_voice = i-1;
        }else if(i>=8 && i<=0xA){
            head->channel[i].map_voice = i-8;
        }else{
            head->channel[i].map_voice = 0;
        }
    }
    head->opn2mult = ((7670454 << 8) / 500000); //0x1000; //TODO adjust in real time
    head->psgmult = 0x1000; //TODO adjust in real time
    head->psgfreq0to1 = 1;
    head->tempomult = 0x1000;
    head->iswait = 1;
    head->iswrite = 0;
    head->isdone = 0;
    if(source->type == VGM_SOURCE_TYPE_RAM){
        head->data = VGM_HeadRAM_Create(source);
    }else if(source->type == VGM_SOURCE_TYPE_STREAM){
        head->data = VGM_HeadStream_Create(source);
    }else if(source->type == VGM_SOURCE_TYPE_QUEUE){
        head->data = VGM_HeadQueue_Create(source);
    }
    MIOS32_IRQ_Disable();
    if(vgm_numheads == VGM_HEAD_MAXNUM){
        MIOS32_IRQ_Enable();
        free(head);
        return NULL;
    }
    vgm_heads[vgm_numheads] = head;
    ++vgm_numheads;
    MIOS32_IRQ_Enable();
    return head;
}
s32 VGM_Head_Delete(VgmHead* head){
    if(head == NULL) return -1;
    s32 ret = -1;
    MIOS32_IRQ_Disable();
    u32 i;
    for(i=0; i<vgm_numheads; ++i){
        if(vgm_heads[i] == head){
            for(; i<vgm_numheads-1; ++i){
                vgm_heads[i] = vgm_heads[i+1];
            }
            vgm_heads[i] = NULL;
            --vgm_numheads;
            ret = 0;
            break;
        }
    }
    MIOS32_IRQ_Enable();
    if(head->source->type == VGM_SOURCE_TYPE_RAM){
        VGM_HeadRAM_Delete(head->data);
    }else if(head->source->type == VGM_SOURCE_TYPE_STREAM){
        VGM_HeadStream_Delete(head->data);
    }else if(head->source->type == VGM_SOURCE_TYPE_QUEUE){
        VGM_HeadQueue_Delete(head->data);
    }
    free(head);
    return ret;
}

void VGM_Head_Restart(VgmHead* head, u32 vgm_time){
    if(head == NULL) return;
    head->ticks = vgm_time;
    if(head->source->type == VGM_SOURCE_TYPE_RAM){
        VGM_HeadRAM_Restart(head);
        VGM_HeadRAM_cmdNext(head, vgm_time);
    }else if(head->source->type == VGM_SOURCE_TYPE_STREAM){
        VGM_HeadStream_Restart(head);
        VGM_HeadStream_cmdNext(head, vgm_time);
    }else if(head->source->type == VGM_SOURCE_TYPE_QUEUE){
        VGM_HeadQueue_Restart(head);
        VGM_HeadQueue_cmdNext(head, vgm_time);
    }
}
void VGM_Head_cmdNext(VgmHead* head, u32 vgm_time){
    if(head == NULL) return;
    if(head->source->type == VGM_SOURCE_TYPE_RAM){
        VGM_HeadRAM_cmdNext(head, vgm_time);
    }else if(head->source->type == VGM_SOURCE_TYPE_STREAM){
        VGM_HeadStream_cmdNext(head, vgm_time);
    }else if(head->source->type == VGM_SOURCE_TYPE_QUEUE){
        VGM_HeadQueue_cmdNext(head, vgm_time);
    }
}

void VGM_Head_fixCmd(VgmHead* head, VgmChipWriteCmd* cmd){
    u8 chan, board;
    u8 addrhi = (cmd->cmd & 0x01);
    if(cmd->cmd == 0x00){
        //PSG write command
        if(cmd->data & 0x80){
            chan = ((cmd->data & 0x60) >> 5);
            head->psglastchannel = chan;
            chan += 8;
            if(head->channel[chan].mute) { cmd->cmd = 0xFF; return; }
            board = head->channel[chan].map_chip;
            if(chan != 0xB){
                chan = head->channel[chan].map_voice;
                cmd->data = (cmd->data & 0x9F) | (chan << 5);
            }
        }else{
            chan = head->psglastchannel + 8;
            if(head->channel[chan].mute) { cmd->cmd = 0xFF; return; }
            board = head->channel[chan].map_chip;
        }
    }else{
        //OPN2 write command
        if(cmd->addr == 0x28){
            //Key on
            chan = cmd->data & 0x07;
            if(chan < 0x03) chan += 1; //Move channels 0,1,2 up to 1,2,3
            if(head->channel[chan].mute) { cmd->cmd = 0xFF; return; }
            board = head->channel[chan].map_chip;
            chan = head->channel[chan].map_voice;
            if(chan >= 0x03) chan += 1; //Move 3,4,5 up to 4,5,6
            cmd->data = (cmd->data & 0xF8) | chan;
        }else if(cmd->addr == 0x2A || cmd->addr == 0x2B){
            //DAC write
            if(head->channel[7].mute) { cmd->cmd = 0xFF; return; }
            board = head->channel[7].map_chip;
        }else if((cmd->addr <= 0xAE && cmd->addr >= 0xA8) || (cmd->addr <= 0x27 && cmd->addr >= 0x24)){
            //Ch3 frequency write or options/timers write
            if(head->channel[3].mute) { cmd->cmd = 0xFF; return; }
            board = head->channel[3].map_chip;
        }else if(cmd->addr <= 0x2F || cmd->addr >= 0xB8){
            //Other chip write
            if(head->channel[0].mute) { cmd->cmd = 0xFF; return; }
            board = head->channel[0].map_chip;
        }else{
            //Operator or channel/voice write
            chan = (cmd->addr & 0x03) + addrhi + (addrhi << 1) + 1; //Add 3 for channels 3,4,5
            if(head->channel[chan].mute) { cmd->cmd = 0xFF; return; }
            board = head->channel[chan].map_chip;
            chan = head->channel[chan].map_voice;
            if(chan >= 0x03) chan += 1; //Move 3,4,5 up to 4,5,6
            addrhi = (chan & 0x04) >> 2;
            chan &= 0x03;
            cmd->addr = (cmd->addr & 0xFC) | chan;
            cmd->cmd = (cmd->cmd & 0xFE) | addrhi;
        }
    }
    cmd->cmd |= board << 4;
}
