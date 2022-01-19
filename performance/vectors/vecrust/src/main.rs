#![feature(test)]
extern crate test;

fn main() {
}

#[cfg(test)]
mod tests {
    use test::Bencher;

    const NUM_ITEMS_SMALL: usize = 10;
    const NUM_ITEMS_BIG: usize = 1_000_000;

    #[bench]
    fn small(b: &mut Bencher) {
        let mut v = vec![];
        b.iter(move || {
            for i in 0..NUM_ITEMS_SMALL {
                v.push(i);
            }
            v.clear();
        })
    }

    #[bench]
    fn big(b: &mut Bencher) {
        let mut v = vec![];
        b.iter(move || {
            for i in 0..NUM_ITEMS_BIG {
                v.push(i);
            }
            v.clear();
        })
    }
}