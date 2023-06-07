#define DEBUG
// SILENT  		- 	prints PC and regs at end of all instructions
// VERBOSE 		- 	prints PC and regs at end of each instruction
// DEBUG 		- 	prints miscellaneous information
// DEBUGMEM 	- 	Prints Memory Contents
// STEPVERBOSE 	- 	Prints PC and regs at end of each instruction and needs user to press a key followed by "ENTER" to proceed to next instruction
#include <sstream>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <cstdint>
#include <vector>
#include <bitset>
#include <unordered_map>
#include <bits/stdc++.h>
#include "declarations.h"					// Contains Memory, Constants and Instruction Fields Struct Declarations
// #include "BaseExtendFunctions.h"			// Contains Definition of PC/Memory/Register Read/Write functions, RV32I,RV32M Functions.
// #include "PREload.h"						// File to Preload the registers and memory locations for Function Specific testing

/***************************************** DECODE FUNCTIONS SPECIFIC TO INSTRUCTION TYPE *****************************************/

 void set_pc(uint32_t pcvalue) { pc = pcvalue; }												// Getters and setters for the program counter
    uint32_t get_pc() { return pc; }

    void set_reg(uint32_t regindex, uint32_t regvalue) { RegisterFile[regindex] = regvalue; }	// Getters and setters for general-purpose registers
    uint32_t get_reg(uint32_t regindex) { return RegisterFile[regindex]; }

uint32_t RtypeDecode(struct InstrFields *Fields, uint32_t instruction){
	uint32_t instrtemp;								// temporary variable to store the Instruction from file
	instrtemp 		= instruction;  				// assigning the temporary variable with the Instruction
	Fields->opcode 	= instrtemp & 0x0000007F; 		// masking instrtemp with mask value to get value of Opcode
	instrtemp 		= instruction;  				// assigning the temporary variable with the Instruction
	instrtemp 		= instrtemp >> 7; 				// shifting instrtemp by 6 positions to get RD
	Fields->rd 		= instrtemp & 0x0000001F; 		// masking instrtemp with mask value to get value of RD
	instrtemp  		= instruction; 					// assigning  Instruction again to temporary variable
    instrtemp		= instrtemp >> 12; 				// shifting instrtemp by 12 positions to get funct3
    Fields->funct3 	= instrtemp & 0x00000007; 		// masking instrtemp with a mask value to get value of funct3
    instrtemp  		= instruction; 					// assigning  Instruction again to temporary variable
    instrtemp 		= instrtemp >> 15; 				// shifting instrtemp by 15 positions to get rs1
    Fields->rs1 	= instrtemp & 0x0000001F; 		// masking instrtemp with a mask value to get value of rs1
	instrtemp  		= instruction; 					// assigning  Instruction again to temporary variable
    instrtemp 		= instrtemp >> 20; 				// shifting instrtemp by 20 positions to get rs2
    Fields->rs2 	= instrtemp & 0x0000001F; 		// masking instrtemp with a mask value to get value of rs2
    instrtemp  		= instruction; 					// assigning  Instruction again to temporary variable
    instrtemp 		= instrtemp >> 25; 				// shifting instrtemp by 25 positions to get funct7
    Fields->funct7 	= instrtemp & 0X0000007F; 		// masking instrtemp with a mask value to get value of funct7
	#ifdef DEBUG
		printf("***R-TYPE:0x%08x****\nfunct7\t=0x%02x\n,rs2\t=0x%02x\n,rs1\t=0x%02x\n,funct3\t=0x%01x\n,rd\t=0x%02x\n,opcode\t=0x%02x\n",instruction,Fields->funct7,Fields->rs2,Fields->rs1,Fields->funct3,Fields->rd,Fields->opcode);
	#endif
	//if(RtypeExecute(Fields, instruction)) return 1;
	//else return 0;
	return 1;
}

