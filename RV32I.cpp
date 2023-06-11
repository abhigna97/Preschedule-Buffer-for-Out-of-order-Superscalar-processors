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
		const int PRESCHEDULE_SIZE=20;
        const int SCHEDULE_WIDTH=4;
        const int MAX_INSTRUCTIONS=12;
        const int LOAD=0b0000011;
        const int STORE=0b0100011;
        const int IMM_INST=0b0010011;
        const int INTEGER=0b0110011;
        const int BRANCH=0b1100011;

		uint32_t pre_arr [PRESCHEDULE_SIZE][SCHEDULE_WIDTH]={};
		int dependency;
		int a=0;
		int k[PRESCHEDULE_SIZE];
		int new_a=0;
		int new_k;
		int ex_lat;
		int war = 0;

int opcode_arr[PRESCHEDULE_SIZE],rs1_arr[PRESCHEDULE_SIZE],rs2_arr[PRESCHEDULE_SIZE],rd_arr[PRESCHEDULE_SIZE],instr_arr[PRESCHEDULE_SIZE];

/***************************************** DECODE FUNCTIONS SPECIFIC TO INSTRUCTION TYPE *****************************************/

 void set_pc(uint32_t pcvalue) { pc = pcvalue; }												// Getters and setters for the program counter
    uint32_t get_pc() { return pc; }

    void set_reg(uint32_t regindex, uint32_t regvalue) { RegisterFile[regindex] = regvalue; }	// Getters and setters for general-purpose registers
    uint32_t get_reg(uint32_t regindex) { return RegisterFile[regindex]; }

int fill_arrays(struct InstrFields *Fields, int size){
	opcode_arr[size] = Fields->opcode;
	rs1_arr[size] = Fields->rs1;
	rs2_arr[size] = Fields->rs2;
	rd_arr[size] = Fields->rd;
	instr_arr[size] = Fields->instr32bit;
	//for(int loop=0; loop<PRESCHEDULE_SIZE; loop++)
		//printf("instr_arr[%0d] = %08x\n", loop, instr_arr[loop]);
	return 1;
}

uint32_t RtypeDecode(struct InstrFields *Fields, uint32_t instruction,int size){
	int sizetemp;
	sizetemp = size;
	Fields->instr32bit 		= instruction;
	// printf("RtypeDecode instruction %0x  Fields->instr32bit %0x\n", instruction, Fields->instr32bit);
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
	if(fill_arrays(Fields, sizetemp)) return 1;
	else return 0;
}

uint32_t ItypeDecode(struct InstrFields *Fields, uint32_t instruction,int size){
	int sizetemp;
	sizetemp = size;
	Fields->instr32bit 		= instruction;
	// printf("ItypeDecode instruction %0x Fields->instr32bit %0x\n", instruction, Fields->instr32bit);
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
	if(fill_arrays(Fields, sizetemp)) return 1;
	else return 0;
}

uint32_t StypeDecode(struct InstrFields *Fields, uint32_t instruction,int size){
	int sizetemp;
	sizetemp = size;
	Fields->instr32bit 		= instruction;
	// printf("StypeDecode instruction %0x Fields->instr32bit %0x\n", instruction, Fields->instr32bit);
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
	if(fill_arrays(Fields, sizetemp)) return 1;
	else return 0;
}

uint32_t BtypeDecode(struct InstrFields *Fields, uint32_t instruction,int size){
	int sizetemp;
	sizetemp = size;
	Fields->instr32bit 		= instruction;
	// printf("BtypeDecode instruction %0x Fields->instr32bit %0x\n", instruction, Fields->instr32bit);
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
	if(fill_arrays(Fields, sizetemp)) return 1;
	else return 0;
}

uint32_t UtypeDecode(struct InstrFields *Fields, uint32_t instruction,int size){
	int sizetemp;
	sizetemp = size;
	Fields->instr32bit 		= instruction;
	// printf("UtypeDecode instruction %0x Fields->instr32bit %0x\n", instruction, Fields->instr32bit);
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
	if(fill_arrays(Fields, sizetemp)) return 1;
	else return 0;
}

