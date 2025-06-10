use std::collections::HashSet;
use std::fmt::format;
use std::io::BufReader;
use std::io::prelude::*;
use std::path::Path;
use std::process;
use std::{env, fs::File};

fn main() -> std::io::Result<()> {
    let types: HashSet<&str> = ["int", "bool", "void", "float"].iter().cloned().collect();
    //let not_types: HashSet<&str> = ["if", "for", "while"].iter().cloned().collect();
    let args: Vec<String> = env::args().collect();
    //dbg!(args);
    if args.len() != 2 {
        eprintln!("Usage: {} <file_path>", args[0]);
        process::exit(1);
    }
    let file_path = Path::new(&args[1]);
    let file = File::open(&file_path).expect("Couldn't open file");

    let file_type = file_path.extension().unwrap();
    if file_type != "c" {
        eprintln!(
            "Please insert a .c file, not a .{} file",
            &file_type.to_str().unwrap()
        );
    }
    let file_stem = file_path.file_stem();

    //println!("{:?}", &file_type);
    //let include_name = format("INCLUDE_", );
    println!("#ifndef ");
    let mut buf_reader = BufReader::new(&file);
    let mut contents = String::new();
    buf_reader.read_to_string(&mut contents)?;
    'line_loop: for line in contents.lines() {
        match line.split_whitespace().next() {
            Some(first_word) => {
                if types.contains(first_word) {
                    ()
                } else {
                    continue;
                };
            }
            None => continue,
        }
        let mut buffer = String::new();
        //let mut contains_open_bracket = false;
        for c in line.chars() {
            match c {
                '{' => break,
                ';' => continue 'line_loop,
                _ => buffer.push(c),
            }
        }
        println!("{};", buffer.trim());
    }
    Ok(())
}
