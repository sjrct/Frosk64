//
// inline_asm.h
//
// written by sjrct
//

#ifndef INLINE_ASM_H
#define INLINE_ASM_H

#define OUTB(P,X) asm volatile ("outb %0, %1" :: "a" ((uchar) (X)), "Nd" ((ushort)(P)))
#define OUTW(P,X) asm volatile ("outw %0, %1" :: "a" ((ushort)(X)), "Nd" ((ushort)(P)))
#define OUTL(P,X) asm volatile ("outl %0, %1" :: "a" ((uint)  (X)), "Nd" ((ushort)(P)))

#define INB(X,P)  asm volatile ("inb %1, %0" : "=a" ((uchar) (X)) : "Nd" ((ushort)(P)))
#define INW(X,P)  asm volatile ("inw %1, %0" : "=a" ((ushort)(X)) : "Nd" ((ushort)(P)))
#define INL(X,P)  asm volatile ("inl %1, %0" : "=a" ((uint)  (X)) : "Nd" ((ushort)(P)))

#define IO_WAIT   asm volatile ("outb %%al, $0x80" :: "a" (0))

#define INVLPG(X) asm volatile ("invlpg (%0)" :: "r" (X) : "memory")
#define HLT       asm volatile ("hlt")

#define GET_FLAGS(X) asm volatile ("pushfq; movq (%%rsp), %0; addq $8, %%rsp" : "=r" (X))

#endif
