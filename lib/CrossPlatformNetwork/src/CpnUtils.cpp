/*
** EPITECH PROJECT, 2023
** B-CPP-500-PAR-5-1-rtype-ethan.nguyen
** File description:
** CrossPlatformNetwork
*/

#include "CpnUtils.hpp"

uint32_t cpn::Utils::ipStringToInt(const std::string &ip)
{
    int i = 0;
    uint32_t finalIp = 0;
    uint8_t bytes[sizeof(finalIp)];
    std::string tmp;
    std::istringstream f(ip);

    while (getline(f, tmp, '.') && i < sizeof(finalIp)) {
        bytes[i] = atoi(tmp.c_str());
        ++i;
    }
    if (i < sizeof(uint32_t))
        throw std::runtime_error("Invalid IP address");
    memcpy(&finalIp, bytes, sizeof(uint32_t));
    return (ntohl(finalIp));
}

std::string cpn::Utils::ipIntToString(uint32_t ip)
{
    ip = htonl(ip);
    uint8_t *bytes = reinterpret_cast<uint8_t*>(&ip);

    return (std::to_string(bytes[0]) + "." + std::to_string(bytes[1]) + "." + std::to_string(bytes[2]) + "." + std::to_string(bytes[3]));
}

cpn::connectionID_t cpn::Utils::getConnectionID(const sockaddr_in &sockaddr)
{
    return (cpn::Utils::getConnectionID(cpn::Utils::getIPInt(sockaddr), cpn::Utils::getPort(sockaddr)));
}

cpn::connectionID_t cpn::Utils::getConnectionID(const std::string &ip, uint16_t port)
{
    return (cpn::Utils::getConnectionID(cpn::Utils::ipStringToInt(ip), port));
}

cpn::connectionID_t cpn::Utils::getConnectionID(uint32_t ip, uint16_t port)
{
    cpn::connectionID_t cid;

    cid._ip = ip;
    cid._port = port;
    cid._valid = 1;
    return (cid);
}

std::string cpn::Utils::getIP(const sockaddr_in &sockaddr)
{
    #ifdef _WIN32
        return (cpn::Utils::ipIntToString(ntohl(sockaddr.sin_addr.S_un.S_addr)));
    #else
        return (cpn::Utils::ipIntToString(ntohl(sockaddr.sin_addr.s_addr)));
    #endif
}

std::string cpn::Utils::getIP(cpn::connectionID_t cid)
{
    return (cpn::Utils::ipIntToString(cid._ip));
}

uint32_t cpn::Utils::getIPInt(const sockaddr_in &sockaddr)
{
    #ifdef _WIN32
        return (ntohl(sockaddr.sin_addr.S_un.S_addr));
    #else
        return (ntohl(sockaddr.sin_addr.s_addr));
    #endif
}

uint32_t cpn::Utils::getIPInt(cpn::connectionID_t cid)
{
    return (cid._ip);
}

uint16_t cpn::Utils::getPort(const sockaddr_in &sockaddr)
{
    return (ntohs(sockaddr.sin_port));
}

uint16_t cpn::Utils::getPort(cpn::connectionID_t cid)
{
    return (cid._port);
}

void cpn::Utils::setSockAddr(sockaddr_in &sockaddr, const cpn::connectionID_t &cid)
{
    cpn::Utils::setSockAddr(sockaddr, cid._ip, cid._port);
}

void cpn::Utils::setSockAddr(sockaddr_in &sockaddr, const std::string &ip, uint16_t port)
{
    cpn::Utils::setSockAddr(sockaddr, cpn::Utils::ipStringToInt(ip), port);
}

void cpn::Utils::setSockAddr(sockaddr_in &sockaddr, uint32_t ip, uint16_t port)
{
    memset(&sockaddr, 0 , sizeof(sockaddr));
    sockaddr.sin_family = AF_INET;
    sockaddr.sin_port = htons(port);
    #ifdef _WIN32
        sockaddr.sin_addr.S_un.S_addr = htonl(ip);
    #else
        sockaddr.sin_addr.s_addr = htonl(ip);
    #endif
}



// ssize_t cpn::isDataAvailable(int socket, struct sockaddr_in &servaddr)
// {
//     int flags;
//     ssize_t bytes;
//     char data[MAX_BUF_SIZE];
//     #ifdef _WIN32
//         u_long mode = nonBlock;
//         int sockLen;

