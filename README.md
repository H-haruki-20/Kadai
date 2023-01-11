# TCP/UDP Port Scan with socket programming

### This is a repository of programs created for OS-B class assignments

### How to compile and run
- TCP Scan
gcc TCPPort-Scanner.c DieWithError.c -o TCPPort-Scanner
./TCPPort-Scanner <servIP>

- UDP Scan
gcc UDPPort-Scanner.c DieWithError.c -o UDPPort-Scanner
./UDPPort-Scanner.sh <servIP> <startPort> <endPort>
