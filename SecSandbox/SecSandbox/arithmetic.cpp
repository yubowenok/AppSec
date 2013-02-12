#include "sandbox.h"

void Sandbox::setv(Var *pvar1, Var *pvar2){
	int *p1 = pvar1->p();
	int *p2 = pvar2->p();
	if(pvar1->len()>1) p1 = pvar1->arrayp();
	if(pvar2->len()>1) p2 = pvar2->arrayp();
	*p1 = *p2;
}

void Sandbox::add(Var *pvar1, Var *pvar2){
	int *p1 = pvar1->p();
	int *p2 = pvar2->p();
	if(pvar1->len()>1) p1 = pvar1->arrayp();
	if(pvar2->len()>1) p2 = pvar2->arrayp();
	*p1 = *p1 + *p2;
	if(*p1>MAXINT || *p1<-MAXINT) errAbort(Error_UnboundedNum);
}

void Sandbox::sub(Var *pvar1, Var *pvar2){
	int *p1 = pvar1->p();
	int *p2 = pvar2->p();
	if(pvar1->len()>1) p1 = pvar1->arrayp();
	if(pvar2->len()>1) p2 = pvar2->arrayp();
	*p1 = *p1 - *p2;
	if(*p1>MAXINT || *p1<-MAXINT) errAbort(Error_UnboundedNum);
}

void Sandbox::mul(Var *pvar1, Var *pvar2){
	int *p1 = pvar1->p();
	int *p2 = pvar2->p();
	if(pvar1->len()>1) p1 = pvar1->arrayp();
	if(pvar2->len()>1) p2 = pvar2->arrayp();
	long long res = (*p1) * (*p2);
	if(res>MAXINT || res<-MAXINT) errAbort(Error_UnboundedNum);
	(*p1) = (int)res;
}

void Sandbox::div(Var *pvar1, Var *pvar2){
	int *p1 = pvar1->p();
	int *p2 = pvar2->p();
	if(pvar1->len()>1) p1 = pvar1->arrayp();
	if(pvar2->len()>1) p2 = pvar2->arrayp();
	if(*p2==0) errAbort(Error_DivideByZero);
	*p1 = *p1 / *p2;
	if(*p1>MAXINT || *p1<-MAXINT) errAbort(Error_UnboundedNum);
}

void Sandbox::inc(Var *pvar){
	int *p = pvar->p();
	if(pvar->len()>1) p = pvar->arrayp();
	*p ++;
	if(*p>MAXINT) errAbort(Error_UnboundedNum);
}

void Sandbox::dec(Var *pvar){
	int *p = pvar->p();
	if(pvar->len()>1) p = pvar->arrayp();
	*p --;
	if(*p<-MAXINT) errAbort(Error_UnboundedNum);
}