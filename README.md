# attilabo

The hardware design you can find here:
https://oshwlab.com/viewsonic97q/ikea_train

The demo videos you can find here:
https://www.youtube.com/watch?v=yAJqygvw9WM

## Attiny13 version

### Wiring
LED_PIN is PB2 (PIN7 on chip)
IR_IN_PIN is PB1 (PIN6 on chip)

### Fuses
Pay attention about fuses. Default Attiny13 chip has CKDIV8 programmed (FUSE_L=0x6A) and you should unprogramm it.
The fuses must be:
```
FUSE_L=0x7A
FUSE_H=0xFF
```

To read the fuses you can use:
```
C:\Program Files (x86)\Arduino\hardware\tools\avr\bin>avrdude -v -p attiny13 -P COM22 -b 19200 -c stk500v1 -C ..\etc\avrdude.conf -U hfuse:r:-:h -U lfuse:r:-:h
```

To programm the fuses:
```
C:\Program Files (x86)\Arduino\hardware\tools\avr\bin>avrdude -v -p attiny13 -P COM22 -b 19200 -c stk500v1 -C ..\etc\avrdude.conf -U hfuse:w:0xFF:m -U lfuse:w:0
x7A:m
```

## Attiny85 version

###  Wiring
LED_PIN is PB1 (PIN6 on chip)
IR_IN_PIN is PB2 (PIN7 on chip)

### Fuses

To read the fuses you can use:
```
C:\Program Files (x86)\Arduino\hardware\tools\avr\bin>avrdude -v -p attiny85 -P COM22 -b 19200 -c stk500v1 -C ..\etc\avrdude.conf -U hfuse:r:-:h -U lfuse:r:-:h
```