uint32_t ItypeDecode(struct InstrFields *Fields, uint32_t instruction){
	uint32_t instrtemp; 							// temporary variable to store the Instruction from file
	instrtemp 		= instruction;  				// assigning the temporary variable with the Instruction
	Fields->opcode 	= instrtemp & 0x0000007F; 		// masking instrtemp with mask value to get value of Opcode
	instrtemp 		= instruction;  				// assigning the temporary variable with the Instruction
	instrtemp 		= instrtemp >> 7; 				// shifting instrtemp by 6 positions to get RD
	Fields->rd 		= instrtemp & 0x0000001F; 		// masking instrtemp with mask value to get value of RD
	instrtemp  		= instruction; 					// assigning  Instruction again to temporary variable
    instrtemp		= instrtemp >> 12; 				// shifting instrtemp by 12 positions to get funct3
    Fields->funct3 	= instrtemp & 0x00000007; 		// masking instrtemp with a mask value to get value of funct3
    instrtemp  		= instruction; 					// assigning  Instruction again to temporary variable
    instrtemp 		= instrtemp >> 15; 				// shifting instrtemp by 15 positions to get rs1
    Fields->rs1 	= instrtemp & 0x0000001F; 		// masking instrtemp with a mask value to get value of rs1
	instrtemp  		= instruction; 					// assigning  Instruction again to temporary variable
    instrtemp 		= instrtemp >> 20; 				// shifting instrtemp by 20 positions to get shamt/imm[4:0]
    Fields->shamt 	= instrtemp & 0x0000001F; 		// masking instrtemp with a mask value to get value of shamt/imm[4:0]
	instrtemp  		= instruction; 					// assigning  Instruction again to temporary variable
    instrtemp 		= instrtemp >> 25; 				// shifting instrtemp by 25 positions to get imm[11:5]
    Fields->imm_I11_5= instrtemp & 0x0000007F; 		// masking instrtemp with a mask value to get value of imm[11:5]
	instrtemp  		= instruction; 					// assigning  Instruction again to temporary variable
    instrtemp 		= instrtemp >> 20; 				// shifting instrtemp by 20 positions to get imm[11:0]
    Fields->imm_I11_0 	= instrtemp & 0x00000FFF; 	// masking instrtemp with a mask value to get value of imm[11:0]
    #ifdef DEBUG
		printf("***I-TYPE:0x%08x****\nimm[11:0]\t=0x%03x\n,rs1\t=0x%02x\n,funct3\t=0x%01x\n,rd\t=0x%02x\n,opcode\t=0x%02x\n",instruction,Fields->imm_I11_0,Fields->rs1,Fields->funct3,Fields->rd,Fields->opcode);
	#endif
	// if(ItypeExecute(Fields, instruction)) return 1;
	// else return 0;
	return 1;
}

uint32_t StypeDecode(struct InstrFields *Fields, uint32_t instruction){
	uint32_t instrtemp,immtemp; 					// temporary variable to store the Instruction from file
	instrtemp 		= instruction;  				// assigning the temporary variable with the Instruction
	Fields->opcode 	= instrtemp & 0x0000007F; 		// masking instrtemp with mask value to get value of Opcode
	instrtemp 		= instruction;  				// assigning the temporary variable with the Instruction
	instrtemp 		= instrtemp >> 7; 				// shifting instrtemp by 6 positions to get imm[4:0]
	Fields->imm_S4_0= instrtemp & 0x0000001F; 		// masking instrtemp with mask value to get value of imm[4:0]
	instrtemp  		= instruction; 					// assigning  Instruction again to temporary variable
    instrtemp		= instrtemp >> 12; 				// shifting instrtemp by 12 positions to get funct3
    Fields->funct3 	= instrtemp & 0x00000007; 		// masking instrtemp with a mask value to get value of funct3
    instrtemp  		= instruction; 					// assigning  Instruction again to temporary variable
    instrtemp 		= instrtemp >> 15; 				// shifting instrtemp by 15 positions to get rs1
    Fields->rs1 	= instrtemp & 0x0000001F; 		// masking instrtemp with a mask value to get value of rs1
	instrtemp  		= instruction; 					// assigning  Instruction again to temporary variable
    instrtemp 		= instrtemp >> 20; 				// shifting instrtemp by 20 positions to get rs2
    Fields->rs2 	= instrtemp & 0x0000001F; 		// masking instrtemp with a mask value to get value of rs2
    instrtemp  		= instruction; 					// assigning  Instruction again to temporary variable
    instrtemp 		= instrtemp >> 25; 				// shifting instrtemp by 25 positions to get imm[11:5]
    Fields->imm_S11_5 = instrtemp & 0X0000007F; 	// masking instrtemp with a mask value to get value of imm[11:5]
	immtemp 		= Fields->imm_S11_5 << 5;		// combining parts of immediate field to form a single immediate field
	immtemp			= immtemp | Fields->imm_S4_0;
	Fields->imm_S11_0 = immtemp;
	#ifdef DEBUG
		printf("***S-TYPE:0x%08x****\nimm[11:0]\t=0x%02x\n,imm[11:5]\t=0x%02x\n,rs2\t=0x%02x\n,rs1\t=0x%02x\n,funct3\t=0x%01x\n,imm[4:0]\t=0x%02x\n,opcode\t=0x%02x\n",instruction,Fields->imm_S11_0,Fields->imm_S11_5,Fields->rs2,Fields->rs1,Fields->funct3,Fields->imm_S4_0,Fields->opcode);
	#endif
	// if(StypeExecute(Fields, instruction)) return 1;
	// else return 0;
	return 1;
}

