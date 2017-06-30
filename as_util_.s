        .include "Linux.s"

        .section .text

#
#       change the following to ".if 1" to assemble with test driver code.
#       change the following to ".if 0" to make this a package to link with.
#
        .if     1
        .globl  _start
_start:
        movq    $0xFEDCBA9876543210,%rax
        call    as_u_Phex16
        call    as_u_NL
        call    as_u_Phex8
        call    as_u_NL
        call    as_u_Phex4
        call    as_u_NL
        call    as_u_Phex2
        call    as_u_NL
        call    as_u_Phex1
        call    as_u_NL
        call    as_u_Phex16
        call    as_u_NL
        movq    $5,%rcx
        call    as_u_PhexCl
        call    as_u_NL

        call    as_u_DisplayInline
        .asciz  "The first two\n   lines."
        call    as_u_DisplayInline
        .asciz  "\nThe last line.\n"
        .page
        call    as_u_DumpRIP
        call    as_u_DumpRIP
        call    as_u_DumpRIP
        call    as_u_DumpRIP
        call    as_u_DumpRIP
        movq    $-1234567890,%rax
        call    as_u_PSDec
        call    as_u_NL
        call    as_u_PUDec
        call    as_u_NL
        movq    $8,%rax
        call    as_u_PSDec
        call    as_u_NL
        movq    $9876543210,%rax
        call    as_u_PUDec
        call    as_u_NL
        call    as_u_DumpRegs

        call    as_u_DisplayInline
        .asciz  "\nNow to test the dump function 1"
        push    %rbp
        movq    %rsp,%rbp
        subq    $200,%rsp
        movq    %rsp,%rcx
        movq    %rbp,%rdx
        call    as_u_dump

        lea     -128(%rdx),%rsi #seed some same lines
        lea     16(%rsi),%rdi
        pushq   %rcx
        movq    $48,%rcx
        cld
    rep movsb
        popq    %rcx
        call    as_u_DisplayInline
        .asciz  "\nDump 2 seeded 4 same lines dump 2"
        call    as_u_dump

        call    as_u_DisplayInline
        .asciz  "\nNow to test the dump function 3"
        addq    $5,%rcx
        subq    $5,%rdx
        call    as_u_dump

        call    as_u_DisplayInline
        .asciz  "\nNow a short dump of 6 bytes"
        movq    %rcx,%rdx
        addq    $5,%rdx
        call    as_u_dump

        movq    $sys_exit_group,%rax
        xorq    %rdi,%rdi

# rdi contains zero
        inc     %rdi
        inc     %rdi
        inc     %rdi
        inc     %rdi
1:                      #rdi should have a 4
        syscall
        dec     %rdi
        dec     %rdi
        dec     %rdi
        dec     %rdi
        dec     %rdi
        dec     %rdi
        dec     %rdi
        dec     %rdi
        jmp     1b

        .endif

#PURPOSE:       This function displays the zero terminated ascii string
#               immeadiatly following the call as_u_di
#               thus            .asciz "string to display"
#               adjusting the return rip to immediatly after the string.
#
#PROCESSING:    1) Set up a frame.
#               2) save flags, rax, rdi, rsi ,rcx, rdx
#               3) get the return rip into rsi & rdi. 0 into al. Clear the direction flag.
#               4) do a repne scas.
#
#               6) compute length (rdi-return rip + 1) into rdx
#               7) put modified return rip (return rip + length) back on the
#                       stack replacing the old return rip
#               8) rax := $sys_write; rdi := $STDOUT; rsi has old return rip
#                       the buffer to write. rdx has correct count
#               9) syscall
#               10) restore rdx, rcx, rsi, rdi, rax and flags
#               11) return
#

        .equ    old_ret_rip,8   # offset of return %rip from %rbp

        .globl  as_u_DisplayInline

as_u_DisplayInline:
        pushq   %rbp            # save every thing the scas needs as well
        movq    %rsp,%rbp       # as everything sys_write needs
        pushfq
        pushq   %rax
        pushq   %rdi
        pushq   %rsi
        pushq   %rcx
        pushq   %rdx

        movq    old_ret_rip(%rbp),%rsi  # ordinarily where to return to
        movq    %rsi,%rdi               # it is in this case the address
                                        # of the zero terminated ascii
                                        # string we want to display (%rsi)

        xorq    %rcx,%rcx
        dec     %rcx                    # rcx is -1 which is not zero
        xorb    %al,%al                 # the zero we want to search for (EOS)
        cld                             # direction is up (incrementing)

    repne scasb                         # find the end of string

        movq    %rdi,%rdx               # compute length of string into dx
        subq    %rsi,%rdx
        dec     %rdx

        movq    %rdi,old_ret_rip(%rbp)  # the return rip back on the stack

        movq    $STDOUT,%rdi            # fd where we want to write to.
        movq    $sys_write,%rax         # the fact we want to write
