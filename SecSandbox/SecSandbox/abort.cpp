#include "sandbox.h"

// abort with error
void Sandbox::errAbort(ErrorType e){
	switch(e){
	case Error_ProgFile:
		cout << "Cannot access program file" << endl;
		break;
	case Error_DataFile:
		cout << "Cannot access data file" << endl;
		break;
	case Error_LineTooLong:
		cout << "Script line is too long " << printLine(curInst) << endl;
		break;
	case Error_InstNotFound:
		cout << "Instruction not found " << printLineInst(curInst) << endl;
		break;
	case Error_Realloc:
		cout << "Realloc an existing variable " << printLineInst(curInst) << endl;
		break;
	case Error_DeallocNull:
		cout << "Dealloc a non-existing variable " << printLineInst(curInst) << endl;
		break;
	case Error_InvalidNum:
		cout << "Invalid number " << printLineInst(curInst) << endl;
		break;
	case Error_UnboundedNum:
		cout << "Number exceeds integer range " << printLineInst(curInst) << endl;
		break;
	case Error_InvalidArray:
		cout << "Invalid array reference " << printLineInst(curInst) << endl;
		break;
	case Error_MemLimitExceeded:
		cout << "Memory limit exceeded " << printLineInst(curInst) << endl;
		break;
	case Error_LineLimitExceeded:
		cout << "Line limit exceeded " << printLineInst(curInst) << endl;
		break;
	case Error_InvalidChar:
		cout << "Invalid character " << printLineInst(curInst) << endl;
		break;
	case Error_InvalidName:
		cout << "Invalid variable/array name " << printLineInst(curInst) << endl;
		break;
	case Error_VarNotFound:
		cout << "Undefined variable " << printLineInst(curInst) << endl;
		break;
	case Error_DivideByZero:
		cout << "Divide by zero " << printLineInst(curInst) << endl;
		break;
	case Error_IndexOutOfRange:
		cout << "Array index out of range " << printLineInst(curInst) << endl;
		break;
	case Error_NoData:
		cout << "Data is empty upon reading " << printLineInst(curInst) << endl;
		break;
	case Error_MaxRecursive:
		cout << "Maximum recursive level exceeded " << printLineInst(curInst) << endl;
		break;
	case Error_JptNotFound:
		cout << "Jump point not found " << printLineInst(curInst) << endl;
		break;
	case Error_JptDuplicated:
		cout << "Jump point duplicated " << printLineInst(curInst) << endl;
		break;
	default:
		cout << "Unknown error" << endl;
	}
	cleanUp();
	exit(0);
}
