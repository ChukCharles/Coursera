#pragma once

#include<iostream>

namespace RAII {
	template<typename Provider>
	class Booking
	{
	public:
		Booking(Provider* p, int c) : provider_(p), counter_(c)
		{}
		Booking(const Booking&) = delete;
		Booking& operator=(const Booking& b) = delete;
		Booking& operator=(Booking&& b) {
			if (provider_) delete provider_;
			provider_ = b.GetProvider();
			b.Release();
			return *this;
		}

		Booking(Booking&& source) {
			provider_ = source.GetProvider();
			source.Release();
		}
		void Release() { provider_ = nullptr; }
		Provider* GetProvider() { return provider_; }

		~Booking() {
			if (provider_) {
				provider_->CancelOrComplete(*this);
			}
		}

	private:
		Provider* provider_;
		int counter_;
	};
}