/***************************************************************************
 *
 *  hdreg.h
 *
 *    
 *
 *    Copyright (c) 2003 XiMeta, Inc. All rights reserved.
 *
 **************************************************************************/

#ifndef _HDREG_H
#define _HDREG_H

/*
 * This file contains some defines for the AT-hd-controller.
 * Various sources.  
 */

/* ide.c has its own port definitions in "ide.h" */

/* Hd controller regs. Ref: IBM AT Bios-listing */
#define HD_DATA     0x1f0       /* _CTL when writing */
#define HD_ERROR    0x1f1       /* see err-bits */
#define HD_NSECTOR  0x1f2       /* nr of sectors to read/write */
#define HD_SECTOR   0x1f3       /* starting sector */
#define HD_LCYL     0x1f4       /* starting cylinder */
#define HD_HCYL     0x1f5       /* high byte of starting cyl */
#define HD_CURRENT  0x1f6       /* 101dhhhh , d=drive, hhhh=head */
#define HD_STATUS   0x1f7       /* see status-bits */
#define HD_FEATURE  HD_ERROR    /* same io address, read=error, write=feature */
#define HD_PRECOMP  HD_FEATURE  /* obsolete use of this port - predates IDE */
#define HD_COMMAND  HD_STATUS   /* same io address, read=status, write=cmd */

#define HD_CMD      0x3f6       /* used for resets */
#define HD_ALTSTATUS    0x3f6       /* same as HD_STATUS but doesn't clear irq */

/* remainder is shared between hd.c, ide.c, ide-cd.c, and the hdparm utility */

/* Bits of HD_STATUS */
#define ERR_STAT        0x01
#define INDEX_STAT      0x02
#define ECC_STAT        0x04    /* Corrected error */
#define DRQ_STAT        0x08
#define SEEK_STAT       0x10
#define WRERR_STAT      0x20
#define READY_STAT      0x40
#define BUSY_STAT       0x80

/* Bits for HD_ERROR */
#define MARK_ERR        0x01    /* Bad address mark */
#define TRK0_ERR        0x02    /* couldn't find track 0 */
#define ABRT_ERR        0x04    /* Command aborted */
#define MCR_ERR         0x08    /* media change request */
#define ID_ERR          0x10    /* ID field not found */
#define MC_ERR          0x20    /* media changed */
#define ECC_ERR         0x40    /* Uncorrectable ECC error */
#define BBD_ERR         0x80    /* pre-EIDE meaning:  block marked bad */
#define ICRC_ERR        0x80    /* new meaning:  CRC error during transfer */

/*
 * Command Header sizes for IOCTL commands
 *  HDIO_DRIVE_CMD, HDIO_DRIVE_TASK, and HDIO_DRIVE_TASKFILE
 */

#if 0
#include <asm/hdreg.h>
typedef ide_ioreg_t task_ioreg_t;
#else
typedef uint8_t task_ioreg_t;
#endif


/* ATA/ATAPI Commands pre T13 Spec */
#define WIN_NOP             0x00
/*
 *  0x01->0x02 Reserved
 */
#define CFA_REQ_EXT_ERROR_CODE      0x03 /* CFA Request Extended Error Code */
/*
 *  0x04->0x07 Reserved
 */
#define WIN_SRST            0x08 /* ATAPI soft reset command */
#define WIN_DEVICE_RESET        0x08
/*
 *  0x09->0x0F Reserved
 */
#define WIN_RECAL           0x10
/*
 *  0x10->0x1F Reserved
 */
#define WIN_RESTORE         WIN_RECAL
#define WIN_READ            0x20 /* 28-Bit */
#define WIN_READ_ONCE           0x21 /* 28-Bit without retries */
#define WIN_READ_LONG           0x22 /* 28-Bit */
#define WIN_READ_LONG_ONCE      0x23 /* 28-Bit without retries */
#define WIN_READ_EXT            0x24 /* 48-Bit */
#define WIN_READDMA_EXT         0x25 /* 48-Bit */
#define WIN_READDMA_QUEUED_EXT      0x26 /* 48-Bit */
#define WIN_READ_NATIVE_MAX_EXT     0x27 /* 48-Bit */
#define WIN_MULTREAD_EXT        0x29 /* 48-Bit */
/*
 *  0x2A->0x2F Reserved
 */
