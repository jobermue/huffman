---
format:          pml-0.1
triple:          patmos-unknown-unknown-elf
flowfacts:
  - scope:
      function: merge_sort_nrecursive
    lhs:
      - factor: -128
        program-point:
          marker : 8
      - factor: 1
        program-point:
          marker : 10
      - factor: 1
        program-point:
          marker : 11
      - factor: 1
        program-point:
          marker : 12
    op: less-equal
    rhs: 0
    level: bitcode
    origin: user.bc
  - scope:
      function: merge_sort_nrecursive
    lhs:
      - factor: -128
        program-point:
          marker : 8
      - factor: 1
        program-point:
          marker : 13
    op: less-equal
    rhs: 0
    level: bitcode
    origin: user.bc