uint32_t JtypeDecode(struct InstrFields *Fields, uint32_t instruction,int size){
	int sizetemp;
	sizetemp = size;
	Fields->instr32bit 		= instruction;
	// printf("JtypeDecode instruction %0x Fields->instr32bit %0x\n", instruction, Fields->instr32bit);
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
	if(fill_arrays(Fields, sizetemp)) return 1;
	else return 0;
}

/***************************************** MAIN FUNCTION FOR RISCV SIMULATOR *****************************************/

void placement_logic(int a, int k[PRESCHEDULE_SIZE], int instr_arr[PRESCHEDULE_SIZE], int i, uint32_t pre_arr[PRESCHEDULE_SIZE][SCHEDULE_WIDTH]) {
	
				int match=0;
				int found=0;
				while(!found){
				for(int r = 0; r < SCHEDULE_WIDTH; r++){
						//cout << "pre_arr["<<dec<<a<<"]["<<r<<"] = " << "0x" << setfill('0') << setw(8) << hex << pre_arr[a][r] << "\n" << endl;
						if(instr_arr[i] == pre_arr[a][r]){
								printf("Instruction already Present : instr_arr[%0d]=%0d pre_arr[%0d][%0d]=%0d\n", i, instr_arr[i], a, r, pre_arr[a][r]);
								match=1;
								break;
						} else {
							if((pre_arr[a][r] == 0) && !match && !found){
								new_a = a;
								new_k = r;
								//printf("new_a=%d new_k=%d match=%d pre_arr[%0d][%0d]=%0x\n",a,r,match,a,r,pre_arr[a][r]);
								found = 1;
								break;
							}
						}
				}
						a++; match=0;
				}
				
				pre_arr[new_a][new_k] = instr_arr[i];
				//printf("placing Instr=%08x into pre_arr[%0d][%0d]\n",instr_arr[i],new_a,new_k);
				k[new_a] = (k[new_a] < SCHEDULE_WIDTH) ? k[new_a]+1 : 0;
				//return new_a;
		}
		
void dependency_calc(){
	for(int i=0; i<MAX_INSTRUCTIONS; i++) {
	
		dependency=0;
		//printf("i=%0d\t",i);
		if(opcode_arr[i] == LOAD || opcode_arr[i] == STORE || opcode_arr[i] == BRANCH) ex_lat = 4;
		else	ex_lat = 1; // ADD, SUB, MUL
		for(int j=i+1; j<(i+ex_lat+1); j++) {
			//printf("j=%0d\n",j);
			if( (rd_arr[i] == rs1_arr[j]) || (rd_arr[i] == rs2_arr[j])){
				printf("RAW dependency between instructions %d and %d\n", i , j);
				//printf("rd_arr[%0d]=%0d rs1_arr[%0d]=%0d rs2_arr[%0d]=%0d\n", i, rd_arr[i], j, rs1_arr[j], j, rs2_arr[j]);
				dependency = 1;
				break;
			} else if((rs1_arr[i] == rd_arr[j]) || (rs2_arr[i] == rd_arr[j])){
				printf("WAR dependency between instructions %d and %d\n", i , j);
				//printf("rs1_arr[%0d]=%0d rs2_arr[%0d]=%0d rd_arr[%0d]=%0d \n", i, rs1_arr[i], i, rs2_arr[i], j, rd_arr[j]);
				dependency = 1;
				war=1;
				break;
			} else if(rd_arr[i] == rd_arr[j]){
				printf("WAW dependency between instructions %d and %d\n", i , j);
				//printf("rs1_arr[%0d]=%0d rs2_arr[%0d]=%0d rd_arr[%0d]=%0d rs1_arr[%0d]=%0d rs2_arr[%0d]=%0d rd_arr[%0d]=%0d ", i, rs1_arr[i], i, rs2_arr[i], i, rd_arr[i], j, rs1_arr[j], j, rs2_arr[j], j, rd_arr[j]);
				dependency = 1;
				break;
			} else {
				//printf("rs1_arr[%0d]=%0d rs2_arr[%0d]=%0d rd_arr[%0d]=%0d \n", i, rs1_arr[i], i, rs2_arr[i], j, rd_arr[j]);
				printf("No dependency between instructions %d and %d\n", i , j);
				dependency = 0;
			}
		}
		placement_logic(a, k,instr_arr,i,pre_arr);		
		if(dependency || (i== (MAX_INSTRUCTIONS-1))){
			//placement_logic(a, k,instr_arr,i,pre_arr);
				if(war){
					for(int loop=0; loop<PRESCHEDULE_SIZE; loop++){
						if(k[loop] < SCHEDULE_WIDTH) {
							a = loop;
							break;
						}
					}	
					war = 0;
					} 
				else a = new_a + ex_lat;
		dependency = 0;
		}
	}
}