#define WIN_WRITE           0x30 /* 28-Bit */
#define WIN_WRITE_ONCE          0x31 /* 28-Bit without retries */
#define WIN_WRITE_LONG          0x32 /* 28-Bit */
#define WIN_WRITE_LONG_ONCE     0x33 /* 28-Bit without retries */
#define WIN_WRITE_EXT           0x34 /* 48-Bit */
#define WIN_WRITEDMA_EXT        0x35 /* 48-Bit */
#define WIN_WRITEDMA_QUEUED_EXT     0x36 /* 48-Bit */
#define WIN_SET_MAX_EXT         0x37 /* 48-Bit */
#define CFA_WRITE_SECT_WO_ERASE     0x38 /* CFA Write Sectors without erase */
#define WIN_MULTWRITE_EXT       0x39 /* 48-Bit */
/*
 *  0x3A->0x3B Reserved
 */
#define WIN_WRITE_VERIFY        0x3C /* 28-Bit */
/*
 *  0x3D->0x3F Reserved
 */
#define WIN_VERIFY          0x40 /* 28-Bit - Read Verify Sectors */
#define WIN_VERIFY_ONCE         0x41 /* 28-Bit - without retries */
#define WIN_VERIFY_EXT          0x42 /* 48-Bit */
/*
 *  0x43->0x4F Reserved
 */
#define WIN_FORMAT          0x50
/*
 *  0x51->0x5F Reserved
 */
#define WIN_INIT            0x60
/*
 *  0x61->0x5F Reserved
 */
#define WIN_SEEK            0x70 /* 0x70-0x7F Reserved */
#define CFA_TRANSLATE_SECTOR        0x87 /* CFA Translate Sector */
#define WIN_DIAGNOSE            0x90
#define WIN_SPECIFY         0x91 /* set drive geometry translation */
#define WIN_DOWNLOAD_MICROCODE      0x92
#define WIN_STANDBYNOW2         0x94
#define WIN_SETIDLE2            0x97
#define WIN_CHECKPOWERMODE2     0x98
#define WIN_SLEEPNOW2           0x99
/*
 *  0x9A VENDOR
 */
#define WIN_PACKETCMD           0xA0 /* Send a packet command. */
#define WIN_PIDENTIFY           0xA1 /* identify ATAPI device   */
#define WIN_QUEUED_SERVICE      0xA2
#define WIN_SMART           0xB0 /* self-monitoring and reporting */
#define CFA_ERASE_SECTORS           0xC0
#define WIN_MULTREAD            0xC4 /* read sectors using multiple mode*/
#define WIN_MULTWRITE           0xC5 /* write sectors using multiple mode */
#define WIN_SETMULT         0xC6 /* enable/disable multiple mode */
#define WIN_READDMA_QUEUED      0xC7 /* read sectors using Queued DMA transfers */
#define WIN_READDMA         0xC8 /* read sectors using DMA transfers */
#define WIN_READDMA_ONCE        0xC9 /* 28-Bit - without retries */
#define WIN_WRITEDMA            0xCA /* write sectors using DMA transfers */
#define WIN_WRITEDMA_ONCE       0xCB /* 28-Bit - without retries */
#define WIN_WRITEDMA_QUEUED     0xCC /* write sectors using Queued DMA transfers */
#define CFA_WRITE_MULTI_WO_ERASE    0xCD /* CFA Write multiple without erase */
#define WIN_GETMEDIASTATUS      0xDA    
#define WIN_DOORLOCK            0xDE /* lock door on removable drives */
#define WIN_DOORUNLOCK          0xDF /* unlock door on removable drives */
#define WIN_STANDBYNOW1         0xE0
#define WIN_IDLEIMMEDIATE       0xE1 /* force drive to become "ready" */
#define WIN_STANDBY                 0xE2 /* Set device in Standby Mode */
#define WIN_SETIDLE1            0xE3
#define WIN_READ_BUFFER         0xE4 /* force read only 1 sector */
#define WIN_CHECKPOWERMODE1     0xE5
#define WIN_SLEEPNOW1           0xE6
#define WIN_FLUSH_CACHE         0xE7
#define WIN_WRITE_BUFFER        0xE8 /* force write only 1 sector */
#define WIN_WRITE_SAME          0xE9 /* read ata-2 to use */
#define WIN_FLUSH_CACHE_EXT     0xEA /* 48-Bit */
#define WIN_IDENTIFY            0xEC /* ask drive to identify itself    */
#define WIN_MEDIAEJECT          0xED
#define WIN_IDENTIFY_DMA        0xEE /* same as WIN_IDENTIFY, but DMA */
#define WIN_SETFEATURES         0xEF /* set special drive features */
#define EXABYTE_ENABLE_NEST     0xF0
#define WIN_SECURITY_SET_PASS       0xF1
#define WIN_SECURITY_UNLOCK     0xF2
#define WIN_SECURITY_ERASE_PREPARE  0xF3
#define WIN_SECURITY_ERASE_UNIT     0xF4
#define WIN_SECURITY_FREEZE_LOCK    0xF5
#define WIN_SECURITY_DISABLE        0xF6
#define WIN_READ_NATIVE_MAX     0xF8 /* return the native maximum address */
#define WIN_SET_MAX         0xF9
#define DISABLE_SEAGATE         0xFB

