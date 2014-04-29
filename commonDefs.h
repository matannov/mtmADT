#define SAFE_ASSIGN(ptr,val) \
	if ((ptr) != NULL) { \
		*(ptr) = val; \
	}
	