# rsi set up w/buffer address and rdx set up with count

        syscall                         # Linux does the sys_write

        popq    %rdx                    # we saved everything. Now restore it.
        popq    %rcx
        popq    %rsi
        popq    %rdi
        popq    %rax
        popfq
        movq    %rbp,%rsp
        popq    %rbp
        ret                             # hopefully, we goto the right spot.

#PURPOSE:       This function writes a block with 1 byte in it to stdout.
#               The block contains the newline ^J character '\n', which is
#               equivalent to crlf on certain other unmentioned O.S.'s.
#
        .global as_u_NL
as_u_NL:                                # write a new line
        pushq   %rax                    # for sys_write function code
        pushq   %rsi                    # for buffer
        pushq   %rdi                    # for STDOUT file descriptor
        pushq   %rdx                    # for the count of one
        pushq   %rcx                    # in debug discovered %rcx not preserved
        movq    $sys_write,%rax
        movq    $1,%rdx
        movq    $STDOUT,%rdi
        call    1f
        .byte   '\n'                    # new line
1:
        popq    %rsi
        syscall
        popq    %rcx                    # sure is preserved now
        popq    %rdx
        popq    %rdi
        popq    %rsi
        pop     %rax
        ret

#PURPOSE:       This function puts out a ascii space
        .globl  as_u_Blank
as_u_Blank:
        call    as_u_DisplayInline
        .asciz  " "
        ret
        .page

#PURPOSE:       This function will writes the contents of %rax in hex on stdout.
#               The length will be the number of nibbles in %rcx. %rcx must be
#               between 1 and 16 inclusive. An invalid %rcx results in no
#               output.
#
#               A number of convience routines as_u_Phex1, as_u_Phex2,
#               as_u_Phex4, as_u_Phex8 and as_u_Phex16 exists to load %rcx
#               with the more common values and then call as_u_PhexCl. These
#               also preserve the caller %rcx
#
        .globl as_u_PhexCl
as_u_PhexCl:
        pushq   %rax            # number to be displayed. Byte as converted from nibble. sys_write
        pushq   %rbx            # address of translation table
        pushq   %rcx            # count of digits to convert
        pushq   %rdx            # copy of count for sys_write
        decb    %cl
        cmpb    $16,%cl         # 0 not allowed. >15 not allowed. unsigned.
          jnc   9f
        incb    %cl             # 1 through 16 inclusive.
        pushq   %rsi            # pointer to buffer *(--rsi) as built
        push    %rdi            # number moved here & worked with in %al, fd
        push    %rbp            # A frame
        movq    %rsp,%rbp

        movq    %rsp,%rsi       # end of buffer + 1
        subq    $16,%rsp        # allocate buffer of 16 bytes

        movq    %rcx,%rdx       # copy of length for sys_write
        movq    %rax,%rdi       # working copy of number
        call    1f              # translation buffer address for %rbx
        .ascii  "0123456789ABCDEF"
1:
        popq    %rbx            # address of translation buffer
2:
        movb    %dil,%al        # next byte trimmed to low nibble
        andb    $0x0f,%al
        shrq    $4,%rdi         # ready for next nibble
        xlatb                   # convert nibble to ascii
        decq    %rsi            # put byte in buffer (predecrement)
        movb    %al,(%rsi)
          loop  2b              # count down till cx becomes 0

        # %rsi buffer address, %rdx is count.
        movq    $sys_write,%rax # function code for Linux to output.
        movq    $STDOUT,%rdi    # write is to stdout.
        syscall
        movq    %rbp,%rsp       # Restore the world. We are going home.
        popq    %rbp
        popq    %rdi
        popq    %rsi
9:                              # came here if count %rcx was invalid.
        popq    %rdx
        popq    %rcx
        popq    %rbx
        popq    %rax
        ret                             # everything as it was now back

        .globl  as_u_Phex1
as_u_Phex1:
        pushq   %rcx
        movq    $1,%rcx         # a nibble
        call    as_u_PhexCl
        popq    %rcx
        ret

        .globl  as_Phex2
