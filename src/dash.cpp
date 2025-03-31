#include "dash.h"

#include "Hash.h"
#include "allocator.h"
#include "ex_finger.h"
using namespace std;

bool file_exist = false;

void DASH::Dash::Init(const char *poolFileName, size_t poolSize) {
  if (FileExists(poolFileName)) file_exist = true;
  Allocator::Initialize(poolFileName, poolSize);
}
void DASH::Dash::Close(){
    Allocator::Close_pool();
}
DASH::Dash::Dash(size_t segmentNumber) {
  inner = reinterpret_cast<Hash<uint64_t> *>(
      Allocator::GetRoot(sizeof(extendible::Finger_EH<uint64_t>)));
  if (!file_exist) {
    new (inner) extendible::Finger_EH<uint64_t>(segmentNumber,
                                                Allocator::Get()->pm_pool_);
  } else {
    new (inner) extendible::Finger_EH<uint64_t>();
    reinterpret_cast<Hash<uint64_t> *>(inner)->Recovery();
  }
}
int DASH::Dash::Insert(uint64_t key, uint64_t value) {
  Value_t v;
  if ((reinterpret_cast<Hash<uint64_t> *>(inner)->Get(key, &v))) {
    v = (char *)(value);
    reinterpret_cast<Hash<uint64_t> *>(inner)->Update(key, &v);
    return 2;
  }
  bool ret = reinterpret_cast<Hash<uint64_t> *>(inner)->Insert(key, (char *)(value), false);
  return ret;
}
int DASH::Dash::Update(uint64_t key, uint64_t value) {
  Value_t v = (char *)(value);
  bool ret = reinterpret_cast<Hash<uint64_t> *>(inner)->Update(key, &v);
  return ret;
}
uint64_t DASH::Dash::Remove(uint64_t key) {
  Value_t value;
  reinterpret_cast<Hash<uint64_t> *>(inner)->Get(key, &value);
  return (uint64_t)(value);
}
uint64_t DASH::Dash::Find(uint64_t key) {
  return (uint64_t)(reinterpret_cast<Hash<uint64_t> *>(inner)->Delete(key));
}
