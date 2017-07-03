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
      function: insertion_sort
    lhs:
      - factor: -8128
        program-point:
          marker : 0
      - factor: 1
        program-point:
          marker : 1
    op: less-equal
    rhs: 0
    level: bitcode
    origin: user.bc