as_u_Phex2:
        pushq   %rcx
        movq    $2,%rcx         # a byte
        call    as_u_PhexCl
        popq    %rcx
        ret

        .globl  as_u_Phex4
as_u_Phex4:
        pushq   %rcx
        movq    $4,%rcx         # a 16 bit half word
        call    as_u_PhexCl
        popq    %rcx
        ret

        .globl  as_u_Phex8
as_u_Phex8:
        pushq   %rcx
        movq    $8,%rcx         # a 32 bit long
        call    as_u_PhexCl
        popq    %rcx
        ret

        .globl  as_u_Phex16
as_u_Phex16:
        pushq   %rcx
        movq    $16,%rcx        # a 64 bit quad
        call    as_u_PhexCl
        popq    %rcx
        ret

#PURPOSE:       This function let's the caller trace where he has been.
#               This writes out the callers %rip in 16 digits of Hexadecimal.
#               Along with a we are here message.
#
#               We use as_u_Phex16, as_u_NL and as_u_DisplayInline to do this.

        .global as_u_DumpRIP
as_u_DumpRIP:
        call    as_u_DisplayInline              # print the we are here message.
        .asciz  "RIP reaches "
        pushq   %rax                    # We preserve %rax
        movq    8(%rsp),%rax            # Cause we need it to hold callers %rip.
        call    as_u_Phex16
        call    as_u_NL                 # and end the line
        popq    %rax
        ret

#PURPOSE:       These 2 functions convert the quad in %rax into ascii in a
#               buffer and print the buffer. as_u_PSDec puts out a minus sign
#               if %rax < 0 then negates it and does the same as as_u_PUDec
#

        .globl  as_u_PSDec
        .globl  as_u_PUDec
as_u_PSDec:
        push    %rax            # non-destructive number
        pushq   %rsi            # will point to beginning of the buffer
        pushq   %rdi            # pointer into the buffer
        pushq   %rcx            # count while building buffer
        xorq    %rcx,%rcx
        pushq   %rbp            # to give buffer back
        movq    %rsp,%rbp
        subq    $24,%rsp        # buffer of 24 characters. >21 max length & /8.
        movq    %rsp,%rsi       # beginning of buffer
        movq    %rsp,%rdi       # buffer pointer
        testq   %rax,%rax       # signed handling of negative %rax is different
          jge   1f
        negq    %rax            # negate %rax
        movb    $'-',(%rdi)     # and put a minus sign in the buffer
        incq    %rdi            # increment buffer pointer
        incb    %cl             # and count
        jmp     1f              # join in the unsigned put decimal at the right
                                # place

as_u_PUDec:
        push    %rax            # non-destructive of number
        pushq   %rsi            # will point to beginning of the buffer
        pushq   %rdi            # pointer into the buffer
        pushq   %rcx            # count while building buffer
        xorq    %rcx,%rcx
        pushq   %rbp            # to give buffer back
        movq    %rsp,%rbp
        subq    $24,%rsp        # buffer of 24 characters >21 max length & /8.
        movq    %rsp,%rsi       # beginning of buffer
        movq    %rsp,%rdi       # buffer pointer
1:
        pushq   %rdx            # remainder and count for write
        pushq   %rbx            # %rbx contains 10
        movq    $10,%rbx        # divisor - base for conversion
        call    2f

# now write the results in the buffer.
        movq    $sys_write,%rax # function code to write
        mov     $STDOUT,%rdi    # file descriptor to write to.
                                # %rsi already set up
        movq    %rcx,%rdx       # the count
        syscall

        popq    %rbx            # restore all for return
        popq    %rdx
        movq    %rbp,%rsp
        popq    %rbp
        popq    %rcx
        popq    %rdi
        popq    %rsi
        popq    %rax
        ret
2:
        xorq    %rdx,%rdx       # upper 64 bits are zero for divide.
        divq    %rbx            # divide by 10. The base.
        pushq   %rdx            # save remainder
        testq   %rax,%rax       # when %rax gone,
          je    3f              # end descent.
        call    2b              # %rax not zero, so descend.
3:
        popq    %rdx            # get digit (i.e. the remainder)
        orb     $'0',%dl        # make ascii digit + '0'
        movb    %dl,(%rdi)      # put ascii into the buffer
        incq    %rdi            # buffer pointer increment
        incb    %cl             # count increment
        ret
        .page
#FUNCTION as_u_dump: dumps memory from (%rcx) to (%rdx). Each line upto 82
#               characters.
        .globl  as_u_dump
