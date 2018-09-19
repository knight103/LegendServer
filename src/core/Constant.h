
#include <core/log.h>
#include <assert.h>


// 接收缓冲区的大小
#define DEFAULT_RCVBUF_SIZE    (200*1024)
// 发送缓冲区的大小
#define DEFAULT_SNDBUF_SIZE    (128*1024)


#define ASSERT(cond, ...)			assert(cond)

#define CC_SAFE_DELETE(p)           do { delete (p); (p) = nullptr; } while(0)
