__SP_H__ = 0x3e
__SP_L__ = 0x3d
__SREG__ = 0x3f
__tmp_reg__ = 0
__zero_reg__ = 1

.global get_port
    .type get_port, @function
get_port:

    push r28
    push r29
    rcall .
    rcall .
    in r28,__SP_L__
    in r29,__SP_H__
.L__stack_usage = 6
    std Y+2,r25
    std Y+1,r24
    std Y+4,r23
    std Y+3,r22
    ldi r24,lo8(43)
    ldi r25,0
    ldi r18,lo8(43)
    ldi r19,0
    movw r30,r18
    ld r18,Z
    ori r18,lo8(16)
    movw r30,r24
    st Z,r18
    ldi r24,lo8(72)
    ldi r25,0
    ldd r18,Y+3
    movw r30,r24
    st Z,r18
    pop __tmp_reg__
    pop __tmp_reg__
    pop __tmp_reg__
    pop __tmp_reg__
    pop r29
    pop r28
    ret
    .size   get_port, .-get_port
