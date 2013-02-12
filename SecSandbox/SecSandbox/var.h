#ifndef VAR_H
#define VAR_H

#include <string>

using namespace std;

// variable types 
enum VarType{
	Var_Int = 0,
	Var_IntArray
};

// variable class
class Var{
public:
	Var(){}
	Var(int index){ _index = index; }

	VarType type(){ return _type; }
	void setType(VarType type){ type = _type; }
	int *p(){ return _p; }
	int *arrayp(){ return _p+_arrayindex; }	// this function is called after checking array range
	void setP(int *p){ _p = p; }
	int len(){ return _len; }
	void setLen(int len){ _len = len; }
	int index(){ return _index; }
	void setIndex(int index){ _index = index; }
	void setArrayindex(int index){ _arrayindex = index; }
private:
	VarType _type;
	int *_p;	// pointer to the memory location
	int _len; // array length
	int _index;	// index in the vector vvec
	int _arrayindex; // index in the array
};

#endif