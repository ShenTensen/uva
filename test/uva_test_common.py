#!/usr/bin/env python3

# 借鉴CS61C project
# https://github.com/61c-teach/fa20-proj3-starter/blob/master/test_runner.py

import os
import re
import subprocess
import sys
from itertools import zip_longest

IS_WINDOWS = os.name == 'nt'
IS_LINUX = os.name == 'posix' and os.uname()[0] == 'Linux'
script_dir = os.path.realpath(sys.path[0])


class ProgramTest():
  """
  Runs a executable file and compares output against the provided reference file.
  """

  def __init__(self, group_path, program_path, input_path, trace_path):
    self.group_path = group_path
    self.program_path = program_path
    self.input_path = input_path
    self.trace_path = trace_path

  def __call__(self, output_path):
    with open(self.input_path, 'r') as input_f, open(output_path, 'w') as output_f:
      proc = subprocess.Popen([self.program_path],
                              cwd=self.group_path, stdin=input_f, stdout=output_f)
      proc.wait()  # 等待进程完成
      passed = compare_files(self.trace_path, output_path)
    if passed:
      print('Passed.')
      return (True, "Matched expected output")
    else:
      print('Failed!')
      return (False, "Did not match expected output")


# AI生成后手动修改
def compare_files(path1: str, path2: str, encoding: str = 'utf-8') -> bool:
  """
  逐行比较两个文本文件的内容。

  参数:
  - path1, path2: 文件路径
  - encoding: 打开文件时使用的编码（默认 'utf-8'）

  返回:
  - True: 两个文件逐行完全相同（忽略行尾的换行符差异）
  - False: 发现任意一行不同，或两文件行数不同
  """
  try:
    with open(path1, 'r', encoding=encoding, errors='replace') as f1, \
         open(path2, 'r', encoding=encoding, errors='replace') as f2:
      line_no = 0
      result = True
      for a, b in zip_longest(f1, f2, fillvalue=None):
        line_no += 1
        if a is None:
          print("line no: " + str(line_no))
          print("Refernce line empty, but output not!")
          return False
        elif b is None:
          print("line no: " + str(line_no))
          print("Output line empty, but reference not!")
          return False
        else:
          # 比较时去掉行尾的换行符差异（但保留其他空格）
          a = a.rstrip('\r\n')
          b = b.rstrip('\r\n')
          if a != b:
            print("line no: " + str(line_no))
            print("reference line:")
            print(a, end="]\n")
            print("output line:")
            print(b, end="]\n")
            return False
    return True
  except FileNotFoundError:
    return False
  except OSError:
    return False


def run_test(script_dir, problem_dir, program_name, test_slug, output_type=None):
  output_slug = test_slug
  if output_type:
    output_slug += "-" + output_type
  program_path = program_name
  input_path = os.path.join(problem_dir, "program_input", "%s-input.txt" % test_slug)
  reference_path = os.path.join(problem_dir, "reference_output", "%s-output-ref.txt" % output_slug)
  output_path = os.path.join(problem_dir, "program_output", "%s-output.txt" % output_slug)
  test_runner = ProgramTest(script_dir, program_path, input_path, reference_path)
  return test_runner(output_path)


def run_tests(problem_dir, command):
  problem_dir = os.path.join(script_dir, problem_dir)
  fls = os.listdir(os.path.join(problem_dir, "program_input"))
  inputs = []
  for filename in sorted(fls):
    match = re.match(r"^(.+)\-input.txt$", filename)
    if match:
      test_slug = match.group(1)
      inputs.append("%s" % test_slug)
  tests_passed = 0
  tests_failed = 0
  for input_filename in inputs:
    did_pass, fail_reason = False, "Unknown test error"
    did_pass, fail_reason = run_test(script_dir, problem_dir, command, input_filename)
    if did_pass:
      print("\tPASSED test: %s" % input_filename)
      tests_passed += 1
    else:
      print("\tFAILED test: %s (%s)" % (input_filename, fail_reason))
      tests_failed += 1
    if len(inputs) == 0:
      print("There are no tests for %s!\n" % command)
    else:
      print("Passed %d/%d tests\n" % (tests_passed, (tests_passed + tests_failed)))
  if tests_failed != 0:
    return False
  else:
    return True
