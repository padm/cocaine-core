#ifndef COCAINE_BACKENDS_ABSTRACT_HPP
#define COCAINE_BACKENDS_ABSTRACT_HPP

#include "cocaine/common.hpp"
#include "cocaine/forwards.hpp"

namespace cocaine { namespace engine {

class backend_t:
    public boost::noncopyable,
    public unique_id_t,
    public birth_control_t<backend_t>
{
    public:
        typedef std::map<
            const std::string,
            boost::shared_ptr<lines::promise_t>
        > request_queue_t;

    protected:
        backend_t();

    public:       
        virtual ~backend_t();

    public:
        void rearm(float timeout);
        
        bool active() const;

        request_queue_t& queue();
        const request_queue_t& queue() const;

    protected:
        virtual void timeout(ev::timer& w, int revents) = 0;

    private:
        bool m_active;
        request_queue_t m_queue;
        ev::timer m_heartbeat;
};

}}

#endif
