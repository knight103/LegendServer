#include "uv.h"
#include "core/ProtocolDefine.h"
#include "TestClient.h"
#include <time.h>

uv_loop_t *loop;

void thread_cb(void* arg) {
	while (1) {
		TestClient * client = new TestClient(loop);
		client->connect("0.0.0.0", 7300);

		Sleep(rand() % 3000);
	}
}

void idle_cb(uv_idle_t* handle) {
	Sleep(100);
}

int main() {

	srand(time(NULL));

	loop = uv_default_loop();

	for (size_t i = 0; i < 10; i++)
	{
		uv_thread_t* thread_handle = new uv_thread_t;
		uv_thread_create(thread_handle, thread_cb, NULL);
	}

	//TestClient * client = new TestClient(loop);
	//client->connect("0.0.0.0", 7300);

	uv_idle_t idle;
	uv_idle_init(loop, &idle);
	uv_idle_start(&idle, idle_cb);

	uv_run(loop, UV_RUN_DEFAULT);

    return 0;
}





