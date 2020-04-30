#ifndef redis_lock_h__
#define redis_lock_h__

#include <thread>
#include <atomic>
#include <string>
#include "client.hpp"

namespace cpp_redis {
	class redis_lock {
	public:
		redis_lock(const std::shared_ptr<redis_client<String>>& client):client_(client) {
			try
			{
				ios_ = traits::make_unique<boost::asio::io_service >();
				timer_ = traits::make_unique<boost::asio::steady_timer>(*ios_);
				thread_ = traits::make_unique<std::thread>([this]() {
					while (exit_) {
						boost::system::error_code ec;
						ios_->poll(ec);
						if (ec) {
							break;
						}

						std::this_thread::sleep_for(std::chrono::milliseconds(200));
					}
					});
			}
			catch (const std::exception & ex)
			{
				client_ = nullptr;
				ios_ = nullptr;
				timer_ = nullptr;
				thread_ = nullptr;
			}

		}

		void lock(size_t seconds = 30) {
			if (client_ == nullptr){
				return;
			}

			while (1) {
				std::string key = key_;
				if (client_->setnx(std::move(key), 1, seconds)) {
					reset();
					return;
				}

				if (!is_start_timer){
					is_start_timer = true;
					timer_check_key();
				}

				std::this_thread::sleep_for(std::chrono::milliseconds(200));
			}
		}

		void unlock() {
			if (client_ ==nullptr){
				return;
			}

			reset();
			std::string key = key_;
			client_->delete_key(std::move(key));
		}

		~redis_lock() {
			unlock();
			exit_ = false;

			//线程回收资源，防止孤线程
			thread_->join();
		}

		redis_lock(const redis_lock&) = default;
		redis_lock(redis_lock&&) = default;
		redis_lock& operator=(const redis_lock&) = default;
		redis_lock& operator=(redis_lock&&) = default;
	private:
		void reset() {
			cancle_timer();
			is_start_timer = false;
		}

		void cancle_timer() {
			boost::system::error_code ec;
			timer_->cancel(ec);
		}

		void timer_check_key() {
			timer_->expires_from_now(std::chrono::milliseconds(1));
			timer_->async_wait([this](const boost::system::error_code& ec) {
				if (ec) {
					return;
				}

				std::string key = key_;
				int seconds = client_->remainder_ttl(std::move(key));
				if (seconds <=0){
					return;
				}

				if (seconds<10){
					key = key_;
					client_->setnx(std::move(key), 1, 30);
				}

				timer_check_key();
				});
		}

	private:
		std::atomic<bool>exit_ = true;
		const std::string key_ = "lock";
		std::atomic<bool>is_start_timer = false;
		std::unique_ptr<std::thread>thread_;
		std::unique_ptr<boost::asio::steady_timer>timer_;
		std::shared_ptr<redis_client<String>>client_;
		std::unique_ptr<boost::asio::io_service>ios_;
	};
}
#endif // redis_lock_h__
