 .global psp_func1
 psp_func1:
 
 lbz     %r0, 0(%r4)
 addi    %r7, %r4, 0x12
 lbz     %r9, 1(%r4)
 addi    %r6, %r4, 0x32
 lbz     %r11, 2(%r4)
 rldicl  %r7, %r7, 0,32
 lbz     %r10, 3(%r4)
 rldicl  %r6, %r6, 0,32
 stb     %r0, 0(%r3)
 stb     %r9, 1(%r3)
 stb     %r11, 2(%r3)
 stb     %r10, 3(%r3)
 lbz     %r0, 4(%r4)
 lbz     %r9, 5(%r4)
 lbz     %r11, 6(%r4)
 lbz     %r10, 7(%r4)
 stb     %r0, 4(%r3)
 stb     %r9, 5(%r3)
 stb     %r11, 6(%r3)
 stb     %r10, 7(%r3)
 lbz     %r0, 8(%r4)
 lbz     %r9, 9(%r4)
 lbz     %r11, 0xA(%r4)
 lbz     %r10, 0xB(%r4)
 stb     %r0, 8(%r3)
 stb     %r10, 0xB(%r3)
 stb     %r9, 9(%r3)
 stb     %r11, 0xA(%r3)
 lbz     %r11, 0xE(%r4)
 lbz     %r8, 0xF(%r4)
 lbz     %r0, 0xC(%r4)
 lbz     %r9, 0xD(%r4)
 stb     %r11, 0xE(%r3)
 mr      %r11, %r4
 stb     %r8, 0xF(%r3)
 stb     %r0, 0xC(%r3)
 stb     %r9, 0xD(%r3)
 lbzu    %r9, 0x10(%r11)
 addi    %r8, %r4, 0x14
 lbz     %r0, 1(%r11)
 clrlwi  %r9, %r9, 24
 rldicl  %r8, %r8, 0,32
 slwi    %r0, %r0, 8
 or      %r0, %r0, %r9
 sth     %r0, 0x10(%r3)
 lbz     %r9, 1(%r7)
 lbz     %r0, 0(%r7)
 addi    %r7, %r4, 0x18
 slwi    %r9, %r9, 8
 rldicl  %r7, %r7, 0,32
 or      %r9, %r9, %r0
 sth     %r9, 0x12(%r3)
 lbz     %r11, 3(%r8)
 lbz     %r10, 1(%r8)
 lbz     %r9, 2(%r8)
 slwi    %r11, %r11, 24
 lbz     %r0, 0(%r8)
 slwi    %r10, %r10, 8
 slwi    %r9, %r9, 16
 or      %r0, %r0, %r10
 or      %r9, %r9, %r11
 addi    %r8, %r4, 0x1C
 or      %r0, %r0, %r9
 rldicl  %r8, %r8, 0,32
 stw     %r0, 0x14(%r3)
 lbz     %r11, 3(%r7)
 lbz     %r10, 1(%r7)
 lbz     %r9, 2(%r7)
 slwi    %r11, %r11, 24
 lbz     %r0, 0(%r7)
 slwi    %r10, %r10, 8
 slwi    %r9, %r9, 16
 or      %r0, %r0, %r10
 or      %r9, %r9, %r11
 addi    %r7, %r4, 0x20
 or      %r0, %r0, %r9
 rldicl  %r7, %r7, 0,32
 stw     %r0, 0x18(%r3)
 lbz     %r11, 3(%r8)
 lbz     %r10, 1(%r8)
 lbz     %r9, 2(%r8)
 slwi    %r11, %r11, 24
 lbz     %r0, 0(%r8)
 slwi    %r10, %r10, 8
 slwi    %r9, %r9, 16
 or      %r0, %r0, %r10
 or      %r9, %r9, %r11
 addi    %r8, %r4, 0x24
 or      %r0, %r0, %r9
 rldicl  %r8, %r8, 0,32
 stw     %r0, 0x1C(%r3)
 lbz     %r11, 3(%r7)
 lbz     %r10, 1(%r7)
 lbz     %r9, 2(%r7)
 slwi    %r11, %r11, 24
 lbz     %r0, 0(%r7)
 slwi    %r10, %r10, 8
 slwi    %r9, %r9, 16
 or      %r0, %r0, %r10
 or      %r9, %r9, %r11
 addi    %r7, %r4, 0x28
 or      %r0, %r0, %r9
 rldicl  %r7, %r7, 0,32
 stw     %r0, 0x20(%r3)
 lbz     %r11, 3(%r8)
 lbz     %r10, 1(%r8)
 lbz     %r9, 2(%r8)
 slwi    %r11, %r11, 24
 lbz     %r0, 0(%r8)
 slwi    %r10, %r10, 8
 slwi    %r9, %r9, 16
 or      %r0, %r0, %r10
 or      %r9, %r9, %r11
 addi    %r11, %r4, 0x2A
 or      %r0, %r0, %r9
 rldicl  %r11, %r11, 0,32
 stw     %r0, 0x24(%r3)
 lbz     %r9, 1(%r7)
 addi    %r10, %r4, 0x2C
 lbz     %r0, 0(%r7)
 addi    %r8, %r4, 0x2E
 slwi    %r9, %r9, 8
 rldicl  %r10, %r10, 0,32
 or      %r9, %r9, %r0
 rldicl  %r8, %r8, 0,32
 sth     %r9, 0x28(%r3)
 lbz     %r0, 1(%r11)
 lbz     %r9, 0(%r11)
 slwi    %r0, %r0, 8
 or      %r0, %r0, %r9
 sth     %r0, 0x2A(%r3)
 lbz     %r9, 1(%r10)
 lbz     %r0, 0(%r10)
 addi    %r10, %r4, 0x30
 slwi    %r9, %r9, 8
 rldicl  %r10, %r10, 0,32
 or      %r9, %r9, %r0
 sth     %r9, 0x2C(%r3)
 lbz     %r11, 1(%r8)
 lbz     %r0, 0(%r8)
 slwi    %r11, %r11, 8
 or      %r11, %r11, %r0
 sth     %r11, 0x2E(%r3)
 lbz     %r0, 1(%r10)
 lbz     %r9, 0(%r10)
 slwi    %r0, %r0, 8
 or      %r0, %r0, %r9
 sth     %r0, 0x30(%r3)
 lbz     %r9, 1(%r6)
 lbz     %r0, 0(%r6)
 slwi    %r9, %r9, 8
 or      %r9, %r9, %r0
 sth     %r9, 0x32(%r3)
 blr
 

 

