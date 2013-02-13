#include "sandbox.h"

// deallocate a variable
string Sandbox::deallocVar(string name){
	map<string, Var *>::iterator ri;
	if((ri=vmap.find(name))==vmap.end()){
		// variable not exist
		errAbort(Error_DeallocNull);
	}
	string varname = name;
	checkValidName(varname);
	Var *pvar = (*ri).second;
	vmap.erase(ri);

	int offset = pvar->len();

	vector<Var *>::iterator rv, rvf;
	rv = vvec.begin() + pvar->index();
	// decrease the indices of the variables in the vector that follow the deallocated variable 
	for(rvf=rv+1; rvf!=vvec.end(); rvf++){
		int index = (*rvf)->index();
		(*rvf)->setIndex(index-1);
		(*rvf)->setP((*rvf)->p()-offset);
	}

	// move the memory that follows forward
	// this is slow, anyway for the sandbox it's ok
	int *ptr;
	for(ptr=pvar->p(); ptr+offset<mem+MEMSIZE; ptr++){
		*ptr = *(ptr+offset);
	}

	// free the memory
	memUsed -= (*rv)->len();	
	numVar --;
	delete pvar;
	vvec.erase(rv);	// removing an element in a vector is slow, but it's ok if we don't have many variables

	if(vvec.size()){
		vector<Var *>::reverse_iterator rrv;
		rrv = vvec.rbegin();
		freeMem = (*rrv)->p() + (*rrv)->len();
	}

	return varname;
}

// allocate a variable
string Sandbox::allocVar(string name){
	map<string, Var *>::iterator ri;
	if((ri=vmap.find(name))!=vmap.end()){
		// variable already existing
		errAbort(Error_Realloc);
	}
	string varname = name;

	int arraylen;
	string arrayname;
	bool isArray = false;
	if(isArray=checkArray(name, arrayname, arraylen)){
		varname = arrayname;
	}
	checkValidName(varname);	// check if the name is valid

	Var *newvar;
	if(!isArray){
		// single variable allocation
		if(memUsed+1>MEMSIZE) errAbort(Error_MemLimitExceeded);	// not enough memory for even one variable

		newvar = new Var(numVar);
		newvar->setLen(1);
		newvar->setP(freeMem);
		freeMem++;
		memUsed++;

		vvec.push_back(newvar);
		vmap.insert(make_pair(varname, newvar));
	}else{
		// array allocation
		if(arraylen<=0) errAbort(Error_InvalidArray);	// invalid array length
		if(memUsed+arraylen>MEMSIZE) errAbort(Error_MemLimitExceeded);	// not enough memory for such large array

		newvar = new Var(numVar);
		newvar->setLen(arraylen);
		newvar->setP(freeMem);
		freeMem+=arraylen;
		memUsed+=arraylen;

		vvec.push_back(newvar);
		vmap.insert(make_pair(varname, newvar));
	}
	numVar++;
	return varname;
}
