/*
 * Misc system wide definitions
 *
 * Copyright (C) 1999-2016, Broadcom Corporation
 * 
 *      Unless you and Broadcom execute a separate written software license
 * agreement governing use of this software, this software is licensed to you
 * under the terms of the GNU General Public License version 2 (the "GPL"),
 * available at http://www.broadcom.com/licenses/GPLv2.php, with the
 * following added to such license:
 * 
 *      As a special exception, the copyright holders of this software give you
 * permission to link this software with independent modules, and to copy and
 * distribute the resulting executable under terms of your choice, provided that
 * you also meet, for each linked independent module, the terms and conditions of
 * the license of that module.  An independent module is a module which is not
 * derived from this software.  The special exception does not apply to any
 * modifications of the software.
 * 
 *      Notwithstanding the above, under no circumstances may you combine this
 * software in any way with any other Broadcom software provided under a license
 * other than the GPL, without Broadcom's express prior written consent.
 *
 * $Id: bcmdefs.h 516456 2014-11-19 21:00:05Z $
 */

#ifndef	_bcmdefs_h_
#define	_bcmdefs_h_

/*
 * One doesn't need to include this file explicitly, gets included automatically if
 * typedefs.h is included.
 */

/* Use BCM_REFERENCE to suppress warnings about intentionally-unused function
 * arguments or local variables.
 */
#define BCM_REFERENCE(data)	((void)(data))

/* Allow for suppressing unused variable warnings. */
#ifdef __GNUC__
#define UNUSED_VAR     __attribute__ ((unused))
#else
#define UNUSED_VAR
#endif

/* Compile-time assert can be used in place of ASSERT if the expression evaluates
 * to a constant at compile time.
 */
#define STATIC_ASSERT(expr) { \
	/* Make sure the expression is constant. */ \
	typedef enum { _STATIC_ASSERT_NOT_CONSTANT = (expr) } _static_assert_e UNUSED_VAR; \
	/* Make sure the expression is true. */ \
	typedef char STATIC_ASSERT_FAIL[(expr) ? 1 : -1] UNUSED_VAR; \
}

/* Reclaiming text and data :
 * The following macros specify special linker sections that can be reclaimed
 * after a system is considered 'up'.
 * BCMATTACHFN is also used for detach functions (it's not worth having a BCMDETACHFN,
 * as in most cases, the attach function calls the detach function to clean up on error).
 */

#define bcmreclaimed 		0
#define _data	_data
#define _fn	_fn
#define BCMPREATTACHDATA(_data)	_data
#define BCMPREATTACHFN(_fn)	_fn
#define _data	_data
#define _fn		_fn
#define _fn	_fn
#define	BCMNMIATTACHFN(_fn)	_fn
#define	BCMNMIATTACHDATA(_data)	_data
#define CONST	const

#undef BCM47XX_CA9

#ifndef BCMFASTPATH
#define BCMFASTPATH
#define BCMFASTPATH_HOST
#endif /* BCMFASTPATH */


/* Use the BCMRAMFN() macro to tag functions in source that must be included in RAM (excluded from
 * ROM). This should eliminate the need to manually specify these functions in the ROM config file.
 * It should only be used in special cases where the function must be in RAM for *all* ROM-based
 * chips.
 */
	#define BCMRAMFN(_fn)	_fn

#define STATIC	static

/* Bus types */
#define	SI_BUS			0	/* SOC Interconnect */
#define	PCI_BUS			1	/* PCI target */
#define	PCMCIA_BUS		2	/* PCMCIA target */
#define SDIO_BUS		3	/* SDIO target */
#define JTAG_BUS		4	/* JTAG */
#define USB_BUS			5	/* USB (does not support R/W REG) */
#define SPI_BUS			6	/* gSPI target */
#define RPC_BUS			7	/* RPC target */

/* Allows size optimization for single-bus image */
#ifdef BCMBUSTYPE
#define BUSTYPE(bus) 	(BCMBUSTYPE)
#else
#define BUSTYPE(bus) 	(bus)
#endif

/* Allows size optimization for single-backplane image */
#ifdef BCMCHIPTYPE
#define CHIPTYPE(bus) 	(BCMCHIPTYPE)
#else
#define CHIPTYPE(bus) 	(bus)
#endif


/* Allows size optimization for SPROM support */
#if defined(BCMSPROMBUS)
#define SPROMBUS	(BCMSPROMBUS)
#elif defined(SI_PCMCIA_SROM)
#define SPROMBUS	(PCMCIA_BUS)
#else
#define SPROMBUS	(PCI_BUS)
#endif

