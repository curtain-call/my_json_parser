#pragma once
#ifndef J_CONTEXT_
#define J_CONTEXT_
#include<vector>

class j_context {
public:	
	char* stack;
	size_t top, size;
	const char* json;

};
#endif // !J_CONTEXT_
void * j_context_push(j_context * pack, size_t size);

void * j_context_pop(j_context * pack, size_t size);