/* WIN_SMART sub-commands */

#define SMART_READ_VALUES       0xD0
#define SMART_READ_THRESHOLDS       0xD1
#define SMART_AUTOSAVE          0xD2
#define SMART_SAVE          0xD3
#define SMART_IMMEDIATE_OFFLINE     0xD4
#define SMART_READ_LOG_SECTOR       0xD5
#define SMART_WRITE_LOG_SECTOR      0xD6
#define SMART_WRITE_THRESHOLDS      0xD7
#define SMART_ENABLE            0xD8
#define SMART_DISABLE           0xD9
#define SMART_STATUS            0xDA
#define SMART_AUTO_OFFLINE      0xDB

/* Password used in TF4 & TF5 executing SMART commands */

#define SMART_LCYL_PASS         0x4F
#define SMART_HCYL_PASS         0xC2
        
/* WIN_SETFEATURES sub-commands */
#define SETFEATURES_EN_8BIT 0x01    /* Enable 8-Bit Transfers */
#define SETFEATURES_EN_WCACHE   0x02    /* Enable write cache */
#define SETFEATURES_XFER    0x03    /* Set transfer mode */
#   define XFER_UDMA_7  0x47    /* 0100|0111 */
#   define XFER_UDMA_6  0x46    /* 0100|0110 */
#   define XFER_UDMA_5  0x45    /* 0100|0101 */
#   define XFER_UDMA_4  0x44    /* 0100|0100 */
#   define XFER_UDMA_3  0x43    /* 0100|0011 */
#   define XFER_UDMA_2  0x42    /* 0100|0010 */
#   define XFER_UDMA_1  0x41    /* 0100|0001 */
#   define XFER_UDMA_0  0x40    /* 0100|0000 */
#   define XFER_MW_DMA_2    0x22    /* 0010|0010 */
#   define XFER_MW_DMA_1    0x21    /* 0010|0001 */
#   define XFER_MW_DMA_0    0x20    /* 0010|0000 */
#   define XFER_SW_DMA_2    0x12    /* 0001|0010 */
#   define XFER_SW_DMA_1    0x11    /* 0001|0001 */
#   define XFER_SW_DMA_0    0x10    /* 0001|0000 */
#   define XFER_PIO_4   0x0C    /* 0000|1100 */
#   define XFER_PIO_3   0x0B    /* 0000|1011 */
#   define XFER_PIO_2   0x0A    /* 0000|1010 */
#   define XFER_PIO_1   0x09    /* 0000|1001 */
#   define XFER_PIO_0   0x08    /* 0000|1000 */
#   define XFER_PIO_SLOW    0x00    /* 0000|0000 */
#define SETFEATURES_DIS_DEFECT  0x04    /* Disable Defect Management */
#define SETFEATURES_EN_APM  0x05    /* Enable advanced power management */
#define SETFEATURES_DIS_MSN 0x31    /* Disable Media Status Notification */
#define SETFEATURES_DIS_RETRY   0x33    /* Disable Retry */
#define SETFEATURES_EN_AAM  0x42    /* Enable Automatic Acoustic Management */
#define SETFEATURES_RW_LONG 0x44    /* Set Lenght of VS bytes */
#define SETFEATURES_SET_CACHE   0x54    /* Set Cache segments to SC Reg. Val */
#define SETFEATURES_DIS_RLA 0x55    /* Disable read look-ahead feature */
#define SETFEATURES_EN_RI   0x5D    /* Enable release interrupt */
#define SETFEATURES_EN_SI   0x5E    /* Enable SERVICE interrupt */
#define SETFEATURES_DIS_RPOD    0x66    /* Disable reverting to power on defaults */
#define SETFEATURES_DIS_ECC 0x77    /* Disable ECC byte count */
#define SETFEATURES_DIS_8BIT    0x81    /* Disable 8-Bit Transfers */
#define SETFEATURES_DIS_WCACHE  0x82    /* Disable write cache */
#define SETFEATURES_EN_DEFECT   0x84    /* Enable Defect Management */
#define SETFEATURES_DIS_APM 0x85    /* Disable advanced power management */
#define SETFEATURES_EN_ECC  0x88    /* Enable ECC byte count */
#define SETFEATURES_EN_MSN  0x95    /* Enable Media Status Notification */
#define SETFEATURES_EN_RETRY    0x99    /* Enable Retry */
#define SETFEATURES_EN_RLA  0xAA    /* Enable read look-ahead feature */
#define SETFEATURES_PREFETCH    0xAB    /* Sets drive prefetch value */
#define SETFEATURES_4B_RW_LONG  0xBB    /* Set Lenght of 4 bytes */
#define SETFEATURES_DIS_AAM 0xC2    /* Disable Automatic Acoustic Management */
#define SETFEATURES_EN_RPOD 0xCC    /* Enable reverting to power on defaults */
#define SETFEATURES_DIS_RI  0xDD    /* Disable release interrupt */
#define SETFEATURES_DIS_SI  0xDE    /* Disable SERVICE interrupt */

