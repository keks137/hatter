package hatter

main :: proc() {
	// when ODIN_DEBUG {
	// 	track: mem.Tracking_Allocator
	// 	mem.tracking_allocator_init(&track, context.allocator)
	// 	context.allocator = mem.tracking_allocator(&track)
	//
	// 	defer {
	// 		if len(track.allocation_map) > 0 {
	// 			fmt.eprintf("=== %v allocations not freed: ===\n", len(track.allocation_map))
	// 			for _, entry in track.allocation_map {
	// 				fmt.eprintf("- %v bytes @ %v\n", entry.size, entry.location)
	// 			}
	// 		}
	// 		mem.tracking_allocator_destroy(&track)
	// 	}
	// }
	logger := log.create_console_logger()
	context.logger = logger
	defer log.destroy_console_logger(logger)

	pkg, ok := parser.collect_package(os.args[1])
	ensure(ok)
	prsr := parser.Parser{}
	ok = parser.parse_package(pkg, &prsr)
	ensure(ok)
	// fmt.println(pkg)
	// fmt.println(prsr)
	for file_name, file in pkg.files {
		for decl in file.decls {
			value_decl, ok := decl.derived.(^ast.Value_Decl)
			if !ok {continue}

			for val in value_decl.values {
				proc_lit, ok := val.derived.(^ast.Proc_Lit)
				if !ok {continue}

				if len(value_decl.names) <= 0 {continue}
				id, ok_name := value_decl.names[0].derived.(^ast.Ident)
				if !ok_name {continue}
				name := id.name

				pt := proc_lit.type
				if pt.calling_convention != `"c"` && pt.calling_convention != `"cdecl"` {continue}

				ret_count := count_results(pt.results)
				if ret_count > 1 {
					fmt.eprintf(
						"error: %s has %d return values but \"c\" calling convention allows at most 1\n",
						name,
						ret_count,
					)
					continue
				}

				fmt.println(name)
				fmt.println("params:")
				print_field_list(file.src, pt.params)
				fmt.println("ret:")
				print_field_list(file.src, pt.results)
				fmt.println()
			}
		}
	}
}

count_results :: proc(fl: ^ast.Field_List) -> int {
	if fl == nil {return 0}
	count := 0
	for field in fl.list {
		if len(field.names) > 0 {
			count += len(field.names)
		} else {
			count += 1
		}
	}
	return count
}

print_field_list :: proc(src: string, fl: ^ast.Field_List) {
	if fl == nil || len(fl.list) == 0 {
		fmt.println("void")
		return
	}
	for field in fl.list {
		type_str := slice_text(src, field.type.pos.offset, field.type.end.offset)
		fmt.print(builtin_to_c(type_str))
		if len(field.names) > 0 {
			for name_expr in field.names {
				name_str := slice_text(src, name_expr.pos.offset, name_expr.end.offset)
				fmt.print("", name_str)
			}
		}
		fmt.println("")
	}
}

slice_text :: proc(src: string, start, end: int) -> string {
	if start < 0 || end > len(src) || start > end {
		return ""
	}
	return src[start:end]
}
import "core:fmt"
import "core:log"
import "core:mem"
import "core:odin/ast"
import "core:odin/parser"
import "core:odin/tokenizer"
import "core:os"
import "core:strings"
