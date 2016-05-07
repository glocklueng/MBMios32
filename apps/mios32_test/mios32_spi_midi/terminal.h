// $Id: terminal.h 2032 2014-08-17 22:27:36Z tk $
/*
 * The command/configuration Terminal
 *
 * ==========================================================================
 *
 *  Copyright (C) 2010 Thorsten Klose (tk@midibox.org)
 *  Licensed for personal non-commercial use only.
 *  All other rights reserved.
 * 
 * ==========================================================================
 */

#ifndef _TERMINAL_H
#define _TERMINAL_H


/////////////////////////////////////////////////////////////////////////////
// Global definitions
/////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////
// Global Types
/////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////
// Prototypes
/////////////////////////////////////////////////////////////////////////////

extern s32 TERMINAL_Init(u32 mode);
extern s32 TERMINAL_Parse(mios32_midi_port_t port, char byte);
extern s32 TERMINAL_ParseLine(char *input, void *_output_function);

extern s32 TERMINAL_KissboxSendMsg(char *cmd);
extern s32 TERMINAL_KissboxReceiveMsgPackage(mios32_midi_package_t p);


/////////////////////////////////////////////////////////////////////////////
// Export global variables
/////////////////////////////////////////////////////////////////////////////


#endif /* _TERMINAL_H */
