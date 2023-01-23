#ifndef NETHELPERS_H
#define NETHELPERS_H

#include <netinet/in.h>
#include <sys/socket.h>

#include "errorHelpers.h"

int MY_WARN_UNUSED MY_NON_NULL(1)
	init_sockaddr_in(struct sockaddr_in* addr,
			sa_family_t family, in_port_t port, uint32_t s_addr) {
		NON_ATTR_IF(!addr) {
			DBGprintf("null sockaddr_in struct? bruhhh");
			return -1;
		}

		memset((void*) addr, 0, sizeof(struct sockaddr_in));

		addr->sin_family = family;
		addr->sin_port = port;
		addr->sin_addr.s_addr = s_addr;

		return 0;
	}

struct sockaddr_in MY_WARN_UNUSED
make_sockaddr_in(sa_family_t family, in_port_t port, uint32_t s_addr) {
	struct sockaddr_in addr;
	ERR_NEG1(init_sockaddr_in(&addr, family, port, s_addr));

	return addr;
}

in_addr_t MY_WARN_UNUSED MY_NON_NULL(1)
	inet_addr_(const char* cp) {
		NON_ATTR_IF(!addr) {
			ERR("null string? bruhhh");
		}

		in_addr_t addr;
		ERR_IN_ADDR_T(addr = inet_addr(cp));
		return addr;
	}

#endif // NETHELPERS_H