/* WIN_SECURITY sub-commands */

#define SECURITY_SET_PASSWORD       0xBA
#define SECURITY_UNLOCK         0xBB
#define SECURITY_ERASE_PREPARE      0xBC
#define SECURITY_ERASE_UNIT     0xBD
#define SECURITY_FREEZE_LOCK        0xBE
#define SECURITY_DISABLE_PASSWORD   0xBF

struct hd_geometry {
      uint8_t heads;
      uint8_t sectors;
      uint16_t cylinders;
      uint32_t start;
};

/* BIG GEOMETRY */
struct hd_big_geometry {
    uint8_t heads;
    uint8_t sectors;
    uint32_t cylinders;
    uint32_t start;
};

/* hd/ide ctl's that pass (arg) ptrs to user space are numbered 0x030n/0x031n */
#define HDIO_GETGEO     0x0301  /* get device geometry */
#define HDIO_GET_UNMASKINTR 0x0302  /* get current unmask setting */
#define HDIO_GET_MULTCOUNT  0x0304  /* get current IDE blockmode setting */
#define HDIO_GET_QDMA       0x0305  /* get use-qdma flag */
#define HDIO_OBSOLETE_IDENTITY  0x0307  /* OBSOLETE, DO NOT USE: returns 142 bytes */
#define HDIO_GET_KEEPSETTINGS   0x0308  /* get keep-settings-on-reset flag */
#define HDIO_GET_32BIT      0x0309  /* get current io_32bit setting */
#define HDIO_GET_NOWERR     0x030a  /* get ignore-write-error flag */
#define HDIO_GET_DMA        0x030b  /* get use-dma flag */
#define HDIO_GET_NICE       0x030c  /* get nice flags */
#define HDIO_GET_IDENTITY   0x030d  /* get IDE identification info */
#define HDIO_GET_WCACHE     0x030e  /* get write cache mode on|off */
#define HDIO_GET_ACOUSTIC   0x030f  /* get acoustic value */
#define HDIO_GET_ADDRESS    0x0310  /* */

