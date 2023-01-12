# TCP/UDP Port Scan with socket programming

### This is a repository of programs created for OS-B class assignments

### How to compile and run
- TCP Scan<br>
gcc TCPPort-Scanner.c DieWithError.c -o TCPPort-Scanner<br>
./TCPPort-Scanner \<servIP\>


- UDP Scan<br>
chmod +x UDPPort-Scanner.sh<br>
gcc UDPPort-Scanner.c DieWithError.c -o UDPPort-Scanner<br>
./UDPPort-Scanner.sh \<servIP\> \<startPort\> \<endPort\>