uint32_t BtypeDecode(struct InstrFields *Fields, uint32_t instruction){
	uint32_t instrtemp,immtemp; 					// temporary variable to store the Instruction from file
	instrtemp 		= instruction;  				// assigning the temporary variable with the Instruction
	Fields->opcode 	= instrtemp & 0x0000007F; 		// masking instrtemp with mask value to get value of Opcode
	instrtemp 		= instruction;  				// assigning the temporary variable with the Instruction
	instrtemp 		= instrtemp >> 7; 				// shifting instrtemp by 6 positions to get imm[11]
	Fields->imm_B11 = instrtemp & 0x00000001; 		// masking instrtemp with mask value to get value of imm[11]
	instrtemp 		= instruction;  				// assigning the temporary variable with the Instruction
	instrtemp 		= instrtemp >> 8; 				// shifting instrtemp by 6 positions to get imm[4:1]
	Fields->imm_B4_1= instrtemp & 0x0000000F; 		// masking instrtemp with mask value to get value of imm[4:1]
	instrtemp  		= instruction; 					// assigning  Instruction again to temporary variable
    instrtemp		= instrtemp >> 12; 				// shifting instrtemp by 12 positions to get funct3
    Fields->funct3 	= instrtemp & 0x00000007; 		// masking instrtemp with a mask value to get value of funct3
    instrtemp  		= instruction; 					// assigning  Instruction again to temporary variable
    instrtemp 		= instrtemp >> 15; 				// shifting instrtemp by 15 positions to get rs1
    Fields->rs1 	= instrtemp & 0x0000001F; 		// masking instrtemp with a mask value to get value of rs1
	instrtemp  		= instruction; 					// assigning  Instruction again to temporary variable
    instrtemp 		= instrtemp >> 20; 				// shifting instrtemp by 20 positions to get rs2
    Fields->rs2 	= instrtemp & 0x0000001F; 		// masking instrtemp with a mask value to get value of rs2
    instrtemp  		= instruction; 					// assigning  Instruction again to temporary variable
    instrtemp 		= instrtemp >> 25; 				// shifting instrtemp by 25 positions to get imm[10:5]
    Fields->imm_B10_5 = instrtemp & 0X0000003F; 	// masking instrtemp with a mask value to get value of imm[10:5]
	instrtemp  		= instruction; 					// assigning  Instruction again to temporary variable
    instrtemp 		= instrtemp >> 31; 				// shifting instrtemp by 25 positions to get imm[11:5]
    Fields->imm_B12 = instrtemp & 0X00000001; 		// masking instrtemp with a mask value to get value of imm[11:5]
	immtemp 		= Fields->imm_B12 << 1;			// combining parts of immediate field to form a single immediate field
	immtemp			= immtemp | Fields->imm_B11;
	immtemp			= immtemp << 6;
	immtemp			= immtemp | Fields->imm_B10_5;
	immtemp			= immtemp << 4;
	immtemp			= immtemp | Fields->imm_B4_1;
	Fields->imm_B12_1 = immtemp;
	#ifdef DEBUG
		printf("***B-TYPE:0x%08x****\nimm[12:1]\t=%0x\n,imm[12]\t=%0x\n,imm[10:5]\t=%0x\n,rs2\t=%0x\n,rs1\t=%0x\n,funct3\t=%0x\n,imm[4:1]\t=%0x\n,imm[11]\t=%0x\n,opcode\t=%0x\n",instruction,Fields->imm_B12_1,Fields->imm_B12,Fields->imm_B10_5,Fields->rs2,Fields->rs1,Fields->funct3,Fields->imm_B4_1,Fields->imm_B11,Fields->opcode);
	#endif
	// if(BtypeExecute(Fields, instruction)) return 1;
	// else return 0;
	return 1;
}

