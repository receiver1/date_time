# receiver::date_time
## What?
Class for working with date and time according to the Internet standard. The class is designed to work with Windows and works with its API. You can find them all in this path: `Computer\HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Windows NT\CurrentVersion\Time Zones`. The class does not work with millisecond zone offsets. The `date_time::current` method takes UTC time without a time zone, you can set the zone yourself using `date_time::set_timezone`.

## Why?
Calling `std::chrono::current_zone()` causes a crash on most builds of Windows due to the presence of an outdated `icu.dll` file on the system. Microsoft is not going to update it in `C++ Redistributable`, so this library appeared. This library depends on the Windows implementation. Does not use an Internet connection to obtain time zones, because it receives them directly from the system. And it is supported on all builds of Windows. Uses only 1 import - advapi32.dll

## How to use?
A very short example of all the functionality: parsing and formatting according to the ISO_8601 standard, as well as indicating the starting time zone
```cpp
using namespace receiver;
std::cout << date_time{"2023-09-07T00:00:00Z", date_format::ISO_8601,
                    time_zone{"Russia TZ 2 Standard Time"}}.format(date_format::ISO_8601) << std::endl;
// Output: 2023-09-07T00:00:00+03:00
```
You can add/sub seconds/minutes/hours/days via std::chrono:
```cpp
using namespace receiver;
using namespace std::chrono_literals;
std::cout << (date_time{"2023-09-07T00:00:00Z"} + 15min)
                 .format(date_format::ISO_8601)
          << std::endl;
// Output: 2023-09-07T00:15:00+00:00
```
Get the current time in the current time zone:
```cpp
using namespace receiver;
std::cout << date_time::current()
                 .set_time_zone(time_zone::current())
                 .format(date_format::ISO_8601)
          << std::endl;
// Output: 2023-09-12T12:01:17+04:00
```

All functions are documented in the code, you can figure it out yourself by looking at the .h file.

## Problem?
If you encounter a problem or have ideas for improving the code/functionality of the class, then I will be glad to see your issue!
