
;
; toggle.asm

; 
; Slowly toggles all the data lines on PIA port B (PB0-PB7). You should 
; be able to detect a voltage alternating between ~0V and ~5V every 
; few seconds, with a meter connected between any of the data 
; pins and ground.
; Runs from high ROM ($FF00).
; If this works then: CPU works, read cycles work, write cycles work, 
;   EPROM decode/read works, PIA works.
; Not tested: system RAM read/write, memory refresh, other address
;   decode.
;
; Ben Zotto 2024
; 
; Adapted for SBASM by Andrew Shapton 2025
;
        .CR   6800            Select the MC6800 Cross-Overlay
        .TF   TESTROM.BIN,BIN    Send code in binary format to
                              ; this file
        .OR   $FFD0           Program starts at this address

piadat  .EQ     $f042
piacr   .EQ     $f043


entry   CLR     piacr   ; setup PIA port B for output
        LDAA    #$ff
        STAA    piadat
        LDAA    #$04
        STAA    piacr
        CLRB            ; start the outputs low
loop    STAA    piadat  ; update pia outputs
        BSR     delay   ; wait a few seconds
        COMB            ; flip the bits
        BRA     loop    ; do it again
delay   LDX     #$ffff
        LDAA    #5
delay1  DEX
        BNE     delay1
        DECA
        BNE     delay1
        RTS
        
        NOP
        NOP
        NOP
        NOP
        NOP
        NOP

irq     .DW     entry
int     .DW     entry
nmi     .DW     entry
rst     .DW     entry