#define HDIO_GET_BUSSTATE   0x031a  /* get the bus state of the hwif */
#define HDIO_TRISTATE_HWIF  0x031b  /* execute a channel tristate */
#define HDIO_DRIVE_RESET    0x031c  /* execute a device reset */
#define HDIO_DRIVE_TASKFILE 0x031d  /* execute raw taskfile */
#define HDIO_DRIVE_TASK     0x031e  /* execute task and special drive command */
#define HDIO_DRIVE_CMD      0x031f  /* execute a special drive command */

#define HDIO_DRIVE_CMD_AEB  HDIO_DRIVE_TASK

/* hd/ide ctl's that pass (arg) non-ptr values are numbered 0x032n/0x033n */
#define HDIO_SET_MULTCOUNT  0x0321  /* change IDE blockmode */
#define HDIO_SET_UNMASKINTR 0x0322  /* permit other irqs during I/O */
#define HDIO_SET_KEEPSETTINGS   0x0323  /* keep ioctl settings on reset */
#define HDIO_SET_32BIT      0x0324  /* change io_32bit flags */
#define HDIO_SET_NOWERR     0x0325  /* change ignore-write-error flag */
#define HDIO_SET_DMA        0x0326  /* change use-dma flag */
#define HDIO_SET_PIO_MODE   0x0327  /* reconfig interface to new speed */
#define HDIO_SCAN_HWIF      0x0328  /* register and (re)scan interface */
#define HDIO_SET_NICE       0x0329  /* set nice flags */
#define HDIO_UNREGISTER_HWIF    0x032a  /* unregister interface */
#define HDIO_SET_WCACHE     0x032b  /* change write cache enable-disable */
#define HDIO_SET_ACOUSTIC   0x032c  /* change acoustic behavior */
#define HDIO_SET_BUSSTATE   0x032d  /* set the bus state of the hwif */
#define HDIO_SET_QDMA       0x032e  /* change use-qdma flag */
#define HDIO_SET_ADDRESS    0x032f  /* change lba addressing modes */

/* bus states */
enum {
    BUSSTATE_OFF = 0,
    BUSSTATE_ON,
    BUSSTATE_TRISTATE
};

/* hd/ide ctl's that pass (arg) ptrs to user space are numbered 0x033n/0x033n */
#define HDIO_GETGEO_BIG     0x0330  /* */
#define HDIO_GETGEO_BIG_RAW 0x0331  /* */

#define __NEW_HD_DRIVE_ID
/* structure returned by HDIO_GET_IDENTITY,
 * as per ANSI NCITS ATA6 rev.1b spec
 */
