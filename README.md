# receiver::date_time
Class for working with date and time according to the Internet standard. The class is designed to work with Windows and works with its API. The class does not work with millisecond zone offsets. The `date_time::current` method takes UTC time without a time zone, you can set the zone yourself using `date_time::set_timezone`.

## Demonstration:
A very short example of all the functionality: parsing and formatting according to the ISO_8601 standard, as well as indicating the starting time zone
```cpp
std::cout << date_time{"2023-09-07T00:00:00Z", date_format::ISO_8601,
                    time_zone{"Russia TZ 2 Standard Time"}}.format(date_format::ISO_8601) << std::endl;
```
You can add/sub seconds/minutes/hours/days via std::chrono:
```cpp
using namespace std::chrono_literals;
std::cout << (date_time{"2023-09-07T00:00:00Z"} + 15min)
                 .format(date_format::ISO_8601)
          << std::endl;
```

All functions are documented in the code, you can figure it out yourself by looking at the .h file.
If you find a problem or want to see a new feature, you can create an issue.
