extern crate rand;

use std::io;
use std::io::Write;
use std::cmp::Ordering;
use rand::Rng;

fn main() {
    let secret = rand::thread_rng().gen_range(1, 101);

    loop {
        print!("Guess the number (1-100): ");
        io::stdout().flush()
            .expect("Flush failed.");
        
        let mut guess = String::new();

        io::stdin().read_line(&mut guess)
            .expect("Failed to read line...");

        let guess: u32 = match guess.trim().parse() {
            Ok(num) => num,
            Err(_) => {
                println!("You did not type a number!");
                continue;
            },
        };

        println!("Your guess: {}", guess);

        match guess.cmp(&secret) {
            Ordering::Less => println!("Too small..."),
            Ordering::Greater => println!("Too big..."),
            Ordering::Equal => {
                println!("Yep!");
                break;
            },
        }
    }
}
