package hatter
builtin_to_c :: proc(type: string) -> string {
	switch type {
	case "":
		return "void"
	case "c.int":
		return "int"
	case "int":
		return "int64_t"
	case "i32":
		return "int32_t"
	case "byte":
		return "char"
	case:
		unreachable()
	}

}

