#pragma once
#ifndef J_CONTEXT_
#define J_CONTEXT_

typedef struct
{
	std::vector<> stack;
	const char* json;
}j_context;


#endif // !J_CONTEXT_
