#pragma once
#ifndef J_CONTEXT_
#define J_CONTEXT_

class j_context {
public:	
	char* stack;
	size_t top, size;
	const char* json;

};
void * j_context_push(j_context * pack, size_t size);
void * j_context_pop(j_context * pack, size_t size);

#endif // !J_CONTEXT_
