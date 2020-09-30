use std::collections::HashMap;
use rand::Rng;
use spmc;
use std::sync::{Arc, Weak, Mutex};

type Receiver<T> = Arc<spmc::Receiver<T>>;

struct Bus<T: Send + 'static> {
    sender: spmc::Sender<T>,
    receiver: spmc::Receiver<T>,
    weak_receivers: Vec<Weak<spmc::Receiver<T>>>,
}

impl<T: Clone + Send> Bus<T> {
    fn new() -> Self {
        let (sender, receiver) = spmc::channel();
        Bus {
            sender,
            receiver,
            weak_receivers: Vec::<Weak<spmc::Receiver<T>>>::new(),
        }
    }

    fn add_receiver(&mut self) -> Receiver<T> {
        let receiver = Arc::new(self.receiver.clone());
        self.weak_receivers.push(Arc::downgrade(&receiver));
        receiver
    }

    fn broadcast(&mut self, msg: T) {
        let sender = &mut self.sender;
        for receiver in &self.weak_receivers {
            if receiver.strong_count() > 0 {
                sender.send(msg.clone()).unwrap();
            }
        }
    }
}

unsafe impl<T: Send> Sync for Bus<T> {}

struct JobManager {
    map: Mutex<HashMap<String, Bus<String>>>,
}

impl JobManager {
    fn new() -> Arc<Self> {
        Arc::new(JobManager {
            map: Mutex::new(HashMap::<String, Bus<String>>::new()),
        })
    }

    fn do_work(&self, s: String) {
        println!("do_work({}) - you should only see this once for each string", s.as_str());

        // Do some fake work.
        match s.as_str() {
            "metl3" => (),
            "pblt3" => std::thread::sleep(std::time::Duration::from_secs(8)),
            _ => { let mut rng = rand::thread_rng(); rng.gen_range(0, 10); },
        }

        // We are done with the work, and thus won't need this bus on the map
        // anymore. Remove it, and broadcast the result to all the receivers.
        self.map.lock().map(|mut map| {
            let mut bus = map.remove(&s).unwrap();
            bus.broadcast(s.to_uppercase());
        }).unwrap();
    }

    fn subscribe_to(manager: Arc<Self>, s: String) -> String {
        // This gets the Bus from the map if it has already been inserted,
        // otherwise calls the do_work function and inserts a new Bus on the
        // map and returns it.
        let ts = s.clone();
        let tmanager = manager.clone();
        let mut map = manager.map.lock().unwrap();
        let bus = map.entry(s.clone())
            .or_insert_with(|| {
                std::thread::spawn(move || {
                    tmanager.do_work(s);
                });
                Bus::new()
            });

        // Now that we have the Bus, get a receiver for this thread.
        let receiver = bus.add_receiver();
        if ts == "pblt3" {
            return "PBLT3".to_string();
        }

        // IMPORTANT: need to drop the bus explicitly here, as we are going
        // to block on the recv() call below, and would be effectively holding
        // a lock on the map, leading to a deadlock.
        drop(bus);
        drop(map);

        std::thread::sleep(std::time::Duration::from_secs(6));

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