//         flags = MSG_PEEK;
//         ioctlsocket(socket, FIONBIO, &mode);
//     #else
//         socklen_t sockLen = 0;

//         flags = MSG_DONTWAIT | MSG_PEEK;
//     #endif
//     sockLen = sizeof(servaddr);
//     bytes = recvfrom(socket, data, MAX_BUF_SIZE, flags,
//         (struct sockaddr *) &servaddr, &sockLen);
//     return (bytes);
// }

int cpn::Utils::udpSocket(void)
{
    #ifdef _WIN32
        return (socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP));
    #else
        return (socket(AF_INET, SOCK_DGRAM, 0));
    #endif
}

int cpn::Utils::tcpSocket(void)
{
    #ifdef _WIN32
        return (socket(AF_INET, SOCK_STREAM, IPPROTO_TCP));
    #else
        return (socket(AF_INET, SOCK_STREAM, 0));
    #endif
}

int cpn::Utils::bindSocket(int socket, const sockaddr_in &sockaddr)
{
    int out;
    sockaddr_in localAddr;
    socklen_t localAddrSize = sizeof(localAddr);

    out = bind(socket, reinterpret_cast<const struct sockaddr*>(&sockaddr),
    sizeof(sockaddr));
    if (!cpn::Utils::bindSucess(out)) {
        std::cerr << "Socket binding failed: " << strerror(errno) << std::endl;
        return (-1);
    }
    getsockname(socket, reinterpret_cast<struct sockaddr*>(&localAddr), &localAddrSize);
    return (cpn::Utils::getPort(localAddr));
}

bool cpn::Utils::bindSucess(int bindReturn)
{
    #ifdef _WIN32
        return (bindReturn != SOCKET_ERROR);
    #else
        return (bindReturn >= 0);
    #endif
}

bool cpn::Utils::socketCreationSuccess(int socket)
{
    #ifdef _WIN32
        return(socket != SOCKET_ERROR);
    #else
        return(socket >= 0);
    #endif
}

void cpn::Utils::closeSocket(int socket)
{
    #ifdef _WIN32
        closesocket(socket);
    #else
        close(socket);
    #endif
}

std::deque<uint8_t> cpn::Utils::recvFrom(int socket, size_t size, struct sockaddr_in &addr, bool nonBlock)
{
    int flags;
    ssize_t bytes;
    std::vector<uint8_t> data(MAX_BUF_SIZE, 0);
    std::deque<uint8_t> finalData;
    #ifdef _WIN32
        u_long mode = nonBlock;
        int sockLen;

        flags = 0;
        ioctlsocket(socket, FIONBIO, &mode);
    #else
        socklen_t sockLen = 0;

        flags = (nonBlock) ? MSG_DONTWAIT : MSG_WAITALL;
    #endif
    sockLen = sizeof(addr);
    bytes = recvfrom(socket, reinterpret_cast<char*>(data.data()), size, flags,
        reinterpret_cast<struct sockaddr *>(&addr), &sockLen);
    data.resize((bytes > 0) ? bytes : 0);
    finalData.insert(finalData.end(), data.begin(), data.end());
    return (std::move(finalData));
}

std::deque<uint8_t> cpn::Utils::recvFromSocket(int socket, size_t size, bool nonBlock)
{
    int flags;
    ssize_t bytes;
    std::vector<uint8_t> data(MAX_BUF_SIZE, 0);
    std::deque<uint8_t> finalData;
    #ifdef _WIN32
        u_long mode = nonBlock;

        flags = 0;
        ioctlsocket(socket, FIONBIO, &mode);
    #else
        flags = (nonBlock) ? MSG_DONTWAIT : MSG_WAITALL;
    #endif
    bytes = recv(socket, reinterpret_cast<char*>(data.data()), size, flags);
    data.resize((bytes > 0) ? bytes : 0);
    finalData.insert(finalData.end(), data.begin(), data.end());
    return (std::move(finalData));
}

