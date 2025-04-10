#include <array>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <ratio>
#include <thread>
using namespace std;

extern "C" {

struct Dash;

void init(const char *key);

void *dash_create(void);
int dash_insert(void *dash, uint64_t key, uint64_t value);
int dash_update(void *dash, uint64_t key, uint64_t value);
uint64_t dash_remove(void *dash, uint64_t key);
uint64_t dash_find(void *dash, uint64_t key);
}

constexpr size_t threadCnt = 128;
int64_t totals[threadCnt];
void ingest(void *dash, size_t threadId) {
  uint64_t key = threadId;
  for (size_t i = 0; i < 1000000; i++) {
    for (size_t j = 0; j < 1000; j++) {
      dash_insert(dash, key, key);
      key += threadCnt;
    }
    totals[threadId] = (i + 1) * 1000;
  }
}
const char *dir = "/mnt/pmem0/bench10";

int main() {
  std::remove(dir);
  init(dir);
  void *dash = dash_create();
  array<std::thread, threadCnt> ts;
  for (auto i = 0; i < threadCnt; i++) {
    ts[i] = std::thread(ingest, dash, i);
  }
  std::thread timer_([](){
    uint64_t total = 0;
    for(;;){
        std::this_thread::sleep_for(1s);
        uint64_t cur_total = 0;
        for(int i = 0;i<threadCnt;i++){
            cur_total += totals[i];
        }
        printf("ops: %lu\n", (cur_total - total));
        total = cur_total;
    }
  });
  for (auto i = 0; i < threadCnt; i++) {
    ts[i].join();
  }
}