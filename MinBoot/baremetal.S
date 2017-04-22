
    // コア0以外は停止させる
core_barrier:
    mrs   x0, mpidr_el1
    ldr   x1, =0xff00ffffff
    and   x0, x0, x1
    cmp   x0, 0x0
    b.eq  core0
sleep:
    wfi
    b     sleep
core0:

    // GPIO16の機能をGPIO出力に設定する
gpio16_output:
    ldr   x2, =0x3f200004   // GPFSEL1の現在の値を取得する
    ldr   w0, [x2]
    mov   w1, #7            // GPIO16の機能選択ビット以外をマスクする
    lsl   w1, w1, #18
    mvn   w1, w1
    and   w0, w0, w1
    mov   w1, #1            // GPIO16の機能選択ビットをGPIO出力にする
    lsl   w1, w1, #18
    orr   w0, w0, w1
    str   w0, [x2]          // GPFSEL1に値を設定する

    // GPIO16の出力をHighにする
gpio16_high:
    ldr   x2, =0x3f20001c   // GPSET0に値を設定する
    mov   w1, #1
    lsl   w1, w1, #16
    str   w1, [x2]

loop:
    b     loop

