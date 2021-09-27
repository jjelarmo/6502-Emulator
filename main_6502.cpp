#include <stdio.h>
#include <stdlib.h>
#include <cstdint>

using BYTE = unsigned char;     //1 byte
using Word = unsigned short;    //2 bytes
using u32  = unsigned int;      //4 bytes


struct Memory{
    static constexpr u32 MAX_MEM = 1024 * 64;
    BYTE Data[MAX_MEM];

    void Initialize(){
        for (u32 i=0; i<= MAX_MEM; i++){
            Data[i]=0;
        }
    }

    BYTE operator[](u32 address){
        return Data[address];
    }

};

struct CPU{

    Word PC;
    Word SP;

    BYTE A, X, Y;   //Accumulator, X,Y registers
    BYTE C : 1;
    BYTE Z : 1;
    BYTE I : 1;
    BYTE D : 1;
    BYTE B : 1;
    BYTE V : 1;
    BYTE N : 1;

    void Reset( Memory& memory){
        PC = 0XFFFC;
        SP = 0X0100;
        D = 0;
        memory.Initialize();

    }

    //OPCODES
    static constexpr BYTE INS_LDA_IM = 0xA9;
    static constexpr BYTE INS_LDA_ZP = 0xA5;

    void LDASetup(){
        Z = (A == 0);
        N = (A & 0b10000000) > 0; 
    }


    BYTE Fetch_byte(u32& Cycles, Memory& memory ){
        BYTE Data = memory[PC];
        PC++;
        Cycles--;
        return Data;
    }

    BYTE Read_byte(u32& Cycles, Memory& memory, BYTE address){        
        BYTE Data = memory[address];
        Cycles--;
        return Data;
    }

    void Execute(u32 Cycles, Memory& memory){
        while( Cycles >0){
            BYTE Instruction = Fetch_byte(Cycles, memory);
            switch (Instruction){
                case INS_LDA_IM:
                {
                    BYTE Value = Fetch_byte (Cycles, memory);
                    A = Value;    
                    LDASetup();
                } break;
                case INS_LDA_ZP:
                {
                    BYTE ZPAddress = Read_byte(Cycles, memory, Instruction);
                    A = Read_byte(Cycles, memory, ZPAddress);
                    LDASetup();
                }break;
            }
        }
    }

};

int main(){

    CPU cpu;
    Memory mem;
    cpu.Reset(mem);
    cpu.Execute(2, mem);

    return 0;

}