const ITEM_BITSIZE: usize = size_of::<usize>() * 8;

#[derive(Debug)]
struct BitSet {
    bits: Vec<usize>,
}

impl BitSet {
    fn new() -> Self {
        BitSet { bits: vec![] }
    }

    fn len(&self) -> usize {
        self.bits.len() * ITEM_BITSIZE
    }

    fn contains(&self, number: usize) -> bool {
        let bucket = number / ITEM_BITSIZE;
        match self.bits.get(bucket) {
            Some(bucket) => (bucket & (1 << (number % ITEM_BITSIZE))) != 0,
            None => false,
        }
    }

    fn set(&mut self, number: usize) {
        let bucket = number / ITEM_BITSIZE;
        if bucket >= self.bits.len() {
            self.bits
                .append(&mut vec![0usize; bucket - self.bits.len() + 1]);
        }
        self.bits[bucket] |= 1 << (number % ITEM_BITSIZE);
    }
}

fn main() {
    let mut bitset = BitSet::new();
    bitset.set(10);
    bitset.set(128);
    bitset.set(191);
    assert_eq!(bitset.len(), 3 * ITEM_BITSIZE);
    assert_eq!(bitset.contains(0), false);
    assert_eq!(bitset.contains(10), true);
    assert_eq!(bitset.contains(64), false);
    assert_eq!(bitset.contains(128), true);
    println!("{bitset:#?}");
}
