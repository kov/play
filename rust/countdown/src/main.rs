use tokio::net::TcpListener;
use tokio::prelude::*;

#[tokio::main]
async fn main() -> Result<(), Box<dyn std::error::Error>> {
    let mut listener = TcpListener::bind("127.0.0.1:8080").await?;

    loop {
        let (mut socket, _) = listener.accept().await?;

        tokio::spawn(async move {
            let mut socket = tokio::io::BufStream::new(socket);

            // In a loop, read data from the socket and write the data back.
            loop {
                // Read all of the request.
                loop {
                    let mut buf = String::new();

                    match socket.read_line(&mut buf).await {
                        // socket closed
                        Ok(n) if n == 0 => return,
                        Ok(_) => (),
                        Err(e) => {
                            eprintln!("failed to read from socket; err = {:?}", e);
                            return;
                        }
                    };

                    if buf.as_bytes() == b"\r\n" {
                        break;
                    }
                }
                // Write back a response.
                let response: Vec::<&str> = vec!["5", "4", "3", "2", "1"];
                let response = response.join("\r\n");
                let headers = format!("HTTP/1.1 200 OK\r\nContent-Length: {}\r\n\r\n", response.len());
                println!("{:?}", headers);
                if let Err(e) = socket.write_all(headers.as_bytes()).await {
                    eprintln!("failed to write to socket; err = {:?}", e);
                    return;
                }

                for resp in response.split("\r\n") {
                    let resp = format!("{}\r\n", resp);
                    std::thread::sleep(std::time::Duration::new(1, 0));
                    if let Err(e) = socket.write_all(resp.as_bytes()).await {
                        eprintln!("failed to write to socket; err = {:?}", e);
                        return;  
                    }

                    if let Err(e) = socket.flush().await {
                        eprintln!("failed to write to socket; err = {:?}", e);
                        return;  
                    }
                }

                if let Err(e) = socket.write_all(b"\r\n\r\n").await {
                    eprintln!("failed to write to socket; err = {:?}", e);
                    return;  
                }

                if let Err(e) = socket.flush().await {
                    eprintln!("failed to write to socket; err = {:?}", e);
                    return;  
                }
        }
        });
    }
}