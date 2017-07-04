---
format:          pml-0.1
triple:          patmos-unknown-unknown-elf
flowfacts:
  - scope:
      function: fillTable
    lhs:
      - factor: -128
        program-point:
          marker : 3
      - factor: 1
        program-point:
          marker : 4
    op: less-equal
    rhs: 0
    level: bitcode
    origin: user.bc
  - scope:
      function: compress
    lhs:
      - factor: -28672
        program-point:
          marker : 5
      - factor: 1
        program-point:
          marker : 6
    op: less-equal
    rhs: 0
    level: bitcode
    origin: user.bc
  - scope:
      function: compress
    lhs:
      - factor: -3584
        program-point:
          marker : 5
      - factor: 1
        program-point:
          marker : 7
    op: less-equal
    rhs: 0
    level: bitcode
    origin: user.bc
  - scope:
      function: merge_sort_nrecursive
    lhs:
      - factor: -127
        program-point:
          marker : 8
      - factor: 1
        program-point:
          marker : 9
      - factor: 1
        program-point:
          marker : 10
      - factor: 1
        program-point:
          marker : 11
    op: less-equal
    rhs: 0
    level: bitcode
    origin: user.bc
  - scope:
      function: merge_sort_nrecursive
    lhs:
      - factor: -127
        program-point:
          marker : 8
      - factor: 1
        program-point:
          marker : 12
    op: less-equal
    rhs: 0
    level: bitcode
    origin: user.bc