as_u_dump:
        pushfq                  # save because we mess with the direction bit.
        pushq   %rax
        pushq   %rbx
        pushq   %rcx
        pushq   %rdx
        pushq   %rsi
        pushq   %rdi
        call    as_u_NL         # start with a clean line
3:
# are we done
        cmpq    %rcx,%rdx       # %rcx > %rdx we are done
          jl    9f
# first put out the address of the line we are dumping with a trailing blank
        movq    %rcx,%rax
        call    as_u_Phex16
        call    as_u_Blank
# %rsi the beginning paragraph of the area to be dumped
        movq    %rcx,%rsi
        andb    $0xf0,%sil
# %rdi  the last paragraph of the area to be dumped
        movq    %rdx,%rdi
        addq    $15,%rdi
        andb    $0xf0,%dil
        call    dump_x_preblank
        call    dump_x
        call    dump_x_postblank
        call    dump_a_preblank
        call    dump_a
        call    dump_chk_same   # %rax returned with 1+n of same lines
        cmpq    $1,%rax         # ? any sames?
          jle   5f
        call    as_u_DisplayInline
        .asciz  "\n\t\t\tSAME"
# now advance %rcx by %rax+16 rounded back to nearest nibble
5:
        andb    $0xf0,%cl
        shlq    $4,%rax
        addq    %rax,%rcx
        call    as_u_NL
        jmp     3b

9:
        popq    %rdi
        popq    %rsi
        popq    %rdx
        popq    %rcx
        popq    %rbx
        popq    %rax
        popfq
        ret

#
# dump_a_preblank fill in spaces before the hex portion of the dump, if the
#       address does not start on a paragraph boundary
#
dump_x_preblank:
        pushq   %rcx
        andb    $0x0f,%cl       # is this a paragraph boundaryl
1:
        test    %cl,%cl         # when cl is zero we are done
          jz    9f
        call    as_u_DisplayInline
        .asciz  "   "           # 3 x one space.
        decb    %cl
        jmp     1b

9:
        popq    %rcx
        ret

#
# dump_x dumps the hex portion of the line. With 1 blank trailing each 2
#               nibble hex byte.
#
dump_x:
        pushq   %rax
        pushq   %rbx
        pushq   %rcx
# first determine where we will stop
        movq    %rcx,%rbx       # end of paragraph = %rcx | 0x0f
        orb     $0x0f,%bl
        cmpq    %rbx,%rdx       # however, cannot be bigger than %rdx
          jg    1f
        movq    %rdx,%rbx       # if it is, then use %rdx.
# are we done? That is %rcx bigger than %rbx
1:
        cmpq    %rcx,%rbx
          jl    9f
        xorq    %rax,%rax       # get byte with unsigned extension to 64 bits.
        movb    (%rcx),%al
        call    as_u_Phex2      # display it.
        call    as_u_Blank      # and the blank
        incq    %rcx            # %rcx to point to next
        jmp     1b

9:
        popq    %rcx
        popq    %rbx
        popq    %rax
        ret

dump_x_postblank:
        pushq   %rcx
                                # where did we stop
        orb     $0x0f,%cl       # end of paragraph
        cmpq    %rcx,%rdx       # or %rdx?
          jge   9f              # if %rcx we are done.
        subq    %rdx,%rcx       # No, How much of paragraph we missed.
1:
        testb   %cl,%cl
          jz    9f
        call    as_u_DisplayInline
        .asciz  "   "
        decb    %cl
        jmp     1b

9:
        popq    %rcx
        ret

#
# dump_a_preblank fill in spaces before the hex portion of the dump, if the
#       address does not start on a paragraph boundary
#
dump_a_preblank:
        pushq   %rcx
        andb    $0x0f,%cl       # is this a paragraph boundaryl
1:
        test    %cl,%cl         # when cl is zero we are done
          jz    9f
        call    as_u_Blank
        decb    %cl
        jmp     1b

9:
        popq    %rcx
        ret
#
# dump_a dumps the printable ascii portion of the line.
#
dump_a:
        pushq   %rax
        pushq   %rbx
        pushq   %rcx
        pushq   %rdx
        pushq   %rsi
        pushq   %rdi

# first determine where we will stop
        movq    %rcx,%rbx       # end of paragraph = %rcx | 0x0f
        orb     $0x0f,%bl
        cmpq    %rbx,%rdx       # however, cannot be bigger than %rdx
          jg    1f
        movq    %rdx,%rbx       # if it is, then use %rdx.
