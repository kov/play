#[link_name = "hello"]
native mod native_hello {
    fn hello(to: *u8);
}

fn main() {
    let to = str::bytes("Rust");

    unsafe {
        native_hello::hello(vec::unsafe::to_ptr(to));
    }
}