int main(int argc, char *argv[]) {
	int size=0;	
	string memoryimage= "program.mem";		// Default memory image file
	uint32_t max_pc;						// To track the Maximum value of PC supplied from mem file
	char choice;
	uint32_t count=0;
	
	for(int loop1=0; loop1 < PRESCHEDULE_SIZE; loop1++){
		for(int loop2=0; loop2 < SCHEDULE_WIDTH; loop2++){
			pre_arr[loop1][loop2]=0;
		}
	}

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
							case 0b0110111: { if(!UtypeDecode(&Fields,currentINSTR,size)) return 0;	
												size++;
												break;	// For LUI
											}
											
							case 0b0010111: { if(!UtypeDecode(&Fields,currentINSTR,size)) return 0;	
												size++;
												break;	// For AUIPC
											}
							case 0b1101111: { if(!JtypeDecode(&Fields,currentINSTR,size)) return 0;	
											size++;
											break;	// For JAL
											}
							case 0b1100111: { if(!ItypeDecode(&Fields,currentINSTR,size)) return 0;
											size++;
											break;	// For JALR
							}
							case 0b1100011: { if(!BtypeDecode(&Fields,currentINSTR,size)) return 0;	
											size++;	
											break; // For BEQ,BNE,BLT,BGE,BLTU,BGEU
							 }
							case 0b0000011: { if(!ItypeDecode(&Fields,currentINSTR,size)) return 0;	
											size++;
											break; // For LB,LH,LW,LBU,LHU
							 				} 	
							case 0b0100011: { if(!StypeDecode(&Fields,currentINSTR,size)) return 0;	
											size++;
											break; // For SB,SH,SW
							 				}
											
							case 0b0010011: { if(!ItypeDecode(&Fields,currentINSTR,size)) return 0;
											size++;
											break; // For ADDI,SLTI,SLTIU,ANDI,ORI,XORI,SLLI,SRLI,SRAI
							 				}	
							case 0b0110011: { if(!RtypeDecode(&Fields,currentINSTR,size)) return 0;	
											size++;
											break; // For ADD,SLT,SLTU,AND,OR,XOR,SLL,SRL,SUB,SRA
							 				}
							default : 		cerr << "***ILLEGAL OPCODE*** Detected at address(hex): "<< hex << currentPC << "\tInstruction:"<< setfill('0') << setw(8) << hex << currentINSTR <<"\topcode(binary): " << bitset<7>(opcode) <<"\n"; 
											return 0; 
											break;
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
			if(size == MAX_INSTRUCTIONS) {
				dependency_calc();
				break;
			}
		}

	}
	


ofstream outputFile("prescheduled_instr.mem");

    if (outputFile.is_open()) {
 		for(int i=0;i<max_pc/4;i++){
			for(int j=0;j<SCHEDULE_WIDTH;j++){
				if(!(pre_arr[i][j]==0)) {
				outputFile << "pre_arr["<<dec<<i<<"]["<<j<<"] = " << "0x" << setfill('0') << setw(8) << hex << pre_arr[i][j] << "\n" << endl;
				}
			}
		}
         outputFile.close();
        cout << "Data written to the file." << endl;
    } else {
        cout << "Unable to open the file." << endl;
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
