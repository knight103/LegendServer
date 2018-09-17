
#include <log.h>
#include <assert.h>

#define ASSERT(cond)				assert(cond)
#define CC_SAFE_DELETE(p)           do { delete (p); (p) = nullptr; } while(0)