struct hd_driveid {
    uint16_t  config;     /* lots of obsolete bit flags */
    uint16_t  cyls;       /* Obsolete, "physical" cyls */
    uint16_t  reserved2;  /* reserved (word 2) */
    uint16_t  heads;      /* Obsolete, "physical" heads */
    uint16_t  track_bytes;    /* unformatted bytes per track */
    uint16_t  sector_bytes;   /* unformatted bytes per sector */
    uint16_t  sectors;    /* Obsolete, "physical" sectors per track */
    uint16_t  vendor0;    /* vendor unique */
    uint16_t  vendor1;    /* vendor unique */
    uint16_t  vendor2;    /* Retired vendor unique */
    uint8_t   serial_no[20];  /* 0 = not_specified */
    uint16_t  buf_type;   /* Retired */
    uint16_t  buf_size;   /* Retired, 512 byte increments
                     * 0 = not_specified
                     */
    uint16_t  ecc_bytes;  /* for r/w long cmds; 0 = not_specified */
    uint8_t   fw_rev[8];  /* 0 = not_specified */
    uint8_t   model[40];  /* 0 = not_specified */
    uint8_t   max_multsect;   /* 0=not_implemented */
    uint8_t   vendor3;    /* vendor unique */
    uint16_t  dword_io;   /* 0=not_implemented; 1=implemented */
    uint8_t   vendor4;    /* vendor unique */
    uint8_t   capability; /* (upper byte of word 49)
                     *  3:  IORDYsup
                     *  2:  IORDYsw
                     *  1:  LBA
                     *  0:  DMA
                     */
    uint16_t  reserved50; /* reserved (word 50) */
    uint8_t   vendor5;    /* Obsolete, vendor unique */
    uint8_t   tPIO;       /* Obsolete, 0=slow, 1=medium, 2=fast */
    uint8_t   vendor6;    /* Obsolete, vendor unique */
    uint8_t   tDMA;       /* Obsolete, 0=slow, 1=medium, 2=fast */
    uint16_t  field_valid;    /* (word 53)
                     *  2:  ultra_ok    word  88
                     *  1:  eide_ok     words 64-70
                     *  0:  cur_ok      words 54-58
                     */
    uint16_t  cur_cyls;   /* Obsolete, logical cylinders */
    uint16_t  cur_heads;  /* Obsolete, l heads */
    uint16_t  cur_sectors;    /* Obsolete, l sectors per track */
    uint16_t  cur_capacity0;  /* Obsolete, l total sectors on drive */
    uint16_t  cur_capacity1;  /* Obsolete, (2 words, misaligned int)     */
    uint8_t   multsect;   /* current multiple sector count */
    uint8_t   multsect_valid; /* when (bit0==1) multsect is ok */
    uint32_t    lba_capacity;   /* Obsolete, total number of sectors */
    uint16_t  dma_1word;  /* Obsolete, single-word dma info */
    uint16_t  dma_mword;  /* multiple-word dma info */
    uint16_t  eide_pio_modes; /* bits 0:mode3 1:mode4 */
    uint16_t  eide_dma_min;   /* min mword dma cycle time (ns) */
    uint16_t  eide_dma_time;  /* recommended mword dma cycle time (ns) */
    uint16_t  eide_pio;       /* min cycle time (ns), no IORDY  */
    uint16_t  eide_pio_iordy; /* min cycle time (ns), with IORDY */
    uint16_t  words69_70[2];  /* reserved words 69-70
                     * future command overlap and queuing
                     */
    /* HDIO_GET_IDENTITY currently returns only words 0 through 70 */
    uint16_t  words71_74[4];  /* reserved words 71-74
                     * for IDENTIFY PACKET DEVICE command
                     */
    uint16_t  queue_depth;    /* (word 75)
                     * 15:5 reserved
                     *  4:0 Maximum queue depth -1
                     */
    uint16_t  words76_79[4];  /* reserved words 76-79 */
    uint16_t  major_rev_num;  /* (word 80) */
    uint16_t  minor_rev_num;  /* (word 81) */
    uint16_t  command_set_1;  /* (word 82) supported
                     * 15:  Obsolete
                     * 14:  NOP command
                     * 13:  READ_BUFFER
                     * 12:  WRITE_BUFFER
                     * 11:  Obsolete
                     * 10:  Host Protected Area
                     *  9:  DEVICE Reset
                     *  8:  SERVICE Interrupt
                     *  7:  Release Interrupt
                     *  6:  look-ahead
                     *  5:  write cache
                     *  4:  PACKET Command
                     *  3:  Power Management Feature Set
                     *  2:  Removable Feature Set
                     *  1:  Security Feature Set
                     *  0:  SMART Feature Set
                     */
    uint16_t  command_set_2;  /* (word 83)
                     * 15:  Shall be ZERO
                     * 14:  Shall be ONE
                     * 13:  FLUSH CACHE EXT
                     * 12:  FLUSH CACHE
                     * 11:  Device Configuration Overlay
                     * 10:  48-bit Address Feature Set
                     *  9:  Automatic Acoustic Management
                     *  8:  SET MAX security
                     *  7:  reserved 1407DT PARTIES
                     *  6:  SetF sub-command Power-Up
                     *  5:  Power-Up in Standby Feature Set
                     *  4:  Removable Media Notification
                     *  3:  APM Feature Set
                     *  2:  CFA Feature Set
                     *  1:  READ/WRITE DMA QUEUED
                     *  0:  Download MicroCode
                     */
    uint16_t  cfsse;      /* (word 84)
                     * cmd set-feature supported extensions
                     * 15:  Shall be ZERO
                     * 14:  Shall be ONE
                     * 13:6 reserved
                     *  5:  General Purpose Logging
                     *  4:  Streaming Feature Set
                     *  3:  Media Card Pass Through
                     *  2:  Media Serial Number Valid
                     *  1:  SMART selt-test supported
                     *  0:  SMART error logging
                     */
    uint16_t  cfs_enable_1;   /* (word 85)
                     * command set-feature enabled
                     * 15:  Obsolete
                     * 14:  NOP command
                     * 13:  READ_BUFFER
                     * 12:  WRITE_BUFFER
                     * 11:  Obsolete
                     * 10:  Host Protected Area
                     *  9:  DEVICE Reset
                     *  8:  SERVICE Interrupt
                     *  7:  Release Interrupt
                     *  6:  look-ahead
                     *  5:  write cache
                     *  4:  PACKET Command
                     *  3:  Power Management Feature Set
                     *  2:  Removable Feature Set
                     *  1:  Security Feature Set
                     *  0:  SMART Feature Set
                     */
    uint16_t  cfs_enable_2;   /* (word 86)
                     * command set-feature enabled
                     * 15:  Shall be ZERO
                     * 14:  Shall be ONE
                     * 13:  FLUSH CACHE EXT
                     * 12:  FLUSH CACHE
                     * 11:  Device Configuration Overlay
                     * 10:  48-bit Address Feature Set
                     *  9:  Automatic Acoustic Management
                     *  8:  SET MAX security
                     *  7:  reserved 1407DT PARTIES
                     *  6:  SetF sub-command Power-Up
                     *  5:  Power-Up in Standby Feature Set
                     *  4:  Removable Media Notification
                     *  3:  APM Feature Set
                     *  2:  CFA Feature Set
                     *  1:  READ/WRITE DMA QUEUED
                     *  0:  Download MicroCode
                     */
    uint16_t  csf_default;    /* (word 87)
                     * command set-feature default
                     * 15:  Shall be ZERO
                     * 14:  Shall be ONE
                     * 13:6 reserved
                     *  5:  General Purpose Logging enabled
                     *  4:  Valid CONFIGURE STREAM executed
                     *  3:  Media Card Pass Through enabled
                     *  2:  Media Serial Number Valid
                     *  1:  SMART selt-test supported
                     *  0:  SMART error logging
                     */
    uint16_t  dma_ultra;  /* (word 88) */
    uint16_t  trseuc;     /* time required for security erase */
    uint16_t  trsEuc;     /* time required for enhanced erase */
    uint16_t  CurAPMvalues;   /* current APM values */
    uint16_t  mprc;       /* master password revision code */
    uint16_t  hw_config;  /* hardware config (word 93)
                     * 15:  Shall be ZERO
                     * 14:  Shall be ONE
                     * 13:
                     * 12:
                     * 11:
                     * 10:
                     *  9:
                     *  8:
                     *  7:
                     *  6:
                     *  5:
                     *  4:
                     *  3:
                     *  2:
                     *  1:
                     *  0:  Shall be ONE
                     */
    uint16_t  acoustic;   /* (word 94)
                     * 15:8 Vendor's recommended value
                     *  7:0 current value
                     */
    uint16_t  msrqs;      /* min stream request size */
    uint16_t  sxfert;     /* stream transfer time */
    uint16_t  sal;        /* stream access latency */
    uint32_t    spg;        /* stream performance granularity */
    //unsigned long long lba_capacity_2;/* 48-bit total number of sectors */
    uint64_t lba_capacity_2;/* 48-bit total number of sectors */
    
