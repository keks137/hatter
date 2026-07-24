package example_add
add :: proc "c" (a: int, b: int) -> int {return a + b}
add_c :: proc "c" (a: c.int, b: c.int) -> c.int {return a + b}
noop :: proc "c" () {}
not_add :: proc(a: int, b: int) -> int {return a + b}
import "core:c"
