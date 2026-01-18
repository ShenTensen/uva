#!/usr/bin/env python3

# 借鉴CS61C project
# https://github.com/61c-teach/fa20-proj3-starter/blob/master/test_runner.py

import os
import re
import subprocess
import sys

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
  line_count = 0
  while True:
    line_count = line_count + 1
    line1 = program_out.readline().rstrip().decode("utf-8", "namereplace")
    line2 = reference_out.readline().rstrip()
    print('program  (' + str(line_count) + '): ' + line1)
    print('reference(' + str(line_count) + '): ' + line2)
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


def run_test(script_dir, problem_dir, program_name, test_slug, output_type=None):
  output_slug = test_slug
  if output_type:
    output_slug += "-" + output_type
  program_path = program_name
  input_path = os.path.join(problem_dir, "program_input", "%s-input.txt" % test_slug)
  output_path = os.path.join(problem_dir, "program_output", "%s-output.txt" % output_slug)
  reference_path = os.path.join(problem_dir, "reference_output", "%s-output-ref.txt" % output_slug)
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
