00001 $0000                ;
00002 $0000                ; toggle.asm

00003 $0000                ; 
00004 $0000                ; Slowly toggles all the data lines on PIA port B (PB0-PB7). You should 
00005 $0000                ; be able to detect a voltage alternating between ~0V and ~5V every 
00006 $0000                ; few seconds, with a meter connected between any of the data 
00007 $0000                ; pins and ground.
00008 $0000                ;
00009 $0000                ; Runs from high ROM ($FF00).
00010 $0000                ;
00011 $0000                ; If this works then: CPU works, read cycles work, write cycles work, 
00012 $0000                ;   EPROM decode/read works, PIA works.
00013 $0000                ; Not tested: system RAM read/write, memory refresh, other address 
00014 $0000                ;   decode.
00015 $0000                ;
00016 $0000                ; Ben Zotto 2024 
00017 $0000                ;
00018 $0000                
00019 $0000                
00020 $0000                piadat  equ     $f042
00021 $0000                piacr   equ     $f043
00022 $0000                
00023 $0000                        org     $ff00
00024 $FF00                
00025 $FF00  7F F0 43       entry   clr     piacr   ; setup PIA port B for output
00026 $FF03  86 FF                  lda a   #$ff
00027 $FF05  B7 F0 42               sta a   piadat
00028 $FF08  86 04                  lda a   #$04    
00029 $FF0A  B7 F0 43               sta a   piacr        
00030 $FF0D                        
00031 $FF0D  5F                     clr b           ; start the outputs low
00032 $FF0E  F7 F0 42       loop    sta b   piadat  ; update pia outputs
00033 $FF11  8D 03                  bsr     delay   ; wait a few seconds
00034 $FF13  53                     com b           ; flip the bits
00035 $FF14  20 F8                  bra     loop    ; do it again
00036 $FF16                        
00037 $FF16  CE FF FF       delay   ldx     #$ffff   
00038 $FF19  86 05                  lda a   #5
00039 $FF1B  09             delay1  dex
00040 $FF1C  26 FD                  bne     delay1
00041 $FF1E  4A                     dec a
00042 $FF1F  26 FA                  bne     delay1
00043 $FF21  39                     rts
00044 $FF22                        
00045 $FF22                        ; All vectors point to the start of the program
00046 $FF22                        
00047 $FF22                        org     $fff8
00048 $FFF8                        
00049 $FFF8  FF 00          irq     fdb     entry
00050 $FFFA  FF 00          int     fdb     entry
00051 $FFFC  FF 00          nmi     fdb     entry
00052 $FFFE  FF 00          rst     fdb     entry
