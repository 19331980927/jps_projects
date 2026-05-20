	.cpu cortex-m3
	.arch armv7-m
	.fpu softvfp
	.eabi_attribute 20, 1
	.eabi_attribute 21, 1
	.eabi_attribute 23, 3
	.eabi_attribute 24, 1
	.eabi_attribute 25, 1
	.eabi_attribute 26, 1
	.eabi_attribute 30, 6
	.eabi_attribute 34, 1
	.eabi_attribute 18, 4
	.file	"arch_offset.c"
	.text
.Ltext0:
	.cfi_sections	.debug_frame
	.file 1 "D:/AutoSAR_Project/classic-platform-master/system/Os/osal/arm/armv7_m/kernel/arch_offset.c"
	.section	.text.arch_offset_dummy,"ax",%progbits
	.align	1
	.global	arch_offset_dummy
	.syntax unified
	.thumb
	.thumb_func
	.type	arch_offset_dummy, %function
arch_offset_dummy:
.LFB87:
	.loc 1 11 30
	.cfi_startproc
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 0
	@ link register save eliminated.
	push	{r7}
.LCFI0:
	.cfi_def_cfa_offset 4
	.cfi_offset 7, -4
	add	r7, sp, #0
.LCFI1:
	.cfi_def_cfa_register 7
	.loc 1 13 5
	.syntax unified
@ 13 "D:/AutoSAR_Project/classic-platform-master/system/Os/osal/arm/armv7_m/kernel/arch_offset.c" 1
	#define	EXC_FRM_R0	#0
@ 0 "" 2
	.loc 1 14 5
@ 14 "D:/AutoSAR_Project/classic-platform-master/system/Os/osal/arm/armv7_m/kernel/arch_offset.c" 1
	#define	EXC_FRM_R1	#4
@ 0 "" 2
	.loc 1 15 5
@ 15 "D:/AutoSAR_Project/classic-platform-master/system/Os/osal/arm/armv7_m/kernel/arch_offset.c" 1
	#define	EXC_FRM_R2	#8
@ 0 "" 2
	.loc 1 16 5
@ 16 "D:/AutoSAR_Project/classic-platform-master/system/Os/osal/arm/armv7_m/kernel/arch_offset.c" 1
	#define	EXC_FRM_R3	#12
@ 0 "" 2
	.loc 1 17 5
@ 17 "D:/AutoSAR_Project/classic-platform-master/system/Os/osal/arm/armv7_m/kernel/arch_offset.c" 1
	#define	EXC_FRM_R12	#16
@ 0 "" 2
	.loc 1 18 5
@ 18 "D:/AutoSAR_Project/classic-platform-master/system/Os/osal/arm/armv7_m/kernel/arch_offset.c" 1
	#define	EXC_FRM_LR	#20
@ 0 "" 2
	.loc 1 19 5
@ 19 "D:/AutoSAR_Project/classic-platform-master/system/Os/osal/arm/armv7_m/kernel/arch_offset.c" 1
	#define	EXC_FRM_PC	#24
@ 0 "" 2
	.loc 1 20 5
@ 20 "D:/AutoSAR_Project/classic-platform-master/system/Os/osal/arm/armv7_m/kernel/arch_offset.c" 1
	#define	EXC_FRM_PSR	#28
@ 0 "" 2
	.loc 1 23 5
@ 23 "D:/AutoSAR_Project/classic-platform-master/system/Os/osal/arm/armv7_m/kernel/arch_offset.c" 1
	#define	STACK_APA	#32
@ 0 "" 2
	.loc 1 26 5
@ 26 "D:/AutoSAR_Project/classic-platform-master/system/Os/osal/arm/armv7_m/kernel/arch_offset.c" 1
	#define	STACK_CURR	#0
@ 0 "" 2
	.loc 1 27 5
@ 27 "D:/AutoSAR_Project/classic-platform-master/system/Os/osal/arm/armv7_m/kernel/arch_offset.c" 1
	#define	STACK_CURR_TOP	#4
@ 0 "" 2
	.loc 1 28 5
@ 28 "D:/AutoSAR_Project/classic-platform-master/system/Os/osal/arm/armv7_m/kernel/arch_offset.c" 1
	#define	STACK_CURR_SIZE	#8
@ 0 "" 2
	.loc 1 29 5
@ 29 "D:/AutoSAR_Project/classic-platform-master/system/Os/osal/arm/armv7_m/kernel/arch_offset.c" 1
	#define	TASK_CONST	#64
@ 0 "" 2
	.loc 1 30 1
	.thumb
	.syntax unified
	nop
	mov	sp, r7
.LCFI2:
	.cfi_def_cfa_register 13
	@ sp needed
	pop	{r7}
.LCFI3:
	.cfi_restore 7
	.cfi_def_cfa_offset 0
	bx	lr
	.cfi_endproc
.LFE87:
	.size	arch_offset_dummy, .-arch_offset_dummy
	.text
.Letext0:
	.section	.debug_info,"",%progbits
