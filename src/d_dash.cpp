
#include "Hash.h"
#include "allocator.h"
#include "ex_finger.h"


using namespace std;

extern "C" {

struct Dash;

void* dash_create(void);
int dash_insert(void* dash, uint64_t key, uint64_t value);
int dash_update(void* dash, uint64_t key, uint64_t value);
uint64_t dash_remove(void* dash, uint64_t key);
uint64_t dash_find(void* dash, uint64_t key);
int plus(int a, int b);
}
// pool path and name
static const char *pool_name = "/mnt/pmem0/pmem_hash.data";
// pool size
static const size_t pool_size = 1024ul * 1024ul * 1024ul * 10ul;

struct Dash {
    Hash<uint64_t>* inner;
};

int plus(int a, int b) {
    return a+b;
}

void* dash_create(void) {

    // Step 2: Allocate the initial space for the hash table on PM and get the
    // root; we use Dash-EH in this case.
    // printf("+++\n");

    Dash* dash = new Dash;
    dash->inner = new extendible::Finger_EH<uint64_t>;
    return (void*)(dash);
}


int dash_insert(void* dash, uint64_t key, uint64_t value) {

    Value_t v;
    if ((((Dash*)(dash))->inner->Get(key, &v))) {

        v = (char *)(value);
        ((Dash*)(dash))->inner->Update(key, &v);
        return 2;
    }
    bool ret = ((Dash*)(dash))->inner->Insert(key, (char *)(value), false);
    // printf("%x insert %x\n", key, (entry_key_t)(curr_addr));

    // if(ret == false) {
    //     printf("%ld insert failed\n", key);
    // }
    return ret;
}

int dash_update(void* dash, uint64_t key, uint64_t value) {

    Value_t v = (char *)(value);

    bool ret = ((Dash*)(dash))->inner->Update(key, &v);
    return ret;
}

uint64_t dash_find(void* dash, uint64_t key) {
    // printf("%ld -3\n", key);
    Value_t value;
    ((Dash*)(dash))->inner->Get(key, &value);
    return (uint64_t) (value);
}
uint64_t dash_remove(void* dash, uint64_t key) {
    // printf("%ld -3\n", key);
    return (uint64_t)(((Dash*)(dash))->inner->Delete(key));
}

int main(void) {
    void* dash = dash_create();
    printf("create ok\n");
    dash_insert(dash, 10, 10);
    printf("%ld\n", dash_find(dash, 10));
    return 0;
}