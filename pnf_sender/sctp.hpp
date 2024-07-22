#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/sctp.h>
#include <cstring>

class Sctp
{
public:
    Sctp() : fd_(-1), sender_({AF_INET}), receiver_({AF_INET}){};
    ~Sctp()
    {
        close(fd_);
    }
    bool Create()
    {
        fd_ = socket(PF_INET, SOCK_STREAM, IPPROTO_SCTP);

        return fd_ != -1;
    };
    bool SetLocalAddress(in_addr host, uint16_t port)
    {
        sender_.sin_port = htons(port);
        sender_.sin_addr = host;
        return bind(fd_, (struct sockaddr *)&sender_, sizeof(sender_)) >= 0;
    }
    bool Connect(in_addr host, uint16_t port)
    {
        receiver_.sin_port = htons(port);
        receiver_.sin_addr = host;
        return connect(fd_, (struct sockaddr *)&receiver_, sizeof(receiver_)) == 0;
    };
    int SendMsg(const void *msg, size_t len)
    {
        return sctp_sendmsg(fd_, msg, len, NULL, 0, 0, 0, 0, 0, 0);
    }

private:
    int fd_ = -1;
    struct sockaddr_in sender_;
    struct sockaddr_in receiver_;
};