.Ldebug_info0:
	.4byte	0x9f
	.2byte	0x2
	.4byte	.Ldebug_abbrev0
	.byte	0x4
	.uleb128 0x1
	.4byte	.LASF13
	.byte	0xc
	.4byte	.LASF14
	.4byte	.LASF15
	.4byte	.Ldebug_ranges0+0
	.4byte	0
	.4byte	0
	.4byte	.Ldebug_line0
	.uleb128 0x2
	.byte	0x4
	.byte	0x7
	.4byte	.LASF0
	.uleb128 0x3
	.byte	0x4
	.byte	0x5
	.ascii	"int\000"
	.uleb128 0x2
	.byte	0x1
	.byte	0x6
	.4byte	.LASF1
	.uleb128 0x2
	.byte	0x1
	.byte	0x8
	.4byte	.LASF2
	.uleb128 0x2
	.byte	0x2
	.byte	0x5
	.4byte	.LASF3
	.uleb128 0x2
	.byte	0x2
	.byte	0x7
	.4byte	.LASF4
	.uleb128 0x2
	.byte	0x4
	.byte	0x5
	.4byte	.LASF5
	.uleb128 0x2
	.byte	0x4
	.byte	0x7
	.4byte	.LASF6
	.uleb128 0x2
	.byte	0x8
	.byte	0x5
	.4byte	.LASF7
	.uleb128 0x2
	.byte	0x8
	.byte	0x7
	.4byte	.LASF8
	.uleb128 0x2
	.byte	0x1
	.byte	0x8
	.4byte	.LASF9
	.uleb128 0x2
	.byte	0x4
	.byte	0x4
	.4byte	.LASF10
	.uleb128 0x2
	.byte	0x8
	.byte	0x4
	.4byte	.LASF11
	.uleb128 0x2
	.byte	0x1
	.byte	0x2
	.4byte	.LASF12
	.uleb128 0x4
	.byte	0x1
	.4byte	.LASF16
	.byte	0x1
	.byte	0xb
	.byte	0x6
	.byte	0x1
	.4byte	.LFB87
	.4byte	.LFE87
	.4byte	.LLST0
	.byte	0x1
	.byte	0
	.section	.debug_abbrev,"",%progbits
.Ldebug_abbrev0:
	.uleb128 0x1
	.uleb128 0x11
	.byte	0x1
	.uleb128 0x25
	.uleb128 0xe
	.uleb128 0x13
	.uleb128 0xb
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x1b
	.uleb128 0xe
	.uleb128 0x55
	.uleb128 0x6
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x52
	.uleb128 0x1
	.uleb128 0x10
	.uleb128 0x6
	.byte	0
	.byte	0
	.uleb128 0x2
	.uleb128 0x24
	.byte	0
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0x3e
	.uleb128 0xb
	.uleb128 0x3
	.uleb128 0xe
	.byte	0
	.byte	0
	.uleb128 0x3
	.uleb128 0x24
	.byte	0
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0x3e
	.uleb128 0xb
	.uleb128 0x3
	.uleb128 0x8
	.byte	0
	.byte	0
	.uleb128 0x4
	.uleb128 0x2e
	.byte	0
	.uleb128 0x3f
	.uleb128 0xc
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x27
	.uleb128 0xc
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x12
	.uleb128 0x1
	.uleb128 0x40
	.uleb128 0x6
	.uleb128 0x2117
	.uleb128 0xc
	.byte	0
	.byte	0
	.byte	0
	.section	.debug_loc,"",%progbits
.Ldebug_loc0:
.LLST0:
	.4byte	.LFB87
	.4byte	.LCFI0
	.2byte	0x2
	.byte	0x7d
	.sleb128 0
	.4byte	.LCFI0
	.4byte	.LCFI1
	.2byte	0x2
	.byte	0x7d
	.sleb128 4
	.4byte	.LCFI1
	.4byte	.LCFI2
	.2byte	0x2
	.byte	0x77
	.sleb128 4
	.4byte	.LCFI2
	.4byte	.LCFI3
	.2byte	0x2
	.byte	0x7d
	.sleb128 4
	.4byte	.LCFI3
	.4byte	.LFE87
	.2byte	0x2
	.byte	0x7d
	.sleb128 0
	.4byte	0
	.4byte	0
	.section	.debug_aranges,"",%progbits
	.4byte	0x1c
	.2byte	0x2
	.4byte	.Ldebug_info0
	.byte	0x4
	.byte	0
	.2byte	0
	.2byte	0
	.4byte	.LFB87
	.4byte	.LFE87-.LFB87
	.4byte	0
	.4byte	0
	.section	.debug_ranges,"",%progbits
.Ldebug_ranges0:
	.4byte	.LFB87
	.4byte	.LFE87
	.4byte	0
	.4byte	0
	.section	.debug_line,"",%progbits
.Ldebug_line0:
	.section	.debug_str,"MS",%progbits,1
.LASF7:
	.ascii	"long long int\000"
.LASF0:
	.ascii	"unsigned int\000"
.LASF14:
	.ascii	"D:/AutoSAR_Project/classic-platform-master/system/O"
	.ascii	"s/osal/arm/armv7_m/kernel/arch_offset.c\000"
.LASF16:
	.ascii	"arch_offset_dummy\000"
.LASF6:
	.ascii	"long unsigned int\000"
.LASF3:
	.ascii	"short int\000"
.LASF8:
	.ascii	"long long unsigned int\000"
.LASF2:
	.ascii	"unsigned char\000"
.LASF9:
	.ascii	"char\000"
.LASF5:
	.ascii	"long int\000"
.LASF11:
	.ascii	"double\000"
.LASF12:
	.ascii	"_Bool\000"
.LASF15:
	.ascii	"D:\\AutoSAR_Project\\classic-platform-master\\bcm_a"
	.ascii	"pp\\obj_stm32f103_bcm\000"
.LASF4:
	.ascii	"short unsigned int\000"
.LASF1:
	.ascii	"signed char\000"
.LASF10:
	.ascii	"float\000"
.LASF13:
	.ascii	"GNU C99 13.3.0 -mcpu=cortex-m3 -mthumb -mfloat-abi="
	.ascii	"soft -march=armv7-m -g -gdwarf-2 -O0 -std=gnu99 -ff"
	.ascii	"unction-sections -fno-strict-aliasing -fno-builtin\000"
	.ident	"GCC: (GNU) 13.3.0"
