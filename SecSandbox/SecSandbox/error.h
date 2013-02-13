#ifndef ERROR_H
#define ERROR_H

using namespace std;

// error types
enum ErrorType{
	Error_SandboxArg = 0,
	Error_ProgFile,
	Error_DataFile,
	Error_LineTooLong,
	Error_InstNotFound,
	Error_Realloc,
	Error_DeallocNull,
	Error_InvalidNum,
	Error_UnboundedNum,
	Error_InvalidArray,
	Error_MemLimitExceeded,
	Error_LineLimitExceeded,
	Error_InvalidChar,
	Error_InvalidName,
	Error_VarNotFound,
	Error_DivideByZero,
	Error_IndexOutOfRange,
	Error_NoData,
	Error_MaxRecursive,
	Error_JptNotFound,
	Error_JptDuplicated,
	Error_InvalidJump,
	Error_ProgramExit
};


#endif