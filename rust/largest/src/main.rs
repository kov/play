fn largest<'a, T: PartialOrd>(list: &'a [T]) -> &'a T {
    let mut largest = &list[0];
    let mut largest_index = 0;

    for (i, ref item) in list.iter().enumerate() {
        if *item > largest {
            largest = &list[i];
            largest_index = i;
        }
    }

    &list[largest_index]
}

fn main() {
    let list = vec![2, 5, 6, 8];
    println!("Largest: {}", largest(&list));
}
