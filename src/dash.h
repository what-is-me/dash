#include <cstddef>
#include <cstdint>
namespace DASH {
class Dash {
 public:
  static void Init(const char* poolFileName, size_t poolSize);
  static void Close();
  Dash(size_t segmentNumber = 256);
  bool Insert(uint64_t key, uint64_t value);
  bool Update(uint64_t key, uint64_t value);
  bool Get(uint64_t key, uint64_t* value);
  bool Delete(uint64_t key);

 private:
  void* inner;
};
}  // namespace DASH