uint32_t UtypeDecode(struct InstrFields *Fields, uint32_t instruction){
	uint32_t instrtemp; 							// temporary variable to store the Instruction from file
	instrtemp 		= instruction;  				// assigning the temporary variable with the Instruction
	Fields->opcode 	= instrtemp & 0x0000007F; 		// masking instrtemp with mask value to get value of Opcode
	instrtemp 		= instruction;  				// assigning the temporary variable with the Instruction
	instrtemp 		= instrtemp >> 7; 				// shifting instrtemp by 6 positions to get RD
	Fields->rd 		= instrtemp & 0x0000001F; 		// masking instrtemp with mask value to get value of RD
	instrtemp  		= instruction; 					// assigning  Instruction again to temporary variable
    instrtemp		= instrtemp >> 12; 				// shifting instrtemp by 12 positions to get imm[31:12]
    Fields->imm_U31_12 	= instrtemp & 0x000FFFFF; 	// masking instrtemp with a mask value to get value of imm[31:12]
	#ifdef DEBUG
		printf("***U-TYPE:0x%08x****\nimm[31:12]\t=0x%05x\n,rd\t=0x%02x\n,opcode\t=0x%02x\n",instruction,Fields->imm_U31_12,Fields->rd,Fields->opcode);
	#endif
	// if(UtypeExecute(Fields, instruction)) return 1;
	// else return 0;
	return 1;
}

uint32_t JtypeDecode(struct InstrFields *Fields, uint32_t instruction){
	uint32_t instrtemp,immtemp; 					// temporary variable to store the Instruction from file
	instrtemp 		= instruction;  				// assigning the temporary variable with the Instruction
	Fields->opcode 	= instrtemp & 0x0000007F; 		// masking instrtemp with mask value to get value of Opcode
	instrtemp 		= instruction;  				// assigning the temporary variable with the Instruction
	instrtemp 		= instrtemp >> 7; 				// shifting instrtemp by 6 positions to get RD
	Fields->rd 		= instrtemp & 0x0000001F; 		// masking instrtemp with mask value to get value of RD
	instrtemp 		= instruction;  				// assigning the temporary variable with the Instruction
	instrtemp 		= instrtemp >> 12; 				// shifting instrtemp by 6 positions to get imm[19:12]
	Fields->imm_J19_12= instrtemp & 0x000000FF; 	// masking instrtemp with mask value to get value of imm[19:12]
	instrtemp  		= instruction; 					// assigning  Instruction again to temporary variable
    instrtemp		= instrtemp >> 20; 				// shifting instrtemp by 12 positions to get imm[11]
    Fields->imm_J11 = instrtemp & 0x00000001; 		// masking instrtemp with a mask value to get value of imm[11]
    instrtemp  		= instruction; 					// assigning  Instruction again to temporary variable
    instrtemp 		= instrtemp >> 21; 				// shifting instrtemp by 15 positions to get imm[10:1]
    Fields->imm_J10_1= instrtemp & 0x000003FF; 		// masking instrtemp with a mask value to get value of imm[10:1]
	instrtemp  		= instruction; 					// assigning  Instruction again to temporary variable
    instrtemp 		= instrtemp >> 31; 				// shifting instrtemp by 20 positions to get imm[20]
    Fields->imm_J20	= instrtemp & 0x00000001; 		// masking instrtemp with a mask value to get value of imm[20]
	immtemp 		= Fields->imm_J20 << 8;		// combining parts of immediate field to form a single immediate field
	immtemp			= immtemp | Fields->imm_J19_12;
	immtemp			= immtemp << 1;
	immtemp			= immtemp | Fields->imm_J11;
	immtemp			= immtemp << 10;
	immtemp			= immtemp | Fields->imm_J10_1;
	Fields->imm_J20_1 = immtemp;
	#ifdef DEBUG
		printf("***J-TYPE:0x%08x****\nimm[20]\t=0x%01x\n,imm[20]\t=0x%01x\n,imm[10:1]\t=0x%03x\n,imm[11]\t=0x%01x\n,imm[19:12]\t=0x%02x\n,rd\t=0x%02x\n,opcode\t=0x%02x\n",instruction,Fields->imm_J20_1,Fields->imm_J20,Fields->imm_J10_1,Fields->imm_J11,Fields->imm_J19_12,Fields->rd,Fields->opcode);
	#endif
	// if(JtypeExecute(Fields, instruction)) return 1;
	// else return 0;
	return 1;
}

