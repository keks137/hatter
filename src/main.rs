use std::collections::HashSet;
use std::io::BufReader;
use std::io::prelude::*;
use std::process;
use std::{env, fs::File};

fn main() -> std::io::Result<()> {
    let types: HashSet<&str> = ["int", "bool"].iter().cloned().collect();
    let args: Vec<String> = env::args().collect();
    //dbg!(args);
    if args.len() != 2 {
        println!("Err: You should provide exactly one argument");
        process::exit(1);
    }
    let file = File::open(&args[1]).expect("Couldn't open file");
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
