#!/usr/bin/env python3

# 借鉴google test的测试用例

"""测试UAa Online Judge 100 The 3n + 1 Problem解答是否正确"""

from test import gtest_test_utils  # 注意test目录下不能有test.py文件，否则test有歧义
from test import uva_test_common

COMMAND = gtest_test_utils.GetTestExecutablePath('judge100_the_3n_plus_1_problem')

if __name__ == "__main__":
  uva_test_common.run_tests("judge100", COMMAND)
