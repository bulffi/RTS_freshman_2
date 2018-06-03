#ifndef __SERVER_H__
#define __SERVER_H__

#include<iostream>
#include<string>
#include<vector>
#include<deque>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include<thread>

using namespace boost::asio;
using namespace boost::posix_time;
io_service service_2;

class talk_to_client;
typedef boost::shared_ptr<talk_to_client> client_ptr;
typedef std::vector<client_ptr> array;
array clients;

#define MEM_FN(x)       boost::bind(&self_type::x, shared_from_this())
#define MEM_FN1(x,y)    boost::bind(&self_type::x, shared_from_this(),y)
#define MEM_FN2(x,y,z)  boost::bind(&self_type::x, shared_from_this(),y,z)

std::string the_true_and_only_host_name;
void update_changes(int);

/** simple connection to server:
- logs in just with username (no password)
- all connections are initiated by the client: client asks, server answers
- server disconnects any client that hasn't pinged for 5 seconds

Possible client requests:
- gets a list of all connected clients
- ping: the server answers either with "ping ok" or "ping client_list_changed"
*/
class talk_to_client : public boost::enable_shared_from_this<talk_to_client>
	, boost::noncopyable {
	typedef talk_to_client self_type;
	talk_to_client() : sock_(service_2), started_(false),
		timer_(service_2), status(0) {
	}
public:
	typedef boost::system::error_code error_code;
	typedef boost::shared_ptr<talk_to_client> ptr;

	void start() {
		started_ = true;
		clients.push_back(shared_from_this());
		last_ping = boost::posix_time::microsec_clock::local_time();
		// first, we wait for client to login
		do_read();
	}
	static ptr new_() {
		ptr new_(new talk_to_client);
		return new_;
	}
	void stop() {
		if (!started_) return;
		started_ = false;
		sock_.close();

		ptr self = shared_from_this();
		array::iterator it = std::find(clients.begin(), clients.end(), self);
		clients.erase(it);
		update_changes(1);
	}
	bool started() const { return started_; }
	ip::tcp::socket & sock() { return sock_; }
	std::string username() const { return username_; }
	void set_changes(int a) { status = a; }
private:
	void on_read(const error_code & err, size_t bytes) {
		if (err) stop();
		if (!started()) return;
		// process the msg
		std::string msg(read_buffer_, bytes);
		msg.pop_back();
		//std::cout <<"on read" <<msg << std::endl;
		if (msg.find("login ") == 0) on_login(msg);
		else if (msg.find("ping") == 0) on_ping();
		else if (msg.find("ask_clients") == 0) on_clients();
		else if (msg.find("what_happend") == 0)
		{
			//	std::cout << "i am asked what happend" << std::endl;
			on_broadcast();
		}
		else if (msg.find("who_are_you") == 0)on_tell_host();
		else on_something_to_say(msg);

	}
	void on_tell_host()
	{
		do_write("my_name_is"+the_true_and_only_host_name + "$");
	}
	void on_something_to_say(std::string msg)
	{
		std::istringstream in(msg);
		std::string tempt;
		while (in>>tempt)
		{
			for (auto client : clients)
			{
				client->messages_to_send.push_back(tempt);
			}
			std::cout << tempt << std::endl;;
		}
		update_changes(2);
		do_write("something_happend$");
		status = 0;
	}
	void on_broadcast()
	{
		std::string tempt;

		while (!messages_to_send.empty())
		{
			tempt += messages_to_send.front() + " ";
			messages_to_send.pop_front();
		}
		tempt += "$";
		//std::cout << username_ << " is broadcast " << tempt << std::endl;
		do_write(tempt);
	}
	void on_login(const std::string & msg) {
		std::istringstream in(msg);
		in >> username_>>username_;
		//std::getline(in,username_);
		//username_.erase(0);
		if (clients.size() == 1)
		{
			the_true_and_only_host_name = username_;
		}
		std::cout << username_ << " logged in" << std::endl;
		std::string tempt = std::to_string(clients.size());
		user_id = std::make_pair(username_, clients.size());
		log("get my client");
		do_write("login_ok " + tempt + "$");
		update_changes(1);
	}
	void on_ping() {
		//	std::cout << username_ << "ping" << std::endl;
//		log( (username_+"client in touch").c_str());
		switch (status)
		{
		case(0) : do_write("ping ok$");  break;
		case(1) : do_write("client_list_changed$"); break;
		case(2) : do_write("something_happend$"); break;
		}
		status = 0;
	}
	void on_clients() {
		std::string msg;
		//	std::cout << "on clients" << std::endl;
		for (array::const_iterator b = clients.begin(), e = clients.end(); b != e; ++b)
			msg += (*b)->username() + " ";
		do_write("clients " + msg + "$");
	}

	void do_ping() {
		do_write("ping ok$");
	}
	void do_ask_clients() {
		do_write("ask_clients$");
	}

	void on_check_ping() {
		boost::posix_time::ptime now = boost::posix_time::microsec_clock::local_time();
		if ((now - last_ping).total_milliseconds() > 5000) {
			std::cout << "stopping " << username_ << " - no ping in time" << std::endl;
			//stop();
		}
//		log("ponst %d", (now - last_ping).total_milliseconds());
		last_ping = boost::posix_time::microsec_clock::local_time();
	}
	void post_check_ping() {
		timer_.expires_from_now(boost::posix_time::millisec(5000));
		timer_.async_wait(MEM_FN(on_check_ping));
	}


	void on_write(const error_code & err, size_t bytes) {
		do_read();
	}
	void do_read() {
		async_read(sock_, buffer(read_buffer_),
			MEM_FN2(read_complete, _1, _2), MEM_FN2(on_read, _1, _2));
		//on_check_ping();
		
		post_check_ping();
	}
	void do_write(const std::string & msg) {
		if (!started()) return;
		std::copy(msg.begin(), msg.end(), write_buffer_);
		sock_.async_write_some(buffer(write_buffer_, msg.size()),
			MEM_FN2(on_write, _1, _2));
	}
	size_t read_complete(const boost::system::error_code & err, size_t bytes) {
		if (err) return 0;
		bool found = std::find(read_buffer_, read_buffer_ + bytes, '$') < read_buffer_ + bytes;
		// we read one-by-one until we get to enter, no buffering
		return found ? 0 : 1;
	}
private:
	ip::tcp::socket sock_;
	enum { max_msg = 1024 };
	char read_buffer_[max_msg];
	char write_buffer_[max_msg];
	bool started_;
	std::string username_;
	std::pair<std::string, int> user_id;
	std::deque<std::string> messages_to_send;
	deadline_timer timer_;
	boost::posix_time::ptime last_ping;
	int status;
	
};

void update_changes(int status) {
	for (array::iterator b = clients.begin(), e = clients.end(); b != e; ++b)
		(*b)->set_changes(status);
}

ip::tcp::acceptor acceptor(service_2, ip::tcp::endpoint(ip::tcp::v4(), 8001));

void handle_accept(talk_to_client::ptr client, const boost::system::error_code & err) {
	client->start();
	talk_to_client::ptr new_client = talk_to_client::new_();
	acceptor.async_accept(new_client->sock(), boost::bind(handle_accept, new_client, _1));
}
void run_()
{
	log("run server");
	service_2.run();
}
void run_server()
{
	std::thread  t(run_);
	t.detach();
}
	
#endif