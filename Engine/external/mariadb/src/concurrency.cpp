









#include <mysql.h>
#include <mariadb++/types.hpp>

#include <atomic>
#include <thread>
#include <mutex>

#define LOCK_MUTEX() std::lock_guard<std::mutex> lock(g_mutex)

#include "worker.hpp"
#include "private.hpp"

using namespace mariadb;
using namespace mariadb::concurrency;

namespace {
handle g_next_handle(0);
account_ref g_account;
std::mutex g_mutex;
std::vector<worker*> g_querys_in;
std::map<handle, worker*> g_querys_out;
bool g_thread_running(false);

typedef std::map<handle, worker*> map_t;




void worker_thread() {
    g_thread_running = true;

    mysql_thread_init();

    
    while (true) {
        worker* w = NULL;
        {
            LOCK_MUTEX();
            auto possible_w = g_querys_in.begin();
            if (possible_w == g_querys_in.end()) break;

            w = *possible_w;
            g_querys_in.erase(possible_w);
        }

        w->execute();

        if (!w->keep_handle()) delete w;
    }

    g_thread_running = false;
    mysql_thread_end();
}




void start_thread() {
    if (!g_thread_running) {
        std::thread t(worker_thread);
        t.detach();
    }
}




const worker& get_worker(handle h) {
    LOCK_MUTEX();
    const map_t::const_iterator w = g_querys_out.find(h);

    if (w != g_querys_out.end()) return *w->second;

    static worker removed;
    return removed;
}




handle add(const std::string& query, command::type command, bool keep_handle) {
    LOCK_MUTEX();
    worker* w = new worker(g_account, ++g_next_handle, keep_handle, command, query);
    g_querys_in.push_back(w);

    if (keep_handle) g_querys_out[g_next_handle] = w;

    start_thread();

    return g_next_handle;
}

handle add(statement_ref& statement, command::type command, bool keep_handle) {
    LOCK_MUTEX();
    worker* w = new worker(g_account, ++g_next_handle, keep_handle, command, statement);
    g_querys_in.push_back(w);

    if (keep_handle) g_querys_out[g_next_handle] = w;

    start_thread();

    return g_next_handle;
}
}




void concurrency::set_account(account_ref& account) { g_account = account; }




status::type concurrency::worker_status(handle h) {
    const worker& w = get_worker(h);
    return w.status();
}




u64 concurrency::get_execute_result(handle h) {
    const worker& w = get_worker(h);
    return w.status() == status::removed ? 0 : w.result();
}

u64 concurrency::get_insert_result(handle h) {
    const worker& w = get_worker(h);
    return w.status() == status::removed ? 0 : w.result();
}

result_set_ref concurrency::get_query_result(handle h) {
    const worker& w = get_worker(h);
    return w.status() == status::removed ? result_set_ref() : w.result_set();
}




handle concurrency::execute(const std::string& query, bool keep_handle) {
    return add(query, command::execute, keep_handle);
}

handle concurrency::insert(const std::string& query, bool keep_handle) {
    return add(query, command::insert, keep_handle);
}

handle concurrency::query(const std::string& query, bool keep_handle) {
    return add(query, command::query, keep_handle);
}




statement_ref concurrency::create_statement(const std::string& query) {
    connection_ref connection = connection::create(g_account);
    statement_ref statement = connection->create_statement(query);
    statement->set_connection(connection);
    return statement;
}

handle concurrency::execute(statement_ref& statement, bool keep_handle) {
    return add(statement, command::execute, keep_handle);
}

handle concurrency::insert(statement_ref& statement, bool keep_handle) {
    return add(statement, command::insert, keep_handle);
}

handle concurrency::query(statement_ref& statement, bool keep_handle) {
    return add(statement, command::query, keep_handle);
}






void concurrency::release_handle(handle h) {
    LOCK_MUTEX();
    map_t::iterator w = g_querys_out.find(h);

    if (w == g_querys_out.end()) return;

    delete w->second;
    g_querys_out.erase(w);
}

bool concurrency::wait_handle(handle h, u64 wait_time_ms) {
    while (worker_status(h) < status::succeed) {
        std::this_thread::sleep_for(std::chrono::milliseconds(wait_time_ms));
    }

    return worker_status(h) == status::succeed;
}
