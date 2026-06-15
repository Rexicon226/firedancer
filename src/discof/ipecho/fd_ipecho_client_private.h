#ifndef HEADER_fd_src_discof_ipecho_fd_ipecho_client_private_h
#define HEADER_fd_src_discof_ipecho_fd_ipecho_client_private_h

#include "../../util/fd_util_base.h"
#include "../../util/net/fd_net_headers.h"

#include <poll.h>

struct fd_ipecho_client_peer {
  int writing;
  ulong request_bytes_sent;

  ulong response_bytes_read;
  uchar response[ 28UL ];
};

typedef struct fd_ipecho_client_peer fd_ipecho_client_peer_t;

struct fd_ipecho_client_private {
  long start_time_nanos;
  ulong peer_cnt;
  ulong remaining_peer_cnt;

  struct pollfd pollfds[ 16UL ];
  fd_ipecho_client_peer_t peers[ 16UL ];

  /* The entrypoint list is retained so that connections can be
     re-established if they are closed before a response is received
     (e.g. an entrypoint drops an idle connection during a slow boot,
     before the run loop sends the request). */
  fd_ip4_port_t servers[ 16UL ];
  ulong         servers_len;

  ulong magic;
};

#define FD_IPECHO_PARSE_OK  ( 0)
#define FD_IPECHO_PARSE_ERR (-1)

int
fd_ipecho_client_parse_response( uchar const * response,
                                 ulong         response_len,
                                 ushort *      shred_version );

#endif /* HEADER_fd_src_discof_ipecho_fd_ipecho_client_private_h */
