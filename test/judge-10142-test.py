#!/usr/bin/env python3

from test import gtest_test_utils  # 注意test目录下不能有test.py文件，否则test有歧义
from test import uva_test_common

COMMAND = gtest_test_utils.GetTestExecutablePath('judge10142_australian_voting')

if __name__ == "__main__":
  uva_test_common.run_tests("judge10142", COMMAND)
