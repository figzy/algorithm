#include <algorithm>
#include <iostream>
using namespace std;

class Solution {
public:
    vector<vector<int>> threeSum(vector<int>& nums) {
        vector<vector<int>> result;
        int len = nums.size();
        if (len < 3) {
            return result;
        }
        sort(nums.begin(), nums.end(), [](int a, int b) {return a < b; });
        if (nums[len - 1] < 0) {
            // ���һ��Ԫ�ض�С��0���򲻴��ڵ���0����Ԫ��
            return result;
        }
        for (int i = 0; i < len; i++) {
            if (nums[i] > 0) {
                return result;
            }
            if (i > 0 && nums[i] == nums[i - 1]) {
                // �����ظ���Ԫ��
                continue;
            }
            int l = i + 1;
            int r = len - 1;
            while (l < r) {
                int count = nums[i] + nums[l] + nums[r];
                if (count == 0) {
                    result.push_back({ nums[i], nums[l], nums[r] });
                    while (l < r && nums[l] == nums[l + 1]) l++; // �����ظ���Ԫ��
                    while (l < r && nums[r] == nums[r - 1]) r--; // �����ظ���Ԫ��
                    l++;
                    r--;
                }
                else if (count > 0) {
                    r--;
                }
                else {
                    l++;
                }
            }
        }
        return result;
    }
};