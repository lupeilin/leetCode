#include <vector>
#include <algorithm>
#include <iostream>

using namespace std;

class Solution {
public:
    long long maxBeauty(vector<int>& flowers, long long newFlowers, int target, int full, int partial) {
        int n = flowers.size();

        // 如果全部种满，还剩下多少朵花？
        long long left_flowers = newFlowers - 1LL * target * n; // 先减掉
        for (int& flower : flowers) {
            flower = min(flower, target);
            left_flowers += flower; // 把已有的加回来
        }

        // 没有种花，所有花园都已种满
        if (left_flowers == newFlowers) {
            return 1LL * n * full; // 答案只能是 n*full（注意不能减少花的数量）
        }

        // 可以全部种满
        if (left_flowers >= 0) {
            // 两种策略取最大值：留一个花园种 target-1 朵花，其余种满；或者，全部种满
            return max(1LL * (target - 1) * partial + 1LL * (n - 1) * full, 1LL * n * full);
        }

        sort(flowers.begin(), flowers.end()); // 时间复杂度的瓶颈在这，尽量写在后面

        long long ans = 0, pre_sum = 0;
        int j = 0;
        // 枚举 i，表示后缀 [i, n-1] 种满（i=0 的情况上面已讨论）
        for (int i = 1; i <= n; i++) {
            // 撤销，flowers[i-1] 不变成 target
            left_flowers += target - flowers[i - 1];
            if (left_flowers < 0) { // 花不能为负数，需要继续撤销
                continue;
            }

            // 满足以下条件说明 [0, j] 都可以种 flowers[j] 朵花
            while (j < i && 1LL * flowers[j] * j <= pre_sum + left_flowers) {
                pre_sum += flowers[j];
                j++;
            }

            // 计算总美丽值
            // 在前缀 [0, j-1] 中均匀种花，这样最小值最大
            long long avg = (left_flowers + pre_sum) / j; // 由于上面特判了，这里 avg 一定小于 target
            long long total_beauty = avg * partial + 1LL * (n - i) * full;
            ans = max(ans, total_beauty);
        }

        return ans;
    }
};

int main() {
    Solution sol;

    vector<int> flowers1 = { 1, 3, 1, 1 };
    cout << sol.maxBeauty(flowers1, 7, 6, 12, 1) << endl; // 14

    vector<int> flowers2 = { 2, 4, 5, 3 };
    cout << sol.maxBeauty(flowers2, 10, 5, 2, 6) << endl; // 30

    // 新的测试用例
    vector<int> flowers3 = { 18, 16, 10, 10, 5 };
    cout << sol.maxBeauty(flowers3, 10, 3, 15, 4) << endl; // 75

    vector<int> flowers4 = { 8,20,13 };
    cout << sol.maxBeauty(flowers4, 12, 12, 4, 3) << endl; // 41

    return 0;
}