    uint16_t  words104_105[2];/* reserved words 104-105 */

    uint16_t  words106;		/* Physical sector size / Logical Sector Size
							 * 15 : Shall be cleared to zeor
							 * 14 : Shall be set to one
							 * 13 : 1 = Device has multiple logical sectors per physical sector.
							 * 12 : 1 = Device Logical Sector Longer than 256 Words
							 * 11:4 : Reserved
							 * 3:0 : 2 ^ x logical sectors per physical sector
							*/
	
    uint16_t  words107_116[10];	/* reserved words 107-116 */
	
    uint32_t	WordsPerLogicalSector;	/* Word per Logical sector (word 117:118) */
    
	uint16_t  words119_125[7];/* reserved words 119-125 */
    uint16_t  last_lun;   /* (word 126) */
    uint16_t  word127;    /* (word 127) Feature Set
                     * Removable Media Notification
                     * 15:2 reserved
                     *  1:0 00 = not supported
                     *  01 = supported
                     *  10 = reserved
                     *  11 = reserved
                     */
    uint16_t  dlf;        /* (word 128)
                     * device lock function
                     * 15:9 reserved
                     *  8   security level 1:max 0:high
                     *  7:6 reserved
                     *  5   enhanced erase
                     *  4   expire
                     *  3   frozen
                     *  2   locked
                     *  1   en/disabled
                     *  0   capability
                     */
    uint16_t  csfo;       /*  (word 129)
                     * current set features options
                     * 15:4 reserved
                     *  3:  auto reassign
                     *  2:  reverting
                     *  1:  read-look-ahead
                     *  0:  write cache
                     */
    uint16_t  words130_155[26];/* reserved vendor words 130-155 */
    uint16_t  word156;    /* reserved vendor word 156 */
    uint16_t  words157_159[3];/* reserved vendor words 157-159 */
    uint16_t  cfa_power;  /* (word 160) CFA Power Mode
                     * 15 word 160 supported
                     * 14 reserved
                     * 13
                     * 12
                     * 11:0
                     */
    uint16_t  words161_175[14];/* Reserved for CFA */
    uint16_t  words176_205[31];/* Current Media Serial Number */
    uint16_t  words206_208[3];/* reserved words 206-208 */
    uint16_t  words209;			/* Alignment of logical blocks within a larger physical block (word 209)
								 * 15 : Shall be cleared to zero
								 * 14 : Shall be set to one
								 * 13:0 : 'Logical sector' offset whthin the first physical sector where the first logical sector is placed
								 */
    uint16_t  words210_254[45];/* reserved words 210-254 */
    uint16_t  integrity_word; /* (word 255)
                     * 15:8 Checksum
                     *  7:0 Signature
                     */
};

/*
 * IDE "nice" flags. These are used on a per drive basis to determine
 * when to be nice and give more bandwidth to the other devices which
 * share the same IDE bus.
 */
#define IDE_NICE_DSC_OVERLAP    (0) /* per the DSC overlap protocol */
#define IDE_NICE_ATAPI_OVERLAP  (1) /* not supported yet */
#define IDE_NICE_0      (2) /* when sure that it won't affect us */
#define IDE_NICE_1      (3) /* when probably won't affect us much */
#define IDE_NICE_2      (4) /* when we know it's on our expense */

#ifdef __KERNEL__
/*
 * These routines are used for kernel command line parameters from main.c:
 */
#include <linux/config.h>

#if defined(CONFIG_BLK_DEV_IDE) || defined(CONFIG_BLK_DEV_IDE_MODULE)
int ide_register(int io_port, int ctl_port, int irq);
void ide_unregister(uint32_t);
#endif /* CONFIG_BLK_DEV_IDE || CONFIG_BLK_DEV_IDE_MODULE */

#endif  /* __KERNEL__ */

#endif  /* _HDREG_H */
