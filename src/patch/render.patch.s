.target main150
.asm
;;; sub_7100C65F5C(sead::TaskBase *a1, __int64 x1_0) some kind of render function
;;; Patch a branch to be calling ComputeDebugData
0x00C661EC BL ksys::igd::ComputeDebugData

;;; Main Print Hack
; S8 = +offset
; S9 = -offset
; textWriter [SP, #0x430]
; x [SP, #0x458]
; y [SP, #0x45C]
0x00C662AC FMOV S8,  #1.0            ; Modify the offset so the text is more visible
0x00C662B0 FMOV S9,  #-1.0
0x00C662B4 FADD S10, S0, S9          ; x:S10 = -2
                                     ; y:S11 = -2
                                     ; Load Color = Black
0x00C66300 BL ksys::igd::RenderDebugScreen    ; Draw x = -2, y = -2
0x00C66308 FADD S12, S10, S8         ; x:S12 = 0
0x00C6630C NOP                       ; 
0x00C66310 NOP
0x00C66314 NOP      
                                     ; Save y
0x00C6631C STR  S12, [SP, #0x458]    ; Save x 
0x00C66320 NOP                       ; No color change
0x00C66324 NOP
0x00C66328 NOP
0x00C6632C NOP
0x00C66344 BL ksys::igd::RenderDebugScreen    ; Draw x = 0, y = -2
0x00C66348 FMOV S10, #0.0            ; Patch out the original line offset
0x00C6634C FMOV S0,  S11             ; y:S0 = -2
0x00C66374 NOP                       ; Skip string constructor
                                     ; x:S0 = 0
                                     ; y:S1 = -2
0x00C66380 FMOV S12, S1              ; y:S12 = -2
0x00C66384 FADD S11, S0, S8          ; x:S11 = 2
0x00C663C8 BL ksys::igd::RenderDebugScreen     ; Draw x = 2, y = -2
0x00C663D0 NOP                       ; No x change
                                     ; y:S12 = 0
0x00C663D8 NOP                       ; No color change
0x00C663DC NOP
0x00C663E8 NOP
0x00C663EC NOP
0x00C663F0 NOP
0x00C663F4 NOP
0x00C6640C BL ksys::igd::RenderDebugScreen     ; Draw x = 2, y = 0
0x00C66440 NOP                       ; Skip string constructor
                                     ; x:S0 = 2
                                     ; y:S1 = 0
0x00C6644C FADD S12, S1, S8          ; y:S12 = 2
0x00C66450 FMOV S11, S0              ; x:S11 = 2
0x00C66498 BL ksys::igd::RenderDebugScreen     ; Draw x = 2, y = 2
                                     ; x:S11 = 0
0x00C664A0 NOP                       ; No y change
0x00C664AC NOP                       ; No color change
0x00C664B0 NOP
0x00C664B4 NOP
0x00C664B8 NOP
0x00C664BC NOP
0x00C664C0 NOP
0x00C664C4 NOP
0x00C664C8 NOP
0x00C664CC NOP
0x00C664D0 NOP
0x00C664E8 BL ksys::igd::RenderDebugScreen     ; Draw x = 0, y = 2
0x00C664EC FADD S11, S11, S9         ; x:S11 = -2
                                     ; Save x change
0x00C6650C STR  S12, [SP, #0x45C]    ; Save y
0x00C6651C ADD  X0,  SP, #0x430      ; Load textWriter
0x00C66520 BL ksys::igd::RenderDebugScreen     ; Draw x = -2, y = 2 (Extra Call)
0x00C66524 NOP                       ; Skipping reloading x and y
0x00C66528 NOP                                     
0x00C6652C FMOV S10, S11             ; x:S10 = -2
0x00C66534 FADD S11, S12, S9         ; y:S11 = 0
0x00C6656C BL ksys::igd::RenderDebugScreen     ; Draw x = -2, y = 0
0x00C66570 FADD S0,  S10, S8         ; x:S0 = 0
0x00C66574 FMOV S1,  S11             ; y:S1 = 0
0x00C665B8 BL ksys::igd::RenderDebugScreen     ; Draw x = 0, y = 0
