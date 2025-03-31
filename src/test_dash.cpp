#include <iostream>
#include <cassert>
#include "dash.h"
using namespace DASH;
int main() {
    // 初始化 Dash
    Dash::Init("/mnt/pmem2/cqc", 1024 * 1024 * 1024);
    Dash dash;

    // 测试插入功能
    uint64_t key1 = 1, value1 = 100;
    assert(dash.Insert(key1, value1)); // 插入成功
    uint64_t key2 = 2, value2 = 200;
    assert(dash.Insert(key2, value2)); // 插入成功
    uint64_t key3 = 3, value3 = 300;
    assert(dash.Insert(key3, value3)); // 插入成功

    // 测试更新功能
    uint64_t newValue1 = 101;
    assert(dash.Update(key1, newValue1)); // 更新成功
    uint64_t newValue2 = 201;
    assert(dash.Update(key2, newValue2)); // 更新成功

    // 测试获取功能
    uint64_t getValue;
    assert(dash.Get(key1, &getValue)); // 获取成功
    assert(getValue == newValue1); // 检查值是否更新
    assert(dash.Get(key2, &getValue)); // 获取成功
    assert(getValue == newValue2); // 检查值是否更新
    assert(dash.Get(key3, &getValue)); // 获取成功
    assert(getValue == value3); // 检查值是否未被更新

    // 测试删除功能
    assert(dash.Delete(key2)); // 删除成功
    assert(!dash.Get(key2, &getValue)); // 获取失败，因为 key2 已被删除

    // 测试插入重复键
    uint64_t key4 = 1, value4 = 400;
    assert(dash.Insert(key4, value4)); // 插入成功，即使 key1 已存在

    // 测试更新不存在的键
    uint64_t key5 = 5, value5 = 500;
    assert(!dash.Update(key5, value5)); // 更新失败，因为 key5 不存在

    // 测试删除不存在的键
    assert(!dash.Delete(key5)); // 删除失败，因为 key5 不存在

    std::cout << "所有测试通过！" << std::endl;
    return 0;
}