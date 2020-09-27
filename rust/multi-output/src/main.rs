use bus::Bus;
use dashmap::DashMap;
use std::sync::Arc;

struct JobManager {
    map: DashMap<String, Bus<String>>,
}

impl JobManager {
    fn new() -> Arc<Self> {
        Arc::new(JobManager {
            map: DashMap::<String, Bus<String>>::new(),
        })
    }

    fn do_work(&self, s: String) {
        println!("do_work({}) - you should only see this once for each string", s.as_str());

        // Do some fake work.
        match s.as_str() {
            "metl3" => (),
            "pblt3" => std::thread::sleep(std::time::Duration::from_secs(8)),
            _ => std::thread::sleep(std::time::Duration::from_secs(5)),
        }

        // We are done with the work, and thus won't need this bus on the map
        // anymore. Remove it, and broadcast the result to all the receivers.
        let (_, mut bus) = self.map.remove(&s).unwrap();
        bus.broadcast(s.to_uppercase());
    }

    fn subscribe_to(manager: Arc<Self>, s: String) -> String {
        // This gets the Bus from the map if it has already been inserted,
        // otherwise calls the do_work function and inserts a new Bus on the
        // map and returns it.
        let tmanager = manager.clone();
        let mut bus = manager.map.entry(s.clone())
            .or_insert_with(|| {
                std::thread::spawn(move || {
                    tmanager.do_work(s);
                });
                Bus::new(1)
            });

        // Now that we have the Bus, get a receiver for this thread.
        let mut receiver = bus.add_rx();

        // IMPORTANT: need to drop the bus explicitly here, as we are going
        // to block on the recv() call below, and would be effectively holding
        // a lock on the map, leading to a deadlock.
        drop(bus);

        // Here we wait for the processing we are interested in to finish.
        receiver.recv().unwrap()
    }
}

fn main() {
    let manager = JobManager::new();

    // Register interest on processing for 'kov3' on 3 threads. They should
    // all get the results of a single call to do_work('kov3').
    let m = manager.clone();
    let thread1 = std::thread::spawn(move || {
        let upper = JobManager::subscribe_to(m, "kov3".to_string());
        println!("Thread 1: {}", upper);
    });

    let m = manager.clone();
    let thread2 = std::thread::spawn(move || {
        let upper = JobManager::subscribe_to(m, "kov3".to_string());
        println!("Thread 2: {}", upper);
    });

    let m = manager.clone();
    let thread3 = std::thread::spawn(move || {
        let upper = JobManager::subscribe_to(m, "kov3".to_string());
        println!("Thread 3: {}", upper);
    });

    // Register a few more workloads to make sure they are not hampered.
    let m = manager.clone();
    let thread4 = std::thread::spawn(move || {
        let upper = JobManager::subscribe_to(m, "metl3".to_string());
        println!("Thread 4: {}", upper);
    });

    let m = manager;
    let thread5 = std::thread::spawn(move || {
        let upper = JobManager::subscribe_to(m, "pblt3".to_string());
        println!("Thread 5: {}", upper);
    });

    thread1.join().unwrap();
    thread2.join().unwrap();
    thread3.join().unwrap();
    thread4.join().unwrap();
    thread5.join().unwrap();
}
