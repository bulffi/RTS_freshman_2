

#include <boost/thread.hpp>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include<deque>
#include<iostream>
using namespace boost::asio;
io_service service;

#define MEM_FN(x)       boost::bind(&self_type::x, shared_from_this())
#define MEM_FN1(x,y)    boost::bind(&self_type::x, shared_from_this(),y)
#define MEM_FN2(x,y,z)  boost::bind(&self_type::x, shared_from_this(),y,z)

class talk_to_svr : public boost::enable_shared_from_this<talk_to_svr>
	, boost::noncopyable {
	typedef talk_to_svr self_type;
	talk_to_svr(const std::string & username)
		: sock_(service), started_(true), username_(username) {}
	void start(ip::tcp::endpoint ep) {
		sock_.async_connect(ep, MEM_FN1(on_connect, _1));
	}
	
public:
	typedef boost::system::error_code error_code;
	typedef boost::shared_ptr<talk_to_svr> ptr;
	static ptr start(ip::tcp::endpoint ep, const std::string & username) {
		ptr new_(new talk_to_svr(username));
		new_->start(ep);
		return new_;
	}
	void write(std::string msg)
	{
		message_to_send.push_back(msg);
	}
	std::string read()
	{
		if (!message_to_accept.empty())
		{
			std::string tempt = message_to_accept.front();
			message_to_accept.pop_front();
			return tempt;
		}
		return "no";
	}
	void stop() {
		if (!started_) return;
		//std::cout << "stopping " << username_ << std::endl;
		started_ = false;
		sock_.close();
	}
	bool started() { return started_; }
private:
	void on_connect(const error_code & err) {
		if (!err)      do_write("login " + username_ + "$");
		else            stop();
	}
	void on_read(const error_code & err, size_t bytes) {
		if (err) stop();
		if (!started()) return;
		// process the msg
		std::string msg(read_buffer_, bytes);
		msg.pop_back();
		if (msg.find("login_ok") == 0)
			on_login(msg);
		else if (msg.find("ping ok") == 0) 
			on_ping_ok();
		else if (msg.find("clients ") == 0) 
			on_clients(msg);
		else if (msg.find("client_list_changed") == 0)
			do_ask_clients();
		else if (msg.find("something_happend") == 0)
			on_ask_what_happend();
		else
		{
			on_record(msg);
		}
	}
	void on_record(std::string msg)
	{
		std::istringstream in(msg);
		std::string tempt;
		//std::cout << "i have sth to record" << msg << std::endl;
		while (in >> tempt)
		{
			message_to_accept.push_back(tempt); 
		//	std::cout << tempt << std::endl;;
		}
		do_ping();

		
	}
	void on_ask_what_happend()
	{
		do_write("what_happend$");
	}
	void on_login(std::string msg) {
		//std::cout << username_ << " logged in" << std::endl;
		std::string tempt;
		std::istringstream in(msg);
		in >> tempt >> tempt;
		int id = std::atoi(tempt.c_str());
		user_id = std::make_pair(username_, id);
		//do_ask_clients();
		do_ping();
	}
	void on_ping_ok() {
		if (message_to_send.empty())
		{
			do_ping();
		}
		else
		{
			std::string tempt;
			while (!message_to_send.empty())
			{
				tempt += message_to_send.front() + " ";
				message_to_send.pop_front();
			}
			tempt += "$";
			do_write(tempt);
		}
	}
	void on_clients(const std::string & msg) {
		std::string clients = msg.substr(8);
	//	std::cout << username_ << ", new client list:" << clients << std::endl;
		do_ping();
	}

	void do_ping() {
		do_write("ping$");
	}
	void do_ask_clients() {
		do_write("ask_clients$");
	}

	void on_write(const error_code & err, size_t bytes) {
		do_read();
	}
	void do_read() {
		async_read(sock_, buffer(read_buffer_),
			MEM_FN2(read_complete, _1, _2), MEM_FN2(on_read, _1, _2));
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
	std::deque<std::string> message_to_send;
	std::deque<std::string> message_to_accept;
	std::pair<std::string, int> user_id;
	//deadline_timer timer_;
};
void run_client()
{
	service.run();
}















