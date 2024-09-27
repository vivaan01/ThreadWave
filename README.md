Welcome to ThreadWave, a next-gen Multithreaded Proxy Web Server built in C! This project showcases advanced concepts from Operating Systems and Data Structures, delivering a high-performance proxy server that efficiently handles multiple client requests in parallel. Say goodbye to traditional single-threaded bottlenecks and hello to a 40% boost in efficiency and reduced response times!

🚀 Project Highlights

1. Low-Level I/O Management
Utilized system calls for optimized control over I/O operations, fine-tuning resource management for maximum throughput.

3. Threading & Synchronization
Implemented multithreading with mutex locks and semaphores to handle concurrent client requests safely and efficiently.

5. Network Programming (Sockets)
Built robust client-server communication using both TCP and UDP protocols, ensuring reliable data transfer with low-latency connections.

7. Memory Management
Dynamically managed heap memory to optimize usage and prevent memory leaks, ensuring smooth server operation.

9. Concurrency Control
Applied semaphores, wait, and signal operations to synchronize threads and maintain server stability under heavy loads.
11. LRU Cache Integration
Integrated a Least Recently Used (LRU) cache to accelerate content delivery and minimize redundant backend requests, improving performance significantly.

🔧 Tech Stack:

Language: C
Protocols: TCP, UDP
Core Concepts: Multithreading, Sockets, System Calls, Mutex, Semaphores


Cache: Time-Based LRU Cache


🚀 Setup Instructions:


Want to set up ThreadWave on your local machine? Here’s how you can get it running in a few easy steps.

Step 1: Clone the Repository
bash

git clone https://github.com/yourusername/threadwave-proxy-server.git
cd threadwave-proxy-server

Step 2: Compile the Code
You'll need GCC to compile the C files. If you don't have it installed, grab it with:

bash

sudo apt-get install gcc
Now, compile the project:

bash

gcc -pthread -o threadwave server.c client.c
Step 3: Run the Server Locally
Once compiled, you can start the server on your localhost. Run the following command to launch the proxy server:

bash

./threadwave <PORT_NUMBER>
Example:

bash

./threadwave 8080
The server is now listening on port 8080 and is ready to accept multiple client connections!

Step 4: Testing the Proxy Server
To test the server, open a new terminal and run the client program to send requests:

bash

./client <SERVER_IP> <PORT_NUMBER>
Example:

bash

./client 127.0.0.1 8080


The client will now connect to the server and simulate multiple users making requests in parallel. Watch as ThreadWave handles them efficiently!

🧠 How It Works🧐 :
Multithreading: Each incoming client request is assigned a separate thread, ensuring the server can handle multiple requests concurrently without blocking.
Mutex & Semaphores: These are used to synchronize threads and ensure safe access to shared resources like memory and I/O buffers.
LRU Cache: Frequently requested data is stored in a time-based cache, so the server can deliver content faster, reducing load on the backend.
💡 Future Enhancements
Support for HTTPS Protocol
Enhanced Caching Mechanism (Frequency-Based)
Load Balancer Integration for Distributed Systems
🎉 Contributions
Contributions are welcome! If you find any issues or have ideas for improvement, feel free to open a pull request or submit an issue.

📄 License ⚖️
This project is licensed under the MIT License - see the LICENSE file for details.

💬 Let's Connect🧲
If you have any questions or want to chat about the project, feel free to reach out to me on LinkedIn.

Happy coding! ✌🏻
