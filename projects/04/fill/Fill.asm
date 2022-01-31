// This file is part of www.nand2tetris.org
// and the book "The Elements of Computing Systems"
// by Nisan and Schocken, MIT Press.
// File name: projects/04/Fill.asm

// Runs an infinite loop that listens to the keyboard input.
// When a key is pressed (any key), the program blackens the screen,
// i.e. writes "black" in every pixel;
// the screen should remain fully black as long as the key is pressed. 
// When no key is pressed, the program clears the screen, i.e. writes
// "white" in every pixel;
// the screen should remain fully clear as long as no key is pressed.

(LOOP)
    // load keyboard
    @KBD
    D=M
    @ON
    D;JGT
    @OFF
    D;JEQ

    (ON)
        @R0
        M=-1
        @DRAW
        0;JMP

    (OFF)
        @R0
        M=0
        @DRAW
        0;JMP
    
    (DRAW)
        @8191
        D=A
        @R1
        M=D
        
        (NEXT)
            // set position
            @SCREEN
            D=A
            @R1
            A=D+M

            // draw color
            @R0
            M=R0

            @R1
            D=M-1
            @NEXT
            D;JLT

    @LOOP
    0;JMP