/***************************************** MAIN FUNCTION FOR RISCV SIMULATOR *****************************************/

int main(int argc, char *argv[]) {

	const PRESCHEDULE_SIZE=10;
	const SCHEDULE_WIDTH=4
	const MAX_INSTRUCTIONS=10;
	const LOAD=0b0000011;
	const STORE=0b0100011;
	const IMM_INST=0b0010011;
	const INTEGER=0b0110011;
	const BRANCH=0b1100111;
	const STORAGE=40;

	InstrFields *arr[STORAGE];

	int size=0;
	
	string memoryimage= "program.mem";		// Default memory image file
	uint32_t max_pc;						// To track the Maximum value of PC supplied from mem file
	char choice;
	uint32_t count=0;

    if (argc > 4) {
        cerr << "***CHECK ARGUMENTS*** Incorrect Number of Arguments Provided. Should be Used as follows: \n" << argv[0] << " <pc> <sp> <memoryimage>" << endl;
        return EXIT_FAILURE;
    } else {																// Parse PC and SP as 32-bit hexadecimal values, and Memory Image File as a String
		if(argc>1){								
			stringstream ss_pc(argv[1]);
			ss_pc >> hex >> pc;	
		}
		if(argc>2){
			stringstream ss_sp(argv[2]);
			ss_sp >> hex >> sp;
			set_reg(2,sp);
		} 	
		if(argc>3){
			memoryimage = string(argv[3]);
		}	
	}
	
	#ifdef DEBUG
    cout << "PC = 0x" << setfill('0') << setw(8) << hex << pc << endl;		// printing the parsed arguments in DEBUG Mode
    cout << "SP = 0x" << setfill('0') << setw(8) << hex << sp << endl;
    cout << "Memory image filename = " << memoryimage << endl;
	#endif
	
	ifstream memory_file(memoryimage);										// Loading the Memory Image into Memory Map
	uint32_t address,data;
	if (memory_file.is_open()) {
        string line;
        while (getline(memory_file, line)) {
            stringstream ss(line);
            ss >> hex >> address;
			if(address%4 != 0){
				cerr << "***ILLEGAL PC PROVIDED IN MEMFILE*** Wrong Address detected in the Memory Image. Instruction Address Can only be a multiple of 4. Stopped at address 0x" << hex << address << endl;
				return 0;
			}
			if(address >= MEM_SIZE){
				cerr << "***MEMORY ADDRESS OUT OF BOUNDS*** Instruction Memory is limited to 64KB Wrong Address detected in the Memory Image. Stopped at address 0x" << hex << address << endl;
				return 0;
			}									
            ss.ignore(2, ' '); 												// Ignore the colon and space between address and data
            ss >> hex >> data;
            Memory[address] = data & 0xFF;
			Memory[address + 1] = (data >> 8)  & 0xFF;
			Memory[address + 2] = (data >> 16) & 0xFF;
			Memory[address + 3] = (data >> 24) & 0xFF;
			max_pc = address + 4;
		}
		#ifdef DEBUG
			printf("max_pc=0x%08x\n",max_pc);
		#endif	
        memory_file.close();
    } else {
        cerr << "***FILE ERROR*** Failed to open memory image file." << endl;
        return 0;
    }
	
	#ifdef DEBUG															// To preload Memory/Registers for Instruction Debug Purposes
		// PREload();
	#endif
	
	if(pc > (Memory.rbegin() -> first)){
		cerr <<"***WRONG INPUTS*** PC value is not within mem file range.Please enter a valid PC value in the First argument you supply" << endl;
		return 0;
	} else {
		for (pc ; pc < max_pc; pc = pc + 4) {
			uint32_t currentPC = pc;	
			if(currentPC % 4 == 0) {
				uint32_t currentINSTR = Memory[currentPC] | (Memory[currentPC + 1] << 8) | (Memory[currentPC + 2] << 16) | (Memory[currentPC + 3] << 24);
				if(currentINSTR == 0x00000000){
					cerr << "***TRAP*** Instruction at address:0x" << hex << currentPC << " is all 0's. Terminating the Simulation" << endl;
					return 0;
				} else {
					count ++;
					uint32_t opcode = currentINSTR & 0x7F;
						#ifdef VERBOSE
							cout << "******************************** Instruction: 0x" << hex << currentINSTR << " ***********************************\n" << endl;
						#endif
						#ifdef STEPVERBOSE
							cout << "\n****Executing Instruction #" << count <<"****\n\nChoose to print : Instruction(I/i),Registers(R/r),Memory(M/m), All (A/a), Only Instruction&Registers (B/b),\n Only Instruction&Memory (C/c),Only Registers&Memory (D/d),No(Any Other Key) ****\n" ;
							cin >> choice;
							if(choice == 'I' || choice == 'i' || choice == 'A' || choice == 'a' || choice == 'B' || choice == 'b' || choice == 'C' || choice == 'c' ){
								cout << "******************************** Instruction: 0x" << hex << currentINSTR << " ***********************************\n" << endl;
							}
						#endif
						switch(opcode){
							case 0b0110111: { if(!UtypeDecode(&Fields,currentINSTR)) 
												arr[size] = &Fields;
												return 0;	
												break;	// For LUI
											}
											
							case 0b0010111: { if(!UtypeDecode(&Fields,currentINSTR)) 
												arr[size] = &Fields;
												return 0;	
												break;	// For AUIPC
											}
							case 0b1101111: { if(!JtypeDecode(&Fields,currentINSTR)) 
											arr[size] = &Fields;
											return 0;	
											break;	// For JAL
											}
							case 0b1100111: { if(!ItypeDecode(&Fields,currentINSTR)) 
											arr[size] = &Fields;
											return 0;	
											break;	// For JALR
							}
							case 0b1100011: { if(!BtypeDecode(&Fields,currentINSTR)) 
											arr[size] = &Fields;
											return 0;	
											break; // For BEQ,BNE,BLT,BGE,BLTU,BGEU
							 }
							case 0b0000011: { if(!ItypeDecode(&Fields,currentINSTR))
											arr[size] = &Fields;
											return 0;	
											break; // For LB,LH,LW,LBU,LHU
							 				} 	
							case 0b0100011: { if(!StypeDecode(&Fields,currentINSTR)) 
											arr[size] = &Fields;
											return 0;	
											break; // For SB,SH,SW
							 				}
											
							case 0b0010011: { if(!ItypeDecode(&Fields,currentINSTR)) 
											arr[size] = &Fields;
											return 0;	
											break; // For ADDI,SLTI,SLTIU,ANDI,ORI,XORI,SLLI,SRLI,SRAI
							 				}	
							case 0b0110011: { if(!RtypeDecode(&Fields,currentINSTR)) 
											arr[size] = &Fields;
											return 0;	
											break; // For ADD,SLT,SLTU,AND,OR,XOR,SLL,SRL,SUB,SRA
							 				}
							default : 		cerr << "***ILLEGAL OPCODE*** Detected at address(hex): "<< hex << currentPC << "\tInstruction:"<< setfill('0') << setw(8) << hex << currentINSTR <<"\topcode(binary): " << bitset<7>(opcode) <<"\n"; 
											return 0; 
											break;

											size++;
						}
						#ifdef VERBOSE
							print_regs();
							cout << "*********************************** YOU DESERVE EXTRA CREDIT ***********************************\n" << endl;
						#endif
						#ifdef STEPVERBOSE
							if(choice == 'R' || choice == 'r'|| choice == 'B' || choice == 'b'){
								print_regs();
							} else if (choice == 'M' || choice == 'm'|| choice == 'C' || choice == 'c'){
								print_memory();
							} else if (choice == 'A' || choice == 'a' || choice == 'D' || choice == 'd'){
								print_regs();
								print_memory();
							} else {
								cout << "\n\nThanks...Proceeding to next instruction execution....\n\n" << endl;
							};
						#endif
				}
			} else {
				cerr << "***TRAP*** MISALIGNED INSTRUCTION ADDRESS is fetched at PC(Hex) :0x" << hex << pc << endl;
					return 0;
			}
		}
	}

InstrFields pre_arr [0:PRESCHEDULE_SIZE-1][SCHEDULE_WIDTH-1:0];
int dependency;
int a=0;
int k[PRESCHEDULE_SIZE-1];
int b=0;
int new_a;

int placement_logic(int a, int k[], InstrFields arr[], int i, InstrFields pre_arr[][]) {
	match=0;
	found=0;
	while(!found){
	for(int r=0; r < SCHEDULE_WIDTH; r++){
		if(arr[i] == pre_arr[a][r]){
			printf("Instruction already present");
			match=1;
		} else if((pre_arr[a][r] == 0) && !match && !found){
			new_a = a;
			new_k = r;
			found = 1;
		}
	}
		a++; match=0;
	}
	pre_arr[new_a][new_k] = arr[i];
	k[new_a] = (k[new_a] < SCHEDULE_WIDTH) ? k[new_a]+1 : 0;
	return new_a;
}

if((sizeof(arr)/sizeof(InstrFields)) == 10) {

	size=0;

for(int i=0; i<MAX_INSTRUCTIONS; i++) {
    // determining the execution latency

    dependency=0;
    if(arr[i]->opcode == LOAD || arr[i]->opcode == STORE || arr[i]->opcode == BRANCH)
    ex_lat = 4;
    else
    ex_lat = 1; // ADD, SUB, MUL

    for(int j=i+1; j<(j+ex_lat); j++) {
        if(arr[i]->rd == (arr[j]->rs1 | arr[j]->rs2)){
            printf("There is RAW data dependency between instruction %d and %d", i , j);
            dependency = 1;
		//	break;
        } else if((arr[i]->rs1 | arr[i]->rs2) == arr[j]->rd){
            printf("There is WAR data dependency between instruction %d and %d", i , j);
            dependency = 1;
			int war=1;
			break;
        } else if(arr[i]->rd == arr[j]->rd){
            printf("There is WAW data dependency between instruction %d and %d", i , j);
            dependency = 1;
		//	break;
        } else {
            printf("No dependency");
			dependency = 0;
			
		}
    }

    if(dependency){
					new_a = placement_logic_war(a, k, arr, i, pre_arr);
					if(war){
						for(int loop=0; loop<PRESCHEDULE_SIZE; loop++){
							if(k[loop] < SCHEDULE_WIDTH)
								a = loop;
						}	
					war = 0;
					}
					else {
						a = new_a + ex_lat;
					}
				}
    	}
}
	#ifdef SILENT
		cout << "************************************ SIMULATION SUMMARY ***************************************\n" << endl;
		print_regs();
		cout << "*********************************** YOU DESERVE EXTRA CREDIT ***********************************\n" << endl;
	#endif
	#ifdef DEBUGMEM
    print_memory();
	#endif
    return EXIT_SUCCESS;
	
}
// commands to use: g++ -o rv RV32I.cpp ; ./rv PC SP MEMFILE