ssize_t cpn::Utils::sendToUdp(const std::string &ip, uint16_t port, const void *buf, size_t size)
{
    #ifdef _WIN32
        int flags = 0;
    #else
        int flags = MSG_CONFIRM;
    #endif
    int sock = cpn::Utils::udpSocket();
    ssize_t bytes;
    sockaddr_in addr;
    const char *buffer = static_cast<const char *>(buf);

    if (!(cpn::Utils::socketCreationSuccess(sock)))
        return (-1);
    cpn::Utils::setSockAddr(addr, cpn::Utils::getConnectionID(ip, port));
    bytes = sendto(sock, buffer, size, flags,
    static_cast<const struct sockaddr *>(static_cast<void*>(&addr)), sizeof(addr));
    cpn::Utils::closeSocket(sock);
    LOG("UDP", "Anonymously sent " << bytes << "/" << size << " byte(s) to " << ip << ":" << port << ".");
    return (bytes);
}

ssize_t cpn::Utils::sendToTcp(const std::string &ip, uint16_t port, const void *buf, size_t size)
{
    #ifdef _WIN32
        int flags = 0;
    #else
        int flags = MSG_CONFIRM;
    #endif
    int sock = cpn::Utils::tcpSocket();
    int fd;
    ssize_t bytes;
    sockaddr_in addr;
    const char *buffer = static_cast<const char *>(buf);

    if (!(cpn::Utils::socketCreationSuccess(sock))) {
        LOG("TCP", "Temporary socket creation failed.");
        return (-1);
    }
    cpn::Utils::setSockAddr(addr, cpn::Utils::getConnectionID(ip, port));
    if (connect(sock, reinterpret_cast<const struct sockaddr*>(&addr), sizeof(addr)) == -1) {
        LOG("TCP", "Temporary connection to " << ip << ":" << port << " failed.");
        return (-1);
    }
    bytes = sendto(sock, buffer, size, flags,
    static_cast<const struct sockaddr *>(static_cast<void*>(&addr)), sizeof(addr));
    cpn::Utils::closeSocket(sock);
    LOG("TCP", "Anonymously sent " << bytes << "/" << size << " byte(s) to " << ip << ":" << port << ".");
    return (bytes);
}

ssize_t cpn::Utils::sendTo(int socket, const sockaddr_in &sockaddr, const void *data, size_t size)
{
    ssize_t chunkSize = 0;
    ssize_t pkgSize = 0;
    ssize_t sentSize = 0;
    const char *buf = reinterpret_cast<const char *>(data);

    while (sentSize < size) {
        pkgSize = size - sentSize;
        if (pkgSize > MAX_BUF_SIZE)
            pkgSize = MAX_BUF_SIZE;
        chunkSize = sendto(socket, buf + sentSize, pkgSize, 0,
        reinterpret_cast<const struct sockaddr *>(&sockaddr), sizeof(sockaddr));

        if (chunkSize != pkgSize)
            return (-1);
        sentSize += chunkSize;
    }
    return sentSize;
}

ssize_t cpn::Utils::sendToSocket(int socket, const void *data, size_t size)
{
    ssize_t chunkSize = 0;
    ssize_t pkgSize = 0;
    ssize_t sentSize = 0;
    const char *buf = reinterpret_cast<const char *>(data);

    while (sentSize < size) {
        pkgSize = size - sentSize;
        if (pkgSize > MAX_BUF_SIZE)
            pkgSize = MAX_BUF_SIZE;
        chunkSize = send(socket, buf + sentSize, pkgSize, 0);

        if (chunkSize != pkgSize)
            return (-1);
        sentSize += chunkSize;
    }
    return sentSize;
}

void cpn::Utils::sleep(size_t seconds)
{
    std::this_thread::sleep_for(std::chrono::seconds(seconds));
}

void cpn::Utils::usleep(size_t microSeconds)
{
    std::this_thread::sleep_for(std::chrono::microseconds(microSeconds));
}

void cpn::Utils::msleep(size_t milliSeconds)
{
    std::this_thread::sleep_for(std::chrono::milliseconds(milliSeconds));
}

void cpn::Utils::nsleep(size_t nanoSeconds)
{
    std::this_thread::sleep_for(std::chrono::nanoseconds(nanoSeconds));
}

std::ostream &operator<<(std::ostream &os, const cpn::connectionID_t &cid)
{
    if (!CID_VALID(cid))
        return (os << "X.X.X.X:XXXXX");
    return (os << cpn::Utils::ipIntToString(cid._ip) << ':' << cid._port);

}

std::ostream &operator<<(std::ostream &os, const sockaddr_in &sockaddr)
{
    return (os << cpn::Utils::getIP(sockaddr) << ':' << cpn::Utils::getPort(sockaddr));
}
