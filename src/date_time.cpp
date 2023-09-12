#include "date_time.h"

using namespace receiver;

time_zone::time_zone() : native_{0} {}

time_zone::time_zone(value_type offset) : native_{offset} {}

time_zone::time_zone(const std::string& timezone) : native_{0} {
  DWORD index{0};
  std::wstring timezone_name{timezone.begin(), timezone.end()};
  DYNAMIC_TIME_ZONE_INFORMATION time_zone_info{};

  while (EnumDynamicTimeZoneInformation(++index, &time_zone_info) !=
         ERROR_NO_MORE_ITEMS) {
    if (timezone_name.compare(time_zone_info.StandardName) != 0) continue;
    native_ = time_zone_info.Bias * -60;
    break;
  }
}

time_zone receiver::time_zone::current() {
  TIME_ZONE_INFORMATION time_zone_info{};
  GetTimeZoneInformation(&time_zone_info);
  return time_zone{time_zone_info.Bias * -60};
}

receiver::date_time::date_time() : native_{0}, zone_{} {}

receiver::date_time::date_time(const std::string& datetime,
                               date_format standard, const time_zone& zone)
    : native_{0}, zone_{zone} {
  SYSTEMTIME system_time{};

  if (standard == date_format::ISO_8601) {
    sscanf_s(datetime.c_str(), "%04u-%02u-%02uT%02u:%02u:%02u",
             reinterpret_cast<std::uint32_t*>(&system_time.wYear),
             reinterpret_cast<std::uint32_t*>(&system_time.wMonth),
             reinterpret_cast<std::uint32_t*>(&system_time.wDay),
             reinterpret_cast<std::uint32_t*>(&system_time.wHour),
             reinterpret_cast<std::uint32_t*>(&system_time.wMinute),
             reinterpret_cast<std::uint32_t*>(&system_time.wSecond));
  }

  FILETIME file_time{};
  SystemTimeToFileTime(&system_time, &file_time);
  native_ = file_time_to_timestamp(file_time) -
            static_cast<value_type>(zone_.offset());
}

date_time::date_time(value_type timestamp, const time_zone& zone)
    : native_{timestamp}, zone_{zone} {}

date_time& date_time::set_time_zone(const time_zone& timezone) {
  this->zone_ = timezone;
  return *this;
}
date_time& receiver::date_time::operator+=(value_type seconds) {
  this->native_ += seconds;
  return *this;
}

date_time& receiver::date_time::operator-=(value_type seconds) {
  this->native_ -= seconds;
  return *this;
}

date_time& date_time::operator+=(const std::chrono::seconds& seconds) {
  return (*this += seconds.count());
}

date_time& date_time::operator-=(const std::chrono::seconds& seconds) {
  return (*this -= seconds.count());
}

const date_time receiver::date_time::operator+(value_type seconds) const {
  return date_time{*this} += seconds;
}

const date_time receiver::date_time::operator-(value_type seconds) const {
  return date_time{*this} -= seconds;
}

const date_time receiver::date_time::operator+(
    const std::chrono::seconds& seconds) const {
  return date_time{*this} += seconds;
}

const date_time receiver::date_time::operator-(
    const std::chrono::seconds& seconds) const {
  return date_time{*this} -= seconds;
}

date_time& date_time::operator=(value_type timestamp) {
  this->native_ = timestamp;
  return *this;
}

std::string receiver::date_time::format(date_format standard,
                                        bool consider_zone) {
  std::string result{};
  const auto file_time = timestamp_to_file_time(timestamp(true));
  const auto file_offset = timestamp_to_file_time(zone_.offset());
  SYSTEMTIME system_time{}, system_offset{};

  FileTimeToSystemTime(&file_time, &system_time);
  FileTimeToSystemTime(&file_offset, &system_offset);

  if (standard == date_format::ISO_8601) {
    result.resize(21 + 5);
    sprintf_s(&result[0], result.size(),
              "%04u-%02u-%02uT%02u:%02u:%02u+%02u:%02u", system_time.wYear,
              system_time.wMonth, system_time.wDay, system_time.wHour,
              system_time.wMinute, system_time.wSecond, system_offset.wHour,
              system_offset.wMinute);
  }

  return result;
}

date_time receiver::date_time::current() {
  FILETIME file_time{};
  GetSystemTimeAsFileTime(&file_time);
  return date_time{file_time_to_timestamp(file_time)};
}

date_time::value_type receiver::date_time::timestamp(bool consider_zone) const {
  return native_ +
         (consider_zone ? static_cast<value_type>(zone_.offset()) : 0);
}

auto receiver::date_time::file_time_to_timestamp(const FILETIME& file_time)
    -> value_type {
  LARGE_INTEGER time_parts{};
  time_parts.LowPart = file_time.dwLowDateTime;
  time_parts.HighPart = file_time.dwHighDateTime;

  return (time_parts.QuadPart - UNIX_TIME_START) / TICKS_PER_SECOND;
}

auto receiver::date_time::timestamp_to_file_time(value_type timestamp)
    -> FILETIME {
  std::uint64_t ticks{timestamp * TICKS_PER_SECOND + UNIX_TIME_START};
  FILETIME file_time{};
  file_time.dwLowDateTime = (DWORD)ticks;
  file_time.dwHighDateTime = ticks >> 32;

  return file_time;
}