/* Allows size optimization for single-chip image */
#ifdef BCMCHIPID
#define CHIPID(chip)	(BCMCHIPID)
#else
#define CHIPID(chip)	(chip)
#endif

#ifdef BCMCHIPREV
#define CHIPREV(rev)	(BCMCHIPREV)
#else
#define CHIPREV(rev)	(rev)
#endif

/* Defines for DMA Address Width - Shared between OSL and HNDDMA */
#define DMADDR_MASK_32 0x0		/* Address mask for 32-bits */
#define DMADDR_MASK_30 0xc0000000	/* Address mask for 30-bits */
#define DMADDR_MASK_26 0xFC000000	/* Address maks for 26-bits */
#define DMADDR_MASK_0  0xffffffff	/* Address mask for 0-bits (hi-part) */

#define	DMADDRWIDTH_26  26 /* 26-bit addressing capability */
#define	DMADDRWIDTH_30  30 /* 30-bit addressing capability */
#define	DMADDRWIDTH_32  32 /* 32-bit addressing capability */
#define	DMADDRWIDTH_63  63 /* 64-bit addressing capability */
#define	DMADDRWIDTH_64  64 /* 64-bit addressing capability */

typedef struct {
	uint32 loaddr;
	uint32 hiaddr;
} dma64addr_t;

#define PHYSADDR64HI(_pa) ((_pa).hiaddr)
#define PHYSADDR64HISET(_pa, _val) \
	do { \
		(_pa).hiaddr = (_val);		\
	} while (0)
#define PHYSADDR64LO(_pa) ((_pa).loaddr)
#define PHYSADDR64LOSET(_pa, _val) \
	do { \
		(_pa).loaddr = (_val);		\
	} while (0)

#define PHYSADDR64ADD(_pa, _hi0, _lo0, _hi1, _lo1) \
	do { \
		uint32 _lo = (uint32)(_lo0); \
		(_pa).loaddr = _lo + (uint32)(_lo1); \
		(_pa).hiaddr = (uint32)(_hi0) + (uint32)(_hi1) + \
			(((_pa).loaddr < _lo)? 1 : 0); \
	} while (0)
#define PHYSADDR64ADD64BITDATA(_pa, _paorg, _hi, _lo) \
	PHYSADDR64ADD((_pa), (_paorg).hiaddr, (_paorg).loaddr, (_hi), (_lo))
#define PHYSADDR64ADDOFFSET(_pa, _paorg, _offset) \
	PHYSADDR64ADD((_pa), (_paorg).hiaddr, (_paorg).loaddr, 0, (_offset))
#define PHYSADDR64ROUNDUP(_pa, _paorg, _align) \
	do { \
		PHYSADDR64ADDOFFSET(_pa, _paorg, (uint32)((_align) - 1)); \
		(_pa).loaddr = ((_pa).loaddr / (_align)) * (_align); \
	} while (0)

#define PHYSADDR64SUB(_pa, _hi0, _lo0, _hi1, _lo1) \
	do { \
		uint32 _lo = (uint32)(_lo0); \
		(_pa).loaddr = _lo - (uint32)(_lo1); \
		(_pa).hiaddr = (uint32)(_hi0) - (uint32)(_hi1) - \
			(((_pa).loaddr > _lo)? 1 : 0); \
	} while (0)
#define PHYSADDR64SUB64BITDATA(_pa, _paorg, _hi, _lo) \
	PHYSADDR64SUB((_pa), (_paorg).hiaddr, (_paorg).loaddr, (_hi), (_lo))
#define PHYSADDR64SUBOFFSET(_pa, _paorg, _offset) \
	PHYSADDR64SUB((_pa), (_paorg).hiaddr, (_paorg).loaddr, 0, (_offset))

#ifdef BCMDMA64OSL
typedef dma64addr_t dmaaddr_t;
#define PHYSADDRHI(_pa) PHYSADDR64HI(_pa)
#define PHYSADDRHISET(_pa, _val) PHYSADDR64HISET(_pa, _val)
#define PHYSADDRLO(_pa)  PHYSADDR64LO(_pa)
#define PHYSADDRLOSET(_pa, _val) PHYSADDR64LOSET(_pa, _val)
#define PHYSADDRADD(_pa, _hi0, _lo0, _hi1, _lo1) \
	PHYSADDR64ADD(_pa, _hi0, _lo0, _hi1, _lo1)