# are we done? That is %rcx bigger than %rbx?
1:
        cmpq    %rcx,%rbx
          jl    9f
        movb    (%rcx),%al      # get byte.
# can this byte be displayed or not?
        cmpb    $' ',%al
          jl    4f
        cmpb    $'~',%al
          jg    4f
        movq    $sys_write,%rax # let Linux write a 1 byte block.
        movq    $STDOUT,%rdi    # to stdout
        movq    %rcx,%rsi       # the block being at %rcx
        xorq    %rdx,%rdx       # 1 is the count
        incq    %rdx
        pushq   %rcx
        syscall
        popq    %rcx
        jmp     7f

4:
        call    as_u_DisplayInline
        .asciz  "."

7:
        incq    %rcx            # %rcx to point to next
        jmp     1b

9:
        popq    %rdi
        popq    %rsi
        popq    %rdx
        popq    %rcx
        popq    %rbx
        popq    %rax
        ret

dump_chk_same:
        pushq   %rcx
        pushq   %rsi
        pushq   %rdi
        pushq   %rdx
        movq    $1,%rax
        test    $0x0f,%cl       # %rcx start addr must be a paragraph boundary.
          jne   9f
        andb    $0xf0,%dl       # stop addr must be a paragraph boundary.
        subq    %rcx,%rdx       # bytes total
        cmpq    $32,%rdx        # needs to be >= 32 to make attempt
          jl    9f
        subq    $16,%rdx        # count 16 less for offset
        movq    %rcx,%rsi       # starting address block 1
        movq    %rcx,%rax       # copy of starting address block 1
        leaq    16(%rsi),%rdi   # starting offset block 2 = block 1 + 16
        movq    %rdx,%rcx       # set up count in %rcx
        cld                     # compare up
      repe cmpsb
        andb    $0xf0,%sil      # back to paragraph boundary.
        subq    %rax,%rsi       # bytes same
        shrq    $4,%rsi
        movq    %rsi,%rax       # lines that would be same.
        cmpq    $3,%rax         # must be at least three.
          jge   9f
        movq    $1,%rax
9:
        popq    %rdx
        popq    %rdi
        popq    %rsi
        popq    %rcx
        ret
        .page
#FUNCTION as_u_DumpReg: This dumps the callers registers. The normal
# working registers not floating point or MM
        .globl as_u_DumpRegs
as_u_DumpRegs:
        pushfq                  #also dumps flags
        pushq   %r15
        pushq   %r14            #put all registers on stack
        pushq   %r13
        pushq   %r12
        pushq   %r11
        pushq   %r10
        pushq   %r9
        pushq   %r8
        lea     80(%rsp),%r8    #we want what %rsp was
        push    %r8
        pushq   %rbp
        pushq   %rdi
        pushq   %rsi
        pushq   %rdx
        pushq   %rcx
        pushq   %rbx
        pushq   %rax
# all registers on stack
        movq    %rsp,%rbp
        call    1f              #address table of register names
        .ascii  "\nrax rbx rcx rdx\nrsi rdi rbp rsp"
        .ascii  "\n r8  r9 r10 r11\nr12 r13 r14 r15"
        .ascii  "\n  F rip"
1:
        popq    %rbx
        movw    $18,%cx         #18 things to be printed
1:                              # let us print the text part
        movq    %rbx,%rsi       # address of text to print
        movq    $4,%rdx         # 4 bytes to display each text
        movq    $STDOUT,%rdi    # to the standard output
        movq    $sys_write,%rax # we do want to write
        pushq   %rcx            #%rcx not saved
        syscall
        popq    %rcx
        call    as_u_DisplayInline
        .asciz  "="
# now to display the register
        movq    (%rbp),%rax     #rbp is address of
        call    as_u_Phex16
# next text and next reg
        addq    $4,%rbx
        addq    $8,%rbp
          loop  1b              #till cx is zero
        call    as_u_NL
# we used any register we wanted cause every thing is on the stack
        popq    %rax
        popq    %rbx
        popq    %rcx
        popq    %rdx
        popq    %rsi
        popq    %rdi
        popq    %rbp
        addq    $8,%rsp         #nor do we wish to pop this, so skip
        popq    %r8
        popq    %r9
        popq    %r10
        popq    %r11
        popq    %r12
        popq    %r13
        popq    %r14
        popq    %r15
        popfq
        ret
