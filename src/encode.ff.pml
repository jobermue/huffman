---
format:          pml-0.1
triple:          patmos-unknown-unknown-elf
flowfacts:
  - scope:
      function: fillTable
    lhs:
      - factor: -127
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
      - factor: -28665
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
      - factor: -3583
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
      - factor: -8001
        program-point:
          marker : 0
      - factor: 1
        program-point:
          marker : 1
    op: less-equal
    rhs: 0
    level: bitcode
    origin: user.bc