#define PHYSADDRADD64BITDATA(_pa, _paorg, _hi, _lo) \
	PHYSADDR64ADD64BITDATA(_pa, _paorg, _hi, _lo)
#define PHYSADDRADDOFFSET(_pa, _paorg, _offset) \
	PHYSADDR64ADDOFFSET(_pa, _paorg, _offset)
#define PHYSADDRROUNDUP(_pa, _paorg, _align) \
	PHYSADDR64ROUNDUP(_pa, _paorg, _align)
#define PHYSADDRSUB(_pa, _hi0, _lo0, _hi1, _lo1) \
	PHYSADDR64SUB(_pa, _hi0, _lo0, _hi1, _lo1)
#define PHYSADDRSUB64BITDATA(_pa, _paorg, _hi, _lo) \
	PHYSADDR64SUB64BITDATA(_pa, _paorg, _hi, _lo)
#define PHYSADDRSUBOFFSET(_pa, _paorg, _offset) \
	PHYSADDR64SUBOFFSET(_pa, _paorg, _offset)
#else
typedef unsigned long dmaaddr_t;
#define PHYSADDRHI(_pa) (0)
#define PHYSADDRHISET(_pa, _val)
#define PHYSADDRLO(_pa) ((_pa))
#define PHYSADDRLOSET(_pa, _val) \
	do { \
		(_pa) = (_val);			\
	} while (0)
#define PHYSADDRADD(_pa, _hi0, _lo0, _hi1, _lo1) \
	do { \
		(_pa) = (uint32)(_lo0) + (uint32)(_lo1); \
	} while (0)
#define PHYSADDRADDOFFSET(_pa, _paorg, _offset) \
	do { \
		(_pa) = (uint32)(_paorg) + (uint32)(_offset); \
	} while (0)
#define PHYSADDRADD64BITDATA(_pa, _paorg, _hi, _lo) \
	do { \
		(_pa) = (uint32)(_paorg) + (uint32)(_lo); \
	} while (0)
#define PHYSADDRROUNDUP(_pa, _paorg, _align) \
	do { \
		(_pa) = (((_paorg) + ((_align) - 1)) / (_align)) * (_align); \
	} while (0)
#define PHYSADDRSUB(_pa, _hi0, _lo0, _hi1, _lo1) \
	do { \
		(_pa) = (uint32)(_lo0) - (uint32)(_lo1); \
	} while (0)
#define PHYSADDRSUB64BITDATA(_pa, _paorg, _hi, _lo) \
	do { \
		(_pa) = (uint32)(_paorg) - (uint32)(_lo); \
	} while (0)
#define PHYSADDRSUBOFFSET(_pa, _paorg, _offset) \
	do { \
		(_pa) = (uint32)(_paorg) - (uint32)(_offset); \
	} while (0)
#endif /* BCMDMA64OSL */
#define PHYSADDRISZERO(_pa) (PHYSADDRLO(_pa) == 0 && PHYSADDRHI(_pa) == 0)

/* One physical DMA segment */
typedef struct  {
	dmaaddr_t addr;
	uint32	  length;
} hnddma_seg_t;

#define MAX_DMA_SEGS 8


typedef struct {
	void *oshdmah; /* Opaque handle for OSL to store its information */
	uint origsize; /* Size of the virtual packet */
	uint nsegs;
	hnddma_seg_t segs[MAX_DMA_SEGS];
} hnddma_seg_map_t;


/* packet headroom necessary to accommodate the largest header in the system, (i.e TXOFF).
 * By doing, we avoid the need  to allocate an extra buffer for the header when bridging to WL.
 * There is a compile time check in wlc.c which ensure that this value is at least as big
 * as TXOFF. This value is used in dma_rxfill (hnddma.c).
 */

#if defined(BCM_RPC_NOCOPY) || defined(BCM_RCP_TXNOCOPY)
/* add 40 bytes to allow for extra RPC header and info  */
#define BCMEXTRAHDROOM 260
#else /* BCM_RPC_NOCOPY || BCM_RPC_TXNOCOPY */
#define BCMEXTRAHDROOM 204
#endif /* BCM_RPC_NOCOPY || BCM_RPC_TXNOCOPY */

/* Packet alignment for most efficient SDIO (can change based on platform) */
#ifndef SDALIGN
#define SDALIGN	32
#endif

