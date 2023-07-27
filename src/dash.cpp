
#include "Hash.h"
#include "allocator.h"
#include "ex_finger.h"


using namespace std;

extern "C" {

struct Dash;

void init(void);

void* dash_create(const char* key);
int dash_insert(void* dash, uint64_t key, uint64_t value);
int dash_update(void* dash, uint64_t key, uint64_t value);
uint64_t dash_remove(void* dash, uint64_t key);
uint64_t dash_find(void* dash, uint64_t key);

struct DashString;

void* dashstring_create(void);
int dashstring_insert(void* dash, const char* key, int length, uint64_t value);
int dashstring_update(void* dash, const char* key, int length, uint64_t value);
uint64_t dashstring_remove(void* dash, const char* key, int length);
uint64_t dashstring_find(void* dash, const char* key, int length);

int plus(int a, int b);
}
// pool path and name
static const char *pool_name = "/mnt/pmem0/pmem_hash.data";

static const size_t pool_size = 1024ul * 1024ul * 1024ul * 10ul;
size_t segment_number = 256;

struct Dash {
    Hash<uint64_t>* inner;
};

int plus(int a, int b) {
    return a+b;
}
bool file_exist = false;

void init(const char* key) {
    pool_name = key;
    if (FileExists(pool_name)) file_exist = true;
    Allocator::Initialize(pool_name, pool_size);
}

void* dash_create(void) {

    // Step 2: Allocate the initial space for the hash table on PM and get the
    // root; we use Dash-EH in this case.
    // printf("+++\n");

    Dash* dash = new Dash;
    // dash->inner = new extendible::Finger_EH<uint64_t>(segment_number, Allocator::Get()->pm_pool_);
    // printf("+++\n");
    dash->inner = reinterpret_cast<Hash<uint64_t> *>(
        Allocator::GetRoot(sizeof(extendible::Finger_EH<uint64_t>)));
    if (!file_exist) {
    // During initialization phase, allocate 64 segments for Dash-EH
    new (dash->inner) extendible::Finger_EH<uint64_t>(
        segment_number, Allocator::Get()->pm_pool_);
    } else {
        new (dash->inner) extendible::Finger_EH<uint64_t>();
        dash->inner->Recovery();
    }
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

    // printf("%d %llx\n", key, value);
    Value_t v = (char *)(value);

    bool ret = ((Dash*)(dash))->inner->Update(key, &v);
    return ret;
}

uint64_t dash_find(void* dash, uint64_t key) {
    // printf("%ld -3\n", key);
    Value_t value;
    ((Dash*)(dash))->inner->Get(key, &value);
    // printf("%d %llx\n", key, value);

    return (uint64_t) (value);
}
uint64_t dash_remove(void* dash, uint64_t key) {
    // printf("%ld -3\n", key);
    return (uint64_t)(((Dash*)(dash))->inner->Delete(key));
}

// int main(void) {
//     void* tree = dash_create();
//     printf("create ok\n");
//     dash_insert(tree, 10, 10);
//     printf("%ld\n", dash_find(tree, 10));
//     return 0;
// }

struct DashString {
    Hash<string_key*>* inner;
};



void* dashstring_create(void) {
     bool file_exist = false;
    if (FileExists(pool_name)) file_exist = true;

    // Step 2: Allocate the initial space for the hash table on PM and get the
    // root; we use Dash-EH in this case.
    // printf("+++\n");
    DashString* dash = new DashString;
    dash->inner = new extendible::Finger_EH<string_key*>(segment_number, Allocator::Get()->pm_pool_);
    // printf("+++\n");
//     if (!file_exist) {
//     // During initialization phase, allocate 64 segments for Dash-EH
    
//     new (dash->inner) extendible::Finger_EH<string_key*>(
//         segment_number, Allocator::Get()->pm_pool_);
//   }else{
//     new (dash->inner) extendible::Finger_EH<string_key*>();
//   }

    // dash->inner = new extendible::Finger_EH<string_key*>(segment_number);
    
    return (void*)(dash);
}


int dashstring_insert(void* dash, const char* key, int length, uint64_t value) {

    string_key* var_key = reinterpret_cast<string_key *>(malloc( (length + sizeof(string_key))));
    var_key->length = length;
    memcpy(var_key->key, key, length);
    Value_t v;
    if ((((DashString*)(dash))->inner->Get(var_key, &v))) {

        v = (char *)(value);
        ((DashString*)(dash))->inner->Update(var_key, &v);
        return 2;
    }
    bool ret = ((DashString*)(dash))->inner->Insert(var_key, (char *)(value), false);
    return ret;
}

int dashstring_update(void* dash, const char* key, int length, uint64_t value) {

    Value_t v = (char *)(value);
    string_key* var_key = reinterpret_cast<string_key *>(malloc( (length + sizeof(string_key))));
    var_key->length = length;
    memcpy(var_key->key, key, length);
    bool ret = ((DashString*)(dash))->inner->Update(var_key, &v);
    return ret;
}

uint64_t dashstring_find(void* dash, const char* key, int length) {
    string_key* var_key = reinterpret_cast<string_key *>(malloc( (length + sizeof(string_key))));
    var_key->length = length;
    memcpy(var_key->key, key, length);

    Value_t value;
    ((DashString*)(dash))->inner->Get(var_key, &value);
    return (uint64_t) (value);
}
uint64_t dashstring_remove(void* dash, const char* key, int length) {
    int variable_length = length;
    string_key* var_key = reinterpret_cast<string_key *>(malloc( (length + sizeof(string_key))));
    var_key->length = length;
    memcpy(var_key->key, key, length);
    return (uint64_t)(((DashString*)(dash))->inner->Delete(var_key));
}


// int main(void) {
//     void* ds = dashstring_create();
//     printf("create ok\n");
//     string a = "ggb";
//     dashstring_insert(ds, a.c_str(), 3, 3333);
//     dashstring_insert(ds, a.c_str(), 3, 4444);
//     printf("%ld\n", dashstring_find(ds, a.c_str(), 3));
//     dashstring_update(ds, a.c_str(), 3, 5555);
    
//     printf("%ld\n", dashstring_find(ds, a.c_str(), 3));
//     return 0;
// }