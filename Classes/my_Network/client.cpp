
#include <stdio.h>
#include "client.h"
#include"boost\bind.hpp"
#include"boost\thread.hpp"
static client* the_client;
std::string ip_addr;
void chat_client::do_read_body()
{
	boost::asio::async_read(socket_,
		boost::asio::buffer(read_msg_.body(), read_msg_.body_length()),
		[this](boost::system::error_code ec, std::size_t /*length*/)
	{
		if (!ec)
		{
			the_client->t_lock.lock();
			std::string temp(read_msg_.body(), read_msg_.body_length());
			the_client->_orderList.push_back(temp);
			the_client->t_lock.unlock();
			do_read_header();
		}
		else
		{
			socket_.close();
		}
	});
}
void client::runclient(std::string ip_addres)
{
	ip_addr = ip_addres;
	the_client = this;
	std::thread t(&client::Client_, the_client);
	typedef std::chrono::duration<int, std::milli> millisecond;
	std::this_thread::sleep_for(millisecond(90));
	t.detach();
}

void client::sendMessage(const std::string & message)
{
	chat_message msg;
	std::string temp;
	temp.append(message);
	msg.body_length(temp.size());
	memcpy(msg.body(), temp.c_str(), msg.body_length());
	msg.encode_header();
	_clientInstance->write(msg);
}

std::string client::executeOrder(void)
{
	std::lock_guard<std::mutex> l(the_client->t_lock);
	std::string temp;

	if (_orderList.size() != 0)
	{
		temp = _orderList.front();
		_orderList.pop_front();
	}
	else 
	{
		temp = "no";
	}
	return temp;
}
int client::Client_()
{
	try {
		boost::asio::io_service io_service;

		tcp::resolver resolver(io_service);
		auto endpoint_iterator = resolver.resolve({ ip_addr, "8001" });
		chat_client c(io_service, endpoint_iterator);

		std::thread t([&io_service]() { io_service.run(); });

		_clientInstance = &c;
		while (1)
		{
			;
		}
		c.close();
		t.join();
	}
	catch(std::exception& e)
	{
		std::cerr << "Exception: " << e.what() << "\n";
	}
	return 0;
}