/* Headroom required for dongle-to-host communication.  Packets allocated
 * locally in the dongle (e.g. for CDC ioctls or RNDIS messages) should
 * leave this much room in front for low-level message headers which may
 * be needed to get across the dongle bus to the host.  (These messages
 * don't go over the network, so room for the full WL header above would
 * be a waste.).
*/
#define BCMDONGLEHDRSZ 12
#define BCMDONGLEPADSZ 16

#define BCMDONGLEOVERHEAD	(BCMDONGLEHDRSZ + BCMDONGLEPADSZ)


#if defined(NO_BCMDBG_ASSERT)
# undef BCMDBG_ASSERT
# undef BCMASSERT_LOG
#endif

#if defined(BCMASSERT_LOG)
#define BCMASSERT_SUPPORT
#endif 

/* Macros for doing definition and get/set of bitfields
 * Usage example, e.g. a three-bit field (bits 4-6):
 *    #define <NAME>_M	BITFIELD_MASK(3)
 *    #define <NAME>_S	4
 * ...
 *    regval = R_REG(osh, &regs->regfoo);
 *    field = GFIELD(regval, <NAME>);
 *    regval = SFIELD(regval, <NAME>, 1);
 *    W_REG(osh, &regs->regfoo, regval);
 */
#define BITFIELD_MASK(width) \
		(((unsigned)1 << (width)) - 1)
#define GFIELD(val, field) \
		(((val) >> field ## _S) & field ## _M)
#define SFIELD(val, field, bits) \
		(((val) & (~(field ## _M << field ## _S))) | \
		 ((unsigned)(bits) << field ## _S))

/* define BCMSMALL to remove misc features for memory-constrained environments */
#ifdef BCMSMALL
#undef	BCMSPACE
#define bcmspace	FALSE	/* if (bcmspace) code is discarded */
#else
#define	BCMSPACE
#define bcmspace	TRUE	/* if (bcmspace) code is retained */
#endif

/* Max. nvram variable table size */
#ifndef MAXSZ_NVRAM_VARS
#define	MAXSZ_NVRAM_VARS	4096
#endif



/* WL_ENAB_RUNTIME_CHECK may be set based upon the #define below (for ROM builds). It may also
 * be defined via makefiles (e.g. ROM auto abandon unoptimized compiles).
 */


#ifdef BCMLFRAG /* BCMLFRAG support enab macros  */
	extern bool _bcmlfrag;
	#if defined(WL_ENAB_RUNTIME_CHECK) || !defined(DONGLEBUILD)
		#define BCMLFRAG_ENAB() (_bcmlfrag)
	#elif defined(BCMLFRAG_DISABLED)
		#define BCMLFRAG_ENAB()	(0)
	#else
		#define BCMLFRAG_ENAB()	(1)
	#endif
#else
	#define BCMLFRAG_ENAB()		(0)
#endif /* BCMLFRAG_ENAB */
#ifdef BCMSPLITRX /* BCMLFRAG support enab macros  */
	extern bool _bcmsplitrx;
	#if defined(WL_ENAB_RUNTIME_CHECK) || !defined(DONGLEBUILD)
		#define BCMSPLITRX_ENAB() (_bcmsplitrx)
	#elif defined(BCMSPLITRX_DISABLED)
		#define BCMSPLITRX_ENAB()	(0)
	#else
		#define BCMSPLITRX_ENAB()	(1)
	#endif
#else
	#define BCMSPLITRX_ENAB()		(0)
#endif /* BCMSPLITRX */
#ifdef BCM_SPLITBUF
	extern bool _bcmsplitbuf;
	#if defined(WL_ENAB_RUNTIME_CHECK) || !defined(DONGLEBUILD)
		#define BCM_SPLITBUF_ENAB() (_bcmsplitbuf)
	#elif defined(BCM_SPLITBUF_DISABLED)
		#define BCM_SPLITBUF_ENAB()	(0)
	#else
		#define BCM_SPLITBUF_ENAB()	(1)
	#endif
#else
	#define BCM_SPLITBUF_ENAB()		(0)
#endif	/* BCM_SPLITBUF */
/* Max size for reclaimable NVRAM array */
#ifdef DL_NVRAM
#define NVRAM_ARRAY_MAXSIZE	DL_NVRAM
#else
#define NVRAM_ARRAY_MAXSIZE	MAXSZ_NVRAM_VARS
#endif /* DL_NVRAM */

extern uint32 gFWID;


#endif /* _bcmdefs_h_ */
