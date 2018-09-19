#include "uv.h"
#include "core/ProtocolDefine.h"
#include "TestClient.h"
#include <random>

uv_loop_t *loop;

std::default_random_engine e;

void thread_cb(void* arg) {
	while (1) {
		TestClient(loop).connect("0.0.0.0", 7300);
		Sleep(e() % 3000);
	}
}

void wait_for_a_while(uv_idle_t* handle) {
	//printf("dasdasdas");
	handle = 0;
}

int main() {

	loop = uv_default_loop();

	e.seed(10);

	for (size_t i = 0; i < 1; i++)
	{
		uv_thread_t* thread_handle = new uv_thread_t;
		uv_thread_create(thread_handle, thread_cb, NULL);
	}
	
	uv_idle_t idler;

	uv_idle_init(loop, &idler);
	uv_idle_start(&idler, wait_for_a_while);

	uv_run(loop, UV_RUN_DEFAULT);

    return 0;
}





