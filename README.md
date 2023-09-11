# receiver::date_time
Class for working with date and time according to the Internet standard.

## Demonstration:
A very short example of all the functionality: parsing and formatting according to the ISO_8601 standard, as well as indicating the starting time zone
```cpp
std::cout << date_time{"2023-09-07T00:00:00Z", date_format::ISO_8601,
                    time_zone{"Russia TZ 2 Standard Time"}}.format(date_format::ISO_8601) << std::endl;
```
You can add/sub seconds/minutes/hours/days via std::chrono:
```cpp
using namespace std::chrono_literals;
std::cout << date_time{"2023-09-07T00:00:00Z"} + 15min << std::endl;
```

All functions are documented in the code, you can figure it out yourself by looking at the .h file.
