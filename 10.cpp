 #include <iostream>
#include <cstring> // memset()
#include <netdb.h> // getaddrinfo(), freeaddrinfo()
#include <arpa/inet.h> // inet_ntop()
#include <sys/types.h> // struct sockaddr
#include <sys/socket.h> // AF_INET, AF_INET6
 void resolveDNS(const std::string& hostname) { struct addrinfo hints, *res, *p; char
ipstr[INET6_ADDRSTRLEN];
 memset(&hints, 0, sizeof hints);
hints.ai_family = AF_UNSPEC; // AF_INET for IPv4, AF_INET6 for IPv6, AF_UNSPEC
for both
hints.ai_socktype = SOCK_STREAM;
 int status = getaddrinfo(hostname.c_str(), NULL, &hints, &res);
if (status != 0) {
std::cerr << "Error: " << gai_strerror(status) << std::endl;
return;
}
 std::cout << "IP addresses for " << hostname << ":\n";
for (p = res; p != NULL; p = p->ai_next) {
void *addr;
std::string ipver;

if (p->ai_family == AF_INET) { // IPv4
struct sockaddr_in *ipv4 = (struct sockaddr_in *)p->ai_addr;
addr = &(ipv4->sin_addr);
ipver = "IPv4";
} else { // IPv6
struct sockaddr_in6 *ipv6 = (struct sockaddr_in6 *)p->ai_addr;
addr = &(ipv6->sin6_addr);
ipver = "IPv6";
}

inet_ntop(p->ai_family, addr, ipstr, sizeof ipstr);
std::cout << ipver << ": " << ipstr << std::endl;
} freeaddrinfo(res);
 }
 int main() { std::string hostname; std::cout << "Enter domain name: "; 
std::cin >>hostname; resolveDNS(hostname); return 0; }
