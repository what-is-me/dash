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

DASH::Dash::Dash(size_t segmentNumber) {
  inner = Allocator::GetRoot(sizeof(extendible::Finger_EH<uint64_t>));
  if (!file_exist) {
    new (inner) extendible::Finger_EH<uint64_t>(segmentNumber,
                                                Allocator::Get()->pm_pool_);
  } else {
    new (inner) extendible::Finger_EH<uint64_t>();
    reinterpret_cast<Hash<uint64_t> *>(inner)->Recovery();
  }
}

bool DASH::Dash::Insert(uint64_t key, uint64_t value) {
  Value_t v;
  if ((reinterpret_cast<Hash<uint64_t> *>(inner)->Get(key, &v))) {
    v = (char *)(value);
    return reinterpret_cast<Hash<uint64_t> *>(inner)->Update(key, &v);
  }
  return reinterpret_cast<Hash<uint64_t> *>(inner)->Insert(key, (char *)(value),
                                                           false) == 0;
}

bool DASH::Dash::Update(uint64_t key, uint64_t value) {
  Value_t v = (char *)(value);
  return reinterpret_cast<Hash<uint64_t> *>(inner)->Update(key, &v);
}

bool DASH::Dash::Get(uint64_t key, uint64_t *value) {
  Value_t v;
  bool ret = reinterpret_cast<Hash<uint64_t> *>(inner)->Get(key, &v);
  *value = (uint64_t)(v);
  return ret;
}

bool DASH::Dash::Delete(uint64_t key) {
  return reinterpret_cast<Hash<uint64_t> *>(inner)->Delete(key);
}
