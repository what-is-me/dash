#include <cstddef>
#include <cstdint>
namespace DASH {
class Dash {
 public:
  static void Init(const char* poolFileName, size_t poolSize);
  static void Close();
  Dash(size_t segmentNumber = 256);
  int Insert(uint64_t key, uint64_t value);
  int Update(uint64_t key, uint64_t value);
  uint64_t Remove(uint64_t key);
  uint64_t Find(uint64_t key);

 private:
  void* inner;
};
}  // namespace DASH