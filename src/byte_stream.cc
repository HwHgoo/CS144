#include <algorithm>
#include <iostream>
#include <stdexcept>
#include <string_view>
#include <utility>

#include "byte_stream.hh"

using namespace std;

ByteStream::ByteStream( uint64_t capacity ) : capacity_( capacity ) {}

void Writer::push( string data )
{
  if (available_capacity() == 0 || data.empty()) {
    return;
  }

  auto n = min(data.size(), available_capacity());
  data = data.substr(0, n);

  buffer_.push_back(std::move(data));
  bytes_pushed_ += n;
  len_ += n;
}

void Writer::close()
{
  closed = true;
}

void Writer::set_error()
{
  error_ = true;
}

bool Writer::is_closed() const
{
  return closed;
}

uint64_t Writer::available_capacity() const
{
  return capacity_ - len_;
}

uint64_t Writer::bytes_pushed() const
{
  return bytes_pushed_;
}

string_view Reader::peek() const
{
  if (!front_view_.empty()) {
    return front_view_;
  }

  if (buffer_.empty()) {
    return {};
  }

  return buffer_.front();
}

bool Reader::is_finished() const
{
  return closed && len_ == 0;
}

bool Reader::has_error() const
{
  return error_;
}

void Reader::pop( uint64_t len )
{
  auto n = min(len, len_);
  if (front_view_.empty()) {
    front_view_ = buffer_.front();
  }
  while (n > 0) {
    if (n < front_view_.size()) {
      front_view_.remove_prefix(n);
      len_ -= n;
      bytes_poped_ += n;
      return;
    }

    n -= front_view_.size();
    buffer_.pop_front();

    len_ -= front_view_.size();
    bytes_poped_ += front_view_.size();
    if (!buffer_.empty()) {
      front_view_ = buffer_.front();
   } else {
      front_view_ = {};
   }
  }
}

uint64_t Reader::bytes_buffered() const
{
  return len_;
}

uint64_t Reader::bytes_popped() const
{
  return bytes_poped_;
}
