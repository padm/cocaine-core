/*
    Copyright (c) 2011-2012 Andrey Sibiryov <me@kobology.ru>
    Copyright (c) 2011-2012 Other contributors as noted in the AUTHORS file.

    This file is part of Cocaine.

    Cocaine is free software; you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation; either version 3 of the License, or
    (at your option) any later version.

    Cocaine is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef COCAINE_IO_MESSAGES_HPP
#define COCAINE_IO_MESSAGES_HPP

#include "cocaine/common.hpp"

#include <boost/mpl/int.hpp>
#include <boost/mpl/list.hpp>

namespace cocaine { namespace io {

// Service locator interface

struct locator_tag;

namespace locator {
    struct description_t {
        // An endpoint for the client to connect to in order to use the service.
        std::string endpoint;

        // Service protocol version. If the client wishes to use the service, the
        // protocol versions must match between them.
        unsigned int version;

        // A mapping between method slot numbers and names for use in dynamic
        // languages like Python or Ruby.
        std::map<int, std::string> methods;

        MSGPACK_DEFINE(endpoint, version, methods)
    };

    struct resolve {
        typedef locator_tag tag;

        typedef boost::mpl::list<
            /* service */ std::string
        > tuple_type;
    };
}

template<>
struct protocol<locator_tag> {
    typedef boost::mpl::int_<
        1
    >::type version;

    typedef boost::mpl::list<
        locator::resolve
    > type;
};

// Streaming service interface

struct rpc_tag;

namespace rpc {
    struct handshake {
        typedef rpc_tag tag;

        typedef boost::mpl::list<
            /* peer id */ std::string
        > tuple_type;
    };

    struct heartbeat {
        typedef rpc_tag tag;
    };

    struct terminate {
        typedef rpc_tag tag;

        enum codes: int {
            normal,
            abnormal
        };

        typedef boost::mpl::list<
            /* reason */  int,
            /* message */ std::string
        > tuple_type;
    };

    struct invoke {
        typedef rpc_tag tag;

        typedef boost::mpl::list<
            /* event */ std::string
        > tuple_type;
    };

    struct chunk {
        typedef rpc_tag tag;

        typedef boost::mpl::list<
            /* chunk */ std::string
        > tuple_type;
    };

    struct error {
        typedef rpc_tag tag;

        typedef boost::mpl::list<
            /* code */   int,
            /* reason */ std::string
        > tuple_type;
    };

    struct choke {
        typedef rpc_tag tag;
    };
}

template<>
struct protocol<rpc_tag> {
    typedef boost::mpl::int_<
        1
    >::type version;

    typedef boost::mpl::list<
        rpc::handshake,
        rpc::heartbeat,
        rpc::terminate,
        rpc::invoke,
        rpc::chunk,
        rpc::error,
        rpc::choke
    >::type type;
};

// Logging service interface

struct logging_tag;

namespace logging {
    struct emit {
        typedef logging_tag tag;

        typedef boost::mpl::list<
            /* level */   int,
            /* source */  std::string,
            /* message */ std::string
        > tuple_type;
    };
}

template<>
struct protocol<logging_tag> {
    typedef boost::mpl::int_<
        1
    >::type version;

    typedef boost::mpl::list<
        logging::emit
    > type;
};

// Node service interface

struct node_tag;

namespace node {
    struct start_app {
        typedef node_tag tag;

        typedef boost::mpl::list<
            /* runlist */ std::map<std::string, std::string>
        > tuple_type;
    };

    struct pause_app {
        typedef node_tag tag;

        typedef boost::mpl::list<
            /* applist */ std::vector<std::string>
        > tuple_type;
    };

    struct info {
        typedef node_tag tag;
    };
}

template<>
struct protocol<node_tag> {
    typedef boost::mpl::int_<
        1
    >::type version;

    typedef boost::mpl::list<
        node::start_app,
        node::pause_app,
        node::info
    > type;
};

}} // namespace cocaine::io

#endif
