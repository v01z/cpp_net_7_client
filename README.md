Homework N 7, part 2 (client)

Based on libcpr (a C++ wrapper around libcurl): https://github.com/libcpr/cpr

Using:
    ./client <remote host> <port> <user> <password> <file-to-get>
For example:
    ./client localhost 8080 user2 pass2 /etc/host.conf

Compatible to work with server that made as part 1 of Homework N 7, but can
be replaced with curl:
curl -XGET -v -k 'https://localhost:8080/etc/fstab' -u 'user1:pass1'