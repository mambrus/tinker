// FPGA device driver
// mtrojer@arrownordic.com
// $WCREV$
//! Functions for sending commands and data to the Bitfire FPGA. This is controlled by GPIO P0.12 and P0.13.
//! Defines FGPACMD_* constants.
//! \page AEB01_FPGA aeb01_fpga.h 
//! \ingroup BSPINC

#ifndef _AEB01_FPGA_H
#define _AEB01_FPGA_H

#define FPGACMD_shdn_ledsup_1  0x00
#define FPGACMD_shdn_ledsup_0  0x01
#define FPGACMD_shdnn_muxsup_0 0x02
#define FPGACMD_shdnn_muxsup_1 0x03
#define FPGACMD_lbn_rsx_1      0x04
#define FPGACMD_lbn_rsx_0      0x05
#define FPGACMD_on_rsx_1       0x06
#define FPGACMD_on_rsx_0       0x07
#define FPGACMD_sel1_rsx_0     0x08
#define FPGACMD_sel1_rsx_1     0x09
#define FPGACMD_sel2_rsx_0     0x0A
#define FPGACMD_sel2_rsx_1     0x0B
#define FPGACMD_DIMMER_PRE0    0x10
#define FPGACMD_DIMMER_PRE1    0x11
#define FPGACMD_DIMMER_PRE2    0x12
#define FPGACMD_DIMMER_PRE3    0x13
#define FPGACMD_DIMMER_PRE4    0x14
#define FPGACMD_DIMMER_PRE5    0x15
#define FPGACMD_SHOW_BUF_0     0x20
#define FPGACMD_SHOW_BUF_1     0x21
#define FPGACMD_WORK_BUF_0     0x22
#define FPGACMD_WORK_BUF_1     0x23

//! \defgroup aeb01_fpga aeb01_fpga

//! Initializes the GPIO ports that are needed to control the FPGA.
//! \ingroup aeb01_fpga
void fpga_init(void);

//! Setup the FPGA for command communication. Must be called once before each command.
//! \ingroup aeb01_fpga
void fpga_command(void);

//! Setup the FPGA for data communication.
//! \ingroup aeb01_fpga
void fpga_data(void);

#endif
