use chrono::{DateTime, Local, TimeZone, Utc};
use yahoo_finance::{history};
 
fn main() {
    let data = history::retrieve_range("ANIM3.SA", DateTime::<Utc>::from(Local.ymd(2006, 1, 1).and_hms(0, 0, 0)), None).unwrap();
    for bar in &data {
       println!("On {} ANIM3 closed at ${:.2}", bar.timestamp.format("%b %e %Y"), bar.close)
    }
}

