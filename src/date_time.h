#pragma once

#define WIN32_MEAN_AND_LEAN
#include <Windows.h>
#include <sysinfoapi.h>
#include <timezoneapi.h>

#include <chrono>
#include <string>

namespace receiver {
enum date_format { ISO_8601 };

class time_zone {
 public:
  using value_type = long;

  /**
   * Creates an instance of a class with zero offset.
   */
  time_zone();
  /**
   * Creates an instance of a class with the specified offset.
   *
   * @param offset Offset relative to UTC
   */
  time_zone(value_type offset);
  /**
   * Creates an instance of a class with the specified time zone.
   * The time zone is taken from the Windows registry definitions.
   *
   * @param timezone Name of the time zone from the Windows registry. Example:
   * Russia TZ 2 Standard Time
   */
  time_zone(const std::string& timezone);

  static time_zone current();

  value_type offset() const noexcept { return native_; }

 private:
  value_type native_;
};

class date_time {
 public:
  using value_type = long long;

  /**
   *  Creates an instance of a class with 01-01-1970 00:00:00 datetime.
   */
  date_time();
  /**
   * Creates an instance of a class using a string with date & time.
   * The class does not parse milliseconds and zone offset.
   *
   * @param datetime Date with time in string format
   * @param standard Standard for string processing
   * @param zone Time zone of the specified date
   */
  date_time(const std::string& datetime,
            date_format standard = date_format::ISO_8601,
            const time_zone& zone = {});
  /**
   * Creates an instance of a class using a number with a date & time.
   *
   * @param timestamp Date with time in numeric format
   * @param zone Time zone of the specified date
   */
  date_time(value_type timestamp, const time_zone& zone = {});

  /**
   * Sets the time zone for storing time.
   *
   * @return date_time Reference to this class instance
   */
  date_time& set_time_zone(const time_zone& timezone);

  date_time& operator+=(value_type seconds);
  date_time& operator-=(value_type seconds);
  date_time& operator+=(const std::chrono::seconds& seconds);
  date_time& operator-=(const std::chrono::seconds& seconds);
  const date_time operator+(value_type seconds) const;
  const date_time operator-(value_type seconds) const;
  const date_time operator+(const std::chrono::seconds& seconds) const;
  const date_time operator-(const std::chrono::seconds& seconds) const;
  date_time& operator=(value_type timestamp);

  /**
   * Formats the stored time according to the specified standard.
   *
   * @param standard Formatting standard
   * @param consider_zone Should we consider the time zone offset?
   * @return std::string Formatted date in string format
   */
  std::string format(date_format standard = date_format::ISO_8601,
                     bool consider_zone = true);

  /**
   * Returns an initialized class with the current UTC time
   * @return date_time
   */
  static date_time current();

  /**
   * Returns unix timestamp relative to 01-01-1970 00:00:00
   * @return std::uint64_t
   */
  value_type timestamp(bool consider_zone = true) const;

 private:
  static constexpr std::uint64_t UNIX_TIME_START{0x019DB1DED53E8000};
  static constexpr std::uint64_t TICKS_PER_SECOND{10000000};

  static auto file_time_to_timestamp(const FILETIME& file_time) -> value_type;
  static auto timestamp_to_file_time(value_type timestamp) -> FILETIME;

 private:
  value_type native_;  // offset relative to 01-01-1970 00:00:00
  time_zone zone_;
};
}  // namespace receiver
