#!/usr/bin/env python3

# Copyright 2008, Google Inc.
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are
# met:
#
#     * Redistributions of source code must retain the above copyright
# notice, this list of conditions and the following disclaimer.
#     * Redistributions in binary form must reproduce the above
# copyright notice, this list of conditions and the following disclaimer
# in the documentation and/or other materials provided with the
# distribution.
#     * Neither the name of Google Inc. nor the names of its
# contributors may be used to endorse or promote products derived from
# this software without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
# "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
# LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
# A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
# OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
# SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
# LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
# DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
# THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
# (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
# OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

# 借鉴google test的测试用例
# 借鉴CS61C project
# https://github.com/61c-teach/fa20-proj3-starter/blob/master/test_runner.py

"""测试UAa Online Judge 100 The 3n + 1 Problem解答是否正确"""

import os
import re
import sys
from test import gtest_test_utils  # 注意test目录下不能有test.py文件，否则test有歧义

IS_WINDOWS = os.name == 'nt'
IS_LINUX = os.name == 'posix' and os.uname()[0] == 'Linux'

COMMAND = gtest_test_utils.GetTestExecutablePath('judge100_the_3n_plus_1_problem')


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

  def __call__(self, filename):
    with open(self.input_path, 'r') as input_f:
      proc = subprocess.Popen([self.program_path],
                              cwd=self.group_path, stdin=input_f, stdout=subprocess.PIPE)
    try:
      reference = open(self.trace_path)
      passed = compare_unbounded(proc.stdout, reference, filename)
    finally:
      try:
        os.kill(proc.pid, signal.SIGTERM)
      except Exception as e:
        pass
    if passed:
      print('Passed.')
      return (True, "Matched expected output")
    else:
      print('Failed!')
      return (False, "Did not match expected output")


def compare_unbounded(program_out, reference_out, filename):
  passed = True
  program_output_array = []
  while True:
    line1 = program_out.readline().rstrip().decode("utf-8", "namereplace")
    line2 = reference_out.readline().rstrip()
    print('program: ' + line1)
    print('referen: ' + line2)
    if line2 == "":
      break
    program_output_array.append(line1)
    m = re.match(line2, line1)
    if m == None or m.start() != 0 or m.end() != len(line2):
      passed = False
  with open(filename, "w") as program_output:
    for line in program_output_array:
      program_output.write(line + "\n")
  return passed


def run_test(script_dir, program_name, test_slug, output_type=None):
  output_slug = test_slug
  if output_type:
    output_slug += "-" + output_type
  # program_path = os.path.join(script_dir, program_name)
  program_path = program_name;
  input_path = os.path.join(script_dir, "program_input", "%s-input.txt" % test_slug)
  output_path = os.path.join(script_dir, "program_output", "%s-output.txt" % output_slug)
  reference_path = os.path.join(script_dir, "reference_output", "%s-output-ref.txt" % output_slug)
  test_runner = ProgramTest(script_dir, program_path, input_path, reference_path)


def run_tests(problem_dir):
  problem_dir = os.path.join(script_dir, problem_dir)
  fls = os.listdir(os.path.join(problem_dir, "program_input"))
  inputs = []
  for filename in sorted(fls):
    match = re.match(r"^(.+)\-input.txt$", filename)
    if match:
      test_slug = match.group(1)
      inputs.append("%s" % test_slug)
  for input_filename in inputs:
    run_test(script_dir, COMMAND, input_filename)

if __name__ == "__main__":
  run_tests("judge100")

