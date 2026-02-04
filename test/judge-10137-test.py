#!/usr/bin/env python3

from test import gtest_test_utils  # 注意test目录下不能有test.py文件，否则test有歧义
from test import uva_test_common

COMMAND = gtest_test_utils.GetTestExecutablePath('judge10137_the_trip')

def TestJudge10137():
  return uva_test_common.run_tests("judge10137", COMMAND)


class Judge10137Test(gtest_test_utils.TestCase):

  def test(self):
    self.assertTrue(TestJudge10137())


if __name__ == "__main__":
  gtest_test_utils.Main()
