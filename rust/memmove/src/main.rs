const SIZE: usize = 16384;
const NEW_SIZE: usize = SIZE + 128;

fn main() {
    let src = [1u64; SIZE];
    let dst: *mut u64 = unsafe {
        std::alloc::alloc(std::alloc::Layout::array::<u64>(SIZE).expect("Bad layout")).cast()
    };
    unsafe { std::intrinsics::copy(src.as_ptr(), dst, SIZE) };
    println!("Hello, world! {}", src.get(0).unwrap());

    let dst: *mut u64 = unsafe {
        std::alloc::realloc(dst, std::alloc::Layout::array::<u64>(NEW_SIZE), NEW_SIZE)
    }
}
