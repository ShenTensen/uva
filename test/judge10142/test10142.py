#!/usr/bin/env python3

# inspired from
# https://github.com/61c-teach/fa20-proj3-starter/blob/master/test_runner.py

import os
import os.path
import subprocess
import signal
import re
import sys

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

def run_test(group_path, test_slug, output_type=None):
  output_slug = test_slug
  if output_type:
    output_slug += "-" + output_type
  program_path = os.path.join(group_path, "%s.exe" % test_slug)
  input_path = os.path.join(group_path, "program_input", "%s-input.txt" % output_slug)
  output_path = os.path.join(group_path, "program_output", "%s-output.txt" % output_slug)
  reference_output_path = os.path.join(group_path, "reference_output", "%s-ref.txt" % output_slug)
  test_runner = ProgramTest(group_path, program_path, input_path, reference_output_path)
  return test_runner(output_path)


if __name__ == "__main__":
  run_test(script_dir, "judge10142_australian_voting")
